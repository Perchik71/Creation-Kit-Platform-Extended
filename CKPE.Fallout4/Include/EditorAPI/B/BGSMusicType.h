// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/F/FormInterfaces.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x78, vtable RVA 0x3060568, 124 virtual functions
				class BGSMusicType :
					public TESForm,       // 000
					public BSIMusicType   // 028
				{
				public:
					virtual ~BGSMusicType() = default;

				public:
					std::uint32_t flags; // 30 - BSIMusicType::flags
					std::int8_t priority; // 34
					std::uint8_t pad35; // 35
					std::uint16_t ducksOtherMusicBy; // 36
					float fadeTime; // 38
					std::uint32_t currentTrackIndex; // 3C
					BSTArray<void*> trackHistory; // 40 - BSTArray<std::uint32_t>
					BSTArray<void*> tracks; // 58 - BSTArray<BSIMusicTrack*>
					std::int32_t typeStatus; // 70 - REX::EnumSet<BSIMusicTrack::MUSIC_STATUS, std::int32_t>
					std::uint32_t pad74; // 74
				};
				static_assert(sizeof(BGSMusicType) == 0x78);
			}
		}
	}
}
