// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/F/FormInterfaces.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/B/BGSPropertySheet.h>
#include <EditorAPI/B/BGSForcedLocRefType.h>
#include <EditorAPI/B/BGSNativeTerminalForm.h>
#include <EditorAPI/B/BGSLocalizedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundDescriptorForm;
				class TESWaterForm;

				// 1.11.240.0: size 0x1E8, vtable RVA 0x3092248, 154 virtual functions
				class __declspec(empty_bases) TESObjectACTI :
					public TESBoundAnimObject,         // 000
					public TESFullName,                // 090
					public BGSModelMaterialSwap,       // 0A8
					public BGSDestructibleObjectForm,  // 0F8
					public BGSOpenCloseForm,           // 108
					public BGSKeywordForm,             // 110
					public BGSPropertySheet,           // 130
					public BGSForcedLocRefType,        // 140
					public BGSNativeTerminalForm,      // 150
					public TESMagicTargetForm,         // 160
					public BGSNavmeshableObject        // 160
				{
				public:
					virtual ~TESObjectACTI() = default;

				public:
					BGSSoundDescriptorForm* soundLoop; // 160
					BGSSoundDescriptorForm* soundActivate; // 168
					TESWaterForm* waterForm; // 170
					std::uint16_t flags; // 178
					std::uint8_t unk17A[0x6]; // 17A
					std::uint16_t unk180; // 180
					std::uint8_t unk182[0x6]; // 182
					BGSLocalizedString unk188; // 188
					std::uint8_t unk198[0x10]; // 198
					std::uint8_t unk1A8[0x10]; // 1A8
					std::uint8_t unk1B8[0x10]; // 1B8
					std::uint64_t unk1C8; // 1C8
					std::uint8_t unk1D0[0x18]; // 1D0
				};
				static_assert(sizeof(TESObjectACTI) == 0x1E8);
			}
		}
	}
}
