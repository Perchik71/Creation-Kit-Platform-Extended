// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <cstddef>
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

				// 1.11.246.0: size 0x2D0, vtable RVA 0x30C22F8, 188 virtual functions
				class MagicItem :
					public TESBoundObject,  // 000
					public TESFullName,     // 090
					public BGSKeywordForm   // 0A8
				{
				public:
					virtual ~MagicItem() = default;

					virtual std::uint32_t GetSpellType() const noexcept(true) { return 0; } // 154 - RVA 0x2D783EC (_purecall)
					virtual void SetCastingType(std::uint32_t nCastingType) noexcept(true) { return; } // 155 - RVA 0xE7C380
					virtual std::uint32_t GetCastingType() const noexcept(true) { return 0; } // 156 - RVA 0x2D783EC (_purecall)
					virtual void SetDelivery(std::uint32_t nDelivery) noexcept(true) { return; } // 157 - RVA 0xE7C390
					virtual std::uint32_t GetDelivery() const noexcept(true) { return 0; } // 158 - RVA 0x2D783EC (_purecall)
					virtual bool IsValidDelivery(std::uint32_t nDelivery) noexcept(true) { return true; } // 159 - RVA 0xE7C3A0
					virtual float GetFixedCastDuration() const noexcept(true) { return 0.0f; } // 160 - RVA 0xE7C3B0
					virtual float GetRange() const noexcept(true) { return 0.0f; } // 161 - RVA 0xE7C3C0
					virtual bool IgnoreResistance() const noexcept(true) { return false; } // 162 - RVA 0xE7C530
					virtual bool IgnoreLOS() const noexcept(true) { return false; } // 163 - RVA 0xE7C540
					virtual bool IsFood() const noexcept(true) { return false; } // 164 - RVA 0xE7C550
					virtual bool GetNoAbsorb() const noexcept(true) { return false; } // 165 - RVA 0xE7C560
					virtual bool GetNoDualCastModifications() const noexcept(true) { return false; } // 166 - RVA 0xE7C570
					virtual bool IsPoison() const noexcept(true) { return false; } // 167 - RVA 0xE79190
					virtual bool IsMedicine() const noexcept(true) { return false; } // 168 - RVA 0xE79180
					virtual void AdjustCost(float& fCost, void* pActor) const noexcept(true) { return; } // 169 - RVA 0xE7E110
					virtual float GetChargeTime() const noexcept(true) { return 0.0f; } // 170 - RVA 0xE65970
					virtual std::uint32_t GetMaxEffectCount() const noexcept(true) { return 0; } // 171 - RVA 0xE659D0
					virtual void* GetAssociatedSkill() const noexcept(true) { return nullptr; } // 172 - RVA 0xE7F8E0
					virtual bool IsTwoHanded() const noexcept(true) { return false; } // 173 - RVA 0xE7E6C0
					virtual std::uint32_t GetChunkID() noexcept(true) { return 0; } // 174
					virtual void CopyMagicItemData(MagicItem* Copy) noexcept(true) { return; } // 175
					[[nodiscard]] virtual bool IsMagicItemDataDifferent(const MagicItem* Other) const noexcept(true) { return false; } // 176
					virtual void SaveMagicItemComponents() noexcept(true) { return; } // 177
					virtual void SaveMagicItemData() noexcept(true) { return; } // 178
					virtual void LoadMagicItemChunk(TESFile* File, std::uint32_t ChunkID) noexcept(true) { return; } // 179
					virtual void LoadChunkDataPostProcess(TESFile* File) noexcept(true) { return; } // 180
					[[nodiscard]] virtual const void* GetConstData() const noexcept(true) { return nullptr; } // 181
					virtual void* GetData() noexcept(true) { return nullptr; } // 182
					[[nodiscard]] virtual std::size_t GetDataSize() const noexcept(true) { return 0; } // 183
					virtual void InitFromChunk(TESFile* File) noexcept(true) { return; } // 184
					virtual void UpdateMagicItemDataInDialogBox(HWND hDialogBox) noexcept(true) { return; } // 185
					virtual void AddMagicItemReferences(TESForm* ReferencingForm) noexcept(true) { return; } // 186
					virtual void RemoveMagicItemReferences(TESForm* ReferencingForm) noexcept(true) { return; } // 187

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
