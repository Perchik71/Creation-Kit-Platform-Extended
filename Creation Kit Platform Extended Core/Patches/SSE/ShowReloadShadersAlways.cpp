// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "ShowReloadShadersAlways.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			ShowReloadShadersAlwaysPatch::ShowReloadShadersAlwaysPatch() : Module(GlobalEnginePtr)
			{}

			bool ShowReloadShadersAlwaysPatch::HasOption() const
			{
				return false;
			}

			bool ShowReloadShadersAlwaysPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* ShowReloadShadersAlwaysPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* ShowReloadShadersAlwaysPatch::GetName() const
			{
				return "Show Reload Shaders Always";
			}

			bool ShowReloadShadersAlwaysPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> ShowReloadShadersAlwaysPatch::GetDependencies() const
			{
				return {};
			}

			bool ShowReloadShadersAlwaysPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool ShowReloadShadersAlwaysPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 2);	// Force bShowReloadShadersButton to always be enabled

					return true;
				}

				return false;
			}

			bool ShowReloadShadersAlwaysPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}