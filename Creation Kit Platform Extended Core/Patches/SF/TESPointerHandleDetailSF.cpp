// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "TESPointerHandleDetailSF.h"
#include "Editor API/SF/TESPointerHandleDetail.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Starfield
		{
			using namespace EditorAPI::Starfield;

			TESPointerHandleDetailSFPatch::TESPointerHandleDetailSFPatch() : Module(GlobalEnginePtr)
			{}

			bool TESPointerHandleDetailSFPatch::HasOption() const
			{
				return false;
			}

			bool TESPointerHandleDetailSFPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* TESPointerHandleDetailSFPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* TESPointerHandleDetailSFPatch::GetName() const
			{
				return "Pointer Handle Manager";
			}

			bool TESPointerHandleDetailSFPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> TESPointerHandleDetailSFPatch::GetDependencies() const
			{
				return {};
			}

			bool TESPointerHandleDetailSFPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_STARFIELD_LAST;
			}

			bool TESPointerHandleDetailSFPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					EditorAPI::Starfield::HandleManager::Singleton = _RELDATA_ADDR(0);

					return true;
				}

				return false;
			}

			bool TESPointerHandleDetailSFPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}