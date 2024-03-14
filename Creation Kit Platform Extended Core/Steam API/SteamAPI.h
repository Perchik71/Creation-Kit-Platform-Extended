// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "SteamAPIConfig.h"

#ifdef __cplusplus
extern "C"
{
#endif

STEAM_API void* SteamApps();
STEAM_API void* SteamUser();
STEAM_API void* SteamUtils();
STEAM_API void* SteamClient();
STEAM_API void* SteamRemoteStorage();
STEAM_API bool SteamAPI_Init();
STEAM_API void SteamAPI_RunCallbacks();
STEAM_API void SteamAPI_RegisterCallback(void* pCallbackBase, int32_t nCallbackIndex);
STEAM_API void SteamAPI_UnregisterCallback(void* pCallbackBase);
STEAM_API void SteamAPI_RegisterCallResult(void* pCallbackBase, uint64_t hAPICall);
STEAM_API void  SteamAPI_UnregisterCallResult(void* pCallbackBase, uint64_t hAPICall);
STEAM_API int32_t SteamAPI_GetHSteamUser();
STEAM_API void* SteamInternal_FindOrCreateUserInterface(int32_t hSteamUser, const char* pszVersion);
STEAM_API void* SteamInternal_ContextInit(void* pContextInitData);
STEAM_API void* SteamInternal_CreateInterface(const char* pszVersion);

#ifdef __cplusplus
} // __cplusplus defined.
#endif