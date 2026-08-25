// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.MessageBox.h>
#include <CKPE.Application.h>
#include <CKPE.Asserts.h>
#include <CKPE.ErrorHandler.h>
#include <CKPE.StringUtils.h>
#include <CKPE.PathUtils.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Detours.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.DialogManager.h>
#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.PluginAPI.PluginManager.h>

namespace CKPE
{
	namespace PluginAPI
	{
		static CKPEPluginHandle _currentHandle = 0;
		static PluginManager _PluginManager{};

		static CKPEDialogManagerInterface _DialogManagerInterface
		{
			CKPEDialogManagerInterface::kInterfaceVersion,
			_DialogManagerInterface.HasDialog =
				[](const std::uintptr_t uid) { return Common::DialogManager::GetSingleton()->HasDialog(uid); },
			_DialogManagerInterface.AddDialog =
				[](const char* json_file, const std::uintptr_t uid) { return json_file && json_file[0] ?
				Common::DialogManager::GetSingleton()->AddDialog(json_file, uid) : false; },
			_DialogManagerInterface.AddDialogByCode =
				[](const char* json_code, const std::uintptr_t uid) { return json_code && json_code[0] ?
				Common::DialogManager::GetSingleton()->AddDialogByCode(json_code, uid) : false; },
			_DialogManagerInterface.LoadFromFilePackage =
				[](const char* filename)
			{
				if (!filename || !filename[0] || !CKPE::PathUtils::FileExists(filename) || 
					_stricmp(CKPE::PathUtils::ExtractFileExt(filename).c_str(), ".pak"))
					return false;

				Common::DialogManager::GetSingleton()->LoadFromFilePackage(filename);
				return true;
			},
		};

		static CKPEAddressLibraryInterface _AddressLibraryInterface
		{
			CKPEAddressLibraryInterface::kInterfaceVersion,
			_AddressLibraryInterface.IsLoaded =
				[]() { return Common::AddressLibrary::GetSingleton()->IsLoaded(); },
			_AddressLibraryInterface.Resolve =
				[](const std::uint64_t id, const std::ptrdiff_t offset) -> std::uintptr_t
			{
				auto base = Common::AddressLibrary::GetSingleton()->Resolve(id);
				return base ? (std::uintptr_t)((std::intptr_t)base + offset) : 0;
			},
			_AddressLibraryInterface.Write =
				[](std::uintptr_t address, const std::uint8_t* data, std::size_t size)
				{ SafeWrite::Write(address, data, size); },
			_AddressLibraryInterface.WriteSet =
				[](std::uintptr_t address, std::uint8_t value, std::size_t size)
				{ SafeWrite::WriteSet(address, value, size); },
			_AddressLibraryInterface.WriteNop =
				[](std::uintptr_t address, std::size_t size)
				{ SafeWrite::WriteNop(address, size); },
			_AddressLibraryInterface.DetourJump =
				[](std::uintptr_t target, std::uintptr_t destination) -> std::uintptr_t
				{ return Detours::DetourJump(target, destination); },
			_AddressLibraryInterface.DetourCall =
				[](std::uintptr_t target, std::uintptr_t destination) -> std::uintptr_t
				{ return Detours::DetourCall(target, destination); },
		};

		void PluginManager::ReportPluginErrors(const std::vector<std::wstring>* v) const noexcept(true)
		{
			std::wstring message = L"A plug-in you have installed contains a DLL plugin that has failed to load correctly. "
				"If a new version of Creation Kit was just released, the plugin needs to be updated. "
				"Please check the mod's webpage for updates. This is not a problem with CKPE.\n";

			for (auto& plugin : *v)
				message += std::format(L"\n{}: no compatibility with current version CK", PathUtils::ExtractFileName(plugin));

			message += L"\n\nContinuing to load may result in lost save data or other undesired behavior.";
			message += L"\nExit Creation Kit? (yes highly suggested)";

			if (MessageBox::OpenWarning(message, MessageBox::mbYesNo) == MessageBox::mrYes)
				Common::Interface::GetSingleton()->GetApplication()->Terminate();
		}

		CKPEPluginHandle PluginManager::GetPluginHandle() noexcept(true)
		{
			CKPE_ASSERT_MSG(_currentHandle, "A plugin has called CKPEPluginInterface::GetPluginHandle outside of its Load handlers");

			return _currentHandle;
		}

		void* PluginManager::QueryInterface(std::uint32_t id) noexcept(true)
		{
			if (!_PluginManager._plugins || !_currentHandle)
				return nullptr;

			switch (id)
			{
			case kInterface_DialogManager:
				return (void*)&_DialogManagerInterface;
			case kInterface_AddressLibrary:
				return (void*)&_AddressLibraryInterface;
			default:
				return nullptr;
			}
		}

		PluginManager::PluginManager() noexcept(true) :
			_plugins(new std::vector<Plugin*>)
		{
			
		}

		PluginManager::~PluginManager() noexcept(true)
		{
			if (_plugins)
			{
				for (auto plugin : *_plugins)
				{
					if (plugin)
						delete plugin;
				}

				delete _plugins;
				_plugins = nullptr;
			}
		}

		std::uint32_t PluginManager::Search() noexcept(true)
		{
			if (!_plugins)
				return 0;

			auto path = PathUtils::GetCKPEPluginPath();
			CKPE::_MESSAGE(L"Scanning plugin directory: \"%s\"", path.c_str());

			std::vector<std::wstring> pluginInv;

			auto modules = PathUtils::GetFilesInDir(path, L".dll", true);
			for (auto& info : modules)
			{
				if (_plugins->size() == std::numeric_limits<CKPEPluginHandle>::max())
					break;

				auto plugin = new Plugin;

				auto result = plugin->CanLoad(info.first);
				if (result == Plugin::ErrorNoCompatibility)
					pluginInv.push_back(PathUtils::ExtractFileName(info.first));

				if (result == Plugin::NoError)
				{
					if (plugin->Load(info.first, false))
						_plugins->push_back(plugin);
					else
						delete plugin;
				}
				else
					delete plugin;
			}

			if (pluginInv.size())
				ReportPluginErrors(&pluginInv);

			return (CKPEPluginHandle)_plugins->size();
		}

		void PluginManager::InstallPlugins() noexcept(true)
		{
			if (!_plugins)
				return;

			auto s = Common::Interface::GetSingleton();
			_currentHandle = 1;

			for (auto plug : *_plugins)
			{
				_interface.CKPEVersion = s->GetCKPEInterface()->ckpeVersion.pack();
				_interface.CKPECommonVersion = s->GetVersionDLL().pack();
				_interface.CKPEGameLibraryVersion = s->GetGameLibraryVersionDLL().pack();
				_interface.RuntimeVersion = s->GetEditorVersion().pack();
				_interface.GetPluginHandle = GetPluginHandle;
				_interface.QueryInterface = QueryInterface;

				if (plug->Active((Common::RelocatorDB::PatchDB*)&_interface))
					_currentHandle++;
			}

			_currentHandle = 0;
		}

		PluginManager* PluginManager::GetSingleton() noexcept(true)
		{
			return &_PluginManager;
		}
	}
}