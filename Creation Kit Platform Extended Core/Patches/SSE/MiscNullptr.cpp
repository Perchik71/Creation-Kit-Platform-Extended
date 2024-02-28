// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "MiscNullptr.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			static uintptr_t sub1_MiscPatch_ptr = 0;

			MiscPatch::MiscPatch() : Module(GlobalEnginePtr)
			{}

			bool MiscPatch::HasOption() const
			{
				return false;
			}

			bool MiscPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* MiscPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* MiscPatch::GetName() const
			{
				return "Misc";
			}

			bool MiscPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> MiscPatch::GetDependencies() const
			{
				return {};
			}

			bool MiscPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return (eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_438) &&
					(eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST);
			}

			bool MiscPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					sub1_MiscPatch_ptr =
						Detours::X64::DetourFunctionClass(lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(0)),
							&MiscPatch::sub1);

					return true;
				}

				return false;
			}

			bool MiscPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			bool MiscPatch::sub1(int64_t* a1, int32_t a2)
			{
				if (!*a1)
					return false;

				return fastCall<bool>(sub1_MiscPatch_ptr, a1, a2);
			}
		}
	}
}