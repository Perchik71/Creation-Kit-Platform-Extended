// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include <cstdint>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSSystemFileStreamer
			{
				struct ID
				{
					std::uint32_t Id;
				};
				static_assert(sizeof(ID) == 0x4);
			}

			class BSSystemFile
			{
			public:
				struct Info
				{
					_FILETIME AccessTime;
					_FILETIME ModifyTime;
					_FILETIME CreateTime;
					std::uint64_t FileSize;
				};
				static_assert(sizeof(Info) == 0x20);

				std::uint64_t Flags;
				HANDLE Handle;
			};
			static_assert(sizeof(BSSystemFile) == 0x10);
		}
	}
}
