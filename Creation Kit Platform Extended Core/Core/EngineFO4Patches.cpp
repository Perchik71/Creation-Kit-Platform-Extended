// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Engine.h"
#include "ModuleManager.h"
#include "CommandLineParser.h"

#include "Patches/FO4/AllowSaveESMandMasterESPF4.h"
#include "Patches/FO4/ReplaceBSPointerHandleAndManagerF4.h"
#include "Patches/FO4/TESFormRE.h"
#include "Patches/FO4/BSResourceLooseFilesPatch.h"
#include "Patches/FO4/TESDataHandlerHook.h"
#include "Patches/FO4/BSArchiveManagerModdedF4.h"
#include "Patches/FO4/RemoveUselessMessagesF4.h"
#include "Patches/FO4/FixVC.h"
#include "Patches/FO4/FixLoadD3DCompiler.h"
#include "Patches/FO4/FixSmoothValue.h"
#include "Patches/FO4/FixCrashNoRootMat.h"
#include "Patches/FO4/FixDeleteTintToRace.h"
#include "Patches/FO4/FixInfiniteLoopPapyrusCompile.h"
#include "Patches/FO4/AssertD3D11.h"
#include "Patches/FO4/EnableGoInSelGame.h"
#include "Patches/FO4/FixCrashMapMarkerCmd.h"
#include "Patches/FO4/FixTabDeleteCrash.h"
#include "Patches/FO4/FixRecursiveSorting.h"
#include "Patches/FO4/SkipUpdateCheck.h"
#include "Patches/FO4/FixBNet.h"
#include "Patches/FO4/FixPluginTXT.h"
#include "Patches/FO4/PreCombined.h"
#include "Patches/FO4/FixQuoteCmdLine.h"
#include "Patches/FO4/UIDefferF4.h"
#include "Patches/FO4/OptimizationLoadF4.h"
#include "Patches/FO4/FixSky.h"
#include "Patches/FO4/FixFog.h"
#include "Patches/FO4/FixCrashSpellEaxDur.h"
#include "Patches/FO4/MiscF4.h"
#include "Patches/FO4/FixEncounterZone.h"
#include "Patches/FO4/WrongUpdateActorWnd.h"
#include "Patches/FO4/CreateDDS.h"
#include "Patches/FO4/ESLTip.h"
#include "Patches/FO4/PreVis.h"
#include "Patches/FO4/AddChangeRefF4.h"
#include "Patches/FO4/SortCrashCombined.h"
#include "Patches/FO4/FakeMoveLight.h"
#include "Patches/FO4/IncreaseChunkSizeForSNAMPatch.h"
#include "Patches/FO4/FixSortPropObjectMod.h"
#include "Patches/FO4/BSResourceTextureDB_RE.h"
#include "Patches/FO4/FixLandspaceVNML.h"
#include "Patches/FO4/CrashInventory.h"
#include "Patches/FO4/ChooseSoundFileF4.h"
#include "Patches/FO4/RunNetworkDisable.h"
#include "Patches/FO4/CrashConditionItemGetCrime.h"
#include "Patches/FO4/DontMatchForms.h"
#include "Patches/FO4/ResponseIgnoreMaxF4.h"

#include "Patches/Windows/FO4/MainWindowF4.h"
#include "Patches/Windows/FO4/ObjectWindowF4.h"
#include "Patches/Windows/FO4/CellViewWindowF4.h"
#include "Patches/Windows/FO4/DataWindowF4.h"
#include "Patches/Windows/FO4/RenderWindowF4.h"
#include "Patches/Windows/FO4/LayersWindow.h"
#include "Patches/Windows/FO4/ResponseWindowF4.h"
#include "Patches/Windows/FO4/ProgressWindowF4.h"
#include "Patches/Windows/FO4/NavMeshWindowF4.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		void Fallout4_AppendPatches(ModuleManager* PatchesManager)
		{
			namespace Patches = CreationKitPlatformExtended::Patches::Fallout4;

			PatchesManager->Append({
				new Patches::AllowSaveESMandMasterESPPatch(),
				new Patches::ReplaceBSPointerHandleAndManagerPatch(),
				new Patches::TESFormREPatch(),
				new Patches::BSResourceLooseFilesPatch(),
				new Patches::TESDataHandlerPatch(),
				new Patches::BSArchiveManagerModdedPatch(),
				new Patches::RemoveUselessMessagesPatch(),
				new Patches::FixVCPatch(),
				new Patches::FixLoadD3DCompilerPatch(),
				new Patches::FixSmoothValuePatch(),
				new Patches::FixCrashNoRootMatPatch(),
				new Patches::FixDeleteTintToRacePatch(),
				new Patches::FixInfiniteLoopPapyrusCompilePatch(),
				new Patches::AssertD3D11Patch(),
				new Patches::EnableGoInSelGamePatch(),
				new Patches::FixCrashMapMarkerCmdPatch(),
				new Patches::FixTabDeleteCrashPatch(),
				new Patches::FixRecursiveSortingPatch(),
				new Patches::SkipUpdateCheckPatch(),
				new Patches::FixBNetPatch(),
				new Patches::FixPluginTXTPatch(),
				new Patches::FixQuoteCmdLinePatch(),
				new Patches::UIDefferPatch(),
				new Patches::OptimizationLoadPatch(),
				new Patches::FixSkyPatch(),
				new Patches::FixFogPatch(),
				new Patches::FixCrashSpellEaxDurPatch(),
				new Patches::MiscPatch(),
				new Patches::FixEncounterZonePatch(),
				new Patches::WrongUpdateActorWndPatch(),
				new Patches::CreateDDSPatch(),
				new Patches::ESLTipPatch(),
				new Patches::PreVisPatch(),
				new Patches::AddChangeRefPatch(),
				new Patches::SortCrashCombinedPatch(),
				new Patches::FakeMoveLightPatch(),
				new Patches::IncreaseChunkSizeForSNAMPatch(),
				new Patches::FixSortPropObjectModPatch(),
				new Patches::BSResourceTextureDBREPatch(),
				new Patches::FixLandspaceVNMLPatch(),
				new Patches::ChooseSoundFilePatch(),
				new Patches::CrashInventoryPatch(),
				new Patches::RunNetworkDisablePatch(),
				new Patches::CrashConditionItemGetCrimePatch(),
				new Patches::DontMatchFormsPatch(),
				new Patches::ResponseIgnoreMaxPatch(),
				
				new Patches::MainWindow(),
				new Patches::ObjectWindow(),
				new Patches::CellViewWindow(),
				new Patches::DataWindow(),
				new Patches::RenderWindow(),
				new Patches::LayersWindow(),
				new Patches::ResponseWindow(),
				new Patches::ProgressWindow(),
				new Patches::NavMeshWindow(),
			});

			CommandLineParser CommandLine;
			if (CommandLine.HasCommandRun())
			{
				auto Cmd = CommandLine.GetCommand();
				auto Sep = Cmd.find_first_of(':');
				if (Sep == String::npos)
					return;

				Cmd = Cmd.substr(0, Sep);
				_MESSAGE("[FO4] Console command: %s", Cmd.c_str());
				if (!_stricmp(Cmd.c_str(), "-GeneratePreCombined"))
				{
					_CONSOLE("The precombining operation has been launched");

					PatchesManager->Append(new Patches::PreCombinedPatch());
				}
			}
		}
	}
}