"""Address Library lookup, namespace-aware ID parsing and body comparison."""

from collections import Counter
import re

from .disasm import Disasm
from .config import VERSIONS
from .pe import Image


class AddressLibrary:
    def __init__(self, config, version):
        path = config.library(version)
        data = config.cached(path, "addrlib", lambda: self._parse(path))
        self.ids = {int(k): v for k, v in data.items()}
        self.reverse = {}
        for ident, rva in self.ids.items():
            self.reverse.setdefault(rva, []).append(ident)

    @staticmethod
    def _parse(path):
        result = {}
        with path.open(encoding="utf-8-sig") as stream:
            for number, line in enumerate(stream, 1):
                if not line.strip():
                    continue
                match = re.fullmatch(r"\s*(\d+)\s+([0-9a-fA-Fx]+)\s*", line)
                if not match:
                    raise ValueError(f"{path.name}:{number}: malformed address")
                ident, rva = int(match[1]), int(match[2], 16)
                if ident in result and result[ident] != rva:
                    raise ValueError(f"{path.name}:{number}: conflicting ID")
                result[ident] = rva
        return result


def parse_ids(path):
    text = path.read_text("utf-8-sig")
    text = re.sub(r"/\*.*?\*/|//[^\n]*", "", text, flags=re.S)
    def expand_array(match):
        entries = re.findall(r"Common::ID\s*\{([^}]+)\}", match[2])
        return "\n".join(
            f"inline constexpr Common::ID {match[1]}_slot{index}{{{values}}};"
            for index, values in enumerate(entries))
    text = re.sub(
        r"inline\s+constexpr\s+std::array<Common::ID,\s*\d+>\s+(\w+)\s*"
        r"\{((?:\s*Common::ID\s*\{[^}]+\}\s*,?)+)\s*\}\s*;",
        expand_array, text)
    tokens = re.finditer(
        r"namespace\s+([\w:]+)\s*\{|"
        r"(?:inline\s+)?constexpr\s+Common::ID\s+(\w+)\s*\{([^}]+)\}\s*;|"
        r"([{}])", text)
    stack, entries = [], []
    for match in tokens:
        if match[1]:
            stack.append(match[1])
        elif match[2]:
            values = [int(s.strip(), 0) for s in match[3].split(",")]
            if not 1 <= len(values) <= 4:
                raise ValueError(f"Invalid ID initializer: {match[0]}")
            namespace = [s for s in stack if s]
            if "IDs" in namespace:
                namespace = namespace[namespace.index("IDs"):]
            elif "IDs_VTABLE" in namespace:
                namespace = namespace[namespace.index("IDs_VTABLE"):]
            entries.append({"name": "::".join(namespace + [match[2]]),
                            "ids": values})
        elif match[4] == "{":
            stack.append(None)
        elif stack:
            stack.pop()
    return entries


def similarity(left, right):
    a, b = Counter(left), Counter(right)
    union = sum((a | b).values())
    return sum((a & b).values()) / union if union else 0.0


def resolve(config, path, versions, names=None):
    from .ctor import Constructors
    from .rtti import RTTI
    entries = parse_ids(path)
    if names:
        entries = [e for e in entries if e["name"] in names]
    result = [{"name": e["name"], "ids": e["ids"], "versions": {}}
              for e in entries]
    for version in versions:
        image = Image(config, version)
        library, disasm = AddressLibrary(config, version), Disasm(image)
        constructors = None
        try:
            for entry, record in zip(entries, result):
                index = min(list(VERSIONS).index(version), len(entry["ids"]) - 1)
                ident = entry["ids"][index]
                row = {"id": ident}
                record["versions"][version] = row
                if ident == 0:
                    row["status"] = "UNRESOLVED"
                    continue
                rva = library.ids.get(ident)
                if rva is None:
                    row["status"] = "MISSING"
                    continue
                row.update(image.address(rva))
                section = image.section(rva)
                row["section"] = section["name"] if section else None
                if image.is_code(rva):
                    function = disasm.function(rva)
                    row["function_start"] = bool(function and function["start"] == rva)
                    row["function_source"] = function["source"] if function else None
                    row["function_provenance"] = function["provenance"] if function else []
                    row["status"] = "PASS" if row["function_start"] else "FAIL"
                    row["fingerprint"] = disasm.fingerprint(rva)
                    if entry["name"].endswith("::Constructor"):
                        if constructors is None:
                            constructors = Constructors(RTTI(image), disasm)
                        stores = constructors.analyze(rva)["stores"]
                        row["fingerprint"]["field_offsets"] = sorted({
                            s["offset"] for s in stores})
                        row["fingerprint"]["vtable_classes"] = sorted({
                            s["value"]["class"] for s in stores
                            if s["value"].get("kind") == "vtable"})
                else:
                    sane = section and not section["characteristics"] & 0x20000000
                    row["status"] = "DATA" if sane else "FAIL"
        finally:
            image.close()
    for record in result:
        resolved_rows = [r for r in record["versions"].values() if r.get("section")]
        expected = None
        if resolved_rows:
            expected = "code" if resolved_rows[-1]["section"] == ".text" else "data"
        if re.search(r"(Constructor|Destructor)$", record["name"]):
            expected = "code"
        if "::VTABLE::" in record["name"] or record["name"].endswith("::VTable"):
            expected = "data"
        record["expected_kind"] = expected
        for row in resolved_rows:
            actual = "code" if row["section"] == ".text" else "data"
            row["kind_mismatch"] = actual != expected
            if row["kind_mismatch"]:
                row["status"] = "FAIL"
        fingerprints = [(v, r["fingerprint"])
                        for v, r in record["versions"].items()
                        if r.get("fingerprint", {}).get("available")]
        if fingerprints:
            baseline = fingerprints[-1][1]
            for version, fingerprint in fingerprints:
                score = similarity(baseline["normalized"], fingerprint["normalized"])
                record["versions"][version]["similarity"] = round(score, 4)
                fields = set(fingerprint.get("field_offsets", []))
                shared = fields.intersection(baseline.get("field_offsets", []))
                anchored = bool(fingerprint.get("vtable_classes") and
                                fingerprint["vtable_classes"] ==
                                baseline.get("vtable_classes") and len(shared) >= 8)
                row = record["versions"][version]
                row["shared_field_offsets"] = len(shared)
                row["match_method"] = "vtable_and_fields" if anchored else "instructions"
                row["body_match"] = score >= 0.55 or anchored
            record["body_mismatch"] = any(
                not r.get("body_match", True) for r in record["versions"].values())
        else:
            record["body_mismatch"] = None
    return result
