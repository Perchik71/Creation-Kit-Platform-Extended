// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.FileUtils.h>
#include <CKPE.PathUtils.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Module.h>
#include <memory>
#include <sstream>
#include <vector>

namespace CKPE
{
	std::optional<Version> FileUtils::GetFileVersion(const std::string& fname) noexcept(true)
	{
		return GetFileVersion(fname.c_str());
	}

	std::optional<Version> FileUtils::GetFileVersion(const std::wstring& fname) noexcept(true)
	{
		return GetFileVersion(fname.c_str());
	}

	std::optional<Version> FileUtils::GetFileVersion(const char* a_filename) noexcept(true)
	{
		DWORD dummy{};
		std::vector<char> buf(GetFileVersionInfoSizeA(a_filename, std::addressof(dummy)));
		if (buf.empty()) return std::nullopt;

		if (!GetFileVersionInfoA(a_filename, 0, static_cast<DWORD>(buf.size()), buf.data()))
			return std::nullopt;

		void* verBuf{ nullptr };
		std::uint32_t verLen{ 0 };
		if (!VerQueryValueA(buf.data(), "\\StringFileInfo\\040904B0\\ProductVersion",
			std::addressof(verBuf), std::addressof(verLen)))
			return std::nullopt;

		Version            version;
		std::istringstream ss(std::string(static_cast<const char*>(verBuf), verLen));
		std::string        token;
		for (std::size_t i = 0; i < 4 && std::getline(ss, token, '.'); ++i)
			version[i] = static_cast<std::uint16_t>(std::stoi(token));
		return version;
	}

	std::optional<Version> FileUtils::GetFileVersion(const wchar_t* a_filename) noexcept(true)
	{
		DWORD dummy{};
		std::vector<char> buf(GetFileVersionInfoSizeW(a_filename, std::addressof(dummy)));
		if (buf.empty()) return std::nullopt;

		if (!GetFileVersionInfoW(a_filename, 0, static_cast<DWORD>(buf.size()), buf.data()))
			return std::nullopt;

		void* verBuf{ nullptr };
		std::uint32_t verLen{ 0 };
		if (!VerQueryValueW(buf.data(), L"\\StringFileInfo\\040904B0\\ProductVersion",
			std::addressof(verBuf), std::addressof(verLen)))
			return std::nullopt;

		Version             version;
		std::wistringstream ss(std::wstring(static_cast<const wchar_t*>(verBuf), verLen));
		std::wstring        token;
		for (std::size_t i = 0; i < 4 && std::getline(ss, token, L'.'); ++i)
			version[i] = static_cast<std::uint16_t>(std::stoi(token));
		return version;
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