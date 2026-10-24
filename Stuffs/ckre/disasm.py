"""Proven function bounds and reusable code-reference indexes."""

from bisect import bisect_right, insort
from collections import Counter
import hashlib
from itertools import chain
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
        self._leaves = {}
        self._leaf_starts = []

    def function(self, rva):
        bounds = self.image.bounds(rva)
        if bounds:
            return {"start": bounds[0], "end": bounds[1], "source": "pdata",
                    "complete": True, "provenance": [{"kind": "pdata"}]}
        if not self.image.is_code(rva):
            return None
        if rva in self._leaves:
            return self._leaves[rva]
        at = bisect_right(self._leaf_starts, rva) - 1
        if at >= 0:
            leaf = self._leaves[self._leaf_starts[at]]
            if rva < leaf["end"]:
                return leaf
        proof = self.padding_start(rva)
        if proof:
            found = self._leaf(rva, {rva: [proof]})
            if found:
                self._leaves[rva] = found[0]
                insort(self._leaf_starts, rva)
                return found[0]
        if self._index is None:
            _ = self.index
            return self.function(rva)
        return None

    def padding_start(self, rva):
        if rva % 16:
            return None
        section = self.image.section(rva)
        start = max(section["rva"], rva - 64)
        match = re.search(rb"(?:\xCC{2,}|\x90{4,})$",
                          self.image.read(start, rva - start))
        return ({"kind": "aligned_padding", "padding_rva": start + match.start(),
                 "padding_size": len(match[0])} if match else None)

    def bounds(self, rva):
        function = self.function(rva)
        return (function["start"], function["end"]) if function else None

    def instructions(self, rva):
        bounds = self.image.bounds(rva)
        if bounds:
            start, end = bounds
            return list(self.md.disasm(self.image.read(start, end - start), start))
        function = self.function(rva)
        return [ins for start, end in function["ranges"]
                for ins in self.md.disasm(self.image.read(start, end - start), start)
                ] if function else []

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
        function = self.function(body)
        if not function:
            return {"available": False, "reason": "No proven function bounds"}
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
            "size": function["end"] - function["start"],
            "function_source": function["source"],
            "function_provenance": function["provenance"],
            "bounds_complete": function["complete"],
            "normalized": normalized,
            "sha256": hashlib.sha256("\n".join(normalized).encode()).hexdigest(),
            "strings": self.strings(body),
        }

    @property
    def index(self):
        if self._index is None:
            rows = self.image.config.cached(self.image.path, "xrefs", self._scan)
            leaves = self.image.config.cached(
                self.image.path, "leaf-functions-v1", lambda: self._scan_leaves(rows))
            self._leaves = {row["start"]: row for row in leaves["functions"]}
            self._leaf_starts = sorted(self._leaves)
            self._index = {}
            self.virtual_calls = {}
            self.call_frequency = Counter()
            for target, site, function, kind in chain(rows, leaves["references"]):
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
                result.extend(self.references(start, at, size, mnemonic, operands))
        return result

    @staticmethod
    def references(start, at, size, mnemonic, operands):
        result = []
        match = re.search(r"\[rip ([+-]) (0x[0-9a-f]+)\]", operands)
        if match:
            displacement = int(match[2], 16) * (1 if match[1] == "+" else -1)
            result.append([at + size + displacement, at, start, mnemonic])
        if mnemonic in ("call", "jmp") and re.fullmatch(r"0x[0-9a-f]+", operands):
            result.append([int(operands, 16), at, start, mnemonic])
        if mnemonic == "call":
            match = re.fullmatch(
                r"qword ptr \[[a-z0-9]+(?: \+ (0x[0-9a-f]+))?\]", operands)
            if match and "rip" not in operands:
                result.append([int(match[1], 16) if match[1] else 0,
                               at, start, "virtual_call"])
        return result

    def _leaf(self, start, seeds):
        section = self.image.section(start)
        end = min(start + 0x4000, section["rva"] + section["raw_size"])
        pending, instructions, occupied, complete = [start], {}, set(), True
        while pending:
            block = pending.pop()
            if block in instructions:
                continue
            if not start <= block < end or self.image.bounds(block):
                return None
            if block in occupied:
                return None
            terminated = False
            for ins in self._stream(block, end):
                at = ins.address
                if at in instructions:
                    terminated = True
                    break
                if self.image.bounds(at) or len(instructions) >= 2048:
                    return None
                if at != start and at in seeds and any(
                        p["kind"] != "aligned_padding" for p in seeds[at]):
                    return None
                if any(byte in occupied for byte in range(at, at + ins.size)):
                    return None
                instructions[at] = ins
                occupied.update(range(at, at + ins.size))
                if ins.mnemonic in ("ret", "retf", "ud2", "int3"):
                    if at == start and ins.mnemonic == "int3":
                        return None
                    terminated = True
                    break
                if ins.mnemonic.startswith("j"):
                    direct = ins.operands and ins.operands[0].type == X86_OP_IMM
                    if not direct:
                        complete = complete and ins.mnemonic == "jmp" and any(
                            op.type == X86_OP_MEM and op.mem.base == X86_REG_RIP
                            for op in ins.operands)
                        terminated = True
                        break
                    target = ins.operands[0].imm
                    external = bool(self.image.bounds(target)) or (
                        target != start and target in seeds and any(
                            p["kind"] != "aligned_padding" for p in seeds[target]))
                    if not external:
                        if target < start or target >= end:
                            if ins.mnemonic != "jmp":
                                return None
                        else:
                            pending.append(target)
                    if ins.mnemonic == "jmp":
                        terminated = True
                        break
            if not terminated:
                return None
        ranges, refs = [], []
        for at, ins in sorted(instructions.items()):
            if ranges and ranges[-1][1] == at:
                ranges[-1][1] = at + ins.size
            else:
                ranges.append([at, at + ins.size])
            refs.extend(self.references(start, at, ins.size, ins.mnemonic, ins.op_str))
        return ({"start": start, "end": ranges[-1][1], "ranges": ranges,
                 "source": "inferred", "complete": complete,
                 "provenance": seeds[start]}, refs)

    def _stream(self, start, end):
        # Capstone decodes its whole input before yielding. Do not decode 16 KiB
        # per tiny leaf/thunk just to stop at the first RET/JMP.
        while start < end:
            previous = start
            for ins in self.md.disasm(
                    self.image.read(start, min(128, end - start)), start, count=16):
                start = ins.address + ins.size
                yield ins
            if start == previous:
                break

    def _scan_leaves(self, rows):
        from .rtti import RTTI
        seeds, queue = {}, []

        def seed(target, proof):
            if self.image.bounds(target) or not self.image.is_code(target):
                return
            if target not in seeds:
                seeds[target] = []
                queue.append(target)
            if sum(p["kind"] == proof["kind"] for p in seeds[target]) < 3:
                seeds[target].append(proof)

        for target, site, _, kind in rows:
            if kind == "call":
                seed(target, {"kind": "direct_call", "site_rva": site})
        rtti = RTTI(self.image)
        for record in rtti.classes:
            for table in record["vtables"]:
                for index, target in enumerate(table["slots"]):
                    seed(target, {"kind": "vtable_slot", "table_rva": table["rva"],
                                  "slot": index})
        for section in self.image.sections:
            if section["name"] != ".text":
                continue
            data = self.image.read(section["rva"], section["raw_size"])
            for match in re.finditer(rb"\xCC{2,}|\x90{4,}", data):
                target = section["rva"] + match.end()
                if target % 16 == 0:
                    seed(target, {"kind": "aligned_padding",
                                  "padding_rva": section["rva"] + match.start(),
                                  "padding_size": len(match[0])})
        functions, references, visited = [], [], set()
        index = 0
        while index < len(queue):
            target = queue[index]
            index += 1
            if target in visited:
                continue
            visited.add(target)
            found = self._leaf(target, seeds)
            if not found:
                continue
            function, refs = found
            functions.append(function)
            references.extend(refs)
            for destination, site, _, kind in refs:
                if kind == "call":
                    seed(destination, {"kind": "direct_call", "site_rva": site})
            body, chain = self.follow(target)
            if chain:
                seed(body, {"kind": "proven_thunk", "thunk_rva": target})
        return {"functions": sorted(functions, key=lambda row: row["start"]),
                "references": references}

    def xrefs(self, rva, slot_offset=None):
        result = list(self.index.get(rva, []))
        if slot_offset is not None:
            result.extend(self.virtual_calls.get(slot_offset, []))
        return [{**row, "function_source": "inferred",
                 "function_provenance": self._leaves[row["function_rva"]]["provenance"]}
                if row["function_rva"] in self._leaves else row for row in result]
