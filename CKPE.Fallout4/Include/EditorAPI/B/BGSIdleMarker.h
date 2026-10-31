// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/B/BGSIdleCollection.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSKeyword;

				// 1.11.240.0: size 0x118, vtable RVA 0x3037160, 154 virtual functions
				class BGSIdleMarker :
					public TESBoundObject,     // 000
					public BGSKeywordForm,     // 090
					public TESModel,           // 0B0
					public BGSIdleCollection   // 0E8
				{
				public:
					virtual ~BGSIdleMarker() = default;

					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 190 */ [[nodiscard]] bool HasIgnoredBySandbox() const noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 408 */ void UnClone3D(void* pRequester) noexcept(true) override;
					/* 480 */ void Clone3D(TESObjectREFR* pRequester, NiAPI::NiPointer<NiAPI::NiAVObject>* pObj3D) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftIdleMarker;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSIdleMarker[0];
				public:
					BGSKeyword* animArchType; // 108
					BGSKeyword* flavorAnim; // 110
				};
				static_assert(sizeof(BGSIdleMarker) == 0x118);
			}
		}
	}
}
