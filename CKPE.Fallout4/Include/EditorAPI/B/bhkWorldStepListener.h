// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkIWorldStepListener
	{
	public:
		/* 00 */ virtual ~bhkIWorldStepListener();
		/* 01 */ virtual void WorldStep_Unknown01();
		/* 02 */ virtual void WorldStep_Unknown02();
		/* 03 */ virtual void WorldStep_Unknown03();
		/* 04 */ virtual void WorldStep_Unknown04();
		/* 05 */ virtual void WorldStep_Unknown05();
		/* 06 */ virtual void WorldStep_Unknown06();
		/* 07 */ virtual void WorldStep_Unknown07();
		/* 08 */ virtual void WorldStep_Unknown08();
		/* 09 */ virtual void WorldStep_Unknown09();
		/* 0A */ virtual void WorldStep_Unknown0A();

		/* 0x008 */ std::byte unk008[0x8];
	};
	static_assert(sizeof(bhkIWorldStepListener) == 0x10);

	class bhkCharProxyManager : public bhkIWorldStepListener
	{
	public:
		/* 0x010 */ std::byte unk010[0x68];
	};
	static_assert(sizeof(bhkCharProxyManager) == 0x78);
	static_assert(offsetof(bhkCharProxyManager, unk010) == 0x10);

	class bhkCharRigidBodyManager : public bhkIWorldStepListener
	{
	public:
		/* 0x010 */ std::byte unk010[0x68];
	};
	static_assert(sizeof(bhkCharRigidBodyManager) == 0x78);
	static_assert(offsetof(bhkCharRigidBodyManager, unk010) == 0x10);
}
