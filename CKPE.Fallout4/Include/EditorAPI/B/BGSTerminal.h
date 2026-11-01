// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESFurniture.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BGSLocalizedString.h>
#include <EditorAPI/T/TESTexture.h>
#include <EditorAPI/T/TESCondition.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x330, vtable RVA 0x2FBF938, 154 virtual functions
				class BGSTerminal :
					public TESFurniture   // 000
				{
				public:
					virtual ~BGSTerminal() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2B8 */ void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 338 */ void OnDialogFormSaved(HWND hDialogBox, TESForm* SavedForm) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftTerminal;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSTerminal[0];
				public:
					// Body text entry (0x28 bytes, the array stride)
					struct BodyTextItem
					{
						BGSLocalizedString itemText; // 00
						TESCondition conditions; // 10
					};
					static_assert(sizeof(BodyTextItem) == 0x28);

					BSTArray<BodyTextItem> bodyTextItems; // 2A0
					// Menu entry (0x50 bytes, the array stride); member order as CommonLibF4's MenuItem with the CK's 0x10 byte BGSLocalizedString
					struct MenuItem
					{
						BGSLocalizedString itemText; // 00
						BGSLocalizedString responseText; // 10
						void* selectionResult; // 20 - sub menu, display text, display image or holotape depending on the flags
						TESCondition conditions; // 28 - TerminalItemCondition
						std::uint16_t id; // 40 - 0xFFFF on construction
						std::uint8_t flags; // 42
						std::uint8_t pad43[0x5]; // 43
						std::uint64_t unk48; // 48 - cleared on construction
					};
					static_assert(sizeof(MenuItem) == 0x50);

					BSTArray<MenuItem> menuItems; // 2B8
					BGSLocalizedString headerTextOverride; // 2D0
					BGSLocalizedString welcomeText; // 2E0
					std::uint64_t unk2F0; // 2F0
					std::uint64_t unk2F8; // 2F8
					std::uint16_t unk300; // 300
					std::uint16_t unk302; // 302
					std::uint32_t unk304; // 304
					TESTexture unk308; // 308
				};
				static_assert(sizeof(BGSTerminal) == 0x330);
			}
		}
	}
}
