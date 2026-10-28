// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/B/BGSMessageIcon.h>
#include <EditorAPI/T/TESModelRDT.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 1.11.240.0: size 0x190, vtable RVA 0x301C6E8, 21 virtual functions
			class TESBipedModelForm :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESBipedModelForm() = default;

				constexpr static Common::ID VTABLE = IDs::VTABLE::TESBipedModelForm[0];
			public:
				Forms::BGSModelMaterialSwap worldModel[2]; // 08 - sizeof 0x50 each (0x08, 0x58)
				TESIcon inventoryIcon[2]; // A8 - sizeof 0x28 each (0xA8, 0xD0)
				BGSMessageIcon messageIcon[2]; // F8 - sizeof 0x30 each (0xF8, 0x128)
				TESModelRDT constraintTemplate; // 158
			};
			static_assert(sizeof(TESBipedModelForm) == 0x190);
		}
	}
}
