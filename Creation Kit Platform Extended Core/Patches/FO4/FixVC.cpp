// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixVC.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			FixVCPatch::FixVCPatch() : Module(GlobalEnginePtr)
			{}

			bool FixVCPatch::HasOption() const
			{
				return false;
			}

			bool FixVCPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixVCPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixVCPatch::GetName() const
			{
				return "Version Control fixes";
			}

			bool FixVCPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixVCPatch::GetDependencies() const
			{
				return {};
			}

			bool FixVCPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixVCPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					// By disabling version control, allow the start
					lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0xEB, 0x81 });	// skip msgbox 
					lpRelocator->Patch(lpRelocationDatabaseItem->At(2), { 0xEB, 0xD9 });	// skip msgbox

					return true;
				}

				return false;
			}

			bool FixVCPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixVCPatch::sub(HWND hWnd, const char* lpText, const char* lpCaption, uint32_t uType)
			{
				if (lpText)
					_CONSOLE("MESSAGE: %s", lpText);
			}
		}
	}
}