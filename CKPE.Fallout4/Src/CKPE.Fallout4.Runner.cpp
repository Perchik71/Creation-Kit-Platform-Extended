// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.Interface.h>
#include <CKPE.Common.RuntimeOptimization.h>
#include <CKPE.Common.PatchManager.h>

#include <CKPE.Fallout4.Runner.h>
#include <CKPE.Fallout4.VersionLists.h>

#include <Patches/CKPE.Fallout4.Patch.AllowMultipleWindowAndMaster.h>
#include <Patches/CKPE.Fallout4.Patch.Console.h>
#include <Patches/CKPE.Fallout4.Patch.CrashConditionItemGetCrime.h>
#include <Patches/CKPE.Fallout4.Patch.D3D11.h>
#include <Patches/CKPE.Fallout4.Patch.DontMatchForms.h>
#include <Patches/CKPE.Fallout4.Patch.EnableGoInSelGame.h>
#include <Patches/CKPE.Fallout4.Patch.EncounterZone.h>
#include <Patches/CKPE.Fallout4.Patch.Facegen.h>
#include <Patches/CKPE.Fallout4.Patch.FixBadPictureInRender.h>
#include <Patches/CKPE.Fallout4.Patch.FixBNet.h>
#include <Patches/CKPE.Fallout4.Patch.FixCrashMapMarkerCmd.h>
#include <Patches/CKPE.Fallout4.Patch.FixCrashNoRootMat.h>
#include <Patches/CKPE.Fallout4.Patch.FixCrashSpellEaxDur.h>
#include <Patches/CKPE.Fallout4.Patch.FixDeleteTintToRace.h>
#include <Patches/CKPE.Fallout4.Patch.FixSmoothValue.h>
#include <Patches/CKPE.Fallout4.Patch.FixTabDeleteCrash.h>
#include <Patches/CKPE.Fallout4.Patch.IncreaseChunkSizeForSNAM.h>
#include <Patches/CKPE.Fallout4.Patch.MemoryManager.h>
#include <Patches/CKPE.Fallout4.Patch.Unicode.h>
#include <Patches/CKPE.Fallout4.Patch.VersionControlMergeWorkaround.h>

namespace CKPE
{
	namespace Fallout4
	{
		static Runner _srunner;

		void Runner::RegisterPatches() noexcept(true)
		{
			auto mgr = Common::PatchManager::GetSingleton();

			mgr->Register(new Patch::AllowMultipleWindowAndMaster);
			mgr->Register(new Patch::Console);
			mgr->Register(new Patch::CrashConditionItemGetCrime);
			mgr->Register(new Patch::D3D11);
			mgr->Register(new Patch::DontMatchForms);
			mgr->Register(new Patch::EnableGoInSelGame);
			mgr->Register(new Patch::EncounterZone);
			mgr->Register(new Patch::Facegen);
			mgr->Register(new Patch::FixBadPictureInRender);
			mgr->Register(new Patch::FixBNet);
			mgr->Register(new Patch::FixCrashMapMarkerCmd);
			mgr->Register(new Patch::FixCrashNoRootMat);
			mgr->Register(new Patch::FixCrashSpellEaxDur);
			mgr->Register(new Patch::FixDeleteTintToRace);
			mgr->Register(new Patch::FixSmoothValue);
			mgr->Register(new Patch::FixTabDeleteCrash);
			mgr->Register(new Patch::IncreaseChunkSizeForSNAM);
			mgr->Register(new Patch::MemoryManager);
			mgr->Register(new Patch::Unicode);
			mgr->Register(new Patch::VersionControlMergeWorkaround);
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