// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/F/FormInterfaces.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.240.0: size 0x48, vtable RVA 0x3060DD0, 124 virtual functions
				class BGSReverbParameters :
					public TESForm,        // 000
					public BSIReverbType   // 028
				{
				public:
					virtual ~BGSReverbParameters() = default;

				public:
					std::uint32_t unk30; // 30
					std::uint32_t unk34; // 34
					std::uint32_t unk38; // 38
					std::uint8_t unk3C; // 3C
					std::uint8_t unk3D[0x3]; // 3D
					std::uint32_t reverbClass; // 40
					std::uint32_t unk44; // 44
				};
				static_assert(sizeof(BGSReverbParameters) == 0x48);
			}
		}
	}
}
