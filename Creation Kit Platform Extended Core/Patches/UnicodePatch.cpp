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
		uintptr_t pointer_UnicodePatch_sub1 = 0;
		uintptr_t pointer_UnicodePatch_sub2 = 0;

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
			// Initial mode
			// Initially, the original state must be set
			EditorAPI::ConvertorString.SetMode(EditorAPI::BGSConvertorString::MODE_ANSI);

			auto verPatch = lpRelocationDatabaseItem->Version();
			if (verPatch == 1)
			{
				// Intercepting the receipt of a string
				*(uintptr_t*)&EditorAPI::BGSLocalizedString_OldGetStrProc =
					voltek::detours_function_class_jump(_RELDATA_ADDR(0), &EditorAPI::BGSLocalizedString::GetStr);

				// Also delete it message "You must close all Dialoge Boxes",
				// which has problems with programs that work with multiple monitors.
				lpRelocator->DetourCall(_RELDATA_RAV(1), (uintptr_t)&HKBeginPluginSave);
				// I don't quite understand the meaning of calling SetCursor in this function, which deals with saving
				// But we'll make the call in hook.
				lpRelocator->DetourCall(_RELDATA_RAV(2), (uintptr_t)&HKEndPluginSave);
				// Deleting book checks, spam in the log is excessive
				lpRelocator->PatchNop(_RELDATA_RAV(3), 5);

				// In the "Data" dialog box, the "author" and "description" controls are independent, 
				// and I'm forced to make a trap for WinAPI calls
				lpRelocator->DetourCall(_RELDATA_RAV(4), (uintptr_t)&HKSetDlgItemTextA);
				lpRelocator->DetourCall(_RELDATA_RAV(5), (uintptr_t)&HKSetDlgItemTextA);
				lpRelocator->DetourCall(_RELDATA_RAV(6), (uintptr_t)&HKSetDlgItemTextA);
				lpRelocator->DetourCall(_RELDATA_RAV(7), (uintptr_t)&HKSendDlgItemMessageA);
				lpRelocator->DetourCall(_RELDATA_RAV(8), (uintptr_t)&HKSendDlgItemMessageA);
				lpRelocator->DetourCall(_RELDATA_RAV(9), (uintptr_t)&HKSendDlgItemMessageA);
				lpRelocator->DetourCall(_RELDATA_RAV(10), (uintptr_t)&HKSendDlgItemMessageA);

				return true;
			}
			else if (verPatch == 2)
			{
				// Also delete it message "You must close all Dialoge Boxes",
				// which has problems with programs that work with multiple monitors.
				lpRelocator->DetourCall(_RELDATA_RAV(0), (uintptr_t)&HKBeginPluginSave);
				// I don't quite understand the meaning of calling SetCursor in this function, which deals with saving
				// But we'll make the call in hook.
				lpRelocator->DetourCall(_RELDATA_RAV(1), (uintptr_t)&HKEndPluginSave);

				// Introduction of string processing.
				auto lOff = (uintptr_t)_RELDATA_RAV(3);

				lpRelocator->Patch(_RELDATA_RAV(2), { 0x22 });
				lpRelocator->Patch(lOff, { 0x51, 0x48, 0x89, 0xC1 });
				lpRelocator->DetourCall(_RELDATA_RAV(4), (uintptr_t)&EditorAPI::BGSLocalizedString::GetStr2);
				lpRelocator->Patch(_RELDATA_RAV(5), { 0x59, 0xC3, 0x31, 0xC0, 0xC3 });
				lpRelocator->PatchJump(_RELDATA_RAV(6), lOff);

				// In the "Data" dialog box, the "author" and "description" controls are independent, 
				// and I'm forced to make a trap for WinAPI calls
				lpRelocator->DetourCall(_RELDATA_RAV(7), (uintptr_t)&HKSetDlgItemTextA);
				lpRelocator->DetourCall(_RELDATA_RAV(8), (uintptr_t)&HKSetDlgItemTextA);
				lpRelocator->DetourCall(_RELDATA_RAV(9), (uintptr_t)&HKSendDlgItemMessageA);
				lpRelocator->DetourCall(_RELDATA_RAV(10), (uintptr_t)&HKSendDlgItemMessageA);
				lpRelocator->DetourCall(_RELDATA_RAV(11), (uintptr_t)&HKSendDlgItemMessageA);
				lpRelocator->DetourCall(_RELDATA_RAV(12), (uintptr_t)&HKSendDlgItemMessageA);

				//
				// Cut check spelling window
				//

				ScopeRelocator text;

				for (uint32_t i = 13; i < lpRelocationDatabaseItem->Count(); i++)
					lpRelocator->PatchNop(_RELDATA_RAV(i), 5);

				return true;
			}
			else if (verPatch == 3)
			{
				// Hook save plugin
				pointer_UnicodePatch_sub1 =
					voltek::detours_function_class_jump(_RELDATA_ADDR(0), (uintptr_t)&HKPluginSaveSF);
				
				// Introduction of string processing.
				pointer_UnicodePatch_sub2 =
					voltek::detours_function_class_jump(_RELDATA_ADDR(1), (uintptr_t)&HKGetStringLocalizeSF);

				// In the "Data" dialog box, the "author" and "description" controls are independent, 
				// and I'm forced to make a trap for WinAPI calls
				lpRelocator->DetourCall(_RELDATA_RAV(2), (uintptr_t)&HKSetDlgItemTextA);
				lpRelocator->DetourCall(_RELDATA_RAV(3), (uintptr_t)&HKSetDlgItemTextA);
				lpRelocator->DetourCall(_RELDATA_RAV(4), (uintptr_t)&HKSendDlgItemMessageA);
				lpRelocator->DetourCall(_RELDATA_RAV(5), (uintptr_t)&HKSendDlgItemMessageA);
				lpRelocator->DetourCall(_RELDATA_RAV(6), (uintptr_t)&HKSendDlgItemMessageA);
				lpRelocator->DetourCall(_RELDATA_RAV(7), (uintptr_t)&HKSendDlgItemMessageA);

				//
				// Cut check spelling window
				//

				ScopeRelocator text;

				for (uint32_t i = 8; i < lpRelocationDatabaseItem->Count(); i++)
					lpRelocator->PatchNop(_RELDATA_RAV(i), 5);
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

		BOOL UnicodePatch::HKSetDlgItemTextA(HWND hDlg, int nIDDlgItem, LPCSTR lpString) {
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

		LRESULT UnicodePatch::HKSendDlgItemMessageA(HWND hDlg, INT nIDDlgItem, UINT Msg, 
			WPARAM wParam, LPARAM lParam) {
			if ((Msg != WM_GETTEXT) && (Msg != WM_GETTEXTLENGTH))
				MsgTextDef:
				return SendDlgItemMessageA(hDlg, nIDDlgItem, Msg, wParam, lParam);

			HWND hCtrlWnd;

			switch (nIDDlgItem)
			{
			case 1024:
			case 1025:
			{
				hCtrlWnd = GetDlgItem(hDlg, nIDDlgItem);
				INT32 maxlen = GetWindowTextLengthA(hCtrlWnd);

				if (maxlen <= 0)
					goto MsgTextDef;

				maxlen <<= 1;
				auto ansi_str = std::make_unique<char[]>((size_t)maxlen + 1);
				GetWindowTextA(hCtrlWnd, ansi_str.get(), maxlen);
				ansi_str.get()[maxlen] = 0;

				String utf8_str = Conversion::AnsiToUtf8(ansi_str.get());

				if (Msg == WM_GETTEXT)
					strncpy((LPSTR)(lParam), utf8_str.c_str(), wParam);

				return utf8_str.length() + 1;
			}
			default:
				goto MsgTextDef;
			}
		}

		void UnicodePatch::HKPluginSaveSF(__int64 unk01, __int64 unk02, __int64 unk03, __int64 unk04)
		{
			EditorAPI::ConvertorString.SetMode(EditorAPI::BGSConvertorString::MODE_UTF8);
			fastCall<void>(pointer_UnicodePatch_sub1, unk01, unk02, unk03, unk04);
			EditorAPI::ConvertorString.SetMode(EditorAPI::BGSConvertorString::MODE_ANSI);
		}

		const char* UnicodePatch::HKGetStringLocalizeSF(__int64 lstring)
		{
			// Fixed:
			// First problem, Bethesda removed the check for an null pointer

			if (!lstring) return nullptr;
			auto utf8_string = fastCall<const char*>(pointer_UnicodePatch_sub2, lstring);
			if (!utf8_string) return nullptr;

			// Second problem is that all the strings have become BGSLocalizedString,
			// Even the path, system strings, and script properties.
			// It can't have localization, goonies, "<ID=" it also don't have.

			// Extremely surprisingly, I don't get CTD when I translate a string,
			// but I return the same memory address, however, this is only applicable when converting WinCP.
			if (EditorAPI::ConvertorString.GetMode() == EditorAPI::BGSConvertorString::MODE_UTF8)
				return EditorAPI::BGSLocalizedString::GetStr2(utf8_string);
			// All APIs are implemented as ANSI. In this regard, utf-8 must be converted to the specified ANSI.
			else
			{
				EditorAPI::BGSLocalizedString::GetStr2(utf8_string);
				return utf8_string;
			}
		}
	}
}