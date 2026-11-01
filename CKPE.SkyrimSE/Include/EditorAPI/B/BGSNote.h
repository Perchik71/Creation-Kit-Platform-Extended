// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/T/TESModel.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x100, vtable RVA 0x309F408
				class BGSNote :
					public TESBoundObject,	// 00
					public TESModel,	// 50
					public TESFullName,	// 80
					public TESIcon,	// 98
					public BGSPickupPutdownSounds	// C0
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftNote;

					virtual ~BGSNote() = default;
				public:
					std::uint8_t unkD8[0x28];	// D8 - data, layout not resolved
				};
				static_assert(offsetof(BGSNote, unkD8) == 0xD8);
				static_assert(sizeof(BGSNote) == 0x100);
			}
		}
	}
}