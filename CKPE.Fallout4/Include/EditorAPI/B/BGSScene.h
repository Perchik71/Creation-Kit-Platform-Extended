// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/F/FormInterfaces.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSString.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BGSKeywordForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESQuest;
				class TESTopic;
				class BGSKeyword;
				class BGSSoundOutput;

				class BGSSceneAction;
					class BGSScene;

				// 1.11.246.0: size 0x80, created per HNAM chunk (constructor 0x140A56D60); the game's BGSScenePhase is 0x18 bytes. The two
				// condition lists are the start and completion conditions, in the game's order.
				struct BGSScenePhase
				{
					std::uint64_t unk00; // 00 - only cleared by the constructor
					std::uint64_t unk08; // 08 - only cleared by the constructor
					std::uint16_t unk10; // 10 - only cleared by the constructor
					std::uint8_t pad12[0x6]; // 12
					BSString name; // 18 - NAM0 (set from a 256 byte buffer, cleared by the constructor)
					std::uint32_t editorWidth; // 28 - WNAM (4 bytes), initialised from a global default
					std::int32_t index; // 2C - -1 on construction; Load sets it to the scene's phases.size before appending
					BGSScene* owner; // 30 - Load stores the scene here (the embedded phase gets it from the BGSScene constructor)
					std::uint64_t unk38; // 38 - only cleared by the constructor
					std::int16_t scqsStage0; // 40 - SCQS, first word; -1 on construction
					std::int16_t scqsStage1; // 42 - SCQS, second word; -1 on construction
					std::uint8_t pad44[0x4]; // 44
					std::uint8_t startConditions[0x18]; // 48 - TESCondition (0x18 bytes in the CK)
					std::uint8_t completionConditions[0x18]; // 60 - TESCondition
					std::uint16_t flags; // 78 - FNAM (2 bytes)
					std::uint8_t pad7A[0x6]; // 7A
				};
				static_assert(offsetof(BGSScenePhase, name) == 0x18);
				static_assert(offsetof(BGSScenePhase, editorWidth) == 0x28);
				static_assert(offsetof(BGSScenePhase, flags) == 0x78);
				static_assert(offsetof(BGSScenePhase, startConditions) == 0x48);
				static_assert(offsetof(BGSScenePhase, completionConditions) == 0x60);
				static_assert(sizeof(BGSScenePhase) == 0x80);

				// 1.11.240.0: size 0x230, vtable RVA 0x303DEA8, 124 virtual functions
				class BGSScene :
					public TESForm,           // 000
					public IKeywordFormBase   // 028
				{
				public:
					virtual ~BGSScene() = default;

					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 0F0 */ [[nodiscard]] bool IsBeforeGroup(const TESGroupHeader* GroupRecord) const noexcept(true) override;
					/* 0F8 */ [[nodiscard]] bool IsBeforeForm(TESForm* Rhs) const noexcept(true) override;
					/* 100 */ TESForm* CreateDuplicateForm(bool bCreateEditorID, BSTHashMap<TESForm*, TESForm*>* CopyMap) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 1A0 */ void MarkAsChanged(bool bValue) noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 220 */ bool BelongsInGroup(TESGroupHeader* Group, bool bAllowParentGroups, bool bCurrentOnly) override;
					/* 228 */ void CreateGroupData(TESGroupHeader* OutGroup, TESGroupHeader* ParentGroup) override;
					/* 288 */ bool CanCheckOutChildren() noexcept(true) override;
					/* 298 */ bool MarkChildFormsAsDeleted(bool bValue, TESBitArrayFile* UserFile, TESBitArrayFile* VersionFile) noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 338 */ void OnDialogFormSaved(HWND hDialogBox, TESForm* SavedForm) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftScene;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSScene[0];
				public:
					std::uint32_t unk30; // 30
					std::uint32_t unk34; // 34
					BSTArray<BGSScenePhase*> phases; // 38 - HNAM chunks create the 0x80 byte phases
					BSTArray<std::uint32_t> actors; // 50
					BSTArray<std::uint32_t> actorFlags; // 68
					BSTArray<std::uint32_t> actorProgressionFlags; // 80
					BSTArray<BGSSceneAction*> actions; // 98
					TESQuest* parentQuest; // B0
					BGSScene* templateScene; // B8
					std::uint32_t flags; // C0
					std::uint32_t unkC4; // C4
					std::uint8_t repeatConditions[0x18]; // C8 - TESCondition (0x18 in the CK)
					BGSScenePhase phase; // E0 - constructed in place by the BGSScene constructor (0x80 bytes, index -1, owner = this)
					// Embedded BGSSceneActionDialogue (0x160-0x1E7, vtable BGSSceneActionDialogue installed by the constructor). The CK scene action base
					// is 0x30 bytes (the game's is 0x20); the dialogue members then follow CommonLibF4's BGSSceneActionDialogue (topic, loop times,
					// head track targets, loop timer, keywords, output model) shifted by 0x170.
					void* dialogueActionVTable; // 160
					std::uint64_t unk168; // 168
					std::uint32_t unk170; // 170
					std::uint32_t unk174; // 174
					std::uint64_t unk178; // 178
					std::int32_t unk180; // 180 - -1 on construction
					std::uint8_t unk184[0xC]; // 184
					TESTopic* dialogueTopic; // 190
					float maxLoopTime; // 198 - 10.0 on construction
					float minLoopTime; // 19C - 1.0 on construction
					BSTArray<std::uint32_t> headTrackTargets; // 1A0 - ObjectRefHandle
					float currentLoopTimer; // 1B8
					std::uint32_t unk1BC; // 1BC
					BGSKeyword* animFaceArchType; // 1C0
					BGSKeyword* subtypeKeyword; // 1C8
					BGSSoundOutput* outputModel; // 1D0
					BSFixedString notes; // 1D8 - NNAM string
					std::uint16_t xnam; // 1E0 - XNAM (2 bytes)
					std::uint8_t unk1E2; // 1E2
					std::uint8_t unk1E3[0x5]; // 1E3
					BGSKeywordForm* keywordForm; // 1E8 - allocated in the constructor (KSIZ chunk)
					std::int16_t scqsStage0; // 1F0 - SCQS, first word; -1 on construction
					std::int16_t scqsStage1; // 1F2 - SCQS, second word; -1 on construction
					std::uint32_t vnam0; // 1F4 - VNAM (20 byte chunk; its first four dwords go to 1F4-200 and INAM supplies 204); 3 on construction. 1F4-207 is one block that CopyFrom copies and Compare tests
					std::uint32_t vnam1; // 1F8 - VNAM, second dword; 3 on construction
					std::uint32_t vnam2; // 1FC - VNAM, third dword; 3 on construction
					std::uint32_t vnam3; // 200 - VNAM, fourth dword; 3 on construction
					std::uint32_t inam; // 204 - INAM (4 bytes); 0 on construction
					void* unk208; // 208 - pointee holds a smart pointer at +0x48 that Compare reads, skipped when unk228 is set
					void* unk210; // 210 - as 208, skipped when unk229 is set
					std::uint64_t unk218; // 218 - null on construction
					void* unk220; // 220 - reference counted object (count at +0), read by Compare
					std::uint8_t unk228; // 228 - 0 on construction
					std::uint8_t unk229; // 229
					std::uint8_t unk22A[0x6]; // 22A
				};
				static_assert(offsetof(BGSScene, phase) == 0xE0);
				static_assert(offsetof(BGSScene, dialogueActionVTable) == 0x160);
				static_assert(offsetof(BGSScene, headTrackTargets) == 0x1A0);
				static_assert(offsetof(BGSScene, keywordForm) == 0x1E8);
				static_assert(sizeof(BGSScene) == 0x230);
			}
		}
	}
}
