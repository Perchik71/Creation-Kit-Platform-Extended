// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BSTHashMap.h>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSKeyword;
				class BGSSoundDescriptorForm;

				// 1.11.240.0: size 0xB8, vtable RVA 0x2FBAC48, 124 virtual functions
				class BGSSoundKeywordMapping :
					public TESForm   // 000
				{
				public:
					virtual ~BGSSoundKeywordMapping() = default;

				public:
					BSTSet<BGSKeyword*> keywordSet; // 28 - KNAM, same type as CommonLibF4
					std::uint8_t unk58[0x10]; // 58
					BSTHashMap<std::uint32_t, BGSSoundDescriptorForm*> reverbDescMap; // 68 - PTRN, same type as CommonLibF4
					BGSSoundDescriptorForm* descriptor; // 98 - KNAM
					BGSSoundDescriptorForm* exteriorTail; // A0 - KNAM
					BGSSoundDescriptorForm* vatsDescriptor; // A8 - PTRN
					float vatsDescThreshold; // B0 - PTRN
					std::uint32_t unkB4; // B4
				};
				static_assert(sizeof(BGSSoundKeywordMapping) == 0xB8);
			}
		}
	}
}
