﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "ResultCoreErrNo.h"
#include "ModuleManager.h"
#include "PluginManager.h"

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
			virtual uintptr_t GetModuleBase() const;
			virtual uint64_t GetModuleSize() const;
			virtual EDITOR_EXECUTABLE_TYPE GetEditorVersion() const;
			inline ModuleManager* GetPatchesManager() const { return PatchesManager; }
			inline PluginManager* GetUserPluginsManager() const { return UserPluginsManager; }
			virtual bool HasAVX2() const;
			virtual bool HasSSE41() const;
			virtual Section GetSection(uint32_t nIndex) const;
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
			PluginManager* UserPluginsManager;
			Module* _Theme;
		};

		extern Engine* GlobalEnginePtr;
	}
}