// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSTEvents.h>
#include <EditorAPI/B/BSTSmartPointer.h>
#include <EditorAPI/B/BShkbHkxDB.h>
#include <EditorAPI/H/hkbBehaviorGraph.h>
#include <EditorAPI/T/TESObjectREFRFwd.h>

namespace CKPE::Fallout4::EditorAPI
{
	class bhkWorld;

	class BSIRagdollDriver
	{
	public:
		/* 0x000 */ virtual ~BSIRagdollDriver();
		/* 0x008 */ [[nodiscard]] virtual bool HasRagdoll() const noexcept(true);

	private:
		/* 0x010 */ virtual void sub_010();

	public:
		// Adds the ragdoll's bodies to the physics world (asserts when the world does not match); false when there is no world or ragdoll
		/* 0x018 */ virtual bool AddRagdollToWorld();
		// Removes the ragdoll from the physics world (asserts when it is not in it); false when there is no world or ragdoll
		/* 0x020 */ virtual bool RemoveRagdollFromWorld();
		/* 0x028 */ virtual void SetWorld(bhkWorld* world) noexcept(true);

	private:
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

	public:
		// True when the world is set and the ragdoll is in it
		/* 0x098 */ virtual bool IsRagdollInWorld() const;

	private:
		/* 0x0A0 */ virtual void sub_0A0();
		/* 0x0A8 */ virtual void sub_0A8();
	};
	static_assert(sizeof(BSIRagdollDriver) == 0x8);

	class BShkbAnimationGraph :
		public BSIRagdollDriver,
		public BSIntrusiveRefCounted,
		public BSTEventSource<BSActiveGraphIfInactiveEvent>,
		public BSTEventSource<BSAnimationGraphEvent>,
		public BSTEventSource<BSMovementDataChangedEvent>,
		public BSTEventSource<BSSubGraphActivationUpdate>,
		public BSTEventSource<BSTransformDeltaEvent>
	{
	public:
		/* 0x1C8 */ hkbCharacter character;
		/* 0x2A0 */ std::byte unk2A0[0xD0];
		/* 0x370 */ BShkbHkxDB::ProjectDBData* projectDBData;
		/* 0x378 */ hkbBehaviorGraph* behaviorGraph;
		/* 0x380 */ Forms::TESObjectREFR* holder;
		/* 0x388 */ std::byte unk388[0x30];
		/* 0x3B8 */ bhkWorld* world;
		/* 0x3C0 */ std::byte unk3C0[0x10];
	};
	static_assert(sizeof(BShkbAnimationGraph) == 0x3D0);
	static_assert(offsetof(BShkbAnimationGraph, character) == 0x1C8);
	static_assert(offsetof(BShkbAnimationGraph, projectDBData) == 0x370);
	static_assert(offsetof(BShkbAnimationGraph, behaviorGraph) == 0x378);
	static_assert(offsetof(BShkbAnimationGraph, holder) == 0x380);
	static_assert(offsetof(BShkbAnimationGraph, world) == 0x3B8);
}
