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
			class BGSFileSelectorDialog
			{
			public:
				class RegisterArchiveFileCallback
				{};
			};

			extern BGSFileSelectorDialog::RegisterArchiveFileCallback* lpArchiveFileCallback;

			namespace BSResource
			{
				class LocationTree
				{};

				extern LocationTree* lpArchiveTree;

				class Archive2
				{
				public:
					enum EResultError : uint32_t
					{
						EC_NONE = 0,
					};
				public:
					static void Initialize();
					static void GetFileSizeStr(uint64_t fileSize, BSString& fileSizeStr);
					static EResultError HKLoadArchive(void* arrayDataList, LooseFileStream*& resFile, void* Unk1, uint32_t Unk2);
					static void LoadArchive(const char* fileName);
					static bool IsAvailableForLoad(const char* fileName);
				};
			}
		}
	}
}

#pragma pack(pop)