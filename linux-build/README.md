# CKPE Linux cross-build (msvc-wine)

Build the Creation Kit Platform Extended DLLs/EXEs on Linux using **msvc-wine**:
the genuine Microsoft `cl.exe` / `link.exe` / `lib.exe` / `rc.exe` running under
Wine. Because this is the exact toolchain the `.vcxproj` files target, the sources
build essentially unmodified — this is a parallel build tree that never touches the
Visual Studio `.sln`/`.vcxproj` or the shared sources.

## Prerequisites

- `wine`, `python3`, `git`, and `msitools` (provides `msiextract`).
- `cmake` (>= 3.21) and `ninja`.

On Arch/CachyOS:

```sh
sudo pacman -S --needed wine python git msitools cmake ninja
```

Initialize submodules (Detours, mzip, libdeflate, toml11, ImGUI, and jDialogs with
its nested nlohmann/json):

```sh
git submodule update --init --recursive
```

## 1. Install the toolchain (once)

```sh
linux-build/bootstrap-msvc-wine.sh
```

Installs msvc-wine into `~/.local/share/msvc-wine` by default. Customize with
`--prefix DIR` or the `MSVC_WINE_DIR` environment variable. Useful flags:

- `--install-deps`         install missing system packages (uses sudo)
- `--major VER`           Visual Studio line to pull (default: 17 = VS 2022;
                          vsdownload's own default 18 is the preview/next VS)
- `--msvc-version VER`     pin a specific MSVC toolset (default: newest in the line)
- `--sdk-version VER`      pin the Windows SDK (default: latest)
- `--force`               reinstall over an existing toolchain
- `--help`                full option list

## 2. Configure and build

From the repo root (explicit form — recommended):

```sh
cmake -S linux-build -B build-linux -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE="$PWD/cmake/msvc-wine-toolchain.cmake"
cmake --build build-linux
```

Or with the preset — note `cmake --preset` reads `CMakePresets.json` from the
current directory, so run it from `linux-build/`:

```sh
(cd linux-build && cmake --preset msvc-wine)
cmake --build build-linux
```

Build a single target, e.g.:

```sh
cmake --build build-linux --target CKPE.Common
```

Artifacts land in `build-linux/`: `ckpe_loader.exe`, `CKPE.dll`, `winhttp.dll`,
`CKPE.Common.dll`, `CKPE.PluginAPI.dll`, `CKPE.Fallout4.dll`, `CKPE.SkyrimSE.dll`,
`CKPE.Starfield.dll`.

The `msvc-wine-noavx2` preset builds the SSE2 (NoAVX2) variant.
