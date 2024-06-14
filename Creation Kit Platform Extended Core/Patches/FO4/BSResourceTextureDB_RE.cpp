// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/FO4/BSResourceEntryDB.h"
#include "BSResourceTextureDB_RE.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			using namespace EditorAPI::Fallout4;

			BSResourceTextureDBREPatch::BSResourceTextureDBREPatch() : Module(GlobalEnginePtr)
			{}

			bool BSResourceTextureDBREPatch::HasOption() const
			{
				return false;
			}

			bool BSResourceTextureDBREPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* BSResourceTextureDBREPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* BSResourceTextureDBREPatch::GetName() const
			{
				return "BSResource Texture DB RE";
			}

			bool BSResourceTextureDBREPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> BSResourceTextureDBREPatch::GetDependencies() const
			{
				return {};
			}

			bool BSResourceTextureDBREPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_1_10_982_3;
			}

			bool BSResourceTextureDBREPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					*(uintptr_t*)&BSResource::CreateHashDB = _RELDATA_ADDR(0);
					*(uintptr_t*)&BSResource::GetTextureDBTraits = _RELDATA_ADDR(1);
					*(uintptr_t*)&BSResource::CreateStringEntryDB = _RELDATA_ADDR(2);
					*(uintptr_t*)&BSResource::ReleaseStringEntryDB = _RELDATA_ADDR(3);
					*(uintptr_t*)&BSResource::Cache = _RELDATA_ADDR(4);
					*(uintptr_t*)&BSResource::GetTextureDBTraitsFromCache = _RELDATA_ADDR(5);

					return true;
				}

				return false;
			}

			bool BSResourceTextureDBREPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}