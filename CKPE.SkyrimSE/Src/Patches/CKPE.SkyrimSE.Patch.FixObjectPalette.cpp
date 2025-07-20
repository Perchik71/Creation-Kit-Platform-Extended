// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixObjectPalette.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			typedef std::int64_t(*TFixObjectPaletteSub1)(std::int64_t, std::uint32_t*);
			typedef void(*TFixObjectPaletteSub2)(std::int64_t);

			static TFixObjectPaletteSub1 FixObjectPaletteSub1;
			static TFixObjectPaletteSub2 FixObjectPaletteSub2;

			FixObjectPalette::FixObjectPalette() : Common::Patch()
			{
				SetName("Fix Object Palette");
			}

			bool FixObjectPalette::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixObjectPalette::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixObjectPalette::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixObjectPalette::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixObjectPalette::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixObjectPalette::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for the "Object Palette" preview window not working. Window render state has to be set to '2'.
				//
				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub);
				FixObjectPaletteSub1 = (TFixObjectPaletteSub1)__CKPE_OFFSET(1);
				FixObjectPaletteSub2 = (TFixObjectPaletteSub2)__CKPE_OFFSET(2);

				return true;
			}

			void FixObjectPalette::sub(std::int64_t WindowHandle, std::uint32_t* ControlId) noexcept(true)
			{
				auto previewControl = FixObjectPaletteSub1(WindowHandle, ControlId);
				if (previewControl) FixObjectPaletteSub2(previewControl);
			}
		}
	}
}