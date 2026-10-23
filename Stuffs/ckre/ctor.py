"""Conservative constructor stores and allocation-call evidence."""

import re

from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG

from .config import HEADERS, VERSIONS


def register(name):
    if name is None:
        return ""
    aliases = {"eax": "rax", "ax": "rax", "al": "rax",
               "ecx": "rcx", "cx": "rcx", "cl": "rcx",
               "edx": "rdx", "dx": "rdx", "dl": "rdx",
               "ebx": "rbx", "ebp": "rbp", "esi": "rsi",
               "edi": "rdi", "esp": "rsp"}
    if re.fullmatch(r"r\d+[dwb]", name):
        return name[:-1]
    return aliases.get(name, name)


class Constructors:
    def __init__(self, rtti, disasm):
        self.rtti, self.disasm = rtti, disasm
        self.image = rtti.image
        self._allocators = None
        self._body_callers = None
        self._analyses = {}
        self._candidates = {}
        self._id_constructors = None

    def declared_constructors(self, name):
        if self._id_constructors is None:
            from .addrlib import AddressLibrary, parse_ids
            library = AddressLibrary(self.image.config, self.image.version)
            self._id_constructors = {}
            for entry in parse_ids(HEADERS / "IDs.h"):
                parts = entry["name"].split("::")
                leaf = parts.pop()
                if not leaf.endswith("Constructor") or len(parts) < 2:
                    continue
                parts = parts[1:]
                suffix = leaf.removesuffix("Constructor")
                parts[-1] += suffix
                index = min(list(VERSIONS).index(self.image.version),
                            len(entry["ids"]) - 1)
                rva = library.ids.get(entry["ids"][index])
                if rva and self.image.is_code(rva):
                    key = "::".join(parts)
                    self._id_constructors.setdefault(key, []).append(rva)
        return self._id_constructors.get(name, [])

    def candidates(self, name):
        if name in self._candidates:
            return self._candidates[name]
        try:
            record = self.rtti.get(name)
        except ValueError:
            result = [self.analyze(rva) for rva in self.declared_constructors(name)]
            self._candidates[name] = result
            return result
        primary = self.rtti.primary(record)
        if not primary:
            return []
        functions = {}
        for ref in self.disasm.xrefs(primary["rva"]):
            start = ref["function_rva"]
            result = self.analyze(start)
            primary_stores = [s for s in result["stores"]
                              if s["value"].get("kind") == "vtable" and
                              s["offset"] == 0]
            if primary_stores and primary_stores[-1]["value"]["rva"] == primary["rva"]:
                stores = [s for s in primary_stores
                          if s["value"]["rva"] == primary["rva"]]
                functions[start] = {**result, "vtable_store": stores[0]}
        self._candidates[name] = list(functions.values())
        return self._candidates[name]

    def analyze(self, rva):
        body = self.disasm.follow(rva)[0]
        if body in self._analyses:
            return self._analyses[body]
        regs = {"rcx": {"kind": "this", "offset": 0}}
        stores, calls = [], []

        def source(ins, operand):
            if operand.type == X86_OP_IMM:
                return {"kind": "constant", "value": operand.imm}
            if operand.type == X86_OP_REG:
                reg = register(ins.reg_name(operand.reg))
                return regs.get(reg, {"kind": "register", "register": reg}).copy()
            return {"kind": "memory", "operand": ins.op_str}

        for ins in self.disasm.instructions(body):
            operands = ins.operands
            mnemonic = ins.mnemonic
            if mnemonic == "call":
                target = operands[0].imm if operands[0].type == X86_OP_IMM else None
                calls.append({"site_rva": ins.address, "target_rva": target,
                              "rcx": regs.get("rcx"), "rdx": regs.get("rdx")})
                for reg in ("rax", "rcx", "rdx", "r8", "r9", "r10", "r11"):
                    regs.pop(reg, None)
                regs["rax"] = {"kind": "call_result", "call_rva": ins.address,
                               "target_rva": target}
                continue
            if not operands:
                continue
            dst = operands[0]
            value = None
            if mnemonic in ("xor", "pxor", "xorps", "xorpd") and len(operands) == 2:
                if dst.type == X86_OP_REG and operands[1].type == X86_OP_REG:
                    if dst.reg == operands[1].reg:
                        value = {"kind": "zero", "value": 0}
            elif mnemonic == "lea" and len(operands) == 2:
                src = operands[1]
                target = self.disasm.rip_target(ins, src)
                if target is not None:
                    value = {"kind": "vtable" if target in self.rtti.bytable else
                             "address", "rva": target,
                             "class": self.rtti.bytable.get(target)}
                elif src.type == X86_OP_MEM:
                    base = regs.get(register(ins.reg_name(src.mem.base)), {})
                    if base.get("kind") == "this" and not src.mem.index:
                        value = {"kind": "this",
                                 "offset": base["offset"] + src.mem.disp}
            elif mnemonic.startswith("mov") and len(operands) == 2:
                value = source(ins, operands[1])
            elif mnemonic in ("add", "sub") and len(operands) == 2:
                if dst.type == X86_OP_REG and operands[1].type == X86_OP_IMM:
                    old = regs.get(register(ins.reg_name(dst.reg)), {})
                    if old.get("kind") == "this":
                        delta = operands[1].imm * (1 if mnemonic == "add" else -1)
                        value = {**old, "offset": old["offset"] + delta}
            if dst.type == X86_OP_REG:
                reg = register(ins.reg_name(dst.reg))
                if value:
                    regs[reg] = value
                elif mnemonic not in ("cmp", "test", "push"):
                    regs.pop(reg, None)
            elif dst.type == X86_OP_MEM and value:
                base = regs.get(register(ins.reg_name(dst.mem.base)), {})
                if base.get("kind") == "this" and not dst.mem.index:
                    stores.append({
                        "site_rva": ins.address,
                        "offset": base["offset"] + dst.mem.disp,
                        "width": dst.size, "value": value,
                        "instruction": f"{mnemonic} {ins.op_str}",
                    })
            if mnemonic.startswith("rep stos"):
                base = regs.get("rdi", {})
                count = regs.get("rcx", {})
                value = regs.get("rax", {})
                if base.get("kind") == "this":
                    width = 8 if "q" in mnemonic else 4 if "d" in mnemonic else 1
                    stores.append({
                        "site_rva": ins.address, "offset": base["offset"],
                        "width": width, "count": count.get("value"),
                        "value": value, "instruction": mnemonic,
                    })
        result = {"rva": body, "address": self.image.address(body),
                  "symbol": self.rtti.publics.lookup(body),
                  "stores": stores, "calls": calls}
        self._analyses[body] = result
        return result

    @property
    def allocators(self):
        if self._allocators is None:
            _ = self.disasm.index
            named = {}
            for name, rva in self.rtti.publics.byname.items():
                if re.search(r"operator new|MemoryManager::Allocate|"
                             r"MemoryManager::.*Alloc|ScrapHeap::Allocate", name):
                    body = self.disasm.follow(rva)[0]
                    named[body] = name
            # Frequent targets are candidates, not allocator identities.
            samples = {}
            for target, refs in self.disasm.index.items():
                if len(refs) < 8 or not self.image.is_code(target):
                    continue
                if self.disasm.call_frequency[target] >= 100:
                    body = self.disasm.follow(target)[0]
                    samples[body] = samples.get(body, 0) + self.disasm.call_frequency[target]
            diagnostic = {}
            for body in samples:
                strings = self.disasm.strings(body)
                evidence = [s["text"] for s in strings
                            if re.search(r"memorymanager\.cpp|"
                                         r"alignment on allocation", s["text"], re.I)]
                if evidence:
                    diagnostic[body] = evidence
            self._allocators = {"named": named, "frequent": samples,
                                "diagnostic": diagnostic}
        return self._allocators

    def allocations(self, constructor):
        body = self.disasm.follow(constructor)[0]
        if self._body_callers is None:
            self._body_callers = {}
            for target, callers in self.disasm.index.items():
                calls = [c for c in callers if c["kind"] == "call"]
                if calls and self.image.is_code(target):
                    real = self.disasm.follow(target)[0]
                    self._body_callers.setdefault(real, []).extend(calls)
        refs = self._body_callers.get(body, [])
        results = []
        for ref in refs:
            if ref["kind"] != "call":
                continue
            instructions = self.disasm.instructions(ref["function_rva"])
            at = next((n for n, ins in enumerate(instructions)
                       if ins.address == ref["site_rva"]), None)
            if at is None:
                continue
            following = instructions[at + 1:]
            overwritten = False
            for ins in following[:24]:
                if ins.mnemonic == "call":
                    break
                if ins.mnemonic == "lea":
                    for operand in ins.operands:
                        target = self.disasm.rip_target(ins, operand)
                        if target in self.rtti.bytable:
                            overwritten = True
            if overwritten:
                continue
            preceding = instructions[max(0, at - 30):at]
            call_at = next((n for n in range(len(preceding) - 1, -1, -1)
                            if preceding[n].mnemonic == "call"), None)
            if call_at is None:
                continue
            call = preceding[call_at]
            if call.operands[0].type != X86_OP_IMM:
                continue
            allocator = self.disasm.follow(call.operands[0].imm)[0]
            between = preceding[call_at + 1:]
            forwards_result = any(
                ins.mnemonic == "mov" and ins.op_str == "rcx, rax"
                for ins in between)
            if not forwards_result:
                continue
            sizes = []
            for ins in reversed(preceding[max(0, call_at - 12):call_at]):
                if ins.mnemonic == "call":
                    break
                if ins.mnemonic == "mov" and len(ins.operands) == 2:
                    dst, src = ins.operands
                    if dst.type == X86_OP_REG and src.type == X86_OP_IMM:
                        reg = register(ins.reg_name(dst.reg))
                        if reg in ("rcx", "rdx") and 8 <= src.imm <= 0x1000000:
                            if not any(s["register"] == reg for s in sizes):
                                sizes.append({"register": reg, "size": src.imm,
                                              "site_rva": ins.address})
            if not sizes:
                continue
            named = self.allocators["named"].get(allocator)
            diagnostic = self.allocators["diagnostic"].get(allocator)
            frequency = self.disasm.call_frequency[call.operands[0].imm]
            if not named and allocator not in self.allocators["frequent"]:
                continue
            for size in sizes:
                if diagnostic and size["register"] != "rdx":
                    continue
                results.append({
                    **size, "constructor_call_rva": ref["site_rva"],
                    "allocator_call_rva": call.address, "allocator_rva": allocator,
                    "allocator_name": named, "xref_count": frequency,
                    "allocator_diagnostics": diagnostic,
                    "confidence": "named" if named else "binary_diagnostic"
                    if diagnostic else "frequency_heuristic",
                })
        return results

    def describe(self, target):
        try:
            rva = int(target, 0)
        except ValueError:
            candidates = self.candidates(target)
        else:
            candidates = [self.analyze(rva)]
        for candidate in candidates:
            candidate["allocations"] = self.allocations(candidate["rva"])
            candidate["allocation_sizes"] = sorted({
                a["size"] for a in candidate["allocations"]})
            candidate["initialized_extent"] = max((
                s["offset"] + s["width"] * (s.get("count") or 1)
                for s in candidate["stores"] if s["offset"] >= 0), default=None)
        return {"target": target, "version": self.image.version,
                "candidates": candidates,
                "limitations": "Linear register provenance; branches are not merged. "
                "Allocation sizes are call-site evidence, not a proof of ownership."}
