// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BSFixedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESForm;

				// 1.11.240.0: size 0x40, vtable RVA 0x2FB9800, 124 virtual functions
				class BGSDefaultObject :
					public TESForm   // 000
				{
				public:
					virtual ~BGSDefaultObject() = default;

					constexpr static std::uint8_t TYPE_ID = ftDefaultObject;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSDefaultObject[0];
				public:
					TESForm* form; // 28
					std::uint8_t type[0x8]; // 30 - ENUM_FORM_ID
					BSFixedString formEditorID; // 38
				};
				static_assert(sizeof(BGSDefaultObject) == 0x40);
			}
		}
	}
}
