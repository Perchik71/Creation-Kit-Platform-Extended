"""MSVC class-layout gate and shared source inventory."""

from collections import Counter
from concurrent.futures import ThreadPoolExecutor
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import xml.etree.ElementTree as ET

from .config import HEADERS, ROOT, atomic_write


def object_alias(expression):
    """Pointer/reference arguments are objects; pointer/reference aliases are not."""
    depth = 0
    for character in expression:
        if character == "<":
            depth += 1
        elif character == ">":
            depth -= 1
            if depth < 0:
                return False
        elif depth == 0 and character in "&*(){}[]":
            return False
    return depth == 0


def source_inventory(headers=None, instances=()):
    result, aliases, declarations = [], [], {}
    selected = {p.resolve() for p in headers} if headers else None
    for path in sorted(HEADERS.rglob("*.h")):
        text = path.read_text("utf-8-sig", errors="replace")
        masked = re.sub(r"/\*.*?\*/|//[^\n]*|\"(?:\\.|[^\"\\])*\"",
                        lambda m: " " * len(m[0]), text, flags=re.S)
        sizes = [(m.start(), m[1].strip(), int(m[2], 0)) for m in re.finditer(
            r"static_assert\s*\(\s*sizeof\s*\(\s*([\w:]+)\s*\)"
            r"\s*==\s*(0x[0-9a-fA-F]+|\d+)", masked)]
        tokens = list(re.finditer(
            r"\bnamespace\s+([\w:]+)\s*\{|"
            r"\b(?:class|struct)\s+"
            r"(?:(?:alignas|__declspec)\([^)]*\)\s*)*(\w+)"
            r"(?:\s+final)?\s*(?::[^;{}]+)?\s*\{|([{}])|"
            r"\busing\s+(\w+)\s*=\s*([^;]+);|"
            r"\btypedef\s+([^;]+?)\s+(\w+)\s*;|"
            r"\b(public|private|protected)\s*:|"
            r"\b(?:class|struct)\s+(\w+)\s*;", masked))
        stack, access = [], {}

        def templated(at):
            return bool(re.search(r"\btemplate\s*<[^;{}]+>\s*$",
                                  masked[max(0, at - 2000):at]))

        for token in tokens:
            if token[1]:
                stack.append(("namespace", token[1], token))
            elif token[2]:
                kind = "other" if re.search(
                    r"\benum\s*$", masked[max(0, token.start() - 10):token.start()]
                ) else "class"
                stack.append((kind, token[2] if kind == "class" else "", token))
                access[token.start()] = "public" if token[0].startswith("struct") else "private"
            elif token[4] or token[7]:
                name, expression = ((token[4], token[5]) if token[4] else
                                    (token[7], token[6]))
                if not object_alias(expression):
                    continue
                scope = "::".join(s[1] for s in stack if s[1])
                qualified = scope + "::" + name if scope else name
                after = [(at, size) for at, target, size in sizes
                         if at > token.start() and target in (name, qualified)]
                aliases.append({
                    "cpp_name": qualified, "expression": expression.strip(),
                    "scope": scope, "assert_size": min(after)[1] if after else None,
                    "is_template": templated(token.start()),
                    "probe_alias": not templated(token.start()) and all(
                        kind == "namespace" or (
                            kind == "class" and not templated(opening.start()) and
                            access[opening.start()] == "public")
                        for kind, _, opening in stack),
                    "header": path.relative_to(ROOT).as_posix()})
            elif token[8]:
                if stack and stack[-1][0] == "class":
                    access[stack[-1][2].start()] = token[8]
            elif token[9]:
                if all(kind in ("namespace", "class") for kind, _, _ in stack):
                    qualified = "::".join([s[1] for s in stack if s[1]] + [token[9]])
                    declarations.setdefault(qualified, path.relative_to(ROOT).as_posix())
            elif token[3] == "{":
                stack.append(("other", "", token))
            elif stack:
                kind, name, opening = stack.pop()
                if kind != "class":
                    continue
                body = text[opening.end():token.start()]
                body_masked = masked[opening.end():token.start()]
                qualified = "::".join([s[1] for s in stack if s[1]] + [name])
                binary = qualified.removeprefix("CKPE::Fallout4::EditorAPI::")
                for prefix in ("Forms::", "NiAPI::"):
                    binary = binary.removeprefix(prefix)
                asserts = [(at, size) for at, target, size in sizes
                           if target in (name, binary, qualified)]
                after = [(at, size) for at, size in asserts if at > token.start()]
                asserted = min(after)[1] if after else None
                arrays, scalars, unknown_bytes = [], 0, 0
                for field in re.finditer(
                    r"\b([\w:]+)\s+((_?pad|_?unk)\w*)\s*"
                    r"(?:\[\s*(0x[0-9a-fA-F]+|\d+)\s*\])?\s*;",
                    body, re.I):
                    if field[4]:
                        widths = {"char": 1, "byte": 1, "uint8_t": 1,
                                  "uint16_t": 2, "uint32_t": 4,
                                  "uint64_t": 8, "float": 4, "double": 8}
                        width = widths.get(field[1].split("::")[-1])
                        amount = int(field[4], 0) * width if width else None
                        arrays.append({"field": field[2], "bytes": amount})
                        unknown_bytes += amount or 0
                    elif "unk" in field[2].lower():
                        scalars += 1
                result.append({
                    "name": binary, "cpp_name": qualified,
                    "header": path.relative_to(ROOT).as_posix(),
                    "assert_size": asserted,
                    "value_fields": [{
                        "type": field[1].strip(), "field": field[2],
                    } for field in re.finditer(
                        r"^\s*(?:mutable\s+)?"
                        r"([\w:]+(?:\s*<[^;{}]+>)?)\s+(\w+)"
                        r"\s*(?:\[[^\]]+\]\s*)*(?:=[^;]+|\{[^{}]*\})?;",
                        body_masked, re.M)
                        if not re.search(r"\b(static|using|typedef|return)\b",
                                         field[1])],
                    "unknown_bytes": unknown_bytes,
                    "unknown_arrays": arrays, "unk_scalars": scalars,
                    "sub_virtuals": len(re.findall(
                        r"\bvirtual\b[^;{}]*\bsub_\w*\s*\(", body)),
                })
    definitions = {source["cpp_name"]: source for source in result}
    alias_names = {alias["cpp_name"]: alias for alias in aliases}
    expanded = {}

    def lookup(name, scope):
        parts = scope.split("::") if scope else []
        for depth in range(len(parts), -1, -1):
            candidate = "::".join(parts[:depth] + [name])
            if candidate in definitions or candidate in alias_names or candidate in declarations:
                return candidate
        return name

    def expand(expression, scope, seen, loose=False):
        def identifier(match):
            target = lookup(match[0], scope)
            if loose and target == match[0]:
                candidates = [name for name in
                              definitions.keys() | alias_names.keys() | declarations.keys()
                              if name.endswith("::" + target)]
                if len(candidates) == 1:
                    target = candidates[0]
            if target in alias_names:
                if target in seen:
                    return target
                alias = alias_names[target]
                if alias["is_template"]:
                    # Let MSVC substitute template parameters and defaults.
                    # Textual expansion here would leave free Key/T/etc. and
                    # append a second argument list to the underlying type.
                    return target
                return expand(alias["expression"], alias["scope"], seen | {target}, loose)
            return target
        return re.sub(r"\b[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*", identifier, expression)

    def definition_of(target):
        root, seen = target.split("<", 1)[0].strip(), set()
        while root in alias_names and root not in seen:
            seen.add(root)
            alias = alias_names[root]
            root = expand(alias["expression"], alias["scope"], seen).split("<", 1)[0].strip()
        return definitions.get(root)

    for alias in aliases:
        target = expand(alias["expression"], alias["scope"], {alias["cpp_name"]})
        definition = definition_of(target)
        if not definition or target == alias["cpp_name"]:
            continue
        name = alias["cpp_name"].removeprefix("CKPE::Fallout4::EditorAPI::")
        for prefix in ("Forms::", "NiAPI::"):
            name = name.removeprefix(prefix)
        inherited_assert = alias["assert_size"]
        if inherited_assert is None:
            parent = alias_names.get(lookup(alias["expression"], alias["scope"]))
            if parent:
                inherited_assert = parent["assert_size"]
        row = {
            **definition, "name": name, "cpp_name": alias["cpp_name"],
            "header": alias["header"], "assert_size": inherited_assert,
            "alias_of": target, "template_definition": definition["cpp_name"],
            "probe_alias": alias["probe_alias"]}
        previous = expanded.get(alias["cpp_name"])
        if previous and previous["assert_size"] is not None and inherited_assert is None:
            continue
        expanded[alias["cpp_name"]] = row
    # Explicit evidence can name a concrete template specialization directly.
    # Generic definitions remain UNKNOWN; no proof is generalized to other T's.
    for expression in sorted(set(instances)):
        if "<" not in expression or not re.fullmatch(r"[\w:<>,*&\s]+", expression):
            continue
        if not object_alias(expression):
            continue
        target = expand(expression, "CKPE::Fallout4::EditorAPI", set(), loose=True)
        definition = definition_of(target)
        if not definition or target in expanded or target in definitions:
            continue
        expanded[target] = {
            **definition, "name": expression.removeprefix("CKPE::Fallout4::EditorAPI::"),
            "cpp_name": target, "assert_size": None, "alias_of": target,
            "template_definition": definition["cpp_name"], "probe_alias": True,
            "evidence_instance": True, "evidence_class": expression,
            "witness_headers": sorted({
                (definitions[match[0]]["header"] if match[0] in definitions
                 else declarations[match[0]] if match[0] in declarations
                 else alias_names[match[0]]["header"])
                for match in re.finditer(r"\b[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*", target)
                if match[0] in definitions or match[0] in declarations
                or match[0] in alias_names})}
    binary_aliases = {}
    for alias in expanded.values():
        definition_name = alias["template_definition"].split("::")[-1]
        if definition_name == alias["name"].split("::")[-1] + "_base":
            binary_aliases[alias["template_definition"]] = alias["name"]
    for alias in expanded.values():
        alias["declaration_headers"] = sorted({
            a["header"] for a in aliases if a["cpp_name"] == alias["cpp_name"]}
            or {alias["header"]})
        if alias["template_definition"] in binary_aliases:
            alias["binary_name"] = binary_aliases[alias["template_definition"]]
    result.extend(expanded.values())
    if selected:
        result = [source for source in result if
                  any((ROOT / header).resolve() in selected for header in
                      source.get("declaration_headers", [source["header"]]))]
    return result


def normalize_type(name):
    return re.sub(r"\s+", "", re.sub(r"\b(class|struct|__ptr64)\s*", "", name))


def evidence_identities(source):
    return {normalize_type(source[key]) for key in
            ("name", "cpp_name", "binary_name", "alias_of", "evidence_class")
            if key in source}


def bind_alias_layouts(inventory, compiled, bindings=None):
    normalized = {normalize_type(name): (name, layout)
                  for name, layout in compiled.items()}
    for source in inventory:
        if "alias_of" not in source:
            continue
        target = (bindings or {}).get(source["cpp_name"], source["alias_of"])
        match = normalized.get(normalize_type(target))
        if match:
            name, layout = match
            compiled[source["cpp_name"]] = {**layout, "instantiated_cpp_name": name}


def compiler_environment():
    if os.name != "nt":
        raise RuntimeError("layout check requires Windows and MSVC")
    installer = Path(os.environ.get("ProgramFiles(x86)", "")) / (
        "Microsoft Visual Studio/Installer/vswhere.exe")
    if not installer.is_file():
        raise FileNotFoundError("vswhere.exe not found")
    installation = subprocess.check_output(
        [str(installer), "-latest", "-products", "*", "-requires",
         "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
         "-property", "installationPath"], text=True).strip()
    if not installation:
        raise RuntimeError("No MSVC x64 installation found by vswhere")
    vcvars = Path(installation) / "VC/Auxiliary/Build/vcvars64.bat"
    command = f'"{os.environ["COMSPEC"]}" /d /s /c ""{vcvars}" >nul && set"'
    output = subprocess.check_output(command, text=True, errors="replace")
    return dict(line.split("=", 1) for line in output.splitlines()
                if "=" in line and not line.startswith("="))


def compile_headers(headers, directory, environment, extra=""):
    source = directory / "layout.cpp"
    source.write_text("\n".join(
        f'#include "{p.as_posix()}"' for p in headers) + "\n" + extra, "utf-8")
    includes = ["CKPE/Include", "CKPE.Common/Include", "CKPE.Fallout4/Include",
                "Dependencies", "Dependencies/toml11", "Dependencies/zydis/include",
                "Dependencies/xbyak"]
    project = ET.parse(ROOT / "CKPE.Fallout4/CKPE.Fallout4.vcxproj")
    definitions = []
    for element in project.iter():
        if element.tag.endswith("}PreprocessorDefinitions") and element.text:
            definitions.extend(d for d in element.text.split(";")
                               if d and not d.startswith("%"))
    executable = shutil.which("cl.exe", path=next(
        v for k, v in environment.items() if k.lower() == "path"))
    if not executable:
        raise FileNotFoundError("cl.exe not found in vcvars64 environment")
    args = [executable, "/nologo", "/c", "/std:c++latest", "/EHsc",
            "/d1reportAllClassLayout", "/utf-8", "/w",
            f"/Fo{directory / 'layout.obj'}", str(source)]
    args += [f"/I{ROOT / p}" for p in includes]
    args += [f"/D{d}" for d in sorted(set(definitions))]
    process = subprocess.run(args, cwd=directory, env=environment,
                             stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                             text=True, errors="replace")
    return process.returncode, process.stdout


def parse_layout(output):
    classes = {}
    blocks = list(re.finditer(
        r"^(?:class|struct)\s+(.+?)\s+size\((\d+)\):", output, re.M))
    for index, match in enumerate(blocks):
        name = match[1].strip()
        if "CKPE::Fallout4::EditorAPI::" not in name:
            continue
        end = blocks[index + 1].start() if index + 1 < len(blocks) else len(output)
        block = output[match.end():end].split("\n\n", 1)[0]
        bases = [{"offset": int(base[1]), "cpp_name": base[2]}
                 for base in re.finditer(
                     r"^\s*(\d+)\s*\|\s*\+--- \(base class (.+)\)",
                     block, re.M)]
        members = [member[1].strip() for member in re.finditer(
            r"^\s*\d+\s*\|\s*([^|+{}\n][^\n]*)", block, re.M)]
        classes[name] = {"size": int(match[2]), "vtable_slots": None,
                         "bases": bases, "members": members, "methods": []}
    for match in re.finditer(r"^(.+?)::\$vftable(?:@.*?)?:\s*\n", output, re.M):
        name = match[1].strip()
        if name not in classes or classes[name]["vtable_slots"] is not None:
            continue
        rest = output[match.end():]
        block = rest.split("\n\n", 1)[0]
        indices = [int(m[1]) for m in re.finditer(r"^\s*(\d+)\s+\|", block, re.M)]
        classes[name]["vtable_slots"] = max(indices) + 1 if indices else 0
        classes[name]["methods"] = [
            method[1].strip().split("::")[-1] for method in re.finditer(
                r"^\s*\d+\s*\|\s*&?(.+?)\s*$", block, re.M)]
    return classes


def ordered_headers(headers):
    selected = {p.resolve(): p for p in headers}
    ordered, active, done = [], set(), set()

    def visit(path):
        key = path.resolve()
        if key in done or key in active:
            return
        active.add(key)
        for dependency in re.findall(
                r"#include\s*[<\"](EditorAPI/[^>\"]+)",
                path.read_text("utf-8-sig", errors="replace")):
            target = (HEADERS.parent / dependency).resolve()
            if target in selected:
                visit(selected[target])
        active.remove(key)
        done.add(key)
        ordered.append(path)
    for path in sorted(headers):
        visit(path)
    return ordered


def compile_inventory(config, headers=None, inventory=None):
    headers = ordered_headers(headers or sorted(HEADERS.rglob("*.h")))
    inventory = inventory if inventory is not None else source_inventory(headers)
    witness_headers = sorted({header for source in inventory
                              for header in source.get("witness_headers", [])})
    headers = ordered_headers([*headers, *(ROOT / header for header in witness_headers)])
    aliases = sorted(source["cpp_name"] for source in inventory
                     if source.get("probe_alias"))
    extra = "\n".join(
        f"template<class T, unsigned N> struct ckre_type_{index} "
        f"{{ unsigned char bytes[N]; }};\n"
        f"ckre_type_{index}<{name}, sizeof({name})> ckre_type_value_{index};"
        for index, name in enumerate(aliases))
    # Transitive includes and project flags invalidate cached compiler reports.
    inputs = sorted((ROOT / "CKPE/Include").rglob("*.h"))
    inputs += sorted((ROOT / "CKPE.Common/Include").rglob("*.h"))
    inputs += sorted(HEADERS.rglob("*.h"))
    stamp = hashlib.sha256()
    for path in inputs:
        stamp.update(path.relative_to(ROOT).as_posix().encode())
        stamp.update(path.read_bytes())
    stamp.update((ROOT / "CKPE.Fallout4/CKPE.Fallout4.vcxproj").read_bytes())
    stamp.update("\n".join(str(p) for p in headers).encode())
    stamp.update(("alias-probes-v1" + extra).encode())
    cache = config.cache / f"layout-{stamp.hexdigest()[:24]}.json"
    if cache.exists():
        result = json.loads(cache.read_text("utf-8"))
        report = config.cache / result["report"]
        if report.exists() and result["compiler_exit_code"] == 0:
            result["classes"] = parse_layout(report.read_text("utf-8"))
        return result
    environment = compiler_environment()
    with tempfile.TemporaryDirectory(prefix="ckre-layout-") as temporary:
        directory = Path(temporary)
        code, output = compile_headers(headers, directory, environment, extra)
        skipped = []
        if code:
            # Keep standalone failures visible instead of masking them.
            def probe(item):
                index, header = item
                work = directory / str(index)
                work.mkdir()
                status, diagnostic = compile_headers([header], work, environment)
                return header, status, diagnostic
            with ThreadPoolExecutor(max_workers=4) as pool:
                probes = list(pool.map(probe, enumerate(headers)))
            good = []
            for header, status, diagnostic in probes:
                if status:
                    skipped.append({
                        "header": header.relative_to(ROOT).as_posix(),
                        "status": "UNKNOWN",
                        "errors": [line for line in diagnostic.splitlines()
                                   if ": error " in line or ": fatal error " in line],
                    })
                else:
                    good.append(header)
            code, output = compile_headers(good, directory, environment, extra)
        report = config.cache / f"layout-{stamp.hexdigest()[:24]}.txt"
        atomic_write(report, output)
        result = {"classes": parse_layout(output), "skipped_headers": skipped,
                  "witness_headers": witness_headers,
                  "alias_bindings": {
                      aliases[int(match[1])]: match[2].strip()
                      for match in re.finditer(
                          r"^(?:class|struct) ckre_type_(\d+)<(.+),\s*\d+>"
                          r"\s+size\(", output, re.M)},
                  "compiler_exit_code": code,
                  "report": report.relative_to(config.cache).as_posix(),
                  "errors": [line for line in output.splitlines()
                             if ": error " in line or ": fatal error " in line]}
        if code:
            result["classes"] = {}
        atomic_write(cache, json.dumps(result))
        return result


GATE_STATES = ("PASS", "PARTIAL", "STUB", "UNKNOWN", "FAIL")


def evidence_alignments(config, compiled, headers, names):
    if not names:
        return {}
    names = sorted(names)
    stamp = hashlib.sha256(
        ("alignment-v2" + compiled["report"] + repr(names)).encode()).hexdigest()[:24]
    cache = config.cache / f"alignment-{stamp}.json"
    if cache.exists():
        return json.loads(cache.read_text("utf-8"))
    skipped = {item["header"] for item in compiled["skipped_headers"]}
    includes = ordered_headers(headers or sorted(HEADERS.rglob("*.h")))
    includes = ordered_headers([
        *includes, *(ROOT / header for header in compiled.get("witness_headers", []))])
    includes = [path for path in includes
                if path.relative_to(ROOT).as_posix() not in skipped]
    environment = compiler_environment()

    def probe(selected):
        extra = "\n".join(
            f"template<unsigned N> struct ckre_alignment_{index} "
            f"{{ unsigned char bytes[N]; }};\n"
            f"ckre_alignment_{index}<alignof({name})> ckre_alignment_value_{index};"
            for index, name in enumerate(selected))
        with tempfile.TemporaryDirectory(prefix="ckre-align-") as temporary:
            code, text = compile_headers(includes, Path(temporary), environment, extra)
        if code:
            return {}
        return {selected[int(match[1])]: int(match[2])
                for match in re.finditer(
                    r"\b(?:class|struct) ckre_alignment_(\d+)<(\d+)>\s+size\(", text)}

    result = probe(names)
    if len(result) != len(names):
        for name in names:
            if name not in result:
                result.update(probe([name]))
    atomic_write(cache, json.dumps(result))
    return result


def embedded_classes(inventory, compiled):
    sources = {source["cpp_name"]: source for source in inventory}
    embedded = {}
    for owner in inventory:
        layout = compiled.get(owner["cpp_name"])
        if not layout:
            continue
        for base in layout["bases"]:
            target = sources.get(base["cpp_name"])
            if target:
                embedded.setdefault(target["name"], []).append({
                    "owner": owner["name"], "field": "(base subobject)"})
        fields = list(owner["value_fields"])
        for member in layout["members"]:
            field = re.fullmatch(r"([\w:]+)\s+(\w+)", member)
            if field and not any(f["field"] == field[2] for f in fields):
                fields.append({"type": field[1], "field": field[2]})
        for field in fields:
            if not any(re.search(r"\b" + re.escape(field["field"]) + r"$", member)
                       for member in layout["members"]):
                continue
            datatype = re.sub(r"\b(const|volatile)\b", "", field["type"]).strip()
            if "<" in datatype:
                array = re.fullmatch(r"std::array\s*<\s*([\w:]+)\s*,.+>", datatype)
                if not array:
                    continue
                datatype = array[1]
            scopes = owner["cpp_name"].split("::")
            candidates = ["::".join(scopes[:depth] + [datatype])
                          for depth in range(len(scopes), -1, -1)]
            target = next((sources[name] for name in candidates if name in sources),
                          None)
            if target:
                embedded.setdefault(target["name"], []).append({
                    "owner": owner["name"], "field": field["field"]})
    return embedded


def embedded_base_footprints(rtti):
    gaps = {}
    for owner in rtti.classes:
        bases = owner["bases"]
        for index, parent in enumerate(bases):
            siblings, child = [], index + 1
            end = min(len(bases), child + parent["contained_bases"])
            while child < end:
                siblings.append(bases[child])
                child += bases[child]["contained_bases"] + 1
            for base, following in zip(siblings, siblings[1:]):
                if base["pdisp"] != -1 or following["pdisp"] != -1:
                    continue
                size = following["mdisp"] - base["mdisp"]
                if size > 0:
                    gaps.setdefault(base["name"], []).append({
                        "owner": owner["name"], "parent": parent["name"],
                        "offset": base["mdisp"], "next_base": following["name"],
                        "next_offset": following["mdisp"], "size": size})
    result = {}
    for name, evidence in gaps.items():
        minimum = min(item["size"] for item in evidence)
        supporting = [item for item in evidence if item["size"] == minimum]
        if len({item["owner"] for item in supporting}) >= 2:
            result[name] = {"size": minimum, "evidence": supporting}
    return result


def check(config, rtti, constructors, headers=None, allocations=True,
          evidence_directory=None):
    from .evidence import evaluate, verify
    from .hkclass import Reflection
    proof_report = verify(config, [rtti.image.version],
                          images={rtti.image.version: rtti.image},
                          directory=evidence_directory)
    entries = [entry for entry in proof_report["entries"] if entry["status"] == "PASS"]
    inventory = source_inventory(headers, instances=[entry["class"] for entry in entries])
    compiled = compile_inventory(config, headers, inventory)
    bind_alias_layouts(inventory, compiled["classes"], compiled.get("alias_bindings"))
    embedded = embedded_classes(inventory, compiled["classes"])
    footprints = embedded_base_footprints(rtti)
    reflection = Reflection(constructors)

    def claims(source):
        identities = evidence_identities(source)
        result = [entry for entry in entries
                  if normalize_type(entry["class"]) in identities]
        reflected = reflection.size_evidence(source)
        if reflected:
            result.append(reflected)
        return result

    matched = {source["cpp_name"]: claims(source) for source in inventory}
    bounded = {name for name, evidence in matched.items()
               if {"lower_bound", "upper_bound"} <= {e["kind"] for e in evidence}
               and name in compiled["classes"]}
    alignments = evidence_alignments(config, compiled, headers, bounded)
    rows, binary_records = [], {}

    def fail(row, kind, header, binary, reason):
        row[kind + "_status"] = "FAIL"
        row["failures"].append({"kind": kind, "header": header,
                                "binary": binary, "reason": reason})
        row["reasons"].append(reason)

    for source in inventory:
        layout = compiled["classes"].get(source["cpp_name"])
        row = {**source, "status": "UNKNOWN", "size_status": "UNKNOWN",
               "vtable_status": "UNKNOWN", "verified": [], "failures": [],
               "header_size": layout["size"] if layout else None,
               "header_vtable_slots": layout["vtable_slots"] if layout else None,
               "binary_vtable_slots": None, "binary_sizes": [],
               "embedded_by": embedded.get(source["name"], []),
               "primary_methods": layout["methods"] if layout else [],
               "instantiated_cpp_name": layout.get("instantiated_cpp_name") if layout else None,
               "new_primary_virtuals": None, "reasons": [], "evidence": [],
               "header_alignment": alignments.get(source["cpp_name"]),
               "size_evidence": matched[source["cpp_name"]],
               "size_evidence_status": "UNKNOWN"}
        rows.append(row)
        if not layout:
            row["reasons"].append("Compiler did not emit this class layout")
            continue
        try:
            binary = rtti.get(source.get("binary_name", source["name"]))
        except ValueError:
            binary = None
        if binary:
            binary_records[source["name"]] = binary
            footprint = footprints.get(binary["name"])
            if footprint:
                row["embedded_footprint"] = footprint
            table = rtti.primary(binary)
            if table:
                row["binary_vtable_slots"] = len(table["slots"])
                row["evidence"].append("RTTI primary vtable")
            if allocations:
                description = constructors.describe(binary["name"])
                evidence = [a for c in description["candidates"]
                            for a in c["allocations"] if a["confidence"] in
                            ("named", "binary_diagnostic")]
                evidence += description["factory_allocations"]
                row["binary_sizes"] = sorted({a["size"] for a in evidence})
                row["allocation_evidence"] = evidence
                boundary = footprint["size"] if footprint else layout["size"]
                composite = [a for a in evidence if
                             a.get("constructor_initialized_extent") is not None
                             and a["constructor_initialized_extent"] > boundary]
                row["possible_composite_allocations"] = composite
                row["size_gate_binary_sizes"] = sorted({
                    a["size"] for a in evidence if a not in composite})
                if composite:
                    row["reasons"].append(
                        "Possible composite allocation: constructor stores "
                        "extend beyond the embedded footprint or claimed size")
        claimed = (source["assert_size"] is not None or bool(row["embedded_by"])
                   or bool(row["size_evidence"]))
        row["size_obligation"] = claimed
        if not claimed:
            row["size_status"] = "STUB" if binary else "UNKNOWN"
            row["reasons"].append("No size claim or by-value embedding")
        elif source["assert_size"] is not None and layout["size"] != source["assert_size"]:
            fail(row, "size", layout["size"], source["assert_size"],
                 "MSVC size disagrees with static_assert")
        elif row.get("size_gate_binary_sizes"):
            sizes = row["size_gate_binary_sizes"]
            if sizes == [layout["size"]]:
                row["size_status"] = "PASS"
                row["verified"].append("size")
            elif layout["size"] not in sizes:
                fail(row, "size", layout["size"], sizes,
                     "Claimed or embedded size disagrees with binary allocation")
            else:
                row["reasons"].append("Conflicting allocation sizes")
        elif row.get("embedded_footprint", {}).get("size") == layout["size"]:
            row["size_status"] = "PASS"
            row["verified"].append("size")
            row["evidence"].append("RTTI sibling-base offset gaps")
            row["reasons"].append("PASS (embedded footprint)")
        else:
            row["reasons"].append("No unambiguous binary allocation size")
        if row["size_evidence"]:
            assessed = evaluate(row["size_evidence"], rtti.image.version,
                                layout["size"], row["header_alignment"])
            row["size_evidence_status"] = assessed.get("label", assessed["status"])
            row["size_evidence_bounds"] = {
                key: assessed[key] for key in (
                    "lower_bound", "upper_bound", "rounded_lower", "rounded_upper",
                    "alignment", "exact_size")}
            if assessed["status"] == "ERROR":
                for message in assessed["errors"]:
                    proof_report["errors"].append({
                        "file": None, "entry": None, "class": row["name"],
                        "error": message})
            elif assessed["status"] == "FAIL":
                fail(row, "size", layout["size"],
                     assessed["exact_size"] or {
                         "lower_bound": assessed["lower_bound"],
                         "upper_bound": assessed["upper_bound"]},
                     "; ".join(assessed["reasons"]))
            elif assessed["status"] == "PASS" and row["size_status"] != "FAIL":
                row["size_status"] = "PASS"
                if "size" not in row["verified"]:
                    row["verified"].append("size")
                row["reasons"].append(assessed["label"])
                row["evidence"].append("Mechanically verified committed size evidence")
            elif row["size_status"] != "FAIL":
                row["reasons"].append("Verified bounds do not establish an exact size")
        cpp_slots, binary_slots = row["header_vtable_slots"], row["binary_vtable_slots"]
        if cpp_slots is not None and binary_slots is not None:
            if cpp_slots == binary_slots:
                row["vtable_status"] = "PASS"
                row["verified"].append("vtable")
            elif cpp_slots < binary_slots:
                row["vtable_status"] = "PARTIAL"
                row["reasons"].append("Trailing virtual slots use vtbl_call")
            else:
                fail(row, "vtable", cpp_slots, binary_slots,
                     "Compiled primary vtable overruns binary vtable")
        elif binary_slots is not None:
            row["reasons"].append("MSVC did not emit primary vftable")
        if cpp_slots is not None:
            primary = next((compiled["classes"].get(base["cpp_name"])
                            for base in layout["bases"] if base["offset"] == 0
                            and compiled["classes"].get(base["cpp_name"], {}).get(
                                "vtable_slots") is not None), None)
            if primary:
                row["new_primary_virtuals"] = max(
                    0, cpp_slots - primary["vtable_slots"])
            elif not binary or len(binary["bases"]) == 1:
                row["new_primary_virtuals"] = cpp_slots

    byname = {row["name"]: row for row in rows}
    for row in rows:
        binary = binary_records.get(row["name"])
        layout = compiled["classes"].get(row["cpp_name"])
        if row["new_primary_virtuals"] is not None or not binary or not layout:
            continue
        if layout["bases"]:
            continue
        # Flattened facades still override the binary base's destructor.
        methods = Counter(m for m in row["primary_methods"] if m != "{dtor}")
        inherited = Counter()
        for descriptor in binary["bases"][1:]:
            base = byname.get(descriptor["name"])
            if base and descriptor["mdisp"] == 0:
                inherited |= Counter(base["primary_methods"])
        row["new_primary_virtuals"] = sum((methods - inherited).values())
    for row in rows:
        binary = binary_records.get(row["name"])
        if not binary or not row["new_primary_virtuals"]:
            continue
        primary_base = rtti.direct_base(binary)
        base_table = rtti.primary(primary_base) if primary_base else None
        required = (len(base_table["slots"]) if base_table else 0) + row[
            "new_primary_virtuals"]
        row["required_primary_slots"] = required
        if row["header_vtable_slots"] < required:
            fail(row, "vtable", row["header_vtable_slots"], required,
                 f"New virtuals in {row['name']} require at least {required} slots "
                 "after the binary primary base")
        for descriptor in binary["bases"][1:]:
            if descriptor["mdisp"] != 0 or descriptor["pdisp"] != -1:
                continue
            base = byname.get(descriptor["name"])
            if not base or base["header_vtable_slots"] is None or base[
                    "binary_vtable_slots"] is None:
                continue
            if base["header_vtable_slots"] < base["binary_vtable_slots"]:
                fail(base, "vtable", base["header_vtable_slots"],
                     base["binary_vtable_slots"],
                     f"Partial base shifts new virtuals declared by {row['name']}")
                if row["vtable_status"] != "FAIL":
                    fail(row, "vtable", row["header_vtable_slots"], required,
                         f"New virtuals follow partial modeled base {base['name']}")
    for row in rows:
        states = (row["size_status"], row["vtable_status"])
        row["status"] = next((state for state in
                              ("FAIL", "STUB", "PARTIAL", "PASS")
                              if state in states), "UNKNOWN")
    counts = {state: sum(row["status"] == state for row in rows)
              for state in GATE_STATES}
    if proof_report["errors"]:
        proof_report["status"] = "ERROR"
    return {"version": rtti.image.version, "counts": counts, "classes": rows,
            "skipped_headers": compiled["skipped_headers"],
            "compiler_exit_code": compiled["compiler_exit_code"],
            "compiler_errors": compiled["errors"],
            "compiler_report": compiled["report"],
            "evidence_check": proof_report,
            "hkclass_summary": {
                "class_count": len(reflection.by_name),
                "record_count": len(reflection.records),
                "errors": reflection.data["errors"]}}
