// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESModel.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSTextureSet;
				class BGSSoundDescriptorForm;
				class BGSExplosion;
				class BGSHazard;

				struct IMPACT_DATA_DATA
				{
					float effectDuration; // 00
					std::int32_t orient; // 04 - REX::TEnumSet<ORIENTATION, std::int32_t>
					float angleThreshold; // 08
					float placementRadius; // 0C
					std::int32_t soundLevel; // 10 - REX::TEnumSet<SOUND_LEVEL, std::int32_t>
					std::int8_t flags; // 14
					std::int8_t resultOverride; // 15
				};
				static_assert(sizeof(IMPACT_DATA_DATA) == 0x18);

				struct DECAL_DATA_DATA
				{
					float decalMinWidth; // 00
					float decalMaxWidth; // 04
					float decalMinHeight; // 08
					float decalMaxHeight; // 0C
					float depth; // 10
					float shininess; // 14
					float parallaxScale; // 18
					std::int8_t parallaxPasses; // 1C
					std::int8_t flags; // 1D
					std::uint32_t color; // 20
				};
				static_assert(sizeof(DECAL_DATA_DATA) == 0x24);

				struct DecalData
				{
					DECAL_DATA_DATA data; // 00
				};
				static_assert(sizeof(DecalData) == 0x24);

				// 1.11.240.0: size 0xD0, vtable RVA 0x305B1D8, 124 virtual functions
				class BGSImpactData :
					public TESForm,   // 000
					public TESModel   // 028
				{
				public:
					virtual ~BGSImpactData() = default;

					constexpr static std::uint8_t TYPE_ID = ftImpactData;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSImpactData[0];
				public:
					IMPACT_DATA_DATA data; // 60
					BGSTextureSet* decalTextureSet; // 78
					BGSTextureSet* decalTextureSet2; // 80
					BGSSoundDescriptorForm* sound1; // 88
					BGSSoundDescriptorForm* sound2; // 90
					BGSExplosion* explosion; // 98
					BGSHazard* hazard; // A0
					DecalData decalData; // A8
					float maxFootstepParticleDist; // CC
				};
				static_assert(sizeof(BGSImpactData) == 0xD0);
			}
		}
	}
}
