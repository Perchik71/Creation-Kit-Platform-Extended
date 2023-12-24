// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixObjectPalette.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_FixObjectPalette_sub1 = 0;
			uintptr_t pointer_FixObjectPalette_sub2 = 0;

			FixObjectPalettePatch::FixObjectPalettePatch() : Module(GlobalEnginePtr)
			{}

			bool FixObjectPalettePatch::HasOption() const
			{
				return false;
			}

			bool FixObjectPalettePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixObjectPalettePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixObjectPalettePatch::GetName() const
			{
				return "Fix Object Palette";
			}

			bool FixObjectPalettePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixObjectPalettePatch::GetDependencies() const
			{
				return {};
			}

			bool FixObjectPalettePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixObjectPalettePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for the "Object Palette" preview window not working. Window render state has to be set to '2'.
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					pointer_FixObjectPalette_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));
					pointer_FixObjectPalette_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));

					return true;
				}

				return false;
			}

			bool FixObjectPalettePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixObjectPalettePatch::sub(HWND WindowHandle, uint32_t* ControlId)
			{
				__int64 previewControl = ((__int64(__fastcall*)(HWND, uint32_t*))pointer_FixObjectPalette_sub1)(WindowHandle, ControlId);

				if (previewControl)
					((void(__fastcall*)(__int64))pointer_FixObjectPalette_sub2)(previewControl);
			}
		}
	}
}