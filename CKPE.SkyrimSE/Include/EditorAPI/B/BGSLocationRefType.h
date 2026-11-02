// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BGSKeyword.h>
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
				class BGSLocationRefType : public BGSKeyword
				{
				public:
					constexpr static Common::ID RTTI = IDs::RTTI::BGSLocationRefType;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSLocationRefType[0];

					constexpr static std::uint8_t TYPE_ID = ftLocationRef;

					virtual ~BGSLocationRefType() = default;
				};
				static_assert(sizeof(BGSLocationRefType) == 0x30);
			}
		}
	}
}
