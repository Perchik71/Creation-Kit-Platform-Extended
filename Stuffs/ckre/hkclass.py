"""Havok 2014 reflection, reconstructed from checked static initializers.

The x64 hkClass is 0x50 bytes: name +00, parent +08, size +10,
interface count +14, enums +18/count +20, members +28/count +30,
defaults +38, attributes +40, flags +48, described version +4C.
hkClassMember is 0x28: three pointers, type/subtype bytes at +18/+19,
array count +1A, flags +1C, offset +1E, attributes pointer +20.
hkClassEnum is 0x28: name +00, items +08, count +10, attrs +18,
flags +20. Each enum item is 0x10: signed value +00, name +08.

Hand checked in 246: hkClass's own reflected fields agree with the
constructor at 0x1CDD260. It copies RDX/R8/R9D and caller stack slots
+28..+68 into those offsets. hkReferencedObject's initializer at
0x26D510 sets size 0x10, parent hkBaseObject and its member array.
The records themselves are loader-zero-filled globals, not file data.
Old serialization schemas reuse names; conflicting sizes stay ambiguous
unless an independent hkTypeInfo record identifies the native size.
"""

from collections import defaultdict
import struct

from capstone.x86 import X86_OP_MEM, X86_OP_REG

from .ctor import register

TYPE_NAMES = (
    "VOID", "BOOL", "CHAR", "INT8", "UINT8", "INT16", "UINT16", "INT32",
    "UINT32", "INT64", "UINT64", "REAL", "VECTOR4", "QUATERNION", "MATRIX3",
    "ROTATION", "QSTRANSFORM", "MATRIX4", "TRANSFORM", "ZERO", "POINTER",
    "FUNCTIONPOINTER", "ARRAY", "INPLACEARRAY", "ENUM", "STRUCT", "SIMPLEARRAY",
    "HOMOGENEOUSARRAY", "VARIANT", "CSTRING", "ULONG", "FLAGS", "HALF",
    "STRINGPTR", "RELARRAY", "MAX",
)


class Reflection:
    def __init__(self, constructors):
        self.ctors = constructors
        self.image, self.disasm = constructors.image, constructors.disasm
        self.data = self.image.config.cached(
            self.image.path, "hkclass-v3", self.build)
        self.records = self.data["records"]
        self.by_rva = {row["rva"]: row for row in self.records}
        self.by_name = defaultdict(list)
        for row in self.records:
            self.by_name[row["name"]].append(row)

    def text(self, pointer):
        rva = pointer - self.image.base
        section = self.image.section(rva)
        if not section or section["name"] not in (".rdata", ".data"):
            raise ValueError("Reflection string pointer is not in a data section")
        value = self.image.cstring(rva, 256)
        if not value or len(value) >= 255 or not all(32 <= ord(c) < 127 for c in value):
            raise ValueError("Invalid reflection string")
        return value

    def constructor(self, target):
        body = self.disasm.follow(target)[0]
        section = self.image.section(body)
        if not section or not self.image.is_code(body):
            return None
        length = min(256, section["rva"] + section["raw_size"] - body)
        fields, loads, arguments = {}, {}, {"rdx": 0, "r8": 8, "r9": 16}
        for ins in self.disasm.md.disasm(self.image.read(body, length), body):
            if ins.mnemonic == "ret":
                break
            if ins.mnemonic != "mov" or len(ins.operands) != 2:
                continue
            dst, src = ins.operands
            if dst.type == X86_OP_REG:
                name = register(ins.reg_name(dst.reg))
                if src.type == X86_OP_MEM and ins.reg_name(src.mem.base) == "rsp":
                    loads[name] = src.mem.disp
                else:
                    loads.pop(name, None)
            elif dst.type == X86_OP_MEM and src.type == X86_OP_REG:
                if ins.reg_name(dst.mem.base) != "rcx" or dst.mem.index:
                    continue
                name = register(ins.reg_name(src.reg))
                value = ("argument", arguments[name]) if name in arguments else (
                    "stack", loads[name]) if name in loads else None
                fields[(dst.mem.disp, dst.size)] = value
        expected = {
            (0, 8): ("argument", 0), (8, 8): ("argument", 8),
            (16, 4): ("argument", 16), (20, 4): ("stack", 0x30),
            (24, 8): ("stack", 0x38), (32, 4): ("stack", 0x40),
            (40, 8): ("stack", 0x48), (48, 4): ("stack", 0x50),
            (56, 8): ("stack", 0x58), (64, 8): ("stack", 0x60),
            (72, 4): ("stack", 0x68), (76, 4): ("stack", 0x70)}
        return body if all(fields.get(key) == value
                           for key, value in expected.items()) else None

    def enum(self, rva):
        name, items, count, _, attributes, flags = struct.unpack(
            "<QQIIQI4x", self.image.read(rva, 40))
        if count > 65535 or bool(items) != bool(count):
            raise ValueError("Invalid hkClassEnum item array")
        values = []
        for index in range(count):
            at = items - self.image.base + index * 16
            value, label = struct.unpack("<i4xQ", self.image.read(at, 16))
            values.append({"name": self.text(label), "value": value, "rva": at})
        return {"name": self.text(name), "rva": rva, "items": values,
                "item_count": count, "flags": flags,
                "attributes_rva": attributes - self.image.base if attributes else None}

    def members(self, pointer, count):
        result = []
        for index in range(count):
            rva = pointer - self.image.base + index * 40
            data = self.image.read(rva, 40)
            if not any(data):
                result.append({"name": None, "rva": rva, "offset": None,
                               "type": None, "class_rva": None,
                               "metadata_available": False})
                continue
            name, cls, enum, kind, subtype, array, flags, offset, attrs = struct.unpack(
                "<QQQBBHHHQ", data)
            if kind >= len(TYPE_NAMES) or subtype >= len(TYPE_NAMES):
                raise ValueError("Invalid hkClassMember type code")
            result.append({
                "name": self.text(name), "rva": rva, "offset": offset,
                "type": TYPE_NAMES[kind], "type_code": kind,
                "subtype": TYPE_NAMES[subtype], "subtype_code": subtype,
                "array_size": array, "flags": flags,
                "class_rva": cls - self.image.base if cls else None,
                "enum": self.enum(enum - self.image.base) if enum else None,
                "attributes_rva": attrs - self.image.base if attrs else None})
        return result

    def type_infos(self, records):
        names = {row["name"] for row in records}
        pointers = defaultdict(list)
        for row in records:
            pointers[row["name_rva"] + self.image.base].append(row["name"])
        result = defaultdict(list)
        for section in self.image.sections:
            if section["name"] not in (".data", ".rdata"):
                continue
            length = section["raw_size"] // 8 * 8
            data = self.image.read(section["rva"], length)
            for index, (pointer,) in enumerate(struct.iter_unpack("<Q", data)):
                if pointer not in pointers or index * 8 + 48 > length:
                    continue
                rva = section["rva"] + index * 8
                scoped, finish, cleanup, vtable, size = struct.unpack_from(
                    "<5Q", data, index * 8 + 8)
                try:
                    label = self.text(scoped)
                except ValueError:
                    continue
                if label[0] != "!" or label[1:] not in names or not 0 < size < 0x1000000:
                    continue
                if not any(label[1:] == name for name in pointers[pointer]):
                    continue
                if any(value and not self.image.is_code(value - self.image.base)
                       for value in (finish, cleanup)):
                    continue
                result[label[1:]].append({"rva": rva, "size": size,
                                         "source": "file_backed",
                                         "vtable_rva": vtable - self.image.base
                                         if vtable else None})
        seen = set()
        for pointer in pointers:
            for ref in self.disasm.xrefs(pointer - self.image.base):
                entry = ref["function_rva"]
                if entry in seen:
                    continue
                seen.add(entry)
                _, writes = self.ctors.constant_trace(entry)
                stores = {write["rva"]: write for write in writes
                          if write["rva"] is not None and write["width"] == 8}
                for at, write in stores.items():
                    if write["value"] not in pointers:
                        continue
                    if not all(at + offset in stores for offset in (8, 16, 24, 40)):
                        continue
                    try:
                        label = self.text(stores[at + 8]["value"])
                    except ValueError:
                        continue
                    size = stores[at + 40]["value"]
                    if label[0] != "!" or label[1:] not in pointers[write["value"]]:
                        continue
                    if not 0 < size < 0x1000000 or any(
                            not self.image.is_code(stores[at + offset]["value"] -
                                                   self.image.base)
                            for offset in (16, 24)):
                        continue
                    result[label[1:]].append({
                        "rva": at, "size": size, "source": "static_initializer",
                        "initializer_rva": entry,
                        "size_instruction_rva": stores[at + 40]["site_rva"],
                        "size_instruction": stores[at + 40]["instruction"]})
        return dict(result)

    def build(self):
        targets = set()
        for anchor in ("hkClass", "hkReferencedObject", "hkpRigidBody"):
            for string in self.image.find(anchor.encode() + b"\0", ".rdata"):
                for ref in self.disasm.xrefs(string):
                    calls, _ = self.ctors.constant_trace(ref["function_rva"])
                    for call in calls:
                        if call["arguments"].get("rdx", {}).get("value") != (
                                self.image.base + string):
                            continue
                        body = self.constructor(call["target_rva"])
                        if body is not None:
                            targets.add(body)
        records, errors, skipped, seen = [], [], [], set()
        for target in sorted(targets):
            for ref in self.ctors.callers(target):
                if ref["site_rva"] in seen:
                    continue
                seen.add(ref["site_rva"])
                calls, _ = self.ctors.constant_trace(ref["function_rva"])
                call = next((c for c in calls if c["site_rva"] == ref["site_rva"]), None)
                if not call or not all(key in call["arguments"] for key in
                                       ("rcx", "rdx", "r8", "r9")):
                    skipped.append(ref["site_rva"])
                    continue
                args, stack = call["arguments"], call["stack_arguments"]
                if not all(offset in stack for offset in range(0x20, 0x70, 8)):
                    skipped.append(ref["site_rva"])
                    continue
                try:
                    record = args["rcx"]["value"] - self.image.base
                    name = self.text(args["rdx"]["value"])
                    section = self.image.section(record)
                    if not section or section["name"] != ".data":
                        raise ValueError("hkClass destination is not static data")
                    size = args["r9"]["value"]
                    if not 0 <= size < 0x1000000:
                        raise ValueError("Invalid reflected object size")
                    values = {offset: value["value"] for offset, value in stack.items()}
                    for pointer, count in ((values[0x30], values[0x38]),
                                           (values[0x40], values[0x48])):
                        if count > 65535 or bool(pointer) != bool(count):
                            raise ValueError("Invalid declared reflection array")
                    parent = args["r8"]["value"]
                    members = self.members(values[0x40], values[0x48])
                    enums = [self.enum(values[0x30] - self.image.base + i * 40)
                             for i in range(values[0x38])]
                    records.append({
                        "name": name, "rva": record, "address": self.image.address(record),
                        "name_rva": args["rdx"]["value"] - self.image.base,
                        "size": size, "parent_rva": parent - self.image.base if parent else None,
                        "interface_count": values[0x28], "members": members,
                        "members_rva": values[0x40] - self.image.base if values[0x40] else None,
                        "enums_rva": values[0x30] - self.image.base if values[0x30] else None,
                        "member_count": len(members), "enums": enums,
                        "metadata_complete": all(m.get("metadata_available", True)
                                                 for m in members),
                        "enum_count": len(enums), "flags": values[0x60],
                        "described_version": values[0x68],
                        "defaults_rva": values[0x50] - self.image.base if values[0x50] else None,
                        "attributes_rva": values[0x58] - self.image.base if values[0x58] else None,
                        "source": "static_initializer",
                        "initializer_rva": ref["function_rva"],
                        "constructor_rva": target, "call_rva": call["site_rva"],
                        "size_instruction_rva": args["r9"]["site_rva"],
                        "size_instruction": args["r9"]["instruction"]})
                except ValueError as exc:
                    errors.append({"site_rva": ref["site_rva"], "error": str(exc)})
        by_rva = {row["rva"]: row for row in records}
        for row in records:
            row["parent"] = by_rva.get(row["parent_rva"], {}).get("name")
            for member in row["members"]:
                member["class"] = by_rva.get(member["class_rva"], {}).get("name")
        return {"records": sorted(records, key=lambda r: (r["name"], r["rva"])),
                "constructors": sorted(targets), "errors": errors,
                "skipped_dynamic_calls": sorted(skipped),
                "type_infos": self.type_infos(records)}

    def get(self, name):
        variants = self.by_name.get(name, [])
        if not variants:
            return None
        native = {info["size"] for info in self.data["type_infos"].get(name, [])}
        conflicting = len({row["size"] for row in variants}) > 1
        selected = ([row for row in variants if row["size"] in native]
                    if native and conflicting else variants)
        sizes = {row["size"] for row in selected}
        if len(sizes) != 1:
            return {"name": name, "size": None, "ambiguous": True,
                    "variants": variants, "type_infos": self.data["type_infos"].get(name, [])}
        row = max(selected, key=lambda r: (r["described_version"], -r["rva"]))
        return {**row, "ambiguous": False,
                "selection": "type_info" if native and conflicting else "consistent_size",
                "type_info_size_disagreement": bool(native and row["size"] not in native),
                "variants": [item for item in variants if item["rva"] != row["rva"]],
                "type_infos": self.data["type_infos"].get(name, [])}

    def size_evidence(self, source):
        name = source.get("binary_name", source["name"])
        candidates = [name]
        parts = name.split("::")
        if len(parts) > 1 and parts[-1].startswith("hk"):
            candidates.append(parts[-1])
        for index, part in enumerate(parts):
            if part.startswith("hk"):
                candidates.append("".join(parts[index:]))
        for name in dict.fromkeys(candidates):
            row = self.get(name)
            if row and not row["ambiguous"] and row["size"] > 0:
                return {
                    "class": source["name"], "size": row["size"], "kind": "exact",
                    "versions": [self.image.version], "status": "PASS",
                    "source": "hkclass", "reflected_class": name,
                    "record_rva": row["rva"], "record_address": row["address"],
                    "initializer_rva": row["initializer_rva"],
                    "constructor_rva": row["constructor_rva"],
                    "call_rva": row["call_rva"],
                    "size_instruction_rva": row["size_instruction_rva"],
                    "size_instruction": row["size_instruction"],
                    "selection": row["selection"],
                    "type_info_size_disagreement": row["type_info_size_disagreement"]}
        return None

    def describe(self, name=None):
        if name:
            result = self.get(name)
            if result is None:
                raise ValueError(f"No reflected Havok class named {name}")
            inherited, seen = [], set()
            parent = self.by_rva.get(result.get("parent_rva"))
            while parent and parent["rva"] not in seen:
                seen.add(parent["rva"])
                inherited.extend({"declared_by": parent["name"], **member}
                                 for member in parent["members"])
                parent = self.by_rva.get(parent["parent_rva"])
            result["inherited_members"] = sorted(
                inherited, key=lambda member: member["offset"] or 0)
            return {"version": self.image.version, **result}
        classes = [self.get(name) for name in sorted(self.by_name)]
        return {"version": self.image.version, "count": len(classes),
                "record_count": len(self.records), "classes": classes,
                "ambiguous_count": sum(row["ambiguous"] for row in classes),
                "constructors": self.data["constructors"],
                "errors": self.data["errors"],
                "skipped_dynamic_calls": self.data["skipped_dynamic_calls"]}
