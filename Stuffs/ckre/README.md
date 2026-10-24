# ckre

Python 3.12+ reverse-engineering tools for the Fallout 4 Creation Kit binaries
and the CKPE EditorAPI headers. No IDA needed; binary inputs are read-only.

## Setup

```powershell
python -m pip install -e Stuffs\ckre   # or run `python -m ckre` from Stuffs
$env:CKRE_DATA = 'D:\CK-inputs'
ckre selftest
```

The input directory comes from `--data`, then `CKRE_DATA`, then `data = '...'`
in the git-ignored `ckre.local.toml`. It must contain:

| `--version` | Executable | Address Library |
|---|---|---|
| `162` | `creationkit_f4_1_10_162.exe` | `version-1-10-162-0.txt` |
| `982` | `creationkit_f4_1_10_982_3.exe` | `version-1-10-982-3.txt` |
| `137` | `creationkit_f4_1_11_137.exe` | `version-1-11-137-0.txt` |
| `246` | `creationkit_f4_1_11_246.exe` | `version-1-11-246-0.txt` |

Version `246` also needs `creationkit_f4_1_11_246.pdb` and `llvm-pdbutil` on
`PATH` (public symbols only; older builds don't borrow its names).
`layout check` needs an x64 MSVC install discoverable through `vswhere`.
Generated indexes and reports live in `.cache/`; delete it to rebuild. Never
commit executables, PDBs, Address Library files or generated reports.

## Common options

Every command accepts `--json`, `--data DIR` and
`--version {162,982,137,246,all}` (default `246`). Addresses are reported as
RVA and VA. Nearest-symbol names carry `nearest: true` and are not evidence.
Exit codes: `0` ok, `1` verification FAIL, `2` input or tool error.

## Commands

| Command | Purpose |
|---|---|
| `rtti [Class]` | RTTI type descriptors, COLs, hierarchy and vtables |
| `vtable Class` | Vtable slots with thunks followed and overrides marked |
| `xversion Class` | Vtable structure compared across all four builds |
| `id ID` / `id --rva RVA` | Address Library lookup in either direction |
| `resolve-ids FILE` | Check every `IDs.h` entry per version, fingerprint bodies |
| `ctor Class\|RVA` | Constructor field stores and allocation sizes |
| `ctor --registry` | 1.11.246 form-factory registry and form types |
| `strings-in RVA` | Strings referenced by a function |
| `xrefs [RVA] [--slot-offset N]` | Code references and `[reg+N]` virtual calls |
| `layout check [--headers ...]` | Compiled layouts versus the binary (the gate) |
| `coverage` | Per-class modeling status and ranked next targets |
| `selftest` | End-to-end check against layouts already verified |

```powershell
ckre vtable TESForm --version all
ckre ctor Sky
ckre resolve-ids CKPE.Fallout4\Include\EditorAPI\IDs.h --version all
ckre xrefs 0x301ED80 --slot-offset 0x10
ckre layout check --headers TESForm.h Sky.h
ckre coverage --output Stuffs\ckre
```

`resolve-ids` reads `Common::ID{a}` as one ID for all versions and `{a, b}`
as `a` for 1.10.162 and `b` for 1.10.982.3 onward; 0 means unresolved.
Fingerprint matches are a triage aid, not proof that two bodies are the same.

`ctor` sizes come from the allocator call before a constructor, or for forms
from slot 1 (`Create`) of `ConcreteFormFactory<T>`. Unproven sizes stay
unknown; `initialized_extent` is only a lower bound.

## Layout gate

`layout check` compiles the EditorAPI headers with `/d1reportAllClassLayout`
and compares each class with the 1.11.246 binary.

| Status | Meaning |
|---|---|
| `PASS` | Size and/or vtable proven to match |
| `PARTIAL` | Vtable declares fewer slots than the binary; the rest go through `vtbl_call` |
| `STUB` | Pointer-only placeholder with no size claim |
| `UNKNOWN` | No allocation or footprint evidence yet; not a verification |
| `FAIL` | Proven mismatch |

A vtable FAILs when it overruns the binary, or when it's partial and a modeled
subclass adds virtuals after it. A size is checked only when the header asserts
it or the class is embedded by value. Embedded-base footprints come from RTTI
base-offset gaps, so a larger composite allocation can't fail a component.
Only FAIL makes the command exit 1.

## Workflow

1. Before editing, save `layout check --json` and `coverage --json`.
2. After editing, rerun the full `layout check` and `selftest`.
3. Treat every FAIL as a header bug or a ckre bug. Fix the cause; never edit a
   header just to silence the gate.
4. Don't report UNKNOWN sizes or skipped headers as verified.
