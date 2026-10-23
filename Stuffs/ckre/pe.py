"""Read-only mapped PE images, section translation and unwind bounds."""

from bisect import bisect_right
import mmap
import struct

import pefile


class Image:
    def __init__(self, config, version="246"):
        self.config, self.version = config, version
        self.path = config.exe(version)
        self.pe = pefile.PE(str(self.path), fast_load=True)
        self.base = self.pe.OPTIONAL_HEADER.ImageBase
        if self.pe.FILE_HEADER.Machine != 0x8664:
            raise ValueError(f"Not an x64 image: {self.path}")
        self.stream = self.path.open("rb")
        self.raw = mmap.mmap(self.stream.fileno(), 0, access=mmap.ACCESS_READ)
        self.sections = [{
            "name": s.Name.rstrip(b"\0").decode(),
            "rva": s.VirtualAddress, "size": s.Misc_VirtualSize,
            "raw": s.PointerToRawData, "raw_size": s.SizeOfRawData,
            "characteristics": s.Characteristics,
        } for s in self.pe.sections]
        self.functions = [tuple(x) for x in config.cached(
            self.path, "pdata", self._functions)]
        self.starts = [f[0] for f in self.functions]

    def close(self):
        self.raw.close()
        self.stream.close()
        self.pe.close()

    def _functions(self):
        directory = self.pe.OPTIONAL_HEADER.DATA_DIRECTORY[3]
        result = []
        for at in range(directory.VirtualAddress,
                        directory.VirtualAddress + directory.Size, 12):
            start, end, unwind = struct.unpack("<III", self.read(at, 12))
            if start and end > start:
                result.append((start, end))
        return sorted(set(result))

    def section(self, rva):
        return next((s for s in self.sections
                     if s["rva"] <= rva < s["rva"] + s["size"]), None)

    def is_code(self, rva):
        section = self.section(rva)
        return bool(section and section["name"] == ".text")

    def offset(self, rva):
        section = self.section(rva)
        if not section or rva - section["rva"] >= section["raw_size"]:
            raise ValueError(f"RVA 0x{rva:X} is not file-backed")
        return section["raw"] + rva - section["rva"]

    def rva(self, offset):
        section = next((s for s in self.sections
                        if s["raw"] <= offset < s["raw"] + s["raw_size"]), None)
        if not section:
            raise ValueError(f"File offset 0x{offset:X} is not in a section")
        return section["rva"] + offset - section["raw"]

    def read(self, rva, size):
        offset = self.offset(rva)
        section = self.section(rva)
        if rva + size > section["rva"] + section["raw_size"]:
            raise ValueError(f"Read crosses section at 0x{rva:X}")
        return self.raw[offset:offset + size]

    def u32(self, rva):
        return struct.unpack("<I", self.read(rva, 4))[0]

    def i32(self, rva):
        return struct.unpack("<i", self.read(rva, 4))[0]

    def u64(self, rva):
        return struct.unpack("<Q", self.read(rva, 8))[0]

    def cstring(self, rva, limit=1024):
        return self.read(rva, min(limit, self.section(rva)["rva"] +
                                 self.section(rva)["raw_size"] - rva)
                         ).split(b"\0", 1)[0].decode("ascii", "replace")

    def bounds(self, rva):
        at = bisect_right(self.starts, rva) - 1
        if at >= 0 and rva < self.functions[at][1]:
            return self.functions[at]
        return None

    def find(self, needle, section=None):
        sections = [s for s in self.sections if
                    section is None or s["name"] == section]
        for item in sections:
            at, end = item["raw"], item["raw"] + item["raw_size"]
            while (at := self.raw.find(needle, at, end)) != -1:
                yield item["rva"] + at - item["raw"]
                at += 1

    def address(self, rva):
        return {"rva": rva, "rva_hex": f"0x{rva:X}",
                "va": f"0x{self.base + rva:X}", "version": self.version}
