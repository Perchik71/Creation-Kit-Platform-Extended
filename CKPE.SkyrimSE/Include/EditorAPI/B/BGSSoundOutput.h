// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BSISoundOutputModel.h>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x48, vtable RVA 0x30CEAD0
				class BGSSoundOutput :
					public TESForm,	// 00
					public BSISoundOutputModel	// 28
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftSoundOutput;

					virtual ~BGSSoundOutput() = default;

					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C8 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint32_t nam1;	// 30 - NAM1
					std::uint32_t mnam;	// 34 - MNAM
					void* anam;	// 38 - ANAM
					void* onam;	// 40 - ONAM
				};
				static_assert(offsetof(BGSSoundOutput, nam1) == 0x30);
				static_assert(offsetof(BGSSoundOutput, mnam) == 0x34);
				static_assert(offsetof(BGSSoundOutput, anam) == 0x38);
				static_assert(offsetof(BGSSoundOutput, onam) == 0x40);
				static_assert(sizeof(BGSSoundOutput) == 0x48);
			}
		}
	}
}