// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BSTList.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESQuest;

				struct SCRIPT_HEADER
				{
					std::uint32_t variableCount; // 00
					std::uint32_t refObjectCount; // 04
					std::uint32_t dataSize; // 08
					std::uint32_t lastID; // 0C
					bool isQuestScript; // 10
					bool isMagicEffectScript; // 11
					bool isCompiled; // 12
				};
				static_assert(sizeof(SCRIPT_HEADER) == 0x14);

				// 1.11.246.0: size 0x78, vtable RVA 0x3067FB8, 127 virtual functions
				class Script :
					public TESForm   // 000
				{
				public:
					virtual ~Script() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 0E8 */ bool SaveForm(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 3A0 */ [[nodiscard]] const char* GetObjectTypeName() const noexcept(true) override;
					// Plus 3 virtual(s) beyond the base class vtable (127 slots, base 124), not named yet.

					constexpr static std::uint8_t TYPE_ID = ftScript;
					constexpr static Common::ID VTABLE = IDs::VTABLE::Script[0];

					[[nodiscard]] virtual bool CanCheckInReferencedForms(TESForm* Owner, const void* Forms) const noexcept(true) { return true; } // 124
					virtual void BuildReferencedFormsCheckInMessage(TESForm* Owner, const void* Forms,
						BSString& Message) const noexcept(true) { return; } // 125
					virtual Script* OpenScriptEditor(HWND hParentWnd) noexcept(true) { return this; } // 126
				public:
					SCRIPT_HEADER header; // 28
					std::uint32_t unk3C; // 3C
					char* text; // 40
					std::byte* data; // 48
					TESQuest* parentQuest; // 50
					BSSimpleList<void*> refObjects; // 58 - BSSimpleList<SCRIPT_REFERENCED_OBJECT*>
					BSSimpleList<void*> variables; // 68 - BSSimpleList<ScriptVariable*>
				};
				static_assert(sizeof(Script) == 0x78);
			}
		}
	}
}
