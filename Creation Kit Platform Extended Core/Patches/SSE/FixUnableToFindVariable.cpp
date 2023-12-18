// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixUnableToFindVariable.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixUnableToFindVariablePatch::FixUnableToFindVariablePatch() : Module(GlobalEnginePtr)
			{}

			bool FixUnableToFindVariablePatch::HasOption() const
			{
				return false;
			}

			bool FixUnableToFindVariablePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixUnableToFindVariablePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixUnableToFindVariablePatch::GetName() const
			{
				return "Fix Unable To Find Variable";
			}

			bool FixUnableToFindVariablePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixUnableToFindVariablePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash (invalid parameter termination) when the "Unable to find variable" warning would exceed 
					// the buffer size
					//
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), (uint8_t*)", Text \"%.240s\"", 
						(uint32_t)(strlen(", Text \"%.240s\"") + 1));

					return true;
				}

				return false;
			}

			bool FixUnableToFindVariablePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}