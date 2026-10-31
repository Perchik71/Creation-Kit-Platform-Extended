// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/N/NiColor.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSTextureSet;

				struct SplineData
				{
					float numTiles; // 00
					std::uint16_t numSlices; // 04
					std::uint16_t flags; // 06
					NiAPI::NiColor color; // 08
					float windSensibility; // 14
					float flexibility; // 18
				};
				static_assert(sizeof(SplineData) == 0x1C);

				// 1.11.240.0: size 0xB8, vtable RVA 0x2FBD638, 154 virtual functions
				class BGSBendableSpline :
					public TESBoundObject   // 000
				{
				public:
					virtual ~BGSBendableSpline() = default;

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

					constexpr static std::uint8_t TYPE_ID = ftBendableSpline;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSBendableSpline[0];
				public:
					SplineData data; // 90
					std::uint32_t unkAC; // AC
					BGSTextureSet* textureSet; // B0
				};
				static_assert(sizeof(BGSBendableSpline) == 0xB8);
			}
		}
	}
}
