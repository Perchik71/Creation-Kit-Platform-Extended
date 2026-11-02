// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/T/TESObjectACTI.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.7.99.0: size 0x148, vtable RVA 0x30EFBE0
				class BGSTalkingActivator :
					public TESObjectACTI
				{
				public:
					constexpr static Common::ID RTTI = IDs::RTTI::BGSTalkingActivator;
					constexpr static Common::ID VTABLE = IDs::VTABLE::BGSTalkingActivator[0];

					constexpr static std::uint8_t TYPE_ID = ftTalkingActivator;

					virtual ~BGSTalkingActivator() = default;


				public:
					void* unk138;			// 138
					void* unk140;			// 140 - VNAM
				};
				static_assert(offsetof(BGSTalkingActivator, unk138) == 0x138);
				static_assert(sizeof(BGSTalkingActivator) == 0x148);
			}
		}
	}
}