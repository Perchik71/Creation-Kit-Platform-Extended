// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiAVObject.h

#pragma once

#include <EditorAPI/NiAPI/NiTypes.h>
#include <EditorAPI/NiAPI/NiTSimpleArray.h>
#include <EditorAPI/NiAPI/NiTransform.h>
#include <EditorAPI/NiAPI/NiObjectNET.h>
#include <EditorAPI/NiAPI/NiFlags.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiAVObject : public NiObjectNET
				{
				public:
					enum : std::uint32_t
					{
						APP_CULLED = (std::uint32_t)1 << 0,
						ALWAYS_DRAW = (std::uint32_t)1 << 11,
						PREPROCESSED = (std::uint32_t)1 << 12,
						NOT_VISIBLE = (std::uint32_t)1 << 20,
						ACCUMULATED = (std::uint32_t)1 << 26,
					};
				public:
					virtual ~NiAVObject() = default;

					[[nodiscard]] inline NiAVObject* GetParent() const noexcept(true) { return _Parent; }
					[[nodiscard]] inline const NiTransform& GetLocalTransform() const noexcept(true) { return _Local; }
					[[nodiscard]] inline const NiTransform& GetWorldTransform() const noexcept(true) { return _World; }
					[[nodiscard]] inline const NiTransform& GetPreviousWorldTransform() const noexcept(true) { return _PreviousWorld; }
					[[nodiscard]] inline const NiPoint3& GetWorldTranslate() const noexcept(true) { return _World.m_Translate; }
					[[nodiscard]] inline const void* GetFormRef() const noexcept(true) { return FormRef; }

					[[nodiscard]] inline bool QAppCulled() const noexcept(true) { return  _Flags.Has(APP_CULLED); }
					[[nodiscard]] inline bool QAlwaysDraw() const noexcept(true) { return  _Flags.Has(ALWAYS_DRAW); }
					[[nodiscard]] inline bool QPreProcessedNode() const noexcept(true) { return  _Flags.Has(PREPROCESSED); }
					[[nodiscard]] inline bool QNotVisible() const noexcept(true) { return  _Flags.Has(NOT_VISIBLE); }
					[[nodiscard]] inline bool QAccumulated() const noexcept(true) { return  _Flags.Has(ACCUMULATED); }

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

					inline void SetAccumulated(bool Accumulated) noexcept(true)
					{
						if (Accumulated)
							_Flags.Set(ACCUMULATED);
						else
							_Flags.Unset(ACCUMULATED);
					}
				private:
					NiAVObject* _Parent;
					char pad38[0x8];
					class bhkCollisionObject* _Collision;
					NiTransform _Local;
					NiTransform _World;
					NiTransform _PreviousWorld;
					NiBound _WorldBound;
					NiTFlags<std::uint32_t, NiAVObject> _Flags;
					void* FormRef;
					char pad100[0x10];
				};
				static_assert(sizeof(NiAVObject) == 0x110);
			}
		}
	}
}