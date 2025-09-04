// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/NiAPI/NiObject.h>
#include <EditorAPI/BSFixedString.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				// 20
				class NiExtraData
				{
				public:
					virtual ~NiExtraData() = 0;
				private:
					char pad08[0x18];
				};
				static_assert(sizeof(NiExtraData) == 0x20);

				// 30
				class NiObjectNET : public NiObject
				{
				public:
					virtual ~NiObjectNET() = default;

					[[nodiscard]] inline const char* GetName() const noexcept(true) { return _Name.c_str<char>(); }

					inline void GetViewerStrings(void(*Callback)(const char*, ...)) const noexcept(true)
					{
						Callback("-- NiObjectNET --\n");
						Callback("Name = %s\n", GetName());
					}
				private:
					// It seems to have remained unchanged
					BSFixedString _Name;
					char _pad18[0x8];
					NiExtraData** _ExtraData;
					std::uint16_t _ExtraDataListLen;
					std::uint16_t _ExtraDataListCapacity;
				};
				static_assert(sizeof(NiObjectNET) == 0x30);
			}
		}
	}
}