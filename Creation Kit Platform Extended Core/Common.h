// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#pragma warning (disable : 26812)

#include <windows.h>
#include <dbghelp.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <shlobj_core.h>
#include <uxtheme.h>
#include <strsafe.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdio.h>
#include <wchar.h>
#include <errno.h>
#include <stdint.h>
#include <mmsystem.h>

#include <algorithm>
#include <fstream>
#include <memory>
#include <version>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <initializer_list>

#include <concurrent_vector.h>
#include <concurrent_unordered_map.h>

#include "Crc32.h"
#include "Detours.h"
#include "ini.h"
#include "zip.h"
#include "vmm.h"

#include "Types.h"
#include "Utils.h"

#include "Core/CoreCommon.h"
#include "Core/DebugLog.h"
#include "Core/INIWrapper.h"
#include "Core/Relocator.h"