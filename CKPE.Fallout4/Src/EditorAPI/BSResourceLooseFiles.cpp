// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.FileUtils.h>
#include <EditorAPI/BSResourceLooseFiles.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSResource
			{
				void LooseFileStream::CreateInstance(const char* FileName, LooseFileStream** Instance) noexcept(true)
				{
					if (!FileName || !(*Instance))
						return;

					auto stream = *Instance;
					stream->Initialize64BitData(FileName);
				}

				void LooseFileStream::Initialize64BitData(const char* FileName) noexcept(true)
				{
					dw64FileSize = 0;
					dw64Position = 0;

					auto path = BSString::Utils::GetApplicationPath() + FileName;
					if (BSString::Utils::FileExists(path))
						dw64FileSize = FileUtils::GetFileSize(*path);
					else
						dw64FileSize = dwFileSize;
				}
			}
		}
	}
}