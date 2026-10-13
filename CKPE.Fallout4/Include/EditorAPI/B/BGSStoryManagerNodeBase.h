// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BGSStoryManagerTreeForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x58, vtable RVA 0x3013048, 135 virtual functions
				class BGSStoryManagerNodeBase :
					public BGSStoryManagerTreeForm   // 000
				{
				public:
					virtual ~BGSStoryManagerNodeBase() = default;

					virtual void sub_400() { return; } // 128 - RVA 0xFF8360
					virtual void* GetConditions() noexcept(true) { return nullptr; } // 129 - RVA 0x87CF30, return (CDocument *)((char *)this + 64);
					virtual void sub_410() { return; } // 130 - RVA 0x87B260, return 1;
					virtual void sub_418() { return; } // 131 - RVA 0x87B270, return 1;
					virtual void sub_420() { return; } // 132 - RVA 0x87BF40
					virtual void sub_428() { return; } // 133 - RVA 0xFF8800
					virtual void sub_430() { return; } // 134 - RVA 0xFF8BF0

				public:
					std::uint8_t unk28[0x18]; // 28
					std::uint8_t unk40[0x18]; // 40
				};
				static_assert(sizeof(BGSStoryManagerNodeBase) == 0x58);
			}
		}
	}
}
