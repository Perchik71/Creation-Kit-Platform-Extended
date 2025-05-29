// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashNullptrDXGISurface.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			CrashNullptrDXGISurfacePatch::CrashNullptrDXGISurfacePatch() : Module(GlobalEnginePtr)
			{}

			bool CrashNullptrDXGISurfacePatch::HasOption() const
			{
				return false;
			}

			bool CrashNullptrDXGISurfacePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashNullptrDXGISurfacePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashNullptrDXGISurfacePatch::GetName() const
			{
				return "Crash Nullptr DXGISurface";
			}

			bool CrashNullptrDXGISurfacePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> CrashNullptrDXGISurfacePatch::GetDependencies() const
			{
				return {};
			}

			bool CrashNullptrDXGISurfacePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_1378_1;
			}

			bool CrashNullptrDXGISurfacePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
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
					} static DXGISurfaceHookInstance(_RELDATA_ADDR(0)+6);
					
					lpRelocator->DetourJump(_RELDATA_RAV(0), (uintptr_t)DXGISurfaceHookInstance.getCode());

					return true;
				}

				return false;
			}

			bool CrashNullptrDXGISurfacePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}