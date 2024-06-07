// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "NiAPI/NiPointer.h"
#include "NiAPI/NiProperty.h"
#include "NiAPI/NiAVObject.h"
#include "BSSkin.h"
#include "BSShaderProperty.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			using namespace NiAPI::Fallout4;

			// 160
			class BSGeometry : public NiAVObject
			{
			public:
				inline NiAPI::NiBound GetModelBound() const { return _ModelBound; }
				inline NiAPI::NiPointer<NiProperty> GetEffectProperty() const { return _EffectProperty; }
				inline NiAPI::NiPointer<BSShaderProperty> GetShaderProperty() const { return _ShaderProperty; }
				inline NiAPI::NiPointer<BSSkin::Instance> GetSkinInstance() const { return _SkinInstance; }
			private:
				NiAPI::NiBound						_ModelBound;		// 120
				NiAPI::NiPointer<NiProperty>		_EffectProperty;	// 130
				NiAPI::NiPointer<BSShaderProperty>	_ShaderProperty;	// 138
				NiAPI::NiPointer<BSSkin::Instance>	_SkinInstance;		// 140
				void*								_GeometryData;		// 148
				uint64_t							_VertexDesc;		// 150
				char								_Pad158[0x8];		// 158
			};
			static_assert(sizeof(BSGeometry) == 0x160);
		}
	}
}