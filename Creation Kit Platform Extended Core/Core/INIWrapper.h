// Copyright © 2023 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		enum INIConfigOptionType : int
		{
			cotBool = 0,			// b
			cotChar,				// c
			cotInteger,				// i, n
			cotUnsignedInteger,		// u
			cotFloat,				// f
			cotColorRGB,			// r
			cotColorRGBA,			// a
			cotString,				// s, S
			cotMAX
		};

		class INIConfig
		{
		public:
			INIConfig();
			INIConfig(const char* filename);
			INIConfig(int folderID, const char* relPath);
			INIConfig(const wchar_t* filename);
			INIConfig(int folderID, const wchar_t* relPath);
			~INIConfig() = default;

			void Open(const char* filename);
			void Open(const wchar_t* filename);
			void OpenRelative(int folderID, const char* relPath);
			void OpenRelative(int folderID, const wchar_t* relPath);

			static bool SplitOptionName(const char* option, String& section, String& name);
			static INIConfigOptionType GetOptionTypeByName(const char* name);

			bool ReadBool(const char* section, const char* option, bool def);
			long ReadInt(const char* section, const char* option, long def);
			unsigned long ReadUInt(const char* section, const char* option, unsigned long def);
			unsigned long ReadHex(const char* section, const char* option, unsigned long def);
			float ReadFloat(const char* section, const char* option, float def);
			// Для чтения ansi строк
			String ReadString(const char* section, const char* option, const char* def);
			// Для чтения utf8 строк
			WideString ReadUnicodeString(const char* section, const char* option, const wchar_t* def);
		private:
			INIConfig(const INIConfig&) = default;
			INIConfig& operator=(const INIConfig&) = default;

			void OpenFile(const char* filename);
		private:
			SmartPointer<mINI::INIStructure> _iniFile;
		};

		// Необходимо создать, чтобы использовать нижние функции

		extern INIConfig* GlobalINIConfigPtr;
	}

	inline bool _READ_OPTION_BOOL(const char* section, const char* option, bool def)
	{
		return Core::GlobalINIConfigPtr->ReadBool(section, option, def);
	}

	inline long _READ_OPTION_INT(const char* section, const char* option, long def)
	{
		return Core::GlobalINIConfigPtr->ReadInt(section, option, def);
	}

	inline unsigned long _READ_OPTION_UINT(const char* section, const char* option, unsigned long def)
	{
		return Core::GlobalINIConfigPtr->ReadUInt(section, option, def);
	}

	inline unsigned long _READ_OPTION_HEX(const char* section, const char* option, unsigned long def)
	{
		return Core::GlobalINIConfigPtr->ReadHex(section, option, def);
	}

	inline float _READ_OPTION_FLOAT(const char* section, const char* option, float def)
	{
		return Core::GlobalINIConfigPtr->ReadFloat(section, option, def);
	}

	inline String _READ_OPTION_STR(const char* section, const char* option, const char* def)
	{
		return Core::GlobalINIConfigPtr->ReadString(section, option, def);
	}

	inline WideString _READ_OPTION_USTR(const char* section, const char* option, const wchar_t* def)
	{
		return Core::GlobalINIConfigPtr->ReadUnicodeString(section, option, def);
	}
}
