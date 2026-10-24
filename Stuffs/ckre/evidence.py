"""Reviewed size claims pinned to mechanically checked binary instructions."""

from collections import Counter
import re
import tomllib

from capstone.x86 import X86_OP_IMM, X86_OP_MEM

from .config import HOME, VERSIONS
from .disasm import Disasm
from .pe import Image

KINDS = ("exact", "lower_bound", "upper_bound")


def integer(value, label):
    if isinstance(value, bool) or not isinstance(value, int):
        raise ValueError(f"{label} must be a TOML integer")
    return value


def expectation(value):
    if isinstance(value, str) and value.strip():
        return {"text": value}
    allowed = {"text", "mnemonic", "immediate", "scale", "displacement"}
    if not isinstance(value, dict) or not value or set(value) - allowed:
        raise ValueError("expect must be instruction text or a supported operand table")
    if not set(value) & (allowed - {"mnemonic"}):
        raise ValueError("expect requires text, immediate, scale or displacement")
    for key, item in value.items():
        if key in ("text", "mnemonic"):
            if not isinstance(item, str) or not item.strip():
                raise ValueError(f"expect.{key} must be nonempty text")
        else:
            integer(item, "expect." + key)
    return value


def matches(ins, expect):
    normalize = lambda text: re.sub(r"\s+", " ", text.strip()).lower()
    text = f"{ins.mnemonic} {ins.op_str}".strip()
    if "text" in expect and normalize(text) != normalize(expect["text"]):
        return False
    if "mnemonic" in expect and normalize(ins.mnemonic) != normalize(expect["mnemonic"]):
        return False
    if "immediate" in expect and not any(
            op.type == X86_OP_IMM and op.imm == expect["immediate"]
            for op in ins.operands):
        return False
    memory = {key: expect[key] for key in ("scale", "displacement") if key in expect}
    return not memory or any(
        op.type == X86_OP_MEM and all(
            getattr(op.mem, "disp" if key == "displacement" else key) == value
            for key, value in memory.items()) for op in ins.operands)


def verify(config, versions=None, images=None, directory=None):
    selected = set(versions or VERSIONS)
    images = dict(images or {})
    owned, decoders, functions = [], {}, {}
    rows, errors = [], []
    directory = directory if directory is not None else HOME / "evidence"

    def error(file, index, name, message):
        detail = {"file": file, "entry": index, "class": name, "error": message}
        errors.append(detail)
        return detail

    def instruction(version, rva):
        if version not in images:
            image = Image(config, version)
            images[version] = image
            owned.append(image)
        image = images[version]
        if not image.is_code(rva):
            raise ValueError(f"RVA 0x{rva:X} is not in .text")
        if version not in decoders:
            decoders[version] = Disasm(image)
        decoder = decoders[version]
        bounds = image.bounds(rva)
        if bounds:
            key = (version, bounds[0])
            if key not in functions:
                functions[key] = {ins.address: ins for ins in
                                  decoder.instructions(bounds[0])}
            ins = functions[key].get(rva)
        else:
            section = image.section(rva)
            length = min(15, section["rva"] + section["raw_size"] - rva)
            ins = next(decoder.md.disasm(image.read(rva, length), rva, count=1), None)
        if ins is None:
            raise ValueError(f"RVA 0x{rva:X} is not a decoded instruction boundary")
        return ins

    try:
        for path in sorted(directory.glob("*.toml")):
            file = path.name
            try:
                document = tomllib.loads(path.read_text("utf-8-sig"))
                if set(document) != {"evidence"} or not isinstance(
                        document["evidence"], list) or not document["evidence"]:
                    raise ValueError("Use one or more [[evidence]] tables")
            except (OSError, ValueError) as exc:
                error(file, None, None, str(exc))
                continue
            for index, entry in enumerate(document["evidence"], 1):
                name = entry.get("class") if isinstance(entry, dict) else None
                try:
                    if not isinstance(entry, dict) or set(entry) - {
                            "class", "size", "kind", "versions", "proof", "note"}:
                        raise ValueError("Unknown evidence entry fields")
                    if not isinstance(name, str) or not name.strip():
                        raise ValueError("class must be nonempty text")
                    size = integer(entry.get("size"), "size")
                    if size <= 0 or entry.get("kind") not in KINDS:
                        raise ValueError("size must be positive and kind must be supported")
                    versions_value = entry.get("versions")
                    if not isinstance(versions_value, list) or not versions_value:
                        raise ValueError("versions must be a nonempty list")
                    declared = [str(value) for value in versions_value]
                    if len(set(declared)) != len(declared) or set(declared) - VERSIONS.keys():
                        raise ValueError("versions must be unique selectors: 162, 982, 137, 246")
                    if not isinstance(entry.get("note", ""), str):
                        raise ValueError("note must be text")
                    proofs = entry.get("proof")
                    if not isinstance(proofs, list) or not proofs:
                        raise ValueError("proof must be a nonempty list")
                    for proof in proofs:
                        if not isinstance(proof, dict) or set(proof) != {
                                "version", "rva", "expect"}:
                            raise ValueError("Each proof requires version, rva and expect")
                        if str(proof["version"]) not in declared:
                            raise ValueError("Proof version is not declared by its entry")
                        if integer(proof["rva"], "proof.rva") < 0:
                            raise ValueError("proof.rva must be nonnegative")
                        expectation(proof["expect"])
                    if set(declared) != {str(p["version"]) for p in proofs}:
                        raise ValueError("Every declared version needs its own proof")
                except ValueError as exc:
                    error(file, index, name, str(exc))
                    continue
                if not selected.intersection(declared):
                    continue
                row = {**entry, "versions": declared, "file": file, "entry": index,
                       "status": "PASS", "proof": []}
                rows.append(row)
                for proof in proofs:
                    result = {**proof, "version": str(proof["version"]), "status": "PASS"}
                    try:
                        ins = instruction(result["version"], proof["rva"])
                        result["instruction"] = f"{ins.mnemonic} {ins.op_str}".strip()
                        result["bytes"] = ins.bytes.hex()
                        if not matches(ins, expectation(proof["expect"])):
                            raise ValueError("Decoded instruction does not match expect")
                    except (OSError, ValueError) as exc:
                        result["status"] = row["status"] = "ERROR"
                        result["error"] = str(exc)
                        error(file, index, name, f"{result['version']} "
                              f"RVA 0x{proof['rva']:X}: {exc}")
                    row["proof"].append(result)
    finally:
        for image in owned:
            image.close()
    counts = dict(Counter(row["status"] for row in rows))
    return {"status": "ERROR" if errors else "PASS", "counts": counts,
            "entries": rows, "errors": errors}


def evaluate(entries, version, header_size, alignment=None):
    entries = [entry for entry in entries
               if entry["status"] == "PASS" and version in entry["versions"]]
    exact = {entry["size"] for entry in entries if entry["kind"] == "exact"}
    lower = max((entry["size"] for entry in entries
                 if entry["kind"] == "lower_bound"), default=None)
    upper = min((entry["size"] for entry in entries
                 if entry["kind"] == "upper_bound"), default=None)
    result = {"status": "UNKNOWN", "entries": entries, "lower_bound": lower,
              "upper_bound": upper, "alignment": alignment, "exact_size": None,
              "reasons": [], "errors": []}
    lo = ((lower + alignment - 1) // alignment * alignment
          if lower is not None and alignment else lower)
    hi = upper // alignment * alignment if upper is not None and alignment else upper
    result.update({"rounded_lower": lo, "rounded_upper": hi})
    bounded = bool(alignment and lo is not None and lo == hi)
    if len(exact) > 1 or (lo is not None and hi is not None and lo > hi) or any(
            (lo is not None and size < lo) or (hi is not None and size > hi)
            for size in exact):
        result["status"] = "ERROR"
        result["errors"].append("Verified evidence has incompatible exact sizes or bounds")
        return result
    size = next(iter(exact)) if exact else lo if bounded else None
    result["exact_size"] = size
    if size is not None and header_size != size:
        result["status"] = "FAIL"
        result["reasons"].append(f"Header size differs from verified exact size 0x{size:X}")
    if lower is not None and header_size < lower:
        result["status"] = "FAIL"
        result["reasons"].append(f"Header size is below verified lower bound 0x{lower:X}")
    if upper is not None and header_size > upper:
        result["status"] = "FAIL"
        result["reasons"].append(f"Header size exceeds verified upper bound 0x{upper:X}")
    if result["status"] != "FAIL" and size is not None:
        result["status"] = "PASS"
        result["label"] = "PASS (evidence)" if exact else "PASS (bounded)"
    return result
