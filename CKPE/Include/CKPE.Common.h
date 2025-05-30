// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#ifdef CKPE_EXPORTS
#	define CKPE_API __declspec(dllexport)
#else
#	define CKPE_API __declspec(dllimport)
#endif