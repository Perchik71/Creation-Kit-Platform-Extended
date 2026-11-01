// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/TESProduceForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0xF8, vtable RVA 0x3101FD8
				class TESObjectTREE :
					public TESBoundObject,		// 00
					public TESModel,			// 50
					public TESFullName,			// 80
					public TESProduceForm		// 98
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftTree;

					virtual ~TESObjectTREE() = default;

				/* 070 */ void InitializeData() noexcept(true) override;
				/* 080 */ bool Load(TESFile* File) noexcept(true) override;
				/* 098 */ void Save() noexcept(true) override;
				/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
				/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
				/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
				/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
				/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					float unkB8[12];		// B8 - twelve floats set by InitializeData (CNAM; individual roles not resolved)
					void* unkE8;			// E8
					std::uint32_t unkF0;	// F0 - 2 on construction
					std::uint32_t padF4;	// F4
				};
				static_assert(offsetof(TESObjectTREE, unkE8) == 0xE8);
				static_assert(sizeof(TESObjectTREE) == 0xF8);
			}
		}
	}
}