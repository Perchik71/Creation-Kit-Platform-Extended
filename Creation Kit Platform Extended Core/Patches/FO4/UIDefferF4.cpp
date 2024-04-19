// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "UIDefferF4.h"
#include "Editor API/EditorUI.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			uintptr_t pointer_UIDeffer_sub1 = 0;
			uintptr_t pointer_UIDeffer_sub2 = 0;
			uintptr_t pointer_UIDeffer_sub3 = 0;

			UIDefferPatch::UIDefferPatch() : Module(GlobalEnginePtr)
			{}

			bool UIDefferPatch::HasOption() const
			{
				return false;
			}

			bool UIDefferPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* UIDefferPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* UIDefferPatch::GetName() const
			{
				return "UI Deffer";
			}

			bool UIDefferPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> UIDefferPatch::GetDependencies() const
			{
				return {};
			}

			bool UIDefferPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool UIDefferPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Deferred dialog loading (batched UI updates)

					class FormIteratorHook : public Xbyak::CodeGenerator {
					public:
						FormIteratorHook(uintptr_t Callback) : Xbyak::CodeGenerator()
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
							mov(rax, (uintptr_t)&EditorAPI::EditorUI::HKBeginUIDefer);
							call(rax);
							mov(rcx, rbp);
							mov(rdx, rsi);
							mov(r8, rdi);
							mov(r9, rbx);

							for (uint32_t i = 0; i < 6; i++)
							{
								mov(rax, ptr[rsp + (0xA8 - i * 0x8)]);
								mov(ptr[rsp + (0x48 - i * 0x8)], rax);
							}

							mov(rax, Callback); // Callback (X params)
							call(rax);
							mov(rbx, rax);

							mov(rax, (uintptr_t)&EditorAPI::EditorUI::HKEndUIDefer);
							call(rax);
							mov(rax, rbx);
							add(rsp, 0x50);
							pop(rdi);
							mov(rsi, ptr[rsp + 0x18]);
							mov(rbp, ptr[rsp + 0x10]);
							mov(rbx, ptr[rsp + 0x8]);
							ret();
						}

						static VOID Generate(uintptr_t Target) {
							// Manually resolve the called function address. NOTE: This is leaking memory on purpose. It's a mess.
							Assert(*(uint8_t*)Target == 0xE9);

							auto destination = Target + *(int32_t*)(Target + 1) + 5;
							auto hook = new FormIteratorHook(destination);

							voltek::detours_jump(Target, (uintptr_t)hook->getCode());
						}
					};

					// List view
					FormIteratorHook::Generate(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)));
					FormIteratorHook::Generate(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1)));
					FormIteratorHook::Generate(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2)));
					FormIteratorHook::Generate(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(3)));
					FormIteratorHook::Generate(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(4)));
					FormIteratorHook::Generate(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(5)));

					// Combo box
					FormIteratorHook::Generate(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(6)));
					FormIteratorHook::Generate(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(7)));
					FormIteratorHook::Generate(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(8)));
					FormIteratorHook::Generate(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(9)));
					FormIteratorHook::Generate(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(10)));

					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(11),
						(uintptr_t)&EditorAPI::EditorUI::ComboBoxInsertItemDeferred);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(12),
						(uintptr_t)&EditorAPI::EditorUI::ListViewInsertItemDeferred);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(13), (uintptr_t)&sub1);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(14), (uintptr_t)&sub2);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(15), (uintptr_t)&sub3);

					pointer_UIDeffer_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(16));
					pointer_UIDeffer_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(17));
					pointer_UIDeffer_sub3 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(18));

					return true;
				}

				return false;
			}

			bool UIDefferPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void UIDefferPatch::sub1(void* Thisptr, HWND ControlHandle, int64_t Unknown)
			{
				SendMessageA(ControlHandle, WM_SETREDRAW, FALSE, 0);
				fastCall<VOID, LPVOID, HWND, INT64>(pointer_UIDeffer_sub1, Thisptr, ControlHandle, Unknown);
				SendMessageA(ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow(ControlHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			void UIDefferPatch::sub2(void* Thisptr, HWND ControlHandle, int64_t Unknown)
			{
				SendMessageA(ControlHandle, WM_SETREDRAW, FALSE, 0);
				fastCall<VOID, LPVOID, HWND, INT64>(pointer_UIDeffer_sub2, Thisptr, ControlHandle, Unknown);
				SendMessageA(ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow(ControlHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}

			void UIDefferPatch::sub3(void* Thisptr, HWND* ControlHandle)
			{
				SendMessageA(*ControlHandle, WM_SETREDRAW, FALSE, 0);
				fastCall<VOID, LPVOID, HWND*>(pointer_UIDeffer_sub3, Thisptr, ControlHandle);
				SendMessageA(*ControlHandle, WM_SETREDRAW, TRUE, 0);
				RedrawWindow(*ControlHandle, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_NOCHILDREN);
			}
		}
	}
}