// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once
#pragma pack(push, 1)

#include "..\BSString.h"
#include "NiAPI\NiTypes.h"
#include "Editor API/SF/BSResourceLooseFilesSF.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			namespace BSResource
			{
				class Archive2
				{
				public:
					static void Initialize();
					static void GetFileSizeStr(uint64_t fileSize, BSString& fileSizeStr);
					static uint32_t HKLoadArchive(const char* fileName, __int64 unknown01,
						__int64 unknown02, __int64 unknown03, __int64 unknown04, __int64 unknown05, 
						__int64 unknown06, __int64 unknown07);
					static void LoadArchive(const char* fileName);
					static bool IsAvailableForLoad(const char* fileName);
				};
			}
		}
	}
}

#pragma pack(pop)