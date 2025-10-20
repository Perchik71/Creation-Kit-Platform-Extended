// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Asserts.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.EditorUI.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.UIDeffer.h>

#include <xbyak.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			std::uintptr_t pointer_UIDeffer_sub1 = 0;
			std::uintptr_t pointer_UIDeffer_sub2 = 0;
			std::uintptr_t pointer_UIDeffer_sub3 = 0;

			UIDeffer::UIDeffer() : Common::Patch()
			{
				SetName("UI Deffer");
			}

			bool UIDeffer::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* UIDeffer::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool UIDeffer::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> UIDeffer::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool UIDeffer::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool UIDeffer::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				// Deferred dialog loading (batched UI updates)

				class FormIteratorHook : public Xbyak::CodeGenerator 
				{
				public:
					FormIteratorHook(std::uintptr_t Callback) : Xbyak::CodeGenerator()
					{
						// Allocate enough space for 8 copied parameters (4 regs)
						mov(ptr[rsp + 0x8], rbx);
						mov(ptr[rsp + 0x10], rbp);
						mov(ptr[rsp + 0x18], rsi);
						push(rdi);
						sub(rsp, 0x50);

						mov(rbp, rcx);
						mov(rsi, rdx);
						mov(rdi, r8);
						mov(rbx, r9);
						mov(rax, (std::uintptr_t)&Common::EditorUI::Hook::HKBeginUIDefer);
						call(rax);
						mov(rcx, rbp);
						mov(rdx, rsi);
						mov(r8, rdi);
						mov(r9, rbx);

						for (std::uint32_t i = 0; i < 6; i++)
						{
							mov(rax, ptr[rsp + (0xA8 - i * 0x8)]);
							mov(ptr[rsp + (0x48 - i * 0x8)], rax);
						}

						mov(rax, Callback); // Callback (X params)
						call(rax);
						mov(rbx, rax);

						mov(rax, (std::uintptr_t)&Common::EditorUI::Hook::HKEndUIDefer);
						call(rax);
						mov(rax, rbx);
						add(rsp, 0x50);
						pop(rdi);
						mov(rsi, ptr[rsp + 0x18]);
						mov(rbp, ptr[rsp + 0x10]);
						mov(rbx, ptr[rsp + 0x8]);
						ret();
					}

					static VOID Generate(std::uintptr_t Target)
					{
						// Manually resolve the called function address. NOTE: This is leaking memory on purpose. It's a mess.
						CKPE_ASSERT(*(std::uint8_t*)Target == 0xE9);

						auto destination = Target + *(std::int32_t*)(Target + 1) + 5;
						auto hook = new FormIteratorHook(destination);

						Detours::DetourJump(Target, (std::uintptr_t)hook->getCode());
					}
				};

				if (verPatch == 1)
				{
					// List view
					FormIteratorHook::Generate(__CKPE_OFFSET(0));
					FormIteratorHook::Generate(__CKPE_OFFSET(1));
					FormIteratorHook::Generate(__CKPE_OFFSET(2));
					FormIteratorHook::Generate(__CKPE_OFFSET(3));
					FormIteratorHook::Generate(__CKPE_OFFSET(4));
					FormIteratorHook::Generate(__CKPE_OFFSET(5));

					// Combo box
					FormIteratorHook::Generate(__CKPE_OFFSET(6));
					FormIteratorHook::Generate(__CKPE_OFFSET(7));
					FormIteratorHook::Generate(__CKPE_OFFSET(8));
					FormIteratorHook::Generate(__CKPE_OFFSET(9));
					FormIteratorHook::Generate(__CKPE_OFFSET(10));

					Detours::DetourJump(__CKPE_OFFSET(11),
						(std::uintptr_t)&Common::EditorUI::ComboBoxInsertItemDeferred);
					Detours::DetourJump(__CKPE_OFFSET(12),
						(std::uintptr_t)&Common::EditorUI::ListViewInsertItemDeferred);
					Detours::DetourCall(__CKPE_OFFSET(13), (std::uintptr_t)&sub1);
					Detours::DetourCall(__CKPE_OFFSET(14), (std::uintptr_t)&sub2);
					Detours::DetourCall(__CKPE_OFFSET(15), (std::uintptr_t)&sub3);

					pointer_UIDeffer_sub1 = __CKPE_OFFSET(16);
					pointer_UIDeffer_sub2 = __CKPE_OFFSET(17);
					pointer_UIDeffer_sub3 = __CKPE_OFFSET(18);

					return true;
				}
				else if (verPatch == 2)
				{
					Detours::DetourJump(__CKPE_OFFSET(0),
						(std::uintptr_t)&Common::EditorUI::ComboBoxInsertItemDeferred);
					Detours::DetourJump(__CKPE_OFFSET(1),
						(std::uintptr_t)&Common::EditorUI::ListViewInsertItemDeferred);
					Detours::DetourCall(__CKPE_OFFSET(2), (std::uintptr_t)&sub1);
					Detours::DetourCall(__CKPE_OFFSET(3), (std::uintptr_t)&sub2);
					Detours::DetourCall(__CKPE_OFFSET(4), (std::uintptr_t)&sub3);

					pointer_UIDeffer_sub1 = __CKPE_OFFSET(5);
					pointer_UIDeffer_sub2 = __CKPE_OFFSET(6);
					pointer_UIDeffer_sub3 = __CKPE_OFFSET(7);

					for (std::uint32_t nId = 8; nId < db->GetCount(); nId++)
						FormIteratorHook::Generate(__CKPE_OFFSET(nId));

					return true;
				}

				return true;
			}

			void UIDeffer::sub1(void* Thisptr, void* ControlHandle, std::int64_t Unknown) noexcept(true)
			{
				Common::EditorUI::Hook::HKSendMessageA(ControlHandle, WM_SETREDRAW, FALSE, 0);
				fast_call<void>(pointer_UIDeffer_sub1, Thisptr, ControlHandle, Unknown);
				Common::EditorUI::Hook::HKSendMessageA(ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow((HWND)ControlHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			void UIDeffer::sub2(void* Thisptr, void* ControlHandle, std::int64_t Unknown) noexcept(true)
			{
				Common::EditorUI::Hook::HKSendMessageA(ControlHandle, WM_SETREDRAW, FALSE, 0);
				fast_call<void>(pointer_UIDeffer_sub2, Thisptr, ControlHandle, Unknown);
				Common::EditorUI::Hook::HKSendMessageA(ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow((HWND)ControlHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			void UIDeffer::sub3(void* Thisptr, void** ControlHandle) noexcept(true)
			{
				Common::EditorUI::Hook::HKSendMessageA(*ControlHandle, WM_SETREDRAW, FALSE, 0);
				fast_call<void>(pointer_UIDeffer_sub3, Thisptr, ControlHandle);
				Common::EditorUI::Hook::HKSendMessageA(*ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow(*((HWND*)ControlHandle), NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}
		}
	}
}