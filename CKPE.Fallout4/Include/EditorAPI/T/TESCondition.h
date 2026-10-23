// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESCondition
				{
				public:
					virtual ~TESCondition() = default;

					/* 0x008 */ void* head;
				private:
					/* 0x010 */ char pad10[0x8];
				public:
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(TESCondition, head) == 0x8;
					}
				};
				static_assert(sizeof(TESCondition) == 0x18);
				static_assert(TESCondition::VerifyLayout());
			}
		}
	}
}