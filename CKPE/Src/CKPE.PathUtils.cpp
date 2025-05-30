// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <windows.h>
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
		auto it = r.find_last_of(L'/');
		if (it != std::wstring::npos)
			r.erase(it + 1, -1);
		else
			r += L"/";
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
		return GetApplicationPath() + L"Data/";
	}

	std::wstring PathUtils::GetCKPEPluginPath() noexcept(true)
	{
		return GetApplicationPath() + L"CKPEPlugins/";
	}

	bool PathUtils::FileExists(const std::wstring& path) noexcept(true)
	{
		auto attr = GetFileAttributesW(path.c_str());
		return ((attr != INVALID_FILE_ATTRIBUTES) && !(attr & FILE_ATTRIBUTE_DIRECTORY));
	}

	bool PathUtils::DirExists(const std::wstring& path) noexcept(true)
	{
		auto attr = GetFileAttributesW(path.c_str());
		return ((attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY));
	}

	std::wstring& PathUtils::Normalize(std::wstring& path) noexcept(true)
	{
		std::replace(path.begin(), path.end(), L'\\', L'/');
		return path;
	}

	std::wstring& PathUtils::IncludeTrailingPathDelimiter(std::wstring& path) noexcept(true)
	{
		if (!path.empty() && (path.size() > 0))
			if (!wcschr(L"\\/", path[path.size() - 1]))
				path += L"/";
		return path;
	}

	std::wstring& PathUtils::ExcludeTrailingPathDelimiter(std::wstring& path) noexcept(true)
	{
		if (!path.empty() && (path.size() > 0))
			if (wcschr(L"\\/", path[path.size() - 1]))
				path.resize(path.size() - 1);
		return path;
	}

	std::wstring PathUtils::ExtractFileName(std::wstring& path)
	{
		auto it = path.find_last_of(L"\\/");
		if (it != std::wstring::npos)
			return path.substr(it + 1);
		return path;
	}

	std::wstring PathUtils::ExtractFilePath(std::wstring& path)
	{
		auto it = path.find_last_of(L"\\/");
		if (it != std::wstring::npos)
			return path.substr(0, it + 1);
		return L"";
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
			_ERROR("FindFirstFileW returned failed: \"%s\" \n\t%s", StringUtils::Utf16ToUtf8(path).c_str(),
				ErrorHandler::GetSystemMessage(GetLastError()).c_str());
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