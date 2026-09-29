// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BaseFormComponent.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class TESValueForm : public BaseFormComponent
			{
				// members
				std::uint32_t value;
			public:
				virtual ~TESValueForm() = default;

				[[nodiscard]] inline std::uint32_t GetValue() const noexcept(true) { return value; }
				inline void SetValue(std::uint32_t a_value) noexcept(true) { value = a_value; }

				CKPE_PROPERTY(GetValue, SetValue) std::uint32_t Value;
			};
			static_assert(sizeof(TESValueForm) == 0x10);
		}
	}
}