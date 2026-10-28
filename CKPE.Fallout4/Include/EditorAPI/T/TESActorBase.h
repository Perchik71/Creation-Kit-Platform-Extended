// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESActorBaseData.h>
#include <EditorAPI/T/TESContainer.h>
#include <EditorAPI/T/TESSpellList.h>
#include <EditorAPI/T/TESAIForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/F/FormInterfaces.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSSkinForm.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/B/BGSAttackDataForm.h>
#include <EditorAPI/B/BGSPerkRankArray.h>
#include <EditorAPI/B/BGSPropertySheet.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESCombatStyle;
				class TESForm;

				// 1.11.240.0: size 0x248, vtable RVA 0x30454C0, 158 virtual functions
				class TESActorBase :
					public TESBoundAnimObject,         // 000
					public TESActorBaseData,           // 090
					public TESContainer,               // 0F8
					public TESSpellList,               // 170
					public TESAIForm,                  // 180
					public TESFullName,                // 1B0
					public ActorValueOwner,            // 1C8
					public BGSDestructibleObjectForm,  // 1D0
					public BGSSkinForm,                // 1E0
					public BGSKeywordForm,             // 1F8
					public BGSAttackDataForm,          // 218
					public BGSPerkRankArray,           // 228
					public BGSPropertySheet            // 238
				{
				public:
					virtual ~TESActorBase() = default;

					constexpr static std::uint8_t TYPE_ID = ftReserved000;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESActorBase[0];

					virtual bool GetHasPLSpecTex() const noexcept(true) { return false; } // 154 - RVA 0xAB1160, return 0;
					virtual TESCombatStyle* GetCombatStyle() noexcept(true) { return nullptr; } // 155 - RVA 0xAB1020, return 0;
					virtual void SetCombatStyle(TESCombatStyle* pCombatStyle) noexcept(true) { return; } // 156 - RVA 0xAB2B80
					virtual TESForm* GetAsForm() noexcept(true) { return nullptr; } // 157 - RVA 0xAB0B00, return a1;
				};
				static_assert(sizeof(TESActorBase) == 0x248);
			}
		}
	}
}
