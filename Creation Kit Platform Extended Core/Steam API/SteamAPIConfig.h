// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#ifdef CREATIOKITPLATFORMEXTENDEDCORE_EXPORTS
#	ifdef CREATIOKITPLATFORMEXTENDEDCORE_STEAM_API_IMPORT
#		define STEAM_API __declspec(dllimport)
#	else
#		define STEAM_API __declspec(dllexport)
#	endif
#else
#	define STEAM_API
#endif