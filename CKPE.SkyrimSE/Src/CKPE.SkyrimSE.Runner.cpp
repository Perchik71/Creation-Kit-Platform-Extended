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
#include <Patches/CKPE.SkyrimSE.Patch.CrashDump.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashDuplicateWorldspace.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashFlowChartX.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashHairKS.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashNullptrDXGISurface.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashNullptrFaceGen.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashUsingMore16NPCForFaceGen.h>
#include <Patches/CKPE.SkyrimSE.Patch.DisableAssertion.h>
#include <Patches/CKPE.SkyrimSE.Patch.EnableGameButtonToVC.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixActorDlg.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixBGSEntryPointFunctionDataTwoValue.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixBrightLightColor.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixBSShadowDirectionalLight.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixClassDlg.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixCrashDuplicateForm.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixCrashInArmorAddon.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixCrashLightingShader.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixCrashMakeXYZCircles.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixCrashPluginTXT.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixCrashTabControl.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixDataDlgWithPluginTXT.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixDialogueBranch.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixFileInUse.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixIconsScriptProp.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixLoadMore32KAnimation.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixMemoryLeakActorDlg.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixMoveToTopic.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixMusicTrackForm.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixNiSkinInstance.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixObjectPalette.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixParamsATXT.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixRaceDlg.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixRemoteDesktop.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixReverbParameters.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixSelectedPackageData.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixSpellEffectsDuration.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixUnableToFindVariable.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixVertexNormals.h>
#include <Patches/CKPE.SkyrimSE.Patch.FixWaterOrtho.h>
#include <Patches/CKPE.SkyrimSE.Patch.FlowChartX.h>
#include <Patches/CKPE.SkyrimSE.Patch.IgnoreGroundHeightTest.h>
#include <Patches/CKPE.SkyrimSE.Patch.IncreaseMaximumNavmesh.h>
#include <Patches/CKPE.SkyrimSE.Patch.MemoryManager.h>
#include <Patches/CKPE.SkyrimSE.Patch.NavMeshInfoMap.h>
#include <Patches/CKPE.SkyrimSE.Patch.NiCollisionObjectClonedWarning.h>
#include <Patches/CKPE.SkyrimSE.Patch.PapyrusEditorLimit.h>
#include <Patches/CKPE.SkyrimSE.Patch.ReEnableLandShadows.h>
#include <Patches/CKPE.SkyrimSE.Patch.RefLinkGeometryHangWorkaround.h>
#include <Patches/CKPE.SkyrimSE.Patch.RenameCreationKitApp.h>
#include <Patches/CKPE.SkyrimSE.Patch.RenderWindow60FPS.h>
#include <Patches/CKPE.SkyrimSE.Patch.ResponseIgnoreMax.h>
#include <Patches/CKPE.SkyrimSE.Patch.ShowReloadShadersAlways.h>
#include <Patches/CKPE.SkyrimSE.Patch.SkipTopicInfoValidation.h>
#include <Patches/CKPE.SkyrimSE.Patch.TESModelTextureSwapCorrectlyLoad.h>
#include <Patches/CKPE.SkyrimSE.Patch.TESParameters_CTDA.h>
#include <Patches/CKPE.SkyrimSE.Patch.UIHotkeys.h>
#include <Patches/CKPE.SkyrimSE.Patch.UnEquipSound.h>
#include <Patches/CKPE.SkyrimSE.Patch.Unicode.h>
#include <Patches/CKPE.SkyrimSE.Patch.VersionControlMergeWorkaround.h>
#include <Patches/CKPE.SkyrimSE.Patch.WeaponCriticalEffect.h>

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
			mgr->Register(new Patch::CrashDump);
			mgr->Register(new Patch::CrashDuplicateWorldspace);
			mgr->Register(new Patch::CrashFlowChartX);
			mgr->Register(new Patch::CrashHairKS);
			mgr->Register(new Patch::CrashNullptrDXGISurface);
			mgr->Register(new Patch::CrashNullptrFaceGen);
			mgr->Register(new Patch::CrashUsingMore16NPCForFaceGen);
			mgr->Register(new Patch::DisableAssertion);
			mgr->Register(new Patch::EnableGameButtonToVC);
			mgr->Register(new Patch::FixActorDlg);
			mgr->Register(new Patch::FixBGSEntryPointFunctionDataTwoValue);
			mgr->Register(new Patch::FixBrightLightColor);
			mgr->Register(new Patch::FixBSShadowDirectionalLight);
			mgr->Register(new Patch::FixClassDlg);
			mgr->Register(new Patch::FixCrashDuplicateForm);
			mgr->Register(new Patch::FixCrashInArmorAddon);
			mgr->Register(new Patch::FixCrashLightingShader);
			mgr->Register(new Patch::FixCrashMakeXYZCircles);
			mgr->Register(new Patch::FixCrashPluginTXT);
			mgr->Register(new Patch::FixCrashTabControl);
			mgr->Register(new Patch::FixDataDlgWithPluginTXT);
			mgr->Register(new Patch::FixDialogueBranch);
			mgr->Register(new Patch::FixFileInUse);
			mgr->Register(new Patch::FixIconsScriptProp);
			mgr->Register(new Patch::FixLoadMore32KAnimation);
			mgr->Register(new Patch::FixMemoryLeakActorDlg);
			mgr->Register(new Patch::FixMoveToTopic);
			mgr->Register(new Patch::FixMusicTrackForm);
			mgr->Register(new Patch::FixNiSkinInstance);
			mgr->Register(new Patch::FixObjectPalette);
			mgr->Register(new Patch::FixParamsATXT);
			mgr->Register(new Patch::FixRaceDlg);
			mgr->Register(new Patch::FixRemoteDesktop);
			mgr->Register(new Patch::FixReverbParameters);
			mgr->Register(new Patch::FixSelectedPackageData);
			mgr->Register(new Patch::FixSpellEffectsDuration);
			mgr->Register(new Patch::FixUnableToFindVariable);
			mgr->Register(new Patch::FixVertexNormals);
			mgr->Register(new Patch::FixWaterOrtho);
			mgr->Register(new Patch::FlowChartX);
			mgr->Register(new Patch::IgnoreGroundHeightTest);
			mgr->Register(new Patch::IncreaseMaximumNavmesh);
			mgr->Register(new Patch::MemoryManager);
			mgr->Register(new Patch::NavMeshInfoMap);
			mgr->Register(new Patch::NiCollisionObjectClonedWarning);
			mgr->Register(new Patch::PapyrusEditorLimit);
			mgr->Register(new Patch::ReEnableLandShadows);
			mgr->Register(new Patch::RefLinkGeometryHangWorkaround);
			mgr->Register(new Patch::RenameCreationKitApp);
			mgr->Register(new Patch::RenderWindow60FPS);
			mgr->Register(new Patch::ResponseIgnoreMax);
			mgr->Register(new Patch::ShowReloadShadersAlways);
			mgr->Register(new Patch::SkipTopicInfoValidation);
			mgr->Register(new Patch::TESModelTextureSwapCorrectlyLoad);
			mgr->Register(new Patch::TESParameters_CTDA);
			mgr->Register(new Patch::UIHotkeys);
			mgr->Register(new Patch::UnEquipSound);
			mgr->Register(new Patch::Unicode);
			mgr->Register(new Patch::VersionControlMergeWorkaround);
			mgr->Register(new Patch::WeaponCriticalEffect);
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