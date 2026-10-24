// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseObject.h>
#include <EditorAPI/H/hkBaseTypes.h>

#include <EditorAPI/H/hkReflectionTypes.h>

namespace CKPE::Fallout4::EditorAPI
{
	struct hkVector4d;
	class hkAabb;
	class hkAabb16;
	class hkAabbHalf;
	class hkAabbUint32;
	class hkAlignSceneToNodeOptions;
	class hkArrayStreamWriter;
	class hkArrayTypeAttribute;
	class hkBitField;
	class hkBitFieldBasehkBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator;
	class hkBitFieldBasehkOffsetBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator;
	class hkBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator;
	class hkBufferedStreamWriter;
	class hkClass;
	class hkClassEnum;
	class hkClassEnumItem;
	class hkClassMember;
	class hkColor;
	class hkCompressedMassProperties;
	class hkContactPoint;
	class hkContactPointMaterial;
	class hkCustomAttributes;
	class hkCustomAttributesAttribute;
	class hkDataObjectTypeAttribute;
	class hkDefaultCompoundMeshBody;
	class hkDefaultCompoundMeshShape;
	class hkDefaultMemoryTracker;
	class hkDefaultMeshMaterialRegistry;
	class hkDescriptionAttribute;
	class hkDocumentationAttribute;
	class hkFloat16;
	class hkFloat16Transform;
	class hkFourTransposedPointsf;
	class hkFreeListArrayhknpMaterialhknpMaterialId8hknpMaterialFreeListArrayOperations;
	class hkFreeListArrayhknpMotionPropertieshknpMotionPropertiesId8hknpMotionPropertiesFreeListArrayOperations;
	class hkFreeListArrayhknpShapeInstancehkHandleshort32767hknpShapeInstanceIdDiscriminant8hknpShapeInstance;
	class hkGeometry;
	class hkGeometryTriangle;
	class hkGizmoAttribute;
	class hkIndexedTransformSet;
	class hkIntRealPair;
	class hkLinkAttribute;
	class hkLocalFrame;
	class hkLocalFrameGroup;
	class hkMassProperties;
	class hkMemoryMeshBody;
	class hkMemoryMeshMaterial;
	class hkMemoryMeshShape;
	class hkMemoryMeshShapeSection;
	class hkMemoryMeshSystem;
	class hkMemoryMeshTexture;
	class hkMemoryMeshVertexBuffer;
	class hkMemoryResourceContainer;
	class hkMemoryResourceHandle;
	class hkMemoryResourceHandleExternalLink;
	class hkMeshBody;
	class hkMeshBoneIndexMapping;
	class hkMeshMaterial;
	class hkMeshMaterialRegistry;
	class hkMeshSection;
	class hkMeshSectionCinfo;
	class hkMeshShape;
	class hkMeshSystem;
	class hkMeshTexture;
	class hkMeshTextureRawBufferDescriptor;
	class hkMeshVertexBuffer;
	class hkModelerNodeTypeAttribute;
	class hkMonitorStreamColorTable;
	class hkMonitorStreamColorTableColorPair;
	class hkMonitorStreamFrameInfo;
	class hkMonitorStreamStringMap;
	class hkMonitorStreamStringMapStringMap;
	class hkMotionState;
	class hkMultiThreadCheck;
	class hkMultipleVertexBuffer;
	class hkMultipleVertexBufferElementInfo;
	class hkMultipleVertexBufferLockedElement;
	class hkMultipleVertexBufferVertexBufferInfo;
	class hkOffsetBitField;
	class hkOffsetBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator;
	class hkOstream;
	class hkPackedVector3;
	class hkPackedVector8_3;
	class hkPackfileHeader;
	class hkPackfileSectionHeader;
	class hkPostFinishAttribute;
	class hkQTransform;
	class hkQTransformf;
	class hkRangeInt32Attribute;
	class hkRangeRealAttribute;
	class hkRefCountedProperties;
	class hkRefCountedPropertiesEntry;
	class hkResourceBase;
	class hkResourceContainer;
	class hkResourceHandle;
	class hkRootLevelContainer;
	class hkRootLevelContainerNamedVariant;
	class hkScriptableAttribute;
	class hkSemanticsAttribute;
	class hkSetIntFloatPair;
	class hkSetUint32;
	class hkSetUint64;
	class hkSethkIntRealPairhkContainerHeapAllocatorhkMapOperationshkIntRealPair;
	class hkSetunsignedinthkContainerHeapAllocatorhkMapOperationsunsignedint;
	class hkSetunsignedlonglonghkContainerHeapAllocatorhkMapOperationsunsignedlonglong;
	class hkSimpleLocalFrame;
	class hkSimpleProperty;
	class hkSimplePropertyValue;
	class hkSkinBinding;
	class hkSkinnedMeshShape;
	class hkSkinnedMeshShapeBoneSection;
	class hkSkinnedMeshShapeBoneSet;
	class hkSkinnedMeshShapePart;
	class hkSkinnedRefMeshShape;
	class hkSphere;
	class hkStorageSkinnedMeshShape;
	class hkStringObject;
	class hkSweptTransformf;
	class hkTraceStreamTitle;
	class hkUFloat8;
	class hkUiAttribute;
	class hkUuid;
	class hkVariableTweakingHelper;
	class hkVariableTweakingHelperBoolVariableInfo;
	class hkVariableTweakingHelperIntVariableInfo;
	class hkVariableTweakingHelperRealVariableInfo;
	class hkVariableTweakingHelperVector4VariableInfo;
	class hkVertexFormat;
	class hkVertexFormatElement;
	class hkWorldMemoryAvailableWatchDog;
	class hkcdAdf;
	class hkcdDynamicAabbTree;
	class hkcdDynamicTreeAnisotropicMetric;
	class hkcdDynamicTreeBalanceMetric;
	class hkcdDynamicTreeCentroidMetric;
	class hkcdDynamicTreeCodec18;
	class hkcdDynamicTreeCodec32;
	class hkcdDynamicTreeCodecInt16;
	class hkcdDynamicTreeCodecInt16IntAabb;
	class hkcdDynamicTreeCodecRawUint;
	class hkcdDynamicTreeCodecRawUlong;
	class hkcdDynamicTreeCodecRawunsignedint;
	class hkcdDynamicTreeCodecRawunsignedlong;
	class hkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodec32;
	class hkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecInt16;
	class hkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecRawUint;
	class hkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecRawUlong;
	class hkcdDynamicTreeDefaultTree32Storage;
	class hkcdDynamicTreeDefaultTree48Storage;
	class hkcdDynamicTreeDefaultTreeInt16Storage;
	class hkcdDynamicTreeDefaultTreePtrStorage;
	class hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodec32;
	class hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecInt16;
	class hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUint;
	class hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUlong;
	class hkcdDynamicTreeDynamicStorage16;
	class hkcdDynamicTreeDynamicStorage32;
	class hkcdDynamicTreeDynamicStorageInt16;
	class hkcdDynamicTreeDynamicStoragePtr;
	class hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage16;
	class hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage32;
	class hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorageInt16;
	class hkcdDynamicTreeTreehkcdDynamicTreeDynamicStoragePtr;
	class hkcdFourAabb;
	class hkcdPlanarCsgOperand;
	class hkcdPlanarCsgOperandGeomSource;
	class hkcdPlanarEntity;
	class hkcdPlanarGeometry;
	class hkcdPlanarGeometryPlanesCollection;
	class hkcdPlanarGeometryPolygonCollection;
	class hkcdPlanarGeometryPrimitivesCollection28;
	class hkcdPlanarGeometryPrimitivesPlane;
	class hkcdPlanarSolid;
	class hkcdPlanarSolidNode;
	class hkcdPlanarSolidNodeStorage;
	class hkcdShapeDispatchType;
	class hkcdShapeInfoCodecType;
	class hkcdShapeType;
	class hkcdSimdTree;
	class hkcdSimdTreeNode;
	class hkcdStaticAabbTree;
	class hkcdStaticMeshTreeBase;
	class hkcdStaticMeshTreeBasePrimitive;
	class hkcdStaticMeshTreeBasePrimitiveDataRunBasehknpCompressedMeshShapeTreeDataRunData;
	class hkcdStaticMeshTreeBasePrimitiveDataRunBaseunsignedshort;
	class hkcdStaticMeshTreeBaseSection;
	class hkcdStaticMeshTreeBaseSectionDataRuns;
	class hkcdStaticMeshTreeBaseSectionPrimitives;
	class hkcdStaticMeshTreeBaseSectionSharedVertices;
	class hkcdStaticMeshTreeDefaultDataRun;
	class hkcdStaticMeshTreehkcdStaticMeshTreeCommonConfigunsignedintunsignedlonglong1121hknpCompressedMeshShapeTreeDataRun;
	class hkcdStaticPvs;
	class hkcdStaticPvsBlockHeader;
	class hkcdStaticTreeCodec3Axis;
	class hkcdStaticTreeCodec3Axis4;
	class hkcdStaticTreeCodec3Axis5;
	class hkcdStaticTreeCodec3Axis6;
	class hkcdStaticTreeCodecRaw;
	class hkcdStaticTreeDefaultTreeStorage32;
	class hkcdStaticTreeDefaultTreeStorage4;
	class hkcdStaticTreeDefaultTreeStorage5;
	class hkcdStaticTreeDefaultTreeStorage6;
	class hkcdStaticTreeDynamicStorage32;
	class hkcdStaticTreeDynamicStorage4;
	class hkcdStaticTreeDynamicStorage5;
	class hkcdStaticTreeDynamicStorage6;
	class hkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis4;
	class hkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis5;
	class hkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis6;
	class hkcdStaticTreeDynamicStoragehkcdStaticTreeCodecRaw;
	class hkcdStaticTreeTreehkcdStaticTreeDynamicStorage32;
	class hkcdStaticTreeTreehkcdStaticTreeDynamicStorage4;
	class hkcdStaticTreeTreehkcdStaticTreeDynamicStorage5;
	class hkcdStaticTreeTreehkcdStaticTreeDynamicStorage6;
	class hkxAnimatedFloat;
	class hkxAnimatedMatrix;
	class hkxAnimatedQuaternion;
	class hkxAnimatedVector;
	class hkxAttribute;
	class hkxAttributeGroup;
	class hkxAttributeHolder;
	class hkxBlob;
	class hkxBlobMeshShape;
	class hkxCamera;
	class hkxEdgeSelectionChannel;
	class hkxEnum;
	class hkxEnumItem;
	class hkxEnvironment;
	class hkxEnvironmentVariable;
	class hkxIndexBuffer;
	class hkxLight;
	class hkxMaterial;
	class hkxMaterialEffect;
	class hkxMaterialProperty;
	class hkxMaterialShader;
	class hkxMaterialShaderSet;
	class hkxMaterialTextureStage;
	class hkxMesh;
	class hkxMeshSection;
	class hkxMeshUserChannelInfo;
	class hkxNode;
	class hkxNodeAnnotationData;
	class hkxNodeSelectionSet;
	class hkxScene;
	class hkxSkinBinding;
	class hkxSparselyAnimatedBool;
	class hkxSparselyAnimatedEnum;
	class hkxSparselyAnimatedInt;
	class hkxSparselyAnimatedString;
	class hkxSpline;
	class hkxSplineControlPoint;
	class hkxTextureFile;
	class hkxTextureInplace;
	class hkxTriangleSelectionChannel;
	class hkxVertexAnimation;
	class hkxVertexAnimationUsageMap;
	class hkxVertexBuffer;
	class hkxVertexBufferVertexData;
	class hkxVertexDescription;
	class hkxVertexDescriptionElementDecl;
	class hkxVertexFloatDataChannel;
	class hkxVertexIntDataChannel;
	class hkxVertexSelectionChannel;
	class hkxVertexVectorDataChannel;
	class hkStreamReader;
	class hkStreamWriter;
	class hkSeekableStreamReader;
	class hkMemoryTracker;

	class hkcdAdf
	{
	public:
		/* 0x000 */ float m_accuracy;
		/* 0x004 */ std::byte pad004[0xC];
		/* 0x010 */ hkAabb m_domain;
		/* 0x030 */ hkVector4 m_origin;
		/* 0x040 */ hkVector4 m_scale;
		/* 0x050 */ float m_range[2];
		/* 0x058 */ hkArray<std::uint32_t> m_nodes;
		/* 0x068 */ hkArray<std::uint16_t> m_voxels;
		/* 0x078 */ std::byte pad078[0x8];
	};
	static_assert(sizeof(hkcdAdf) == 0x80);
	static_assert(offsetof(hkcdAdf, m_accuracy) == 0x0);
	static_assert(offsetof(hkcdAdf, m_domain) == 0x10);
	static_assert(offsetof(hkcdAdf, m_origin) == 0x30);
	static_assert(offsetof(hkcdAdf, m_scale) == 0x40);
	static_assert(offsetof(hkcdAdf, m_range) == 0x50);
	static_assert(offsetof(hkcdAdf, m_nodes) == 0x58);
	static_assert(offsetof(hkcdAdf, m_voxels) == 0x68);

	class hkcdDynamicAabbTree : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkBool m_shouldDeleteTree;
		/* 0x011 */ std::byte pad011[0x7];
		/* 0x018 */ hkcdDynamicTreeDefaultTree48Storage* m_treePtr;
	};
	static_assert(sizeof(hkcdDynamicAabbTree) == 0x20);
	static_assert(offsetof(hkcdDynamicAabbTree, m_shouldDeleteTree) == 0x10);
	static_assert(offsetof(hkcdDynamicAabbTree, m_treePtr) == 0x18);

	class hkcdDynamicTreeAnisotropicMetric
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeAnisotropicMetric) == 0x1);

	class hkcdDynamicTreeBalanceMetric
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeBalanceMetric) == 0x1);

	class hkcdDynamicTreeCentroidMetric
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeCentroidMetric) == 0x1);

	class hkcdDynamicTreeCodec18
	{
	public:
		/* 0x000 */ hkAabbHalf m_aabb;
		/* 0x010 */ std::uint16_t m_parent;
	};
	static_assert(sizeof(hkcdDynamicTreeCodec18) == 0x12);
	static_assert(offsetof(hkcdDynamicTreeCodec18, m_aabb) == 0x0);
	static_assert(offsetof(hkcdDynamicTreeCodec18, m_parent) == 0x10);

	class hkcdDynamicTreeCodec32
	{
	public:
		/* 0x000 */ hkAabb m_aabb;
	};
	static_assert(sizeof(hkcdDynamicTreeCodec32) == 0x20);
	static_assert(offsetof(hkcdDynamicTreeCodec32, m_aabb) == 0x0);

	class hkcdDynamicTreeCodecInt16IntAabb : public hkAabb16
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeCodecInt16IntAabb) == 0x10);

	class hkcdDynamicTreeCodecInt16
	{
	public:
		/* 0x000 */ hkcdDynamicTreeCodecInt16IntAabb m_aabb;
		/* 0x010 */ std::uint32_t m_parent;
		/* 0x014 */ std::uint32_t m_children[2];
		/* 0x01C */ std::uint32_t m_pad;
	};
	static_assert(sizeof(hkcdDynamicTreeCodecInt16) == 0x20);
	static_assert(offsetof(hkcdDynamicTreeCodecInt16, m_aabb) == 0x0);
	static_assert(offsetof(hkcdDynamicTreeCodecInt16, m_parent) == 0x10);
	static_assert(offsetof(hkcdDynamicTreeCodecInt16, m_children) == 0x14);
	static_assert(offsetof(hkcdDynamicTreeCodecInt16, m_pad) == 0x1C);

	class hkcdDynamicTreeCodecRawunsignedint
	{
	public:
		/* 0x000 */ hkAabb m_aabb;
		/* 0x020 */ std::uint32_t m_parent;
		/* 0x024 */ std::uint32_t m_children[2];
		/* 0x02C */ std::byte pad02C[0x4];
	};
	static_assert(sizeof(hkcdDynamicTreeCodecRawunsignedint) == 0x30);
	static_assert(offsetof(hkcdDynamicTreeCodecRawunsignedint, m_aabb) == 0x0);
	static_assert(offsetof(hkcdDynamicTreeCodecRawunsignedint, m_parent) == 0x20);
	static_assert(offsetof(hkcdDynamicTreeCodecRawunsignedint, m_children) == 0x24);

	class hkcdDynamicTreeCodecRawUint : public hkcdDynamicTreeCodecRawunsignedint
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeCodecRawUint) == 0x30);

	class hkcdDynamicTreeCodecRawunsignedlong
	{
	public:
		/* 0x000 */ hkAabb m_aabb;
		/* 0x020 */ std::size_t m_parent;
		/* 0x028 */ std::size_t m_children[2];
		/* 0x038 */ std::byte pad038[0x8];
	};
	static_assert(sizeof(hkcdDynamicTreeCodecRawunsignedlong) == 0x40);
	static_assert(offsetof(hkcdDynamicTreeCodecRawunsignedlong, m_aabb) == 0x0);
	static_assert(offsetof(hkcdDynamicTreeCodecRawunsignedlong, m_parent) == 0x20);
	static_assert(offsetof(hkcdDynamicTreeCodecRawunsignedlong, m_children) == 0x28);

	class hkcdDynamicTreeCodecRawUlong : public hkcdDynamicTreeCodecRawunsignedlong
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeCodecRawUlong) == 0x40);

	class hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodec32 : public hkcdDynamicTreeAnisotropicMetric
	{
	public:
		/* 0x000 */ hkArray<hkcdDynamicTreeCodec32> m_nodes;
		/* 0x010 */ std::uint16_t m_firstFree;
		/* 0x012 */ std::byte pad012[0x6];
	};
	static_assert(sizeof(hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodec32) == 0x18);
	static_assert(offsetof(hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodec32, m_nodes) == 0x0);
	static_assert(offsetof(hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodec32, m_firstFree) == 0x10);

	class hkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodec32 : public hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodec32
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodec32) == 0x18);

	class hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecInt16 : public hkcdDynamicTreeAnisotropicMetric
	{
	public:
		/* 0x000 */ hkArray<hkcdDynamicTreeCodecInt16> m_nodes;
		/* 0x010 */ std::uint32_t m_firstFree;
		/* 0x014 */ std::byte pad014[0x4];
	};
	static_assert(sizeof(hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecInt16) == 0x18);
	static_assert(offsetof(hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecInt16, m_nodes) == 0x0);
	static_assert(offsetof(hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecInt16, m_firstFree) == 0x10);

	class hkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecInt16 : public hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecInt16
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecInt16) == 0x18);

	class hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUint : public hkcdDynamicTreeAnisotropicMetric
	{
	public:
		/* 0x000 */ hkArray<hkcdDynamicTreeCodecRawUint> m_nodes;
		/* 0x010 */ std::uint32_t m_firstFree;
		/* 0x014 */ std::byte pad014[0x4];
	};
	static_assert(sizeof(hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUint) == 0x18);
	static_assert(offsetof(hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUint, m_nodes) == 0x0);
	static_assert(offsetof(hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUint, m_firstFree) == 0x10);

	class hkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecRawUint : public hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUint
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecRawUint) == 0x18);

	class hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUlong : public hkcdDynamicTreeAnisotropicMetric
	{
	public:
		/* 0x000 */ hkArray<hkcdDynamicTreeCodecRawUlong> m_nodes;
		/* 0x010 */ std::size_t m_firstFree;
	};
	static_assert(sizeof(hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUlong) == 0x18);
	static_assert(offsetof(hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUlong, m_nodes) == 0x0);
	static_assert(offsetof(hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUlong, m_firstFree) == 0x10);

	class hkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecRawUlong : public hkcdDynamicTreeDynamicStorage0hkcdDynamicTreeAnisotropicMetrichkcdDynamicTreeCodecRawUlong
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecRawUlong) == 0x18);

	class hkcdDynamicTreeDynamicStorage16 : public hkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodec32
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeDynamicStorage16) == 0x18);

	class hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage16 : public hkcdDynamicTreeDynamicStorage16
	{
	public:
		/* 0x018 */ std::uint32_t m_numLeaves;
		/* 0x01C */ std::uint32_t m_path;
		/* 0x020 */ std::uint16_t m_root;
		/* 0x022 */ std::byte pad022[0x6];
	};
	static_assert(sizeof(hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage16) == 0x28);
	static_assert(offsetof(hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage16, m_numLeaves) == 0x18);
	static_assert(offsetof(hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage16, m_path) == 0x1C);
	static_assert(offsetof(hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage16, m_root) == 0x20);

	class hkcdDynamicTreeDefaultTree32Storage : public hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage16
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeDefaultTree32Storage) == 0x28);

	class hkcdDynamicTreeDynamicStorage32 : public hkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecRawUint
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeDynamicStorage32) == 0x18);

	class hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage32 : public hkcdDynamicTreeDynamicStorage32
	{
	public:
		/* 0x018 */ std::uint32_t m_numLeaves;
		/* 0x01C */ std::uint32_t m_path;
		/* 0x020 */ std::uint32_t m_root;
		/* 0x024 */ std::byte pad024[0x4];
	};
	static_assert(sizeof(hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage32) == 0x28);
	static_assert(offsetof(hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage32, m_numLeaves) == 0x18);
	static_assert(offsetof(hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage32, m_path) == 0x1C);
	static_assert(offsetof(hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage32, m_root) == 0x20);

	class hkcdDynamicTreeDefaultTree48Storage : public hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorage32
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeDefaultTree48Storage) == 0x28);

	class hkcdDynamicTreeDynamicStorageInt16 : public hkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecInt16
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeDynamicStorageInt16) == 0x18);

	class hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorageInt16 : public hkcdDynamicTreeDynamicStorageInt16
	{
	public:
		/* 0x018 */ std::uint32_t m_numLeaves;
		/* 0x01C */ std::uint32_t m_path;
		/* 0x020 */ std::uint32_t m_root;
		/* 0x024 */ std::byte pad024[0x4];
	};
	static_assert(sizeof(hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorageInt16) == 0x28);
	static_assert(offsetof(hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorageInt16, m_numLeaves) == 0x18);
	static_assert(offsetof(hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorageInt16, m_path) == 0x1C);
	static_assert(offsetof(hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorageInt16, m_root) == 0x20);

	class hkcdDynamicTreeDefaultTreeInt16Storage : public hkcdDynamicTreeTreehkcdDynamicTreeDynamicStorageInt16
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeDefaultTreeInt16Storage) == 0x28);

	class hkcdDynamicTreeDynamicStoragePtr : public hkcdDynamicTreeDefaultDynamicStoragehkcdDynamicTreeCodecRawUlong
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeDynamicStoragePtr) == 0x18);

	class hkcdDynamicTreeTreehkcdDynamicTreeDynamicStoragePtr : public hkcdDynamicTreeDynamicStoragePtr
	{
	public:
		/* 0x018 */ std::uint32_t m_numLeaves;
		/* 0x01C */ std::uint32_t m_path;
		/* 0x020 */ std::size_t m_root;
	};
	static_assert(sizeof(hkcdDynamicTreeTreehkcdDynamicTreeDynamicStoragePtr) == 0x28);
	static_assert(offsetof(hkcdDynamicTreeTreehkcdDynamicTreeDynamicStoragePtr, m_numLeaves) == 0x18);
	static_assert(offsetof(hkcdDynamicTreeTreehkcdDynamicTreeDynamicStoragePtr, m_path) == 0x1C);
	static_assert(offsetof(hkcdDynamicTreeTreehkcdDynamicTreeDynamicStoragePtr, m_root) == 0x20);

	class hkcdDynamicTreeDefaultTreePtrStorage : public hkcdDynamicTreeTreehkcdDynamicTreeDynamicStoragePtr
	{
	public:
	};
	static_assert(sizeof(hkcdDynamicTreeDefaultTreePtrStorage) == 0x28);

	class hkcdFourAabb
	{
	public:
		/* 0x000 */ hkVector4 m_lx;
		/* 0x010 */ hkVector4 m_hx;
		/* 0x020 */ hkVector4 m_ly;
		/* 0x030 */ hkVector4 m_hy;
		/* 0x040 */ hkVector4 m_lz;
		/* 0x050 */ hkVector4 m_hz;
	};
	static_assert(sizeof(hkcdFourAabb) == 0x60);
	static_assert(offsetof(hkcdFourAabb, m_lx) == 0x0);
	static_assert(offsetof(hkcdFourAabb, m_hx) == 0x10);
	static_assert(offsetof(hkcdFourAabb, m_ly) == 0x20);
	static_assert(offsetof(hkcdFourAabb, m_hy) == 0x30);
	static_assert(offsetof(hkcdFourAabb, m_lz) == 0x40);
	static_assert(offsetof(hkcdFourAabb, m_hz) == 0x50);

	class hkcdPlanarCsgOperand : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkcdPlanarGeometry* m_geometry;
		/* 0x018 */ hkcdPlanarGeometry* m_danglingGeometry;
		/* 0x020 */ hkcdPlanarSolid* m_solid;
		/* 0x028 */ hkArray<hkcdPlanarCsgOperandGeomSource> m_geomSources;
		/* 0x038 */ void* m_regions;
	};
	static_assert(sizeof(hkcdPlanarCsgOperand) == 0x40);
	static_assert(offsetof(hkcdPlanarCsgOperand, m_geometry) == 0x10);
	static_assert(offsetof(hkcdPlanarCsgOperand, m_danglingGeometry) == 0x18);
	static_assert(offsetof(hkcdPlanarCsgOperand, m_solid) == 0x20);
	static_assert(offsetof(hkcdPlanarCsgOperand, m_geomSources) == 0x28);
	static_assert(offsetof(hkcdPlanarCsgOperand, m_regions) == 0x38);

	class hkcdPlanarCsgOperandGeomSource
	{
	public:
		/* 0x000 */ hkcdPlanarGeometry* m_geometry;
		/* 0x008 */ std::int32_t m_materialOffset;
		/* 0x00C */ std::int32_t m_numMaterialIds;
		/* 0x010 */ std::uint32_t m_flipPolygons;
		/* 0x014 */ std::byte pad014[0x4];
	};
	static_assert(sizeof(hkcdPlanarCsgOperandGeomSource) == 0x18);
	static_assert(offsetof(hkcdPlanarCsgOperandGeomSource, m_geometry) == 0x0);
	static_assert(offsetof(hkcdPlanarCsgOperandGeomSource, m_materialOffset) == 0x8);
	static_assert(offsetof(hkcdPlanarCsgOperandGeomSource, m_numMaterialIds) == 0xC);
	static_assert(offsetof(hkcdPlanarCsgOperandGeomSource, m_flipPolygons) == 0x10);

	class hkcdPlanarEntity : public hkReferencedObject
	{
	public:
		virtual void hkcdPlanarEntity_Unk04();
		/* 0x010 */ void* m_debugger;
	};
	static_assert(sizeof(hkcdPlanarEntity) == 0x18);
	static_assert(offsetof(hkcdPlanarEntity, m_debugger) == 0x10);

	class hkcdPlanarGeometry : public hkcdPlanarEntity
	{
	public:
		class VertexStorage :
			public hkReferencedObject, public hkArray<hkVector4d> {};
		static_assert(sizeof(VertexStorage) == 0x20);

		/* 0x018 */ hkcdPlanarGeometryPlanesCollection* m_planes;
		/* 0x020 */ hkcdPlanarGeometryPolygonCollection* m_polys;
		/* 0x028 */ void* m_vertices;
	};
	static_assert(sizeof(hkcdPlanarGeometry) == 0x30);
	static_assert(offsetof(hkcdPlanarGeometry, m_planes) == 0x18);
	static_assert(offsetof(hkcdPlanarGeometry, m_polys) == 0x20);
	static_assert(offsetof(hkcdPlanarGeometry, m_vertices) == 0x28);

	class hkcdPlanarGeometryPlanesCollection : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkVector4 m_offsetAndScale;
		/* 0x020 */ hkArray<hkcdPlanarGeometryPrimitivesPlane> m_planes;
		/* 0x030 */ void* m_cache;
		/* 0x038 */ void* m_criticalAccess;
	};
	static_assert(sizeof(hkcdPlanarGeometryPlanesCollection) == 0x40);
	static_assert(offsetof(hkcdPlanarGeometryPlanesCollection, m_offsetAndScale) == 0x10);
	static_assert(offsetof(hkcdPlanarGeometryPlanesCollection, m_planes) == 0x20);
	static_assert(offsetof(hkcdPlanarGeometryPlanesCollection, m_cache) == 0x30);
	static_assert(offsetof(hkcdPlanarGeometryPlanesCollection, m_criticalAccess) == 0x38);

	class hkcdPlanarGeometryPrimitivesCollection28 : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<std::int32_t> m_storage;
		/* 0x020 */ std::uint32_t m_primaryBitmap;
		/* 0x024 */ std::uint32_t m_secondaryBitmaps[26];
		/* 0x08C */ std::uint32_t m_freeBlocks[832];
		/* 0xD8C */ std::byte padD8C[0x4];
	};
	static_assert(sizeof(hkcdPlanarGeometryPrimitivesCollection28) == 0xD90);
	static_assert(offsetof(hkcdPlanarGeometryPrimitivesCollection28, m_storage) == 0x10);
	static_assert(offsetof(hkcdPlanarGeometryPrimitivesCollection28, m_primaryBitmap) == 0x20);
	static_assert(offsetof(hkcdPlanarGeometryPrimitivesCollection28, m_secondaryBitmaps) == 0x24);
	static_assert(offsetof(hkcdPlanarGeometryPrimitivesCollection28, m_freeBlocks) == 0x8C);

	class hkcdPlanarGeometryPolygonCollection : public hkcdPlanarGeometryPrimitivesCollection28
	{
	public:
	};
	static_assert(sizeof(hkcdPlanarGeometryPolygonCollection) == 0xD90);

	class hkcdPlanarGeometryPrimitivesPlane
	{
	public:
		/* 0x000 */ std::int64_t m_iEqn[4];
		/* 0x020 */ std::int64_t m_dEqn[4];
	};
	static_assert(sizeof(hkcdPlanarGeometryPrimitivesPlane) == 0x40);
	static_assert(offsetof(hkcdPlanarGeometryPrimitivesPlane, m_iEqn) == 0x0);
	static_assert(offsetof(hkcdPlanarGeometryPrimitivesPlane, m_dEqn) == 0x20);

	class hkcdPlanarSolidNodeStorage : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkcdPlanarSolidNode> m_storage;
		/* 0x020 */ std::uint32_t m_firstFreeNodeId;
		/* 0x024 */ std::byte pad024[0x4];
	};
	static_assert(sizeof(hkcdPlanarSolidNodeStorage) == 0x28);
	static_assert(offsetof(hkcdPlanarSolidNodeStorage, m_storage) == 0x10);
	static_assert(offsetof(hkcdPlanarSolidNodeStorage, m_firstFreeNodeId) == 0x20);

	class hkcdPlanarSolid : public hkcdPlanarEntity
	{
	public:
		using NodeStorage = hkcdPlanarSolidNodeStorage;

		class ArrayMgr : public hkcdPlanarGeometryPrimitivesCollection28
		{
		public:
			/* 0xD90 */ std::byte padD90[0x18];
		};
		static_assert(sizeof(ArrayMgr) == 0xDA8);

		/* 0x018 */ hkcdPlanarSolidNodeStorage* m_nodes;
		/* 0x020 */ hkcdPlanarGeometryPlanesCollection* m_planes;
		/* 0x028 */ std::uint32_t m_rootNodeId;
		/* 0x02C */ std::byte pad02C[0x4];
	};
	static_assert(sizeof(hkcdPlanarSolid) == 0x30);
	static_assert(offsetof(hkcdPlanarSolid, m_nodes) == 0x18);
	static_assert(offsetof(hkcdPlanarSolid, m_planes) == 0x20);
	static_assert(offsetof(hkcdPlanarSolid, m_rootNodeId) == 0x28);

	class hkcdPlanarSolidNode
	{
	public:
		/* 0x000 */ std::uint32_t m_parent;
		/* 0x004 */ std::uint32_t m_left;
		/* 0x008 */ std::uint32_t m_right;
		/* 0x00C */ std::uint32_t m_nextFreeNodeId;
		/* 0x010 */ std::uint32_t m_planeId;
		/* 0x014 */ std::uint32_t m_data;
		/* 0x018 */ std::uint32_t m_typeAndFlags;
	};
	static_assert(sizeof(hkcdPlanarSolidNode) == 0x1C);
	static_assert(offsetof(hkcdPlanarSolidNode, m_parent) == 0x0);
	static_assert(offsetof(hkcdPlanarSolidNode, m_left) == 0x4);
	static_assert(offsetof(hkcdPlanarSolidNode, m_right) == 0x8);
	static_assert(offsetof(hkcdPlanarSolidNode, m_nextFreeNodeId) == 0xC);
	static_assert(offsetof(hkcdPlanarSolidNode, m_planeId) == 0x10);
	static_assert(offsetof(hkcdPlanarSolidNode, m_data) == 0x14);
	static_assert(offsetof(hkcdPlanarSolidNode, m_typeAndFlags) == 0x18);

	class hkcdShapeDispatchType
	{
	public:
	};
	static_assert(sizeof(hkcdShapeDispatchType) == 0x1);

	class hkcdShapeInfoCodecType
	{
	public:
	};
	static_assert(sizeof(hkcdShapeInfoCodecType) == 0x1);

	class hkcdShapeType
	{
	public:
	};
	static_assert(sizeof(hkcdShapeType) == 0x1);

	class hkcdSimdTree : public hkBaseObject
	{
	public:
		class BuildContext
		{
		public:
			virtual ~BuildContext();
			virtual void BuildContext_Unk01();

			/* 0x008 */ std::byte pad008[0x28];
		};
		static_assert(sizeof(BuildContext) == 0x30);

		class LeafCollector
		{
		public:
			virtual ~LeafCollector();
			virtual void LeafCollector_Unk01();
		};
		static_assert(sizeof(LeafCollector) == 0x8);

		class IParallelBuild
		{
		public:
			class IRunnable
			{
			public:
				virtual ~IRunnable();
				virtual void IRunnable_Unk01();
			};
			static_assert(sizeof(IRunnable) == 0x8);
		};

		virtual void hkcdSimdTree_Unk02();
		/* 0x008 */ hkArray<hkcdSimdTreeNode> m_nodes;
	};
	static_assert(sizeof(hkcdSimdTree) == 0x18);
	static_assert(offsetof(hkcdSimdTree, m_nodes) == 0x8);

	class hkcdSimdTreeNode : public hkcdFourAabb
	{
	public:
		/* 0x060 */ std::uint32_t m_data[4];
	};
	static_assert(sizeof(hkcdSimdTreeNode) == 0x70);
	static_assert(offsetof(hkcdSimdTreeNode, m_data) == 0x60);

	class hkcdStaticAabbTree : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkBool m_shouldDeleteTree;
		/* 0x011 */ std::byte pad011[0x7];
		/* 0x018 */ hkcdStaticTreeDefaultTreeStorage6* m_treePtr;
	};
	static_assert(sizeof(hkcdStaticAabbTree) == 0x20);
	static_assert(offsetof(hkcdStaticAabbTree, m_shouldDeleteTree) == 0x10);
	static_assert(offsetof(hkcdStaticAabbTree, m_treePtr) == 0x18);

	class hkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis5
	{
	public:
		/* 0x000 */ hkArray<hkcdStaticTreeCodec3Axis5> m_nodes;
	};
	static_assert(sizeof(hkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis5) == 0x10);
	static_assert(offsetof(hkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis5, m_nodes) == 0x0);

	class hkcdStaticTreeDynamicStorage5 : public hkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis5
	{
	public:
	};
	static_assert(sizeof(hkcdStaticTreeDynamicStorage5) == 0x10);

	class hkcdStaticTreeTreehkcdStaticTreeDynamicStorage5 : public hkcdStaticTreeDynamicStorage5
	{
	public:
		/* 0x010 */ hkAabb m_domain;
	};
	static_assert(sizeof(hkcdStaticTreeTreehkcdStaticTreeDynamicStorage5) == 0x30);
	static_assert(offsetof(hkcdStaticTreeTreehkcdStaticTreeDynamicStorage5, m_domain) == 0x10);

	class hkcdStaticMeshTreeBase : public hkcdStaticTreeTreehkcdStaticTreeDynamicStorage5
	{
	public:
		/* 0x030 */ std::int32_t m_numPrimitiveKeys;
		/* 0x034 */ std::int32_t m_bitsPerKey;
		/* 0x038 */ std::uint32_t m_maxKeyValue;
		/* 0x03C */ std::byte pad03C[0x4];
		/* 0x040 */ hkArray<hkcdStaticMeshTreeBaseSection> m_sections;
		/* 0x050 */ hkArray<hkcdStaticMeshTreeBasePrimitive> m_primitives;
		/* 0x060 */ hkArray<std::uint16_t> m_sharedVerticesIndex;
	};
	static_assert(sizeof(hkcdStaticMeshTreeBase) == 0x70);
	static_assert(offsetof(hkcdStaticMeshTreeBase, m_numPrimitiveKeys) == 0x30);
	static_assert(offsetof(hkcdStaticMeshTreeBase, m_bitsPerKey) == 0x34);
	static_assert(offsetof(hkcdStaticMeshTreeBase, m_maxKeyValue) == 0x38);
	static_assert(offsetof(hkcdStaticMeshTreeBase, m_sections) == 0x40);
	static_assert(offsetof(hkcdStaticMeshTreeBase, m_primitives) == 0x50);
	static_assert(offsetof(hkcdStaticMeshTreeBase, m_sharedVerticesIndex) == 0x60);

	class hkcdStaticMeshTreeBasePrimitive
	{
	public:
		/* 0x000 */ std::uint8_t m_indices[4];
	};
	static_assert(sizeof(hkcdStaticMeshTreeBasePrimitive) == 0x4);
	static_assert(offsetof(hkcdStaticMeshTreeBasePrimitive, m_indices) == 0x0);

	class hkcdStaticMeshTreeBasePrimitiveDataRunBasehknpCompressedMeshShapeTreeDataRunData
	{
	public:
		/* 0x000 */ std::byte pad000[0x2];
		/* 0x002 */ std::uint8_t m_index;
		/* 0x003 */ std::uint8_t m_count;
	};
	static_assert(sizeof(hkcdStaticMeshTreeBasePrimitiveDataRunBasehknpCompressedMeshShapeTreeDataRunData) == 0x4);
	static_assert(offsetof(hkcdStaticMeshTreeBasePrimitiveDataRunBasehknpCompressedMeshShapeTreeDataRunData, m_index) == 0x2);
	static_assert(offsetof(hkcdStaticMeshTreeBasePrimitiveDataRunBasehknpCompressedMeshShapeTreeDataRunData, m_count) == 0x3);

	class hkcdStaticMeshTreeBasePrimitiveDataRunBaseunsignedshort
	{
	public:
		/* 0x000 */ std::uint16_t m_value;
		/* 0x002 */ std::uint8_t m_index;
		/* 0x003 */ std::uint8_t m_count;
	};
	static_assert(sizeof(hkcdStaticMeshTreeBasePrimitiveDataRunBaseunsignedshort) == 0x4);
	static_assert(offsetof(hkcdStaticMeshTreeBasePrimitiveDataRunBaseunsignedshort, m_value) == 0x0);
	static_assert(offsetof(hkcdStaticMeshTreeBasePrimitiveDataRunBaseunsignedshort, m_index) == 0x2);
	static_assert(offsetof(hkcdStaticMeshTreeBasePrimitiveDataRunBaseunsignedshort, m_count) == 0x3);

	class hkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis4
	{
	public:
		/* 0x000 */ hkArray<hkcdStaticTreeCodec3Axis4> m_nodes;
	};
	static_assert(sizeof(hkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis4) == 0x10);
	static_assert(offsetof(hkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis4, m_nodes) == 0x0);

	class hkcdStaticTreeDynamicStorage4 : public hkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis4
	{
	public:
	};
	static_assert(sizeof(hkcdStaticTreeDynamicStorage4) == 0x10);

	class hkcdStaticTreeTreehkcdStaticTreeDynamicStorage4 : public hkcdStaticTreeDynamicStorage4
	{
	public:
		/* 0x010 */ hkAabb m_domain;
	};
	static_assert(sizeof(hkcdStaticTreeTreehkcdStaticTreeDynamicStorage4) == 0x30);
	static_assert(offsetof(hkcdStaticTreeTreehkcdStaticTreeDynamicStorage4, m_domain) == 0x10);

	class hkcdStaticMeshTreeBaseSectionSharedVertices
	{
	public:
		/* 0x000 */ std::uint32_t m_data;
	};
	static_assert(sizeof(hkcdStaticMeshTreeBaseSectionSharedVertices) == 0x4);
	static_assert(offsetof(hkcdStaticMeshTreeBaseSectionSharedVertices, m_data) == 0x0);

	class hkcdStaticMeshTreeBaseSectionPrimitives
	{
	public:
		/* 0x000 */ std::uint32_t m_data;
	};
	static_assert(sizeof(hkcdStaticMeshTreeBaseSectionPrimitives) == 0x4);
	static_assert(offsetof(hkcdStaticMeshTreeBaseSectionPrimitives, m_data) == 0x0);

	class hkcdStaticMeshTreeBaseSectionDataRuns
	{
	public:
		/* 0x000 */ std::uint32_t m_data;
	};
	static_assert(sizeof(hkcdStaticMeshTreeBaseSectionDataRuns) == 0x4);
	static_assert(offsetof(hkcdStaticMeshTreeBaseSectionDataRuns, m_data) == 0x0);

	class hkcdStaticMeshTreeBaseSection : public hkcdStaticTreeTreehkcdStaticTreeDynamicStorage4
	{
	public:
		/* 0x030 */ float m_codecParms[6];
		/* 0x048 */ std::uint32_t m_firstPackedVertex;
		/* 0x04C */ hkcdStaticMeshTreeBaseSectionSharedVertices m_sharedVertices;
		/* 0x050 */ hkcdStaticMeshTreeBaseSectionPrimitives m_primitives;
		/* 0x054 */ hkcdStaticMeshTreeBaseSectionDataRuns m_dataRuns;
		/* 0x058 */ std::uint8_t m_numPackedVertices;
		/* 0x059 */ std::uint8_t m_numSharedIndices;
		/* 0x05A */ std::uint16_t m_leafIndex;
		/* 0x05C */ std::uint8_t m_page;
		/* 0x05D */ std::uint8_t m_flags;
		/* 0x05E */ std::uint8_t m_layerData;
		/* 0x05F */ std::uint8_t m_unusedData;
	};
	static_assert(sizeof(hkcdStaticMeshTreeBaseSection) == 0x60);
	static_assert(offsetof(hkcdStaticMeshTreeBaseSection, m_codecParms) == 0x30);
	static_assert(offsetof(hkcdStaticMeshTreeBaseSection, m_firstPackedVertex) == 0x48);
	static_assert(offsetof(hkcdStaticMeshTreeBaseSection, m_sharedVertices) == 0x4C);
	static_assert(offsetof(hkcdStaticMeshTreeBaseSection, m_primitives) == 0x50);
	static_assert(offsetof(hkcdStaticMeshTreeBaseSection, m_dataRuns) == 0x54);
	static_assert(offsetof(hkcdStaticMeshTreeBaseSection, m_numPackedVertices) == 0x58);
	static_assert(offsetof(hkcdStaticMeshTreeBaseSection, m_numSharedIndices) == 0x59);
	static_assert(offsetof(hkcdStaticMeshTreeBaseSection, m_leafIndex) == 0x5A);
	static_assert(offsetof(hkcdStaticMeshTreeBaseSection, m_page) == 0x5C);
	static_assert(offsetof(hkcdStaticMeshTreeBaseSection, m_flags) == 0x5D);
	static_assert(offsetof(hkcdStaticMeshTreeBaseSection, m_layerData) == 0x5E);
	static_assert(offsetof(hkcdStaticMeshTreeBaseSection, m_unusedData) == 0x5F);

	class hkcdStaticMeshTreeDefaultDataRun : public hkcdStaticMeshTreeBasePrimitiveDataRunBaseunsignedshort
	{
	public:
	};
	static_assert(sizeof(hkcdStaticMeshTreeDefaultDataRun) == 0x4);

	class hkcdStaticMeshTreehkcdStaticMeshTreeCommonConfigunsignedintunsignedlonglong1121hknpCompressedMeshShapeTreeDataRun : public hkcdStaticMeshTreeBase
	{
	public:
		/* 0x070 */ hkArray<std::uint32_t> m_packedVertices;
		/* 0x080 */ hkArray<std::uint64_t> m_sharedVertices;
		/* 0x090 */ std::byte pad090[0x10];
	};
	static_assert(sizeof(hkcdStaticMeshTreehkcdStaticMeshTreeCommonConfigunsignedintunsignedlonglong1121hknpCompressedMeshShapeTreeDataRun) == 0xA0);
	static_assert(offsetof(hkcdStaticMeshTreehkcdStaticMeshTreeCommonConfigunsignedintunsignedlonglong1121hknpCompressedMeshShapeTreeDataRun, m_packedVertices) == 0x70);
	static_assert(offsetof(hkcdStaticMeshTreehkcdStaticMeshTreeCommonConfigunsignedintunsignedlonglong1121hknpCompressedMeshShapeTreeDataRun, m_sharedVertices) == 0x80);

	class hkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis6
	{
	public:
		/* 0x000 */ hkArray<hkcdStaticTreeCodec3Axis6> m_nodes;
	};
	static_assert(sizeof(hkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis6) == 0x10);
	static_assert(offsetof(hkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis6, m_nodes) == 0x0);

	class hkcdStaticTreeDynamicStorage6 : public hkcdStaticTreeDynamicStoragehkcdStaticTreeCodec3Axis6
	{
	public:
	};
	static_assert(sizeof(hkcdStaticTreeDynamicStorage6) == 0x10);

	class hkcdStaticTreeTreehkcdStaticTreeDynamicStorage6 : public hkcdStaticTreeDynamicStorage6
	{
	public:
		/* 0x010 */ hkAabb m_domain;
	};
	static_assert(sizeof(hkcdStaticTreeTreehkcdStaticTreeDynamicStorage6) == 0x30);
	static_assert(offsetof(hkcdStaticTreeTreehkcdStaticTreeDynamicStorage6, m_domain) == 0x10);

	class hkcdStaticPvs
	{
	public:
		/* 0x000 */ hkcdStaticTreeTreehkcdStaticTreeDynamicStorage6 m_cells;
		/* 0x030 */ std::int32_t m_bytesPerCells;
		/* 0x034 */ std::int32_t m_cellsPerBlock;
		/* 0x038 */ hkArray<std::uint8_t> m_pvs;
		/* 0x048 */ hkArray<std::uint16_t> m_map;
		/* 0x058 */ hkArray<hkcdStaticPvsBlockHeader> m_blocks;
		/* 0x068 */ std::byte pad068[0x8];
	};
	static_assert(sizeof(hkcdStaticPvs) == 0x70);
	static_assert(offsetof(hkcdStaticPvs, m_cells) == 0x0);
	static_assert(offsetof(hkcdStaticPvs, m_bytesPerCells) == 0x30);
	static_assert(offsetof(hkcdStaticPvs, m_cellsPerBlock) == 0x34);
	static_assert(offsetof(hkcdStaticPvs, m_pvs) == 0x38);
	static_assert(offsetof(hkcdStaticPvs, m_map) == 0x48);
	static_assert(offsetof(hkcdStaticPvs, m_blocks) == 0x58);

	class hkcdStaticPvsBlockHeader
	{
	public:
		/* 0x000 */ std::uint32_t m_offset;
		/* 0x004 */ std::uint32_t m_length;
	};
	static_assert(sizeof(hkcdStaticPvsBlockHeader) == 0x8);
	static_assert(offsetof(hkcdStaticPvsBlockHeader, m_offset) == 0x0);
	static_assert(offsetof(hkcdStaticPvsBlockHeader, m_length) == 0x4);

	class hkcdStaticTreeCodec3Axis
	{
	public:
		/* 0x000 */ std::uint8_t m_xyz[3];
	};
	static_assert(sizeof(hkcdStaticTreeCodec3Axis) == 0x3);
	static_assert(offsetof(hkcdStaticTreeCodec3Axis, m_xyz) == 0x0);

	class hkcdStaticTreeCodec3Axis4 : public hkcdStaticTreeCodec3Axis
	{
	public:
		/* 0x003 */ std::uint8_t m_data;
	};
	static_assert(sizeof(hkcdStaticTreeCodec3Axis4) == 0x4);
	static_assert(offsetof(hkcdStaticTreeCodec3Axis4, m_data) == 0x3);

	class hkcdStaticTreeCodec3Axis5 : public hkcdStaticTreeCodec3Axis
	{
	public:
		/* 0x003 */ std::uint8_t m_hiData;
		/* 0x004 */ std::uint8_t m_loData;
	};
	static_assert(sizeof(hkcdStaticTreeCodec3Axis5) == 0x5);
	static_assert(offsetof(hkcdStaticTreeCodec3Axis5, m_hiData) == 0x3);
	static_assert(offsetof(hkcdStaticTreeCodec3Axis5, m_loData) == 0x4);

	class hkcdStaticTreeCodec3Axis6 : public hkcdStaticTreeCodec3Axis
	{
	public:
		/* 0x003 */ std::uint8_t m_hiData;
		/* 0x004 */ std::uint16_t m_loData;
	};
	static_assert(sizeof(hkcdStaticTreeCodec3Axis6) == 0x6);
	static_assert(offsetof(hkcdStaticTreeCodec3Axis6, m_hiData) == 0x3);
	static_assert(offsetof(hkcdStaticTreeCodec3Axis6, m_loData) == 0x4);

	class hkcdStaticTreeCodecRaw
	{
	public:
		/* 0x000 */ hkAabb m_aabb;
	};
	static_assert(sizeof(hkcdStaticTreeCodecRaw) == 0x20);
	static_assert(offsetof(hkcdStaticTreeCodecRaw, m_aabb) == 0x0);

	class hkcdStaticTreeDynamicStoragehkcdStaticTreeCodecRaw
	{
	public:
		/* 0x000 */ hkArray<hkcdStaticTreeCodecRaw> m_nodes;
	};
	static_assert(sizeof(hkcdStaticTreeDynamicStoragehkcdStaticTreeCodecRaw) == 0x10);
	static_assert(offsetof(hkcdStaticTreeDynamicStoragehkcdStaticTreeCodecRaw, m_nodes) == 0x0);

	class hkcdStaticTreeDynamicStorage32 : public hkcdStaticTreeDynamicStoragehkcdStaticTreeCodecRaw
	{
	public:
	};
	static_assert(sizeof(hkcdStaticTreeDynamicStorage32) == 0x10);

	class hkcdStaticTreeTreehkcdStaticTreeDynamicStorage32 : public hkcdStaticTreeDynamicStorage32
	{
	public:
		/* 0x010 */ hkAabb m_domain;
	};
	static_assert(sizeof(hkcdStaticTreeTreehkcdStaticTreeDynamicStorage32) == 0x30);
	static_assert(offsetof(hkcdStaticTreeTreehkcdStaticTreeDynamicStorage32, m_domain) == 0x10);

	class hkcdStaticTreeDefaultTreeStorage32 : public hkcdStaticTreeTreehkcdStaticTreeDynamicStorage32
	{
	public:
	};
	static_assert(sizeof(hkcdStaticTreeDefaultTreeStorage32) == 0x30);

	class hkcdStaticTreeDefaultTreeStorage4 : public hkcdStaticTreeTreehkcdStaticTreeDynamicStorage4
	{
	public:
	};
	static_assert(sizeof(hkcdStaticTreeDefaultTreeStorage4) == 0x30);

	class hkcdStaticTreeDefaultTreeStorage5 : public hkcdStaticTreeTreehkcdStaticTreeDynamicStorage5
	{
	public:
	};
	static_assert(sizeof(hkcdStaticTreeDefaultTreeStorage5) == 0x30);

	class hkcdStaticTreeDefaultTreeStorage6 : public hkcdStaticTreeTreehkcdStaticTreeDynamicStorage6
	{
	public:
	};
	static_assert(sizeof(hkcdStaticTreeDefaultTreeStorage6) == 0x30);

	class hkxAnimatedFloat : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<float> m_floats;
		/* 0x020 */ std::uint8_t m_hint;
		/* 0x021 */ std::byte pad021[0x7];
	};
	static_assert(sizeof(hkxAnimatedFloat) == 0x28);
	static_assert(offsetof(hkxAnimatedFloat, m_floats) == 0x10);
	static_assert(offsetof(hkxAnimatedFloat, m_hint) == 0x20);

	class hkxAnimatedMatrix : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<float> m_matrices;
		/* 0x020 */ std::uint8_t m_hint;
		/* 0x021 */ std::byte pad021[0x7];
	};
	static_assert(sizeof(hkxAnimatedMatrix) == 0x28);
	static_assert(offsetof(hkxAnimatedMatrix, m_matrices) == 0x10);
	static_assert(offsetof(hkxAnimatedMatrix, m_hint) == 0x20);

	class hkxAnimatedQuaternion : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<float> m_quaternions;
	};
	static_assert(sizeof(hkxAnimatedQuaternion) == 0x20);
	static_assert(offsetof(hkxAnimatedQuaternion, m_quaternions) == 0x10);

	class hkxAnimatedVector : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<float> m_vectors;
		/* 0x020 */ std::uint8_t m_hint;
		/* 0x021 */ std::byte pad021[0x7];
	};
	static_assert(sizeof(hkxAnimatedVector) == 0x28);
	static_assert(offsetof(hkxAnimatedVector, m_vectors) == 0x10);
	static_assert(offsetof(hkxAnimatedVector, m_hint) == 0x20);

	class hkxAttribute
	{
	public:
		/* 0x000 */ hkStringPtr m_name;
		/* 0x008 */ hkReferencedObject* m_value;
	};
	static_assert(sizeof(hkxAttribute) == 0x10);
	static_assert(offsetof(hkxAttribute, m_name) == 0x0);
	static_assert(offsetof(hkxAttribute, m_value) == 0x8);

	class hkxAttributeGroup
	{
	public:
		/* 0x000 */ hkStringPtr m_name;
		/* 0x008 */ hkArray<hkxAttribute> m_attributes;
	};
	static_assert(sizeof(hkxAttributeGroup) == 0x18);
	static_assert(offsetof(hkxAttributeGroup, m_name) == 0x0);
	static_assert(offsetof(hkxAttributeGroup, m_attributes) == 0x8);

	class hkxAttributeHolder : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkxAttributeGroup> m_attributeGroups;
	};
	static_assert(sizeof(hkxAttributeHolder) == 0x20);
	static_assert(offsetof(hkxAttributeHolder, m_attributeGroups) == 0x10);

	class hkxBlob : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<std::uint8_t> m_data;
	};
	static_assert(sizeof(hkxBlob) == 0x20);
	static_assert(offsetof(hkxBlob, m_data) == 0x10);

	class hkxBlobMeshShape : public hkMeshShape
	{
	public:
		/* 0x010 */ hkxBlob m_blob;
		/* 0x030 */ hkStringPtr m_name;
	};
	static_assert(sizeof(hkxBlobMeshShape) == 0x38);
	static_assert(offsetof(hkxBlobMeshShape, m_blob) == 0x10);
	static_assert(offsetof(hkxBlobMeshShape, m_name) == 0x30);

	class hkxCamera : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkVector4 m_from;
		/* 0x020 */ hkVector4 m_focus;
		/* 0x030 */ hkVector4 m_up;
		/* 0x040 */ float m_fov;
		/* 0x044 */ float m_far;
		/* 0x048 */ float m_near;
		/* 0x04C */ hkBool m_leftHanded;
		/* 0x04D */ std::byte pad04D[0x3];
	};
	static_assert(sizeof(hkxCamera) == 0x50);
	static_assert(offsetof(hkxCamera, m_from) == 0x10);
	static_assert(offsetof(hkxCamera, m_focus) == 0x20);
	static_assert(offsetof(hkxCamera, m_up) == 0x30);
	static_assert(offsetof(hkxCamera, m_fov) == 0x40);
	static_assert(offsetof(hkxCamera, m_far) == 0x44);
	static_assert(offsetof(hkxCamera, m_near) == 0x48);
	static_assert(offsetof(hkxCamera, m_leftHanded) == 0x4C);

	class hkxEdgeSelectionChannel : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<std::int32_t> m_selectedEdges;
	};
	static_assert(sizeof(hkxEdgeSelectionChannel) == 0x20);
	static_assert(offsetof(hkxEdgeSelectionChannel, m_selectedEdges) == 0x10);

	class hkxEnum : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkxEnumItem> m_items;
	};
	static_assert(sizeof(hkxEnum) == 0x20);
	static_assert(offsetof(hkxEnum, m_items) == 0x10);

	class hkxEnumItem
	{
	public:
		/* 0x000 */ std::int32_t m_value;
		/* 0x004 */ std::byte pad004[0x4];
		/* 0x008 */ hkStringPtr m_name;
	};
	static_assert(sizeof(hkxEnumItem) == 0x10);
	static_assert(offsetof(hkxEnumItem, m_value) == 0x0);
	static_assert(offsetof(hkxEnumItem, m_name) == 0x8);

	class hkxEnvironment : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkxEnvironmentVariable> m_variables;
	};
	static_assert(sizeof(hkxEnvironment) == 0x20);
	static_assert(offsetof(hkxEnvironment, m_variables) == 0x10);

	class hkxEnvironmentVariable
	{
	public:
		/* 0x000 */ hkStringPtr m_name;
		/* 0x008 */ hkStringPtr m_value;
	};
	static_assert(sizeof(hkxEnvironmentVariable) == 0x10);
	static_assert(offsetof(hkxEnvironmentVariable, m_name) == 0x0);
	static_assert(offsetof(hkxEnvironmentVariable, m_value) == 0x8);

	class hkxIndexBuffer : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::int8_t m_indexType;
		/* 0x011 */ std::byte pad011[0x7];
		/* 0x018 */ hkArray<std::uint16_t> m_indices16;
		/* 0x028 */ hkArray<std::uint32_t> m_indices32;
		/* 0x038 */ std::uint32_t m_vertexBaseOffset;
		/* 0x03C */ std::uint32_t m_length;
	};
	static_assert(sizeof(hkxIndexBuffer) == 0x40);
	static_assert(offsetof(hkxIndexBuffer, m_indexType) == 0x10);
	static_assert(offsetof(hkxIndexBuffer, m_indices16) == 0x18);
	static_assert(offsetof(hkxIndexBuffer, m_indices32) == 0x28);
	static_assert(offsetof(hkxIndexBuffer, m_vertexBaseOffset) == 0x38);
	static_assert(offsetof(hkxIndexBuffer, m_length) == 0x3C);

	class hkxLight : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::int8_t m_type;
		/* 0x011 */ std::byte pad011[0xF];
		/* 0x020 */ hkVector4 m_position;
		/* 0x030 */ hkVector4 m_direction;
		/* 0x040 */ std::uint32_t m_color;
		/* 0x044 */ float m_angle;
		/* 0x048 */ float m_range;
		/* 0x04C */ float m_fadeStart;
		/* 0x050 */ float m_fadeEnd;
		/* 0x054 */ std::int16_t m_decayRate;
		/* 0x056 */ std::byte pad056[0x2];
		/* 0x058 */ float m_intensity;
		/* 0x05C */ hkBool m_shadowCaster;
		/* 0x05D */ std::byte pad05D[0x3];
	};
	static_assert(sizeof(hkxLight) == 0x60);
	static_assert(offsetof(hkxLight, m_type) == 0x10);
	static_assert(offsetof(hkxLight, m_position) == 0x20);
	static_assert(offsetof(hkxLight, m_direction) == 0x30);
	static_assert(offsetof(hkxLight, m_color) == 0x40);
	static_assert(offsetof(hkxLight, m_angle) == 0x44);
	static_assert(offsetof(hkxLight, m_range) == 0x48);
	static_assert(offsetof(hkxLight, m_fadeStart) == 0x4C);
	static_assert(offsetof(hkxLight, m_fadeEnd) == 0x50);
	static_assert(offsetof(hkxLight, m_decayRate) == 0x54);
	static_assert(offsetof(hkxLight, m_intensity) == 0x58);
	static_assert(offsetof(hkxLight, m_shadowCaster) == 0x5C);

	class hkxMaterial : public hkxAttributeHolder
	{
	public:
		/* 0x020 */ hkStringPtr m_name;
		/* 0x028 */ hkArray<hkxMaterialTextureStage> m_stages;
		/* 0x038 */ std::byte pad038[0x8];
		/* 0x040 */ hkVector4 m_diffuseColor;
		/* 0x050 */ hkVector4 m_ambientColor;
		/* 0x060 */ hkVector4 m_specularColor;
		/* 0x070 */ hkVector4 m_emissiveColor;
		/* 0x080 */ hkArray<hkxMaterial*> m_subMaterials;
		/* 0x090 */ hkReferencedObject* m_extraData;
		/* 0x098 */ float m_uvMapScale[2];
		/* 0x0A0 */ float m_uvMapOffset[2];
		/* 0x0A8 */ float m_uvMapRotation;
		/* 0x0AC */ std::uint32_t m_uvMapAlgorithm;
		/* 0x0B0 */ float m_specularMultiplier;
		/* 0x0B4 */ float m_specularExponent;
		/* 0x0B8 */ std::uint8_t m_transparency;
		/* 0x0B9 */ std::byte pad0B9[0x7];
		/* 0x0C0 */ std::size_t m_userData;
		/* 0x0C8 */ hkArray<hkxMaterialProperty> m_properties;
		/* 0x0D8 */ std::byte pad0D8[0x8];
	};
	static_assert(sizeof(hkxMaterial) == 0xE0);
	static_assert(offsetof(hkxMaterial, m_name) == 0x20);
	static_assert(offsetof(hkxMaterial, m_stages) == 0x28);
	static_assert(offsetof(hkxMaterial, m_diffuseColor) == 0x40);
	static_assert(offsetof(hkxMaterial, m_ambientColor) == 0x50);
	static_assert(offsetof(hkxMaterial, m_specularColor) == 0x60);
	static_assert(offsetof(hkxMaterial, m_emissiveColor) == 0x70);
	static_assert(offsetof(hkxMaterial, m_subMaterials) == 0x80);
	static_assert(offsetof(hkxMaterial, m_extraData) == 0x90);
	static_assert(offsetof(hkxMaterial, m_uvMapScale) == 0x98);
	static_assert(offsetof(hkxMaterial, m_uvMapOffset) == 0xA0);
	static_assert(offsetof(hkxMaterial, m_uvMapRotation) == 0xA8);
	static_assert(offsetof(hkxMaterial, m_uvMapAlgorithm) == 0xAC);
	static_assert(offsetof(hkxMaterial, m_specularMultiplier) == 0xB0);
	static_assert(offsetof(hkxMaterial, m_specularExponent) == 0xB4);
	static_assert(offsetof(hkxMaterial, m_transparency) == 0xB8);
	static_assert(offsetof(hkxMaterial, m_userData) == 0xC0);
	static_assert(offsetof(hkxMaterial, m_properties) == 0xC8);

	class hkxMaterialEffect : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkStringPtr m_name;
		/* 0x018 */ std::uint8_t m_type;
		/* 0x019 */ std::byte pad019[0x7];
		/* 0x020 */ hkArray<std::uint8_t> m_data;
	};
	static_assert(sizeof(hkxMaterialEffect) == 0x30);
	static_assert(offsetof(hkxMaterialEffect, m_name) == 0x10);
	static_assert(offsetof(hkxMaterialEffect, m_type) == 0x18);
	static_assert(offsetof(hkxMaterialEffect, m_data) == 0x20);

	class hkxMaterialProperty
	{
	public:
		/* 0x000 */ std::uint32_t m_key;
		/* 0x004 */ std::uint32_t m_value;
	};
	static_assert(sizeof(hkxMaterialProperty) == 0x8);
	static_assert(offsetof(hkxMaterialProperty, m_key) == 0x0);
	static_assert(offsetof(hkxMaterialProperty, m_value) == 0x4);

	class hkxMaterialShader : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkStringPtr m_name;
		/* 0x018 */ std::uint8_t m_type;
		/* 0x019 */ std::byte pad019[0x7];
		/* 0x020 */ hkStringPtr m_vertexEntryName;
		/* 0x028 */ hkStringPtr m_geomEntryName;
		/* 0x030 */ hkStringPtr m_pixelEntryName;
		/* 0x038 */ hkArray<std::uint8_t> m_data;
	};
	static_assert(sizeof(hkxMaterialShader) == 0x48);
	static_assert(offsetof(hkxMaterialShader, m_name) == 0x10);
	static_assert(offsetof(hkxMaterialShader, m_type) == 0x18);
	static_assert(offsetof(hkxMaterialShader, m_vertexEntryName) == 0x20);
	static_assert(offsetof(hkxMaterialShader, m_geomEntryName) == 0x28);
	static_assert(offsetof(hkxMaterialShader, m_pixelEntryName) == 0x30);
	static_assert(offsetof(hkxMaterialShader, m_data) == 0x38);

	class hkxMaterialShaderSet : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkxMaterialShader*> m_shaders;
	};
	static_assert(sizeof(hkxMaterialShaderSet) == 0x20);
	static_assert(offsetof(hkxMaterialShaderSet, m_shaders) == 0x10);

	class hkxMaterialTextureStage
	{
	public:
		/* 0x000 */ hkReferencedObject* m_texture;
		/* 0x008 */ std::int32_t m_usageHint;
		/* 0x00C */ std::int32_t m_tcoordChannel;
	};
	static_assert(sizeof(hkxMaterialTextureStage) == 0x10);
	static_assert(offsetof(hkxMaterialTextureStage, m_texture) == 0x0);
	static_assert(offsetof(hkxMaterialTextureStage, m_usageHint) == 0x8);
	static_assert(offsetof(hkxMaterialTextureStage, m_tcoordChannel) == 0xC);

	class hkxMeshUserChannelInfo : public hkxAttributeHolder
	{
	public:
		/* 0x020 */ hkStringPtr m_name;
		/* 0x028 */ hkStringPtr m_className;
	};
	static_assert(sizeof(hkxMeshUserChannelInfo) == 0x30);
	static_assert(offsetof(hkxMeshUserChannelInfo, m_name) == 0x20);
	static_assert(offsetof(hkxMeshUserChannelInfo, m_className) == 0x28);

	class hkxMesh : public hkReferencedObject
	{
	public:
		using UserChannelInfo = hkxMeshUserChannelInfo;

		/* 0x010 */ hkArray<hkxMeshSection*> m_sections;
		/* 0x020 */ hkArray<hkxMeshUserChannelInfo*> m_userChannelInfos;
	};
	static_assert(sizeof(hkxMesh) == 0x30);
	static_assert(offsetof(hkxMesh, m_sections) == 0x10);
	static_assert(offsetof(hkxMesh, m_userChannelInfos) == 0x20);

	class hkxMeshSection : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkxVertexBuffer* m_vertexBuffer;
		/* 0x018 */ hkArray<hkxIndexBuffer*> m_indexBuffers;
		/* 0x028 */ hkxMaterial* m_material;
		/* 0x030 */ hkArray<hkReferencedObject*> m_userChannels;
		/* 0x040 */ hkArray<hkxVertexAnimation*> m_vertexAnimations;
		/* 0x050 */ hkArray<float> m_linearKeyFrameHints;
		/* 0x060 */ hkArray<hkMeshBoneIndexMapping> m_boneMatrixMap;
	};
	static_assert(sizeof(hkxMeshSection) == 0x70);
	static_assert(offsetof(hkxMeshSection, m_vertexBuffer) == 0x10);
	static_assert(offsetof(hkxMeshSection, m_indexBuffers) == 0x18);
	static_assert(offsetof(hkxMeshSection, m_material) == 0x28);
	static_assert(offsetof(hkxMeshSection, m_userChannels) == 0x30);
	static_assert(offsetof(hkxMeshSection, m_vertexAnimations) == 0x40);
	static_assert(offsetof(hkxMeshSection, m_linearKeyFrameHints) == 0x50);
	static_assert(offsetof(hkxMeshSection, m_boneMatrixMap) == 0x60);

	class hkxNode : public hkxAttributeHolder
	{
	public:
		/* 0x020 */ hkStringPtr m_name;
		/* 0x028 */ hkReferencedObject* m_object;
		/* 0x030 */ hkArray<hkMatrix4> m_keyFrames;
		/* 0x040 */ hkArray<hkxNode*> m_children;
		/* 0x050 */ hkArray<hkxNodeAnnotationData> m_annotations;
		/* 0x060 */ hkArray<float> m_linearKeyFrameHints;
		/* 0x070 */ hkStringPtr m_userProperties;
		/* 0x078 */ hkBool m_selected;
		/* 0x079 */ hkBool m_bone;
		/* 0x07A */ std::byte pad07A[0x6];
	};
	static_assert(sizeof(hkxNode) == 0x80);
	static_assert(offsetof(hkxNode, m_name) == 0x20);
	static_assert(offsetof(hkxNode, m_object) == 0x28);
	static_assert(offsetof(hkxNode, m_keyFrames) == 0x30);
	static_assert(offsetof(hkxNode, m_children) == 0x40);
	static_assert(offsetof(hkxNode, m_annotations) == 0x50);
	static_assert(offsetof(hkxNode, m_linearKeyFrameHints) == 0x60);
	static_assert(offsetof(hkxNode, m_userProperties) == 0x70);
	static_assert(offsetof(hkxNode, m_selected) == 0x78);
	static_assert(offsetof(hkxNode, m_bone) == 0x79);

	class hkxNodeAnnotationData
	{
	public:
		/* 0x000 */ float m_time;
		/* 0x004 */ std::byte pad004[0x4];
		/* 0x008 */ hkStringPtr m_description;
	};
	static_assert(sizeof(hkxNodeAnnotationData) == 0x10);
	static_assert(offsetof(hkxNodeAnnotationData, m_time) == 0x0);
	static_assert(offsetof(hkxNodeAnnotationData, m_description) == 0x8);

	class hkxNodeSelectionSet : public hkxAttributeHolder
	{
	public:
		/* 0x020 */ hkArray<hkxNode*> m_selectedNodes;
		/* 0x030 */ hkStringPtr m_name;
	};
	static_assert(sizeof(hkxNodeSelectionSet) == 0x38);
	static_assert(offsetof(hkxNodeSelectionSet, m_selectedNodes) == 0x20);
	static_assert(offsetof(hkxNodeSelectionSet, m_name) == 0x30);

	class hkxScene : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkStringPtr m_modeller;
		/* 0x018 */ hkStringPtr m_asset;
		/* 0x020 */ float m_sceneLength;
		/* 0x024 */ std::uint32_t m_numFrames;
		/* 0x028 */ hkxNode* m_rootNode;
		/* 0x030 */ hkArray<hkxNodeSelectionSet*> m_selectionSets;
		/* 0x040 */ hkArray<hkxCamera*> m_cameras;
		/* 0x050 */ hkArray<hkxLight*> m_lights;
		/* 0x060 */ hkArray<hkxMesh*> m_meshes;
		/* 0x070 */ hkArray<hkxMaterial*> m_materials;
		/* 0x080 */ hkArray<hkxTextureInplace*> m_inplaceTextures;
		/* 0x090 */ hkArray<hkxTextureFile*> m_externalTextures;
		/* 0x0A0 */ hkArray<hkxSkinBinding*> m_skinBindings;
		/* 0x0B0 */ hkArray<hkxSpline*> m_splines;
		/* 0x0C0 */ hkMatrix3 m_appliedTransform;
	};
	static_assert(sizeof(hkxScene) == 0xF0);
	static_assert(offsetof(hkxScene, m_modeller) == 0x10);
	static_assert(offsetof(hkxScene, m_asset) == 0x18);
	static_assert(offsetof(hkxScene, m_sceneLength) == 0x20);
	static_assert(offsetof(hkxScene, m_numFrames) == 0x24);
	static_assert(offsetof(hkxScene, m_rootNode) == 0x28);
	static_assert(offsetof(hkxScene, m_selectionSets) == 0x30);
	static_assert(offsetof(hkxScene, m_cameras) == 0x40);
	static_assert(offsetof(hkxScene, m_lights) == 0x50);
	static_assert(offsetof(hkxScene, m_meshes) == 0x60);
	static_assert(offsetof(hkxScene, m_materials) == 0x70);
	static_assert(offsetof(hkxScene, m_inplaceTextures) == 0x80);
	static_assert(offsetof(hkxScene, m_externalTextures) == 0x90);
	static_assert(offsetof(hkxScene, m_skinBindings) == 0xA0);
	static_assert(offsetof(hkxScene, m_splines) == 0xB0);
	static_assert(offsetof(hkxScene, m_appliedTransform) == 0xC0);

	class hkxSkinBinding : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkxMesh* m_mesh;
		/* 0x018 */ hkArray<hkStringPtr> m_nodeNames;
		/* 0x028 */ hkArray<hkMatrix4> m_bindPose;
		/* 0x038 */ std::byte pad038[0x8];
		/* 0x040 */ hkMatrix4 m_initSkinTransform;
	};
	static_assert(sizeof(hkxSkinBinding) == 0x80);
	static_assert(offsetof(hkxSkinBinding, m_mesh) == 0x10);
	static_assert(offsetof(hkxSkinBinding, m_nodeNames) == 0x18);
	static_assert(offsetof(hkxSkinBinding, m_bindPose) == 0x28);
	static_assert(offsetof(hkxSkinBinding, m_initSkinTransform) == 0x40);

	class hkxSparselyAnimatedBool : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkBool> m_bools;
		/* 0x020 */ hkArray<float> m_times;
	};
	static_assert(sizeof(hkxSparselyAnimatedBool) == 0x30);
	static_assert(offsetof(hkxSparselyAnimatedBool, m_bools) == 0x10);
	static_assert(offsetof(hkxSparselyAnimatedBool, m_times) == 0x20);

	class hkxSparselyAnimatedInt : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<std::int32_t> m_ints;
		/* 0x020 */ hkArray<float> m_times;
	};
	static_assert(sizeof(hkxSparselyAnimatedInt) == 0x30);
	static_assert(offsetof(hkxSparselyAnimatedInt, m_ints) == 0x10);
	static_assert(offsetof(hkxSparselyAnimatedInt, m_times) == 0x20);

	class hkxSparselyAnimatedEnum : public hkxSparselyAnimatedInt
	{
	public:
		/* 0x030 */ hkxEnum* m_enum;
	};
	static_assert(sizeof(hkxSparselyAnimatedEnum) == 0x38);
	static_assert(offsetof(hkxSparselyAnimatedEnum, m_enum) == 0x30);

	class hkxSparselyAnimatedString : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkStringPtr> m_strings;
		/* 0x020 */ hkArray<float> m_times;
	};
	static_assert(sizeof(hkxSparselyAnimatedString) == 0x30);
	static_assert(offsetof(hkxSparselyAnimatedString, m_strings) == 0x10);
	static_assert(offsetof(hkxSparselyAnimatedString, m_times) == 0x20);

	class hkxSpline : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkxSplineControlPoint> m_controlPoints;
		/* 0x020 */ hkBool m_isClosed;
		/* 0x021 */ std::byte pad021[0x7];
	};
	static_assert(sizeof(hkxSpline) == 0x28);
	static_assert(offsetof(hkxSpline, m_controlPoints) == 0x10);
	static_assert(offsetof(hkxSpline, m_isClosed) == 0x20);

	class hkxSplineControlPoint
	{
	public:
		/* 0x000 */ hkVector4 m_position;
		/* 0x010 */ hkVector4 m_tangentIn;
		/* 0x020 */ hkVector4 m_tangentOut;
		/* 0x030 */ std::uint8_t m_inType;
		/* 0x031 */ std::uint8_t m_outType;
		/* 0x032 */ std::byte pad032[0xE];
	};
	static_assert(sizeof(hkxSplineControlPoint) == 0x40);
	static_assert(offsetof(hkxSplineControlPoint, m_position) == 0x0);
	static_assert(offsetof(hkxSplineControlPoint, m_tangentIn) == 0x10);
	static_assert(offsetof(hkxSplineControlPoint, m_tangentOut) == 0x20);
	static_assert(offsetof(hkxSplineControlPoint, m_inType) == 0x30);
	static_assert(offsetof(hkxSplineControlPoint, m_outType) == 0x31);

	class hkxTextureFile : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkStringPtr m_filename;
		/* 0x018 */ hkStringPtr m_name;
		/* 0x020 */ hkStringPtr m_originalFilename;
	};
	static_assert(sizeof(hkxTextureFile) == 0x28);
	static_assert(offsetof(hkxTextureFile, m_filename) == 0x10);
	static_assert(offsetof(hkxTextureFile, m_name) == 0x18);
	static_assert(offsetof(hkxTextureFile, m_originalFilename) == 0x20);

	class hkxTextureInplace : public hkReferencedObject
	{
	public:
		/* 0x010 */ char m_fileType[4];
		/* 0x014 */ std::byte pad014[0x4];
		/* 0x018 */ hkArray<std::uint8_t> m_data;
		/* 0x028 */ hkStringPtr m_name;
		/* 0x030 */ hkStringPtr m_originalFilename;
	};
	static_assert(sizeof(hkxTextureInplace) == 0x38);
	static_assert(offsetof(hkxTextureInplace, m_fileType) == 0x10);
	static_assert(offsetof(hkxTextureInplace, m_data) == 0x18);
	static_assert(offsetof(hkxTextureInplace, m_name) == 0x28);
	static_assert(offsetof(hkxTextureInplace, m_originalFilename) == 0x30);

	class hkxTriangleSelectionChannel : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<std::int32_t> m_selectedTriangles;
	};
	static_assert(sizeof(hkxTriangleSelectionChannel) == 0x20);
	static_assert(offsetof(hkxTriangleSelectionChannel, m_selectedTriangles) == 0x10);

	class hkxVertexBufferVertexData
	{
	public:
		/* 0x000 */ hkArray<std::uint32_t> m_vectorData;
		/* 0x010 */ hkArray<std::uint32_t> m_floatData;
		/* 0x020 */ hkArray<std::uint32_t> m_uint32Data;
		/* 0x030 */ hkArray<std::uint16_t> m_uint16Data;
		/* 0x040 */ hkArray<std::uint8_t> m_uint8Data;
		/* 0x050 */ std::uint32_t m_numVerts;
		/* 0x054 */ std::uint32_t m_vectorStride;
		/* 0x058 */ std::uint32_t m_floatStride;
		/* 0x05C */ std::uint32_t m_uint32Stride;
		/* 0x060 */ std::uint32_t m_uint16Stride;
		/* 0x064 */ std::uint32_t m_uint8Stride;
	};
	static_assert(sizeof(hkxVertexBufferVertexData) == 0x68);
	static_assert(offsetof(hkxVertexBufferVertexData, m_vectorData) == 0x0);
	static_assert(offsetof(hkxVertexBufferVertexData, m_floatData) == 0x10);
	static_assert(offsetof(hkxVertexBufferVertexData, m_uint32Data) == 0x20);
	static_assert(offsetof(hkxVertexBufferVertexData, m_uint16Data) == 0x30);
	static_assert(offsetof(hkxVertexBufferVertexData, m_uint8Data) == 0x40);
	static_assert(offsetof(hkxVertexBufferVertexData, m_numVerts) == 0x50);
	static_assert(offsetof(hkxVertexBufferVertexData, m_vectorStride) == 0x54);
	static_assert(offsetof(hkxVertexBufferVertexData, m_floatStride) == 0x58);
	static_assert(offsetof(hkxVertexBufferVertexData, m_uint32Stride) == 0x5C);
	static_assert(offsetof(hkxVertexBufferVertexData, m_uint16Stride) == 0x60);
	static_assert(offsetof(hkxVertexBufferVertexData, m_uint8Stride) == 0x64);

	class hkxVertexDescription
	{
	public:
		/* 0x000 */ hkArray<hkxVertexDescriptionElementDecl> m_decls;
	};
	static_assert(sizeof(hkxVertexDescription) == 0x10);
	static_assert(offsetof(hkxVertexDescription, m_decls) == 0x0);

	class hkxVertexBuffer : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkxVertexBufferVertexData m_data;
		/* 0x078 */ hkxVertexDescription m_desc;
	};
	static_assert(sizeof(hkxVertexBuffer) == 0x88);
	static_assert(offsetof(hkxVertexBuffer, m_data) == 0x10);
	static_assert(offsetof(hkxVertexBuffer, m_desc) == 0x78);

	class hkxVertexAnimation : public hkReferencedObject
	{
	public:
		/* 0x010 */ float m_time;
		/* 0x014 */ std::byte pad014[0x4];
		/* 0x018 */ hkxVertexBuffer m_vertData;
		/* 0x0A0 */ hkArray<std::int32_t> m_vertexIndexMap;
		/* 0x0B0 */ hkArray<hkxVertexAnimationUsageMap> m_componentMap;
	};
	static_assert(sizeof(hkxVertexAnimation) == 0xC0);
	static_assert(offsetof(hkxVertexAnimation, m_time) == 0x10);
	static_assert(offsetof(hkxVertexAnimation, m_vertData) == 0x18);
	static_assert(offsetof(hkxVertexAnimation, m_vertexIndexMap) == 0xA0);
	static_assert(offsetof(hkxVertexAnimation, m_componentMap) == 0xB0);

	class hkxVertexAnimationUsageMap
	{
	public:
		/* 0x000 */ std::uint16_t m_use;
		/* 0x002 */ std::uint8_t m_useIndexOrig;
		/* 0x003 */ std::uint8_t m_useIndexLocal;
	};
	static_assert(sizeof(hkxVertexAnimationUsageMap) == 0x4);
	static_assert(offsetof(hkxVertexAnimationUsageMap, m_use) == 0x0);
	static_assert(offsetof(hkxVertexAnimationUsageMap, m_useIndexOrig) == 0x2);
	static_assert(offsetof(hkxVertexAnimationUsageMap, m_useIndexLocal) == 0x3);

	class hkxVertexDescriptionElementDecl
	{
	public:
		/* 0x000 */ std::uint32_t m_byteOffset;
		/* 0x004 */ std::uint16_t m_type;
		/* 0x006 */ std::uint16_t m_usage;
		/* 0x008 */ std::uint32_t m_byteStride;
		/* 0x00C */ std::uint8_t m_numElements;
		/* 0x00D */ std::byte pad00D[0x3];
		/* 0x010 */ hkStringPtr m_channelID;
	};
	static_assert(sizeof(hkxVertexDescriptionElementDecl) == 0x18);
	static_assert(offsetof(hkxVertexDescriptionElementDecl, m_byteOffset) == 0x0);
	static_assert(offsetof(hkxVertexDescriptionElementDecl, m_type) == 0x4);
	static_assert(offsetof(hkxVertexDescriptionElementDecl, m_usage) == 0x6);
	static_assert(offsetof(hkxVertexDescriptionElementDecl, m_byteStride) == 0x8);
	static_assert(offsetof(hkxVertexDescriptionElementDecl, m_numElements) == 0xC);
	static_assert(offsetof(hkxVertexDescriptionElementDecl, m_channelID) == 0x10);

	class hkxVertexFloatDataChannel : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<float> m_perVertexFloats;
		/* 0x020 */ std::uint8_t m_dimensions;
		/* 0x021 */ std::byte pad021[0x7];
	};
	static_assert(sizeof(hkxVertexFloatDataChannel) == 0x28);
	static_assert(offsetof(hkxVertexFloatDataChannel, m_perVertexFloats) == 0x10);
	static_assert(offsetof(hkxVertexFloatDataChannel, m_dimensions) == 0x20);

	class hkxVertexIntDataChannel : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<std::int32_t> m_perVertexInts;
	};
	static_assert(sizeof(hkxVertexIntDataChannel) == 0x20);
	static_assert(offsetof(hkxVertexIntDataChannel, m_perVertexInts) == 0x10);

	class hkxVertexSelectionChannel : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<std::int32_t> m_selectedVertices;
	};
	static_assert(sizeof(hkxVertexSelectionChannel) == 0x20);
	static_assert(offsetof(hkxVertexSelectionChannel, m_selectedVertices) == 0x10);

	class hkxVertexVectorDataChannel : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<float> m_perVertexVectors;
	};
	static_assert(sizeof(hkxVertexVectorDataChannel) == 0x20);
	static_assert(offsetof(hkxVertexVectorDataChannel, m_perVertexVectors) == 0x10);

}
