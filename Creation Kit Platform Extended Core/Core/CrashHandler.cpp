// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <new.h>
#include <signal.h>
#include <psapi.h>
#include "CrashHandler.h"
#include "INIWrapper.h"

#include "Engine.h"
#include "PluginManager.h"
#include "Editor API/BSString.h"
#include "Version/resource_version2.h"
#include "TypeInfo/ms_rtti.h"
#include "Zydis/Zydis.h"
#include "iw.h"

#include "Editor API/SSE/TESForm.h"
#include "Editor API/FO4/TESFormF4.h"
#include "Editor API/SF/TESForm.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class Introspection
		{
			bool Init;
			ZydisDecoder Decoder;
			ZydisFormatter Formatter;
		public:
			enum AnalyzeItemType
			{
				aitUnknown = 0,
				aitNumber,
				aitString,
				aitCode,
				aitClass,
				aitInstance
			};

			struct AnalyzeInfo
			{
				AnalyzeItemType Type;
				EditorAPI::BSString Text;
				EditorAPI::BSString Additional;
			};

			Introspection();

			[[nodiscard]] AnalyzeInfo Analyze(FILE* Stream, uintptr_t Address, CrashHandler::ModuleMapInfo* Modules,
				CrashHandler::ArrayMemoryInfo* Memory) const noexcept(true);
		private:
			[[nodiscard]] bool AnalyzeClass(FILE* Stream, uintptr_t Address, AnalyzeInfo* Info, uintptr_t RefAddress = 0) const noexcept(true);
		};

		CrashHandler* GlobalCrashHandlerPtr = nullptr;

		HANDLE GlobalCrashDumpTriggerEvent;
		HMODULE GlobalCrashLoadedModules[1024];
		Introspection* GlobalIntrospection = nullptr;
		NT_TIB64* GlobalCrashTib = nullptr;
		Array<uintptr_t> ProbablyCallStack;

		std::atomic<PEXCEPTION_POINTERS> GlobalCrashDumpExceptionInfo;
		std::atomic_uint32_t GlobalCrashDumpTargetThreadId;

		Introspection::Introspection() :
			Init(false)
		{
			if (ZYDIS_SUCCESS(ZydisDecoderInit(&Decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64)))
				if (ZYDIS_SUCCESS(ZydisFormatterInit(&Formatter, ZYDIS_FORMATTER_STYLE_INTEL)))
					Init = true;
		}

		bool Introspection::AnalyzeClass(FILE* Stream, uintptr_t Address, AnalyzeInfo* Info, uintptr_t RefAddress) const noexcept(true)
		{
			auto ObjRtti = MSRTTI::FindByAddr(Address);
			if (ObjRtti)
			{
				Info->Type = aitClass;
				auto RttiName = strchr(ObjRtti->Name, ' ');
				Info->Text = RttiName ? RttiName + 1 : ObjRtti->Name;

				if (RefAddress)
				{
					auto form = _DYNAMIC_CAST((void*)RefAddress, 0, ObjRtti->Name, "class TESForm");
					if (form)
					{
						switch (GetShortExecutableTypeFromFull(GlobalEnginePtr->GetEditorVersion()))
						{
						case CreationKitPlatformExtended::Core::EDITOR_SHORT_SKYRIM_SE:
						{
							auto formSSE = (EditorAPI::SkyrimSpectialEdition::TESForm*)form;
							auto buf = std::make_unique<char[]>(256);
							formSSE->DebugInfo(buf.get(), 256);
							Info->Additional = buf.get();
						}
						break;
						case CreationKitPlatformExtended::Core::EDITOR_SHORT_FALLOUT_C4:
						{
							auto formFO4 = (EditorAPI::Fallout4::TESForm*)form;
							auto buf = std::make_unique<char[]>(256);
							formFO4->DebugInfo(buf.get(), 256);
							Info->Additional = buf.get();
						}
						break;
						case CreationKitPlatformExtended::Core::EDITOR_SHORT_STARFIELD:
						{
							auto formSF = (EditorAPI::Starfield::TESForm*)form;
							auto buf = std::make_unique<char[]>(256);
							formSF->DebugInfo(buf.get(), 256);
							Info->Additional = buf.get();
						}
						break;
						default:
							break;
						}
					}
				}

				return true;
			}

			return false;
		}

		Introspection::AnalyzeInfo Introspection::Analyze(FILE* Stream, uintptr_t Address, CrashHandler::ModuleMapInfo* Modules,
			CrashHandler::ArrayMemoryInfo* Memory) const noexcept(true)
		{
			AnalyzeInfo Info;
			ZeroMemory(&Info, sizeof(Info));

			if (!Init)
			{
				Info.Text = "<NO_INIT>";
				return Info;
			}

			if (!Modules)
			{
				Info.Text = "<INVALID_ARGS>";
				return Info;
			}

			auto itMod = Modules->begin();
			for (; itMod != Modules->end(); itMod++)
			{
				if ((Address >= itMod->second.Start) && (Address < itMod->second.End))
					break;
			}

			if (itMod != Modules->end())
			{
				// Address module

				PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)(itMod->second.Start + ((PIMAGE_DOS_HEADER)itMod->second.Start)->e_lfanew);

				if ((Address >= itMod->second.SecCode.Start) && (Address < itMod->second.SecCode.End))
				{
					Info.Type = aitCode;
					Info.Text = EditorAPI::BSString::FormatString("%s+%X ", itMod->first.c_str(), (uint32_t)(Address - itMod->second.Start));

					char InstructionText[256];
					ZydisDecodedInstruction Instruction;
					if (ZYDIS_SUCCESS(ZydisDecoderDecodeBuffer(&Decoder, (void*)Address, ZYDIS_MAX_INSTRUCTION_LENGTH, Address, &Instruction)))
					{
						ZydisFormatterFormatInstruction(&Formatter, &Instruction, InstructionText, ARRAYSIZE(InstructionText));
						Info.Text.Append(InstructionText);
						ProbablyCallStack.push_back(Address);
					}
					else
						Info.Text.Append("<FATAL DECODER INSTRUCTION>");
				}
				else if (!(Address - itMod->second.Start))
				{
					Info.Type = aitInstance;
					Info.Text = itMod->first.c_str();
				}
				else
					goto Analize_Continue;
			}
			else
			{
			Analize_Continue:
				if (Memory)
				{
					auto itMem = Memory->begin();
					for (; itMem != Memory->end(); itMem++)
					{
						if ((Address >= itMem->Start) && (Address < itMem->End))
							break;
					}

					if (itMem != Memory->end())
					{
						if (GlobalCrashTib && (GlobalCrashTib->StackLimit <= Address) && (GlobalCrashTib->StackBase > Address))
						{
							// Stack
							auto RefInfo = Analyze(Stream, *(uintptr_t*)Address, Modules, Memory);
							if (RefInfo.Type != aitCode)
							{
								Info.Type = RefInfo.Type;
								if ((RefInfo.Type == aitNumber) || (RefInfo.Type == aitClass) || (RefInfo.Type == aitString))
									Info.Text.AppendFormat("&%s", RefInfo.Text.c_str());
								else
								{
									if (!AnalyzeClass(Stream, *(uintptr_t*)Address, &Info, Address))
										Info.Type = aitUnknown;
								}
							}
						}
						else
						{
							if (!AnalyzeClass(Stream, *(uintptr_t*)Address, &Info, Address))
							{
								// maybe string

								const auto printable = [](char ch) noexcept(true)
								{
									return (((0x20 <= ch) && (ch <= 0x7E)) || (ch == 0x9) || (ch == 0x10));
								};

								const auto str = (const char*)Address;
								constexpr std::size_t max = 1000;
								std::size_t len = 0;
								for (; len < max && str[len]; ++len) 
									if (!printable(str[len]))
										break;

								if ((len > 4) && (len < max))
								{
									Info.Type = aitString;
									Info.Text.AppendFormat("\"%s\"", str);
								}
							}
						}
					}
					else
					{
						if (!AnalyzeClass(Stream, Address, &Info))
						{
							// maybe part string

							const auto printable = [](char ch) noexcept(true)
							{
								return (((0x20 <= ch) && (ch <= 0x7E)) || (ch == 0x9) || (ch == 0x10));
							};

							const auto str = (const char*)(&Address);
							constexpr std::size_t max = 8;
							std::size_t len = 0;
							for (; len < max && str[len]; ++len)
								if (!printable(str[len]))
									break;

							if ((len > 5) && (len <= max))
							{
								EditorAPI::BSString a(str, (WORD)len);
								Info.Type = aitString;
								Info.Text.AppendFormat("\"%s\"", a.c_str());
							}
							else
								Info.Type = aitNumber;
						}
					}
				}
			}

			return Info;
		}

		CrashHandler::CrashHandler() 
		{
			GlobalCrashDumpTriggerEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
			GlobalIntrospection = new Introspection();
		}

		CrashHandler::~CrashHandler() 
		{
			CloseHandle(GlobalCrashDumpTriggerEvent);

			if (GlobalIntrospection)
			{
				delete GlobalIntrospection;
				GlobalIntrospection = nullptr;
			}
		}

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
				auto BigDump = GlobalINIConfigPtr->ReadBool("Crashes", "bGenerateFullDump", false);

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
					GetModuleFileNameA(GetModuleHandleA(nullptr), exePath, ARRAYSIZE(exePath));

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

						UINT32 dumpFlags = 
							MiniDumpNormal | MiniDumpWithThreadInfo | MiniDumpWithIndirectlyReferencedMemory;

						if (BigDump)
							dumpFlags |= (MiniDumpWithDataSegs | MiniDumpWithFullMemory | MiniDumpWithHandleData | MiniDumpWithFullMemoryInfo);
						else
							dumpFlags |= MiniDumpWithoutOptionalData;
			
						dumpWritten = miniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, (MINIDUMP_TYPE)dumpFlags,
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

		void CrashHandler::PrintException(FILE* Stream, ModuleMapInfo* Modules, PEXCEPTION_RECORD lpExceptionRecord)
		{
			if (!Stream || !lpExceptionRecord || !Modules)
				return;

			String ExceptionName;
			String ExceptionDescription;
			EditorAPI::BSString ExceptionInstructionText;

			switch (lpExceptionRecord->ExceptionCode)
			{
			case EXCEPTION_ACCESS_VIOLATION:
				ExceptionName = "EXCEPTION_ACCESS_VIOLATION";
				ExceptionDescription = "The thread tried to read from or write to a virtual address for which it does not have the appropriate access.";
				break;
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
				ExceptionName = "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
				ExceptionDescription = "The thread tried to access an array element that is out of boundsand the underlying hardware supports bounds checking.";
				break;
			case EXCEPTION_BREAKPOINT:
				ExceptionName = "EXCEPTION_BREAKPOINT";
				ExceptionDescription = "A breakpoint was encountered.";
				break;
			case EXCEPTION_DATATYPE_MISALIGNMENT:
				ExceptionName = "EXCEPTION_DATATYPE_MISALIGNMENT";
				ExceptionDescription = "The thread tried to read or write data that is misaligned on hardware that does not provide alignment. "
					"For example, 16 - bit values must be aligned on 2 - byte boundaries; 32 - bit values on 4 - byte boundaries, and so on.";
				break;
			case EXCEPTION_FLT_DENORMAL_OPERAND:
				ExceptionName = "EXCEPTION_FLT_DENORMAL_OPERAND";
				ExceptionDescription = "One of the operands in a floating - point operation is denormal. A denormal value is one that is too small "
					"to represent as a standard floating - point value.";
				break;
			case EXCEPTION_FLT_DIVIDE_BY_ZERO:
				ExceptionName = "EXCEPTION_FLT_DIVIDE_BY_ZERO";
				ExceptionDescription = "The thread tried to divide a floating - point value by a floating - point divisor of zero.";
				break;
			case EXCEPTION_FLT_INEXACT_RESULT:
				ExceptionName = "EXCEPTION_FLT_INEXACT_RESULT";
				ExceptionDescription = "The result of a floating - point operation cannot be represented exactly as a decimal fraction.";
				break;
			case EXCEPTION_FLT_INVALID_OPERATION:
				ExceptionName = "EXCEPTION_FLT_INVALID_OPERATION";
				ExceptionDescription = "This exception represents any floating - point exception not included in this list.";
				break;
			case EXCEPTION_FLT_OVERFLOW:
				ExceptionName = "EXCEPTION_FLT_OVERFLOW";
				ExceptionDescription = "The exponent of a floating - point operation is greater than the magnitude allowed by the corresponding type.";
				break;
			case EXCEPTION_FLT_STACK_CHECK:
				ExceptionName = "EXCEPTION_FLT_STACK_CHECK";
				ExceptionDescription = "The stack overflowed or underflowed as the result of a floating - point operation.";
				break;
			case EXCEPTION_FLT_UNDERFLOW:
				ExceptionName = "EXCEPTION_FLT_UNDERFLOW";
				ExceptionDescription = "The exponent of a floating - point operation is less than the magnitude allowed by the corresponding type.";
				break;
			case EXCEPTION_ILLEGAL_INSTRUCTION:
				ExceptionName = "EXCEPTION_ILLEGAL_INSTRUCTION";
				ExceptionDescription = "The thread tried to execute an invalid instruction.";
				break;
			case EXCEPTION_IN_PAGE_ERROR:
				ExceptionName = "EXCEPTION_IN_PAGE_ERROR";
				ExceptionDescription = "The thread tried to access a page that was not present, and the system was unable to load the page. "
					"For example, this exception might occur if a network connection is lost while running a program over the network.";
				break;
			case EXCEPTION_INT_DIVIDE_BY_ZERO:
				ExceptionName = "EXCEPTION_INT_DIVIDE_BY_ZERO";
				ExceptionDescription = "The thread tried to divide an integer value by an integer divisor of zero.";
				break;
			case EXCEPTION_INT_OVERFLOW:
				ExceptionName = "EXCEPTION_INT_OVERFLOW";
				ExceptionDescription = "The result of an integer operation caused a carry out of the most significant bit of the result.";
				break;
			case EXCEPTION_INVALID_DISPOSITION:
				ExceptionName = "EXCEPTION_INVALID_DISPOSITION";
				ExceptionDescription = "An exception handler returned an invalid disposition to the exception dispatcher. Programmers using a high -"
					" level language such as C should never encounter this exception.";
				break;
			case EXCEPTION_NONCONTINUABLE_EXCEPTION:
				ExceptionName = "EXCEPTION_NONCONTINUABLE_EXCEPTION";
				ExceptionDescription = "The thread tried to continue execution after a noncontinuable exception occurred.";
				break;
			case EXCEPTION_PRIV_INSTRUCTION:
				ExceptionName = "EXCEPTION_PRIV_INSTRUCTION";
				ExceptionDescription = "The thread tried to execute an instruction whose operation is not allowed in the current machine mode.";
				break;
			case EXCEPTION_SINGLE_STEP:
				ExceptionName = "EXCEPTION_SINGLE_STEP";
				ExceptionDescription = "A trace trap or other single - instruction mechanism signaled that one instruction has been executed.";
				break;
			case EXCEPTION_STACK_OVERFLOW:
				ExceptionName = "EXCEPTION_STACK_OVERFLOW";
				ExceptionDescription = "The thread used up its stack.";
				break;
			default:
				ExceptionName = "EXCEPTION_UNKNOWN";
				ExceptionDescription = "Unspecified exception.";
				break;
			}

			if (GlobalIntrospection)
			{
				auto Analize = GlobalIntrospection->Analyze(Stream, (uintptr_t)lpExceptionRecord->ExceptionAddress, Modules, nullptr);
				ExceptionInstructionText.Append(Analize.Text);
			}

			fprintf(Stream, "\nUnhandled exception ""%s"" at 0x%016llX %s\n", ExceptionName.c_str(),
				(uintptr_t)lpExceptionRecord->ExceptionAddress, ExceptionInstructionText.c_str());

			// Log exception flags
			fprintf(Stream, "Exception Flags: 0x%08X\n", lpExceptionRecord->ExceptionFlags);
			// Log number of parameters
			fprintf(Stream, "Number of Parameters: %u\n", lpExceptionRecord->NumberParameters);
			// Description
			fprintf(Stream, "Exception Description: %s\n", ExceptionDescription.c_str());

			// Log additional exception information for specific exception types
			if (lpExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) 
			{
				const auto accessType = lpExceptionRecord->ExceptionInformation[0] == 0 ? "read" :
										lpExceptionRecord->ExceptionInformation[0] == 1 ? "write" :
										lpExceptionRecord->ExceptionInformation[0] == 8 ? "execute" :
																						  "unknown";
				const auto faultAddress = lpExceptionRecord->ExceptionInformation[1];
				fprintf(Stream, "Access Violation: Tried to %s memory at 0x%016llX\n", accessType, faultAddress);
			}
			else if (lpExceptionRecord->ExceptionCode == EXCEPTION_IN_PAGE_ERROR) 
			{
				const auto accessType = lpExceptionRecord->ExceptionInformation[0] == 0 ? "read" :
										lpExceptionRecord->ExceptionInformation[0] == 1 ? "write" :
										lpExceptionRecord->ExceptionInformation[0] == 8 ? "execute" :
																						  "unknown";
				const auto faultAddress = lpExceptionRecord->ExceptionInformation[1];
				const auto ntStatus = lpExceptionRecord->ExceptionInformation[2];
				fprintf(Stream, "In-Page Error: Tried to %s memory at 0x%016llX, NTSTATUS: 0x%08X\n", accessType, faultAddress, (uint32_t)ntStatus);
			}
			else if (lpExceptionRecord->NumberParameters > 0)
			{
				fprintf(Stream, "Exception Information Parameters:\n");	
				for (DWORD i = 0; i < lpExceptionRecord->NumberParameters; ++i)
					fprintf(Stream, "\tParameter[%u]: 0x%016llX:\n", i, lpExceptionRecord->ExceptionInformation[i]);
			}

			// Check for nested exceptions
			if (lpExceptionRecord->ExceptionRecord) 
			{
				fprintf(Stream, "Nested Exception:\n");
				// Recursively print nested exception
				PrintException(Stream, Modules, lpExceptionRecord->ExceptionRecord);
			}

			fprintf(Stream, "\n");
			fflush(Stream);
		}

		void CrashHandler::PrintSettings(FILE* Stream)
		{
			auto smINI = ((SmartPointer<mINI::INIStructure>*)GlobalINIConfigPtr)->Get();

			for (auto itSec = smINI->begin(); itSec != smINI->end(); itSec++)
			{
				if (!itSec->second.size() || (itSec->first == "hotkeys"))
					continue;

				fprintf(Stream, "\t[%s]\n", itSec->first.c_str());

				for (auto itVal = itSec->second.begin(); itVal != itSec->second.end(); itVal++)
					fprintf(Stream, "\t\t%s: %s\n", itVal->first.c_str(), itVal->second.c_str());
			}

			fprintf(Stream, "\n");
			fflush(Stream);
		}

		void CrashHandler::PrintSysInfo(FILE* Stream)
		{
			fprintf(Stream, "SYSTEM SPECS:\n");
			fprintf(Stream, "\tOS: %s\n", iw::system::os_info().c_str());

			auto cpu_info = iw::cpu::cpu_info();
			fprintf(Stream, "\tCPU: %s\n", iw::cpu::cpu_brand(&cpu_info).c_str());

			auto gpu_info = iw::graphics::graphics_info();
			for (uint8_t i = 0; i < gpu_info.videocard_num; i++)
			{
				auto gpu = gpu_info.videocards + i;
				fprintf(Stream, "\tGPU #%u: %s %u Gb\n", i + 1, gpu->name, (uint16_t)std::roundf(gpu->memory));
			}
			
			auto mem_info = iw::system::memory_info();
			fprintf(Stream, "\tPHYSICAL MEMORY: %.2f GB / %.2f GB\n", mem_info.physical_total - mem_info.physical_free, mem_info.physical_total);
			fprintf(Stream, "\tSHARED MEMORY: %.2f GB / %.2f GB\n\n", mem_info.shared_total - mem_info.shared_free, mem_info.shared_total);
			fflush(Stream);
		}

		void CrashHandler::PrintMemory(FILE* Stream, ArrayMemoryInfo* Memory)
		{
			if (!Memory)
				return;

			fprintf(Stream, "MEMORY:\n");

			uint32_t id = 0;
			for (auto itM = Memory->begin(); itM != Memory->end(); itM++, id++)
				fprintf(Stream, "\t[%04u]: %llX / %llX\n", id, itM->Start, itM->End);

			fprintf(Stream, "\n");
			fflush(Stream);
		}

		void CrashHandler::PrintRegistry(FILE* Stream, ModuleMapInfo* Modules, ArrayMemoryInfo* Memory, PEXCEPTION_POINTERS lpExceptionRecord)
		{
			if (!Modules || !lpExceptionRecord)
				return;

			fprintf(Stream, "REGISTERS:\n");

			auto lamda_print_register = [&](FILE* Stream, const char* NameReg, uintptr_t Value)
			{
				fprintf(Stream, "\t%s %-*llX ", NameReg, 16, Value);

				EditorAPI::BSString AnalizeText;
				auto Analize = GlobalIntrospection->Analyze(Stream, Value, Modules, Memory);

				switch (Analize.Type)
				{
				case Introspection::aitCode:
					fprintf(Stream, "(void* -> %s)\n", Analize.Text.c_str());
					break;
				case Introspection::aitInstance:
					fprintf(Stream, "(HINSTANCE*) %s\n", Analize.Text.c_str());
					break;
				case Introspection::aitNumber:
					if (Value >> 63)
						fprintf(Stream, "(size_t) [uint: %llu int: %lld]\n", Value, (ptrdiff_t)Value);
					else
						fprintf(Stream, "(size_t) [%llu]\n", Value);
					break;
				case Introspection::aitClass:
					if (Analize.Additional.IsEmpty())
						fprintf(Stream, "(%s*)\n", Analize.Text.c_str());
					else
						fprintf(Stream, "(%s*) | %s\n", Analize.Text.c_str(), Analize.Additional.c_str());
					break;
				case Introspection::aitString:
					fprintf(Stream, "(char*) %s\n", Analize.Text.c_str());
					break;
				default:
					fprintf(Stream, "(void*)\n");
					break;
				}
			};

			lamda_print_register(Stream, "RAX", (uintptr_t)lpExceptionRecord->ContextRecord->Rax);
			lamda_print_register(Stream, "RBX", (uintptr_t)lpExceptionRecord->ContextRecord->Rbx);
			lamda_print_register(Stream, "RCX", (uintptr_t)lpExceptionRecord->ContextRecord->Rcx);
			lamda_print_register(Stream, "RDX", (uintptr_t)lpExceptionRecord->ContextRecord->Rdx);
			lamda_print_register(Stream, "RBP", (uintptr_t)lpExceptionRecord->ContextRecord->Rbp);
			lamda_print_register(Stream, "RSP", (uintptr_t)lpExceptionRecord->ContextRecord->Rsp);
			lamda_print_register(Stream, "RSI", (uintptr_t)lpExceptionRecord->ContextRecord->Rsi);
			lamda_print_register(Stream, "RDI", (uintptr_t)lpExceptionRecord->ContextRecord->Rdi);
			lamda_print_register(Stream, "R8 ", (uintptr_t)lpExceptionRecord->ContextRecord->R8);
			lamda_print_register(Stream, "R9 ", (uintptr_t)lpExceptionRecord->ContextRecord->R9);
			lamda_print_register(Stream, "R10", (uintptr_t)lpExceptionRecord->ContextRecord->R10);
			lamda_print_register(Stream, "R11", (uintptr_t)lpExceptionRecord->ContextRecord->R11);
			lamda_print_register(Stream, "R12", (uintptr_t)lpExceptionRecord->ContextRecord->R12);
			lamda_print_register(Stream, "R13", (uintptr_t)lpExceptionRecord->ContextRecord->R13);
			lamda_print_register(Stream, "R14", (uintptr_t)lpExceptionRecord->ContextRecord->R14);
			lamda_print_register(Stream, "R15", (uintptr_t)lpExceptionRecord->ContextRecord->R15);

			auto lamda_is_rflag = [](DWORD Flags, uint8_t bit) -> uint8_t
			{
				return (uint8_t)((Flags & (1 << bit)) >> bit);
			};
			
			fprintf(Stream, "\n\tZF\t%u\tPF\t%u\tAF\t%u\n\tOF\t%u\tSF\t%u\tDF\t%u\n\tCF\t%u\tTF\t%u\tIF\t%u\n\n",
				lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 6), lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 2), 
				lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 4), lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 11), 
				lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 7), lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 10),
				lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 0), lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 8),
				lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 9));

			fprintf(Stream, "\tLastError\t%08X\n\n", GetLastError());
			fprintf(Stream, "\tGS\t%04X\tFS\t%04X\n\tES\t%04X\tDS\t%04X\n\tCS\t%04X\tSS\t%04X\n\n",
				lpExceptionRecord->ContextRecord->SegGs, lpExceptionRecord->ContextRecord->SegFs, lpExceptionRecord->ContextRecord->SegEs, 
				lpExceptionRecord->ContextRecord->SegDs, lpExceptionRecord->ContextRecord->SegCs, lpExceptionRecord->ContextRecord->SegSs);
			
			auto lamda_xmm_printf = [](FILE* Stream, uint8_t RegId, M128A Reg)
			{
				auto p = (DWORD*)&Reg;
				auto pf = (float*)&Reg;
				fprintf(Stream, "\tXMM%u\t%08X (%.6f)\t%08X (%.6f)\t%08X (%.6f)\t%08X (%.6f)\n", RegId, 
					p[0], pf[0], p[1], pf[1], p[2], pf[2], p[3], pf[3]);
			};

			lamda_xmm_printf(Stream, 0, lpExceptionRecord->ContextRecord->Xmm0);
			lamda_xmm_printf(Stream, 1, lpExceptionRecord->ContextRecord->Xmm1);
			lamda_xmm_printf(Stream, 2, lpExceptionRecord->ContextRecord->Xmm2);
			lamda_xmm_printf(Stream, 3, lpExceptionRecord->ContextRecord->Xmm3);
			lamda_xmm_printf(Stream, 4, lpExceptionRecord->ContextRecord->Xmm4);
			lamda_xmm_printf(Stream, 5, lpExceptionRecord->ContextRecord->Xmm5);
			lamda_xmm_printf(Stream, 6, lpExceptionRecord->ContextRecord->Xmm6);
			lamda_xmm_printf(Stream, 7, lpExceptionRecord->ContextRecord->Xmm7);
			lamda_xmm_printf(Stream, 8, lpExceptionRecord->ContextRecord->Xmm8);
			lamda_xmm_printf(Stream, 9, lpExceptionRecord->ContextRecord->Xmm9);
			lamda_xmm_printf(Stream, 10, lpExceptionRecord->ContextRecord->Xmm10);
			lamda_xmm_printf(Stream, 11, lpExceptionRecord->ContextRecord->Xmm11);
			lamda_xmm_printf(Stream, 12, lpExceptionRecord->ContextRecord->Xmm12);
			lamda_xmm_printf(Stream, 13, lpExceptionRecord->ContextRecord->Xmm13);
			lamda_xmm_printf(Stream, 14, lpExceptionRecord->ContextRecord->Xmm14);
			lamda_xmm_printf(Stream, 15, lpExceptionRecord->ContextRecord->Xmm15);

			fprintf(Stream, "\n");
			fflush(Stream);
		}

		void CrashHandler::PrintRegistrySafe(FILE* Stream, ModuleMapInfo* Modules, ArrayMemoryInfo* Memory, PEXCEPTION_POINTERS lpExceptionRecord)
		{
			__try
			{
				PrintRegistry(Stream, Modules, Memory, lpExceptionRecord);
			}
			__except (1)
			{
				fprintf(Stream, "\n");
			}
		}

		void CrashHandler::PrintCallStack(FILE* Stream, ModuleMapInfo* Modules)
		{
			if (!Modules)
				return;

			fprintf(Stream, "PROBABLE CALL STACK:\n");

			uint32_t id = 0;
			for (auto itS : ProbablyCallStack)
			{
				auto Info = GlobalIntrospection->Analyze(Stream, itS, Modules, nullptr);
				if (Info.Type == Introspection::aitCode)
				{
					fprintf(Stream, "\t[%-*u]: 0x%016llX %s\n", 3, id, itS, Info.Text.c_str());
					id++;
				}
			}

			fprintf(Stream, "\n");
			fflush(Stream);
		}

		void CrashHandler::PrintCallStackSafe(FILE* Stream, ModuleMapInfo* Modules)
		{
			__try
			{
				PrintCallStack(Stream, Modules);
			}
			__except (1)
			{
				fprintf(Stream, "\n");
			}
		}

		void CrashHandler::PrintStack(FILE* Stream, ModuleMapInfo* Modules, ArrayMemoryInfo* Memory, PEXCEPTION_POINTERS lpExceptionInfo)
		{
			if (!lpExceptionInfo || !Modules || !GlobalIntrospection)
				return;

			fprintf(Stream, "STACK:\n");

			if (!GlobalCrashTib || !GlobalCrashTib->StackBase)
				fprintf(Stream, "\tFAILED TO READ TIB\n");
			else
			{
				uintptr_t stack_end = GlobalCrashTib->StackBase;
				uintptr_t stack_base = GlobalCrashTib->StackLimit;
				uintptr_t stack_last = lpExceptionInfo->ContextRecord->Rsp;
				uintptr_t stack_iter = stack_last;

				fprintf(Stream, "\tBase: %llX / %llX Last: %llX\n", stack_base, stack_end, stack_last);

				while ((stack_iter < (stack_end - 8)) && ((stack_iter - stack_last) < 0x2500))
				{
					EditorAPI::BSString AnalizeText;
					auto Analize = GlobalIntrospection->Analyze(Stream, *(uintptr_t*)stack_iter, Modules, Memory);

					fprintf(Stream, "\t[RSP+%-*X] 0x%-*llX ", 4, (uint32_t)(stack_iter - stack_last), 16, *(uintptr_t*)stack_iter);

					switch (Analize.Type)
					{
					case Introspection::aitCode:
						fprintf(Stream, "(void* -> %s)\n", Analize.Text.c_str());
						break;
					case Introspection::aitInstance:
						fprintf(Stream, "(HINSTANCE*) %s\n", Analize.Text.c_str());
						break;
					case Introspection::aitNumber:
						if (*(uintptr_t*)stack_iter >> 63)
							fprintf(Stream, "(size_t) [uint: %llu int: %lld]\n", *(uintptr_t*)stack_iter, *(ptrdiff_t*)stack_iter);
						else
							fprintf(Stream, "(size_t) [%llu]\n", *(uintptr_t*)stack_iter);
						break;
					case Introspection::aitClass:
						if (Analize.Additional.IsEmpty())
							fprintf(Stream, "(%s*)\n", Analize.Text.c_str());
						else
							fprintf(Stream, "(%s*) | %s\n", Analize.Text.c_str(), Analize.Additional.c_str());
						break;
					case Introspection::aitString:
						fprintf(Stream, "(char*) %s\n", Analize.Text.c_str());
						break;
					default:
						fprintf(Stream, "(void*)\n");
						break;
					}

					stack_iter += 8;
				}
			}

			fprintf(Stream, "\n");
			fflush(Stream);
		}

		void CrashHandler::PrintStackSafe(FILE* Stream, ModuleMapInfo* Modules, ArrayMemoryInfo* Memory, PEXCEPTION_POINTERS lpExceptionInfo)
		{
			__try
			{
				PrintStack(Stream, Modules, Memory, lpExceptionInfo);
			}
			__except (1)
			{
				fprintf(Stream, "\n");
			}
		}

		void CrashHandler::PrintPatches(FILE* Stream)
		{
			fprintf(Stream, "PATCHES:\n");

			auto Manager = GlobalEnginePtr->GetPatchesManager();

			fprintf(Stream, "\tTotal: %u\n", Manager->Count());

			uint32_t id = 0;
			auto Modules = Manager->GetModuleMap();
			for (auto itM = Modules->begin(); itM != Modules->end(); itM++)
			{
				auto& Patch = itM->second;
				if (Patch->HasActive())
				{
					fprintf(Stream, "\t[%04u]: %s\n", id, Patch->GetName());
					id++;
				}
			}

			fprintf(Stream, "\n");
			fflush(Stream);
		}

		void CrashHandler::PrintModules(FILE* Stream, ModuleMapInfo* Modules)
		{
			fprintf(Stream, "MODULES:\n");
			fprintf(Stream, "\tTotal: %u\n", (uint32_t)Modules->size());

			std::size_t column_max = 0;
			for (auto itM = Modules->begin(); itM != Modules->end(); itM++)
				column_max = std::max(column_max, itM->first.length());
			
			for (auto itM = Modules->begin(); itM != Modules->end(); itM++)
				fprintf(Stream, "\t%-*s\t%016llX\n", (unsigned int)column_max, itM->first.c_str(), itM->second.Start);

			fprintf(Stream, "\n");
			fflush(Stream);
		}

		void CrashHandler::PrintPlugins(FILE* Stream)
		{
			fprintf(Stream, "PLUGINS:\n");
			
			auto Manager = GlobalEnginePtr->GetUserPluginsManager();

			fprintf(Stream, "\tTotal: %u\n", Manager->Count());
			
			uint32_t id = 0;
			auto Plugins = Manager->GetPluginMap();
			for (auto itP = Plugins->begin(); itP != Plugins->end(); itP++, id++)
				fprintf(Stream, "\t[%04u]: %s\n", id, itP->second->GetPluginDllName());

			fprintf(Stream, "\n");
			fflush(Stream);
		}

		LONG WINAPI CrashHandler::DumpExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo)
		{
			GlobalCrashDumpExceptionInfo = ExceptionInfo;
			GlobalCrashDumpTargetThreadId = GetCurrentThreadId();
			GlobalCrashTib = (NT_TIB64*)__readgsqword(0x30);

			SetEvent(GlobalCrashDumpTriggerEvent);
			Sleep(INFINITE);

			return EXCEPTION_CONTINUE_SEARCH;
		}

		void CrashHandler::ContextWriteToCrashLogSafe(FILE* Stream, PEXCEPTION_POINTERS ExceptionInfo)
		{
			fprintf(Stream, "\n\n====== CRASH INFO ======\n\n");
			fprintf(Stream, "CK %s\n", allowedEditorVersionStr[(size_t)GlobalEnginePtr->GetEditorVersion()].data());
			fprintf(Stream, "CKPE Runtime %s %s %s\n", VER_FILE_VERSION_STR, __DATE__, __TIME__);
			fflush(Stream);

			ModuleMapInfo Modules;
			ArrayMemoryInfo Memory;

			DWORD cbNeeded;
			auto hProcess = GetCurrentProcess();

			// Get a list of all the modules in this process.
			if (EnumProcessModules(hProcess, GlobalCrashLoadedModules, sizeof(GlobalCrashLoadedModules), &cbNeeded))
			{
				for (DWORD i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
				{
					CHAR szModName[MAX_PATH];
					// Get the full path to the module's file.
					if (GetModuleFileNameExA(hProcess, GlobalCrashLoadedModules[i], szModName, ARRAYSIZE(szModName)))
					{
						MODULEINFO Info;
						GetModuleInformation(hProcess, GlobalCrashLoadedModules[i], &Info, sizeof(MODULEINFO));

						CrashHandler::Region secCode;
						voltek::get_pe_section_range((uintptr_t)Info.lpBaseOfDll, ".text", &secCode.Start, &secCode.End);

						uintptr_t tempStart, tempEnd;
						if (voltek::get_pe_section_range((uintptr_t)Info.lpBaseOfDll, ".textbss", &tempStart, &tempEnd))
						{
							secCode.Start = std::min(secCode.Start, tempStart);
							secCode.End = std::max(secCode.End, tempEnd);
						}

						if (voltek::get_pe_section_range((uintptr_t)Info.lpBaseOfDll, ".interpr", &tempStart, &tempEnd))
						{
							secCode.Start = std::min(secCode.Start, tempStart);
							secCode.End = std::max(secCode.End, tempEnd);
						}

						Modules.insert(Modules.end(),
							std::make_pair<String, Module>(PathFindFileNameA(szModName),
								{ (uintptr_t)Info.lpBaseOfDll, (uintptr_t)Info.lpBaseOfDll + (uintptr_t)Info.SizeOfImage, secCode }));
					}
				}
			}

			MEMORY_BASIC_INFORMATION mbi;
			ZeroMemory(&mbi, sizeof(mbi));

			uint64_t Address = 0;
			while (Address < std::numeric_limits<uint64_t>::max() && VirtualQuery((LPCVOID)Address, std::addressof(mbi), sizeof(mbi)))
			{
				if ((mbi.State == MEM_COMMIT) && !(mbi.Protect & PAGE_GUARD) && 
					!((mbi.Protect & PAGE_EXECUTE) || (mbi.Protect & PAGE_EXECUTE_READ) || 
					  (mbi.Protect & PAGE_EXECUTE_READWRITE) || (mbi.Protect & PAGE_EXECUTE_WRITECOPY)))
					Memory.push_back({ (uintptr_t)mbi.BaseAddress, (uintptr_t)mbi.BaseAddress + mbi.RegionSize });

				Address += mbi.RegionSize;
				ZeroMemory(&mbi, sizeof(mbi));  
			}

			PEXCEPTION_RECORD lpExceptionRecord = ExceptionInfo->ExceptionRecord;

			PrintException(Stream, &Modules, lpExceptionRecord);
			PrintSettings(Stream);
			PrintSysInfo(Stream);
			//PrintMemory(Stream, &Memory);
			PrintRegistrySafe(Stream, &Modules, &Memory, ExceptionInfo);
			PrintStackSafe(Stream, &Modules, &Memory, ExceptionInfo);
			PrintCallStackSafe(Stream, &Modules);
			PrintPatches(Stream);
			PrintModules(Stream, &Modules);
			PrintPlugins(Stream);
		}

		void CrashHandler::ContextWriteToCrashLog(FILE* Stream, PEXCEPTION_POINTERS ExceptionInfo)
		{
			__try
			{
				ContextWriteToCrashLogSafe(Stream, ExceptionInfo);
			}
			__except (1)
			{
				// nope
			}
		}
	}
}