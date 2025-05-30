// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

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
		[[nodiscard]] static std::wstring GetCKPEPluginPath() noexcept(true);

		[[nodiscard]] static bool FileExists(const std::wstring& path) noexcept(true);
		[[nodiscard]] static bool DirExists(const std::wstring& path) noexcept(true);

		static std::wstring& Normalize(std::wstring& path) noexcept(true);
		static std::wstring& IncludeTrailingPathDelimiter(std::wstring& path) noexcept(true);
		static std::wstring& ExcludeTrailingPathDelimiter(std::wstring& path) noexcept(true);

		static std::wstring ExtractFileName(std::wstring& path);
		static std::wstring ExtractFilePath(std::wstring& path);

		static std::unordered_map<std::wstring, std::uint64_t> GetFilesInDir(const std::wstring& path, 
			const std::wstring& what, bool recursive = false) noexcept(true);
	};
}