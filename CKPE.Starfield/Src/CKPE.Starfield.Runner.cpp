// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include "QtCore/qresource.h"

#include <CKPE.PathUtils.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.RuntimeOptimization.h>
#include <CKPE.Common.PatchManager.h>
#include <CKPE.PluginAPI.PluginManager.h>

#include <CKPE.Starfield.Runner.h>
#include <CKPE.Starfield.VersionLists.h>

#include <Patches/CKPE.Starfield.Patch.AboutWindow.h>
#include <Patches/CKPE.Starfield.Patch.AllowMultipleWindowAndMaster.h>
#include <Patches/CKPE.Starfield.Patch.CrashDump.h>
#include <Patches/CKPE.Starfield.Patch.CellViewWindow.h>
#include <Patches/CKPE.Starfield.Patch.Console.h>
#include <Patches/CKPE.Starfield.Patch.DataWindow.h>
#include <Patches/CKPE.Starfield.Patch.DisableAssertion.h>
#include <Patches/CKPE.Starfield.Patch.FlowChartX.h>
#include <Patches/CKPE.Starfield.Patch.LoadMaterialsQSplash.h>
#include <Patches/CKPE.Starfield.Patch.MainWindow.h>
#include <Patches/CKPE.Starfield.Patch.MemoryManager.h>
#include <Patches/CKPE.Starfield.Patch.ModernThemePatchAdditional.h>
#include <Patches/CKPE.Starfield.Patch.ObjectWindow.h>
#include <Patches/CKPE.Starfield.Patch.PapyrusEditorLimit.h>
#include <Patches/CKPE.Starfield.Patch.RemoveCheckDriver.h>
#include <Patches/CKPE.Starfield.Patch.RemoveThreadDpi.h>
#include <Patches/CKPE.Starfield.Patch.RenameCreationKitApp.h>
#include <Patches/CKPE.Starfield.Patch.ReplaceImgQSplashWnd.h>
#include <Patches/CKPE.Starfield.Patch.RTDynamicCastCrash.h>
#include <Patches/CKPE.Starfield.Patch.RunAppWithoutNet.h>
#include <Patches/CKPE.Starfield.Patch.SkipTopicInfoValidation.h>
#include <Patches/CKPE.Starfield.Patch.TESDataHandlerRE.h>
#include <Patches/CKPE.Starfield.Patch.TESFormRE.h>
#include <Patches/CKPE.Starfield.Patch.Unicode.h>
#include <Patches/CKPE.Starfield.Patch.VersionControlMergeWorkaround.h>

namespace CKPE
{
	namespace Starfield
	{
		static Runner _srunner;

		void Runner::RegisterPatches() noexcept(true)
		{
			auto mgr = Common::PatchManager::GetSingleton();

			mgr->Register(new Patch::AboutWindow);
			mgr->Register(new Patch::AllowMultipleWindowAndMaster);
			mgr->Register(new Patch::CrashDump);
			mgr->Register(new Patch::Console);
			mgr->Register(new Patch::CellViewWindow);
			mgr->Register(new Patch::DataWindow);
			mgr->Register(new Patch::DisableAssertion);
			mgr->Register(new Patch::FlowChartX);
			mgr->Register(new Patch::LoadMaterialsQSplash);
			mgr->Register(new Patch::MainWindow);
			mgr->Register(new Patch::MemoryManager);
			mgr->Register(new Patch::ModernThemePatchAdditional);
			mgr->Register(new Patch::ObjectWindow);
			mgr->Register(new Patch::PapyrusEditorLimit);
			mgr->Register(new Patch::RemoveCheckDriver);
			mgr->Register(new Patch::RemoveThreadDpi);
			mgr->Register(new Patch::RenameCreationKitApp);
			mgr->Register(new Patch::ReplaceImgQSplashWnd);
			mgr->Register(new Patch::RTDynamicCastCrash);
			mgr->Register(new Patch::RunAppWithoutNet);
			mgr->Register(new Patch::SkipTopicInfoValidation);
			mgr->Register(new Patch::TESDataHandlerRE);
			mgr->Register(new Patch::TESFormRE);
			mgr->Register(new Patch::Unicode);
			mgr->Register(new Patch::VersionControlMergeWorkaround);
		}

		bool Runner::RegisterExternalResources() noexcept(true)
		{
			auto externalResource = VersionLists::GetExternalResourcePackageFileName();
			if (!PathUtils::FileExists(externalResource))
			{
				_ERROR(L"This version uses Qt and requires resources for this, the file \"%s\" was not found",
					externalResource.c_str());
				return false;
			}

			if (!QResource::registerResource(StringUtils::Utf16ToUtf8(externalResource).c_str()))
			{
				_ERROR(L"QRESOURCE: Failed to load external resource file \"%s\"",
					externalResource.c_str());
				return false;
			}
			
			_MESSAGE(L"[SF] Open external resource file \"%s\"", externalResource.c_str());
			return true;
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
				if (!RegisterExternalResources())
					return false;

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