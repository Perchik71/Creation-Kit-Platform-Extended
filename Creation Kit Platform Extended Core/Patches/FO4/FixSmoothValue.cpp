// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixSmoothValue.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			FixSmoothValuePatch::FixSmoothValuePatch() : Module(GlobalEnginePtr)
			{}

			bool FixSmoothValuePatch::HasOption() const
			{
				return false;
			}

			bool FixSmoothValuePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixSmoothValuePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixSmoothValuePatch::GetName() const
			{
				return "Fixed Smoothness value to material";
			}

			bool FixSmoothValuePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixSmoothValuePatch::GetDependencies() const
			{
				return {};
			}

			bool FixSmoothValuePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixSmoothValuePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Fixed when the value is different from 0.0 to 1.0. Smoothness value to material (nif)
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					lpRelocator->Patch(lpRelocationDatabaseItem->At(1), 
						{ 0x66, 0x0F, 0x7E, 0x85, 0x88, 0x00, 0x00, 0x00, 0xEB, 0x18 });

					return true;
				}

				return false;
			}

			bool FixSmoothValuePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			float FixSmoothValuePatch::sub(float a1, float a2)
			{
				return (a2 > 1.0) ? 1.0 : ((a2 < 0.0) ? 0.0 : a2);
			}
		}
	}
}