// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class Forms;
				class EffectSetting;

				// 1.11.240.0: size 0x2D0, vtable RVA 0x30C12F8, 188 virtual functions
				class MagicItem :
					public TESBoundObject,  // 000
					public TESFullName,     // 090
					public BGSKeywordForm   // 0A8
				{
				public:
					virtual ~MagicItem() = default;

					virtual std::uint32_t GetSpellType() const noexcept(true) { return 0; } // 154 - RVA 0x8258190
					virtual void SetCastingType(std::uint32_t nCastingType) noexcept(true) { return; } // 155 - RVA 0xE7C310, *((_DWORD *)this + 124) = a2;
					virtual std::uint32_t GetCastingType() const noexcept(true) { return 0; } // 156 - RVA 0x8258190
					virtual void SetDelivery(std::uint32_t nDelivery) noexcept(true) { return; } // 157 - RVA 0xE7C320, *(_DWORD *)(a1 + 500) = a2;
					virtual std::uint32_t GetDelivery() const noexcept(true) { return 0; } // 158 - RVA 0x8258190
					virtual bool IsValidDelivery(std::uint32_t nDelivery) noexcept(true) { return true; } // 159 - RVA 0xE7C330, return 1;
					virtual float GetFixedCastDuration() const noexcept(true) { return 0.0f; } // 160 - RVA 0xE7C340, return 0.0;
					virtual float GetRange() const noexcept(true) { return 0.0f; } // 161 - RVA 0xE7C350, return 0.0;
					virtual bool IgnoreResistance() const noexcept(true) { return false; } // 162 - RVA 0xE7C4C0, return 0;
					virtual bool IgnoreLOS() const noexcept(true) { return false; } // 163 - RVA 0xE7C4D0, return 0;
					virtual bool IsFood() const noexcept(true) { return false; } // 164 - RVA 0xE7C4E0, return 0;
					virtual bool GetNoAbsorb() const noexcept(true) { return false; } // 165 - RVA 0xE7C4F0, return 0;
					virtual bool GetNoDualCastModifications() const noexcept(true) { return false; } // 166 - RVA 0xE7C500, return 0;
					virtual bool IsPoison() const noexcept(true) { return false; } // 167 - RVA 0xE79120, return (*(unsigned int (__fastcall **)(__int64))(*(_QWORD *)a1 + 1232LL))(a1) == 5;
					virtual bool IsMedicine() const noexcept(true) { return false; } // 168 - RVA 0xE79110, return 0;
					virtual void AdjustCost(float& fCost, void* pActor) const noexcept(true) { return; } // 169 - RVA 0xE7E0A0
					virtual float GetChargeTime() const noexcept(true) { return 0.0f; } // 170 - RVA 0xE65900, return 0.0;
					virtual std::uint32_t GetMaxEffectCount() const noexcept(true) { return 0; } // 171 - RVA 0xE65960, return 0;
					virtual void* GetAssociatedSkill() const noexcept(true) { return nullptr; } // 172 - RVA 0xE7F870, return 0;
					virtual bool IsTwoHanded() const noexcept(true) { return false; } // 173 - RVA 0xE7E650, return 0;
					virtual void sub_570() { return; } // 174 - RVA 0x8258190
					virtual void sub_578() { return; } // 175 - RVA 0x8258190
					virtual void sub_580() { return; } // 176 - RVA 0x8258190
					virtual void sub_588() { return; } // 177 - RVA 0xE794F0
					virtual void sub_590() { return; } // 178 - RVA 0x8258190
					virtual void sub_598() { return; } // 179 - RVA 0xE79190
					virtual void sub_5A0() { return; } // 180 - RVA 0xE7E7F0
					virtual void sub_5A8() { return; } // 181 - RVA 0x8258190
					virtual void sub_5B0() { return; } // 182 - RVA 0x8258190
					virtual void sub_5B8() { return; } // 183 - RVA 0x8258190
					virtual void sub_5C0() { return; } // 184 - RVA 0x8258190
					virtual void sub_5C8() { return; } // 185 - RVA 0xE7F860
					virtual void sub_5D0() { return; } // 186 - RVA 0xE81FC0
					virtual void sub_5D8() { return; } // 187 - RVA 0xE820E0

				public:
					BSTArray<void*> listOfEffects; // C8 - BSTArray<EffectItem*>
					std::int32_t hostileCount; // E0
					std::uint32_t unkE4; // E4
					EffectSetting* avEffectSetting; // E8
					std::uint8_t unkF0[0x100]; // F0
					std::uint32_t castingType; // 1F0 - verified: SetCastingType (slot 155) writes it
					std::uint32_t delivery; // 1F4 - verified: SetDelivery (slot 157) writes it
					std::uint8_t unk1F8[0xD8]; // 1F8
				};
				static_assert(sizeof(MagicItem) == 0x2D0);
			}
		}
	}
}
