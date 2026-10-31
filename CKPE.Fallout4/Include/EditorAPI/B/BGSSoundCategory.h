// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/F/FormInterfaces.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x88, vtable RVA 0x3061CF8, 124 virtual functions
				class BGSSoundCategory :
					public TESForm,           // 000
					public TESFullName,       // 028
					public BSISoundCategory   // 040
				{
				public:
					virtual ~BGSSoundCategory() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftSoundCategory;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSSoundCategory[0];
				public:
					BGSSoundCategory* parentCategory; // 48
					BGSSoundCategory* sliderCategory; // 50
					float volumeMult; // 58
					float snapshotMult; // 5C
					float currCompressionMult; // 60
					float fullyCompressedMult; // 64
					float frequencyMult; // 68
					float minFrequencyMult; // 6C
					std::uint32_t appFlags; // 70
					std::uint16_t attenuation[6]; // 74
					std::uint16_t stateFlags; // 80
					std::uint16_t staticMult; // 82
					std::uint16_t defaultMenuValue; // 84
					std::uint16_t unk86; // 86
				};
				static_assert(sizeof(BGSSoundCategory) == 0x88);
			}
		}
	}
}
