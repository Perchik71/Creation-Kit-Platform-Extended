﻿// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixRemoteDesktop.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixRemoteDesktopPatch::FixRemoteDesktopPatch() : Module(GlobalEnginePtr)
			{}

			bool FixRemoteDesktopPatch::HasOption() const
			{
				return false;
			}

			bool FixRemoteDesktopPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixRemoteDesktopPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixRemoteDesktopPatch::GetName() const
			{
				return "Fix Remote Desktop";
			}

			bool FixRemoteDesktopPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixRemoteDesktopPatch::GetDependencies() const
			{
				return {};
			}

			bool FixRemoteDesktopPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixRemoteDesktopPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0xEB });

					return true;
				}

				return false;
			}

			bool FixRemoteDesktopPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}