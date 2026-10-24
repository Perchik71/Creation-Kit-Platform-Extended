// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseObject.h>
#include <EditorAPI/H/hkBaseTypes.h>

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

	class hkAabb
	{
	public:
		/* 0x000 */ hkVector4 m_min;
		/* 0x010 */ hkVector4 m_max;
	};
	static_assert(sizeof(hkAabb) == 0x20);
	static_assert(offsetof(hkAabb, m_min) == 0x0);
	static_assert(offsetof(hkAabb, m_max) == 0x10);

	class hkAabb16
	{
	public:
		/* 0x000 */ std::uint16_t m_min[3];
		/* 0x006 */ std::uint16_t m_key;
		/* 0x008 */ std::uint16_t m_max[3];
		/* 0x00E */ std::uint16_t m_key1;
	};
	static_assert(sizeof(hkAabb16) == 0x10);
	static_assert(offsetof(hkAabb16, m_min) == 0x0);
	static_assert(offsetof(hkAabb16, m_key) == 0x6);
	static_assert(offsetof(hkAabb16, m_max) == 0x8);
	static_assert(offsetof(hkAabb16, m_key1) == 0xE);

	class hkAabbHalf
	{
	public:
		/* 0x000 */ std::uint16_t m_data[8];
	};
	static_assert(sizeof(hkAabbHalf) == 0x10);
	static_assert(offsetof(hkAabbHalf, m_data) == 0x0);

	class hkAabbUint32
	{
	public:
		/* 0x000 */ std::uint32_t m_min[3];
		/* 0x00C */ std::uint8_t m_expansionMin[3];
		/* 0x00F */ std::uint8_t m_expansionShift;
		/* 0x010 */ std::uint32_t m_max[3];
		/* 0x01C */ std::uint8_t m_expansionMax[3];
		/* 0x01F */ std::uint8_t m_shapeKeyByte;
	};
	static_assert(sizeof(hkAabbUint32) == 0x20);
	static_assert(offsetof(hkAabbUint32, m_min) == 0x0);
	static_assert(offsetof(hkAabbUint32, m_expansionMin) == 0xC);
	static_assert(offsetof(hkAabbUint32, m_expansionShift) == 0xF);
	static_assert(offsetof(hkAabbUint32, m_max) == 0x10);
	static_assert(offsetof(hkAabbUint32, m_expansionMax) == 0x1C);
	static_assert(offsetof(hkAabbUint32, m_shapeKeyByte) == 0x1F);

	class hkAlignSceneToNodeOptions : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkBool m_invert;
		/* 0x011 */ hkBool m_transformPositionX;
		/* 0x012 */ hkBool m_transformPositionY;
		/* 0x013 */ hkBool m_transformPositionZ;
		/* 0x014 */ hkBool m_transformRotation;
		/* 0x015 */ hkBool m_transformScale;
		/* 0x016 */ hkBool m_transformSkew;
		/* 0x017 */ std::byte pad017[0x1];
		/* 0x018 */ std::int32_t m_keyframe;
		/* 0x01C */ std::byte pad01C[0x4];
		/* 0x020 */ hkStringPtr m_nodeName;
	};
	static_assert(sizeof(hkAlignSceneToNodeOptions) == 0x28);
	static_assert(offsetof(hkAlignSceneToNodeOptions, m_invert) == 0x10);
	static_assert(offsetof(hkAlignSceneToNodeOptions, m_transformPositionX) == 0x11);
	static_assert(offsetof(hkAlignSceneToNodeOptions, m_transformPositionY) == 0x12);
	static_assert(offsetof(hkAlignSceneToNodeOptions, m_transformPositionZ) == 0x13);
	static_assert(offsetof(hkAlignSceneToNodeOptions, m_transformRotation) == 0x14);
	static_assert(offsetof(hkAlignSceneToNodeOptions, m_transformScale) == 0x15);
	static_assert(offsetof(hkAlignSceneToNodeOptions, m_transformSkew) == 0x16);
	static_assert(offsetof(hkAlignSceneToNodeOptions, m_keyframe) == 0x18);
	static_assert(offsetof(hkAlignSceneToNodeOptions, m_nodeName) == 0x20);

	class hkStreamWriter : public hkReferencedObject
	{
	public:
		virtual hkBool IsOk();
		virtual std::int32_t Write(const void* buffer, std::int32_t numBytes);
		virtual void hkStreamWriter_Unk06();
		virtual void hkStreamWriter_Unk07();
		virtual void hkStreamWriter_Unk08();
		virtual void hkStreamWriter_Unk09();
		virtual void hkStreamWriter_Unk0A();
	};
	static_assert(sizeof(hkStreamWriter) == 0x10);

	class hkArrayStreamWriter : public hkStreamWriter
	{
	public:
		virtual void hkArrayStreamWriter_Unk0B();
		/* 0x010 */ std::byte pad010[0x18];
	};
	static_assert(sizeof(hkArrayStreamWriter) == 0x28);

	class hkArrayTypeAttribute
	{
	public:
		/* 0x000 */ std::int8_t m_type;
	};
	static_assert(sizeof(hkArrayTypeAttribute) == 0x1);
	static_assert(offsetof(hkArrayTypeAttribute, m_type) == 0x0);

	class hkBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator
	{
	public:
		/* 0x000 */ hkArray<std::uint32_t> m_words;
		/* 0x010 */ std::int32_t m_numBits;
		/* 0x014 */ std::byte pad014[0x4];
	};
	static_assert(sizeof(hkBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator) == 0x18);
	static_assert(offsetof(hkBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator, m_words) == 0x0);
	static_assert(offsetof(hkBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator, m_numBits) == 0x10);

	class hkBitFieldBasehkBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator
	{
	public:
		/* 0x000 */ hkBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator m_storage;
	};
	static_assert(sizeof(hkBitFieldBasehkBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator) == 0x18);
	static_assert(offsetof(hkBitFieldBasehkBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator, m_storage) == 0x0);

	class hkBitField : public hkBitFieldBasehkBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator
	{
	public:
	};
	static_assert(sizeof(hkBitField) == 0x18);

	class hkOffsetBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator
	{
	public:
		/* 0x000 */ hkArray<std::uint32_t> m_words;
		/* 0x010 */ std::int32_t m_offset;
		/* 0x014 */ std::byte pad014[0x4];
	};
	static_assert(sizeof(hkOffsetBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator) == 0x18);
	static_assert(offsetof(hkOffsetBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator, m_words) == 0x0);
	static_assert(offsetof(hkOffsetBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator, m_offset) == 0x10);

	class hkBitFieldBasehkOffsetBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator
	{
	public:
		/* 0x000 */ hkOffsetBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator m_storage;
	};
	static_assert(sizeof(hkBitFieldBasehkOffsetBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator) == 0x18);
	static_assert(offsetof(hkBitFieldBasehkOffsetBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator, m_storage) == 0x0);

	class hkBufferedStreamWriter : public hkStreamWriter
	{
	public:
		/* 0x010 */ std::byte pad010[0x20];
	};
	static_assert(sizeof(hkBufferedStreamWriter) == 0x30);

	class hkClass
	{
	public:
		/* 0x000 */ const char* m_name;
		/* 0x008 */ hkClass* m_parent;
		/* 0x010 */ std::int32_t m_objectSize;
		/* 0x014 */ std::int32_t m_numImplementedInterfaces;
		/* 0x018 */ hkSimpleArray<hkClassEnum> m_declaredEnums;
		/* 0x028 */ hkSimpleArray<hkClassMember> m_declaredMembers;
		/* 0x038 */ void* m_defaults;
		/* 0x040 */ hkCustomAttributes* m_attributes;
		/* 0x048 */ std::uint32_t m_flags;
		/* 0x04C */ std::int32_t m_describedVersion;
	};
	static_assert(sizeof(hkClass) == 0x50);
	static_assert(offsetof(hkClass, m_name) == 0x0);
	static_assert(offsetof(hkClass, m_parent) == 0x8);
	static_assert(offsetof(hkClass, m_objectSize) == 0x10);
	static_assert(offsetof(hkClass, m_numImplementedInterfaces) == 0x14);
	static_assert(offsetof(hkClass, m_declaredEnums) == 0x18);
	static_assert(offsetof(hkClass, m_declaredMembers) == 0x28);
	static_assert(offsetof(hkClass, m_defaults) == 0x38);
	static_assert(offsetof(hkClass, m_attributes) == 0x40);
	static_assert(offsetof(hkClass, m_flags) == 0x48);
	static_assert(offsetof(hkClass, m_describedVersion) == 0x4C);

	class hkClassEnum
	{
	public:
		/* 0x000 */ const char* m_name;
		/* 0x008 */ hkSimpleArray<hkClassEnumItem> m_items;
		/* 0x018 */ hkCustomAttributes* m_attributes;
		/* 0x020 */ std::uint32_t m_flags;
		/* 0x024 */ std::byte pad024[0x4];
	};
	static_assert(sizeof(hkClassEnum) == 0x28);
	static_assert(offsetof(hkClassEnum, m_name) == 0x0);
	static_assert(offsetof(hkClassEnum, m_items) == 0x8);
	static_assert(offsetof(hkClassEnum, m_attributes) == 0x18);
	static_assert(offsetof(hkClassEnum, m_flags) == 0x20);

	class hkClassEnumItem
	{
	public:
		/* 0x000 */ std::int32_t m_value;
		/* 0x004 */ std::byte pad004[0x4];
		/* 0x008 */ const char* m_name;
	};
	static_assert(sizeof(hkClassEnumItem) == 0x10);
	static_assert(offsetof(hkClassEnumItem, m_value) == 0x0);
	static_assert(offsetof(hkClassEnumItem, m_name) == 0x8);

	class hkClassMember
	{
	public:
		/* 0x000 */ const char* m_name;
		/* 0x008 */ hkClass* m_class;
		/* 0x010 */ hkClassEnum* m_enum;
		/* 0x018 */ std::uint8_t m_type;
		/* 0x019 */ std::uint8_t m_subtype;
		/* 0x01A */ std::int16_t m_cArraySize;
		/* 0x01C */ std::uint16_t m_flags;
		/* 0x01E */ std::uint16_t m_offset;
		/* 0x020 */ hkCustomAttributes* m_attributes;
	};
	static_assert(sizeof(hkClassMember) == 0x28);
	static_assert(offsetof(hkClassMember, m_name) == 0x0);
	static_assert(offsetof(hkClassMember, m_class) == 0x8);
	static_assert(offsetof(hkClassMember, m_enum) == 0x10);
	static_assert(offsetof(hkClassMember, m_type) == 0x18);
	static_assert(offsetof(hkClassMember, m_subtype) == 0x19);
	static_assert(offsetof(hkClassMember, m_cArraySize) == 0x1A);
	static_assert(offsetof(hkClassMember, m_flags) == 0x1C);
	static_assert(offsetof(hkClassMember, m_offset) == 0x1E);
	static_assert(offsetof(hkClassMember, m_attributes) == 0x20);

	class hkColor
	{
	public:
	};
	static_assert(sizeof(hkColor) == 0x1);

	class hkCompressedMassProperties
	{
	public:
		/* 0x000 */ std::int16_t m_centerOfMass[4];
		/* 0x008 */ std::int16_t m_inertia[4];
		/* 0x010 */ std::int16_t m_majorAxisSpace[4];
		/* 0x018 */ float m_mass;
		/* 0x01C */ float m_volume;
	};
	static_assert(sizeof(hkCompressedMassProperties) == 0x20);
	static_assert(offsetof(hkCompressedMassProperties, m_centerOfMass) == 0x0);
	static_assert(offsetof(hkCompressedMassProperties, m_inertia) == 0x8);
	static_assert(offsetof(hkCompressedMassProperties, m_majorAxisSpace) == 0x10);
	static_assert(offsetof(hkCompressedMassProperties, m_mass) == 0x18);
	static_assert(offsetof(hkCompressedMassProperties, m_volume) == 0x1C);

	class hkContactPoint
	{
	public:
		/* 0x000 */ hkVector4 m_position;
		/* 0x010 */ hkVector4 m_separatingNormal;
	};
	static_assert(sizeof(hkContactPoint) == 0x20);
	static_assert(offsetof(hkContactPoint, m_position) == 0x0);
	static_assert(offsetof(hkContactPoint, m_separatingNormal) == 0x10);

	class hkUFloat8
	{
	public:
		/* 0x000 */ std::uint8_t m_value;
	};
	static_assert(sizeof(hkUFloat8) == 0x1);
	static_assert(offsetof(hkUFloat8, m_value) == 0x0);

	class hkContactPointMaterial
	{
	public:
		/* 0x000 */ std::size_t m_userData;
		/* 0x008 */ hkUFloat8 m_friction;
		/* 0x009 */ std::uint8_t m_restitution;
		/* 0x00A */ hkUFloat8 m_maxImpulse;
		/* 0x00B */ std::uint8_t m_flags;
		/* 0x00C */ std::byte pad00C[0x4];
	};
	static_assert(sizeof(hkContactPointMaterial) == 0x10);
	static_assert(offsetof(hkContactPointMaterial, m_userData) == 0x0);
	static_assert(offsetof(hkContactPointMaterial, m_friction) == 0x8);
	static_assert(offsetof(hkContactPointMaterial, m_restitution) == 0x9);
	static_assert(offsetof(hkContactPointMaterial, m_maxImpulse) == 0xA);
	static_assert(offsetof(hkContactPointMaterial, m_flags) == 0xB);

	class hkCustomAttributes
	{
	public:
		/* 0x000 */ hkSimpleArray<hkCustomAttributesAttribute> m_attributes;
	};
	static_assert(sizeof(hkCustomAttributes) == 0x10);
	static_assert(offsetof(hkCustomAttributes, m_attributes) == 0x0);

	class hkCustomAttributesAttribute
	{
	public:
		/* 0x000 */ const char* m_name;
		/* 0x008 */ std::byte pad008[0x10];
	};
	static_assert(sizeof(hkCustomAttributesAttribute) == 0x18);
	static_assert(offsetof(hkCustomAttributesAttribute, m_name) == 0x0);

	class hkDataObjectTypeAttribute
	{
	public:
		/* 0x000 */ const char* m_typeName;
	};
	static_assert(sizeof(hkDataObjectTypeAttribute) == 0x8);
	static_assert(offsetof(hkDataObjectTypeAttribute, m_typeName) == 0x0);

	class hkMeshBody : public hkReferencedObject
	{
	public:
		virtual void hkMeshBody_Unk04();
		virtual void hkMeshBody_Unk05();
		virtual void hkMeshBody_Unk06();
		virtual void hkMeshBody_Unk07();
		virtual void hkMeshBody_Unk08();
		virtual void hkMeshBody_Unk09();
		virtual void hkMeshBody_Unk0A();
		virtual void hkMeshBody_Unk0B();
		virtual void hkMeshBody_Unk0C();
		virtual void hkMeshBody_Unk0D();
		virtual void hkMeshBody_Unk0E();
		virtual void hkMeshBody_Unk0F();
		virtual void hkMeshBody_Unk10();
		virtual void hkMeshBody_Unk11();
		virtual void hkMeshBody_Unk12();
		virtual void hkMeshBody_Unk13();
		virtual void hkMeshBody_Unk14();
	};
	static_assert(sizeof(hkMeshBody) == 0x10);

	class hkDefaultCompoundMeshBody : public hkMeshBody
	{
	public:
		virtual void hkDefaultCompoundMeshBody_Unk15();
		virtual void hkDefaultCompoundMeshBody_Unk16();
		/* 0x010 */ std::byte pad010[0x70];
	};
	static_assert(sizeof(hkDefaultCompoundMeshBody) == 0x80);

	class hkMeshShape : public hkReferencedObject
	{
	public:
		virtual void hkMeshShape_Unk04();
		virtual void hkMeshShape_Unk05();
		virtual void hkMeshShape_Unk06();
		virtual void hkMeshShape_Unk07();
		virtual void hkMeshShape_Unk08();
	};
	static_assert(sizeof(hkMeshShape) == 0x10);

	class hkDefaultCompoundMeshShape : public hkMeshShape
	{
	public:
		/* 0x010 */ std::byte pad010[0x30];
	};
	static_assert(sizeof(hkDefaultCompoundMeshShape) == 0x40);

	class hkMemoryTracker
	{
	public:
		virtual void hkMemoryTracker_Unk00();
		virtual void hkMemoryTracker_Unk01();
		virtual void hkMemoryTracker_Unk02();
		virtual void hkMemoryTracker_Unk03();
		virtual void hkMemoryTracker_Unk04();
		virtual void hkMemoryTracker_Unk05();
		virtual void hkMemoryTracker_Unk06();
		virtual void hkMemoryTracker_Unk07();
		virtual void hkMemoryTracker_Unk08();
		virtual void hkMemoryTracker_Unk09();
		virtual void hkMemoryTracker_Unk0A();
		virtual ~hkMemoryTracker();
	};
	static_assert(sizeof(hkMemoryTracker) == 0x8);

	class hkDefaultMemoryTracker : public hkMemoryTracker
	{
	public:
		/* 0x008 */ std::byte pad008[0xF0];
	};
	static_assert(sizeof(hkDefaultMemoryTracker) == 0xF8);

	class hkMeshMaterialRegistry : public hkReferencedObject
	{
	public:
		virtual void hkMeshMaterialRegistry_Unk04();
		virtual void hkMeshMaterialRegistry_Unk05();
		virtual void hkMeshMaterialRegistry_Unk06();
		virtual void hkMeshMaterialRegistry_Unk07();
		virtual void hkMeshMaterialRegistry_Unk08();
		virtual void hkMeshMaterialRegistry_Unk09();
		virtual void hkMeshMaterialRegistry_Unk0A();
	};
	static_assert(sizeof(hkMeshMaterialRegistry) == 0x10);

	class hkDefaultMeshMaterialRegistry : public hkMeshMaterialRegistry
	{
	public:
		/* 0x010 */ std::byte pad010[0x10];
	};
	static_assert(sizeof(hkDefaultMeshMaterialRegistry) == 0x20);

	class hkDescriptionAttribute
	{
	public:
		/* 0x000 */ const char* m_string;
	};
	static_assert(sizeof(hkDescriptionAttribute) == 0x8);
	static_assert(offsetof(hkDescriptionAttribute, m_string) == 0x0);

	class hkDocumentationAttribute
	{
	public:
		/* 0x000 */ const char* m_docsSectionTag;
	};
	static_assert(sizeof(hkDocumentationAttribute) == 0x8);
	static_assert(offsetof(hkDocumentationAttribute, m_docsSectionTag) == 0x0);

	class hkFloat16
	{
	public:
		/* 0x000 */ std::uint16_t m_value;
	};
	static_assert(sizeof(hkFloat16) == 0x2);
	static_assert(offsetof(hkFloat16, m_value) == 0x0);

	class hkFloat16Transform
	{
	public:
		/* 0x000 */ hkFloat16 m_elements[12];
	};
	static_assert(sizeof(hkFloat16Transform) == 0x18);
	static_assert(offsetof(hkFloat16Transform, m_elements) == 0x0);

	class hkFourTransposedPointsf
	{
	public:
		/* 0x000 */ hkVector4 m_vertices[3];
	};
	static_assert(sizeof(hkFourTransposedPointsf) == 0x30);
	static_assert(offsetof(hkFourTransposedPointsf, m_vertices) == 0x0);

	class hkFreeListArrayhknpMaterialhknpMaterialId8hknpMaterialFreeListArrayOperations
	{
	public:
		/* 0x000 */ std::byte pad000[0x10];
		/* 0x010 */ std::int32_t m_firstFree;
		/* 0x014 */ std::byte pad014[0x4];
	};
	static_assert(sizeof(hkFreeListArrayhknpMaterialhknpMaterialId8hknpMaterialFreeListArrayOperations) == 0x18);
	static_assert(offsetof(hkFreeListArrayhknpMaterialhknpMaterialId8hknpMaterialFreeListArrayOperations, m_firstFree) == 0x10);

	class hkFreeListArrayhknpMotionPropertieshknpMotionPropertiesId8hknpMotionPropertiesFreeListArrayOperations
	{
	public:
		/* 0x000 */ std::byte pad000[0x10];
		/* 0x010 */ std::int32_t m_firstFree;
		/* 0x014 */ std::byte pad014[0x4];
	};
	static_assert(sizeof(hkFreeListArrayhknpMotionPropertieshknpMotionPropertiesId8hknpMotionPropertiesFreeListArrayOperations) == 0x18);
	static_assert(offsetof(hkFreeListArrayhknpMotionPropertieshknpMotionPropertiesId8hknpMotionPropertiesFreeListArrayOperations, m_firstFree) == 0x10);

	class hkFreeListArrayhknpShapeInstancehkHandleshort32767hknpShapeInstanceIdDiscriminant8hknpShapeInstance
	{
	public:
		/* 0x000 */ std::byte pad000[0x10];
		/* 0x010 */ std::int32_t m_firstFree;
		/* 0x014 */ std::byte pad014[0x4];
	};
	static_assert(sizeof(hkFreeListArrayhknpShapeInstancehkHandleshort32767hknpShapeInstanceIdDiscriminant8hknpShapeInstance) == 0x18);
	static_assert(offsetof(hkFreeListArrayhknpShapeInstancehkHandleshort32767hknpShapeInstanceIdDiscriminant8hknpShapeInstance, m_firstFree) == 0x10);

	class hkGeometry : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkVector4> m_vertices;
		/* 0x020 */ hkArray<hkGeometryTriangle> m_triangles;
	};
	static_assert(sizeof(hkGeometry) == 0x30);
	static_assert(offsetof(hkGeometry, m_vertices) == 0x10);
	static_assert(offsetof(hkGeometry, m_triangles) == 0x20);

	class hkGeometryTriangle
	{
	public:
		/* 0x000 */ std::int32_t m_a;
		/* 0x004 */ std::int32_t m_b;
		/* 0x008 */ std::int32_t m_c;
		/* 0x00C */ std::int32_t m_material;
	};
	static_assert(sizeof(hkGeometryTriangle) == 0x10);
	static_assert(offsetof(hkGeometryTriangle, m_a) == 0x0);
	static_assert(offsetof(hkGeometryTriangle, m_b) == 0x4);
	static_assert(offsetof(hkGeometryTriangle, m_c) == 0x8);
	static_assert(offsetof(hkGeometryTriangle, m_material) == 0xC);

	class hkGizmoAttribute
	{
	public:
		/* 0x000 */ hkBool m_visible;
		/* 0x001 */ std::byte pad001[0x7];
		/* 0x008 */ const char* m_label;
		/* 0x010 */ std::int8_t m_type;
		/* 0x011 */ std::byte pad011[0x7];
	};
	static_assert(sizeof(hkGizmoAttribute) == 0x18);
	static_assert(offsetof(hkGizmoAttribute, m_visible) == 0x0);
	static_assert(offsetof(hkGizmoAttribute, m_label) == 0x8);
	static_assert(offsetof(hkGizmoAttribute, m_type) == 0x10);

	class hkIndexedTransformSet : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkMatrix4> m_matrices;
		/* 0x020 */ hkArray<hkMatrix4> m_inverseMatrices;
		/* 0x030 */ hkArray<std::int16_t> m_matricesOrder;
		/* 0x040 */ hkArray<hkStringPtr> m_matricesNames;
		/* 0x050 */ hkArray<hkMeshBoneIndexMapping> m_indexMappings;
		/* 0x060 */ hkBool m_allMatricesAreAffine;
		/* 0x061 */ std::byte pad061[0x7];
	};
	static_assert(sizeof(hkIndexedTransformSet) == 0x68);
	static_assert(offsetof(hkIndexedTransformSet, m_matrices) == 0x10);
	static_assert(offsetof(hkIndexedTransformSet, m_inverseMatrices) == 0x20);
	static_assert(offsetof(hkIndexedTransformSet, m_matricesOrder) == 0x30);
	static_assert(offsetof(hkIndexedTransformSet, m_matricesNames) == 0x40);
	static_assert(offsetof(hkIndexedTransformSet, m_indexMappings) == 0x50);
	static_assert(offsetof(hkIndexedTransformSet, m_allMatricesAreAffine) == 0x60);

	class hkIntRealPair
	{
	public:
		/* 0x000 */ std::int32_t m_key;
		/* 0x004 */ float m_value;
	};
	static_assert(sizeof(hkIntRealPair) == 0x8);
	static_assert(offsetof(hkIntRealPair, m_key) == 0x0);
	static_assert(offsetof(hkIntRealPair, m_value) == 0x4);

	class hkLinkAttribute
	{
	public:
		/* 0x000 */ std::int8_t m_type;
	};
	static_assert(sizeof(hkLinkAttribute) == 0x1);
	static_assert(offsetof(hkLinkAttribute, m_type) == 0x0);

	class hkLocalFrame : public hkReferencedObject
	{
	public:
		virtual void hkLocalFrame_Unk04();
		virtual void hkLocalFrame_Unk05();
		virtual void hkLocalFrame_Unk06();
		virtual void hkLocalFrame_Unk07();
		virtual void hkLocalFrame_Unk08();
		virtual void hkLocalFrame_Unk09();
		virtual void hkLocalFrame_Unk0A();
		virtual void hkLocalFrame_Unk0B();
		virtual void hkLocalFrame_Unk0C();
		virtual void hkLocalFrame_Unk0D();
		virtual void hkLocalFrame_Unk0E();
		virtual void hkLocalFrame_Unk0F();
	};
	static_assert(sizeof(hkLocalFrame) == 0x10);

	class hkLocalFrameGroup : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkStringPtr m_name;
	};
	static_assert(sizeof(hkLocalFrameGroup) == 0x18);
	static_assert(offsetof(hkLocalFrameGroup, m_name) == 0x10);

	class hkMassProperties
	{
	public:
		/* 0x000 */ float m_volume;
		/* 0x004 */ float m_mass;
		/* 0x008 */ std::byte pad008[0x8];
		/* 0x010 */ hkVector4 m_centerOfMass;
		/* 0x020 */ hkMatrix3 m_inertiaTensor;
	};
	static_assert(sizeof(hkMassProperties) == 0x50);
	static_assert(offsetof(hkMassProperties, m_volume) == 0x0);
	static_assert(offsetof(hkMassProperties, m_mass) == 0x4);
	static_assert(offsetof(hkMassProperties, m_centerOfMass) == 0x10);
	static_assert(offsetof(hkMassProperties, m_inertiaTensor) == 0x20);

	class hkMemoryMeshBody : public hkMeshBody
	{
	public:
		/* 0x010 */ hkMatrix4 m_transform;
		/* 0x050 */ hkIndexedTransformSet* m_transformSet;
		/* 0x058 */ hkMeshShape* m_shape;
		/* 0x060 */ hkArray<hkMeshVertexBuffer*> m_vertexBuffers;
		/* 0x070 */ hkStringPtr m_name;
		/* 0x078 */ std::byte pad078[0x8];
	};
	static_assert(sizeof(hkMemoryMeshBody) == 0x80);
	static_assert(offsetof(hkMemoryMeshBody, m_transform) == 0x10);
	static_assert(offsetof(hkMemoryMeshBody, m_transformSet) == 0x50);
	static_assert(offsetof(hkMemoryMeshBody, m_shape) == 0x58);
	static_assert(offsetof(hkMemoryMeshBody, m_vertexBuffers) == 0x60);
	static_assert(offsetof(hkMemoryMeshBody, m_name) == 0x70);

	class hkMeshMaterial : public hkReferencedObject
	{
	public:
		virtual void hkMeshMaterial_Unk04();
		virtual void hkMeshMaterial_Unk05();
		virtual void hkMeshMaterial_Unk06();
		virtual void hkMeshMaterial_Unk07();
		virtual void hkMeshMaterial_Unk08();
		virtual void hkMeshMaterial_Unk09();
		virtual void hkMeshMaterial_Unk0A();
		virtual void hkMeshMaterial_Unk0B();
		virtual void hkMeshMaterial_Unk0C();
		virtual void hkMeshMaterial_Unk0D();
		virtual void hkMeshMaterial_Unk0E();
		virtual void hkMeshMaterial_Unk0F();
		virtual void hkMeshMaterial_Unk10();
		virtual void hkMeshMaterial_Unk11();
		virtual void hkMeshMaterial_Unk12();
		virtual void hkMeshMaterial_Unk13();
		virtual void hkMeshMaterial_Unk14();
		virtual void hkMeshMaterial_Unk15();
	};
	static_assert(sizeof(hkMeshMaterial) == 0x10);

	class hkMemoryMeshMaterial : public hkMeshMaterial
	{
	public:
		/* 0x010 */ hkStringPtr m_materialName;
		/* 0x018 */ hkArray<hkMeshTexture*> m_textures;
		/* 0x028 */ std::byte pad028[0x8];
		/* 0x030 */ hkVector4 m_diffuseColor;
		/* 0x040 */ hkVector4 m_ambientColor;
		/* 0x050 */ hkVector4 m_specularColor;
		/* 0x060 */ hkVector4 m_emissiveColor;
		/* 0x070 */ std::size_t m_userData;
		/* 0x078 */ float m_tesselationFactor;
		/* 0x07C */ float m_displacementAmount;
	};
	static_assert(sizeof(hkMemoryMeshMaterial) == 0x80);
	static_assert(offsetof(hkMemoryMeshMaterial, m_materialName) == 0x10);
	static_assert(offsetof(hkMemoryMeshMaterial, m_textures) == 0x18);
	static_assert(offsetof(hkMemoryMeshMaterial, m_diffuseColor) == 0x30);
	static_assert(offsetof(hkMemoryMeshMaterial, m_ambientColor) == 0x40);
	static_assert(offsetof(hkMemoryMeshMaterial, m_specularColor) == 0x50);
	static_assert(offsetof(hkMemoryMeshMaterial, m_emissiveColor) == 0x60);
	static_assert(offsetof(hkMemoryMeshMaterial, m_userData) == 0x70);
	static_assert(offsetof(hkMemoryMeshMaterial, m_tesselationFactor) == 0x78);
	static_assert(offsetof(hkMemoryMeshMaterial, m_displacementAmount) == 0x7C);

	class hkMemoryMeshShape : public hkMeshShape
	{
	public:
		/* 0x010 */ hkArray<hkMemoryMeshShapeSection> m_sections;
		/* 0x020 */ hkArray<std::uint16_t> m_indices16;
		/* 0x030 */ hkArray<std::uint32_t> m_indices32;
		/* 0x040 */ hkStringPtr m_name;
	};
	static_assert(sizeof(hkMemoryMeshShape) == 0x48);
	static_assert(offsetof(hkMemoryMeshShape, m_sections) == 0x10);
	static_assert(offsetof(hkMemoryMeshShape, m_indices16) == 0x20);
	static_assert(offsetof(hkMemoryMeshShape, m_indices32) == 0x30);
	static_assert(offsetof(hkMemoryMeshShape, m_name) == 0x40);

	class hkMeshBoneIndexMapping
	{
	public:
		/* 0x000 */ hkArray<std::int16_t> m_mapping;
	};
	static_assert(sizeof(hkMeshBoneIndexMapping) == 0x10);
	static_assert(offsetof(hkMeshBoneIndexMapping, m_mapping) == 0x0);

	class hkMemoryMeshShapeSection
	{
	public:
		/* 0x000 */ hkMeshVertexBuffer* m_vertexBuffer;
		/* 0x008 */ hkMeshMaterial* m_material;
		/* 0x010 */ hkMeshBoneIndexMapping m_boneMatrixMap;
		/* 0x020 */ std::uint8_t m_primitiveType;
		/* 0x021 */ std::byte pad021[0x3];
		/* 0x024 */ std::int32_t m_numPrimitives;
		/* 0x028 */ std::uint8_t m_indexType;
		/* 0x029 */ std::byte pad029[0x3];
		/* 0x02C */ std::int32_t m_vertexStartIndex;
		/* 0x030 */ std::int32_t m_transformIndex;
		/* 0x034 */ std::int32_t m_indexBufferOffset;
	};
	static_assert(sizeof(hkMemoryMeshShapeSection) == 0x38);
	static_assert(offsetof(hkMemoryMeshShapeSection, m_vertexBuffer) == 0x0);
	static_assert(offsetof(hkMemoryMeshShapeSection, m_material) == 0x8);
	static_assert(offsetof(hkMemoryMeshShapeSection, m_boneMatrixMap) == 0x10);
	static_assert(offsetof(hkMemoryMeshShapeSection, m_primitiveType) == 0x20);
	static_assert(offsetof(hkMemoryMeshShapeSection, m_numPrimitives) == 0x24);
	static_assert(offsetof(hkMemoryMeshShapeSection, m_indexType) == 0x28);
	static_assert(offsetof(hkMemoryMeshShapeSection, m_vertexStartIndex) == 0x2C);
	static_assert(offsetof(hkMemoryMeshShapeSection, m_transformIndex) == 0x30);
	static_assert(offsetof(hkMemoryMeshShapeSection, m_indexBufferOffset) == 0x34);

	class hkMeshSystem : public hkReferencedObject
	{
	public:
		virtual void hkMeshSystem_Unk04();
		virtual void hkMeshSystem_Unk05();
		virtual void hkMeshSystem_Unk06();
		virtual void hkMeshSystem_Unk07();
		virtual void hkMeshSystem_Unk08();
		virtual void hkMeshSystem_Unk09();
		virtual void hkMeshSystem_Unk0A();
		virtual void hkMeshSystem_Unk0B();
		virtual void hkMeshSystem_Unk0C();
		virtual void hkMeshSystem_Unk0D();
		virtual void hkMeshSystem_Unk0E();
		virtual void hkMeshSystem_Unk0F();
		virtual void hkMeshSystem_Unk10();
		virtual void hkMeshSystem_Unk11();
		virtual void hkMeshSystem_Unk12();
	};
	static_assert(sizeof(hkMeshSystem) == 0x10);

	class hkMemoryMeshSystem : public hkMeshSystem
	{
	public:
		virtual void hkMemoryMeshSystem_Unk13();
		/* 0x010 */ std::byte pad010[0x18];
	};
	static_assert(sizeof(hkMemoryMeshSystem) == 0x28);

	class hkMeshTexture : public hkReferencedObject
	{
	public:
		class Sampler : public hkReferencedObject
		{
		public:
			virtual void Sampler_Unk04();
			virtual void Sampler_Unk05();
		};
		static_assert(sizeof(Sampler) == 0x10);

		virtual void hkMeshTexture_Unk04();
		virtual void hkMeshTexture_Unk05();
		virtual void hkMeshTexture_Unk06();
		virtual void hkMeshTexture_Unk07();
		virtual void hkMeshTexture_Unk08();
		virtual void hkMeshTexture_Unk09();
		virtual void hkMeshTexture_Unk0A();
		virtual void hkMeshTexture_Unk0B();
		virtual void hkMeshTexture_Unk0C();
		virtual void hkMeshTexture_Unk0D();
		virtual void hkMeshTexture_Unk0E();
		virtual void hkMeshTexture_Unk0F();
		virtual void hkMeshTexture_Unk10();
		virtual void hkMeshTexture_Unk11();
		virtual void hkMeshTexture_Unk12();
		virtual void hkMeshTexture_Unk13();
	};
	static_assert(sizeof(hkMeshTexture) == 0x10);

	class hkMemoryMeshTexture : public hkMeshTexture
	{
	public:
		class Sampler : public hkMeshTexture::Sampler
		{
		public:
			/* 0x010 */ std::byte pad010[0x8];
		};
		static_assert(sizeof(Sampler) == 0x18);

		/* 0x010 */ hkStringPtr m_filename;
		/* 0x018 */ hkArray<std::uint8_t> m_data;
		/* 0x028 */ std::int8_t m_format;
		/* 0x029 */ hkBool m_hasMipMaps;
		/* 0x02A */ std::int8_t m_filterMode;
		/* 0x02B */ std::int8_t m_usageHint;
		/* 0x02C */ std::int32_t m_textureCoordChannel;
	};
	static_assert(sizeof(hkMemoryMeshTexture) == 0x30);
	static_assert(offsetof(hkMemoryMeshTexture, m_filename) == 0x10);
	static_assert(offsetof(hkMemoryMeshTexture, m_data) == 0x18);
	static_assert(offsetof(hkMemoryMeshTexture, m_format) == 0x28);
	static_assert(offsetof(hkMemoryMeshTexture, m_hasMipMaps) == 0x29);
	static_assert(offsetof(hkMemoryMeshTexture, m_filterMode) == 0x2A);
	static_assert(offsetof(hkMemoryMeshTexture, m_usageHint) == 0x2B);
	static_assert(offsetof(hkMemoryMeshTexture, m_textureCoordChannel) == 0x2C);

	class hkMeshVertexBuffer : public hkReferencedObject
	{
	public:
		virtual void hkMeshVertexBuffer_Unk04();
		virtual void hkMeshVertexBuffer_Unk05();
		virtual void hkMeshVertexBuffer_Unk06();
		virtual void hkMeshVertexBuffer_Unk07();
		virtual void hkMeshVertexBuffer_Unk08();
		virtual void hkMeshVertexBuffer_Unk09();
		virtual void hkMeshVertexBuffer_Unk0A();
		virtual void hkMeshVertexBuffer_Unk0B();
		virtual void hkMeshVertexBuffer_Unk0C();
		virtual void hkMeshVertexBuffer_Unk0D();
		virtual void hkMeshVertexBuffer_Unk0E();
	};
	static_assert(sizeof(hkMeshVertexBuffer) == 0x10);

	class hkVertexFormatElement
	{
	public:
		/* 0x000 */ std::uint8_t m_dataType;
		/* 0x001 */ std::uint8_t m_numValues;
		/* 0x002 */ std::uint8_t m_usage;
		/* 0x003 */ std::uint8_t m_subUsage;
		/* 0x004 */ std::uint8_t m_flags;
		/* 0x005 */ std::uint8_t m_pad[3];
	};
	static_assert(sizeof(hkVertexFormatElement) == 0x8);
	static_assert(offsetof(hkVertexFormatElement, m_dataType) == 0x0);
	static_assert(offsetof(hkVertexFormatElement, m_numValues) == 0x1);
	static_assert(offsetof(hkVertexFormatElement, m_usage) == 0x2);
	static_assert(offsetof(hkVertexFormatElement, m_subUsage) == 0x3);
	static_assert(offsetof(hkVertexFormatElement, m_flags) == 0x4);
	static_assert(offsetof(hkVertexFormatElement, m_pad) == 0x5);

	class hkVertexFormat
	{
	public:
		/* 0x000 */ hkVertexFormatElement m_elements[32];
		/* 0x100 */ std::int32_t m_numElements;
	};
	static_assert(sizeof(hkVertexFormat) == 0x104);
	static_assert(offsetof(hkVertexFormat, m_elements) == 0x0);
	static_assert(offsetof(hkVertexFormat, m_numElements) == 0x100);

	class hkMemoryMeshVertexBuffer : public hkMeshVertexBuffer
	{
	public:
		/* 0x010 */ hkVertexFormat m_format;
		/* 0x114 */ std::int32_t m_elementOffsets[32];
		/* 0x194 */ std::byte pad194[0x4];
		/* 0x198 */ hkArray<std::uint8_t> m_memory;
		/* 0x1A8 */ std::int32_t m_vertexStride;
		/* 0x1AC */ hkBool m_locked;
		/* 0x1AD */ std::byte pad1AD[0x3];
		/* 0x1B0 */ std::int32_t m_numVertices;
		/* 0x1B4 */ hkBool m_isBigEndian;
		/* 0x1B5 */ hkBool m_isSharable;
		/* 0x1B6 */ std::byte pad1B6[0x2];
	};
	static_assert(sizeof(hkMemoryMeshVertexBuffer) == 0x1B8);
	static_assert(offsetof(hkMemoryMeshVertexBuffer, m_format) == 0x10);
	static_assert(offsetof(hkMemoryMeshVertexBuffer, m_elementOffsets) == 0x114);
	static_assert(offsetof(hkMemoryMeshVertexBuffer, m_memory) == 0x198);
	static_assert(offsetof(hkMemoryMeshVertexBuffer, m_vertexStride) == 0x1A8);
	static_assert(offsetof(hkMemoryMeshVertexBuffer, m_locked) == 0x1AC);
	static_assert(offsetof(hkMemoryMeshVertexBuffer, m_numVertices) == 0x1B0);
	static_assert(offsetof(hkMemoryMeshVertexBuffer, m_isBigEndian) == 0x1B4);
	static_assert(offsetof(hkMemoryMeshVertexBuffer, m_isSharable) == 0x1B5);

	class hkResourceBase : public hkReferencedObject
	{
	public:
		virtual void hkResourceBase_Unk04();
		virtual void hkResourceBase_Unk05();
	};
	static_assert(sizeof(hkResourceBase) == 0x10);

	class hkResourceContainer : public hkResourceBase
	{
	public:
		virtual void hkResourceContainer_Unk06();
		virtual void hkResourceContainer_Unk07();
		virtual void hkResourceContainer_Unk08();
		virtual void hkResourceContainer_Unk09();
		virtual void hkResourceContainer_Unk0A();
		virtual void hkResourceContainer_Unk0B();
		virtual void hkResourceContainer_Unk0C();
		virtual void hkResourceContainer_Unk0D();
		virtual void hkResourceContainer_Unk0E();
		virtual void hkResourceContainer_Unk0F();
		virtual void hkResourceContainer_Unk10();
	};
	static_assert(sizeof(hkResourceContainer) == 0x10);

	class hkMemoryResourceContainer : public hkResourceContainer
	{
	public:
		/* 0x010 */ hkStringPtr m_name;
		/* 0x018 */ hkMemoryResourceContainer* m_parent;
		/* 0x020 */ hkArray<hkMemoryResourceHandle*> m_resourceHandles;
		/* 0x030 */ hkArray<hkMemoryResourceContainer*> m_children;
	};
	static_assert(sizeof(hkMemoryResourceContainer) == 0x40);
	static_assert(offsetof(hkMemoryResourceContainer, m_name) == 0x10);
	static_assert(offsetof(hkMemoryResourceContainer, m_parent) == 0x18);
	static_assert(offsetof(hkMemoryResourceContainer, m_resourceHandles) == 0x20);
	static_assert(offsetof(hkMemoryResourceContainer, m_children) == 0x30);

	class hkResourceHandle : public hkResourceBase
	{
	public:
		virtual void hkResourceHandle_Unk06();
		virtual void hkResourceHandle_Unk07();
		virtual void hkResourceHandle_Unk08();
		virtual void hkResourceHandle_Unk09();
		virtual void hkResourceHandle_Unk0A();
		virtual void hkResourceHandle_Unk0B();
		virtual void hkResourceHandle_Unk0C();
		virtual void hkResourceHandle_Unk0D();
		virtual void hkResourceHandle_Unk0E();
	};
	static_assert(sizeof(hkResourceHandle) == 0x10);

	class hkMemoryResourceHandle : public hkResourceHandle
	{
	public:
		/* 0x010 */ hkReferencedObject* m_variant;
		/* 0x018 */ hkStringPtr m_name;
		/* 0x020 */ hkArray<hkMemoryResourceHandleExternalLink> m_references;
	};
	static_assert(sizeof(hkMemoryResourceHandle) == 0x30);
	static_assert(offsetof(hkMemoryResourceHandle, m_variant) == 0x10);
	static_assert(offsetof(hkMemoryResourceHandle, m_name) == 0x18);
	static_assert(offsetof(hkMemoryResourceHandle, m_references) == 0x20);

	class hkMemoryResourceHandleExternalLink
	{
	public:
		/* 0x000 */ hkStringPtr m_memberName;
		/* 0x008 */ hkStringPtr m_externalId;
	};
	static_assert(sizeof(hkMemoryResourceHandleExternalLink) == 0x10);
	static_assert(offsetof(hkMemoryResourceHandleExternalLink, m_memberName) == 0x0);
	static_assert(offsetof(hkMemoryResourceHandleExternalLink, m_externalId) == 0x8);

	class hkMeshSection
	{
	public:
		/* 0x000 */ std::uint8_t m_primitiveType;
		/* 0x001 */ std::byte pad001[0x3];
		/* 0x004 */ std::int32_t m_numPrimitives;
		/* 0x008 */ std::int32_t m_numIndices;
		/* 0x00C */ std::int32_t m_vertexStartIndex;
		/* 0x010 */ std::int32_t m_transformIndex;
		/* 0x014 */ std::uint8_t m_indexType;
		/* 0x015 */ std::byte pad015[0x3];
		/* 0x018 */ void* m_indices;
		/* 0x020 */ hkMeshVertexBuffer* m_vertexBuffer;
		/* 0x028 */ hkMeshMaterial* m_material;
		/* 0x030 */ hkMeshBoneIndexMapping* m_boneMatrixMap;
		/* 0x038 */ std::int32_t m_sectionIndex;
		/* 0x03C */ std::byte pad03C[0x4];
	};
	static_assert(sizeof(hkMeshSection) == 0x40);
	static_assert(offsetof(hkMeshSection, m_primitiveType) == 0x0);
	static_assert(offsetof(hkMeshSection, m_numPrimitives) == 0x4);
	static_assert(offsetof(hkMeshSection, m_numIndices) == 0x8);
	static_assert(offsetof(hkMeshSection, m_vertexStartIndex) == 0xC);
	static_assert(offsetof(hkMeshSection, m_transformIndex) == 0x10);
	static_assert(offsetof(hkMeshSection, m_indexType) == 0x14);
	static_assert(offsetof(hkMeshSection, m_indices) == 0x18);
	static_assert(offsetof(hkMeshSection, m_vertexBuffer) == 0x20);
	static_assert(offsetof(hkMeshSection, m_material) == 0x28);
	static_assert(offsetof(hkMeshSection, m_boneMatrixMap) == 0x30);
	static_assert(offsetof(hkMeshSection, m_sectionIndex) == 0x38);

	class hkMeshSectionCinfo
	{
	public:
		/* 0x000 */ hkMeshVertexBuffer* m_vertexBuffer;
		/* 0x008 */ hkMeshMaterial* m_material;
		/* 0x010 */ hkMeshBoneIndexMapping m_boneMatrixMap;
		/* 0x020 */ std::uint8_t m_primitiveType;
		/* 0x021 */ std::byte pad021[0x3];
		/* 0x024 */ std::int32_t m_numPrimitives;
		/* 0x028 */ std::uint8_t m_indexType;
		/* 0x029 */ std::byte pad029[0x7];
		/* 0x030 */ void* m_indices;
		/* 0x038 */ std::int32_t m_vertexStartIndex;
		/* 0x03C */ std::int32_t m_transformIndex;
	};
	static_assert(sizeof(hkMeshSectionCinfo) == 0x40);
	static_assert(offsetof(hkMeshSectionCinfo, m_vertexBuffer) == 0x0);
	static_assert(offsetof(hkMeshSectionCinfo, m_material) == 0x8);
	static_assert(offsetof(hkMeshSectionCinfo, m_boneMatrixMap) == 0x10);
	static_assert(offsetof(hkMeshSectionCinfo, m_primitiveType) == 0x20);
	static_assert(offsetof(hkMeshSectionCinfo, m_numPrimitives) == 0x24);
	static_assert(offsetof(hkMeshSectionCinfo, m_indexType) == 0x28);
	static_assert(offsetof(hkMeshSectionCinfo, m_indices) == 0x30);
	static_assert(offsetof(hkMeshSectionCinfo, m_vertexStartIndex) == 0x38);
	static_assert(offsetof(hkMeshSectionCinfo, m_transformIndex) == 0x3C);

	class hkMeshTextureRawBufferDescriptor
	{
	public:
		/* 0x000 */ std::int64_t m_offset;
		/* 0x008 */ std::uint32_t m_stride;
		/* 0x00C */ std::uint32_t m_numElements;
	};
	static_assert(sizeof(hkMeshTextureRawBufferDescriptor) == 0x10);
	static_assert(offsetof(hkMeshTextureRawBufferDescriptor, m_offset) == 0x0);
	static_assert(offsetof(hkMeshTextureRawBufferDescriptor, m_stride) == 0x8);
	static_assert(offsetof(hkMeshTextureRawBufferDescriptor, m_numElements) == 0xC);

	class hkModelerNodeTypeAttribute
	{
	public:
		/* 0x000 */ std::int8_t m_type;
	};
	static_assert(sizeof(hkModelerNodeTypeAttribute) == 0x1);
	static_assert(offsetof(hkModelerNodeTypeAttribute, m_type) == 0x0);

	class hkMonitorStreamColorTable : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkMonitorStreamColorTableColorPair> m_colorPairs;
		/* 0x020 */ std::uint32_t m_defaultColor;
		/* 0x024 */ std::byte pad024[0x4];
	};
	static_assert(sizeof(hkMonitorStreamColorTable) == 0x28);
	static_assert(offsetof(hkMonitorStreamColorTable, m_colorPairs) == 0x10);
	static_assert(offsetof(hkMonitorStreamColorTable, m_defaultColor) == 0x20);

	class hkMonitorStreamColorTableColorPair
	{
	public:
		/* 0x000 */ hkStringPtr m_colorName;
		/* 0x008 */ std::uint32_t m_color;
		/* 0x00C */ std::byte pad00C[0x4];
	};
	static_assert(sizeof(hkMonitorStreamColorTableColorPair) == 0x10);
	static_assert(offsetof(hkMonitorStreamColorTableColorPair, m_colorName) == 0x0);
	static_assert(offsetof(hkMonitorStreamColorTableColorPair, m_color) == 0x8);

	class hkMonitorStreamFrameInfo
	{
	public:
		/* 0x000 */ hkStringPtr m_heading;
		/* 0x008 */ std::int32_t m_indexOfTimer0;
		/* 0x00C */ std::int32_t m_indexOfTimer1;
		/* 0x010 */ std::uint32_t m_absoluteTimeCounter;
		/* 0x014 */ float m_timerFactor0;
		/* 0x018 */ float m_timerFactor1;
		/* 0x01C */ std::int32_t m_threadId;
		/* 0x020 */ std::int32_t m_frameStreamStart;
		/* 0x024 */ std::int32_t m_frameStreamEnd;
	};
	static_assert(sizeof(hkMonitorStreamFrameInfo) == 0x28);
	static_assert(offsetof(hkMonitorStreamFrameInfo, m_heading) == 0x0);
	static_assert(offsetof(hkMonitorStreamFrameInfo, m_indexOfTimer0) == 0x8);
	static_assert(offsetof(hkMonitorStreamFrameInfo, m_indexOfTimer1) == 0xC);
	static_assert(offsetof(hkMonitorStreamFrameInfo, m_absoluteTimeCounter) == 0x10);
	static_assert(offsetof(hkMonitorStreamFrameInfo, m_timerFactor0) == 0x14);
	static_assert(offsetof(hkMonitorStreamFrameInfo, m_timerFactor1) == 0x18);
	static_assert(offsetof(hkMonitorStreamFrameInfo, m_threadId) == 0x1C);
	static_assert(offsetof(hkMonitorStreamFrameInfo, m_frameStreamStart) == 0x20);
	static_assert(offsetof(hkMonitorStreamFrameInfo, m_frameStreamEnd) == 0x24);

	class hkMonitorStreamStringMap
	{
	public:
		/* 0x000 */ hkArray<hkMonitorStreamStringMapStringMap> m_map;
	};
	static_assert(sizeof(hkMonitorStreamStringMap) == 0x10);
	static_assert(offsetof(hkMonitorStreamStringMap, m_map) == 0x0);

	class hkMonitorStreamStringMapStringMap
	{
	public:
		/* 0x000 */ std::uint64_t m_id;
		/* 0x008 */ hkStringPtr m_string;
	};
	static_assert(sizeof(hkMonitorStreamStringMapStringMap) == 0x10);
	static_assert(offsetof(hkMonitorStreamStringMapStringMap, m_id) == 0x0);
	static_assert(offsetof(hkMonitorStreamStringMapStringMap, m_string) == 0x8);

	class hkMotionState
	{
	public:
		/* 0x000 */ hkTransform m_transform;
		/* 0x040 */ hkVector4 m_sweptTransform[5];
		/* 0x090 */ hkVector4 m_deltaAngle;
		/* 0x0A0 */ float m_objectRadius;
		/* 0x0A4 */ hkHalf m_linearDamping;
		/* 0x0A6 */ hkHalf m_angularDamping;
		/* 0x0A8 */ hkHalf m_timeFactor;
		/* 0x0AA */ hkUFloat8 m_maxLinearVelocity;
		/* 0x0AB */ hkUFloat8 m_maxAngularVelocity;
		/* 0x0AC */ std::uint8_t m_deactivationClass;
		/* 0x0AD */ std::byte pad0AD[0x3];
	};
	static_assert(sizeof(hkMotionState) == 0xB0);
	static_assert(offsetof(hkMotionState, m_transform) == 0x0);
	static_assert(offsetof(hkMotionState, m_sweptTransform) == 0x40);
	static_assert(offsetof(hkMotionState, m_deltaAngle) == 0x90);
	static_assert(offsetof(hkMotionState, m_objectRadius) == 0xA0);
	static_assert(offsetof(hkMotionState, m_linearDamping) == 0xA4);
	static_assert(offsetof(hkMotionState, m_angularDamping) == 0xA6);
	static_assert(offsetof(hkMotionState, m_timeFactor) == 0xA8);
	static_assert(offsetof(hkMotionState, m_maxLinearVelocity) == 0xAA);
	static_assert(offsetof(hkMotionState, m_maxAngularVelocity) == 0xAB);
	static_assert(offsetof(hkMotionState, m_deactivationClass) == 0xAC);

	class hkMultiThreadCheck
	{
	public:
		/* 0x000 */ std::uint32_t m_threadId;
		/* 0x004 */ std::int32_t m_stackTraceId;
		/* 0x008 */ std::uint16_t m_markCount;
		/* 0x00A */ std::uint16_t m_markBitStack;
	};
	static_assert(sizeof(hkMultiThreadCheck) == 0xC);
	static_assert(offsetof(hkMultiThreadCheck, m_threadId) == 0x0);
	static_assert(offsetof(hkMultiThreadCheck, m_stackTraceId) == 0x4);
	static_assert(offsetof(hkMultiThreadCheck, m_markCount) == 0x8);
	static_assert(offsetof(hkMultiThreadCheck, m_markBitStack) == 0xA);

	class hkMultipleVertexBuffer : public hkMeshVertexBuffer
	{
	public:
		/* 0x010 */ hkVertexFormat m_vertexFormat;
		/* 0x114 */ std::byte pad114[0x4];
		/* 0x118 */ hkArray<hkMultipleVertexBufferLockedElement> m_lockedElements;
		/* 0x128 */ hkMemoryMeshVertexBuffer* m_lockedBuffer;
		/* 0x130 */ hkArray<hkMultipleVertexBufferElementInfo> m_elementInfos;
		/* 0x140 */ hkArray<hkMultipleVertexBufferVertexBufferInfo> m_vertexBufferInfos;
		/* 0x150 */ std::int32_t m_numVertices;
		/* 0x154 */ hkBool m_isLocked;
		/* 0x155 */ std::byte pad155[0x3];
		/* 0x158 */ std::uint32_t m_updateCount;
		/* 0x15C */ hkBool m_writeLock;
		/* 0x15D */ hkBool m_isSharable;
		/* 0x15E */ hkBool m_constructionComplete;
		/* 0x15F */ std::byte pad15F[0x1];
	};
	static_assert(sizeof(hkMultipleVertexBuffer) == 0x160);
	static_assert(offsetof(hkMultipleVertexBuffer, m_vertexFormat) == 0x10);
	static_assert(offsetof(hkMultipleVertexBuffer, m_lockedElements) == 0x118);
	static_assert(offsetof(hkMultipleVertexBuffer, m_lockedBuffer) == 0x128);
	static_assert(offsetof(hkMultipleVertexBuffer, m_elementInfos) == 0x130);
	static_assert(offsetof(hkMultipleVertexBuffer, m_vertexBufferInfos) == 0x140);
	static_assert(offsetof(hkMultipleVertexBuffer, m_numVertices) == 0x150);
	static_assert(offsetof(hkMultipleVertexBuffer, m_isLocked) == 0x154);
	static_assert(offsetof(hkMultipleVertexBuffer, m_updateCount) == 0x158);
	static_assert(offsetof(hkMultipleVertexBuffer, m_writeLock) == 0x15C);
	static_assert(offsetof(hkMultipleVertexBuffer, m_isSharable) == 0x15D);
	static_assert(offsetof(hkMultipleVertexBuffer, m_constructionComplete) == 0x15E);

	class hkMultipleVertexBufferElementInfo
	{
	public:
		/* 0x000 */ std::uint8_t m_vertexBufferIndex;
		/* 0x001 */ std::uint8_t m_elementIndex;
	};
	static_assert(sizeof(hkMultipleVertexBufferElementInfo) == 0x2);
	static_assert(offsetof(hkMultipleVertexBufferElementInfo, m_vertexBufferIndex) == 0x0);
	static_assert(offsetof(hkMultipleVertexBufferElementInfo, m_elementIndex) == 0x1);

	class hkMultipleVertexBufferLockedElement
	{
	public:
		/* 0x000 */ std::uint8_t m_vertexBufferIndex;
		/* 0x001 */ std::uint8_t m_elementIndex;
		/* 0x002 */ std::uint8_t m_lockedBufferIndex;
		/* 0x003 */ std::uint8_t m_vertexFormatIndex;
		/* 0x004 */ std::uint8_t m_lockFlags;
		/* 0x005 */ std::uint8_t m_outputBufferIndex;
		/* 0x006 */ std::int8_t m_emulatedIndex;
	};
	static_assert(sizeof(hkMultipleVertexBufferLockedElement) == 0x7);
	static_assert(offsetof(hkMultipleVertexBufferLockedElement, m_vertexBufferIndex) == 0x0);
	static_assert(offsetof(hkMultipleVertexBufferLockedElement, m_elementIndex) == 0x1);
	static_assert(offsetof(hkMultipleVertexBufferLockedElement, m_lockedBufferIndex) == 0x2);
	static_assert(offsetof(hkMultipleVertexBufferLockedElement, m_vertexFormatIndex) == 0x3);
	static_assert(offsetof(hkMultipleVertexBufferLockedElement, m_lockFlags) == 0x4);
	static_assert(offsetof(hkMultipleVertexBufferLockedElement, m_outputBufferIndex) == 0x5);
	static_assert(offsetof(hkMultipleVertexBufferLockedElement, m_emulatedIndex) == 0x6);

	class hkMultipleVertexBufferVertexBufferInfo
	{
	public:
		/* 0x000 */ hkMeshVertexBuffer* m_vertexBuffer;
		/* 0x008 */ void* m_lockedVertices;
		/* 0x010 */ hkBool m_isLocked;
		/* 0x011 */ std::byte pad011[0x7];
	};
	static_assert(sizeof(hkMultipleVertexBufferVertexBufferInfo) == 0x18);
	static_assert(offsetof(hkMultipleVertexBufferVertexBufferInfo, m_vertexBuffer) == 0x0);
	static_assert(offsetof(hkMultipleVertexBufferVertexBufferInfo, m_lockedVertices) == 0x8);
	static_assert(offsetof(hkMultipleVertexBufferVertexBufferInfo, m_isLocked) == 0x10);

	class hkOffsetBitField : public hkBitFieldBasehkOffsetBitFieldStoragehkArrayunsignedinthkContainerHeapAllocator
	{
	public:
	};
	static_assert(sizeof(hkOffsetBitField) == 0x18);

	class hkOstream : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::byte pad010[0x8];
	};
	static_assert(sizeof(hkOstream) == 0x18);

	class hkPackedVector3
	{
	public:
		/* 0x000 */ std::int16_t m_values[4];
	};
	static_assert(sizeof(hkPackedVector3) == 0x8);
	static_assert(offsetof(hkPackedVector3, m_values) == 0x0);

	class hkPackedVector8_3
	{
	public:
		/* 0x000 */ std::int8_t m_values[4];
	};
	static_assert(sizeof(hkPackedVector8_3) == 0x4);
	static_assert(offsetof(hkPackedVector8_3, m_values) == 0x0);

	class hkPackfileHeader
	{
	public:
		/* 0x000 */ std::int32_t m_magic[2];
		/* 0x008 */ std::int32_t m_userTag;
		/* 0x00C */ std::int32_t m_fileVersion;
		/* 0x010 */ std::uint8_t m_layoutRules[4];
		/* 0x014 */ std::int32_t m_numSections;
		/* 0x018 */ std::int32_t m_contentsSectionIndex;
		/* 0x01C */ std::int32_t m_contentsSectionOffset;
		/* 0x020 */ std::int32_t m_contentsClassNameSectionIndex;
		/* 0x024 */ std::int32_t m_contentsClassNameSectionOffset;
		/* 0x028 */ char m_contentsVersion[16];
		/* 0x038 */ std::int32_t m_flags;
		/* 0x03C */ std::uint16_t m_maxpredicate;
		/* 0x03E */ std::uint16_t m_predicateArraySizePlusPadding;
	};
	static_assert(sizeof(hkPackfileHeader) == 0x40);
	static_assert(offsetof(hkPackfileHeader, m_magic) == 0x0);
	static_assert(offsetof(hkPackfileHeader, m_userTag) == 0x8);
	static_assert(offsetof(hkPackfileHeader, m_fileVersion) == 0xC);
	static_assert(offsetof(hkPackfileHeader, m_layoutRules) == 0x10);
	static_assert(offsetof(hkPackfileHeader, m_numSections) == 0x14);
	static_assert(offsetof(hkPackfileHeader, m_contentsSectionIndex) == 0x18);
	static_assert(offsetof(hkPackfileHeader, m_contentsSectionOffset) == 0x1C);
	static_assert(offsetof(hkPackfileHeader, m_contentsClassNameSectionIndex) == 0x20);
	static_assert(offsetof(hkPackfileHeader, m_contentsClassNameSectionOffset) == 0x24);
	static_assert(offsetof(hkPackfileHeader, m_contentsVersion) == 0x28);
	static_assert(offsetof(hkPackfileHeader, m_flags) == 0x38);
	static_assert(offsetof(hkPackfileHeader, m_maxpredicate) == 0x3C);
	static_assert(offsetof(hkPackfileHeader, m_predicateArraySizePlusPadding) == 0x3E);

	class hkPackfileSectionHeader
	{
	public:
		/* 0x000 */ char m_sectionTag[19];
		/* 0x013 */ char m_nullByte;
		/* 0x014 */ std::int32_t m_absoluteDataStart;
		/* 0x018 */ std::int32_t m_localFixupsOffset;
		/* 0x01C */ std::int32_t m_globalFixupsOffset;
		/* 0x020 */ std::int32_t m_virtualFixupsOffset;
		/* 0x024 */ std::int32_t m_exportsOffset;
		/* 0x028 */ std::int32_t m_importsOffset;
		/* 0x02C */ std::int32_t m_endOffset;
		/* 0x030 */ std::int32_t m_pad[4];
	};
	static_assert(sizeof(hkPackfileSectionHeader) == 0x40);
	static_assert(offsetof(hkPackfileSectionHeader, m_sectionTag) == 0x0);
	static_assert(offsetof(hkPackfileSectionHeader, m_nullByte) == 0x13);
	static_assert(offsetof(hkPackfileSectionHeader, m_absoluteDataStart) == 0x14);
	static_assert(offsetof(hkPackfileSectionHeader, m_localFixupsOffset) == 0x18);
	static_assert(offsetof(hkPackfileSectionHeader, m_globalFixupsOffset) == 0x1C);
	static_assert(offsetof(hkPackfileSectionHeader, m_virtualFixupsOffset) == 0x20);
	static_assert(offsetof(hkPackfileSectionHeader, m_exportsOffset) == 0x24);
	static_assert(offsetof(hkPackfileSectionHeader, m_importsOffset) == 0x28);
	static_assert(offsetof(hkPackfileSectionHeader, m_endOffset) == 0x2C);
	static_assert(offsetof(hkPackfileSectionHeader, m_pad) == 0x30);

	class hkPostFinishAttribute
	{
	public:
		/* 0x000 */ void* m_postFinishFunction;
	};
	static_assert(sizeof(hkPostFinishAttribute) == 0x8);
	static_assert(offsetof(hkPostFinishAttribute, m_postFinishFunction) == 0x0);

	class hkQTransformf
	{
	public:
		/* 0x000 */ hkQuaternion m_rotation;
		/* 0x010 */ hkVector4 m_translation;
	};
	static_assert(sizeof(hkQTransformf) == 0x20);
	static_assert(offsetof(hkQTransformf, m_rotation) == 0x0);
	static_assert(offsetof(hkQTransformf, m_translation) == 0x10);

	class hkQTransform : public hkQTransformf
	{
	public:
	};
	static_assert(sizeof(hkQTransform) == 0x20);

	class hkRangeInt32Attribute
	{
	public:
		/* 0x000 */ std::int32_t m_absmin;
		/* 0x004 */ std::int32_t m_absmax;
		/* 0x008 */ std::int32_t m_softmin;
		/* 0x00C */ std::int32_t m_softmax;
	};
	static_assert(sizeof(hkRangeInt32Attribute) == 0x10);
	static_assert(offsetof(hkRangeInt32Attribute, m_absmin) == 0x0);
	static_assert(offsetof(hkRangeInt32Attribute, m_absmax) == 0x4);
	static_assert(offsetof(hkRangeInt32Attribute, m_softmin) == 0x8);
	static_assert(offsetof(hkRangeInt32Attribute, m_softmax) == 0xC);

	class hkRangeRealAttribute
	{
	public:
		/* 0x000 */ float m_absmin;
		/* 0x004 */ float m_absmax;
		/* 0x008 */ float m_softmin;
		/* 0x00C */ float m_softmax;
	};
	static_assert(sizeof(hkRangeRealAttribute) == 0x10);
	static_assert(offsetof(hkRangeRealAttribute, m_absmin) == 0x0);
	static_assert(offsetof(hkRangeRealAttribute, m_absmax) == 0x4);
	static_assert(offsetof(hkRangeRealAttribute, m_softmin) == 0x8);
	static_assert(offsetof(hkRangeRealAttribute, m_softmax) == 0xC);

	class hkRefCountedProperties
	{
	public:
		/* 0x000 */ hkArray<hkRefCountedPropertiesEntry> m_entries;
	};
	static_assert(sizeof(hkRefCountedProperties) == 0x10);
	static_assert(offsetof(hkRefCountedProperties, m_entries) == 0x0);

	class hkRefCountedPropertiesEntry
	{
	public:
		/* 0x000 */ hkReferencedObject* m_object;
		/* 0x008 */ std::uint16_t m_key;
		/* 0x00A */ std::uint16_t m_flags;
		/* 0x00C */ std::byte pad00C[0x4];
	};
	static_assert(sizeof(hkRefCountedPropertiesEntry) == 0x10);
	static_assert(offsetof(hkRefCountedPropertiesEntry, m_object) == 0x0);
	static_assert(offsetof(hkRefCountedPropertiesEntry, m_key) == 0x8);
	static_assert(offsetof(hkRefCountedPropertiesEntry, m_flags) == 0xA);

	class hkRootLevelContainer
	{
	public:
		/* 0x000 */ hkArray<hkRootLevelContainerNamedVariant> m_namedVariants;
	};
	static_assert(sizeof(hkRootLevelContainer) == 0x10);
	static_assert(offsetof(hkRootLevelContainer, m_namedVariants) == 0x0);

	class hkRootLevelContainerNamedVariant
	{
	public:
		/* 0x000 */ hkStringPtr m_name;
		/* 0x008 */ hkStringPtr m_className;
		/* 0x010 */ hkReferencedObject* m_variant;
	};
	static_assert(sizeof(hkRootLevelContainerNamedVariant) == 0x18);
	static_assert(offsetof(hkRootLevelContainerNamedVariant, m_name) == 0x0);
	static_assert(offsetof(hkRootLevelContainerNamedVariant, m_className) == 0x8);
	static_assert(offsetof(hkRootLevelContainerNamedVariant, m_variant) == 0x10);

	class hkScriptableAttribute
	{
	public:
	};
	static_assert(sizeof(hkScriptableAttribute) == 0x1);

	class hkSemanticsAttribute
	{
	public:
		/* 0x000 */ std::int8_t m_type;
	};
	static_assert(sizeof(hkSemanticsAttribute) == 0x1);
	static_assert(offsetof(hkSemanticsAttribute, m_type) == 0x0);

	class hkSethkIntRealPairhkContainerHeapAllocatorhkMapOperationshkIntRealPair
	{
	public:
		/* 0x000 */ hkArray<hkIntRealPair> m_elem;
		/* 0x010 */ std::int32_t m_numElems;
		/* 0x014 */ std::byte pad014[0x4];
	};
	static_assert(sizeof(hkSethkIntRealPairhkContainerHeapAllocatorhkMapOperationshkIntRealPair) == 0x18);
	static_assert(offsetof(hkSethkIntRealPairhkContainerHeapAllocatorhkMapOperationshkIntRealPair, m_elem) == 0x0);
	static_assert(offsetof(hkSethkIntRealPairhkContainerHeapAllocatorhkMapOperationshkIntRealPair, m_numElems) == 0x10);

	class hkSetIntFloatPair : public hkSethkIntRealPairhkContainerHeapAllocatorhkMapOperationshkIntRealPair
	{
	public:
	};
	static_assert(sizeof(hkSetIntFloatPair) == 0x18);

	class hkSetunsignedinthkContainerHeapAllocatorhkMapOperationsunsignedint
	{
	public:
		/* 0x000 */ hkArray<std::uint32_t> m_elem;
		/* 0x010 */ std::int32_t m_numElems;
		/* 0x014 */ std::byte pad014[0x4];
	};
	static_assert(sizeof(hkSetunsignedinthkContainerHeapAllocatorhkMapOperationsunsignedint) == 0x18);
	static_assert(offsetof(hkSetunsignedinthkContainerHeapAllocatorhkMapOperationsunsignedint, m_elem) == 0x0);
	static_assert(offsetof(hkSetunsignedinthkContainerHeapAllocatorhkMapOperationsunsignedint, m_numElems) == 0x10);

	class hkSetUint32 : public hkSetunsignedinthkContainerHeapAllocatorhkMapOperationsunsignedint
	{
	public:
	};
	static_assert(sizeof(hkSetUint32) == 0x18);

	class hkSetunsignedlonglonghkContainerHeapAllocatorhkMapOperationsunsignedlonglong
	{
	public:
		/* 0x000 */ hkArray<std::uint64_t> m_elem;
		/* 0x010 */ std::int32_t m_numElems;
		/* 0x014 */ std::byte pad014[0x4];
	};
	static_assert(sizeof(hkSetunsignedlonglonghkContainerHeapAllocatorhkMapOperationsunsignedlonglong) == 0x18);
	static_assert(offsetof(hkSetunsignedlonglonghkContainerHeapAllocatorhkMapOperationsunsignedlonglong, m_elem) == 0x0);
	static_assert(offsetof(hkSetunsignedlonglonghkContainerHeapAllocatorhkMapOperationsunsignedlonglong, m_numElems) == 0x10);

	class hkSetUint64 : public hkSetunsignedlonglonghkContainerHeapAllocatorhkMapOperationsunsignedlonglong
	{
	public:
	};
	static_assert(sizeof(hkSetUint64) == 0x18);

	class hkSimpleLocalFrame : public hkLocalFrame
	{
	public:
		/* 0x010 */ hkTransform m_transform;
		/* 0x050 */ hkArray<hkLocalFrame*> m_children;
		/* 0x060 */ hkLocalFrame* m_parentFrame;
		/* 0x068 */ hkLocalFrameGroup* m_group;
		/* 0x070 */ hkStringPtr m_name;
		/* 0x078 */ std::byte pad078[0x8];
	};
	static_assert(sizeof(hkSimpleLocalFrame) == 0x80);
	static_assert(offsetof(hkSimpleLocalFrame, m_transform) == 0x10);
	static_assert(offsetof(hkSimpleLocalFrame, m_children) == 0x50);
	static_assert(offsetof(hkSimpleLocalFrame, m_parentFrame) == 0x60);
	static_assert(offsetof(hkSimpleLocalFrame, m_group) == 0x68);
	static_assert(offsetof(hkSimpleLocalFrame, m_name) == 0x70);

	class hkSimplePropertyValue
	{
	public:
		/* 0x000 */ std::uint64_t m_data;
	};
	static_assert(sizeof(hkSimplePropertyValue) == 0x8);
	static_assert(offsetof(hkSimplePropertyValue, m_data) == 0x0);

	class hkSimpleProperty
	{
	public:
		/* 0x000 */ std::uint32_t m_key;
		/* 0x004 */ std::uint32_t m_alignmentPadding;
		/* 0x008 */ hkSimplePropertyValue m_value;
	};
	static_assert(sizeof(hkSimpleProperty) == 0x10);
	static_assert(offsetof(hkSimpleProperty, m_key) == 0x0);
	static_assert(offsetof(hkSimpleProperty, m_alignmentPadding) == 0x4);
	static_assert(offsetof(hkSimpleProperty, m_value) == 0x8);

	class hkSkinBinding : public hkMeshShape
	{
	public:
		/* 0x010 */ hkMeshShape* m_skin;
		/* 0x018 */ hkArray<hkMatrix4> m_worldFromBoneTransforms;
		/* 0x028 */ hkArray<hkStringPtr> m_boneNames;
	};
	static_assert(sizeof(hkSkinBinding) == 0x38);
	static_assert(offsetof(hkSkinBinding, m_skin) == 0x10);
	static_assert(offsetof(hkSkinBinding, m_worldFromBoneTransforms) == 0x18);
	static_assert(offsetof(hkSkinBinding, m_boneNames) == 0x28);

	class hkSkinnedMeshShape : public hkReferencedObject
	{
	public:
		virtual void hkSkinnedMeshShape_Unk04();
		virtual void hkSkinnedMeshShape_Unk05();
		virtual void hkSkinnedMeshShape_Unk06();
		virtual void hkSkinnedMeshShape_Unk07();
		virtual void hkSkinnedMeshShape_Unk08();
		virtual void hkSkinnedMeshShape_Unk09();
		virtual void hkSkinnedMeshShape_Unk0A();
		virtual void hkSkinnedMeshShape_Unk0B();
		virtual void hkSkinnedMeshShape_Unk0C();
		virtual void hkSkinnedMeshShape_Unk0D();
		virtual void hkSkinnedMeshShape_Unk0E();
		virtual void hkSkinnedMeshShape_Unk0F();
		virtual void hkSkinnedMeshShape_Unk10();
	};
	static_assert(sizeof(hkSkinnedMeshShape) == 0x10);

	class hkSkinnedMeshShapeBoneSection
	{
	public:
		/* 0x000 */ hkMeshShape* m_meshBuffer;
		/* 0x008 */ std::uint16_t m_startBoneSetId;
		/* 0x00A */ std::int16_t m_numBoneSets;
		/* 0x00C */ std::byte pad00C[0x4];
	};
	static_assert(sizeof(hkSkinnedMeshShapeBoneSection) == 0x10);
	static_assert(offsetof(hkSkinnedMeshShapeBoneSection, m_meshBuffer) == 0x0);
	static_assert(offsetof(hkSkinnedMeshShapeBoneSection, m_startBoneSetId) == 0x8);
	static_assert(offsetof(hkSkinnedMeshShapeBoneSection, m_numBoneSets) == 0xA);

	class hkSkinnedMeshShapeBoneSet
	{
	public:
		/* 0x000 */ std::uint16_t m_boneBufferOffset;
		/* 0x002 */ std::uint16_t m_numBones;
	};
	static_assert(sizeof(hkSkinnedMeshShapeBoneSet) == 0x4);
	static_assert(offsetof(hkSkinnedMeshShapeBoneSet, m_boneBufferOffset) == 0x0);
	static_assert(offsetof(hkSkinnedMeshShapeBoneSet, m_numBones) == 0x2);

	class hkSkinnedMeshShapePart
	{
	public:
		/* 0x000 */ std::int32_t m_startVertex;
		/* 0x004 */ std::int32_t m_numVertices;
		/* 0x008 */ std::int32_t m_startIndex;
		/* 0x00C */ std::int32_t m_numIndices;
		/* 0x010 */ std::uint16_t m_boneSetId;
		/* 0x012 */ std::uint16_t m_meshSectionIndex;
		/* 0x014 */ std::byte pad014[0xC];
		/* 0x020 */ hkVector4 m_boundingSphere;
	};
	static_assert(sizeof(hkSkinnedMeshShapePart) == 0x30);
	static_assert(offsetof(hkSkinnedMeshShapePart, m_startVertex) == 0x0);
	static_assert(offsetof(hkSkinnedMeshShapePart, m_numVertices) == 0x4);
	static_assert(offsetof(hkSkinnedMeshShapePart, m_startIndex) == 0x8);
	static_assert(offsetof(hkSkinnedMeshShapePart, m_numIndices) == 0xC);
	static_assert(offsetof(hkSkinnedMeshShapePart, m_boneSetId) == 0x10);
	static_assert(offsetof(hkSkinnedMeshShapePart, m_meshSectionIndex) == 0x12);
	static_assert(offsetof(hkSkinnedMeshShapePart, m_boundingSphere) == 0x20);

	class hkSkinnedRefMeshShape : public hkMeshShape
	{
	public:
		/* 0x010 */ hkSkinnedMeshShape* m_skinnedMeshShape;
		/* 0x018 */ hkArray<std::int16_t> m_bones;
		/* 0x028 */ hkArray<hkVector4> m_localFromRootTransforms;
		/* 0x038 */ hkStringPtr m_name;
	};
	static_assert(sizeof(hkSkinnedRefMeshShape) == 0x40);
	static_assert(offsetof(hkSkinnedRefMeshShape, m_skinnedMeshShape) == 0x10);
	static_assert(offsetof(hkSkinnedRefMeshShape, m_bones) == 0x18);
	static_assert(offsetof(hkSkinnedRefMeshShape, m_localFromRootTransforms) == 0x28);
	static_assert(offsetof(hkSkinnedRefMeshShape, m_name) == 0x38);

	class hkSphere
	{
	public:
		/* 0x000 */ hkVector4 m_pos;
	};
	static_assert(sizeof(hkSphere) == 0x10);
	static_assert(offsetof(hkSphere, m_pos) == 0x0);

	class hkStorageSkinnedMeshShape : public hkSkinnedMeshShape
	{
	public:
		/* 0x010 */ hkArray<std::int16_t> m_bonesBuffer;
		/* 0x020 */ hkArray<hkSkinnedMeshShapeBoneSet> m_boneSets;
		/* 0x030 */ hkArray<hkSkinnedMeshShapeBoneSection> m_boneSections;
		/* 0x040 */ hkArray<hkSkinnedMeshShapePart> m_parts;
		/* 0x050 */ hkStringPtr m_name;
	};
	static_assert(sizeof(hkStorageSkinnedMeshShape) == 0x58);
	static_assert(offsetof(hkStorageSkinnedMeshShape, m_bonesBuffer) == 0x10);
	static_assert(offsetof(hkStorageSkinnedMeshShape, m_boneSets) == 0x20);
	static_assert(offsetof(hkStorageSkinnedMeshShape, m_boneSections) == 0x30);
	static_assert(offsetof(hkStorageSkinnedMeshShape, m_parts) == 0x40);
	static_assert(offsetof(hkStorageSkinnedMeshShape, m_name) == 0x50);

	class hkStringObject : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkStringPtr m_string;
	};
	static_assert(sizeof(hkStringObject) == 0x18);
	static_assert(offsetof(hkStringObject, m_string) == 0x10);

	class hkSweptTransformf
	{
	public:
		/* 0x000 */ hkVector4 m_centerOfMass0;
		/* 0x010 */ hkVector4 m_centerOfMass1;
		/* 0x020 */ hkQuaternion m_rotation0;
		/* 0x030 */ hkQuaternion m_rotation1;
		/* 0x040 */ hkVector4 m_centerOfMassLocal;
	};
	static_assert(sizeof(hkSweptTransformf) == 0x50);
	static_assert(offsetof(hkSweptTransformf, m_centerOfMass0) == 0x0);
	static_assert(offsetof(hkSweptTransformf, m_centerOfMass1) == 0x10);
	static_assert(offsetof(hkSweptTransformf, m_rotation0) == 0x20);
	static_assert(offsetof(hkSweptTransformf, m_rotation1) == 0x30);
	static_assert(offsetof(hkSweptTransformf, m_centerOfMassLocal) == 0x40);

	class hkTraceStreamTitle
	{
	public:
		/* 0x000 */ char m_value[32];
	};
	static_assert(sizeof(hkTraceStreamTitle) == 0x20);
	static_assert(offsetof(hkTraceStreamTitle, m_value) == 0x0);

	class hkUiAttribute
	{
	public:
		/* 0x000 */ hkBool m_visible;
		/* 0x001 */ hkBool m_editable;
		/* 0x002 */ std::int8_t m_hideCriteria;
		/* 0x003 */ std::byte pad003[0x5];
		/* 0x008 */ const char* m_label;
		/* 0x010 */ const char* m_group;
		/* 0x018 */ const char* m_hideBaseClassMembers;
		/* 0x020 */ hkBool m_endGroup;
		/* 0x021 */ hkBool m_endGroup2;
		/* 0x022 */ hkBool m_advanced;
		/* 0x023 */ std::byte pad023[0x5];
	};
	static_assert(sizeof(hkUiAttribute) == 0x28);
	static_assert(offsetof(hkUiAttribute, m_visible) == 0x0);
	static_assert(offsetof(hkUiAttribute, m_editable) == 0x1);
	static_assert(offsetof(hkUiAttribute, m_hideCriteria) == 0x2);
	static_assert(offsetof(hkUiAttribute, m_label) == 0x8);
	static_assert(offsetof(hkUiAttribute, m_group) == 0x10);
	static_assert(offsetof(hkUiAttribute, m_hideBaseClassMembers) == 0x18);
	static_assert(offsetof(hkUiAttribute, m_endGroup) == 0x20);
	static_assert(offsetof(hkUiAttribute, m_endGroup2) == 0x21);
	static_assert(offsetof(hkUiAttribute, m_advanced) == 0x22);

	class hkUuid
	{
	public:
		/* 0x000 */ std::uint32_t m_data[4];
	};
	static_assert(sizeof(hkUuid) == 0x10);
	static_assert(offsetof(hkUuid, m_data) == 0x0);

	class hkVariableTweakingHelper
	{
	public:
		/* 0x000 */ hkArray<hkVariableTweakingHelperBoolVariableInfo> m_boolVariableInfo;
		/* 0x010 */ hkArray<hkVariableTweakingHelperIntVariableInfo> m_intVariableInfo;
		/* 0x020 */ hkArray<hkVariableTweakingHelperRealVariableInfo> m_realVariableInfo;
		/* 0x030 */ hkArray<hkVariableTweakingHelperVector4VariableInfo> m_vector4VariableInfo;
		/* 0x040 */ void* m_behavior;
		/* 0x048 */ std::byte pad048[0x10];
		/* 0x058 */ std::byte pad058[0x10];
		/* 0x068 */ std::byte pad068[0x10];
		/* 0x078 */ std::byte pad078[0x10];
	};
	static_assert(sizeof(hkVariableTweakingHelper) == 0x88);
	static_assert(offsetof(hkVariableTweakingHelper, m_boolVariableInfo) == 0x0);
	static_assert(offsetof(hkVariableTweakingHelper, m_intVariableInfo) == 0x10);
	static_assert(offsetof(hkVariableTweakingHelper, m_realVariableInfo) == 0x20);
	static_assert(offsetof(hkVariableTweakingHelper, m_vector4VariableInfo) == 0x30);
	static_assert(offsetof(hkVariableTweakingHelper, m_behavior) == 0x40);

	class hkVariableTweakingHelperBoolVariableInfo
	{
	public:
		/* 0x000 */ hkStringPtr m_name;
		/* 0x008 */ hkBool m_value;
		/* 0x009 */ hkBool m_tweakOn;
		/* 0x00A */ std::byte pad00A[0x6];
	};
	static_assert(sizeof(hkVariableTweakingHelperBoolVariableInfo) == 0x10);
	static_assert(offsetof(hkVariableTweakingHelperBoolVariableInfo, m_name) == 0x0);
	static_assert(offsetof(hkVariableTweakingHelperBoolVariableInfo, m_value) == 0x8);
	static_assert(offsetof(hkVariableTweakingHelperBoolVariableInfo, m_tweakOn) == 0x9);

	class hkVariableTweakingHelperIntVariableInfo
	{
	public:
		/* 0x000 */ hkStringPtr m_name;
		/* 0x008 */ std::int32_t m_value;
		/* 0x00C */ hkBool m_tweakOn;
		/* 0x00D */ std::byte pad00D[0x3];
	};
	static_assert(sizeof(hkVariableTweakingHelperIntVariableInfo) == 0x10);
	static_assert(offsetof(hkVariableTweakingHelperIntVariableInfo, m_name) == 0x0);
	static_assert(offsetof(hkVariableTweakingHelperIntVariableInfo, m_value) == 0x8);
	static_assert(offsetof(hkVariableTweakingHelperIntVariableInfo, m_tweakOn) == 0xC);

	class hkVariableTweakingHelperRealVariableInfo
	{
	public:
		/* 0x000 */ hkStringPtr m_name;
		/* 0x008 */ float m_value;
		/* 0x00C */ hkBool m_tweakOn;
		/* 0x00D */ std::byte pad00D[0x3];
	};
	static_assert(sizeof(hkVariableTweakingHelperRealVariableInfo) == 0x10);
	static_assert(offsetof(hkVariableTweakingHelperRealVariableInfo, m_name) == 0x0);
	static_assert(offsetof(hkVariableTweakingHelperRealVariableInfo, m_value) == 0x8);
	static_assert(offsetof(hkVariableTweakingHelperRealVariableInfo, m_tweakOn) == 0xC);

	class hkVariableTweakingHelperVector4VariableInfo
	{
	public:
		/* 0x000 */ hkStringPtr m_name;
		/* 0x008 */ float m_x;
		/* 0x00C */ float m_y;
		/* 0x010 */ float m_z;
		/* 0x014 */ float m_w;
		/* 0x018 */ hkBool m_tweakOn;
		/* 0x019 */ std::byte pad019[0x7];
	};
	static_assert(sizeof(hkVariableTweakingHelperVector4VariableInfo) == 0x20);
	static_assert(offsetof(hkVariableTweakingHelperVector4VariableInfo, m_name) == 0x0);
	static_assert(offsetof(hkVariableTweakingHelperVector4VariableInfo, m_x) == 0x8);
	static_assert(offsetof(hkVariableTweakingHelperVector4VariableInfo, m_y) == 0xC);
	static_assert(offsetof(hkVariableTweakingHelperVector4VariableInfo, m_z) == 0x10);
	static_assert(offsetof(hkVariableTweakingHelperVector4VariableInfo, m_w) == 0x14);
	static_assert(offsetof(hkVariableTweakingHelperVector4VariableInfo, m_tweakOn) == 0x18);

	class hkWorldMemoryAvailableWatchDog : public hkReferencedObject
	{
	public:
	};
	static_assert(sizeof(hkWorldMemoryAvailableWatchDog) == 0x10);

	class hkStreamReader : public hkReferencedObject
	{
	public:
		virtual hkBool IsOk();
		virtual std::int32_t Read(void* buffer, std::int32_t numBytes);
		virtual void hkStreamReader_Unk06();
		virtual std::int32_t Peek(void* buffer, std::int32_t numBytes);
		virtual hkSeekableStreamReader* IsSeekTellSupported();
	};
	static_assert(sizeof(hkStreamReader) == 0x10);

	class hkSeekableStreamReader : public hkStreamReader
	{
	public:
		virtual void hkSeekableStreamReader_Unk09();
		virtual void hkSeekableStreamReader_Unk0A();
	};
	static_assert(sizeof(hkSeekableStreamReader) == 0x10);

}
