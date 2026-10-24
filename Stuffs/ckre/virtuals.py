"""Source slot annotations versus MSVC's actual primary virtual table."""

from collections import defaultdict
import re

NUMBER = r"((?:0[xX])?(?=[0-9A-Fa-f]*[0-9])[0-9A-Fa-f]+)"


def method_name(name):
    name = name.strip().rsplit("::", 1)[-1].strip()
    return "{dtor}" if name.startswith("~") or name == "{dtor}" else name


def declarations(body, first_line=1):
    """Read direct member declarations, not nested classes or inline bodies."""
    masked = re.sub(r"/\*.*?\*/|//[^\n]*|\"(?:\\.|[^\"\\])*\"",
                    lambda m: re.sub(r"[^\n]", " ", m[0]), body, flags=re.S)
    masked = re.sub(r"\[\[.*?\]\]", lambda m: re.sub(r"[^\n]", " ", m[0]),
                    masked, flags=re.S)
    result, depth, start, active = [], 0, 0, None
    ignored = {"noexcept", "decltype", "sizeof", "alignof", "alignas",
               "__declspec", "static_assert", "requires"}

    def member(end):
        header = masked[start:end]
        match = next((m for m in re.finditer(r"(~?[A-Za-z_]\w*)\s*\(", header)
                      if m[1] not in ignored), None)
        if not match or re.search(r"\b(using|typedef|return)\b", header[:match.start()]):
            return None
        name_at = start + match.start()
        opening, nesting = start + match.end() - 1, 1
        closing = opening + 1
        while closing < end and nesting:
            nesting += (masked[closing] == "(") - (masked[closing] == ")")
            closing += 1
        if nesting:
            return None
        suffix = masked[closing:end]
        virtual = bool(re.search(r"\bvirtual\b", header[:match.start()]))
        override = bool(re.search(r"\b(?:override|final)\b", suffix))
        annotation, annotation_kind = None, None
        prefix = body[start:name_at]
        marks = list(re.finditer(r"/\*\s*" + NUMBER + r"\s*\*/", prefix))
        if marks:
            annotation, annotation_kind = marks[-1][1], "block"
        else:
            marks = list(re.finditer(
                r"(?m)^[ \t]*//\s*" + NUMBER + r"\s*$", prefix))
            if marks:
                annotation, annotation_kind = marks[-1][1], "line"
        return {"name": method_name(match[1]), "declaration_order": len(result),
                "line": first_line + body.count("\n", 0, name_at),
                "parameters": masked[opening + 1:closing - 1].strip(),
                "const": bool(re.match(r"\s*const\b", suffix)),
                "virtual": virtual, "override": override,
                "annotation": int(annotation, 16) if annotation is not None else None,
                "annotation_literal": annotation, "annotation_kind": annotation_kind}

    def trailing(row, end):
        line_end = body.find("\n", end)
        if line_end < 0:
            line_end = len(body)
        tail = body[end:line_end]
        match = re.match(
            r"[ \t]*(?://\s*" + NUMBER + r"(?:\s+-[^\n]*)?\s*$|"
            r"/\*\s*" + NUMBER + r"\s*\*/)", tail)
        if match:
            if row is not None:
                row["annotation"] = int(match[1] or match[2], 16)
                row["annotation_literal"] = match[1] or match[2]
                row["annotation_kind"] = "line" if match[1] else "block"
            return end + match.end()
        return end

    for at, character in enumerate(masked):
        if at < start:
            continue
        if character == "{":
            if depth == 0:
                active = member(at)
                if active:
                    result.append(active)
            depth += 1
        elif character == "}":
            depth -= 1
            if depth == 0:
                start = trailing(active, at + 1)
                active = None
        elif character == ";" and depth == 0:
            row = member(at)
            if row:
                result.append(row)
            start = trailing(row, at + 1)
    return result


def parameter_key(declaration):
    # Parameter variable names/defaults do not participate in an override.
    parameters = []
    depth, start = 0, 0
    text = declaration["parameters"]
    for at, char in enumerate(text + ","):
        depth += char in "<(["
        depth -= char in ">)]"
        if char == "," and depth == 0:
            parameter = text[start:at].split("=", 1)[0].strip()
            variable = re.search(r"(?<=[\s*&])\b[A-Za-z_]\w*\s*(\[[^]]*\])?$", parameter)
            if variable and parameter[:variable.start()].strip() not in (
                    "", "const", "volatile", "class", "struct", "unsigned",
                    "signed", "long", "unsigned long", "long long"):
                parameter = parameter[:variable.start()] + (variable[1] or "")
            parameters.append(re.sub(r"\s+", "", parameter))
            start = at + 1
    if parameters in ([""], ["void"]):
        parameters = []
    return tuple(parameters), declaration["const"]


def bind_declarations(source, layout, base=None):
    """MSVC reverses newly introduced overloads; overrides keep base slots."""
    entries = layout.get("vtable_entries", [])
    families, groups = defaultdict(list), defaultdict(list)
    for entry in entries:
        families[entry["name"]].append(entry)
    explicit_names = {row["name"] for row in source.get("virtual_declarations", [])
                      if row["virtual"] or row["override"]}
    for declaration in source.get("virtual_declarations", []):
        if declaration["name"] in families:
            if (declaration["name"] in explicit_names and
                    not declaration["virtual"] and not declaration["override"] and
                    not any(row["name"] == declaration["name"] and
                            parameter_key(row) == parameter_key(declaration)
                            for row in base or [])):
                continue  # A nonvirtual overload must not consume a virtual slot.
            groups[declaration["name"]].append(declaration)
    result = []
    for name, declarations_ in groups.items():
        slots = families[name]
        if len(slots) == len(declarations_) == 1:
            result.append({**declarations_[0], "compiled_slot": slots[0]["slot"]})
            continue
        assigned, remaining = set(), []
        base_family = [row for row in base or [] if row["name"] == name
                       and row.get("compiled_slot") is not None]
        for declaration in declarations_:
            matches = [row for row in base_family
                       if parameter_key(row) == parameter_key(declaration)]
            if len(matches) == 1:
                slot = matches[0]["compiled_slot"]
                if any(entry["slot"] == slot for entry in slots):
                    result.append({**declaration, "compiled_slot": slot})
                    assigned.add(slot)
                    continue
            remaining.append(declaration)
        inherited_slots = {row["compiled_slot"] for row in base_family}
        candidates = [entry for entry in slots
                      if entry["slot"] not in assigned | inherited_slots]
        # Without signatures in /d1report, never guess between a hidden inherited
        # overload and a newly introduced one.
        if len(candidates) == len(remaining):
            for declaration, entry in zip(remaining, reversed(candidates)):
                result.append({**declaration, "compiled_slot": entry["slot"]})
        else:
            result.extend({**declaration, "compiled_slot": None,
                           "candidate_slots": [entry["slot"] for entry in candidates]}
                          for declaration in remaining)
    for row in result:
        entry = next((entry for entry in entries if entry["slot"] == row["compiled_slot"]), None)
        owner = entry["symbol"].rsplit("::", 1)[0] if entry else ""
        owner = re.sub(r"\b(class|struct)\s+", "", owner)
        source_name = source["cpp_name"]
        row["compiled_virtual"] = bool(
            row["virtual"] or row["override"] or
            owner == source_name or owner == layout.get("instantiated_cpp_name") or
            owner.startswith(source_name + "<"))
    return sorted(result, key=lambda row: row["declaration_order"])


def inventory_bindings(inventory, compiled, normalize):
    layouts = {normalize(name): layout for name, layout in compiled.items()}
    sources = {normalize(source["cpp_name"]): source for source in inventory}
    cache, active = {}, set()

    def visit(name):
        key = normalize(name)
        if key in cache:
            return cache[key]
        if key in active:
            return []
        active.add(key)
        layout = layouts.get(key)
        source = sources.get(key) or sources.get(key.split("<", 1)[0])
        result = []
        if layout and source:
            base = next((base["cpp_name"] for base in layout["bases"]
                         if base["offset"] == 0 and
                         layouts.get(normalize(base["cpp_name"]), {}).get(
                             "vtable_slots") is not None), None)
            result = bind_declarations(source, layout, visit(base) if base else [])
        active.remove(key)
        cache[key] = result
        return result

    return {name: visit(name) for name in
            dict.fromkeys([*(source["cpp_name"] for source in inventory), *compiled])}


def annotation_check(source, layout, bindings=None):
    bindings = bindings if bindings is not None else bind_declarations(source, layout)
    annotated = [row for row in bindings if row["annotation"] is not None and
                 row["compiled_virtual"]]
    virtual_orders = {row["declaration_order"] for row in bindings if row["compiled_virtual"]}
    declared = [row for row in source.get("virtual_declarations", [])
                if row["annotation"] is not None and (
                    row["virtual"] or row["override"] or
                    row["declaration_order"] in virtual_orders)]
    if not declared:
        return {"status": "SKIP", "convention": None, "checks": [], "warnings": []}
    count = layout["vtable_slots"] or 0
    hexadecimal_lines = any(
        row["annotation_kind"] == "line" and (
            re.search(r"[a-fx]", row["annotation_literal"], re.I) or
            (len(row["annotation_literal"]) >= 3 and
             row["annotation_literal"].startswith("0")))
        for row in declared)
    line_rows = [row for row in declared if row["annotation_kind"] == "line"]
    line_radix = 16 if hexadecimal_lines else 10
    ambiguous_radix = False
    if line_rows and not hexadecimal_lines:
        in_range = {base: all(int(row["annotation_literal"], base) < count
                             for row in line_rows) for base in (10, 16)}
        virtual_order = {
            row["declaration_order"]: index for index, row in enumerate(
                row for row in source.get("virtual_declarations", [])
                if row["virtual"] or row["override"] or
                row["declaration_order"] in virtual_orders)}
        scores = {base: sum(
            int(right["annotation_literal"], base) - int(left["annotation_literal"], base)
            == virtual_order[right["declaration_order"]] - virtual_order[left["declaration_order"]]
            for left, right in zip(line_rows, line_rows[1:])) for base in (10, 16)}
        if in_range[16] and not in_range[10]:
            line_radix = 16
        elif in_range[10] and not in_range[16]:
            line_radix = 10
        elif scores[16] != scores[10]:
            line_radix = 16 if scores[16] > scores[10] else 10
        elif any(int(row["annotation_literal"], 10) != int(row["annotation_literal"], 16)
                 for row in line_rows):
            ambiguous_radix = True

    def value(row):
        radix = (16 if row["annotation_kind"] == "block" or
                 re.search(r"[a-fx]", row["annotation_literal"], re.I) else 10)
        if row["annotation_kind"] == "line":
            radix = line_radix
        return int(row["annotation_literal"], radix)

    values = [value(row) for row in declared]
    byte_offsets = (not ambiguous_radix and
                    all(value % 8 == 0 for value in values) and max(values) > count)
    convention = "byte_offset" if byte_offsets else "slot_index"
    checks, warnings = [], []
    for declaration in declared:
        bound = next((row for row in annotated
                      if row["declaration_order"] == declaration["declaration_order"]), None)
        expected = value(declaration) // (8 if byte_offsets else 1)
        check = {"method": declaration["name"], "line": declaration["line"],
                 "parameters": declaration["parameters"], "const": declaration["const"],
                 "declaration_order": declaration["declaration_order"],
                 "annotation": value(declaration),
                 "annotation_literal": declaration["annotation_literal"],
                 "annotated_slot": expected,
                 "compiled_slot": bound["compiled_slot"] if bound else None}
        if bound and bound["compiled_slot"] is not None:
            check["status"] = "PASS" if bound["compiled_slot"] == expected else "FAIL"
            if ambiguous_radix and declaration["annotation_kind"] == "line":
                candidates = sorted({int(declaration["annotation_literal"], base)
                                     for base in (10, 16)})
                check["candidate_annotated_slots"] = candidates
                if len(candidates) > 1 and bound["compiled_slot"] in candidates:
                    check["status"] = "WARNING"
                    check["reason"] = "Decimal/hex annotation radix is ambiguous"
                    warnings.append(check)
        else:
            secondary = [table["offset"] for table in layout.get("vftables", [])
                         if table["offset"] and any(
                             entry["name"] == declaration["name"]
                             for entry in table["entries"])]
            check["status"] = "SKIP" if secondary else "WARNING"
            check["reason"] = ("Secondary table only; primary annotations checked"
                               if secondary else "Method/overload cannot be bound unambiguously")
            if secondary:
                check["secondary_offsets"] = secondary
            else:
                warnings.append(check)
        checks.append(check)
    status = ("FAIL" if any(row["status"] == "FAIL" for row in checks) else
              "WARNING" if warnings else "PASS")
    return {"status": status, "convention": convention,
            "line_annotation_radix": None if ambiguous_radix else line_radix, "checks": checks,
            "warnings": warnings}


class OverrideChecker:
    """Use primary-base positions, thunk-normalized bodies and exact publics only."""

    def __init__(self, rtti, disasm):
        self.rtti, self.disasm = rtti, disasm
        self.bodies, self.names = {}, {}

    def body(self, rva):
        if rva not in self.bodies:
            self.bodies[rva] = self.disasm.follow(rva)[0]
        return self.bodies[rva]

    def names_at(self, rva):
        if rva not in self.names:
            names = set()
            for at in {rva, self.body(rva)}:
                for symbol in self.rtti.publics.names.get(at, []):
                    if "::" not in symbol or symbol.startswith("?"):
                        continue
                    name = symbol.split("(", 1)[0].rsplit("::", 1)[-1].strip()
                    if name in ("scalar_deleting_destructor", "vector_deleting_destructor"):
                        name = "{dtor}"
                    if re.fullmatch(r"(?:vf|sub_)[0-9a-fA-F]+", name):
                        continue
                    if re.fullmatch(r"[A-Za-z_]\w*|\{dtor\}", name):
                        names.add(name)
            self.names[rva] = names
        return self.names[rva]

    def check(self, source, layout, bindings, base_layout, base_bindings, binary):
        base = self.rtti.direct_base(binary) if binary else None
        table = self.rtti.primary(binary) if binary else None
        base_table = self.rtti.primary(base) if base else None
        if not base_layout or not table or not base_table:
            return {"status": "SKIP", "checks": [], "warnings": []}
        base_slots = defaultdict(list)
        for entry in base_layout.get("vtable_entries", []):
            base_slots[entry["name"]].append(entry["slot"])
        native_slots = defaultdict(set)
        for slot, target in enumerate(table["slots"]):
            for name in self.names_at(target):
                native_slots[name].add(slot)
        checks, warnings = [], []
        for declaration in bindings:
            name, slot = declaration["name"], declaration["compiled_slot"]
            if name not in base_slots:
                continue
            # A same-named nonvirtual helper need not override its base.
            if not declaration["virtual"] and not declaration["override"]:
                entry = next((entry for entry in layout.get("vtable_entries", [])
                              if entry["slot"] == slot), None)
                if not entry or "::" + name not in entry["symbol"] or not any(
                        parameter_key(row) == parameter_key(declaration)
                        for row in base_bindings if row["name"] == name):
                    continue
            signatures = [row for row in base_bindings if row["name"] == name
                          and parameter_key(row) == parameter_key(declaration)
                          and row["compiled_slot"] is not None]
            if base_bindings and not signatures and not declaration["override"] and (
                    slot is not None and slot not in base_slots[name]):
                # Same name, different signature: a new overload, not an override.
                continue
            expected = (signatures[0]["compiled_slot"] if len(signatures) == 1 else
                        base_slots[name][0] if len(base_slots[name]) == 1 else None)
            check = {"method": name, "compiled_slot": slot, "base_slot": expected,
                     "binary_base": base["name"], "status": "WARNING"}
            if slot is None or expected is None:
                check["reason"] = "Overload or primary-base slot is ambiguous"
            elif slot != expected:
                unambiguous = len(signatures) == 1 and declaration["compiled_virtual"]
                check["status"] = "FAIL" if unambiguous else "WARNING"
                check["reason"] = "Declared override differs from modeled primary-base slot"
            elif slot >= len(table["slots"]) or slot >= len(base_table["slots"]):
                check["status"] = "FAIL"
                check["reason"] = "Override has no corresponding binary primary-base slot"
            else:
                target, inherited = table["slots"][slot], base_table["slots"][slot]
                check.update({
                    "binary_target_rva": target, "binary_base_target_rva": inherited,
                    "binary_relation": ("inherited" if self.body(target) == self.body(inherited)
                                        else "override")})
                named = native_slots.get(name, set())
                if len(named) == 1 and slot not in named and self.names_at(target):
                    check["status"] = "FAIL"
                    check["binary_named_slot"] = next(iter(named))
                    check["reason"] = "Exact binary method public is at a different primary slot"
                elif slot in named:
                    check["status"] = "PASS"
                    check["reason"] = "Exact binary public agrees with the primary-base position"
                elif check["binary_relation"] == "inherited":
                    check["status"] = "PASS"
                    check["reason"] = "Exact base body inherited at the modeled primary slot"
                else:
                    check["reason"] = (
                        "Binary override/inheritance observed; method identity is not unique "
                        "in exact publics (folding, missing symbols or renamed method)")
            checks.append(check)
            if check["status"] == "WARNING":
                warnings.append(check)
        return {"status": ("FAIL" if any(c["status"] == "FAIL" for c in checks)
                           else "WARNING" if warnings else "PASS" if checks else "SKIP"),
                "checks": checks, "warnings": warnings}
