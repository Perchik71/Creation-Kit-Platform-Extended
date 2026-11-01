// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESModelTextureSwap.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x150, vtable RVA 0x309BE08
				class BGSHeadPart :
					public TESForm,	// 00
					public TESFullName,	// 28
					public TESModelTextureSwap	// 40
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftHeadPart;

					virtual ~BGSHeadPart() = default;
				public:
					std::uint8_t unk88[0xC8];	// 88 - data, layout not resolved
				};
				static_assert(offsetof(BGSHeadPart, unk88) == 0x88);
				static_assert(sizeof(BGSHeadPart) == 0x150);
			}
		}
	}
}