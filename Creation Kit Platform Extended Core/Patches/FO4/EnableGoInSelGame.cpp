// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "EnableGoInSelGame.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			EnableGoInSelGamePatch::EnableGoInSelGamePatch() : Module(GlobalEnginePtr)
			{}

			bool EnableGoInSelGamePatch::HasOption() const
			{
				return false;
			}

			bool EnableGoInSelGamePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* EnableGoInSelGamePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* EnableGoInSelGamePatch::GetName() const
			{
				return "Go to selection in game";
			}

			bool EnableGoInSelGamePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> EnableGoInSelGamePatch::GetDependencies() const
			{
				return {};
			}

			bool EnableGoInSelGamePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				// remove 1.10.943.1
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_1_10_162_0;
			}

			bool EnableGoInSelGamePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Enable the render window "Go to selection in game" hotkey even if version control is off
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 2);

					return true;
				}

				return false;
			}

			bool EnableGoInSelGamePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}