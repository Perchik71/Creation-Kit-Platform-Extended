// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "NiAPI/NiPointer.h"
#include "NiAPI/NiProperty.h"
#include "NiAPI/NiAVObject.h"
#include "BSSkin.h"
#include "BSShaderProperty.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 160
			class BSGeometry : public NiAPI::NiAVObject
			{
			public:
				[[nodiscard]] inline NiAPI::NiBound GetModelBound() const noexcept(true) { return _ModelBound; }
				[[nodiscard]] inline NiAPI::NiPointer<NiAPI::NiProperty> GetEffectProperty() const noexcept(true) { return _EffectProperty; }
				[[nodiscard]] inline NiAPI::NiPointer<BSShaderProperty> GetShaderProperty() const noexcept(true) { return _ShaderProperty; }
				[[nodiscard]] inline NiAPI::NiPointer<BSSkin::Instance> GetSkinInstance() const noexcept(true) { return _SkinInstance; }
			private:
				NiAPI::NiBound							_ModelBound;		// 120
				NiAPI::NiPointer<NiAPI::NiProperty>		_EffectProperty;	// 130
				NiAPI::NiPointer<BSShaderProperty>		_ShaderProperty;	// 138
				NiAPI::NiPointer<BSSkin::Instance>		_SkinInstance;		// 140
				void*									_GeometryData;		// 148
				std::uint64_t							_VertexDesc;		// 150
				char									_Pad158[0x8];		// 158
			};
			static_assert(sizeof(BSGeometry) == 0x160);
		}
	}
}