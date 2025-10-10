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
	CKPE_API bool CKPE_UserUseWine() noexcept(true)
	{
		auto hmod = GetModuleHandleA("kernel32.dll");
		if (hmod && GetProcAddress(hmod, "wine_get_unix_file_name"))
			return true;

		if (getenv("WINEDATADIR") || getenv("WINEPREFIX") || getenv("WINEHOMEDIR"))
			return true;
		
		return false;
	}

	// https://stackoverflow.com/questions/45125550/check-a-user-is-an-admin-on-local-machine-in-c-in-windows
	CKPE_API bool CKPE_UserHasAdminRights() noexcept(true)
	{
		bool result;
		DWORD rc;
		wchar_t user_name[256];
		USER_INFO_1* info{};
		DWORD size = sizeof(user_name);

		GetUserNameW(user_name, &size);

		rc = NetUserGetInfo(NULL, user_name, 1, (LPBYTE*)&info);
		if (rc) return false;

		result = info->usri1_priv == USER_PRIV_ADMIN;

		NetApiBufferFree(info);
		return result;
	}
}