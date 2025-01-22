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

#include "Voltek.MemoryManager.h"
#include "Types.h"

#define PROPERTY(read_func, write_func)	__declspec(property(get = read_func, put = write_func))
#define READ_PROPERTY(read_func)		__declspec(property(get = read_func))
#define READ_PROPERTY2(type, Name, Var) \
	__forceinline type Get##Name(VOID) const { return Var; } \
	__declspec(property(get = Get##Name)) type Name

#ifdef _ASSERT
#undef _ASSERT
#endif // _ASSERT

void __stdcall _FATALERROR(const char* fmt, ...);
void __stdcall _ERROR(const char* fmt, ...);
void __stdcall _WARNING(const char* fmt, ...);
void __stdcall _MESSAGE(const char* fmt, ...);
void __stdcall _DMESSAGE(const char* fmt, ...);
void __stdcall _FATALERROR(const wchar_t* fmt, ...);
void __stdcall _ERROR(const wchar_t* fmt, ...);
void __stdcall _WARNING(const wchar_t* fmt, ...);
void __stdcall _MESSAGE(const wchar_t* fmt, ...);
void __stdcall _DMESSAGE(const wchar_t* fmt, ...);
void __stdcall __ASSERT(const char* File, int Line, const char* Format, ...);
void __stdcall __ASSERT(const wchar_t* File, int Line, const wchar_t* Format, ...);

#define _ASSERT(Cond)				if(!(Cond)) __ASSERT(__FILE__, __LINE__, #Cond)
#define _ASSERTMSG(Cond, Msg, ...)	if(!(Cond)) __ASSERT(__FILE__, __LINE__, "%s\n\n" Msg, #Cond, ##__VA_ARGS__)

#define Assert(Cond)				if(!(Cond)) __ASSERT(__FILE__, __LINE__, #Cond);
#define AssertMsgVa(Cond, Msg, ...)	if(!(Cond)) __ASSERT(__FILE__, __LINE__, "%s\n\n" Msg, #Cond, ##__VA_ARGS__);
#define AssertMsg(Cond, Msg)		AssertMsgVa(Cond, Msg)

// thread-safe template versions of thisVirtualCall()

template<typename TR>
__forceinline TR thisVirtualCall(size_t reloff, const void* ths) {
	return (*(TR(__fastcall**)(const void*))(*(__int64*)ths + reloff))(ths);
}

template<typename TR, typename T1>
__forceinline TR thisVirtualCall(size_t reloff, const void* ths, T1 a1) {
	return (*(TR(__fastcall**)(const void*, T1))(*(__int64*)ths + reloff))(ths, a1);
}

template<typename TR, typename T1, typename T2>
__forceinline TR thisVirtualCall(size_t reloff, const void* ths, T1 a1, T2 a2) {
	return (*(TR(__fastcall**)(const void*, T1, T2))(*(__int64*)ths + reloff))(ths, a1, a2);
}

template<typename TR, typename T1, typename T2, typename T3>
__forceinline TR thisVirtualCall(size_t reloff, const void* ths, T1 a1, T2 a2, T3 a3) {
	return (*(TR(__fastcall**)(const void*, T1, T2, T3))(*(__int64*)ths + reloff))(ths, a1, a2, a3);
}

template<typename TR, typename T1, typename T2, typename T3, typename T4>
__forceinline TR thisVirtualCall(size_t reloff, const void* ths, T1 a1, T2 a2, T3 a3, T4 a4) {
	return (*(TR(__fastcall**)(const void*, T1, T2, T3, T4))(*(__int64*)ths + reloff))(ths, a1, a2, a3, a4);
}

//
// All these functions use the full address, not the rva

// thread-safe template versions of fastCall()

template<typename TR>
__forceinline TR fastCall(size_t reloff) {
	return ((TR(__fastcall*)())(reloff))();
}

template<typename TR, typename T1>
__forceinline TR fastCall(size_t reloff, T1 a1) {
	return ((TR(__fastcall*)(T1))(reloff))(a1);
}

template<typename TR, typename T1, typename T2>
__forceinline TR fastCall(size_t reloff, T1 a1, T2 a2) {
	return ((TR(__fastcall*)(T1, T2))(reloff))(a1, a2);
}

template<typename TR, typename T1, typename T2, typename T3>
__forceinline TR fastCall(size_t reloff, T1 a1, T2 a2, T3 a3) {
	return ((TR(__fastcall*)(T1, T2, T3))(reloff))(a1, a2, a3);
}

template<typename TR, typename T1, typename T2, typename T3, typename T4>
__forceinline TR fastCall(size_t reloff, T1 a1, T2 a2, T3 a3, T4 a4) {
	return ((TR(__fastcall*)(T1, T2, T3, T4))(reloff))(a1, a2, a3, a4);
}

template<typename TR, typename T1, typename T2, typename T3, typename T4, typename T5>
__forceinline TR fastCall(size_t reloff, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5) {
	return ((TR(__fastcall*)(T1, T2, T3, T4, T5))(reloff))(a1, a2, a3, a4, a5);
}

template<typename TR, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
__forceinline TR fastCall(size_t reloff, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6) {
	return ((TR(__fastcall*)(T1, T2, T3, T4, T5, T6))(reloff))(a1, a2, a3, a4, a5, a6);
}

template<typename TR, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
__forceinline TR fastCall(size_t reloff, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7) {
	return ((TR(__fastcall*)(T1, T2, T3, T4, T5, T6, T7))(reloff))(a1, a2, a3, a4, a5, a6, a7);
}

template<typename TR, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
__forceinline TR fastCall(size_t reloff, T1 a1, T2 a2, T3 a3, T4 a4, T5 a5, T6 a6, T7 a7, T8 a8) {
	return ((TR(__fastcall*)(T1, T2, T3, T4, T5, T6, T7, T8))(reloff))(a1, a2, a3, a4, a5, a6, a7, a8);
}

// thread-safe template versions of thisCall()

template<typename TR>
__forceinline TR thisCall(size_t reloff, const void* ths) {
	return ((TR(__fastcall*)(const void*))(reloff))(ths);
}

template<typename TR, typename T1>
__forceinline TR thisCall(size_t reloff, const void* ths, T1 a1) {
	return ((TR(__fastcall*)(const void*, T1))(reloff))(ths, a1);
}

template<typename TR, typename T1, typename T2>
__forceinline TR thisCall(size_t reloff, const void* ths, T1 a1, T2 a2) {
	return ((TR(__fastcall*)(const void*, T1, T2))(reloff))(ths, a1, a2);
}

template<typename TR, typename T1, typename T2, typename T3>
__forceinline TR thisCall(size_t reloff, const void* ths, T1 a1, T2 a2, T3 a3) {
	return ((TR(__fastcall*)(const void*, T1, T2, T3))(reloff))(ths, a1, a2, a3);
}

template<typename TR, typename T1, typename T2, typename T3, typename T4>
__forceinline TR thisCall(size_t reloff, const void* ths, T1 a1, T2 a2, T3 a3, T4 a4) {
	return ((TR(__fastcall*)(const void*, T1, T2, T3, T4))(reloff))(ths, a1, a2, a3, a4);
}