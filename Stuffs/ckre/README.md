# ckre

Python 3.12+ tools for the Fallout 4 Creation Kit binaries and CKPE EditorAPI.
No command uses IDA or builds the CKPE solution. Binary inputs are read-only.

## Setup

From the repository root:

```powershell
python -m pip install -e Stuffs\ckre
$env:CKRE_DATA = 'D:\CK-inputs'
ckre selftest --json
```

Alternatively, run `python -m ckre` from `Stuffs` without installing.
The input directory is selected by `--data`, then `CKRE_DATA`, then the `data`
key in the optional, git-ignored `ckre.local.toml`:

```toml
data = 'D:\CK-inputs'
```

Relative paths in this TOML file are relative to the toolkit directory.

The directory must contain:

| Version selector | Executable | Address Library |
|---|---|---|
| `162` | `creationkit_f4_1_10_162.exe` | `version-1-10-162-0.txt` |
| `982` | `creationkit_f4_1_10_982_3.exe` | `version-1-10-982-3.txt` |
| `137` | `creationkit_f4_1_11_137.exe` | `version-1-11-137-0.txt` |
| `246` | `creationkit_f4_1_11_246.exe` | `version-1-11-246-0.txt` |

Version `246` also requires `creationkit_f4_1_11_246.pdb` and `llvm-pdbutil`
on `PATH`. Its public-symbol dump is generated with
`llvm-pdbutil dump -publics`. Older images do not borrow names from this PDB.

`layout check` requires Windows, an x64 MSVC installation discoverable through
`vswhere`, and the repository's existing dependencies. It discovers vcvars64,
reads preprocessor definitions from `CKPE.Fallout4.vcxproj`, uses
`/std:c++latest`, and compiles only temporary translation units.

All generated indexes and compiler reports live in `.cache/`. Binary indexes
are keyed by absolute input identity, size, nanosecond mtime and cache schema.
Delete `.cache/` to rebuild them. Compiler reports are keyed by header contents,
selected includes and project definitions; delete them after updating MSVC or
the Windows SDK. Concurrent writers publish complete JSON files atomically.
No executable, PDB, address library or generated report belongs in a commit.

## Common options and output

Every command accepts `--json`, `--data DIRECTORY` and
`--version {162,982,137,246,all}` before or after its command name.
The default version is `246`; `xversion` always uses all four versions.
`coverage` is specifically a `246` inventory. `selftest` always uses all four.

Output is JSON, including in the default terminal mode. Each result is either
an object or, for multi-version commands, a version-ordered array of objects.
Addresses include integer `rva`, hexadecimal `rva_hex`, `va` and `version`;
VA is the selected image's ImageBase plus RVA (normally `0x140000000`).
RVA arguments accept `0x` hexadecimal or decimal. A bare hexadecimal value
containing A–F is also accepted by address-lookup commands.
Nearest public names always carry `nearest: true` and `displacement`; they are
not exact symbols or evidence of a function's identity.

Exit codes: `0` completed without FAIL, `1` failed verification, `2` input/tool error.
`UNKNOWN` means insufficient evidence, not a successful binary verification.

## Commands

### `rtti [Class]`

```powershell
ckre rtti Sky --json
```

Without a class: `{version, count, classes}` with type descriptors, flattened
base descriptors and vtables. With a class: the same detailed vtable result as
`vtable`. Names are namespace-qualified; unqualified names must be unique.
Unexpanded MSVC template type names remain decorated and are retained in the
noise bucket rather than guessed.

The scanner independently finds type descriptors, verifies signature-1 COLs
and their self RVAs, and walks Class Hierarchy Descriptor base arrays. It
does not require the repository's RTTI dumps. The committed legacy dumps are
currently `Stuffs/rtti_f4_*.txt`; they are reference material, not an input index.

### `vtable Class`

```powershell
ckre vtable TESForm --version all --json
```

Output: `{name, version, direct_base, bases, vtables}`. Each vtable contains its
RVA, COL RVA, subobject `offset`, `slot_count` and `slots`. Slots contain `index`,
`byte_offset`, `target`, thunk-resolved `body`, `symbol`, `inherited` and
`override`. Unknown inheritance comparisons are `null`, not false.

Walking stops at another exact public symbol or a non-`.text` pointer.
`E9 rel32` and `FF 25 [rip+disp32]` thunks are followed with cycle protection.
Primary-table overrides use the direct base at displacement zero. Secondary
tables are compared to a base whose nonvirtual PMD displacement matches the
COL offset. The selected `comparison_base` is reported. Virtual-base or other
unresolved subobject comparisons stay unknown.

### `xversion Class`

```powershell
ckre xversion BGSRenderWindow --json
```

Output: `{class, versions, layout_differences}`. Per-version tables give
subobject offsets, slot counts and override patterns. Differences describe
vtable structure, not a claim about every nonvirtual data member.

### `id ID` / `id --rva RVA`

```powershell
ckre id 198086 --version all --json
ckre id --rva 0x301ED80 --version 246 --json
```

Forward output: `{id, version, address}`. Reverse output: an address record
with all matching `ids`. A missing forward ID is `address: null`.
Address Library text uses decimal IDs and hexadecimal RVAs, with leading
whitespace permitted.

### `resolve-ids FILE`

```powershell
ckre resolve-ids CKPE.Fallout4\Include\EditorAPI\IDs.h --version all --json
```

Output: `[{name, ids, versions, body_mismatch}]`. Each version records
`PASS`, `DATA`, `UNRESOLVED`, `MISSING` or `FAIL`, section, function-start
validation, and an available code `fingerprint`.

Fingerprints include unwind-record size, the first 64 normalized instructions,
a hash, and referenced strings. Instruction comparison is a multiset Jaccard
score; a score below 0.55 is flagged for inspection. Constructor fingerprints
also include object-field offsets and written RTTI vtable classes. An identical
vtable-class set and at least eight shared field offsets can match constructors
whose loops were unrolled or registers changed between compiler generations.
`match_method` and the raw instruction score remain visible. This is a cheap
triage heuristic, not proof of semantic equivalence.

ID declarations are parsed with their namespace nesting, not by name-only
grep. `Common::ID{a}` repeats `a` for all versions. `{a,b}` selects `a` for
162 and `b` for 982 onward. Zero means explicitly unresolved. The actual
`Common::ID` implementation also supports three/four initializers and repeats
the last value; ckre follows that rule. `IDs_VTABLE.h` arrays are supported;
their entries are exposed as `Class_slot0`, `Class_slot1`, etc.
Run the command separately on either header, or on a copied IDs block.

### `ctor Class|RVA`

```powershell
ckre ctor Sky --json
```

Output: `{target, version, candidates, limitations}`. Each candidate contains
constructor RVA, field `stores`, calls, `allocations`, `allocation_sizes` and
`initialized_extent`. A store gives offset, width, source kind and instruction
RVA. Sources distinguish constants, zeros, other vtables, registers and call
results. XOR-zeroed vector registers and `rep stos` stores are recognized.
Class discovery follows references to its primary vtable and requires that
the last known primary-vtable write at offset zero is the requested class.
Constructor IDs in `IDs.h` also locate nonpolymorphic classes; `DataConstructor`
under `Renderer` denotes `RendererData`.

Allocation evidence is from an immediately preceding call with a constant
ECX/EDX size and `mov rcx,rax` forwarding to the constructor. Call/jump thunks
are canonicalized once. Exact allocator PDB names are preferred. Otherwise
targets called at least 100 times are candidates; embedded allocation and
`memorymanager.cpp` diagnostics identify CK memory-manager allocation bodies.
That path uses EDX as size, retaining diagnostic strings and xref counts.
Frequency-only suggestions are labeled `frequency_heuristic` and excluded
from the layout size gate. A subsequent different vtable materialization
invalidates a base-constructor allocation inference.

Register provenance is linear, not a control-flow proof. Constructor and
destructor vtable writes can both produce candidates. Inlined initialization,
placement construction, custom allocators, indirect calls and split unwind
records can leave sizes unknown. `initialized_extent` is a lower bound on
written bytes, never silently promoted to an exact allocation size.

### `strings-in RVA`

```powershell
ckre strings-in 0xE9A890 --json
```

Output: `{address, bounds, strings}`. Strings give text, encoding, RVA and
referring instruction RVA. LEA/MOV references and one pointer indirection
are checked for printable ASCII and UTF-16LE in `.rdata`/`.data`.
Function bounds are taken from `.pdata`, not a fixed byte window.

### `xrefs [RVA] [--slot-offset BYTES]`

```powershell
ckre xrefs 0x301ED80 --slot-offset 0x10 --json
```

Output: `{address, slot_offset, references, virtual_call_note}`. References
give instruction RVA, containing unwind function RVA and kind. The reusable
index covers direct calls/jumps and RIP-relative operands in `.pdata` ranges.
`--slot-offset` additionally finds `call qword ptr [reg+offset]`; it does not
prove the receiver's dynamic type. Omit the target for an offset-only query.
References in unlisted leaf functions, embedded data or computed addresses
are not exhaustive.

### `layout check [--headers HEADER ...]`

```powershell
ckre layout check --headers TESForm.h BGSRenderWindow.h Sky.h --json
```

Default: all EditorAPI headers, topologically ordered by their include graph.
On aggregate compilation failure, standalone probes identify failing headers;
those are reported under `skipped_headers`, then compilable headers are retried.
Compiler diagnostics and raw layout text remain in `.cache`.

Output: `{version, counts, classes, skipped_headers, compiler_exit_code}`.
Per-class rows give header/assert sizes, primary-vtable sizes, binary allocation
sizes, verified dimensions, reasons and evidence. MSVC's first vftable block
is primary, including `Class::$vftable@FirstBase@` spellings. Namespace and
template spellings are preserved.

Counts always include `PASS`, `PARTIAL`, `STUB`, `UNKNOWN` and `FAIL`.
Rows expose separate `size_status` and `vtable_status` plus structured
`failures` with kind, header value, binary value and reason.

Equal vtable counts are PASS; a compiled overrun is FAIL. A shorter table is
PARTIAL: omitted trailing slots may be called through `vtbl_call`. A partial
primary base becomes FAIL if a modeled binary descendant introduces new
primary virtuals; the reason names that subclass. The subclass also fails if
its compiled count cannot hold its new slots after the binary primary base.
New slots are measured against compiled primary bases. Flattened facades
without C++ inheritance compare method names with modeled binary bases;
their destructors still override binary-base destructors and are not new slots.

Size comparison applies to explicit `static_assert(sizeof(X) == ...)` claims
and by-value uses in compiled modeled objects, base subobjects or arrays.
Pointer-only binary stubs with neither obligation are STUB, even when their
observed allocation is larger. A claimed/embedded size that disagrees with
unambiguous binary allocation evidence is FAIL. An assert alone does not
prove the binary size; absent or ambiguous allocation evidence is UNKNOWN.

Overall status prioritizes FAIL, STUB, PARTIAL, PASS, then UNKNOWN. Both
dimension statuses remain visible; a STUB can therefore also have a PARTIAL
vtable. Only FAIL makes the layout command exit 1. Skipped headers and
insufficient evidence remain UNKNOWN, not successful verification.

### `coverage`

```powershell
ckre coverage --output Stuffs\ckre --json
```

Writes `coverage.json` and `coverage.md` (both ignored in the toolkit directory).
Terminal output contains totals, layout counts and the top 20 next targets.
JSON retains every RTTI class, a separate noise bucket, header mapping, size
assert/verification, vtable verification, pad/unk array bytes, unk scalars,
`sub_` virtual count, source-reference files and descendant counts. Per-class
overall, size and vtable statuses preserve PARTIAL and STUB separately.

The Markdown report has totals, TES/BGS, BS/Ni, editor-window/dialog and other
tables. Targets sort by source-file usage, descendant count, unknown bytes,
then name. Unmodeled unknown-byte counts are null, not a measured zero.
Array bytes are counted only for literal extents and known primitive widths;
unresolved element widths remain visible in source metadata.
Noise includes MFC/Qt naming patterns, standard/library templates, anonymous
types and known external SDK namespaces. Havok and other named engine types
remain available. Totals include both complete RTTI count and filtered count;
these must not be compared as if they described the same population.

### `selftest`

```powershell
ckre selftest --json
```

Output: `{status, counts, checks}`. No mocks or synthetic fixtures are used.
Checks cover TESForm's 124 slots and primary RVA in all four executables,
the committed BGSRenderWindow/Sky constructor IDs and semantic fingerprints,
the seven established layout sizes, and Sky's `0x448` allocation. NiCamera
specifically requires size PASS at `0x1A0` and vtable PARTIAL, not vtable PASS.
Selftest does not rewrite headers or waive genuine gate failures.

## Fleet layout gate

1. Before a wave, save `layout check --json` and `coverage --json` results.
2. After edits, run the same full `layout check`, then `selftest`.
3. Investigate every FAIL: compiler-parser bug, vtable overrun, new slots after
   a partial base, incorrect claimed layout or constructor provenance.
4. Fix tool bugs in ckre; report header bugs separately. Do not change a header
   merely to silence a count without verifying binary inheritance and slots.
5. Do not describe UNKNOWN sizes, skipped headers or frequency-only allocator
   guesses as verified. Regenerate coverage when the wave is complete.
