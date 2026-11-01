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
				// 1.7.99.0: size 0x60, vtable RVA 0x30927E0
				class TESRegion :
					public TESForm	// 00
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftRegion;

					virtual ~TESRegion() = default;

					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 288 */ void OnFormIDsRemapped(void* FormIDMap) noexcept(true) override;
				public:
					std::uint8_t unk28[0x38];	// 28 - data, layout not resolved
				};
				static_assert(offsetof(TESRegion, unk28) == 0x28);
				static_assert(sizeof(TESRegion) == 0x60);
			}
		}
	}
}