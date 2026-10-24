// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseObject.h>
#include <EditorAPI/H/hkBaseTypes.h>
#include <EditorAPI/H/hkpCollisionFilter.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkpWorld;
	class hkpSimulationIsland;
	class hkpEntity;
	class hkpConstraintChainInstance;

	class hkpAction : public hkReferencedObject
	{
	public:
		virtual void sub_04();
		virtual void sub_05();
		virtual void sub_06();
		virtual void sub_07();
		virtual void sub_08();

		/* 0x010 */ hkpWorld* m_world;
		/* 0x018 */ hkpSimulationIsland* m_island;
		/* 0x020 */ std::uintptr_t m_userData;
		/* 0x028 */ hkStringPtr m_name;
	};
	static_assert(sizeof(hkpAction) == 0x30);
	static_assert(offsetof(hkpAction, m_world) == 0x10);
	static_assert(offsetof(hkpAction, m_userData) == 0x20);

	class hkpUnaryAction : public hkpAction
	{
	public:
		/* 0x030 */ hkpEntity* m_entity;
	};
	static_assert(sizeof(hkpUnaryAction) == 0x38);
	static_assert(offsetof(hkpUnaryAction, m_entity) == 0x30);

	class hkpBinaryAction : public hkpAction
	{
	public:
		/* 0x030 */ hkpEntity* m_entityA;
		/* 0x038 */ hkpEntity* m_entityB;
	};
	static_assert(sizeof(hkpBinaryAction) == 0x40);
	static_assert(offsetof(hkpBinaryAction, m_entityB) == 0x38);

	class hkpAngularDashpotAction : public hkpBinaryAction
	{
	public:
		/* 0x040 */ std::uint8_t unk040[0x20];
	};
	static_assert(sizeof(hkpAngularDashpotAction) == 0x60);

	class hkpDashpotAction : public hkpBinaryAction
	{
	public:
		/* 0x040 */ std::uint8_t unk040[0x40];
	};
	static_assert(sizeof(hkpDashpotAction) == 0x80);

	class hkpSpringAction : public hkpBinaryAction
	{
	public:
		/* 0x040 */ std::uint8_t unk040[0x40];
	};
	static_assert(sizeof(hkpSpringAction) == 0x80);

	class hkpMotorAction : public hkpUnaryAction
	{
	public:
		/* 0x038 */ std::uint8_t unk038[0x28];
	};
	static_assert(sizeof(hkpMotorAction) == 0x60);

	class hkpMouseSpringAction : public hkpUnaryAction
	{
	public:
		/* 0x038 */ std::uint8_t unk038[0x58];
	};
	static_assert(sizeof(hkpMouseSpringAction) == 0x90);

	class hkpConstraintChainInstanceAction : public hkpAction
	{
	public:
		/* 0x030 */ hkpConstraintChainInstance* m_constraintInstance;
	};
	static_assert(sizeof(hkpConstraintChainInstanceAction) == 0x38);
	static_assert(offsetof(hkpConstraintChainInstanceAction, m_constraintInstance) == 0x30);

	class hkpStickyContactAction : public hkpContactListener,
		public hkpEntityActivationListener, public hkpUnaryAction
	{
	public:
		/* 0x048 */ std::uint8_t unk048[0x38];
	};
	static_assert(sizeof(hkpStickyContactAction) == 0x80);
}
