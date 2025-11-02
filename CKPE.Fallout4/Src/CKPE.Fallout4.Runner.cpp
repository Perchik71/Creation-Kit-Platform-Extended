// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.Interface.h>
#include <CKPE.Common.RuntimeOptimization.h>
#include <CKPE.Common.PatchManager.h>

#include <CKPE.Fallout4.Runner.h>
#include <CKPE.Fallout4.VersionLists.h>

#include <Patches/CKPE.Fallout4.Patch.AddChangeRef.h>
#include <Patches/CKPE.Fallout4.Patch.AllowMultipleWindowAndMaster.h>
#include <Patches/CKPE.Fallout4.Patch.AllowSaveESMandMasterESP.h>
#include <Patches/CKPE.Fallout4.Patch.AssertD3D11.h>
#include <Patches/CKPE.Fallout4.Patch.BSArchiveManagerModded.h>
#include <Patches/CKPE.Fallout4.Patch.BSResourceLooseFiles.h>
#include <Patches/CKPE.Fallout4.Patch.CellViewWindow.h>
#include <Patches/CKPE.Fallout4.Patch.ChooseSoundFile.h>
#include <Patches/CKPE.Fallout4.Patch.Console.h>
#include <Patches/CKPE.Fallout4.Patch.CrashConditionItemGetCrime.h>
#include <Patches/CKPE.Fallout4.Patch.CrashDump.h>
#include <Patches/CKPE.Fallout4.Patch.CrashInvalidStrings.h>
#include <Patches/CKPE.Fallout4.Patch.CreateDDS.h>
#include <Patches/CKPE.Fallout4.Patch.D3D11.h>
#include <Patches/CKPE.Fallout4.Patch.DataWindow.h>
#include <Patches/CKPE.Fallout4.Patch.DisableAssertion.h>
#include <Patches/CKPE.Fallout4.Patch.DontMatchForms.h>
#include <Patches/CKPE.Fallout4.Patch.EnableGoInSelGame.h>
#include <Patches/CKPE.Fallout4.Patch.EncounterZone.h>
#include <Patches/CKPE.Fallout4.Patch.ESLTip.h>
#include <Patches/CKPE.Fallout4.Patch.Facegen.h>
#include <Patches/CKPE.Fallout4.Patch.FakeMoveLight.h>
#include <Patches/CKPE.Fallout4.Patch.FixBadPictureInRender.h>
#include <Patches/CKPE.Fallout4.Patch.FixBNet.h>
#include <Patches/CKPE.Fallout4.Patch.FixCrashMapMarkerCmd.h>
#include <Patches/CKPE.Fallout4.Patch.FixCrashNoRootMat.h>
#include <Patches/CKPE.Fallout4.Patch.FixCrashSpellEaxDur.h>
#include <Patches/CKPE.Fallout4.Patch.FixDeleteTintToRace.h>
#include <Patches/CKPE.Fallout4.Patch.FixInfiniteLoopPapyrusCompile.h>
#include <Patches/CKPE.Fallout4.Patch.FixLandspaceVNML.h>
#include <Patches/CKPE.Fallout4.Patch.FixLoadD3DCompiler.h>
#include <Patches/CKPE.Fallout4.Patch.FixPluginTXT.h>
#include <Patches/CKPE.Fallout4.Patch.FixQuoteCmdLine.h>
#include <Patches/CKPE.Fallout4.Patch.FixRecursiveSorting.h>
#include <Patches/CKPE.Fallout4.Patch.FixSky.h>
#include <Patches/CKPE.Fallout4.Patch.FixSmoothValue.h>
#include <Patches/CKPE.Fallout4.Patch.FixSortPropObjectMod.h>
//#include <Patches/CKPE.Fallout4.Patch.FixSSAOInIntCell.h>
#include <Patches/CKPE.Fallout4.Patch.FixTabDeleteCrash.h>
#include <Patches/CKPE.Fallout4.Patch.FixVC.h>
#include <Patches/CKPE.Fallout4.Patch.FlowChartX.h>
#include <Patches/CKPE.Fallout4.Patch.IncreaseChunkSizeForSNAM.h>
#include <Patches/CKPE.Fallout4.Patch.LayersWindow.h>
#include <Patches/CKPE.Fallout4.Patch.LoadShaders.h>
#include <Patches/CKPE.Fallout4.Patch.MainWindow.h>
#include <Patches/CKPE.Fallout4.Patch.MemoryManager.h>
#include <Patches/CKPE.Fallout4.Patch.Misc.h>
#include <Patches/CKPE.Fallout4.Patch.ModernThemePatchAdditional.h>
#include <Patches/CKPE.Fallout4.Patch.NavMeshWindow.h>
#include <Patches/CKPE.Fallout4.Patch.ObjectWindow.h>
#include <Patches/CKPE.Fallout4.Patch.OptimizationLoad.h>
#include <Patches/CKPE.Fallout4.Patch.PapyrusEditorLimit.h>
#include <Patches/CKPE.Fallout4.Patch.PreCombined.h>
#include <Patches/CKPE.Fallout4.Patch.PreVis.h>
#include <Patches/CKPE.Fallout4.Patch.ProgressWindow.h>
#include <Patches/CKPE.Fallout4.Patch.RemoveUselessMessages.h>
#include <Patches/CKPE.Fallout4.Patch.RenameCreationKitApp.h>
#include <Patches/CKPE.Fallout4.Patch.RenderWindow.h>
#include <Patches/CKPE.Fallout4.Patch.RenderWindow60FPS.h>
#include <Patches/CKPE.Fallout4.Patch.ReplaceBSPointerHandleAndManager.h>
#include <Patches/CKPE.Fallout4.Patch.ResponseIgnoreMax.h>
#include <Patches/CKPE.Fallout4.Patch.ResponseWindow.h>
#include <Patches/CKPE.Fallout4.Patch.RunNetworkDisable.h>
#include <Patches/CKPE.Fallout4.Patch.SkipTopicInfoValidation.h>
#include <Patches/CKPE.Fallout4.Patch.SkipUpdateCheck.h>
#include <Patches/CKPE.Fallout4.Patch.SortCrashCombined.h>
#include <Patches/CKPE.Fallout4.Patch.TESDataHandlerHook.h>
#include <Patches/CKPE.Fallout4.Patch.TESFormHook.h>
#include <Patches/CKPE.Fallout4.Patch.TESHook.h>
#include <Patches/CKPE.Fallout4.Patch.UIDeffer.h>
#include <Patches/CKPE.Fallout4.Patch.UIHotkeys.h>
#include <Patches/CKPE.Fallout4.Patch.Unicode.h>
#include <Patches/CKPE.Fallout4.Patch.VersionControlMergeWorkaround.h>
#include <Patches/CKPE.Fallout4.Patch.WrongUpdateActorWnd.h>

namespace CKPE
{
	namespace Fallout4
	{
		static Runner _srunner;

		void Runner::RegisterPatches() noexcept(true)
		{
			auto mgr = Common::PatchManager::GetSingleton();

			mgr->Register(new Patch::AddChangeRef);
			mgr->Register(new Patch::AllowMultipleWindowAndMaster);
			mgr->Register(new Patch::AllowSaveESMandMasterESP);
			mgr->Register(new Patch::AssertD3D11);
			mgr->Register(new Patch::BSArchiveManagerModded);
			mgr->Register(new Patch::BSResourceLooseFiles);
			mgr->Register(new Patch::CellViewWindow);
			mgr->Register(new Patch::ChooseSoundFile);
			mgr->Register(new Patch::Console);
			mgr->Register(new Patch::CrashConditionItemGetCrime);
			mgr->Register(new Patch::CrashDump);
			mgr->Register(new Patch::CrashInvalidStrings);
			mgr->Register(new Patch::CreateDDS);
			mgr->Register(new Patch::D3D11);
			mgr->Register(new Patch::DataWindow);
			mgr->Register(new Patch::DisableAssertion);
			mgr->Register(new Patch::DontMatchForms);
			mgr->Register(new Patch::EnableGoInSelGame);
			mgr->Register(new Patch::EncounterZone);
			mgr->Register(new Patch::ESLTip);
			mgr->Register(new Patch::Facegen);
			mgr->Register(new Patch::FakeMoveLight);
			mgr->Register(new Patch::FixBadPictureInRender);
			mgr->Register(new Patch::FixBNet);
			mgr->Register(new Patch::FixCrashMapMarkerCmd);
			mgr->Register(new Patch::FixCrashNoRootMat);
			mgr->Register(new Patch::FixCrashSpellEaxDur);
			mgr->Register(new Patch::FixDeleteTintToRace);
			mgr->Register(new Patch::FixInfiniteLoopPapyrusCompile);
			mgr->Register(new Patch::FixLandspaceVNML);
			mgr->Register(new Patch::FixLoadD3DCompiler);
			mgr->Register(new Patch::FixPluginTXT);
			mgr->Register(new Patch::FixQuoteCmdLine);
			mgr->Register(new Patch::FixRecursiveSorting);
			mgr->Register(new Patch::FixSky);
			mgr->Register(new Patch::FixSmoothValue);
			mgr->Register(new Patch::FixSortPropObjectMod);
			//mgr->Register(new Patch::FixSSAOInIntCell);
			mgr->Register(new Patch::FixTabDeleteCrash);
			mgr->Register(new Patch::FixVC);
			mgr->Register(new Patch::FlowChartX);
			mgr->Register(new Patch::IncreaseChunkSizeForSNAM);
			mgr->Register(new Patch::LayersWindow);
			mgr->Register(new Patch::LoadShaders);
			mgr->Register(new Patch::MainWindow);
			mgr->Register(new Patch::MemoryManager);
			mgr->Register(new Patch::Misc);
			mgr->Register(new Patch::ModernThemePatchAdditional);
			mgr->Register(new Patch::NavMeshWindow);
			mgr->Register(new Patch::ObjectWindow);
			mgr->Register(new Patch::OptimizationLoad);
			mgr->Register(new Patch::PapyrusEditorLimit);
			mgr->Register(new Patch::PreVis);
			mgr->Register(new Patch::ProgressWindow);
			mgr->Register(new Patch::RemoveUselessMessages);
			mgr->Register(new Patch::RenameCreationKitApp);
			mgr->Register(new Patch::RenderWindow);
			mgr->Register(new Patch::RenderWindow60FPS);
			mgr->Register(new Patch::ReplaceBSPointerHandleAndManager);
			mgr->Register(new Patch::ResponseIgnoreMax);
			mgr->Register(new Patch::ResponseWindow);
			mgr->Register(new Patch::RunNetworkDisable);
			mgr->Register(new Patch::SkipTopicInfoValidation);
			mgr->Register(new Patch::SkipUpdateCheck);
			mgr->Register(new Patch::SortCrashCombined);
			mgr->Register(new Patch::TESDataHandlerHook);
			mgr->Register(new Patch::TESFormHook);
			mgr->Register(new Patch::TESHook);
			mgr->Register(new Patch::UIDeffer);
			mgr->Register(new Patch::UIHotkeys);
			mgr->Register(new Patch::Unicode);
			mgr->Register(new Patch::VersionControlMergeWorkaround);
			mgr->Register(new Patch::WrongUpdateActorWnd);

			auto cmd = Common::Interface::GetSingleton()->GetCommandLineParser();
			if (cmd->HasCommandRun())
			{
				if (!_wcsnicmp(cmd->GetCommand().c_str(), L"-GeneratePreCombined", 20))
				{
					_CONSOLE("The precombining operation has been launched");

					mgr->Register(new Patch::PreCombined);
				}
				else if (!_wcsnicmp(cmd->GetCommand().c_str(), L"-GeneratePreVisData", 19))
				{
					_CONSOLE("The prevising operation has been launched");

					EditorAPI::BSResource::NoTextureLoad = true;
				}
			}
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