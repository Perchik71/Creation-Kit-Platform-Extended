// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.FileUtils.h>
#include <CKPE.PathUtils.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Module.h>
#include <memory>

namespace CKPE
{
	std::uint64_t FileUtils::GetFileVersion(const std::string& fname) noexcept(true)
	{
		return GetFileVersion(fname.c_str());
	}

	std::uint64_t FileUtils::GetFileVersion(const std::wstring& fname) noexcept(true)
	{
		return GetFileVersion(fname.c_str());
	}

	std::uint64_t FileUtils::GetFileVersion(const char* fname) noexcept(true)
	{
		if (!PathUtils::FileExists(fname))
			return (std::uint64_t)-1;

		const auto ver_info_len = GetFileVersionInfoSizeA(fname, nullptr);
		auto ver_info = std::make_unique<std::uint8_t[]>(ver_info_len);
		if (!GetFileVersionInfoA(fname, 0, ver_info_len, ver_info.get()))
			return (std::uint64_t)-1;

		VS_FIXEDFILEINFO* file_version = nullptr;
		unsigned int file_version_len = 0;
		if (!VerQueryValueW(ver_info.get(), L"\\",
			reinterpret_cast<void**>(&file_version), &file_version_len))
			return (std::uint64_t)-1;

		return MAKE_EXE_VERSION_EX(
			HIWORD(file_version->dwProductVersionMS),
			LOWORD(file_version->dwProductVersionMS),
			HIWORD(file_version->dwProductVersionLS),
			LOWORD(file_version->dwProductVersionLS));
	}

	std::uint64_t FileUtils::GetFileVersion(const wchar_t* fname) noexcept(true)
	{
		if (!PathUtils::FileExists(fname))
			return (std::uint64_t)-1;

		const auto ver_info_len = GetFileVersionInfoSizeW(fname, nullptr);
		auto ver_info = std::make_unique<std::uint8_t[]>(ver_info_len);
		if (!GetFileVersionInfoW(fname, 0, ver_info_len, ver_info.get()))
			return (std::uint64_t)-1;

		VS_FIXEDFILEINFO* file_version = nullptr;
		unsigned int file_version_len = 0;
		if (!VerQueryValueW(ver_info.get(), L"\\",
			reinterpret_cast<void**>(&file_version), &file_version_len))
			return (std::uint64_t)-1;

		return MAKE_EXE_VERSION_EX(
			HIWORD(file_version->dwProductVersionMS),
			LOWORD(file_version->dwProductVersionMS),
			HIWORD(file_version->dwProductVersionLS),
			LOWORD(file_version->dwProductVersionLS));
	}

	std::uint64_t FileUtils::GetFileSize(const std::string& fname) noexcept(true)
	{
		return GetFileSize(fname.c_str());
	}

	std::uint64_t FileUtils::GetFileSize(const std::wstring& fname) noexcept(true)
	{
		return GetFileSize(fname.c_str());
	}

	std::uint64_t FileUtils::GetFileSize(const char* fname) noexcept(true)
	{
		if (!PathUtils::FileExists(fname))
			return 0ull;

		WIN32_FILE_ATTRIBUTE_DATA FileData{};
		if (!GetFileAttributesExA(fname, GetFileExInfoStandard, &FileData))
			return 0ull;

		LARGE_INTEGER size{};
		size.HighPart = FileData.nFileSizeHigh;
		size.LowPart = FileData.nFileSizeLow;
		return size.QuadPart;
	}

	std::uint64_t FileUtils::GetFileSize(const wchar_t* fname) noexcept(true)
	{
		if (!PathUtils::FileExists(fname))
			return 0ull;

		WIN32_FILE_ATTRIBUTE_DATA FileData{};
		if (!GetFileAttributesExW(fname, GetFileExInfoStandard, &FileData))
			return 0ull;

		LARGE_INTEGER size{};
		size.HighPart = FileData.nFileSizeHigh;
		size.LowPart = FileData.nFileSizeLow;
		return size.QuadPart;
	}
}