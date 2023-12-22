// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "ResultCoreErrNo.h"
#include "ModuleManager.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		struct Section
		{
			uintptr_t base;
			uintptr_t end;
		};

		constexpr static uint32_t SECTION_TEXT = 0;
		constexpr static uint32_t SECTION_DATA_READONLY = 1;
		constexpr static uint32_t SECTION_DATA = 2;

		class Engine
		{
		public:
			Engine(HMODULE hModule, EDITOR_EXECUTABLE_TYPE eEditorVersion, uintptr_t nModuleBase);
			~Engine() = default;
			
			inline HINSTANCE GetInstanceDLL() const { return (HINSTANCE)_module; }
			inline uintptr_t GetModuleBase() const { return _moduleBase; }
			inline uint64_t GetModuleSize() const { return _moduleSize; }
			inline EDITOR_EXECUTABLE_TYPE GetEditorVersion() const { return _editorVersion; }
			inline bool HasAVX2() const { return _hasAVX2; }
			inline bool HasSSE41() const { return _hasSSE41; }
			inline Section GetSection(uint32_t nIndex) const { return Sections[nIndex]; }
		public:
			static IResult Initialize(HMODULE hModule, LPCSTR lpcstrAppName);
		private:
			Engine& operator=(Engine&) = default;
			Engine(Engine&) = default;

			void CommandLineRun();
			void ContinueInitialize();
			void EnableBreakpoint();
			void DisableBreakpoint();
		private:
			uintptr_t _moduleBase;
			uint64_t _moduleSize;
			bool _hasAVX2;
			bool _hasSSE41;
			EDITOR_EXECUTABLE_TYPE _editorVersion;
			HMODULE _module;
			Section Sections[3];
			ModuleManager* PatchesManager;
		};

		extern Engine* GlobalEnginePtr;
	}
}