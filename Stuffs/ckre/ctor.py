"""Conservative constructor stores and allocation-call evidence."""

import re

from capstone.x86 import X86_OP_IMM, X86_OP_MEM, X86_OP_REG

from .config import HEADERS, VERSIONS


def register(name):
    if name is None:
        return ""
    aliases = {"eax": "rax", "ax": "rax", "al": "rax", "ah": "rax",
               "ecx": "rcx", "cx": "rcx", "cl": "rcx", "ch": "rcx",
               "edx": "rdx", "dx": "rdx", "dl": "rdx", "dh": "rdx",
               "ebx": "rbx", "bx": "rbx", "bl": "rbx", "bh": "rbx",
               "ebp": "rbp", "bp": "rbp", "bpl": "rbp",
               "esi": "rsi", "si": "rsi", "sil": "rsi",
               "edi": "rdi", "di": "rdi", "dil": "rdi",
               "esp": "rsp", "sp": "rsp", "spl": "rsp"}
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
        self._allocator_identities = {}
        self._factories = {}
        self._registry = None
        self._constant_traces = {}

    def constants(self, instructions):
        values, calls, writes = {}, [], []

        def operand_value(ins, operand):
            if operand.type == X86_OP_IMM:
                return operand.imm
            if operand.type == X86_OP_REG:
                name = ins.reg_name(operand.reg)
                known = values.get(register(name), {})
                shift = 8 if name in ("ah", "bh", "ch", "dh") else 0
                if known.get("width", 0) >= (2 if shift else min(operand.size, 4)):
                    return (known["value"] >> shift) & ((1 << operand.size * 8) - 1)
            return None

        def address(ins, operand):
            if operand.type != X86_OP_MEM:
                return None
            target = self.disasm.rip_target(ins, operand)
            if target is not None:
                return self.image.base + target
            mem = operand.mem
            base_value = values.get(register(ins.reg_name(mem.base)), {})
            index_value = values.get(register(ins.reg_name(mem.index)), {})
            base = base_value.get("value") if base_value.get("width", 0) >= 4 else None
            index = index_value.get("value") if index_value.get("width", 0) >= 4 else None
            if mem.base and base is None or mem.index and index is None:
                return None
            return (base or 0) + (index or 0) * mem.scale + mem.disp

        for ins in instructions:
            operands = ins.operands
            if ins.mnemonic in ("call", "jmp"):
                if operands and operands[0].type == X86_OP_IMM:
                    calls.append({
                        "site_rva": ins.address, "target_rva": operands[0].imm,
                        "kind": ins.mnemonic,
                        "arguments": {reg: values[reg].copy() for reg in
                                      ("rcx", "rdx", "r8", "r9") if reg in values}})
                if ins.mnemonic == "call":
                    for reg in ("rax", "rcx", "rdx", "r8", "r9", "r10", "r11"):
                        values.pop(reg, None)
                continue
            if not operands:
                _, changed = ins.regs_access()
                for reg in changed:
                    values.pop(register(ins.reg_name(reg)), None)
                continue
            dst = operands[0]
            value = None
            if len(operands) == 2:
                src = operands[1]
                if ins.mnemonic in ("xor", "sub") and dst.type == X86_OP_REG:
                    if src.type == X86_OP_REG and src.reg == dst.reg:
                        value = 0
                elif ins.mnemonic in ("mov", "movabs", "movzx", "movsxd"):
                    value = operand_value(ins, src)
                elif ins.mnemonic == "lea":
                    value = address(ins, src)
                elif ins.mnemonic in ("add", "sub"):
                    left, right = operand_value(ins, dst), operand_value(ins, src)
                    if left is not None and right is not None:
                        value = left + right * (1 if ins.mnemonic == "add" else -1)
            if dst.type == X86_OP_MEM and value is not None:
                at = address(ins, dst)
                writes.append({
                    "rva": at - self.image.base if at is not None else None,
                    "base": register(ins.reg_name(dst.mem.base)),
                    "displacement": dst.mem.disp, "indexed": bool(dst.mem.index),
                    "value": value, "site_rva": ins.address})
            _, changed = ins.regs_access()
            for reg in changed:
                values.pop(register(ins.reg_name(reg)), None)
            if dst.type == X86_OP_REG:
                reg = register(ins.reg_name(dst.reg))
                if value is not None and ins.reg_name(dst.reg) not in (
                        "ah", "bh", "ch", "dh"):
                    if dst.size <= 4:
                        value &= (1 << (dst.size * 8)) - 1
                    values[reg] = {
                        "value": value, "width": dst.size, "site_rva": ins.address,
                        "instruction": f"{ins.mnemonic} {ins.op_str}"}
        return calls, writes

    def allocator_identity(self, target, depth=0):
        body = self.disasm.follow(target)[0]
        if body in self._allocator_identities:
            return self._allocator_identities[body]
        if depth > 3 or not self.image.is_code(body):
            return None
        self._allocator_identities[body] = None
        symbol = self.rtti.publics.lookup(body)
        names = symbol["names"] if not symbol["nearest"] else []
        named = next((name for name in names if re.search(
            r"operator new|^\?\?2@|MemoryManager::.*Alloc|ScrapHeap::Allocate", name)), None)
        diagnostics = [s["text"] for s in self.disasm.strings(body) if re.search(
            r"memorymanager\.cpp|alignment on allocation", s["text"], re.I)]
        identity = None
        if named or diagnostics:
            identity = {
                "allocator_rva": body, "allocator_name": named,
                "allocator_diagnostics": diagnostics,
                "register": "rcx" if named and re.search(
                    r"operator new|^\?\?2@", named) else "rdx",
                "confidence": "named" if named else "binary_diagnostic",
                "allocator_wrappers": []}
        else:
            instructions = self.disasm.instructions(body)
            if len(instructions) <= 80:
                origins = {reg: reg for reg in ("rcx", "rdx", "r8", "r9")}
                for ins in instructions:
                    operands = ins.operands
                    if ins.mnemonic == "jmp" and operands[0].type == X86_OP_IMM:
                        parent = self.allocator_identity(operands[0].imm, depth + 1)
                        if parent and origins.get(parent["register"]) in ("rcx", "rdx"):
                            identity = {
                                **parent, "register": origins[parent["register"]],
                                "allocator_wrappers": [body, *parent["allocator_wrappers"]]}
                            break
                    if ins.mnemonic == "mov" and len(operands) == 2:
                        dst, src = operands
                        if dst.type == X86_OP_REG:
                            reg = register(ins.reg_name(dst.reg))
                            origin = origins.get(register(ins.reg_name(src.reg))) if (
                                src.type == X86_OP_REG) else None
                            if origin:
                                origins[reg] = origin
                            else:
                                origins.pop(reg, None)
                    elif ins.mnemonic == "call":
                        for reg in ("rax", "rcx", "rdx", "r8", "r9", "r10", "r11"):
                            origins.pop(reg, None)
                    else:
                        _, changed = ins.regs_access()
                        for reg in changed:
                            origins.pop(register(ins.reg_name(reg)), None)
        self._allocator_identities[body] = identity
        return identity

    def constant_trace(self, entry, instructions=None):
        if entry not in self._constant_traces:
            if instructions is None:
                instructions = [ins for ins in self.disasm.instructions(entry)
                                if ins.address >= entry]
            self._constant_traces[entry] = self.constants(instructions)
        return self._constant_traces[entry]

    def allocation_event(self, call):
        identity = self.allocator_identity(call["target_rva"])
        if not identity:
            return None
        argument = call["arguments"].get(identity["register"])
        if not argument or argument["width"] < 4 or not (
                8 <= argument["value"] <= 0x1000000):
            return None
        return {
            **identity, "size": argument["value"],
            "site_rva": argument["site_rva"],
            "size_instruction_rva": argument["site_rva"],
            "size_instruction": argument["instruction"],
            "allocator_call_rva": call["site_rva"],
            "allocation_site_rva": call["site_rva"],
            "allocator_target_rva": call["target_rva"]}

    def factory_allocations(self, name):
        if name in self._factories:
            return self._factories[name]
        prefix = ".?AV?$ConcreteFormFactory@V" + name + "@@"
        factories = []
        for record in self.rtti.classes:
            if not record["decorated"].startswith(prefix):
                continue
            table = self.rtti.primary(record)
            if not table or len(table["slots"]) < 2:
                continue
            create = self.disasm.follow(table["slots"][1])[0]
            visited = set()

            def inspect(entry, path, depth=0):
                body = self.disasm.follow(entry)[0]
                if body in visited or len(visited) >= 16:
                    return []
                visited.add(body)
                instructions = [i for i in self.disasm.instructions(body)
                                if i.address >= body]
                calls, _ = self.constant_trace(body, instructions)
                direct = [allocation for call in calls
                          if (allocation := self.allocation_event(call))]
                if direct:
                    return [{**direct[0], "create_path": [*path, body]}]
                if depth >= 2:
                    return []
                bounds = self.image.bounds(body)
                for ins in instructions:
                    if not ins.operands or ins.operands[0].type != X86_OP_IMM:
                        continue
                    target = ins.operands[0].imm
                    branch = ins.mnemonic.startswith("j") and not (
                        bounds and bounds[0] <= target < bounds[1])
                    if not (branch or ins.mnemonic == "call") or not self.image.is_code(target):
                        continue
                    child = self.disasm.follow(target)[0]
                    if len(self.disasm.instructions(child)) > 160:
                        continue
                    found = inspect(child, [*path, body], depth + 1)
                    if found:
                        return found
                return []

            allocations = [{
                **allocation, "source": "factory_create",
                "factory_vtable_rva": table["rva"],
                "factory_type_descriptor_rva": record["type_descriptor_rva"],
                "factory_decorated": record["decorated"], "create_rva": create,
            } for allocation in inspect(create, [])]
            factories.append({
                "decorated": record["decorated"], "factory_vtable_rva": table["rva"],
                "create_rva": create, "allocations": allocations})
        self._factories[name] = factories
        return factories

    def callers(self, body):
        if self._body_callers is None:
            self._body_callers = {}
            for target, callers in self.disasm.index.items():
                calls = [c for c in callers if c["kind"] == "call"]
                if calls and self.image.is_code(target):
                    real = self.disasm.follow(target)[0]
                    self._body_callers.setdefault(real, []).extend(calls)
        return self._body_callers.get(body, [])

    def factory_registry(self):
        if self._registry is not None:
            return self._registry
        if self.image.version != "246":
            return {"version": self.image.version, "status": "UNKNOWN",
                    "reason": "Registry anchors are established only for 246"}
        table, register_rva, lookup = 0x3D5F1B0, 0x8C1820, 0x8C17C0
        registrations, seen = [], set()
        for ref in self.callers(register_rva):
            if ref["function_rva"] in seen:
                continue
            seen.add(ref["function_rva"])
            calls, writes = self.constant_trace(ref["function_rva"])
            for call in calls:
                if self.disasm.follow(call["target_rva"])[0] != register_rva:
                    continue
                form_type = call["arguments"].get("rcx")
                pointer = call["arguments"].get("rdx")
                if not form_type or not pointer:
                    continue
                factory = pointer["value"] - self.image.base
                vtable = next((w["value"] - self.image.base for w in writes
                               if w["rva"] == factory and
                               w["value"] - self.image.base in self.rtti.bytable), None)
                if vtable is None:
                    try:
                        candidate = self.image.u64(factory) - self.image.base
                        if candidate in self.rtti.bytable:
                            vtable = candidate
                    except ValueError:
                        pass
                registrations.append({
                    "form_type": form_type["value"] & 0xFF,
                    "factory_rva": factory, "factory_vtable_rva": vtable,
                    "factory": self.rtti.bytable.get(vtable),
                    "registration_site_rva": call["site_rva"],
                    "function_rva": ref["function_rva"],
                    "form_type_instruction": form_type["instruction"],
                    "factory_instruction": pointer["instruction"]})
        entries = []
        for index in range(159):
            try:
                pointer = self.image.u64(table + index * 8)
                zero_fill = False
            except ValueError:
                pointer, zero_fill = 0, True
            entries.append({
                "form_type": index, "slot_rva": table + index * 8,
                "image_pointer": pointer, "loader_zero_fill": zero_fill,
                "registrations": [r for r in registrations if r["form_type"] == index]})
        anchors = {}
        for name, rva in (("register", register_rva), ("lookup", lookup)):
            instructions = self.disasm.instructions(rva)
            anchors[name] = any(
                self.disasm.rip_target(ins, operand) == table
                for ins in instructions for operand in ins.operands)
            anchors[name + "_bound"] = any(
                operand.type == X86_OP_IMM and operand.imm == 159
                for ins in instructions for operand in ins.operands)
        self._registry = {
            "version": "246", "status": "PASS" if all(anchors.values()) else "FAIL",
            "registry_rva": table, "slot_count": 159,
            "register_rva": register_rva, "lookup_rva": lookup,
            "unregister_rva": 0x8C1880, "anchor_checks": anchors,
            "requires_runtime_initialization": True, "entries": entries,
            "registration_count": len(registrations)}
        return self._registry

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
        refs = self.callers(body)
        results = []
        for ref in refs:
            if ref["kind"] != "call":
                continue
            instructions = self.disasm.instructions(ref["function_rva"])
            at = next((n for n, ins in enumerate(instructions)
                       if ins.address == ref["site_rva"]), None)
            if at is None:
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
            events, writes = self.constant_trace(ref["function_rva"], instructions)
            allocation_at = max(0, at - 30) + call_at
            if not self.owns_allocation(body, instructions, allocation_at, at, writes):
                continue
            event = next((e for e in events if e["site_rva"] == call.address), None)
            if not event:
                continue
            frequency = self.disasm.call_frequency[call.operands[0].imm]
            allocation = self.allocation_event(event)
            if allocation:
                results.append({
                    **allocation, "constructor_call_rva": ref["site_rva"],
                    "xref_count": frequency, "source": "constructor_call"})
                continue
            if frequency < 100:
                continue
            for reg in ("rcx", "rdx"):
                argument = event["arguments"].get(reg)
                if not argument or argument["width"] < 4 or not (
                        8 <= argument["value"] <= 0x1000000):
                    continue
                results.append({
                    "register": reg, "size": argument["value"],
                    "site_rva": argument["site_rva"],
                    "size_instruction": argument["instruction"],
                    "constructor_call_rva": ref["site_rva"],
                    "allocator_call_rva": call.address, "allocator_rva": allocator,
                    "allocator_name": None, "xref_count": frequency,
                    "allocator_diagnostics": [], "confidence": "frequency_heuristic",
                })
        return results

    def owns_allocation(self, constructor, instructions, allocation_at, at, writes):
        aliases = {"rax": 0}
        primary = [s["value"]["rva"] for s in self.analyze(constructor)["stores"]
                   if s["offset"] == 0 and s["value"]["kind"] == "vtable"]
        expected = primary[-1] if primary else None
        vptr_writes = {w["site_rva"]: w for w in writes
                       if w["value"] - self.image.base in self.rtti.bytable}
        for index in range(allocation_at + 1, len(instructions)):
            ins = instructions[index]
            if index == at:
                if aliases.get("rcx") != 0:
                    return False
                for reg in ("rcx", "rdx", "r8", "r9", "r10", "r11"):
                    aliases.pop(reg, None)
                aliases["rax"] = 0
                continue
            if index > at:
                write = vptr_writes.get(ins.address)
                if write and not write["indexed"] and write["base"] in aliases and (
                        aliases[write["base"]] + write["displacement"] == 0):
                    if write["value"] - self.image.base != expected:
                        return False
                if ins.mnemonic in ("call", "jmp", "ret"):
                    break
            operands = ins.operands
            offset = None
            if len(operands) == 2 and operands[0].type == X86_OP_REG:
                dst, src = operands
                if ins.mnemonic == "mov" and src.type == X86_OP_REG:
                    offset = aliases.get(register(ins.reg_name(src.reg)))
                elif ins.mnemonic == "lea" and src.type == X86_OP_MEM:
                    base = aliases.get(register(ins.reg_name(src.mem.base)))
                    if base is not None and not src.mem.index:
                        offset = base + src.mem.disp
                elif ins.mnemonic in ("add", "sub") and src.type == X86_OP_IMM:
                    base = aliases.get(register(ins.reg_name(dst.reg)))
                    if base is not None:
                        offset = base + src.imm * (1 if ins.mnemonic == "add" else -1)
            _, changed = ins.regs_access()
            for reg in changed:
                aliases.pop(register(ins.reg_name(reg)), None)
            if offset is not None and operands[0].size == 8:
                aliases[register(ins.reg_name(operands[0].reg))] = offset
        return True

    def describe(self, target):
        factories = []
        try:
            rva = int(target, 0)
        except ValueError:
            candidates = self.candidates(target)
            factories = self.factory_allocations(target)
        else:
            candidates = [self.analyze(rva)]
        for candidate in candidates:
            candidate["allocations"] = self.allocations(candidate["rva"])
            candidate["allocation_sizes"] = sorted({
                a["size"] for a in candidate["allocations"]})
            candidate["initialized_extent"] = max((
                s["offset"] + s["width"] * (s.get("count") or 1)
                for s in candidate["stores"] if s["offset"] >= 0), default=None)
            for allocation in candidate["allocations"]:
                allocation["constructor_initialized_extent"] = candidate[
                    "initialized_extent"]
        if factories and self.image.version == "246":
            registry = self.factory_registry()
            registrations = [r for slot in registry["entries"]
                             for r in slot["registrations"]]
            for factory in factories:
                matched = [r for r in registrations if r["factory_vtable_rva"] ==
                           factory["factory_vtable_rva"]]
                factory["registrations"] = matched
                for allocation in factory["allocations"]:
                    allocation["factory_rvas"] = sorted({
                        r["factory_rva"] for r in matched})
                    allocation["form_types"] = sorted({r["form_type"] for r in matched})
        factory_evidence = [a for factory in factories for a in factory["allocations"]]
        return {"target": target, "version": self.image.version,
                "candidates": candidates, "factories": factories,
                "factory_allocations": factory_evidence,
                "allocation_sizes": sorted({
                    a["size"] for a in factory_evidence} | {
                    a["size"] for c in candidates for a in c["allocations"]
                    if a["confidence"] in ("named", "binary_diagnostic")}),
                "limitations": "Linear register provenance; branches are not merged. "
                "Allocation sizes are call-site evidence, not a proof of ownership."}
