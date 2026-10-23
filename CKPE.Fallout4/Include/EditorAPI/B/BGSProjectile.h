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
