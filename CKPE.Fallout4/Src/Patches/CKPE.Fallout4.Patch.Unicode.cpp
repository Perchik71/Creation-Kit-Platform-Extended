// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.Unicode.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			static StringCache UnicodeStringCache;
			static ConvertorString UnicodeConvertorString;

			class BGSLocalizedString
			{
			public:
				static const char* GetStr(const char* str) noexcept(true)
				{
					return UnicodeConvertorString.Convert(str);
				}
			};

			bool ConvertorString::IsValid(const char* s) const noexcept(true)
			{
				return ((s != nullptr) && (s != LPSTR_TEXTCALLBACKA) && (strlen(s) > 0));
			}

			const char* ConvertorString::Utf8ToWinCP(const char* src) noexcept(true)
			{
				// Ansi verification is necessary because there are a lot of strings, especially short and system strings. 
				// The debug file without this option was more than 70 mb, compared to 2604 kb.
				// Translation of fallout4.esm has become significantly faster.

				if ((src == pre) || !IsValid(src) || !StringUtils::IsUtf8(src))
					return src;

				std::string wincp_str = StringUtils::Utf8ToWinCP(src);

				// utf-8 takes up more memory than ansi, so I can use active memory
				pre = src;
				strcpy(const_cast<char*>(pre), wincp_str.c_str());

				return pre;
			}

			const char* ConvertorString::WinCPToUtf8(const char* src) noexcept(true)
			{
				// Not all strings are translated during loading and remain in Utf-8. 
				// They are loaded after opening the dialog. As an example "Description".

				if (!IsValid(src) || StringUtils::IsUtf8(src))
					return src;

				// in the Creation Kit code, the request to return a string occurs twice in a row.
				if ((pre == src) && (UnicodeStringCache.Size() > 0))
					return UnicodeStringCache.Last();

				// convert
				std::string utf8_str = StringUtils::WinCPToUtf8(src);

				// Unicode initially takes up more memory than ansi. 
				// Therefore, a heap is created that will store memory for the duration of saving.
				// Lists work quickly on adding items.
				UnicodeStringCache.Push(utf8_str);

				// pointer to the memory of the contents of the last line, it is of course unique
				pre = src;
				return UnicodeStringCache.Last();
			}

			void ConvertorString::SetMode(Mode m) noexcept(true)
			{
				mode = m;
				UnicodeStringCache.Clear();
			}

			const char* ConvertorString::Convert(const char* s) noexcept(true)
			{
				// Back to utf-8 (temporarily)
				if (mode == ConvertorString::MODE_UTF8)
					return WinCPToUtf8(s);
				// All APIs are implemented as ANSI. In this regard, utf-8 must be converted to the specified ANSI.
				else
					return Utf8ToWinCP(s);
			}

			Unicode::Unicode() : Common::Patch()
			{
				SetName("Unicode");
			}

			bool Unicode::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* Unicode::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bUnicode";
			}

			bool Unicode::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> Unicode::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool Unicode::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool Unicode::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 2)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Initial mode
				// Initially, the original state must be set
				UnicodeConvertorString.SetMode(ConvertorString::MODE_ANSI);

				// Also delete it message "You must close all Dialoge Boxes",
				// which has problems with programs that work with multiple monitors.
				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&BeginPluginSave);
				// I don't quite understand the meaning of calling SetCursor in this function, which deals with saving
				// But we'll make the call in hook.
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&EndPluginSave);

				// Introduction of string processing.
				auto lOff = (std::uintptr_t)__CKPE_OFFSET(3);

				SafeWrite::Write(__CKPE_OFFSET(2), { 0x22 });
				SafeWrite::Write(lOff, { 0x51, 0x48, 0x89, 0xC1 });
				Detours::DetourCall(__CKPE_OFFSET(4), (std::uintptr_t)&BGSLocalizedString::GetStr);
				SafeWrite::Write(__CKPE_OFFSET(5), { 0x59, 0xC3, 0x31, 0xC0, 0xC3 });
				SafeWrite::WriteJump(__CKPE_OFFSET(6), lOff);

				// In the "Data" dialog box, the "author" and "description" controls are independent, 
				// and I'm forced to make a trap for WinAPI calls
				Detours::DetourCall(__CKPE_OFFSET(7), (std::uintptr_t)&HKSetDlgItemTextA);
				Detours::DetourCall(__CKPE_OFFSET(8), (std::uintptr_t)&HKSetDlgItemTextA);
				Detours::DetourCall(__CKPE_OFFSET(9), (std::uintptr_t)&HKSendDlgItemMessageA);
				Detours::DetourCall(__CKPE_OFFSET(10), (std::uintptr_t)&HKSendDlgItemMessageA);
				Detours::DetourCall(__CKPE_OFFSET(11), (std::uintptr_t)&HKSendDlgItemMessageA);
				Detours::DetourCall(__CKPE_OFFSET(12), (std::uintptr_t)&HKSendDlgItemMessageA);

				//
				// Cut check spelling window
				//

				// Cutting a lot is faster this way
				auto stext = interface->GetApplication()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				for (std::uint32_t i = 13; i < db->GetCount(); i++)
					text.WriteNop(__CKPE_OFFSET(i), 5);

				return true;
			}

			bool Unicode::BeginPluginSave() noexcept(true)
			{
				UnicodeConvertorString.SetMode(ConvertorString::MODE_UTF8);
				return TRUE;
			}

			void Unicode::EndPluginSave(std::int64_t hCursor) noexcept(true)
			{
				UnicodeConvertorString.SetMode(ConvertorString::MODE_ANSI);
				SetCursor((HCURSOR)hCursor);
			}

			bool Unicode::HKSetDlgItemTextA(std::int64_t hDlg, std::int32_t nIDDlgItem,
				const char* lpString) noexcept(true)
			{
				switch (nIDDlgItem)
				{
				case 1024:
				case 1025:
				{
					if (!lpString || !StringUtils::IsUtf8(lpString))
						goto SetTextDef;

					auto wincp_str = StringUtils::Utf8ToWinCP(lpString);
					return SetDlgItemTextA((HWND)hDlg, nIDDlgItem, wincp_str.c_str());
				}
				default:
				SetTextDef:
					return SetDlgItemTextA((HWND)hDlg, nIDDlgItem, lpString);
				}
			}
			
			std::int64_t Unicode::HKSendDlgItemMessageA(std::int64_t hDlg, std::int32_t nIDDlgItem, std::uint32_t Msg,
				std::int64_t wParam, std::int64_t lParam) noexcept(true)
			{
				if ((Msg != WM_GETTEXT) && (Msg != WM_GETTEXTLENGTH))
					MsgTextDef:
				return SendDlgItemMessageA((HWND)hDlg, nIDDlgItem, Msg, wParam, lParam);

				HWND hCtrlWnd;

				switch (nIDDlgItem)
				{
				case 1024:
				case 1025:
				{
					hCtrlWnd = GetDlgItem((HWND)hDlg, nIDDlgItem);
					INT32 maxlen = GetWindowTextLengthA(hCtrlWnd);

					if (maxlen <= 0)
						goto MsgTextDef;

					maxlen <<= 1;
					auto ansi_str = std::make_unique<char[]>((size_t)maxlen + 1);
					GetWindowTextA(hCtrlWnd, ansi_str.get(), maxlen);
					ansi_str.get()[maxlen] = 0;

					auto utf8_str = StringUtils::WinCPToUtf8(ansi_str.get());

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
}