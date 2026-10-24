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
		/* 0x028 */ virtual void sub_028();
		/* 0x030 */ virtual void sub_030();
		/* 0x038 */ virtual void sub_038();
		/* 0x040 */ virtual void sub_040();
		/* 0x048 */ virtual void sub_048();
		/* 0x050 */ virtual void sub_050();
		/* 0x058 */ virtual void sub_058();
		/* 0x060 */ virtual void sub_060();
		/* 0x068 */ virtual void sub_068();
		/* 0x070 */ virtual void sub_070();
		/* 0x078 */ virtual void sub_078();
		/* 0x080 */ virtual void sub_080();
		/* 0x088 */ virtual void sub_088();
		/* 0x090 */ virtual void sub_090();
		/* 0x098 */ virtual void sub_098();
		/* 0x0A0 */ virtual void sub_0A0();
		/* 0x0A8 */ virtual void sub_0A8();
		/* 0x0B0 */ virtual void sub_0B0();
		/* 0x0B8 */ virtual void sub_0B8();
		/* 0x0C0 */ virtual void sub_0C0();
		/* 0x0C8 */ virtual void sub_0C8();
		/* 0x0D0 */ virtual void sub_0D0();
		/* 0x0D8 */ virtual void sub_0D8();
		/* 0x0E0 */ virtual void sub_0E0();
		/* 0x0E8 */ virtual void sub_0E8();
	};
	static_assert(sizeof(bhkPositionConstraintMotor) == 0x30);

	class bhkVelocityConstraintMotor : public hkpVelocityConstraintMotor
	{
	private:
		/* 0x028 */ virtual void sub_028();
		/* 0x030 */ virtual void sub_030();
		/* 0x038 */ virtual void sub_038();
		/* 0x040 */ virtual void sub_040();
		/* 0x048 */ virtual void sub_048();
		/* 0x050 */ virtual void sub_050();
		/* 0x058 */ virtual void sub_058();
		/* 0x060 */ virtual void sub_060();
		/* 0x068 */ virtual void sub_068();
		/* 0x070 */ virtual void sub_070();
		/* 0x078 */ virtual void sub_078();
		/* 0x080 */ virtual void sub_080();
		/* 0x088 */ virtual void sub_088();
		/* 0x090 */ virtual void sub_090();
		/* 0x098 */ virtual void sub_098();
		/* 0x0A0 */ virtual void sub_0A0();
		/* 0x0A8 */ virtual void sub_0A8();
		/* 0x0B0 */ virtual void sub_0B0();
		/* 0x0B8 */ virtual void sub_0B8();
		/* 0x0C0 */ virtual void sub_0C0();
		/* 0x0C8 */ virtual void sub_0C8();
		/* 0x0D0 */ virtual void sub_0D0();
		/* 0x0D8 */ virtual void sub_0D8();
		/* 0x0E0 */ virtual void sub_0E0();
		/* 0x0E8 */ virtual void sub_0E8();
	};
	static_assert(sizeof(bhkVelocityConstraintMotor) == 0x30);

	class bhkSpringDamperConstraintMotor : public hkpSpringDamperConstraintMotor
	{
	private:
		/* 0x028 */ virtual void sub_028();
		/* 0x030 */ virtual void sub_030();
		/* 0x038 */ virtual void sub_038();
		/* 0x040 */ virtual void sub_040();
		/* 0x048 */ virtual void sub_048();
		/* 0x050 */ virtual void sub_050();
		/* 0x058 */ virtual void sub_058();
		/* 0x060 */ virtual void sub_060();
		/* 0x068 */ virtual void sub_068();
		/* 0x070 */ virtual void sub_070();
		/* 0x078 */ virtual void sub_078();
		/* 0x080 */ virtual void sub_080();
		/* 0x088 */ virtual void sub_088();
		/* 0x090 */ virtual void sub_090();
		/* 0x098 */ virtual void sub_098();
		/* 0x0A0 */ virtual void sub_0A0();
		/* 0x0A8 */ virtual void sub_0A8();
		/* 0x0B0 */ virtual void sub_0B0();
		/* 0x0B8 */ virtual void sub_0B8();
		/* 0x0C0 */ virtual void sub_0C0();
		/* 0x0C8 */ virtual void sub_0C8();
		/* 0x0D0 */ virtual void sub_0D0();
		/* 0x0D8 */ virtual void sub_0D8();
		/* 0x0E0 */ virtual void sub_0E0();
		/* 0x0E8 */ virtual void sub_0E8();
	};
	static_assert(sizeof(bhkSpringDamperConstraintMotor) == 0x28);
}
