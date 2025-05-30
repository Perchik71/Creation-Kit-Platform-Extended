// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <windows.h>
#include <CKPE.FileUtils.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Module.h>
#include <memory>

namespace CKPE
{
	std::uint32_t FileUtils::GetFileVersion(const std::wstring& fname) noexcept(true)
	{
		if (!PathUtils::FileExists(fname))
			return (std::uint32_t)-1;

		VS_FIXEDFILEINFO* file_version = nullptr;
		unsigned int file_version_len;
		const auto ver_info_len = GetFileVersionInfoSizeW(fname.c_str(), nullptr);
		auto ver_info = std::make_unique<std::uint8_t[]>(ver_info_len * sizeof(wchar_t));
		if (!GetFileVersionInfoW(fname.c_str(), 0, ver_info_len, ver_info.get()))
			return (std::uint32_t)-1;

		if (!VerQueryValueA(ver_info.get(), "", reinterpret_cast<void**>(&file_version), &file_version_len))
			return (std::uint32_t)-1;

		return MAKE_EXE_VERSION(
			HIWORD(file_version->dwProductVersionMS),
			LOWORD(file_version->dwProductVersionMS),
			HIWORD(file_version->dwProductVersionLS));
	}
}