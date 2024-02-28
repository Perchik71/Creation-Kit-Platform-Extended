// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "BGSImpactData.h"
#include "NiAPI/NiTMap.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class BGSMaterialType;

			// size 0x68
			// func 101
			class BGSImpactDataSet : public TESForm
			{
			public:
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

#pragma pack(pop)