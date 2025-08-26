// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <string>
#include <cstdint>
#include <unordered_map>
#include <CKPE.Common.h>

namespace CKPE
{
	class CKPE_API PathUtils
	{
		constexpr PathUtils() noexcept(true) = default;
		PathUtils(const PathUtils&) = delete;
		PathUtils& operator=(const PathUtils&) = delete;
	public:
		[[nodiscard]] static std::wstring GetApplicationFileName() noexcept(true);
		[[nodiscard]] static std::wstring GetApplicationPath() noexcept(true);
		[[nodiscard]] static std::wstring GetCurrentPath() noexcept(true);
		[[nodiscard]] static std::wstring GetDataPath() noexcept(true);
		[[nodiscard]] static std::wstring GetCKPELogsPath() noexcept(true);
		[[nodiscard]] static std::wstring GetCKPEPluginPath() noexcept(true);
		[[nodiscard]] static std::wstring GetCKPELogsPluginPath() noexcept(true);

		static bool CreateFolder(const std::string& path) noexcept(true);
		static bool CreateFolder(const std::wstring& path) noexcept(true);

		[[nodiscard]] static bool IsPrefix(const std::string& subpath, const std::string& path) noexcept(true);
		[[nodiscard]] static bool IsPrefix(const std::wstring& subpath, const std::wstring& path) noexcept(true);
		[[nodiscard]] static bool IsRelative(const std::string& path) noexcept(true);
		[[nodiscard]] static bool IsRelative(const std::wstring& path) noexcept(true);

		[[nodiscard]] static bool FileExists(const std::string& path) noexcept(true);
		[[nodiscard]] static bool DirExists(const std::string& path) noexcept(true);
		[[nodiscard]] static bool FileExists(const std::wstring& path) noexcept(true);
		[[nodiscard]] static bool DirExists(const std::wstring& path) noexcept(true);
		[[nodiscard]] static bool FileExists(const char* path) noexcept(true);
		[[nodiscard]] static bool DirExists(const char* path) noexcept(true);
		[[nodiscard]] static bool FileExists(const wchar_t* path) noexcept(true);
		[[nodiscard]] static bool DirExists(const wchar_t* path) noexcept(true);

		static std::string& Normalize(std::string& path) noexcept(true);
		static std::string& IncludeTrailingPathDelimiter(std::string& path) noexcept(true);
		static std::string& ExcludeTrailingPathDelimiter(std::string& path) noexcept(true);

		static std::wstring& Normalize(std::wstring& path) noexcept(true);
		static std::wstring& IncludeTrailingPathDelimiter(std::wstring& path) noexcept(true);
		static std::wstring& ExcludeTrailingPathDelimiter(std::wstring& path) noexcept(true);

		[[nodiscard]] static std::string ExtractFileExt(const std::string& path) noexcept(true);
		[[nodiscard]] static std::string ExtractFileName(const std::string& path) noexcept(true);
		[[nodiscard]] static std::string ExtractFilePath(const std::string& path) noexcept(true);
		[[nodiscard]] static std::wstring ExtractFileExt(const std::wstring& path) noexcept(true);
		[[nodiscard]] static std::wstring ExtractFileName(const std::wstring& path) noexcept(true);
		[[nodiscard]] static std::wstring ExtractFilePath(const std::wstring& path) noexcept(true);

		[[nodiscard]] static std::string ChangeFileExt(const std::string& path, const std::string& newext) noexcept(true);
		[[nodiscard]] static std::wstring ChangeFileExt(const std::wstring& path, const std::wstring& newext) noexcept(true);

		static std::unordered_map<std::wstring, std::uint64_t> GetFilesInDir(const std::wstring& path, 
			const std::wstring& what, bool recursive = false) noexcept(true);
	};
}