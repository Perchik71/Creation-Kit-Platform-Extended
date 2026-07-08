// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <lmaccess.h>
#include <lmapibuf.h>
#include <CKPE.Utils.h>

#include <CKPE.HardwareInfo.h>

namespace CKPE
{
	static std::uint8_t LINUX_DETECT = 0xFF;

	CKPE_API bool CKPE_UserUseWine() noexcept(true)
	{
		if (LINUX_DETECT == 0xFF)
		{
			auto hmod = GetModuleHandleA("kernel32.dll");
			if (hmod && GetProcAddress(hmod, "wine_get_unix_file_name"))
			{
				LINUX_DETECT = 1;
				return true;
			}

			if (getenv("WINEDATADIR") || getenv("WINEPREFIX") || getenv("WINEHOMEDIR"))
			{
				LINUX_DETECT = 1;
				return true;
			}

			LINUX_DETECT = 0;
			return false;
		}
		
		return (bool)LINUX_DETECT;
	}

	CKPE_API bool CKPE_UserHasAdminRights() noexcept(true)
	{
		return CKPE_IsProcessElevated() || CKPE_IsUserInAdminGroup() || CKPE_UserIsAdminFromSAMTable();
	}

	// https://stackoverflow.com/questions/45125550/check-a-user-is-an-admin-on-local-machine-in-c-in-windows
	CKPE_API bool CKPE_UserIsAdminFromSAMTable() noexcept(true)
	{
		wchar_t user_name[256]{};		
		DWORD size = sizeof(user_name);

		if (!GetUserNameW(user_name, &size))
			return false;

		USER_INFO_1* info{};

		if (NetUserGetInfo(NULL, user_name, 1, (LPBYTE*)&info))
			return false;

		auto result = info->usri1_priv == USER_PRIV_ADMIN;

		NetApiBufferFree(info);
		return result;
	}

	CKPE_API bool CKPE_IsUserInAdminGroup() noexcept(true)
	{
		BOOL isAdmin = false;
		PSID administratorsGroup = NULL;

		// Allocate and initialize a SID for the built-in Administrators group
		SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
		if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
			DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &administratorsGroup)) 
		{

			// Check if the SID is active in the current process token
			// Passing NULL for the first parameter checks the current thread/process token
			if (!CheckTokenMembership(NULL, administratorsGroup, &isAdmin))
				isAdmin = false;

			// Free the allocated SID memory
			FreeSid(administratorsGroup);
		}

		return isAdmin;
	}

	CKPE_API bool CKPE_IsProcessElevated() noexcept(true)
	{
		bool isElevated = false;
		HANDLE hToken = NULL;

		// Open the access token of the current process
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		{
			TOKEN_ELEVATION elevation{};
			DWORD dwSize = sizeof(elevation);

			// Retrieve elevation information
			if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize))
				isElevated = elevation.TokenIsElevated != 0;

			CloseHandle(hToken);
		}

		return isElevated;
	}

}