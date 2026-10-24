// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>

namespace CKPE::Fallout4::EditorAPI::EditorWindows
{
	// These Win32 resource IDs do not imply C++ dialog inheritance.
	namespace ObjectWindow
	{
		enum ControlID : std::uint32_t
		{
			kFilter = 2581,
			kCategoryTree = 2093,
			kObjectList = 1041,
			kSplitter = 2157,
			kLayout = 6024,
			kEditLayout = 6025,
			kDecompose = 6027
		};
		inline constexpr std::uint16_t kDialogResource = 122;
	}

	namespace CellViewWindow
	{
		enum ControlID : std::uint32_t
		{
			kWorldSpace = 2083,
			kWorldSpaceLabel = 1164,
			kX = 5283,
			kY = 5099,
			kXLabel = 5281,
			kYLabel = 5282,
			kGo = 3681,
			kLoadedAtTop = 5662,
			kCellList = 1155,
			kObjectList = 1156,
			kFilter = 2581,
			kSelectedCellLabel = 1163,
			kFilteredOnly = 5664,
			kSelectedOnly = 5665,
			kVisibleOnly = 5666
		};
		inline constexpr std::uint16_t kDialogResource = 175;
	}

	namespace ResponseWindow
	{
		enum ControlID : std::uint32_t
		{
			kAudioList = 2168,
			kGenerateLip = 1016,
			kFromWAV = 2379,
			kFromLTF = 2380
		};
		inline constexpr std::uint16_t kDialogResource = 3231;
	}

	namespace DataWindow
	{
		enum ControlID : std::uint32_t { kPluginList = 1056 };
		inline constexpr std::uint16_t kDialogResource = 162;
	}
}
