// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/N/NiColor.h>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/T/TESIcon.h>
#include <EditorAPI/B/BGSMessageIcon.h>
#include <EditorAPI/T/TESWeightForm.h>
#include <EditorAPI/T/TESValueForm.h>
#include <EditorAPI/B/BGSDestructibleObjectForm.h>
#include <EditorAPI/B/BGSEquipType.h>
#include <EditorAPI/B/BGSPropertySheet.h>
#include <EditorAPI/F/FormInterfaces.h>
#include <EditorAPI/B/BGSKeywordForm.h>
#include <EditorAPI/T/TESTexture.h>
#include <CKPE.Common.h>
#include <EditorAPI/N/NiTypes.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				struct OBJ_LIGH
				{
					std::int32_t time; // 00
					std::uint32_t radius; // 04
					std::uint32_t color; // 08
					std::uint32_t flags; // 0C - REX::TEnumSet<Flag, std::uint32_t>
					float fallOffExponent; // 10
					float fov; // 14
					float nearDistance; // 18
					float flickerPeriodRecip; // 1C
					float flickerIntensityAmplitude; // 20
					float flickerMovementAmplitude; // 24
					float attenConstant; // 28
					float attenScalar; // 2C
					float attenExponent; // 30
					float godrayNearClipDistance; // 34
				};
				static_assert(sizeof(OBJ_LIGH) == 0x38);

				class BGSSoundDescriptorForm;
				class BGSLensFlare;
				class BGSGodRays;

				// 1.11.240.0: size 0x268, vtable RVA 0x308F650, 154 virtual functions
				class TESObjectLIGH :
					public TESBoundAnimObject,         // 000
					public TESFullName,                // 090
					public BGSModelMaterialSwap,       // 0A8
					public TESIcon,                    // 0F8
					public BGSMessageIcon,             // 120
					public TESWeightForm,              // 150
					public TESValueForm,               // 160
					public BGSDestructibleObjectForm,  // 170
					public BGSEquipType,               // 180
					public BGSPropertySheet,           // 190
					public BGSOpenCloseForm,           // 1A0
					public BGSKeywordForm              // 1A8
				{
				public:
					virtual ~TESObjectLIGH() = default;

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
					/* 338 */ void OnDialogFormSaved(HWND hDialogBox, TESForm* SavedForm) noexcept(true) override;
					/* 370 */ void CollectRequiredFiles(BSTBTree<TESForm*>* VisitedForms, BSTBTree<BSFixedString>* RequiredFiles) noexcept(true) override;
					/* 3D8 */ [[nodiscard]] float GetDesirability() const noexcept(true) override;
					/* 408 */ void UnClone3D(void* pRequester) noexcept(true) override;
					/* 420 */ bool ReplaceModel() noexcept(true) override;
					/* 438 */ void* LoadGraphics(void* pRequester) noexcept(true) override;
					/* 480 */ void Clone3D(TESObjectREFR* pRequester, NiAPI::NiPointer<NiAPI::NiAVObject>* pObj3D) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftLight;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESObjectLIGH[0];

					[[nodiscard]] inline NiAPI::NiRGB GetSpecularColor() const noexcept(true) { return *reinterpret_cast<const NiAPI::NiRGB*>(&data.color); }

					CKPE_READ_PROPERTY(GetSpecularColor) NiAPI::NiRGB SpecularColor;

				public:
					std::uint8_t unk1C8[0x10]; // 1C8
					OBJ_LIGH data; // 1D8 - DATA (the 3 bytes at 0x1E0 were named m_SpecularColor: they are data.color)
					float fade; // 210 - FNAM
					std::uint32_t pad214; // 214
					BGSSoundDescriptorForm* sound; // 218 - SNAM
					TESTexture goboTexture; // 220 - NAM0
					NiAPI::NiColor emittanceColor; // 248 - by position after goboTexture
					std::uint32_t pad254; // 254
					BGSLensFlare* lensFlare; // 258 - LNAM
					BGSGodRays* godRays; // 260 - WGDR
				};
				static_assert(sizeof(TESObjectLIGH) == 0x268);
			}
		}
	}
}
