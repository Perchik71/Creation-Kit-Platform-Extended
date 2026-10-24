// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseObject.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkpCollidableCollidableFilter
	{
	public:
		virtual ~hkpCollidableCollidableFilter();
		virtual void sub_CollidableCollidableFilter_01();
	};
	static_assert(sizeof(hkpCollidableCollidableFilter) == 0x8);

	class hkpShapeCollectionFilter
	{
	public:
		virtual void sub_ShapeCollectionFilter_00();
		virtual void sub_ShapeCollectionFilter_01();
		virtual void sub_ShapeCollectionFilter_02();
		virtual ~hkpShapeCollectionFilter();
	};
	static_assert(sizeof(hkpShapeCollectionFilter) == 0x8);

	class hkpRayShapeCollectionFilter
	{
	public:
		virtual void sub_RayShapeCollectionFilter_00();
		virtual ~hkpRayShapeCollectionFilter();
	};
	static_assert(sizeof(hkpRayShapeCollectionFilter) == 0x8);

	class hkpRayCollidableFilter
	{
	public:
		virtual ~hkpRayCollidableFilter();
		virtual void sub_RayCollidableFilter_01();
	};
	static_assert(sizeof(hkpRayCollidableFilter) == 0x8);

	class hkpConstraintListener
	{
	public:
		virtual ~hkpConstraintListener();
		virtual void sub_ConstraintListener_01();
		virtual void sub_ConstraintListener_02();
		virtual void sub_ConstraintListener_03();
		virtual void sub_ConstraintListener_04();
		virtual void sub_ConstraintListener_05();
		virtual void sub_ConstraintListener_06();
	};
	static_assert(sizeof(hkpConstraintListener) == 0x8);

	class hkpContactListener
	{
	public:
		virtual void sub_ContactListener_00();
		virtual void sub_ContactListener_01();
		virtual void sub_ContactListener_02();
		virtual ~hkpContactListener();
		virtual void sub_ContactListener_04();
		virtual void sub_ContactListener_05();
		virtual void sub_ContactListener_06();
	};
	static_assert(sizeof(hkpContactListener) == 0x8);

	class hkpEntityActivationListener
	{
	public:
		virtual ~hkpEntityActivationListener();
		virtual void sub_EntityActivationListener_01();
		virtual void sub_EntityActivationListener_02();
	};
	static_assert(sizeof(hkpEntityActivationListener) == 0x8);

	class hkpWorldDeletionListener
	{
	public:
		virtual ~hkpWorldDeletionListener();
		virtual void sub_WorldDeletionListener_01();
		virtual void sub_WorldDeletionListener_02();
	};
	static_assert(sizeof(hkpWorldDeletionListener) == 0x8);

	class hkpWorldPostSimulationListener
	{
	public:
		virtual ~hkpWorldPostSimulationListener();
		virtual void sub_WorldPostSimulationListener_01();
		virtual void sub_WorldPostSimulationListener_02();
	};
	static_assert(sizeof(hkpWorldPostSimulationListener) == 0x8);

	class hkpPhantomOverlapListener
	{
	public:
		virtual void sub_PhantomOverlapListener_00();
		virtual void sub_PhantomOverlapListener_01();
		virtual ~hkpPhantomOverlapListener();
	};
	static_assert(sizeof(hkpPhantomOverlapListener) == 0x8);

	class hkpBroadPhaseListener
	{
	public:
		virtual ~hkpBroadPhaseListener();
		virtual void sub_BroadPhaseListener_01();
		virtual void sub_BroadPhaseListener_02();
	};
	static_assert(sizeof(hkpBroadPhaseListener) == 0x8);

	class hkpCollisionFilter : public hkReferencedObject,
		public hkpCollidableCollidableFilter, public hkpShapeCollectionFilter,
		public hkpRayShapeCollectionFilter, public hkpRayCollidableFilter
	{
	public:
		virtual void sub_04();

		/* 0x030 */ std::uint32_t m_prepad[2];
		/* 0x038 */ std::uint32_t m_type;
		/* 0x03C */ std::uint32_t m_postpad[3];
	};
	static_assert(sizeof(hkpCollisionFilter) == 0x48);
	static_assert(offsetof(hkpCollisionFilter, m_type) == 0x38);

	class hkpNullCollisionFilter : public hkpCollisionFilter {};
	static_assert(sizeof(hkpNullCollisionFilter) == 0x48);

	class hkpPairCollisionFilter : public hkpCollisionFilter
	{
	public:
		/* 0x048 */ std::uint8_t unk048[0x18];
	};
	static_assert(sizeof(hkpPairCollisionFilter) == 0x60);

	class hkpConstraintCollisionFilter : public hkpPairCollisionFilter,
		public hkpConstraintListener {};
	static_assert(sizeof(hkpConstraintCollisionFilter) == 0x68);

	class hkpConvexListFilter : public hkReferencedObject
	{
	public:
		virtual void sub_04();
	};
	static_assert(sizeof(hkpConvexListFilter) == 0x10);

	class hkpDefaultConvexListFilter : public hkpConvexListFilter {};
	static_assert(sizeof(hkpDefaultConvexListFilter) == 0x10);

	class hkpBroadPhaseBorderListener : public hkReferencedObject,
		public hkpBroadPhaseListener {};
	static_assert(sizeof(hkpBroadPhaseBorderListener) == 0x18);

	class hkpNullBroadPhaseListener : public hkReferencedObject,
		public hkpBroadPhaseListener {};
	static_assert(sizeof(hkpNullBroadPhaseListener) == 0x18);

	class hkpPhantomBroadPhaseListener : public hkReferencedObject,
		public hkpBroadPhaseListener {};
	static_assert(sizeof(hkpPhantomBroadPhaseListener) == 0x18);

	class hkpEntityEntityBroadPhaseListener : public hkReferencedObject,
		public hkpBroadPhaseListener
	{
	public:
		/* 0x018 */ std::uint8_t unk018[8];
	};
	static_assert(sizeof(hkpEntityEntityBroadPhaseListener) == 0x20);

	class hkpGroupFilter;
}
