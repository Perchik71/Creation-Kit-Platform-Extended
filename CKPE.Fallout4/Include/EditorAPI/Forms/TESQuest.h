// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "TESForm.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// size = 0x270
				// func = 129
				class TESQuest : public TESForm
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftQuest;

					virtual ~TESQuest() = default;
				public:
				private:
					char pad28[0x248];
				};
				static_assert(sizeof(TESQuest) == 0x270);
			}
		}
	}
}