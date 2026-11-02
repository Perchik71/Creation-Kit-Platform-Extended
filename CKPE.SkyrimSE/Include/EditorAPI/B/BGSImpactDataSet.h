// Copyright © 2023-2024 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <EditorAPI/B/BGSImpactData.h>
#include <EditorAPI/N/NiTMap.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSMaterialType;

				// size 0x68
				// func 101
				class BGSImpactDataSet : public TESForm
				{
				public:
					constexpr static Common::ID RTTI = IDs::RTTI::BGSImpactDataSet;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSImpactDataSet[0];

					constexpr static uint8_t TYPE_ID = ftImpactDataSet;

					typedef NiAPI::NiTMap<BGSMaterialType*, BGSImpactData*> SetT;
				public:
					virtual ~BGSImpactDataSet() = default;

					SetT* GetList() const { return List; }
				private:
					char pad28[0x38];
					SetT* List;
				};
				static_assert(sizeof(BGSImpactDataSet) == 0x68);
			}
		}
	}
}