// This tutorial is about using the CKPE plugin system.
// To clarify how you can use this and how to write your plugins.
// 
// LICENSE
// This file for a test project, and serves as a guide for creating plugins.
// 
// You can also use the assets of this test project, some platform classes are passed with their interfaces 
// without implementation, so that they can be used inside the plugin, while you will not necessarily need 
// to open the code to the public. But you cannot use CKPE assets, in which case you must disclose 
// the code and indicate authorship. And also you can't re-license, only GPLv3.

#pragma once

#pragma warning (disable : 4244)
#pragma warning (disable : 4477)
#pragma warning (disable : 4996)
#pragma warning (disable : 26812)

#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <shlobj_core.h>
#include <strsafe.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <wchar.h>
#include <errno.h>
#include <stdint.h>
#include <intrin.h>
#include <xmmintrin.h>

#include <atomic>
#include <algorithm>
#include <fstream>
#include <memory>
#include <version>
#include <vector>
#include <list>
#include <execution>
#include <chrono>
#include <array>
#include <map>
#include <set>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <initializer_list>

#include <concurrent_vector.h>
#include <concurrent_unordered_map.h>

#include "vmm.h"
#include "Types.h"

#define PROPERTY(read_func, write_func)	__declspec(property(get = read_func, put = write_func))
#define READ_PROPERTY(read_func)		__declspec(property(get = read_func))
#define READ_PROPERTY2(type, Name, Var) \
	__forceinline type Get##Name(VOID) const { return Var; } \
	__declspec(property(get = Get##Name)) type Name

void _FATALERROR(const char* fmt, ...);
void _ERROR(const char* fmt, ...);
void _WARNING(const char* fmt, ...);
void _MESSAGE(const char* fmt, ...);
void _DMESSAGE(const char* fmt, ...);
void _FATALERROR(const wchar_t* fmt, ...);
void _ERROR(const wchar_t* fmt, ...);
void _WARNING(const wchar_t* fmt, ...);
void _MESSAGE(const wchar_t* fmt, ...);
void _DMESSAGE(const wchar_t* fmt, ...);