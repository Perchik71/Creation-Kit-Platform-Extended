// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "TESFileSF.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			bool TESFile::ReadFirstChunk(const char* fileName, TESChunk& chunk)
			{
				auto fileStream = _fsopen(fileName, "rb", _SH_DENYWR);
				if (!fileStream) return false;

				Utils::ScopeFileStream file(fileStream);
				Utils::FileReadBuffer(fileStream, &chunk);

				return true;
			}

			uint32_t TESFile::GetTypeFile(const char* fileName)
			{
				TESChunk chunk;
				if (ReadFirstChunk(fileName, chunk))
					return chunk.flags;
				return 0;
			}

			SYSTEMTIME FileTimeToSystemTimeEx(const FILETIME& Time)
			{
				SYSTEMTIME T;
				FILETIME Local_File_Time;

				FileTimeToLocalFileTime(&Time, &Local_File_Time);
				FileTimeToSystemTime(&Local_File_Time, &T);

				return T;
			}

			//SYSTEMTIME TESFile::GetCreationTime() const
			//{
			//	//return FileTimeToSystemTimeEx(m_findData.ftCreationTime);
			//}

			//SYSTEMTIME TESFile::GetLastWriteTime() const
			//{
			//	//return FileTimeToSystemTimeEx(m_findData.ftLastWriteTime);
			//}
		}
	}
}