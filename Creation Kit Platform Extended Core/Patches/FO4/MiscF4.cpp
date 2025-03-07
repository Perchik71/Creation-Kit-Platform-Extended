// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "MiscF4.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			MiscPatch::MiscPatch() : Module(GlobalEnginePtr)
			{}

			bool MiscPatch::HasOption() const
			{
				return false;
			}

			bool MiscPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* MiscPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* MiscPatch::GetName() const
			{
				return "Misc";
			}

			bool MiscPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> MiscPatch::GetDependencies() const
			{
				return {};
			}

			bool MiscPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool MiscPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
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
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(5), (uintptr_t)&sub);

					ScopeRelocator text;

					// Send text to 4 part StatusBar (Game cam: .....)
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0x02 });
					// CheckMenuItem is called, however, it always gets zero, but eight is written on top, 
					// which is equal to MFS_CHECKED.
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(1), 6);
					// CheckMenuItem is called, however, it always gets MFS_CHECKED.
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(2), 6);
					// Fix repeat CheckMenuItem is called
					lpRelocator->Patch(lpRelocationDatabaseItem->At(3), { 0x20, 0x63 });
					// ret skip Warnings window
					lpRelocator->Patch(lpRelocationDatabaseItem->At(4), { 0xC3, 0xCC, 0xCC, 0xCC, 0xCC });
					// ret skip spam "%u main draw calls, %u polys, %s textures, %u FPS"
					//lpRelocator->Patch(lpRelocationDatabaseItem->At(6), { 0xC3, 0xCC });

					return true;
				}

				return false;
			}

			bool MiscPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void MiscPatch::sub(uint64_t a1, uint64_t a2, uint64_t a3)
			{
				if (a1) 
				{
					// In the fallout, everything is in the higher addresses, let's check the match with the base address. 

					uintptr_t f = (uintptr_t)(*(PINT64)a1);
					if ((GlobalEnginePtr->GetModuleBase() & 0xffffffff00000000) == 
						(*((uintptr_t*)(f + 0x190)) & 0xffffffff00000000))
						thisVirtualCall<void>((size_t)0x190, (void*)a1, a2, 0);
				}
			}
		}
	}
}