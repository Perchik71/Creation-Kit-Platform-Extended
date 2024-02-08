// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#pragma warning (disable : 4244)
#pragma warning (disable : 4302)
#pragma warning (disable : 4311)
#pragma warning (disable : 4312)
#pragma warning (disable : 4477)
#pragma warning (disable : 4996)
#pragma warning (disable : 26812)

#include <windows.h>
#include <windowsx.h>
#include <dbghelp.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <shlobj_core.h>
#include <d3d11.h>
#include <dxgi.h>
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
#include <commctrl.h>
#include <commdlg.h>
#include <richedit.h>
#include <intrin.h>
#include <xmmintrin.h>
#include <vssym32.h>
#include <Richedit.h>

// Microsoft: It's probably cool when you need to make several includes instead of one 
// Perchik71: NO
#include <gdiplusenums.h>
#include <gdiplustypes.h>
#include <gdiplus.h>

#include <atomic>
#include <algorithm>
#include <fstream>
#include <memory>
#include <version>
#include <vector>
#include <list>
#include <execution>
#include <chrono>
#include <array>
#include <map>
#include <set>
#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <initializer_list>

#include <concurrent_vector.h>
#include <concurrent_unordered_map.h>
#include <detours/Detours.h>
#include <xbyak.h>
#include <jdialogs.h>

#include "Crc32.h"
#include "ini.h"
#include "zip.h"
#include "vmm.h"

#include "Types.h"
#include "Utils.h"
#include "UtfStr.h"

#include "Core/CoreCommon.h"
#include "Core/DebugLog.h"
#include "Core/INIWrapper.h"
#include "Core/Relocator.h"
#include "Core/ConsoleWindow.h"
#include "Core/MemoryManager.h"
#include "Core/DynamicCast.h"

#undef StrToInt