// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.Interface.h>
#include <CKPE.Common.RuntimeOptimization.h>
#include <CKPE.Common.PatchManager.h>
#include <CKPE.PluginAPI.PluginManager.h>

#include <CKPE.Starfield.Runner.h>
#include <CKPE.Starfield.VersionLists.h>

#include <Patches/CKPE.Starfield.Patch.AllowMultipleWindowAndMaster.h>
#include <Patches/CKPE.Starfield.Patch.CellViewWindow.h>
#include <Patches/CKPE.Starfield.Patch.RemoveCheckDriver.h>
#include <Patches/CKPE.Starfield.Patch.RunAppWithoutNet.h>

namespace CKPE
{
	namespace Starfield
	{
		static Runner _srunner;

		void Runner::RegisterPatches() noexcept(true)
		{
			auto mgr = Common::PatchManager::GetSingleton();

			mgr->Register(new Patch::AllowMultipleWindowAndMaster);
			mgr->Register(new Patch::CellViewWindow);
			mgr->Register(new Patch::RemoveCheckDriver);
			mgr->Register(new Patch::RunAppWithoutNet);
		}

		void Runner::InstallPatches() noexcept(true)
		{
			auto mgr = Common::PatchManager::GetSingleton();

			_MESSAGE("[SF]\tTotal patches: %u", mgr->GetCount());
			// Query all patches
			mgr->QueryAll(VersionLists::GetGameName());
			// Active remaining patches
			mgr->ActiveAll(VersionLists::GetGameName());
		}

		void Runner::InstallPlugins() noexcept(true)
		{
			auto mgr_plug = PluginAPI::PluginManager::GetSingleton();

			if (mgr_plug->Search())
				mgr_plug->InstallPlugins();
		}

		Runner* Runner::GetSingleton() noexcept(true)
		{
			return &_srunner;
		}

		bool Runner::Install() noexcept(true)
		{
			__try
			{
				_MESSAGE("[SF] Register patches...");
				RegisterPatches();
				_MESSAGE("[SF] Install patches...");
				InstallPatches();
				InstallPlugins();
				_MESSAGE("[SF] Important optimization patch...");
				// Important: this end operation
				Common::RuntimeOptimization ro;
				ro.Apply();

				return true;
			}
			__except (1)
			{
				return false;
			}
		}
	}
}