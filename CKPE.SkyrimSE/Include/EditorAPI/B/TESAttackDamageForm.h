// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BaseFormComponent.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// 1.7.99.0: size 0x10, vtable RVA 0x3079BB8. A 16 bit value @08 is zeroed on construction (damage; set by InitializeData of the owner).
			class TESAttackDamageForm : public BaseFormComponent
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::TESAttackDamageForm;
				constexpr static Common::ID VTABLE = IDs::VTABLE::TESAttackDamageForm[0];

				virtual ~TESAttackDamageForm() = default;

				std::uint16_t unk08;		// 08
				std::uint8_t pad0A[6];	// 0A
			};
			static_assert(sizeof(TESAttackDamageForm) == 0x10);
		}
	}
}