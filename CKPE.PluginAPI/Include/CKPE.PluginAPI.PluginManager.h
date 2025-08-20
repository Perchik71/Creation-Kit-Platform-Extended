// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Singleton.h>
#include <CKPE.PluginAPI.Plugin.h>

namespace CKPE
{
	namespace PluginAPI
	{
		class CKPE_PLUGINAPI_API PluginManager
		{
			std::vector<Plugin*>* _plugins{ nullptr };
			CKPEPluginInterface _interface{};

			PluginManager(const PluginManager&) = delete;
			PluginManager& operator=(const PluginManager&) = delete;

			void ReportPluginErrors(const std::vector<std::wstring>* v) const noexcept(true);

			static CKPEPluginHandle GetPluginHandle() noexcept(true);
			static void* QueryInterface(std::uint32_t id) noexcept(true);
		public:
			PluginManager() noexcept(true);
			virtual ~PluginManager() noexcept(true);

			[[nodiscard]] constexpr virtual Plugin* GetAt(std::uint32_t id) const noexcept(true)
			{ return _plugins ? (*_plugins)[id] : nullptr; }
			[[nodiscard]] constexpr virtual std::uint32_t GetCount() const noexcept(true)
			{ return _plugins ? (std::uint32_t)_plugins->size() : 0; }

			virtual std::uint32_t Search() noexcept(true);
			virtual void InstallPlugins() noexcept(true);

			[[nodiscard]] static PluginManager* GetSingleton() noexcept(true);
		};
	}
}