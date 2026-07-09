# ============================================================================
# CKPE Linux cross-build with msvc-wine (real MSVC under Wine)
#
# This is a parallel build tree, independent of the Visual Studio .sln/.vcxproj;
# it never modifies the shared sources. It reproduces the MSBuild Release|x64
# build using the genuine Microsoft toolchain running under Wine.
#
#   linux-build/bootstrap-msvc-wine.sh        # install the toolchain (once)
#   cmake --preset msvc-wine                  # configure
#   cmake --build build-linux                 # build
#
# All per-target settings below are taken verbatim from each project's
# .vcxproj (Release|x64). Source file lists are parsed directly from the
# .vcxproj so they stay in sync with the MSBuild build.
# ============================================================================
cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0091 NEW)   # MSVC runtime library selectable via property

project(ckpe_linux_crossbuild CXX C RC)

# Emit compile_commands.json for IDEs (CLion, clangd) so they can resolve headers.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(FATAL_ERROR
        "This project must be configured with the msvc-wine toolchain.\n"
        "Use:  cmake --preset msvc-wine\n"
        "  or: -DCMAKE_TOOLCHAIN_FILE=cmake/msvc-wine-toolchain.cmake")
endif()

# msvc-wine ships ONLY the release CRT and every .vcxproj is Release|x64, so this
# cross-build is Release-only. Coerce anything else (incl. a Debug default) to
# Release, otherwise Debug's /RTC1 /Od collide with our /O2 and the debug CRT is
# absent at link time.
if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE STREQUAL "Release")
    if(CMAKE_BUILD_TYPE AND NOT CMAKE_BUILD_TYPE STREQUAL "")
        message(WARNING "Forcing CMAKE_BUILD_TYPE=Release (was '${CMAKE_BUILD_TYPE}'); this cross-build is Release-only.")
    endif()
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build configuration (Release-only)" FORCE)
endif()

# Static release CRT (/MT), matching RuntimeLibrary=MultiThreaded in every vcxproj.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")

get_filename_component(CKPE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)

# Release (/arch:AVX, PluginAPI /arch:AVX2) vs Release-NoAVX2 (/arch:SSE2).
option(CKPE_ENABLE_AVX2 "Build the AVX variant (OFF = NoAVX2 / SSE2)" ON)
if(CKPE_ENABLE_AVX2)
    set(CKPE_ARCH_FLAG   "/arch:AVX")
    set(CKPE_ARCH_FLAG_2 "/arch:AVX2")
else()
    set(CKPE_ARCH_FLAG   "/arch:SSE2")
    set(CKPE_ARCH_FLAG_2 "/arch:SSE2")
endif()

# ----------------------------------------------------------------------------
# Parse <ClCompile>/<ResourceCompile> source lists from a .vcxproj so the build
# mirrors MSBuild exactly (the projects list sources explicitly, no globs, no
# excludes). Paths are resolved relative to the .vcxproj's own directory.
# ----------------------------------------------------------------------------
function(ckpe_vcxproj_sources out_var proj)
    get_filename_component(_base "${proj}" DIRECTORY)
    file(READ "${proj}" _content)
    set(_srcs "")
    string(REGEX MATCHALL "<(ClCompile|ResourceCompile) Include=\"[^\"]+\"" _matches "${_content}")
    foreach(_m IN LISTS _matches)
        string(REGEX REPLACE "^<[A-Za-z]+ Include=\"(.*)\"$" "\\1" _p "${_m}")
        string(REPLACE "\\" "/" _p "${_p}")
        if(NOT IS_ABSOLUTE "${_p}")
            set(_p "${_base}/${_p}")
        endif()
        get_filename_component(_p "${_p}" ABSOLUTE)
        list(APPEND _srcs "${_p}")
    endforeach()
    if(NOT _srcs)
        message(FATAL_ERROR "No sources parsed from ${proj}")
    endif()
    set(${out_var} "${_srcs}" PARENT_SCOPE)
endfunction()

# ----------------------------------------------------------------------------
# Apply the shared compile/link flags. Options:
#   STDCALL      -> /Gz   (CallingConvention=StdCall)
#   UNICODE      -> UNICODE/_UNICODE (CharacterSet=Unicode)
#   ARCH <flag>  -> enhanced instruction set (defaults to CKPE_ARCH_FLAG)
# C++-only flags are gated so C sources don't get /std:c++latest.
# ----------------------------------------------------------------------------
function(ckpe_apply_flags tgt)
    cmake_parse_arguments(A "STDCALL;UNICODE" "ARCH" "" ${ARGN})
    if(NOT A_ARCH)
        set(A_ARCH "${CKPE_ARCH_FLAG}")
    endif()
    # Flags mirror each .vcxproj Release|x64 (do NOT add flags the vcxproj lacks,
    # e.g. /Zc:__cplusplus, which would flip __cplusplus-guarded code paths).
    # /bigobj is the one addition: purely raises the object section limit for the
    # large template-heavy TUs, with no semantic effect.
    target_compile_options(${tgt} PRIVATE
        $<$<COMPILE_LANGUAGE:CXX>:/std:c++latest /permissive- /EHsc /GR>
        /O2 /Gy /Oi /Ot /fp:fast /bigobj /nologo "${A_ARCH}")
    if(A_STDCALL)
        target_compile_options(${tgt} PRIVATE /Gz)
    endif()
    if(A_UNICODE)
        target_compile_definitions(${tgt} PRIVATE UNICODE _UNICODE)
    endif()
    target_compile_definitions(${tgt} PRIVATE NDEBUG)
    target_link_options(${tgt} PRIVATE /OPT:REF /OPT:ICF /INCREMENTAL:NO /NOLOGO)
endfunction()

set(DXTEX_LIB "${CKPE_ROOT}/Dependencies/DirectXTex/lib/DirectXTex.lib")
set(QT5_LIBDIR "${CKPE_ROOT}/Dependencies/qt5/lib")

# ============================================================================
# Dependency static libraries
# ============================================================================

# ---- detours (Detours + bundled zydis) -------------------------------------
ckpe_vcxproj_sources(DETOURS_SRC "${CKPE_ROOT}/Dependencies/detours.vcxproj")
add_library(detours STATIC ${DETOURS_SRC})
set_target_properties(detours PROPERTIES OUTPUT_NAME detours PREFIX "")
target_include_directories(detours PRIVATE
    "${CKPE_ROOT}/Dependencies/Detours/detours/zydis/msvc"
    "${CKPE_ROOT}/Dependencies/Detours/detours/zydis/src"
    "${CKPE_ROOT}/Dependencies/Detours/detours/zydis/dependencies/zycore/include"
    "${CKPE_ROOT}/Dependencies/Detours/detours/zydis/include")
target_compile_definitions(detours PRIVATE ZYDIS_STATIC_DEFINE WIN32 _LIB UNICODE _UNICODE)
ckpe_apply_flags(detours)

# ---- libzip (mzip / miniz) -------------------------------------------------
ckpe_vcxproj_sources(LIBZIP_SRC "${CKPE_ROOT}/Dependencies/Zipper.vcxproj")
add_library(libzip STATIC ${LIBZIP_SRC})
set_target_properties(libzip PROPERTIES OUTPUT_NAME libzip PREFIX "")
target_include_directories(libzip PRIVATE "${CKPE_ROOT}/Dependencies/mzip/src")
target_compile_definitions(libzip PRIVATE _LIB _CRT_SECURE_NO_WARNINGS)
ckpe_apply_flags(libzip)

# ---- Toml (toml11) ---------------------------------------------------------
ckpe_vcxproj_sources(TOML_SRC "${CKPE_ROOT}/Dependencies/Toml.vcxproj")
add_library(Toml STATIC ${TOML_SRC})
set_target_properties(Toml PROPERTIES OUTPUT_NAME Toml PREFIX "")
target_include_directories(Toml PRIVATE "${CKPE_ROOT}/Dependencies/toml11/include")
target_compile_definitions(Toml PRIVATE _LIB TOML11_COMPILE_SOURCES)
ckpe_apply_flags(Toml)

# ---- libzydis --------------------------------------------------------------
ckpe_vcxproj_sources(ZYDIS_SRC "${CKPE_ROOT}/Dependencies/libzydis.vcxproj")
add_library(libzydis STATIC ${ZYDIS_SRC})
set_target_properties(libzydis PROPERTIES OUTPUT_NAME libzydis PREFIX "")
target_include_directories(libzydis PRIVATE
    "${CKPE_ROOT}/Dependencies/zydis/include"
    "${CKPE_ROOT}/Dependencies/zydis/msvc"
    "${CKPE_ROOT}/Dependencies/zydis/src")
target_compile_definitions(libzydis PRIVATE _LIB ZYDIS_STATIC_DEFINE)
ckpe_apply_flags(libzydis)

# ---- libdeflate ------------------------------------------------------------
ckpe_vcxproj_sources(DEFLATE_SRC "${CKPE_ROOT}/Dependencies/libdeflate.vcxproj")
add_library(libdeflate STATIC ${DEFLATE_SRC})
set_target_properties(libdeflate PROPERTIES OUTPUT_NAME libdeflate PREFIX "")
target_include_directories(libdeflate PRIVATE
    "${CKPE_ROOT}/Dependencies/libdeflate"
    "${CKPE_ROOT}/Dependencies/libdeflate/common")
target_compile_definitions(libdeflate PRIVATE _LIB DFREESTANDING)
ckpe_apply_flags(libdeflate)

# ---- ImGUI (core + dx11/dx12/win32 backends) -------------------------------
ckpe_vcxproj_sources(IMGUI_SRC "${CKPE_ROOT}/Dependencies/ImGUI/ImGUI.vcxproj")
add_library(ImGUI STATIC ${IMGUI_SRC})
set_target_properties(ImGUI PROPERTIES OUTPUT_NAME ImGUI PREFIX "")
target_include_directories(ImGUI PRIVATE "${CKPE_ROOT}/Dependencies/ImGUI")
target_compile_definitions(ImGUI PRIVATE _LIB)
ckpe_apply_flags(ImGUI STDCALL)

# ---- vmm (Voltek memory manager) -------------------------------------------
ckpe_vcxproj_sources(VMM_SRC "${CKPE_ROOT}/Dependencies/vmm/vmm.vcxproj")
add_library(vmm STATIC ${VMM_SRC})
set_target_properties(vmm PROPERTIES OUTPUT_NAME vmm PREFIX "")
target_include_directories(vmm PRIVATE "${CKPE_ROOT}/Dependencies/vmm/include")
target_compile_definitions(vmm PRIVATE
    NOMINMAX WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS _WINDOWS VOLTEK_LIB_BUILD)
ckpe_apply_flags(vmm ARCH "/arch:SSE2")   # vmm.vcxproj uses SSE2 explicitly

# ============================================================================
# Primary artifacts
# ============================================================================

# ---- ckpe_loader.exe (Console, Unicode) ------------------------------------
ckpe_vcxproj_sources(LOADER_SRC "${CKPE_ROOT}/CKPE.Loader/ckpe_loader.vcxproj")
add_executable(ckpe_loader ${LOADER_SRC})
set_target_properties(ckpe_loader PROPERTIES OUTPUT_NAME ckpe_loader)
target_include_directories(ckpe_loader PRIVATE "${CKPE_ROOT}/CKPE.Loader")
target_compile_definitions(ckpe_loader PRIVATE _CONSOLE _CRT_SECURE_NO_WARNINGS)
ckpe_apply_flags(ckpe_loader UNICODE)
target_link_options(ckpe_loader PRIVATE /SUBSYSTEM:CONSOLE)

# ---- CKPE.dll (MultiByte) --------------------------------------------------
ckpe_vcxproj_sources(CKPE_SRC "${CKPE_ROOT}/CKPE/CKPE.vcxproj")
add_library(CKPE SHARED ${CKPE_SRC})
set_target_properties(CKPE PROPERTIES OUTPUT_NAME CKPE PREFIX "")
target_include_directories(CKPE PRIVATE
    "${CKPE_ROOT}/CKPE" "${CKPE_ROOT}/CKPE/Include"
    "${CKPE_ROOT}/Dependencies/iw" "${CKPE_ROOT}/Dependencies/Detours"
    "${CKPE_ROOT}/Dependencies/mzip/src")
target_compile_definitions(CKPE PRIVATE
    CKPE_EXPORTS _WINDOWS _USRDLL WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS
    _SILENCE_CXX23_DENORM_DEPRECATION_WARNING _SILENCE_ALL_CXX23_DEPRECATION_WARNINGS)
ckpe_apply_flags(CKPE STDCALL)
target_link_libraries(CKPE PRIVATE detours libzip
    Shell32.lib msimg32.lib gdiplus.lib comctl32.lib shlwapi.lib Netapi32.lib)

# ---- winhttp.dll (proxy, MultiByte) ----------------------------------------
ckpe_vcxproj_sources(WINHTTP_SRC "${CKPE_ROOT}/CKPE.WinHttp/CKPE.WinHttp.vcxproj")
add_library(winhttp SHARED ${WINHTTP_SRC})
set_target_properties(winhttp PROPERTIES OUTPUT_NAME winhttp PREFIX "")
target_include_directories(winhttp PRIVATE
    "${CKPE_ROOT}/CKPE/Include" "${CKPE_ROOT}/CKPE.WinHttp")
target_compile_definitions(winhttp PRIVATE
    CKPEWINHTTP_EXPORTS _WINDOWS _USRDLL WIN32_LEAN_AND_MEAN NOMINMAX
    CREATIOKITPLATFORMEXTENDEDCORE_EXPORTS
    _SILENCE_CXX23_DENORM_DEPRECATION_WARNING _SILENCE_ALL_CXX23_DEPRECATION_WARNINGS)
ckpe_apply_flags(winhttp STDCALL)
target_link_libraries(winhttp PRIVATE CKPE)

# ---- CKPE.Common.dll (MultiByte) -------------------------------------------
ckpe_vcxproj_sources(COMMON_SRC "${CKPE_ROOT}/CKPE.Common/CKPE.Common.vcxproj")
add_library(CKPE.Common SHARED ${COMMON_SRC})
set_target_properties(CKPE.Common PROPERTIES OUTPUT_NAME CKPE.Common PREFIX "")
target_include_directories(CKPE.Common PRIVATE
    "${CKPE_ROOT}/Dependencies/DirectXTex/include"
    "${CKPE_ROOT}/CKPE/Include" "${CKPE_ROOT}/CKPE.Common/Include"
    "${CKPE_ROOT}/Dependencies/zydis/msvc" "${CKPE_ROOT}/Dependencies/zydis/include"
    "${CKPE_ROOT}/Dependencies/toml11/include" "${CKPE_ROOT}/Dependencies/jDialogs/include"
    "${CKPE_ROOT}/Dependencies/mzip/src" "${CKPE_ROOT}/CKPE.Common"
    "${CKPE_ROOT}/Dependencies/vmm/include")
target_compile_definitions(CKPE.Common PRIVATE
    ZYDIS_STATIC_DEFINE CKPECOMMON_EXPORTS _WINDOWS _USRDLL WIN32_LEAN_AND_MEAN NOMINMAX
    _CRT_SECURE_NO_WARNINGS CKPE_NO_LOGGER_FUNCTION VOLTEK_LIB_BUILD
    $<$<BOOL:${CKPE_ENABLE_AVX2}>:MIMALLOC_PURGE_DECOMMITS=1>
    _SILENCE_CXX23_DENORM_DEPRECATION_WARNING _SILENCE_ALL_CXX23_DEPRECATION_WARNINGS)
ckpe_apply_flags(CKPE.Common STDCALL)
target_link_libraries(CKPE.Common PRIVATE
    CKPE Toml libzydis libzip vmm "${DXTEX_LIB}"
    Shell32.lib comctl32.lib msimg32.lib uxtheme.lib gdiplus.lib shlwapi.lib
    dwmapi.lib dbghelp.lib dxgi.lib d3d11.lib d3dcompiler.lib dxguid.lib)

# ---- CKPE.PluginAPI.dll (Unicode, /arch:AVX2) ------------------------------
ckpe_vcxproj_sources(PLUGINAPI_SRC "${CKPE_ROOT}/CKPE.PluginAPI/CKPE.PluginAPI.vcxproj")
add_library(CKPE.PluginAPI SHARED ${PLUGINAPI_SRC})
set_target_properties(CKPE.PluginAPI PROPERTIES OUTPUT_NAME CKPE.PluginAPI PREFIX "")
target_include_directories(CKPE.PluginAPI PRIVATE
    "${CKPE_ROOT}/CKPE/Include" "${CKPE_ROOT}/CKPE.PluginAPI/Include"
    "${CKPE_ROOT}/CKPE.PluginAPI" "${CKPE_ROOT}/CKPE.Common/Include"
    "${CKPE_ROOT}/Dependencies/jDialogs/include")
target_compile_definitions(CKPE.PluginAPI PRIVATE
    CKPEPLUGINAPI_EXPORTS _WINDOWS _USRDLL WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS
    _SILENCE_CXX23_DENORM_DEPRECATION_WARNING _SILENCE_ALL_CXX23_DEPRECATION_WARNINGS)
ckpe_apply_flags(CKPE.PluginAPI STDCALL UNICODE ARCH "${CKPE_ARCH_FLAG_2}")
target_link_libraries(CKPE.PluginAPI PRIVATE CKPE CKPE.Common)

# ---- Game modules: Fallout4 & SkyrimSE (MultiByte) -------------------------
function(ckpe_add_game name export_def)
    ckpe_vcxproj_sources(_src "${CKPE_ROOT}/${name}/${name}.vcxproj")
    add_library(${name} SHARED ${_src})
    set_target_properties(${name} PROPERTIES OUTPUT_NAME "${name}" PREFIX "")
    target_include_directories(${name} PRIVATE
        "${CKPE_ROOT}/Dependencies/DirectXTex/include"
        "${CKPE_ROOT}/CKPE/Include" "${CKPE_ROOT}/${name}/Include"
        "${CKPE_ROOT}/CKPE.Common/Include" "${CKPE_ROOT}/Dependencies/xbyak"
        "${CKPE_ROOT}/${name}" "${CKPE_ROOT}/Dependencies/ImGUI"
        "${CKPE_ROOT}/Dependencies/libdeflate" "${CKPE_ROOT}/CKPE.PluginAPI/Include")
    target_compile_definitions(${name} PRIVATE
        ${export_def} _WINDOWS _USRDLL WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS
        CKPE_NO_LOGGER_FUNCTION XBYAK_NO_OP_NAMES
        _SILENCE_CXX23_DENORM_DEPRECATION_WARNING _SILENCE_ALL_CXX23_DEPRECATION_WARNINGS)
    ckpe_apply_flags(${name} STDCALL)
    target_link_libraries(${name} PRIVATE
        CKPE CKPE.Common CKPE.PluginAPI ImGUI libdeflate "${DXTEX_LIB}"
        comctl32.lib dxgi.lib d3d11.lib d3dcompiler.lib dxguid.lib shlwapi.lib)
endfunction()

ckpe_add_game(CKPE.Fallout4 CKPEFALLOUT4_EXPORTS)
ckpe_add_game(CKPE.SkyrimSE CKPESKYRIMSE_EXPORTS)

# ---- CKPE.Starfield.dll (MultiByte, Qt5 link-only) -------------------------
ckpe_vcxproj_sources(SF_SRC "${CKPE_ROOT}/CKPE.Starfield/CKPE.Starfield.vcxproj")
add_library(CKPE.Starfield SHARED ${SF_SRC})
set_target_properties(CKPE.Starfield PROPERTIES OUTPUT_NAME CKPE.Starfield PREFIX "")
target_include_directories(CKPE.Starfield PRIVATE
    "${CKPE_ROOT}/Dependencies/DirectXTex/include"
    "${CKPE_ROOT}/CKPE/Include" "${CKPE_ROOT}/CKPE.Starfield/Include"
    "${CKPE_ROOT}/CKPE.Starfield" "${CKPE_ROOT}/CKPE.Common/Include"
    "${CKPE_ROOT}/Dependencies/xbyak" "${CKPE_ROOT}/CKPE.PluginAPI/Include"
    "${CKPE_ROOT}/Dependencies/qt5")
target_compile_definitions(CKPE.Starfield PRIVATE
    CKPESTARFIELD_EXPORTS _WINDOWS _USRDLL WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS
    CKPE_NO_LOGGER_FUNCTION XBYAK_NO_OP_NAMES
    _SILENCE_CXX23_DENORM_DEPRECATION_WARNING _SILENCE_ALL_CXX23_DEPRECATION_WARNINGS)
ckpe_apply_flags(CKPE.Starfield STDCALL)
# Qt 5.12 headers vs /std:c++latest can emit C5054 (enum-enum ops); demote to non-fatal.
target_compile_options(CKPE.Starfield PRIVATE /wd5054)
# Qt 5.12 uses stdext::make_[un]checked_array_iterator, which recent MSVC STLs
# removed. Force-include a shim that restores them on affected toolsets only.
target_compile_options(CKPE.Starfield PRIVATE
    "/FI${CMAKE_CURRENT_SOURCE_DIR}/shim/qt5_stdext_compat.h")
target_link_libraries(CKPE.Starfield PRIVATE
    CKPE CKPE.Common CKPE.PluginAPI ImGUI libdeflate "${DXTEX_LIB}"
    comctl32.lib dxgi.lib d3d11.lib d3dcompiler.lib dxguid.lib shlwapi.lib
    "${QT5_LIBDIR}/Qt5Core.lib" "${QT5_LIBDIR}/Qt5Gui.lib" "${QT5_LIBDIR}/Qt5Widgets.lib")

message(STATUS "CKPE msvc-wine cross-build configured (AVX2=${CKPE_ENABLE_AVX2}). MSVC_WINE_DIR=${MSVC_WINE_DIR}")
