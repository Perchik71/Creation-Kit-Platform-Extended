// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 1.11.246.0: size 0x10. Reference counted per-instance data of a bound object (weapons, armor): TESBoundObject::CreateInstanceData
				// allocates it (weapon 0x138, armor 0x58 bytes) and copies the base instance data, GetBaseInstanceData returns the form's own copy.
				// The derived classes add virtual functions (reset/clear/copy slots) that are not mapped here.
				class TBO_InstanceData
				{
				public:
					virtual ~TBO_InstanceData() = default;

				public:
					std::uint32_t refCount; // 08 - BSIntrusiveRefCounted
					std::uint32_t pad0C; // 0C
				};
				static_assert(sizeof(TBO_InstanceData) == 0x10);
			}
		}
	}
}
