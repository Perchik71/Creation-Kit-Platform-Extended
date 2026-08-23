// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <shlwapi.h>
#include <shlobj_core.h>

#include <CKPE.PathUtils.h>
#include <CKPE.ErrorHandler.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Logger.h>
#include <algorithm>

namespace CKPE
{
	std::wstring PathUtils::GetApplicationFileName() noexcept(true)
	{
		std::wstring r{};
		r.resize(1024);
		r.resize(GetModuleFileNameW(GetModuleHandleA(nullptr), r.data(), 1024));
		return Normalize(r);
	}

	std::wstring PathUtils::GetApplicationPath() noexcept(true)
	{
		std::wstring r = GetApplicationFileName();
		Normalize(r);
		auto it = r.find_last_of(L"\\/");
		if (it != std::wstring::npos)
			r.erase(it + 1, -1);
		else
			r += L"\\";
		return r;
	}

	std::wstring PathUtils::GetCurrentPath() noexcept(true)
	{
		std::wstring r{};
		r.resize(1024);
		r.resize(GetCurrentDirectoryW(1024, r.data()));
		return IncludeTrailingPathDelimiter(Normalize(r));
	}

	std::wstring PathUtils::GetDataPath() noexcept(true)
	{
		return GetApplicationPath() + L"Data\\";
	}

	std::wstring PathUtils::GetCKPELogsPath() noexcept(true)
	{
		return GetApplicationPath() + L"Logs\\CKPE\\";
	}

	std::wstring PathUtils::GetCKPEPluginPath() noexcept(true)
	{
		return GetApplicationPath() + L"CKPEPlugins\\";
	}

	std::wstring PathUtils::GetCKPEAddressLibraryPath() noexcept(true)
	{
		return GetApplicationPath() + L"CKPEBins\\";
	}

	std::wstring PathUtils::GetCKPELogsPluginPath() noexcept(true)
	{
		return GetApplicationPath() + L"Logs\\CKPE\\Plug-ins\\";
	}

	bool PathUtils::CreateFolder(const std::string& path) noexcept(true)
	{
		std::string spath = path;
		if (IsRelative(spath))
			spath = StringUtils::Utf16ToWinCP(GetApplicationPath()) + path;
		return SHCreateDirectoryExA(nullptr, spath.c_str(), nullptr) == ERROR_SUCCESS;
	}

	bool PathUtils::CreateFolder(const std::wstring& path) noexcept(true)
	{
		std::wstring spath = path;
		if (IsRelative(spath))
			spath = GetApplicationPath() + path;
		return SHCreateDirectoryExW(nullptr, spath.c_str(), nullptr) == ERROR_SUCCESS;
	}

	bool PathUtils::IsPrefix(const std::string& subpath, const std::string& path) noexcept(true)
	{
		return PathIsPrefixA(subpath.c_str(), path.c_str());
	}

	bool PathUtils::IsPrefix(const std::wstring& subpath, const std::wstring& path) noexcept(true)
	{
		return PathIsPrefixW(subpath.c_str(), path.c_str());
	}

	bool PathUtils::IsRelative(const std::string& path) noexcept(true)
	{
		return PathIsRelativeA(path.c_str());
	}

	bool PathUtils::IsRelative(const std::wstring& path) noexcept(true)
	{
		return PathIsRelativeW(path.c_str());
	}

	bool PathUtils::FileExists(const std::string& path) noexcept(true)
	{
		return FileExists(path.c_str());
	}

	bool PathUtils::DirExists(const std::string& path) noexcept(true)
	{
		return DirExists(path.c_str());
	}

	bool PathUtils::FileExists(const std::wstring& path) noexcept(true)
	{
		return FileExists(path.c_str());
	}

	bool PathUtils::DirExists(const std::wstring& path) noexcept(true)
	{
		return DirExists(path.c_str());
	}

	bool PathUtils::FileExists(const char* path) noexcept(true)
	{
		if (!path) return false;
		auto attr = GetFileAttributesA(path);
		return ((attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY));
	}

	bool PathUtils::DirExists(const char* path) noexcept(true)
	{
		if (!path) return false;
		auto attr = GetFileAttributesA(path);
		return ((attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY));
	}

	bool PathUtils::FileExists(const wchar_t* path) noexcept(true)
	{
		if (!path) return false;
		auto attr = GetFileAttributesW(path);
		return ((attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY));
	}

	bool PathUtils::DirExists(const wchar_t* path) noexcept(true)
	{
		if (!path) return false;
		auto attr = GetFileAttributesW(path);
		return ((attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY));
	}

	std::string& PathUtils::Normalize(std::string& path) noexcept(true)
	{
		std::replace(path.begin(), path.end(), '/', '\\');
		return path;
	}

	std::string& PathUtils::IncludeTrailingPathDelimiter(std::string& path) noexcept(true)
	{
		if (!path.empty() && (path.size() > 0))
			if (!strchr("\\/", path[path.size() - 1]))
				path += "\\";
		return path;
	}

	std::string& PathUtils::ExcludeTrailingPathDelimiter(std::string& path) noexcept(true)
	{
		if (!path.empty() && (path.size() > 0))
			if (strchr("\\/", path[path.size() - 1]))
				path.resize(path.size() - 1);
		return path;
	}

	std::wstring& PathUtils::Normalize(std::wstring& path) noexcept(true)
	{
		std::replace(path.begin(), path.end(), L'/', L'\\');
		return path;
	}

	std::wstring& PathUtils::IncludeTrailingPathDelimiter(std::wstring& path) noexcept(true)
	{
		if (!path.empty() && (path.size() > 0))
			if (!wcschr(L"\\/", path[path.size() - 1]))
				path += L"\\";
		return path;
	}

	std::wstring& PathUtils::ExcludeTrailingPathDelimiter(std::wstring& path) noexcept(true)
	{
		if (!path.empty() && (path.size() > 0))
			if (wcschr(L"\\/", path[path.size() - 1]))
				path.resize(path.size() - 1);
		return path;
	}

	std::string PathUtils::ExtractFileExt(const std::string& path) noexcept(true)
	{
		auto it = path.find_last_of(".");
		if (it != std::string::npos)
			return path.substr(it);
		return "";
	}

	std::string PathUtils::ExtractFileName(const std::string& path) noexcept(true)
	{
		return PathFindFileNameA(path.c_str());
	}

	std::string PathUtils::ExtractFilePath(const std::string& path) noexcept(true)
	{
		auto it = path.find_last_of("\\/");
		if (it != std::string::npos)
			return path.substr(0, it + 1);
		return "";
	}

	std::wstring PathUtils::ExtractFileExt(const std::wstring& path) noexcept(true)
	{
		auto it = path.find_last_of(L".");
		if (it != std::wstring::npos)
			return path.substr(it);
		return L"";
	}

	std::wstring PathUtils::ExtractFileName(const std::wstring& path) noexcept(true)
	{
		return PathFindFileNameW(path.c_str());
	}
	
	std::wstring PathUtils::ExtractFilePath(const std::wstring& path) noexcept(true)
	{
		auto it = path.find_last_of(L"\\/");
		if (it != std::wstring::npos)
			return path.substr(0, it + 1);
		return L"";
	}

	std::string PathUtils::ChangeFileExt(const std::string& path, const std::string& newext) noexcept(true)
	{
		auto it = path.find_last_of(".");
		if (it != std::wstring::npos)
			return path.substr(0, it) + newext;
		return path + newext;
	}

	std::wstring PathUtils::ChangeFileExt(const std::wstring& path, const std::wstring& newext) noexcept(true)
	{
		auto it = path.find_last_of(L".");
		if (it != std::wstring::npos)
			return path.substr(0, it) + newext;
		return path + newext;
	}

	std::unordered_map<std::wstring, std::uint64_t> PathUtils::GetFilesInDir(const std::wstring& path,
		const std::wstring& what, bool recursive) noexcept(true)
	{
		std::wstring spath = path;
		WIN32_FIND_DATAW FindFileData{};
		auto hFind = FindFirstFileExW((IncludeTrailingPathDelimiter(Normalize(spath)) + L"*.*").c_str(), 
			FindExInfoBasic, &FindFileData, FindExSearchNameMatch, nullptr, FIND_FIRST_EX_LARGE_FETCH);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			_ERROR_EX("FindFirstFileW returned failed: \"{}\" \n\t{}", StringUtils::Utf16ToUtf8(path),
				ErrorHandler::GetSystemMessageUTF8(GetLastError()));
			return {};
		}

		std::unordered_map<std::wstring, std::uint64_t> r;

		bool all = what.empty() ? true : ((what.size() == 1) && (what[0] == L'*'));

		do
		{
			if (((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) && recursive)
			{
				std::wstring name = FindFileData.cFileName;
				if (!name.compare(L".") || !name.compare(L".."))
					continue;
				
				auto sr = GetFilesInDir(spath + FindFileData.cFileName + L"/", what, true);
				r.insert(sr.begin(), sr.end());	
			}
			else
			{
				if (!all)
				{
					std::wstring ext = FindFileData.cFileName;
					auto it = ext.find_last_of(L'.');
					if (it == std::wstring::npos)
						continue;

					ext.erase(0, it);
					if (_wcsicmp(ext.c_str(), what.c_str()))
						continue;
				}

				r.insert({
					spath + FindFileData.cFileName,
					(static_cast<std::uint64_t>(FindFileData.nFileSizeHigh) << 32) | FindFileData.nFileSizeLow });
			}
		} while (FindNextFileW(hFind, &FindFileData));

		FindClose(hFind);
		return r;
	}
}