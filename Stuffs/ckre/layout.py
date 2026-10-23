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


def source_inventory(headers=None):
    result = []
    for path in headers or sorted(HEADERS.rglob("*.h")):
        text = path.read_text("utf-8-sig", errors="replace")
        masked = re.sub(r"/\*.*?\*/|//[^\n]*|\"(?:\\.|[^\"\\])*\"",
                        lambda m: " " * len(m[0]), text, flags=re.S)
        sizes = [(m.start(), m[1].strip(), int(m[2], 0)) for m in re.finditer(
            r"static_assert\s*\(\s*sizeof\s*\(\s*([\w:]+)\s*\)"
            r"\s*==\s*(0x[0-9a-fA-F]+|\d+)", masked)]
        tokens = list(re.finditer(
            r"\bnamespace\s+([\w:]+)\s*\{|"
            r"\b(?:class|struct)\s+(?:alignas\([^)]*\)\s*)?(\w+)"
            r"(?:\s+final)?\s*(?::[^;{}]+)?\s*\{|([{}])", masked))
        stack = []
        for token in tokens:
            if token[1]:
                stack.append(("namespace", token[1], token))
            elif token[2]:
                kind = "other" if re.search(
                    r"\benum\s*$", masked[max(0, token.start() - 10):token.start()]
                ) else "class"
                stack.append((kind, token[2] if kind == "class" else "", token))
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
    return result


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


def compile_headers(headers, directory, environment):
    source = directory / "layout.cpp"
    source.write_text("\n".join(
        f'#include "{p.as_posix()}"' for p in headers), "utf-8")
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


def compile_inventory(config, headers=None):
    headers = ordered_headers(headers or sorted(HEADERS.rglob("*.h")))
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
        code, output = compile_headers(headers, directory, environment)
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
            code, output = compile_headers(good, directory, environment)
        report = config.cache / f"layout-{stamp.hexdigest()[:24]}.txt"
        atomic_write(report, output)
        result = {"classes": parse_layout(output), "skipped_headers": skipped,
                  "compiler_exit_code": code,
                  "report": report.relative_to(config.cache).as_posix(),
                  "errors": [line for line in output.splitlines()
                             if ": error " in line or ": fatal error " in line]}
        if code:
            result["classes"] = {}
        atomic_write(cache, json.dumps(result))
        return result


GATE_STATES = ("PASS", "PARTIAL", "STUB", "UNKNOWN", "FAIL")


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


def check(config, rtti, constructors, headers=None, allocations=True):
    compiled = compile_inventory(config, headers)
    inventory = source_inventory(headers)
    embedded = embedded_classes(inventory, compiled["classes"])
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
               "new_primary_virtuals": None, "reasons": [], "evidence": []}
        rows.append(row)
        if not layout:
            row["reasons"].append("Compiler did not emit this class layout")
            continue
        try:
            binary = rtti.get(source["name"])
        except ValueError:
            binary = None
        if binary:
            binary_records[source["name"]] = binary
            table = rtti.primary(binary)
            if table:
                row["binary_vtable_slots"] = len(table["slots"])
                row["evidence"].append("RTTI primary vtable")
            if allocations:
                description = constructors.describe(binary["name"])
                evidence = [a for c in description["candidates"]
                            for a in c["allocations"] if a["confidence"] in
                            ("named", "binary_diagnostic")]
                row["binary_sizes"] = sorted({a["size"] for a in evidence})
                row["allocation_evidence"] = evidence
        claimed = source["assert_size"] is not None or bool(row["embedded_by"])
        row["size_obligation"] = claimed
        if not claimed:
            row["size_status"] = "STUB" if binary else "UNKNOWN"
            row["reasons"].append("No size claim or by-value embedding")
        elif source["assert_size"] is not None and layout["size"] != source["assert_size"]:
            fail(row, "size", layout["size"], source["assert_size"],
                 "MSVC size disagrees with static_assert")
        elif row["binary_sizes"]:
            if row["binary_sizes"] == [layout["size"]]:
                row["size_status"] = "PASS"
                row["verified"].append("size")
            elif layout["size"] not in row["binary_sizes"]:
                fail(row, "size", layout["size"], row["binary_sizes"],
                     "Claimed or embedded size disagrees with binary allocation")
            else:
                row["reasons"].append("Conflicting allocation sizes")
        else:
            row["reasons"].append("No unambiguous binary allocation size")
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
    return {"version": rtti.image.version, "counts": counts, "classes": rows,
            "skipped_headers": compiled["skipped_headers"],
            "compiler_exit_code": compiled["compiler_exit_code"],
            "compiler_errors": compiled["errors"],
            "compiler_report": compiled["report"]}
