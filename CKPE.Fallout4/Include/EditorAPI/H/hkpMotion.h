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
		// Simd arguments (hkSimdRealParameter, hkVector4) are passed by pointer; each overload pair is laid out in reverse declaration order
		virtual void setMass(const void* Mass); // 0x20 - hkSimdRealParameter
		virtual void setMass(float Mass); // 0x28
		virtual void setMassInv(const void* MassInv); // 0x30 - hkSimdRealParameter
		virtual void setMassInv(float MassInv); // 0x38 - writes the w lane of the inertia and inverse mass vector
		virtual void getInertiaLocal(void* InertiaOut) const; // 0x40 - hkMatrix3
		virtual void getInertiaWorld(void* InertiaOut) const; // 0x48
		virtual void setInertiaLocal(const void* Inertia); // 0x50
		virtual void setInertiaInvLocal(const void* InertiaInv); // 0x58
		virtual void getInertiaInvLocal(void* InertiaInvOut) const; // 0x60
		virtual void getInertiaInvWorld(void* InertiaInvOut) const; // 0x68
		virtual void setCenterOfMassInLocal(const void* CenterOfMass); // 0x70
		virtual void setPosition(const void* Position); // 0x78
		virtual void setRotation(const void* Rotation); // 0x80 - hkQuaternion
		virtual void setPositionAndRotation(const void* Position, const void* Rotation); // 0x88
		virtual void setTransform(const void* Transform); // 0x90
		virtual void setLinearVelocity(const void* Velocity); // 0x98
		virtual void setAngularVelocity(const void* Velocity); // 0xA0
		virtual void getProjectedPointVelocity(const void* Point, const void* Normal, float* VelocityOut, float* InvVirtualMassOut) const; // 0xA8
		virtual void getProjectedPointVelocitySimd(const void* Point, const void* Normal, void* VelocityOut, void* InvVirtualMassOut) const; // 0xB0 - hkSimdReal outputs
		virtual void applyLinearImpulse(const void* Impulse); // 0xB8
		virtual void applyPointImpulse(const void* Impulse, const void* Point); // 0xC0
		virtual void applyAngularImpulse(const void* Impulse); // 0xC8
		virtual void applyForce(float DeltaTime, const void* Force, const void* Point); // 0xD0
		virtual void applyForce(float DeltaTime, const void* Force); // 0xD8
		virtual void applyTorque(float DeltaTime, const void* Torque); // 0xE0
		virtual void getMotionStateAndVelocitiesAndDeactivationType(void* MotionOut) const; // 0xE8

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
