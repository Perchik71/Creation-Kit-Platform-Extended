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

void _FATALERROR(const char* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->Log(
		CreationKitPlatformExtended::PluginAPI::vmlFatalError, fmt, args);
	va_end(args);

	TerminateProcess(GetCurrentProcess(), 1);
	__assume(0);
}

void _ERROR(const char* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->Log(
		CreationKitPlatformExtended::PluginAPI::vmlError, fmt, args);
	va_end(args);
}

void _WARNING(const char* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->Log(
		CreationKitPlatformExtended::PluginAPI::vmlWarning, fmt, args);
	va_end(args);
}

void _MESSAGE(const char* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->Log(
		CreationKitPlatformExtended::PluginAPI::vmlMessage, fmt, args);
	va_end(args);
}

void _DMESSAGE(const char* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->Log(
		CreationKitPlatformExtended::PluginAPI::vmlDebugMessage, fmt, args);
	va_end(args);
}

void _FATALERROR(const wchar_t* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->Log(
		CreationKitPlatformExtended::PluginAPI::vmlFatalError, fmt, args);
	va_end(args);

	TerminateProcess(GetCurrentProcess(), 1);
	__assume(0);
}

void _ERROR(const wchar_t* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->Log(
		CreationKitPlatformExtended::PluginAPI::vmlError, fmt, args);
	va_end(args);
}

void _WARNING(const wchar_t* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->Log(
		CreationKitPlatformExtended::PluginAPI::vmlWarning, fmt, args);
	va_end(args);
}

void _MESSAGE(const wchar_t* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->Log(
		CreationKitPlatformExtended::PluginAPI::vmlMessage, fmt, args);
	va_end(args);
}

void _DMESSAGE(const wchar_t* fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	CreationKitPlatformExtended::PluginAPI::DebugLog::Instance->Log(
		CreationKitPlatformExtended::PluginAPI::vmlDebugMessage, fmt, args);
	va_end(args);
}