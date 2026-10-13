// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESFormIDListView.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x40, vtable RVA 0x30657A8, 126 virtual functions
				class GameSettingForm :
					public TESFormIDListView   // 000
				{
				public:
					virtual ~GameSettingForm() = default;

				public:
					std::uint8_t unk28[0x18]; // 28
				};
				static_assert(sizeof(GameSettingForm) == 0x40);
			}
		}
	}
}
