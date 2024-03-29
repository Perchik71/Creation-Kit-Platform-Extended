﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"
#include "Editor API/FO4/BSEntryString.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		using namespace CreationKitPlatformExtended::Core;

		class UIHotkeysPatch : public Module
		{
		public:
			UIHotkeysPatch();

			virtual bool HasOption() const;
			virtual bool HasCanRuntimeDisabled() const;
			virtual const char* GetOptionName() const;
			virtual const char* GetName() const;
			virtual bool HasDependencies() const;
			virtual Array<String> GetDependencies() const;

			static void sub(void* Thisptr, void(*Callback)(), const char** HotkeyFunction,
				const char** DisplayText, char VirtualKey, bool Alt, bool Ctrl, bool Shift);
			static void sub_ver2(void* Thisptr, void(*Callback)(), EditorAPI::Fallout4::BSEntryString** HotkeyFunction,
				EditorAPI::Fallout4::BSEntryString** DisplayText, char VirtualKey, bool Alt, bool Ctrl, bool Shift);
		protected:
			virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const;
			virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
		private:
			UIHotkeysPatch(const UIHotkeysPatch&) = default;
			UIHotkeysPatch& operator=(const UIHotkeysPatch&) = default;
		};
	}
}