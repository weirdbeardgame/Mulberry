# pyright: reportInvalidTypeForm=false
from __future__ import annotations

import os
import re
import io
import enum
import math
import tqdm
import argparse
import pydantic
import subprocess

from ctypes import c_uint16, c_int16, c_uint64, c_uint32, c_int32, c_uint8, c_float, sizeof as c_sizeof
from typing import Protocol, BinaryIO, Type, Iterable, cast
from pathlib import Path

from cstruct import CStructure, ctypes_types, CTypeType, c_addr_ptr, c_str, print_carr


def vram2offset(vram: int):
    return vram - 0xFF000


sceVu0FVECTOR = c_float * 4
sceVu0IVECTOR = c_int32 * 4
sceVu0FMATRIX = sceVu0FVECTOR * 4

qword = c_int32 * 4


class ANI_CODE(c_uint16):
    def __str__(self):
        return f"{self.value}"


class GS_REG(c_uint64):
    def __str__(self):
        return f"0x{self.value:x}"


class HEX_ADDR(c_uint32):
    def __str__(self):
        return f"0x{self.value:x}"


# CTypeTypeEX = CTypeType | type[sceVu0FVECTOR] | type[sceVu0IVECTOR]


###########################################################################

# struct _MNG_STATE_ST {
#     // Members
#     int state; // offset 0x0, size 0x4
#     u_int flag; // offset 0x4, size 0x4
#     u_int calc_total_time; // offset 0x8, size 0x4
#     u_int total_time; // offset 0xC, size 0x4
#     u_int get_time; // offset 0x10, size 0x4
#     u_int dmy[3]; // offset 0x14, size 0xC
# };
class MNG_STATE_ST(CStructure):
    state: c_int32
    flag: c_uint32
    calc_total_time: c_uint32
    total_time: c_uint32
    get_time: c_uint32
    flag: c_uint32
    dmy: c_uint32 * 3


# struct _TMP_MENU_MNG_ST {
#     // Members
#     struct _MNG_STATE_ST mng_state; // offset 0x0, size 0x20
#     int DrawState; // offset 0x20, size 0x4
#     u_char** pTmpMenuFileBuf; // offset 0x24, size 0x4
#     float draw_up_rate; // offset 0x28, size 0x4
#     u_char dmy[12]; // offset 0x2C, size 0xC
# };
class TMP_MENU_MNG_ST(CStructure):
    mng_state: MNG_STATE_ST
    DrawState: c_int32
    pTmpMenuFileBuf: c_addr_ptr
    draw_up_rate: c_float
    dmy: c_uint8 * 12

###########################################################################

elf_names: dict[str, str] = {
    "us": "SLUS_203.88",
    "eu": "SLES_508.21",
}


class VRamElf(BinaryIO):
    def __init__(self, elf: BinaryIO):
        self.elf = elf

    def read(self, n: int = -1):
        return self.elf.read(n)

    def seek(self, offset: int, whence: int = 0):
        return self.elf.seek(vram2offset(offset), whence)


# matches "var_name = 0x12345678; // attr1:val1 attr2:val2 ... attrN:valN"
line_pattern = re.compile(r"^([^\s=]+)\s*=\s*(0x[0-9a-fA-F]+);\s*\/\/((?:\s*[0-9a-zA-Z_]+(:?:[0-9a-zA-Z_\*]+)?)+)\s*$")
# matches multiple attributes in the form of attr:val or attr
attr_pattern = re.compile(r"([0-9a-zA-Z_]+)(?::([0-9a-zA-Z_\*]+))?")
# match address lines: "0xaabbccdd = &var"
addr_pattern = re.compile(r"^\s*(0x[0-9a-fA-F]+)\s*=\s*(.*)\s*;\s*(\/\/.*)?$")


class DataVar(pydantic.BaseModel):
    model_config = pydantic.ConfigDict(extra="forbid")

    _elf: VRamElf = pydantic.PrivateAttr()

    address: int
    name: str
    type: Type[CStructure] | CTypeType | type[sceVu0FVECTOR] | type[sceVu0IVECTOR] | type[qword] | type[c_str]
    numel: int | list[int] = 0
    nosize: bool = False
    static: bool = False

    num_ptr: int = 0

    @pydantic.model_validator(mode="after")
    def store_num_ptr(self):
        # transfer class attribute to instance
        if DataVar.num_ptr > 0:
            self.num_ptr = DataVar.num_ptr
        return self

    @pydantic.field_validator("type", mode="before")
    @classmethod
    def type_from_str(  # pyright: ignore
        cls, v: str | Type[CStructure] | CTypeType
    ) -> Type[CStructure] | CTypeType | sceVu0FVECTOR | sceVu0IVECTOR | qword | ANI_CODE | GS_REG | HEX_ADDR:
        if not isinstance(v, str):
            return v
        cls.num_ptr = v.count("*")  # temporary store num_ptr in class attribute
        v = v.rstrip("*")  # remove pointer(s)
        if v in ctypes_types:
            return ctypes_types[v]
        if v == "sceVu0FVECTOR":
            return sceVu0FVECTOR
        if v == "sceVu0IVECTOR":
            return sceVu0IVECTOR
        if v == "qword":
            return qword
        if v == "ANI_CODE":
            return ANI_CODE
        if v == "GS_REG":
            return GS_REG
        if v == "HEX_ADDR":
            return HEX_ADDR
        if v == "c_str":
            return c_str
        class_type = globals()[v]
        if issubclass(class_type, CStructure):
            return class_type
        raise ValueError(f"{v} is unknown/not a valid type")

    def data_var_dumps(self, addresses: dict[int, str]):
        self._elf.seek(self.address, os.SEEK_SET)

        if isinstance(self.numel, list):
            numel = math.prod(self.numel)
            ndims = len(self.numel)
        else:
            numel = max(1, self.numel)
            ndims = 1
        if self.num_ptr > 0:
            var_data = (c_addr_ptr * numel).from_buffer_copy(self._elf.read(numel * c_sizeof(c_addr_ptr)))
            if self.type == ANI_CODE:  # pyright: ignore
                type_str = "ANI_CODE" + ("*" * self.num_ptr)
            if self.type == GS_REG:  # pyright: ignore
                type_str = "u_long" + ("*" * self.num_ptr)
            if self.type == HEX_ADDR:  # pyright: ignore
                type_str = "u_int" + ("*" * self.num_ptr)
            elif issubclass(self.type, CStructure):  # pyright: ignore
                type_str = self.type.__name__ + ("*" * self.num_ptr)
            elif self.type == sceVu0FVECTOR:  # pyright: ignore
                type_str = "sceVu0FVECTOR" + ("*" * self.num_ptr)
            elif self.type == sceVu0IVECTOR:  # pyright: ignore
                type_str = "sceVu0IVECTOR" + ("*" * self.num_ptr)
            else:
                type_str = next(
                    k
                    for k, v in ctypes_types.items()
                    if getattr(v, "_type_") == getattr(self.type, "_type_")  # pyright: ignore
                ) + ("*" * self.num_ptr)
            stream = io.StringIO()
            if self.static:
                stream.write("static ")
            stream.write(f"{type_str} {self.name}")
            if numel > 1:
                numel = f"{self.numel}" if not self.nosize else ""
                var_str = f"{{ {', '.join(str(var) for var in cast(Iterable[c_addr_ptr], var_data))}, }}"  # pyright: ignore
                stream.write(f"[{numel}]")
            else:
                # should have been parsed as c_float_Array_4_Array_1
                assert len(var_data) == 1 and var_data[0].__class__ is c_addr_ptr
                var_str = str(var_data[0])
            stream.write(f" = {var_str};")
        elif issubclass(self.type, CStructure):  # pyright: ignore
            data = self._elf.read(numel * self.type.sizeof())

            return self.type.dumps(
                self.name,
                data,
                numel=self.numel,
                static=self.static,
                nosize=self.nosize,
                noarray=self.numel == 0,
            )
        elif self.type == sceVu0FVECTOR:  # pyright: ignore[reportUnknownMemberType]
            var_data = (sceVu0FVECTOR * numel).from_buffer_copy(self._elf.read(numel * c_sizeof(sceVu0FVECTOR)))
            type_str = "sceVu0FVECTOR"
            stream = io.StringIO()
            if self.static:
                stream.write("static ")
            stream.write(f"{type_str} {self.name}")

            def sceVu0FVECTOR_to_str(_v: sceVu0FVECTOR):  # pyright: ignore
                return f"{{ {', '.join(f'{flt}f' for flt in cast(Iterable[c_float], _v))} }}"

            if numel > 1:
                numel = f"{self.numel}" if not self.nosize else ""
                var_str = (
                    f"{{ {', '.join(sceVu0FVECTOR_to_str(var) for var in cast(Iterable[sceVu0FVECTOR], var_data))}, }}"  # pyright: ignore
                )
                stream.write(f"[{numel}]")
            else:
                # should have been parsed as c_float_Array_4_Array_1
                assert len(var_data) == 1 and var_data[0].__class__ is sceVu0FVECTOR
                var_str = sceVu0FVECTOR_to_str(var_data[0])
            stream.write(f" = {var_str};")
        elif self.type == sceVu0IVECTOR:  # pyright: ignore[reportUnknownMemberType]
            var_data = (sceVu0IVECTOR * numel).from_buffer_copy(self._elf.read(numel * c_sizeof(sceVu0IVECTOR)))
            type_str = "sceVu0IVECTOR"
            stream = io.StringIO()
            if self.static:
                stream.write("static ")
            stream.write(f"{type_str} {self.name}")

            def sceVu0IVECTOR_to_str(_v: sceVu0IVECTOR):  # pyright: ignore
                return f"{{ {', '.join(f'{int32}' for int32 in cast(Iterable[c_int32], _v))} }}"

            if numel > 1:
                numel = f"{self.numel}" if not self.nosize else ""
                var_str = (
                    f"{{ {', '.join(sceVu0IVECTOR_to_str(var) for var in cast(Iterable[sceVu0IVECTOR], var_data))}, }}"  # pyright: ignore
                )
                stream.write(f"[{numel}]")
            else:
                # should have been parsed as c_int_Array_4_Array_1
                assert len(var_data) == 1 and var_data[0].__class__ is sceVu0IVECTOR
                var_str = sceVu0IVECTOR_to_str(var_data[0])
            stream.write(f" = {var_str};")
        elif self.type == c_str:  # pyright: ignore
            assert numel > 1, "we only handle string arrays here, simple strings should be embedded in the source"
            var_data = (self.type * numel).from_buffer_copy(self._elf.read(numel * c_sizeof(self.type)))  # pyright: ignore
            stream = io.StringIO()
            if self.static:
                stream.write("static ")
            numel = f"{self.numel}" if not self.nosize else ""
            stream.write(f"char *{self.name}[{numel}] = {{")
            for var in cast(Iterable[c_str], var_data):
                stream.write(f"{var.to_str(self._elf)},")
            stream.write("};")
        else:
            typ = self.type  # pyright: ignore
            if ndims == 1:
                typ *= numel  # pyright: ignore
            elif isinstance(self.numel, list):
                for n in reversed(self.numel):
                    typ *= n  # pyright: ignore
            var_data = (typ).from_buffer_copy(self._elf.read(numel * c_sizeof(self.type)))  # pyright: ignore
            if self.type.__name__ == "ANI_CODE":  # pyright: ignore
                type_str = "ANI_CODE"
            elif self.type.__name__ == "GS_REG":  # pyright: ignore
                type_str = "u_long"
            elif self.type.__name__ == "HEX_ADDR":  # pyright: ignore
                type_str = "u_int"
            else:
                type_str = next(
                    k
                    for k, v in ctypes_types.items()
                    if getattr(v, "_type_") == getattr(self.type, "_type_")  # pyright: ignore
                )
            stream = io.StringIO()
            if self.static:
                stream.write("static ")
            stream.write(f"{type_str} {self.name}")
            if ndims == 1:
                numel = f"{self.numel}" if not self.nosize else ""
                var_str = f"{{ {', '.join(f'{var}' for var in cast(Iterable[CTypeType], var_data))} }}"
                stream.write(f"[{numel}]")
            elif ndims > 1:
                assert isinstance(self.numel, list)
                numel = ""
                for n, num in enumerate(self.numel):
                    numel += "[]" if n == 0 and self.nosize else f"[{num}]"
                stream.write(numel)
                tmpbuf = io.StringIO()
                tmpbuf.write("{")
                print_carr(var_data, tmpbuf)  # pyright: ignore
                tmpbuf.write("}")
                var_str = tmpbuf.getvalue()
            else:
                var_str = f"{var_data.value}"  # pyright: ignore
            stream.write(f" = {var_str};")
        return stream.getvalue()


def parse_data_vars(data_vars_txt: Path, strict: bool = True):
    data_vars: list[DataVar] = []
    addr_vals: dict[int, str] = {}

    with open(data_vars_txt, mode="r") as fh:
        for n, line in enumerate(fh):
            line = line.strip()

            # skip empty lines and comments
            if not line or line.startswith("//"):
                continue

            valid = False

            if addr_match := addr_pattern.match(line):
                address = int(addr_match.group(1), 16)
                value = addr_match.group(2)
                addr_vals[address] = value
                valid = True

            elif line_match := line_pattern.match(line):
                address = int(line_match.group(2), 16)
                options = line_match.group(3)
                name = line_match.group(1)
                if attr_match := attr_pattern.findall(options):
                    attrs: dict[str, str | list[str] | bool] = {}
                    for attr_name, attr_val in cast(list[tuple[str, str]], attr_match):
                        if attr_name not in attrs:
                            attrs[attr_name] = attr_val
                        elif not isinstance(attrs[attr_name], list):
                            attrs[attr_name] = cast(list[str], [attrs[attr_name], attr_val])
                        else:
                            cast(list[str], attrs[attr_name]).append(attr_val)
                    for key, value in attrs.items():
                        if not isinstance(value, bool) and len(value) == 0:
                            attrs[key] = True
                    try:
                        data_var = DataVar(address=address, name=name, **attrs)  # pyright: ignore[reportArgumentType]
                        data_vars.append(data_var)
                        if address not in addr_vals:
                            if (
                                isinstance(data_var.numel, list) or data_var.numel > 0 or data_var.type == sceVu0FVECTOR  # type: ignore
                            ):
                                addr_vals[address] = name
                            else:
                                addr_vals[address] = f"&{name}"
                        valid = True
                    except pydantic.ValidationError as ve:
                        print(f"syntax error at line {n}")
                        raise ve

            if strict and not valid:
                raise ValueError(f"syntax error at line {n}: {line}")

    return data_vars, addr_vals


def parse_data(lang: str):
    data_vars_txt = Path("config") / lang / "data_vars.txt"

    data_vars, addr_vals = parse_data_vars(data_vars_txt)

    if not data_vars:
        return

    if len(set(data_var.name for data_var in data_vars)) != len(data_vars):
        raise RuntimeError("duplicate names in data_vars.txt")

    elf_path = Path("config") / lang / elf_names[lang]

    include_path = Path("config") / lang / "include" / "data"
    include_path.mkdir(parents=True, exist_ok=True)

    c_addr_ptr.set_addresses(addr_vals)

    extract_filter: list[str] = []
    extract_filter_str = os.environ.get("EXTRACT_DATA_FILTER", "").strip()
    if extract_filter_str:
        extract_filter: list[str] = extract_filter_str.split(",")

    with open(elf_path, mode="rb") as elf:
        vram_elf = VRamElf(elf)
        CStructure.__elf__ = vram_elf

        for data_var in tqdm.tqdm(data_vars, desc="Extracting data"):
            header_path = include_path / f"{data_var.name}.h"

            if extract_filter and data_var.name not in extract_filter:
                continue

            with header_path.open(mode="w") as fw:
                data_var._elf = vram_elf  # pyright: ignore[reportPrivateUsage]

                fw.write(data_var.data_var_dumps(addr_vals))

            try:
                subprocess.run(["clang-format", "-i", "--style=Microsoft", header_path])
            except Exception:
                # formatting is best effort for now
                pass


def main():
    class Language(str, enum.Enum):
        US = "us"
        EU = "eu"

    languages = [lang.value for lang in Language]

    class ArgsProtocol(Protocol):
        language: Language

    parser = argparse.ArgumentParser(description="fixes asm removing gp_rel macro")
    parser.add_argument("language", type=str, choices=languages, help="language to extract data for")

    args = cast(ArgsProtocol, parser.parse_args())

    parse_data(args.language)


if __name__ == "__main__":
    main()
