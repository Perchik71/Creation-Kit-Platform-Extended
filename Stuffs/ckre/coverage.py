"""Binary/source coverage with explicit verification provenance."""

from collections import Counter
import json
from pathlib import Path
import re

from .config import HEADERS, ROOT, atomic_write
from .layout import check, source_inventory


def noise(name):
    return bool(re.search(
        r"(^|::)(std|stdext|Concurrency|boost|ATL|WRL|wil|Microsoft|DirectX|"
        r"winrt|type_info)(::|$)|<lambda|lambda_|anonymous namespace|"
        r"`|\.\?A|\?\$|(^|::)\?A0x|^Q[A-Z]|^C[A-Z]|^_com_|^tag|^__|^I[A-Z]|"
        r"^(fbxsdk|Alembic|Json)(_|::|$)",
        name))


def family(name):
    if name.startswith(("TES", "BGS")):
        return "TES/BGS"
    if name.startswith(("BS", "Ni")):
        return "BS/Ni"
    if re.search(r"Window|Dialog|Dlg|View|Editor|Render", name, re.I):
        return "Editor windows/dialogs"
    return "Other"


def build(config, rtti, constructors, output):
    gate = check(config, rtti, constructors)
    inventory = source_inventory()
    modeled = {}
    for source in inventory:
        modeled.setdefault(source["name"], source)
    layouts = {row["name"]: row for row in gate["classes"]}
    header_names = {}
    for path in sorted(HEADERS.rglob("*.h")):
        header_names.setdefault(path.stem, path)
    files = list((ROOT / "CKPE.Fallout4/Src").rglob("*.cpp"))
    files += list((ROOT / "CKPE.Fallout4/Src").rglob("*.h"))
    usages = {}
    for path in sorted(files):
        text = path.read_text("utf-8-sig", errors="replace")
        text = re.sub(r"/\*.*?\*/|//[^\n]*", "", text, flags=re.S)
        names = set()
        for token in re.findall(r"\b[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*", text):
            parts = token.split("::")
            names.update("::".join(parts[index:]) for index in range(len(parts)))
        for name in names:
            usages.setdefault(name, []).append(path.relative_to(ROOT).as_posix())
    descendants = Counter()
    for record in rtti.classes:
        descendants.update({b["name"] for b in record["bases"][1:]})
    rows, ignored = [], []
    for record in rtti.classes:
        name = record["name"]
        basename = name.split("::")[-1]
        source = modeled.get(name)
        if source is None:
            matching = header_names.get(basename)
            if matching:
                source = {"header": matching.relative_to(ROOT).as_posix()}
        layout = layouts.get(source.get("name")) if source else None
        table = rtti.primary(record)
        row = {
            "name": name, "family": family(name), "modeled": source is not None,
            "header": source["header"] if source else None,
            "size_verified": bool(layout and "size" in layout.get("verified", [])
                                  and source.get("assert_size") == layout["header_size"]),
            "assert_size": source.get("assert_size") if source else None,
            "vtable_verified": bool(layout and "vtable" in layout.get("verified", [])),
            "binary_vtable_slots": len(table["slots"]) if table else None,
            "unknown_bytes": source.get("unknown_bytes", 0) if source else None,
            "unk_scalars": source.get("unk_scalars", 0) if source else None,
            "sub_virtuals": source.get("sub_virtuals", 0) if source else None,
            "patch_usage": len(usages.get(name, [])),
            "patch_files": usages.get(name, []),
            "descendant_count": descendants[name],
            "layout_status": layout["status"] if layout else "UNKNOWN",
            "size_status": layout["size_status"] if layout else "UNKNOWN",
            "vtable_status": layout["vtable_status"] if layout else "UNKNOWN",
        }
        (ignored if noise(name) else rows).append(row)
    targets = sorted((r for r in rows if not r["modeled"] or
                      not r["size_verified"] or not r["vtable_verified"]),
                     key=lambda r: (-r["patch_usage"], -r["descendant_count"],
                                    -(r["unknown_bytes"] or 0), r["name"]))
    totals = {
        "rtti_classes": len(rtti.classes), "game_classes": len(rows),
        "noise_classes": len(ignored),
        "modeled": sum(r["modeled"] for r in rows),
        "modeled_including_noise": sum(r["modeled"] for r in rows + ignored),
        "source_definitions": len(inventory),
        "size_verified": sum(r["size_verified"] for r in rows),
        "vtable_verified": sum(r["vtable_verified"] for r in rows),
        "unknown_bytes": sum(r["unknown_bytes"] or 0 for r in rows),
        "unk_scalars": sum(r["unk_scalars"] or 0 for r in rows),
        "sub_virtuals": sum(r["sub_virtuals"] or 0 for r in rows),
    }
    result = {"version": "246", "totals": totals, "classes": rows,
              "noise": ignored, "next_targets": targets,
              "layout_counts": gate["counts"],
              "skipped_headers": gate["skipped_headers"]}
    output = Path(output)
    atomic_write(output / "coverage.json", json.dumps(result, indent=2))
    markdown = ["# EditorAPI coverage", "", "## Totals", "",
                "| Metric | Count |", "|---|---:|"]
    markdown += [f"| {key} | {value} |" for key, value in totals.items()]
    for group in ("TES/BGS", "BS/Ni", "Editor windows/dialogs", "Other"):
        markdown += ["", f"## {group}", "",
                     "| Class | Status | Modeled | Size | Vtable | Unknown bytes | Unk | Sub |",
                     "|---|---|---|---|---|---:|---:|---:|"]
        for row in rows:
            if row["family"] == group:
                markdown.append(
                    f"| {row['name']} | {row['layout_status']} | {row['modeled']} | "
                    f"{row['size_status']} | {row['vtable_status']} | {row['unknown_bytes']} | "
                    f"{row['unk_scalars']} | {row['sub_virtuals']} |")
    markdown += ["", "## Next targets", "",
                 "| Class | Patch files | Descendants | Unknown bytes |",
                 "|---|---:|---:|---:|"]
    markdown += [
        f"| {r['name']} | {r['patch_usage']} | {r['descendant_count']} | "
        f"{r['unknown_bytes']} |" for r in targets[:50]]
    markdown += ["", "Noise is retained separately in coverage.json.",
                 "Size verification requires matching binary allocation evidence.",
                 "A static_assert alone is not binary verification.", ""]
    atomic_write(output / "coverage.md", "\n".join(markdown))
    return result
