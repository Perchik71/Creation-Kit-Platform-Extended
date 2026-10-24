// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkICharOrientationController
	{
	public:
		/* 00 */ virtual ~bhkICharOrientationController();
		/* 01 */ virtual void Orientation_Unknown01() = 0;
		/* 02 */ virtual void Orientation_Unknown02() = 0;
		/* 03 */ virtual void Orientation_Unknown03() = 0;
		/* 04 */ virtual void Orientation_Unknown04() = 0;
		/* 05 */ virtual void Orientation_Unknown05() = 0;
		/* 06 */ virtual void Orientation_Unknown06() = 0;
		/* 07 */ virtual void Orientation_Unknown07() = 0;
		/* 08 */ virtual void Orientation_Unknown08() = 0;
		/* 09 */ virtual void Orientation_Unknown09() = 0;
		/* 0A */ virtual void Orientation_Unknown0A() = 0;
		/* 0B */ virtual void Orientation_Unknown0B();
		/* 0C */ virtual void Orientation_Unknown0C();
		/* 0D */ virtual void Orientation_Unknown0D();
		/* 0E */ virtual void Orientation_Unknown0E();
	};
	static_assert(sizeof(bhkICharOrientationController) == 0x8);

	class bhkBipedOrientationController : public bhkICharOrientationController
	{
	public:
		/* 0x008 */ std::byte pad008[0x8];
	};
	static_assert(sizeof(bhkBipedOrientationController) == 0x10);

	class alignas(16) bhkQuadOrientationController :
		public bhkICharOrientationController
	{
	public:
		/* 0x008 */ std::byte unk008[0xD8];
	};
	static_assert(sizeof(bhkQuadOrientationController) == 0xE0);
	static_assert(offsetof(bhkQuadOrientationController, unk008) == 0x8);
}
