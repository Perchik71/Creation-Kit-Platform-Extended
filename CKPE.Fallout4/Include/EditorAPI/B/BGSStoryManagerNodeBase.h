// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BGSStoryManagerTreeForm.h>
#include <EditorAPI/T/TESCondition.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSStoryManagerBranchNode;
				// 1.11.246.0: size 0x58, vtable RVA 0x3014048, 135 virtual functions
				class BGSStoryManagerNodeBase :
					public BGSStoryManagerTreeForm   // 000
				{
				public:
					virtual ~BGSStoryManagerNodeBase() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftReserved000;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSStoryManagerNodeBase[0];

					virtual void SaveNodeData() noexcept(true) { return; } // 128
					virtual void* GetConditions() noexcept(true) { return nullptr; } // 129 - RVA 0x87CFA0
					[[nodiscard]] virtual bool CanCheckOutChildNodes() const noexcept(true) { return true; } // 130
					virtual bool UpdateChildNodeTrackingData() noexcept(true) { return true; } // 131
					// Likely a node lifecycle hook; inspected overrides are no-ops.
					// All four native node tables inherit this no-op.
					virtual void sub_420() { return; } // 132 - RVA 0x87BFB0
					virtual void UpdateNodeDataInDialogBox(HWND hDialogBox) noexcept(true) { return; } // 133
					virtual bool PerformNodeDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam,
						LPARAM lParam, LRESULT Result) noexcept(true) { return false; } // 134

				public:
					/* 0x028 */ BGSStoryManagerBranchNode* parent;
					/* 0x030 */ BGSStoryManagerNodeBase* prevSibling;
					/* 0x038 */ std::uint32_t maxQuests;
					/* 0x03C */ std::uint32_t flags;
					/* 0x040 */ TESCondition conditions;
				};
				static_assert(sizeof(BGSStoryManagerNodeBase) == 0x58);
				static_assert(offsetof(BGSStoryManagerNodeBase, parent) == 0x28);
				static_assert(offsetof(BGSStoryManagerNodeBase, flags) == 0x3C);
				static_assert(offsetof(BGSStoryManagerNodeBase, conditions) == 0x40);
			}
		}
	}
}
