// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSShaderMaterial.h>
#include <EditorAPI/N/NiColor.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSLightingShaderMaterial : public BSLightingShaderMaterialBase
{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::BSLightingShaderMaterial;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSLightingShaderMaterial[0];
			};
			static_assert(sizeof(BSLightingShaderMaterial) == 0xC0);

			class BSLightingShaderMaterialEnvmap : public BSLightingShaderMaterialBase
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::BSLightingShaderMaterialEnvmap;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSLightingShaderMaterialEnvmap[0];

				NiAPI::NiPointer<NiAPI::NiTexture> EnvironmentTexture;	// C0
				NiAPI::NiPointer<NiAPI::NiTexture> EnvironmentMaskTexture;	// C8
				float EnvironmentMapScale;							// D0
				bool ScreenSpaceReflections;						// D4
				bool AlphaTest;										// D5
				std::uint16_t PadD6;								// D6
			};
			static_assert(sizeof(BSLightingShaderMaterialEnvmap) == 0xD8);
			static_assert(offsetof(BSLightingShaderMaterialEnvmap, EnvironmentTexture) == 0xC0);
			static_assert(offsetof(BSLightingShaderMaterialEnvmap, EnvironmentMapScale) == 0xD0);

			class BSLightingShaderMaterialGlowmap : public BSLightingShaderMaterialBase
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::BSLightingShaderMaterialGlowmap;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSLightingShaderMaterialGlowmap[0];

				NiAPI::NiPointer<NiAPI::NiTexture> GlowTexture;		// C0
			};
			static_assert(sizeof(BSLightingShaderMaterialGlowmap) == 0xC8);
			static_assert(offsetof(BSLightingShaderMaterialGlowmap, GlowTexture) == 0xC0);

			class BSLightingShaderMaterialParallax : public BSLightingShaderMaterialBase
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::BSLightingShaderMaterialParallax;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSLightingShaderMaterialParallax[0];

				NiAPI::NiPointer<NiAPI::NiTexture> HeightTexture;		// C0
			};
			static_assert(sizeof(BSLightingShaderMaterialParallax) == 0xC8);
			static_assert(offsetof(BSLightingShaderMaterialParallax, HeightTexture) == 0xC0);

			class BSLightingShaderMaterialParallaxOcc : public BSLightingShaderMaterialBase
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::BSLightingShaderMaterialParallaxOcc;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSLightingShaderMaterialParallaxOcc[0];

				NiAPI::NiPointer<NiAPI::NiTexture> HeightTexture;		// C0
				float MaxPasses;									// C8
				float Scale;										// CC
			};
			static_assert(sizeof(BSLightingShaderMaterialParallaxOcc) == 0xD0);
			static_assert(offsetof(BSLightingShaderMaterialParallaxOcc, MaxPasses) == 0xC8);

			class BSLightingShaderMaterialDismemberment : public BSLightingShaderMaterialBase
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::BSLightingShaderMaterialDismemberment;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSLightingShaderMaterialDismemberment[0];

				NiAPI::NiPointer<NiAPI::NiTexture> DisplacementTexture;	// C0
			};
			static_assert(sizeof(BSLightingShaderMaterialDismemberment) == 0xC8);
			static_assert(offsetof(BSLightingShaderMaterialDismemberment, DisplacementTexture) == 0xC0);

			class BSLightingShaderMaterialSkinTint : public BSLightingShaderMaterialBase
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::BSLightingShaderMaterialSkinTint;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSLightingShaderMaterialSkinTint[0];

				NiAPI::NiColorA TintColor;							// C0
			};
			static_assert(sizeof(BSLightingShaderMaterialSkinTint) == 0xD0);
			static_assert(offsetof(BSLightingShaderMaterialSkinTint, TintColor) == 0xC0);

			class BSLightingShaderMaterialHairTint : public BSLightingShaderMaterialBase
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::BSLightingShaderMaterialHairTint;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSLightingShaderMaterialHairTint[0];

				NiAPI::NiColor TintColor;							// C0
				std::uint32_t PadCC;								// CC
			};
			static_assert(sizeof(BSLightingShaderMaterialHairTint) == 0xD0);
			static_assert(offsetof(BSLightingShaderMaterialHairTint, TintColor) == 0xC0);

			class BSLightingShaderMaterialSnow : public BSLightingShaderMaterialBase
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::BSLightingShaderMaterialSnow;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSLightingShaderMaterialSnow[0];

				NiAPI::NiColor SparkleColor;							// C0
				float UnkCC;										// CC
			};
			static_assert(sizeof(BSLightingShaderMaterialSnow) == 0xD0);
			static_assert(offsetof(BSLightingShaderMaterialSnow, SparkleColor) == 0xC0);

			class BSLightingShaderMaterialEye : public BSLightingShaderMaterialBase
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::BSLightingShaderMaterialEye;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSLightingShaderMaterialEye[0];

				NiAPI::NiPointer<NiAPI::NiTexture> EnvironmentTexture;	// C0
				NiAPI::NiPointer<NiAPI::NiTexture> EnvironmentMaskTexture;	// C8
				NiAPI::NiPoint3 EyeCenters[2];						// D0
				float EnvironmentMapScale;							// E8
				std::uint32_t PadEC;								// EC
			};
			static_assert(sizeof(BSLightingShaderMaterialEye) == 0xF0);
			static_assert(offsetof(BSLightingShaderMaterialEye, EyeCenters) == 0xD0);
			static_assert(offsetof(BSLightingShaderMaterialEye, EnvironmentMapScale) == 0xE8);

			class BSLightingShaderMaterialMultiLayerParallax : public BSLightingShaderMaterialBase
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::BSLightingShaderMaterialMultiLayerParallax;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSLightingShaderMaterialMultiLayerParallax[0];

				NiAPI::NiPointer<NiAPI::NiTexture> LayerTexture;		// C0
				NiAPI::NiPointer<NiAPI::NiTexture> EnvironmentTexture;	// C8
				NiAPI::NiPointer<NiAPI::NiTexture> EnvironmentMaskTexture;	// D0
				float LayerThickness;								// D8
				float RefractionScale;								// DC
				NiAPI::NiPoint2 InnerLayerScale;						// E0
				float EnvironmentMapScale;							// E8
				std::uint32_t PadEC;								// EC
			};
			static_assert(sizeof(BSLightingShaderMaterialMultiLayerParallax) == 0xF0);
			static_assert(offsetof(BSLightingShaderMaterialMultiLayerParallax, LayerThickness) == 0xD8);
			static_assert(offsetof(BSLightingShaderMaterialMultiLayerParallax, InnerLayerScale) == 0xE0);

			class BSLightingShaderMaterialLODLandscape : public BSLightingShaderMaterialBase
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::BSLightingShaderMaterialLODLandscape;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSLightingShaderMaterialLODLandscape[0];

				NiAPI::NiPointer<NiAPI::NiTexture> Textures[3];		// C0
				char UnkD8[0x18];									// D8
			};
			static_assert(sizeof(BSLightingShaderMaterialLODLandscape) == 0xF0);
			static_assert(offsetof(BSLightingShaderMaterialLODLandscape, Textures) == 0xC0);

			class BSLightingShaderMaterialLandscape : public BSLightingShaderMaterialBase
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::BSLightingShaderMaterialLandscape;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSLightingShaderMaterialLandscape[0];

				std::uint8_t UnkC0[3];								// C0
				std::uint8_t PadC3;									// C3
				float UnkC4[6][3];									// C4
				std::uint32_t Unk10C;								// 10C
				NiAPI::NiPointer<NiAPI::NiTexture> Textures110[3];	// 110
				NiAPI::NiPointer<NiAPI::NiTexture> Textures128[3];	// 128
				NiAPI::NiPointer<NiAPI::NiTexture> Textures140[3];	// 140
				NiAPI::NiPointer<NiAPI::NiTexture> Texture158;		// 158
				NiAPI::NiPointer<NiAPI::NiTexture> Texture160;		// 160
				std::uint64_t Unk168;								// 168
				NiAPI::NiColorA Unk170;								// 170
				std::uint32_t Unk180[3];							// 180
				std::uint32_t Pad18C;								// 18C
			};
			static_assert(sizeof(BSLightingShaderMaterialLandscape) == 0x190);
			static_assert(offsetof(BSLightingShaderMaterialLandscape, Textures110) == 0x110);
			static_assert(offsetof(BSLightingShaderMaterialLandscape, Unk170) == 0x170);
		}
	}
}
