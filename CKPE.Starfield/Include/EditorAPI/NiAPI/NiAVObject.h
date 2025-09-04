// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiAVObject.h

#pragma once

#include <EditorAPI/NiAPI/NiTypes.h>
#include <EditorAPI/NiAPI/NiTSimpleArray.h>
#include <EditorAPI/NiAPI/NiTransform.h>
#include <EditorAPI/NiAPI/NiObjectNET.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiStringExtraData
				{};

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
					[[nodiscard]] inline const std::uint32_t GetFormRef() const noexcept(true) { return FormRefID; }

					[[nodiscard]] inline bool QAppCulled() const noexcept(true) { return (m_uFlags & APP_CULLED) != 0; }
					[[nodiscard]] inline bool QNotVisible() const noexcept(true) { return (m_uFlags & NOT_VISIBLE) != 0; }

					inline void SetAppCulled(bool Culled) noexcept(true)
					{
						if (Culled)
							m_uFlags |= APP_CULLED;
						else
							m_uFlags &= ~APP_CULLED;
					}

					inline void SetNotVisible(bool Culled) noexcept(true)
					{
						if (Culled)
							m_uFlags |= NOT_VISIBLE;
						else
							m_uFlags &= ~NOT_VISIBLE;
					}
				private:
					NiTSimpleArray<NiStringExtraData*>* _ArrayStringExtraData;
					NiAVObject* _Parent;
					NiTransform _Local;
					NiTransform _World;
					NiTransform _PreviousWorld;
					char padC0[0x18];
					std::uint64_t m_uFlags;
					std::uint32_t FormRefID;
					char pad124[0x1C];
				};
				static_assert(sizeof(NiAVObject) == 0x140);
			}
		}
	}
}