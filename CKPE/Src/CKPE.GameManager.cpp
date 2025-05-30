// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include <windows.h>
#include <CKPE.GameManager.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Application.h>
#include <CKPE.Module.h>
#include <CKPE.ErrorHandler.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Logger.h>

namespace CKPE
{
	static GameManager _sgmanager;

	GameManager::LoadedLibrary::LoadedLibrary() :
		handle(nullptr), internalHandle(-1), query(nullptr), load(nullptr), errorCode(0), 
		hasLoad(false), hasQuery(false)
	{
		wmemset(dllName, 0, 128);
		memset(errorState, 0, 128);
		memset(&info, 0, sizeof(LibaryInfo));
		memset(&data, 0, sizeof(CKPEGameLibraryData));
	}

	GameManager::~GameManager()
	{
		if (_libs)
		{
			delete _libs;
			_libs = nullptr;
		}
	}

	void GameManager::InitializeGameLibraries() noexcept(true)
	{
		std::uint32_t handleIdx = 1;	// start at 1, 0 is reserved for internal use

		auto libraries = PathUtils::GetFilesInDir(Application::GetSingleton()->GetPath(), L".dll", false);
		for (auto& lib : libraries)
		{
			LoadedLibrary library;
			
			std::wstring path = lib.first;
			wcscpy_s(library.dllName, PathUtils::ExtractFileName(path).c_str());
			_MESSAGE(L"Checking module %s", library.dllName);

			HMODULE resourceHandle = (HMODULE)LoadLibraryExW(path.c_str(), nullptr, LOAD_LIBRARY_AS_IMAGE_RESOURCE);
			if (resourceHandle)
			{
				Module resourceModule = resourceHandle;
				if (resourceModule.Is64())
				{
					auto* data = (const CKPEGameLibraryData*)
						resourceModule.GetResourceLibraryViaProcAddress("CKPEGameLibrary_Data");
					if (data)
					{
						memcpy(&library.data, data, sizeof(CKPEGameLibraryData));
						library.hasLoad = resourceModule.GetResourceLibraryViaProcAddress("CKPEGameLibrary_Load") != nullptr;
						library.hasQuery = resourceModule.GetResourceLibraryViaProcAddress("CKPEGameLibrary_Query") != nullptr;
						if (library.hasLoad && library.hasQuery)
						{
							library.internalHandle = handleIdx;
							library.info.version = library.data.dataVersion;
							wcscpy_s(library.info.name, StringUtils::WinCPToUtf16(library.data.name).c_str());

							_libs->insert({ handleIdx, library });
							handleIdx++;
						}
						else
							_MESSAGE(L"[ERROR]\t\tWithout CKPEGameLibrary_Load or CKPEGameLibrary_Query function");
					}
				}
				else
					_MESSAGE(L"\t\tIt's 32-bit", path.c_str());

				FreeLibrary(resourceHandle);
			}
			else
				_MESSAGE("LoadLibraryExW returned failed: \"%s\" \n\t%s", StringUtils::Utf16ToUtf8(path).c_str(),
					ErrorHandler::GetSystemMessage(GetLastError()).c_str());
		}
	}

	std::uint32_t GameManager::StartGameLibraries() noexcept(true)
	{
		std::wstring path = Application::GetSingleton()->GetPath();
		std::map<GameLibraryHandle, std::uint32_t> result_query;

		for (auto& lib : *_libs)
		{
			if (!lib.second.handle)
			{
				lib.second.handle = (void*)LoadLibraryW((path + lib.second.dllName).c_str());
				if (!lib.second.handle)
					_ERROR("Couldn't load plugin \"%s\"\n\t\"%s\"", StringUtils::Utf16ToUtf8(lib.second.dllName).c_str(),
						ErrorHandler::GetSystemMessage(GetLastError()));
				else
				{
					lib.second.query = (_CKPEGameLibrary_Query)GetProcAddress((HMODULE)lib.second.handle, "CKPEGameLibrary_Query");
					if (lib.second.query)
					{
						auto result = SafeQuery(lib.second);
						result_query.insert({ lib.first, result });
						if (result != GameManager::CK_SUPPORTED)
						{
							FreeLibrary((HMODULE)lib.second.handle);
							lib.second.handle = nullptr;
						}
						else
						{
							lib.second.load = (_CKPEGameLibrary_Load)GetProcAddress((HMODULE)lib.second.handle, "CKPEGameLibrary_Load");
							auto success = SafeLoad(lib.second);	
							if (!success)
							{
								_ERROR(L"An error occurred initializing the game library \"%s\"", lib.second.dllName);

								FreeLibrary((HMODULE)lib.second.handle);
								lib.second.handle = nullptr;
							}
							else
								_MESSAGE_EX("\tName: \"{}\", Author: \"{}\", Version: {:x} | set index {:x}",
									lib.second.data.name, lib.second.data.author, lib.second.data.dataVersion, lib.first);
						}
					}
					else
					{
						_ERROR("Does not appear to be an CKPE game library");

						FreeLibrary((HMODULE)lib.second.handle);
						lib.second.handle = nullptr;
					}
				}
			}
		}
	}

	std::uint32_t GameManager::Initialize() const noexcept(true)
	{
		if (_init) return GameManager::CK_UNKNOWN;
		auto _This = const_cast<GameManager*>(this);
		_This->_init = true;

		_This->_libs = new std::map<GameLibraryHandle, LoadedLibrary>;
		if (_This->_libs)
		{
			_This->InitializeGameLibraries();
			return _This->StartGameLibraries();
		}
		else
			_FATALERROR("Out of memory");

		return GameManager::CK_UNKNOWN;
	}

	std::uint32_t GameManager::GetCount() const noexcept(true)
	{
		return _libs ? (std::uint32_t)_libs->size() : 0;
	}

	const GameManager* GameManager::GetSingleton() noexcept(true)
	{
		return &_sgmanager;
	}

	std::uint32_t GameManager::SafeQuery(const LoadedLibrary& lib)
	{
		__try
		{
			return lib.query();
		}
		__except (1)
		{
			return GameManager::CK_UNKNOWN;
		}
	}

	bool GameManager::SafeLoad(const LoadedLibrary& lib)
	{
		__try
		{
			return lib.load(nullptr);
		}
		__except (1)
		{
			return false;
		}
	}
}