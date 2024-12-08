// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "ResponseIgnoreMaxF4.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			ResponseIgnoreMaxPatch::ResponseIgnoreMaxPatch() : Module(GlobalEnginePtr)
			{}

			bool ResponseIgnoreMaxPatch::HasOption() const
			{
				return false;
			}

			bool ResponseIgnoreMaxPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* ResponseIgnoreMaxPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* ResponseIgnoreMaxPatch::GetName() const
			{
				return "Response Ignore Max";
			}

			bool ResponseIgnoreMaxPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> ResponseIgnoreMaxPatch::GetDependencies() const
			{
				return {};
			}

			bool ResponseIgnoreMaxPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool ResponseIgnoreMaxPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Skip message setting blocking text input after 149 characters.
					//
					lpRelocator->PatchNop(_RELDATA_RAV(0), 5);

					return true;
				}

				return false;
			}

			bool ResponseIgnoreMaxPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}