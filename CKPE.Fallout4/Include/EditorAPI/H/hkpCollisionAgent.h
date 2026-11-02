// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkpWorld.h>
#include <EditorAPI/H/hkpCollisionCollector.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkpCollisionAgent : public hkReferencedObject
	{
	public:
		virtual void getPenetrations(const void* BodyA, const void* BodyB, const void* Input, void* Collector); // 0x20 - hkpCdBodyPairCollector
		virtual void getClosestPoints(const void* BodyA, const void* BodyB, const void* Input, void* Collector); // 0x28 - hkpCdPointCollector
		virtual void linearCast(const void* BodyA, const void* BodyB, const void* Input, void* Collector, void* StartCollector); // 0x30 - shared by the hkpIterativeLinearCastAgent family
		virtual void processCollision(const void* BodyA, const void* BodyB, const void* Input, void* Result); // 0x38 - hkpProcessCollisionInput / Output
		// Removes the agent's contact points from the manager and destroys it (hkCollisionConstraintOwner argument)
		virtual void cleanup(void* ConstraintOwner); // 0x40
		virtual void invalidateTim(const void* Input); // 0x48 - default stub
		virtual void warpTime(float OldTime, float NewTime, const void* Input); // 0x50 - default stub
		virtual void updateShapeCollectionFilter(const void* BodyA, const void* BodyB, const void* Input, void* ConstraintOwner); // 0x58 - default stub
		virtual void removePoint(std::uint16_t ContactPointId); // 0x60 - default stub
		virtual void commitPotential(std::uint16_t NewId); // 0x68 - default stub
		virtual void createZombie(std::uint16_t IdToBecomeZombie); // 0x70 - default stub
		/* 0x010 */ std::uint8_t unk010[8];
	};
	static_assert(sizeof(hkpCollisionAgent) == 0x18);

	class hkpIterativeLinearCastAgent : public hkpCollisionAgent {};
	static_assert(sizeof(hkpIterativeLinearCastAgent) == 0x18);

	class hkpBoxBoxAgent : public hkpIterativeLinearCastAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpBoxBoxAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpBoxBoxAgent[0];

		/* 0x018 */ std::uint8_t unk018[0x48];
	};
	static_assert(sizeof(hkpBoxBoxAgent) == 0x60);

	class hkpBvAgent : public hkpCollisionAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpBvAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpBvAgent[0];

		/* 0x018 */ std::uint8_t unk018[0x10];
	};
	static_assert(sizeof(hkpBvAgent) == 0x28);

	class hkpBvTreeAgent : public hkpCollisionAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpBvTreeAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpBvTreeAgent[0];

		/* 0x018 */ std::uint8_t unk018[0x38];
		class LinearCastAabbCastCollector : public hkpAabbCastCollector
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkpBvTreeAgent__LinearCastAabbCastCollector;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkpBvTreeAgent__LinearCastAabbCastCollector[0];

			/* 0x020 */ std::uint8_t unk020[0x30];
		};
	};
	static_assert(sizeof(hkpBvTreeAgent) == 0x50);
	static_assert(sizeof(hkpBvTreeAgent::LinearCastAabbCastCollector) == 0x50);

	class hkpBvCompressedMeshAgent : public hkpBvTreeAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpBvCompressedMeshAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpBvCompressedMeshAgent[0];
	};
	static_assert(sizeof(hkpBvCompressedMeshAgent) == 0x50);

	class hkpBvTreeStreamAgent : public hkpCollisionAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpBvTreeStreamAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpBvTreeStreamAgent[0];

		/* 0x018 */ std::uint8_t unk018[0x38];
	};
	static_assert(sizeof(hkpBvTreeStreamAgent) == 0x50);

	class hkpMoppAgent : public hkpBvTreeAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpMoppAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpMoppAgent[0];
	};
	static_assert(sizeof(hkpMoppAgent) == 0x50);

	class hkpStaticCompoundAgent : public hkpBvTreeAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpStaticCompoundAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpStaticCompoundAgent[0];
	};
	static_assert(sizeof(hkpStaticCompoundAgent) == 0x50);

	class hkpCapsuleCapsuleAgent : public hkpIterativeLinearCastAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpCapsuleCapsuleAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpCapsuleCapsuleAgent[0];

		/* 0x018 */ std::uint8_t unk018[8];
	};
	static_assert(sizeof(hkpCapsuleCapsuleAgent) == 0x20);

	class hkpCapsuleTriangleAgent : public hkpIterativeLinearCastAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpCapsuleTriangleAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpCapsuleTriangleAgent[0];

		/* 0x018 */ std::uint8_t unk018[0x20];
	};
	static_assert(sizeof(hkpCapsuleTriangleAgent) == 0x38);

	class hkpGskBaseAgent : public hkpIterativeLinearCastAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpGskBaseAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpGskBaseAgent[0];

		/* 0x018 */ std::uint8_t unk018[0x28];
	};
	static_assert(sizeof(hkpGskBaseAgent) == 0x40);

	class hkpGskfAgent : public hkpGskBaseAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpGskfAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpGskfAgent[0];

		/* 0x040 */ std::uint8_t unk040[0x50];
	};
	static_assert(sizeof(hkpGskfAgent) == 0x90);

	class hkpPredGskfAgent : public hkpGskfAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpPredGskfAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpPredGskfAgent[0];
	};
	static_assert(sizeof(hkpPredGskfAgent) == 0x90);

	class hkpConvexListAgent : public hkpPredGskfAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpConvexListAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpConvexListAgent[0];

		/* 0x090 */ std::uint8_t unk090[0x10];
	};
	static_assert(sizeof(hkpConvexListAgent) == 0xA0);

	class hkpListAgent : public hkpCollisionAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpListAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpListAgent[0];

		/* 0x018 */ std::uint8_t unk018[0x18];
	};
	static_assert(sizeof(hkpListAgent) == 0x30);

	class hkpMultiSphereAgent : public hkpCollisionAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpMultiSphereAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpMultiSphereAgent[0];

		/* 0x018 */ std::uint8_t unk018[0x50];
	};
	static_assert(sizeof(hkpMultiSphereAgent) == 0x68);

	class hkpMultiSphereTriangleAgent : public hkpIterativeLinearCastAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpMultiSphereTriangleAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpMultiSphereTriangleAgent[0];

		/* 0x018 */ std::uint8_t unk018[0x20];
	};
	static_assert(sizeof(hkpMultiSphereTriangleAgent) == 0x38);

	class hkpNullAgent : public hkpCollisionAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpNullAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpNullAgent[0];
	};
	static_assert(sizeof(hkpNullAgent) == 0x18);

	class hkpPhantomAgent : public hkpCollisionAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpPhantomAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpPhantomAgent[0];

		/* 0x018 */ std::uint8_t unk018[0x28];
	};
	static_assert(sizeof(hkpPhantomAgent) == 0x40);

	class hkpShapeCollectionAgent : public hkpCollisionAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpShapeCollectionAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpShapeCollectionAgent[0];

		/* 0x018 */ std::uint8_t unk018[0x50];
	};
	static_assert(sizeof(hkpShapeCollectionAgent) == 0x68);

	class hkpSphereBoxAgent : public hkpIterativeLinearCastAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpSphereBoxAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpSphereBoxAgent[0];

		/* 0x018 */ std::uint8_t unk018[8];
	};
	static_assert(sizeof(hkpSphereBoxAgent) == 0x20);

	class hkpSphereCapsuleAgent : public hkpIterativeLinearCastAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpSphereCapsuleAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpSphereCapsuleAgent[0];

		/* 0x018 */ std::uint8_t unk018[8];
	};
	static_assert(sizeof(hkpSphereCapsuleAgent) == 0x20);

	class hkpSphereSphereAgent : public hkpIterativeLinearCastAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpSphereSphereAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpSphereSphereAgent[0];

		/* 0x018 */ std::uint8_t unk018[8];
	};
	static_assert(sizeof(hkpSphereSphereAgent) == 0x20);

	class hkpSphereTriangleAgent : public hkpIterativeLinearCastAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpSphereTriangleAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpSphereTriangleAgent[0];

		/* 0x018 */ std::uint8_t unk018[0x18];
	};
	static_assert(sizeof(hkpSphereTriangleAgent) == 0x30);

	class hkpTransformAgent : public hkpCollisionAgent
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpTransformAgent;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpTransformAgent[0];

		/* 0x018 */ std::uint8_t unk018[8];
	};
	static_assert(sizeof(hkpTransformAgent) == 0x20);

	class hkpContactMgr : public hkReferencedObject
	{
	public:
		// Contact point ids are 16 bit (hkContactPointId); hkResult / hkBool returns go through a hidden pointer
		virtual std::uint16_t addContactPoint(const void* BodyA, const void* BodyB, const void* Input, void* Output, const void* ContactCache, void* ContactPoint); // 0x20
		virtual std::int32_t* reserveContactPoints(std::int32_t* HkResult, std::int32_t NumPoints); // 0x28
		virtual void removeContactPoint(std::uint16_t ContactPointId, void* ConstraintOwner); // 0x30
		virtual void processContact(const void* BodyA, const void* BodyB, const void* Input, void* Output, void* ContactData); // 0x38
		virtual void addToi(const void* BodyA, const void* BodyB, const void* Input, void* Output, void* Toi); // 0x40
		virtual void removeToi(void* ConstraintOwner, void* Toi); // 0x48
		// Invokes the deleting destructor of the manager
		virtual void deleteThis(); // 0x50
		/* 0x010 */ std::uint8_t unk010[8];
	};
	static_assert(sizeof(hkpContactMgr) == 0x18);

	class hkpContactMgrFactory : public hkReferencedObject
	{
	public:
		virtual void sub_04();
	};
	static_assert(sizeof(hkpContactMgrFactory) == 0x10);

	class hkpDynamicsContactMgr : public hkpContactMgr
	{
	public:
		virtual void* getContactPointProperties(std::uint16_t ContactPointId); // 0x58 - null when the id is unused
		virtual void* getContactPoint(std::uint16_t ContactPointId); // 0x60
		virtual void getAllContactPointIds(void* IdArray); // 0x68
		virtual void sub_0E();
		virtual void sub_0F();
		virtual void sub_10();
		virtual void sub_11();
		virtual void sub_12();
		virtual void sub_13();
		/* 0x018 */ std::uint8_t unk018[8];
	};
	static_assert(sizeof(hkpDynamicsContactMgr) == 0x20);

	class hkpSimpleConstraintContactMgr : public hkpDynamicsContactMgr
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpSimpleConstraintContactMgr;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpSimpleConstraintContactMgr[0];

		virtual void sub_14();
		/* 0x020 */ std::uint8_t unk020[0xD0];

		class Factory : public hkpContactMgrFactory
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkpSimpleConstraintContactMgr__Factory;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkpSimpleConstraintContactMgr__Factory[0];

			/* 0x010 */ std::uint8_t unk010[8];
		};
	};
	static_assert(sizeof(hkpSimpleConstraintContactMgr) == 0xF0);
	static_assert(sizeof(hkpSimpleConstraintContactMgr::Factory) == 0x18);

	class hkpReportContactMgr : public hkpDynamicsContactMgr
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpReportContactMgr;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpReportContactMgr[0];

		/* 0x020 */ std::uint8_t unk020[0x18];

		class Factory : public hkpContactMgrFactory
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkpReportContactMgr__Factory;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkpReportContactMgr__Factory[0];

			/* 0x010 */ std::uint8_t unk010[8];
		};
	};
	static_assert(sizeof(hkpReportContactMgr) == 0x38);
	static_assert(sizeof(hkpReportContactMgr::Factory) == 0x18);

	class hkpNullContactMgr : public hkpContactMgr
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpNullContactMgr;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpNullContactMgr[0];
	};
	static_assert(sizeof(hkpNullContactMgr) == 0x18);

	class hkpNullContactMgrFactory : public hkpContactMgrFactory
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpNullContactMgrFactory;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpNullContactMgrFactory[0];

		/* 0x010 */ std::uint8_t unk010[0x18];
	};
	static_assert(sizeof(hkpNullContactMgrFactory) == 0x28);

	class hkpMapPointsToSubShapeContactMgr : public hkpContactMgr
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpMapPointsToSubShapeContactMgr;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpMapPointsToSubShapeContactMgr[0];

		/* 0x018 */ std::uint8_t unk018[0x10];
	};
	static_assert(sizeof(hkpMapPointsToSubShapeContactMgr) == 0x28);

	class hkpCollisionDispatcher : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpCollisionDispatcher;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpCollisionDispatcher[0];

		/* 0x010 */ std::uint8_t unk010[0x2A70];
	};
	static_assert(sizeof(hkpCollisionDispatcher) == 0x2A80);

	class hkpBroadPhase : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpBroadPhase;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpBroadPhase[0];

		virtual void getPenetrations(const void* BodyA, const void* BodyB, const void* Input, void* Collector); // 0x20 - hkpCdBodyPairCollector
		virtual void getClosestPoints(const void* BodyA, const void* BodyB, const void* Input, void* Collector); // 0x28 - hkpCdPointCollector
		virtual void linearCast(const void* BodyA, const void* BodyB, const void* Input, void* Collector, void* StartCollector); // 0x30 - shared by the hkpIterativeLinearCastAgent family
		virtual void processCollision(const void* BodyA, const void* BodyB, const void* Input, void* Result); // 0x38 - hkpProcessCollisionInput / Output
		// Removes the agent's contact points from the manager and destroys it (hkCollisionConstraintOwner argument)
		virtual void cleanup(void* ConstraintOwner); // 0x40
		virtual void invalidateTim(const void* Input); // 0x48 - default stub
		virtual void warpTime(float OldTime, float NewTime, const void* Input); // 0x50 - default stub
		virtual void updateShapeCollectionFilter(const void* BodyA, const void* BodyB, const void* Input, void* ConstraintOwner); // 0x58 - default stub
		virtual void removePoint(std::uint16_t ContactPointId); // 0x60 - default stub
		virtual void commitPotential(std::uint16_t NewId); // 0x68 - default stub
		virtual void createZombie(std::uint16_t IdToBecomeZombie); // 0x70 - default stub
		virtual void sub_0F();
		virtual void sub_10();
		virtual void sub_11();
		virtual void sub_12();
		virtual void sub_13();
		virtual void sub_14();
		virtual void sub_15();
		virtual void sub_16();
		virtual void sub_17();
		virtual void sub_18();
		virtual void sub_19();
		virtual void sub_1A();
		virtual void sub_1B();
		virtual void sub_1C();
		virtual void sub_1D();
		virtual void sub_1E();
		virtual void sub_1F();
		/* 0x010 */ std::uint8_t unk010[0x20];
	};
	static_assert(sizeof(hkpBroadPhase) == 0x30);

	class hkpSimulation : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpSimulation;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpSimulation[0];

		virtual void getPenetrations(const void* BodyA, const void* BodyB, const void* Input, void* Collector); // 0x20 - hkpCdBodyPairCollector
		virtual void getClosestPoints(const void* BodyA, const void* BodyB, const void* Input, void* Collector); // 0x28 - hkpCdPointCollector
		virtual void linearCast(const void* BodyA, const void* BodyB, const void* Input, void* Collector, void* StartCollector); // 0x30 - shared by the hkpIterativeLinearCastAgent family
		virtual void processCollision(const void* BodyA, const void* BodyB, const void* Input, void* Result); // 0x38 - hkpProcessCollisionInput / Output
		// Removes the agent's contact points from the manager and destroys it (hkCollisionConstraintOwner argument)
		virtual void cleanup(void* ConstraintOwner); // 0x40
		virtual void invalidateTim(const void* Input); // 0x48 - default stub
		virtual void warpTime(float OldTime, float NewTime, const void* Input); // 0x50 - default stub
		virtual void updateShapeCollectionFilter(const void* BodyA, const void* BodyB, const void* Input, void* ConstraintOwner); // 0x58 - default stub
		virtual void removePoint(std::uint16_t ContactPointId); // 0x60 - default stub
		virtual void commitPotential(std::uint16_t NewId); // 0x68 - default stub
		virtual void createZombie(std::uint16_t IdToBecomeZombie); // 0x70 - default stub
		virtual void sub_0F();
		virtual void sub_10();
		virtual void sub_11();
		virtual void sub_12();
		virtual void sub_13();
		/* 0x010 */ std::uint8_t unk010[0x30];
	};
	static_assert(sizeof(hkpSimulation) == 0x40);

	class hkpBroadPhaseBorder : public hkReferencedObject,
		public hkpWorldDeletionListener, public hkpPhantomOverlapListener,
		public hkpWorldPostSimulationListener
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpBroadPhaseBorder;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpBroadPhaseBorder[0];

		virtual void sub_04();
		virtual void sub_05();
		/* 0x028 */ std::uint8_t unk028[0x50];
	};
	static_assert(sizeof(hkpBroadPhaseBorder) == 0x78);

	class hkpCollisionCallbackUtil : public hkpWorldExtension,
		public hkpConstraintListener
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpCollisionCallbackUtil;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpCollisionCallbackUtil[0];

		/* 0x028 */ std::uint8_t unk028[0x50];
	};
	static_assert(sizeof(hkpCollisionCallbackUtil) == 0x78);

	class hkpEndOfStepCallbackUtil : public hkReferencedObject,
		public hkpWorldPostSimulationListener
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpEndOfStepCallbackUtil;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpEndOfStepCallbackUtil[0];

		/* 0x018 */ std::uint8_t unk018[0x38];
	};
	static_assert(sizeof(hkpEndOfStepCallbackUtil) == 0x50);

	class hkpVehicleFrictionDescription : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpVehicleFrictionDescription;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpVehicleFrictionDescription[0];

		/* 0x010 */ std::uint8_t unk010[0xD0];
	};
	static_assert(sizeof(hkpVehicleFrictionDescription) == 0xE0);

	class hkpStickContactParam : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpStickContactParam;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpStickContactParam[0];

		/* 0x010 */ std::uint8_t unk010[0x18];
	};
	static_assert(sizeof(hkpStickContactParam) == 0x28);

	class hkpSerializedAgentNnEntry : public hkReferencedObject
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpSerializedAgentNnEntry;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpSerializedAgentNnEntry[0];

		/* 0x010 */ std::uint8_t unk010[0x180];
	};
	static_assert(sizeof(hkpSerializedAgentNnEntry) == 0x190);
}
