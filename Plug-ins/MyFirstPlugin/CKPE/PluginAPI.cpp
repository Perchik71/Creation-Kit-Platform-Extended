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

#include "PluginAPI.h"

using namespace CreationKitPlatformExtended;
using namespace CreationKitPlatformExtended::PluginAPI;

void __stdcall __ASSERT(const char* File, int Line, const char* Format, ...)
{
	CHAR buffer[1024] = { 0 };
	CHAR message[1024] = { 0 };

	va_list ap;
	va_start(ap, Format);
	vsprintf_s(buffer, _TRUNCATE, Format, ap);
	sprintf_s(message, "%s(%d):\n\n%s", File, Line, buffer);
	va_end(ap);

	MessageBoxA(nullptr, message, "ASSERTION", MB_ICONERROR);

	if (IsDebuggerPresent()) {
		OutputDebugStringA(message);
		__debugbreak();
	}

	TerminateProcess(GetCurrentProcess(), 1);
	__assume(0);
}

void __stdcall __ASSERT(const wchar_t* File, int Line, const wchar_t* Format, ...)
{
	WCHAR buffer[1024] = { 0 };
	WCHAR message[1024] = { 0 };

	va_list ap;
	va_start(ap, Format);
	vswprintf_s(buffer, _TRUNCATE, Format, ap);
	swprintf_s(message, L"%s(%d):\n\n%s", File, Line, buffer);
	va_end(ap);

	MessageBoxW(nullptr, message, L"ASSERTION", MB_ICONERROR);

	if (IsDebuggerPresent()) {
		OutputDebugStringW(message);
		__debugbreak();
	}

	TerminateProcess(GetCurrentProcess(), 1);
	__assume(0);
}

void __stdcall _FATALERROR(const char* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->LogVa(
		CreationKitPlatformExtended::PluginAPI::vmlFatalError, fmt, args);
	va_end(args);

	TerminateProcess(GetCurrentProcess(), 1);
	__assume(0);
}

void __stdcall _ERROR(const char* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->LogVa(
		CreationKitPlatformExtended::PluginAPI::vmlError, fmt, args);
	va_end(args);
}

void __stdcall _WARNING(const char* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->LogVa(
		CreationKitPlatformExtended::PluginAPI::vmlWarning, fmt, args);
	va_end(args);
}

void __stdcall _MESSAGE(const char* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->LogVa(
		CreationKitPlatformExtended::PluginAPI::vmlMessage, fmt, args);
	va_end(args);
}

void __stdcall _DMESSAGE(const char* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->LogVa(
		CreationKitPlatformExtended::PluginAPI::vmlDebugMessage, fmt, args);
	va_end(args);
}

void __stdcall _FATALERROR(const wchar_t* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->LogVa(
		CreationKitPlatformExtended::PluginAPI::vmlFatalError, fmt, args);
	va_end(args);

	TerminateProcess(GetCurrentProcess(), 1);
	__assume(0);
}

void __stdcall _ERROR(const wchar_t* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->LogVa(
		CreationKitPlatformExtended::PluginAPI::vmlError, fmt, args);
	va_end(args);
}

void __stdcall _WARNING(const wchar_t* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->LogVa(
		CreationKitPlatformExtended::PluginAPI::vmlWarning, fmt, args);
	va_end(args);
}

void __stdcall _MESSAGE(const wchar_t* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->LogVa(
		CreationKitPlatformExtended::PluginAPI::vmlMessage, fmt, args);
	va_end(args);
}

void __stdcall _DMESSAGE(const wchar_t* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->LogVa(
		CreationKitPlatformExtended::PluginAPI::vmlDebugMessage, fmt, args);
	va_end(args);
}

bool __stdcall CreationKitPlatformExtended::PluginAPI::AppendMenuItem(HMENU hMenu, uint32_t u32MenuID,
	String Caption, OnMenuItemEvent Function)
{
	if (!Function || !Caption.length() || !hMenu || !u32MenuID)
		return false;

	MENUITEMINFOA mii;
	ZeroMemory(&mii, sizeof(MENUITEMINFOA));

	// Windows OS takes me for an idiot. In general, MIIM_DATA between modules does not seem to work. 
	// Memory is lost, the Platform receives garbage. By the way, the platform receives the string correctly.
	// I don't like it when the system gets impudent.
	//
	// Let's pass the address of the function in the name of the menu item.

	auto len = Caption.length();
	Caption.resize(len + _MAX_U64TOSTR_BASE16_COUNT + 1);
	if (Caption.size() >= MAX_PATH)
		// The string is too big, need a shorter one, state your thoughts correctly
		return false;

	auto Cap = Caption.data();
	// Add seperator
	Cap[len + 0] = '\1';
	// I write the address at the end of the string
	_ui64toa_s((UINT_PTR)Function, Cap + len + 1, _MAX_U64TOSTR_BASE16_COUNT, 16);

	mii.cbSize = sizeof(MENUITEMINFOA);
	mii.fMask = MIIM_STRING | MIIM_DATA | MIIM_ID;
	mii.dwTypeData = (LPSTR)const_cast<char*>(Caption.c_str());
	mii.cch = (UINT)len + _MAX_U64TOSTR_BASE16_COUNT + 1;			// need to pass the whole string
	mii.wID = (UINT)u32MenuID;

	return InsertMenuItemA(hMenu, 0xFFFFFFFF, TRUE, &mii);
}

bool __stdcall CreationKitPlatformExtended::PluginAPI::AppendMenuSeperator(HMENU hMenu, uint32_t u32MenuID)
{
	return AppendMenuA(hMenu, MF_SEPARATOR, u32MenuID, "-");
}

void* __stdcall _DYNAMIC_CAST(const void* InPtr, long VfDelta, const char* lpstrFromType,
	const char* lpstrTargetType, bool isReference)
{
	return CreationKitPlatformExtended::PluginAPI::DynamicCast::Instance->Cast(InPtr, VfDelta, 
		lpstrFromType, lpstrTargetType, isReference);
}

void* __stdcall _DYNAMIC_CAST2(const void* InPtr, long VfDelta, const char* lpstrFromType,
	const char* lpstrTargetType, bool isReference)
{
	return CreationKitPlatformExtended::PluginAPI::DynamicCast::Instance->CastNoCache(InPtr, VfDelta, 
		lpstrFromType, lpstrTargetType, isReference);
}

void __stdcall _CONSOLE(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	_CONSOLEVA(fmt, ap);
	va_end(ap);
}

void __stdcall _CONSOLEVA(const char* fmt, va_list va)
{
	CreationKitPlatformExtended::PluginAPI::ConsoleWindow::Instance->InputLogVa(fmt, va);
}