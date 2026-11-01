// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESObjectACTI.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/T/TESModel.h>
#include <EditorAPI/B/BGSAttachParentArray.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESForm;

				// 1.11.246.0: size 0x8 (CommonLibF4 TESFurniture::EntryPointData)
				struct FurnitureEntryPointData
				{
					std::uint32_t entryPoint; // 00
					std::uint32_t flags; // 04
				};
				static_assert(sizeof(FurnitureEntryPointData) == 0x8);

				// 1.11.240.0: size 0x2A0, vtable RVA 0x308AC68, 154 virtual functions
				class TESFurniture :
					public TESObjectACTI   // 000
				{
				public:
					virtual ~TESFurniture() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 168 */ [[nodiscard]] bool HasObstacle() const noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2B8 */ void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 368 */ void CollectAdditionalResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true) override;
					/* 408 */ void UnClone3D(void* pRequester) noexcept(true) override;
					/* 450 */ bool GetAttachedMarkersVisible(void* pObj3D) const noexcept(true) override;
					/* 458 */ void SetAttachedMarkersVisible(void* pObj3D, bool bVisible) const noexcept(true) override;
					/* 480 */ void Clone3D(TESObjectREFR* pRequester, NiAPI::NiPointer<NiAPI::NiAVObject>* pObj3D) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftFurniture;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESFurniture[0];
				public:
					BSTArray<std::uint32_t> unk1E8; // 1E8 - 4-byte elements (Compare walks them as dwords), freed by ClearData, copied by CopyFrom; purpose not identified
					TESModel unk200; // 200
					BSTArray<FurnitureEntryPointData> entryPointDataArray; // 238 - cleared by ClearData (plain elements), by position like CommonLibF4
					BGSAttachParentArray attachParents; // 250
					std::int8_t wbData; // 270 - WorkbenchData::type, by position (same member order and sizes as the game after the last base)
					std::uint8_t unk271[0x3]; // 271
					std::uint32_t furnFlags; // 274 - by position (same member order and sizes as the game after the last base)
					TESForm* associatedForm; // 278 - by position (same member order and sizes as the game after the last base)
					// 0x20 byte entries (the array stride, found in the indexing code); member order as CommonLibF4's BSFurnitureMarker
					struct BSFurnitureMarker
					{
						NiAPI::NiPoint3 position; // 00
						std::uint32_t pad0C; // 0C
						void* actorMustHaveKeyword; // 10 - BGSKeyword*
						float heading; // 18
						std::uint32_t allowedAnimations; // 1C
					};
					static_assert(sizeof(BSFurnitureMarker) == 0x20);

					BSTArray<BSFurnitureMarker> markersArray; // 280
					std::uint16_t unk298; // 298
					std::uint8_t unk29A; // 29A
					std::uint8_t unk29B[0x5]; // 29B
				};
				static_assert(sizeof(TESFurniture) == 0x2A0);
			}
		}
	}
}
