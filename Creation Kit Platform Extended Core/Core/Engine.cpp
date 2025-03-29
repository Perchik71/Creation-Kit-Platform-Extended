// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Version/resource_version2.h"
#include <VersionHelpers.h>

#include "ConsoleWindow.h"
#include "CommandLineParser.h"
#include "RelocationDatabase.h" 
#include "DialogManager.h"
#include "GDIPlusInit.h"
#include "Engine.h"
#include "DynamicCast.h"
#include "TracerManager.h"
#include "RegistratorWindow.h"
#include "CrashHandler.h"
#include "ResourcesPackerManager.h"

#include "Editor API/EditorUI.h"
#include "Editor API/BSString.h"

#include "Patches/INICacheData.h"
#include "Patches/CrashDumpPatch.h"
#include "Patches/MemoryManagerPatch.h"
#include "Patches/QuitHandlerPatch.h"
#include "Patches/ConsolePatch.h"
#include "Patches/FlowChartXPatch.h"
#include "Patches/UnicodePatch.h"
#include "Patches/ThreadPatch.h"
#include "Patches/VersionControlMergeWorkaround.h"
#include "Patches/RenderWindow60FPS.h"
#include "Patches/RenameCreationKitApp.h"
#include "Patches/FaceGen.h"
#include "Patches/DisableAssertion.h"
#include "Patches/SkipTopicInfoValidation.h"
#include "Patches/AllowMultipleWindowAndMaster.h"
#include "Patches/UIPatch.h"
#include "Patches/UIThemePatch.h"
#include "Patches/UIThemeClassicPatch.h"
#include "Patches/UIThemePatchAdditional.h"
#include "Patches/PapyrusEditorLimit.h"
#include "Patches/UIHotkeys.h"
#include "Patches/D3D11Patch.h"

#include "Experimental/RuntimeOptimization.h"

#ifdef _CKPE_WITH_QT5
#	include "QtCore/qresource.h"
#endif // !_CKPE_WITH_QT5

#define GET_VERSION_FROM_CMD_VER 1
#define HACK_VERSION_RETRIEVAL 1

#if GET_VERSION_FROM_CMD_VER == 0
#if HACK_VERSION_RETRIEVAL == 1

// In the Windows Native API Programming book by Pavel Yosifovich, it is mentioned a method to get the Windows version 
// through the KUSER_SHARED_DATA struct located at 0x7ffe0000 address in every process. For more information about KUSER_SHARED_DATA,
// read this MSDN link. KUSER_SHARED_DATA is located in <ntddk.h> that comes with Windows Driver Development Kit (DDK). 
// You must install the DDK to compile the code snippet below.
// https://www.codeproject.com/Articles/5336372/Windows-Version-Detection

#define PROCESSOR_FEATURE_MAX 64

typedef struct _KSYSTEM_TIME
{
	ULONG LowPart;
	LONG High1Time;
	LONG High2Time;
} KSYSTEM_TIME, * PKSYSTEM_TIME;

typedef enum _NT_PRODUCT_TYPE
{
	NtProductWinNt = 1,
	NtProductLanManNt = 2,
	NtProductServer = 3
} NT_PRODUCT_TYPE;

typedef enum _ALTERNATIVE_ARCHITECTURE_TYPE
{
	StandardDesign = 0,
	NEC98x86 = 1,
	EndAlternatives = 2
} ALTERNATIVE_ARCHITECTURE_TYPE;

typedef struct _KUSER_SHARED_DATA {
	ULONG                         TickCountLowDeprecated;
	ULONG                         TickCountMultiplier;
	KSYSTEM_TIME                  InterruptTime;
	KSYSTEM_TIME                  SystemTime;
	KSYSTEM_TIME                  TimeZoneBias;
	USHORT                        ImageNumberLow;
	USHORT                        ImageNumberHigh;
	WCHAR                         NtSystemRoot[260];
	ULONG                         MaxStackTraceDepth;
	ULONG                         CryptoExponent;
	ULONG                         TimeZoneId;
	ULONG                         LargePageMinimum;
	ULONG                         AitSamplingValue;
	ULONG                         AppCompatFlag;
	ULONGLONG                     RNGSeedVersion;
	ULONG                         GlobalValidationRunlevel;
	LONG                          TimeZoneBiasStamp;
	ULONG                         NtBuildNumber;
	NT_PRODUCT_TYPE               NtProductType;
	BOOLEAN                       ProductTypeIsValid;
	BOOLEAN                       Reserved0[1];
	USHORT                        NativeProcessorArchitecture;
	ULONG                         NtMajorVersion;
	ULONG                         NtMinorVersion;
	BOOLEAN                       ProcessorFeatures[PROCESSOR_FEATURE_MAX];
	ULONG                         Reserved1;
	ULONG                         Reserved3;
	ULONG                         TimeSlip;
	ALTERNATIVE_ARCHITECTURE_TYPE AlternativeArchitecture;
	ULONG                         BootId;
	LARGE_INTEGER                 SystemExpirationDate;
	ULONG                         SuiteMask;
	BOOLEAN                       KdDebuggerEnabled;
	union {
		UCHAR MitigationPolicies;
		struct {
			UCHAR NXSupportPolicy : 2;
			UCHAR SEHValidationPolicy : 2;
			UCHAR CurDirDevicesSkippedForDlls : 2;
			UCHAR Reserved : 2;
		};
	};
	USHORT                        CyclesPerYield;
	ULONG                         ActiveConsoleId;
	ULONG                         DismountCount;
	ULONG                         ComPlusPackage;
	ULONG                         LastSystemRITEventTickCount;
	ULONG                         NumberOfPhysicalPages;
	BOOLEAN                       SafeBootMode;
	union {
		UCHAR VirtualizationFlags;
		struct {
			UCHAR ArchStartedInEl2 : 1;
			UCHAR QcSlIsSupported : 1;
		};
	};
	UCHAR                         Reserved12[2];
	union {
		ULONG SharedDataFlags;
		struct {
			ULONG DbgErrorPortPresent : 1;
			ULONG DbgElevationEnabled : 1;
			ULONG DbgVirtEnabled : 1;
			ULONG DbgInstallerDetectEnabled : 1;
			ULONG DbgLkgEnabled : 1;
			ULONG DbgDynProcessorEnabled : 1;
			ULONG DbgConsoleBrokerEnabled : 1;
			ULONG DbgSecureBootEnabled : 1;
			ULONG DbgMultiSessionSku : 1;
			ULONG DbgMultiUsersInSessionSku : 1;
			ULONG DbgStateSeparationEnabled : 1;
			ULONG SpareBits : 21;
		} DUMMYSTRUCTNAME2;
	} DUMMYUNIONNAME2;
	ULONG                         DataFlagsPad[1];
	ULONGLONG                     TestRetInstruction;
	LONGLONG                      QpcFrequency;
	ULONG                         SystemCall;
	ULONG                         Reserved2;
	ULONGLONG                     FullNumberOfPhysicalPages;
	ULONGLONG                     SystemCallPad[1];
	union {
		KSYSTEM_TIME TickCount;
		ULONG64      TickCountQuad;
		struct {
			ULONG ReservedTickCountOverlay[3];
			ULONG TickCountPad[1];
		} DUMMYSTRUCTNAME;
	} DUMMYUNIONNAME3;
	ULONG                         Cookie;
	ULONG                         CookiePad[1];
	LONGLONG                      ConsoleSessionForegroundProcessId;
	ULONGLONG                     TimeUpdateLock;
	ULONGLONG                     BaselineSystemTimeQpc;
	ULONGLONG                     BaselineInterruptTimeQpc;
	ULONGLONG                     QpcSystemTimeIncrement;
	ULONGLONG                     QpcInterruptTimeIncrement;
	UCHAR                         QpcSystemTimeIncrementShift;
	UCHAR                         QpcInterruptTimeIncrementShift;
	USHORT                        UnparkedProcessorCount;
	ULONG                         EnclaveFeatureMask[4];
	ULONG                         TelemetryCoverageRound;
	USHORT                        UserModeGlobalLogger[16];
	ULONG                         ImageFileExecutionOptions;
	ULONG                         LangGenerationCount;
	ULONGLONG                     Reserved4;
	ULONGLONG                     InterruptTimeBias;
	ULONGLONG                     QpcBias;
	ULONG                         ActiveProcessorCount;
	UCHAR                         ActiveGroupCount;
	UCHAR                         Reserved9;
	union {
		USHORT QpcData;
		struct {
			UCHAR QpcBypassEnabled;
			UCHAR QpcReserved;
		};
	};
	LARGE_INTEGER                 TimeZoneBiasEffectiveStart;
	LARGE_INTEGER                 TimeZoneBiasEffectiveEnd;
	XSTATE_CONFIGURATION          XState;
	KSYSTEM_TIME                  FeatureConfigurationChangeStamp;
	ULONG                         Spare;
	ULONG64                       UserPointerAuthMask;
	XSTATE_CONFIGURATION          XStateArm64;
	ULONG                         Reserved10[210];
} KUSER_SHARED_DATA, * PKUSER_SHARED_DATA;
#endif // HACK_VERSION_RETRIEVAL
#endif // GET_VERSION_FROM_CMD_VER

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		void SkyrimSpecialEdition_AppendPatches(ModuleManager* PatchesManager);
		void Fallout4_AppendPatches(ModuleManager* PatchesManager);
		void Starfield_AppendPatches(ModuleManager* PatchesManager);

		Engine* GlobalEnginePtr = nullptr;

		CHAR TempNTSIT[16];
		ULONG_PTR TempNTSITAddress;
		std::atomic_uint32_t g_DumpTargetThreadId;
		LONG(NTAPI* NtSetInformationThread)(HANDLE ThreadHandle, LONG ThreadInformationClass,
			PVOID ThreadInformation, ULONG ThreadInformationLength);
		using VCoreNotifyEvent = void (Engine::*)();
		VCoreNotifyEvent VCoreDisableBreakpoint;
		VCoreNotifyEvent VCoreContinueInitialize;
		CreationKitPlatformExtended::Patches::INICacheDataPatch* INICacheData = nullptr;

		//////////////////////////////////////////////

		static BOOL WINAPI GetVerOs(LPDWORD lpdwMajorVersion, LPDWORD lpdwMinorVersion, LPDWORD lpdwBuildNubmer)
		{
			if (!lpdwMajorVersion || !lpdwMinorVersion || !lpdwBuildNubmer)
				return FALSE;

#if GET_VERSION_FROM_CMD_VER == 0
#if HACK_VERSION_RETRIEVAL == 0
			LONG(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW) = nullptr;
			OSVERSIONINFOEXW osInfo = { 0 };
			*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");
			if (RtlGetVersion)
			{
				osInfo.dwOSVersionInfoSize = sizeof(osInfo);
				RtlGetVersion(&osInfo);

				*lpdwMajorVersion = osInfo.dwMajorVersion;
				*lpdwMinorVersion = osInfo.dwMinorVersion;
				*lpdwBuildNubmer  = osInfo.dwBuildNumber;

				return TRUE;
			}
#else
			auto data = (KUSER_SHARED_DATA*)0x7ffe0000;
			*lpdwMajorVersion = data->NtMajorVersion;
			*lpdwMinorVersion = data->NtMinorVersion;
			*lpdwBuildNubmer = data->NtBuildNumber;
#endif // HACK_VERSION_RETRIEVAL
#else
			LONG(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW) = nullptr;
			OSVERSIONINFOEXW osInfo = { 0 };
			*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");
			if (RtlGetVersion)
			{
				osInfo.dwOSVersionInfoSize = sizeof(osInfo);
				RtlGetVersion(&osInfo);

				*lpdwMajorVersion = osInfo.dwMajorVersion;
				*lpdwMinorVersion = osInfo.dwMinorVersion;
				*lpdwBuildNubmer = osInfo.dwBuildNumber;

				if (osInfo.dwMajorVersion == 6)
					// There is a problem with stripped-down versions of OS, the function returns that it is still Win 8.
					goto ver_win_home_maybe;

				return TRUE;
			}

		ver_win_home_maybe:
			
			std::string str_ver;

			char cmd_exe[256];
			GetEnvironmentVariable("COMSPEC", cmd_exe, 256);

			size_t				it, it_l;
			DWORD               read;
			SECURITY_ATTRIBUTES sa;
			STARTUPINFO         si;
			PROCESS_INFORMATION pi;
			HANDLE              hReadOut = NULL;   // pipe handles for redirecting STDOUT
			HANDLE              hWriteOut = NULL;
			char                buf[255];

			std::string scmd = cmd_exe;
			scmd += " /c ver";

			// Prepare security attributes for pipes.
			// Note - inheritance flag is ON!
			memset(&sa, 0, sizeof(sa));
			sa.nLength = sizeof(sa);
			sa.bInheritHandle = TRUE;
			sa.lpSecurityDescriptor = NULL;

			// Create pipe for output redirection
			if (!CreatePipe(&hReadOut, &hWriteOut, &sa, 0))
				goto invalid_error;

			// Prepare child process startup data
			// Force child process to use hWriteOut as stdout
			memset(&si, 0, sizeof(si));
			si.cb = sizeof(si);
			si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
			si.wShowWindow = SW_HIDE;
			si.hStdOutput = hWriteOut;
			si.hStdError = hWriteOut;

			// Spawn child process.
			if (!CreateProcessA(NULL, scmd.data(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
			{
				CloseHandle(hReadOut);
				CloseHandle(hWriteOut);

				goto invalid_error;
			}

			// Wait until child process exits.
			WaitForSingleObject(pi.hProcess, INFINITE);
			// Close pipe ends, which i're not going to use
			CloseHandle(hWriteOut);  // child process will write here

			// Read process output.
			while (TRUE)
			{
				if (!ReadFile(hReadOut, buf, sizeof(buf) - 1, &read, NULL)) 
					// It might be "normal" error. Spawned app just closed the
					// stdout, and in this case ReadFile returned FALSE.
					break;

				// Test for the "normal" end of the output
				if (0 == read)
					// Nothing to read, child processs has closed the output
					break;   

				// Write all data from redirected stdout to our own stdout.
				buf[read] = 0;
				str_ver += buf;
			}

			//  Close all remaining handles
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			CloseHandle(hReadOut);

			it = str_ver.find_first_of('[');
			if (it == std::string::npos)
				goto invalid_error;

			it_l = str_ver.find_first_of(' ', it + 1);
			if (it_l == std::string::npos)
				goto invalid_error;

			if (sscanf(str_ver.c_str() + it_l + 1, "%u.%u.%u", lpdwMajorVersion, lpdwMinorVersion, lpdwBuildNubmer) != 3)
				goto invalid_error;

			return TRUE;

		invalid_error:
#endif // GET_VERSION_FROM_CMD_VER

			return FALSE;
		}

		static BOOL WINAPI hk_QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount)
		{
			// Выключение точки останова
			(GlobalEnginePtr->*VCoreDisableBreakpoint)();

			// Остановка для отладчика
			__try
			{
				__debugbreak();
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{}

			(GlobalEnginePtr->*VCoreContinueInitialize)();
			return QueryPerformanceCounter(lpPerformanceCount);
		}

		static LONG NTAPI hk_NtSetInformationThread(HANDLE ThreadHandle, LONG ThreadInformationClass,
			PVOID ThreadInformation, ULONG ThreadInformationLength)
		{
			// Для Steam
			if (ThreadInformationClass == 0x11)
				return 0;

			return NtSetInformationThread(ThreadHandle, ThreadInformationClass,
				ThreadInformation, ThreadInformationLength);
		}

		//////////////////////////////////////////////

		Engine::Engine(HMODULE hModule, EDITOR_EXECUTABLE_TYPE eEditorVersion, uintptr_t nModuleBase) :
			_module(hModule), _moduleBase(nModuleBase), _editorVersion(eEditorVersion), 
			PatchesManager(new ModuleManager()), UserPluginsManager(new PluginManager())
		{
			GlobalEnginePtr = this;

			auto OsVer = &_OsVersion;
			ZeroMemory(OsVer, sizeof(OsVersion));
			GetVerOs((LPDWORD)&OsVer->MajorVersion, (LPDWORD)&OsVer->MinorVersion, (LPDWORD)&OsVer->BuildNubmer);
			auto str = EditorAPI::BSString::FormatString("CKPE Runtime: Initialize (Version: %s, OS: %u.%u Build %u)",
				VER_FILE_VERSION_STR, OsVer->MajorVersion, OsVer->MinorVersion, OsVer->BuildNubmer);
			_MESSAGE(str.c_str());

			if ((OsVer->MajorVersion == 6) && (OsVer->MinorVersion < 3))
				_CONSOLE("[WARNING] Your OS is not fully supported");

			int info[4];
			__cpuid(info, 7);
			_hasAVX2 = (info[1] & (1 << 5)) != 0;

			__cpuid(info, 1);
			_hasSSE41 = (info[2] & (1 << 19)) != 0;

			// Detect hyper-threads and cores
			_hyperThreads = info[3] & (1 << 28);
			_threads = (info[1] >> 16) & 0xff;
			if (_hyperThreads) _threads;
			_logicalCores = _threads;
			_physicalCores = _logicalCores >> 1;

			_MESSAGE("The processor has a number of threads: %u", _threads);
			_MESSAGE("The processor has a number of logical cores: %u", _logicalCores);
			_MESSAGE("The processor has a number of physical cores: %u", _physicalCores);
			_MESSAGE("The processor implements hyper-threads technology: %s", (_hyperThreads ? "true" : "false"));
			_MESSAGE("The processor supports the SSE 4.1 instruction set: %s", (_hasSSE41 ? "true" : "false"));
			_MESSAGE("The processor supports the AVX 2 instruction set: %s", (_hasAVX2 ? "true" : "false"));

			// Передача приватных функций класса, для продолжения инициализации
			VCoreDisableBreakpoint = &Engine::DisableBreakpoint;
			VCoreContinueInitialize = &Engine::ContinueInitialize;

			GlobalCrashHandlerPtr = new CrashHandler();
			GlobalRelocationDatabasePtr = new RelocationDatabase(this);
			GlobalRelocatorPtr = new Relocator(this);
			GlobalDialogManagerPtr = new DialogManager();

			// Нужно управлять над окнами, так как, похоже сам CK это делает плохо.
			// В первую очередь нужно отделить окна: главные и обычные, поможет при закрытии программы.
			// К слову, также можно ими будет манипулировать, скрыть все окна и т.д.
			GlobalRegistratorWindowPtr = new RegistratorWindow();
			
			// Данный патч, является исключением от остальных, он инициализируется раньше, как
			// системный модуль, но его можно отключать как патч. Это нужно для покрытия консоли новой темой.

			// Инициализация кеша для .ini файлов
			INICacheData = new CreationKitPlatformExtended::Patches::INICacheDataPatch();
			// Инициализация темы, если это необходимо, раньше, чем окно консоли
			_Theme = new CreationKitPlatformExtended::Patches::UIThemePatch();
			_ClassicTheme = new CreationKitPlatformExtended::Patches::UIThemeClassicPatch();
			// Отправка не нулевого адреса необходима, это специально, чтобы пропустить проверки
			_ClassicTheme->Enable(GlobalRelocatorPtr, (RelocationDatabaseItem*)1);
			if (!_ClassicTheme->HasActive())
				_Theme->Enable(GlobalRelocatorPtr, (RelocationDatabaseItem*)1);	

			GlobalConsoleWindowPtr = new ConsoleWindow(this);
			GlobalGDIPlusInitPtr = new GDIPlusInit(this);

			// Добавление патчей
			PatchesManager->Append({
				new CreationKitPlatformExtended::Patches::QuitHandlerPatch(),
				new CreationKitPlatformExtended::Patches::MemoryManagerPatch(),
				new CreationKitPlatformExtended::Patches::CrashDumpPatch(),
				new CreationKitPlatformExtended::Patches::ConsolePatch(),
				new CreationKitPlatformExtended::Patches::FlowChartXPatch(),
				new CreationKitPlatformExtended::Patches::UnicodePatch(),
				new CreationKitPlatformExtended::Patches::ThreadPatch(),
				new CreationKitPlatformExtended::Patches::VersionControlMergeWorkaroundPatch(),
				new CreationKitPlatformExtended::Patches::RenderWindow60FPSPatch(),
				new CreationKitPlatformExtended::Patches::DisableAssertionPatch(),
				new CreationKitPlatformExtended::Patches::SkipTopicInfoValidationPatch(),
				new CreationKitPlatformExtended::Patches::AllowMultipleWindowAndMasterPatch(),
				new CreationKitPlatformExtended::Patches::RenameCreationKitAppPatch(),
				new CreationKitPlatformExtended::Patches::FaceGenPatch(),
				new CreationKitPlatformExtended::Patches::UIPatch(),
				new CreationKitPlatformExtended::Patches::UIHotkeysPatch(),
				new CreationKitPlatformExtended::Patches::UIThemePatchAdditional(),
				new CreationKitPlatformExtended::Patches::PapyrusEditorLimitPatch(),
				new CreationKitPlatformExtended::Patches::D3D11Patch(),
				_Theme,
				_ClassicTheme,
				INICacheData,
			});

			// Добавление патчей только для редактора скайрима специального издания
			if (eEditorVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST)
				SkyrimSpecialEdition_AppendPatches(PatchesManager);
			else
			// Добавление патчей только для редактора фолыча
			if (eEditorVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST)
			{
				// Распаковка ресурсов
				ResourcesPackerManager::UnpackResources();

				Fallout4_AppendPatches(PatchesManager);
			}
#ifdef _CKPE_WITH_QT5
			else
			// Добавление патчей только для редактора старфилда
			if (eEditorVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_LAST)
				Starfield_AppendPatches(PatchesManager);
#endif // !_CKPE_WITH_QT5

			// Установка точки останова, чтобы дождаться расшифровки DRM приложения
			EnableBreakpoint();
		}

		void Engine::EnableBreakpoint()
		{
			_MESSAGE("Module base: %p", _moduleBase);

			PIMAGE_NT_HEADERS ntHeaders = (PIMAGE_NT_HEADERS)(_moduleBase +
				((PIMAGE_DOS_HEADER)_moduleBase)->e_lfanew);

			// Получить раздел конфигурации загрузки, в котором содержится адрес security cookie
			auto dataDirectory = ntHeaders->OptionalHeader.DataDirectory;
			auto sectionRVA = dataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress;
			auto sectionSize = dataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size;
			auto loadConfig = (PIMAGE_LOAD_CONFIG_DIRECTORY)(_moduleBase + sectionRVA);

			Assert(sectionRVA > 0 && sectionSize > 0);
			AssertMsg(loadConfig->SecurityCookie, "SecurityCookie is a null pointer!");

			// Определить адреса и размеры модулей/разделов кода

			_moduleSize = ntHeaders->OptionalHeader.SizeOfImage;
			Assert(voltek::get_pe_section_range(_moduleBase, ".text", &Sections[SECTION_TEXT].base, &Sections[SECTION_TEXT].end));
			Assert(voltek::get_pe_section_range(_moduleBase, ".rdata", &Sections[SECTION_DATA_READONLY].base, &Sections[SECTION_DATA_READONLY].end));
			Assert(voltek::get_pe_section_range(_moduleBase, ".data", &Sections[SECTION_DATA].base, &Sections[SECTION_DATA].end));

			uintptr_t tempStart, tempEnd;
			if (voltek::get_pe_section_range(_moduleBase, ".textbss", &tempStart, &tempEnd))
			{
				Sections[SECTION_TEXT].base = std::min(Sections[SECTION_TEXT].base, tempStart);
				Sections[SECTION_TEXT].end = std::max(Sections[SECTION_TEXT].end, tempEnd);
			}

			if (voltek::get_pe_section_range(_moduleBase, ".interpr", &tempStart, &tempEnd))
			{
				Sections[SECTION_TEXT].base = std::min(Sections[SECTION_TEXT].base, tempStart);
				Sections[SECTION_TEXT].end = std::max(Sections[SECTION_TEXT].end, tempEnd);
			}

			_MESSAGE("Section range \".text\": (base: %p, end: %p)", Sections[SECTION_TEXT].base, Sections[SECTION_TEXT].end);
			_MESSAGE("Section range \".rdata\": (base: %p, end: %p)", Sections[SECTION_DATA_READONLY].base, Sections[SECTION_DATA_READONLY].end);
			_MESSAGE("Section range \".data\": (base: %p, end: %p)", Sections[SECTION_DATA].base, Sections[SECTION_DATA].end);

			// Установить магическое значение, которое запускает ранний вызов QueryPerformanceCounter
			*(uint64_t*)loadConfig->SecurityCookie = 0x2B992DDFA232;
			PatchIAT(hk_QueryPerformanceCounter, "kernel32.dll", "QueryPerformanceCounter");

			// Отключить вызов распаковщика steam для NtSetInformationThread(ThreadHideFromDebugger)
			TempNTSITAddress = (uintptr_t)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtSetInformationThread");
			if (TempNTSITAddress)
			{
				memcpy(&TempNTSIT, (LPVOID)TempNTSITAddress, sizeof(TempNTSIT));
				*(uintptr_t*)&NtSetInformationThread = voltek::detours_function_class_jump(TempNTSITAddress, &hk_NtSetInformationThread);
			}
		}

		void Engine::DisableBreakpoint()
		{
			// Восстановить оригинал указатель на QPC
			PatchIAT(QueryPerformanceCounter, "kernel32.dll", "QueryPerformanceCounter");

			if (TempNTSITAddress)
				// Восстановить исходный код NtSetInformationThread
				voltek::detours_patch_memory(TempNTSITAddress, (PBYTE)&TempNTSIT, sizeof(TempNTSIT));
		}

		void Engine::CommandLineRun()
		{
			CommandLineParser CommandLine;

			_hasCommandRun = CommandLine.HasCommandRun();
			if (_hasCommandRun)
			{
				_MESSAGE("\tAccessing the console...");

				// Определяем команду
				auto Command = CommandLine.GetCommand();
				_MESSAGE("\tCommand: \"%s\"", Command.c_str());

				if (!_stricmp(Command.c_str(), "-PECreateDatabase"))
				{
					// Создаём базу
					GlobalRelocationDatabasePtr->CreateDatabase();
					// Закрываем Creation Kit
					CreationKitPlatformExtended::Utils::Quit();
				}
				else if (!_stricmp(Command.c_str(), "-PEUpdateDatabase"))
				{
					if (CommandLine.Count() != 2)
					{
						_ERROR("Invalid number of command arguments: %u", CommandLine.Count());
						_MESSAGE("Example: CreationKit -PEUpdateDatabase \"test.relb\"");
					}
					else if (!CreationKitPlatformExtended::Utils::FileExists(CommandLine[1].c_str()))
					{
						_ERROR("The file does not exist: \"%s\"", CommandLine[1].c_str());
					}
					else
					{
						// Открываем базу данных
						if (GlobalRelocationDatabasePtr->OpenDatabase())
						{
							auto Patch = GlobalRelocationDatabasePtr->Append(CommandLine[1].c_str());
							// Патча нет?
							if (Patch.Empty())
								// Закрываем Creation Kit
								CreationKitPlatformExtended::Utils::Quit();
							// Сохраняем базу данных
							GlobalRelocationDatabasePtr->SaveDatabase();
						}
						else
							_FATALERROR("The database is not loaded");
					}
					// Закрываем Creation Kit
					CreationKitPlatformExtended::Utils::Quit();
				}
				else if (!_stricmp(Command.c_str(), "-PERemoveFromDatabase"))
				{
					if (CommandLine.Count() != 2)
					{
						_ERROR("Invalid number of command arguments: %u", CommandLine.Count());
						_MESSAGE("Example: CreationKit -PERemoveFromDatabase \"test\"");
					}
					else
					{
						// Открываем базу данных
						if (GlobalRelocationDatabasePtr->OpenDatabase())
						{
							if (GlobalRelocationDatabasePtr->Remove(CommandLine[1].c_str()))
								// Сохраняем базу данных
								GlobalRelocationDatabasePtr->SaveDatabase();
							// Закрываем Creation Kit
							CreationKitPlatformExtended::Utils::Quit();
						}
						else
							_FATALERROR("The database is not loaded");
					}
					// Закрываем Creation Kit
					CreationKitPlatformExtended::Utils::Quit();
				}
				else if (!_stricmp(Command.c_str(), "-PEExtractFromDatabase"))
				{
					if (CommandLine.Count() != 3)
					{
						_ERROR("Invalid number of command arguments: %u", CommandLine.Count());
						_MESSAGE("Example: CreationKit -PEExtractFromDatabase \"test\" \"test.relb\"");
					}
					else
					{
						// Открываем базу данных
						if (GlobalRelocationDatabasePtr->OpenDatabase())
						{
							auto Patch = GlobalRelocationDatabasePtr->GetByName(CommandLine[1].c_str());
							// Патча есть?
							if (!Patch.Empty())
								Patch->SaveToFileDeveloped(CommandLine[2].c_str());
						}
						else
							_FATALERROR("The database is not loaded");
					}
					// Закрываем Creation Kit
					CreationKitPlatformExtended::Utils::Quit();
				}
				else if (!_stricmp(Command.c_str(), "-PEExportRTTI"))
				{
					if (CommandLine.Count() != 2)
					{
						_ERROR("Invalid number of command arguments: %u", CommandLine.Count());
						_MESSAGE("Example: CreationKit -PEExportRTTI \"rtti.txt\"");
					}

					if (!GlobalDynamicCastPtr || !GlobalDynamicCastPtr->Dump(CommandLine[1].c_str()))
						_ERROR("It was not possible to create a file and write data there.");

					// Закрываем Creation Kit
					CreationKitPlatformExtended::Utils::Quit();
				}
				else if (!_stricmp(Command.c_str(), "-PEPackedDialogs"))
				{
					if (CommandLine.Count() != 3)
					{
						_ERROR("Invalid number of command arguments: %u", CommandLine.Count());
						_MESSAGE("Example: CreationKit -PEPackedDialogs \"dia.pak\" \"dia\\sse\\\"");
					}

					GlobalDialogManagerPtr->PackToFilePackage(CommandLine[1].c_str(), CommandLine[2].c_str());
					// Закрываем Creation Kit
					CreationKitPlatformExtended::Utils::Quit();
				}
			}
		}

		void Engine::ContinueInitialize()
		{
			// Включение RTTI
			GlobalDynamicCastPtr = new DynamicCast();
			if (!GlobalDynamicCastPtr)
			{
				_FATALERROR("Failed to create a class for RTTI");
				return;
			}

			// Парсинг командной строки
			CommandLineRun();
			
			if (!GlobalRelocationDatabasePtr->OpenDatabase())
			{
				_FATALERROR("The database is not loaded, patches are not installed");
				return;
			}
			
			if (!PatchesManager)
			{
				_FATALERROR("The patch manager has not been initialized");
				return;
			}

			if (!UserPluginsManager)
			{
				_FATALERROR("The plugins manager has not been initialized");
				return;
			}

			auto editorShortVersion = GetShortExecutableTypeFromFull(_editorVersion);
			if (editorShortVersion == EDITOR_SHORT_UNKNOWN)
			{
				_FATALERROR("The version of the game for the editor is not defined");
				return;
			}

			auto ShortNameGameIterator = allowedShortNameGame.find(editorShortVersion);
			if (ShortNameGameIterator == allowedShortNameGame.end())
			{
				_FATALERROR("INTERNAL: There is no short name for the game");
				return;
			}

			auto DialogsFileName = EditorAPI::BSString::FormatString("CreationKitPlatformExtended_%s_Dialogs.pak",
				ShortNameGameIterator->second.data());
			
			if (!EditorAPI::BSString::Utils::FileExists(DialogsFileName))
			{
				_FATALERROR("The dialog file is not specified for this version of the editor or no found");
				return;
			}

			GlobalDialogManagerPtr->LoadFromFilePackage(DialogsFileName.c_str());

#ifdef _CKPE_WITH_QT5
			auto QExternalResourceIterator = qtExternalResourcePackageFile.find(editorShortVersion);
			if (QExternalResourceIterator != qtExternalResourcePackageFile.end())
			{
				if (!Utils::FileExists(QExternalResourceIterator->second.data()))
				{
					_FATALERROR("This version uses Qt and requires resources for this, the file \"%s\" was not found",
						QExternalResourceIterator->second.data());
					return;
				}

				if (!QResource::registerResource(QExternalResourceIterator->second.data()))
				{
					_FATALERROR("QRESOURCE: Failed to load external resource file \"%s\"", QExternalResourceIterator->second.data());
					return;
				}
				else
					_MESSAGE("QRESOURCE: Open external resource file \"%s\"", QExternalResourceIterator->second.data());
			}
#endif // !_CKPE_WITH_QT5

			// Создание класса отвечающий за UI
			EditorAPI::GlobalEditorUIPtr = new EditorAPI::EditorUI();
			if (!EditorAPI::GlobalEditorUIPtr)
			{
				_FATALERROR("Failed to create a UI control class");
				return;
			}

			if (CheckOutdatedEditor(_editorVersion))
			{
				_WARNING("This version has been declared outdated, is no longer supported, and there may be errors.");
				_CONSOLE("[WARNING] This version has been declared outdated, is no longer supported, and there may be errors.");
			}

			// Запросы и проверка всех патчей на валидность
			PatchesManager->QueryAll();
			// Включение неотбракованных патчей
			PatchesManager->EnableAll();
			// Поиск плагинов в корневой папке
			UserPluginsManager->FindPlugins();
			// Запросы и проверка всех плагинов на валидность
			UserPluginsManager->QueryAll();
			// Включение неотбракованных плагинов
			UserPluginsManager->EnableAll();

			// После всех патчей и прочее, пересоберём бинарник в памяти.
			// Убрать трамполины, установить DeferUI от Nukem9
			Experimental::RunOptimizations();
		}

		uintptr_t Engine::GetModuleBase() const
		{ 
			return _moduleBase; 
		}

		uint64_t Engine::GetModuleSize() const
		{ 
			return _moduleSize; 
		}

		EDITOR_EXECUTABLE_TYPE Engine::GetEditorVersion() const 
		{ 
			return _editorVersion; 
		}

		bool Engine::HasAVX2() const 
		{ 
			return _hasAVX2; 
		}

		bool Engine::HasSSE41() const 
		{
			return _hasSSE41; 
		}

		bool Engine::HasCommandRun() const
		{
			return _hasCommandRun;
		}

		Section Engine::GetSection(uint32_t nIndex) const 
		{ 
			return Sections[nIndex]; 
		}

		OsVersion Engine::GetSystemVersion() const
		{
			return _OsVersion;
		}

		bool Engine::HasPatch(const char* lpstrName) const
		{
			return PatchesManager->Has(lpstrName);
		}

		bool Engine::HasPlugin(const char* lpstrName) const
		{
			return UserPluginsManager->Has(lpstrName);
		}

		bool Engine::HasHyperThreads() const noexcept
		{
			return _hyperThreads;
		}

		unsigned char Engine::GetTotalThreadsProcessor() const noexcept
		{
			return _threads;
		}

		unsigned char Engine::GetTotalLogicalCores() const noexcept
		{
			return _logicalCores;
		}

		unsigned char Engine::GetTotalPhysicalCores() const noexcept
		{
			return _physicalCores;
		}

		IResult Engine::Initialize(HMODULE hModule, LPCSTR lpcstrAppName)
		{
			if (GlobalEnginePtr)
				return RC_INITIALIZATION_ENGINE_AGAIN;

			IResult Result = RC_OK;

			DWORD dwMajor, dwMinor, dwBuild;
			if (!GetVerOs(&dwMajor, &dwMinor, &dwBuild))
			{
				// Need 8.1 or newer
				if ((dwMajor < 6) || ((dwMajor == 6) && (dwMinor < 2)))
				{
					_ERROR("Unsupported OS version");

					return RC_UNSUPPORT_VERSION_OS;
				}
			}

			// Доступные имена для Creation Kit
			if (CheckFileNameProcess(lpcstrAppName))
			{
				// Инициализация менеджера памяти
				GlobalMemoryManagerPtr = new MemoryManager();
				AssertMsg(GlobalMemoryManagerPtr, "Failed to initialize class \"MemoryManager\".");
#if CKPE_USES_TRACER
				// Инициализация менеджера трассеровки памяти, для поиска утечек памяти
				GlobalTracerManagerPtr = new TracerManager();
				AssertMsg(GlobalTracerManagerPtr, "Failed to initialize class \"TracerManager\".");
#endif
				GlobalDebugLogPtr = new DebugLog(L"CreationKitPlatformExtended.log");
				AssertMsg(GlobalDebugLogPtr, "Failed create the log file \"CreationKitPlatformExtended.log\"");
				
				GlobalINIConfigPtr = new INIConfig(L"CreationKitPlatformExtended.ini");
				AssertMsg(GlobalINIConfigPtr, "Failed open the config file \"CreationKitPlatformExtended.ini\"");

				auto LogCurrentTime = []() {
					char timeBuffer[80];
					struct tm* timeInfo;
					time_t rawtime;
					time(&rawtime);
					timeInfo = localtime(&rawtime);
					strftime(timeBuffer, sizeof(timeBuffer), "%A %d %b %Y %r %Z", timeInfo);

					_MESSAGE("Current time: %s", timeBuffer);
				};

				LogCurrentTime();

				// Получение CRC32 с файла
				uint32_t hash_crc32 = ::Utils::CRC32File((String(lpcstrAppName) + ".exe").c_str());
				_MESSAGE("CRC32 executable file: 0x%08X", hash_crc32);

				// Получение начального адреса памяти главного процесса
				uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);

				bool Outdated = false;
				// Пытаемся определить файл
				EDITOR_EXECUTABLE_TYPE editorVersion = EDITOR_UNKNOWN;
				auto editorVersionIterator = allowedEditorVersion.find(hash_crc32);
				if (editorVersionIterator == allowedEditorVersion.end())
				{
					// Файл не найден среди разрешённых, попытка определить по ключевым местам
					_WARNING("CRC32 does not match any of the known ones, running a version check by signature");
					// Определение версии по смещению в файле нужной сигнатуры
					editorVersion = GetEditorVersionByOffsetSign(moduleBase);
				}
				else
					editorVersion = editorVersionIterator->second;

#ifndef _CKPE_WITH_QT5
				// don't let it run without Qt5
				if (GetShortExecutableTypeFromFull(editorVersion) == EDITOR_SHORT_STARFIELD)
					editorVersion = EDITOR_UNKNOWN;
#endif // !_CKPE_WITH_QT5

				// Проверка неустаревшая ли версия
				Outdated = CheckOutdatedEditor(editorVersion);

				if ((editorVersion != EDITOR_UNKNOWN) && !Outdated)
				{
					_MESSAGE("Current CK version: %s", allowedEditorVersionStr[(int)editorVersion].data());

					new Engine(hModule, editorVersion, moduleBase);
				}
				else
				{
					if (Outdated)
						Result = RC_DEPRECATED_VERSION_CREATIONKIT;
					else
					{
						_ERROR("The CK version has not been determined");

						Result = RC_UNSUPPORTED_VERSION_CREATIONKIT;
					}
				}
			}
			else
				Result = RC_UNKNOWN_APPLICATION;

			return Result;
		}
	}
}