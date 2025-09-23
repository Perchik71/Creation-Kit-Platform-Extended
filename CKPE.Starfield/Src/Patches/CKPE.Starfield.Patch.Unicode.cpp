// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <CKPE.Utils.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.Unicode.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			static std::uintptr_t pointer_UnicodePatch_sub1 = 0;
			static std::uintptr_t pointer_UnicodePatch_sub2 = 0;

			static StringCache UnicodeStringCache;
			static ConvertorString UnicodeConvertorString;

			typedef char*(*TUnicodeSub)(const char*);
			static TUnicodeSub UnicodeSub;

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
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_STARFIELD_LAST;
			}

			bool Unicode::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 3)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Initial mode
				// Initially, the original state must be set
				UnicodeConvertorString.SetMode(ConvertorString::MODE_ANSI);

				// Hook save plugin
				pointer_UnicodePatch_sub1 = Detours::DetourClassJump(__CKPE_OFFSET(0), (std::uintptr_t)&HKPluginSave);
				// Introduction of string processing.
				pointer_UnicodePatch_sub2 = Detours::DetourClassJump(__CKPE_OFFSET(1), (std::uintptr_t)&HKGetStringLocalize);

				// In the "Data" dialog box, the "author" and "description" controls are independent, 
				// and I'm forced to make a trap for WinAPI calls
				Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&HKSetDlgItemTextA);
				Detours::DetourCall(__CKPE_OFFSET(3), (std::uintptr_t)&HKSetDlgItemTextA);
				Detours::DetourCall(__CKPE_OFFSET(4), (std::uintptr_t)&HKSendDlgItemMessageA);
				Detours::DetourCall(__CKPE_OFFSET(5), (std::uintptr_t)&HKSendDlgItemMessageA);
				Detours::DetourCall(__CKPE_OFFSET(6), (std::uintptr_t)&HKSendDlgItemMessageA);
				Detours::DetourCall(__CKPE_OFFSET(7), (std::uintptr_t)&HKSendDlgItemMessageA);

				//
				// Cut check spelling window
				//

				// Cutting a lot is faster this way
				auto stext = interface->GetApplication()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				for (std::uint32_t i = 8; i < db->GetCount(); i++)
					text.WriteNop(__CKPE_OFFSET(i), 5);

				return true;
			}

			void Unicode::HKPluginSave(std::int64_t unk01, std::int64_t unk02, std::int64_t unk03,
				std::int64_t unk04) noexcept(true)
			{
				UnicodeConvertorString.SetMode(ConvertorString::MODE_UTF8);
				fast_call<void>(pointer_UnicodePatch_sub1, unk01, unk02, unk03, unk04);
				UnicodeConvertorString.SetMode(ConvertorString::MODE_ANSI);
			}

			const char* Unicode::HKGetStringLocalize(std::int64_t lstring) noexcept(true)
			{
				// Fixed:
				// First problem, Bethesda removed the check for an null pointer

				if (!lstring) return nullptr;
				auto utf8_string = fast_call<const char*>(pointer_UnicodePatch_sub2, lstring);
				if (!utf8_string) return nullptr;

				// Second problem is that all the strings have become BGSLocalizedString,
				// Even the path, system strings, and script properties.
				// It can't have localization, goonies, "<ID=" it also don't have.

				// Extremely surprisingly, I don't get CTD when I translate a string,
				// but I return the same memory address, however, this is only applicable when converting WinCP.
				if (UnicodeConvertorString.GetMode() == ConvertorString::MODE_UTF8)
					return BGSLocalizedString::GetStr(utf8_string);
				// All APIs are implemented as ANSI. In this regard, utf-8 must be converted to the specified ANSI.
				else
				{
					BGSLocalizedString::GetStr(utf8_string);
					return utf8_string;
				}
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