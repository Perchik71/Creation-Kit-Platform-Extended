// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/BSShader/Shaders/BSEffectShaderMaterial.h

#pragma once

#include "NiAPI\NiPointer.h"
#include "NiAPI\NiColor.h"
#include "NiAPI\NiSourceTexture.h"
#include "BSShaderMaterial.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			using namespace NiAPI;

			class BSEffectShaderMaterial : public BSShaderMaterial
			{
			public:
				virtual ~BSEffectShaderMaterial() = default;
				virtual BSShaderMaterial* CreateNew() override;
				virtual void CopyMembers(BSShaderMaterial* Other) override;
				virtual bool DoIsCopy(const BSShaderMaterial* Other) override;
				virtual std::uint32_t ComputeCRC32(std::uint32_t Unknown) override;
				virtual BSShaderMaterial* GetDefault() override;
				// virtual std::uint32_t GetUnknown1(); -- Not an override
				virtual std::uint32_t GetUnknown2() override;
				// virtual void Print() override; -- Creation kit only

				float fFalloffStartAngle;
				float fFalloffStopAngle;
				float fFalloffStartOpacity;
				float fFalloffStopOpacity;
				NiColorA kBaseColor;
				char _pad[0x10];
				float fSoftFalloffDepth;
				float fBaseColorScale;
				NiPointer<NiSourceTexture> spTexture;
				NiPointer<NiSourceTexture> spGrayscaleTexture;
				std::uint8_t eEffectClampMode;
				std::uint8_t eUnknown;
			};
			static_assert(sizeof(BSEffectShaderMaterial) == 0x88);
		}
	}
}