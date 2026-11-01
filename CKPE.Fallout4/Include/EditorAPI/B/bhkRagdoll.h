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
		/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
		/* 1B */ virtual void LoadBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1C */ virtual void LinkObject(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1D */ virtual bool RegisterStreamables(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1E */ virtual void SaveBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 0x010 */ std::byte unk010[0x20];
	};
	static_assert(sizeof(bhkPoseArray) == 0x30);
	static_assert(offsetof(bhkPoseArray, unk010) == 0x10);

	class bhkRagdollSystem : public bhkPhysicsSystem {};
	static_assert(sizeof(bhkRagdollSystem) == 0x28);

	class bhkRagdollTemplate : public NiAPI::NiExtraData
	{
	private:
		/* 0x170 */ virtual void sub_170();

	public:
		/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
		/* 1A */ virtual NiAPI::NiObject* CreateClone(NiAPI::NiCloningProcess& Process) noexcept(true) override;
		/* 1B */ virtual void LoadBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1C */ virtual void LinkObject(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1D */ virtual bool RegisterStreamables(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1E */ virtual void SaveBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 0x018 */ std::byte unk018[0x18];
	};
	static_assert(sizeof(bhkRagdollTemplate) == 0x30);
	static_assert(offsetof(bhkRagdollTemplate, unk018) == 0x18);

	class bhkRagdollTemplateData : public NiAPI::NiObject
	{
	public:
		/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
		/* 1B */ virtual void LoadBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1C */ virtual void LinkObject(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1D */ virtual bool RegisterStreamables(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1E */ virtual void SaveBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 0x010 */ std::byte unk010[0x50];
	};
	static_assert(sizeof(bhkRagdollTemplateData) == 0x60);
	static_assert(offsetof(bhkRagdollTemplateData, unk010) == 0x10);
}
