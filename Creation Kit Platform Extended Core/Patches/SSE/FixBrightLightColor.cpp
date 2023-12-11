// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixBrightLightColor.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_brightlightcolor_sub = 0;

			FixBrightLightColorPatch::FixBrightLightColorPatch() : Module(GlobalEnginePtr)
			{}

			bool FixBrightLightColorPatch::HasOption() const
			{
				return false;
			}

			bool FixBrightLightColorPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixBrightLightColorPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixBrightLightColorPatch::GetName() const
			{
				return "Fix Bright Light Color";
			}

			bool FixBrightLightColorPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixBrightLightColorPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for the "Bright Light Color" option having incorrect colors in the preferences window. The blue and green channels are swapped.
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					pointer_brightlightcolor_sub = lpRelocationDatabaseItem->At(1);

					return true;
				}

				return false;
			}

			bool FixBrightLightColorPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixBrightLightColorPatch::sub(__int64 a1, uint32_t Color)
			{
				// Blue and green channels are swapped
				uint32_t newColor = Color & 0xFF;
				newColor |= ((Color >> 16) & 0xFF) << 8;
				newColor |= ((Color >> 8) & 0xFF) << 16;

				((void(__fastcall*)(__int64, uint32_t))pointer_brightlightcolor_sub)(a1, newColor);
			}
		}
	}
}