// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESObjectACTI.h>
#include <EditorAPI/T/TESObjectREFR.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSVoiceType;

				// 1.11.240.0: size 0x1F8, vtable RVA 0x3086378, 155 virtual functions
				class BGSTalkingActivator :
					public TESObjectACTI   // 000
				{
				public:
					virtual ~BGSTalkingActivator() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 158 */ [[nodiscard]] bool HasDangerous() const noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2B8 */ void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 468 */ void SetObjectVoiceType(BGSVoiceType* pVoiceType) noexcept(true) override;
					/* 470 */ BGSVoiceType* GetObjectVoiceType() const noexcept(true) override;
					/* 4A0 */ void CollectVoiceTypes(BSSimpleList<BGSVoiceType*>* VoiceTypes) const noexcept(true) override;
					// Plus 1 virtual(s) beyond the base class vtable (155 slots, base 154), not named yet.

					constexpr static std::uint8_t TYPE_ID = ftTalkingActivator;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSTalkingActivator[0];

					virtual bool GetReflectedByAutoWater() noexcept(true) { return false; } // 154 - RVA 0xC2E650, return 0;
				public:
					TESObjectREFR* tempRef; // 1E8
					BGSVoiceType* voiceType; // 1F0
				};
				static_assert(sizeof(BGSTalkingActivator) == 0x1F8);
			}
		}
	}
}
