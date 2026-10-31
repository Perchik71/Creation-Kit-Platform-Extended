// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/B/BSTList.h>
#include <EditorAPI/N/NiColor.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESRegionData;
				class TESRegionPointList;
				class TESWorldSpace;
				class TESWeather;

				// 1.11.246.0: size 0x68, vtable RVA 0x302EB90, 126 virtual functions
				class TESRegion :
					public TESForm   // 000
				{
				public:
					virtual ~TESRegion() = default;

					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2A8 */ [[nodiscard]] bool CanCheckIn(const CheckInFormList* Forms) const noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2E0 */ bool Revert() noexcept(true) override;
					/* 2E8 */ void OnFormIDsRemapped(BSTHashMap<std::uint32_t, std::uint32_t>* FormIDMap) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftRegion;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESRegion[0];

					virtual TESRegion* CloneRegion(TESRegion* Target) const noexcept(true) { return nullptr; } // 124
					virtual bool Validate() noexcept(true) { return false; } // 125

				public:
					std::uint32_t mapColor; // 28 - RCLR, 0xC80A8A on construction
					std::uint32_t pad2C; // 2C
					BSSimpleList<TESRegionData*>* dataList; // 30 - saved through each entry's virtual 01
					BSSimpleList<TESRegionPointList*>* pointLists; // 38 - 56 byte point lists freed by ClearData
					TESWorldSpace* worldSpace; // 40 - WNAM
					TESWeather* currentWeather; // 48 - by position like CommonLibF4
					NiAPI::NiColor emittanceColor; // 50
					float lodDistanceModifier; // 5C
					float occlusionAccuracy; // 60
					std::uint32_t pad64; // 64
				};
				static_assert(offsetof(TESRegion, worldSpace) == 0x40);
				static_assert(offsetof(TESRegion, emittanceColor) == 0x50);
				static_assert(sizeof(TESRegion) == 0x68);
			}
		}
	}
}
