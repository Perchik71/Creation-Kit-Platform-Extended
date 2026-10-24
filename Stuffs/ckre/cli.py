"""Command-line JSON contracts and real-input regression checks."""

import argparse
from collections import Counter
import json
from pathlib import Path
import subprocess
import sys
import tempfile

from .addrlib import AddressLibrary, resolve
from .config import Config, HEADERS, HOME, ROOT, VERSIONS
from .ctor import Constructors
from .disasm import Disasm
from .pe import Image
from .rtti import RTTI


def number(value):
    try:
        return int(value, 0)
    except ValueError:
        return int(value, 16)


def common(parser):
    parser.add_argument("--data", default=argparse.SUPPRESS,
                        help="Directory containing the original binary inputs")
    parser.add_argument("--json", action="store_true", default=argparse.SUPPRESS)
    parser.add_argument("--version", choices=[*VERSIONS, "all"],
                        default=argparse.SUPPRESS)


def parser():
    root = argparse.ArgumentParser(prog="ckre")
    root.set_defaults(data=None, json=False, version="246")
    common(root)
    commands = root.add_subparsers(dest="command", required=True)
    for name in ("rtti", "vtable", "xversion", "ctor", "strings-in", "xrefs", "hkclass"):
        command = commands.add_parser(name)
        common(command)
        command.add_argument("target", nargs="?" if name in (
            "rtti", "xrefs", "ctor", "hkclass") else None)
        if name == "ctor":
            command.add_argument("--registry", action="store_true",
                                 help="Include the established 246 factory registry")
        if name == "xrefs":
            command.add_argument("--slot-offset", type=number,
                                 help="Include virtual calls with this byte offset")
    command = commands.add_parser("id")
    common(command)
    command.add_argument("target", nargs="?", type=number)
    command.add_argument("--rva", type=number)
    command = commands.add_parser("resolve-ids")
    common(command)
    command.add_argument("file", type=Path)
    command = commands.add_parser("layout")
    common(command)
    nested = command.add_subparsers(dest="layout_command", required=True)
    gate = nested.add_parser("check")
    common(gate)
    gate.add_argument("--headers", nargs="+")
    command = commands.add_parser("evidence")
    common(command)
    command.set_defaults(version="all")
    nested = command.add_subparsers(dest="evidence_command", required=True)
    common(nested.add_parser("check"))
    command = commands.add_parser("coverage")
    common(command)
    command.add_argument("--output", type=Path, default=HOME)
    command = commands.add_parser("selftest")
    common(command)
    return root


class Context:
    def __init__(self, config, version):
        self.image = Image(config, version)
        self.rtti = RTTI(self.image)
        self.disasm = Disasm(self.image)
        self.ctors = Constructors(self.rtti, self.disasm)

    def close(self):
        self.image.close()


def selected_headers(values):
    if not values:
        return None
    result = []
    for value in values:
        path = Path(value)
        if not path.is_file():
            path = ROOT / value
        if not path.is_file():
            candidates = list(HEADERS.rglob(value))
            if len(candidates) != 1:
                raise ValueError(f"Header not found or ambiguous: {value}")
            path = candidates[0]
        if path.is_dir():
            result.extend(sorted(path.rglob("*.h")))
        else:
            result.append(path.resolve())
    return result


def selftest(config):
    from .layout import check
    checks = []

    def record(name, passed, detail):
        checks.append({"name": name, "status": "PASS" if passed else "FAIL",
                       "detail": detail})

    expected = {"162": 0x388C388, "982": 0x2F83448,
                "137": 0x300DA90, "246": 0x301ED80}
    for version in VERSIONS:
        context = Context(config, version)
        try:
            table = context.rtti.primary(context.rtti.get("TESForm"))
            record(f"TESForm {version}", bool(table and
                   len(table["slots"]) == 124 and table["rva"] == expected[version]),
                   {"slots": len(table["slots"]) if table else None,
                    "rva": table["rva"] if table else None})
            if version == "246":
                from .hkclass import Reflection
                reflection = Reflection(context.ctors)
                referenced = reflection.get("hkReferencedObject")
                record("Havok reflected hkReferencedObject",
                       bool(referenced and referenced["size"] == 0x10 and
                            referenced["parent"] == "hkBaseObject" and
                            any(member["offset"] == 8 and member["type"] == "UINT32"
                                and member["name"] == "memSizeAndRefCount"
                                for member in referenced["members"]) and
                            not reflection.data["errors"]), referenced)
                public = context.rtti.publics.byname.get("??_7TESForm@@6B@")
                record("PDB decimal section offsets", public == expected[version],
                       {"TESForm_vtable_rva": public})
                registry = context.ctors.factory_registry()
                record("Form factory registry", registry["status"] == "PASS" and
                       registry["slot_count"] == 159 and
                       registry["registration_count"] == 138, {
                           k: registry[k] for k in (
                               "registry_rva", "slot_count", "registration_count",
                               "anchor_checks")})
            factories = context.ctors.factory_allocations("BGSLayer")
            allocations = [a for f in factories for a in f["allocations"]]
            record(f"BGSLayer factory {version}",
                   sorted({a["size"] for a in allocations}) == [0x60], factories)
        finally:
            context.close()
    names = ["IDs::BGSRenderWindow::Constructor", "IDs::Sky::Constructor"]
    resolved = resolve(config, HEADERS / "IDs.h", list(VERSIONS), names)
    for name in names:
        row = next((r for r in resolved if r["name"] == name), None)
        passed = bool(row and not row["body_mismatch"] and
                      len(row["versions"]) == 4 and
                      all(v["status"] == "PASS" for v in row["versions"].values()))
        record(name, passed, row)
    expected_sizes = {"TESForm": 0x28, "BGSRenderWindow": 0x180,
                      "BGSRenderWindowCamera": 0x88, "NiCamera": 0x1A0,
                      "Sky": 0x448, "BSGraphics::RendererData": 0x25D0,
                      "BSGraphics::State": 0x3C0, "TESObjectACTI": 0x1E8,
                      "TESFullName": 0x18}
    headers = selected_headers(["TESForm.h", "BGSRenderWindow.h",
                                "BGSRenderWindowCamera.h", "NiCamera.h",
                                "Sky.h", "BSGraphicsRenderer.h", "BSGraphicsState.h",
                                "TESObjectACTI.h", "TESObjectREFR.h",
                                "TESFullName.h", "NiObjectNET.h", "BGSLayer.h"])
    context = Context(config, "246")
    try:
        gate = check(config, context.rtti, context.ctors, headers)
        for name, size in expected_sizes.items():
            row = next((r for r in gate["classes"] if r["name"] == name), None)
            passed = bool(row and row["header_size"] == size and
                          row["status"] != "FAIL" and gate["compiler_exit_code"] == 0)
            if name == "NiCamera":
                passed = passed and row["size_status"] == "PASS" and row[
                    "vtable_status"] in ("PASS", "PARTIAL")
            if name == "TESFullName":
                passed = passed and row["size_status"] == "PASS" and row[
                    "embedded_footprint"]["size"] == 0x18 and row[
                    "binary_sizes"] == [0x20] and bool(row[
                        "possible_composite_allocations"])
            record(f"layout {name}", passed, row)
        aliases = [next((r for r in gate["classes"] if r["name"] == name), None)
                   for name in ("TESObjectREFR", "TESObjectREFR_Original",
                                "TESObjectREFR_Extremly_NG", "TESObjectREFR_Extremly")]
        record("REFR template aliases", all(
            row and row["header_size"] == 0x130 and row["size_status"] == "PASS"
            and row["status"] != "FAIL" for row in aliases), aliases)
        from .evidence import evaluate, verify
        committed = verify(config, images={"246": context.image})
        record("Committed size evidence verifies in every declared build",
               committed["status"] == "PASS", committed)
        layer = next((r for r in gate["classes"] if r["name"] == "BGSLayer"), None)
        record("Exact committed allocation evidence", bool(
            layer and layer["header_size"] == 0x60 and
            layer["size_status"] == "PASS" and
            layer["size_evidence_status"] == "PASS (evidence)"), layer)
        net = next(r for r in gate["classes"] if r["name"] == "NiObjectNET")
        bounded = evaluate([entry for entry in net["size_evidence"]
                            if entry["kind"] in ("lower_bound", "upper_bound")],
                           "246", net["header_size"], net["header_alignment"])
        for kind, invalid_size in (("lower_bound", 0x20), ("upper_bound", 0x30)):
            evidence = [entry for entry in net["size_evidence"] if entry["kind"] == kind]
            assessed = evaluate(evidence, "246", net["header_size"],
                                net["header_alignment"])
            mismatch = evaluate(evidence, "246", invalid_size,
                                net["header_alignment"])
            passed = bool(evidence and assessed["status"] == "UNKNOWN" and
                          mismatch["status"] == "FAIL" and
                          net["size_status"] == "PASS" and
                          net["header_size"] == 0x28 and
                          bounded.get("label") == "PASS (bounded)")
            record(f"Committed {kind} and bounded size",
                   passed, {"class": net["name"], "alignment": net["header_alignment"],
                            "matching": assessed, "mismatch": mismatch})
        with tempfile.TemporaryDirectory(prefix="bad-evidence-", dir=config.cache) as directory:
            path = Path(directory) / "invalid.toml"
            path.write_text(
                '[[evidence]]\nclass = "NiObjectNET"\nsize = 0x28\n'
                'kind = "lower_bound"\nversions = ["246"]\n'
                'proof = [{version = "246", rva = 0x2497DF5, expect = "ud2"}]\n',
                "utf-8")
            invalid = verify(config, directory=Path(directory),
                             images={"246": context.image})
        record("Unverifiable instruction evidence is never trusted",
               invalid["status"] == "ERROR" and bool(invalid["errors"]) and
               all(entry["status"] != "PASS" for entry in invalid["entries"]), invalid)
        sky = context.ctors.describe("Sky")
        sizes = sorted({a["size"] for c in sky["candidates"]
                        for a in c["allocations"] if a["confidence"] in
                        ("named", "binary_diagnostic")})
        record("Sky allocation", 0x448 in sizes, sky)
        derived_calls = {"NiExtraData": 0xF98FBF, "NiObjectNET": 0x5733BA,
                         "TESFormIDListView": 0xBDA267}
        ownership = []
        for name, site in derived_calls.items():
            description = context.ctors.describe(name)
            callers = [ref["site_rva"] for c in description["candidates"]
                       for ref in context.ctors.callers(c["rva"])]
            accepted = [a["constructor_call_rva"]
                        for c in description["candidates"] for a in c["allocations"]]
            ownership.append({"class": name, "derived_call_rva": site,
                              "found": site in callers, "accepted": site in accepted})
        record("Derived allocations do not size base objects",
               all(row["found"] and not row["accepted"] for row in ownership),
               ownership)
    finally:
        context.close()
    regression_checks(config, record)
    virtual_slot_regression(config, record)
    return {"status": "PASS" if all(c["status"] == "PASS" for c in checks) else "FAIL",
            "counts": dict(Counter(c["status"] for c in checks)), "checks": checks}


def regression_checks(config, record):
    from .evidence import evaluate, verify
    from .layout import (bind_alias_layouts, check, compile_inventory,
                         evidence_identities, normalize_type, object_alias, source_inventory)
    with tempfile.TemporaryDirectory(prefix="leaf-regression-", dir=config.cache) as work:
        path = Path(work) / "IDs.h"
        path.write_text("namespace IDs { namespace Leaf { "
                        "inline constexpr Common::ID Constructor{199447}; } }", "utf-8")
        resolved = resolve(config, path, list(VERSIONS))[0]
    leaves, anchors = [], []
    for version in VERSIONS:
        context = Context(config, version)
        try:
            rva = AddressLibrary(config, version).ids[199447]
            analysis = context.ctors.analyze(rva)
            refs = context.disasm.xrefs(rva)
            interior = context.disasm.function(rva + 2)
            try:
                context.ctors.describe(hex(rva + 2))
            except ValueError:
                interior_rejected = True
            else:
                interior_rejected = False
            row = resolved["versions"][version]
            leaves.append({
                "version": version, "rva": rva,
                "passed": not context.image.bounds(rva) and
                row["status"] == "PASS" and row["fingerprint"]["size"] == 0x23 and
                row["fingerprint"]["bounds_complete"] and
                {s["offset"] for s in analysis["stores"]} == {0, 8, 0x10, 0x12, 0x18, 0x20} and
                any(ref.get("function_source") == "inferred" for ref in refs) and
                any(c["rva"] == rva for c in
                    context.ctors.candidates("DialogExtraPreviewControl")),
                "interior_rejected": bool(
                    interior_rejected and interior and interior["start"] == rva),
                "function": analysis["function"]})
            if version == "246":
                for kind in ("direct_call", "vtable_slot"):
                    leaf = next((f for f in context.disasm._leaves.values()
                                 if any(p["kind"] == kind for p in f["provenance"])
                                 and not any(p["kind"] == "aligned_padding"
                                             for p in f["provenance"])), None)
                    anchors.append({
                        "kind": kind, "function": leaf,
                        "passed": bool(leaf and not context.image.bounds(leaf["start"])
                                       and context.disasm.instructions(leaf["start"]))})
        finally:
            context.close()
    record("No-unwind ID199447 resolves and exposes constructor stores/xrefs in all builds",
           not resolved["body_mismatch"] and all(r["passed"] for r in leaves), leaves)
    record("Inferred leaf bounds reject arbitrary interior function starts",
           all(r["interior_rejected"] for r in leaves), leaves)
    record("Non-padding direct-call and vtable targets prove leaf starts",
           all(row["passed"] for row in anchors), anchors)

    headers = selected_headers(["TESDataHandler.h", "hkBaseTypes.h",
                                "hkBaseObject.h", "hkGeometryProcessing.h", "BSTHashMap.h"])
    map_type = ("CKPE::Fallout4::EditorAPI::BSTHashMap<"
                "CKPE::Fallout4::EditorAPI::BSFixedString,void*>")
    inventory = source_inventory(headers, instances=[
        "hkEnum<hkReferencedObject&,std::uint8_t>", map_type])
    compiled = compile_inventory(config, headers, inventory)
    bind_alias_layouts(inventory, compiled["classes"], compiled.get("alias_bindings"))
    expected = {"TESFormArray": 0x18, "hkgpIndexedMeshStorage": 0x40,
                "hkEnum<hkReferencedObject&,std::uint8_t>": 1,
                map_type.removeprefix("CKPE::Fallout4::EditorAPI::"): 0x38}
    bindings = {}
    for name, size in expected.items():
        source = next((s for s in inventory if s["name"] == name), None)
        layout = compiled["classes"].get(source["cpp_name"]) if source else None
        bindings[name] = {"layout": layout, "passed": bool(
            layout and layout["size"] == size and layout.get("instantiated_cpp_name") and
            (source.get("evidence_class") != map_type or
             normalize_type(map_type) in evidence_identities(source)))}
    record("Pointer/reference template arguments bind actual MSVC instantiations",
           compiled["compiler_exit_code"] == 0 and
           all(item["passed"] for item in bindings.values()) and
           not object_alias("hkArray<hkReferencedObject*>*") and
           not object_alias("hkArray<hkReferencedObject*>&"), bindings)

    # One real class deliberately has two sizes. Exercise the complete gate with
    # isolated proofs: the modern facade must FAIL against 162, not conflict
    # with, or accidentally consume, the other builds' exact evidence.
    sites = {"162": 0x462EA5, "982": 0x5582C5,
             "137": 0x571635, "246": 0x573B05}
    with tempfile.TemporaryDirectory(prefix="version-evidence-", dir=config.cache) as work:
        path = Path(work) / "versions.toml"
        blocks = []
        for versions, size in ((["162"], 0x210), (["982", "137", "246"], 0x220)):
            proofs = ",\n".join(
                f'{{version = "{v}", rva = 0x{sites[v]:X}, '
                f'expect = "mov edx, 0x{size:x}"}}' for v in versions)
            blocks.append(
                '[[evidence]]\nclass = "NiWindow"\n'
                f'size = 0x{size:X}\nkind = "exact"\n'
                f'versions = {json.dumps(versions)}\nproof = [{proofs}]\n'
                'note = "NiWindow own allocation immediately before its constructor."\n')
        path.write_text("\n".join(blocks), "utf-8")
        proofs = verify(config, directory=Path(work))
        results = []
        for version in VERSIONS:
            context = Context(config, version)
            try:
                gate = check(config, context.rtti, context.ctors,
                             selected_headers(["NiTypes.h"]), allocations=False,
                             evidence_directory=Path(work))
                row = next(r for r in gate["classes"] if r["name"] == "NiWindow")
                correct = 0x210 if version == "162" else 0x220
                evaluated = evaluate(proofs["entries"], version, correct)
                results.append({
                    "version": version, "header_size": row["header_size"],
                    "evidence_status": row["size_evidence_status"],
                    "expected_binary_size": correct,
                    "passed": gate["compiler_exit_code"] == 0 and
                    evaluated["status"] == "PASS" and
                    {e["size"] for e in row["size_evidence"]} == {correct} and
                    row["size_evidence_status"] ==
                    ("FAIL" if version == "162" else "PASS (evidence)")})
            finally:
                context.close()
    record("NiWindow disjoint-version exact sizes are selected by the layout gate",
           proofs["status"] == "PASS" and all(row["passed"] for row in results), results)


def virtual_slot_regression(config, record):
    from .layout import (annotation_check, compile_headers, compiler_environment,
                         parse_layout, source_inventory)
    header = HEADERS / "N/NiObject.h"
    source = next(row for row in source_inventory([header]) if row["name"] == "NiObject")
    text = header.read_text("utf-8-sig")
    import re
    pattern = (r"(?m)^[ \t]*using Templates::NiObjectT<[^;\n]+>::"
               r"(?:CreateClone|LinkObject|RegisterStreamables);[ \t]*\r?\n")
    usings = re.findall(pattern, text)
    modified = re.sub(pattern, "", text)
    at = modified.index("/* 02 */", modified.index("class NiObject :"))
    modified = modified[:at] + "\n".join(usings) + "\n" + modified[at:]
    environment = compiler_environment()
    with tempfile.TemporaryDirectory(prefix="ckre-slot-regression-") as temporary:
        directory = Path(temporary)
        good_code, good_text = compile_headers([header], directory, environment)
        bad_code, bad_text = compile_headers([], directory, environment, modified)
    good = parse_layout(good_text).get(source["cpp_name"])
    bad = parse_layout(bad_text).get(source["cpp_name"])
    good_check = annotation_check(source, good) if good else {}
    bad_check = annotation_check(source, bad) if bad else {}
    good_slots = {entry["name"]: entry["slot"] for entry in good["vtable_entries"]} if good else {}
    bad_slots = {entry["name"]: entry["slot"] for entry in bad["vtable_entries"]} if bad else {}
    passed = bool(
        len(usings) == 3 and good_code == bad_code == 0 and good and bad and
        good["vtable_slots"] == bad["vtable_slots"] == 42 and
        good_check["status"] == "PASS" and bad_check["status"] == "FAIL" and
        good_slots.get("IsTriShape") == 0x0A and good_slots.get("IsGeometry") == 8 and
        bad_slots.get("IsTriShape") == 0x0D and bad_slots.get("IsGeometry") == 0x0B)
    record("NiObject using-first ordering fails despite an unchanged virtual slot count",
           passed, {"real": good_check, "using_first": bad_check,
                    "real_slots": good_slots, "using_first_slots": bad_slots,
                    "compiler_exit_codes": [good_code, bad_code]})


def run(args, config):
    versions = list(VERSIONS) if args.version == "all" else [args.version]
    if args.command == "evidence":
        from .evidence import verify
        result = verify(config, versions)
        return result, 2 if result["errors"] else 0
    if args.command == "selftest":
        result = selftest(config)
        return result, int(result["status"] != "PASS")
    if args.command == "resolve-ids":
        path = args.file if args.file.exists() else ROOT / args.file
        result = resolve(config, path, versions)
        return result, int(any(r["body_mismatch"] or any(
            v["status"] in ("MISSING", "FAIL") for v in r["versions"].values())
            for r in result))
    if args.command == "id":
        if (args.target is None) == (args.rva is None):
            raise ValueError("Supply exactly one ID or --rva")
        result = []
        for version in versions:
            image = Image(config, version)
            try:
                library = AddressLibrary(config, version)
                if args.rva is not None:
                    result.append({**image.address(args.rva),
                                   "ids": library.reverse.get(args.rva, [])})
                else:
                    rva = library.ids.get(args.target)
                    result.append({"id": args.target, "version": version,
                                   "address": image.address(rva) if rva else None})
            finally:
                image.close()
        return result, int(any(r.get("address", True) is None for r in result))
    if args.command == "xversion":
        versions = list(VERSIONS)
    results, exit_code = [], 0
    for version in versions:
        context = Context(config, version)
        try:
            image, rtti = context.image, context.rtti
            if args.command == "hkclass":
                from .hkclass import Reflection
                reflection = Reflection(context.ctors)
                results.append(reflection.describe(args.target))
                if reflection.data["errors"]:
                    exit_code = 2
            elif args.command in ("vtable", "xversion") or (
                    args.command == "rtti" and args.target):
                description = rtti.describe(args.target, context.disasm)
                if args.command == "xversion":
                    description = {
                        "version": version, "name": description["name"],
                        "bases": [{k: b[k] for k in ("name", "mdisp", "pdisp", "vdisp")}
                                  for b in description["bases"]],
                        "vtables": [{
                            "offset": table["offset"],
                            "rva": table["rva"], "slot_count": table["slot_count"],
                            "override_pattern": [s["override"] for s in table["slots"]],
                        } for table in description["vtables"]],
                    }
                results.append(description)
            elif args.command == "rtti":
                results.append({"version": version, "count": len(rtti.classes),
                                "classes": rtti.classes})
            elif args.command == "ctor":
                if not args.target and not args.registry:
                    raise ValueError("Supply a class/RVA or --registry")
                result = context.ctors.describe(args.target) if args.target else {}
                if args.registry:
                    result["factory_registry"] = context.ctors.factory_registry()
                results.append(result)
            elif args.command == "strings-in":
                rva = number(args.target)
                results.append({"address": image.address(rva),
                                "bounds": context.disasm.bounds(rva),
                                "function": context.disasm.function(rva),
                                "strings": context.disasm.strings(rva)})
            elif args.command == "xrefs":
                rva = number(args.target) if args.target else 0
                results.append({"address": image.address(rva),
                                "slot_offset": args.slot_offset,
                                "references": context.disasm.xrefs(
                                    rva, args.slot_offset),
                                "virtual_call_note": "Offset-only matches do not "
                                "prove the receiver's dynamic type."})
            elif args.command == "layout":
                from .layout import check
                result = check(config, rtti, context.ctors,
                               selected_headers(args.headers))
                results.append(result)
                exit_code = max(exit_code, 2 if result["evidence_check"]["errors"]
                                else int(result["counts"].get("FAIL", 0) > 0))
            elif args.command == "coverage":
                from .coverage import build
                if version != "246":
                    raise ValueError("coverage requires --version 246")
                result = build(config, rtti, context.ctors, args.output)
                results.append({"totals": result["totals"],
                                "layout_counts": result["layout_counts"],
                                "next_targets": result["next_targets"][:20],
                                "output": str(args.output),
                                "evidence_check": result["evidence_check"]})
                if result["evidence_check"]["errors"]:
                    exit_code = 2
        finally:
            context.close()
    if args.command == "xversion":
        patterns = [r["vtables"] for r in results]
        signatures = [[(t["offset"], t["slot_count"], t["override_pattern"])
                       for t in tables] for tables in patterns]
        return {"class": args.target, "versions": results,
                "layout_differences": any(s != signatures[0]
                                          for s in signatures[1:]) or any(
                    r["bases"] != results[0]["bases"] for r in results[1:])}, 0
    return (results[0] if len(results) == 1 else results), exit_code


def main(argv=None):
    args = parser().parse_args(argv)
    try:
        result, code = run(args, Config(args.data))
    except (OSError, ValueError, RuntimeError, subprocess.SubprocessError) as error:
        if args.json:
            print(json.dumps({"error": str(error)}, indent=2))
        else:
            print(f"ckre: {error}", file=sys.stderr)
        return 2
    print(json.dumps(result, indent=2))
    return code
