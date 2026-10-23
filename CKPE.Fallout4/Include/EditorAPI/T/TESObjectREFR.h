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

					inline void Predestroy() noexcept(true) { CKPE::vtbl_call<void>(0x3E0, this); }
					inline void PreModelLoaderQueueing() noexcept(true) { CKPE::vtbl_call<void>(0x3E8, this); }
					[[nodiscard]] inline void* GetEditorLocation() const noexcept(true) { return CKPE::vtbl_call<void*>(0x3F0, this); }
					[[nodiscard]] inline bool GetEditorLocation(NiAPI::NiPoint3* originalLocation, NiAPI::NiPoint3* originalAngle,
						TESForm** locationFormOut, TESForm* locationForm) noexcept(true) 
					{ return CKPE::vtbl_call<bool>(0x3F8, this, originalLocation, originalAngle, locationFormOut, locationForm); }

					inline void AddDynamicIdleCRC(std::uint32_t crc) noexcept(true) { CKPE::vtbl_call<void>(0x400, this, crc); }
					inline void GetPreviousDynamicIdleCRCS(std::uint32_t* crcs, std::uint32_t requestSize) const noexcept(true)
					{ CKPE::vtbl_call<void>(0x408, this, crcs, requestSize); }

					[[nodiscard]] inline bool GetFullLODRef() const noexcept(true) { return CKPE::vtbl_call<bool>(0x410, this); }
					inline void SetFullLODRef(bool val) noexcept(true) { CKPE::vtbl_call<void>(0x418, this, val); }
					[[nodiscard]] inline void* GetSequencer() const noexcept(true) { return CKPE::vtbl_call<void*>(0x420, this); }
					inline void SetActorCause(void* actorCause) noexcept(true) { CKPE::vtbl_call<void>(0x440, this, actorCause); }
					[[nodiscard]] inline void* GetActorCause() const noexcept(true) { return CKPE::vtbl_call<void*>(0x448, this); }
					[[nodiscard]] inline float GetActorWidthOrRefDiameter() const noexcept(true) { return CKPE::vtbl_call<float>(0x450, this); }
					[[nodiscard]] inline float GetActorHeightOrRefBound() const noexcept(true) { return CKPE::vtbl_call<float>(0x458, this); }
					[[nodiscard]] inline NiAPI::NiPoint3 GetStartingRotate() const noexcept(true) { return CallPoint3Virtual(0x460); }
					[[nodiscard]] inline NiAPI::NiPoint3 GetStartingPosition() const noexcept(true) { return CallPoint3Virtual(0x468); }
					inline void SetStartingPosition(const NiAPI::NiPoint3* position) noexcept(true) { CKPE::vtbl_call<void>(0x470, this, position); }
					inline void UpdateRefLight(float delta) noexcept(true) { CKPE::vtbl_call<void>(0x478, this, delta); }

					inline void SetEquipStateLocked(TESBoundObject* object, bool lock) noexcept(true) { CKPE::vtbl_call<void>(0x4A8, this, object, lock); }

					[[nodiscard]] inline BSFadeNode* GetFaceNodeSkinned() { return CKPE::vtbl_call<BSFadeNode*>(0x500, this); }
					[[nodiscard]] inline BSFadeNode* GetFaceNode() { return CKPE::vtbl_call<BSFadeNode*>(0x508, this); }
					[[nodiscard]] inline bool ClampToGround() { return CKPE::vtbl_call<bool>(0x510, this); }
					[[nodiscard]] inline bool DetachHavok(NiAPI::NiAVObject* obj3D) { return CKPE::vtbl_call<bool>(0x518, this, obj3D); }
					inline void InitHavok() { CKPE::vtbl_call<void>(0x520, this); }
					[[nodiscard]] inline NiAPI::NiAVObject* Load3D(bool backgroundLoading) { return CKPE::vtbl_call<NiAPI::NiAVObject*>(0x528, this, backgroundLoading); }
					inline void Release3DRelatedData() { CKPE::vtbl_call<void>(0x530, this); }
					inline void Set3D(NiAPI::NiAVObject* object, bool queue3DTasks) { CKPE::vtbl_call<void>(0x538, this, object, queue3DTasks); }
					[[nodiscard]] inline bool ShouldBackgroundClone() const noexcept(true) { return CKPE::vtbl_call<bool>(0x540, this); }
					[[nodiscard]] inline bool IsReadyForAttach(const void* priority) const noexcept(true) { return CKPE::vtbl_call<bool>(0x548, this, priority); }
					[[nodiscard]] inline NiAPI::NiAVObject* Get3D() const noexcept(true) { return CKPE::vtbl_call<NiAPI::NiAVObject*>(0x558, this); }
					// 1.11.246.0: slot 0x550 is Get3D(bool) and returns slot 0x558 (Get3D())
					[[nodiscard]] inline NiAPI::NiAVObject* Get3D(bool firstPerson) const noexcept(true) { return CKPE::vtbl_call<NiAPI::NiAVObject*>(0x550, this, firstPerson); }
					[[nodiscard]] inline NiAPI::NiAVObject* GetFullyLoaded3D() const noexcept(true) { return CKPE::vtbl_call<NiAPI::NiAVObject*>(0x560, this); }
					[[nodiscard]] inline bool Is3rdPersonVisible() noexcept(true) { return CKPE::vtbl_call<bool>(0x568, this); }

					[[nodiscard]] inline TESForm* GetTESModel() const noexcept(true) { return CKPE::vtbl_call<TESForm*>(0x578, this); }
					[[nodiscard]] inline TESForm* GetVisualsRace() const noexcept(true) { return CKPE::vtbl_call<TESForm*>(0x580, this); }
					[[nodiscard]] inline void GetPhysicsTransform(NiAPI::NiTransform* outTransform) const noexcept(true) { CKPE::vtbl_call<void>(0x588, this, outTransform); }

					[[nodiscard]] inline NiAPI::NiPoint3 GetBoundMin() const noexcept(true) { return CallPoint3Virtual(0x590); }
					[[nodiscard]] inline NiAPI::NiPoint3 GetBoundMax() const noexcept(true) { return CallPoint3Virtual(0x598); }

					inline void OnHeadInitialized() noexcept(true) { CKPE::vtbl_call<void>(0x5B8, this); }
					[[nodiscard]] inline bool InitNonNPCAnimation(NiAPI::NiNode* nodeForAnim) noexcept(true) { return CKPE::vtbl_call<bool>(0x5C0, this, nodeForAnim); }
					[[nodiscard]] inline bool CheckAndFixSkinAndBoneOrder(NiAPI::NiNode* nodeToTest) const noexcept(true) { return CKPE::vtbl_call<bool>(0x5C8, this, nodeToTest); }
					inline void ModifyAnimationUpdateData(void* updateData) const noexcept(true) { CKPE::vtbl_call<void>(0x5D0, this, updateData); }
					[[nodiscard]] inline bool ShouldSaveAnimationOnUnloading() const noexcept(true) { return CKPE::vtbl_call<bool>(0x5D8, this); }
					[[nodiscard]] inline bool ShouldSaveAnimationOnSaving() const noexcept(true) { return CKPE::vtbl_call<bool>(0x5E0, this); }
					[[nodiscard]] inline bool ShouldPerformRevert() const noexcept(true) { return CKPE::vtbl_call<bool>(0x5E8, this); }
					inline void UpdateAnimation(float delta) noexcept(true) { CKPE::vtbl_call<void>(0x5F0, this, delta); }
					inline void CollectPickNodes() noexcept(true) { CKPE::vtbl_call<void>(0x5F8, this); }

					inline void CreateInventoryList(const TESForm* container) noexcept(true) { CKPE::vtbl_call<void>(0x630, this, container); }
					inline void DestroyInventoryList() noexcept(true) { CKPE::vtbl_call<void>(0x638, this); }
					// Verified against 1.11.246.0 (bodies and call chains). Slots 0x4B0-0x4D8, 0x4E0 and 0x538 are not identified yet.
					inline std::uint32_t RemoveItem(void* removeItemData) noexcept(true) { return CKPE::vtbl_call<std::uint32_t>(0x480, this, removeItemData); }
					inline void AddWornOutfitImpl(void* outfit, bool queueItem) noexcept(true) { CKPE::vtbl_call<void>(0x488, this, outfit, queueItem); }
					inline bool AddWornItem(TESBoundObject* object, void* extra, std::int32_t number, bool forceEquip, std::int32_t equipSlot) noexcept(true)
					{ return CKPE::vtbl_call<bool>(0x490, this, object, extra, number, forceEquip, equipSlot); }
					inline void DoTrap(void* trap, void* target) noexcept(true) { CKPE::vtbl_call<void>(0x498, this, trap, target); }
					inline void DoTrapData(void* trapData) noexcept(true) { CKPE::vtbl_call<void>(0x4A0, this, trapData); }
					[[nodiscard]] inline bool IsChild() const noexcept(true) { return CKPE::vtbl_call<bool>(0x4E8, this); }
					[[nodiscard]] inline TESForm* GetTemplateActorBase() noexcept(true) { return CKPE::vtbl_call<TESForm*>(0x4F0, this); }
					inline void SetTemplateActorBase(TESForm* actorBase) noexcept(true) { CKPE::vtbl_call<void>(0x4F8, this, actorBase); }
					[[nodiscard]] inline bool PopulateGraphProjectsToLoad(const NiAPI::NiAVObject* obj3D, void* projectFileNames) const noexcept(true)
					{ return CKPE::vtbl_call<bool>(0x570, this, obj3D, projectFileNames); }
					[[nodiscard]] inline bool GetBehaviorRootNameOverrides(void* behaviorRoots) const noexcept(true) { return CKPE::vtbl_call<bool>(0x5A0, this, behaviorRoots); }
					[[nodiscard]] inline bool GetBehaviorRootNameReplacement(void* rootNameReplacement) const noexcept(true) { return CKPE::vtbl_call<bool>(0x5A8, this, rootNameReplacement); }
					[[nodiscard]] inline bool GetBehaviorRootData(bool onlyCollectActiveRoots, void* behaviorRoots) const noexcept(true)
					{ return CKPE::vtbl_call<bool>(0x5B0, this, onlyCollectActiveRoots, behaviorRoots); }
					// 0x600 is GetBiped(bool) and returns 0x608 (GetBiped()); 0x610 GetCurrentBiped() also returns GetBiped()
					[[nodiscard]] inline void* GetBiped(bool firstPerson) const noexcept(true) { return CKPE::vtbl_call<void*>(0x600, this, firstPerson); }
					[[nodiscard]] inline void* GetBiped() const noexcept(true) { return CKPE::vtbl_call<void*>(0x608, this); }
					[[nodiscard]] inline void* GetCurrentBiped() const noexcept(true) { return CKPE::vtbl_call<void*>(0x610, this); }
					inline void SetBiped(void* biped) noexcept(true) { CKPE::vtbl_call<void>(0x618, this, biped); }
					inline void AttachWeapon(void* weaponInstance, std::int32_t equipIndex) noexcept(true) { CKPE::vtbl_call<void>(0x620, this, weaponInstance, equipIndex); }
					inline void RemoveWeapon(void* weaponInstance, std::int32_t equipIndex) noexcept(true) { CKPE::vtbl_call<void>(0x628, this, weaponInstance, equipIndex); }
					inline void SetRunsInLow(bool runsInLow) noexcept(true) { CKPE::vtbl_call<void>(0x640, this, runsInLow); }
					inline void SetObjectReference(TESBoundObject* object) noexcept(true) { CKPE::vtbl_call<void>(0x648, this, object); }

					inline void MoveHavok(bool forceRec) const noexcept(true) { CKPE::vtbl_call<void>(0x650, this, forceRec); }
					inline void GetLinearVelocity(NiAPI::NiPoint3* velocity) const noexcept(true) { CKPE::vtbl_call<void>(0x658, this, velocity); }
					inline void ApplyMovementDelta(float timeDelta, const NiAPI::NiPoint3* delta, const NiAPI::NiPoint3* angleDelta) noexcept(true)
					{ CKPE::vtbl_call<void>(0x660, this, timeDelta, delta, angleDelta); }

					[[nodiscard]] inline NiAPI::NiAVObject* GetCurrent3D() const noexcept(true) { return Get3D(); }	// 668

					inline void SetParentCell(TESObjectCELL* cell) noexcept(true) { CKPE::vtbl_call<bool>(0x678, this, cell); }
					[[nodiscard]] inline bool IsDead() const noexcept(true) { return CKPE::vtbl_call<bool>(0x680, this); }
					[[nodiscard]] inline bool ProcessInWater(std::uint32_t bodyID, float waterHeight, float deltaTime) noexcept(true) 
					{ return CKPE::vtbl_call<bool>(0x688, this, bodyID, waterHeight, deltaTime); }
					
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