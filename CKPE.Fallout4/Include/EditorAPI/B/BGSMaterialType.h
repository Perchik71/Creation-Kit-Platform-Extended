// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/N/NiColor.h>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSImpactDataSet;

				// 1.11.240.0: size 0x90, vtable RVA 0x305D4D8, 124 virtual functions
				class BGSMaterialType :
					public TESForm   // 000
				{
				public:
					virtual ~BGSMaterialType() = default;

					constexpr static std::uint8_t TYPE_ID = ftMaterialType;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSMaterialType[0];
				public:
					TESModel breakableFXModel; // 28
					BGSMaterialType* parentType; // 60
					BGSImpactDataSet* havokImpactDataSet; // 68
					BSFixedString materialName; // 70
					std::uint32_t materialID; // 78
					NiAPI::NiColor materialColor; // 7C
					float buoyancy; // 88
					std::uint32_t flags; // 8C
				};
				static_assert(sizeof(BGSMaterialType) == 0x90);
			}
		}
	}
}
