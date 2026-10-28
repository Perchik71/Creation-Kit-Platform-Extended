// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/N/NiPointer.h>
#include <EditorAPI/N/NiProperty.h>
#include <EditorAPI/N/NiAVObject.h>
#include <EditorAPI/B/BSSkin.h>
#include <EditorAPI/B/BSGraphics.h>
#include <EditorAPI/B/BSShaderProperty.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSGeometrySegmentData;
			class BSCombinedTriShape;
			class BSMergeInstancedTriShape;
			class BSMultiIndexTriShape;

			namespace BSGraphics
			{
				class IndexBuffer;
			}
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

				constexpr static Common::ID VTABLE = IDs::VTABLE::BSGeometry[0];

				/* 3C */ virtual void UpdatePropertyControllers(NiAPI::NiUpdateData& Data) noexcept(true);
				/* 3D */ virtual BSGeometrySegmentData* GetSegmentData() noexcept(true);
				/* 3E */ virtual void SetSegmentData(BSGeometrySegmentData* Data) noexcept(true);
				/* 3F */ virtual BSGraphics::IndexBuffer* GetCustomIndexBuffer() noexcept(true);
				/* 40 */ virtual BSCombinedTriShape* IsBSCombinedTriShape() noexcept(true);
				/* 41 */ virtual BSMergeInstancedTriShape* IsBSMergeInstancedTriShape() noexcept(true);
				/* 42 */ virtual BSMultiIndexTriShape* IsMultiIndexTriShape() noexcept(true);
				/* 43 */ virtual std::uint32_t GetRenderableTris(std::uint32_t LODMode) noexcept(true);

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
				std::uint32_t							_Unk;				// 158
				std::uint8_t							_Type;				// 15C
				std::uint8_t							_unk15D;			// 15D
			public:
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BSGeometry, _ModelBound) == 0x120 &&
						offsetof(BSGeometry, _EffectProperty) == 0x130 &&
						offsetof(BSGeometry, _ShaderProperty) == 0x138 &&
						offsetof(BSGeometry, _SkinInstance) == 0x140 &&
						offsetof(BSGeometry, _GeometryData) == 0x148 &&
						offsetof(BSGeometry, _VertexDesc) == 0x150 &&
						offsetof(BSGeometry, _Type) == 0x15C &&
						offsetof(BSGeometry, _unk15D) == 0x15D;
				}
			};
			static_assert(sizeof(BSGeometry) == 0x160);
			static_assert(BSGeometry::VerifyLayout());
		}
	}
}