// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <xbyak.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashNullptrDXGISurface.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			CrashNullptrDXGISurface::CrashNullptrDXGISurface() : Common::Patch()
			{
				SetName("Crash Nullptr DXGISurface");
			}

			bool CrashNullptrDXGISurface::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CrashNullptrDXGISurface::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CrashNullptrDXGISurface::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CrashNullptrDXGISurface::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CrashNullptrDXGISurface::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_SKYRIM_SE_1_6_1378_1;
			}

			bool CrashNullptrDXGISurface::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				class changeDXGISurfaceHook : public Xbyak::CodeGenerator
				{
				public:
					changeDXGISurfaceHook(uintptr_t addr) : Xbyak::CodeGenerator()
					{
						test(rcx, rcx);
						je(".skips");
						// get DXGISurface table function
						mov(rax, ptr[rcx]);
						mov(rax, ptr[rax + 0x10]);
						call(rax);
						L(".skips");
						jmp(ptr[rip]);
						dq(addr);
					}
				} static DXGISurfaceHookInstance(__CKPE_OFFSET(0) + 6);

				Detours::DetourJump(__CKPE_OFFSET(0), (uintptr_t)DXGISurfaceHookInstance.getCode());

				return true;
			}
		}
	}
}