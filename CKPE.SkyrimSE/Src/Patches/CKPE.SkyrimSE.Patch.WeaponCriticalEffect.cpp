// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/Forms/TESForm.h>
#include <Patches/CKPE.SkyrimSE.Patch.WeaponCriticalEffect.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			typedef bool(*TWeaponCriticalEffectSub1)(std::int64_t, std::int64_t);
			typedef std::uint32_t(*TWeaponCriticalEffectSub2)(std::int64_t);

			static TWeaponCriticalEffectSub1 WeaponCriticalEffectSub1;
			static TWeaponCriticalEffectSub2 WeaponCriticalEffectSub2;

			WeaponCriticalEffect::WeaponCriticalEffect() : Common::Patch()
			{
				SetName("Weapon Critical Effect");
			}

			bool WeaponCriticalEffect::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* WeaponCriticalEffect::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool WeaponCriticalEffect::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> WeaponCriticalEffect::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool WeaponCriticalEffect::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool WeaponCriticalEffect::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				//
				// Fix for weapon critical effect data (CRDT) being destroyed when upgrading from form 
				// version <= 43 to form version 44. The CK reads a structure that has 64bit alignment
				// and is incompatible with old versions.
				//

				SafeWrite::WriteNop(__CKPE_OFFSET(0), 98);
				SafeWrite::WriteNop(__CKPE_OFFSET(1), 7);

				Detours::DetourJump(__CKPE_OFFSET(2), (std::uintptr_t)&sub1);
				Detours::DetourCall(__CKPE_OFFSET(3), (std::uintptr_t)&sub2);

				WeaponCriticalEffectSub1 = (TWeaponCriticalEffectSub1)__CKPE_OFFSET(4);
				WeaponCriticalEffectSub2 = (TWeaponCriticalEffectSub2)__CKPE_OFFSET(5);

				return true;
			}

			void WeaponCriticalEffect::sub1(std::int64_t DiskCRDT, std::int64_t SourceCRDT) noexcept(true)
			{
				// Convert in-memory CRDT to on-disk CRDT data, but do it properly this time
				memset((void*)DiskCRDT, 0, 24);

				*(std::uint16_t*)DiskCRDT = *(std::uint16_t*)(SourceCRDT + 0x10);		// Damage
				*(std::uint32_t*)(DiskCRDT + 0x4) = *(std::uint32_t*)SourceCRDT;		// Percentage multiplier
				*(std::uint8_t*)(DiskCRDT + 0x8) = *(std::uint8_t*)(SourceCRDT + 0x12);	// Flags

				EditorAPI::Forms::TESForm* effectForm = *(EditorAPI::Forms::TESForm**)(SourceCRDT + 0x8);
				if (effectForm)
					*(uint64_t*)(DiskCRDT + 0x10) = effectForm->FormID;
			}

			void WeaponCriticalEffect::sub2(std::int64_t TESFile, std::int64_t SourceCRDT) noexcept(true)
			{
				WeaponCriticalEffectSub1(TESFile, SourceCRDT);

				// Apply struct fixup after reading SkyrimLE data
				uint32_t chunkVersion = WeaponCriticalEffectSub2(TESFile);
				if (chunkVersion < 44)
					*(uint32_t*)(SourceCRDT + 0x10) = *(uint32_t*)(SourceCRDT + 0xC);
			}
		}
	}
}