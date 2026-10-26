// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <richedit.h>
#include <cstring>
#include <string>
#include <EditorAPI/IDs.h>
#include <EditorAPI/B/BSString.h>
#include <CKPE.Utils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.Misc.h>

#define UI_MODELDATA_PREVIEW_CONTROL		2175
#define UI_MODELDATA_GROUNDPLANE_CHECKBOX	52011

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			namespace
			{
				std::string s_ArchiveOutput;

				using TBSStringAppend = EditorAPI::BSString*(*)(EditorAPI::BSString*, const char*);
				using TModelDataDlgProc = INT_PTR(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);
				using TGetPreviewControl = void*(*)(HWND, std::uint32_t);
				using TToggleGroundPlane = void(*)(void*);

				TBSStringAppend OldBSStringAppend = nullptr;
				TModelDataDlgProc OldModelDataDlgProc = nullptr;
				TGetPreviewControl GetPreviewControl = nullptr;
				TToggleGroundPlane ToggleGroundPlane = nullptr;

				EditorAPI::BSString* AppendArchiveOutput(EditorAPI::BSString* Str, const char* Chunk) noexcept(true)
				{
					if (Str && Chunk)
					{
						if (Str->IsEmpty())
							s_ArchiveOutput.clear();
						s_ArchiveOutput.append(Chunk);

						if (OldBSStringAppend && (static_cast<std::size_t>(Str->Length()) + std::strlen(Chunk) < 0xFF00))
							return OldBSStringAppend(Str, Chunk);
					}
					return Str;
				}

				BOOL SetErrorDlgRichEditText(HWND hRichEdit, LPCSTR lpString) noexcept(true)
				{
					SendMessageA(hRichEdit, EM_EXLIMITTEXT, 0, 0x7FFFFFFF);
					if (!s_ArchiveOutput.empty() && lpString && lpString[0] != '\0' &&
						std::strncmp(s_ArchiveOutput.c_str(), lpString, 64) == 0)
					{
						const BOOL res = SetWindowTextA(hRichEdit, s_ArchiveOutput.c_str());
						s_ArchiveOutput.clear();
						return res;
					}
					return SetWindowTextA(hRichEdit, lpString);
				}

				void SyncGroundPlaneCheck(HWND hDlg) noexcept(true)
				{
					const auto valueAddr = EditorAPI::IDs::Functions::bPreviewShowLandPlane.Address();
					if (HWND hCheck = GetDlgItem(hDlg, UI_MODELDATA_GROUNDPLANE_CHECKBOX); hCheck && valueAddr)
					{
						const bool showGround = *reinterpret_cast<const bool*>(valueAddr);
						SendMessageA(hCheck, BM_SETCHECK, showGround ? BST_CHECKED : BST_UNCHECKED, 0);
					}
				}

				INT_PTR CALLBACK HKModelDataDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept(true)
				{
					if (uMsg == WM_INITDIALOG)
					{
						const INT_PTR res = OldModelDataDlgProc(hDlg, uMsg, wParam, lParam);

						HWND hPreview = GetDlgItem(hDlg, UI_MODELDATA_PREVIEW_CONTROL);
						HWND hOk = GetDlgItem(hDlg, IDOK);
						if (hPreview && hOk && !GetDlgItem(hDlg, UI_MODELDATA_GROUNDPLANE_CHECKBOX))
						{
							RECT rcPreview{}, rcOk{};
							GetWindowRect(hPreview, &rcPreview);
							GetWindowRect(hOk, &rcOk);
							MapWindowPoints(HWND_DESKTOP, hDlg, reinterpret_cast<LPPOINT>(&rcPreview), 2);
							MapWindowPoints(HWND_DESKTOP, hDlg, reinterpret_cast<LPPOINT>(&rcOk), 2);

							HWND hCheck = CreateWindowExA(0, WC_BUTTONA, "Ground Plane",
								WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_AUTOCHECKBOX,
								rcPreview.left, rcOk.top, 110, rcOk.bottom - rcOk.top,
								hDlg, reinterpret_cast<HMENU>(static_cast<std::uintptr_t>(UI_MODELDATA_GROUNDPLANE_CHECKBOX)),
								GetModuleHandleA(nullptr), nullptr);

							if (hCheck)
							{
								SendMessageA(hCheck, WM_SETFONT, SendMessageA(hDlg, WM_GETFONT, 0, 0), TRUE);
								SyncGroundPlaneCheck(hDlg);
							}
						}

						return res;
					}

					if (uMsg == WM_COMMAND && LOWORD(wParam) == UI_MODELDATA_GROUNDPLANE_CHECKBOX && HIWORD(wParam) == BN_CLICKED)
					{
						if (GetPreviewControl && ToggleGroundPlane)
						{
							if (void* preview = GetPreviewControl(hDlg, UI_MODELDATA_PREVIEW_CONTROL))
							{
								ToggleGroundPlane(preview);
								CKPE::vtbl_call<void>(0x28, preview, 0.0f, false);
							}
						}
						SyncGroundPlaneCheck(hDlg);
						return TRUE;
					}

					const INT_PTR res = OldModelDataDlgProc(hDlg, uMsg, wParam, lParam);

					if (uMsg == WM_NOTIFY || uMsg == WM_KEYDOWN)
						SyncGroundPlaneCheck(hDlg);

					return res;
				}
			}
			Misc::Misc() : Common::Patch()
			{
				SetName("Misc");
			}

			bool Misc::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* Misc::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool Misc::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> Misc::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool Misc::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool Misc::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// Send text to 4 part StatusBar (Game cam: .....)
				Relocation(EditorAPI::IDs::Functions::sub_56E940, Offset{ 0x23A, 0x22F }).Write({ 0x02 });
				// CheckMenuItem is called, however, it always gets zero, but eight is written on top,
				// which is equal to MFS_CHECKED.
				Relocation(EditorAPI::IDs::Functions::MainWindow_WndProc, Offset{ 0xD3D, 0xF9C, 0xFC1, 0xFC3 }).WriteFill(NOP, 6);
				// CheckMenuItem is called, however, it always gets MFS_CHECKED.
				Relocation(EditorAPI::IDs::Functions::sub_558F90, Offset{ 0x53, 0x51 }).WriteFill(NOP, 6);
				// Fix repeat CheckMenuItem is called
				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
					Relocation(EditorAPI::IDs::Functions::legacy_Misc_643475, 0xFF1).Write({ 0x20, 0x63 });
				else
					Relocation(EditorAPI::IDs::Functions::MainWindow_WndProc_2, Offset{ 0x125A, 0x125A, 0x127F, 0x1281 }).Write({ 0xE9, 0xAE, 0x66, 0x00, 0x00, 0x90 });
				// ret skip Warnings window
				Relocation(EditorAPI::IDs::Functions::sub_632EA0).Write({ 0xC3, 0xCC, 0xCC, 0xCC, 0xCC });

				// Fixed crash when closing the window, occurs extremely rarely
				// CONTEXT:  (.ecxr)
				//  rax = 000002db7f90f850 rbx = 000000f780cfea40 rcx = 000002db7f90ed10
				//	rdx = 000000f780cfea40 rsi = 000002db299aa800 rdi = 000002db7f90ed10
				//	rip = 00007ff77f381176 rsp = 000000f780cfe9f0 rbp = 000000f780cfeb80
				//	r8 = 0000000000000000  r9 = 0000000000000016 r10 = 000000000000000b
				//	r11 = 000000f780cfea18 r12 = 0000000000000001 r13 = 00000000001506ec
				//	r14 = 0000000000000000 r15 = 0000000000000000
				//	iopl = 0         nv up ei pl zr na po nc
				//	cs = 0033  ss = 002b  ds = 002b  es = 002b  fs = 0053  gs = 002b             efl = 00010246
				//	CreationKit + 0x2511176:
				//  00007ff7`7f381176 ff9090010000    call    qword ptr[rax + 190h] ds:000002db`7f90f9e0=bd8a591342b6b52e
				Relocation(EditorAPI::IDs::Functions::sub_24BDB90, 0x56).WriteCall(&sub);

				// BSString caps at 64K; keep full Archive2 log for the error dialog
				*(std::uintptr_t*)&OldBSStringAppend =
					Relocation(EditorAPI::IDs::Functions::BGSArchiveBrowser_AppendOutput_1, 0x7).WriteJump(&AppendArchiveOutput);
				Relocation(EditorAPI::IDs::Functions::BGSArchiveBrowser_AppendOutput_2, 0x7).WriteJump(&AppendArchiveOutput);
				Relocation(EditorAPI::IDs::Functions::BGSArchiveBrowser_AppendOutput_3, 0x7).WriteJump(&AppendArchiveOutput);
				auto errDlgRel = Relocation(EditorAPI::IDs::Functions::BGSErrorDlg_OnInitDialog, 0x77);
				errDlgRel.WriteFill(NOP, 6);
				errDlgRel.WriteCall(&SetErrorDlgRichEditText);

				GetPreviewControl = Relocation<TGetPreviewControl>(EditorAPI::IDs::Functions::DialogExtraPreviewControl_Get).Get();
				ToggleGroundPlane = Relocation<TToggleGroundPlane>(EditorAPI::IDs::Functions::TESPreviewControl_ToggleGroundPlane).Get();
				*(std::uintptr_t*)&OldModelDataDlgProc =
					Relocation(EditorAPI::IDs::Functions::ModelData_DlgProc).WriteJump(&HKModelDataDlgProc);

				return true;
			}

			void Misc::sub(std::uint64_t a1, std::uint64_t a2, std::uint64_t a3) noexcept(true)
			{
				if (a1)
				{
					// In the fallout, everything is in the higher addresses, let's check the match with the base address. 

					auto f = (std::uintptr_t)(*(std::uintptr_t*)a1);
					if (((std::uintptr_t)GetModuleHandleA(nullptr) & 0xffffffff00000000) ==
						(*((std::uintptr_t*)(f + 0x190)) & 0xffffffff00000000))
						CKPE::vtbl_call<void>((std::size_t)0x190, (void*)a1, a2, 0);
				}
			}
		}
	}
}