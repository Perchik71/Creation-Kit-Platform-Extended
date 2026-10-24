// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkSceneGeometry.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkcdConvexCellsCollection :
		public hkcdPlanarGeometryPrimitivesCollection28 {};
	static_assert(sizeof(hkcdConvexCellsCollection) == 0xD90);

	class hkcdConvexCellsTree2D : public hkcdPlanarEntity
	{
	public:
		/* 0x018 */ std::byte pad018[0x18];
	};
	static_assert(sizeof(hkcdConvexCellsTree2D) == 0x30);

	class hkcdConvexCellsTree3D : public hkcdPlanarEntity
	{
	public:
		/* 0x018 */ std::byte pad018[0x30];
	};
	static_assert(sizeof(hkcdConvexCellsTree3D) == 0x48);

	class hkcdGskBase
	{
	public:
		class ShapeInterface
		{
		public:
			virtual ~ShapeInterface();
			virtual void ShapeInterface_Unk01();
		};
		static_assert(sizeof(ShapeInterface) == 0x8);
	};

	class hkcdGsk_Vector4ShapeInterface : public hkcdGskBase::ShapeInterface {};
	static_assert(sizeof(hkcdGsk_Vector4ShapeInterface) == 0x8);

	class hkcdVertexGeometry : public hkcdPlanarEntity
	{
	public:
		class VPolygonCollectionBase :
			public hkcdPlanarGeometryPrimitivesCollection28 {};
		static_assert(sizeof(VPolygonCollectionBase) == 0xD90);

		class VPolygonCollection : public VPolygonCollectionBase {};
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
