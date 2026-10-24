// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESFormIDListView.h>
#include <EditorAPI/T/TESTexture.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				struct ImageSpaceBaseDataHDR
				{
					float eyeAdaptSpeed; // 00
					float bloomBlurRadius; // 04
					float bloomThreshold; // 08
					float bloomScale; // 0C
					float receiveBloomThreshold; // 10
					float white; // 14
					float sunlightScale; // 18
					float skyScale; // 1C
					float eyeAdaptStrength; // 20
				};
				static_assert(sizeof(ImageSpaceBaseDataHDR) == 0x24);

				struct ImageSpaceBaseDataCinematic
				{
					float saturation; // 00
					float brightness; // 04
					float contrast; // 08
				};
				static_assert(sizeof(ImageSpaceBaseDataCinematic) == 0xC);

				struct ImageSpaceBaseDataColorF
				{
					float red; // 00
					float green; // 04
					float blue; // 08
				};
				static_assert(sizeof(ImageSpaceBaseDataColorF) == 0xC);

				struct ImageSpaceBaseDataTint
				{
					float amount; // 00
					ImageSpaceBaseDataColorF color; // 04
				};
				static_assert(sizeof(ImageSpaceBaseDataTint) == 0x10);

				struct ImageSpaceBaseDataDepthOfField
				{
					float strength; // 00
					float distance; // 04
					float range; // 08
					float vignetteRadius; // 0C
					float vignetteStrength; // 10
					float mode; // 14
				};
				static_assert(sizeof(ImageSpaceBaseDataDepthOfField) == 0x18);

				struct ImageSpaceBaseData
				{
					ImageSpaceBaseDataHDR hdrData; // 00
					ImageSpaceBaseDataCinematic cinematicData; // 24
					ImageSpaceBaseDataTint tintData; // 30
					ImageSpaceBaseDataDepthOfField dofData; // 40
				};
				static_assert(sizeof(ImageSpaceBaseData) == 0x58);

				// The CK embeds its form-list state before the image-space data.
				class TESImageSpace :
					public TESFormIDListView   // 000
				{
				public:
					virtual ~TESImageSpace() = default;

				public:
					std::uint64_t unk28; // 28
					std::uint32_t unk30; // 30
					std::uint32_t unk34; // 34
					ImageSpaceBaseData data; // 38
					TESTexture lutTexture; // 90 - TX00
					std::uint64_t niLutTexture; // B8 - NiPointer<NiTexture>, by position after lutTexture
				};
				static_assert(offsetof(TESImageSpace, data) == 0x38);
				static_assert(offsetof(TESImageSpace, lutTexture) == 0x90);
				static_assert(offsetof(TESImageSpace, niLutTexture) == 0xB8);
				static_assert(sizeof(TESImageSpace) == 0xC0);
			}
		}
	}
}
