// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSSoundDescriptorForm;
				class TESObjectLIGH;

				struct ADDON_DATA
				{
					std::uint16_t masterParticleCap; // 00
					std::int8_t flags; // 02
				};
				static_assert(sizeof(ADDON_DATA) == 0x4);

				// 1.11.240.0: size 0x100, vtable RVA 0x307DA18, 154 virtual functions
				class BGSAddonNode :
					public TESBoundObject,        // 000
					public BGSModelMaterialSwap   // 090
				{
				public:
					virtual ~BGSAddonNode() = default;

				public:
					std::uint32_t index; // E0
					std::uint32_t unkE4; // E4
					BGSSoundDescriptorForm* sound; // E8
					TESObjectLIGH* light; // F0
					ADDON_DATA data; // F8
					std::uint8_t masterParticleLock[0x4]; // FC - BSNonReentrantSpinLock
				};
				static_assert(sizeof(BGSAddonNode) == 0x100);
			}
		}
	}
}
