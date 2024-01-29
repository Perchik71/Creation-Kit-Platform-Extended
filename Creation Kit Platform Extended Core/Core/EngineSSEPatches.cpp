// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "ModuleManager.h"

#include "Patches/SSE/AllowPlayerKnowsCondition.h"
#include "Patches/SSE/NiCollisionObjectClonedWarning.h"
#include "Patches/SSE/LoadOptimization.h"
#include "Patches/SSE/FixRemoteDesktop.h"
#include "Patches/SSE/FixLoadMore32KAnimation.h"
#include "Patches/SSE/FixCrashLightingShader.h"
#include "Patches/SSE/FixCrashInArmorAddon.h"
#include "Patches/SSE/FixCrashMakeXYZCircles.h"
#include "Patches/SSE/FixNiSkinInstance.h"
#include "Patches/SSE/FixCrashPluginTXT.h"
#include "Patches/SSE/FixBrightLightColor.h"
#include "Patches/SSE/FixDialogueBranch.h"
#include "Patches/SSE/FixMemoryLeakActorDlg.h"
#include "Patches/SSE/FixRaceDlg.h"
#include "Patches/SSE/FixClassDlg.h"
#include "Patches/SSE/CheckD3D11.h"
#include "Patches/SSE/FixSpellDlg.h"
#include "Patches/SSE/FixMusicTrackForm.h"
#include "Patches/SSE/FixCrashTabControl.h"
#include "Patches/SSE/FixRenderPass.h"
#include "Patches/SSE/FixWaterOrtho.h"
#include "Patches/SSE/FixActorDlg.h"
#include "Patches/SSE/FixDataDlgWithPluginTXT.h"
#include "Patches/SSE/FixCrashDuplicateForm.h"
#include "Patches/SSE/CrashAfterMultipleMastersWarning.h"
#include "Patches/SSE/MemoryLeakInPreviewWindow.h"
#include "Patches/SSE/AlteredFormList.h"
#include "Patches/SSE/ReplaceBSPointerHandleAndManager.h"
#include "Patches/SSE/CrashInventoryIterators.h"
#include "Patches/SSE/FixObjectPalette.h"
#include "Patches/SSE/FixVertexNormals.h"
#include "Patches/SSE/FixSelectedPackageData.h"
#include "Patches/SSE/FixMoveToTopic.h"
#include "Patches/SSE/IncreaseMaximumNavmesh.h"
#include "Patches/SSE/FixSpellEffectsDuration.h"
#include "Patches/SSE/FixBGSEntryPointFunctionDataTwoValue.h"
#include "Patches/SSE/WeaponCriticalEffect.h"
#include "Patches/SSE/FixFileInUse.h"
#include "Patches/SSE/CrashTestRadius.h"
#include "Patches/SSE/LoadDDSFile.h"
#include "Patches/SSE/FixUnableToFindVariable.h"
#include "Patches/SSE/CrashFlowChartX.h"
#include "Patches/SSE/BrokenTerrainEditDlg.h"
#include "Patches/SSE/CrashDuplicateWorldspace.h"
#include "Patches/SSE/FixWaterType.h"
#include "Patches/SSE/BNetConvertUnicodeString.h"
#include "Patches/SSE/CrashUsingMore16NPCForFaceGen.h"
#include "Patches/SSE/NavMeshInfoMap.h"
#include "Patches/SSE/BGSPerkRankArray.h"
#include "Patches/SSE/TESParameters_CTDA.h"
#include "Patches/SSE/FixNavMesh.h"
#include "Patches/SSE/CrashSortFunc.h"
#include "Patches/SSE/UnEquipSound.h"
#include "Patches/SSE/TESModelTextureSwapCorrectlyLoad.h"
#include "Patches/SSE/FixCellViewObjectList.h"
#include "Patches/SSE/FixIntersectionTriangle.h"
#include "Patches/SSE/FixIconsScriptProp.h"
#include "Patches/SSE/CrashCCCFiles.h"
#include "Patches/SSE/CrashUploadBNetModWithArchive.h"
#include "Patches/SSE/Re-EnableFog.h"
#include "Patches/SSE/Re-EnableLandShadows.h"
#include "Patches/SSE/FixBSShadowDirectionalLight.h"
#include "Patches/SSE/EnableStateParentWorkaround.h"
#include "Patches/SSE/AllowSaveESMandMasterESP.h"
#include "Patches/SSE/RemoveUselessMessages.h"
#include "Patches/SSE/DeferredDlg.h"
#include "Patches/SSE/EnableGameButtonToVC.h"
#include "Patches/SSE/ShowReloadShadersAlways.h"
#include "Patches/SSE/StableSortForPerks.h"
#include "Patches/SSE/LipGen.h"
#include "Patches/SSE/CrashHairKS.h"
#include "Patches/SSE/BSArchiveManagerModded.h"
#include "Patches/SSE/NavMeshPseudoDelete.h"
#include "Patches/SSE/SpellEnableCastingAndDeliveryAlways.h"
#include "Patches/SSE/UpdateUIWhenFogToggled.h"
#include "Patches/SSE/FixParamsATXT.h"
#include "Patches/SSE/NewFormat171.h"
#include "Patches/SSE/CrashGenMaxHeightData.h"
#include "Patches/SSE/TESDataHandlerHookSSE.h"

#include "Patches/Windows/SSE/MainWindow.h"
#include "Patches/Windows/SSE/RenderWindow.h"
#include "Patches/Windows/SSE/ObjectWindow.h"
#include "Patches/Windows/SSE/DataWindow.h"
#include "Patches/Windows/SSE/CellViewWindow.h"
#include "Patches/Windows/SSE/BNetUploadWindow.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		void SkyrimSpecialEdition_AppendPatches(ModuleManager* PatchesManager)
		{
			namespace Patches = CreationKitPlatformExtended::Patches::SkyrimSpectialEdition;

			PatchesManager->Append({
				new Patches::AlteredFormListPatch(),
				new Patches::AllowPlayerKnowsConditionPatch(),
				new Patches::LoadOptimizationPatch(),
				new Patches::FixRemoteDesktopPatch(),
				new Patches::FixLoadMore32KAnimationPatch(),
				new Patches::FixCrashLightingShaderPatch(),
				new Patches::FixCrashInArmorAddonPatch(),
				new Patches::FixCrashMakeXYZCirclesPatch(),
				new Patches::FixNiSkinInstancePatch(),
				new Patches::FixCrashPluginTXTPatch(),
				new Patches::FixBrightLightColorPatch(),
				new Patches::FixDialogueBranchPatch(),
				new Patches::FixMemoryLeakActorDlgPatch(),
				new Patches::CheckD3D11Patch(),
				new Patches::NiCollisionObjectClonedWarningPatch(),
				new Patches::FixRaceDlgPatch(),
				new Patches::FixClassDlgPatch(),
				new Patches::FixSpellDlgPatch(),
				new Patches::FixMusicTrackFormPatch(),
				new Patches::FixCrashTabControlPatch(),
				new Patches::FixRenderPassPatch(),
				new Patches::FixWaterOrthoPatch(),
				new Patches::FixActorDlgPatch(),
				new Patches::FixDataDlgWithPluginTXTPatch(),
				new Patches::FixCrashDuplicateFormPatch(),
				new Patches::CrashAfterMultipleMastersWarningPatch(),
				new Patches::MemoryLeakInPreviewWindowPatch(),
				new Patches::ReplaceBSPointerHandleAndManagerPatch(),
				new Patches::CrashInventoryIteratorsPatch(),
				new Patches::FixObjectPalettePatch(),
				new Patches::FixVertexNormalsPatch(),
				new Patches::FixSelectedPackageDataPatch(),
				new Patches::FixMoveToTopicPatch(),
				new Patches::IncreaseMaximumNavmeshPatch(),
				new Patches::FixSpellEffectsDurationPatch(),
				new Patches::FixBGSEntryPointFunctionDataTwoValuePatch(),
				new Patches::WeaponCriticalEffectPatch(),
				new Patches::FixFileInUsePatch(),
				new Patches::CrashTestRadiusPatch(),
				new Patches::LoadDDSFilePatch(),
				new Patches::FixUnableToFindVariablePatch(),
				new Patches::CrashFlowChartXPatch(),
				new Patches::BrokenTerrainEditDlgPatch(),
				new Patches::CrashDuplicateWorldspacePatch(),
				new Patches::FixWaterTypePatch(),
				new Patches::BNetConvertUnicodeStringPatch(),
				new Patches::CrashUsingMore16NPCForFaceGenPatch(),
				new Patches::NavMeshInfoMapPatch(),
				new Patches::BGSPerkRankArrayPatch(),
				new Patches::TESParameters_CTDAPatch(),
				new Patches::FixNavMeshPatch(),
				new Patches::CrashSortFuncPatch(),
				new Patches::UnEquipSoundPatch(),
				new Patches::TESModelTextureSwapCorrectlyLoadPatch(),
				new Patches::FixCellViewObjectListPatch(),
				new Patches::FixIntersectionTrianglePatch(),
				new Patches::FixIconsScriptPropPatch(),
				new Patches::CrashCCCFilesPatch(),
				new Patches::CrashUploadBNetModWithArchivePatch(),
				new Patches::ReEnableFogPatch(),
				new Patches::ReEnableLandShadowsPatch(),
				new Patches::FixBSShadowDirectionalLightPatch(),
				new Patches::EnableStateParentWorkaroundPatch(),
				new Patches::AllowSaveESMandMasterESPPatch(),
				new Patches::RemoveUselessMessagesPatch(),
				new Patches::DeferredDlgPatch(),
				new Patches::EnableGameButtonToVCPatch(),
				new Patches::ShowReloadShadersAlwaysPatch(),
				new Patches::StableSortForPerksPatch(),
				new Patches::LipGenPatch(),
				new Patches::CrashHairKSPatch(),
				new Patches::BSArchiveManagerModdedPatch(),
				new Patches::NavMeshPseudoDeletePatch(),
				new Patches::SpellEnableCastingAndDeliveryAlwaysPatch(),
				new Patches::UpdateUIWhenFogToggledPatch(),
				new Patches::FixParamsATXTPatch(),
				new Patches::NewFormat171Patch(),
				new Patches::CrashGenMaxHeightDataPatch(),
				new Patches::TESDataHandlerPatch(),
		
				new Patches::MainWindow(),
				new Patches::RenderWindow(),
				new Patches::ObjectWindow(),
				new Patches::DataWindow(),
				new Patches::CellViewWindow(),
				new Patches::BNetUploadWindow(),
			});
		}
	}
}