// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixFileInUse.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixFileInUsePatch::FixFileInUsePatch() : Module(GlobalEnginePtr)
			{}

			bool FixFileInUsePatch::HasOption() const
			{
				return false;
			}

			bool FixFileInUsePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixFileInUsePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixFileInUsePatch::GetName() const
			{
				return "File in use";
			}

			bool FixFileInUsePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixFileInUsePatch::GetDependencies() const
			{
				return {};
			}

			bool FixFileInUsePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixFileInUsePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for "File in use" UI hang after hitting cancel. 
					// It tries to use the main window handle as a parent, but it's suspended during the initial data load, 
					// resulting in a deadlock. The new BGSThreadedProgressDlg causes this.
					//
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0x4D, 0x33, 0xC0, 0x90, 0x90, 0x90, 0x90 });

					return true;
				}

				return false;
			}

			bool FixFileInUsePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}