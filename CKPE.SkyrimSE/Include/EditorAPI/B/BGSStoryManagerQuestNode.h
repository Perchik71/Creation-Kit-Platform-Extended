// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BGSStoryManagerNodeBase.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0xE0, vtable RVA 0x3174910
				class BGSStoryManagerQuestNode :
					public BGSStoryManagerNodeBase	// 00
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftStoryQuestNode;

					virtual ~BGSStoryManagerQuestNode() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 078 */ void ClearData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 0E0 */ void InitItemImpl() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
				public:
					std::uint8_t unk40[0xA0];	// 40 - data, layout not resolved
				};
				static_assert(offsetof(BGSStoryManagerQuestNode, unk40) == 0x40);
				static_assert(sizeof(BGSStoryManagerQuestNode) == 0xE0);
			}
		}
	}
}