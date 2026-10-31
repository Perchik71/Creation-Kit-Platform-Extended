// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/B/BGSPreloadable.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSCollisionLayer;
				class BGSExplosion;
				class BGSProjectile;
				class BGSSoundDescriptorForm;
				class BGSTextureSet;
				class TESObjectLIGH;
				class TESObjectWEAP;

				struct BGSProjectileData
				{
					std::uint32_t flags; // 00
					float gravity; // 04
					float speed; // 08
					float range; // 0C
					TESObjectLIGH* light; // 10
					TESObjectLIGH* muzzleFlashLight; // 18
					float explosionProximity; // 20
					float explosionTimer; // 24
					BGSExplosion* explosionType; // 28
					BGSSoundDescriptorForm* activeSoundLoop; // 30
					float muzzleFlashDuration; // 38
					float fadeOutTime; // 3C
					float force; // 40
					BGSSoundDescriptorForm* countdownSound; // 48
					BGSSoundDescriptorForm* deactivateSound; // 50
					TESObjectWEAP* defaultWeaponSource; // 58
					float coneSpread; // 60
					float collisionRadius; // 64
					float lifetime; // 68
					float relaunchInterval; // 6C
					BGSTextureSet* decalData; // 70
					BGSCollisionLayer* collisionLayer; // 78
					BGSProjectile* vatsProjectile; // 80
					std::int8_t tracerFrequency; // 88
				};
				static_assert(offsetof(BGSProjectileData, vatsProjectile) == 0x80);
				static_assert(sizeof(BGSProjectileData) == 0x90);

				// 1.11.240.0: size 0x1C8, vtable RVA 0x3083F18, 154 virtual functions
				class BGSProjectile :
					public TESBoundObject,             // 000
					public TESFullName,                // 090
					public TESModel,                   // 0A8
					public BGSPreloadable,             // 0E0
					public BGSDestructibleObjectForm   // 0E8
				{
				public:
					virtual ~BGSProjectile() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 278 */ void GetObjectWindowTreePath(BSString& Path) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2B8 */ void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 368 */ void CollectAdditionalResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true) override;
					/* 400 */ void Clone3D(TESObjectREFR* pRequester, NiAPI::NiPointer<NiAPI::NiAVObject>* pObj3D, bool bBackground) noexcept(true) override;
					/* 408 */ void UnClone3D(void* pRequester) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftProjectile;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSProjectile[0];
				public:
					BGSProjectileData data; // F8
					TESModel muzzleFlashModel; // 188
					std::int32_t soundLevel; // 1C0 - REX::TEnumSet<SOUND_LEVEL, std::int32_t>
					std::uint32_t unk1C4; // 1C4
				};
				static_assert(sizeof(BGSProjectile) == 0x1C8);
			}
		}
	}
}
