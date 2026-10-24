// Copyright © 2020-2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <EditorAPI/N/NiObject.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hknpPhysicsSystemData;
	class hknpPhysicsSystem;

	class bhkPhysicsSystem : public NiAPI::NiObject
	{
	public:
		/* 2A */ virtual void UnkPhysicsSystem2A();
		/* 2B */ virtual void UnkPhysicsSystem2B();
		/* 2C */ virtual void UnkPhysicsSystem2C();

		/* 0x010 */ hknpPhysicsSystemData* systemData;
		/* 0x018 */ hknpPhysicsSystem* systemInstance;
		/* 0x020 */ bool activateOnAdd;
		/* 0x021 */ std::uint8_t pad021[0x7];
	};
	static_assert(sizeof(bhkPhysicsSystem) == 0x28);
	static_assert(offsetof(bhkPhysicsSystem, systemData) == 0x10);
	static_assert(offsetof(bhkPhysicsSystem, systemInstance) == 0x18);
	static_assert(offsetof(bhkPhysicsSystem, activateOnAdd) == 0x20);
}
