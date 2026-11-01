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
				class TESGrass;
				class BGSTextureSet;

				class BGSMaterialType;

				struct TEXTURE_HAVOK_DATA
				{
					std::int8_t friction; // 00
					std::int8_t restitution; // 01
				};
				static_assert(sizeof(TEXTURE_HAVOK_DATA) == 0x2);

				// 1.11.240.0: size 0x70, vtable RVA 0x30ABCD8, 124 virtual functions
				class TESLandTexture :
					public TESForm   // 000
				{
				public:
					virtual ~TESLandTexture() = default;

					/* 0A8 */ void InitializeData() override;
					/* 0B0 */ void ClearData() override;
					/* 0B8 */ void Load(TESFile* File) override;
					/* 0D0 */ void Save(TESFile* File) override;
					/* 110 */ void InitItemImpl() noexcept(true) override;
					/* 200 */ std::uint32_t GetRefCount() const noexcept(true) override;
					/* 210 */ void CopyFrom(TESForm* Source) noexcept(true) override;
					/* 218 */ [[nodiscard]] std::int32_t Compare(TESForm* Rhs) const noexcept(true) override;
					/* 2A0 */ void RemoveReferencesToForm(TESForm* Form) noexcept(true) override;
					/* 2B0 */ void BuildCheckInDependencyMessage(const CheckInFormList* Forms, BSString& Message) const noexcept(true) override;
					/* 2B8 */ void GetReferencedForms(BSTBTree<TESForm*>* OutForms) const noexcept(true) override;
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftLandTexture;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESLandTexture[0];
				public:
					BGSTextureSet* textureSet; // 28
					TEXTURE_HAVOK_DATA havokData; // 30
					std::uint8_t unk32[0x6]; // 32
					BGSMaterialType* materialType; // 38
					std::int8_t specularExponent; // 40
					std::uint8_t unk41[0x3]; // 41
					std::int32_t shaderTextureIndex; // 44
					BSSimpleList<TESGrass*> textureGrassList; // 48 - BSSimpleList<TESGrass*>
					std::uint64_t unk58; // 58
					std::uint64_t unk60; // 60
					std::uint32_t unk68; // 68
					std::uint32_t unk6C; // 6C
				};
				static_assert(sizeof(TESLandTexture) == 0x70);
			}
		}
	}
}
