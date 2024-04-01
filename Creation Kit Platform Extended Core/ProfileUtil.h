// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Utils
	{
		bool GetProfileValue(const char* relfilename, const char* section, const char* option, bool defvalue);
		int GetProfileValue(const char* relfilename, const char* section, const char* option, int defvalue);
		float GetProfileValue(const char* relfilename, const char* section, const char* option, float defvalue);
		String GetProfileValue(const char* relfilename, const char* section, const char* option, const char* defvalue);
		void UpdateProfileValue(const char* relfilename, const char* section, const char* option, bool value);
		void UpdateProfileValue(const char* relfilename, const char* section, const char* option, int value);
		void UpdateProfileValue(const char* relfilename, const char* section, const char* option, float value);
		void UpdateProfileValue(const char* relfilename, const char* section, const char* option, const char* value);
	}
}