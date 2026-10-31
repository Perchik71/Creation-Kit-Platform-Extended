// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/M/MagicItem.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x300, vtable RVA 0x30BF688, 188 virtual functions
				class EnchantmentItem :
					public MagicItem   // 000
				{
				public:
					virtual ~EnchantmentItem() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

				public:
					std::uint32_t unk2D0; // 2D0
					std::uint32_t unk2D4; // 2D4
					std::int32_t castingType; // 2D8
					std::uint32_t unk2DC; // 2DC
					std::int32_t delivery; // 2E0
					std::int32_t spellType; // 2E4
					float chargeTime; // 2E8
					std::uint32_t unk2EC; // 2EC
					std::uint64_t unk2F0; // 2F0
					std::uint64_t unk2F8; // 2F8
				};
				static_assert(offsetof(EnchantmentItem, castingType) == 0x2D8);
				static_assert(offsetof(EnchantmentItem, delivery) == 0x2E0);
				static_assert(offsetof(EnchantmentItem, chargeTime) == 0x2E8);
				static_assert(sizeof(EnchantmentItem) == 0x300);
			}
		}
	}
}
