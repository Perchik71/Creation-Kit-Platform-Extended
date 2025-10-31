// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiAVObject.h

#pragma once

#include <EditorAPI/NiAPI/NiTypes.h>
#include <EditorAPI/NiAPI/NiTSimpleArray.h>
#include <EditorAPI/NiAPI/NiTransform.h>
#include <EditorAPI/NiAPI/NiObjectNET.h>
#include <EditorAPI/NiAPI/NiPointer.h>
#include <EditorAPI/NiAPI/NiCollisionObject.h>
#include <EditorAPI/NiAPI/NiFlags.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
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

					inline void SetAppCulled(bool Culled) noexcept(true)
					{
						if (Culled)
							_Flags.Set(APP_CULLED);
						else
							_Flags.Unset(APP_CULLED);
					}

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
				};
				static_assert(sizeof(NiAVObject) == 0x120);
			}
		}
	}
}