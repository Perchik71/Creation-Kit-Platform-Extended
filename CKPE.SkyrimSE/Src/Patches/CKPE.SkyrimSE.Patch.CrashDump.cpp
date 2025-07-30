// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.CrashHandler.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashDump.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			CrashDump::CrashDump() : Common::Patch()
			{
				SetName("Crash Dump");
			}

			bool CrashDump::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* CrashDump::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bGenerateCrashdumps";
			}

			bool CrashDump::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CrashDump::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CrashDump::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashDump::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				Common::CrashHandler::GetSingleton()->Install();

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				auto purecallHandler = []()
					{
						RaiseException('PURE', EXCEPTION_NONCONTINUABLE, 0, NULL);
					};

				auto terminateHandler = []()
					{
						RaiseException('TERM', EXCEPTION_NONCONTINUABLE, 0, NULL);
					};

				auto patchIAT = [&terminateHandler, &purecallHandler, &base](const char* module)
					{
						Detours::DetourIAT(base, module, "_cexit", (uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, module, "_exit", (uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, module, "_Exit", (uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, module, "_c_exit", (uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, module, "exit", (uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, module, "abort", (uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, module, "terminate", (uintptr_t)(void(*)())terminateHandler);
						Detours::DetourIAT(base, module, "_purecall", (uintptr_t)(void(*)())purecallHandler);
					};

				// Патчим все известные библы используемые в CK

				patchIAT("API-MS-WIN-CRT-RUNTIME-L1-1-0.DLL");
				patchIAT("MSVCR110.DLL");

				SafeWrite::Write(__CKPE_OFFSET(0), { 0xC3 });	// StackTrace::MemoryTraceWrite
				SafeWrite::Write(__CKPE_OFFSET(1), { 0xC3 });	// SetUnhandledExceptionFilter, BSWin32ExceptionHandler
				SafeWrite::Write(__CKPE_OFFSET(2), { 0xC3 });	// SetUnhandledExceptionFilter, Unknown
				SafeWrite::WriteNop(__CKPE_OFFSET(3), 6);		// SetUnhandledExceptionFilter, BSWin32ExceptionHandler

				return true;
			}
		}
	}
}