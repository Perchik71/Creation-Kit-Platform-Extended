// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESBoundAnimObject.h>
#include <EditorAPI/B/TESActorBaseData.h>
#include <EditorAPI/B/TESContainer.h>
#include <EditorAPI/B/TESSpellList.h>
#include <EditorAPI/B/TESAIForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/ActorValueOwner.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSSkinForm.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/B/BGSAttackDataForm.h>
#include <EditorAPI/B/BGSPerkRankArray.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x178 (base of TESNPC; no factory of its own)
				class TESActorBase :
					public TESBoundAnimObject,			// 000
					public TESActorBaseData,			// 050
					public TESContainer,				// 0A0
					public TESSpellList,				// 0B8
					public TESAIForm,					// 0C8
					public TESFullName,					// 0F8
					public ActorValueOwner,				// 110
					public BGSDestructibleObjectForm,	// 118
					public BGSSkinForm,					// 128
					public BGSKeywordForm,				// 140
					public BGSAttackDataForm,			// 158
					public BGSPerkRankArray				// 168
				{
				public:
					virtual ~TESActorBase() = default;
				};
				static_assert(sizeof(TESActorBase) == 0x178);
			}
		}
	}
}