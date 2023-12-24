// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "WeaponCriticalEffect.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			uintptr_t pointer_WeaponCriticalEffect_sub1 = 0;
			uintptr_t pointer_WeaponCriticalEffect_sub2 = 0;

			WeaponCriticalEffectPatch::WeaponCriticalEffectPatch() : Module(GlobalEnginePtr)
			{}

			bool WeaponCriticalEffectPatch::HasOption() const
			{
				return false;
			}

			bool WeaponCriticalEffectPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* WeaponCriticalEffectPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* WeaponCriticalEffectPatch::GetName() const
			{
				return "Weapon Critical Effect";
			}

			bool WeaponCriticalEffectPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> WeaponCriticalEffectPatch::GetDependencies() const
			{
				return {};
			}

			bool WeaponCriticalEffectPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool WeaponCriticalEffectPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for weapon critical effect data (CRDT) being destroyed when upgrading from form 
					// version <= 43 to form version 44. The CK reads a structure that has 64bit alignment
					// and is incompatible with old versions.
					//

					{
						ScopeRelocator text;

						lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 98);
						lpRelocator->PatchNop(lpRelocationDatabaseItem->At(1), 7);
					}

					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(2), (uintptr_t)&sub1);
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(3), (uintptr_t)&sub2);

					pointer_WeaponCriticalEffect_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(4));
					pointer_WeaponCriticalEffect_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(5));

					return true;
				}

				return false;
			}

			bool WeaponCriticalEffectPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void WeaponCriticalEffectPatch::sub1(__int64 DiskCRDT, __int64 SourceCRDT)
			{
				// Convert in-memory CRDT to on-disk CRDT data, but do it properly this time
				memset((void*)DiskCRDT, 0, 24);

				*(uint16_t*)DiskCRDT = *(uint16_t*)(SourceCRDT + 0x10);			// Damage
				*(uint32_t*)(DiskCRDT + 0x4) = *(uint32_t*)SourceCRDT;			// Percentage multiplier
				*(uint8_t*)(DiskCRDT + 0x8) = *(uint8_t*)(SourceCRDT + 0x12);	// Flags

				TESForm* effectForm = *(TESForm**)(SourceCRDT + 0x8);

				if (effectForm)
					*(uint64_t*)(DiskCRDT + 0x10) = effectForm->GetFormID();
			}

			void WeaponCriticalEffectPatch::sub2(__int64 TESFile, __int64 SourceCRDT)
			{
				((bool(__fastcall*)(__int64, __int64))pointer_WeaponCriticalEffect_sub1)(TESFile, SourceCRDT);

				// Apply struct fixup after reading SkyrimLE data
				uint32_t chunkVersion = ((uint32_t(__fastcall*)(__int64))pointer_WeaponCriticalEffect_sub2)(TESFile);

				if (chunkVersion < 44)
					*(uint32_t*)(SourceCRDT + 0x10) = *(uint32_t*)(SourceCRDT + 0xC);
			}
		}
	}
}