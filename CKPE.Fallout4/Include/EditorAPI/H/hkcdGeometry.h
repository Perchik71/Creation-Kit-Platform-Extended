// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkSceneGeometry.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkcdConvexCellsCollection :
		public hkcdPlanarGeometryPrimitivesCollection28
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkcdConvexCellsCollection;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkcdConvexCellsCollection[0];
		};
	static_assert(sizeof(hkcdConvexCellsCollection) == 0xD90);

	class hkcdConvexCellsTree2D : public hkcdPlanarEntity
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkcdConvexCellsTree2D;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkcdConvexCellsTree2D[0];

		/* 0x018 */ std::byte pad018[0x18];
	};
	static_assert(sizeof(hkcdConvexCellsTree2D) == 0x30);

	class hkcdConvexCellsTree3D : public hkcdPlanarEntity
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkcdConvexCellsTree3D;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkcdConvexCellsTree3D[0];

		/* 0x018 */ std::byte pad018[0x30];
	};
	static_assert(sizeof(hkcdConvexCellsTree3D) == 0x48);

	class hkcdGskBase
	{
	public:
		class ShapeInterface
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkcdGskBase__ShapeInterface;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkcdGskBase__ShapeInterface[0];

			virtual ~ShapeInterface();
			virtual void ShapeInterface_Unk01();
		};
		static_assert(sizeof(ShapeInterface) == 0x8);
	};

	class hkcdGsk_Vector4ShapeInterface : public hkcdGskBase::ShapeInterface
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkcdGsk_Vector4ShapeInterface;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkcdGsk_Vector4ShapeInterface[0];
	};
	static_assert(sizeof(hkcdGsk_Vector4ShapeInterface) == 0x8);

	class hkcdVertexGeometry : public hkcdPlanarEntity
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkcdVertexGeometry;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkcdVertexGeometry[0];

		class VPolygonCollectionBase :
			public hkcdPlanarGeometryPrimitivesCollection28
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::hkcdVertexGeometry__VPolygonCollectionBase;
				constexpr static Common::ID VTABLE = IDs::VTABLE::hkcdVertexGeometry__VPolygonCollectionBase[0];
			};
		static_assert(sizeof(VPolygonCollectionBase) == 0xD90);

		class VPolygonCollection : public VPolygonCollectionBase
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkcdVertexGeometry__VPolygonCollection;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkcdVertexGeometry__VPolygonCollection[0];
		};
		static_assert(sizeof(VPolygonCollection) == 0xD90);

		/* 0x018 */ std::byte pad018[0xDC8];
	};
	static_assert(sizeof(hkcdVertexGeometry) == 0xDE0);

	namespace hkcdSimdTreeUtils::Build
	{
		class EmptyRunnable : public hkcdSimdTree::IParallelBuild::IRunnable {};
		static_assert(sizeof(EmptyRunnable) == 0x8);

		class IRefit
		{
		public:
			virtual ~IRefit();
			virtual void IRefit_Unk01();
			virtual void IRefit_Unk02();
		};
		static_assert(sizeof(IRefit) == 0x8);

		class Task : public hkcdSimdTree::IParallelBuild::IRunnable
		{
		public:
			/* 0x008 */ std::byte pad008[0x38];
		};
		static_assert(sizeof(Task) == 0x40);
	}
}
