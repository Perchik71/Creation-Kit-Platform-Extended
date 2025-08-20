// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#ifdef CKPEPLUGINAPI_EXPORTS
#	define CKPE_PLUGINAPI_API __declspec(dllexport)
#else
#	define CKPE_PLUGINAPI_API __declspec(dllimport)
#endif