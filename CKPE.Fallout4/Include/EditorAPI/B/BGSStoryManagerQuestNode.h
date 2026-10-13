// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BGSStoryManagerNodeBase.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0xE8, vtable RVA 0x310C630, 136 virtual functions
				class BGSStoryManagerQuestNode :
					public BGSStoryManagerNodeBase   // 000
				{
				public:
					virtual ~BGSStoryManagerQuestNode() = default;

					virtual void sub_438() { return; } // 135 - RVA 0xFFB650, LOBYTE(a3) = 1;; return (*(__int64 (__fastcall **)(__int64, __int64, __int64))(*(_QWORD *)

				public:
					std::uint8_t unk58[0x90]; // 58
				};
				static_assert(sizeof(BGSStoryManagerQuestNode) == 0xE8);
			}
		}
	}
}
