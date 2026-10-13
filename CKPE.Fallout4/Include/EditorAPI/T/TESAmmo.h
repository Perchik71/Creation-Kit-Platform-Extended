// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/B/BGSMessageIcon.h>
#include <EditorAPI/T/TESValueForm.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSPickupPutdownSounds.h>
#include <EditorAPI/T/TESDescription.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/T/TESWeightForm.h>
#include <EditorAPI/B/BGSLocalizedString.h>
#include <EditorAPI/T/TESModel.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSProjectile;

				struct AMMO_DATA
				{
					BGSProjectile* projectile; // 00
					std::uint32_t health; // 08
					std::int8_t flags; // 0C
					float damage; // 10
				};
				static_assert(sizeof(AMMO_DATA) == 0x18);

				// 1.11.240.0: size 0x240, vtable RVA 0x30872B0, 154 virtual functions
				class TESAmmo :
					public TESBoundObject,             // 000
					public TESFullName,                // 090
					public BGSModelMaterialSwap,       // 0A8
					public TESIcon,                    // 0F8
					public BGSMessageIcon,             // 120
					public TESValueForm,               // 150
					public BGSDestructibleObjectForm,  // 160
					public BGSPickupPutdownSounds,     // 170
					public TESDescription,             // 188
					public BGSKeywordForm,             // 1B0
					public TESWeightForm               // 1D0
				{
				public:
					virtual ~TESAmmo() = default;

				public:
					AMMO_DATA data; // 1E0
					BGSLocalizedString shortDesc; // 1F8
					TESModel shellCasing; // 208
				};
				static_assert(sizeof(TESAmmo) == 0x240);
			}
		}
	}
}
