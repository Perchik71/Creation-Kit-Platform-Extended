// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "SteamAPI.h"

#ifdef __cplusplus
extern "C"
{
#endif

STEAM_API bool SteamAPI_Init()
{
	return false;
}

STEAM_API void SteamAPI_RunCallbacks()
{
	// nope
}

STEAM_API void SteamAPI_RegisterCallback(void* pCallbackBase, int32_t nCallbackIndex)
{
	pCallbackBase = nullptr;
}

STEAM_API void SteamAPI_UnregisterCallback(void* pCallbackBase)
{
	pCallbackBase = nullptr;
}

STEAM_API void SteamAPI_RegisterCallResult(void* pCallbackBase, uint64_t hAPICall)
{
	pCallbackBase = nullptr;
}

STEAM_API void SteamAPI_UnregisterCallResult(void* pCallbackBase, uint64_t hAPICall)
{
	pCallbackBase = nullptr;
}

STEAM_API int32_t SteamAPI_GetHSteamUser()
{
	return 0;
}

STEAM_API void* SteamInternal_FindOrCreateUserInterface(int32_t hSteamUser, const char* pszVersion)
{
	return nullptr;
}

STEAM_API void* SteamInternal_ContextInit(void* pContextInitData)
{
	return nullptr;
}

STEAM_API void* SteamInternal_CreateInterface(const char* pszVersion)
{
	return nullptr;
}

#ifdef __cplusplus
} // __cplusplus defined.
#endif