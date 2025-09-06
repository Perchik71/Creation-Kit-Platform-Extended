// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <EditorAPI/TESPointerHandleDetail.h>
#include <Patches/CKPE.Starfield.Patch.TESPointerHandleDetail.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			TESPointerHandleDetail::TESPointerHandleDetail() : Common::Patch()
			{
				SetName("Pointer Handle Manager");
			}

			bool TESPointerHandleDetail::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* TESPointerHandleDetail::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool TESPointerHandleDetail::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> TESPointerHandleDetail::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool TESPointerHandleDetail::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_STARFIELD_LAST;
			}

			bool TESPointerHandleDetail::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				EditorAPI::HandleManager::Singleton = __CKPE_OFFSET(0);

				return true;
			}
		}
	}
}