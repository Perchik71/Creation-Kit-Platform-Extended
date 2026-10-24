// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseObject.h>
#include <EditorAPI/H/hkBaseTypes.h>
#include <EditorAPI/H/hkReflectionTypes.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkpMaxSizeMotion;

	class alignas(16) hkpMotion : public hkReferencedObject
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
		virtual void sub_18();
		virtual void sub_19();
		virtual void sub_1A();
		virtual void sub_1B();
		virtual void sub_1C();
		virtual void sub_1D();

		/* 0x010 */ std::uint8_t m_type;
		/* 0x011 */ std::uint8_t m_deactivationIntegrateCounter;
		/* 0x012 */ std::uint16_t m_deactivationNumInactiveFrames[2];
		/* 0x016 */ std::uint8_t pad016[0xA];
		/* 0x020 */ hkMotionState m_motionState;
		/* 0x0D0 */ hkVector4 m_inertiaAndMassInv;
		/* 0x0E0 */ hkVector4 m_linearVelocity;
		/* 0x0F0 */ hkVector4 m_angularVelocity;
		/* 0x100 */ hkVector4 m_deactivationRefPosition[2];
		/* 0x120 */ std::uint32_t m_deactivationRefOrientation[2];
		/* 0x128 */ hkpMaxSizeMotion* m_savedMotion;
		/* 0x130 */ std::uint16_t m_savedQualityTypeIndex;
		/* 0x132 */ hkHalf m_gravityFactor;
		/* 0x134 */ std::uint8_t pad134[0xC];
	};
	static_assert(sizeof(hkpMotion) == 0x140);
	static_assert(offsetof(hkpMotion, m_motionState) == 0x20);
	static_assert(offsetof(hkpMotion, m_inertiaAndMassInv) == 0xD0);
	static_assert(offsetof(hkpMotion, m_linearVelocity) == 0xE0);
	static_assert(offsetof(hkpMotion, m_savedMotion) == 0x128);

	class hkpSphereMotion : public hkpMotion {};
	static_assert(sizeof(hkpSphereMotion) == 0x140);

	class hkpBoxMotion : public hkpMotion {};
	static_assert(sizeof(hkpBoxMotion) == 0x140);

	class hkpThinBoxMotion : public hkpBoxMotion {};
	static_assert(sizeof(hkpThinBoxMotion) == 0x140);

	class hkpCharacterMotion : public hkpMotion {};
	static_assert(sizeof(hkpCharacterMotion) == 0x140);

	class hkpKeyframedRigidMotion : public hkpMotion
	{
	public:
		virtual void sub_1E();
		virtual void sub_1F();
	};
	static_assert(sizeof(hkpKeyframedRigidMotion) == 0x140);

	class hkpFixedRigidMotion : public hkpKeyframedRigidMotion
	{
	public:
		virtual void sub_20();
	};
	static_assert(sizeof(hkpFixedRigidMotion) == 0x140);

	class hkpMaxSizeMotion : public hkpKeyframedRigidMotion {};
	static_assert(sizeof(hkpMaxSizeMotion) == 0x140);
}
