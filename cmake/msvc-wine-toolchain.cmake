# ============================================================================
# cmake/msvc-wine-toolchain.cmake
#
# CMake toolchain for cross-building CKPE on Linux with msvc-wine: the genuine
# Microsoft cl.exe / link.exe / lib.exe / rc.exe running under Wine. This is the
# exact toolchain the .vcxproj files target, so the project builds essentially
# unmodified.
#
# Install the toolchain first:
#     linux-build/bootstrap-msvc-wine.sh
#
# Configure with it (either form):
#     cmake --preset msvc-wine
#     cmake -S linux-build -B build-linux -G Ninja \
#           -DCMAKE_TOOLCHAIN_FILE=$PWD/cmake/msvc-wine-toolchain.cmake
#
# The install location is resolved from (in order):
#     -DMSVC_WINE_DIR=...   ->   $ENV{MSVC_WINE_DIR}   ->   ~/.local/share/msvc-wine
# and the wrapper arch from MSVC_WINE_ARCH (default: x64).
# ============================================================================

# Mark this as a cross-compile targeting Windows; the CMakeLists guards on this.
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR AMD64)

# ---- Resolve the msvc-wine install directory --------------------------------
if(NOT DEFINED MSVC_WINE_DIR OR MSVC_WINE_DIR STREQUAL "")
    if(DEFINED ENV{MSVC_WINE_DIR} AND NOT "$ENV{MSVC_WINE_DIR}" STREQUAL "")
        set(MSVC_WINE_DIR "$ENV{MSVC_WINE_DIR}")
    else()
        set(MSVC_WINE_DIR "$ENV{HOME}/.local/share/msvc-wine")
    endif()
endif()
set(MSVC_WINE_DIR "${MSVC_WINE_DIR}" CACHE PATH "msvc-wine install prefix")

if(NOT DEFINED MSVC_WINE_ARCH OR MSVC_WINE_ARCH STREQUAL "")
    set(MSVC_WINE_ARCH "x64")
endif()
set(MSVC_WINE_ARCH "${MSVC_WINE_ARCH}" CACHE STRING "msvc-wine wrapper arch (x64/x86/arm/arm64)")

set(_mw_bin "${MSVC_WINE_DIR}/bin/${MSVC_WINE_ARCH}")

if(NOT EXISTS "${_mw_bin}/cl")
    message(FATAL_ERROR
        "msvc-wine compiler wrapper not found:\n"
        "    ${_mw_bin}/cl\n\n"
        "Install the toolchain first:\n"
        "    linux-build/bootstrap-msvc-wine.sh --prefix \"${MSVC_WINE_DIR}\"\n\n"
        "Or point CMake at an existing install:\n"
        "    -DMSVC_WINE_DIR=/path/to/msvc-wine")
endif()

# ---- Toolchain binaries (the Wine wrapper scripts) --------------------------
set(CMAKE_C_COMPILER   "${_mw_bin}/cl")
set(CMAKE_CXX_COMPILER "${_mw_bin}/cl")
set(CMAKE_RC_COMPILER  "${_mw_bin}/rc")
set(CMAKE_AR           "${_mw_bin}/lib")
set(CMAKE_MT           "${_mw_bin}/mt")
set(CMAKE_LINKER       "${_mw_bin}/link")

# The wrappers set INCLUDE / LIB / PATH themselves, so no /imsvc, /libpath, or
# target triple is needed here (unlike an xwin + clang-cl setup).

# ---- Expose the MSVC/SDK system include dirs to CMake -----------------------
# The wrappers hand the STL/SDK headers to cl.exe via an INCLUDE env var set at
# runtime, so CMake (and thus compile_commands.json / IDEs like CLion) never learn
# where <cstdint> etc. live, producing spurious "cannot find file" errors. Source
# the wrapper's env script, translate the Windows-form paths (z:\a\b) back to Unix,
# and register the ones that exist as standard include dirs. This is additive to the
# INCLUDE env the wrappers still set, so the actual build is unaffected.
if(EXISTS "${_mw_bin}/msvcenv.sh")
    execute_process(
        COMMAND bash -c ". '${_mw_bin}/msvcenv.sh' >/dev/null 2>&1 && printf '%s' \"$INCLUDE\""
        OUTPUT_VARIABLE _mw_include_raw)
    string(REGEX REPLACE "[zZ]:" "" _mw_include_raw "${_mw_include_raw}")
    string(REPLACE "\\" "/" _mw_include_raw "${_mw_include_raw}")
    set(_mw_sys_includes "")
    foreach(_d IN LISTS _mw_include_raw)
        if(_d AND IS_DIRECTORY "${_d}")
            list(APPEND _mw_sys_includes "${_d}")
        endif()
    endforeach()
    if(_mw_sys_includes)
        set(CMAKE_C_STANDARD_INCLUDE_DIRECTORIES   "${_mw_sys_includes}")
        set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES "${_mw_sys_includes}")
    endif()
endif()

# The downloaded toolchain ships only the release CRT, so let compiler detection
# compile-only (a test executable would try to link the debug dynamic CRT).
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Keep host tools (cmake, ninja) discoverable; libraries/headers come from Wine.
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE BOTH)
