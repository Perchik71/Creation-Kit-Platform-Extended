// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "TESForm.h"
#include "../NiAPI/NiPoint.h"

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSTransform : public TESForm
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftTransform;

					NiAPI::NiPoint3 Position;	// 00
					NiAPI::NiPoint3 Rotation;	// 0C
					float Scale;				// 18
					float MinZoom;				// 1C
					float MaxZoom;				// 20
				protected:
					char pad[0x14];				// 24
				};
				static_assert(offsetof(BGSTransform, Rotation) == 0x124);
				static_assert(sizeof(BGSTransform) == 0x150);
			}
		}
	}
}