// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/NiAPI/NiPointer.h>
#include <EditorAPI/NiAPI/NiPoint.h>
#include <EditorAPI/NiAPI/NiColor.h>
#include <EditorAPI/BSTriShape.h>
#include <EditorAPI/BSLines.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
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
				};
				static_assert(sizeof(BGSPrimitivePlane) == 0xA0);

				class BGSPrimitiveBox : public BGSPrimitive
				{
				public:
					static constexpr auto TYPE{ kBox };

					virtual ~BGSPrimitiveBox() = default;
				};
				static_assert(sizeof(BGSPrimitiveBox) == 0xA0);

				class BGSPrimitiveLine : public BGSPrimitive
				{
				public:
					static constexpr auto TYPE{ kLine };

					virtual ~BGSPrimitiveLine() = default;
				};
				static_assert(sizeof(BGSPrimitiveLine) == 0xA0);

				class BGSPrimitiveSphere : public BGSPrimitive
				{
				public:
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

