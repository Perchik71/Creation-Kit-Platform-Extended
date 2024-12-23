﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		using namespace CreationKitPlatformExtended::Core;

		class UnicodePatch : public Module
		{
		public:
			UnicodePatch();

			virtual bool HasOption() const;
			virtual bool HasCanRuntimeDisabled() const;
			virtual const char* GetOptionName() const;
			virtual const char* GetName() const;
			virtual bool HasDependencies() const;
			virtual Array<String> GetDependencies() const;

			static bool HKBeginPluginSave();
			static void HKEndPluginSave(HCURSOR hCursor);
			static BOOL HKSetDlgItemTextA(HWND hDlg, int nIDDlgItem, LPCSTR lpString);
			static LRESULT HKSendDlgItemMessageA(HWND hDlg, int nIDDlgItem, UINT Msg,
				WPARAM wParam, LPARAM lParam);

			static void HKPluginSaveSF(__int64 unk01, __int64 unk02, __int64 unk03, __int64 unk04);
			static const char* HKGetStringLocalizeSF(__int64 lstring);
		protected:
			virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const;
			virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
		private:
			UnicodePatch(const UnicodePatch&) = default;
			UnicodePatch& operator=(const UnicodePatch&) = default;
		};
	}
}