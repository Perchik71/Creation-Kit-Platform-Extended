// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESModel.h>
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
				// 1.11.240.0: size 0xD8, vtable RVA 0x305CC18, 124 virtual functions
				class BGSMaterialObject :
					public TESForm,           // 000
					public TESModel,          // 028
					public BSMaterialObject   // 060
				{
				public:
					virtual ~BGSMaterialObject() = default;

					constexpr static std::uint8_t TYPE_ID = ftMaterialObject;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSMaterialObject[0];
				public:
					BSTArray<void*> fileData; // B0 - BSTArray<FILE_DATA>
					std::uint64_t unkC8; // C8
					std::uint64_t unkD0; // D0
				};
				static_assert(sizeof(BGSMaterialObject) == 0xD8);
			}
		}
	}
}
