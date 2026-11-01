// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BGSStoryManagerBranchNode.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x88, vtable RVA 0x3173BA8
				class BGSStoryManagerEventNode :
					public BGSStoryManagerBranchNode	// 00
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftStoryEventNode;

					virtual ~BGSStoryManagerEventNode() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 240 */ bool MarkChildFormsAsDeleted(bool Value, void* UserFile, void* VersionFile) noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint8_t unk68[0x20];	// 68 - data, layout not resolved
				};
				static_assert(offsetof(BGSStoryManagerEventNode, unk68) == 0x68);
				static_assert(sizeof(BGSStoryManagerEventNode) == 0x88);
			}
		}
	}
}