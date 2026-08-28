// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixMusicTrackForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixMusicTrackForm::FixMusicTrackForm() : Common::Patch()
			{
				SetName("Fix MusicTrack Form");
			}

			bool FixMusicTrackForm::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixMusicTrackForm::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixMusicTrackForm::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixMusicTrackForm::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixMusicTrackForm::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixMusicTrackForm::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixMusicTrackForm::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for crash when saving a plugin with an empty single track file path in a Music Track form. 
				// Null pointer dereference.
				//
				Relocation(ID(212189), 0x2C).WriteCall(&sub);

				return true;
			}

			std::size_t FixMusicTrackForm::sub(const char* string) noexcept(true)
			{
				return string ? strlen(string) : 0;
			}
		}
	}
}