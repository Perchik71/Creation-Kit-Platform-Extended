// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESChildCell.h>
#include <EditorAPI/F/FormInterfaces.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x290, vtable RVA 0x30FBD40, 124 virtual functions
				class NavMesh :
					public TESForm,       // 000
					public TESChildCell,  // 028
					public BSNavmesh      // 030
				{
				public:
					virtual ~NavMesh() = default;

				public:
					std::uint32_t unk258; // 258
					std::uint32_t unk25C; // 25C
					BSTArray<void*> unk260; // 260 - element type not identified yet
					BSTArray<void*> unk278; // 278 - element type not identified yet
				};
				static_assert(sizeof(NavMesh) == 0x290);
			}
		}
	}
}
