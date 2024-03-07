// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "..\BSString.h"
#include "BSResourceLooseFilesF4.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			namespace BSResource
			{
				void LooseFileStream::CreateInstance(const char* FileName, LooseFileStream** Instance)
				{
					if (!FileName || !(*Instance))
						return;

					auto stream = *Instance;
					stream->Initialize64BitData(FileName);
				}

				void LooseFileStream::Initialize64BitData(const char* FileName)
				{
					dw64FileSize = 0;
					dw64Position = 0;

					auto path = BSString::Utils::GetApplicationPath() + FileName;
					if (BSString::Utils::FileExists(path))
						dw64FileSize = std::filesystem::file_size(*path);
					else
						dw64FileSize = dwFileSize;
				}
			}
		}
	}
}