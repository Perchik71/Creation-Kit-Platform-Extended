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
				// 1.7.99.0: size 0x58, vtable RVA 0x3001FC8
				class BGSVolumetricLighting :
					public TESForm	// 00
				{
				public:
					virtual ~BGSVolumetricLighting() = default;

					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 0E0 */ void InitItemImpl() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t unk28[0x30];	// 28 - data, layout not resolved
				};
				static_assert(offsetof(BGSVolumetricLighting, unk28) == 0x28);
				static_assert(sizeof(BGSVolumetricLighting) == 0x58);
			}
		}
	}
}