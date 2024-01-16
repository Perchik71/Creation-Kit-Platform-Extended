// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/BGStringLocalize.h"
#include "UnicodePatch.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		UnicodePatch::UnicodePatch() : Module(GlobalEnginePtr)
		{}

		bool UnicodePatch::HasOption() const
		{
			return true;
		}

		bool UnicodePatch::HasCanRuntimeDisabled() const
		{
			return false;
		}

		const char* UnicodePatch::GetOptionName() const
		{
			return "CreationKit:bUnicode";
		}

		const char* UnicodePatch::GetName() const
		{
			return "Unicode";
		}

		bool UnicodePatch::HasDependencies() const
		{
			return false;
		}

		Array<String> UnicodePatch::GetDependencies() const
		{
			return {};
		}

		bool UnicodePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
			const char* lpcstrPlatformRuntimeVersion) const
		{
			return true;
		}

		bool UnicodePatch::Activate(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			auto verPatch = lpRelocationDatabaseItem->Version();
			if (verPatch == 1)
			{
				// Initial mode
				// Initially, the original state must be set
				EditorAPI::ConvertorString.SetMode(EditorAPI::BGSConvertorString::MODE_ANSI);

				// Intercepting the receipt of a string
				*(uintptr_t*)&EditorAPI::BGSLocalizedString_OldGetStrProc =
					Detours::X64::DetourFunctionClass(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)),
						&EditorAPI::BGSLocalizedString::GetStr);

				// Also delete it message "You must close all Dialoge Boxes",
				// which has problems with programs that work with multiple monitors.
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1), (uintptr_t)&HKBeginPluginSave);
				// I don't quite understand the meaning of calling SetCursor in this function, which deals with saving
				// But we'll make the call in hook.
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(2), (uintptr_t)&HKEndPluginSave);
				// Deleting book checks, spam in the log is excessive
				lpRelocator->PatchNop(lpRelocationDatabaseItem->At(3), 5);

				// In the "Data" dialog box, the "author" and "description" controls are independent, 
				// and I'm forced to make a trap for WinAPI calls
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(4), (uintptr_t)&HKSetDlgItemTextA);
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(5), (uintptr_t)&HKSetDlgItemTextA);
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(6), (uintptr_t)&HKSetDlgItemTextA);
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(7), (uintptr_t)&HKSendDlgItemMessageA);
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(8), (uintptr_t)&HKSendDlgItemMessageA);
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(9), (uintptr_t)&HKSendDlgItemMessageA);
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(10), (uintptr_t)&HKSendDlgItemMessageA);

				return true;
			}
			else if (verPatch == 2)
			{
				// Initial mode
				// Initially, the original state must be set
				EditorAPI::ConvertorString.SetMode(EditorAPI::BGSConvertorString::MODE_ANSI);

				// Also delete it message "You must close all Dialoge Boxes",
				// which has problems with programs that work with multiple monitors.
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&HKBeginPluginSave);
				// I don't quite understand the meaning of calling SetCursor in this function, which deals with saving
				// But we'll make the call in hook.
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1), (uintptr_t)&HKEndPluginSave);

				// Introduction of string processing.
				lpRelocator->Patch(lpRelocationDatabaseItem->At(2), { 0x22 });
				lpRelocator->Patch(lpRelocationDatabaseItem->At(3), { 0x51, 0x48, 0x89, 0xC1 });
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(4), (uintptr_t)&EditorAPI::BGSLocalizedString::GetStr2);
				lpRelocator->Patch(lpRelocationDatabaseItem->At(5), { 0x59, 0xC3, 0x31, 0xC0, 0xC3 });
				lpRelocator->Patch(lpRelocationDatabaseItem->At(6), { 0xE9, 0x33, 0xB9, 0xFF, 0xFF });

				// In the "Data" dialog box, the "author" and "description" controls are independent, 
				// and I'm forced to make a trap for WinAPI calls
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(7), (uintptr_t)&HKSetDlgItemTextA);
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(8), (uintptr_t)&HKSetDlgItemTextA);
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(9), (uintptr_t)&HKSendDlgItemMessageA);
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(10), (uintptr_t)&HKSendDlgItemMessageA);
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(11), (uintptr_t)&HKSendDlgItemMessageA);
				lpRelocator->DetourCall(lpRelocationDatabaseItem->At(12), (uintptr_t)&HKSendDlgItemMessageA);

				//
				// Cut check spelling window
				//

				ScopeRelocator text;

				for (uint32_t i = 13; i < lpRelocationDatabaseItem->Count(); i++)
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(i), 5);

				return true;
			}
			
			return false;
		}

		bool UnicodePatch::Shutdown(const Relocator* lpRelocator,
			const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			return false;
		}

		bool UnicodePatch::HKBeginPluginSave() 
		{
			EditorAPI::ConvertorString.SetMode(EditorAPI::BGSConvertorString::MODE_UTF8);
			return TRUE;
		}

		void UnicodePatch::HKEndPluginSave(HCURSOR hCursor) 
		{
			EditorAPI::ConvertorString.SetMode(EditorAPI::BGSConvertorString::MODE_ANSI);
			SetCursor(hCursor);
		}

		bool UnicodePatch::HKSetDlgItemTextA(HWND hDlg, int nIDDlgItem, LPCSTR lpString) {
			switch (nIDDlgItem)
			{
			case 1024:
			case 1025:
			{
				if (!lpString || !Conversion::IsUtf8Valid(lpString))
					goto SetTextDef;

				String wincp_str = Conversion::Utf8ToAnsi(lpString);
				return SetDlgItemTextA(hDlg, nIDDlgItem, wincp_str.c_str());
			}
			default:
			SetTextDef:
				return SetDlgItemTextA(hDlg, nIDDlgItem, lpString);
			}
		}

		bool UnicodePatch::HKSendDlgItemMessageA(HWND hDlg, INT nIDDlgItem, UINT Msg, 
			WPARAM wParam, LPARAM lParam) {
			if (Msg != WM_GETTEXT && Msg != WM_GETTEXTLENGTH)
				MsgTextDef:
				return SendDlgItemMessageA(hDlg, nIDDlgItem, Msg, wParam, lParam);

			HWND hCtrlWnd;

			switch (nIDDlgItem)
			{
			case 1024:
			case 1025:
			{
				hCtrlWnd = GetDlgItem(hDlg, nIDDlgItem);
				INT32 maxlen = GetWindowTextLengthA(hCtrlWnd) << 2;

				if (maxlen <= 0)
					goto MsgTextDef;

				String ansi_str;
				ansi_str.resize(maxlen);
				ansi_str.resize(GetWindowTextA(hCtrlWnd, &ansi_str[0], maxlen));

				if (!Conversion::IsUtf8Valid(ansi_str))
					goto MsgTextDef;

				String utf8_str = Conversion::AnsiToUtf8(ansi_str);

				if (Msg == WM_GETTEXT)
					strncpy((LPSTR)(lParam), utf8_str.c_str(), wParam);

				return utf8_str.length() + 1;
			}
			default:
				goto MsgTextDef;
			}
		}
	}
}