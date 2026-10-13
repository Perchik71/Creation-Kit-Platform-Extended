// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/F/FormInterfaces.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class DynamicAttenuationCharacteristics;
				class SpeakerArrays;
				class BGSAudioEffectChain;

				// 1.11.240.0: size 0x58, vtable RVA 0x30633E0, 124 virtual functions
				class BGSSoundOutput :
					public TESForm,              // 000
					public BSISoundOutputModel   // 028
				{
				public:
					virtual ~BGSSoundOutput() = default;

				public:
					DynamicAttenuationCharacteristics* attenuation; // 30
					SpeakerArrays* speakerOutputArrays; // 38
					BGSAudioEffectChain* effectChain; // 40
					std::uint32_t flags; // 48
					std::int32_t mode; // 4C - REX::TEnumSet<SPEAKER_OUTPUT_MODE, std::int32_t>
					std::uint16_t staticAttenuation; // 50
					std::uint8_t unk52[0x6]; // 52
				};
				static_assert(sizeof(BGSSoundOutput) == 0x58);
			}
		}
	}
}
