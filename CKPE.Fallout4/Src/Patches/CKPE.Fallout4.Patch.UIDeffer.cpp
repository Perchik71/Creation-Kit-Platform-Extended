// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Asserts.h>
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
				using namespace Common;

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

						Relocation(Target).WriteJump(hook->getCode());
					}
				};

				Relocation(ID{ 641924, 1752948 }).WriteJump(&Common::EditorUI::ComboBoxInsertItemDeferred);
				Relocation(ID{ 641951, 1463745 }).WriteJump(&Common::EditorUI::ListViewInsertItemDeferred);

				Relocation(ID{ 636062, 1582406 }, Offset{ 0x71, 0x7A }).WriteCall(&sub1);
				Relocation(ID{ 643131, 1820720 }, Offset{ 0x2DDB, 0x2FBF }).WriteCall(&sub2);
				Relocation(ID{ 206118, 1450783 }, Offset{ 0xAA, 0xAE }).WriteCall(&sub3);

				pointer_UIDeffer_sub1 = Relocation(ID{ 636530, 1675545 }).Address();
				pointer_UIDeffer_sub2 = Relocation(ID{ 645724, 1445476 }).Address();
				pointer_UIDeffer_sub3 = Relocation(ID{ 643197, 1973863 }).Address();


				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
				{
					static constexpr std::uint32_t formIteratorIds[] =
					{ 598079, 616663, 11246, 587937, 425909, 590847, 15116, 293664, 595577, 374116, 370287 };

					for (auto id : formIteratorIds)
					{
						auto addr = Relocation(ID{ id });
						if (!addr)
							return false;

						FormIteratorHook::Generate(addr.Address());
					}
				}
				else
				{
					static constexpr std::uint32_t formIteratorIds[] =
					{ 1390677, 1850037, 1828090, 587937, 1852614, 1795871, 374116, 15116, 1676014 };

					for (auto id : formIteratorIds)
					{
						auto addr = Relocation(ID{ id });
						if (!addr)
							return false;

						FormIteratorHook::Generate(addr.Address());
					}
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