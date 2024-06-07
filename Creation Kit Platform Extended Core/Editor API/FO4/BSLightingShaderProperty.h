// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "BSShaderProperty.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			using namespace NiAPI::Fallout4;

			// 0xE8
			class BSLightingShaderProperty : public BSShaderProperty
			{
			public:
				inline NiAPI::NiColorA& ProjectedUVColor() { return _ProjectedUVColor; }
				inline NiAPI::NiColorA& ProjectedUVParams() { return _ProjectedUVParams; }
				inline NiAPI::NiColor& EmissiveColor() { return *_EmissiveColor; }
				inline BSFixedString GetRootNameFS() const { return _RootName; }
				inline const char* GetRootName() const { return _RootName.c_str(); }
				inline float& EmitColorScale() { return _EmitColorScale; }
				inline float& ForcedDarkness() { return _ForcedDarkness; }
				inline float& SpecularLODFade() { return _SpecularLODFade; }
				inline float& EnvmapLODFade() { return _EnvmapLODFade; }

				inline static NiAPI::NiRTTI* NIRTTI_BSLightingShaderProperty;
			private:
				NiAPI::NiColorA _ProjectedUVColor;			// 70
				NiAPI::NiColorA _ProjectedUVParams;			// 80
				char			_Pad90[0x28];				// 90
				NiAPI::NiColor* _EmissiveColor;				// B8
				BSFixedString	_RootName;					// C0
				float			_EmitColorScale;			// C8
				float			_ForcedDarkness;			// CC
				float			_SpecularLODFade;			// D0
				float			_EnvmapLODFade;				// D4
				uint32_t		_BaseTechniqueID;			// D8
				uint32_t		_ClearCommandBufferCount;	// DC
				uint16_t		_DebugTintIndex;			// E0
				uint16_t		_StencilRef;				// E2
				char			_PadE4[4];					// E4
			};
			static_assert(sizeof(BSLightingShaderProperty) == 0xE8);
		}
	}
}