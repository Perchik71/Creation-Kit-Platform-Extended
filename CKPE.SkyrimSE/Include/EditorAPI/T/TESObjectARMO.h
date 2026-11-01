// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/TESRaceForm.h>
#include <EditorAPI/B/TESEnchantableForm.h>
#include <EditorAPI/T/TESValueForm.h>
#include <EditorAPI/T/TESWeightForm.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/B/TESBipedModelForm.h>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/B/BGSBipedObjectForm.h>
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
				// 1.7.99.0: size 0x300, vtable RVA 0x30FCA08
				class TESObjectARMO :
					public TESBoundObject,				// 000
					public TESFullName,					// 050
					public TESRaceForm,					// 068
					public TESEnchantableForm,			// 078
					public TESValueForm,				// 090
					public TESWeightForm,				// 0A0
					public BGSDestructibleObjectForm,	// 0B0
					public BGSPickupPutdownSounds,		// 0C0
					public TESBipedModelForm,			// 0D8
					public BGSEquipType,				// 250
					public BGSBipedObjectForm,			// 260
					public BGSBlockBashData,			// 270
					public BGSKeywordForm,				// 288
					public TESDescription				// 2A0
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftArmor;

					virtual ~TESObjectARMO() = default;

					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
				public:
					std::uint8_t unk2C8[0x38];	// 2C8 - armor data, layout not resolved
				};
				static_assert(offsetof(TESObjectARMO, unk2C8) == 0x2C8);
				static_assert(sizeof(TESObjectARMO) == 0x300);
			}
		}
	}
}