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
			class TESWeightForm : public BaseFormComponent
			{
				// members
				float weight;
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::TESWeightForm;
				constexpr static Common::ID VTABLE = IDs::VTABLE::TESWeightForm[0];

				virtual ~TESWeightForm() = default;

				[[nodiscard]] inline float GetWeight() const noexcept(true) { return weight; }
				inline void SetWeight(float a_value) noexcept(true) { weight = a_value; }

				CKPE_PROPERTY(GetWeight, SetWeight) float Weight;
			};
			static_assert(sizeof(TESWeightForm) == 0x10);
		}
	}
}