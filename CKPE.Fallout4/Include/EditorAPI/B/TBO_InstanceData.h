// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <limits>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// Forward declarations only: this header is pulled in by TESBoundObject, so it must stay free of heavy includes (any include that
			// reaches TESDataHandler.h creates a cycle through TESObjectARMO.h / TESObjectLIGH.h).
			template <class _Ty, std::uint32_t GROW, std::uint32_t SHRINK> class BSTArray;
			// These two components live in EditorAPI itself, not in Forms
			class BGSKeywordForm;
			class BGSBlockBashData;
			template <std::uint32_t RefCountBits> class BSHandleRefObject32T;
			typedef BSHandleRefObject32T<10> BSHandleRefObject_Original;

			namespace NiAPI
			{
				class NiAVObject;
			}

			namespace Forms
			{
				template <class _Ty> class TESObjectREFR_base;
				typedef TESObjectREFR_base<BSHandleRefObject_Original> TESObjectREFR;
				class EnchantmentItem;
				class BGSMaterialSwap;
				class TESBoundObject;

				// 1.11.246.0: size 0x10. Reference counted per-instance data of a bound object (weapons, armor): TESBoundObject::CreateInstanceData
				// allocates it (weapon 0x138, armor 0x58 bytes) and copies the base instance data, GetBaseInstanceData returns the form's own copy.
				// The CK has the dtor and eighteen further slots, shared with weapon and armor data.
				class TBO_InstanceData
				{
				public:
					virtual ~TBO_InstanceData() = default;

					// The 18 virtual functions (vtable 0x143046388), named after CommonLibF4's; the defaults below are the base implementations
					// (0x140C58920-0x140C58AA0): getters return null / -1 / 0 / FLT_MAX and the create/delete pairs do nothing.
					[[nodiscard]] virtual BGSKeywordForm* GetKeywordData() const noexcept(true) { return nullptr; } // 01
					virtual void DeleteKeywordData() noexcept(true) { return; } // 02
					virtual void CreateKeywordData() noexcept(true) { return; } // 03
					[[nodiscard]] virtual BGSBlockBashData* GetBlockBashData() const noexcept(true) { return nullptr; } // 04
					virtual void DeleteBlockBashData() noexcept(true) { return; } // 05
					virtual void CreateBlockBashData() noexcept(true) { return; } // 06
					[[nodiscard]] virtual BSTArray<EnchantmentItem*, 10, 10>* GetEnchantmentArray() const noexcept(true) { return nullptr; } // 07
					virtual void DeleteEnchantmentArray() noexcept(true) { return; } // 08
					virtual void CreateEnchantmentArray() noexcept(true) { return; } // 09
					[[nodiscard]] virtual BSTArray<BGSMaterialSwap*, 10, 10>* GetMaterialSwapArray() const noexcept(true) { return nullptr; } // 0A
					virtual void DeleteMaterialSwapArray() noexcept(true) { return; } // 0B
					virtual void CreateMaterialSwapArray() noexcept(true) { return; } // 0C
					[[nodiscard]] virtual float GetWeight() const noexcept(true) { return -1.0f; } // 0D
					[[nodiscard]] virtual std::int32_t GetValue() const noexcept(true) { return -1; } // 0E
					[[nodiscard]] virtual std::uint32_t GetHealth() const noexcept(true) { return 0; } // 0F
					[[nodiscard]] virtual float GetColorRemappingIndex() const noexcept(true) { return (std::numeric_limits<float>::max)(); } // 10
					// Real code (0x140C58A20): applies the instance data to a freshly attached 3D model
					virtual void PostAttach3D(NiAPI::NiAVObject* pObj3D, TESObjectREFR* pReference) const noexcept(true); // 11
					virtual void PostApplyMods(const TESBoundObject* pObject) noexcept(true) { return; } // 12

				public:
					std::uint32_t refCount; // 08 - BSIntrusiveRefCounted
					std::uint32_t pad0C; // 0C
				};
				static_assert(sizeof(TBO_InstanceData) == 0x10);
			}
		}
	}
}
