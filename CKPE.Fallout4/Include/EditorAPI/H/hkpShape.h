// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseObject.h>
#include <EditorAPI/H/hkBaseTypes.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkcdShape : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint8_t m_type;
		/* 0x011 */ std::uint8_t m_dispatchType;
		/* 0x012 */ std::uint8_t m_bitsPerKey;
		/* 0x013 */ std::uint8_t m_shapeInfoCodecType;
		/* 0x014 */ std::uint8_t pad014[4];
	};
	static_assert(sizeof(hkcdShape) == 0x18);
	static_assert(offsetof(hkcdShape, m_type) == 0x10);

	class hkpShapeBase : public hkcdShape
	{
	public:
		virtual void sub_04();
		// Box of the shape transformed by LocalToWorld (hkTransform) and grown by Tolerance, written to AabbOut (hkAabb)
		virtual void getAabb(const void* LocalToWorld, float Tolerance, void* AabbOut) const;
		// Ray cast against the shape. The hkBool result is returned through a hidden pointer (HkBoolResult), which is also returned
		virtual char* castRay(char* HkBoolResult, const void* Input, void* Output) const;
		// Default implementation: castRay, then hands the hit to the collector (hkpRayHitCollector)
		virtual char* castRayWithCollector(char* HkBoolResult, const void* Input, const void* CdBody, void* Collector) const;
		virtual void sub_08();
		// Convex shape interface; stubs in the other shape types
		virtual void getSupportingVertex(const void* Direction, void* VertexOut) const;
		virtual void convertVertexIdsToVertices(const std::uint32_t* Ids, std::int32_t NumIds, void* VerticesOut) const;
		virtual void getCentre(void* CentreOut) const;
		[[nodiscard]] virtual std::int32_t getNumCollisionSpheres() const;
		virtual std::int32_t getCollisionSpheres(void* SphereBuffer) const;
		virtual void sub_0E();
	};
	static_assert(sizeof(hkpShapeBase) == 0x18);

	class hkpShape : public hkpShapeBase
	{
	public:
		virtual void sub_0F();
		virtual void sub_10();
		virtual void sub_11();

		/* 0x018 */ std::uintptr_t m_userData;
	};
	static_assert(sizeof(hkpShape) == 0x20);
	static_assert(offsetof(hkpShape, m_userData) == 0x18);

	class hkpShapeContainer
	{
	public:
		virtual ~hkpShapeContainer();
		virtual void sub_ShapeContainer_01();
		virtual void sub_ShapeContainer_02();
		virtual void sub_ShapeContainer_03();
		virtual void sub_ShapeContainer_04();
		virtual void sub_ShapeContainer_05();
		virtual void sub_ShapeContainer_06();
	};
	static_assert(sizeof(hkpShapeContainer) == 0x8);

	class hkpSingleShapeContainer : public hkpShapeContainer
	{
	public:
		/* 0x008 */ const hkpShape* m_childShape;
	};
	static_assert(sizeof(hkpSingleShapeContainer) == 0x10);
	static_assert(offsetof(hkpSingleShapeContainer, m_childShape) == 0x8);

	class hkpSphereRepShape : public hkpShape {};
	static_assert(sizeof(hkpSphereRepShape) == 0x20);

	class hkpConvexShape : public hkpSphereRepShape
	{
	public:
		virtual void sub_12();
		virtual void sub_13();

		/* 0x020 */ float m_radius;
		/* 0x024 */ std::uint8_t pad024[4];
	};
	static_assert(sizeof(hkpConvexShape) == 0x28);
	static_assert(offsetof(hkpConvexShape, m_radius) == 0x20);

	class hkpSphereShape : public hkpConvexShape
	{
	public:
		/* 0x028 */ std::uint8_t unk028[0x10];
	};
	static_assert(sizeof(hkpSphereShape) == 0x38);

	class alignas(16) hkpBoxShape : public hkpConvexShape
	{
	public:
		/* 0x028 */ std::uint8_t unk028[0x18];
	};
	static_assert(sizeof(hkpBoxShape) == 0x40);

	class alignas(16) hkpCapsuleShape : public hkpConvexShape
	{
	public:
		/* 0x028 */ std::uint8_t unk028[0x28];
	};
	static_assert(sizeof(hkpCapsuleShape) == 0x50);

	class alignas(16) hkpCylinderShape : public hkpConvexShape
	{
	public:
		/* 0x028 */ std::uint8_t unk028[0x48];
	};
	static_assert(sizeof(hkpCylinderShape) == 0x70);

	class alignas(16) hkpTriangleShape : public hkpConvexShape
	{
	public:
		/* 0x028 */ std::uint8_t unk028[0x48];
	};
	static_assert(sizeof(hkpTriangleShape) == 0x70);

	class alignas(16) hkpConvexVerticesShape : public hkpConvexShape
	{
	public:
		/* 0x028 */ std::uint8_t unk028[0x58];
	};
	static_assert(sizeof(hkpConvexVerticesShape) == 0x80);

	class hkpConvexVerticesConnectivity : public hkReferencedObject
	{
	public:
		virtual void sub_04();
		/* 0x010 */ std::uint8_t unk010[0x20];
	};
	static_assert(sizeof(hkpConvexVerticesConnectivity) == 0x30);

	class hkpConvexTransformShapeBase : public hkpConvexShape
	{
	public:
		/* 0x028 */ hkpSingleShapeContainer m_childShape;
		/* 0x038 */ std::int32_t m_childShapeSizeForSpu;
		/* 0x03C */ std::uint8_t pad03C[4];
	};
	static_assert(sizeof(hkpConvexTransformShapeBase) == 0x40);
	static_assert(offsetof(hkpConvexTransformShapeBase, m_childShape) == 0x28);

	class alignas(16) hkpConvexTransformShape : public hkpConvexTransformShapeBase
	{
	public:
		/* 0x040 */ std::uint8_t unk040[0x40];
	};
	static_assert(sizeof(hkpConvexTransformShape) == 0x80);

	class alignas(16) hkpConvexTranslateShape : public hkpConvexTransformShapeBase
	{
	public:
		/* 0x040 */ std::uint8_t unk040[0x10];
	};
	static_assert(sizeof(hkpConvexTranslateShape) == 0x50);

	class hkpShapeCollection : public hkpShape, public hkpShapeContainer
	{
	public:
		virtual void sub_12();
		virtual void sub_13();

		/* 0x028 */ hkBool m_disableWelding;
		/* 0x029 */ std::uint8_t m_collectionType;
		/* 0x02A */ std::uint8_t pad02A[6];
	};
	static_assert(sizeof(hkpShapeCollection) == 0x30);
	static_assert(offsetof(hkpShapeCollection, m_disableWelding) == 0x28);

	class hkpListShape : public hkpShapeCollection
	{
	public:
		/* 0x030 */ std::uint8_t unk030[0x60];
	};
	static_assert(sizeof(hkpListShape) == 0x90);

	class hkpConvexListShape : public hkpConvexShape, public hkpShapeContainer
	{
	public:
		/* 0x030 */ std::uint8_t unk030[0x50];
	};
	static_assert(sizeof(hkpConvexListShape) == 0x80);

	class hkpBvTreeShape : public hkpShape
	{
	public:
		virtual void sub_12();
		virtual void sub_13();
		virtual void sub_14();

		/* 0x020 */ std::uint8_t unk020[8];
	};
	static_assert(sizeof(hkpBvTreeShape) == 0x28);

	class hkpBvCompressedMeshShape : public hkpBvTreeShape,
		public hkpShapeContainer
	{
	public:
		/* 0x030 */ std::uint8_t unk030[0xE0];
	};
	static_assert(sizeof(hkpBvCompressedMeshShape) == 0x110);

	class hkpStaticCompoundShape : public hkpBvTreeShape,
		public hkpShapeContainer
	{
	public:
		/* 0x030 */ std::uint8_t unk030[0x70];
	};
	static_assert(sizeof(hkpStaticCompoundShape) == 0xA0);

	class hkpCompressedMeshShape : public hkpShapeCollection
	{
	public:
		/* 0x030 */ std::uint8_t unk030[0x100];
	};
	static_assert(sizeof(hkpCompressedMeshShape) == 0x130);

	class hkpMeshShape : public hkpShapeCollection
	{
	public:
		virtual void sub_14();
		/* 0x030 */ std::uint8_t unk030[0x50];

	};
	static_assert(sizeof(hkpMeshShape) == 0x80);

	class hkpStorageMeshShape : public hkpMeshShape
	{
	public:
		/* 0x080 */ std::uint8_t unk080[0x10];
		class SubpartStorage : public hkReferencedObject
		{
		public:
			/* 0x010 */ std::uint8_t unk010[0x60];
		};
	};
	static_assert(sizeof(hkpStorageMeshShape) == 0x90);
	static_assert(sizeof(hkpStorageMeshShape::SubpartStorage) == 0x70);

	class hkpSimpleMeshShape : public hkpShapeCollection
	{
	public:
		/* 0x030 */ std::uint8_t unk030[0x38];
	};
	static_assert(sizeof(hkpSimpleMeshShape) == 0x68);

	class hkpExtendedMeshShape : public hkpShapeCollection
	{
	public:
		virtual void sub_14();
		virtual void sub_15();
		/* 0x030 */ std::uint8_t unk030[0x110];
	};
	static_assert(sizeof(hkpExtendedMeshShape) == 0x140);

	class hkpStorageExtendedMeshShape : public hkpExtendedMeshShape
	{
	public:
		/* 0x140 */ std::uint8_t unk140[0x20];
		class MeshSubpartStorage : public hkReferencedObject
		{
		public:
			/* 0x010 */ std::uint8_t unk010[0x80];
		};
		class ShapeSubpartStorage : public hkReferencedObject
		{
		public:
			/* 0x010 */ std::uint8_t unk010[0x30];
		};
	};
	static_assert(sizeof(hkpStorageExtendedMeshShape) == 0x160);
	static_assert(sizeof(hkpStorageExtendedMeshShape::MeshSubpartStorage) == 0x90);
	static_assert(sizeof(hkpStorageExtendedMeshShape::ShapeSubpartStorage) == 0x40);

	class hkpHeightFieldShape : public hkpShape
	{
	public:
		virtual void sub_12();
		virtual void sub_13();
	};
	static_assert(sizeof(hkpHeightFieldShape) == 0x20);

	class alignas(16) hkpPlaneShape : public hkpHeightFieldShape
	{
	public:
		/* 0x020 */ std::uint8_t unk020[0x30];
	};
	static_assert(sizeof(hkpPlaneShape) == 0x50);

	class hkpSampledHeightFieldShape : public hkpHeightFieldShape
	{
	public:
		virtual void sub_14();
		virtual void sub_15();
		virtual void sub_16();
		/* 0x020 */ std::uint8_t unk020[0x70];
	};
	static_assert(sizeof(hkpSampledHeightFieldShape) == 0x90);

	class hkpStorageSampledHeightFieldShape : public hkpSampledHeightFieldShape
	{
	public:
		/* 0x090 */ std::uint8_t unk090[0x20];
	};
	static_assert(sizeof(hkpStorageSampledHeightFieldShape) == 0xB0);

	class hkpTriSampledHeightFieldCollection : public hkpShapeCollection
	{
	public:
		/* 0x030 */ std::uint8_t unk030[0x30];
	};
	static_assert(sizeof(hkpTriSampledHeightFieldCollection) == 0x60);

	class alignas(16) hkpTransformShape : public hkpShape
	{
	public:
		/* 0x020 */ std::uint8_t unk020[0x70];
	};
	static_assert(sizeof(hkpTransformShape) == 0x90);

	class hkpMoppCode : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint8_t unk010[0x30];
	};
	static_assert(sizeof(hkpMoppCode) == 0x40);

	class hkMoppBvTreeShapeBase : public hkpBvTreeShape
	{
	public:
		virtual void sub_15();
		/* 0x028 */ std::uint8_t unk028[0x28];
	};
	static_assert(sizeof(hkMoppBvTreeShapeBase) == 0x50);

	class hkpMoppBvTreeShape : public hkMoppBvTreeShapeBase
	{
	public:
		/* 0x050 */ std::uint8_t unk050[0x20];
	};
	static_assert(sizeof(hkpMoppBvTreeShape) == 0x70);

	// The templated geometry-converter base remains an opaque footprint.
	class alignas(8) hkpBvCompressedMeshShapeGc
	{
		hkpBvCompressedMeshShapeGc() = delete;
		hkpBvCompressedMeshShapeGc(const hkpBvCompressedMeshShapeGc&) = delete;
		hkpBvCompressedMeshShapeGc& operator=(const hkpBvCompressedMeshShapeGc&) = delete;
		~hkpBvCompressedMeshShapeGc() = delete;
		/* 0x000 */ std::uint8_t unk000[0x18];
	};
	static_assert(sizeof(hkpBvCompressedMeshShapeGc) == 0x18);
}
