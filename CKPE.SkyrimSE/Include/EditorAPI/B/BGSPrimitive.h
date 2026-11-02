// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/N/NiPointer.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/N/NiColor.h>
#include <EditorAPI/B/BSTriShape.h>
#include <EditorAPI/B/BSFadeNode.h>
#include <EditorAPI/B/BSLines.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSPrimitive
				{
				public:
					constexpr static Common::ID RTTI = IDs::RTTI::BGSPrimitive;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPrimitive[0];

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
					constexpr static Common::ID RTTI = IDs::RTTI::BGSPrimitivePlane;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPrimitivePlane[0];

					static constexpr auto TYPE{ kPlane };

					virtual ~BGSPrimitivePlane() = default;
				};
				static_assert(sizeof(BGSPrimitivePlane) == 0xA0);

				class BGSPrimitiveBox : public BGSPrimitive
				{
				public:
					constexpr static Common::ID RTTI = IDs::RTTI::BGSPrimitiveBox;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPrimitiveBox[0];

					static constexpr auto TYPE{ kBox };

					virtual ~BGSPrimitiveBox() = default;
				};
				static_assert(sizeof(BGSPrimitiveBox) == 0xA0);

				class BGSPrimitiveLine : public BGSPrimitive
				{
				public:
					constexpr static Common::ID RTTI = IDs::RTTI::BGSPrimitiveLine;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPrimitiveLine[0];

					static constexpr auto TYPE{ kLine };

					virtual ~BGSPrimitiveLine() = default;
				};
				static_assert(sizeof(BGSPrimitiveLine) == 0xA0);

				class BGSPrimitiveSphere : public BGSPrimitive
				{
				public:
					constexpr static Common::ID RTTI = IDs::RTTI::BGSPrimitiveSphere;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSPrimitiveSphere[0];

					static constexpr auto TYPE{ kSphere };

					virtual ~BGSPrimitiveSphere() = default;
				};
				static_assert(sizeof(BGSPrimitiveSphere) == 0xA0);

				class BGSPrimitiveEllipsoid : public BGSPrimitive
				{
				public:
					static constexpr auto TYPE{ kEllipsoid };

					virtual ~BGSPrimitiveEllipsoid() = default;
				};
				static_assert(sizeof(BGSPrimitiveEllipsoid) == 0xA0);
			}
		}
	}
}

