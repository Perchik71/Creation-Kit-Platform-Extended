// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

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

#include "Patches/Windows/FO4/MainWindowF4.h"
#include "Patches/Windows/FO4/ObjectWindowF4.h"
#include "Patches/Windows/FO4/CellViewWindowF4.h"
#include "Patches/Windows/FO4/DataWindowF4.h"

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
				
				new Patches::MainWindow(),
				new Patches::ObjectWindow(),
				new Patches::CellViewWindow(),
				new Patches::DataWindow(),
			});

			CommandLineParser CommandLine;
			if (CommandLine.Count() > 0)
			{
				auto Cmd = CommandLine.At(0);
				auto Sep = Cmd.find_first_of(':');
				if (Sep == String::npos)
					return;

				Cmd = Cmd.substr(0, Sep);
				_MESSAGE("[FO4] Console command: %s", Cmd.c_str());
				if (!_stricmp(Cmd.c_str(), "-GeneratePreCombined"))
				{
					_CONSOLE("The precombining operation has been launched");

					PatchesManager->Append(new CreationKitPlatformExtended::Patches::Fallout4::PreCombinedPatch());
				}
			}
		}
	}
}