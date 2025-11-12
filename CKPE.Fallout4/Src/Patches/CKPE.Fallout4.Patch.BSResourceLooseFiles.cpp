// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.FileUtils.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
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

			bool BSResourceLooseFiles::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2) && (verPatch != 3))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

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

				if (verPatch == 1)
				{
					// Set new size class 0x160 to 0x180
					SafeWrite::Write(__CKPE_OFFSET(0), { 0x80 });

					LooseFileStreamHook::Generate(__CKPE_OFFSET(1));
					Detours::DetourJump(__CKPE_OFFSET(8), (uintptr_t)&sub);

					// As I understand it, CK evaluates the amount of useful data with the file size, 
					// since I will now adjust the 64-bit size, sacrificing checking for NullPtr.
					//
					// mov rax, qword ptr ds:[rcx+0x170]
					// jmp -> cmp rsi, rax
					SafeWrite::Write(__CKPE_OFFSET(2), { 0x48, 0x8B, 0x81, 0x70, 0x01, 0x00, 0x00, 0xEB, 0x1D });

					// Ignoring the correctness check is not useful
					SafeWrite::Write(__CKPE_OFFSET(3), { 0xEB });
					SafeWrite::Write(__CKPE_OFFSET(4), { 0xEB });
					SafeWrite::Write(__CKPE_OFFSET(5), { 0xEB });
					SafeWrite::Write(__CKPE_OFFSET(6), { 0xC3 });
					SafeWrite::Write(__CKPE_OFFSET(7), { 0xC3 });
				}
				else if (verPatch == 2)
				{
					LooseFileStreamHook::Generate(__CKPE_OFFSET(7));
					Detours::DetourJump(__CKPE_OFFSET(4), (uintptr_t)&sub);

					// Ignoring the correctness check is not useful

					for (std::uint32_t i = 0; i < 4; i++)
						SafeWrite::Write(__CKPE_OFFSET(i), { 0xEB });

					// As I understand it, CK evaluates the amount of useful data with the file size, 
					// since I will now adjust the 64-bit size, sacrificing checking for NullPtr.
					//
					// mov rax, qword ptr ds:[rcx+0x170]
					// jmp -> cmp rbx, rax
					SafeWrite::Write(__CKPE_OFFSET(5), { 0x48, 0x8B, 0x81, 0x70, 0x01, 0x00, 0x00, 0xEB, 0x19 });

					// Set new size class 0x168 to 0x180
					SafeWrite::Write(__CKPE_OFFSET(6), { 0x80 });
				}
				else
				{
					LooseFileStreamHook::Generate(__CKPE_OFFSET(7));
					Detours::DetourJump(__CKPE_OFFSET(4), (uintptr_t)&sub);

					// Ignoring the correctness check is not useful

					for (std::uint32_t i = 0; i < 4; i++)
						SafeWrite::Write(__CKPE_OFFSET(i), { 0xEB });

					// Set new size class 0x168 to 0x180
					SafeWrite::Write(__CKPE_OFFSET(6), { 0x80 });
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