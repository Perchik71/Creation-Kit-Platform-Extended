// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BGSStoryManagerTreeForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BSTHashMap.h>
#include <EditorAPI/B/BGSBaseAlias.h>
#include <EditorAPI/T/TESCondition.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSLocation;
				class BGSBaseAlias;
				class TESQuestStage;
				class BGSQuestObjective;
				class BGSDialogueBranch;
				class BGSScene;
				class TESGlobal;
				class TESTopic;

				struct QUEST_DATA
				{
					float questDelayTime; // 00
					std::uint16_t flags; // 04
					std::int8_t priority; // 06
					std::int8_t questType; // 07
				};
				static_assert(sizeof(QUEST_DATA) == 0x8);

				// 1.11.246.0: size 0x270, vtable RVA 0x304B590, 129 virtual functions
				class TESQuest :
					public BGSStoryManagerTreeForm,  // 000
					public TESFullName               // 028
				{
				public:
					virtual ~TESQuest() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0C0 */ void LoadPartial(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 0D8 */ bool SavePlugin(TESFile* File) override;
					/* 0E8 */ bool SaveForm(TESFile* File) override;
					/* 0F0 */ [[nodiscard]] bool IsBeforeGroup(const TESGroupHeader* GroupRecord) const noexcept(true) override;
					/* 0F8 */ [[nodiscard]] bool IsBeforeForm(TESForm* Rhs) const noexcept(true) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 240 */ [[nodiscard]] bool IsParentForm() const noexcept(true) override;
					/* 250 */ [[nodiscard]] bool IsFormTypeChild(FormType Type) const noexcept(true) override;
					/* 270 */ [[nodiscard]] bool HasAlteredChildren() const noexcept(true) override;
					/* 278 */ void GetObjectWindowTreePath(BSString& Path) const noexcept(true) override;
					/* 280 */ bool RemoveChildFormsFromList(CheckInFormList* Forms) noexcept(true) override;
					/* 288 */ bool CanCheckOutChildren() noexcept(true) override;
					/* 290 */ bool CheckOutChildren(TESBitArrayFile* UserFile, TESBitArrayFile* VersionFile) noexcept(true) override;
					/* 298 */ bool MarkChildFormsAsDeleted(bool bValue, TESBitArrayFile* UserFile, TESBitArrayFile* VersionFile) noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2E0 */ bool Revert() noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 338 */ void OnDialogFormSaved(HWND hDialogBox, TESForm* SavedForm) noexcept(true) override;
					/* 348 */ void FillListViewItem(NMLVDISPINFOA* pDispInfo) const noexcept(true) override;
					/* 358 */ void InitializeListView(HWND hListView) const noexcept(true) override;
					/* 368 */ void CollectAdditionalResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true) override;
					/* 380 */ [[nodiscard]] std::int32_t GetSavedDialogTab(HWND hDialogBox) const noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftQuest;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESQuest[0];

					virtual void* GetConditions() noexcept(true) { return nullptr; } // 128

				public:
					BSTArray<BGSBaseAlias*> aliases; // 40 - polymorphic objects, destroyed through their virtual destructor by ClearData
					std::uint32_t nextAliasID; // 58 - ANAM
					std::uint32_t unk5C; // 5C
					BGSLocation* nonDormantLocation; // 60 - LNAM
					TESGlobal* questCompleteXPGlobal; // 68 - 0 on construction, by position like CommonLibF4
					BSFixedString swfFile; // 70 - SNAM
					std::uint64_t filter; // 78 - FLTR chunk (CK only), string
					std::uint32_t unk80; // 80
					std::uint32_t unk84; // 84
					std::uint64_t unk88; // 88 - only cleared by the constructor
					std::uint64_t unk90; // 90 - only cleared by the constructor
					std::uint64_t unk98; // 98 - reference counted object (the count is its first dword), compared by Compare when unkA0 or its twin is set
					std::uint8_t unkA0; // A0
					std::uint8_t unkA1[0x3]; // A1
					QUEST_DATA data; // A4
					std::uint32_t eventID; // AC - ENAM
					BSTArray<TESQuestStage*> stages; // B0 - 0x28 byte heap objects
					BSTArray<BGSQuestObjective*> objectives; // C8 - 0x30 byte heap objects (the game's order: stages, objectives)
					TESCondition objConditions; // E0
					TESCondition storyManagerConditions; // F8
					// Two BSTHashMap<BGSDialogueBranch*, BSTArray<TESTopic*>*> in the CK's 0x38 byte layout (CommonLibF4's branchedDialogues; the CK has no
					// stage/objective/alias index tables). The constructor sets the end of chain markers at 128 and 160.
					BSTHashMap<BGSDialogueBranch*, BSTArray<TESTopic*>*> branchedDialogues[2]; // 110
					BSTArray<TESTopic*> topics[6]; // 180 - the destructor loops over 6 arrays
					BSTArray<BGSScene*> scenes; // 210 - by position like CommonLibF4's scenes
					BSTArray<TESGlobal*>* textGlobal; // 228
					Forms::TESForm* unk230; // 230 - GNAM chunk (its form id is saved); the form is told about the reference through TESForm::AddReference, compared by Compare, copied by CopyFrom
					std::uint8_t unk238; // 238 - when set Save writes only the record header and flags the record
					std::uint8_t unk239[0x7]; // 239
					std::uint64_t unk240; // 240 - only cleared by the constructor
					BSTArray<std::uint32_t> unk248; // 248 - 4 byte elements: the array destructor is the instantiation shared with the other 4 byte handle arrays
					std::uint64_t unk260; // 260 - only cleared by the constructor
					std::uint64_t unk268; // 268 - only cleared by the constructor
				};
				static_assert(offsetof(TESQuest, stages) == 0xB0);
				static_assert(offsetof(TESQuest, aliases) == 0x40);
				static_assert(offsetof(TESQuest, nextAliasID) == 0x58);
				static_assert(offsetof(TESQuest, branchedDialogues) == 0x110);
				static_assert(offsetof(TESQuest, topics) == 0x180);
				static_assert(offsetof(TESQuest, textGlobal) == 0x228);
				static_assert(sizeof(TESQuest) == 0x270);
			}
		}
	}
}
