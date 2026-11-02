// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseObject.h>
#include <EditorAPI/H/hkBaseTypes.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI
{
	struct hkgpIndexedMeshDefinitions
	{
		struct Edge;
		struct Vertex;
		struct Triangle;
	};

	struct hkgpMeshBase
	{
		struct Edge;
		struct Vertex;
		struct Triangle;
	};

	template<class Edge, class Vertex, class Triangle, class Allocator>
	class hkgpAbstractMesh : public hkReferencedObject
	{
	public:
		virtual void hkgpAbstractMesh_Unk04();

		/* 0x010 */ std::byte pad010[0x30];
	};

	using hkgpIndexedMeshStorage = hkgpAbstractMesh<hkgpIndexedMeshDefinitions::Edge, hkgpIndexedMeshDefinitions::Vertex, hkgpIndexedMeshDefinitions::Triangle, hkContainerHeapAllocator>;
	using hkgpMeshStorage = hkgpAbstractMesh<hkgpMeshBase::Edge, hkgpMeshBase::Vertex, hkgpMeshBase::Triangle, hkContainerHeapAllocator>;
	static_assert(sizeof(hkgpIndexedMeshStorage) == 0x40);
	static_assert(sizeof(hkgpMeshStorage) == 0x40);

	class hkgpConvexHull : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkgpConvexHull;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkgpConvexHull[0];

		/* 0x010 */ std::byte pad010[0x18];
	};
	static_assert(sizeof(hkgpConvexHull) == 0x28);

	class hkgpIndexedMesh : public hkgpIndexedMeshStorage
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkgpIndexedMesh;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkgpIndexedMesh[0];

		class IVertexRemoval
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkgpIndexedMesh__IVertexRemoval;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkgpIndexedMesh__IVertexRemoval[0];

			virtual ~IVertexRemoval();
			virtual void IVertexRemoval_Unk01();
		};
		static_assert(sizeof(IVertexRemoval) == 0x8);

		class ITriangleRemoval : public IVertexRemoval
		{
		public:
			virtual void ITriangleRemoval_Unk02();
		};
		static_assert(sizeof(ITriangleRemoval) == 0x8);

		class IEdgeCollapse : public ITriangleRemoval
		{
		public:
			virtual void IEdgeCollapse_Unk03();
			virtual void IEdgeCollapse_Unk04();
		};
		static_assert(sizeof(IEdgeCollapse) == 0x8);

		class EdgeBarrier
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkgpIndexedMesh__EdgeBarrier;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkgpIndexedMesh__EdgeBarrier[0];

			virtual ~EdgeBarrier();
			virtual void EdgeBarrier_Unk01();
		};
		static_assert(sizeof(EdgeBarrier) == 0x8);

		/* 0x040 */ std::byte pad040[0x50];
	};
	static_assert(sizeof(hkgpIndexedMesh) == 0x90);

	namespace hkgpIndexedMeshInternals
	{
		class DefaultEdgeCollapseInterface :
			public hkgpIndexedMesh::IEdgeCollapse
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::hkgpIndexedMeshInternals__DefaultEdgeCollapseInterface;
				constexpr static Common::ID VTABLE = IDs::VTABLE::hkgpIndexedMeshInternals__DefaultEdgeCollapseInterface[0];
			};
		static_assert(sizeof(DefaultEdgeCollapseInterface) == 0x8);
	}

	namespace hkgpCgoInternal
	{
		class ICollapse : public hkgpIndexedMesh::IEdgeCollapse
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkgpCgoInternal__ICollapse;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkgpCgoInternal__ICollapse[0];

			/* 0x008 */ std::byte pad008[0x10];
		};
		static_assert(sizeof(ICollapse) == 0x18);
	}

	class hkgpMesh : public hkgpMeshStorage
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkgpMesh;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkgpMesh[0];

		class IConvexOverlap
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkgpMesh__IConvexOverlap;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkgpMesh__IConvexOverlap[0];

			class IConvexShape
			{
			public:
				constexpr static Common::ID RTTI = IDs::RTTI::hkgpMesh__IConvexOverlap__IConvexShape;
				constexpr static Common::ID VTABLE = IDs::VTABLE::hkgpMesh__IConvexOverlap__IConvexShape[0];

				virtual ~IConvexShape();
				virtual void IConvexShape_Unk01();
				virtual void IConvexShape_Unk02();
				virtual void IConvexShape_Unk03();
			};
			static_assert(sizeof(IConvexShape) == 0x8);

			virtual ~IConvexOverlap();
			virtual void IConvexOverlap_Unk01();
			virtual void IConvexOverlap_Unk02();
		};
		static_assert(sizeof(IConvexOverlap) == 0x8);

		class ExternShape : public IConvexOverlap::IConvexShape
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkgpMesh__ExternShape;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkgpMesh__ExternShape[0];

			/* 0x008 */ std::byte pad008[0x10];
		};
		static_assert(sizeof(ExternShape) == 0x18);

		class TriangleShape : public IConvexOverlap::IConvexShape
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkgpMesh__TriangleShape;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkgpMesh__TriangleShape[0];

			/* 0x008 */ std::byte pad008[0x8];
		};
		static_assert(sizeof(TriangleShape) == 0x10);

		/* 0x040 */ std::byte pad040[0x58];
	};
	static_assert(sizeof(hkgpMesh) == 0x98);

	class hkgpTriangulatorBase : public hkReferencedObject {};
	static_assert(sizeof(hkgpTriangulatorBase) == 0x10);

	class hkgpJobQueue
	{
	public:
		class IJob
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkgpJobQueue__IJob;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkgpJobQueue__IJob[0];

			virtual ~IJob();
			virtual void IJob_Unk01();

			/* 0x008 */ std::byte pad008[0x38];
		};
		static_assert(sizeof(IJob) == 0x40);
	};
}
