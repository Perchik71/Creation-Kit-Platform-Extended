// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/BSStringCache.h>
#include <Patches/CKPE.Fallout4.Patch.BSStringRefRE.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			BSStringRefRE::BSStringRefRE() : Common::Patch()
			{
				SetName("BSStringRef RE");
			}

			bool BSStringRefRE::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* BSStringRefRE::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool BSStringRefRE::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> BSStringRefRE::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool BSStringRefRE::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool BSStringRefRE::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db)
				{
					if (db->GetVersion() != 1)
						return false;

					auto _interface = CKPE::Common::Interface::GetSingleton();
					auto base = _interface->GetApplication()->GetBase();

					*(std::uintptr_t*)&EditorAPI::BSStringCache::Ref::ctor = __CKPE_OFFSET(0);
					*(std::uintptr_t*)&EditorAPI::BSStringCache::Ref::set = __CKPE_OFFSET(1);
					*(std::uintptr_t*)&EditorAPI::BSStringCache::Ref::release = __CKPE_OFFSET(2);

					return true;
				}
				else
				{
					using namespace Common;

					*(std::uintptr_t*)&EditorAPI::BSStringCache::Ref::ctor = Relocation(ID{ 1339190 }).Address();
					*(std::uintptr_t*)&EditorAPI::BSStringCache::Ref::set = Relocation(ID{ 1663882 }).Address();
					*(std::uintptr_t*)&EditorAPI::BSStringCache::Ref::release = Relocation(ID{ 1594894 }).Address();

					return true;
				}
			}
		}
	}
}