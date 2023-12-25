// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashHairKS.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			CrashHairKSPatch::CrashHairKSPatch() : Module(GlobalEnginePtr)
			{}

			bool CrashHairKSPatch::HasOption() const
			{
				return false;
			}

			bool CrashHairKSPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashHairKSPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashHairKSPatch::GetName() const
			{
				return "Crash HairKS";
			}

			bool CrashHairKSPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> CrashHairKSPatch::GetDependencies() const
			{
				return {};
			}

			bool CrashHairKSPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashHairKSPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					ScopeRelocator text;

					//
					// Fix crash When getting some kind of index. Sometimes there is an object where after dereferencing rax 
					// becomes equal to 1, which leads to a crash when dereferencing it. Let's add the check to 1.
					//
					lpRelocator->Patch(lpRelocationDatabaseItem->At(0), { 0x0E, 0x83, 0xF8, 0x01, 0x74, 0x09,
						0x8B, 0x40, 0xF8, 0x25, 0xFF, 0xFF, 0xFF, 0x00, 0xC3, 0xC3 });
					lpRelocator->Patch(lpRelocationDatabaseItem->At(1), { 0x0E, 0x83, 0xF8, 0x01, 0x74, 0x09,
						0x8B, 0x40, 0xF8, 0x25, 0xFF, 0xFF, 0xFF, 0x00, 0xC3, 0xC3 });

					return true;
				}

				return false;
			}

			bool CrashHairKSPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}