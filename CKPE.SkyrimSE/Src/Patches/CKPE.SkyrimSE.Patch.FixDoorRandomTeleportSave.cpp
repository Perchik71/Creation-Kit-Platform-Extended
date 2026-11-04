// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <xbyak.h>
#include <CKPE.Utils.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/IDs.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixDoorRandomTeleportSave.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			FixDoorRandomTeleportSave::FixDoorRandomTeleportSave() : Common::Patch()
			{
				SetName("Fix Door Random Teleport Save");
			}

			bool FixDoorRandomTeleportSave::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixDoorRandomTeleportSave::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixDoorRandomTeleportSave::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixDoorRandomTeleportSave::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixDoorRandomTeleportSave::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool FixDoorRandomTeleportSave::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixDoorRandomTeleportSave::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// TESObjectDOOR::Save writes one TNAM subrecord per entry of the "Randomly Teleports to these
				// Interiors / Worldspaces only" list (BSTArray<TESForm*> at +0x100, size at +0x110), in a loop
				// that tests only whether the array is not empty instead of "index < size":
				//
				//   for (i = 0; list.size != 0; ++i) WriteTNAM(list[i]->formID);
				//
				// With a non-empty list the loop never ends, BSTArray::operator[] runs past the end of the buffer,
				// the editor crashes in the middle of writing the plugin and the file is corrupted.
				// Detailed: https://github.com/Perchik71/Creation-Kit-Platform-Extended/issues/59
				//
				// Original loop condition (23 bytes):
				//   mov rax, qword ptr ss:[rsp+0x40]       ; this
				//   add rax, 0x110
				//   mov rcx, rax
				//   call <size getter>                     ; mov eax, dword ptr ds:[rcx]
				//   test eax, eax
				//   jz <exit>

				constexpr std::size_t patch_size = 23;
				constexpr std::size_t exit_offset = 0x4D;	// relative to the start of the patched code

				auto off = Relocation(EditorAPI::IDs::TESObjectDOOR::Save, 0x125).Address();

				// The loop condition is replaced with "size > i" (exit when size <= i); the jump keeps the original target.
				class DoorSaveLoopCondition : public Xbyak::CodeGenerator
				{
				public:
					DoorSaveLoopCondition(void* buf, std::size_t size) : Xbyak::CodeGenerator(size, buf)
					{
						Xbyak::Label exit;

						mov(rax, ptr[rsp + 0x40]);			// this
						mov(eax, ptr[rax + 0x110]);			// list.size
						cmp(eax, ptr[rsp + 0x24]);			// size - i
						nop(6);
						jbe(exit, T_SHORT);					// exit when size <= i
						nop(exit_offset - patch_size);
						L(exit);
					}
				};

				alignas(16) std::uint8_t buffer[0x60];
				DoorSaveLoopCondition cond(buffer, sizeof(buffer));
				cond.ready();

				SafeWrite::Write(off, buffer, patch_size);

				return true;
			}
		}
	}
}
