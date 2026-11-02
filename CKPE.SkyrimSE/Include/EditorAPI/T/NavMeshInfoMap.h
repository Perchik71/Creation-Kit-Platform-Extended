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
				// 1.7.99.0: size 0x110, vtable RVA 0x315EB00
				class NavMeshInfoMap :
					public TESForm	// 00
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftNavMeshInfo;

					virtual ~NavMeshInfoMap() = default;

					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 090 */ bool LoadEdit(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 0E0 */ void InitItemImpl() noexcept(true) override;
					/* 0C8 */ [[nodiscard]] bool IsBeforeForm(TESForm* Rhs) const noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 280 */ bool Revert() noexcept(true) override;
					/* 288 */ void OnFormIDsRemapped(void* FormIDMap) noexcept(true) override;
				public:
					std::uint8_t unk28[0xE8];	// 28 - data, layout not resolved
				};
				static_assert(offsetof(NavMeshInfoMap, unk28) == 0x28);
				static_assert(sizeof(NavMeshInfoMap) == 0x110);
			}
		}
	}
}