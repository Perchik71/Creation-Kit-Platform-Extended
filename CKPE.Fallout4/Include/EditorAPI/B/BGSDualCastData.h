// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSArtObject;
				class BGSExplosion;
				class BGSImpactDataSet;
				class BGSProjectile;
				class TESEffectShader;

				struct BGSDualCastDataDEF
				{
					BGSProjectile* projectile; // 00
					BGSExplosion* explosion; // 08
					TESEffectShader* effectShader; // 10
					BGSArtObject* hitEffectArt; // 18
					BGSImpactDataSet* impactDataSet; // 20
					std::uint32_t flags; // 28
				};
				static_assert(sizeof(BGSDualCastDataDEF) == 0x30);

				// 1.11.240.0: size 0xC0, vtable RVA 0x30BD660, 154 virtual functions
				class BGSDualCastData :
					public TESBoundObject   // 000
				{
				public:
					virtual ~BGSDualCastData() = default;

				public:
					BGSDualCastDataDEF data; // 90
				};
				static_assert(sizeof(BGSDualCastData) == 0xC0);
			}
		}
	}
}
