// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <xbyak.h>
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

			bool CrashNullptrDXGISurface::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool CrashNullptrDXGISurface::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashNullptrDXGISurface::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				auto target = Relocation(ID(269371), Offset{ 0x28, 0x2A }).Address();

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
				} static DXGISurfaceHookInstance(target + 6);

				Detours::DetourJump(target, (std::uintptr_t)DXGISurfaceHookInstance.getCode());

				return true;
			}
		}
	}
}