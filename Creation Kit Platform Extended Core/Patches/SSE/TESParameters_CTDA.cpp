// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "TESParameters_CTDA.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_TESParameters_CTDA_sub = 0;

			TESParameters_CTDAPatch::TESParameters_CTDAPatch() : Module(GlobalEnginePtr)
			{}

			bool TESParameters_CTDAPatch::HasOption() const
			{
				return false;
			}

			bool TESParameters_CTDAPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* TESParameters_CTDAPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* TESParameters_CTDAPatch::GetName() const
			{
				return "TESParameters CTDA";
			}

			bool TESParameters_CTDAPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool TESParameters_CTDAPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for incorrect pointer truncate assertion while saving certain conditions (i.e 3DNPC.esp). TESParameters/CTDA.
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					pointer_TESParameters_CTDA_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					return true;
				}

				return false;
			}

			bool TESParameters_CTDAPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void TESParameters_CTDAPatch::sub(__int64 Buffer, __int64* Value)
			{
				// The Value pointer can't be modified since it's used immediately after. Garbage data is still written to the plugin.
				__int64 newValue = *Value & 0x00000000FFFFFFFF;

				((void(__fastcall*)(__int64, __int64*))pointer_TESParameters_CTDA_sub)(Buffer, &newValue);
			}
		}
	}
}