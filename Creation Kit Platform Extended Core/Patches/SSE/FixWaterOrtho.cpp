// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixWaterOrtho.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixWaterOrthoPatch::FixWaterOrthoPatch() : Module(GlobalEnginePtr)
			{}

			bool FixWaterOrthoPatch::HasOption() const
			{
				return false;
			}

			bool FixWaterOrthoPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixWaterOrthoPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixWaterOrthoPatch::GetName() const
			{
				return "Fix Water In Ortho mode";
			}

			bool FixWaterOrthoPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixWaterOrthoPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for water not rendering correctly while using the orthographic (top-down) camera view. 
					// SSE camera scaling changes cause weird behavior with water shaders.
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);

					return true;
				}

				return false;
			}

			bool FixWaterOrthoPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void FixWaterOrthoPatch::sub(uintptr_t a1, bool a2)
			{
				memset((void*)a1, 0, 0x1C);

				// Unknown camera distance multiplier
				*(bool*)(a1 + 0x18) = a2;
				*(float*)(a1 + 0x10) = 1000.0f;
			}
		}
	}
}