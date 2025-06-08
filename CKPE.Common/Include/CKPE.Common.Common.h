/// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#ifdef CKPECOMMON_EXPORTS
#	define CKPE_COMMON_API __declspec(dllexport)
#else
#	define CKPE_COMMON_API __declspec(dllimport)
#endif