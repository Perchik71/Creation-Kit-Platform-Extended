﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "PreCombined.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			PreCombinedPatch::PreCombinedPatch() : Module(GlobalEnginePtr)
			{}

			bool PreCombinedPatch::HasOption() const
			{
				return false;
			}

			bool PreCombinedPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* PreCombinedPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* PreCombinedPatch::GetName() const
			{
				return "PreCombined";
			}

			bool PreCombinedPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> PreCombinedPatch::GetDependencies() const
			{
				return {};
			}

			bool PreCombinedPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool PreCombinedPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					{
						int precomb_flag1 = _READ_OPTION_INT("PreCombined", "nGenerationVersion1", 0);
						int precomb_flag2 = _READ_OPTION_INT("PreCombined", "nGenerationVersion2", 0);

						ScopeRelocator text;

						//
						// Fix for the -GeneratePreCombined command line option creating files for the XB1 (2) format. 
						//
						lpRelocator->Patch(lpRelocationDatabaseItem->At(0), (uint8_t*)&precomb_flag1, 4);
						lpRelocator->Patch(lpRelocationDatabaseItem->At(1), (uint8_t*)&precomb_flag2, 4);
						lpRelocator->Patch(lpRelocationDatabaseItem->At(2), (uint8_t*)&precomb_flag2, 4);
					}

					return true;
				}

				return false;
			}

			bool PreCombinedPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}