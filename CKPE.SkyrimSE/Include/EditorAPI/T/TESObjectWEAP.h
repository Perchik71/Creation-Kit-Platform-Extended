// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESModelTextureSwap.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/B/TESEnchantableForm.h>
#include <EditorAPI/T/TESValueForm.h>
#include <EditorAPI/T/TESWeightForm.h>
#include <EditorAPI/B/TESAttackDamageForm.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/B/BGSPreloadable.h>
#include <EditorAPI/B/BGSMessageIcon.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/B/BGSBlockBashData.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/B/TESDescription.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x2B0, vtable RVA 0x3103628
				class TESObjectWEAP :
					public TESBoundObject,				// 000
					public TESFullName,					// 050
					public TESModelTextureSwap,			// 068
					public TESIcon,						// 0B0
					public TESEnchantableForm,			// 0D8
					public TESValueForm,				// 0F0
					public TESWeightForm,				// 100
					public TESAttackDamageForm,			// 110
					public BGSDestructibleObjectForm,	// 120
					public BGSEquipType,				// 130
					public BGSPreloadable,				// 140
					public BGSMessageIcon,				// 148
					public BGSPickupPutdownSounds,		// 178
					public BGSBlockBashData,			// 190
					public BGSKeywordForm,				// 1A8
					public TESDescription				// 1C0
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftWeapon;

					virtual ~TESObjectWEAP() = default;

					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
				public:
					std::uint8_t unk1E8[0xC8];	// 1E8 - weapon data, layout not resolved
				};
				static_assert(offsetof(TESObjectWEAP, unk1E8) == 0x1E8);
				static_assert(sizeof(TESObjectWEAP) == 0x2B0);
			}
		}
	}
}