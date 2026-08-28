// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <shlobj_core.h>
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

			bool FixDataDlgWithPluginTXT::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixDataDlgWithPluginTXT::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixDataDlgWithPluginTXT::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for the "Data" window not listing plugins according to the user's load order. 
				// The CK tries to find plugins.txt in the executable directory instead of %localappdata%.
				//
				
				Relocation(ID(564008)).WriteJump(&sub);
				FixDataDlgWithPluginTXTData = reinterpret_cast<char*>(ID(291941).Address());

				return true;
			}

			void FixDataDlgWithPluginTXT::sub([[maybe_unused]] const char* Path) noexcept(true)
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