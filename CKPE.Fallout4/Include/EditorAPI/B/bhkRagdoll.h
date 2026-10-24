// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/bhkPhysicsSystem.h>
#include <EditorAPI/N/NiObjectNET.h>
#include <cstddef>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkPoseArray : public NiAPI::NiObject
	{
	public:
		/* 0x010 */ std::byte unk010[0x20];
	};
	static_assert(sizeof(bhkPoseArray) == 0x30);
	static_assert(offsetof(bhkPoseArray, unk010) == 0x10);

	class bhkRagdollSystem : public bhkPhysicsSystem {};
	static_assert(sizeof(bhkRagdollSystem) == 0x28);

	class bhkRagdollTemplate : public NiAPI::NiExtraData
	{
	public:
		/* 0x018 */ std::byte unk018[0x18];
	};
	static_assert(sizeof(bhkRagdollTemplate) == 0x30);
	static_assert(offsetof(bhkRagdollTemplate, unk018) == 0x18);

	class bhkRagdollTemplateData : public NiAPI::NiObject
	{
	public:
		/* 0x010 */ std::byte unk010[0x50];
	};
	static_assert(sizeof(bhkRagdollTemplateData) == 0x60);
	static_assert(offsetof(bhkRagdollTemplateData, unk010) == 0x10);
}
