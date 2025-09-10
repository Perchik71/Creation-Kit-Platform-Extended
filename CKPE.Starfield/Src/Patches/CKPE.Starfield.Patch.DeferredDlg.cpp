// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Starfield.VersionLists.h>
#include <Patches/CKPE.Starfield.Patch.DeferredDlg.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace Patch
		{
			std::uintptr_t pointer_DeferredDlg_sub1 = 0;
			std::uintptr_t pointer_DeferredDlg_sub2 = 0;
			std::uintptr_t pointer_DeferredDlg_sub3 = 0;

			//std::uintptr_t pointer_DeferredDlg_ll_sub1 = 0;

			DeferredDlg::DeferredDlg() : Common::Patch()
			{
				SetName("UI Deffer");
			}

			bool DeferredDlg::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* DeferredDlg::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool DeferredDlg::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> DeferredDlg::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool DeferredDlg::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_STARFIELD_1_14_70_0;
			}

			bool DeferredDlg::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&sub1);
				Detours::DetourCall(__CKPE_OFFSET(1), (std::uintptr_t)&sub2);
				Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&sub3);
				pointer_DeferredDlg_sub1 = __CKPE_OFFSET(3);
				pointer_DeferredDlg_sub2 = __CKPE_OFFSET(4);
				pointer_DeferredDlg_sub3 = __CKPE_OFFSET(5);

				// Filling out dialogs is now performed by each form independently and this will no longer work.

				//Detours::DetourJump(__CKPE_OFFSET(6), (std::uintptr_t)&Common::EditorUI::ComboBoxInsertItemDeferred);
				//Detours::DetourJump(__CKPE_OFFSET(7), (std::uintptr_t)&Common::EditorUI::ListViewInsertItemDeferred);

				//pointer_DeferredDlg_ll_sub1 = Detours::DetourClassJump(__CKPE_OFFSET(8), (std::uintptr_t)&deferredCombobox);
				

				return true;
			}

			void DeferredDlg::sub1(void* Thisptr, void* ControlHandle, std::int64_t Unknown1, std::int64_t Unknown2) noexcept(true)
			{
				SendMessageA((HWND)ControlHandle, WM_SETREDRAW, FALSE, 0);
				fast_call<void>(pointer_DeferredDlg_sub1, Thisptr, ControlHandle, Unknown1, Unknown2);
				SendMessageA((HWND)ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow((HWND)ControlHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			void DeferredDlg::sub2(void* Thisptr, void* ControlHandle, std::int64_t Unknown1, std::int64_t Unknown2) noexcept(true)
			{
				SendMessageA((HWND)ControlHandle, WM_SETREDRAW, FALSE, 0);
				fast_call<void>(pointer_DeferredDlg_sub2, Thisptr, ControlHandle, Unknown1, Unknown2);
				SendMessageA((HWND)ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow((HWND)ControlHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			void DeferredDlg::sub3(void* Thisptr, void** ControlHandle) noexcept(true)
			{
				SendMessageA((HWND)*ControlHandle, WM_SETREDRAW, FALSE, 0);
				fast_call<void>(pointer_DeferredDlg_sub3, Thisptr, ControlHandle);
				SendMessageA((HWND)*ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow((HWND)*ControlHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

		/*	void DeferredDlg::deferredCombobox(std::int64_t a1, std::int64_t a2, std::int64_t a3, std::int64_t a4,
				std::int64_t a5, std::int64_t a6, std::int64_t a7, std::int64_t a8) noexcept(true)
			{
				Common::EditorUI::GetSingleton()->BeginUIDefer();
				fast_call<void>(pointer_DeferredDlg_ll_sub1, a1, a2, a3, a4, a5, a6, a7, a8);
				Common::EditorUI::GetSingleton()->EndUIDefer();
			}*/
		}
	}
}