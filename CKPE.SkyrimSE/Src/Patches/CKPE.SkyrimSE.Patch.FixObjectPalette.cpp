// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

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
			using TFixObjectPaletteSub1 = std::int64_t(std::int64_t, std::uint32_t*);
			using TFixObjectPaletteSub2 = void(std::int64_t);

			static std::function<TFixObjectPaletteSub1> FixObjectPaletteSub1;
			static std::function<TFixObjectPaletteSub2> FixObjectPaletteSub2;

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

			bool FixObjectPalette::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixObjectPalette::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixObjectPalette::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for the "Object Palette" preview window not working. Window render state has to be set to '2'.
				//
				Relocation(ID(177563), Offset{ 0x2F6, 0x2FE }).WriteCall(&sub);
				FixObjectPaletteSub1 = Relocation<TFixObjectPaletteSub1>(ID(127281)).Get();
				FixObjectPaletteSub2 = Relocation<TFixObjectPaletteSub2>(ID(79119)).Get();

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