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

					[[nodiscard]] inline bool QAppCulled() const noexcept(true) { return _Flags.Has(APP_CULLED); }
					[[nodiscard]] inline bool QNotVisible() const noexcept(true) { return _Flags.Has(NOT_VISIBLE); }

					[[nodiscard]] inline int IsVisualObjectI() const noexcept(true) { return _WorldBound.m_iRadiusAsInt; }

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
				private:
					NiAVObject* _Parent;
					NiTransform _Local;
					NiTransform _World;
					NiBound _WorldBound;
					NiTransform _PreviousWorld;
					NiPointer<NiCollisionObject> _CollisionObject;
					NiTFlags<std::uint64_t, NiAVObject> _Flags;
					void* _FormRef;
					float _FadeAmount;
					std::int8_t _MultType;
					std::int8_t _MeshLODFadingLevel;
					std::int8_t _CurrentMeshLODLevel;
					std::int8_t _PreviousMeshLODLevel;
				};
				static_assert(sizeof(NiAVObject) == 0x120);
			}
		}
	}
}