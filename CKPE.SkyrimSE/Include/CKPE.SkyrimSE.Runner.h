// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

namespace CKPE
{
	namespace SkyrimSE
	{
		class Runner
		{
			Runner(const Runner&) = delete;
			Runner& operator=(const Runner&) = delete;

			void RegisterPatches() const noexcept(true);
			void InstallPatches() const noexcept(true);
			void InstallPlugins() const noexcept(true);
		public:
			constexpr Runner() noexcept(true) = default;

			[[nodiscard]] static Runner* GetSingleton() noexcept(true);
			[[nodiscard]] bool Install() const noexcept(true);
		};
	}
}