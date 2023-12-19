// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiTransform.h

#pragma once

#include "NiMatrix.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		class NiTransform
		{
		public:
			NiMatrix3 m_Rotate;
			NiPoint3 m_Translate;
			float m_fScale;

			inline NiTransform() : m_fScale(1.0f) {}
			inline NiTransform(const NiTransform& Src) : m_Rotate(Src.m_Rotate), m_Translate(Src.m_Translate), 
				m_fScale(Src.m_fScale)
			{}

			void Invert(NiTransform& Dest) const
			{
				Dest.m_Rotate = m_Rotate.Transpose();
				Dest.m_fScale = 1.0f / m_fScale;
				Dest.m_Translate = Dest.m_fScale * (Dest.m_Rotate * -m_Translate);
			}

			inline NiTransform operator* (const NiTransform& xform) const
			{
				NiTransform res;

				res.m_fScale = m_fScale * xform.m_fScale;
				res.m_Rotate = m_Rotate * xform.m_Rotate;
				res.m_Translate = m_Translate + m_fScale * (m_Rotate * xform.m_Translate);

				return res;
			}
		};
		static_assert(sizeof(NiTransform) == 0x34);
	}
}