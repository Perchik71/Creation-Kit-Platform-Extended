// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Utils.h>
#include "..\\BSTEvents.h"
#include "..\\BSFadeNode.h"
#include <EditorAPI/NiAPI/NiTypes.h>
#include <EditorAPI/NiAPI/NiPointer.h>
#include "..\\BSHandleRefObject.h"
#include "..\\BSExtraData.h"
#include "..\\BSTSmartPointer.h"
#include "TESForm.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObjectCELL;
				class TESBoundObject;

				template<class _Ty>
				class TESObjectREFR_base : public TESForm, public BSTEventSink<void*>, public _Ty, 
					public BSTEventSink<BSActiveGraphIfInactiveEvent>, public BSTEventSink<BSAnimationGraphEvent>, 
					public BSTEventSink<BGSInventoryListEvent::Event>
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
					char _pad58[0x70];							// 0x58
					TESObjectCELL* _parentCell;					// 0xC8
					NiAPI::NiPoint3A _rotate;					// 0xD0, 0xD4, 0xD8, 0xDC
					NiAPI::NiPoint3A _position;					// 0xE0, 0xE4, 0xE8, 0xEC
					TESForm* _baseForm;							// 0xF0
					void* _unkB8;								// 0xF8	// inventory			
					void* _unkC0;								// 0x100
					void* _unkC8;								// 0x108
					//void* _unk11;
					BSTSmartPointer<ExtraDataList> _extraData;	// 0x110
					std::uint16_t _scale;						// 0x118
					std::uint8_t _modelState;					// 0x10A
					bool _predestroyed;							// 0x10B
					char _pad11A[0x14];							// 0x11A
				public:
					virtual ~TESObjectREFR_base() = default;

					inline void Predestroy() noexcept(true) { CKPE::vtbl_call<void>(0x3E0, this); }
					inline void PreModelLoaderQueueing() noexcept(true) { CKPE::vtbl_call<void>(0x3E8, this); }
					[[nodiscard]] inline void* GetEditorLocation() const noexcept(true) { return CKPE::vtbl_call<void*>(0x3F0, this); }
					[[nodiscard]] inline bool GetEditorLocation(NiAPI::NiPoint3* originalLocation, NiAPI::NiPoint3* originalAngle,
						TESForm** locationFormOut, TESForm* locationForm) noexcept(true) 
					{ return CKPE::vtbl_call<bool>(0x3F8, this); }

					inline void AddDynamicIdleCRC(std::uint32_t crc) noexcept(true) { CKPE::vtbl_call<void>(0x400, this, crc); }
					inline void GetPreviousDynamicIdleCRCS(std::uint32_t* crcs, std::uint32_t requestSize) const noexcept(true)
					{ CKPE::vtbl_call<void>(0x408, this, crcs, requestSize); }

					[[nodiscard]] inline bool GetFullLODRef() const noexcept(true) { return CKPE::vtbl_call<bool>(0x410, this); }
					inline void SetFullLODRef(bool val) noexcept(true) { CKPE::vtbl_call<void>(0x418, this, val); }
					[[nodiscard]] inline void* GetSequencer() const noexcept(true) { return CKPE::vtbl_call<void*>(0x420, this); }
					inline void SetActorCause(void* actorCause) noexcept(true) { CKPE::vtbl_call<void*>(0x448, this, actorCause); }
					[[nodiscard]] inline void* GetActorCause() const noexcept(true) { return CKPE::vtbl_call<void*>(0x448, this); }
					[[nodiscard]] inline float GetActorWidthOrRefDiameter() const noexcept(true) { return CKPE::vtbl_call<float>(0x450, this); }
					[[nodiscard]] inline float GetActorHeightOrRefBound() const noexcept(true) { return CKPE::vtbl_call<float>(0x458, this); }
					[[nodiscard]] inline NiAPI::NiPoint3 GetStartingRotate() const noexcept(true) { return CKPE::vtbl_call<bool>(0x460, this); }
					[[nodiscard]] inline NiAPI::NiPoint3 GetStartingPosition() const noexcept(true) { return CKPE::vtbl_call<bool>(0x468, this); }
					inline void SetStartingPosition(const NiAPI::NiPoint3* position) noexcept(true) { CKPE::vtbl_call<void>(0x470, this, position); }
					inline void UpdateRefLight(float delta) noexcept(true) { CKPE::vtbl_call<void>(0x478, delta); }

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
					[[nodiscard]] inline NiAPI::NiAVObject* Get3D() const noexcept(true) { return CKPE::vtbl_call<NiAPI::NiAVObject*>(0x550, this); }
					[[nodiscard]] inline NiAPI::NiAVObject* Get3D(bool firstPerson) const noexcept(true) { return CKPE::vtbl_call<NiAPI::NiAVObject*>(0x558, this, firstPerson); }
					[[nodiscard]] inline NiAPI::NiAVObject* GetFullyLoaded3D() const noexcept(true) { return CKPE::vtbl_call<NiAPI::NiAVObject*>(0x560, this); }
					[[nodiscard]] inline bool Is3rdPersonVisible() noexcept(true) { return CKPE::vtbl_call<bool>(0x568, this); }

					[[nodiscard]] inline TESForm* GetTESModel() const noexcept(true) { return CKPE::vtbl_call<TESForm*>(0x578, this); }
					[[nodiscard]] inline TESForm* GetVisualsRace() const noexcept(true) { return CKPE::vtbl_call<TESForm*>(0x580, this); }
					[[nodiscard]] inline void GetPhysicsTransform(NiAPI::NiTransform* outTransform) const noexcept(true) { CKPE::vtbl_call<void>(0x588, this, outTransform); }

					[[nodiscard]] inline NiAPI::NiPoint3 GetBoundMin() const noexcept(true) { return CKPE::vtbl_call<NiAPI::NiPoint3>(0x590, this); }
					[[nodiscard]] inline NiAPI::NiPoint3 GetBoundMax() const noexcept(true) { return CKPE::vtbl_call<NiAPI::NiPoint3>(0x598, this); }

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
					
					inline std::uint16_t GetScale() const noexcept(true) { return *(std::uint16_t*)(((std::uintptr_t)this) + 0x118); }
					inline void SetScale(std::uint16_t val) noexcept(true) { *(std::uint16_t*)(((std::uintptr_t)this) + 0x118) = val; }
					inline float GetScaleFloat() const noexcept(true) { return ((float)GetScale() / 100); }
					inline void SetScaleFloat(float val) { SetScale(std::uint16_t(val * 100)); }
					
					[[nodiscard]] inline BSTSmartPointer<ExtraDataList> GetExtraData() const noexcept(true) { return _extraData; }
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

				// IncRef and DefRef - NO USE!!!
				typedef TESObjectREFR_Original TESObjectREFR;
			}
		}
	}
}