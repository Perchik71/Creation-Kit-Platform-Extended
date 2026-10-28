// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/T/TESBoundObject.h>
#include <EditorAPI/T/TESFullName.h>
#include <EditorAPI/B/BGSModelMaterialSwap.h>
#include <EditorAPI/B/BGSPropertySheet.h>
#include <EditorAPI/B/BGSForcedLocRefType.h>
#include <EditorAPI/F/FormInterfaces.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSMaterialObject;

				struct TESObjectSTATData
				{
					BGSMaterialObject* materialObj; // 00
					float materialThresholdAngle; // 08
					float leafAmplitude; // 0C
					float leafFrequency; // 10
				};
				static_assert(sizeof(TESObjectSTATData) == 0x18);

				// 1.11.240.0: size 0x148, vtable RVA 0x309AC88, 154 virtual functions
				class TESObjectSTAT :
					public TESBoundObject,        // 000
					public TESFullName,           // 090
					public BGSModelMaterialSwap,  // 0A8
					public BGSPropertySheet,      // 0F8
					public BGSForcedLocRefType,   // 108
					public BGSNavmeshableObject
				{
				public:
					virtual ~TESObjectSTAT() = default;

					constexpr static std::uint8_t TYPE_ID = ftStatic;
					constexpr static Common::ID VTABLE = IDs::VTABLE::TESObjectSTAT[0];
				public:
					std::uint8_t pad118[0x8]; // 118
					TESObjectSTATData data; // 120
					std::uint64_t unk138; // 138 - 0 on construction
					std::uint8_t unk140; // 140
					std::uint8_t unk141[0x7]; // 141
				};
				static_assert(offsetof(TESObjectSTAT, data) == 0x120);
				static_assert(sizeof(TESObjectSTAT) == 0x148);
			}
		}
	}
}
