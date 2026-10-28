// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiAVObject.h

#pragma once

#include <EditorAPI/IDs_VTABLE.h>
#include <EditorAPI/N/NiTypes.h>
#include <EditorAPI/N/NiTSimpleArray.h>
#include <EditorAPI/N/NiTransform.h>
#include <EditorAPI/N/NiObjectNET.h>
#include <EditorAPI/N/NiPointer.h>
#include <EditorAPI/N/NiCollisionObject.h>
#include <EditorAPI/N/NiFlags.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSGeometry;

			namespace NiAPI
			{
				class NiUpdateData;
				class PerformOpFunc;
				class NiAlphaProperty;
				class NiCullingProcess;

				class NiAVObject : public NiObjectNET
				{
				public:
					enum : std::uint64_t
					{
						APP_CULLED = (std::uint64_t)1 << 0,
						NOT_VISIBLE = (std::uint64_t)1 << 44,
					};
				public:
					virtual ~NiAVObject() = default;

					constexpr static Common::ID VTABLE = IDs::VTABLE::NiAVObject[0];

					/* 2A */ virtual void UpdateControllers(NiUpdateData& Data) noexcept(true);
					/* 2B */ virtual void PerformOp(PerformOpFunc& Operation) noexcept(true);
					/* 2C */ virtual void AttachProperty(NiAlphaProperty* Property) noexcept(true);
					/* 2D */ virtual void SetMaterialNeedsUpdate(bool NeedsUpdate) noexcept(true);
					/* 2E */ virtual void SetDefaultMaterialNeedsUpdateFlag(bool NeedsUpdate) noexcept(true);
					/* 2F */ virtual void SetAppCulled(bool Culled) noexcept(true);
					/* 30 */ virtual NiAVObject* GetObjectByName(const BSFixedString& Name) noexcept(true);
					/* 31 */ virtual void SetSelectiveUpdateFlags(bool& SelectiveUpdate,
						bool SelectiveUpdateTransforms, bool& Rigid) noexcept(true);
					/* 32 */ virtual void UpdateDownwardPass(NiUpdateData& Data, std::uint32_t Flags) noexcept(true);
					/* 33 */ virtual void UpdateSelectedDownwardPass(NiUpdateData& Data, std::uint32_t Flags) noexcept(true);
					/* 34 */ virtual void UpdateRigidDownwardPass(NiUpdateData& Data, std::uint32_t Flags) noexcept(true);
					/* 35 */ virtual void UpdateWorldBound() noexcept(true);
					/* 36 */ virtual void UpdateWorldData(NiUpdateData* Data) noexcept(true);
					/* 37 */ virtual void UpdateTransformAndBounds(NiUpdateData& Data) noexcept(true);
					/* 38 */ virtual void UpdateTransforms(NiUpdateData& Data) noexcept(true);
					/* 39 */ virtual void PreAttachUpdate(NiNode* Parent, NiUpdateData& Data) noexcept(true);
					/* 3A */ virtual void PostAttachUpdate() noexcept(true);
					/* 3B */ virtual void OnVisible(NiCullingProcess& Culler) noexcept(true);

					[[nodiscard]] inline NiAVObject* GetParent() const noexcept(true) { return _Parent; }
					[[nodiscard]] inline const NiTransform& GetLocalTransform() const noexcept(true) { return _Local; }
					[[nodiscard]] inline const NiTransform& GetWorldTransform() const noexcept(true) { return _World; }
					[[nodiscard]] inline const NiTransform& GetPreviousWorldTransform() const noexcept(true) { return _PreviousWorld; }
					[[nodiscard]] inline const NiPoint3& GetWorldTranslate() const noexcept(true) { return _World.m_Translate; }
					[[nodiscard]] inline const void* GetFormRef() const noexcept(true) { return _FormRef; }
					[[nodiscard]] inline std::int8_t GetMultType() const noexcept(true) { return _MultType; }
					[[nodiscard]] inline std::int8_t GetMeshLODFadingLevel() const noexcept(true) { return _MeshLODFadingLevel; }
					[[nodiscard]] inline std::int8_t GetCurrentMeshLODLevel() const noexcept(true) { return _CurrentMeshLODLevel; }
					[[nodiscard]] inline std::int8_t GetPreviousMeshLODLevel() const noexcept(true) { return _PreviousMeshLODLevel; }

					[[nodiscard]] inline bool QAppCulled() const noexcept(true) { return _Flags.Has(APP_CULLED); }
					[[nodiscard]] inline bool QNotVisible() const noexcept(true) { return _Flags.Has(NOT_VISIBLE); }

					[[nodiscard]] inline std::int32_t IsVisualObjectI() const noexcept(true) { return _WorldBound.m_iRadiusAsInt; }

					inline void SetNotVisible(bool NotVisible) noexcept(true)
					{
						if (NotVisible)
							_Flags.Set(NOT_VISIBLE);
						else
							_Flags.Unset(NOT_VISIBLE);
					}

					inline void GetViewerStrings(void(*Callback)(const char*, ...), bool Recurse = false) const noexcept(true)
					{
						((NiObjectNET*)this)->GetViewerStrings(Callback);
						if (Recurse)
						{
							NiAVObject* lhs = _Parent;
							if (lhs) lhs->GetViewerStrings(Callback, Recurse);
						}
					}
				private:
					NiAVObject* _Parent;							// 0x028
					NiTransform _Local;								// 0x030
					NiTransform _World;								// 0x070
					NiBound _WorldBound;							// 0x0B0
					NiTransform _PreviousWorld;						// 0x0C0
					NiPointer<NiCollisionObject> _CollisionObject;	// 0x100
					NiTFlags<std::uint64_t, NiAVObject> _Flags;		// 0x108
					void* _FormRef;									// 0x110
					float _FadeAmount;								// 0x118
					std::int8_t _MultType;							// 0x11C
					std::int8_t _MeshLODFadingLevel;				// 0x11D
					std::int8_t _CurrentMeshLODLevel;				// 0x11E
					std::int8_t _PreviousMeshLODLevel;				// 0x11F
				public:
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(NiAVObject, _Parent) == 0x28 &&
							offsetof(NiAVObject, _Local) == 0x30 &&
							offsetof(NiAVObject, _World) == 0x70 &&
							offsetof(NiAVObject, _WorldBound) == 0xB0 &&
							offsetof(NiAVObject, _PreviousWorld) == 0xC0 &&
							offsetof(NiAVObject, _CollisionObject) == 0x100 &&
							offsetof(NiAVObject, _Flags) == 0x108 &&
							offsetof(NiAVObject, _FormRef) == 0x110 &&
							offsetof(NiAVObject, _FadeAmount) == 0x118;
					}
				};
				static_assert(sizeof(NiAVObject) == 0x120);
				static_assert(NiAVObject::VerifyLayout());
			}
		}
	}
}