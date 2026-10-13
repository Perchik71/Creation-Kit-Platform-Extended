// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundDescriptorForm;
				class TESRegion;
				class BGSReverbParameters;

				// 1.11.240.0: size 0xB0, vtable RVA 0x3054008, 154 virtual functions
				class BGSAcousticSpace :
					public TESBoundObject   // 000
				{
				public:
					virtual ~BGSAcousticSpace() = default;

				public:
					BGSSoundDescriptorForm* loopingSound; // 90
					TESRegion* soundRegion; // 98
					BGSReverbParameters* reverbType; // A0
					std::uint16_t weatherAttenMillibels; // A8
					bool isInterior; // AA
					std::uint8_t unkAB[0x5]; // AB
				};
				static_assert(sizeof(BGSAcousticSpace) == 0xB0);
			}
		}
	}
}
