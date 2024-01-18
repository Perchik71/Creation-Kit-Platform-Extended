// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "AssertD3D11.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			AssertD3D11Patch::AssertD3D11Patch() : Module(GlobalEnginePtr)
			{}

			bool AssertD3D11Patch::HasOption() const
			{
				return false;
			}

			bool AssertD3D11Patch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* AssertD3D11Patch::GetOptionName() const
			{
				return nullptr;
			}

			const char* AssertD3D11Patch::GetName() const
			{
				return "D3D11 FL11 features are not supported";
			}

			bool AssertD3D11Patch::HasDependencies() const
			{
				return false;
			}

			Array<String> AssertD3D11Patch::GetDependencies() const
			{
				return {};
			}

			bool AssertD3D11Patch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool AssertD3D11Patch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);

					return true;
				}

				return false;
			}

			bool AssertD3D11Patch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void AssertD3D11Patch::sub()
			{
				AssertMsg(FALSE, "Creation Kit renderer initialization failed because your graphics card doesn't"
					" support D3D11 Feature Level 11 (FL11_0). Updating your drivers may fix this.");
			}
		}
	}
}