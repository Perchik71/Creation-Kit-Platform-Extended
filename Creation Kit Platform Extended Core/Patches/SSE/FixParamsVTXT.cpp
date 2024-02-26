// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/SSE/TESObjectLAND.h"
#include "FixParamsVTXT.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixParamsVTXTPatch::FixParamsVTXTPatch() : Module(GlobalEnginePtr)
			{}

			bool FixParamsVTXTPatch::HasOption() const
			{
				return false;
			}

			bool FixParamsVTXTPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixParamsVTXTPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixParamsVTXTPatch::GetName() const
			{
				return "Fix VTXT params sections";
			}

			bool FixParamsVTXTPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixParamsVTXTPatch::GetDependencies() const
			{
				return {};
			}

			bool FixParamsVTXTPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return (eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_438) &&
					(eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST);
			}

			bool FixParamsVTXTPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					ScopeRelocator text;

					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), 
						(uintptr_t)&EditorAPI::SkyrimSpectialEdition::TESObjectLAND::Layers::HKNormalize);

					return true;
				}

				return false;
			}

			bool FixParamsVTXTPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}