// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.FileUtils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/BSResourceLooseFiles.h>
#include <Patches/CKPE.Fallout4.Patch.BSResourceLooseFiles.h>

#include <xbyak.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			BSResourceLooseFiles::BSResourceLooseFiles() : Common::Patch()
			{
				SetName("BSResource Loose Files");
			}

			bool BSResourceLooseFiles::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* BSResourceLooseFiles::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool BSResourceLooseFiles::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> BSResourceLooseFiles::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool BSResourceLooseFiles::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			// I will add initialization to fill in the new fields with data
			class LooseFileStreamHook : public Xbyak::CodeGenerator
			{
			public:
				LooseFileStreamHook() : Xbyak::CodeGenerator()
				{
					push(r11);
					push(rax);
					mov(rcx, ptr[r11 + 0x18]);
					mov(rdx, rbp);
					sub(rsp, 0x40);
					mov(rax, (uintptr_t)&EditorAPI::BSResource::LooseFileStream::CreateInstance);
					call(rax);
					add(rsp, 0x40);
					pop(rax);
					pop(r11);
					mov(rbx, ptr[r11 + 0x10]);
					mov(rbp, ptr[r11 + 0x18]);
					mov(rsi, ptr[r11 + 0x20]);
					mov(rsp, r11);
					pop(rdi);
					ret();
				}

				static void Generate(uintptr_t Target)
				{
					auto hook = new LooseFileStreamHook();
					Detours::DetourJump(Target, (uintptr_t)hook->getCode());
				}
			};

			bool BSResourceLooseFiles::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// Set new size class 0x1XX to 0x180
				Relocation(ID{ 193501, 1992729 }, Offset{ 0x101, 0x101, 0x102 }).Write({ 0x80 });

				LooseFileStreamHook::Generate(Relocation(ID{ 767841, 1456433 }, 0x107).Address());
				Relocation(ID{ 712081, 2009874 }).WriteJump(&sub);

				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
				{
					// As I understand it, CK evaluates the amount of useful data with the file size,
					// since I will now adjust the 64-bit size, sacrificing checking for NullPtr.
					//
					// mov rax, qword ptr ds:[rcx+0x170]
					// jmp -> cmp rsi, rax
					Relocation(ID(552578), 0x484).Write({ 0x48, 0x8B, 0x81, 0x70, 0x01, 0x00, 0x00, 0xEB, 0x1D });
					
					// Ignoring the correctness check is not useful
					Relocation(ID(255793), 0x160).Write(JMP);
					Relocation(ID(647276), 0x14D).Write(JMP);
					Relocation(ID(530259), 0x176).Write(JMP);
					Relocation(ID(530994), 0xD2).Write(JMP);
					Relocation(ID(648213)).Write(RET);
					Relocation(ID(745724)).Write(RET);
				}
				else if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_982_3)
				{
					// As I understand it, CK evaluates the amount of useful data with the file size,
					// since I will now adjust the 64-bit size, sacrificing checking for NullPtr.
					//
					// mov rax, qword ptr ds:[rcx+0x170]
					// jmp -> cmp rbx, rax
					Relocation(ID(552578), 0x3F2).Write({ 0x48, 0x8B, 0x81, 0x70, 0x01, 0x00, 0x00, 0xEB, 0x19 });

					// Ignoring the correctness check is not useful
					Relocation(ID(255793), 0x15A).Write(JMP);
					Relocation(ID(1501752), 0x15E).Write(JMP);
					Relocation(ID(530259), 0x180).Write(JMP);
					Relocation(ID(530994), 0xD2).Write(JMP);
				}
				else
				{
					// Ignoring the correctness check is not useful
					Relocation(ID(2203563), 0x159).Write(JMP);
					Relocation(ID(2129124), 0x162).Write(JMP);
					Relocation(ID(2203564), 0x19B).Write(JMP);
					Relocation(ID(2147855), 0x5D).Write(JMP);
				}

				return true;
			}

			bool BSResourceLooseFiles::sub(const char* fileName, std::uint64_t& fileSize) noexcept(true)
			{
				fileSize = FileUtils::GetFileSize(fileName);
				return fileSize > 0;
			}
		}
	}
}