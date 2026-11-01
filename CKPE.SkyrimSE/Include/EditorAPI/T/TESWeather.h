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
				// 1.7.99.0: size 0xC00, vtable RVA 0x3120FB0
				class TESWeather :
					public TESForm	// 00
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftWeather;

					virtual ~TESWeather() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C8 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t pad28[0x500];	// 28
					std::uint8_t rnam[0x20];	// 528 - RNAM
					std::uint8_t qnam[0x20];	// 548 - QNAM
					std::uint8_t pnam[0x200];	// 568 - PNAM
					std::uint8_t jnam[0x200];	// 768 - JNAM
					std::uint32_t nam1;	// 968 - NAM1
					std::uint8_t data[0x14];	// 96C - DATA
					std::uint8_t fnam[0x20];	// 980 - FNAM
					std::uint8_t nam0[0x138];	// 9A0 - NAM0
					std::uint32_t lnam;	// AD8 - LNAM
					std::uint8_t padADC[0xA4];	// ADC
					std::uint8_t unkB80[0x30];	// B80 - MODL/MODT
					void* gnam;	// BB0 - GNAM
					std::uint8_t padBB8[0x48];	// BB8
				};
				static_assert(offsetof(TESWeather, pad28) == 0x28);
				static_assert(offsetof(TESWeather, rnam) == 0x528);
				static_assert(offsetof(TESWeather, qnam) == 0x548);
				static_assert(offsetof(TESWeather, pnam) == 0x568);
				static_assert(offsetof(TESWeather, jnam) == 0x768);
				static_assert(offsetof(TESWeather, nam1) == 0x968);
				static_assert(offsetof(TESWeather, data) == 0x96C);
				static_assert(offsetof(TESWeather, fnam) == 0x980);
				static_assert(offsetof(TESWeather, nam0) == 0x9A0);
				static_assert(offsetof(TESWeather, lnam) == 0xAD8);
				static_assert(offsetof(TESWeather, unkB80) == 0xB80);
				static_assert(offsetof(TESWeather, gnam) == 0xBB0);
				static_assert(sizeof(TESWeather) == 0xC00);
			}
		}
	}
}