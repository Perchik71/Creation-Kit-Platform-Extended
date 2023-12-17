// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "IncreaseMaximumNavmesh.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			IncreaseMaximumNavmeshPatch::IncreaseMaximumNavmeshPatch() : Module(GlobalEnginePtr)
			{}

			bool IncreaseMaximumNavmeshPatch::HasOption() const
			{
				return false;
			}

			bool IncreaseMaximumNavmeshPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* IncreaseMaximumNavmeshPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* IncreaseMaximumNavmeshPatch::GetName() const
			{
				return "Increase the maximum navmesh";
			}

			bool IncreaseMaximumNavmeshPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool IncreaseMaximumNavmeshPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Increase the maximum navmesh autogeneration cell limit to 100,000 and prevent spamming UI updates (0.01% -> 1.00%)
					//
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xA0, 0x86, 0x01, 0x00 });
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1), (uintptr_t)&sub);

					return true;
				}

				return false;
			}

			bool IncreaseMaximumNavmeshPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			float IncreaseMaximumNavmeshPatch::sub(float Delta)
			{
				return abs(Delta) / 100.0f;
			}
		}
	}
}