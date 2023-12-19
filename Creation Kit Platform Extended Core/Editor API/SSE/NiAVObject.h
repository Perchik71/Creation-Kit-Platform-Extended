// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiAVObject.h

#pragma once

#include "..\NiTransform.h"
#include "NiObjectNET.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			struct NiBound
			{
				NiPoint3 m_kCenter;
				union
				{
					float m_fRadius;
					int m_iRadiusAsInt;
				};
			};

			class NiAVObject : public NiObjectNET
			{
			public:
				enum : uint32_t
				{
					APP_CULLED = 1 << 0,
					ALWAYS_DRAW = 1 << 11,
					PREPROCESSED = 1 << 12,
					NOT_VISIBLE = 1 << 20,
					ACCUMULATED = 1 << 26,
				};

				virtual ~NiAVObject();
				virtual void UpdateControllers();
				virtual void PerformOp();
				virtual void Unk0();
				virtual void Unk1();
				virtual void Unk2();
				virtual void Unk3();
				virtual void SetSelectiveUpdateFlags();
				virtual void UpdateDownwardPass();
				virtual void UpdateSelectedDownwardPass();
				virtual void UpdateRigidDownwardPass();
				virtual void UpdateWorldBound();
				virtual void UpdateWorldData();
				virtual void Unk4();
				virtual void Unk5();
				virtual void Unk6();
				virtual void OnVisible(class NiCullingProcess* Process, uint32_t Unknown);

				char _pad0[0x4C];
				NiTransform m_kWorld;
				NiTransform m_kPreviousWorld;
				NiBound m_kWorldBound;
				uint32_t m_uFlags;
				char _pad2[0x18];

				inline const NiTransform& GetWorldTransform() const { return m_kWorld; }
				inline const NiTransform& GetPreviousWorldTransform() const { return m_kPreviousWorld; }
				inline const NiPoint3& GetWorldTranslate() const { return m_kWorld.m_Translate; }
				inline bool QAppCulled() const { return (m_uFlags & APP_CULLED) != 0; }
				inline bool QAlwaysDraw() const { return (m_uFlags & ALWAYS_DRAW) != 0; }
				inline bool QPreProcessedNode() const { return (m_uFlags & PREPROCESSED) != 0; }
				inline bool QNotVisible() const { return (m_uFlags & NOT_VISIBLE) != 0; }
				inline bool QAccumulated() const { return (m_uFlags & ACCUMULATED) != 0; }
				inline int IsVisualObjectI() const { return m_kWorldBound.m_iRadiusAsInt; }
				
				inline void SetAppCulled(bool Culled)
				{
					if (Culled)
						m_uFlags |= APP_CULLED;
					else
						m_uFlags &= ~APP_CULLED;
				}

				inline void SetAccumulated(bool Accumulated)
				{
					if (Accumulated)
						m_uFlags |= ACCUMULATED;
					else
						m_uFlags &= ~ACCUMULATED;
				}
			};
			static_assert(sizeof(NiAVObject) == 0x110);
		}
	}
}