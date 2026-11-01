// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/bhkCollisionObject.h>
#include <EditorAPI/B/bhkCharacterState.h>
#include <EditorAPI/B/bhkCharacterOrientation.h>
#include <EditorAPI/H/hkBaseTypes.h>
#include <EditorAPI/N/NiObject.h>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkWorld;
	class hknpCharacterProxy;
	class hknpCharacterRigidBody;
	class hknpCollisionResult;
	class hknpShape;

	class alignas(16) bhkCharacterControllerCinfo
	{
	public:
		/* 00 */ virtual ~bhkCharacterControllerCinfo();
		/* 01 */ virtual void ControllerCinfo_Unknown01();
		/* 02 */ virtual void ControllerCinfo_Unknown02();
		/* 03 */ virtual void ControllerCinfo_Unknown03();
		/* 04 */ virtual void ControllerCinfo_Unknown04();
		/* 05 */ virtual void ControllerCinfo_Unknown05();
		/* 06 */ virtual void ControllerCinfo_Unknown06();

		/* 0x008 */ std::byte unk008[0x18];
		/* 0x020 */ bhkWorld* world;
		/* 0x028 */ std::byte unk028[0x78];
	};
	static_assert(sizeof(bhkCharacterControllerCinfo) == 0xA0);
	static_assert(offsetof(bhkCharacterControllerCinfo, world) == 0x20);

	class bhkCharProxyControllerCinfo : public bhkCharacterControllerCinfo
	{
	public:
		/* 0x0A0 */ std::byte unk0A0[0xB0];
	};
	static_assert(sizeof(bhkCharProxyControllerCinfo) == 0x150);
	static_assert(offsetof(bhkCharProxyControllerCinfo, unk0A0) == 0xA0);

	class bhkCharRigidBodyControllerCinfo : public bhkCharacterControllerCinfo
	{
	public:
		/* 0x0A0 */ std::byte unk0A0[0xA0];
	};
	static_assert(sizeof(bhkCharRigidBodyControllerCinfo) == 0x140);

	class alignas(16) bhkCharacterRigidBodyCinfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint32_t collisionFilterInfo;
		/* 0x014 */ std::byte pad014[0x4];
		/* 0x018 */ hknpShape* shape;
		/* 0x020 */ void* world;
		/* 0x028 */ std::byte pad028[0x8];
		/* 0x030 */ hkVector4f position;
		/* 0x040 */ hkQuaternionf orientation;
		/* 0x050 */ float mass;
		/* 0x054 */ float dynamicFriction;
		/* 0x058 */ float staticFriction;
		/* 0x05C */ float weldingTolerance;
		/* 0x060 */ std::uint32_t reservedBodyId;
		/* 0x064 */ std::uint8_t additionMode;
		/* 0x065 */ std::uint8_t additionFlags;
		/* 0x066 */ std::byte pad066[0xA];
		/* 0x070 */ hkVector4f up;
		/* 0x080 */ float maxSlope;
		/* 0x084 */ float maxForce;
		/* 0x088 */ float maxSpeedForSimplexSolver;
		/* 0x08C */ float supportDistance;
		/* 0x090 */ float hardSupportDistance;
		/* 0x094 */ std::byte pad094[0xC];
	};
	static_assert(sizeof(bhkCharacterRigidBodyCinfo) == 0xA0);
	static_assert(offsetof(bhkCharacterRigidBodyCinfo, collisionFilterInfo) == 0x10);
	static_assert(offsetof(bhkCharacterRigidBodyCinfo, shape) == 0x18);
	static_assert(offsetof(bhkCharacterRigidBodyCinfo, world) == 0x20);
	static_assert(offsetof(bhkCharacterRigidBodyCinfo, position) == 0x30);
	static_assert(offsetof(bhkCharacterRigidBodyCinfo, orientation) == 0x40);
	static_assert(offsetof(bhkCharacterRigidBodyCinfo, mass) == 0x50);
	static_assert(offsetof(bhkCharacterRigidBodyCinfo, up) == 0x70);
	static_assert(offsetof(bhkCharacterRigidBodyCinfo, hardSupportDistance) == 0x90);

	class alignas(16) bhkCharacterPointCollector
	{
	public:
		// Non-owning layout view until hknpAllHitsCollector is modeled.
		/* 0x000 */ const void* const* vtable;
		/* 0x008 */ std::byte unk008[0x18];
		/* 0x020 */ hkArray<hknpCollisionResult> hits;
		/* 0x030 */ std::byte hitStorage[0x3C0];
		/* 0x3F0 */ std::byte unk3F0[0x30];
	};
	static_assert(sizeof(bhkCharacterPointCollector) == 0x420);
	static_assert(offsetof(bhkCharacterPointCollector, hits) == 0x20);
	static_assert(offsetof(bhkCharacterPointCollector, hitStorage) == 0x30);

	class alignas(16) bhkCharacterProxy : public NiAPI::NiObject
	{
	public:
		/* 0x010 */ hknpCharacterProxy* proxy;
		/* 0x018 */ std::byte pad018[0x8];
		/* 0x020 */ bhkCharacterPointCollector collector;
	};
	static_assert(sizeof(bhkCharacterProxy) == 0x440);
	static_assert(offsetof(bhkCharacterProxy, proxy) == 0x10);
	static_assert(offsetof(bhkCharacterProxy, collector) == 0x20);

	class bhkCharacterRigidBody : public NiAPI::NiObject
	{
	public:
		/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
		/* 20 */ virtual void GetViewerStringsArray(void* Strings) noexcept(true) override;
		/* 0x010 */ hknpCharacterRigidBody* rigidBody;
	};
	static_assert(sizeof(bhkCharacterRigidBody) == 0x18);
	static_assert(offsetof(bhkCharacterRigidBody, rigidBody) == 0x10);

	class alignas(16) bhkCharacterController :
		public bhkNPCollisionObjectUnlinked
	{
	public:
		/* 33 */ [[nodiscard]] virtual float GetKeepDistanceImpl() const = 0;
		/* 34 */ virtual void CharacterController_Unknown34() = 0;
		/* 35 */ virtual void GetPositionImpl(hkVector4f& Position,
			bool ApplyCenterOffset) const = 0;
		/* 36 */ virtual void CharacterController_Unknown36() = 0;
		/* 37 */ virtual void CharacterController_Unknown37() = 0;
		/* 38 */ virtual void CharacterController_Unknown38() = 0;
		/* 39 */ virtual void CharacterController_Unknown39() = 0;
		/* 3A */ virtual void CharacterController_Unknown3A() = 0;
		/* 3B */ virtual void GetLinearVelocityImpl(hkVector4f& Velocity) const = 0;
		/* 3C */ virtual void SetLinearVelocityImpl(const hkVector4f& Velocity) = 0;
		/* 3D */ virtual void CharacterController_Unknown3D() = 0;
		/* 3E */ virtual void CharacterController_Unknown3E() = 0;
		/* 3F */ virtual void CharacterController_Unknown3F() = 0;
		/* 40 */ virtual void CharacterController_Unknown40() = 0;
		/* 41 */ virtual void CharacterController_Unknown41() = 0;
		/* 42 */ [[nodiscard]] virtual bhkWorld* GetWorldImpl() const = 0;
		/* 43 */ virtual void CharacterController_Unknown43() = 0;
		/* 44 */ virtual void CharacterController_Unknown44() = 0;
		/* 45 */ virtual void CharacterController_Unknown45() = 0;
		/* 46 */ virtual void CharacterController_Unknown46() = 0;
		/* 47 */ virtual void CharacterController_Unknown47() = 0;
		/* 48 */ virtual void CharacterController_Unknown48() = 0;
		/* 49 */ virtual void CharacterController_Unknown49() = 0;
		/* 4A */ virtual void CharacterController_Unknown4A() = 0;
		/* 4B */ virtual void CharacterController_Unknown4B() = 0;
		/* 4C */ virtual void CharacterController_Unknown4C() = 0;
		/* 4D */ virtual void CharacterController_Unknown4D() = 0;
		/* 4E */ virtual void ClearCollectorDataImpl() = 0;

		/* 0x030 */ std::byte unk030[0x238];
		/* 0x268 */ bhkICharOrientationController* orientationController;
		/* 0x270 */ std::byte unk270[0x90];
		/* 0x300 */ std::uint32_t flags;
		/* 0x304 */ bhkCharacterStateType wantedState;
		/* 0x308 */ std::byte unk308[0x58];
		/* 0x360 */ hkRefPtr<hknpShape> shapes[2];
		/* 0x370 */ std::byte unk370[0xF0];
	};
	static_assert(sizeof(bhkCharacterController) == 0x460);
	static_assert(offsetof(bhkCharacterController, orientationController) == 0x268);
	static_assert(offsetof(bhkCharacterController, flags) == 0x300);
	static_assert(offsetof(bhkCharacterController, wantedState) == 0x304);
	static_assert(offsetof(bhkCharacterController, shapes) == 0x360);

	class bhkCharRigidBodyController : public bhkCharacterController
	{
	public:
		// The secondary hknpCharacterRigidBodyListener remains opaque.
		/* 0x460 */ std::byte unk460[0x10];
		/* 0x470 */ bhkCharacterRigidBody characterRigidBody;
		/* 0x488 */ std::byte pad488[0x8];
		/* 0x490 */ bhkCharacterPointCollector collector;
	};
	static_assert(sizeof(bhkCharRigidBodyController) == 0x8B0);
	static_assert(offsetof(bhkCharRigidBodyController, characterRigidBody) == 0x470);
	static_assert(offsetof(bhkCharRigidBodyController, collector) == 0x490);

	class alignas(16) bhkCharProxyController
	{
	public:
		// The controller is secondary to the unmodeled Havok listener.
		[[nodiscard]] bhkCharacterController* GetController() noexcept(true)
		{
			return reinterpret_cast<bhkCharacterController*>(unk010);
		}
		[[nodiscard]] const bhkCharacterController* GetController() const noexcept(true)
		{
			return reinterpret_cast<const bhkCharacterController*>(unk010);
		}

		/* 0x000 */ const void* const* vtable;
		/* 0x008 */ std::byte unk008[0x8];
		/* 0x010 */ std::byte unk010[0x460];
		/* 0x470 */ std::byte unk470[0x10];
		/* 0x480 */ bhkCharacterProxy characterProxy;
		/* 0x8C0 */ std::byte unk8C0[0x10];
	};
	static_assert(sizeof(bhkCharProxyController) == 0x8D0);
	static_assert(offsetof(bhkCharProxyController, unk010) == 0x10);
	static_assert(offsetof(bhkCharProxyController, characterProxy) == 0x480);
}
