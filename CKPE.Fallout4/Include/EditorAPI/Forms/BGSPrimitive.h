// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

//#include <EditorAPI/NiAPI/NiPointer.h>
//#include <EditorAPI/NiAPI/NiPoint.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// RVA + 0x3894BB0
				class BGSPrimitive
				{
				public:
					enum
					{
						kNone = 0,
						kBox,
						kSphere,
						kPlane,
						kLine,
						kEllipsoid
					};

					virtual ~BGSPrimitive();  // 00
				};

				// RVA + 0x3894C58
				class BGSPrimitiveBox : public BGSPrimitive
				{
				};
			}
		}
	}
}

