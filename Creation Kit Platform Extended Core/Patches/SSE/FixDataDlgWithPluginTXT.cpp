// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixDataDlgWithPluginTXT.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_FixDataDlgWithPluginTXT_data = 0;

			FixDataDlgWithPluginTXTPatch::FixDataDlgWithPluginTXTPatch() : Module(GlobalEnginePtr)
			{}

			bool FixDataDlgWithPluginTXTPatch::HasOption() const
			{
				return false;
			}

			bool FixDataDlgWithPluginTXTPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixDataDlgWithPluginTXTPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixDataDlgWithPluginTXTPatch::GetName() const
			{
				return "Fix Data Dlg with PluginTXT";
			}

			bool FixDataDlgWithPluginTXTPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixDataDlgWithPluginTXTPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for the "Data" window not listing plugins according to the user's load order. 
					// The CK tries to find plugins.txt in the executable directory instead of %localappdata%.
					//
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					pointer_FixDataDlgWithPluginTXT_data = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));	

					return true;
				}

				return false;
			}

			bool FixDataDlgWithPluginTXTPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixDataDlgWithPluginTXTPatch::sub(const char* Path)
			{
				char appDataPath[MAX_PATH];
				HRESULT hr = SHGetFolderPathA(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, appDataPath);

				AssertMsg(SUCCEEDED(hr), "Failed to get user AppData path for plugins.txt");

				sprintf_s((char*)pointer_FixDataDlgWithPluginTXT_data, MAX_PATH, 
					"%s\\%s\\", appDataPath, "Skyrim Special Edition");
			}
		}
	}
}