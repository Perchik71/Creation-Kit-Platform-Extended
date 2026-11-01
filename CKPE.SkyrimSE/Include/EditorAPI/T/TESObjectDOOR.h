// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESModelTextureSwap.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSOpenCloseForm.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x118, vtable RVA 0x30FFCF8 (DOOR)
				class TESObjectDOOR :
					public TESBoundObject,				// 00
					public TESFullName,					// 50
					public TESModelTextureSwap,			// 68
					public BGSDestructibleObjectForm,	// B0
					public BGSOpenCloseForm				// C0
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftDoor;

					virtual ~TESObjectDOOR() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t padC8[8];		// C8
					void* unkD0;				// D0 - zeroed by InitializeData (SNAM/ANAM/BNAM sound forms, order not resolved)
					void* unkD8;				// D8 - zeroed by InitializeData
					void* unkE0;				// E0 - zeroed by InitializeData
					std::uint8_t flags;			// E8 - FNAM, zeroed by InitializeData
					std::uint8_t padE9[0x2F];	// E9
				};
				static_assert(offsetof(TESObjectDOOR, unkD0) == 0xD0);
				static_assert(offsetof(TESObjectDOOR, flags) == 0xE8);
				static_assert(sizeof(TESObjectDOOR) == 0x118);
			}
		}
	}
}