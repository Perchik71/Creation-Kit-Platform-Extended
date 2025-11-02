// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "NiAPI/NiPointer.h"
#include "NiAPI/NiProperty.h"
#include "NiAPI/NiAVObject.h"
#include "BSSkin.h"
#include "BSGraphics.h"
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
				enum : std::uint8_t
				{
					kGeometry = 0,
					kParticles,
					kStripParticles,
					kTriShape,
					kDynamicTriShape,
					kMeshLODTriShape,
					kLODMultiIndexTriShape,
					kMultiIndexTriShape,
					kSubIndexTriShape,
					kSubIndexLandTriShape,
					kMultiStreamInstanceTriShape,
					kParticleShaderDynamicTriShape,
					kLines,
					kDynamicLines,
					kInstanceGroup,
					kTotal
				};

				virtual ~BSGeometry() = default;

				[[nodiscard]] inline std::uint8_t GetType() const noexcept(true) { return _Type; }
				[[nodiscard]] inline BSGraphics::TriShape* GetGeometryData() const noexcept(true) { return _GeometryData; }
				[[nodiscard]] inline BSGraphics::VertexDesc GetVertexDesc() const noexcept(true) { return _VertexDesc; }
				[[nodiscard]] inline NiAPI::NiBound GetModelBound() const noexcept(true) { return _ModelBound; }
				[[nodiscard]] inline NiAPI::NiPointer<NiAPI::NiProperty> GetEffectProperty() const noexcept(true) { return _EffectProperty; }
				[[nodiscard]] inline NiAPI::NiPointer<BSShaderProperty> GetShaderProperty() const noexcept(true) { return _ShaderProperty; }
				[[nodiscard]] inline NiAPI::NiPointer<BSSkin::Instance> GetSkinInstance() const noexcept(true) { return _SkinInstance; }
			private:
				NiAPI::NiBound							_ModelBound;		// 120
				NiAPI::NiPointer<NiAPI::NiProperty>		_EffectProperty;	// 130
				NiAPI::NiPointer<BSShaderProperty>		_ShaderProperty;	// 138
				NiAPI::NiPointer<BSSkin::Instance>		_SkinInstance;		// 140
				BSGraphics::TriShape*					_GeometryData;		// 148
				BSGraphics::VertexDesc					_VertexDesc;		// 150
				std::uint32_t							_Unk;				// 158	maybe uses all BSGraphics::VertexDesc::Flags
				std::uint8_t							_Type;				// 15C
			};
			static_assert(sizeof(BSGeometry) == 0x160);
		}
	}
}