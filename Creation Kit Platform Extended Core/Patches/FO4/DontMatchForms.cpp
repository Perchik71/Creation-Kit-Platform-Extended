// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "DontMatchForms.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			DontMatchFormsPatch::DontMatchFormsPatch() : Module(GlobalEnginePtr)
			{}

			bool DontMatchFormsPatch::HasOption() const
			{
				return false;
			}

			bool DontMatchFormsPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* DontMatchFormsPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* DontMatchFormsPatch::GetName() const
			{
				return "Dont Match Forms";
			}

			bool DontMatchFormsPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> DontMatchFormsPatch::GetDependencies() const
			{
				return {};
			}

			bool DontMatchFormsPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return (eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST) &&
					(eEditorCurrentVersion != EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_1_10_943_1);
			}

			bool DontMatchFormsPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					lpRelocator->Patch(_RELDATA_RAV(0), { 0xEB });

					return true;
				}

				return false;
			}

			bool DontMatchFormsPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}