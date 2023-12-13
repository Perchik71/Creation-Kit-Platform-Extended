// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashDumpPatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		static std::atomic_uint32_t GlobalDumpTargetThreadId;

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

		bool CrashDumpPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool CrashDumpPatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			SetUnhandledExceptionFilter(DumpExceptionHandler);

			_set_invalid_parameter_handler([](LPCWSTR, LPCWSTR, LPCWSTR, uint32_t, uintptr_t)
				{
					RaiseException('PARM', EXCEPTION_NONCONTINUABLE, 0, NULL);
				});

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
				PatchIAT((void(*)())terminateHandler, module, "_c_exit");
				PatchIAT((void(*)())terminateHandler, module, "exit");
				PatchIAT((void(*)())terminateHandler, module, "abort");
				PatchIAT((void(*)())terminateHandler, module, "terminate");
				PatchIAT((void(*)())purecallHandler, module, "_purecall");
			};

			// Патчим все известные библы используемые в CK

			patchIAT("API-MS-WIN-CRT-RUNTIME-L1-1-0.DLL");
			patchIAT("MSVCR110.DLL");
			//patchIAT("VCRUNTIME140.DLL");

			if (lpRelocationDatabaseItem->Version() == 1)
			{
				ScopeRelocator SectionTextProtectionRemove;

				lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xC3 });	// StackTrace::MemoryTraceWrite
				lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0xC3 });	// SetUnhandledExceptionFilter, BSWin32ExceptionHandler
				lpRelocator->Patch(lpRelocationDatabaseItem->At(2), { 0xC3 });	// SetUnhandledExceptionFilter, Unknown
				lpRelocator->PatchNop(lpRelocationDatabaseItem->At(3), 6);		// SetUnhandledExceptionFilter, BSWin32ExceptionHandler

				return true;
			}

			return false;
		}

		bool CrashDumpPatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}

		DWORD WINAPI CrashDumpPatch::DumpWriterThread(LPVOID Arg)
		{
			Assert(Arg);

			std::wstring filezip;
			CHAR fileName[MAX_PATH];
			BOOL dumpWritten = FALSE;

			PEXCEPTION_POINTERS exceptionInfo = (PEXCEPTION_POINTERS)Arg;
			auto miniDumpWriteDump = (decltype(&MiniDumpWriteDump))GetProcAddress(LoadLibraryA("dbghelp.dll"),
				"MiniDumpWriteDump");

			if (miniDumpWriteDump)
			{
				LPSTR files[3] = { 0 };

				// Create a dump in the same folder of the exe itself
				CHAR exePath[MAX_PATH];
				GetModuleFileNameA(GetModuleHandleA(NULL), exePath, ARRAYSIZE(exePath));

				SYSTEMTIME sysTime;
				GetSystemTime(&sysTime);
				sprintf_s(fileName, "%s_%4d%02d%02d_%02d%02d%02d.dmp", exePath, sysTime.wYear, sysTime.wMonth, sysTime.wDay,
					sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

				auto slen = strlen(fileName) + 1;
				files[0] = new CHAR[slen];
				ZeroMemory(files[0], slen);
				strcpy(files[0], fileName);

				HANDLE file = CreateFileA(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL, NULL);

				if (file != INVALID_HANDLE_VALUE)
				{
					MINIDUMP_EXCEPTION_INFORMATION dumpInfo = { 0 };
					dumpInfo.ThreadId = GlobalDumpTargetThreadId.load();
					dumpInfo.ExceptionPointers = exceptionInfo;
					dumpInfo.ClientPointers = FALSE;

					uint32_t dumpFlags = MiniDumpNormal | MiniDumpWithIndirectlyReferencedMemory
						| MiniDumpWithThreadInfo | MiniDumpWithoutOptionalData;
					dumpWritten = miniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, (MINIDUMP_TYPE)dumpFlags,
						&dumpInfo, NULL, NULL) != FALSE;

					CloseHandle(file);
				}

				// Закрыть и добавить в архив лог мода
				GlobalDebugLogPtr->Close();
				files[1] = new CHAR[14];
				ZeroMemory(files[1], slen);
				strcpy(files[1], "CreationKitPlatformExtended.log");

				// Нужно 3-ий файл лог самого кита
				if (Core::GlobalConsoleWindowPtr)
				{
					std::string slog = "CreationKitPlatformExtendedCrash.log";
					slen = slog.length() + 1;
					files[2] = new CHAR[slen];
					ZeroMemory(files[2], slen);
					strcpy(files[2], slog.c_str());

					Core::GlobalConsoleWindowPtr->CloseOutputFile();
					Core::GlobalConsoleWindowPtr->SaveRichTextToFile(slog.c_str());
				}

				filezip = WideString(std::filesystem::path(fileName).stem().c_str());
				filezip.append(L".zip");

				zip_create(Utils::Wide2Ansi(filezip.c_str()).c_str(), (LPCSTR*)files, 
					Core::GlobalConsoleWindowPtr ? 3 : 2);

				DeleteFileA(files[0]);
				if (Core::GlobalConsoleWindowPtr)
					DeleteFileA(files[2]);

				if (files[0])
					delete[] files[0];
				if (files[1])
					delete[] files[1];
				if (Core::GlobalConsoleWindowPtr && files[2])
					delete[] files[2];
			}
			else
				strcpy_s(fileName, "UNABLE TO LOAD DBGHELP.DLL");

			LPCSTR message = NULL;
			LPCSTR reason = NULL;

			if (dumpWritten)
				message = "FATAL ERROR\n\nThe Creation Kit encountered a fatal error and has crashed.\n"
				"\nReason: %s (0x%08X).\n\nA minidump has been written to '%s'.\n"
				"\nPlease note it may contain private information such as usernames.";
			else
				message = "FATAL ERROR\n\nThe Creation Kit encountered a fatal error and has crashed.\n"
				"\nReason: %s (0x%08X).\n\nA minidump could not be written to '%s'.\n"
				"\nPlease check that you have proper permissions.";

			switch (exceptionInfo->ExceptionRecord->ExceptionCode)
			{
			case 'PARM':
				reason = "An invalid parameter was sent to a function that considers invalid parameters fatal";
				break;

			case 'TERM':
				reason = "Program requested termination in an unusual way";
				break;

			case 'PURE':
				reason = "Pure virtual function call";
				break;

			default:
				reason = "Unspecified exception";
				break;
			}

			if (filezip.length() > 0)
				Utils::__Assert("", 0, message, reason, exceptionInfo->ExceptionRecord->ExceptionCode, filezip.c_str());
			else
				Utils::__Assert("", 0, message, reason, exceptionInfo->ExceptionRecord->ExceptionCode, "");

			return 0;
		}

		LONG WINAPI CrashDumpPatch::DumpExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo)
		{
			GlobalDumpTargetThreadId.store(GetCurrentThreadId());
			HANDLE threadHandle = CreateThread(NULL, 0, DumpWriterThread, ExceptionInfo, 0, NULL);

			if (threadHandle)
			{
				WaitForSingleObject(threadHandle, INFINITE);
				CloseHandle(threadHandle);
			}

			return EXCEPTION_CONTINUE_SEARCH;
		}
	}
}