// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESDescription.h>
#include <EditorAPI/B/BGSLocalizedString.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x1A8, vtable RVA 0x2FF4C90, 124 virtual functions
				class ActorValueInfo :
					public TESForm,         // 000
					public TESFullName,     // 028
					public TESDescription   // 040
				{
				public:
					virtual ~ActorValueInfo() = default;

					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 268 */ [[nodiscard]] bool CanReferenceDuringCheckIn(TESForm* ReferencingForm, const CheckInFormList* Forms) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftActorValueInfo;
					constexpr static Common::ID VTABLE = IDs::VTABLE::ActorValueInfo[0];
				public:
					ActorValueInfo* dependentActorValues[15]; // 68
					std::uint8_t derivationFunction[0x40]; // E0 - std::function<float(const ActorValueOwner*, const ActorValueInfo&)> (MSVC layout: 0x38 small-object storage + impl pointer)
					const char* enumNames[10]; // 120
					BGSLocalizedString abbreviation; // 170
					std::int32_t oldActorValue; // 180
					std::int32_t flags; // 184 - REX::TEnumSet<ActorValue::Flags, std::int32_t>
					std::int32_t avType; // 188 - REX::TEnumSet<ActorValue::AVType, std::int32_t>
					std::uint32_t numDependentActorValues; // 18C
					std::uint32_t enumCount; // 190
					std::int32_t fullCacheIndex; // 194
					std::int32_t permanentCacheIndex; // 198
					float defaultValue; // 19C
					std::uint32_t sortIndex; // 1A0
					std::uint32_t unk1A4; // 1A4
				};
				static_assert(sizeof(ActorValueInfo) == 0x1A8);
			}
		}
	}
}
