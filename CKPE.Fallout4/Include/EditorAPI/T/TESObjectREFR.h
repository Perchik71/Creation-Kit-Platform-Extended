// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Utils.h>
#include <EditorAPI/B/BSTEvents.h>
#include <EditorAPI/B/BSFadeNode.h>
#include <EditorAPI/N/NiTypes.h>
#include <EditorAPI/N/NiPointer.h>
#include <EditorAPI/N/NiTypes.h>
#include <EditorAPI/B/BSHandleRefObject.h>
#include <EditorAPI/B/BSExtraData.h>
#include <EditorAPI/B/BSTSmartPointer.h>
#include <EditorAPI/F/FormInterfaces.h>
#include <EditorAPI/T/TESChildCell.h>
#include <EditorAPI/T/TESForm.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BGSInventoryList;
			class LOADED_REF_DATA;

			namespace ActorValueEvents
			{
				struct ActorValueChangedEvent;
			}

			namespace Forms
			{
				class TESObjectCELL;
				class TESBoundObject;

				template<class _Ty>
				class TESObjectREFR_base : public TESForm, public TESChildCell, public _Ty,
					public BSTEventSink<BSActiveGraphIfInactiveEvent>, public BSTEventSink<BSAnimationGraphEvent>, 
					public BSTEventSink<BGSInventoryListEvent::Event>,
					public IAnimationGraphManagerHolder, public IKeywordFormBase, public ActorValueOwner
				{
				public:
					constexpr static uint8_t TYPE_ID = TESForm::FormType::ftReference;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESObjectREFR[0];

					enum SpecialFlagsForm 
					{
						// https://github.com/shadeMe/Construction-Set-Extender/blob/master/EditorAPI/TESObjectREFR.h
						fs3DInvisible = 1 << 31,
						fsChildren3DInvisible = 1 << 30,
						fsFrozen = 1 << 29,
					};
				protected:
					BSTEventDispatcher<ActorValueEvents::ActorValueChangedEvent> _actorValueChangedEvents; // 0x70
					TESObjectCELL* _parentCell;					// 0xC8
					NiAPI::NiPoint3A _rotate;					// 0xD0, 0xD4, 0xD8, 0xDC
					NiAPI::NiPoint3A _position;					// 0xE0, 0xE4, 0xE8, 0xEC
					TESForm* _baseForm;							// 0xF0
					char _padF8[0x8];							// 0xF8
					LOADED_REF_DATA* _loadedData;				// 0x100
					BGSInventoryList* _inventoryList;			// 0x108
					BSTSmartPointer<ExtraDataList> _extraData;	// 0x110
					std::uint16_t _scale;						// 0x118
					std::uint8_t _modelState;					// 0x11A
					bool _predestroyed;							// 0x11B
					char _pad11C[0x4];							// 0x11C
					std::uint8_t _unk120[0x10];					// 0x120

					[[nodiscard]] inline NiAPI::NiPoint3 CallPoint3Virtual(std::size_t offset) const noexcept(true)
					{
						// Member-function vector returns put the result buffer after this.
						NiAPI::NiPoint3 result;
						CKPE::vtbl_call<NiAPI::NiPoint3*>(offset, this, &result);
						return result;
					}
				public:
					virtual ~TESObjectREFR_base() = default;
				private:
					// Virtuals 124-214 extend TESForm's table in slot order. Slots whose signature was recovered are public and typed; the rest are
					// private placeholders named after the IDA name (or vfXXX), signature not recovered. Overloads get distinct names because MSVC
					// lays out overloaded virtuals in reverse declaration order.
				public:
					/* 3E0 */ virtual void Predestroy() noexcept(true);
					/* 3E8 */ virtual void PreModelLoaderQueueing() noexcept(true);
					/* 3F0 */ [[nodiscard]] virtual void* GetEditorLocation() const noexcept(true);
					/* 3F8 */ [[nodiscard]] virtual bool GetEditorLocationFull(NiAPI::NiPoint3* originalLocation, NiAPI::NiPoint3* originalAngle,
						TESForm** locationFormOut, TESForm* locationForm) noexcept(true);
					/* 400 */ virtual void AddDynamicIdleCRC(std::uint32_t crc) noexcept(true);
					/* 408 */ virtual void GetPreviousDynamicIdleCRCS(std::uint32_t* crcs, std::uint32_t requestSize) const noexcept(true);
					/* 410 */ [[nodiscard]] virtual bool GetFullLODRef() const noexcept(true);
					/* 418 */ virtual void SetFullLODRef(bool val) noexcept(true);
					/* 420 */ [[nodiscard]] virtual void* GetSequencer() const noexcept(true);
				public:
					/* 428 */ [[nodiscard]] virtual bool QCanUpdateSync() const noexcept(true); // body returns true
					/* 430 */ [[nodiscard]] virtual bool GetAllowPromoteToPersistent() const noexcept(true); // body returns true
					/* 438 */ [[nodiscard]] virtual bool DoesFloat() const noexcept(true); // body returns false
				public:
					/* 440 */ virtual void SetActorCause(void* actorCause) noexcept(true);
					/* 448 */ [[nodiscard]] virtual void* GetActorCause() const noexcept(true);
					/* 450 */ [[nodiscard]] virtual float GetActorWidthOrRefDiameter() const noexcept(true);
					/* 458 */ [[nodiscard]] virtual float GetActorHeightOrRefBound() const noexcept(true);
					/* 460 */ [[nodiscard]] virtual NiAPI::NiPoint3 GetStartingAngle() const noexcept(true);
					/* 468 */ [[nodiscard]] virtual NiAPI::NiPoint3 GetStartingLocation() const noexcept(true);
					/* 470 */ virtual void SetStartingPosition(const NiAPI::NiPoint3* position) noexcept(true);
					/* 478 */ virtual void UpdateRefLight(float delta) noexcept(true);
					/* 480 */ virtual std::uint32_t RemoveItem(void* removeItemData) noexcept(true);
					/* 488 */ virtual void AddWornOutfitImpl(void* outfit, bool queueItem) noexcept(true);
					/* 490 */ virtual bool AddWornItem(TESBoundObject* object, void* extra, std::int32_t number, bool forceEquip, std::int32_t equipSlot) noexcept(true);
					/* 498 */ virtual void DoTrap(void* trap, void* target) noexcept(true);
					/* 4A0 */ virtual void DoTrapData(void* trapData) noexcept(true);
					/* 4A8 */ virtual void SetEquipStateLocked(TESBoundObject* object, bool lock) noexcept(true);
				private:
					/* 4B0 */ virtual void SendContainerChangedEvent(); // name by CommonLibF4 slot order; empty body, arguments not recovered
					/* 4B8 */ virtual void UnequipArmorFromSlot(); // name by CommonLibF4 slot order; empty body, arguments not recovered
					/* 4C0 */ virtual void RemoveAllObjectsWorn(); // name by CommonLibF4 slot order; the body frees the inventory list at 108
					/* 4C8 */ virtual void PostRemoveWornObject(); // name by CommonLibF4 slot order; empty body, arguments not recovered
					/* 4D0 */ virtual void Set3DUpdateFlag(); // name by CommonLibF4 slot order; empty body, arguments not recovered
					/* 4D8 */ virtual void ClearAll3DUpdateFlags(); // name by CommonLibF4 slot order; empty body
					/* 4E0 */ virtual float AdjustItemWeight(void* object, const void* stack, float weight); // returns its float argument (CommonLibF4 also has a trailing bool* that the body ignores)
				public:
					/* 4E8 */ [[nodiscard]] virtual bool IsChild() const noexcept(true);
					/* 4F0 */ [[nodiscard]] virtual TESForm* GetTemplateActorBase() noexcept(true);
					/* 4F8 */ virtual void SetTemplateActorBase(TESForm* actorBase) noexcept(true);
					/* 500 */ [[nodiscard]] virtual BSFadeNode* GetFaceNodeSkinned() noexcept(true);
					/* 508 */ [[nodiscard]] virtual BSFadeNode* GetFaceNode() noexcept(true);
					/* 510 */ [[nodiscard]] virtual bool ClampToGround() noexcept(true);
					/* 518 */ [[nodiscard]] virtual bool DetachHavok(NiAPI::NiAVObject* obj3D) noexcept(true);
					/* 520 */ virtual void InitHavok() noexcept(true);
					/* 528 */ [[nodiscard]] virtual NiAPI::NiAVObject* Load3D(bool backgroundLoading) noexcept(true);
					/* 530 */ virtual void Release3DRelatedData() noexcept(true);
					/* 538 */ virtual void Set3D(NiAPI::NiAVObject* object, bool queue3DTasks) noexcept(true);
					/* 540 */ [[nodiscard]] virtual bool ShouldBackgroundClone() const noexcept(true);
					/* 548 */ [[nodiscard]] virtual bool IsReadyForAttach(const void* priority) const noexcept(true);
					/* 550 */ [[nodiscard]] virtual NiAPI::NiAVObject* Get3DFirstPerson(bool firstPerson) const noexcept(true);
					/* 558 */ [[nodiscard]] virtual NiAPI::NiAVObject* Get3D() const noexcept(true);
					/* 560 */ [[nodiscard]] virtual NiAPI::NiAVObject* GetFullyLoaded3D() const noexcept(true);
					/* 568 */ [[nodiscard]] virtual bool Is3rdPersonVisible() noexcept(true);
					/* 570 */ [[nodiscard]] virtual bool PopulateGraphProjectsToLoad(const NiAPI::NiAVObject* obj3D, void* projectFileNames) const noexcept(true);
					/* 578 */ [[nodiscard]] virtual TESForm* GetTESModel() const noexcept(true);
					/* 580 */ [[nodiscard]] virtual TESForm* GetVisualsRace() const noexcept(true);
					/* 588 */ [[nodiscard]] virtual void GetPhysicsTransform(NiAPI::NiTransform* outTransform) const noexcept(true);
					/* 590 */ [[nodiscard]] virtual NiAPI::NiPoint3 GetBoundMin() const noexcept(true);
					/* 598 */ [[nodiscard]] virtual NiAPI::NiPoint3 GetBoundMax() const noexcept(true);
					/* 5A0 */ [[nodiscard]] virtual bool GetBehaviorRootNameOverrides(void* behaviorRoots) const noexcept(true);
					/* 5A8 */ [[nodiscard]] virtual bool GetBehaviorRootNameReplacement(void* rootNameReplacement) const noexcept(true);
					/* 5B0 */ [[nodiscard]] virtual bool GetBehaviorRootData(bool onlyCollectActiveRoots, void* behaviorRoots) const noexcept(true);
					/* 5B8 */ virtual void OnHeadInitialized() noexcept(true);
					/* 5C0 */ [[nodiscard]] virtual bool InitNonNPCAnimation(NiAPI::NiNode* nodeForAnim) noexcept(true);
					/* 5C8 */ [[nodiscard]] virtual bool CheckAndFixSkinAndBoneOrder(NiAPI::NiNode* nodeToTest) const noexcept(true);
					/* 5D0 */ virtual void ModifyAnimationUpdateData(void* updateData) const noexcept(true);
					/* 5D8 */ [[nodiscard]] virtual bool ShouldSaveAnimationOnUnloading() const noexcept(true);
					/* 5E0 */ [[nodiscard]] virtual bool ShouldSaveAnimationOnSaving() const noexcept(true);
					/* 5E8 */ [[nodiscard]] virtual bool ShouldPerformRevert() const noexcept(true);
					/* 5F0 */ virtual void UpdateAnimation(float delta) noexcept(true);
					/* 5F8 */ virtual void CollectPickNodes() noexcept(true);
					/* 600 */ [[nodiscard]] virtual void* GetBipedFirstPerson(bool firstPerson) const noexcept(true);
					/* 608 */ [[nodiscard]] virtual void* GetBiped() const noexcept(true);
					/* 610 */ [[nodiscard]] virtual void* GetCurrentBiped() const noexcept(true);
					/* 618 */ virtual void SetBiped(void* biped) noexcept(true);
					/* 620 */ virtual void AttachWeapon(void* weaponInstance, std::int32_t equipIndex) noexcept(true);
					/* 628 */ virtual void RemoveWeapon(void* weaponInstance, std::int32_t equipIndex) noexcept(true);
					/* 630 */ virtual void CreateInventoryList(const TESForm* container) noexcept(true);
					/* 638 */ virtual void DestroyInventoryList() noexcept(true);
					/* 640 */ virtual void SetRunsInLow(bool runsInLow) noexcept(true);
					/* 648 */ virtual void SetObjectReference(TESBoundObject* object) noexcept(true);
					/* 650 */ virtual void MoveHavok(bool forceRec) const noexcept(true);
					/* 658 */ virtual void GetLinearVelocity(NiAPI::NiPoint3* velocity) const noexcept(true);
					/* 660 */ virtual void ApplyMovementDelta(float timeDelta, const NiAPI::NiPoint3* delta, const NiAPI::NiPoint3* angleDelta) noexcept(true);
					/* 668 */ [[nodiscard]] virtual NiAPI::NiAVObject* GetCurrent3D() const noexcept(true);
					/* 670 */ [[nodiscard]] virtual bool IsProjectile() const noexcept(true);
					/* 678 */ virtual void SetParentCell(TESObjectCELL* cell) noexcept(true);
					/* 680 */ [[nodiscard]] virtual bool IsDead() const noexcept(true);
					/* 688 */ [[nodiscard]] virtual bool ProcessInWater(std::uint32_t bodyID, float waterHeight, float deltaTime) noexcept(true);
				private:
					/* 690 */ virtual bool ApplyCurrent(); // name by CommonLibF4 slot order; body returns false, arguments not recovered
					/* 698 */ virtual void vf698(); // signature not recovered; large copy helper that asks "Duplicate contents of reference %08X's reflected/refracted lists?"
				public:
					/* 6A0 */ [[nodiscard]] virtual void* GetDecalGroup() const noexcept(true);
				private:
					/* 6A8 */ virtual void vf6A8(); // signature not recovered
				public:
					/* 6B0 */ [[nodiscard]] virtual bool HasKeywordHelper(const void* keyword, const void* instanceData) const noexcept(true); // tests the base form's keywords; instanceData falls back to the reference's extra data
				public:
					inline static void (*SetParentWithRedraw)(TESObjectREFR_base*, TESForm*);
					inline static void (*SetPosition)(TESObjectREFR_base*, const NiAPI::NiPoint3* val);
					
					inline NiAPI::NiPoint3 GetPosition() const noexcept(true) { return _position; }
					inline NiAPI::NiPoint3 GetRotate() const noexcept(true) { return _rotate; }
					inline void SetRotate(const NiAPI::NiPoint3& val) { _rotate = val; }
					
					[[nodiscard]] inline std::uint16_t GetScale() const noexcept(true) { return _scale; }
					inline void SetScale(std::uint16_t val) noexcept(true) { _scale = val; }
					inline float GetScaleFloat() const noexcept(true) { return ((float)GetScale() / 100); }
					inline void SetScaleFloat(float val) { SetScale(std::uint16_t(val * 100)); }
					
					[[nodiscard]] inline BSTSmartPointer<ExtraDataList> GetExtraData() const noexcept(true)
					{
						static_assert(offsetof(TESObjectREFR_base, _actorValueChangedEvents) == 0x70);
						static_assert(offsetof(TESObjectREFR_base, _parentCell) == 0xC8);
						static_assert(offsetof(TESObjectREFR_base, _rotate) == 0xD0);
						static_assert(offsetof(TESObjectREFR_base, _position) == 0xE0);
						static_assert(offsetof(TESObjectREFR_base, _baseForm) == 0xF0);
						static_assert(offsetof(TESObjectREFR_base, _loadedData) == 0x100);
						static_assert(offsetof(TESObjectREFR_base, _inventoryList) == 0x108);
						static_assert(offsetof(TESObjectREFR_base, _extraData) == 0x110);
						static_assert(offsetof(TESObjectREFR_base, _scale) == 0x118);
						static_assert(offsetof(TESObjectREFR_base, _modelState) == 0x11A);
						static_assert(offsetof(TESObjectREFR_base, _predestroyed) == 0x11B);
						static_assert(offsetof(TESObjectREFR_base, _unk120) == 0x120);
						return _extraData;
					}
					[[nodiscard]] inline TESObjectCELL* GetParentCell() const noexcept(true) { return _parentCell; }
					[[nodiscard]] inline TESForm* GetParent() const noexcept(true) { return _baseForm; }
					[[nodiscard]] inline bool IsInvisible() const noexcept(true) { return _FormFlags.Has(fs3DInvisible); }
					[[nodiscard]] inline bool IsChildrenInvisible() const noexcept(true) { return _FormFlags.Has(fsChildren3DInvisible); }
					[[nodiscard]] inline bool IsFrozen() const noexcept(true) { return _FormFlags.Has(fsFrozen); }

					CKPE_READ_PROPERTY(GetParentCell) TESObjectCELL* ParentCell;
					CKPE_READ_PROPERTY(GetParent) TESForm* Parent;
					CKPE_PROPERTY(GetPosition, SetPosition) NiAPI::NiPoint3 Position;
					CKPE_PROPERTY(GetRotate, SetRotate) NiAPI::NiPoint3 Rotate;
					CKPE_PROPERTY(GetScale, SetScale) std::uint16_t Scale;
					CKPE_PROPERTY(GetScaleFloat, SetScaleFloat) float ScaleFloat;
				};

				typedef TESObjectREFR_base<BSHandleRefObject_Original> TESObjectREFR_Original;
				typedef TESObjectREFR_base<BSHandleRefObject_Extremly> TESObjectREFR_Extremly_NG;
				typedef TESObjectREFR_base<BSHandleRefObject_64_Extremly> TESObjectREFR_Extremly;

				static_assert(sizeof(TESObjectREFR_Original) == 0x130);
				static_assert(sizeof(TESObjectREFR_Extremly_NG) == 0x130);
				static_assert(sizeof(TESObjectREFR_Extremly) == 0x130);

				// IncRef and DefRef - NO USE!!!
				typedef TESObjectREFR_Original TESObjectREFR;
			}
		}
	}
}