'''
from alessatool/annotate (memory-of-alessa, silent hill 2+3 decompilation project)

mark up assembly with line numbers.
can also be used to create full-tu scratches in decomp.me.
'''

from sys import stdout, stdin
from pathlib import Path
from subprocess import run
from dataclasses import dataclass
from argparse import ArgumentParser

ADDR2LINE_PATH = f"mips-ps2-decompals-addr2line"

FUNCTION_SYMBOL_LABEL = "glabel"
END_FUNCTION_SYMBOL_LABEL = "endlabel"
SECTION_DIRECTIVE = ".section "
TEXT_SECTION_DIRECTIVE = ".section .text"
UNIQUE_TEXT_SECTION_DIRECTIVE = '.section .text,"ax",@progbits,unique,'
INCLUDE_MACRO_INC_DIRECTIVE = '.include "macro.inc"'

@dataclass
class AnnotationArgs:
    vram_start: int | None
    vram_end: int | None
    elf_path: Path
    asm_path: Path | None
    out_path: Path
    addr2line_path: Path
    tu: bool
    encoding: str
    stdout: bool

def annotate_asm(args: AnnotationArgs):
    if args.asm_path:
        with open(args.asm_path, "r") as asm_file:
            asm_contents = asm_file.read()
    else:
        asm_contents = stdin.read()

    asm_lines = asm_contents.splitlines()
    asm_line_index = 0

    vram_start = args.vram_start
    vram_end = args.vram_end

    if vram_start is None or vram_end is None:
        vram_start, vram_end = find_vram_bounds(asm_lines)

    addresses = (f"0x{v:X}" for v in range(vram_start, vram_end, 0x4))
    proc = run([args.addr2line_path, "-e", args.elf_path, *addresses], capture_output=True, encoding=args.encoding)
    addr2line_output_lines = proc.stdout.splitlines()

    main_tu_name = None
    prev_tu_name = None
    prev_line_number = -1
    function_count = 0
    is_in_function_label = False
    is_in_text_section = False
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

            if asm_line.startswith(SECTION_DIRECTIVE):
                is_in_text_section = asm_line.startswith(TEXT_SECTION_DIRECTIVE)

            if not is_in_text_section:
                annotated_asm_lines.append(asm_line)
                asm_line_index += 1
                continue

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
        print(f"alessatool/annotate: wrote asm to {args.out_path}")
    else:
        stdout.write(annotated_asm_contents)

def to_addr2line_format(compile_unit: str, line_number: int):
    return f"{compile_unit}:{line_number}"

def find_vram_bounds(asm_lines: list[str]):
    start = None
    passed_first_label = False
    end = None

    for line in asm_lines:
        # assumes first address comment is on line after first label
        if "label" in line and not passed_first_label:
            passed_first_label = True
            continue

        if passed_first_label and start is None:
            start = get_vram_addr_from_line(line)

        # assumes .text section comes first
        if line.startswith(SECTION_DIRECTIVE) and not line.startswith(TEXT_SECTION_DIRECTIVE):
            break

        vram_addr = get_vram_addr_from_line(line)
        if vram_addr is None:
            continue

        end = vram_addr

    return (start, end)

def get_vram_addr_from_line(line: str):
    if "/*" not in line:
        return None

    _before_comment, after_comment = line.split("/* ", 1)
    _file_addr, vram_addr, _rest = after_comment.split(" ", 2)

    try: 
        return int(vram_addr, 0x10)
    except:
        return None

def line_has_vram_addr(line: str, addr_str: str) -> bool:
    if addr_str not in line or "*/" not in line:
        return False

    return line.index("*/") > line.index(addr_str)

def append_final_new_line(lines: list[str]):
    if lines[-1] != "":
        lines.append("")

def main():
    annotate_parser = ArgumentParser(  
        description="alessatool/annotate: decorate asm with line numbers (memory of alessa cli utility)"
    )

    annotate_parser.add_argument(
        "--vram-start",
        type=lambda x: x is None and None or int(x, 0),
        help="start of vram range",
        default=None
    )
    annotate_parser.add_argument(
        "--vram-end",
        type=lambda x: int(x, 0),
        help="end of vram range",
        default=None
    )
    annotate_parser.add_argument(
        "--elf-path",
        type=Path,
        default=Path("config/SLUS_210.07"),
        help="path to the elf file"
    )
    annotate_parser.add_argument(
        "--asm-path",
        type=Path,
        default=None,
        help="path to the asm file to annotate"
    )
    annotate_parser.add_argument(
        "--out-path",
        type=Path,
        default=Path("config/asm/decorated.s"),
        help="path to write the annotated output"
    )
    annotate_parser.add_argument(
        "--stdout",
        action="store_true",
        help="ignore output path, write to stdout"
    )
    annotate_parser.add_argument(
        "--addr2line-path",
        type=Path,
        default=ADDR2LINE_PATH,
        help="path to addr2line"
    )
    annotate_parser.add_argument(
        "--line-file-path",
        type=Path,
        default=None,
        help="path to file containing line numbers, packed into unsigned shorts"
    )
    annotate_parser.add_argument(
        "--encoding",
        type=str,
        default="shift-jis"
    )
    annotate_parser.add_argument(
        "--tu",
        action="store_true",
        help="arrange each function into its own text section"
    )
    annotate_parser.set_defaults(func=annotate_asm)

    args = annotate_parser.parse_args()
    if args.asm_path is None:
        annotate_parser.print_help()
    else:
        annotate_asm(args)

if __name__ == "__main__":
    main()
