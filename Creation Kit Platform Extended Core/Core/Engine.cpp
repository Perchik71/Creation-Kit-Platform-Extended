﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Version/resource_version2.h"
#include "TypeInfo/ms_rtti.h"

#include "ConsoleWindow.h"
#include "CommandLineParser.h"
#include "RelocationDatabase.h" 
#include "DialogManager.h"
#include "GDIPlusInit.h"
#include "Engine.h"

#include "Editor API/EditorUI.h"

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
#include "Patches/UIThemePatchAdditional.h"
#include "Patches/PapyrusEditorLimit.h"
#include "Patches/UIHotkeys.h"

#include "Experimental/RuntimeOptimization.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		void SkyrimSpecialEdition_AppendPatches(ModuleManager* PatchesManager);
		void Fallout4_AppendPatches(ModuleManager* PatchesManager);

		Engine* GlobalEnginePtr = nullptr;

		CHAR TempNTSIT[16];
		ULONG_PTR TempNTSITAddress;
		std::atomic_uint32_t g_DumpTargetThreadId;
		LONG(NTAPI* NtSetInformationThread)(HANDLE ThreadHandle, LONG ThreadInformationClass,
			PVOID ThreadInformation, ULONG ThreadInformationLength);
		using VCoreNotifyEvent = void (Engine::*)();
		VCoreNotifyEvent VCoreDisableBreakpoint;
		VCoreNotifyEvent VCoreContinueInitialize;

		//////////////////////////////////////////////

		BOOL WINAPI hk_QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount)
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

		LONG NTAPI hk_NtSetInformationThread(HANDLE ThreadHandle, LONG ThreadInformationClass,
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

			int info[4];
			__cpuid(info, 7);
			_hasAVX2 = (info[1] & (1 << 5)) != 0;

			__cpuid(info, 1);
			_hasSSE41 = (info[2] & (1 << 19)) != 0;

			_MESSAGE("The processor supports the SSE 4.1 instruction set: %s", (_hasSSE41 ? "true" : "false"));
			_MESSAGE("The processor supports the AVX 2 instruction set: %s", (_hasAVX2 ? "true" : "false"));

			// Передача приватных функций класса, для продолжения инициализации
			VCoreDisableBreakpoint = &Engine::DisableBreakpoint;
			VCoreContinueInitialize = &Engine::ContinueInitialize;

			GlobalRelocationDatabasePtr = new RelocationDatabase(this);
			GlobalRelocatorPtr = new Relocator(this);
			GlobalDialogManagerPtr = new DialogManager();
			
			// Данный патч, является исключением от остальных, он инициализируется раньше, как
			// системный модуль, но его можно отключать как патч. Это нужно для покрытия консоли новой темой.

			// Инициализация темы, если это необходимо, раньше, чем окно консоли
			_Theme = new CreationKitPlatformExtended::Patches::UIThemePatch();
			// Отправка не нулевого адреса необходима, это специально, чтобы пропустить проверки
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
				_Theme
			});

			// Добавление патчей только для редактора скайрима специального издания
			if (eEditorVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST)
			{
				SkyrimSpecialEdition_AppendPatches(PatchesManager);
			}
			else
			// Добавление патчей только для редактора фолыча специального издания
			if (eEditorVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST)
			{
				Fallout4_AppendPatches(PatchesManager);
			}

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
			Assert(Utils::GetPESectionRange(_moduleBase, ".text", &Sections[SECTION_TEXT].base, &Sections[SECTION_TEXT].end));
			Assert(Utils::GetPESectionRange(_moduleBase, ".rdata", &Sections[SECTION_DATA_READONLY].base, &Sections[SECTION_DATA_READONLY].end));
			Assert(Utils::GetPESectionRange(_moduleBase, ".data", &Sections[SECTION_DATA].base, &Sections[SECTION_DATA].end));

			uintptr_t tempBssStart, tempBssEnd;
			if (Utils::GetPESectionRange(_moduleBase, ".textbss", &tempBssStart, &tempBssEnd))
			{
				Sections[0].base = std::min(Sections[SECTION_TEXT].base, tempBssStart);
				Sections[0].end = std::max(Sections[SECTION_TEXT].end, tempBssEnd);
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
				*(uintptr_t*)&NtSetInformationThread = Detours::X64::DetourFunctionClass(TempNTSITAddress, &hk_NtSetInformationThread);
			}
		}

		void Engine::DisableBreakpoint()
		{
			// Восстановить оригинал указатель на QPC
			PatchIAT(QueryPerformanceCounter, "kernel32.dll", "QueryPerformanceCounter");

			if (TempNTSITAddress)
				// Восстановить исходный код NtSetInformationThread
				Utils::PatchMemory(TempNTSITAddress, (PBYTE)&TempNTSIT, sizeof(TempNTSIT));
		}

		void Engine::CommandLineRun()
		{
			CommandLineParser CommandLine;

			if (CommandLine.Count() > 0)
			{
				_MESSAGE("\tAccessing the console...");

				// Определяем команду
				auto Command = CommandLine.At(0);
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
					if (CommandLine.Count() != 3)
					{
						_ERROR("Invalid number of command arguments: %u", CommandLine.Count());
						_MESSAGE("Example: CreationKit -PEUpdateDatabase \"test\" \"test.relb\"");
					}
					else if (!CreationKitPlatformExtended::Utils::FileExists(CommandLine.At(2).c_str()))
					{
						_ERROR("The file does not exist: \"%s\"", CommandLine.At(2).c_str());
					}
					else
					{
						// Открываем базу данных
						if (GlobalRelocationDatabasePtr->OpenDatabase())
						{
							auto Patch = GlobalRelocationDatabasePtr->GetByName(CommandLine.At(1).c_str());
							// Патча нет?
							if (Patch.Empty())
							{
								Patch = GlobalRelocationDatabasePtr->Append(CommandLine.At(1).c_str(), new RelocationDatabaseItem());
								if (Patch.Empty())
									// Закрываем Creation Kit
									CreationKitPlatformExtended::Utils::Quit();
							}
							// Загружаем
							if (Patch->LoadFromFileDeveloped(CommandLine.At(2).c_str()))
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
							if (GlobalRelocationDatabasePtr->Remove(CommandLine.At(1).c_str()))
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
							auto Patch = GlobalRelocationDatabasePtr->GetByName(CommandLine.At(1).c_str());
							// Патча есть?
							if (!Patch.Empty())
								Patch->SaveToFileDeveloped(CommandLine.At(2).c_str());
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

					FILE* fileStream = _wfsopen(Utils::Utf82Wide((char8_t*)CommandLine.At(1).c_str()).c_str(), 
						L"wt", _SH_DENYRW);
					if (!fileStream)
						_ERROR("It was not possible to create a file and write data there.");
					else
					{
						Utils::ScopeFileStream file(fileStream);
						MSRTTI::Dump(fileStream);
					}

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

					GlobalDialogManagerPtr->PackToFilePackage(CommandLine.At(1).c_str(), CommandLine.At(2).c_str());
					// Закрываем Creation Kit
					CreationKitPlatformExtended::Utils::Quit();
				}
			}
		}

		void Engine::ContinueInitialize()
		{
			// Включение RTTI
			MSRTTI::Initialize();
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

			auto DialogIterator = allowedDialogsPackageFile.find(_editorVersion);
			if (DialogIterator == allowedDialogsPackageFile.end())
			{
				_FATALERROR("The dialog file is not specified for this version of the editor");
				return;
			}

			GlobalDialogManagerPtr->LoadFromFilePackage(DialogIterator->second.data());

			// Создание класса отвечающий за UI
			EditorAPI::GlobalEditorUIPtr = new EditorAPI::EditorUI();
			if (!EditorAPI::GlobalEditorUIPtr)
			{
				_FATALERROR("Failed to create a UI control class");
				return;
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

		Section Engine::GetSection(uint32_t nIndex) const 
		{ 
			return Sections[nIndex]; 
		}

		IResult Engine::Initialize(HMODULE hModule, LPCSTR lpcstrAppName)
		{
			if (GlobalEnginePtr)
				return RC_INITIALIZATION_ENGINE_AGAIN;

			IResult Result = RC_OK;

			// Доступные имена для Creation Kit
			if (CheckFileNameProcess(lpcstrAppName))
			{
				// Инициализация менеджера памяти
				GlobalMemoryManagerPtr = new MemoryManager();
				// Инициализация библиотеки vup
				AssertMsg(Conversion::LazUnicodePluginInit(), 
					"I can't find the library: \"vup-x86_64.dll\"\nReinstall the mod.");
					
				GlobalDebugLogPtr = new DebugLog(L"CreationKitPlatformExtended.log");
				AssertMsg(GlobalDebugLogPtr, "Failed create the log file \"CreationKitPlatformExtended.log\"");
				
				GlobalINIConfigPtr = new INIConfig(L"CreationKitPlatformExtended.ini");
				AssertMsg(GlobalINIConfigPtr, "Failed open the config file \"CreationKitPlatformExtended.ini\"");
				
				ULONG _osMajorVersion = 0;
				ULONG _osMinorVersion = 0;
				ULONG _osBuildNubmer = 0;

				LONG(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW) = nullptr;
				OSVERSIONINFOEXW osInfo = { 0 };
				*(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");
				if (RtlGetVersion)
				{
					osInfo.dwOSVersionInfoSize = sizeof(osInfo);
					RtlGetVersion(&osInfo);

					_osMajorVersion = osInfo.dwMajorVersion;
					_osMinorVersion = osInfo.dwMinorVersion;
					_osBuildNubmer = osInfo.dwBuildNumber;
				}

				_MESSAGE("Creation Kit Platform Extended Runtime: Initialize (Version: %s, OS: %u.%u Build %u)",
					VER_FILE_VERSION_STR, _osMajorVersion, _osMinorVersion, _osBuildNubmer);

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
				uint32_t hash_crc32 = CRC32File((String(lpcstrAppName) + ".exe").c_str());
				_MESSAGE("CRC32 executable file: 0x%08X", hash_crc32);

				// Получение начального адреса памяти главного процесса
				uintptr_t moduleBase = (uintptr_t)GetModuleHandle(NULL);

				// Пытаемся определить файл
				EDITOR_EXECUTABLE_TYPE editorVersion = EDITOR_UNKNOWN;
				auto editorVersionIterator = allowedEditorVersion.find(hash_crc32);
				if (editorVersionIterator == allowedEditorVersion.end())
				{
					// Файл не найден среди разрешённых, попытка определить по ключевым местам
					_WARNING("CRC32 does not match any of the known ones, running a version check by signature");

					for (auto editorVersionIterator2 = allowedEditorVersion2.begin();
						editorVersionIterator2 != allowedEditorVersion2.end();
						editorVersionIterator2++)
					{
						// Сравнение по указанному смещению нужной строки
						if (!_stricmp((const char*)(moduleBase + editorVersionIterator2->first),
							editorVersionIterator2->second.first.data()))
						{
							editorVersion = editorVersionIterator2->second.second;
							break;
						}
					}
				}
				else
					editorVersion = editorVersionIterator->second;

				if (editorVersion != EDITOR_UNKNOWN)
				{
					_MESSAGE("Current CK version: %s", allowedEditorVersionStr[(int)editorVersion].data());

					new Engine(hModule, editorVersion, moduleBase);
				}
				else
				{
					_ERROR("The CK version has not been determined");

					Result = RC_UNSUPPORTED_VERSION_CREATIONKIT;
				}
			}
			else
				Result = RC_UNKNOWN_APPLICATION;

			return Result;
		}
	}
}