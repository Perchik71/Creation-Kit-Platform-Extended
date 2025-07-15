// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixIconsScriptProp.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixIconsScriptProp::FixIconsScriptProp() : Common::Patch()
			{
				SetName("Fix Icons Script Prop");
			}

			bool FixIconsScriptProp::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixIconsScriptProp::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixIconsScriptProp::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixIconsScriptProp::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixIconsScriptProp::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixIconsScriptProp::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for icons not appearing in the script properties dialog (list view) (LVIF_TEXT -> LVIF_IMAGE)
				//
				SafeWrite::Write(__CKPE_OFFSET(0), { 0x02 });

				return true;
			}
		}
	}
}