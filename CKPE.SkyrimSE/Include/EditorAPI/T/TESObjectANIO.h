// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESModelTextureSwap.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x78, vtable RVA 0x30FB710 (ANIO)
				class TESObjectANIO :
					public TESForm,					// 00
					public TESModelTextureSwap		// 28
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftANIO;

					virtual ~TESObjectANIO() = default;

					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					void* unk70;	// 70 - BNAM string, initialised to an empty string (type unresolved)
				};
				static_assert(offsetof(TESObjectANIO, unk70) == 0x70);
				static_assert(sizeof(TESObjectANIO) == 0x78);
			}
		}
	}
}