// Copyright © 2023-2024 aka perchik71. All rights reserved.
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
				auto verPatch = lpRelocationDatabaseItem->Version();

				if ((verPatch == 1) || (verPatch == 2))
				{
					{
						int precomb_flag = _READ_OPTION_INT("PreCombined", "nGenerationVersion", 0);

						ScopeRelocator text;

						// This of option control the output of the havok collision information generated during the precombine process, which is stuck at mode 1 unpatched.
						// This will not magically make your plugin the same format. - BenRierimanu
						// 0 - 64bit havok little endian[PC or XB1, default setting with CKPEand recommended for PRPand related patches]
						// 1 - 64bit havok big endian[PS4, default setting unpatched.Untested and not recommended unless you can somehow get the files on that platform]
						// 2 - 32bit havok little endian[PC or XB1, default setting when using the user interface generation commands as the XB1 was originally a 32bit platform at one point, will still work, but obsolete]

						lpRelocator->Patch(lpRelocationDatabaseItem->At(0), (uint8_t*)&precomb_flag, 4);
						
						if (verPatch == 2)
							lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0x6A, (uint8_t)precomb_flag, 0x59 });
						else
							lpRelocator->Patch(lpRelocationDatabaseItem->At(1), (uint8_t*)&precomb_flag, 4);
						
						lpRelocator->Patch(lpRelocationDatabaseItem->At(2), (uint8_t*)&precomb_flag, 4);
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