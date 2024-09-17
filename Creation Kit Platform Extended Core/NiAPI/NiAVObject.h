// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiAVObject.h

#pragma once

#include "NiTypes.h"
#include "NiTSimpleArray.h"
#include "NiTransform.h"
#include "NiObjectNET.h"

namespace CreationKitPlatformExtended
{
	namespace NiAPI
	{
		class NiAVObject : public NiObjectNET
		{
		public:
			enum : uint32_t
			{
				APP_CULLED = (uint32_t)1 << 0,
				ALWAYS_DRAW = (uint32_t)1 << 11,
				PREPROCESSED = (uint32_t)1 << 12,
				NOT_VISIBLE = (uint32_t)1 << 20,
				ACCUMULATED = (uint32_t)1 << 26,
			};
		public:
			virtual ~NiAVObject() = default;

			inline NiAVObject* GetParent() const { return _Parent; }
			inline const NiTransform& GetLocalTransform() const { return _Local; }
			inline const NiTransform& GetWorldTransform() const { return _World; }
			inline const NiTransform& GetPreviousWorldTransform() const { return _PreviousWorld; }
			inline const NiPoint3& GetWorldTranslate() const { return _World.m_Translate; }
			inline const void* GetFormRef() const { return FormRef; }

			inline bool QAppCulled() const { return (m_uFlags & APP_CULLED) != 0; }
			inline bool QAlwaysDraw() const { return (m_uFlags & ALWAYS_DRAW) != 0; }
			inline bool QPreProcessedNode() const { return (m_uFlags & PREPROCESSED) != 0; }
			inline bool QNotVisible() const { return (m_uFlags & NOT_VISIBLE) != 0; }
			inline bool QAccumulated() const { return (m_uFlags & ACCUMULATED) != 0; }

			inline int IsVisualObjectI() const { return _WorldBound.m_iRadiusAsInt; }

			inline void SetAppCulled(bool Culled)
			{
				if (Culled)
					m_uFlags |= APP_CULLED;
				else
					m_uFlags &= ~APP_CULLED;
			}

			inline void SetNotVisible(bool Culled)
			{
				if (Culled)
					m_uFlags |= NOT_VISIBLE;
				else
					m_uFlags &= ~NOT_VISIBLE;
			}

			inline void SetAccumulated(bool Accumulated)
			{
				if (Accumulated)
					m_uFlags |= ACCUMULATED;
				else
					m_uFlags &= ~ACCUMULATED;
			}
		private:
			NiAVObject* _Parent;
			char pad38[0x8];
			class bhkCollisionObject* _Collision;
			NiTransform _Local;
			NiTransform _World;
			NiTransform _PreviousWorld;
			NiBound _WorldBound;
			uint32_t m_uFlags;
			void* FormRef;
			char pad100[0x10];
		};
		static_assert(sizeof(NiAVObject) == 0x110);

		namespace Fallout4
		{
			class NiAVObject : public Fallout4::NiObjectNET
			{
			public:
				enum : uint64_t
				{
					APP_CULLED = (uint64_t)1 << 0,
					NOT_VISIBLE = (uint64_t)1 << 44,
				};
			public:
				virtual ~NiAVObject() = default;

				inline NiAVObject* GetParent() const { return _Parent; }
				inline const NiTransform& GetLocalTransform() const { return _Local; }
				inline const NiTransform& GetWorldTransform() const { return _World; }
				inline const NiTransform& GetPreviousWorldTransform() const { return _PreviousWorld; }
				inline const NiPoint3& GetWorldTranslate() const { return _World.m_Translate; }
				inline const void* GetFormRef() const { return FormRef; }

				inline bool QAppCulled() const { return (m_uFlags & APP_CULLED) != 0; }
				inline bool QNotVisible() const { return (m_uFlags & NOT_VISIBLE) != 0; }

				inline void SetAppCulled(bool Culled)
				{
					if (Culled)
						m_uFlags |= APP_CULLED;
					else
						m_uFlags &= ~APP_CULLED;
				}

				inline void SetNotVisible(bool Culled)
				{
					if (Culled)
						m_uFlags |= NOT_VISIBLE;
					else
						m_uFlags &= ~NOT_VISIBLE;
				}
			private:
				NiAVObject* _Parent;
				char pad38[0x8];
				class bhkCollisionObject* _Collision;
				NiTransform _Local;
				NiTransform _World;
				NiTransform _PreviousWorld;
				char padC0[0x8];
				uint64_t m_uFlags;
				void* FormRef;
				char pad118[0x8];
			};
			static_assert(sizeof(NiAVObject) == 0x120);
		}

		namespace Starfield
		{
			class NiStringExtraData
			{};

			class NiAVObject : public Starfield::NiObjectNET
			{
			public:
				enum : uint64_t
				{
					APP_CULLED = (uint64_t)1 << 0,
					NOT_VISIBLE = (uint64_t)1 << 44,
				};
			public:
				virtual ~NiAVObject() = default;

				inline NiAVObject* GetParent() const { return _Parent; }
				inline const NiTransform& GetLocalTransform() const { return _Local; }
				inline const NiTransform& GetWorldTransform() const { return _World; }
				inline const NiTransform& GetPreviousWorldTransform() const { return _PreviousWorld; }
				inline const NiPoint3& GetWorldTranslate() const { return _World.m_Translate; }
				inline const uint32_t GetFormRef() const { return FormRefID; }

				inline bool QAppCulled() const { return (m_uFlags & APP_CULLED) != 0; }
				inline bool QNotVisible() const { return (m_uFlags & NOT_VISIBLE) != 0; }

				inline void SetAppCulled(bool Culled)
				{
					if (Culled)
						m_uFlags |= APP_CULLED;
					else
						m_uFlags &= ~APP_CULLED;
				}

				inline void SetNotVisible(bool Culled)
				{
					if (Culled)
						m_uFlags |= NOT_VISIBLE;
					else
						m_uFlags &= ~NOT_VISIBLE;
				}
			public:
				NiTSimpleArray<NiStringExtraData*>* _ArrayStringExtraData;
				NiAVObject* _Parent;
				NiTransform _Local;
				NiTransform _World;
				NiTransform _PreviousWorld;
				char padC0[0x18];
				uint64_t m_uFlags;
				uint32_t FormRefID;
				char pad124[0x1C];
			};
			static_assert(sizeof(NiAVObject) == 0x140);
		}
	}
}