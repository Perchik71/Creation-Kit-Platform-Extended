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
				// CK has nineteen primary slots, shared with weapon and armor data.
				class TBO_InstanceData
				{
				public:
					virtual ~TBO_InstanceData() = default;

				private:
					// Base defaults do not establish the derived argument contracts.
					/* 008 */ virtual void sub_008();
					/* 010 */ virtual void sub_010();
					/* 018 */ virtual void sub_018();
					/* 020 */ virtual void sub_020();
					/* 028 */ virtual void sub_028();
					/* 030 */ virtual void sub_030();
					/* 038 */ virtual void sub_038();
					/* 040 */ virtual void sub_040();
					/* 048 */ virtual void sub_048();
					/* 050 */ virtual void sub_050();
					/* 058 */ virtual void sub_058();
					/* 060 */ virtual void sub_060();
					/* 068 */ virtual void sub_068();
					/* 070 */ virtual void sub_070();
					/* 078 */ virtual void sub_078();
					/* 080 */ virtual void sub_080();
					/* 088 */ virtual void sub_088();
					/* 090 */ virtual void sub_090();
				public:
					std::uint32_t refCount; // 08 - BSIntrusiveRefCounted
					std::uint32_t pad0C; // 0C
				};
				static_assert(sizeof(TBO_InstanceData) == 0x10);
			}
		}
	}
}
