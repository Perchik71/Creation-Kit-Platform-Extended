// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <CKPE.StringUtils.h>
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
				using namespace Common;

				auto interface = CKPE::Common::Interface::GetSingleton();

				// Initial mode
				// Initially, the original state must be set
				UnicodeConvertorString.SetMode(ConvertorString::MODE_ANSI);

				const auto target1 = ID{ 613852, 1995935 };
				const auto target2 = ID{ 650803, 1852383 };
				const auto target3 = ID{ 643293, 1664109 };
				const auto target4 = ID{ 643289, 1753394 };

				// Also delete it message "You must close all Dialoge Boxes",
				// which has problems with programs that work with multiple monitors.
				Relocation(target1, 0x10).WriteCall(&BeginPluginSave);
				// I don't quite understand the meaning of calling SetCursor in this function, which deals with saving
				// But we'll make the call in hook.
				Relocation(target1, 0xB1).WriteCall(&EndPluginSave);

				// Introduction of string processing. This trampoline is hand-spliced into the tail
				// padding of a small, unrelated function (id 1809106) right after its own epilogue,
				// then BSStringPool::Entry::GetStringC (id 1597058) gets its final "ret" replaced with
				// a jump into it. Verified byte-for-byte identical padding layout between 137/240.
				auto lOff = Relocation(target2, 0x1F).Address();

				Relocation(target2, 0x7).Write({ 0x22 });
				Relocation(target2, 0x1F).Write({ 0x51, 0x48, 0x89, 0xC1 });
				Relocation(target2, 0x23).WriteCall(&BGSLocalizedString::GetStr);
				Relocation(target2, 0x28).Write({ 0x59, 0xC3, 0x31, 0xC0, 0xC3 });
				SafeWrite::WriteJump(Relocation(ID{ 158625, 1627008 }, 0x17).Address(), lOff);

				// In the "Data" dialog box, the "author" and "description" controls are independent,
				// and I'm forced to make a trap for WinAPI calls
				Relocation(target3, Offset{ 0xD1, 0xD5 }).WriteCall(&HKSetDlgItemTextA);
				Relocation(target3, Offset{ 0xF0, 0xF4 }).WriteCall(&HKSetDlgItemTextA);
				Relocation(target4, 0x45).WriteCall(&HKSendDlgItemMessageA);
				Relocation(target4, 0x7D).WriteCall(&HKSendDlgItemMessageA);
				Relocation(target4, 0xB7).WriteCall(&HKSendDlgItemMessageA);
				Relocation(target4, 0xEF).WriteCall(&HKSendDlgItemMessageA);

				//
				// Cut check book characters
				//
				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
					Relocation(ID(245849), 0x7A).Write({0xE9, 0x96, 0x00, 0x00, 0x00, 0x90});
				else
					Relocation(ID(1804550), 0xD5).Write({0xE9, 0x94, 0x00, 0x00, 0x00, 0x90});

				//
				// Cut check spelling window
				//

				// Cutting a lot is faster this way
				auto stext = interface->GetApplication()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				text.WriteNop(Relocation(ID{ 243708, 1596835 }, Offset{ 0x11E, 0x1EA }).Address(), 5);
				text.WriteNop(Relocation(ID{ 407774, 1988972 }, Offset{ 0x67E, 0x63A }).Address(), 5);
				text.WriteNop(Relocation(ID{ 664457, 1894783 }, Offset{ 0x185, 0x165 }).Address(), 5);
				text.WriteNop(Relocation(ID{ 668460, 1645713 }, Offset{ 0xA4F, 0xBBA }).Address(), 5);
				text.WriteNop(Relocation(ID{ 668460, 1645713 }, Offset{ 0x137F, 0x157B }).Address(), 5);
				text.WriteNop(Relocation(ID{ 668460, 1645713 }, Offset{ 0x1569, 0x190E }).Address(), 5);
				text.WriteNop(Relocation(ID{ 668671, 1869554 }, Offset{ 0x3FE, 0x3C3 }).Address(), 5);
				text.WriteNop(Relocation(ID{ 670625, 1774972 }, Offset{ 0x583, 0x547 }).Address(), 5);

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