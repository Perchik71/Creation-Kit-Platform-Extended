// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.Interface.h>
#include <CKPE.Common.RuntimeOptimization.h>
#include <CKPE.Common.PatchManager.h>

#include <CKPE.SkyrimSE.Runner.h>
#include <CKPE.SkyrimSE.VersionLists.h>

#include <Patches/CKPE.SkyrimSE.Patch.AllowMultipleWindowAndMaster.h>

namespace CKPE
{
	namespace SkyrimSE
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

			_MESSAGE("[SSE]\tTotal patches: %u", mgr->GetCount());
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
				_MESSAGE("[SSE] Register patches...");
				RegisterPatches();
				_MESSAGE("[SSE] Install patches...");
				InstallPatches();
				_MESSAGE("[SSE] Important optimization patch...");
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