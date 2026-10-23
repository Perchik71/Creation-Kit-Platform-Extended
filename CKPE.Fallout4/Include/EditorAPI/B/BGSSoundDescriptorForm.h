// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/F/FormInterfaces.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundDescriptor;

				// 1.11.240.0: size 0x48, vtable RVA 0x3062A10, 124 virtual functions
				class BGSSoundDescriptorForm :
					public TESForm,             // 000
					public BSISoundDescriptor   // 028
				{
				public:
					virtual ~BGSSoundDescriptorForm() = default;

				public:
					BSFixedString unk30; // 30
					BGSSoundDescriptor* impl; // 38 - CNAM, created from the sound descriptor type id through BSTCreateFactory
					BSFixedString notes; // 40 - NNAM
				};
				static_assert(sizeof(BGSSoundDescriptorForm) == 0x48);
			}
		}
	}
}
