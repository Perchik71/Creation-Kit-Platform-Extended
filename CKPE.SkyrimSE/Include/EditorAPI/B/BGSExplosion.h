// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BGSPreloadable.h>
#include <EditorAPI/B/TESEnchantableForm.h>
#include <EditorAPI/B/TESImageSpaceModifiableForm.h>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESModel.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x118, vtable RVA 0x30EB328
				class BGSExplosion :
					public TESBoundObject,	// 00
					public TESFullName,	// 50
					public TESModel,	// 68
					public TESEnchantableForm,	// 98
					public BGSPreloadable,	// B0
					public TESImageSpaceModifiableForm	// B8
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftExplosion;

					virtual ~BGSExplosion() = default;
				public:
					std::uint8_t unkC8[0x50];	// C8 - data, layout not resolved
				};
				static_assert(offsetof(BGSExplosion, unkC8) == 0xC8);
				static_assert(sizeof(BGSExplosion) == 0x118);
			}
		}
	}
}