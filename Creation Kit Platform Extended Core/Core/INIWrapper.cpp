// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		INIConfig* GlobalINIConfigPtr = nullptr;

		INIConfig::INIConfig()
		{}

		INIConfig::INIConfig(const char* filename) : INIConfig()
		{
			Open(filename);
		}

		INIConfig::INIConfig(int folderID, const char* relPath) : INIConfig()
		{
			OpenRelative(folderID, relPath);
		}

		INIConfig::INIConfig(const wchar_t* filename) : INIConfig()
		{
			Open(filename);
		}

		INIConfig::INIConfig(int folderID, const wchar_t* relPath) : INIConfig()
		{
			OpenRelative(folderID, relPath);
		}

		void INIConfig::Open(const char* filename)
		{
			Open(Utils::Ansi2Wide(filename).c_str());
		}

		void INIConfig::Open(const wchar_t* filename)
		{
			// Библиотека не поддерживает нормальный юникод, поскольку, STL, а там почитают UTF8.
			// Используем хак с установкой текущей директории.

			auto currentPath = std::make_unique<wchar_t[]>(1025);
			GetCurrentDirectoryW(1024, currentPath.get());

			WideString newPath;
			WideString relPath = filename;

			if (PathIsRelativeW(filename))
			{
				PathAddBackslashW(currentPath.get());

				newPath = currentPath.get();

				auto it = relPath.find_last_of(L"\\/");
				if (it != std::wstring::npos)
					newPath += relPath.substr(0, it);
				else
					newPath += relPath;

				// Изменяем текщую директорию
				SetCurrentDirectoryW(newPath.c_str());

				if (it != std::wstring::npos)
					// Получаем имя файла
					newPath = relPath.substr(it + 1);
				else
					newPath = relPath;
			}
			else
			{
				auto it = relPath.find_last_of(L"\\/");
				if (it != std::wstring::npos)
					newPath = relPath.substr(0, it);
				else
					newPath = relPath;

				// Изменяем текщую директорию
				SetCurrentDirectoryW(newPath.c_str());

				if (it != std::wstring::npos)
					// Получаем имя файла
					newPath = relPath.substr(it + 1);
				else
					newPath = relPath;
			}

			int l = WideCharToMultiByte(CP_ACP, 0, newPath.c_str(), (int)newPath.size(), nullptr, 0,
				nullptr, nullptr);
			if (l > 0)
			{
				auto fname = std::make_unique<char[]>((size_t)l + 1);
				WideCharToMultiByte(CP_ACP, 0, newPath.c_str(), (int)newPath.size(), fname.get(), l,
					nullptr, nullptr);

				_MESSAGE("Open the config ini file: %s", fname.get());

				OpenFile(fname.get());
			}

			// Возвращаем назад
			SetCurrentDirectoryW(currentPath.get());
		}

		void INIConfig::OpenRelative(int folderID, const char* relPath)
		{
			OpenRelative(folderID, Utils::Ansi2Wide(relPath).c_str());
		}

		void INIConfig::OpenRelative(int folderID, const wchar_t* relPath)
		{
			wchar_t path[MAX_PATH];
			HRESULT err = SHGetFolderPathW(NULL, folderID | CSIDL_FLAG_CREATE,
				NULL, SHGFP_TYPE_CURRENT, path);
			if (SUCCEEDED(err))
			{
				wcscat_s(path, relPath);
				PathRemoveFileSpecW(path);
				auto attr = GetFileAttributesW(path);
				if ((attr == INVALID_FILE_ATTRIBUTES) ||
					((attr & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY))
					SHCreateDirectoryExW(0, path, nullptr);
				Open(path);
			}
		}

		bool INIConfig::SplitOptionName(const char* option, String& section, String& name)
		{
			if (!option)
				return false;

			String opt = option;
			if (opt.length() > 0)
			{
				auto it = opt.find_last_of(':');
				if (it != std::string::npos)
				{
					section = opt.substr(0, it);
					name = opt.substr(it + 1);
				}
				else
				{
					section = "";
					name = opt;
				}

				return true;
			}

			return false;
		}

		INIConfigOptionType INIConfig::GetOptionTypeByName(const char* name)
		{
			if (!name) 
				return cotMAX;

			switch (name[0])
			{
			case 'b': return cotBool;
			case 'c': return cotChar;
			case 'i':
			case 'n': return cotInteger;
			case 'u': return cotUnsignedInteger;
			case 'f': return cotFloat;
			case 'r': return cotColorRGB;
			case 'a': return cotColorRGBA;
			case 's':
			case 'S': return cotString;
			default:
				return cotMAX;
			}
		}

		bool INIConfig::ReadBool(const char* section, const char* option, bool def)
		{
			if (_iniFile->has(section))
			{
				auto sec = _iniFile->get(section);
				if (sec.has(option))
				{
					auto val = sec.get(option);

					// слово "Правда" я врядли увижу XD, так что ANSI онли.
					std::transform(val.begin(), val.end(), val.begin(),
						[](unsigned char c) { return std::tolower(c); });

					return ((val == "1") || (val == "true"));
				}
			}

			return def;
		}

		long INIConfig::ReadInt(const char* section, const char* option, long def)
		{
			if (_iniFile->has(section))
			{
				auto sec = _iniFile->get(section);
				if (sec.has(option))
				{
					char* End = nullptr;
					auto val = sec.get(option);

					return strtol(val.c_str(), &End, 10);
				}
			}

			return def;
		}

		unsigned long INIConfig::ReadUInt(const char* section, const char* option, unsigned long def)
		{
			if (_iniFile->has(section))
			{
				auto sec = _iniFile->get(section);
				if (sec.has(option))
				{
					char* End = nullptr;
					auto val = sec.get(option);

					return strtoul(val.c_str(), &End, 10);
				}
			}

			return def;
		}

		unsigned long INIConfig::ReadHex(const char* section, const char* option, unsigned long def)
		{
			if (_iniFile->has(section))
			{
				auto sec = _iniFile->get(section);
				if (sec.has(option))
				{
					char* End = nullptr;
					auto val = sec.get(option);
					_strlwr_s(val.data(), val.length());

					if (val[1] == 'x')
						return strtoul(val.c_str() + 2, &End, 16);
					else
						return strtoul(val.c_str(), &End, 16);
				}
			}

			return def;
		}

		float INIConfig::ReadFloat(const char* section, const char* option, float def)
		{
			if (_iniFile->has(section))
			{
				auto sec = _iniFile->get(section);
				if (sec.has(option))
				{
					char* End = nullptr;
					auto val = sec.get(option);

					return strtof(val.c_str(), &End);
				}
			}

			return def;
		}

		String INIConfig::ReadString(const char* section, const char* option, const char* def)
		{
			if (_iniFile->has(section))
			{
				auto sec = _iniFile->get(section);
				if (sec.has(option))
					return sec.get(option).c_str();
			}

			return def;
		}

		WideString INIConfig::ReadUnicodeString(const char* section, const char* option, const wchar_t* def)
		{
			if (_iniFile->has(section))
			{
				auto sec = _iniFile->get(section);
				if (sec.has(option))
					return Utils::Utf82Wide((const char8_t*)(sec.get(option).c_str()));
			}

			return def;
		}

		void INIConfig::OpenFile(const char* filename)
		{
			_iniFile = MakeSmartPointer<mINI::INIStructure>();
			mINI::INIFile fileIni(filename);
			fileIni.read(**_iniFile);
		}
	}
}