"""MSVC x64 RTTI COLs, hierarchy descriptors and vtable slots."""

import re
import struct

from .pdb import Publics


def type_name(decorated):
    match = re.fullmatch(r"\.\?A[UV](.+)@@", decorated)
    if not match:
        return decorated
    body = match[1]
    if body.startswith("?$"):
        return decorated
    return "::".join(reversed(body.split("@")))


class RTTI:
    def __init__(self, image):
        self.image = image
        self.publics = Publics(image)
        self.classes = image.config.cached(image.path, "rtti", self._scan)
        self.byname = {c["name"]: c for c in self.classes}
        self.bytable = {v["rva"]: c["name"] for c in self.classes
                        for v in c["vtables"]}

    def _scan(self):
        image = self.image
        descriptors = {}
        for section in image.sections:
            if section["name"] not in (".data", ".rdata"):
                continue
            data = image.raw[section["raw"]:section["raw"] + section["raw_size"]]
            for match in re.finditer(rb"\.\?A[UV][^\0]{1,1024}@@\0", data):
                rva = section["rva"] + match.start() - 16
                descriptors[rva] = match[0][:-1].decode("ascii", "replace")
        cols, classes = {}, {}
        for section in image.sections:
            if section["name"] != ".rdata":
                continue
            data = image.raw[section["raw"]:section["raw"] + section["raw_size"]]
            for match in re.finditer(b"\x01\0\0\0", data):
                offset = match.start()
                if offset % 4 or offset + 24 > len(data):
                    continue
                sig, displacement, cd, td, chd, self_rva = struct.unpack_from(
                    "<6I", data, offset)
                col = section["rva"] + offset
                if td not in descriptors or self_rva != col:
                    continue
                try:
                    count, array = image.u32(chd + 8), image.u32(chd + 12)
                    if not 0 < count < 4096:
                        continue
                    bases = []
                    for index in range(count):
                        bcd = image.u32(array + 4 * index)
                        base_td = image.u32(bcd)
                        if base_td not in descriptors:
                            raise ValueError("Invalid base type descriptor")
                        bases.append({
                            "name": type_name(descriptors[base_td]),
                            "decorated": descriptors[base_td],
                            "contained_bases": image.u32(bcd + 4),
                            "mdisp": image.i32(bcd + 8),
                            "pdisp": image.i32(bcd + 12),
                            "vdisp": image.i32(bcd + 16),
                            "attributes": image.u32(bcd + 20),
                        })
                except (ValueError, struct.error):
                    continue
                record = classes.setdefault(td, {
                    "name": type_name(descriptors[td]), "decorated": descriptors[td],
                    "type_descriptor_rva": td, "hierarchy_rva": chd,
                    "bases": bases, "vtables": [],
                })
                cols[col] = (record, displacement, cd)
        col_vas = {image.base + col: col for col in cols}
        for section in image.sections:
            if section["name"] != ".rdata":
                continue
            data = image.raw[section["raw"]:section["raw"] + section["raw_size"]]
            for offset in range(0, len(data) - 8, 8):
                pointer = struct.unpack_from("<Q", data, offset)[0]
                if pointer not in col_vas:
                    continue
                col = col_vas[pointer]
                record, displacement, cd = cols[col]
                table = section["rva"] + offset + 8
                slots = []
                for index in range(4096):
                    at = table + index * 8
                    if index and at in self.publics.names:
                        break
                    try:
                        target = image.u64(at) - image.base
                    except ValueError:
                        break
                    if not image.is_code(target):
                        break
                    slots.append(target)
                if slots:
                    record["vtables"].append({
                        "rva": table, "col_rva": col, "offset": displacement,
                        "construction_displacement": cd, "slots": slots,
                    })
        return sorted((c for c in classes.values() if c["vtables"]),
                      key=lambda c: c["name"])

    def get(self, name):
        if name in self.byname:
            return self.byname[name]
        matches = [c for c in self.classes if c["name"].split("::")[-1] == name]
        if len(matches) != 1:
            raise ValueError(f"RTTI class not found or ambiguous: {name}")
        return matches[0]

    @staticmethod
    def primary(record):
        return next((v for v in record["vtables"] if v["offset"] == 0), None)

    def direct_base(self, record):
        bases = self.direct_bases(record)
        primary = next((b for b in bases if b["mdisp"] == 0 and b["pdisp"] == -1),
                       bases[0] if bases else None)
        return self.byname.get(primary["name"]) if primary else None

    @staticmethod
    def direct_bases(record):
        bases, result, index = record["bases"], [], 1
        while index < len(bases):
            base = bases[index]
            result.append(base)
            index += base["contained_bases"] + 1
        return result

    def describe(self, name, disasm):
        record = self.get(name)
        base = self.direct_base(record)
        base_table = self.primary(base) if base else None
        result = {k: v for k, v in record.items() if k != "vtables"}
        result["version"] = self.image.version
        result["direct_base"] = base["name"] if base else None
        result["direct_bases"] = self.direct_bases(record)
        result["vtables"] = []
        for table in record["vtables"]:
            comparison, comparison_name = base_table, base["name"] if base else None
            if table["offset"] != 0:
                descriptor = next((b for b in record["bases"][1:] if
                                   b["mdisp"] == table["offset"] and
                                   b["pdisp"] == -1), None)
                secondary = self.byname.get(descriptor["name"]) if descriptor else None
                comparison = self.primary(secondary) if secondary else None
                comparison_name = secondary["name"] if secondary else None
            slots = []
            for index, target in enumerate(table["slots"]):
                body, chain = disasm.follow(target)
                inherited = None
                if comparison:
                    if index < len(comparison["slots"]):
                        base_body = disasm.follow(comparison["slots"][index])[0]
                        inherited = body == base_body
                slots.append({
                    "index": index, "byte_offset": index * 8,
                    "target": self.image.address(target),
                    "body": self.image.address(body), "thunks": chain,
                    "symbol": self.publics.lookup(body),
                    "inherited": inherited,
                    "override": not inherited if inherited is not None else None,
                })
            result["vtables"].append({
                **{k: v for k, v in table.items() if k != "slots"},
                "address": self.image.address(table["rva"]),
                "comparison_base": comparison_name,
                "slot_count": len(slots), "slots": slots,
            })
        return result
