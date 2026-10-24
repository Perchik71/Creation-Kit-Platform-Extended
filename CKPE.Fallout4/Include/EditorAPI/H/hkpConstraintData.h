// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseObject.h>
#include <EditorAPI/H/hkBaseTypes.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkpConstraintData : public hkReferencedObject
	{
	public:
		virtual void sub_04();
		virtual void sub_05();
		virtual void sub_06();
		virtual void sub_07();
		virtual void sub_08();
		virtual void sub_09();
		virtual void sub_0A();
		virtual void sub_0B();
		virtual void sub_0C();
		virtual void sub_0D();
		virtual void sub_0E();
		virtual void sub_0F();
		virtual void sub_10();
		virtual void sub_11();
		virtual void sub_12();
		virtual void sub_13();
		virtual void sub_14();
		virtual void sub_15();
		virtual void sub_16();
		virtual void sub_17();

		/* 0x010 */ std::uintptr_t m_userData;
	};
	static_assert(sizeof(hkpConstraintData) == 0x18);
	static_assert(offsetof(hkpConstraintData, m_userData) == 0x10);

	class hkpConstraintMotor : public hkReferencedObject
	{
	public:
		virtual void sub_04();
		/* 0x010 */ std::int8_t m_type;
		/* 0x011 */ std::uint8_t pad011[7];
	};
	static_assert(sizeof(hkpConstraintMotor) == 0x18);
	static_assert(offsetof(hkpConstraintMotor, m_type) == 0x10);

	class hkpLimitedForceConstraintMotor : public hkpConstraintMotor
	{
	public:
		/* 0x018 */ float m_minForce;
		/* 0x01C */ float m_maxForce;
	};
	static_assert(sizeof(hkpLimitedForceConstraintMotor) == 0x20);
	static_assert(offsetof(hkpLimitedForceConstraintMotor, m_minForce) == 0x18);

	class hkpPositionConstraintMotor : public hkpLimitedForceConstraintMotor
	{
	public:
		/* 0x020 */ float m_tau;
		/* 0x024 */ float m_damping;
		/* 0x028 */ float m_proportionalRecoveryVelocity;
		/* 0x02C */ float m_constantRecoveryVelocity;
	};
	static_assert(sizeof(hkpPositionConstraintMotor) == 0x30);
	static_assert(offsetof(hkpPositionConstraintMotor, m_tau) == 0x20);

	class hkpVelocityConstraintMotor : public hkpLimitedForceConstraintMotor
	{
	public:
		/* 0x020 */ float m_tau;
		/* 0x024 */ float m_velocityTarget;
		/* 0x028 */ hkBool m_useVelocityTargetFromConstraintTargets;
		/* 0x029 */ std::uint8_t pad029[7];
	};
	static_assert(sizeof(hkpVelocityConstraintMotor) == 0x30);
	static_assert(offsetof(hkpVelocityConstraintMotor, m_velocityTarget) == 0x24);

	class hkpSpringDamperConstraintMotor : public hkpLimitedForceConstraintMotor
	{
	public:
		/* 0x020 */ float m_springConstant;
		/* 0x024 */ float m_springDamping;
	};
	static_assert(sizeof(hkpSpringDamperConstraintMotor) == 0x28);
	static_assert(offsetof(hkpSpringDamperConstraintMotor, m_springConstant) == 0x20);

	class hkpCallbackConstraintMotor : public hkpLimitedForceConstraintMotor
	{
	public:
		/* 0x020 */ std::uint8_t unk020[0x28];
	};
	static_assert(sizeof(hkpCallbackConstraintMotor) == 0x48);

	class hkpConstraintChainData : public hkpConstraintData
	{
	public:
		virtual void sub_18();
	};
	static_assert(sizeof(hkpConstraintChainData) == 0x18);

	class hkpWrappedConstraintData : public hkpConstraintData
	{
	public:
		virtual void sub_18();
		virtual void sub_19();
		virtual void sub_1A();
		/* 0x018 */ std::uint8_t unk018[8];
	};
	static_assert(sizeof(hkpWrappedConstraintData) == 0x20);

	class hkpBreakableConstraintData : public hkpWrappedConstraintData
	{
	public:
		/* 0x020 */ std::uint8_t unk020[0x30];
	};
	static_assert(sizeof(hkpBreakableConstraintData) == 0x50);

	class hkpMalleableConstraintData : public hkpWrappedConstraintData
	{
	public:
		/* 0x020 */ std::uint8_t unk020[0x30];
	};
	static_assert(sizeof(hkpMalleableConstraintData) == 0x50);

	class alignas(16) hkpBallAndSocketConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0x58];
	};
	static_assert(sizeof(hkpBallAndSocketConstraintData) == 0x70);

	class hkpBallSocketChainData : public hkpConstraintChainData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0x58];
	};
	static_assert(sizeof(hkpBallSocketChainData) == 0x70);

	class alignas(16) hkpCogWheelConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0xA8];
	};
	static_assert(sizeof(hkpCogWheelConstraintData) == 0xC0);

	class alignas(16) hkpDeformableFixedConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0x158];
	};
	static_assert(sizeof(hkpDeformableFixedConstraintData) == 0x170);

	class alignas(16) hkpFixedConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0xC8];
	};
	static_assert(sizeof(hkpFixedConstraintData) == 0xE0);

	class hkpGenericConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0x78];
	};
	static_assert(sizeof(hkpGenericConstraintData) == 0x90);

	class alignas(16) hkpHingeConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0xC8];
	};
	static_assert(sizeof(hkpHingeConstraintData) == 0xE0);

	class alignas(16) hkpHingeLimitsData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0x98];
	};
	static_assert(sizeof(hkpHingeLimitsData) == 0xB0);

	class alignas(16) hkpLimitedHingeConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0x118];
	};
	static_assert(sizeof(hkpLimitedHingeConstraintData) == 0x130);

	class alignas(16) hkpLinearClearanceConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0x128];
	};
	static_assert(sizeof(hkpLinearClearanceConstraintData) == 0x140);

	class alignas(16) hkpPointToPathConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0xB8];
	};
	static_assert(sizeof(hkpPointToPathConstraintData) == 0xD0);

	class alignas(16) hkpPointToPlaneConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0xA8];
	};
	static_assert(sizeof(hkpPointToPlaneConstraintData) == 0xC0);

	class alignas(16) hkpPrismaticConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0x108];
	};
	static_assert(sizeof(hkpPrismaticConstraintData) == 0x120);

	class alignas(16) hkpPulleyConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0x78];
	};
	static_assert(sizeof(hkpPulleyConstraintData) == 0x90);

	class alignas(16) hkpRackAndPinionConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0xA8];
	};
	static_assert(sizeof(hkpRackAndPinionConstraintData) == 0xC0);

	class alignas(16) hkpRagdollConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0x188];
	};
	static_assert(sizeof(hkpRagdollConstraintData) == 0x1A0);

	class alignas(16) hkpRagdollLimitsData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0xD8];
	};
	static_assert(sizeof(hkpRagdollLimitsData) == 0xF0);

	class alignas(16) hkpRotationalConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0x88];
	};
	static_assert(sizeof(hkpRotationalConstraintData) == 0xA0);

	class hkpSimpleContactConstraintData : public hkpConstraintData
	{
	public:
		virtual void sub_18();
		virtual void sub_19();
		/* 0x018 */ std::uint8_t unk018[0x38];
	};
	static_assert(sizeof(hkpSimpleContactConstraintData) == 0x50);

	class alignas(16) hkpStiffSpringConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0x58];
	};
	static_assert(sizeof(hkpStiffSpringConstraintData) == 0x70);

	class alignas(16) hkpWheelConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0x178];
	};
	static_assert(sizeof(hkpWheelConstraintData) == 0x190);

	class alignas(16) hkpWheelFrictionConstraintData : public hkpConstraintData
	{
	public:
		/* 0x018 */ std::uint8_t unk018[0xC8];
	};
	static_assert(sizeof(hkpWheelFrictionConstraintData) == 0xE0);

	class hkpParametricCurve : public hkReferencedObject
	{
	public:
		virtual void sub_04();
		virtual void sub_05();
		virtual void sub_06();
		virtual void sub_07();
		virtual void sub_08();
		virtual void sub_09();
		virtual void sub_0A();
		virtual void sub_0B();
		virtual void sub_0C();
		virtual void sub_0D();
		virtual void sub_0E();
	};
	static_assert(sizeof(hkpParametricCurve) == 0x10);

	class hkpLinearParametricCurve : public hkpParametricCurve
	{
	public:
		/* 0x010 */ std::uint8_t unk010[0x40];
	};
	static_assert(sizeof(hkpLinearParametricCurve) == 0x50);
}
