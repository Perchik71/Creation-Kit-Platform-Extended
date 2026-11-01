// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkpConstraintData.h>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkPositionConstraintMotor : public hkpPositionConstraintMotor
	{
	private:
		/* 0x028 */ virtual const void* GetRTTI() const; // NiObject interface embedded after the Havok virtuals: returns the NiRTTI of the class
		/* 0x030 */ virtual void* IsNodeConst(); // NiObject cast helper, null here
		/* 0x038 */ virtual void* IsNode(); // NiObject cast helper, null here
		/* 0x040 */ virtual void* IsSwitchNode(); // NiObject cast helper, null here
		/* 0x048 */ virtual void* IsFadeNode(); // NiObject cast helper, null here
		/* 0x050 */ virtual void* IsMultiBoundNode(); // NiObject cast helper, null here
		/* 0x058 */ virtual void* IsGeometry(); // NiObject cast helper, null here
		/* 0x060 */ virtual void* IsTriStrips(); // NiObject cast helper, null here
		/* 0x068 */ virtual void* IsTriShape(); // NiObject cast helper, null here
		/* 0x070 */ virtual void* IsDynamicTriShape(); // NiObject cast helper, null here
		/* 0x078 */ virtual void* IsSegmentedTriShape(); // NiObject cast helper, null here
		/* 0x080 */ virtual void* IsSubIndexTriShape(); // NiObject cast helper, null here
		/* 0x088 */ virtual void* IsNiGeometry(); // NiObject cast helper, null here
		/* 0x090 */ virtual void* IsNiTriBasedGeom(); // NiObject cast helper, null here
		/* 0x098 */ virtual void* IsNiTriShape(); // NiObject cast helper, null here
		/* 0x0A0 */ virtual void* IsParticlesGeom(); // NiObject cast helper, null here
		/* 0x0A8 */ virtual void* IsParticleSystem(); // NiObject cast helper, null here
		/* 0x0B0 */ virtual void* IsLinesGeom(); // NiObject cast helper, null here
		/* 0x0B8 */ virtual void* IsLight(); // NiObject cast helper, null here
		/* 0x0C0 */ virtual void* IsBhkNiCollisionObject(); // NiObject cast helper, null here
		/* 0x0C8 */ virtual void* IsBhkBlendCollisionObject(); // NiObject cast helper, null here
		/* 0x0D0 */ virtual void* IsBhkRigidBody(); // NiObject cast helper, null here
		/* 0x0D8 */ virtual void* IsBhkLimitedHingeConstraint(); // NiObject cast helper, null here
		/* 0x0E0 */ virtual void* IsbhkNPCollisionObject(); // NiObject cast helper, null here
		/* 0x0E8 */ virtual void* CreateClone(void* Process); // NiCloningProcess&; allocates a copy of the motor (bshavokmigration)
	};
	static_assert(sizeof(bhkPositionConstraintMotor) == 0x30);

	class bhkVelocityConstraintMotor : public hkpVelocityConstraintMotor
	{
	private:
		/* 0x028 */ virtual const void* GetRTTI() const; // NiObject interface embedded after the Havok virtuals: returns the NiRTTI of the class
		/* 0x030 */ virtual void* IsNodeConst(); // NiObject cast helper, null here
		/* 0x038 */ virtual void* IsNode(); // NiObject cast helper, null here
		/* 0x040 */ virtual void* IsSwitchNode(); // NiObject cast helper, null here
		/* 0x048 */ virtual void* IsFadeNode(); // NiObject cast helper, null here
		/* 0x050 */ virtual void* IsMultiBoundNode(); // NiObject cast helper, null here
		/* 0x058 */ virtual void* IsGeometry(); // NiObject cast helper, null here
		/* 0x060 */ virtual void* IsTriStrips(); // NiObject cast helper, null here
		/* 0x068 */ virtual void* IsTriShape(); // NiObject cast helper, null here
		/* 0x070 */ virtual void* IsDynamicTriShape(); // NiObject cast helper, null here
		/* 0x078 */ virtual void* IsSegmentedTriShape(); // NiObject cast helper, null here
		/* 0x080 */ virtual void* IsSubIndexTriShape(); // NiObject cast helper, null here
		/* 0x088 */ virtual void* IsNiGeometry(); // NiObject cast helper, null here
		/* 0x090 */ virtual void* IsNiTriBasedGeom(); // NiObject cast helper, null here
		/* 0x098 */ virtual void* IsNiTriShape(); // NiObject cast helper, null here
		/* 0x0A0 */ virtual void* IsParticlesGeom(); // NiObject cast helper, null here
		/* 0x0A8 */ virtual void* IsParticleSystem(); // NiObject cast helper, null here
		/* 0x0B0 */ virtual void* IsLinesGeom(); // NiObject cast helper, null here
		/* 0x0B8 */ virtual void* IsLight(); // NiObject cast helper, null here
		/* 0x0C0 */ virtual void* IsBhkNiCollisionObject(); // NiObject cast helper, null here
		/* 0x0C8 */ virtual void* IsBhkBlendCollisionObject(); // NiObject cast helper, null here
		/* 0x0D0 */ virtual void* IsBhkRigidBody(); // NiObject cast helper, null here
		/* 0x0D8 */ virtual void* IsBhkLimitedHingeConstraint(); // NiObject cast helper, null here
		/* 0x0E0 */ virtual void* IsbhkNPCollisionObject(); // NiObject cast helper, null here
		/* 0x0E8 */ virtual void* CreateClone(void* Process); // NiCloningProcess&; allocates a copy of the motor (bshavokmigration)
	};
	static_assert(sizeof(bhkVelocityConstraintMotor) == 0x30);

	class bhkSpringDamperConstraintMotor : public hkpSpringDamperConstraintMotor
	{
	private:
		/* 0x028 */ virtual const void* GetRTTI() const; // NiObject interface embedded after the Havok virtuals: returns the NiRTTI of the class
		/* 0x030 */ virtual void* IsNodeConst(); // NiObject cast helper, null here
		/* 0x038 */ virtual void* IsNode(); // NiObject cast helper, null here
		/* 0x040 */ virtual void* IsSwitchNode(); // NiObject cast helper, null here
		/* 0x048 */ virtual void* IsFadeNode(); // NiObject cast helper, null here
		/* 0x050 */ virtual void* IsMultiBoundNode(); // NiObject cast helper, null here
		/* 0x058 */ virtual void* IsGeometry(); // NiObject cast helper, null here
		/* 0x060 */ virtual void* IsTriStrips(); // NiObject cast helper, null here
		/* 0x068 */ virtual void* IsTriShape(); // NiObject cast helper, null here
		/* 0x070 */ virtual void* IsDynamicTriShape(); // NiObject cast helper, null here
		/* 0x078 */ virtual void* IsSegmentedTriShape(); // NiObject cast helper, null here
		/* 0x080 */ virtual void* IsSubIndexTriShape(); // NiObject cast helper, null here
		/* 0x088 */ virtual void* IsNiGeometry(); // NiObject cast helper, null here
		/* 0x090 */ virtual void* IsNiTriBasedGeom(); // NiObject cast helper, null here
		/* 0x098 */ virtual void* IsNiTriShape(); // NiObject cast helper, null here
		/* 0x0A0 */ virtual void* IsParticlesGeom(); // NiObject cast helper, null here
		/* 0x0A8 */ virtual void* IsParticleSystem(); // NiObject cast helper, null here
		/* 0x0B0 */ virtual void* IsLinesGeom(); // NiObject cast helper, null here
		/* 0x0B8 */ virtual void* IsLight(); // NiObject cast helper, null here
		/* 0x0C0 */ virtual void* IsBhkNiCollisionObject(); // NiObject cast helper, null here
		/* 0x0C8 */ virtual void* IsBhkBlendCollisionObject(); // NiObject cast helper, null here
		/* 0x0D0 */ virtual void* IsBhkRigidBody(); // NiObject cast helper, null here
		/* 0x0D8 */ virtual void* IsBhkLimitedHingeConstraint(); // NiObject cast helper, null here
		/* 0x0E0 */ virtual void* IsbhkNPCollisionObject(); // NiObject cast helper, null here
		/* 0x0E8 */ virtual void* CreateClone(void* Process); // NiCloningProcess&; allocates a copy of the motor (bshavokmigration)
	};
	static_assert(sizeof(bhkSpringDamperConstraintMotor) == 0x28);
}
