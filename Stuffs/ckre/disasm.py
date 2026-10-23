"""Unwind-bounded disassembly and reusable code-reference indexes."""

from collections import Counter
import hashlib
import re

from capstone import Cs, CS_ARCH_X86, CS_MODE_64
from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_REG_RIP


class Disasm:
    def __init__(self, image):
        self.image = image
        self.md = Cs(CS_ARCH_X86, CS_MODE_64)
        self.md.detail = True
        self.lite = Cs(CS_ARCH_X86, CS_MODE_64)
        self.lite.skipdata = True
        self._index = None

    def instructions(self, rva):
        bounds = self.image.bounds(rva)
        if not bounds:
            return []
        start, end = bounds
        return list(self.md.disasm(self.image.read(start, end - start), start))

    def follow(self, rva):
        chain, seen = [], set()
        for _ in range(32):
            if rva in seen or not self.image.is_code(rva):
                break
            seen.add(rva)
            data = self.image.read(rva, 6)
            if data[0] == 0xE9:
                target = rva + 5 + self.image.i32(rva + 1)
            elif data[:2] == b"\xff\x25":
                indirect = rva + 6 + self.image.i32(rva + 2)
                try:
                    target = self.image.u64(indirect) - self.image.base
                except ValueError:
                    break
            else:
                break
            if not self.image.is_code(target):
                break
            chain.append(rva)
            rva = target
        return rva, chain

    @staticmethod
    def rip_target(ins, operand):
        if operand.type == X86_OP_MEM and operand.mem.base == X86_REG_RIP:
            return ins.address + ins.size + operand.mem.disp
        return None

    def string_at(self, rva):
        section = self.image.section(rva)
        if not section or section["name"] not in (".rdata", ".data"):
            return None
        size = min(1024, section["rva"] + section["raw_size"] - rva)
        if size < 4:
            return None
        data = self.image.read(rva, size)
        if data[1] == 0 and data[0] != 0:
            end = next((i for i in range(0, len(data) - 1, 2)
                        if data[i:i + 2] == b"\0\0"), None)
            if end is None:
                return None
            try:
                value = data[:end].decode("utf-16le")
            except UnicodeError:
                return None
            encoding = "utf-16le"
        else:
            part = data.split(b"\0", 1)
            if len(part) != 2:
                return None
            try:
                value = part[0].decode("ascii")
            except UnicodeError:
                return None
            encoding = "ascii"
        if len(value) < 4 or not all(c.isprintable() or c in "\r\n\t" for c in value):
            return None
        return {"rva": rva, "text": value, "encoding": encoding}

    def strings(self, rva):
        result = {}
        for ins in self.instructions(rva):
            if ins.mnemonic not in ("lea", "mov", "movabs"):
                continue
            for operand in ins.operands:
                target = self.rip_target(ins, operand)
                if operand.type == X86_OP_IMM:
                    target = operand.imm - self.image.base
                if target is not None:
                    string = self.string_at(target)
                    if not string and ins.mnemonic.startswith("mov"):
                        try:
                            string = self.string_at(
                                self.image.u64(target) - self.image.base)
                        except ValueError:
                            pass
                    if string:
                        result.setdefault(string["rva"], {
                            **string, "instruction_rva": ins.address})
        return list(result.values())

    def fingerprint(self, rva, limit=64):
        body, chain = self.follow(rva)
        bounds = self.image.bounds(body)
        if not bounds:
            return {"available": False, "reason": "No .pdata bounds"}
        normalized = []
        for ins in self.instructions(body)[:limit]:
            operands = ins.op_str
            if ins.mnemonic.startswith(("call", "j")):
                operands = re.sub(r"0x[0-9a-f]+", "TARGET", operands)
            operands = re.sub(r"rip [+-] 0x[0-9a-f]+", "GLOBAL", operands)
            operands = re.sub(r"0x1[4-9a-f][0-9a-f]{7}", "ADDRESS", operands)
            normalized.append(f"{ins.mnemonic} {operands}".strip())
        return {
            "available": True, "body_rva": body, "thunks": chain,
            "size": bounds[1] - bounds[0],
            "normalized": normalized,
            "sha256": hashlib.sha256("\n".join(normalized).encode()).hexdigest(),
            "strings": self.strings(body),
        }

    @property
    def index(self):
        if self._index is None:
            rows = self.image.config.cached(self.image.path, "xrefs", self._scan)
            self._index = {}
            self.virtual_calls = {}
            self.call_frequency = Counter()
            for target, site, function, kind in rows:
                destination = (self.virtual_calls if kind == "virtual_call"
                               else self._index)
                destination.setdefault(target, []).append({
                    "site_rva": site, "function_rva": function, "kind": kind})
                if kind == "call":
                    self.call_frequency[target] += 1
        return self._index

    def _scan(self):
        result = []
        for start, end in self.image.functions:
            data = self.image.read(start, end - start)
            for at, size, mnemonic, operands in self.lite.disasm_lite(data, start):
                match = re.search(r"\[rip ([+-]) (0x[0-9a-f]+)\]", operands)
                if match:
                    displacement = int(match[2], 16) * (1 if match[1] == "+" else -1)
                    result.append([at + size + displacement, at, start, mnemonic])
                if mnemonic in ("call", "jmp") and re.fullmatch(
                        r"0x[0-9a-f]+", operands):
                    result.append([int(operands, 16), at, start, mnemonic])
                if mnemonic == "call":
                    match = re.fullmatch(
                        r"qword ptr \[[a-z0-9]+(?: \+ (0x[0-9a-f]+))?\]",
                        operands)
                    if match and "rip" not in operands:
                        result.append([int(match[1], 16) if match[1] else 0,
                                       at, start, "virtual_call"])
        return result

    def xrefs(self, rva, slot_offset=None):
        result = list(self.index.get(rva, []))
        if slot_offset is not None:
            result.extend(self.virtual_calls.get(slot_offset, []))
        return result
