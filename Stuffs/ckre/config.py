"""Input discovery and versioned, disposable caches."""

import hashlib
import json
import os
from pathlib import Path
import tempfile
import tomllib

HOME = Path(__file__).resolve().parent
ROOT = HOME.parent.parent
HEADERS = ROOT / "CKPE.Fallout4/Include/EditorAPI"
VERSIONS = {
    "162": ("1_10_162", "1-10-162-0"),
    "982": ("1_10_982_3", "1-10-982-3"),
    "137": ("1_11_137", "1-11-137-0"),
    "246": ("1_11_246", "1-11-246-0"),
}
CACHE_SCHEMA = 1


class Config:
    def __init__(self, data=None):
        local = HOME / "ckre.local.toml"
        settings = tomllib.loads(local.read_text("utf-8-sig")) if local.exists() else {}
        explicit = data or os.environ.get("CKRE_DATA")
        value = explicit or settings.get("data")
        if not value:
            raise ValueError("Set CKRE_DATA, --data, or data in ckre.local.toml")
        path = Path(value).expanduser()
        if not path.is_absolute() and not explicit:
            path = HOME / path
        self.data = path.resolve()
        self.cache = HOME / ".cache"
        self.cache.mkdir(exist_ok=True)

    def exe(self, version):
        return self.require(f"creationkit_f4_{VERSIONS[version][0]}.exe")

    def library(self, version):
        return self.require(f"version-{VERSIONS[version][1]}.txt")

    def require(self, name):
        path = self.data / name
        if not path.is_file():
            raise FileNotFoundError(f"Missing input: {path}")
        return path

    def key(self, path, kind):
        stat = path.stat()
        identity = (str(path.resolve()), stat.st_size, stat.st_mtime_ns,
                    CACHE_SCHEMA, kind)
        digest = hashlib.sha256(repr(identity).encode()).hexdigest()[:24]
        return self.cache / f"{kind}-{digest}"

    def cached(self, path, kind, build):
        target = self.key(path, kind).with_suffix(".json")
        if target.exists():
            try:
                return json.loads(target.read_text("utf-8"))
            except (ValueError, OSError):
                pass
        value = build()
        atomic_write(target, json.dumps(value, separators=(",", ":")))
        return value


def atomic_write(path, text):
    path = Path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.NamedTemporaryFile(mode="w", encoding="utf-8",
                                     dir=path.parent, delete=False) as stream:
        temporary = Path(stream.name)
        stream.write(text)
    try:
        temporary.replace(path)
    finally:
        temporary.unlink(missing_ok=True)
