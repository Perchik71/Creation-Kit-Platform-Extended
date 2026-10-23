// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/N/NiColor.h>
#include <EditorAPI/N/NiObject.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiFloatData;
				class NiColorData;

				class NiInterpolator : public NiObject
				{
				public:
					[[nodiscard]] inline float GetLastTime() const noexcept(true) { return _LastTime; }
				private:
					/* 0x10 */ float _LastTime;
					/* 0x14 */ std::uint32_t _pad14;
				public:
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(NiInterpolator, _LastTime) == 0x10;
					}
				};
				static_assert(sizeof(NiInterpolator) == 0x18);
				static_assert(NiInterpolator::VerifyLayout());

				// 1.11.246.0: size 0x30, constructor 0x14254C660. The Creation Kit embeds these in TESImageSpaceModifier instead of the game's
				// NiPointer<NiFloatInterpolator>. The 0x18 byte NiInterpolator part holds the vtable, the reference count at 08 and the base members.
				class NiFloatInterpolator
				{
				public:
					virtual ~NiFloatInterpolator() = default;

				public:
					std::uint32_t refCount; // 08
					std::uint32_t pad0C; // 0C
					std::uint8_t niInterpolator[0x8]; // 10
					float value; // 18 - -FLT_MAX (invalid) on construction
					std::uint32_t pad1C; // 1C
					NiFloatData* data; // 20 - reference counted (count at +8)
					std::uint32_t lastIndex; // 28
					std::uint32_t pad2C; // 2C
				};
				static_assert(offsetof(NiFloatInterpolator, value) == 0x18);
				static_assert(offsetof(NiFloatInterpolator, data) == 0x20);
				static_assert(sizeof(NiFloatInterpolator) == 0x30);

				// 1.11.246.0: size 0x38, constructor 0x14254D3E0
				class NiColorInterpolator
				{
				public:
					virtual ~NiColorInterpolator() = default;

				public:
					std::uint32_t refCount; // 08
					std::uint32_t pad0C; // 0C
					std::uint8_t niInterpolator[0x8]; // 10
					NiColorA value; // 18 - invalid marker on construction
					NiColorData* data; // 28 - reference counted (count at +8)
					std::uint32_t lastIndex; // 30
					std::uint32_t pad34; // 34
				};
				static_assert(offsetof(NiColorInterpolator, value) == 0x18);
				static_assert(offsetof(NiColorInterpolator, data) == 0x28);
				static_assert(sizeof(NiColorInterpolator) == 0x38);
			}
		}
	}
}
