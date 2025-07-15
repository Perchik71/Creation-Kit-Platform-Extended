// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.Interface.h>
#include <CKPE.Common.RuntimeOptimization.h>
#include <CKPE.Common.PatchManager.h>

#include <CKPE.SkyrimSE.Runner.h>
#include <CKPE.SkyrimSE.VersionLists.h>

#include <Patches/CKPE.SkyrimSE.Patch.AllowCreateBitArray.h>
#include <Patches/CKPE.SkyrimSE.Patch.AllowMultipleWindowAndMaster.h>
#include <Patches/CKPE.SkyrimSE.Patch.AllowPlayerKnowsCondition.h>
#include <Patches/CKPE.SkyrimSE.Patch.BGSPerkRankArray.h>
#include <Patches/CKPE.SkyrimSE.Patch.BNetConvertUnicodeString.h>
#include <Patches/CKPE.SkyrimSE.Patch.BrokenTerrainEditDlg.h>
#include <Patches/CKPE.SkyrimSE.Patch.CheckD3D11.h>
#include <Patches/CKPE.SkyrimSE.Patch.CheckTextureInMaterial.h>
#include <Patches/CKPE.SkyrimSE.Patch.ChooseSoundFile.h>
#include <Patches/CKPE.SkyrimSE.Patch.Console.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashAfterMultipleMastersWarning.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashDuplicateWorldspace.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashFlowChartX.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashHairKS.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashNullptrDXGISurface.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashNullptrFaceGen.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashUsingMore16NPCForFaceGen.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		static Runner _srunner;

		void Runner::RegisterPatches() noexcept(true)
		{
			auto mgr = Common::PatchManager::GetSingleton();

			mgr->Register(new Patch::AllowCreateBitArray);
			mgr->Register(new Patch::AllowMultipleWindowAndMaster);
			mgr->Register(new Patch::AllowPlayerKnowsCondition);
			mgr->Register(new Patch::BGSPerkRankArray);
			mgr->Register(new Patch::BNetConvertUnicodeString);
			mgr->Register(new Patch::BrokenTerrainEditDlg);
			mgr->Register(new Patch::CheckD3D11);
			mgr->Register(new Patch::CheckTextureInMaterial);
			mgr->Register(new Patch::ChooseSoundFile);
			mgr->Register(new Patch::Console);
			mgr->Register(new Patch::CrashAfterMultipleMastersWarning);
			mgr->Register(new Patch::CrashDuplicateWorldspace);
			mgr->Register(new Patch::CrashFlowChartX);
			mgr->Register(new Patch::CrashHairKS);
			mgr->Register(new Patch::CrashNullptrDXGISurface);
			mgr->Register(new Patch::CrashNullptrFaceGen);
			mgr->Register(new Patch::CrashUsingMore16NPCForFaceGen);
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