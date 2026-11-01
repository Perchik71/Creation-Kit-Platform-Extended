// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/IDs_VTABLE.h>
#include <EditorAPI/N/NiPointer.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/N/NiColor.h>
#include <EditorAPI/B/BSTriShape.h>
#include <EditorAPI/B/BSLines.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSMultiBound;
			class BSOcclusionShape;
			class BSPortal;

			namespace Forms
			{
				// RVA + 0x3894BB0
				class BGSPrimitive
				{
				public:
					enum : std::uint32_t
					{
						kNone = 0,
						kBox,
						kSphere,
						kPlane,
						kLine,
						kEllipsoid
					};
				private:
					std::uint32_t _Type;
					NiAPI::NiPoint3 _Radius;
					NiAPI::NiColorA _Color;
					NiAPI::NiPointer<BSTriShape> _TriShape;
					NiAPI::NiPointer<BSLines> _Lines;
					NiAPI::NiPointer<BSFadeNode> _3D;
					char _Unk[0x60];
				public:
					virtual ~BGSPrimitive() = default;

					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPrimitive[0];
				private:
					// Creates the display geometry of the primitive (alpha property and shape/lines)
					/* 008 */ virtual void BuildGeometry() = 0;
				public:
					/* 010 */ virtual void SetColor(const NiAPI::NiColorA& color);
					/* 018 */ virtual void SetRadius(const NiAPI::NiPoint3& radius);
					/* 020 */ [[nodiscard]] virtual bool IsPointInside(const NiAPI::NiPoint3& point);
					// Allocates the BSMultiBound matching the primitive's shape (BSMultiBoundSphere, BSMultiBoundAABB or BSMultiBoundOBB); null for planes and lines
					/* 028 */ [[nodiscard]] virtual BSMultiBound* CreateMultiBound();
				public:
					[[nodiscard]] inline std::uint32_t GetType() const noexcept(true) { return _Type; }
					[[nodiscard]] inline NiAPI::NiPoint3 GetRadius() const noexcept(true) { return _Radius; }
					[[nodiscard]] inline NiAPI::NiColorA GetColor() const noexcept(true) { return _Color; }
					[[nodiscard]] inline NiAPI::NiPointer<BSTriShape> GetTriShape() const noexcept(true) { return _TriShape; }
					[[nodiscard]] inline NiAPI::NiPointer<BSLines> GetLines() const noexcept(true) { return _Lines; }
					[[nodiscard]] inline NiAPI::NiPointer<BSFadeNode> Get3D() const noexcept(true) { return _3D; }
				};
				static_assert(sizeof(BGSPrimitive) == 0xA0);

				class BGSPrimitivePlane : public BGSPrimitive
				{
				public:
					static constexpr auto TYPE{ kPlane };

					virtual ~BGSPrimitivePlane() = default;

					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPrimitivePlane[0];
				public:
					// Allocates a BSOcclusionPlane
					/* 030 */ [[nodiscard]] virtual BSOcclusionShape* CreateOcclusionShape();
					// Allocates a BSPortal
					/* 038 */ [[nodiscard]] virtual BSPortal* CreatePortal();
				};
				static_assert(sizeof(BGSPrimitivePlane) == 0xA0);

				class BGSPrimitiveBox : public BGSPrimitive
				{
				public:
					static constexpr auto TYPE{ kBox };

					virtual ~BGSPrimitiveBox() = default;

					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPrimitiveBox[0];
				public:
					// Allocates a BSOcclusionBox
					/* 030 */ [[nodiscard]] virtual BSOcclusionShape* CreateOcclusionShape();
				};
				static_assert(sizeof(BGSPrimitiveBox) == 0xA0);

				class BGSPrimitiveLine : public BGSPrimitiveBox
				{
				public:
					static constexpr auto TYPE{ kLine };

					virtual ~BGSPrimitiveLine() = default;

					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPrimitiveLine[0];
				};
				static_assert(sizeof(BGSPrimitiveLine) == 0xA0);

				class BGSPrimitiveSphere : public BGSPrimitive
				{
				public:
					static constexpr auto TYPE{ kSphere };

					virtual ~BGSPrimitiveSphere() = default;

					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPrimitiveSphere[0];
				};
				static_assert(sizeof(BGSPrimitiveSphere) == 0xA0);

				class BGSPrimitiveEllipsoid : public BGSPrimitiveSphere
				{
				public:
					static constexpr auto TYPE{ kEllipsoid };

					virtual ~BGSPrimitiveEllipsoid() = default;

					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPrimitiveEllipsoid[0];
				};
				static_assert(sizeof(BGSPrimitiveEllipsoid) == 0xA0);
			}
		}
	}
}
