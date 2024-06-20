// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/CrashHandler.h"
#include "CrashDumpPatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		CrashDumpPatch::CrashDumpPatch() : Module(GlobalEnginePtr)
		{}

		bool CrashDumpPatch::HasOption() const
		{
			return true;
		}

		bool CrashDumpPatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* CrashDumpPatch::GetOptionName() const
		{
			return "CreationKit:bGenerateCrashdumps";
		}

		const char* CrashDumpPatch::GetName() const
		{
			return "Crash Dump";
		}

		bool CrashDumpPatch::HasDependencies() const
		{
			return false;
		}

		Array<String> CrashDumpPatch::GetDependencies() const
		{
			return {};
		}

		bool CrashDumpPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool CrashDumpPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			GlobalCrashHandlerPtr->InstallThreadTheCrashSystem();

			auto purecallHandler = []()
			{
				RaiseException('PURE', EXCEPTION_NONCONTINUABLE, 0, NULL);
			};

			auto terminateHandler = []()
			{
				RaiseException('TERM', EXCEPTION_NONCONTINUABLE, 0, NULL);
			};

			auto patchIAT = [&terminateHandler, &purecallHandler](const char* module)
			{
				PatchIAT((void(*)())terminateHandler, module, "_cexit");
				PatchIAT((void(*)())terminateHandler, module, "_exit");
				PatchIAT((void(*)())terminateHandler, module, "_Exit");
				PatchIAT((void(*)())terminateHandler, module, "_c_exit");
				PatchIAT((void(*)())terminateHandler, module, "exit");
				PatchIAT((void(*)())terminateHandler, module, "abort");
				PatchIAT((void(*)())terminateHandler, module, "terminate");
				PatchIAT((void(*)())purecallHandler, module, "_purecall");
			};

			// Патчим все известные библы используемые в CK

			patchIAT("API-MS-WIN-CRT-RUNTIME-L1-1-0.DLL");
			patchIAT("MSVCR110.DLL");

			auto verPatch = lpRelocationDatabaseItem->Version();
			if (verPatch == 1)
			{
				ScopeRelocator SectionTextProtectionRemove;

				lpRelocator->Patch(_RELDATA_RAV(0), { 0xC3 });	// StackTrace::MemoryTraceWrite
				lpRelocator->Patch(_RELDATA_RAV(1), { 0xC3 });	// SetUnhandledExceptionFilter, BSWin32ExceptionHandler
				lpRelocator->Patch(_RELDATA_RAV(2), { 0xC3 });	// SetUnhandledExceptionFilter, Unknown
				lpRelocator->PatchNop(_RELDATA_RAV(3), 6);		// SetUnhandledExceptionFilter, BSWin32ExceptionHandler

				// Test
				//RaiseException('PARM', EXCEPTION_NONCONTINUABLE, 0, NULL);

				return true;
			}
			else if (verPatch == 2)
			{
				for (uint32_t i = 0; i < lpRelocationDatabaseItem->Count(); i++)
					lpRelocator->Patch(_RELDATA_RAV(i), { 0xC3 });

				//lpRelocator->Patch(_RELDATA_RAV(0), { 0xC3 });	// StackTrace::MemoryTraceWrite
				//lpRelocator->Patch(_RELDATA_RAV(1), { 0xC3 });	// SetUnhandledExceptionFilter, BSWin32ExceptionHandler
				//lpRelocator->Patch(_RELDATA_RAV(2), { 0xC3 });	// SetUnhandledExceptionFilter, Unknown
				//lpRelocator->Patch(_RELDATA_RAV(3), { 0xC3 });	// SetUnhandledExceptionFilter, BSWin32ExceptionHandler

				// Test
				//RaiseException('PARM', EXCEPTION_NONCONTINUABLE, 0, NULL);

				return true;
			}

			return false;
		}

		bool CrashDumpPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}
	}
}