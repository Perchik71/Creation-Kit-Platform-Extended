// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/F/FormInterfaces.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x1A78, vtable RVA 0x3059BA8, 124 virtual functions
				class BGSDefaultObjectManager :
					public TESForm,                                        // 000
					public BSTSingletonImplicit<BGSDefaultObjectManager>   // 028
				{
				public:
					virtual ~BGSDefaultObjectManager() = default;

				public:
					std::uint8_t unk28[0xC60]; // 28
					std::uint8_t unkC88[0xDF0]; // C88
				};
				static_assert(sizeof(BGSDefaultObjectManager) == 0x1A78);
			}
		}
	}
}
