// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BaseFormComponent.h>
#include <EditorAPI/N/NiPoint.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// verified in the CK: TESContainer::ClearDataComponent frees 0x18 bytes per entry (+16 points to a 0x28 byte extra)
			struct ContainerObject
			{
				std::int32_t count;			// 00
				std::int32_t pad04;			// 04
				Forms::TESBoundObject* obj;	// 08
				void* itemExtra;			// 10 - ContainerItemExtra*
			};
			static_assert(sizeof(ContainerObject) == 0x18);


			namespace Forms
			{
				class TESBoundObject;
			}


			// 1.11.240.0: size 0x78, vtable RVA 0x301C938, 22 virtual functions
			class TESContainer :
				public BaseFormComponent   // 000
			{
			public:
				virtual ~TESContainer() = default;

				virtual void sub_0A8() { return; } // 21 - RVA 0x8B5870

			public:
				ContainerObject** containerObjects; // 08
				std::uint32_t numContainerObjects; // 10
				std::uint32_t unk14; // 14
				BSTArray<void*> unk18; // 18 - element type not identified yet
				BSTArray<void*> unk30; // 30 - element type not identified yet
				BSTArray<void*> unk48; // 48 - element type not identified yet
				BSTArray<void*> unk60; // 60 - element type not identified yet
			};
			static_assert(sizeof(TESContainer) == 0x78);
		}
	}
}
