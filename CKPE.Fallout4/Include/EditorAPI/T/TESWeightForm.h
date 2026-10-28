// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BaseFormComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class TESWeightForm : public BaseFormComponent
			{
				// members
				float weight;
			public:
				virtual ~TESWeightForm() = default;

				constexpr static Common::ID VTABLE = IDs::VTABLE::TESWeightForm[0];

				[[nodiscard]] inline float GetWeight() const noexcept(true) { return weight; }
				inline void SetWeight(float a_value) noexcept(true) { weight = a_value; }

				CKPE_PROPERTY(GetWeight, SetWeight) float Weight;
			};
			static_assert(sizeof(TESWeightForm) == 0x10);
		}
	}
}