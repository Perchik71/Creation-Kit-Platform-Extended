// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.FileUtils.h>
#include <EditorAPI/IDs.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/B/BSResourceLooseFiles.h>
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

				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
				{
					// Set new size class 0x1XX to 0x180
					Relocation(EditorAPI::IDs::Functions::BSResource__LooseFileStream_Create, Offset{ 0x101, 0x101, 0x102 }).Write({ 0x80 });
				}
				else
				{
					// Since 1.10.982.3 the stream is created with a second base: the pointer handed out (and the `this` of every
					// consumer, which reads the 32-bit size at +8) is the interface at +0x138 of the allocation, so the new 64-bit
					// fields at interface+0x170/+0x178 lie at allocation+0x2A8/+0x2B0. Allocate 0x2C0 (was 0x168/0x160) or the
					// writes corrupt the neighbouring heap block (e.g. BSSystemFileStreamer's path table: "Scatter table has been trashed").
					Relocation(EditorAPI::IDs::Functions::BSResource__LooseFileStream_Create, Offset{ 0x101, 0x101, 0x102 }).Write({ 0xC0, 0x02 });
				}

				LooseFileStreamHook::Generate(Relocation(EditorAPI::IDs::Functions::BSResource__OpenLooseFile, 0x107).Address());
				Relocation(EditorAPI::IDs::Functions::BSResource__GetLooseFileSize32).WriteJump(&sub);

				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
				{
					// As I understand it, CK evaluates the amount of useful data with the file size,
					// since I will now adjust the 64-bit size, sacrificing checking for NullPtr.
					//
					// mov rax, qword ptr ds:[rcx+0x170]
					// jmp -> cmp rsi, rax
					Relocation(EditorAPI::IDs::Functions::sub_2464400, 0x484).Write({ 0x48, 0x8B, 0x81, 0x70, 0x01, 0x00, 0x00, 0xEB, 0x1D });
					
					// Ignoring the correctness check is not useful
					Relocation(EditorAPI::IDs::Functions::legacy_BSResourceLooseFiles_255793, 0x160).Write(JMP);
					Relocation(EditorAPI::IDs::Functions::legacy_BSResourceLooseFiles_647276, 0x14D).Write(JMP);
					Relocation(EditorAPI::IDs::Functions::legacy_BSResourceLooseFiles_530259, 0x176).Write(JMP);
					Relocation(EditorAPI::IDs::Functions::legacy_BSResourceLooseFiles_530994, 0xD2).Write(JMP);
					Relocation(EditorAPI::IDs::Functions::legacy_BSResourceLooseFiles_648213).Write(RET);
					Relocation(EditorAPI::IDs::Functions::legacy_BSResourceLooseFiles_745724).Write(RET);
				}
				else if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_982_3)
				{
					// As I understand it, CK evaluates the amount of useful data with the file size,
					// since I will now adjust the 64-bit size, sacrificing checking for NullPtr.
					//
					// mov rax, qword ptr ds:[rcx+0x170]
					// jmp -> cmp rbx, rax
					Relocation(EditorAPI::IDs::Functions::sub_2464400, 0x3F2).Write({ 0x48, 0x8B, 0x81, 0x70, 0x01, 0x00, 0x00, 0xEB, 0x19 });

					// Ignoring the correctness check is not useful
					Relocation(EditorAPI::IDs::Functions::legacy_BSResourceLooseFiles_255793, 0x15A).Write(JMP);
					Relocation(EditorAPI::IDs::Functions::legacy_BSResourceLooseFiles_1501752, 0x15E).Write(JMP);
					Relocation(EditorAPI::IDs::Functions::legacy_BSResourceLooseFiles_530259, 0x180).Write(JMP);
					Relocation(EditorAPI::IDs::Functions::legacy_BSResourceLooseFiles_530994, 0xD2).Write(JMP);
				}
				else
				{
					// Ignoring the correctness check is not useful
					Relocation(EditorAPI::IDs::Functions::sub_247F9B0, 0x159).Write(JMP);
					Relocation(EditorAPI::IDs::Functions::sub_247FB80, 0x162).Write(JMP);
					Relocation(EditorAPI::IDs::Functions::sub_247FD60, 0x19B).Write(JMP);
					Relocation(EditorAPI::IDs::Functions::sub_24802A0, 0x5D).Write(JMP);
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