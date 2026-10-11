// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/BSShader/BSShaderMaterial.h

#pragma once

#include <CKPE.Common.h>

#include <EditorAPI/B/BSIntrusiveRefCounted.h>
#include <EditorAPI/N/NiPoint.h>

#include <array>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			using namespace NiAPI;

			class BSShaderMaterial : public BSIntrusiveRefCounted
			{
			public:
				enum class Feature
				{
					kNone = static_cast<std::underlying_type_t<Feature>>(-1),
					kDefault = 0,
					kEnvironmentMap = 1,
					kGlowMap = 2,
					kParallax = 3,
					kFaceGen = 4,
					kFaceGenRGBTint = 5,
					kHairTint = 6,
					kParallaxOcc = 7,
					kMultiTexLand = 8,
					kLODLand = 9,
					kUnknown = 10,
					kMultilayerParallax = 11,
					kTreeAnim = 12,
					kMultiIndexTriShapeSnow = 14,
					kLODObjectsHD = 15,
					kEye = 16,
					kCloud = 17,
					kLODLandNoise = 18,
					kMultiTexLandLODBlend = 19
				};

				enum class Type
				{
					kBase = 0,
					kEffect = 1,
					kLighting = 2,
					kWater = 3
				};

				virtual ~BSShaderMaterial();

				[[nodiscard]] virtual BSShaderMaterial* Create();
				virtual void CopyMembers(BSShaderMaterial* a_other);
				virtual bool DoIsCopy(const BSShaderMaterial* a_other);
				[[nodiscard]] virtual std::uint32_t ComputeCRC32(void);
				[[nodiscard]] virtual BSShaderMaterial* GetDefault();
				[[nodiscard]] virtual Feature GetFeature() const noexcept(true) { return Feature::kDefault; }
				[[nodiscard]] virtual Type GetType() const noexcept(true) { return Type::kBase; }
				virtual void DbgPrint() const noexcept(true);

				[[nodiscard]] inline size_t MaxTexCoordCount() noexcept(true) { return 2; }
				[[nodiscard]] inline NiPoint2* TexCoordOffset() noexcept(true) { return kTexCoordOffset.data(); }
				[[nodiscard]] inline NiPoint2* TexCoordScale() noexcept(true) { return kTexCoordScale.data(); }
			private:
				// members
				std::array<NiPoint2, 2> kTexCoordOffset;
				std::array<NiPoint2, 2> kTexCoordScale;
				char _pad[0xC];
			};
			static_assert(sizeof(BSShaderMaterial) == 0x38);
		}
	}
}