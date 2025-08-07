// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.GameManager.h>
#include <CKPE.Application.h>
#include <CKPE.Module.h>
#include <CKPE.FileUtils.h>
#include <CKPE.ErrorHandler.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Logger.h>
#include <array>

namespace CKPE
{
	static GameManager _sgmanager;

	static std::array GAME_LIBRARIES_FILENAME
	{
		L"",
		L"CKPE.SkyrimSE.dll",
		L"CKPE.Fallout4.dll",
		L"CKPE.Starfield.dll",
	};

	bool GameManager::Initialize(Game game) noexcept(true)
	{
		if (_init) 
			return true;

		if (game == CK_UNKNOWN)
			return false;

		_game_type = game;
		auto _app = Application::GetSingleton();
		auto _path = std::wstring(_app->GetPath()) + GAME_LIBRARIES_FILENAME[_game_type];

		_MESSAGE(L"Load game library: \"%s\"", GAME_LIBRARIES_FILENAME[_game_type]);

		HMODULE resourceHandle = (HMODULE)LoadLibraryExW(_path.c_str(),
			nullptr, LOAD_LIBRARY_AS_IMAGE_RESOURCE);
		if (!resourceHandle)
		{
			_MESSAGE("\tLoadLibraryW returned error: \"%s\"", ErrorHandler::GetSystemMessage(GetLastError()).c_str());
			return false;
		}

		bool hasLoad = false;
		bool hasQuery = false;

		// ScopeLoadedModule unload library because { <code> }
		{
			Module resourceModule(resourceHandle);
			ScopeLoadedModule guard{ resourceModule };

			if (!resourceModule.Is64())
			{
				_MESSAGE("\tIt's 32-bit");
				return false;
			}

			auto _data = (const CKPEGameLibraryData*)resourceModule.Resources.GetProcAddress("CKPEGameLibrary_Data");
			if (!_data)
			{
				_MESSAGE("\tNo data");
				return false;
			}

			if (_data->cbVersion != CKPEGameLibraryData::kVersion)
			{
				_MESSAGE("\tThe data version differs from the supported one, check the mod updates");
				return false;
			}

			hasLoad = resourceModule.Resources.GetProcAddress("CKPEGameLibrary_Load") != nullptr;
			hasQuery = resourceModule.Resources.GetProcAddress("CKPEGameLibrary_Query") != nullptr;
			if (hasLoad && hasQuery)
			{
				_MESSAGE("\tName: %s", _data->name);
				_MESSAGE("\tAuthor: %s", _data->author);
				_MESSAGE("\tVersion: %llX (%u.%u.%u.%u)", _data->dataVersion,
					GET_EXE_VERSION_EX_MAJOR(_data->dataVersion), GET_EXE_VERSION_EX_MINOR(_data->dataVersion),
					GET_EXE_VERSION_EX_BUILD(_data->dataVersion), GET_EXE_VERSION_EX_REVISION(_data->dataVersion));
			}
			else
			{
				_MESSAGE("\tWithout CKPEGameLibrary_Load or CKPEGameLibrary_Query function");
				return false;
			}
		}

		auto handle = (void*)LoadLibraryW(_path.c_str());
		if (!handle)
		{
			_ERROR("\tCouldn't load library: \"%s\"", ErrorHandler::GetSystemMessage(GetLastError()).c_str());
			return false;
		}
		
		Module lib(handle);
		_load = (_CKPEGameLibrary_Load)lib.GetProcAddress("CKPEGameLibrary_Load");
		_query = (_CKPEGameLibrary_Query)lib.GetProcAddress("CKPEGameLibrary_Query");	
		_interface.interfaceVersion = CKPEGameLibraryInterface::kInterfaceVersion;
		_interface.ckpeVersion = FileUtils::GetFileVersion(std::wstring(_app->GetPath()) + L"CKPE.dll");
		_interface.application = const_cast<Application*>(_app);
		_interface.logger = Logger::GetSingleton();
		_interface.QueryInterface = QueryInterface;
		_init = true;

		return true;
	}

	const GameManager* GameManager::GetSingleton() noexcept(true)
	{
		return &_sgmanager;
	}

	std::uint32_t GameManager::QueryLib(std::wstring& version)
	{
		__try
		{
			return _query(version);
		}
		__except (1)
		{
			return GameManager::CK_UNKNOWN;
		}
	}

	bool GameManager::LoadLib()
	{
		__try
		{
			return _load(static_cast<CKPEGameLibraryInterface*>(&_interface));
		}
		__except (1)
		{
			return false;
		}
	}

	void* GameManager::QueryInterface(std::uint32_t id) noexcept(true)
	{
		return nullptr;
	}
}