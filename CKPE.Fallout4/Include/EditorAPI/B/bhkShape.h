// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/bhkRefObject.h>
#include <cstddef>
#include <cstdint>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkShape : public bhkRefObject
	{
	public:
		/* 0x020 */ std::uint32_t Material;
		/* 0x024 */ std::uint8_t pad024[0x4];

		[[nodiscard]] inline std::uint32_t GetMaterial() const noexcept(true)
		{ return Material; }

	private:
		/* 38 */ virtual void UnkShape38() noexcept(true);
		/* 39 */ virtual void UnkShape39() noexcept(true);
		/* 3A */ virtual void UnkShape3A() noexcept(true);
		/* 3B */ virtual void UnkShape3B() noexcept(true);
		/* 3C */ virtual void UnkShape3C() noexcept(true);
		/* 3D */ virtual void UnkShape3D() noexcept(true);
		/* 3E */ virtual void UnkShape3E() noexcept(true);
	};
	static_assert(sizeof(bhkShape) == 0x28);
	static_assert(offsetof(bhkShape, Material) == 0x20);

	class bhkSphereRepShape : public bhkShape {};
	static_assert(sizeof(bhkSphereRepShape) == 0x28);

	class bhkConvexShape : public bhkSphereRepShape {};
	static_assert(sizeof(bhkConvexShape) == 0x28);

	class bhkBoxShape : public bhkConvexShape {};
	static_assert(sizeof(bhkBoxShape) == 0x28);

	class bhkSphereShape : public bhkConvexShape {};
	static_assert(sizeof(bhkSphereShape) == 0x28);

	class bhkCapsuleShape : public bhkConvexShape {};
	static_assert(sizeof(bhkCapsuleShape) == 0x28);

	class bhkCylinderShape : public bhkConvexShape {};
	static_assert(sizeof(bhkCylinderShape) == 0x28);

	class bhkTriangleShape : public bhkConvexShape {};
	static_assert(sizeof(bhkTriangleShape) == 0x28);

	class bhkConvexVerticesShape : public bhkConvexShape {};
	static_assert(sizeof(bhkConvexVerticesShape) == 0x28);

	class bhkConvexTransformShape : public bhkConvexShape {};
	static_assert(sizeof(bhkConvexTransformShape) == 0x28);

	class bhkConvexTranslateShape : public bhkConvexShape {};
	static_assert(sizeof(bhkConvexTranslateShape) == 0x28);

	class bhkTransformShape : public bhkShape {};
	static_assert(sizeof(bhkTransformShape) == 0x28);

	class bhkShapeCollection : public bhkShape
	{
	private:
		/* 3F */ virtual void UnkShapeCollection3F() noexcept(true);
	};
	static_assert(sizeof(bhkShapeCollection) == 0x28);

	class bhkListShape : public bhkShapeCollection
	{
	public:
		/* 0x028 */ std::uint8_t unk028[0x8];
	};
	static_assert(sizeof(bhkListShape) == 0x30);
	static_assert(offsetof(bhkListShape, unk028) == 0x28);

	class bhkBvTreeShape : public bhkShape {};
	static_assert(sizeof(bhkBvTreeShape) == 0x28);

	class bhkMoppBvTreeShape : public bhkBvTreeShape {};
	static_assert(sizeof(bhkMoppBvTreeShape) == 0x28);

	class bhkCompressedMeshShape : public bhkShapeCollection
	{
	public:
		/* 0x028 */ std::uint8_t unk028[0x8];
	};
	static_assert(sizeof(bhkCompressedMeshShape) == 0x30);
	static_assert(offsetof(bhkCompressedMeshShape, unk028) == 0x28);

	class bhkCompressedMeshShapeData : public NiAPI::NiObject
	{
	public:
		/* 0x010 */ std::uint8_t unk010[0x100];
	};
	static_assert(sizeof(bhkCompressedMeshShapeData) == 0x110);
	static_assert(offsetof(bhkCompressedMeshShapeData, unk010) == 0x10);

	class bhkNiTriStripsShape : public bhkShapeCollection {};
	static_assert(sizeof(bhkNiTriStripsShape) == 0x28);

	class bhkPackedNiTriStripsShape : public bhkShapeCollection {};
	static_assert(sizeof(bhkPackedNiTriStripsShape) == 0x28);

	class bhkHeightFieldShape : public bhkShape {};
	static_assert(sizeof(bhkHeightFieldShape) == 0x28);

	class bhkPlaneShape : public bhkHeightFieldShape {};
	static_assert(sizeof(bhkPlaneShape) == 0x28);
}
