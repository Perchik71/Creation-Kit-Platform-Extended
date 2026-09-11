// Copyright © 2024-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Utils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.Misc.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
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
				Relocation(ID(548371), Offset{ 0x23A, 0x22F }).Write({ 0x02 });
				// CheckMenuItem is called, however, it always gets zero, but eight is written on top,
				// which is equal to MFS_CHECKED.
				Relocation(ID{ 643475, 1634204 }, Offset{ 0xD3D, 0xF9C, 0xFC1, 0xFC3 }).WriteFill(NOP, 6);
				// CheckMenuItem is called, however, it always gets MFS_CHECKED.
				Relocation(ID{ 637365, 1698155 }, Offset{ 0x53, 0x51 }).WriteFill(NOP, 6);
				// Fix repeat CheckMenuItem is called
				if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
					Relocation(ID(643475), 0xFF1).Write({ 0x20, 0x63 });
				else
					Relocation(ID(1634204), Offset{ 0x125A, 0x125A, 0x127F, 0x1281 }).Write({ 0xE9, 0xAE, 0x66, 0x00, 0x00, 0x90 });
				// ret skip Warnings window
				Relocation(ID{ 641592, 1843783 }).Write({ 0xC3, 0xCC, 0xCC, 0xCC, 0xCC });

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
				Relocation(ID{ 768935, 1503558 }, 0x56).WriteCall(&sub);

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