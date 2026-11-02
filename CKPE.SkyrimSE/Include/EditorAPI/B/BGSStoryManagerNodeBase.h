// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BGSStoryManagerTreeForm.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x40, vtable RVA 0x306F9E0. Members read at 28/30 (8 bytes), 38 (XNAM) and 3C (DNAM); roles not resolved.
				class BGSStoryManagerNodeBase : public BGSStoryManagerTreeForm
				{
				public:
					constexpr static Common::ID RTTI = IDs::RTTI::BGSStoryManagerNodeBase;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSStoryManagerNodeBase[0];

					virtual ~BGSStoryManagerNodeBase() = default;
				public:
					std::uint8_t unk28[0x18];	// 28
				};
				static_assert(sizeof(BGSStoryManagerNodeBase) == 0x40);
			}
		}
	}
}