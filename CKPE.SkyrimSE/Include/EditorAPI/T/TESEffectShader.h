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
				// 1.7.99.0: size 0x2A0, vtable RVA 0x30F1D00
				class TESEffectShader :
					public TESForm	// 00
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftEffectShader;

					virtual ~TESEffectShader() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 220 */ void GetObjectWindowTreePath(void* Path) const noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C8 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t pad28[0x1A0];	// 28
					std::uint8_t icon[0x28];	// 1C8 - ICON
					std::uint8_t ico2[0x28];	// 1F0 - ICO2
					std::uint8_t nam7[0x28];	// 218 - NAM7
					std::uint8_t nam8[0x28];	// 240 - NAM8
					std::uint8_t nam9[0x38];	// 268 - NAM9
				};
				static_assert(offsetof(TESEffectShader, pad28) == 0x28);
				static_assert(offsetof(TESEffectShader, icon) == 0x1C8);
				static_assert(offsetof(TESEffectShader, ico2) == 0x1F0);
				static_assert(offsetof(TESEffectShader, nam7) == 0x218);
				static_assert(offsetof(TESEffectShader, nam8) == 0x240);
				static_assert(offsetof(TESEffectShader, nam9) == 0x268);
				static_assert(sizeof(TESEffectShader) == 0x2A0);
			}
		}
	}
}