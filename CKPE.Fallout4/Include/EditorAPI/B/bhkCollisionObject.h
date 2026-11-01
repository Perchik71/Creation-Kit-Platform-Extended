// Copyright © 2020-2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/IDs.h>
#include <cstddef>
#include <cstdint>
#include <CKPE.Common.Relocation.h>
#include <EditorAPI/N/NiCollisionObject.h>
#include <EditorAPI/B/bhkPhysicsSystem.h>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkWorldObject;
	class bhkWorld;
	class hknpBody;

	class bhkNiCollisionObject : public NiAPI::NiCollisionObject
	{
	public:
		/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
		/* 15 */ virtual bhkNiCollisionObject* IsBhkNiCollisionObject() noexcept(true) override;
		/* 1B */ virtual void LoadBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1C */ virtual void LinkObject(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1D */ virtual bool RegisterStreamables(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1E */ virtual void SaveBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1F */ virtual bool IsEqual(NiAPI::NiObject* Object) noexcept(true) override;
		/* 29 */ virtual void GetAdditionalViewerStrings(void* Strings) noexcept(true) override;
		/* 2E */ virtual void UnkNiCollisionObject2E();
		/* 2F */ virtual void UnkNiCollisionObject2F();
		/* 30 */ virtual void UnkNiCollisionObject30();
		/* 31 */ virtual void UnkNiCollisionObject31();
		/* 32 */ virtual void UnkNiCollisionObject32();
		/* 33 */ virtual void UnkNiCollisionObject33();
		/* 34 */ virtual void UnkNiCollisionObject34();
		/* 35 */ virtual void UnkNiCollisionObject35();

		/* 0x018 */ std::uint16_t flags;
		/* 0x01A */ std::uint8_t pad01A[0x6];
		/* 0x020 */ bhkWorldObject* body;

		[[nodiscard]] inline bhkWorldObject* GetBody() const noexcept(true)
		{
			return body;
		}
	};
	static_assert(sizeof(bhkNiCollisionObject) == 0x28);
	static_assert(offsetof(bhkNiCollisionObject, flags) == 0x18);
	static_assert(offsetof(bhkNiCollisionObject, body) == 0x20);

	class bhkCollisionObject : public bhkNiCollisionObject {};
	static_assert(sizeof(bhkCollisionObject) == 0x28);

	class bhkBlendCollisionObject : public bhkCollisionObject
	{
	public:
		/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
		/* 16 */ virtual bhkBlendCollisionObject* IsBhkBlendCollisionObject() noexcept(true) override;
		/* 1A */ virtual NiAPI::NiObject* CreateClone(NiAPI::NiCloningProcess& Process) noexcept(true) override;
		/* 1B */ virtual void LoadBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1C */ virtual void LinkObject(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1D */ virtual bool RegisterStreamables(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1E */ virtual void SaveBinary(NiAPI::NiStream& Stream) noexcept(true) override;
		/* 1F */ virtual bool IsEqual(NiAPI::NiObject* Object) noexcept(true) override;
		/* 29 */ virtual void GetAdditionalViewerStrings(void* Strings) noexcept(true) override;
		/* 0x028 */ std::uint8_t unk028[0x20];
	};
	static_assert(sizeof(bhkBlendCollisionObject) == 0x48);

	class bhkPCollisionObject : public bhkNiCollisionObject {};
	static_assert(sizeof(bhkPCollisionObject) == 0x28);

	class bhkSPCollisionObject : public bhkPCollisionObject {};
	static_assert(sizeof(bhkSPCollisionObject) == 0x28);

	class bhkNPCollisionObjectBase : public NiAPI::NiCollisionObject
	{
	public:
		/* 2E */ virtual void LockMotionImpl();

		/* 0x018 */ std::uint16_t flags;
		/* 0x01A */ std::uint8_t pad01A[0x6];
	};
	static_assert(sizeof(bhkNPCollisionObjectBase) == 0x20);
	static_assert(offsetof(bhkNPCollisionObjectBase, flags) == 0x18);

	class bhkNPCollisionObject : public bhkNPCollisionObjectBase
	{
	public:
		/* 2F */ virtual void CreateInstance(bhkWorld* World);
		/* 30 */ virtual void AddToWorld(bhkWorld* World);
		/* 31 */ virtual void RemoveFromWorld();
		/* 32 */ virtual bool SetCollisionFilterInfo(std::uint32_t FilterInfo);

		/* 0x020 */ bhkPhysicsSystem* system;
		/* 0x028 */ std::uint32_t systemBodyIndex;
		/* 0x02C */ std::uint8_t pad02C[0x4];

		[[nodiscard]] inline hknpBody* GetBody() noexcept(true)
		{
			using Function = hknpBody* (*)(bhkNPCollisionObject*);
			static Common::Relocation<Function> function{
				EditorAPI::IDs::bhkNPCollisionObject::GetBody };
			return function(this);
		}
	};
	static_assert(sizeof(bhkNPCollisionObject) == 0x30);
	static_assert(offsetof(bhkNPCollisionObject, system) == 0x20);
	static_assert(offsetof(bhkNPCollisionObject, systemBodyIndex) == 0x28);

	class bhkNPCollisionObjectUnlinked : public bhkNPCollisionObject {};
	static_assert(sizeof(bhkNPCollisionObjectUnlinked) == 0x30);
}
