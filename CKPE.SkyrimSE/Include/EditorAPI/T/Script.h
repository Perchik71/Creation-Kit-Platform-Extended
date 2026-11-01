// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x78, vtable RVA 0x30D2BF0
				class Script :
					public TESForm	// 00
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftScript;

					virtual ~Script() = default;
				public:
					std::uint8_t schr[0x8];	// 28 - SCHR
					std::uint32_t scda;	// 30 - SCDA
					std::uint8_t pad34[0xC];	// 34
					void* sctx;	// 40 - SCTX
					void* scda_48;	// 48 - SCDA
					void* qnam;	// 50 - QNAM
					std::uint8_t pad58[0x20];	// 58
				};
				static_assert(offsetof(Script, schr) == 0x28);
				static_assert(offsetof(Script, scda) == 0x30);
				static_assert(offsetof(Script, sctx) == 0x40);
				static_assert(offsetof(Script, scda_48) == 0x48);
				static_assert(offsetof(Script, qnam) == 0x50);
				static_assert(sizeof(Script) == 0x78);
			}
		}
	}
}