// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSArtObject;
				class TESEffectShader;

				// 1.11.240.0: size 0x40, vtable RVA 0x30A5BB8, 124 virtual functions
				class BGSReferenceEffect :
					public TESForm   // 000
				{
				public:
					virtual ~BGSReferenceEffect() = default;

				public:
					// CommonLibF4's Data (the constructor / InitializeData zero 28, 30 and 38)
					BGSArtObject* artObject; // 28
					TESEffectShader* effectShader; // 30
					std::uint32_t flags; // 38 - REX::TEnumSet<Flag, std::uint32_t>: 1 face target, 2 attach to camera, 4 inherit rotation
					std::uint32_t pad3C; // 3C
				};
				static_assert(sizeof(BGSReferenceEffect) == 0x40);
			}
		}
	}
}
