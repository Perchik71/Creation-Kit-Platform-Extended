// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <ShlObj_core.h>
#include <CKPE.DirUtils.h>

namespace CKPE
{
	bool DirUtils::DirCreate(const std::wstring& path) noexcept(true)
	{
		return SUCCEEDED(SHCreateDirectoryExW(0, path.c_str(), nullptr));
	}
}