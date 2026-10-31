// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include <cstdint>
#include <CKPE.Common.Relocation.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// Dialog helpers.
			namespace DialogHelpers
			{
				// Shows the open / save file dialog (GetSaveFileNameA based) and returns whether a file was chosen.
				// Parameters: parent, base path, filter, title, default extension, hook proc, unknown, unknown flag, result buffer, buffer size,
				// directory, unknown, unknown flag.
				// CK 1.11.246.0 0x1407138A0.
				inline bool ShowFileDialog(HWND Parent, const char* BasePath, const char* Filter, const char* Title, const char* Extension,
					void* HookProc, std::int64_t Unk7, bool Unk8, char* Buffer, std::uint32_t BufferSize, const char* Directory, void* Unk12,
					bool Unk13 = false) noexcept(true)
				{
					using func_t = decltype(&ShowFileDialog);
					static Common::Relocation<func_t> func{ Common::ID{ 645762, 1356949 } };
					return func(Parent, BasePath, Filter, Title, Extension, HookProc, Unk7, Unk8, Buffer, BufferSize, Directory, Unk12, Unk13);
				}

				// Sets the text of a dialog item to a float formatted with the given number of decimals ("%.{Precision}f").
				// CK 1.11.246.0 0x140641ED0.
				inline void SetItemFloat(HWND Dialog, std::int32_t ItemID, float Value, std::int32_t Precision) noexcept(true)
				{
					using func_t = decltype(&SetItemFloat);
					static Common::Relocation<func_t> func{ Common::ID{ 577351, 1588570 } };
					return func(Dialog, ItemID, Value, Precision);
				}

				// Encounter zone dialog helper called with the zone, the dialog and the level.
				// CK 1.11.246.0 0x140CAC840.
				inline void EncounterZoneSub(void* Class, void* Dialog, std::uint8_t Level) noexcept(true)
				{
					using func_t = decltype(&EncounterZoneSub);
					static Common::Relocation<func_t> func{ Common::ID{ 501084 } };
					return func(Class, Dialog, Level);
				}
			}
		}
	}
}
