// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkpShape.h>
#include <EditorAPI/H/hkpMotion.h>
#include <EditorAPI/H/hkpConstraintData.h>
#include <EditorAPI/H/hkpCollisionFilter.h>
#include <EditorAPI/H/hkpAction.h>
#include <EditorAPI/H/hkReflectionTypes.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkpWorld;
	class hkpEntity;
	class hkpRigidBody;
	class hkpPhantom;
	class hkpConstraintInstance;
	class hkpSimulationIsland;
	class hkpConstraintOwner;
	class hkpModifierConstraintAtom;

	struct hkpMaterial
	{
		/* 0x000 */ std::int8_t m_responseType;
		/* 0x001 */ std::uint8_t pad001;
		/* 0x002 */ hkHalf m_rollingFrictionMultiplier;
		/* 0x004 */ float m_friction;
		/* 0x008 */ float m_restitution;
	};
	static_assert(sizeof(hkpMaterial) == 0xC);
	static_assert(offsetof(hkpMaterial, m_friction) == 0x4);

	struct hkpBroadPhaseHandle
	{
		/* 0x000 */ std::uint32_t m_id;
	};
	static_assert(sizeof(hkpBroadPhaseHandle) == 0x4);

	struct hkpTypedBroadPhaseHandle : public hkpBroadPhaseHandle
	{
		/* 0x004 */ std::int8_t m_type;
		/* 0x005 */ std::int8_t m_ownerOffset;
		/* 0x006 */ std::int8_t m_objectQualityType;
		/* 0x007 */ std::uint8_t pad007;
		/* 0x008 */ std::uint32_t m_collisionFilterInfo;
	};
	static_assert(sizeof(hkpTypedBroadPhaseHandle) == 0xC);
	static_assert(offsetof(hkpTypedBroadPhaseHandle, m_collisionFilterInfo) == 0x8);

	class hkpCdBody
	{
	public:
		/* 0x000 */ const hkpShape* m_shape;
		/* 0x008 */ std::uint32_t m_shapeKey;
		/* 0x00C */ std::uint8_t pad00C[4];
		/* 0x010 */ const void* m_motion;
		/* 0x018 */ const hkpCdBody* m_parent;
	};
	static_assert(sizeof(hkpCdBody) == 0x20);
	static_assert(offsetof(hkpCdBody, m_motion) == 0x10);

	class hkpCollidable : public hkpCdBody
	{
	public:
		struct BoundingVolumeData
		{
			/* 0x000 */ std::uint32_t m_min[3];
			/* 0x00C */ std::uint8_t m_expansionMin[3];
			/* 0x00F */ std::uint8_t m_expansionShift;
			/* 0x010 */ std::uint32_t m_max[3];
			/* 0x01C */ std::uint8_t m_expansionMax[3];
			/* 0x01F */ std::uint8_t m_padding;
			/* 0x020 */ std::uint16_t m_numChildShapeAabbs;
			/* 0x022 */ std::uint16_t m_capacityChildShapeAabbs;
			/* 0x024 */ std::uint8_t pad024[4];
			/* 0x028 */ void* m_childShapeAabbs;
			/* 0x030 */ std::uint32_t* m_childShapeKeys;
		};

		/* 0x020 */ std::int8_t m_ownerOffset;
		/* 0x021 */ std::uint8_t m_forceCollideOntoPpu;
		/* 0x022 */ std::uint16_t m_shapeSizeOnSpu;
		/* 0x024 */ hkpTypedBroadPhaseHandle m_broadPhaseHandle;
		/* 0x030 */ BoundingVolumeData m_boundingVolumeData;
		/* 0x068 */ float m_allowedPenetrationDepth;
		/* 0x06C */ std::uint8_t pad06C[4];
	};
	static_assert(sizeof(hkpCollidable::BoundingVolumeData) == 0x38);
	static_assert(sizeof(hkpCollidable) == 0x70);
	static_assert(offsetof(hkpCollidable, m_broadPhaseHandle) == 0x24);
	static_assert(offsetof(hkpCollidable, m_boundingVolumeData) == 0x30);
	static_assert(offsetof(hkpCollidable, m_allowedPenetrationDepth) == 0x68);

	class hkpLinkedCollidable : public hkpCollidable
	{
	public:
		/* 0x070 */ hkArray<void> m_collisionEntries;
	};
	static_assert(sizeof(hkpLinkedCollidable) == 0x80);
	static_assert(offsetof(hkpLinkedCollidable, m_collisionEntries) == 0x70);

	class hkpWorldObject : public hkReferencedObject
	{
	public:
		virtual void sub_04();
		virtual void sub_05();
		virtual void sub_06();

		/* 0x010 */ hkpWorld* m_world;
		/* 0x018 */ std::uintptr_t m_userData;
		/* 0x020 */ hkpLinkedCollidable m_collidable;
		/* 0x0A0 */ hkMultiThreadCheck m_multiThreadCheck;
		/* 0x0AC */ std::uint8_t pad0AC[4];
		/* 0x0B0 */ hkStringPtr m_name;
		/* 0x0B8 */ hkArray<hkSimpleProperty> m_properties;
	};
	static_assert(sizeof(hkpWorldObject) == 0xC8);
	static_assert(offsetof(hkpWorldObject, m_world) == 0x10);
	static_assert(offsetof(hkpWorldObject, m_collidable) == 0x20);
	static_assert(offsetof(hkpWorldObject, m_properties) == 0xB8);

	class alignas(16) hkpEntity : public hkpWorldObject
	{
	public:
		virtual void sub_07();

		struct SpuCollisionCallback
		{
			/* 0x000 */ void* m_util;
			/* 0x008 */ std::uint16_t m_capacity;
			/* 0x00A */ std::uint8_t m_eventFilter;
			/* 0x00B */ std::uint8_t m_userFilter;
			/* 0x00C */ std::uint8_t pad00C[4];
		};

		class ExtendedListeners
		{
		public:
			/* 0x000 */ hkSmallArray<hkpEntityActivationListener*> m_activationListeners;
			/* 0x010 */ hkSmallArray<void*> m_entityListeners;
		};
		/* 0x0C8 */ hkpMaterial m_material;
		/* 0x0D4 */ std::uint8_t pad0D4[4];
		/* 0x0D8 */ void* m_limitContactImpulseUtilAndFlag;
		/* 0x0E0 */ float m_damageMultiplier;
		/* 0x0E4 */ std::uint8_t pad0E4[4];
		/* 0x0E8 */ void* m_breakableBody;
		/* 0x0F0 */ std::uint32_t m_solverData;
		/* 0x0F4 */ std::uint16_t m_storageIndex;
		/* 0x0F6 */ std::uint16_t m_contactPointCallbackDelay;
		/* 0x0F8 */ hkSmallArray<void> m_constraintsMaster;
		/* 0x108 */ hkArray<hkpConstraintInstance*> m_constraintsSlave;
		/* 0x118 */ hkArray<std::uint8_t> m_constraintRuntime;
		/* 0x128 */ hkpSimulationIsland* m_simulationIsland;
		/* 0x130 */ std::int8_t m_autoRemoveLevel;
		/* 0x131 */ std::uint8_t m_numShapeKeysInContactPointProperties;
		/* 0x132 */ std::uint8_t m_responseModifierFlags;
		/* 0x133 */ std::uint8_t pad133;
		/* 0x134 */ std::uint32_t m_uid;
		/* 0x138 */ SpuCollisionCallback m_spuCollisionCallback;
		/* 0x148 */ std::uint8_t pad148[8];
		/* 0x150 */ hkpMaxSizeMotion m_motion;
		/* 0x290 */ hkSmallArray<hkpContactListener*> m_contactListeners;
		/* 0x2A0 */ hkSmallArray<hkpAction*> m_actions;
		/* 0x2B0 */ hkLocalFrame* m_localFrame;
		/* 0x2B8 */ ExtendedListeners* m_extendedListeners;
		/* 0x2C0 */ std::uint32_t m_npData;
		/* 0x2C4 */ std::uint8_t pad2C4[0xC];
	};
	static_assert(sizeof(hkpEntity) == 0x2D0);
	static_assert(sizeof(hkpEntity::SpuCollisionCallback) == 0x10);
	static_assert(sizeof(hkpEntity::ExtendedListeners) == 0x20);
	static_assert(offsetof(hkpEntity, m_material) == 0xC8);
	static_assert(offsetof(hkpEntity, m_motion) == 0x150);
	static_assert(offsetof(hkpEntity, m_localFrame) == 0x2B0);

	class hkpRigidBody : public hkpEntity
	{
	public:
		virtual void sub_08();
	};
	static_assert(sizeof(hkpRigidBody) == 0x2D0);

	class hkpPhantom : public hkpWorldObject
	{
	public:
		virtual void sub_07();
		virtual void sub_08();
		virtual void sub_09();
		virtual void sub_0A();
		virtual void sub_0B();
		virtual void sub_0C();
		virtual void sub_0D();
		virtual void sub_0E();
		virtual void sub_0F();
		/* 0x0C8 */ hkArray<hkpPhantomOverlapListener*> m_overlapListeners;
		/* 0x0D8 */ hkArray<void*> m_phantomListeners;
	};
	static_assert(sizeof(hkpPhantom) == 0xE8);
	static_assert(offsetof(hkpPhantom, m_overlapListeners) == 0xC8);

	class alignas(16) hkpAabbPhantom : public hkpPhantom
	{
	public:
		/* 0x0E8 */ std::uint8_t unk0E8[0x48];
	};
	static_assert(sizeof(hkpAabbPhantom) == 0x130);

	class alignas(16) hkpShapePhantom : public hkpPhantom
	{
	public:
		virtual void sub_10();
		virtual void sub_11();
		virtual void sub_12();
		virtual void sub_13();
		/* 0x0E8 */ std::uint8_t unk0E8[0xB8];
	};
	static_assert(sizeof(hkpShapePhantom) == 0x1A0);

	class hkpCachingShapePhantom : public hkpShapePhantom
	{
	public:
		/* 0x1A0 */ std::uint8_t unk1A0[0x20];
	};
	static_assert(sizeof(hkpCachingShapePhantom) == 0x1C0);

	class hkpSimpleShapePhantom : public hkpShapePhantom
	{
	public:
		/* 0x1A0 */ std::uint8_t unk1A0[0x20];
	};
	static_assert(sizeof(hkpSimpleShapePhantom) == 0x1C0);

	class hkpConstraintOwner : public hkReferencedObject
	{
	public:
		virtual void sub_04();
		virtual void sub_05();
		virtual void sub_06();
		virtual void sub_07();
		/* 0x010 */ std::uint8_t unk010[0x10];
	};
	static_assert(sizeof(hkpConstraintOwner) == 0x20);

	class hkpConstraintInstance : public hkReferencedObject
	{
	public:
		virtual void sub_04();
		virtual void sub_05();
		virtual void sub_06();
		virtual void sub_07();

		/* 0x010 */ hkpConstraintOwner* m_owner;
		/* 0x018 */ hkpConstraintData* m_data;
		/* 0x020 */ hkpModifierConstraintAtom* m_constraintModifiers;
		/* 0x028 */ hkpEntity* m_entities[2];
		/* 0x038 */ std::uint8_t m_priority;
		/* 0x039 */ hkBool m_wantRuntime;
		/* 0x03A */ std::uint8_t m_destructionRemapInfo;
		/* 0x03B */ std::uint8_t pad03B[5];
		/* 0x040 */ hkSmallArray<hkpConstraintListener*> m_listeners;
		/* 0x050 */ hkStringPtr m_name;
		/* 0x058 */ std::uintptr_t m_userData;
		/* 0x060 */ void* m_internal;
		/* 0x068 */ std::uint32_t m_uid;
		/* 0x06C */ std::uint8_t pad06C[4];
	};
	static_assert(sizeof(hkpConstraintInstance) == 0x70);
	static_assert(offsetof(hkpConstraintInstance, m_data) == 0x18);
	static_assert(offsetof(hkpConstraintInstance, m_entities) == 0x28);
	static_assert(offsetof(hkpConstraintInstance, m_userData) == 0x58);

	class hkpConstraintChainInstance : public hkpConstraintInstance
	{
	public:
		/* 0x070 */ std::uint8_t unk070[0x20];
	};
	static_assert(sizeof(hkpConstraintChainInstance) == 0x90);

	class hkpSimulationIsland : public hkpConstraintOwner
	{
	public:
		/* 0x020 */ std::uint8_t unk020[0x98];
	};
	static_assert(sizeof(hkpSimulationIsland) == 0xB8);

	class hkpWorld : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint8_t unk010[0x430];
	};
	static_assert(sizeof(hkpWorld) == 0x440);

	class alignas(16) hkpWorldCinfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkVector4 m_gravity;
		/* 0x020 */ std::int32_t m_broadPhaseQuerySize;
		/* 0x024 */ float m_contactRestingVelocity;
		/* 0x028 */ std::int8_t m_broadPhaseType;
		/* 0x029 */ std::int8_t m_broadPhaseBorderBehaviour;
		/* 0x02A */ hkBool m_mtPostponeAndSortBroadPhaseBorderCallbacks;
		/* 0x02B */ std::uint8_t pad02B[5];
		/* 0x030 */ hkAabb m_broadPhaseWorldAabb;
		/* 0x050 */ float m_collisionTolerance;
		/* 0x054 */ std::uint8_t pad054[4];
		/* 0x058 */ hkpCollisionFilter* m_collisionFilter;
		/* 0x060 */ hkpConvexListFilter* m_convexListFilter;
		/* 0x068 */ float m_expectedMaxLinearVelocity;
		/* 0x06C */ std::int32_t m_sizeOfToiEventQueue;
		/* 0x070 */ float m_expectedMinPsiDeltaTime;
		/* 0x074 */ std::uint8_t pad074[4];
		/* 0x078 */ hkWorldMemoryAvailableWatchDog* m_memoryWatchDog;
		/* 0x080 */ std::int32_t m_broadPhaseNumMarkers;
		/* 0x084 */ std::int8_t m_contactPointGeneration;
		/* 0x085 */ hkBool m_allowToSkipConfirmedCallbacks;
		/* 0x086 */ std::uint8_t pad086[2];
		/* 0x088 */ float m_solverTau;
		/* 0x08C */ float m_solverDamp;
		/* 0x090 */ std::int32_t m_solverIterations;
		/* 0x094 */ std::int32_t m_solverMicrosteps;
		/* 0x098 */ float m_maxConstraintViolation;
		/* 0x09C */ hkBool m_forceCoherentConstraintOrderingInSolver;
		/* 0x09D */ std::uint8_t pad09D[3];
		/* 0x0A0 */ float m_snapCollisionToConvexEdgeThreshold;
		/* 0x0A4 */ float m_snapCollisionToConcaveEdgeThreshold;
		/* 0x0A8 */ hkBool m_enableToiWeldRejection;
		/* 0x0A9 */ hkBool m_enableDeprecatedWelding;
		/* 0x0AA */ std::uint8_t pad0AA[2];
		/* 0x0AC */ float m_iterativeLinearCastEarlyOutDistance;
		/* 0x0B0 */ std::int32_t m_iterativeLinearCastMaxIterations;
		/* 0x0B4 */ std::uint8_t m_deactivationNumInactiveFramesSelectFlag0;
		/* 0x0B5 */ std::uint8_t m_deactivationNumInactiveFramesSelectFlag1;
		/* 0x0B6 */ std::uint8_t m_deactivationIntegrateCounter;
		/* 0x0B7 */ hkBool m_shouldActivateOnRigidBodyTransformChange;
		/* 0x0B8 */ float m_deactivationReferenceDistance;
		/* 0x0BC */ float m_toiCollisionResponseRotateNormal;
		/* 0x0C0 */ hkBool m_useCompoundSpuElf;
		/* 0x0C1 */ std::uint8_t pad0C1[3];
		/* 0x0C4 */ std::int32_t m_maxSectorsPerMidphaseCollideTask;
		/* 0x0C8 */ std::int32_t m_maxSectorsPerNarrowphaseCollideTask;
		/* 0x0CC */ hkBool m_processToisMultithreaded;
		/* 0x0CD */ std::uint8_t pad0CD[3];
		/* 0x0D0 */ std::int32_t m_maxEntriesPerToiMidphaseCollideTask;
		/* 0x0D4 */ std::int32_t m_maxEntriesPerToiNarrowphaseCollideTask;
		/* 0x0D8 */ std::int32_t m_maxNumToiCollisionPairsSinglethreaded;
		/* 0x0DC */ float m_numToisTillAllowedPenetrationSimplifiedToi;
		/* 0x0E0 */ float m_numToisTillAllowedPenetrationToi;
		/* 0x0E4 */ float m_numToisTillAllowedPenetrationToiHigher;
		/* 0x0E8 */ float m_numToisTillAllowedPenetrationToiForced;
		/* 0x0EC */ hkBool m_enableDeactivation;
		/* 0x0ED */ std::int8_t m_simulationType;
		/* 0x0EE */ hkBool m_enableSimulationIslands;
		/* 0x0EF */ std::uint8_t pad0EF;
		/* 0x0F0 */ std::uint32_t m_minDesiredIslandSize;
		/* 0x0F4 */ hkBool m_processActionsInSingleThread;
		/* 0x0F5 */ hkBool m_allowIntegrationOfIslandsWithoutConstraintsInASeparateJob;
		/* 0x0F6 */ std::uint8_t pad0F6[2];
		/* 0x0F8 */ float m_frameMarkerPsiSnap;
		/* 0x0FC */ hkBool m_fireCollisionCallbacks;
		/* 0x0FD */ std::uint8_t pad0FD[3];
	};
	static_assert(sizeof(hkpWorldCinfo) == 0x100);
	static_assert(offsetof(hkpWorldCinfo, m_gravity) == 0x10);
	static_assert(offsetof(hkpWorldCinfo, m_collisionFilter) == 0x58);
	static_assert(offsetof(hkpWorldCinfo, m_solverIterations) == 0x90);

	class hkpPhysicsSystem : public hkReferencedObject
	{
	public:
		virtual void sub_04();
		virtual void sub_05();
		/* 0x010 */ hkArray<hkpRigidBody*> m_rigidBodies;
		/* 0x020 */ hkArray<hkpConstraintInstance*> m_constraints;
		/* 0x030 */ hkArray<hkpAction*> m_actions;
		/* 0x040 */ hkArray<hkpPhantom*> m_phantoms;
		/* 0x050 */ hkStringPtr m_name;
		/* 0x058 */ std::uintptr_t m_userData;
		/* 0x060 */ hkBool m_active;
		/* 0x061 */ std::uint8_t pad061[7];
	};
	static_assert(sizeof(hkpPhysicsSystem) == 0x68);
	static_assert(offsetof(hkpPhysicsSystem, m_constraints) == 0x20);
	static_assert(offsetof(hkpPhysicsSystem, m_phantoms) == 0x40);

	class hkpPhysicsSystemWithContacts : public hkpPhysicsSystem
	{
	public:
		/* 0x068 */ std::uint8_t unk068[0x10];
	};
	static_assert(sizeof(hkpPhysicsSystemWithContacts) == 0x78);

	class hkpPhysicsData : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint8_t unk010[0x18];
	};
	static_assert(sizeof(hkpPhysicsData) == 0x28);

	class hkpWorldExtension : public hkReferencedObject
	{
	public:
		virtual void sub_04();
		virtual void sub_05();
		/* 0x010 */ std::uint8_t unk010[0x10];
	};
	static_assert(sizeof(hkpWorldExtension) == 0x20);

	class hkpWorldMaintenanceMgr : public hkReferencedObject
	{
	public:
		virtual void sub_04();
		virtual void sub_05();
		virtual void sub_06();
	};
	static_assert(sizeof(hkpWorldMaintenanceMgr) == 0x10);

	class hkpDefaultWorldMaintenanceMgr : public hkpWorldMaintenanceMgr
	{
	public:
		/* 0x010 */ std::uint8_t unk010[8];
	};
	static_assert(sizeof(hkpDefaultWorldMaintenanceMgr) == 0x18);

	class hkpCharacterProxy;
	class hkpCharacterRigidBody;
	class hkpCharacterContext;
	class hkpCharacterState;
	class hkpProperty;
}
