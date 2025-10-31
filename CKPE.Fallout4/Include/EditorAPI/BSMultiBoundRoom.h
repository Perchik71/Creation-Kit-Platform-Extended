// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "NiAPI/NiAVObject.h"
#include "NiAPI/NiPointer.h"
#include "BSTArray.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSMultiBound;
			class BSLight;

			// BSMultiBoundRoom -> NiAPI::NiObject but this copy NiAPI::NiAVObject data
			class BSMultiBoundRoom : public NiAPI::NiAVObject		
			{
				std::uint16_t _BoundsX;
				std::uint16_t _BoundsY;
				std::uint16_t _BoundsZ;
				char _Unk1[0xA];
				NiAPI::NiPointer<BSMultiBound> _MultiBound;
				char _Unk2[0x50];
				BSTArray<BSLight*> _Lights;
				char _Unk3[0x10];
			public:
				virtual ~BSMultiBoundRoom() = default;

				[[nodiscard]] inline std::uint16_t GetBoundsX() const noexcept(true) { return _BoundsX; }
				[[nodiscard]] inline std::uint16_t GetBoundsY() const noexcept(true) { return _BoundsY; }
				[[nodiscard]] inline std::uint16_t GetBoundsZ() const noexcept(true) { return _BoundsZ; }
				[[nodiscard]] inline NiAPI::NiPointer<BSMultiBound> GetMultiBound() const noexcept(true) { return _MultiBound; }
				[[nodiscard]] inline BSTArray<BSLight*>& GetLights() noexcept(true) { return _Lights; }
			};
			static_assert(sizeof(BSMultiBoundRoom) == 0x1B0);
		}
	}
}