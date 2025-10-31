// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "TESForm.h"
#include "TESObjectREFR.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObjectCELL;

				class BGSLayer : public TESForm
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftLayer;
				private:
					BGSLayer* _parent;
					BSTArray<TESObjectREFR*> _items;
					BSTArray<BGSLayer*> _childs;
				public:
					[[nodiscard]] inline std::uint32_t GetChildsCount() const noexcept(true) { return (std::uint32_t)_childs.size(); }
					[[nodiscard]] inline const BGSLayer** GetChildsArrayConst() const noexcept(true) { return (const BGSLayer**)_childs.data(); }
					[[nodiscard]] inline BGSLayer** GetChildsArray() noexcept(true) { return (BGSLayer**)_childs.data(); }
					[[nodiscard]] inline BGSLayer* GetChildById(uint32_t dwId) noexcept(true) { return _childs[dwId]; }
					[[nodiscard]] inline std::uint32_t GetItemsCount() const noexcept(true) { return (std::uint32_t)_items.size(); }
					[[nodiscard]] inline const TESObjectREFR** GetItemsArrayConst() const noexcept(true) { return (const TESObjectREFR**)_items.data(); }
					[[nodiscard]] inline TESObjectREFR** GetItemsArray() noexcept(true) { return (TESObjectREFR**)_items.data(); }
					[[nodiscard]] inline TESObjectREFR* GetItemById(uint32_t dwId) noexcept(true) { return _items[dwId]; }
					[[nodiscard]] inline const BGSLayer* GetParentConst() const noexcept(true) { return _parent; }
					[[nodiscard]] inline BGSLayer* GetParent() noexcept(true) { return _parent; }
					[[nodiscard]] inline bool IsParent() const noexcept(true) { return _parent != nullptr; }
				public:
					std::uint32_t GetItemsCountInCell(const TESObjectCELL* cell) const noexcept(true);
				public:
					inline TESObjectREFR* operator[](uint32_t dwId) noexcept(true) { return _items[dwId]; }
					inline const TESObjectREFR* operator[](uint32_t dwId) const noexcept(true) { return _items[dwId]; }
				public:
					CKPE_READ_PROPERTY(GetChildsCount) uint32_t ChildsCount;
					CKPE_READ_PROPERTY(GetChildsArrayConst) const BGSLayer** ChildsArrayConst;
					CKPE_READ_PROPERTY(GetChildsArray) BGSLayer** ChildsArray;
					CKPE_READ_PROPERTY(GetItemsCount) uint32_t ItemsCount;
					CKPE_READ_PROPERTY(GetItemsArrayConst) const TESObjectREFR** ItemsArrayConst;
					CKPE_READ_PROPERTY(GetItemsArray) TESObjectREFR** ItemsArray;
					CKPE_READ_PROPERTY(GetParentConst) const BGSLayer* ParentConst;
					CKPE_READ_PROPERTY(GetParent) BGSLayer* Parent;
				};
			}
		}
	}
}