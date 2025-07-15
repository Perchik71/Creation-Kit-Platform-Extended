// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <shlobj_core.h>
#include <CKPE.Detours.h>
#include <CKPE.Asserts.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixDataDlgWithPluginTXT.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			static char* FixDataDlgWithPluginTXTData;

			FixDataDlgWithPluginTXT::FixDataDlgWithPluginTXT() : Common::Patch()
			{
				SetName("Fix Data Dlg with PluginTXT");
			}

			bool FixDataDlgWithPluginTXT::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixDataDlgWithPluginTXT::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixDataDlgWithPluginTXT::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixDataDlgWithPluginTXT::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixDataDlgWithPluginTXT::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixDataDlgWithPluginTXT::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto cc_interface = CKPE::Common::Interface::GetSingleton();
				auto base = cc_interface->GetApplication()->GetBase();

				//
				// Fix for the "Data" window not listing plugins according to the user's load order. 
				// The CK tries to find plugins.txt in the executable directory instead of %localappdata%.
				//
				Detours::DetourJump(__CKPE_OFFSET(0), (uintptr_t)&sub);
				FixDataDlgWithPluginTXTData = (char*)__CKPE_OFFSET(1);

				return true;
			}

			void FixDataDlgWithPluginTXT::sub(const char* Path) noexcept(true)
			{
				char appDataPath[MAX_PATH];
				HRESULT hr = SHGetFolderPathA(nullptr, CSIDL_LOCAL_APPDATA, nullptr, 0, appDataPath);

				CKPE_ASSERT_MSG(SUCCEEDED(hr), "Failed to get user AppData path for plugins.txt");

				sprintf_s(FixDataDlgWithPluginTXTData, MAX_PATH,
					"%s\\%s\\", appDataPath, "Skyrim Special Edition");
			}
		}
	}
}