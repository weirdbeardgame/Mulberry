#! /usr/bin/env python3

# pyright: reportUnknownMemberType=false, reportMissingTypeStubs=false

from __future__ import annotations

import os
import re
import sys
import json
import yaml
import copy
import splat
import shutil
import argparse
import tempfile
import subprocess
import ninja_syntax

from dataclasses import dataclass, asdict
#from utils import ensure_path_and_write, normalize_object_path, to_expected_path

import splat.util.options as splat_options
from splat.segtypes.linker_entry import LinkerEntry, clean_up_path
from spimdisasm.common.CompilerConfig import compilerOptions

from typing import Any, Union, Protocol, Literal, cast
from pathlib import Path
from contextlib import contextmanager

from splat.scripts import split
from splat.util.conf import load as splat_load_yaml
from splat.segtypes.linker_entry import LinkerEntry

# from tools.python.fix_gp import fix_gp
from tools.python.fix_assets import fix_assets
from tools.python.fix_linkerscript import fix_linkerscript

from sys import stdout
from subprocess import run
from dataclasses import dataclass
from struct import unpack

@contextmanager
def suppress_stdout_stderr():
    null_fds = [os.open(os.devnull, os.O_RDWR) for _ in range(2)]
    save_fds = [os.dup(1), os.dup(2)]
    os.dup2(null_fds[0], 1)
    os.dup2(null_fds[1], 2)
    try:
        yield
    finally:
        os.dup2(save_fds[0], 1)
        os.dup2(save_fds[1], 2)
        for fd in null_fds + save_fds:
            os.close(fd)


ROOT = Path(__file__).parent.resolve()
TOOLS_DIR = ROOT / "tools"

CROSS = "mips-ps2-decompals-"

COMPILER = "mwcps2-3.0b52-030722"
COMPILER_FLAGS = "-O3,p -sym on -str readonly -cwd source"

PLATFORM = "x86_64" # @todo get this from makefile
# PLATFORM = "macos"
WIBO = f"{TOOLS_DIR}/wibo-{PLATFORM}"
LCF = "SLUS_210.07.lcf"

LANGUAGES: dict[str, str] = {
    "SLUS_210.07": "",
}

FUNCTION_SYMBOL_LABEL = "glabel"
END_FUNCTION_SYMBOL_LABEL = "endlabel"
UNIQUE_TEXT_SECTION_DIRECTIVE = '.section .text,"ax",@progbits,unique,'
INCLUDE_MACRO_INC_DIRECTIVE = '.include "macro.inc"'


def get_compiler_command(command: str):
    compiler_dir = Path("tools") / "cc" / COMPILER
    ee_dir = compiler_dir / "lib" / "gcc-lib" / "ee"
    ee_compiler_dirname = next(os.walk(ee_dir))[1][0]

    commands = {
        #"ee-gcc": compiler_dir / "bin" / "ee-gcc",
        #"cc1": compiler_dir / "lib" / "gcc-lib" / "ee" / ee_compiler_dirname / "cc1",
        #"cc1plus": compiler_dir / "lib" / "gcc-lib" / "ee" / ee_compiler_dirname / "cc1plus",
        #"cpp": compiler_dir / "lib" / "gcc-lib" / "ee" / ee_compiler_dirname / "cpp",
        #"as": compiler_dir / "ee" / "bin" / "as",
    }

    #return commands[command]


def make_compiler_cmd(config_dir: Path, src_path: Path, language: str):
    rel_root = Path(os.path.relpath(ROOT, config_dir))
    tools_dir = rel_root / "tools"

    game_cc_dir = f"{tools_dir}/cc/{COMPILER}"

    compiler_includes = f"-I{src_path.parent / 'src'} -I{src_path.parent / 'include'} -Iinclude/sdk/ee"
    assembler_includes = f"-I{src_path.parent / 'include'} -I{src_path.parent}"
    common_includes = f"{compiler_includes} {assembler_includes}"
    game_compile_cmd = f"{WIBO} {game_cc_dir}/mwccps2.exe -c {common_includes} {COMPILER_FLAGS}"

    return game_compile_cmd, common_includes


def exec_shell(command: list[str]) -> str:
    ret = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    return ret.stdout


def clean(config_dir: Path, config: dict[str, Any]):
    """
    Removes all temporary files and folders.
    """
    basename = Path(config["options"]["basename"])

    asm_path = Path(config["options"]["asm_path"])
    asset_path = Path(config["options"]["asset_path"])
    build_path = Path(config["options"]["build_path"])
    cc_str_path = Path("cc-src")

    relative_asm_path = (config_dir / asm_path).resolve().relative_to(ROOT)
    relative_asset_path = (config_dir / asset_path).resolve().relative_to(ROOT)
    relative_build_path = (config_dir / build_path).resolve().relative_to(ROOT)
    relative_cc_str_path = (config_dir / cc_str_path).resolve().relative_to(ROOT)

    for file in (
        ".splache",
        "build.ninja",
        ".ninja_log",
        "permuter_settings.toml",
        "undefined_syms_auto.txt",
        f"{basename}.ld",
    ):
        (config_dir / file).unlink(missing_ok=True)

    shutil.rmtree(relative_asm_path, ignore_errors=True)
    shutil.rmtree(relative_asset_path, ignore_errors=True)
    shutil.rmtree(relative_build_path, ignore_errors=True)
    shutil.rmtree(relative_cc_str_path, ignore_errors=True)


def write_permuter_settings(config_dir: Path, src_path: Path, language: str):
    game_compile_cmd, _ = make_compiler_cmd(config_dir, src_path, language)
    with open(config_dir / "permuter_settings.toml", "w") as f:
        f.write(
            f"""compiler_command = "{game_compile_cmd} -D__GNUC__"
assembler_command = "{CROSS}as -march=r5900 -mabi=eabi -Iinclude"
compiler_type = "mwcc"

[preserve_macros]

[decompme.compilers]
"tools/build/cc/mwcc/mwcc" = "{COMPILER}"
"""
        )


def build_stuff(
    config_dir: Path,
    config: dict[str, Any],
    linker_entries: list[LinkerEntry],
    language: str,
):
    basename = Path(config["options"]["basename"]).name
    ld_path = f"{basename}.ld"
    elf_path = f"build/{basename}"
    map_path = f"build/{basename}.map"
    pre_elf_path = f"build/{basename}.elf"

    src_path = Path(config["options"]["src_path"])
    game_compile_cmd, common_includes = make_compiler_cmd(config_dir, src_path, language)

    built_objects: set[Path] = set()

    def build(
        object_paths: Union[Path, list[Path]],
        src_paths: list[Path],
        task: str,
        variables: dict[str, str] = {},
        implicit_outputs: list[str] = [],
    ):
        if not isinstance(object_paths, list):
            object_paths = [object_paths]

        # FIX: escape any $ sign with $$ as ninja_syntax
        #      doesn't seem to do any escaping
        object_paths = [Path(str(entry).replace("$", "$$")) for entry in object_paths]
        src_paths = [Path(str(entry).replace("$", "$$")) for entry in src_paths]

        object_strs = [str(obj) for obj in object_paths]

        for object_path in object_paths:
            if object_path.name == "elf_header.s.o":
                continue

            if object_path.suffix == ".o":
                built_objects.add(object_path)

            ninja.build(
                outputs=object_strs,
                rule=task,
                inputs=[str(s) for s in src_paths],
                variables=variables,
                implicit_outputs=implicit_outputs,
            )

    ninja = ninja_syntax.Writer(open(str(ROOT / config_dir / "build.ninja"), "w"), width=9999)

    # Rules
    ld_args = "-map -sym on,elf -noinhibit-exec -main _start -o $out $lcf $objects"

    rel_root = Path(os.path.relpath(ROOT, config_dir))

    ld = Path("..", (Path("tools") / "cc" / COMPILER / "mwldps2.exe"))
    cpp = Path("tools") / "cc" / COMPILER / "mwccps2.exe"

    print({cpp})

    ninja.rule(
        "as",
        description="as $in",
        command=f"{CROSS}as -no-pad-sections -EL -march=5900 -mabi=eabi -I{src_path.parent / 'include'} -o $out",
    )

    ninja.rule(
        "cc-s",
        description="compile c source to object through assembly",
        command=(
            f"s_in=$$(echo $in.S | sed 's,^[^/]*/[^/]*/,cc-src/,') && "  # .............. 1) remove ../../ from path + prefix with cc-src/ + suffix with .S and store it into s_in var: ../src/file.c -> s_in=cc-src/src/file.c.S
            f'mkdir -p $$(dirname "$$s_in") && '  # ..................................... 2) create directory from s_in var: s_in=cc-src/src/path/to/file.c.S -> mkdir -p cc-src/src/path/to/
            f"{game_compile_cmd} $$s_in -o $out && "  # ................................. 5) compile assembly file into object
        ),
    )

    ninja.rule(
        "cc",
        description="cc $in",
        command=f"{game_compile_cmd} $in -o $out",
    )

    #ninja.rule(
     #   "libcc",
      #  description="cc $in",
      #  command=f"{lib_compile_cmd} $in -o $out && {ld}strip $out -N dummy-symbol-name",
   # )

    ninja.rule(
        "ld",
        description="link $out",
        command=f"{WIBO} {ld} {ld_args}",
    )

    ninja.rule(
        "sha1sum",
        description="sha1sum $in",
        command="sha1sum -c $in && touch $out",
    )

    ninja.rule(
        "elf",
        description="elf $out",
        command=f"{CROSS}objcopy $in $out -O binary",
    )

    for entry in linker_entries:
        seg = entry.segment

        if seg.type[0] == ".":
            continue

        if entry.object_path is None:
            continue

        if isinstance(
            seg,
            (
                splat.segtypes.common.asm.CommonSegAsm,
                splat.segtypes.common.data.CommonSegData,
            ),
        ):
            build(entry.object_path, entry.src_paths, "as")

        elif isinstance(seg, splat.segtypes.common.c.CommonSegC):
            entry.src_paths = [Path("..") / Path(f"{src_path}") for src_path in entry.src_paths]
            if any(str(src_path).startswith("src/lib/") for src_path in entry.src_paths):
                build(entry.object_path, entry.src_paths, "libcc")

            else:
                build(entry.object_path, entry.src_paths, "cc")

        elif isinstance(seg, splat.segtypes.common.databin.CommonSegDatabin):
            build(entry.object_path, entry.src_paths, "as")

        elif isinstance(seg, splat.segtypes.common.rodatabin.CommonSegRodatabin):
            build(entry.object_path, entry.src_paths, "as")

        elif isinstance(seg, splat.segtypes.common.textbin.CommonSegTextbin):
            build(entry.object_path, entry.src_paths, "as")

        elif isinstance(seg, splat.segtypes.common.sbss.CommonSegSbss):
            build(entry.object_path, entry.src_paths, "as")

        else:
            print(f"ERROR: Unsupported build segment type {seg.type}")
            sys.exit(1)

    built_object_paths = [str(obj) for obj in built_objects]

    ninja.build(
        pre_elf_path,
        "ld",
        ld_path.replace(".ld",".lcf"),
        implicit=built_object_paths,
        variables={
            "mapfile": map_path,
            "objects": [str(obj) for obj in built_objects],
            "lcf": LCF
        },
    )

    ninja.build(
        elf_path,
        "elf",
        pre_elf_path,
    )

    ninja.build(
        elf_path + ".ok",
        "sha1sum",
        "checksum.sha1",
        implicit=[elf_path],
    )


def get_line_file_path(args: AnnotationArgs):
    if args.line_file_path is not None:
        return args.line_file_path

 #   if args.elf_path.name == SH2_SERIAL and "Event/stage" in args.asm_path.as_posix():
#        return Path(f"{TOOLS}/alessatool/dwarf") / Path(args.asm_path.name).with_suffix(".line")

    return None


def line_has_vram_addr(line: str, addr_str: str) -> bool:
    if addr_str not in line or "*/" not in line:
        return False

    return line.index("*/") > line.index(addr_str)


def annotate_asm(args: AnnotationArgs):
    with open(args.asm_path, "r") as asm_file:
        asm_contents = asm_file.read()

    asm_lines = asm_contents.splitlines()
    asm_line_index = 0

    vram_start = args.vram_start
    vram_end = args.vram_end

    if vram_start is None or vram_end is None:
        vram_start, vram_end = find_vram_bounds(asm_lines)

    line_file_path = get_line_file_path(args)

    if line_file_path is None or not line_file_path.exists():
        addresses = (f"0x{v:X}" for v in range(vram_start, vram_end, 0x4))
        proc = run([args.addr2line_path, "-e", args.elf_path, *addresses], capture_output=True, encoding=args.encoding)
        addr2line_output_lines = proc.stdout.splitlines()
    else:
        # parse the binary line number file.

        # the format is a list of u_shorts, one per line number.
        # there should be one line number per vram address, and each vram
        # address should be exactly 4 bytes apart, mirroring how the addr2line
        # output is formatted

        with open(line_file_path, "rb") as line_file:
            line_data = line_file.read()
            line_numbers = unpack(f"<{len(line_data) // 2}H", line_data)
            compile_unit = args.asm_path.with_suffix(".c").name
            addr2line_output_lines = list(map(lambda n : to_addr2line_format(compile_unit, n), line_numbers))

    main_tu_name = None
    prev_tu_name = None
    prev_line_number = -1
    function_count = 0
    is_in_function_label = False
    current_vram_addr = vram_start
    annotated_asm_lines = []

    for addr_index in range(0, len(addr2line_output_lines) - 1):
        line = addr2line_output_lines[addr_index]

        if line.startswith("?"):
            current_vram_addr += 0x4
            continue

        separator_index = line.rfind(":")
        current_line_number = int(line[separator_index+1:])

        if current_line_number == prev_line_number:
            current_vram_addr += 0x4
            continue

        vram_addr_str = f"{current_vram_addr:X}"

        current_tu_name = line[0:separator_index]
        if current_tu_name != main_tu_name and current_tu_name != prev_tu_name:
            if main_tu_name:
                print(f"[warn] alessatool/annotate: address at {vram_addr_str} belongs to {current_tu_name}")
            else:
                main_tu_name = current_tu_name
        prev_tu_name = current_tu_name

        while True:
            if asm_line_index >= len(asm_lines):
                raise AssertionError(
                    f"address 0x{current_vram_addr:X} not found in asm"
                )

            asm_line = asm_lines[asm_line_index]

            if line_has_vram_addr(asm_line, vram_addr_str):
                break

            should_append_asm_line = True
            if args.tu:
                asm_line_trimmed = asm_line.strip()

                # track when we go in & out of function symbols
                if asm_line_trimmed.startswith(FUNCTION_SYMBOL_LABEL):
                    function_count += 1
                    is_in_function_label = True
                    annotated_asm_lines.append(f"{UNIQUE_TEXT_SECTION_DIRECTIVE}{function_count}")
                    annotated_asm_lines.append("")
                elif asm_line_trimmed.startswith(END_FUNCTION_SYMBOL_LABEL):
                    is_in_function_label = False

                # remove `nop`s
                if not is_in_function_label and asm_line_trimmed.endswith("nop"):
                    should_append_asm_line = False
                
                # remove `macro.inc` include directive
                elif asm_line_trimmed == INCLUDE_MACRO_INC_DIRECTIVE:
                    should_append_asm_line = False

            if should_append_asm_line:
                annotated_asm_lines.append(asm_line)

            asm_line_index += 1

        annotated_asm_lines.append(f"\t.loc 1 {current_line_number}")    
        annotated_asm_lines.append(asm_line)
        asm_line_index += 1

        prev_line_number = current_line_number
        current_vram_addr += 0x4
    
    while asm_line_index < len(asm_lines) - 1:
        annotated_asm_lines.append(asm_lines[asm_line_index])
        asm_line_index += 1

    assert main_tu_name, "no valid compilation unit found"
    annotated_asm_lines = [
        ".section .debug",
        ".previous",
        ".text",
        f".file 1 \"{main_tu_name}\"",
        *annotated_asm_lines
    ]

    append_final_new_line(annotated_asm_lines)
    annotated_asm_contents = "\n".join(annotated_asm_lines)

    if not args.stdout and args.out_path:
        with open(args.out_path, "w") as out_file:
            out_file.write(annotated_asm_contents)
        if args.verbose:
            print(f"alessatool/annotate: wrote asm to {args.out_path}")
    else:
        stdout.write(annotated_asm_contents)


def append_final_new_line(lines: list[str]):
    if lines[-1] != "":
        lines.append("")


def make_asm(config_path: Path, config: dict[str, Any]):
    """
    Extracts assembly for each function into 'expected/asm/' subfolder.

    The extraction is done following these steps:
        1. create temporary directory in project folder
        2. copy yaml to temporary directory
        3. modify yaml to have c subsegments instead of asm subsegments
        4. remove data, bss, etc., subsegments
        5. run splat on the new config yaml
        6. splat extracts assembly for each function into '<temp_dir>/asm/nonmatchings/'
        7. copy '<temp_dir>/asm/nonmatchings/' to '<project_dir>/expected/asm/'
    """
    with tempfile.TemporaryDirectory(dir=config_path, prefix="tmp_") as tmp_dir:
        tmp_path = Path(tmp_dir)

        yaml_path = tmp_path / "config.yaml"
        asm_path = tmp_path / "asm" / "nonmatchings"
        dst_path = tmp_path / ".." / "expected" / "asm"
        dst_path = dst_path.resolve().relative_to(ROOT)

        if dst_path.exists():
            print(f"expected asm dir '{dst_path}' already exists")
            return

        config = copy.deepcopy(config)
        config["options"]["target_path"] = "../" + config["options"]["target_path"]
        config["options"]["asm_path"] = "asm"
        config["options"]["src_path"] = "src"
        config["options"]["build_path"] = "build"
        config["options"]["asset_path"] = "assets"
        config["options"]["undefined_funcs_auto_path"] = "../" + config["options"]["undefined_funcs_auto_path"]
        config["options"]["undefined_syms_auto_path"] = "../" + config["options"]["undefined_syms_auto_path"]
        config["options"]["symbol_addrs_path"] = "../" + config["options"]["symbol_addrs_path"]
        config["options"]["extensions_path"] = "../" + config["options"]["extensions_path"]

        new_segments: list[Any] = []
        segments: list[Any] = config["segments"]
        for segment in segments:
            if isinstance(segment, list):
                new_segments.append(segment)
            elif isinstance(segment, dict) and segment["name"] == "main":
                new_subsegments: list[Any] = []
                subsegments = cast(list[Any], segment["subsegments"])
                for subsegment in subsegments:
                    if isinstance(subsegment, list):
                        if subsegment[1] == "asm":
                            subsegment[1] = "c"
                        new_subsegments.append(subsegment)
                    elif isinstance(subsegment, dict):
                        subsegment["type"] = subsegment["type"].strip(".")
                        if subsegment["type"] == "rodata":
                            # splat now requires rodata to be always prefixed with a dot (.)
                            # for the rodata migration to work properly
                            subsegment["type"] = ".rodata"
                        new_subsegments.append(subsegment)
                segment["subsegments"] = new_subsegments
                new_segments.append(segment)
        config["segments"] = new_segments

        def rename_locals(base_path: Path):
            for asm_file in base_path.rglob("*.s"):
                data = asm_file.read_text()
                data = re.sub(r"__local_\d+", "", data)
                asm_file.write_text(data)

        with yaml_path.open(mode="w") as yaml_file:
            yaml.dump(config, yaml_file, default_flow_style=False)

        with suppress_stdout_stderr():
            split.main([yaml_path], modes=["all"], verbose=False)

        # remove '__local_#' from asm
        rename_locals(asm_path)

        dst_path.parent.mkdir(parents=True, exist_ok=True)
        shutil.copytree(asm_path, dst_path, dirs_exist_ok=True)

        print(f"expected asm extracted to '{dst_path}'")

        # make expected objs

        for subseg in new_segments[1]["subsegments"]:
            if isinstance(subseg, list) and subseg[1] == "c":
                subseg[1] = "asm"
                subseg[2] += ".c"

        config["options"]["asm_jtbl_label_macro"] = "llabel"

        with yaml_path.open(mode="w") as yaml_file:
            yaml.dump(config, yaml_file, default_flow_style=False)

        shutil.rmtree(tmp_path / "asm")
        (tmp_path / ".splache").unlink()

        with suppress_stdout_stderr():
            split.main([yaml_path], modes=["all"], verbose=False)

        # remove '__local_#' from asm
        rename_locals(asm_path)

        dst_path = dst_path.parent / "obj"
        tmp_obj_path = tmp_path / "obj"
        tmp_asm_dir = tmp_path / "asm"

        cpp = Path("..", "..", "..", (Path("tools") / "cc" / COMPILER))

        for asm_file in tmp_asm_dir.rglob("*.c.s"):
            asm_file_rel = asm_file.relative_to(tmp_path)
            obj_file_rel = Path("obj") / asm_file.relative_to(tmp_asm_dir).with_suffix(".o")
            obj_file = tmp_obj_path / obj_file_rel.relative_to("obj")
            obj_file.parent.mkdir(parents=True, exist_ok=True)
            subprocess.run(
                f"{cpp} -I../src -I../include -Iinclude -Iinclude/sdk/ee -I../.. '{asm_file_rel}' -o - | "
                #f"iconv -f=UTF-8 -t=EUC-JP '{asm_file_rel}' | "
                f"mips-linux-gnu-as -no-pad-sections -EL -march=5900 -mabi=eabi -I../include -o {obj_file_rel} {asm_file_rel}",
                shell=True,
                cwd=tmp_path,
            )

        shutil.copytree(tmp_obj_path, dst_path, dirs_exist_ok=True)

        print(f"expected obj built to '{dst_path}'")


def generate_objdiff_configuration(config_path: Path, config: dict[str, Any], language: str):
    """
    Generate `objdiff.json` configuration from splat YAML config.

    Parse splat YAML config to get a list of the TUs that need to
    be diffed and create appropriate `units` for objdiff to process.

    Target objects need to be extracted separately (see the
    `make <lang>-make-asm` command) in order for objdiff to find the
    target files.
    """
    segments: list[Any] = config["segments"]

    tu_to_diff: list[tuple[Literal["asm", "c"], str]] = []

    for segment in segments:
        if not (isinstance(segment, dict) and segment["name"] == "main"):
            # we are looking for the main segment
            continue

        subsegments = cast(list[Any], segment["subsegments"])

        for subsegment in subsegments:
            if isinstance(subsegment, list):
                _, subs_type, subs_name = cast(tuple[int, str, str], subsegment)

            elif isinstance(subsegment, dict):
                subs_type = cast(int, subsegment["type"])
                subs_name = cast(str, subsegment["name"])

            else:
                raise RuntimeError("invalid subsegment type")

            if subs_type in ("asm", "c"):
                if subs_name in (
                    "crt0",
                    "main/glob",
                    "graphics/motion/mdldat",
                    "graphics/motion/mim_dat",
                    "graphics/motion/acs_dat",
                    "graphics/scene/scene_dat",
                    "ingame/ig_glob",
                    "mc/mc",
                    "mc/mc_icon",
                    "ingame/entry/fgst_dat",
                    "ingame/entry/rgst_dat",
                    "outgame/btl_mode/btl_dat",
                    "ingame/map/find_dat",
                    "ingame/map/furn_spe/fsla_dat",
                ):
                    # skip it as it's either not part of the game files or a data only TU
                    continue

                tu_to_diff.append((subs_type, subs_name))

    units: list[dict[str, Any]] = []

    for tu_type, tu_name in tu_to_diff:
        target_path = Path("expected", "obj", tu_name).with_suffix(".c.o")

        # since we only compile fully decompiled TUs, the
        # "c" type implies that the TU is complete
        is_complete = tu_type == "c"

        if is_complete:
            # compose the build path as "build/src/path/of/tu.c.o"
            base_path = Path("build", "src", tu_name).with_suffix(".c.o")
        else:
            # leave unset if the TU is not yet decompiled
            base_path = None

        unit: dict[str, Any] = {
            "name": tu_name,
            "target_path": str(target_path),
            "base_path": str(base_path) if base_path else None,
            "metadata": {"progress_categories": [language]},
        }

        if not base_path:
            # remove base_path for not yet decompiled TUs
            # (adding first and then removing if base_path is none allows
            # for key ordering to be preserved, otherwise adding it later
            # would put it after metadata)
            del unit["base_path"]

        units.append(unit)

    category_name = {
        "": "Kuon",
    }

    progress_categories = [
        {"id": language, "name": category_name[language]},
    ]

    objdiff_json: dict[str, Any] = {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "custom_make": "true",
        "custom_args": [],
        "build_target": False,
        "build_base": False,
        "watch_patterns": [],
        "units": units,
        "progress_categories": progress_categories,
    }

    objdiff_path = config_path / "objdiff.json"

    with objdiff_path.open(mode="w") as fw:
        json.dump(objdiff_json, fw, indent=2)

    print(f"Wrote objdiff configuration ({len(units)} units) to {objdiff_path}")


SECTION_ALIGNMENT_PAIRS: list[tuple[str, int]] = [
    (".text", 0x10),
    (".data", 0x8),
    (".rodata", 0x8),
    (".lit4", 0x4),
    (".sdata", 0x4),
    (".sbss", 0x4),
    (".bss", 0x10),
]

TRACKED_SEGMENT_TYPES = ["asm", "c", "data", "rodata"]

def generate_lcf():
    '''
    Generate a linker command file. It uses an oversimplified method of looping
    over the linker entries emitted by Splat and looking for splits that are
    parented to a `code` segment. Then it writes out the list of object files
    for each section.

    Expects a template file with the extension `.inc.lcf`.
    '''

    entries_by_section_type: dict[str, list[LinkerEntry]] = dict()
    for section_type, _ in SECTION_ALIGNMENT_PAIRS:
        entries_by_section_type[section_type] = []

    for entry in split.linker_writer.entries:
        segment = entry.segment
        parent = segment.parent

        if not parent or parent.type != "code":
            continue

        section_type = entry.section_link_type
        if section_type not in entries_by_section_type:
            continue

        entries_by_section_type[section_type].append(entry)

    lcf_blocks: list[str] = []

    for section_type, alignment in SECTION_ALIGNMENT_PAIRS:
        objects = entries_by_section_type[section_type]
        if not objects:
            continue

        block = [
            f"\t\t# {section_type}",
            f"\t\t__{section_type[1:]}_start = .;",
            f"\t\tALIGNALL(0x{alignment:X});",
        ]
            
        if section_type == ".sdata":
            block = [
                "\t\t_gp\t= ALIGN(128) + 0x7FF0;",
                "",
            ] + block

        if section_type == ".sbss":
            block = [
                "\t\t_fbss\t= .;",
                "",
            ] + block
        
        for entry in objects:
            alignment = entry.segment.ld_align_segment_start 
            if alignment is not None:
                block.append(f"\t\tALIGNALL(0x{alignment:X});")

            object_name = entry.object_path.name
            block.append(f"\t\t{object_name} ({section_type})")

        lcf_blocks.append("\n".join(block))

    generated_lcf = "\n\n".join(lcf_blocks)

    template_path = Path("config") / LCF.replace(".lcf", ".inc.lcf")
    output_path = Path("config") / LCF

    template = template_path.read_text()
    output = template.replace(
        "### 🔴 code 🔴 ###",
        generated_lcf.strip(),
    )
    output_path.write_text(output)

    print(f"✅ Mulberry/generate: wrote LCF to {output_path}")


def main():
    class ArgsProtocol(Protocol):
        YAML_FILE: Path
        clean: bool
        reset: bool
        make_asm: bool

    parser = argparse.ArgumentParser(description="Configure the project")
    parser.add_argument(
        "YAML_FILE",
        type=Path,
        help="yaml file to configure the project",
    )
    parser.add_argument(
        "-c",
        "--clean",
        help="Clean extraction and build artifacts",
        action="store_true",
    )
    parser.add_argument(
        "--make-asm",
        help="Extract assembly for each function into 'expected/asm/' subfolder",
        action="store_true",
    )
    parser.add_argument(
        "-r",
        "--reset",
        help="Reset config dir to original state",
        action="store_true",
    )
    args = cast(ArgsProtocol, parser.parse_args())

    cwd = Path(os.getcwd()).resolve()
    if not ROOT.samefile(cwd):
        print("ERROR: this script must be run from it's directory!")
        exit(1)

    compilerOptions["MWCCPS2"].value.bigAddendWorkaroundForMigratedFunctions = False

    config = splat_load_yaml(
        [args.YAML_FILE],
        modes=["all"],
        verbose=False,
        disassemble_all=False,
    )

    basename = config["options"]["basename"]
    config_dir = Path(args.YAML_FILE).parent

    compiler_dir = Path("tools") / "cc" / COMPILER


    if basename not in LANGUAGES:
        supported_elfs = f"{set(f'{elf} ({lang})' for elf, lang in LANGUAGES.items())}".replace("'", "")
        print(f"unsupported game ELF. Supported versions are: {supported_elfs}")
        exit(1)

    language = LANGUAGES[basename]

    if args.reset:
        clean(config_dir, config)
        return

    if args.make_asm:
        make_asm(config_dir, config)
        return

    if args.clean:
        clean(config_dir, config)

    split.main([args.YAML_FILE], modes=["all"], verbose=False)

    src_path = Path(config["options"]["src_path"])
    asm_path = Path(config["options"]["asm_path"])
    asset_path = Path(config["options"]["asset_path"])

    asm_data_path = (config_dir / asm_path).resolve().relative_to(ROOT) / "data"
    asset_rel_path = (config_dir / asset_path).resolve().relative_to(ROOT)
    assert asm_data_path.is_dir(), f"{asm_data_path} not found or not a directory"
    assert asset_rel_path.is_dir(), f"{asset_rel_path} not found or not a directory"

    generate_lcf()


    # fix asset .incbin path from relative to root to relative to config dir:
    #  e.g., change:
    #     .incbin "config/assets/name.section.bin"
    #  to:
    #     .incbin "assets/name.section.bin"
    fix_assets(asm_data_path, asset_rel_path)

    linker_entries = split.linker_writer.entries

    build_stuff(config_dir, split.config, linker_entries, language)

    write_permuter_settings(config_dir, src_path, language)

    # # replace gp_rel assembler macro with explicit offset as the gcc used
    # # to compile the code does not support it
    # gp_value = split.config["options"]["gp_value"]
    # symbol_addrs_path = Path(split.config["options"]["symbol_addrs_path"])
    # asm_rel_path = (config_dir / asm_path).resolve().relative_to(ROOT)
    # symbol_addrs_rel_path = (config_dir / symbol_addrs_path).resolve().relative_to(ROOT)
    # assert asm_rel_path.is_dir(), f"{asm_rel_path} not found or not a directory"
    # assert symbol_addrs_rel_path.is_file(), f"{symbol_addrs_rel_path} not found"
    # fix_gp(asm_rel_path, gp_value, symbol_addrs_rel_path)

    # fix linkerscript by applying explicit alignments as
    # specified in the config yaml
    linkerscript_path = (config_dir / f"{basename}.ld").resolve().relative_to(ROOT)
    assert linkerscript_path.is_file(), f"{linkerscript_path} not found"
    fix_linkerscript(split.config, linkerscript_path)

    generate_objdiff_configuration(config_dir, split.config, language)


if __name__ == "__main__":
    main()
