// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CheckD3D11.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			CheckD3D11Patch::CheckD3D11Patch() : Module(GlobalEnginePtr)
			{}

			bool CheckD3D11Patch::HasOption() const
			{
				return false;
			}

			bool CheckD3D11Patch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CheckD3D11Patch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CheckD3D11Patch::GetName() const
			{
				return "Check D3D11";
			}

			bool CheckD3D11Patch::HasDependencies() const
			{
				return false;
			}

			Array<String> CheckD3D11Patch::GetDependencies() const
			{
				return {};
			}

			bool CheckD3D11Patch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool CheckD3D11Patch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Assert if D3D11 FL11 features are not supported
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);

					return true;
				}

				return false;
			}

			bool CheckD3D11Patch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void CheckD3D11Patch::sub()
			{
				AssertMsg(false, 
					"Creation Kit renderer initialization failed because your graphics card doesn't support D3D11 Feature Level 11 (FL11_0).\n"
					"Updating your drivers may fix this.");
			}
		}
	}
}