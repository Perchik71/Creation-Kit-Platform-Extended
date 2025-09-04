// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/NiAPI/NiMatrix.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiTransform
				{
				public:
					NiMatrix43 m_Rotate;
					NiPoint3 m_Translate;
					float m_fScale;

					inline NiTransform() noexcept(true) : m_fScale(1.0f) {}
					inline NiTransform(const NiTransform& Src) noexcept(true) : 
						m_Rotate(Src.m_Rotate), m_Translate(Src.m_Translate),
						m_fScale(Src.m_fScale)
					{}

					void Invert(NiTransform& Dest) const noexcept(true)
					{
						Dest.m_Rotate = m_Rotate.Transpose();
						Dest.m_fScale = 1.0f / m_fScale;
						Dest.m_Translate = Dest.m_fScale * (Dest.m_Rotate * -m_Translate);
					}

					inline NiTransform operator*(const NiTransform& xform) const noexcept(true)
					{
						NiTransform res;

						res.m_fScale = m_fScale * xform.m_fScale;
						res.m_Rotate = m_Rotate * xform.m_Rotate;
						res.m_Translate = m_Translate + m_fScale * (m_Rotate * xform.m_Translate);

						return res;
					}
				};
				static_assert(sizeof(NiTransform) == 0x40);
			}
		}
	}
}