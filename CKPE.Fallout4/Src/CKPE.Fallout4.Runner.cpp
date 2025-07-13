// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.Interface.h>
#include <CKPE.Common.RuntimeOptimization.h>
#include <CKPE.Common.PatchManager.h>

#include <CKPE.Fallout4.Runner.h>
#include <CKPE.Fallout4.VersionLists.h>

#include <Patches/CKPE.Fallout4.Patch.AllowMultipleWindowAndMaster.h>

namespace CKPE
{
	namespace Fallout4
	{
		static Runner _srunner;

		void Runner::RegisterPatches() noexcept(true)
		{
			auto mgr = Common::PatchManager::GetSingleton();

			mgr->Register(new Patch::AllowMultipleWindowAndMaster);
		}

		void Runner::InstallPatches() noexcept(true)
		{
			auto mgr = Common::PatchManager::GetSingleton();

			_MESSAGE("[FO4]\tTotal patches: %u", mgr->GetCount());
			// Query all patches
			mgr->QueryAll(VersionLists::GetGameName());
			// Active remaining patches
			mgr->ActiveAll(VersionLists::GetGameName());
		}

		Runner* Runner::GetSingleton() noexcept(true)
		{
			return &_srunner;
		}

		bool Runner::Install() noexcept(true)
		{
			__try
			{
				_MESSAGE("[FO4] Register patches...");
				RegisterPatches();
				_MESSAGE("[FO4] Install patches...");
				InstallPatches();
				_MESSAGE("[FO4] Important optimization patch...");
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