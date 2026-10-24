// Copyright © 2020-2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <EditorAPI/B/bhkRefObject.h>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkWorldObject : public bhkRefObject
	{
	public:
		/* 38 */ virtual void UnkWorldObject38();
		/* 39 */ virtual void UnkWorldObject39();
		/* 3A */ virtual void UnkWorldObject3A();

		/* 0x020 */ std::uint8_t unk020[0x8];
	};
	static_assert(sizeof(bhkWorldObject) == 0x28);

	class bhkEntity : public bhkWorldObject {};
	static_assert(sizeof(bhkEntity) == 0x28);

	class bhkRigidBody : public bhkEntity
	{
	public:
		/* 3B */ virtual void UnkRigidBody3B();
		/* 3C */ virtual void UnkRigidBody3C();
		/* 3D */ virtual void UnkRigidBody3D();
		/* 3E */ virtual void UnkRigidBody3E();
		/* 3F */ virtual void UnkRigidBody3F();
		/* 40 */ virtual void UnkRigidBody40();
		/* 41 */ virtual void UnkRigidBody41();
		/* 42 */ virtual void UnkRigidBody42();
		/* 43 */ virtual void UnkRigidBody43();
		/* 44 */ virtual void UnkRigidBody44();

		/* 0x028 */ std::uint8_t unk028[0x18];
	};
	static_assert(sizeof(bhkRigidBody) == 0x40);

	class bhkPhantom : public bhkWorldObject {};
	static_assert(sizeof(bhkPhantom) == 0x28);

	class bhkAabbPhantom : public bhkPhantom {};
	static_assert(sizeof(bhkAabbPhantom) == 0x28);

	class bhkShapePhantom : public bhkPhantom {};
	static_assert(sizeof(bhkShapePhantom) == 0x28);

	class bhkCachingShapePhantom : public bhkShapePhantom {};
	static_assert(sizeof(bhkCachingShapePhantom) == 0x28);
}
