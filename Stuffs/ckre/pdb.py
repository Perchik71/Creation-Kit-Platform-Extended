"""Public symbols use decimal section-relative offsets, not RVAs."""

from bisect import bisect_right
import re
import subprocess

from .config import atomic_write


class Publics:
    def __init__(self, image):
        self.names = {}
        if image.version == "246":
            path = image.config.require("creationkit_f4_1_11_246.pdb")
            data = image.config.cached(path, "publics", lambda: self._load(image, path))
            self.names = {int(k): v for k, v in data.items()}
        self.addresses = sorted(self.names)
        self.byname = {name: rva for rva, names in self.names.items()
                       for name in names}

    @staticmethod
    def _load(image, path):
        dump = image.config.key(path, "publics-dump").with_suffix(".txt")
        if not dump.exists():
            process = subprocess.run(
                ["llvm-pdbutil", "dump", "-publics", str(path)],
                stdout=subprocess.PIPE, stderr=subprocess.PIPE,
                text=True, encoding="utf-8", errors="replace", check=True)
            atomic_write(dump, process.stdout)
        result, current = {}, None
        with dump.open(encoding="utf-8", errors="replace") as stream:
            for line in stream:
                match = re.search(r"S_PUB32 \[size = \d+\] `(.*)`", line)
                if match:
                    current = match[1]
                match = re.search(r"addr = (\d+):(\d+)", line)
                if match and current:
                    section, offset = int(match[1]), int(match[2])
                    if 1 <= section <= len(image.sections):
                        rva = image.sections[section - 1]["rva"] + offset
                        result.setdefault(rva, []).append(current)
                    current = None
        return result

    def lookup(self, rva):
        if rva in self.names:
            return {"names": self.names[rva], "nearest": False, "displacement": 0}
        index = bisect_right(self.addresses, rva) - 1
        if index < 0:
            return {"names": [], "nearest": False, "displacement": None}
        start = self.addresses[index]
        return {"names": self.names[start], "nearest": True,
                "displacement": rva - start}
