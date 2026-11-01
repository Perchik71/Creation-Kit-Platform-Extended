// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESModelTextureSwap.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0xB8, vtable RVA 0x30E78B8
				class BGSAddonNode :
					public TESBoundObject,	// 00
					public TESModelTextureSwap	// 50
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftAddonNode;

					virtual ~BGSAddonNode() = default;

					/* 070 */ void InitializeData() noexcept(true) override;
					/* 080 */ bool Load(TESFile* File) noexcept(true) override;
					/* 098 */ void Save() noexcept(true) override;
					/* 1C8 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 1D0 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 248 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 250 */ [[nodiscard]] bool CanCheckIn(const void* Forms) const noexcept(true) override;
					/* 258 */ void BuildCheckInDependencyMessage(const void* Forms, void* Message) const noexcept(true) override;
					/* 2A0 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* Result) noexcept(true) override;
					/* 2B0 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 2C0 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
				public:
					std::uint32_t data;	// 98 - DATA
					std::uint8_t pad9C[0x4];	// 9C
					void* snam;	// A0 - SNAM
					std::uint8_t dnam[0x10];	// A8 - DNAM
				};
				static_assert(offsetof(BGSAddonNode, data) == 0x98);
				static_assert(offsetof(BGSAddonNode, snam) == 0xA0);
				static_assert(offsetof(BGSAddonNode, dnam) == 0xA8);
				static_assert(sizeof(BGSAddonNode) == 0xB8);
			}
		}
	}
}