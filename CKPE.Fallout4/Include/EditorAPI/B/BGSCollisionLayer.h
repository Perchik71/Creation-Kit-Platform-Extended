// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESDescription.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x80, vtable RVA 0x3055BC8, 124 virtual functions
				class BGSCollisionLayer :
					public TESForm,         // 000
					public TESDescription   // 028
				{
				public:
					virtual ~BGSCollisionLayer() = default;

					constexpr static std::uint8_t TYPE_ID = ftCollisionLayer;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSCollisionLayer[0];
				public:
					std::uint32_t collisionIdx; // 50
					std::uint8_t unk54[0xC]; // 54
					BSFixedString name; // 60
					BSTArray<void*> collidesWith; // 68 - BSTArray<BGSCollisionLayer*>
				};
				static_assert(sizeof(BGSCollisionLayer) == 0x80);
			}
		}
	}
}
