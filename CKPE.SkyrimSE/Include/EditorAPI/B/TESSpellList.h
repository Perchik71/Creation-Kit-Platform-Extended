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
			// 1.7.99.0: size 0x10, vtable RVA 0x3081A30. Pointer @08 is zeroed on construction (spell data; layout not resolved).
			class TESSpellList : public BaseFormComponent
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::TESSpellList;
				constexpr static Common::ID VTABLE = IDs::VTABLE::TESSpellList[0];

				virtual ~TESSpellList() = default;

				void* unk08;	// 08
			};
			static_assert(sizeof(TESSpellList) == 0x10);
		}
	}
}