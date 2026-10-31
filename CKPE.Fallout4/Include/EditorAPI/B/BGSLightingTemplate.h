// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESFormIDListView.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSGodRays;

				struct BGSDirectionalAmbientLightingColors
				{
					std::uint32_t colorValues[7]; // 00
					float fresnelPower; // 1C
				};
				static_assert(sizeof(BGSDirectionalAmbientLightingColors) == 0x20);

				// The CK copy of INTERIOR_DATA stops after fogFarHeightRange (no interiorOffset): 0x88 bytes
				struct BGSLightingTemplateData
				{
					std::uint32_t ambient; // 00
					std::uint32_t directional; // 04
					std::uint32_t fogColorNear; // 08
					float fogNear; // 0C
					float fogFar; // 10
					std::uint32_t directionalXY; // 14
					std::uint32_t directionalZ; // 18
					float directionalFade; // 1C
					float clipDist; // 20
					float fogPower; // 24
					BGSDirectionalAmbientLightingColors directionalAmbientLightingColors; // 28
					std::uint32_t fogColorFar; // 48
					float fogClamp; // 4C
					float lightFadeStart; // 50
					float lightFadeEnd; // 54
					std::uint32_t lightingTemplateInheritanceFlags; // 58
					float fogHeightMid; // 5C
					float fogHeightRange; // 60
					std::uint32_t fogColorHighNear; // 64
					std::uint32_t fogColorHighFar; // 68
					float fogHighDensityScale; // 6C
					float fogNearColorScale; // 70
					float fogFarColorScale; // 74
					float fogHighNearColorScale; // 78
					float fogHighFarColorScale; // 7C
					float fogFarHeightMid; // 80
					float fogFarHeightRange; // 84
				};
				static_assert(sizeof(BGSLightingTemplateData) == 0x88);

				// 1.11.240.0: size 0xD8, vtable RVA 0x30A35E8, 126 virtual functions
				class BGSLightingTemplate :
					public TESFormIDListView   // 000
				{
				public:
					virtual ~BGSLightingTemplate() = default;

					/* 0A8 */ void InitializeData() override;
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

					constexpr static std::uint8_t TYPE_ID = ftLightingTemplate;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSLightingTemplate[0];
				public:
					BGSLightingTemplateData data; // 28 - INTERIOR_DATA
					BGSGodRays* godRays; // B0
					BGSDirectionalAmbientLightingColors directionalAmbientLightingColors; // B8
				};
				static_assert(sizeof(BGSLightingTemplate) == 0xD8);
			}
		}
	}
}
