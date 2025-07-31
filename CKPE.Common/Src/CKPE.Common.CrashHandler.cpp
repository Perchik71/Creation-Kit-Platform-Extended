// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <new.h>
#include <signal.h>
#include <psapi.h>
#include <DbgHelp.h>
#include <Zydis/Zydis.h>
#include <resource.h>
#include <CKPE.Stream.h>
#include <CKPE.Zipper.h>
#include <CKPE.Application.h>
#include <CKPE.HardwareInfo.h>
#include <CKPE.PathUtils.h>
#include <CKPE.FileUtils.h>
#include <CKPE.Module.h>
#include <CKPE.GDIPlus.h>
#include <CKPE.Graphics.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Common.RTTI.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.CrashHandler.h>
#include <CKPE.Common.PatchManager.h>
#include <CKPE.Common.ModernTheme.h>
#include <CKPE.Common.UIVarCommon.h>
#include <vector>
#include <atomic>
#include <thread>
#include <unordered_map>

namespace CKPE
{
	namespace Common
	{
		///////////////////////////////////////////////////
		/// CrashEvent

		class CrashEvent
		{
			HANDLE Trigger{ nullptr };
			NT_TIB64* Tib{ nullptr };
			std::vector<std::uintptr_t> ProbablyCallStack;

			CrashEvent(const CrashEvent&) = delete;
			CrashEvent& operator=(const CrashEvent&) = delete;
		public:
			CrashEvent() noexcept(true);
			virtual ~CrashEvent() noexcept(true);

			virtual void IntoProbablyCallStack(std::uintptr_t address) noexcept(true);
			virtual std::vector<std::uintptr_t>& GetProbablyCallStack() noexcept(true);
			virtual NT_TIB64* GetTib() noexcept(true);
			virtual void CallError(NT_TIB64* tib) noexcept(true);
			virtual bool Wait() noexcept(true);
		};

		///////////////////////////////////////////////////
		/// Introspection

		class Introspection
		{
			bool Init{ false };
			ZydisDecoder Decoder{};
			ZydisFormatter Formatter{};
		public:
			struct Module { std::uintptr_t Start, End; CKPE::Segment SecCode; };
			using ModuleMapInfo = std::unordered_map<std::string, Module>;
			using ArrayMemoryInfo = std::vector<CKPE::Segment>;

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
				std::string Text;
				std::string Additional;
			};

			Introspection();

			static void PrintException(TextFileStream& Stream, ModuleMapInfo* Modules,
				PEXCEPTION_RECORD lpExceptionRecord) noexcept(true);
			static void PrintSettings(TextFileStream& Stream) noexcept(true);
			static void PrintSysInfo(TextFileStream& Stream) noexcept(true);
			//static void PrintMemory(TextFileStream& Stream, ArrayMemoryInfo* Memory) noexcept(true);
			static void PrintRegistry(TextFileStream& Stream, ModuleMapInfo* Modules, ArrayMemoryInfo* Memory,
				PEXCEPTION_POINTERS lpExceptionRecord) noexcept(true);
			static void PrintRegistrySafe(TextFileStream& Stream, ModuleMapInfo* Modules, ArrayMemoryInfo* Memory,
				PEXCEPTION_POINTERS lpExceptionRecord);
			static void PrintCallStack(TextFileStream& Stream, ModuleMapInfo* Modules) noexcept(true);
			static void PrintCallStackSafe(TextFileStream& Stream, ModuleMapInfo* Modules);
			static void PrintStack(TextFileStream& Stream, ModuleMapInfo* Modules, ArrayMemoryInfo* Memory,
				PEXCEPTION_POINTERS lpExceptionInfo) noexcept(true);
			static void PrintStackSafe(TextFileStream& Stream, ModuleMapInfo* Modules, ArrayMemoryInfo* Memory,
				PEXCEPTION_POINTERS lpExceptionInfo);
			static void PrintPatches(TextFileStream& Stream) noexcept(true);
			static void PrintModules(TextFileStream& Stream, ModuleMapInfo* Modules) noexcept(true);
			static void PrintPlugins(TextFileStream& Stream) noexcept(true);

			static void ContextWriteToCrashLogSafe(TextFileStream& Stream, 
				PEXCEPTION_POINTERS ExceptionInfo) noexcept(true);
			static void ContextWriteToCrashLog(TextFileStream& Stream, PEXCEPTION_POINTERS ExceptionInfo);

			[[nodiscard]] AnalyzeInfo Analyze(std::uintptr_t Address, ModuleMapInfo* Modules,
				ArrayMemoryInfo* Memory) const noexcept(true);
		private:
			[[nodiscard]] bool AnalyzeClass(std::uintptr_t Address, AnalyzeInfo* Info,
				std::uintptr_t RefAddress = 0) const noexcept(true);
		};

		///////////////////////////////////////////////////
		/// CrashEvent continue

		CrashEvent::CrashEvent() noexcept(true)
		{
			Trigger = CreateEventA(nullptr, false, false, nullptr);
			ResetEvent(Trigger);
		}

		CrashEvent::~CrashEvent() noexcept(true)
		{
			CloseHandle(Trigger);
		}

		void CrashEvent::IntoProbablyCallStack(std::uintptr_t address) noexcept(true)
		{
			ProbablyCallStack.push_back(address);
		}

		std::vector<std::uintptr_t>& CrashEvent::GetProbablyCallStack() noexcept(true)
		{
			return ProbablyCallStack;
		}

		NT_TIB64* CrashEvent::GetTib() noexcept(true)
		{
			return Tib;
		}

		void CrashEvent::CallError(NT_TIB64* tib) noexcept(true)
		{
			Tib = tib;
			SetEvent(Trigger);
		}

		bool CrashEvent::Wait() noexcept(true)
		{
			auto result = WaitForSingleObject(Trigger, INFINITE) == WAIT_OBJECT_0;
			ResetEvent(Trigger);
			return result;
		}

		///////////////////////////////////////////////////
		/// Global

		CrashHandler GlobalCrashHandler;
		CrashEvent GlobalCrashEvent;
		Introspection GlobalCrashIntrospection;
		std::atomic<PEXCEPTION_POINTERS> GlobalCrashDumpExceptionInfo;
		std::atomic_uint32_t GlobalCrashDumpTargetThreadId;
		HMODULE GlobalModuleList[1024];

		///////////////////////////////////////////////////
		/// Introspection continue

		Introspection::Introspection()
		{
			if (ZYDIS_SUCCESS(ZydisDecoderInit(&Decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64)))
				if (ZYDIS_SUCCESS(ZydisFormatterInit(&Formatter, ZYDIS_FORMATTER_STYLE_INTEL)))
					Init = true;
		}

		bool Introspection::AnalyzeClass(std::uintptr_t Address, AnalyzeInfo* Info,
			std::uintptr_t RefAddress) const noexcept(true)
		{
			auto ObjRtti = Common::RTTI::GetSingleton()->Find(Address, false);
			if (ObjRtti)
			{
				Info->Type = aitClass;
				Info->Text = ObjRtti->Name;
				auto it = Info->Text.find_first_of(' ');
				if (it != std::string::npos) Info->Text.erase(0, it + 1);

				if (RefAddress && GlobalCrashHandler.OnAnalyzeClassRef)
					GlobalCrashHandler.OnAnalyzeClassRef(RefAddress, ObjRtti->Name, Info->Additional);

				return true;
			}

			return false;
		}

		Introspection::AnalyzeInfo Introspection::Analyze(std::uintptr_t Address,
			ModuleMapInfo* Modules, ArrayMemoryInfo* Memory) const noexcept(true)
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

				PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)(itMod->second.Start + 
					((PIMAGE_DOS_HEADER)itMod->second.Start)->e_lfanew);

				if ((Address >= itMod->second.SecCode.GetAddress()) && (Address < itMod->second.SecCode.GetEndAddress()))
				{
					Info.Type = aitCode;
					Info.Text = StringUtils::FormatString("%s+%X ", itMod->first.c_str(),
						(uint32_t)(Address - itMod->second.Start));

					char InstructionText[256];
					ZydisDecodedInstruction Instruction;
					if (ZYDIS_SUCCESS(ZydisDecoderDecodeBuffer(&Decoder, (void*)Address, ZYDIS_MAX_INSTRUCTION_LENGTH, 
						Address, &Instruction)))
					{
						ZydisFormatterFormatInstruction(&Formatter, &Instruction, InstructionText, ARRAYSIZE(InstructionText));
						Info.Text.append(InstructionText);
						GlobalCrashEvent.IntoProbablyCallStack(Address);
					}
					else
						Info.Text.append("<FATAL DECODER INSTRUCTION>");
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
						if ((Address >= itMem->GetAddress()) && (Address < itMem->GetEndAddress()))
							break;
					}

					if (itMem != Memory->end())
					{
						auto Tib = GlobalCrashEvent.GetTib();
						if (Tib && (Tib->StackLimit <= Address) && (Tib->StackBase > Address))
						{
							// Stack
							auto RefInfo = Analyze(*(uintptr_t*)Address, Modules, Memory);
							if (RefInfo.Type != aitCode)
							{
								Info.Type = RefInfo.Type;
								if ((RefInfo.Type == aitNumber) || (RefInfo.Type == aitClass) || (RefInfo.Type == aitString))
									Info.Text.append(StringUtils::FormatString("&%s", RefInfo.Text.c_str()));
								else
								{
									if (!AnalyzeClass(*(uintptr_t*)Address, &Info, Address))
										Info.Type = aitUnknown;
								}
							}
						}
						else
						{
							if (!AnalyzeClass(*(uintptr_t*)Address, &Info, Address))
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
									Info.Text.append(StringUtils::FormatString("\"%s\"", str));
								}
							}
						}
					}
					else
					{
						if (!AnalyzeClass(Address, &Info))
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
								std::string a(str, len);
								Info.Type = aitString;
								Info.Text.append(StringUtils::FormatString("\"%s\"", a.c_str()));
							}
							else
								Info.Type = aitNumber;
						}
					}
				}
			}

			return Info;
		}

		void Introspection::ContextWriteToCrashLogSafe(TextFileStream& Stream,
			PEXCEPTION_POINTERS ExceptionInfo) noexcept(true)
		{
			Stream.WriteString("====== CRASH INFO ======\n\n");
			
			if (GlobalCrashHandler.OnOutputCKVersion)
			{
				std::string str;
				GlobalCrashHandler.OnOutputCKVersion(str);
				Stream.WriteLine("CK %s", str.c_str());
			}

			if (GlobalCrashHandler.OnOutputVersion)
			{
				std::string str;
				GlobalCrashHandler.OnOutputVersion(str);
				Stream.WriteLine(str.c_str());
			}

			Stream.Flush();

			ModuleMapInfo Modules;
			ArrayMemoryInfo Memory;

			DWORD cbNeeded;
			auto hProcess = GetCurrentProcess();
			// Get a list of all the modules in this process.
			if (EnumProcessModules(hProcess, GlobalModuleList, sizeof(GlobalModuleList), &cbNeeded))
			{
				for (DWORD i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
				{
					CHAR szModName[MAX_PATH];
					// Get the full path to the module's file.
					if (GetModuleFileNameExA(hProcess, GlobalModuleList[i], szModName, ARRAYSIZE(szModName)))
					{
						MODULEINFO Info;
						GetModuleInformation(hProcess, GlobalModuleList[i], &Info, sizeof(MODULEINFO));

						CKPE::Module smodule(GlobalModuleList[i]);
						auto secCode = smodule.GetSegment(CKPE::Segment::text);
						Modules.insert(Modules.end(),
							std::make_pair<std::string, Module>(PathFindFileNameA(szModName),
								{ (uintptr_t)Info.lpBaseOfDll, (uintptr_t)Info.lpBaseOfDll + (uintptr_t)Info.SizeOfImage, 
								  secCode }));
					}
				}
			}

			MEMORY_BASIC_INFORMATION mbi;
			ZeroMemory(&mbi, sizeof(mbi));

			uint64_t Address = 0;
			while (Address < std::numeric_limits<uint64_t>::max() &&
				VirtualQuery((LPCVOID)Address, std::addressof(mbi), sizeof(mbi)))
			{
				if ((mbi.State == MEM_COMMIT) && !(mbi.Protect & PAGE_GUARD) &&
					!((mbi.Protect & PAGE_EXECUTE) || (mbi.Protect & PAGE_EXECUTE_READ) ||
						(mbi.Protect & PAGE_EXECUTE_READWRITE) || (mbi.Protect & PAGE_EXECUTE_WRITECOPY)))
					Memory.push_back({ (uintptr_t)mbi.BaseAddress, (uintptr_t)mbi.BaseAddress, mbi.RegionSize });

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

		void Introspection::ContextWriteToCrashLog(TextFileStream& Stream, PEXCEPTION_POINTERS ExceptionInfo)
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

		void Introspection::PrintException(TextFileStream& Stream, ModuleMapInfo* Modules,
			PEXCEPTION_RECORD lpExceptionRecord) noexcept(true)
		{
			if (!lpExceptionRecord || !Modules)
				return;

			std::string ExceptionName;
			std::string ExceptionDescription;
			std::string ExceptionInstructionText;

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

			auto Analize = GlobalCrashIntrospection.Analyze((uintptr_t)lpExceptionRecord->ExceptionAddress, 
				Modules, nullptr);
			ExceptionInstructionText.append(Analize.Text);

			Stream.WriteLine("\nUnhandled exception ""%s"" at 0x%016llX %s", ExceptionName.c_str(),
				(uintptr_t)lpExceptionRecord->ExceptionAddress, ExceptionInstructionText.c_str());

			// Log exception flags
			Stream.WriteLine("Exception Flags: 0x%08X", lpExceptionRecord->ExceptionFlags);
			// Log number of parameters
			Stream.WriteLine("Number of Parameters: %u", lpExceptionRecord->NumberParameters);
			// Description
			Stream.WriteLine("Exception Description: %s", ExceptionDescription.c_str());

			// Log additional exception information for specific exception types
			if (lpExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
			{
				const auto accessType = lpExceptionRecord->ExceptionInformation[0] == 0 ? "read" :
					lpExceptionRecord->ExceptionInformation[0] == 1 ? "write" :
					lpExceptionRecord->ExceptionInformation[0] == 8 ? "execute" :
					"unknown";
				const auto faultAddress = lpExceptionRecord->ExceptionInformation[1];
				Stream.WriteLine("Access Violation: Tried to %s memory at 0x%016llX", accessType, faultAddress);
			}
			else if (lpExceptionRecord->ExceptionCode == EXCEPTION_IN_PAGE_ERROR)
			{
				const auto accessType = lpExceptionRecord->ExceptionInformation[0] == 0 ? "read" :
					lpExceptionRecord->ExceptionInformation[0] == 1 ? "write" :
					lpExceptionRecord->ExceptionInformation[0] == 8 ? "execute" :
					"unknown";
				const auto faultAddress = lpExceptionRecord->ExceptionInformation[1];
				const auto ntStatus = lpExceptionRecord->ExceptionInformation[2];
				Stream.WriteLine("In-Page Error: Tried to %s memory at 0x%016llX, NTSTATUS: 0x%08X", accessType, 
					faultAddress, (uint32_t)ntStatus);
			}
			else if (lpExceptionRecord->NumberParameters > 0)
			{
				Stream.WriteLine("Exception Information Parameters:");
				for (DWORD i = 0; i < lpExceptionRecord->NumberParameters; ++i)
					Stream.WriteLine("\tParameter[%u]: 0x%016llX:", i, lpExceptionRecord->ExceptionInformation[i]);
			}

			// Check for nested exceptions
			if (lpExceptionRecord->ExceptionRecord)
			{
				Stream.WriteLine("Nested Exception:");
				// Recursively print nested exception
				PrintException(Stream, Modules, lpExceptionRecord->ExceptionRecord);
			}

			Stream.WriteString("\n");
			Stream.Flush();
		}

		void Introspection::PrintSettings(TextFileStream& Stream) noexcept(true)
		{
			Interface::GetSingleton()->GetSettings()->Dump(Stream);
			Stream.WriteString("\n");
			Stream.Flush();
		}

		void Introspection::PrintSysInfo(TextFileStream& Stream) noexcept(true)
		{
			Stream.WriteLine("SYSTEM SPECS:");
			Stream.WriteLine("\tOS: %s", StringUtils::Utf16ToWinCP(HardwareInfo::OS::GetVersionByStringEx()).c_str());
			Stream.WriteLine("\tCPU: %s", HardwareInfo::CPU::GetBrand().c_str());

			for (uint8_t i = 0; i < HardwareInfo::GPU::GetCount(); i++)
				Stream.WriteLine("\tGPU #%u: %s %u Gb", i + 1, 
					HardwareInfo::GPU::GetName(i).c_str(), HardwareInfo::GPU::GetMemorySizeByUint(i));

			auto pm = HardwareInfo::OS::GetPhysicalMemory();
			Stream.WriteLine("\tPHYSICAL MEMORY: %.2f GB / %.2f GB", pm.Total - pm.Free, pm.Total);
			auto sm = HardwareInfo::OS::GetSharedMemory();
			Stream.WriteLine("\tSHARED MEMORY: %.2f GB / %.2f GB\n", sm.Total - sm.Free, sm.Total);
			Stream.Flush();
		}

		/*void Introspection::PrintMemory(TextFileStream& Stream, ArrayMemoryInfo* Memory) noexcept(true)
		{
			if (!Memory)
				return;

			fprintf(Stream, "MEMORY:\n");

			uint32_t id = 0;
			for (auto itM = Memory->begin(); itM != Memory->end(); itM++, id++)
				fprintf(Stream, "\t[%04u]: %llX / %llX\n", id, itM->Start, itM->End);

			Stream.WriteString("\n");
			Stream.Flush();
		}*/

		void Introspection::PrintRegistry(TextFileStream& Stream, ModuleMapInfo* Modules,
			ArrayMemoryInfo* Memory, PEXCEPTION_POINTERS lpExceptionRecord) noexcept(true)
		{
			if (!Modules || !lpExceptionRecord)
				return;

			Stream.WriteLine("REGISTERS:");

			auto lamda_print_register = [&](TextFileStream& Stream, const char* NameReg, uintptr_t Value)
				{
					Stream.WriteString("\t%s %-*llX ", NameReg, 16, Value);

					std::string AnalizeText;
					auto Analize = GlobalCrashIntrospection.Analyze(Value, Modules, Memory);

					switch (Analize.Type)
					{
					case Introspection::aitCode:
						Stream.WriteLine("(void* -> %s)", Analize.Text.c_str());
						break;
					case Introspection::aitInstance:
						Stream.WriteLine("(HINSTANCE*) %s", Analize.Text.c_str());
						break;
					case Introspection::aitNumber:
						if (Value >> 63)
							Stream.WriteLine("(size_t) [uint: %llu int: %lld]", Value, (ptrdiff_t)Value);
						else
							Stream.WriteLine("(size_t) [%llu]", Value);
						break;
					case Introspection::aitClass:
						if (!Analize.Additional.length())
							Stream.WriteLine("(%s*)", Analize.Text.c_str());
						else
							Stream.WriteLine("(%s*) | %s", Analize.Text.c_str(), Analize.Additional.c_str());
						break;
					case Introspection::aitString:
						Stream.WriteLine("(char*) %s", Analize.Text.c_str());
						break;
					default:
						Stream.WriteLine("(void*)");
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

			Stream.WriteLine("\n\tZF\t%u\tPF\t%u\tAF\t%u\n\tOF\t%u\tSF\t%u\tDF\t%u\n\tCF\t%u\tTF\t%u\tIF\t%u\n",
				lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 6), lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 2),
				lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 4), lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 11),
				lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 7), lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 10),
				lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 0), lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 8),
				lamda_is_rflag(lpExceptionRecord->ContextRecord->EFlags, 9));

			Stream.WriteLine("\tLastError\t%08X\n", GetLastError());
			Stream.WriteLine("\tGS\t%04X\tFS\t%04X\n\tES\t%04X\tDS\t%04X\n\tCS\t%04X\tSS\t%04X\n",
				lpExceptionRecord->ContextRecord->SegGs, lpExceptionRecord->ContextRecord->SegFs, lpExceptionRecord->ContextRecord->SegEs,
				lpExceptionRecord->ContextRecord->SegDs, lpExceptionRecord->ContextRecord->SegCs, lpExceptionRecord->ContextRecord->SegSs);

			auto lamda_xmm_printf = [](TextFileStream& Stream, uint8_t RegId, M128A Reg)
				{
					auto p = (DWORD*)&Reg;
					//auto pf = (float*)&Reg;
					//fprintf(Stream, "\tXMM%u\t%08X (%.6f)\t%08X (%.6f)\t%08X (%.6f)\t%08X (%.6f)\n", RegId, 
					//	p[0], pf[0], p[1], pf[1], p[2], pf[2], p[3], pf[3]);
					Stream.WriteLine("\tXMM%u\t%08X %08X %08X %08X", RegId, p[0], p[1], p[2], p[3]);
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

			Stream.WriteString("\n");
			Stream.Flush();
		}

		void Introspection::PrintRegistrySafe(TextFileStream& Stream, ModuleMapInfo* Modules,
			ArrayMemoryInfo* Memory, PEXCEPTION_POINTERS lpExceptionRecord)
		{
			__try
			{
				PrintRegistry(Stream, Modules, Memory, lpExceptionRecord);
			}
			__except (1)
			{
				Stream.WriteString("\n");
			}
		}

		void Introspection::PrintCallStack(TextFileStream& Stream, ModuleMapInfo* Modules) noexcept(true)
		{
			if (!Modules)
				return;

			Stream.WriteLine("PROBABLE CALL STACK:");

			uint32_t id = 0;
			for (auto itS : GlobalCrashEvent.GetProbablyCallStack())
			{
				auto Info = GlobalCrashIntrospection.Analyze(itS, Modules, nullptr);
				if (Info.Type == Introspection::aitCode)
				{
					Stream.WriteLine("\t[%-*u]: 0x%016llX %s", 3, id, itS, Info.Text.c_str());
					id++;
				}
			}

			Stream.WriteString("\n");
			Stream.Flush();
		}

		void Introspection::PrintCallStackSafe(TextFileStream& Stream, ModuleMapInfo* Modules)
		{
			__try
			{
				PrintCallStack(Stream, Modules);
			}
			__except (1)
			{
				Stream.WriteString("\n");
			}
		}

		void Introspection::PrintStack(TextFileStream& Stream, ModuleMapInfo* Modules,
			ArrayMemoryInfo* Memory, PEXCEPTION_POINTERS lpExceptionInfo) noexcept(true)
		{
			if (!lpExceptionInfo || !Modules)
				return;

			Stream.WriteLine("STACK:");

			auto tib = GlobalCrashEvent.GetTib();
			if (!tib || !tib->StackBase)
				Stream.WriteLine("\tFAILED TO READ TIB");
			else
			{
				uintptr_t stack_end = tib->StackBase;
				uintptr_t stack_base = tib->StackLimit;
				uintptr_t stack_last = lpExceptionInfo->ContextRecord->Rsp;
				uintptr_t stack_iter = stack_last;

				Stream.WriteLine("\tBase: %llX / %llX Last: %llX", stack_base, stack_end, stack_last);

				while ((stack_iter < (stack_end - 8)) && ((stack_iter - stack_last) < 0x2500))
				{
					std::string AnalizeText;
					auto Analize = GlobalCrashIntrospection.Analyze(*(uintptr_t*)stack_iter, Modules, Memory);

					Stream.WriteString("\t[RSP+%-*X] 0x%-*llX ", 4, (uint32_t)(stack_iter - stack_last), 16, 
						*(uintptr_t*)stack_iter);

					switch (Analize.Type)
					{
					case Introspection::aitCode:
						Stream.WriteLine("(void* -> %s)", Analize.Text.c_str());
						break;
					case Introspection::aitInstance:
						Stream.WriteLine("(HINSTANCE*) %s", Analize.Text.c_str());
						break;
					case Introspection::aitNumber:
						if (*(uintptr_t*)stack_iter >> 63)
							Stream.WriteLine("(size_t) [uint: %llu int: %lld]", *(uintptr_t*)stack_iter, *(ptrdiff_t*)stack_iter);
						else
							Stream.WriteLine("(size_t) [%llu]", *(uintptr_t*)stack_iter);
						break;
					case Introspection::aitClass:
						if (!Analize.Additional.length())
							Stream.WriteLine("(%s*)", Analize.Text.c_str());
						else
							Stream.WriteLine("(%s*) | %s", Analize.Text.c_str(), Analize.Additional.c_str());
						break;
					case Introspection::aitString:
						Stream.WriteLine("(char*) %s", Analize.Text.c_str());
						break;
					default:
						Stream.WriteLine("(void*)");
						break;
					}

					stack_iter += 8;
				}
			}

			Stream.WriteString("\n");
			Stream.Flush();
		}

		void Introspection::PrintStackSafe(TextFileStream& Stream, ModuleMapInfo* Modules,
			ArrayMemoryInfo* Memory, PEXCEPTION_POINTERS lpExceptionInfo)
		{
			__try
			{
				PrintStack(Stream, Modules, Memory, lpExceptionInfo);
			}
			__except (1)
			{
				Stream.WriteString("\n");
			}
		}

		void Introspection::PrintPatches(TextFileStream& Stream) noexcept(true)
		{
			Stream.WriteLine("PATCHES:");

			auto Manager = PatchManager::GetSingleton();

			Stream.WriteLine("\tTotal: %u", Manager->GetCount());

			uint32_t id = 0;
			auto Entries = Manager->GetEntries();
			if (Entries)
			{
				for (auto& itP : *Entries)
				{
					if (itP.patch->IsActive())
					{
						Stream.WriteLine("\t[%04u]: %s", id, itP.patch->GetName().c_str());
						id++;
					}
				}
			}

			Stream.WriteString("\n");
			Stream.Flush();
		}

		void Introspection::PrintModules(TextFileStream& Stream, ModuleMapInfo* Modules) noexcept(true)
		{
			Stream.WriteLine("MODULES:");
			Stream.WriteLine("\tTotal: %u", (uint32_t)Modules->size());

			std::size_t column_max = 0;
			for (auto itM = Modules->begin(); itM != Modules->end(); itM++)
				column_max = std::max(column_max, itM->first.length());

			for (auto itM = Modules->begin(); itM != Modules->end(); itM++)
				Stream.WriteLine("\t%-*s\t%016llX", (unsigned int)column_max, itM->first.c_str(), itM->second.Start);

			Stream.WriteString("\n");
			Stream.Flush();
		}

		void Introspection::PrintPlugins(TextFileStream& Stream) noexcept(true)
		{
			/*Stream.WriteString("PLUGINS:\n");

			auto Manager = GlobalEnginePtr->GetUserPluginsManager();

			Stream.WriteString("\tTotal: %u\n", Manager->Count());

			uint32_t id = 0;
			auto Plugins = Manager->GetPluginMap();
			for (auto itP = Plugins->begin(); itP != Plugins->end(); itP++, id++)
				Stream.WriteString("\t[%04u]: %s\n", id, itP->second->GetPluginDllName());

			fprintf(Stream, "\n");
			Stream.Flush();*/
		}

		///////////////////////////////////////////////////
		/// CrashHandler

		static LONG WINAPI DumpExceptionHandler(PEXCEPTION_POINTERS ExceptionInfo) noexcept(true)
		{
			GlobalCrashDumpExceptionInfo = ExceptionInfo;
			GlobalCrashDumpTargetThreadId = GetCurrentThreadId();
			GlobalCrashEvent.CallError((NT_TIB64*)__readgsqword(0x30));
			// wait always (app no close)
			Sleep(INFINITE);

			return EXCEPTION_CONTINUE_SEARCH;
		}

		void CrashHandler::SetProcessExceptionHandlers() noexcept(true)
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

		void CrashHandler::SetThreadExceptionHandlers() noexcept(true)
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

		static bool TryMiniDumpWriteDump(HANDLE hProcess, DWORD ProcessId, 
			HANDLE hFile, MINIDUMP_TYPE DumpType, PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, 
			PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, PMINIDUMP_CALLBACK_INFORMATION CallbackParam)
		{
			// Crashes RtlInitStrings (maybe not produce a valid stack trace for the calling thread)
			// https://learn.microsoft.com/en-us/windows/win32/api/minidumpapiset/nf-minidumpapiset-minidumpwritedump
			__try
			{
				return MiniDumpWriteDump(hProcess, ProcessId, hFile, DumpType, ExceptionParam, 
					UserStreamParam, CallbackParam);
			}
			__except (1)
			{
				return false;
			}
		}

		void CrashHandler::AssertWithCrashReport(const char* File, std::int32_t Line, const char* ErrorMessage,
			void* ExceptionInfo) noexcept(true)
		{
			auto pExceptionInfo = (PEXCEPTION_POINTERS)ExceptionInfo;

			struct CrashDlgParam
			{
				LPCSTR File;
				int Line;
				LPCSTR ErrorMessage;
				PEXCEPTION_POINTERS ExceptionInfo;
			};

			constexpr static auto CrashReportFName = "CreationKitPlatformExtendedCrashReport.log";

			static CrashDlgParam Param;
			Param.File = File;
			Param.Line = Line;
			Param.ErrorMessage = ErrorMessage;
			Param.ExceptionInfo = pExceptionInfo;

			static auto proc = [](HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) -> INT_PTR {
				static HFONT CaptionFont = nullptr;
				static HBITMAP CKPEFemale = nullptr;
				static HBITMAP ErrorImage = nullptr;

				switch (message)
				{
				case WM_INITDIALOG:
				{
					auto instDLL = Interface::GetSingleton()->GetInstanceDLL();
					CrashDlgParam* Param = (CrashDlgParam*)lParam;

					static int ImageWidth = 306;
					static int ImageHeight = 450;

					auto WndImage = GetDlgItem(hwndDlg, IDC_STATIC_CRASH_PICTURE);
					CKPEFemale = (HBITMAP)GDIPlus::LoadImageFromResource((THandle)instDLL, IDB_PNG4, L"PNG");
					SendMessage(WndImage, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)CKPEFemale);
					RedrawWindow(WndImage, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

					RECT rc;
					GetClientRect(hwndDlg, &rc);

					auto monitor = Screen::GetSingleton()->MonitorFromWindow(hwndDlg);
					CRECT wa = monitor.WorkAreaRect;

					SetWindowPos(hwndDlg, NULL, (wa.Width - rc.right) >> 1, (wa.Height - rc.bottom) >> 1, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
					MoveWindow(WndImage, 24, (rc.bottom - ImageHeight) >> 1, ImageWidth, ImageHeight, TRUE);

					CaptionFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
						CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_ROMAN,
						"Microsoft Sans Serif");

					auto WndCaption = GetDlgItem(hwndDlg, IDC_STATIC_CRASH_CAPTION);
					SendMessageA(WndCaption, WM_SETFONT, (WPARAM)CaptionFont, TRUE);

					auto WndCaptionImageError = GetDlgItem(hwndDlg, IDC_STATIC_CRASH_ERROR_PIC);
					ErrorImage = (HBITMAP)GDIPlus::LoadImageFromResource((THandle)instDLL, IDB_PNG5, L"PNG");
					SendMessageA(WndCaptionImageError, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)ErrorImage);
					SetWindowPos(WndCaptionImageError, NULL, 0, 0, 32, 32, SWP_NOMOVE | SWP_NOZORDER);
					RedrawWindow(WndCaptionImageError, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

					auto WndVersion = GetDlgItem(hwndDlg, IDC_STATIC_CRASH_VERSION);
					if (WndVersion && GlobalCrashHandler.OnOutputVersion)
					{
						std::string ver;
						GlobalCrashHandler.OnOutputVersion(ver);
						SetWindowTextA(WndVersion, ver.c_str());
					}

					auto WndDetails = GetDlgItem(hwndDlg, IDC_EDIT_CRASH_DETAILS);
					if (WndDetails && Param)
					{
						SendMessageA(WndDetails, EM_LIMITTEXT, 500000, 0);

						bool no_ckpe_assert = !Param->File || !Param->File[0];
						if (no_ckpe_assert)
							SetWindowTextA(WndDetails, "ASSERTION: \r\n\r\n");
						else
						{
							auto ErrMsg = StringUtils::FormatString("CKPE_ASSERTION: \"%s\" line: %d\r\n\r\n", 
								Param->File, Param->Line);
							SetWindowTextA(WndDetails, ErrMsg.c_str());
						}

						int index = GetWindowTextLengthA(WndDetails);
						SendMessageA(WndDetails, EM_SETSEL, (WPARAM)index, (LPARAM)index);
						SendMessageA(WndDetails, EM_REPLACESEL, 0, (LPARAM)Param->ErrorMessage);

						index = GetWindowTextLengthA(WndDetails);
						SendMessageA(WndDetails, EM_SETSEL, (WPARAM)index, (LPARAM)index);
						SendMessageA(WndDetails, EM_REPLACESEL, 0, (LPARAM)"\r\n\r\n");

						auto fsize = FileUtils::GetFileSize(CrashReportFName);
						if (Param->ExceptionInfo && ((fsize > 10) && (fsize <= 499000)))
						{
							TextFileStream stm(CrashReportFName, FileStream::fmOpenRead);
							
							char* chSep = nullptr;
							auto TextData = std::make_unique<char[]>(2048);

							while (!stm.Eof())
							{
								stm.ReadLine(TextData.get(), 2048);

								chSep = strchr(TextData.get(), '\n');
								if (chSep && ((chSep - TextData.get()) <= 2044))
								{
									chSep[0] = '\r';
									chSep[1] = '\n';
									chSep[2] = '\0';
								}

								index = GetWindowTextLength(WndDetails);
								SendMessageA(WndDetails, EM_SETSEL, (WPARAM)index, (LPARAM)index);
								SendMessageA(WndDetails, EM_REPLACESEL, 0, (LPARAM)TextData.get());
							}
						}
					}

					return TRUE;
				}
				case WM_SHOWWINDOW:
				{
					SetForegroundWindow(hwndDlg);
					return TRUE;
				}
				case WM_NOTIFY:
				{
					switch (((LPNMHDR)lParam)->code)
					{
					case NM_CLICK:
					case NM_RETURN:
					{
						PNMLINK pNMLink = (PNMLINK)lParam;
						LITEM   item = pNMLink->item;

						if (item.iLink == 0)
						{
							SHELLEXECUTEINFOW ExecInfo;
							ZeroMemory(&ExecInfo, sizeof(ExecInfo));

							ExecInfo.cbSize = sizeof(ExecInfo);
							ExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
							ExecInfo.lpVerb = L"open";
							ExecInfo.lpFile = item.szUrl;
							ExecInfo.lpParameters = L"";
							ExecInfo.nShow = SW_SHOW;

							ShellExecuteExW(&ExecInfo);
						}
					}
					}

					return TRUE;
				}
				case WM_DESTROY:
				{
					DeleteObject(CKPEFemale);
					DeleteObject(ErrorImage);
					DeleteObject(CaptionFont);

					return TRUE;
				}
				case WM_COMMAND:
				{
					switch (wParam)
					{
					case IDOK:
					case IDCANCEL:
					{
						EndDialog(hwndDlg, wParam);
						return TRUE;
					}
					}
				}
				}
				return FALSE;
				};

			LogWindow::GetSingleton()->CloseOutputFile();

			if (Param.ExceptionInfo)
			{
				TextFileStream stm(CrashReportFName, FileStream::fmCreate);
				Introspection::ContextWriteToCrashLog(stm, Param.ExceptionInfo);
			}

			if (DialogBoxParamA((HINSTANCE)Interface::GetSingleton()->GetInstanceDLL(),
				MAKEINTRESOURCE(IDD_CRASHDIALOG), nullptr, proc, (LPARAM)&Param) == IDOK)
			{
				bool dumpWritten = false;
				auto BigDump = _READ_OPTION_BOOL("Crashes", "bGenerateFullDump", false);

				// Create a dump in the same folder of the exe itself
				SYSTEMTIME sysTime;
				char exePath[MAX_PATH];
				GetModuleFileNameA(GetModuleHandleA(nullptr), exePath, ARRAYSIZE(exePath));
				GetSystemTime(&sysTime);
				auto zip_fname = StringUtils::FormatString("%s_%4d%02d%02d_%02d%02d%02d.zip", exePath,
					sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);

				std::vector<std::string> slist;
				slist.push_back("CreationKitPlatformExtended.log");

				// Close and added archive the log's
				Interface::GetSingleton()->GetLogger()->Close();

				// Generate minidump
				if (Param.ExceptionInfo)
				{
					auto minidump_fname = PathUtils::ChangeFileExt(zip_fname, ".dmp");
					HANDLE file = CreateFileA(minidump_fname.c_str(), GENERIC_READ | GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL, nullptr);
					if (file != INVALID_HANDLE_VALUE)
					{
						MINIDUMP_EXCEPTION_INFORMATION dumpInfo
						{
							.ThreadId = GlobalCrashDumpTargetThreadId.load(),
							.ExceptionPointers = Param.ExceptionInfo,
							.ClientPointers = FALSE,
						};

						UINT32 dumpFlags =
							MiniDumpNormal | MiniDumpWithThreadInfo | MiniDumpWithIndirectlyReferencedMemory;

						if (BigDump)
							dumpFlags |= (MiniDumpWithDataSegs | MiniDumpWithFullMemory | MiniDumpWithHandleData | MiniDumpWithFullMemoryInfo);
						else
							dumpFlags |= MiniDumpWithoutOptionalData;

						if (TryMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
							file, (MINIDUMP_TYPE)dumpFlags, &dumpInfo, nullptr, nullptr))
							slist.push_back(minidump_fname);

						CloseHandle(file);
					}
				}

				LogWindow::GetSingleton()->CloseOutputFile();
				if (Param.ExceptionInfo)
				{
					constexpr static auto crash_log = "CreationKitPlatformExtendedCrash.log";
					LogWindow::GetSingleton()->SaveRichTextToFile(crash_log);
					if (PathUtils::FileExists(crash_log) && PathUtils::FileExists(CrashReportFName))
					{
						FileStream stm(crash_log, FileStream::fmOpenReadWrite);
						FileStream stm_cr(CrashReportFName, FileStream::fmOpenRead);

						stm.Offset(0, Stream::ofEnd);
						stm.CopyFrom(stm_cr, stm_cr.GetSize());

						slist.push_back(crash_log);
					}
				}

				Zipper::ZipFiles(zip_fname, slist);

				for (size_t i = 1; i < slist.size(); i++)
					DeleteFileA(slist[i].c_str());
			}
			
			DeleteFileA(CrashReportFName);
			
			if (IsDebuggerPresent())
			{
				if (Param.ErrorMessage)
					OutputDebugStringA(Param.ErrorMessage);

				__debugbreak();
			}

			Interface::GetSingleton()->GetApplication()->Terminate();
		}

		void CrashHandler::Install() noexcept(true)
		{
			SetProcessExceptionHandlers();
			SetThreadExceptionHandlers();

			std::thread t([]()
				{
					if (UI::IsDarkTheme())
						ModernTheme::InitializeCurrentThread();

					if (!GlobalCrashEvent.Wait())
						return;

					auto ehInfo = GlobalCrashDumpExceptionInfo.load();
					const char* reason = nullptr;

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

					char message[128];
					sprintf_s(message, "Reason: %s (0x%08X).\n\n", reason, ehInfo->ExceptionRecord->ExceptionCode);
					GlobalCrashHandler.AssertWithCrashReport("", 0, message, ehInfo);
				});
			t.detach();
		}

		CrashHandler* CrashHandler::GetSingleton() noexcept(true)
		{
			return &GlobalCrashHandler;
		}
	}
}