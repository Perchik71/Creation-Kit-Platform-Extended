// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <cstddef>
#include <Windows.h>
#include <EditorAPI/B/BSTList.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 1.11.246.0: size 0x30, vtable RVA 0x30BB4C0, 6 virtual functions (no destructor). The weather list form component
			// (TESClimate WLST, the two lists of TESRegionDataWeather): a list view of weather type / chance / global
			// with a combo box and an add button in the owning dialog.
			class TESWeatherList
			{
			public:
				// Dialog message handler: Id is the control id, Result the output of a handled message
				virtual bool HandleDialogMessage(HWND Dialog, std::int32_t Message, std::uint16_t Id, std::int64_t Param, std::uint64_t* Result); // 00
				// Checks that the dialog has the four controls of the component
				virtual bool ValidateDialog(HWND Dialog); // 01
				// Adds the "Weather Type", "Chance" and "Global" columns to the list view
				virtual void InitDialog(HWND Dialog); // 02
				// Fills the combo box and the list view from the entries
				virtual void FillDialog(HWND Dialog); // 03
				// Rebuilds the entries from the list view
				virtual void ReadDialog(HWND Dialog); // 04
				// Clears the list view and disables the controls
				virtual void ClearDialog(HWND Dialog); // 05

			public:
				BSSimpleList<void*> entries; // 08
				std::uint32_t listControlID; // 18
				std::uint32_t comboBoxControlID; // 1C
				std::uint32_t addButtonControlID; // 20
				std::uint32_t popupControlID; // 24
				std::uint64_t unk28; // 28
			};
			static_assert(offsetof(TESWeatherList, listControlID) == 0x18);
			static_assert(sizeof(TESWeatherList) == 0x30);
		}
	}
}
