// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseObject.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkpCollidableCollidableFilter
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpCollidableCollidableFilter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpCollidableCollidableFilter[0];

		virtual ~hkpCollidableCollidableFilter();
		virtual void sub_CollidableCollidableFilter_01();
	};
	static_assert(sizeof(hkpCollidableCollidableFilter) == 0x8);

	class hkpShapeCollectionFilter
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpShapeCollectionFilter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpShapeCollectionFilter[0];

		virtual void sub_ShapeCollectionFilter_00();
		virtual void sub_ShapeCollectionFilter_01();
		virtual void sub_ShapeCollectionFilter_02();
		virtual ~hkpShapeCollectionFilter();
	};
	static_assert(sizeof(hkpShapeCollectionFilter) == 0x8);

	class hkpRayShapeCollectionFilter
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpRayShapeCollectionFilter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpRayShapeCollectionFilter[0];

		virtual void sub_RayShapeCollectionFilter_00();
		virtual ~hkpRayShapeCollectionFilter();
	};
	static_assert(sizeof(hkpRayShapeCollectionFilter) == 0x8);

	class hkpRayCollidableFilter
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpRayCollidableFilter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpRayCollidableFilter[0];

		virtual ~hkpRayCollidableFilter();
		virtual void sub_RayCollidableFilter_01();
	};
	static_assert(sizeof(hkpRayCollidableFilter) == 0x8);

	class hkpConstraintListener
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpConstraintListener;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpConstraintListener[0];

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
		constexpr static Common::ID RTTI = IDs::RTTI::hkpContactListener;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpContactListener[0];

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
		constexpr static Common::ID RTTI = IDs::RTTI::hkpEntityActivationListener;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpEntityActivationListener[0];

		virtual ~hkpEntityActivationListener();
		virtual void sub_EntityActivationListener_01();
		virtual void sub_EntityActivationListener_02();
	};
	static_assert(sizeof(hkpEntityActivationListener) == 0x8);

	class hkpWorldDeletionListener
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpWorldDeletionListener;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpWorldDeletionListener[0];

		virtual ~hkpWorldDeletionListener();
		virtual void sub_WorldDeletionListener_01();
		virtual void sub_WorldDeletionListener_02();
	};
	static_assert(sizeof(hkpWorldDeletionListener) == 0x8);

	class hkpWorldPostSimulationListener
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpWorldPostSimulationListener;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpWorldPostSimulationListener[0];

		virtual ~hkpWorldPostSimulationListener();
		virtual void sub_WorldPostSimulationListener_01();
		virtual void sub_WorldPostSimulationListener_02();
	};
	static_assert(sizeof(hkpWorldPostSimulationListener) == 0x8);

	class hkpPhantomOverlapListener
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpPhantomOverlapListener;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpPhantomOverlapListener[0];

		virtual void sub_PhantomOverlapListener_00();
		virtual void sub_PhantomOverlapListener_01();
		virtual ~hkpPhantomOverlapListener();
	};
	static_assert(sizeof(hkpPhantomOverlapListener) == 0x8);

	class hkpBroadPhaseListener
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpBroadPhaseListener;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpBroadPhaseListener[0];

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
		constexpr static Common::ID RTTI = IDs::RTTI::hkpCollisionFilter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpCollisionFilter[0];

		virtual void sub_04();

		/* 0x030 */ std::uint32_t m_prepad[2];
		/* 0x038 */ std::uint32_t m_type;
		/* 0x03C */ std::uint32_t m_postpad[3];
	};
	static_assert(sizeof(hkpCollisionFilter) == 0x48);
	static_assert(offsetof(hkpCollisionFilter, m_type) == 0x38);

	class hkpNullCollisionFilter : public hkpCollisionFilter
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpNullCollisionFilter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpNullCollisionFilter[0];
	};
	static_assert(sizeof(hkpNullCollisionFilter) == 0x48);

	class hkpPairCollisionFilter : public hkpCollisionFilter
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpPairCollisionFilter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpPairCollisionFilter[0];

		/* 0x048 */ std::uint8_t unk048[0x18];
	};
	static_assert(sizeof(hkpPairCollisionFilter) == 0x60);

	class hkpConstraintCollisionFilter : public hkpPairCollisionFilter,
		public hkpConstraintListener
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkpConstraintCollisionFilter;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkpConstraintCollisionFilter[0];
		};
	static_assert(sizeof(hkpConstraintCollisionFilter) == 0x68);

	class hkpConvexListFilter : public hkReferencedObject
	{
	public:
		virtual void sub_04();
	};
	static_assert(sizeof(hkpConvexListFilter) == 0x10);

	class hkpDefaultConvexListFilter : public hkpConvexListFilter
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpDefaultConvexListFilter;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpDefaultConvexListFilter[0];
	};
	static_assert(sizeof(hkpDefaultConvexListFilter) == 0x10);

	class hkpBroadPhaseBorderListener : public hkReferencedObject,
		public hkpBroadPhaseListener
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkpBroadPhaseBorderListener;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkpBroadPhaseBorderListener[0];
		};
	static_assert(sizeof(hkpBroadPhaseBorderListener) == 0x18);

	class hkpNullBroadPhaseListener : public hkReferencedObject,
		public hkpBroadPhaseListener
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkpNullBroadPhaseListener;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkpNullBroadPhaseListener[0];
		};
	static_assert(sizeof(hkpNullBroadPhaseListener) == 0x18);

	class hkpPhantomBroadPhaseListener : public hkReferencedObject,
		public hkpBroadPhaseListener
		{
		public:
			constexpr static Common::ID RTTI = IDs::RTTI::hkpPhantomBroadPhaseListener;
			constexpr static Common::ID VTABLE = IDs::VTABLE::hkpPhantomBroadPhaseListener[0];
		};
	static_assert(sizeof(hkpPhantomBroadPhaseListener) == 0x18);

	class hkpEntityEntityBroadPhaseListener : public hkReferencedObject,
		public hkpBroadPhaseListener
	{
	public:
		constexpr static Common::ID RTTI = IDs::RTTI::hkpEntityEntityBroadPhaseListener;
		constexpr static Common::ID VTABLE = IDs::VTABLE::hkpEntityEntityBroadPhaseListener[0];

		/* 0x018 */ std::uint8_t unk018[8];
	};
	static_assert(sizeof(hkpEntityEntityBroadPhaseListener) == 0x20);

	class hkpGroupFilter;
}
