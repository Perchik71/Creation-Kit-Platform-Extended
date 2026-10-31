// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/N/NiColor.h>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/T/TESTexture.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESImageSpace;

				class BGSSoundDescriptorForm;

				class SpellItem;

				class BGSMaterialType;

				struct WaterShaderData
				{
					float maxDepth; // 00
					std::uint32_t shallowWaterColor; // 04
					std::uint32_t deepWaterColor; // 08
					float shallowFogColorRange; // 0C
					float deepFogColorRange; // 10
					float shallowAlpha; // 14
					float deepAlpha; // 18
					float shallowFogAlphaRange; // 1C
					float deepFogAlphaRange; // 20
					std::uint32_t underwaterFogColor; // 24
					float underwaterFogAmount; // 28
					float underwaterFogDistNear; // 2C
					float underwaterFogDistFar; // 30
					float normalMagnitude; // 34
					float normalFalloffShallow; // 38
					float normalFalloffDeep; // 3C
					float reflectionAmount; // 40
					float fresnelAmount; // 44
					float surfaceEffectFalloff; // 48
					float displacementForce; // 4C
					float displacementVelocity; // 50
					float displacementFalloff; // 54
					float displacementDampener; // 58
					float displacementSize; // 5C
					std::uint32_t reflectionWaterColor; // 60
					float sunSpecularPower; // 64
					float sunSpecularMagnitude; // 68
					float sunSparklePower; // 6C
					float sunSparkleMagnitude; // 70
					float lightRadius; // 74
					float lightBrightness; // 78
					float shininess; // 7C
					float noiseWindDirections[3]; // 80
					float noiseWindSpeeds[3]; // 8C
					float amplitude[3]; // 98
					float uvScales[3]; // A4
					float noiseFalloff[3]; // B0
					float siltAmount; // BC
					std::uint32_t lightSiltColor; // C0
					std::uint32_t darkSiltColor; // C4
					bool ssrEnabled; // C8
				};
				static_assert(sizeof(WaterShaderData) == 0xCC);

				// 1.11.240.0: size 0x290, vtable RVA 0x30B8570, 124 virtual functions
				class TESWaterForm :
					public TESForm,      // 000
					public TESFullName   // 028
				{
				public:
					virtual ~TESWaterForm() = default;

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
					/* 308 */ bool PerformDialogBox(HWND hDialogBox, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult) noexcept(true) override;
					/* 318 */ void WriteDataToDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 328 */ void ReadDataFromDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 330 */ void CleanupDialogBox(HWND hDialogBox) noexcept(true) override;
					/* 368 */ void CollectAdditionalResourceFiles(BSTArray<BSFixedString>& Files) noexcept(true) override;

					constexpr static std::uint8_t TYPE_ID = ftWater;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESWaterForm[0];
				public:
					bool needUpdate; // 40
					std::uint8_t unk41[0x3]; // 41
					NiAPI::NiColorA texScroll0; // 44 - zeroed by the constructor
					NiAPI::NiColorA texScroll1; // 54
					NiAPI::NiColorA texScroll2; // 64
					std::uint8_t pad74[0x4]; // 74
					TESTexture noiseTexture0; // 78
					TESTexture noiseTexture1; // A0
					TESTexture noiseTexture2; // C8
					std::int8_t alpha; // F0 - 75 on construction
					std::int8_t flags; // F1
					std::uint8_t padF2[0x6]; // F2
					BGSMaterialType* materialType; // F8
					BGSSoundDescriptorForm* waterSound; // 100
					WaterShaderData data; // 108 - DATA
					std::uint8_t unk1D4[0x4]; // 1D4
					std::uint64_t unk1D8; // 1D8
					std::uint8_t unk1E0; // 1E0
					std::uint8_t unk1E1[0x7]; // 1E1
					TESWaterForm* waterWeatherControl[3]; // 1E8
					std::int32_t currentTextureSelect01; // 200
					std::int32_t currentTextureSelect02; // 204
					std::uint32_t frequencyX; // 208 - 9 on construction
					std::uint32_t frequencyY; // 20C - 9 on construction
					std::int32_t octaves; // 210 - 4 on construction
					float amplitude; // 214 - 0.5 on construction
					float lacunarity; // 218 - 2.0 on construction
					float bias; // 21C - 0.5 on construction
					float gain; // 220 - 0.5 on construction
					std::uint32_t pad224; // 224
					SpellItem* consumeSpell; // 228
					SpellItem* contactSpell; // 230
					void* niNoiseTexture0; // 238 - NiPointer<NiTexture>
					void* niNoiseTexture1; // 240 - NiPointer<NiTexture>
					void* niNoiseTexture2; // 248 - NiPointer<NiTexture>
					void* placeableAutoWater; // 250 - TESObjectACTI*
					void* placeableLODWater; // 258 - TESObjectACTI*
					void* waterShaderMaterial; // 260 - BSWaterShaderMaterial*
					bool resetNoiseTextures; // 268
					std::uint8_t pad269[0x7]; // 269
					TESImageSpace* imageSpace; // 270
					NiAPI::NiPoint3 linearVelocity; // 278
					NiAPI::NiPoint3 angularVelocity; // 284
				};
				static_assert(offsetof(TESWaterForm, texScroll0) == 0x44);
				static_assert(offsetof(TESWaterForm, noiseTexture2) == 0xC8);
				static_assert(offsetof(TESWaterForm, alpha) == 0xF0);
				static_assert(offsetof(TESWaterForm, currentTextureSelect01) == 0x200);
				static_assert(offsetof(TESWaterForm, gain) == 0x220);
				static_assert(offsetof(TESWaterForm, niNoiseTexture0) == 0x238);
				static_assert(offsetof(TESWaterForm, resetNoiseTextures) == 0x268);
				static_assert(sizeof(TESWaterForm) == 0x290);
			}
		}
	}
}
