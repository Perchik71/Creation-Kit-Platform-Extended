// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

namespace CreationKitPlatformExtended
{
	namespace Utils
	{
		bool GetProfileValue(const char* relfilename, const char* section, const char* option, bool defvalue)
		{
			auto r = GetProfileValue(relfilename, section, option, defvalue ? "true" : "false");
			
			std::transform(r.begin(), r.end(), r.begin(),
				[](unsigned char c) { return std::tolower(c); });

			return (r == "1") || (r == "true");
		}

		int GetProfileValue(const char* relfilename, const char* section, const char* option, int defvalue)
		{
			return GetPrivateProfileInt(section, option, defvalue, (GetApplicationPath() + relfilename).c_str());
		}

		float GetProfileValue(const char* relfilename, const char* section, const char* option, float defvalue)
		{
			char szBuf[64] = { 0 };
			GetPrivateProfileString(section, option, "-", szBuf, 64,
				(GetApplicationPath() + relfilename).c_str());

			if (strcmp(szBuf, "-"))
				return defvalue;

			char* EndPref = nullptr;
			return strtof(szBuf, &EndPref);
		}

		String GetProfileValue(const char* relfilename, const char* section, const char* option, const char* defvalue)
		{
			char szBuf[MAX_PATH] = { 0 };
			GetPrivateProfileString(section, option, defvalue, szBuf, MAX_PATH,
				(GetApplicationPath() + relfilename).c_str());

			// Without support ;
			auto i = strchr(szBuf, ';');
			if (i) *i = '\0';

			const char* whitespaceDelimiters = " \t\n\r\f\v";
			String str = szBuf;
			str.erase(str.find_last_not_of(whitespaceDelimiters) + 1);
			str.erase(0, str.find_first_not_of(whitespaceDelimiters));

			return str;
		}

		void UpdateProfileValue(const char* relfilename, const char* section, const char* option, bool value)
		{
			UpdateProfileValue(relfilename, section, option, value ? "1" : "0");
		}

		void UpdateProfileValue(const char* relfilename, const char* section, const char* option, int value)
		{
			char szBuf[64] = { 0 };
			sprintf_s(szBuf, "%d", value);
			UpdateProfileValue(relfilename, section, option, szBuf);
		}

		void UpdateProfileValue(const char* relfilename, const char* section, const char* option, float value)
		{
			char szBuf[64] = { 0 };
			sprintf_s(szBuf, "%.4f", value);
			UpdateProfileValue(relfilename, section, option, szBuf);
		}

		void UpdateProfileValue(const char* relfilename, const char* section, const char* option, const char* value)
		{
			WritePrivateProfileString(section, option, value, (GetApplicationPath() + relfilename).c_str());
		}
	}
}