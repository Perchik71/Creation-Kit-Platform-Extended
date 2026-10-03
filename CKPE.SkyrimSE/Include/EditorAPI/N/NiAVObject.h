// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiAVObject.h

#pragma once

#include <EditorAPI/N/NiTypes.h>
#include <EditorAPI/N/NiTSimpleArray.h>
#include <EditorAPI/N/NiTransform.h>
#include <EditorAPI/N/NiObjectNET.h>
#include <EditorAPI/N/NiCollisionObject.h>
#include <EditorAPI/N/NiFlags.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class TESObjectREFR;

			namespace NiAPI
			{
				class NiAlphaProperty;
				class NiCullingProcess;

				class NiUpdateData
				{
				public:
					enum class Flag
					{
						kNone = 0,
						kDirty = 1 << 0,
						kDisableCollision = 1 << 13
					};

					float time;
					TEnumSet<Flag, std::uint32_t> flags;
				};

				class PerformOpFunc
				{
				public:
					virtual ~PerformOpFunc();

					// add
					virtual bool operator()(NiAVObject* a_object);
				};
				static_assert(sizeof(PerformOpFunc) == 0x8);

				class NiAVObject : public NiObjectNET
				{
				public:
					enum class Flag : std::uint32_t
					{
						kNone = 0,
						kHidden = 1 << 0,
						kSelectiveUpdate = 1 << 1,
						kSelectiveUpdateTransforms = 1 << 2,
						kSelectiveUpdateController = 1 << 3,
						kSelectiveUpdateRigid = 1 << 4,
						kDisplayObject = 1 << 5,
						kDisableSorting = 1 << 6,
						kSelectiveUpdateTransformsOverride = 1 << 7,
						kSaveExternalGeometryData = 1 << 9,
						kNoDecals = 1 << 10,
						kAlwaysDraw = 1 << 11,
						kMeshLOD = 1 << 12,
						kFixedBound = 1 << 13,
						kTopFadeNode = 1 << 14,
						kIgnoreFade = 1 << 15,
						kNoAnimSyncX = 1 << 16,
						kNoAnimSyncY = 1 << 17,
						kNoAnimSyncZ = 1 << 18,
						kNoAnimSyncS = 1 << 19,
						kNoDismember = 1 << 20,
						kNoDismemberValidity = 1 << 21,
						kRenderUse = 1 << 22,
						kMaterialsApplied = 1 << 23,
						kHighDetail = 1 << 24,
						kForceUpdate = 1 << 25,
						kPreProcessedNode = 1 << 26
					};
				public:
					virtual ~NiAVObject() = default;

					// add
					virtual void UpdateControllers(NiUpdateData& a_data);
					virtual void PerformOp(PerformOpFunc& a_func);
					virtual void AttachProperty(NiAlphaProperty* a_property);
					virtual void SetMaterialNeedsUpdate(bool a_needsUpdate);
					virtual void SetDefaultMaterialNeedsUpdateFlag(bool a_flag);
					virtual NiAVObject* GetObjectByName(const BSFixedString& a_name);
					virtual void SetSelectiveUpdateFlags(bool& a_selectiveUpdate, bool a_selectiveUpdateTransforms, bool& a_rigid);
					virtual void UpdateDownwardPass(NiUpdateData& a_data, std::uint32_t a_arg2);
					virtual void UpdateSelectedDownwardPass(NiUpdateData& a_data, std::uint32_t a_arg2);
					virtual void UpdateRigidDownwardPass(NiUpdateData& a_data, std::uint32_t a_arg2);
					virtual void UpdateWorldBound();
					virtual void UpdateWorldData(NiUpdateData* a_data);
					virtual void UpdateTransformAndBounds(NiUpdateData& a_data);
					virtual void PreAttachUpdate(NiNode* a_parent, NiUpdateData& a_data);
					virtual void PostAttachUpdate();
					virtual void OnVisible(NiCullingProcess& a_process);

					[[nodiscard]] inline NiAVObject* GetParent() const noexcept(true) { return _Parent; }
					[[nodiscard]] inline const NiTransform& GetLocalTransform() const noexcept(true) { return _Local; }
					[[nodiscard]] inline const NiTransform& GetWorldTransform() const noexcept(true) { return _World; }
					[[nodiscard]] inline const NiTransform& GetPreviousWorldTransform() const noexcept(true) { return _PreviousWorld; }
					[[nodiscard]] inline const NiPoint3& GetWorldTranslate() const noexcept(true) { return _World.m_Translate; }
					[[nodiscard]] inline const TESObjectREFR* GetFormRef() const noexcept(true) { return FormRef; }

					[[nodiscard]] inline bool HasHidden() const noexcept(true) { return _Flags.all(Flag::kHidden); }
					[[nodiscard]] inline bool HasSelectiveUpdate() const noexcept(true) { return _Flags.all(Flag::kSelectiveUpdate); }
					[[nodiscard]] inline bool HasSelectiveUpdateTransforms() const noexcept(true) { return _Flags.all(Flag::kSelectiveUpdateTransforms); }
					[[nodiscard]] inline bool HasSelectiveUpdateController() const noexcept(true) { return _Flags.all(Flag::kSelectiveUpdateController); }
					[[nodiscard]] inline bool HasSelectiveUpdateRigid() const noexcept(true) { return _Flags.all(Flag::kSelectiveUpdateRigid); }
					[[nodiscard]] inline bool HasDisplayObject() const noexcept(true) { return _Flags.all(Flag::kDisplayObject); }
					[[nodiscard]] inline bool HasDisableSorting() const noexcept(true) { return _Flags.all(Flag::kDisableSorting); }
					[[nodiscard]] inline bool HasSelectiveUpdateTransformsOverride() const noexcept(true) { return _Flags.all(Flag::kSelectiveUpdateTransformsOverride); }
					[[nodiscard]] inline bool HasSaveExternalGeometryData() const noexcept(true) { return _Flags.all(Flag::kSaveExternalGeometryData); }
					[[nodiscard]] inline bool HasNoDecals() const noexcept(true) { return _Flags.all(Flag::kNoDecals); }
					[[nodiscard]] inline bool HasAlwaysDraw() const noexcept(true) { return _Flags.all(Flag::kAlwaysDraw); }
					[[nodiscard]] inline bool HasMeshLOD() const noexcept(true) { return _Flags.all(Flag::kMeshLOD); }
					[[nodiscard]] inline bool HasFixedBound() const noexcept(true) { return _Flags.all(Flag::kFixedBound); }
					[[nodiscard]] inline bool HasTopFadeNode() const noexcept(true) { return _Flags.all(Flag::kTopFadeNode); }
					[[nodiscard]] inline bool HasIgnoreFade() const noexcept(true) { return _Flags.all(Flag::kIgnoreFade); }
					[[nodiscard]] inline bool HasNoAnimSyncX() const noexcept(true) { return _Flags.all(Flag::kNoAnimSyncX); }
					[[nodiscard]] inline bool HasNoAnimSyncY() const noexcept(true) { return _Flags.all(Flag::kNoAnimSyncY); }
					[[nodiscard]] inline bool HasNoAnimSyncZ() const noexcept(true) { return _Flags.all(Flag::kNoAnimSyncZ); }
					[[nodiscard]] inline bool HasNoAnimSyncS() const noexcept(true) { return _Flags.all(Flag::kNoAnimSyncS); }
					[[nodiscard]] inline bool HasNoDismember() const noexcept(true) { return _Flags.all(Flag::kNoDismember); }
					[[nodiscard]] inline bool HasNoDismemberValidity() const noexcept(true) { return _Flags.all(Flag::kNoDismemberValidity); }
					[[nodiscard]] inline bool HasRenderUse() const noexcept(true) { return _Flags.all(Flag::kRenderUse); }
					[[nodiscard]] inline bool HasMaterialsApplied() const noexcept(true) { return _Flags.all(Flag::kMaterialsApplied); }
					[[nodiscard]] inline bool HasHighDetail() const noexcept(true) { return _Flags.all(Flag::kHighDetail); }
					[[nodiscard]] inline bool HasForceUpdate() const noexcept(true) { return _Flags.all(Flag::kForceUpdate); }
					[[nodiscard]] inline bool HasPreProcessedNode() const noexcept(true) { return _Flags.all(Flag::kPreProcessedNode); }

					inline void SetAsHidden() noexcept(true) { _Flags.set(Flag::kHidden); }
					inline void SetAsSelectiveUpdate() noexcept(true) { _Flags.set(Flag::kSelectiveUpdate); }
					inline void SetAsSelectiveUpdateTransforms() noexcept(true) { _Flags.set(Flag::kSelectiveUpdateTransforms); }
					inline void SetAsSelectiveUpdateController() noexcept(true) { _Flags.set(Flag::kSelectiveUpdateController); }
					inline void SetAsSelectiveUpdateRigid() noexcept(true) { _Flags.set(Flag::kSelectiveUpdateRigid); }
					inline void SetAsDisplayObject() noexcept(true) { _Flags.set(Flag::kDisplayObject); }
					inline void SetAsDisableSorting() noexcept(true) { _Flags.set(Flag::kDisableSorting); }
					inline void SetAsSelectiveUpdateTransformsOverride() noexcept(true) { _Flags.set(Flag::kSelectiveUpdateTransformsOverride); }
					inline void SetAsSaveExternalGeometryData() noexcept(true) { _Flags.set(Flag::kSaveExternalGeometryData); }
					inline void SetAsNoDecals() noexcept(true) { _Flags.set(Flag::kNoDecals); }
					inline void SetAsAlwaysDraw() noexcept(true) { _Flags.set(Flag::kAlwaysDraw); }
					inline void SetAsMeshLOD() noexcept(true) { _Flags.set(Flag::kMeshLOD); }
					inline void SetAsFixedBound() noexcept(true) { _Flags.set(Flag::kFixedBound); }
					inline void SetAsTopFadeNode() noexcept(true) { _Flags.set(Flag::kTopFadeNode); }
					inline void SetAsIgnoreFade() noexcept(true) { _Flags.set(Flag::kIgnoreFade); }
					inline void SetAsNoAnimSyncX() noexcept(true) { _Flags.set(Flag::kNoAnimSyncX); }
					inline void SetAsNoAnimSyncY() noexcept(true) { _Flags.set(Flag::kNoAnimSyncY); }
					inline void SetAsNoAnimSyncZ() noexcept(true) { _Flags.set(Flag::kNoAnimSyncZ); }
					inline void SetAsNoAnimSyncS() noexcept(true) { _Flags.set(Flag::kNoAnimSyncS); }
					inline void SetAsNoDismember() noexcept(true) { _Flags.set(Flag::kNoDismember); }
					inline void SetAsNoDismemberValidity() noexcept(true) { _Flags.set(Flag::kNoDismemberValidity); }
					inline void SetAsRenderUse() noexcept(true) { _Flags.set(Flag::kRenderUse); }
					inline void SetAsMaterialsApplied() noexcept(true) { _Flags.set(Flag::kMaterialsApplied); }
					inline void SetAsHighDetail() noexcept(true) { _Flags.set(Flag::kHighDetail); }
					inline void SetAsForceUpdate() noexcept(true) { _Flags.set(Flag::kForceUpdate); }
					inline void SetAsPreProcessedNode() noexcept(true) { _Flags.set(Flag::kPreProcessedNode); }

					[[nodiscard]] inline float GetFadeAmount() const noexcept(true) { return fadeAmount; }
					[[nodiscard]] inline std::uint32_t GetLastUpdatedFrameCounter() const noexcept(true) { return lastUpdatedFrameCounter; }

				private:
					NiAVObject* _Parent;
					std::uint32_t parentIndex;
					NiPointer<NiCollisionObject> collisionObject;
					NiTransform _Local;
					NiTransform _World;
					NiTransform _PreviousWorld;
					NiBound _WorldBound;
					TEnumSet<Flag, std::uint32_t> _Flags;
					TESObjectREFR* FormRef;
					float fadeAmount;
					std::uint32_t lastUpdatedFrameCounter;
					std::uint8_t unk108;
					std::uint8_t flags02;
					std::uint16_t unk10A;
					std::uint32_t pad10C;
				};
				static_assert(sizeof(NiAVObject) == 0x110);
			}
		}
	}
}