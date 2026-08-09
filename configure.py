#! /usr/bin/env python3

# pyright: reportUnknownMemberType=false, reportMissingTypeStubs=false

from __future__ import annotations

import os
import sys
import json
import splat
import shutil
import argparse
import subprocess
import ninja_syntax

from splat.segtypes.linker_entry import LinkerEntry
from spimdisasm.common.CompilerConfig import compilerOptions

from typing import Any, Union, Protocol, Literal, cast
from pathlib import Path
from contextlib import contextmanager

from splat.scripts import split
from splat.util.conf import load as splat_load_yaml

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

    compiler_includes = f"-I{src_path.parent / 'src'} -I{src_path.parent / 'include'} -Iinclude/sdk/ee -I../src/cs4/Seimei/fslib"
    assembler_includes = f"-I{src_path.parent / 'include'} -I{src_path.parent}"
    common_includes = f"{compiler_includes} {assembler_includes}"
    game_compile_cmd = f"{WIBO} {game_cc_dir}/mwccps2.exe -c {common_includes} {COMPILER_FLAGS}"

    return game_compile_cmd, common_includes


def exec_shell(command: list[str]) -> str:
    ret = subprocess.run(command, capture_output=True, text=True, check=False)
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

        expected_strs = []
        for obj in object_paths:
            if obj.name.endswith(".c.o"):
                obj = obj.with_name(obj.name.removesuffix(".c.o") + ".o")
            elif obj.name.endswith(".s.o"):
                obj = obj.with_name(obj.name.removesuffix(".s.o") + ".o")
    
            if obj.parts[:2] == ("build", "asm"):
                p = obj.relative_to("build/asm")
            elif obj.parts[:2] == ("build", "src"):
                p = obj.relative_to("build/src")
            else:
                p = obj
            expected_strs.append(str(Path("build/expected") / p))

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

            ninja.build(
                outputs=expected_strs,
                rule="as",
                inputs=[str((Path("asm") / s.relative_to("../src") if s.parts[:2] == ("..", "src") else s).with_suffix(".s")) for s in src_paths],
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
        command=f"{CROSS}as -no-pad-sections -EL -march=5900 -mabi=eabi -mno-branch-relocs -Iinclude -I{src_path.parent / 'include'} -o $out $in",
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

        elif isinstance(
                    seg,
                    (
                        splat.segtypes.common.databin.CommonSegDatabin,
                        splat.segtypes.common.rodatabin.CommonSegRodatabin,
                        splat.segtypes.common.textbin.CommonSegTextbin,
                        splat.segtypes.common.sbss.CommonSegSbss,
                    ),
                ):
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

    tu_to_diff: list[tuple[Literal["asmtu", "c"], str]] = []

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

            if subs_type in ("asmtu", "c"):
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
        target_path = Path("build", "expected", tu_name).with_suffix(".o")

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
            "metadata": {
                "source_path": str(Path("..", "src", tu_name).with_suffix(".c")),
                "progress_categories": [language]
            },
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
        "custom_make": "ninja",
        "custom_args": [],
        "build_target": True,
        "build_base": True,
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
        sys.exit(1)

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
        supported_elfs = f"{ {f'{elf} ({lang})' for elf, lang in LANGUAGES.items()} }".replace("'", "")
        print(f"unsupported game ELF. Supported versions are: {supported_elfs}")
        sys.exit(1)

    language = LANGUAGES[basename]

    if args.reset:
        clean(config_dir, config)
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

    linker_entries = split.linker_writer.entries

    build_stuff(config_dir, split.config, linker_entries, language)

    write_permuter_settings(config_dir, src_path, language)

    generate_objdiff_configuration(config_dir, split.config, language)


if __name__ == "__main__":
    main()
