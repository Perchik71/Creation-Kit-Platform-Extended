// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/F/FormInterfaces.h>
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
				// 1.11.240.0: size 0x48, vtable RVA 0x2FB7660, 124 virtual functions
				class BGSAudioEffectChain :
					public TESForm,              // 000
					public BSIAudioEffectChain   // 028
				{
				public:
					virtual ~BGSAudioEffectChain() = default;

				public:
					BSTArray<void*> effects; // 30 - BSTArray<BSIAudioEffectParameters*>
				};
				static_assert(sizeof(BGSAudioEffectChain) == 0x48);
			}
		}
	}
}
