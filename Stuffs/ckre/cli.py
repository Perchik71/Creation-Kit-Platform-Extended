"""Command-line JSON contracts and real-input regression checks."""

import argparse
from collections import Counter
import json
from pathlib import Path
import subprocess
import sys

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
    for name in ("rtti", "vtable", "xversion", "ctor", "strings-in", "xrefs"):
        command = commands.add_parser(name)
        common(command)
        command.add_argument("target", nargs="?" if name in ("rtti", "xrefs", "ctor") else None)
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
                                "TESFullName.h"])
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
    return {"status": "PASS" if all(c["status"] == "PASS" for c in checks) else "FAIL",
            "counts": dict(Counter(c["status"] for c in checks)), "checks": checks}


def run(args, config):
    versions = list(VERSIONS) if args.version == "all" else [args.version]
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
            if args.command in ("vtable", "xversion") or (
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
                                "bounds": image.bounds(rva),
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
                exit_code |= int(result["counts"].get("FAIL", 0) > 0)
            elif args.command == "coverage":
                from .coverage import build
                if version != "246":
                    raise ValueError("coverage requires --version 246")
                result = build(config, rtti, context.ctors, args.output)
                results.append({"totals": result["totals"],
                                "layout_counts": result["layout_counts"],
                                "next_targets": result["next_targets"][:20],
                                "output": str(args.output)})
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
