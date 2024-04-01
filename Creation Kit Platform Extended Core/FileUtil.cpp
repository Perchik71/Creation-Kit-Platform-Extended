// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

namespace CreationKitPlatformExtended
{
	namespace Utils
	{
		bool FileExists(const char* fileName)
		{
			if (!fileName) return false;
			auto Result = GetFileAttributesA(fileName);
			if (Result == INVALID_FILE_ATTRIBUTES || (Result & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
				return false;
			return true;
		}

		bool FileExists(const wchar_t* fileName)
		{
			if (!fileName) return false;
			auto Result = GetFileAttributesW(fileName);
			if (Result == INVALID_FILE_ATTRIBUTES || (Result & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
				return false;
			return true;
		}

		bool DirectoryExists(const char* fileName)
		{
			if (!fileName) return false;
			auto Result = GetFileAttributesA(fileName);
			if (Result == INVALID_FILE_ATTRIBUTES || (Result & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
				return false;
			return true;
		}

		bool DirectoryExists(const wchar_t* fileName)
		{
			if (!fileName) return false;
			auto Result = GetFileAttributesW(fileName);
			if (Result == INVALID_FILE_ATTRIBUTES || (Result & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
				return false;
			return true;
		}
	}
}