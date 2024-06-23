// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <new.h>
#include <signal.h>
#include "CrashHandler.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		CrashHandler* GlobalCrashHandlerPtr = nullptr;

		HANDLE GlobalCrashDumpTriggerEvent;
		std::atomic<PEXCEPTION_POINTERS> GlobalCrashDumpExceptionInfo;
		std::atomic_uint32_t GlobalCrashDumpTargetThreadId;

		CrashHandler::CrashHandler() 
		{
			GlobalCrashDumpTriggerEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		}

		CrashHandler::~CrashHandler() {}

		void CrashHandler::SetProcessExceptionHandlers()
		{
			// Install top-level SEH handler
			SetUnhandledExceptionFilter(DumpExceptionHandler);

			// Catch pure virtual function calls
			_set_purecall_handler([]()
				{
					RaiseException('PURE', EXCEPTION_NONCONTINUABLE, 0, NULL);
				});

			// Catch new operator memory allocation exceptions
			_set_new_handler([](size_t) -> int
				{
					RaiseException('NEW_', EXCEPTION_NONCONTINUABLE, 0, NULL);
					return 0;
				});

			// Catch invalid parameter exceptions
			_set_invalid_parameter_handler([](LPCWSTR, LPCWSTR, LPCWSTR, uint32_t, uintptr_t)
				{
					RaiseException('PARM', EXCEPTION_NONCONTINUABLE, 0, NULL);
				});

			// Set up C++ signal handlers
			_set_abort_behavior(_CALL_REPORTFAULT, _CALL_REPORTFAULT);

			// Catch an abnormal program termination
			signal(SIGABRT, [](int)
				{
					RaiseException('ABRT', EXCEPTION_NONCONTINUABLE, 0, NULL);
				});

			// Catch illegal instruction handler
			signal(SIGINT, [](int)
				{
					RaiseException('ILLI', EXCEPTION_NONCONTINUABLE, 0, NULL);
				});

			// Catch a termination request
			signal(SIGTERM, [](int)
				{
					RaiseException('TERM', EXCEPTION_NONCONTINUABLE, 0, NULL);
				});
		}

		void CrashHandler::SetThreadExceptionHandlers()
		{
#if _HAS_EXCEPTIONS
			// Catch terminate() calls
			set_terminate([]()
				{
					RaiseException('TERM', EXCEPTION_NONCONTINUABLE, 0, NULL);
				});
#else
			// Catch unexpected() calls
			set_unexpected([]()
				{
					RaiseException('UNEX', EXCEPTION_NONCONTINUABLE, 0, NULL);
				});
#endif // ^^^ !_HAS_EXCEPTIONS ^^^

			// Catch a floating point error
			signal(SIGFPE, [](int)
				{
					RaiseException('PARM', EXCEPTION_NONCONTINUABLE, 0, NULL);
				});

			// Catch an illegal instruction
			signal(SIGILL, [](int)
				{
					RaiseException('ILLI', EXCEPTION_NONCONTINUABLE, 0, NULL);
				});

			// Catch illegal storage access errors
			signal(SIGSEGV, [](int)
				{
					RaiseException('ACCS', EXCEPTION_NONCONTINUABLE, 0, NULL);
				});
		}

		void CrashHandler::InstallThreadTheCrashSystem()
		{
			ResetEvent(GlobalCrashDumpTriggerEvent);
			SetProcessExceptionHandlers();
			SetThreadExceptionHandlers();

			std::thread t([]()
			{
				if (WaitForSingleObject(GlobalCrashDumpTriggerEvent, INFINITE) != WAIT_OBJECT_0)
					return;

				char fileNames[4][MAX_PATH];
				bool dumpWritten = false;

				auto ehInfo = GlobalCrashDumpExceptionInfo.load();
				auto miniDumpWriteDump = (decltype(&MiniDumpWriteDump))GetProcAddress(LoadLibraryA("dbghelp.dll"), "MiniDumpWriteDump");
				
				if (miniDumpWriteDump)
				{
					// Create a dump in the same folder of the exe itself
					char exePath[MAX_PATH];
					GetModuleFileNameA(GetModuleHandle(nullptr), exePath, ARRAYSIZE(exePath));

					SYSTEMTIME sysTime;
					GetSystemTime(&sysTime);
					sprintf_s(fileNames[0], "%s_%4d%02d%02d_%02d%02d%02d.dmp", exePath, sysTime.wYear, sysTime.wMonth, 
						sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

					HANDLE file = CreateFileA(fileNames[0], GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL, nullptr);
					if (file != INVALID_HANDLE_VALUE)
					{
						MINIDUMP_EXCEPTION_INFORMATION dumpInfo
						{
							.ThreadId = GlobalCrashDumpTargetThreadId.load(),
							.ExceptionPointers = ehInfo,
							.ClientPointers = FALSE,
						};

						auto dumpFlags = (MINIDUMP_TYPE)(MiniDumpNormal | MiniDumpWithIndirectlyReferencedMemory | 
							MiniDumpWithThreadInfo | MiniDumpWithoutOptionalData);
						dumpWritten = miniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, dumpFlags,
							&dumpInfo, nullptr, nullptr) != FALSE;

						CloseHandle(file);
					}
				}
				else
					strcpy_s(fileNames[0], "UNABLE TO LOAD DBGHELP.DLL");
				
				// Close and added archive the log's
				GlobalDebugLogPtr->Close();
				strcpy(fileNames[1], "CreationKitPlatformExtended.log");
				
				if (Core::GlobalConsoleWindowPtr)
				{
					strcpy(fileNames[2], "CreationKitPlatformExtendedCrash.log");

					Core::GlobalConsoleWindowPtr->CloseOutputFile();
					Core::GlobalConsoleWindowPtr->SaveRichTextToFile(fileNames[2]);

					FILE* fStream = _fsopen(fileNames[2], "at", _SH_DENYWR);
					if (fStream)
					{
						Utils::ScopeFileStream fileStream(fStream);
						ContextWriteToCrashLog(fStream, ehInfo);
					}
				}
				
				strcpy(fileNames[3], fileNames[0]);
				PathRenameExtensionA(fileNames[3], ".zip");

				LPCSTR zNames[3];
				zNames[0] = fileNames[0];
				zNames[1] = fileNames[1];
				zNames[2] = fileNames[2];
				zip_create(fileNames[3], zNames, Core::GlobalConsoleWindowPtr ? 3 : 2);

				DeleteFileA(fileNames[0]);
				if (Core::GlobalConsoleWindowPtr)
					DeleteFileA(fileNames[2]);

				const char* message = nullptr;
				const char* reason = nullptr;

				if (dumpWritten)
					message = "FATAL ERROR\n\nThe Creation Kit encountered a fatal error and has crashed.\n\nReason: %s (0x%08X).\n\nA minidump has been written to '%s'.\n\nPlease note it may contain private information such as usernames.";
				else
					message = "FATAL ERROR\n\nThe Creation Kit encountered a fatal error and has crashed.\n\nReason: %s (0x%08X).\n\nA minidump could not be written to '%s'.\nPlease check that you have proper permissions.";

				switch (ehInfo->ExceptionRecord->ExceptionCode)
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

				case 'ILLI':
					reason = "Your processor will encounter an illegal instruction for it, your processor is outdated";
					break;

				case 'ACCS':
					reason = "Memory access that you are not allowed to access";
					break;

				case 'ABRT':
					reason = "Aborted";
					break;

				default:
					reason = "Unspecified exception";
					break;
				}

				Utils::__Assert("", 0, message, reason, ehInfo->ExceptionRecord->ExceptionCode, fileNames[3]);
			});
			t.detach();
		}

		LONG WINAPI CrashHandler::DumpExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo)
		{
			GlobalCrashDumpExceptionInfo = ExceptionInfo;
			GlobalCrashDumpTargetThreadId = GetCurrentThreadId();

			SetEvent(GlobalCrashDumpTriggerEvent);
			Sleep(INFINITE);

			return EXCEPTION_CONTINUE_SEARCH;
		}

		void CrashHandler::ContextWriteToCrashLog(FILE* Stream, PEXCEPTION_POINTERS ExceptionInfo)
		{
			fprintf(Stream, "====== CRASH INFO ======\nException:\n");

			PEXCEPTION_RECORD pExceptionRecord = ExceptionInfo->ExceptionRecord;

			while (pExceptionRecord)
			{
				fprintf(Stream, "\tCode: %u\n\tAddress: %p\n\t",
					pExceptionRecord->ExceptionCode, pExceptionRecord->ExceptionAddress);

				switch (pExceptionRecord->ExceptionCode)
				{
				case EXCEPTION_ACCESS_VIOLATION:
					fprintf(Stream, "Message: "
						"The thread tried to read from or write to a virtual address for which it does not have the appropriate access.");
					break;
				case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
					fprintf(Stream, "Message: "
						"The thread tried to access an array element that is out of boundsand the underlying hardware supports bounds checking.");
					break;
				case EXCEPTION_BREAKPOINT:
					fprintf(Stream, "Message: "
						"A breakpoint was encountered.");
					break;
				case EXCEPTION_DATATYPE_MISALIGNMENT:
					fprintf(Stream, "Message: "
						"The thread tried to read or write data that is misaligned on hardware that does not provide alignment. For example, 16 - bit values must be aligned on 2 - byte boundaries; 32 - bit values on 4 - byte boundaries, and so on.");
					break;
				case EXCEPTION_FLT_DENORMAL_OPERAND:
					fprintf(Stream, "Message: "
						"One of the operands in a floating - point operation is denormal. A denormal value is one that is too small to represent as a standard floating - point value.");
					break;
				case EXCEPTION_FLT_DIVIDE_BY_ZERO:
					fprintf(Stream, "Message: "
						"The thread tried to divide a floating - point value by a floating - point divisor of zero.");
					break;
				case EXCEPTION_FLT_INEXACT_RESULT:
					fprintf(Stream, "Message: "
						"The result of a floating - point operation cannot be represented exactly as a decimal fraction.");
					break;
				case EXCEPTION_FLT_INVALID_OPERATION:
					fprintf(Stream, "Message: "
						"This exception represents any floating - point exception not included in this list.");
					break;
				case EXCEPTION_FLT_OVERFLOW:
					fprintf(Stream, "Message: "
						"The exponent of a floating - point operation is greater than the magnitude allowed by the corresponding type.");
					break;
				case EXCEPTION_FLT_STACK_CHECK:
					fprintf(Stream, "Message: "
						"The stack overflowed or underflowed as the result of a floating - point operation.");
					break;
				case EXCEPTION_FLT_UNDERFLOW:
					fprintf(Stream, "Message: "
						"The exponent of a floating - point operation is less than the magnitude allowed by the corresponding type.");
					break;
				case EXCEPTION_ILLEGAL_INSTRUCTION:
					fprintf(Stream, "Message: "
						"The thread tried to execute an invalid instruction.");
					break;
				case EXCEPTION_IN_PAGE_ERROR:
					fprintf(Stream, "Message: "
						"The thread tried to access a page that was not present, and the system was unable to load the page. For example, this exception might occur if a network connection is lost while running a program over the network.");
					break;
				case EXCEPTION_INT_DIVIDE_BY_ZERO:
					fprintf(Stream, "Message: "
						"The thread tried to divide an integer value by an integer divisor of zero.");
					break;
				case EXCEPTION_INT_OVERFLOW:
					fprintf(Stream, "Message: "
						"The result of an integer operation caused a carry out of the most significant bit of the result.");
					break;
				case EXCEPTION_INVALID_DISPOSITION:
					fprintf(Stream, "Message: "
						"An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high - level language such as C should never encounter this exception.");
					break;
				case EXCEPTION_NONCONTINUABLE_EXCEPTION:
					fprintf(Stream, "Message: "
						"The thread tried to continue execution after a noncontinuable exception occurred.");
					break;
				case EXCEPTION_PRIV_INSTRUCTION:
					fprintf(Stream, "Message: "
						"The thread tried to execute an instruction whose operation is not allowed in the current machine mode.");
					break;
				case EXCEPTION_SINGLE_STEP:
					fprintf(Stream, "Message: "
						"A trace trap or other single - instruction mechanism signaled that one instruction has been executed.");
					break;
				case EXCEPTION_STACK_OVERFLOW:
					fprintf(Stream, "Message: "
						"The thread used up its stack.");
					break;
				default:
					fprintf(Stream, "Message: "
						"Unspecified exception.");
					break;
				}

				pExceptionRecord = pExceptionRecord->ExceptionRecord;
			}
			
			fprintf(Stream, "\n\nContext:\n");
			
			PCONTEXT Context = ExceptionInfo->ContextRecord;

			fprintf(Stream, 
				"\tRAX\t%016llX\n\tRBX\t%016llX\n\tRCX\t%016llX\n\tRDX\t%016llX\n\tRBP\t%016llX\n\tRSP\t%016llX\n"
				"\tRSI\t%016llX\n\tRDI\t%016llX\n\n"
				"\tR8\t%016llX\n\tR9\t%016llX\n\tR10\t%016llX\n\tR11\t%016llX\n\tR12\t%016llX\n\tR13\t%016llX\n"
				"\tR14\t%016llX\n\tR15\t%016llX\n\n"
				"\tRIP\t%016llX\n\n",
				(uintptr_t)Context->Rax, (uintptr_t)Context->Rbx, (uintptr_t)Context->Rcx, (uintptr_t)Context->Rdx,
				(uintptr_t)Context->Rbp, (uintptr_t)Context->Rsp, (uintptr_t)Context->Rsi, (uintptr_t)Context->Rdi,
				(uintptr_t)Context->R8, (uintptr_t)Context->R9, (uintptr_t)Context->R10, (uintptr_t)Context->R11,
				(uintptr_t)Context->R12, (uintptr_t)Context->R13, (uintptr_t)Context->R14, (uintptr_t)Context->R15, 
				(uintptr_t)Context->Rip);

			fprintf(Stream, "\tRFlags\t%08X\n", Context->EFlags);
			
			auto lamda_is_rflag = [](DWORD Flags, uint8_t bit) -> uint8_t
			{
				return (uint8_t)((Flags & (1 << bit)) >> bit);
			};
			
			fprintf(Stream,
				"\tZF\t%u\tPF\t%u\tAF\t%u\n\tOF\t%u\tSF\t%u\tDF\t%u\n\tCF\t%u\tTF\t%u\tIF\t%u\n\n",
				lamda_is_rflag(Context->EFlags, 6), lamda_is_rflag(Context->EFlags, 2), lamda_is_rflag(Context->EFlags, 4),
				lamda_is_rflag(Context->EFlags, 11), lamda_is_rflag(Context->EFlags, 7), lamda_is_rflag(Context->EFlags, 10),
				lamda_is_rflag(Context->EFlags, 0), lamda_is_rflag(Context->EFlags, 8), lamda_is_rflag(Context->EFlags, 9));

			fprintf(Stream, "\tLastError\t%08X\n\n", GetLastError());

			fprintf(Stream,
				"\tGS\t%04X\tFS\t%04X\n\tES\t%04X\tDS\t%04X\n\tCS\t%04X\tSS\t%04X\n\n",
				Context->SegGs, Context->SegFs, Context->SegEs, Context->SegDs, Context->SegCs, Context->SegSs);
			
			auto lamda_xmm_printf = [](FILE* Stream, uint8_t RegId, M128A Reg)
			{
				auto p = (DWORD*)&Reg;
				fprintf(Stream, "\tXMM%u\t%08X\t%08X\t%08X\t%08X\n", RegId, p[0], p[1], p[2], p[3]);
			};

			lamda_xmm_printf(Stream, 0, Context->Xmm0);
			lamda_xmm_printf(Stream, 1, Context->Xmm1);
			lamda_xmm_printf(Stream, 2, Context->Xmm2);
			lamda_xmm_printf(Stream, 3, Context->Xmm3);
			lamda_xmm_printf(Stream, 4, Context->Xmm4);
			lamda_xmm_printf(Stream, 5, Context->Xmm5);
			lamda_xmm_printf(Stream, 6, Context->Xmm6);
			lamda_xmm_printf(Stream, 7, Context->Xmm7);
			lamda_xmm_printf(Stream, 8, Context->Xmm8);
			lamda_xmm_printf(Stream, 9, Context->Xmm9);
			lamda_xmm_printf(Stream, 10, Context->Xmm10);
			lamda_xmm_printf(Stream, 11, Context->Xmm11);
			lamda_xmm_printf(Stream, 12, Context->Xmm12);
			lamda_xmm_printf(Stream, 13, Context->Xmm13);
			lamda_xmm_printf(Stream, 14, Context->Xmm14);
			lamda_xmm_printf(Stream, 15, Context->Xmm15);
		}
	}
}