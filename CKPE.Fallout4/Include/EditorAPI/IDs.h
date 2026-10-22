// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

// Address IDs of Creation Kit (Fallout 4) functions and globals, grouped by class in the style of CommonLibF4's IDs.h.
// IDs are CKPE address library IDs (version-<ver>.txt next to the Creation Kit builds). Verified against 1.11.246.0;
// an ID is shared by 1.10.982.3 and later. The first slot is 1.10.162.0: the same ID when its address library
// knows it, a 162-specific ID where one was known, 0 where it is unresolved. The vtable IDs are in IDs_VTABLE.h.

#include <CKPE.Common.ID.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace IDs
			{
				namespace TESDataHandler
				{
					// The TESDataHandler object itself (a static of 0x1010 bytes, no vtable): 1.11.246.0 0x143D5CE60.
					inline constexpr Common::ID Instance{ 314871, 1538742 };
					// BSTSingletonSDM<TESDataHandler> pointer to the live instance ("BSTSingletonSDMBase isn't initialized yet").
					inline constexpr Common::ID InstancePointer{ 236382 };
					// Set by the editor to the "user modded" state (CKPE: TESDataHandler::UserModdedSingleton).
					inline constexpr Common::ID UserModded{ 583335 };
					// TESDataHandler::TESDataHandler (constructed on Instance by the TES constructor). 0x140858EE0
					inline constexpr Common::ID Constructor{ 379186 };
					// 0x1406DFC50
					inline constexpr Common::ID BuildFormVCInfo{ 507415 };
					// 0x1406F4B40 - check-in (1017) / undo check-out (1019) dialog procedure
					inline constexpr Common::ID CheckInDialog{ 248766 };
					// 0x140859540
					inline constexpr Common::ID SaveForm{ 0, 1529554 };
					// 0x140863A80
					inline constexpr Common::ID LoadFiles{ 0, 1994158 };
					// 0x1408675F0
					inline constexpr Common::ID Save{ 0, 1386733 };
					// 0x140868710
					inline constexpr Common::ID LoadFileRecords{ 0, 1495600 };
				}

				namespace TESVersionControl
				{
					// 1.11.246.0 0x1406E4F40
					inline constexpr Common::ID OpenFile{ 645113, 1672165 };
					// 0x1406E56A0
					inline constexpr Common::ID Close{ 645120, 1665199 };
					// 0x1406E7E00
					inline constexpr Common::ID UpdateWorldSpaceOffsets{ 77497 };
					// 0x143D48D80
					inline constexpr Common::ID OpenFileGate{ 974693, 2007624 };
					// 0x143D52331
					inline constexpr Common::ID BatchMode{ 500470 };
					// 0x143D52338
					inline constexpr Common::ID TargetFile{ 140124 };
				}

				namespace TES
				{
					// Global TES* (pointer to the instance), 0x143D49150.
					inline constexpr Common::ID Singleton{ 39712 };
					// bShowMarkers setting object
					inline constexpr Common::ID ShowMarkersSetting{ 335995 };
				}

				namespace Sky
				{
					inline constexpr Common::ID SkyViewSetting{ 169737 };
					inline constexpr Common::ID FogEnabledSetting{ 335998 };
				}

				namespace Setting
				{
					namespace INISettingCollection
					{
						// Lazily created global pointer, 1.11.246.0 0x147447C40.
						inline constexpr Common::ID Singleton{ 644679, 1560057 };
						// Creates on first use and returns the collection, 0x1402CE0A0.
						inline constexpr Common::ID GetSingleton{ 527971 };
					}

					namespace INIPrefSettingCollection
					{
						// Lazily created global pointer, 1.11.246.0 0x143F44EE8.
						inline constexpr Common::ID Singleton{ 532558, 1679582 };
						// 0x14033C100
						inline constexpr Common::ID GetSingleton{ 529720 };
					}

					namespace GameSettingCollection
					{
						// Lazily created global pointer, 1.11.246.0 0x143D6C490.
						inline constexpr Common::ID Singleton{ 0, 1461004 };
						// 0x1403466B0
						inline constexpr Common::ID GetSingleton{ 0, 1989776 };
					}
				}

				// Hook targets used by the CKPE patches (function or data start IDs; names from the 1.11.246.0 database,
				// sub_XXXXXX = unnamed function at RVA 0xXXXXXX, legacy_* = no 1.11.246.0 equivalent).
				namespace Functions
				{
					// 1.11.246.0 0x140974FD0 BGSAudioEffectChain::PerformDialogBox
					inline constexpr Common::ID BGSAudioEffectChain__PerformDialogBox{ 1423625 };
					// 1.11.246.0 0x140CABCD0 BGSEncounterZone::PerformDialogBox
					inline constexpr Common::ID BGSEncounterZone__PerformDialogBox{ 553619 };
					// 1.11.246.0 0x140CCEE30 BGSReferenceGroup::InitItemImpl
					inline constexpr Common::ID BGSReferenceGroup__InitItemImpl{ 530118 };
					// 1.11.246.0 0x140A26D80 BGSScene::InitItemImpl
					inline constexpr Common::ID BGSScene__InitItemImpl{ 663575, 1874614 };
					// 1.11.246.0 0x140A264E0 BGSScene::Load
					inline constexpr Common::ID BGSScene__Load{ 1957714 };
					// 1.11.246.0 0x140FF2AB0 BGSStoryManagerEventNode::WriteDataToDialogBox
					inline constexpr Common::ID BGSStoryManagerEventNode__WriteDataToDialogBox{ 691706, 1689193 };
					// 1.11.246.0 0x140A60200 BGSTerminal::PerformDialogBox
					inline constexpr Common::ID BGSTerminal__PerformDialogBox{ 407774, 1988972 };
					// 1.11.246.0 0x140ED9B00 BSFaceGenUtils::SetupHeadPartShaderState
					inline constexpr Common::ID BSFaceGenUtils__SetupHeadPartShaderState{ 685825, 1426540 };
					// 1.11.246.0 0x142B72DB0 BSGraphics::RenderTargetManager::SaveRenderTarget
					inline constexpr Common::ID BSGraphics__RenderTargetManager__SaveRenderTarget{ 75665, 1589229 };
					// 1.11.246.0 0x142B87920 BSGraphics::State::SetCameraData
					inline constexpr Common::ID BSGraphics__State__SetCameraData{ 795781, 1648232 };
					// 1.11.246.0 0x14261D020 BSIStream::BSIStream
					inline constexpr Common::ID BSIStream__BSIStream{ 774104, 1569121 };
					// 1.11.246.0 0x14261D020 BSIStream::BSIStream
					inline constexpr Common::ID BSIStream__BSIStream_2{ 1569121 };
					// 1.11.246.0 0x142CB1040 BSRenderTargetManager::CreateRenderTargets
					inline constexpr Common::ID BSRenderTargetManager__CreateRenderTargets{ 800387, 1690631 };
					// 1.11.246.0 0x14248BA20 BSResource::GetLooseFileSize32
					inline constexpr Common::ID BSResource__GetLooseFileSize32{ 712081, 2009874 };
					// 1.11.246.0 0x14247BD40 BSResource::LooseFileStream_Create
					inline constexpr Common::ID BSResource__LooseFileStream_Create{ 193501, 1992729 };
					// 1.11.246.0 0x14247C6A0 BSResource::OpenLooseFile
					inline constexpr Common::ID BSResource__OpenLooseFile{ 767841, 1456433 };
					// 1.11.246.0 0x142BCC610 BSShaderData::ApplyMaterialData
					inline constexpr Common::ID BSShaderData__ApplyMaterialData{ 797024, 1666424 };
					// 1.11.246.0 0x14063F780 CellViewWindow_InsertForm
					inline constexpr Common::ID CellViewWindow_InsertForm{ 641951, 1463745 };
					// 1.11.246.0 0x1406713B0 CellViewWindow_InsertObject
					inline constexpr Common::ID CellViewWindow_InsertObject{ 1458119 };
					// 1.11.246.0 0x140641ED0 DialogHelpers_SetItemFloat
					inline constexpr Common::ID DialogHelpers_SetItemFloat{ 577351, 1588570 };
					// 1.11.246.0 0x1407138A0 DialogHelpers_ShowFileDialog
					inline constexpr Common::ID DialogHelpers_ShowFileDialog{ 645762, 1356949 };
					// 1.11.246.0 0x1407138A0 DialogHelpers_ShowFileDialog
					inline constexpr Common::ID DialogHelpers_ShowFileDialog_2{ 1356949 };
					// 1.11.246.0 0x140B32600 ?GetBounds@ID2D1Geometry@@QEBAJAEBUD2D_MATRIX_3X2_F@@PEAUD2D_RECT_F@@@Z_5
					inline constexpr Common::ID GetBounds_ID2D1Geometry__QEBAJAEBUD2D_MATRIX_3X2_F__PEAUD2D_RECT_F___Z_5{ 586309 };
					// 1.11.246.0 0x1404EEEC0 LayersDialog::DlgProc
					inline constexpr Common::ID LayersDialog__DlgProc{ 84440, 1450938 };
					// 1.11.246.0 0x1404EEEC0 LayersDialog::DlgProc
					inline constexpr Common::ID LayersDialog__DlgProc_2{ 1450938 };
					// 1.11.246.0 0x1406D6D70 MainWindow_SetStatusBarText
					inline constexpr Common::ID MainWindow_SetStatusBarText{ 142976 };
					// 1.11.246.0 0x140583AE0 MainWindow_ToggleFog
					inline constexpr Common::ID MainWindow_ToggleFog{ 506991 };
					// 1.11.246.0 0x140583ED0 MainWindow_ToggleMarkers
					inline constexpr Common::ID MainWindow_ToggleMarkers{ 359818 };
					// 1.11.246.0 0x140F05410 NavMesh::InitItemImpl
					inline constexpr Common::ID NavMesh__InitItemImpl{ 548011 };
					// 1.11.246.0 0x14243A610 ObjectWindow_MatchFilterText
					inline constexpr Common::ID ObjectWindow_MatchFilterText{ 636375, 1613291 };
					// 1.11.246.0 0x1408426B0 RenderWindowManager::SetSoundMarkerNodeCulled
					inline constexpr Common::ID RenderWindowManager__SetSoundMarkerNodeCulled{ 506883 };
					// 1.11.246.0 0x14062D380 RenderWindow_OnShowMarkersChanged
					inline constexpr Common::ID RenderWindow_OnShowMarkersChanged{ 359498, 1709686 };
					// 1.11.246.0 0x1402CC770 SettingCollectionList_Setting_::Add
					inline constexpr Common::ID SettingCollectionList_Setting___Add{ 364903 };
					// 1.11.246.0 0x140340410 SettingCollectionMap_Setting_::Add
					inline constexpr Common::ID SettingCollectionMap_Setting___Add{ 623367, 1892575 };
					// 1.11.246.0 0x1406765B0 SortCallbacks_CompareItems
					inline constexpr Common::ID SortCallbacks_CompareItems{ 643144, 1621905 };
					// 1.11.246.0 0x140868710 TESDataHandler::LoadFileRecords
					inline constexpr Common::ID TESDataHandler__LoadFileRecords{ 264710, 1495600 };
					// 1.11.246.0 0x140863A80 TESDataHandler::LoadFiles
					inline constexpr Common::ID TESDataHandler__LoadFiles{ 653593, 1994158 };
					// 1.11.246.0 0x140863A80 TESDataHandler::LoadFiles
					inline constexpr Common::ID TESDataHandler__LoadFiles_2{ 1994158 };
					// 1.11.246.0 0x1408675F0 TESDataHandler::Save
					inline constexpr Common::ID TESDataHandler__Save{ 68391, 1386733 };
					// 1.11.246.0 0x1408805B0 TESFile::LoadTESInfo
					inline constexpr Common::ID TESFile__LoadTESInfo{ 654208, 1514423 };
					// 1.11.246.0 0x1408810C0 TESFile::WriteTESInfo
					inline constexpr Common::ID TESFile__WriteTESInfo{ 532384 };
					// 1.11.246.0 0x1408C31F0 TESForm::GetFormByNumericID
					inline constexpr Common::ID TESForm__GetFormByNumericID{ 611970, 1519646 };
					// 1.11.246.0 0x140AB6B70 TESNPC::PerformDialogBox
					inline constexpr Common::ID TESNPC__PerformDialogBox{ 666248, 1989246 };
					// 1.11.246.0 0x140D1AE20 TESObjectCELL::InitItemImpl
					inline constexpr Common::ID TESObjectCELL__InitItemImpl{ 579610 };
					// 1.11.246.0 0x140D7EB50 TESObjectREFR::Set3D
					inline constexpr Common::ID TESObjectREFR__Set3D{ 531824 };
					// 1.11.246.0 0x140D6AE40 TESObjectREFR::vf3E0
					inline constexpr Common::ID TESObjectREFR__vf3E0{ 580337 };
					// 1.11.246.0 0x140DCB100 TESObjectREFR::vf698
					inline constexpr Common::ID TESObjectREFR__vf698{ 1687816 };
					// 1.11.246.0 0x140B6D050 TESTopicInfo::PerformDialogBox
					inline constexpr Common::ID TESTopicInfo__PerformDialogBox{ 670625, 1774972 };
					// 1.11.246.0 0x140699AE0 WinMain_0
					inline constexpr Common::ID WinMain_0{ 297712, 1477214, 2181936 };
					// 1.11.246.0 0x140699AE0 WinMain_0
					inline constexpr Common::ID WinMain_0_2{ 2181936 };
					// 1.11.246.0 0x143016540 aFileSIsAMaster
					inline constexpr Common::ID aFileSIsAMaster{ 41293 };
					// 1.11.246.0 0x143877430 
					inline constexpr Common::ID data_3877430{ 50814, 1938871 };
					// 1.11.246.0 0x143907280 
					inline constexpr Common::ID data_3907280{ 773177, 1789956 };
					// 1.11.246.0 0x143BAC818 off_143BAC818
					inline constexpr Common::ID data_3BAC818{ 338206 };
					// 1.11.246.0 0x143BAF440 unk_143BAF440
					inline constexpr Common::ID data_3BAF440{ 1106870, 1618347 };
					// 1.11.246.0 0x143D27D80 
					inline constexpr Common::ID data_3D27D80{ 1591587 };
					// 1.11.246.0 0x143D2F9B4 
					inline constexpr Common::ID data_3D2F9B4{ 1435996 };
					// 1.11.246.0 0x143D2F9B8 
					inline constexpr Common::ID data_3D2F9B8{ 1873557 };
					// 1.11.246.0 0x143D491E8 
					inline constexpr Common::ID data_3D491E8{ 494701 };
					// 1.11.246.0 0x143D49308 
					inline constexpr Common::ID data_3D49308{ 451607 };
					// 1.11.246.0 0x143D493B8 
					inline constexpr Common::ID data_3D493B8{ 493984 };
					// 1.11.246.0 0x143D493D0 
					inline constexpr Common::ID data_3D493D0{ 506980 };
					// 1.11.246.0 0x143D493E8 
					inline constexpr Common::ID data_3D493E8{ 502295 };
					// 1.11.246.0 0x143D49400 
					inline constexpr Common::ID data_3D49400{ 383762 };
					// 1.11.246.0 0x143D49418 
					inline constexpr Common::ID data_3D49418{ 401851 };
					// 1.11.246.0 0x143D49430 
					inline constexpr Common::ID data_3D49430{ 463613 };
					// 1.11.246.0 0x143D49448 
					inline constexpr Common::ID data_3D49448{ 526097 };
					// 1.11.246.0 0x143D49460 
					inline constexpr Common::ID data_3D49460{ 443228 };
					// 1.11.246.0 0x143D49478 
					inline constexpr Common::ID data_3D49478{ 526100 };
					// 1.11.246.0 0x143D49490 
					inline constexpr Common::ID data_3D49490{ 526106 };
					// 1.11.246.0 0x143D494A8 
					inline constexpr Common::ID data_3D494A8{ 526107 };
					// 1.11.246.0 0x143D5E564 
					inline constexpr Common::ID data_3D5E564{ 365092, 1990122 };
					// 1.11.246.0 0x143D5E5B8 
					inline constexpr Common::ID data_3D5E5B8{ 1110197, 1493376 };
					// 1.11.246.0 0x143D6EC30 
					inline constexpr Common::ID data_3D6EC30{ 394722 };
					// 1.11.246.0 0x147FFD808 
					inline constexpr Common::ID data_7FFD808{ 583093 };
					// 1.11.246.0 0x148017B70 
					inline constexpr Common::ID data_8017B70{ 91815, 1343534 };
					// no 1.11.246.0 equivalent (used by BSResourceLooseFiles)
					inline constexpr Common::ID legacy_BSResourceLooseFiles_1501752{ 1501752 };
					// no 1.11.246.0 equivalent (used by BSResourceLooseFiles)
					inline constexpr Common::ID legacy_BSResourceLooseFiles_255793{ 255793 };
					// no 1.11.246.0 equivalent (used by BSResourceLooseFiles)
					inline constexpr Common::ID legacy_BSResourceLooseFiles_530259{ 530259 };
					// no 1.11.246.0 equivalent (used by BSResourceLooseFiles)
					inline constexpr Common::ID legacy_BSResourceLooseFiles_530994{ 530994 };
					// no 1.11.246.0 equivalent (used by BSResourceLooseFiles)
					inline constexpr Common::ID legacy_BSResourceLooseFiles_647276{ 647276 };
					// no 1.11.246.0 equivalent (used by BSResourceLooseFiles)
					inline constexpr Common::ID legacy_BSResourceLooseFiles_648213{ 648213 };
					// no 1.11.246.0 equivalent (used by BSResourceLooseFiles)
					inline constexpr Common::ID legacy_BSResourceLooseFiles_745724{ 745724 };
					// no 1.11.246.0 equivalent (used by CellViewWindow)
					inline constexpr Common::ID legacy_CellViewWindow_643197{ 643197 };
					// no 1.11.246.0 equivalent (used by CellViewWindow)
					inline constexpr Common::ID legacy_CellViewWindow_643205{ 643205 };
					// no 1.11.246.0 equivalent (used by ChooseSoundFile)
					inline constexpr Common::ID legacy_ChooseSoundFile_119311{ 119311 };
					// no 1.11.246.0 equivalent (used by ChooseSoundFile)
					inline constexpr Common::ID legacy_ChooseSoundFile_216529{ 216529 };
					// no 1.11.246.0 equivalent (used by ChooseSoundFile)
					inline constexpr Common::ID legacy_ChooseSoundFile_419916{ 419916 };
					// no 1.11.246.0 equivalent (used by ChooseSoundFile)
					inline constexpr Common::ID legacy_ChooseSoundFile_429186{ 429186 };
					// no 1.11.246.0 equivalent (used by ChooseSoundFile)
					inline constexpr Common::ID legacy_ChooseSoundFile_581209{ 581209 };
					// no 1.11.246.0 equivalent (used by ChooseSoundFile)
					inline constexpr Common::ID legacy_ChooseSoundFile_645762{ 645762 };
					// no 1.11.246.0 equivalent (used by CrashDump)
					inline constexpr Common::ID legacy_CrashDump_433039{ 433039 };
					// no 1.11.246.0 equivalent (used by CrashDump)
					inline constexpr Common::ID legacy_CrashDump_752875{ 752875 };
					// no 1.11.246.0 equivalent (used by CrashDump)
					inline constexpr Common::ID legacy_CrashDump_752927{ 752927 };
					// no 1.11.246.0 equivalent (used by CrashDump)
					inline constexpr Common::ID legacy_CrashDump_809922{ 809922 };
					// no 1.11.246.0 equivalent (used by CreateDDS)
					inline constexpr Common::ID legacy_CreateDDS_795841{ 795841 };
					// no 1.11.246.0 equivalent (used by CreateDDS)
					inline constexpr Common::ID legacy_CreateDDS_795948{ 795948 };
					// no 1.11.246.0 equivalent (used by FixCrashNoRootMat)
					inline constexpr Common::ID legacy_FixCrashNoRootMat_54722{ 54722 };
					// no 1.11.246.0 equivalent (used by FixCrashNoRootMat)
					inline constexpr Common::ID legacy_FixCrashNoRootMat_649982{ 649982 };
					// no 1.11.246.0 equivalent (used by FixCrashNoRootMat)
					inline constexpr Common::ID legacy_FixCrashNoRootMat_797136{ 797136 };
					// no 1.11.246.0 equivalent (used by FixLoadD3DCompiler)
					inline constexpr Common::ID legacy_FixLoadD3DCompiler_795140{ 795140 };
					// no 1.11.246.0 equivalent (used by HavokRigidBodyCrash)
					inline constexpr Common::ID legacy_HavokRigidBodyCrash_720180{ 720180 };
					// no 1.11.246.0 equivalent (used by Misc)
					inline constexpr Common::ID legacy_Misc_643475{ 643475 };
					// no 1.11.246.0 equivalent (used by ModernThemePatchAdditional)
					inline constexpr Common::ID legacy_ModernThemePatchAdditional_635134{ 635134 };
					// no 1.11.246.0 equivalent (used by ModernThemePatchAdditional)
					inline constexpr Common::ID legacy_ModernThemePatchAdditional_646715{ 646715 };
					// no 1.11.246.0 equivalent (used by ObjectWindow)
					inline constexpr Common::ID legacy_ObjectWindow_636059{ 636059 };
					// no 1.11.246.0 equivalent (used by ObjectWindow)
					inline constexpr Common::ID legacy_ObjectWindow_642052{ 642052 };
					// no 1.11.246.0 equivalent (used by OptimizationLoad)
					inline constexpr Common::ID legacy_OptimizationLoad_528149{ 528149 };
					// no 1.11.246.0 equivalent (used by PreCombined)
					inline constexpr Common::ID legacy_PreCombined_415101{ 415101 };
					// no 1.11.246.0 equivalent (used by PreCombined)
					inline constexpr Common::ID legacy_PreCombined_632758{ 632758 };
					// no 1.11.246.0 equivalent (used by ProgressWindow)
					inline constexpr Common::ID legacy_ProgressWindow_653593{ 653593 };
					// no 1.11.246.0 equivalent (used by ReplaceBSPointerHandleAndManager)
					inline constexpr Common::ID legacy_ReplaceBSPointerHandleAndManager_123735{ 123735 };
					// no 1.11.246.0 equivalent (used by ReplaceBSPointerHandleAndManager)
					inline constexpr Common::ID legacy_ReplaceBSPointerHandleAndManager_1477214{ 1477214 };
					// no 1.11.246.0 equivalent (used by ReplaceBSPointerHandleAndManager)
					inline constexpr Common::ID legacy_ReplaceBSPointerHandleAndManager_161285{ 161285 };
					// no 1.11.246.0 equivalent (used by ReplaceBSPointerHandleAndManager)
					inline constexpr Common::ID legacy_ReplaceBSPointerHandleAndManager_297556{ 297556 };
					// no 1.11.246.0 equivalent (used by ReplaceBSPointerHandleAndManager)
					inline constexpr Common::ID legacy_ReplaceBSPointerHandleAndManager_297712{ 297712 };
					// no 1.11.246.0 equivalent (used by ReplaceBSPointerHandleAndManager)
					inline constexpr Common::ID legacy_ReplaceBSPointerHandleAndManager_369820{ 369820 };
					// no 1.11.246.0 equivalent (used by ReplaceBSPointerHandleAndManager)
					inline constexpr Common::ID legacy_ReplaceBSPointerHandleAndManager_488462{ 488462 };
					// no 1.11.246.0 equivalent (used by ReplaceBSPointerHandleAndManager)
					inline constexpr Common::ID legacy_ReplaceBSPointerHandleAndManager_549078{ 549078 };
					// no 1.11.246.0 equivalent (used by ReplaceBSPointerHandleAndManager)
					inline constexpr Common::ID legacy_ReplaceBSPointerHandleAndManager_619127{ 619127 };
					// no 1.11.246.0 equivalent (used by ReplaceBSPointerHandleAndManager)
					inline constexpr Common::ID legacy_ReplaceBSPointerHandleAndManager_636805{ 636805 };
					// no 1.11.246.0 equivalent (used by ReplaceBSPointerHandleAndManager)
					inline constexpr Common::ID legacy_ReplaceBSPointerHandleAndManager_644059{ 644059 };
					// no 1.11.246.0 equivalent (used by ResponseWindow)
					inline constexpr Common::ID legacy_ResponseWindow_668482{ 668482 };
					// no 1.11.246.0 equivalent (used by Unicode)
					inline constexpr Common::ID legacy_Unicode_245849{ 245849 };
					// 1.11.246.0 0x143BA6D28 lpWindowName
					inline constexpr Common::ID lpWindowName{ 42440 };
					// 1.11.246.0 0x142D75470 __raise_securityfailure
					inline constexpr Common::ID raise_securityfailure{ 1632576 };
					// 1.11.246.0 0x142D75C14 __scrt_fastfail
					inline constexpr Common::ID scrt_fastfail{ 1560296 };
					// 1.11.246.0 0x141137F50 sub_141137F50
					inline constexpr Common::ID sub_1137F50{ 522253 };
					// 1.11.246.0 0x14113A6A0 sub_14113A6A0
					inline constexpr Common::ID sub_113A6A0{ 69589, 1343977 };
					// 1.11.246.0 0x1411425D0 sub_1411425D0
					inline constexpr Common::ID sub_11425D0{ 697171, 1531107 };
					// 1.11.246.0 0x141143C30 sub_141143C30
					inline constexpr Common::ID sub_1143C30{ 1515787 };
					// 1.11.246.0 0x141159DB0 sub_141159DB0
					inline constexpr Common::ID sub_1159DB0{ 70844, 1650572 };
					// 1.11.246.0 0x14117DB60 sub_14117DB60
					inline constexpr Common::ID sub_117DB60{ 580023 };
					// 1.11.246.0 0x141810B70 sub_141810B70
					inline constexpr Common::ID sub_1810B70{ 531888, 1656061 };
					// 1.11.246.0 0x141A2BB60 sub_141A2BB60
					inline constexpr Common::ID sub_1A2BB60{ 1990426 };
					// 1.11.246.0 0x141CC48D0 sub_141CC48D0
					inline constexpr Common::ID sub_1CC48D0{ 660208, 1990223 };
					// 1.11.246.0 0x141FFB440 sub_141FFB440
					inline constexpr Common::ID sub_1FFB440{ 446435, 1704908 };
					// 1.11.246.0 0x141FFBDA0 sub_141FFBDA0
					inline constexpr Common::ID sub_1FFBDA0{ 650953, 1356979 };
					// 1.11.246.0 0x141FFC100 sub_141FFC100
					inline constexpr Common::ID sub_1FFC100{ 642906, 1670063 };
					// 1.11.246.0 0x141FFC6B0 sub_141FFC6B0
					inline constexpr Common::ID sub_1FFC6B0{ 570472, 1372464 };
					// 1.11.246.0 0x141FFF220 sub_141FFF220
					inline constexpr Common::ID sub_1FFF220{ 440122, 1515122 };
					// 1.11.246.0 0x141FFF3E0 sub_141FFF3E0
					inline constexpr Common::ID sub_1FFF3E0{ 724238, 1542517 };
					// 1.11.246.0 0x141FFF470 sub_141FFF470
					inline constexpr Common::ID sub_1FFF470{ 509260, 1687367 };
					// 1.11.246.0 0x141FFF4F0 sub_141FFF4F0
					inline constexpr Common::ID sub_1FFF4F0{ 705489, 1718922 };
					// 1.11.246.0 0x141FFF570 sub_141FFF570
					inline constexpr Common::ID sub_1FFF570{ 433174, 2009876 };
					// 1.11.246.0 0x141FFF610 sub_141FFF610
					inline constexpr Common::ID sub_1FFF610{ 751443, 1618957 };
					// 1.11.246.0 0x141FFF750 sub_141FFF750
					inline constexpr Common::ID sub_1FFF750{ 635645, 1668403 };
					// 1.11.246.0 0x141FFF810 sub_141FFF810
					inline constexpr Common::ID sub_1FFF810{ 714644, 1618560 };
					// 1.11.246.0 0x142004F00 sub_142004F00
					inline constexpr Common::ID sub_2004F00{ 525631, 1991938 };
					// 1.11.246.0 0x142005120 sub_142005120
					inline constexpr Common::ID sub_2005120{ 5005, 1994738 };
					// 1.11.246.0 0x1420057C0 sub_1420057C0
					inline constexpr Common::ID sub_20057C0{ 751586, 1994657 };
					// 1.11.246.0 0x142005AB0 sub_142005AB0
					inline constexpr Common::ID sub_2005AB0{ 540140, 1499067 };
					// 1.11.246.0 0x14201A140 sub_14201A140
					inline constexpr Common::ID sub_201A140{ 1352498 };
					// 1.11.246.0 0x14204C730 sub_14204C730
					inline constexpr Common::ID sub_204C730{ 1574397 };
					// 1.11.246.0 0x14204E890 sub_14204E890
					inline constexpr Common::ID sub_204E890{ 1357980 };
					// 1.11.246.0 0x14204E8D0 sub_14204E8D0
					inline constexpr Common::ID sub_204E8D0{ 1362949 };
					// 1.11.246.0 0x142120A20 sub_142120A20
					inline constexpr Common::ID sub_2120A20{ 724189, 1347142 };
					// 1.11.246.0 0x142429820 sub_142429820
					inline constexpr Common::ID sub_2429820{ 766110, 1340478 };
					// 1.11.246.0 0x142429970 sub_142429970
					inline constexpr Common::ID sub_2429970{ 766111, 1698047 };
					// 1.11.246.0 0x142429C70 sub_142429C70
					inline constexpr Common::ID sub_2429C70{ 409166, 1382155 };
					// 1.11.246.0 0x14242A490 sub_14242A490
					inline constexpr Common::ID sub_242A490{ 650803, 1852383 };
					// 1.11.246.0 0x14242BA90 sub_14242BA90
					inline constexpr Common::ID sub_242BA90{ 766148, 1624720 };
					// 1.11.246.0 0x14242EA10 sub_14242EA10
					inline constexpr Common::ID sub_242EA10{ 158625, 1627008 };
					// 1.11.246.0 0x14242F6B0 sub_14242F6B0
					inline constexpr Common::ID sub_242F6B0{ 481720, 1519241 };
					// 1.11.246.0 0x14244BAD0 sub_14244BAD0
					inline constexpr Common::ID sub_244BAD0{ 84817, 1356072 };
					// 1.11.246.0 0x142464400 sub_142464400
					inline constexpr Common::ID sub_2464400{ 552578 };
					// 1.11.246.0 0x14246EC50 sub_14246EC50
					inline constexpr Common::ID sub_246EC50{ 767509, 1655730 };
					// 1.11.246.0 0x142475B90 sub_142475B90
					inline constexpr Common::ID sub_2475B90{ 767637, 1991833 };
					// 1.11.246.0 0x14247F9B0 sub_14247F9B0
					inline constexpr Common::ID sub_247F9B0{ 2203563 };
					// 1.11.246.0 0x14247FB80 sub_14247FB80
					inline constexpr Common::ID sub_247FB80{ 2129124 };
					// 1.11.246.0 0x14247FD60 sub_14247FD60
					inline constexpr Common::ID sub_247FD60{ 2203564 };
					// 1.11.246.0 0x1424802A0 sub_1424802A0
					inline constexpr Common::ID sub_24802A0{ 2147855 };
					// 1.11.246.0 0x1424AD530 sub_1424AD530
					inline constexpr Common::ID sub_24AD530{ 1471571 };
					// 1.11.246.0 0x1424BDB90 sub_1424BDB90
					inline constexpr Common::ID sub_24BDB90{ 768935, 1503558 };
					// 1.11.246.0 0x1424D00C0 sub_1424D00C0
					inline constexpr Common::ID sub_24D00C0{ 1621802 };
					// 1.11.246.0 0x14253D7F0 sub_14253D7F0
					inline constexpr Common::ID sub_253D7F0{ 142900, 1621640 };
					// 1.11.246.0 0x1425D1410 sub_1425D1410
					inline constexpr Common::ID sub_25D1410{ 305059, 1522044 };
					// 1.11.246.0 0x1427FA7B0 sub_1427FA7B0
					inline constexpr Common::ID sub_27FA7B0{ 103156, 1421864 };
					// 1.11.246.0 0x1402B4570 sub_1402B4570
					inline constexpr Common::ID sub_2B4570{ 2009774 };
					// 1.11.246.0 0x142B4CFF0 sub_142B4CFF0
					inline constexpr Common::ID sub_2B4CFF0{ 1569538 };
					// 1.11.246.0 0x142B4F2A0 sub_142B4F2A0
					inline constexpr Common::ID sub_2B4F2A0{ 570159, 1989813 };
					// 1.11.246.0 0x142B5DE40 sub_142B5DE40
					inline constexpr Common::ID sub_2B5DE40{ 2009800 };
					// 1.11.246.0 0x142B5EAB0 sub_142B5EAB0
					inline constexpr Common::ID sub_2B5EAB0{ 700920, 1476321 };
					// 1.11.246.0 0x142B5FA60 sub_142B5FA60
					inline constexpr Common::ID sub_2B5FA60{ 530343 };
					// 1.11.246.0 0x142B64610 sub_142B64610
					inline constexpr Common::ID sub_2B64610{ 795189, 1627205 };
					// 1.11.246.0 0x142B99AD0 sub_142B99AD0
					inline constexpr Common::ID sub_2B99AD0{ 1355670 };
					// 1.11.246.0 0x142BCC490 sub_142BCC490
					inline constexpr Common::ID sub_2BCC490{ 117776, 1666411 };
					// 1.11.246.0 0x142BD9C40 sub_142BD9C40
					inline constexpr Common::ID sub_2BD9C40{ 1619614 };
					// 1.11.246.0 0x142BDE840 sub_142BDE840
					inline constexpr Common::ID sub_2BDE840{ 122998, 1575961 };
					// 1.11.246.0 0x142BDE840 sub_142BDE840
					inline constexpr Common::ID sub_2BDE840_2{ 1575961 };
					// 1.11.246.0 0x142BDFF60 sub_142BDFF60
					inline constexpr Common::ID sub_2BDFF60{ 797202, 1623969 };
					// 1.11.246.0 0x142C733C0 sub_142C733C0
					inline constexpr Common::ID sub_2C733C0{ 721218, 1856015 };
					// 1.11.246.0 0x142CB92D0 sub_142CB92D0
					inline constexpr Common::ID sub_2CB92D0{ 419645, 1580466 };
					// 1.11.246.0 0x142CBAA30 sub_142CBAA30
					inline constexpr Common::ID sub_2CBAA30{ 718656, 1601882 };
					// 1.11.246.0 0x1402D0670 sub_1402D0670
					inline constexpr Common::ID sub_2D0670{ 116296, 1995445 };
					// 1.11.246.0 0x142D75E6C sub_142D75E6C
					inline constexpr Common::ID sub_2D75E6C{ 1780312 };
					// 1.11.246.0 0x142D816E0 sub_142D816E0
					inline constexpr Common::ID sub_2D816E0{ 811328, 1621670 };
					// 1.11.246.0 0x1402E2B50 sub_1402E2B50
					inline constexpr Common::ID sub_2E2B50{ 620742, 1816981 };
					// 1.11.246.0 0x14032D9F0 sub_14032D9F0
					inline constexpr Common::ID sub_32D9F0{ 587727 };
					// 1.11.246.0 0x140347840 sub_140347840
					inline constexpr Common::ID sub_347840{ 623435, 1989512 };
					// 1.11.246.0 0x140359D00 sub_140359D00
					inline constexpr Common::ID sub_359D00{ 370815 };
					// 1.11.246.0 0x14039AA50 sub_14039AA50
					inline constexpr Common::ID sub_39AA50{ 381562 };
					// 1.11.246.0 0x14003E2B1 sub_14003E2B1
					inline constexpr Common::ID sub_3E2B1{ 584687 };
					// 1.11.246.0 0x14046CF50 sub_14046CF50
					inline constexpr Common::ID sub_46CF50{ 394350 };
					// 1.11.246.0 0x140489830 sub_140489830
					inline constexpr Common::ID sub_489830{ 632480, 1993061 };
					// 1.11.246.0 0x14048D8D0 sub_14048D8D0
					inline constexpr Common::ID sub_48D8D0{ 632497, 1994724 };
					// 1.11.246.0 0x140492110 sub_140492110
					inline constexpr Common::ID sub_492110{ 1462644 };
					// 1.11.246.0 0x140492E90 sub_140492E90
					inline constexpr Common::ID sub_492E90{ 632518, 1388320 };
					// 1.11.246.0 0x1404C7DB0 sub_1404C7DB0
					inline constexpr Common::ID sub_4C7DB0{ 634156, 1725866 };
					// 1.11.246.0 0x1404EEB30 sub_1404EEB30
					inline constexpr Common::ID sub_4EEB30{ 1844613 };
					// 1.11.246.0 0x140520350 sub_140520350
					inline constexpr Common::ID sub_520350{ 636009, 1498789 };
					// 1.11.246.0 0x1405214D0 sub_1405214D0
					inline constexpr Common::ID sub_5214D0{ 636042, 1990688 };
					// 1.11.246.0 0x140528670 sub_140528670
					inline constexpr Common::ID sub_528670{ 1528186 };
					// 1.11.246.0 0x140528B00 sub_140528B00
					inline constexpr Common::ID sub_528B00{ 636062, 1582406 };
					// 1.11.246.0 0x140536B10 sub_140536B10
					inline constexpr Common::ID sub_536B10{ 636530, 1675545 };
					// 1.11.246.0 0x140542280 sub_140542280
					inline constexpr Common::ID sub_542280{ 1618981 };
					// 1.11.246.0 0x140542F30 sub_140542F30
					inline constexpr Common::ID sub_542F30{ 505510 };
					// 1.11.246.0 0x14054DA60 sub_14054DA60
					inline constexpr Common::ID sub_54DA60{ 314534 };
					// 1.11.246.0 0x140553D60 sub_140553D60
					inline constexpr Common::ID sub_553D60{ 1989539 };
					// 1.11.246.0 0x1405566F0 sub_1405566F0
					inline constexpr Common::ID sub_5566F0{ 1515129 };
					// 1.11.246.0 0x140558F90 sub_140558F90
					inline constexpr Common::ID sub_558F90{ 637365, 1698155 };
					// 1.11.246.0 0x140568A00 sub_140568A00
					inline constexpr Common::ID sub_568A00{ 55199, 1619017 };
					// 1.11.246.0 0x14056DA90 sub_14056DA90
					inline constexpr Common::ID sub_56DA90{ 177897, 1371227 };
					// 1.11.246.0 0x14056E940 sub_14056E940
					inline constexpr Common::ID sub_56E940{ 548371 };
					// 1.11.246.0 0x14056EC00 sub_14056EC00
					inline constexpr Common::ID sub_56EC00{ 638107, 1670900 };
					// 1.11.246.0 0x140570C30 sub_140570C30
					inline constexpr Common::ID sub_570C30{ 177103, 1933064 };
					// 1.11.246.0 0x140573AC0 sub_140573AC0
					inline constexpr Common::ID sub_573AC0{ 638120, 1475768 };
					// 1.11.246.0 0x14057F380 sub_14057F380
					inline constexpr Common::ID sub_57F380{ 111996 };
					// 1.11.246.0 0x1405CA7C0 sub_1405CA7C0
					inline constexpr Common::ID sub_5CA7C0{ 1341552 };
					// 1.11.246.0 0x1405CAA40 sub_1405CAA40
					inline constexpr Common::ID sub_5CAA40{ 1599037 };
					// 1.11.246.0 0x1405D1CD0 sub_1405D1CD0
					inline constexpr Common::ID sub_5D1CD0{ 1489339 };
					// 1.11.246.0 0x1405D2370 sub_1405D2370
					inline constexpr Common::ID sub_5D2370{ 639595, 1990553 };
					// 1.11.246.0 0x1405EB370 sub_1405EB370
					inline constexpr Common::ID sub_5EB370{ 87136, 1671826 };
					// 1.11.246.0 0x140632EA0 sub_140632EA0
					inline constexpr Common::ID sub_632EA0{ 641592, 1843783 };
					// 1.11.246.0 0x14063EE20 sub_14063EE20
					inline constexpr Common::ID sub_63EE20{ 641924, 1752948 };
					// 1.11.246.0 0x140641B60 sub_140641B60
					inline constexpr Common::ID sub_641B60{ 642003, 1548425 };
					// 1.11.246.0 0x140642E40 sub_140642E40
					inline constexpr Common::ID sub_642E40{ 642042, 1457407 };
					// 1.11.246.0 0x140642FD0 sub_140642FD0
					inline constexpr Common::ID sub_642FD0{ 642044, 1651586 };
					// 1.11.246.0 0x140643750 sub_140643750
					inline constexpr Common::ID sub_643750{ 1750857 };
					// 1.11.246.0 0x14064C270 sub_14064C270
					inline constexpr Common::ID sub_64C270{ 1657523 };
					// 1.11.246.0 0x140667770 sub_140667770
					inline constexpr Common::ID sub_667770{ 1606845 };
					// 1.11.246.0 0x140667840 sub_140667840
					inline constexpr Common::ID sub_667840{ 1654850 };
					// 1.11.246.0 0x140667DE0 sub_140667DE0
					inline constexpr Common::ID sub_667DE0{ 1654852 };
					// 1.11.246.0 0x140671DB0 sub_140671DB0
					inline constexpr Common::ID sub_671DB0{ 643131, 1820720 };
					// 1.11.246.0 0x140676190 sub_140676190
					inline constexpr Common::ID sub_676190{ 206118, 1450783 };
					// 1.11.246.0 0x140677DA0 sub_140677DA0
					inline constexpr Common::ID sub_677DA0{ 1973863 };
					// 1.11.246.0 0x140677DA0 sub_140677DA0
					inline constexpr Common::ID sub_677DA0_2{ 643197, 1973863 };
					// 1.11.246.0 0x140678E00 sub_140678E00
					inline constexpr Common::ID sub_678E00{ 535497 };
					// 1.11.246.0 0x14068CBD0 sub_14068CBD0
					inline constexpr Common::ID sub_68CBD0{ 643283, 1992435 };
					// 1.11.246.0 0x14068D580 sub_14068D580
					inline constexpr Common::ID sub_68D580{ 643289, 1753394 };
					// 1.11.246.0 0x14068D6E0 sub_14068D6E0
					inline constexpr Common::ID sub_68D6E0{ 643293, 1664109 };
					// 1.11.246.0 0x14069C240 sub_14069C240
					inline constexpr Common::ID sub_69C240{ 643475, 1634204 };
					// 1.11.246.0 0x14069C240 sub_14069C240
					inline constexpr Common::ID sub_69C240_2{ 1634204 };
					// 1.11.246.0 0x1406A67F0 sub_1406A67F0
					inline constexpr Common::ID sub_6A67F0{ 643506, 2007835 };
					// 1.11.246.0 0x1406A7AD0 sub_1406A7AD0
					inline constexpr Common::ID sub_6A7AD0{ 405154, 1993129 };
					// 1.11.246.0 0x1406A8950 sub_1406A8950
					inline constexpr Common::ID sub_6A8950{ 643528, 1351048 };
					// 1.11.246.0 0x1406BD1B0 sub_1406BD1B0
					inline constexpr Common::ID sub_6BD1B0{ 613852, 1995935 };
					// 1.11.246.0 0x1406C83A0 sub_1406C83A0
					inline constexpr Common::ID sub_6C83A0{ 466561, 1988708 };
					// 1.11.246.0 0x1406C8F50 sub_1406C8F50
					inline constexpr Common::ID sub_6C8F50{ 637699, 1580232 };
					// 1.11.246.0 0x1406D6E80 sub_1406D6E80
					inline constexpr Common::ID sub_6D6E80{ 88741, 1654076 };
					// 1.11.246.0 0x1406D74F0 sub_1406D74F0
					inline constexpr Common::ID sub_6D74F0{ 644735, 1777801 };
					// 1.11.246.0 0x1406EF7C0 sub_1406EF7C0
					inline constexpr Common::ID sub_6EF7C0{ 76508, 1361378 };
					// 1.11.246.0 0x1406F1D90 sub_1406F1D90
					inline constexpr Common::ID sub_6F1D90{ 49336 };
					// 1.11.246.0 0x1406F1E00 sub_1406F1E00
					inline constexpr Common::ID sub_6F1E00{ 581077 };
					// 1.11.246.0 0x1406F2400 sub_1406F2400
					inline constexpr Common::ID sub_6F2400{ 581078 };
					// 1.11.246.0 0x1406FB070 sub_1406FB070
					inline constexpr Common::ID sub_6FB070{ 438785 };
					// 1.11.246.0 0x140710360 sub_140710360
					inline constexpr Common::ID sub_710360{ 645724, 1445476 };
					// 1.11.246.0 0x140712B50 sub_140712B50
					inline constexpr Common::ID sub_712B50{ 645755, 1827459 };
					// 1.11.246.0 0x140714300 sub_140714300
					inline constexpr Common::ID sub_714300{ 462857, 1663801 };
					// 1.11.246.0 0x14073BE90 sub_14073BE90
					inline constexpr Common::ID sub_73BE90{ 646694, 1541828 };
					// 1.11.246.0 0x14073D090 sub_14073D090
					inline constexpr Common::ID sub_73D090{ 1673982 };
					// 1.11.246.0 0x140741290 sub_140741290
					inline constexpr Common::ID sub_741290{ 646714, 1410439 };
					// 1.11.246.0 0x1407B48A0 sub_1407B48A0
					inline constexpr Common::ID sub_7B48A0{ 650645, 1337975 };
					// 1.11.246.0 0x140832E70 sub_140832E70
					inline constexpr Common::ID sub_832E70{ 1524313 };
					// 1.11.246.0 0x140863990 sub_140863990
					inline constexpr Common::ID sub_863990{ 653585, 1754118 };
					// 1.11.246.0 0x140866080 sub_140866080
					inline constexpr Common::ID sub_866080{ 653598, 1484721 };
					// 1.11.246.0 0x14086CEC0 sub_14086CEC0
					inline constexpr Common::ID sub_86CEC0{ 452879, 1429824 };
					// 1.11.246.0 0x1408755D0 sub_1408755D0
					inline constexpr Common::ID sub_8755D0{ 636467, 1991005 };
					// 1.11.246.0 0x1408757F0 sub_1408757F0
					inline constexpr Common::ID sub_8757F0{ 384548, 1529588 };
					// 1.11.246.0 0x140876E60 sub_140876E60
					inline constexpr Common::ID sub_876E60{ 24274, 1450164 };
					// 1.11.246.0 0x14087B8E0 sub_14087B8E0
					inline constexpr Common::ID sub_87B8E0{ 654059, 1995336 };
					// 1.11.246.0 0x14087F110 sub_14087F110
					inline constexpr Common::ID sub_87F110{ 531000 };
					// 1.11.246.0 0x140882210 sub_140882210
					inline constexpr Common::ID sub_882210{ 304833, 1995710 };
					// 1.11.246.0 0x140882210 sub_140882210
					inline constexpr Common::ID sub_882210_2{ 1995710 };
					// 1.11.246.0 0x140886890 sub_140886890
					inline constexpr Common::ID sub_886890{ 654263, 1994346 };
					// 1.11.246.0 0x140886930 sub_140886930
					inline constexpr Common::ID sub_886930{ 300396, 1818942 };
					// 1.11.246.0 0x1408BE210 sub_1408BE210
					inline constexpr Common::ID sub_8BE210{ 243708, 1596835 };
					// 1.11.246.0 0x1408C5050 sub_1408C5050
					inline constexpr Common::ID sub_8C5050{ 655843, 1618628 };
					// 1.11.246.0 0x1408DA7B0 sub_1408DA7B0
					inline constexpr Common::ID sub_8DA7B0{ 135014 };
					// 1.11.246.0 0x14092F1E0 sub_14092F1E0
					inline constexpr Common::ID sub_92F1E0{ 11905, 1731262 };
					// 1.11.246.0 0x14093BB30 sub_14093BB30
					inline constexpr Common::ID sub_93BB30{ 1382630 };
					// 1.11.246.0 0x140951020 sub_140951020
					inline constexpr Common::ID sub_951020{ 531868 };
					// 1.11.246.0 0x140993790 sub_140993790
					inline constexpr Common::ID sub_993790{ 250004, 1559469 };
					// 1.11.246.0 0x14099DA30 sub_14099DA30
					inline constexpr Common::ID sub_99DA30{ 286613, 1992272 };
					// 1.11.246.0 0x1409A0F50 sub_1409A0F50
					inline constexpr Common::ID sub_9A0F50{ 313515 };
					// 1.11.246.0 0x1409C88F0 sub_1409C88F0
					inline constexpr Common::ID sub_9C88F0{ 661598, 1991629 };
					// 1.11.246.0 0x140A2D940 sub_140A2D940
					inline constexpr Common::ID sub_A2D940{ 663691, 1468392 };
					// 1.11.246.0 0x140A51340 sub_140A51340
					inline constexpr Common::ID sub_A51340{ 384122 };
					// 1.11.246.0 0x140ABE160 sub_140ABE160
					inline constexpr Common::ID sub_ABE160{ 666256, 1990278 };
					// 1.11.246.0 0x140AEF0B0 sub_140AEF0B0
					inline constexpr Common::ID sub_AEF0B0{ 664457, 1894783 };
					// 1.11.246.0 0x140B37010 sub_140B37010
					inline constexpr Common::ID sub_B37010{ 668460, 1645713 };
					// 1.11.246.0 0x140B3ABB0 sub_140B3ABB0
					inline constexpr Common::ID sub_B3ABB0{ 1992727 };
					// 1.11.246.0 0x140B3B3C0 sub_140B3B3C0
					inline constexpr Common::ID sub_B3B3C0{ 492810, 1647136 };
					// 1.11.246.0 0x140B42770 sub_140B42770
					inline constexpr Common::ID sub_B42770{ 668671, 1869554 };
					// 1.11.246.0 0x140B58490 sub_140B58490
					inline constexpr Common::ID sub_B58490{ 668914, 1541476 };
					// 1.11.246.0 0x140B58710 sub_140B58710
					inline constexpr Common::ID sub_B58710{ 668919, 1995823 };
					// 1.11.246.0 0x140B5AC40 sub_140B5AC40
					inline constexpr Common::ID sub_B5AC40{ 530432 };
					// 1.11.246.0 0x140B65D00 sub_140B65D00
					inline constexpr Common::ID sub_B65D00{ 658771, 1856176 };
					// 1.11.246.0 0x140B68750 sub_140B68750
					inline constexpr Common::ID sub_B68750{ 524811, 1994104 };
					// 1.11.246.0 0x140BB46E0 sub_140BB46E0
					inline constexpr Common::ID sub_BB46E0{ 580601 };
					// 1.11.246.0 0x140C796A0 sub_140C796A0
					inline constexpr Common::ID sub_C796A0{ 1804550 };
					// 1.11.246.0 0x140CAC840 sub_140CAC840
					inline constexpr Common::ID sub_CAC840{ 501084 };
					// 1.11.246.0 0x140CD0870 sub_140CD0870
					inline constexpr Common::ID sub_CD0870{ 127277 };
					// 1.11.246.0 0x140D0F530 sub_140D0F530
					inline constexpr Common::ID sub_D0F530{ 288239 };
					// 1.11.246.0 0x140D267E0 sub_140D267E0
					inline constexpr Common::ID sub_D267E0{ 1990514 };
					// 1.11.246.0 0x140D496E0 sub_140D496E0
					inline constexpr Common::ID sub_D496E0{ 677726, 1377514 };
					// 1.11.246.0 0x140D7F970 sub_140D7F970
					inline constexpr Common::ID sub_D7F970{ 495117 };
					// 1.11.246.0 0x140DB5B60 sub_140DB5B60
					inline constexpr Common::ID sub_DB5B60{ 407591 };
					// 1.11.246.0 0x140E5CF80 sub_140E5CF80
					inline constexpr Common::ID sub_E5CF80{ 552257 };
					// 1.11.246.0 0x140E6B380 sub_140E6B380
					inline constexpr Common::ID sub_E6B380{ 425067, 1669436 };
					// 1.11.246.0 0x140E96F00 sub_140E96F00
					inline constexpr Common::ID sub_E96F00{ 535695 };
					// 1.11.246.0 0x142F9D260 xmmword_142F9D260
					inline constexpr Common::ID xmmword_142F9D260{ 351273 };
				}
			}
		}
	}
}
