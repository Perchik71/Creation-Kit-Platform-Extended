// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.MessageBox.h>
#include <CKPE.Application.h>
#include <CKPE.Asserts.h>
#include <CKPE.ErrorHandler.h>
#include <CKPE.StringUtils.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.PluginAPI.PluginManager.h>

namespace CKPE
{
	namespace PluginAPI
	{
		static CKPEPluginHandle _currentHandle = 0;
		static PluginManager _PluginManager;

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

			/*switch (id)
			{
			default:
				return nullptr;
			}*/

			return nullptr;
		}

		PluginManager::PluginManager() noexcept(true) :
			_plugins(new std::vector<Plugin*>)
		{}

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

				Plugin* plugin = new Plugin;

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
				_interface.CKPEVersion = s->GetCKPEInterface()->ckpeVersion;
				_interface.CKPECommonVersion = s->GetVersionDLL();
				_interface.CKPEGameLibraryVersion = s->GetGameLibraryVersionDLL();
				_interface.RuntimeVersion = s->GetEditorVersion();
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