// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESIcon.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x50, vtable RVA 0x305DDD8, 124 virtual functions
				class BGSMenuIcon :
					public TESForm,  // 000
					public TESIcon   // 028
				{
				public:
					virtual ~BGSMenuIcon() = default;

					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftMenuIcon;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSMenuIcon[0];
				};
				static_assert(sizeof(BGSMenuIcon) == 0x50);
			}
		}
	}
}
