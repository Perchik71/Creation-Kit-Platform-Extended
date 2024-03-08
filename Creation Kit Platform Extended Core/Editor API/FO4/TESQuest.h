// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TESFormF4.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			// size = 0x270
			// func = 129
			class TESQuest : public TESForm
			{
			public:
				constexpr static uint8_t TYPE_ID = ftQuest;

				virtual ~TESQuest() = default;
			public:
			private:
				char pad28[0x248];
			};
			static_assert(sizeof(TESQuest) == 0x270);
		}
	}
}

#pragma pack(pop)