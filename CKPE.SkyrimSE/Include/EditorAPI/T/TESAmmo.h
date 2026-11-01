// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESModelTextureSwap.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/B/BGSMessageIcon.h>
#include <EditorAPI/T/TESValueForm.h>
#include <EditorAPI/T/TESWeightForm.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/B/TESDescription.h>
#include <EditorAPI/B/BGSKeywordForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x1B0, vtable RVA 0x30F0D48
				class TESAmmo :
					public TESBoundObject,				// 000
					public TESFullName,					// 050
					public TESModelTextureSwap,			// 068
					public TESIcon,						// 0B0
					public BGSMessageIcon,				// 0D8
					public TESValueForm,				// 108
					public TESWeightForm,				// 118
					public BGSDestructibleObjectForm,	// 128
					public BGSPickupPutdownSounds,		// 138
					public TESDescription,				// 150
					public BGSKeywordForm				// 178
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftAmmo;

					virtual ~TESAmmo() = default;

				/* 080 */ bool Load(TESFile* File) noexcept(true) override;
				/* 098 */ void Save() noexcept(true) override;
				/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
				/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
				public:
					std::uint8_t unk190[0x20];	// 190 - data, layout not resolved
				};
				static_assert(offsetof(TESAmmo, unk190) == 0x190);
				static_assert(sizeof(TESAmmo) == 0x1B0);
			}
		}
	}
}