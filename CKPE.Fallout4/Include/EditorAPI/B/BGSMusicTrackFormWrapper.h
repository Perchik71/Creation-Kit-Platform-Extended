// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/F/FormInterfaces.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSMusicTrack;

				// 1.11.240.0: size 0x40, vtable RVA 0x305FD20, 124 virtual functions
				class BGSMusicTrackFormWrapper :
					public TESForm,        // 000
					public BSIMusicTrack   // 028
				{
				public:
					virtual ~BGSMusicTrackFormWrapper() = default;

					constexpr static std::uint8_t TYPE_ID = ftMusicTrackFormWrapper;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSMusicTrackFormWrapper[0];
				
					inline BGSMusicTrack* GetTrack() const noexcept(true) { return trackImpl; }

					CKPE_READ_PROPERTY(GetTrack) BGSMusicTrack* Track;
				private:
					BGSMusicTrack* trackImpl; // 38
				};
				static_assert(sizeof(BGSMusicTrackFormWrapper) == 0x40);
			}
		}
	}
}
