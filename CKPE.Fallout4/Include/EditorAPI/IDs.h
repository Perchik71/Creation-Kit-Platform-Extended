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

					inline constexpr Common::ID SaveForm{ 177482, 1529554 };
					inline constexpr Common::ID LoadFiles{ 653593, 1994158 };
					inline constexpr Common::ID Save{ 68391, 1386733 };
					inline constexpr Common::ID LoadFileRecords{ 264710, 1495600 };
					inline constexpr Common::ID AddForm{ 167748, 1522536 };
					inline constexpr Common::ID RemoveForm{ 653538, 1993993 };
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
					inline constexpr Common::ID Singleton{ 1111370, 1986735 };
					inline constexpr Common::ID Constructor{ 577399, 1351847 };
					inline constexpr Common::ID Destructor{ 409929, 1404520 };
					inline constexpr Common::ID DeletingDestructor{ 684759, 1612300 };
					inline constexpr Common::ID Update{ 585881, 1836126 };
					inline constexpr Common::ID UpdateWeather{ 684694, 1744859 };
					inline constexpr Common::ID SetMode{ 535440 };
					inline constexpr Common::ID ReleaseWeatherOverride{ 218787 };
				}

				namespace NiCamera
				{
					inline constexpr Common::ID Constructor{ 96951, 1676096 };
					inline constexpr Common::ID SetFrustum{ 486382, 1618815 };
					inline constexpr Common::ID FitNearFarToBound{ 310174, 1622015 };
					inline constexpr Common::ID LookAt{ 699388, 1657326 };
					inline constexpr Common::ID WindowPointToRay{ 458875, 1671891 };
					inline constexpr Common::ID ScreenPtToWorldRay{ 496643, 1753159 };
					inline constexpr Common::ID WorldPtToScreenPt{ 536158, 1619043 };
					inline constexpr Common::ID WorldPtToScreenPt3{ 571066, 1565274 };
					inline constexpr Common::ID WorldPtToScreenPt3Static{ 769025, 1420850 };
					inline constexpr Common::ID UpdateWorldToCamera{ 355623, 1619422 };
					inline constexpr Common::ID UpdateWorldData{ 619373, 1600693 };
					inline constexpr Common::ID UpdateWorldBound{ 769026, 1600691 };
					inline constexpr Common::ID RTTI{ 1112416, 1545753 };
				}

				namespace BGSRenderWindowCamera
				{
					inline constexpr Common::ID Constructor{ 533084 };
					inline constexpr Common::ID Destructor{ 638414, 1675623 };
					inline constexpr Common::ID Update{ 542406, 1683762 };
					inline constexpr Common::ID FramePoint{ 638416, 1540329 };
					inline constexpr Common::ID Orbit{ 72302, 1683763 };
					inline constexpr Common::ID SetPosition{ 506956 };
					inline constexpr Common::ID SetRotation{ 230208 };
					inline constexpr Common::ID MoveLocal{ 436007 };
					inline constexpr Common::ID MoveWorld{ 566831 };
					inline constexpr Common::ID SetPivot{ 567073 };
					inline constexpr Common::ID ClearPivot{ 529543 };
					inline constexpr Common::ID UpdatePivotDirection{ 579133 };
					inline constexpr Common::ID LookAt{ 578688 };
					inline constexpr Common::ID Pan{ 584228, 1609034 };
					inline constexpr Common::ID Zoom{ 638418, 1638698 };
					inline constexpr Common::ID RotateBy{ 412014, 1734373 };
					inline constexpr Common::ID SetViewMode{ 266543, 1441140 };
					inline constexpr Common::ID SetCameraMode{ 586356 };
					inline constexpr Common::ID UpdateOrthoFrustum{ 638419, 1362071 };
				}

				namespace BGSRenderWindow
				{
					inline constexpr Common::ID Singleton{ 494701 };
					inline constexpr Common::ID Constructor{ 638105, 1994304 };
					inline constexpr Common::ID Destructor{ 470905, 1556221 };
					inline constexpr Common::ID DeletingDestructor{ 56459, 1599017 };
					inline constexpr Common::ID Update{ 177897, 1371227 };
					inline constexpr Common::ID Draw{ 520574 };
					inline constexpr Common::ID Initialize{ 638120, 1475768 };
					inline constexpr Common::ID DialogProc{ 177103, 1933064 };
					inline constexpr Common::ID UpdateDrawInfo{ 638107, 1670900 };
					inline constexpr Common::ID UpdateTitle{ 164835 };
					inline constexpr Common::ID PickObject{ 579794 };
					inline constexpr Common::ID OnMouseMove{ 638124, 1470771 };
					inline constexpr Common::ID Refresh{ 638110, 1605939 };
					inline constexpr Common::ID LoadedEvent{ 528328 };
					inline constexpr Common::ID CellEvent{ 489652 };
					inline constexpr Common::ID GetRefFromNiNode{ 495117 };

					namespace Movement
					{
						inline constexpr Common::ID Flags{ 493984 };
						inline constexpr Common::ID SnapGrid{ 506980 };
						inline constexpr Common::ID SnapAngle{ 502295 };
						inline constexpr Common::ID ArrowSnap{ 383762 };
						inline constexpr Common::ID ObjectRotate{ 401851 };
						inline constexpr Common::ID ObjectMove{ 463613 };
						inline constexpr Common::ID CameraRotate{ 526097 };
						inline constexpr Common::ID CameraZoom{ 443228 };
						inline constexpr Common::ID CameraZoomOrtho{ 526100 };
						inline constexpr Common::ID CameraPan{ 526106 };
						inline constexpr Common::ID LandspaceMult{ 526107 };
					}
				}

				namespace BGSRenderOrthoGrid
				{
					inline constexpr Common::ID Constructor{ 637980, 1508706 };
					inline constexpr Common::ID Destructor{ 637982, 1675621 };
					inline constexpr Common::ID DeletingDestructor{ 638021, 1476521 };
				}

				namespace BGSRenderWindowBorder
				{
					inline constexpr Common::ID Constructor{ 366933, 1675622 };
					inline constexpr Common::ID Destructor{ 342726, 1396321 };
					inline constexpr Common::ID DeletingDestructor{ 70712, 1599019 };
				}

				namespace BSCullingProcess
				{
					inline constexpr Common::ID Constructor{ 773529, 1382316 };
					inline constexpr Common::ID Destructor{ 189842, 1619665 };
					inline constexpr Common::ID GetRTTI{ 492362, 1532191 };
				}

				namespace BSPortalGraphEntry
				{
					inline constexpr Common::ID Constructor{ 774121, 1676417 };
					inline constexpr Common::ID Destructor{ 396887, 1619719 };
					inline constexpr Common::ID DeletingDestructor{ 229698, 1367062 };
					inline constexpr Common::ID DeleteThis{ 768211, 1596429 };
				}

				namespace BSGraphics
				{
					namespace Renderer
					{
						inline constexpr Common::ID Instance{ 496887 };
						inline constexpr Common::ID DataPointer{ 1014067, 1610923 };
						inline constexpr Common::ID CurrentWindowPointer{ 1119708, 1986997 };
						inline constexpr Common::ID DevicePointer{ 565507 };
						inline constexpr Common::ID Constructor{ 609187, 1569538 };
						inline constexpr Common::ID DataConstructor{ 795275, 1423453 };
						inline constexpr Common::ID InitializeD3D{ 530343 };
					}
					namespace State
					{
						inline constexpr Common::ID Instance{ 161367 };
						inline constexpr Common::ID Constructor{ 795776, 1687282 };
						inline constexpr Common::ID UpdateTemporalData{ 795785, 1555185 };
						inline constexpr Common::ID SetCamera{ 795781, 1648232 };
						inline constexpr Common::ID BackBufferWidth{ 583093 };
						inline constexpr Common::ID BackBufferHeight{ 583094 };
						inline constexpr Common::ID ScreenWidth{ 565026 };
						inline constexpr Common::ID ScreenHeight{ 583099 };
						inline constexpr Common::ID FrameCount{ 583101 };
						inline constexpr Common::ID FrameID{ 57812 };
					}
					namespace RenderTargetManager
					{
						inline constexpr Common::ID Instance{ 354850 };
						inline constexpr Common::ID CreateRenderTarget{ 795484, 1497380 };
						inline constexpr Common::ID CreateDepthStencilTarget{ 795486, 1593667 };
						inline constexpr Common::ID CreateCubeMapRenderTarget{ 795487, 1990541 };
						inline constexpr Common::ID AcquireRenderTarget{ 346853, 1621383 };
						inline constexpr Common::ID ReleaseRenderTarget{ 795522, 1389607 };
						inline constexpr Common::ID CreateRenderTargets{ 800387, 1690631 };
						inline constexpr Common::ID GetRenderTargetName{ 800388, 1397815 };
						inline constexpr Common::ID GetDepthStencilName{ 800390, 1667564 };
						inline constexpr Common::ID GetCubeMapName{ 250682, 1669423 };
						inline constexpr Common::ID RenderTargetNames{ 583573 };
						inline constexpr Common::ID DepthStencilNames{ 583719 };
						inline constexpr Common::ID CubeMapNames{ 474744 };
					}
				}

				namespace BGSDefaultObjectManager
				{
					inline constexpr Common::ID Constructor{ 530485 };
					inline constexpr Common::ID Load{ 27927 };
					inline constexpr Common::ID Save{ 187069 };
					inline constexpr Common::ID InitItemImpl{ 41729 };
				}

				namespace NiObject
				{
					inline constexpr Common::ID RTTI{ 494783 };
				}

				namespace NiObjectNET
				{
					inline constexpr Common::ID RTTI{ 338065, 1437333 };
				}

				namespace NiAVObject
				{
					inline constexpr Common::ID RTTI{ 920988, 1614321 };
				}

				namespace NiNode
				{
					inline constexpr Common::ID RTTI{ 721669, 1461188 };
				}

				namespace BSFadeNode
				{
					inline constexpr Common::ID RTTI{ 184085, 1614855 };
				}

				namespace BSGeometry
				{
					inline constexpr Common::ID RTTI{ 1118139, 1614880 };
				}

				namespace BSTriShape
				{
					inline constexpr Common::ID RTTI{ 943464, 1614728 };
				}

				namespace BSDynamicTriShape
				{
					inline constexpr Common::ID RTTI{ 219653, 1614819 };
				}

				namespace NiLight
				{
					inline constexpr Common::ID RTTI{ 874872, 1465372 };
				}

				namespace NiPointLight
				{
					inline constexpr Common::ID RTTI{ 958479, 1614987 };
				}

				namespace NiCollisionObject
				{
					inline constexpr Common::ID RTTI{ 1118187, 1450228 };
					inline constexpr Common::ID SetTarget{ 769362, 1496571 };
				}

				namespace NiProperty
				{
					inline constexpr Common::ID RTTI{ 806243, 1600715 };
				}

				namespace NiInterpolator
				{
					inline constexpr Common::ID RTTI{ 128511, 1617827 };
				}

				namespace TESFile
				{
					inline constexpr Common::ID Constructor{ 531000 };
				}

				namespace BSShaderProperty
				{
					inline constexpr Common::ID Constructor{ 796890, 1676127 };
					inline constexpr Common::ID SetMaterial{ 0, 1428595 };
				}

				namespace BSLightingShaderProperty
				{
					inline constexpr Common::ID Constructor{ 700052, 1676207 };
				}

				namespace BSEffectShaderProperty
				{
					inline constexpr Common::ID Constructor{ 797087, 1364224 };
				}

				namespace BSLightingShaderMaterialBase
				{
					inline constexpr Common::ID Constructor{ 800509, 1676256 };
				}

				namespace BSEffectShaderMaterial
				{
					inline constexpr Common::ID Constructor{ 285684, 1666435 };
				}

				namespace BSLight
				{
					inline constexpr Common::ID Constructor{ 799880, 1676255 };
				}

				namespace NiTexture
				{
					inline constexpr Common::ID Constructor{ 0, 1401123 };
				}

				namespace BSShader
				{
					inline constexpr Common::ID Constructor{ 128165, 1432688 };
				}

				namespace BSLightingShader
				{
					inline constexpr Common::ID Constructor{ 0, 1524881 };
					inline constexpr Common::ID Singleton{ 0, 1614989 };
				}

				namespace BSRenderPass
				{
					inline constexpr Common::ID Constructor{ 0, 1485758 };
				}

				namespace BSShaderManager
				{
					inline constexpr Common::ID Initialize{ 0, 1575961 };
					inline constexpr Common::ID CurrentCamera{ 218067 };
					inline constexpr Common::ID TimerValues{ 577501 };
					inline constexpr Common::ID ShaderArray{ 949374, 1691880 };
					inline constexpr Common::ID GetShader{ 797192, 1624133 };
					namespace State
					{
						inline constexpr Common::ID Singleton{ 123088 };
					}
				}

				namespace BSShaderData
				{
					inline constexpr Common::ID Constructor{ 0, 1648472 };
					inline constexpr Common::ID ApplyMaterialData{ 797024, 1666424 };
				}

				namespace BaseDialog
				{
					inline constexpr Common::ID GetWindowHandle{ 0, 1994744 };
					inline constexpr Common::ID DialogProc{ 0, 1520149 };
				}

				namespace ModalDialog
				{
					inline constexpr Common::ID Show{ 0, 1855063 };
				}

				namespace TESRenderControl
				{
					inline constexpr Common::ID Constructor{ 531711 };
					inline constexpr Common::ID AttachObject{ 490202 };
					inline constexpr Common::ID DetachObject{ 528228 };
					inline constexpr Common::ID ResizeViewport{ 581043 };
				}

				namespace TESPreviewControl
				{
					inline constexpr Common::ID Constructor{ 287227 };
					inline constexpr Common::ID SetPreviewObject{ 581065 };
				}

				namespace DialogExtraPreviewControl
				{
					inline constexpr Common::ID Create{ 0, 1693407 };
				}

				namespace BGSPreviewTransform
				{
					inline constexpr Common::ID UpdateDataInDialogBox{ 579817 };
					inline constexpr Common::ID UpdateDataFromDialogBox{ 579818 };
					inline constexpr Common::ID CleanupDialogBox{ 487280 };
				}

				namespace BaseExtraList
				{
					inline constexpr Common::ID AddExtra{ 651211, 1447487 };
					inline constexpr Common::ID GetByType{ 651217, 1989546 };
					inline constexpr Common::ID HasType{ 340485 };
					inline constexpr Common::ID DetachByType{ 651212, 1989880 };
					inline constexpr Common::ID RemoveAndDelete{ 168687, 1618824 };
					inline constexpr Common::ID GetCount{ 0, 1780294 };
				}

				namespace ExtraDataList
				{
					inline constexpr Common::ID Constructor{ 538478 };
					inline constexpr Common::ID GetByType{ 507082 };
					inline constexpr Common::ID AddExtra{ 0, 1566358 };
					inline constexpr Common::ID RemoveAndDelete{ 0, 1621983 };
				}

				namespace BSPointerHandleManagerInterface
				{
					inline constexpr Common::ID CreateHandle{ 297556, 1618981 };
					inline constexpr Common::ID GetCurrentHandle{ 636805, 0 };
					inline constexpr Common::ID GetSmartPointer1{ 619127, 0 };
					inline constexpr Common::ID GetSmartPointer2{ 161285, 0 };
					inline constexpr Common::ID Destroy1{ 488462, 0 };
					inline constexpr Common::ID Destroy2{ 314534, 0 };
				}

				namespace BSPointerHandleManager
				{
					inline constexpr Common::ID InitSDM{ 644059, 0 };
					inline constexpr Common::ID Table{ 0, 1591587 };
					inline constexpr Common::ID LegacyTable{ 438313, 0 };
					inline constexpr Common::ID FreeListHead{ 843708, 1435996 };
					inline constexpr Common::ID FreeListTail{ 1109175, 1873557 };
					inline constexpr Common::ID Lock{ 1109176, 1614257 };
				}

				namespace ImageSpaceManager
				{
					inline constexpr Common::ID Singleton{ 494960 };
					inline constexpr Common::ID Constructor{ 345214, 1422747 };
					inline constexpr Common::ID InitEffects{ 190238, 1577171 };
				}

				namespace NiAccumulator
				{
					inline constexpr Common::ID Constructor{ 769778, 1676560 };
				}

				namespace NiBackToFrontAccumulator
				{
					inline constexpr Common::ID Constructor{ 87865, 1411630 };
				}

				namespace NiAlphaAccumulator
				{
					inline constexpr Common::ID Constructor{ 769528, 1483782 };
				}

				namespace BSShaderAccumulator
				{
					inline constexpr Common::ID Constructor{ 797695, 1499850 };
				}

				namespace BSBatchRenderer
				{
					inline constexpr Common::ID Constructor{ 799582, 1990352 };
				}

				namespace ImageSpaceEffect
				{
					inline constexpr Common::ID Constructor{ 803752, 1396069 };
				}

				namespace BSImagespaceShader
				{
					inline constexpr Common::ID Constructor{ 111839, 1639434 };
				}

				namespace TESForm
				{
					inline constexpr Common::ID GetFile{ 495107 };
					inline constexpr Common::ID GetFormByNumericID{ 611970, 1519646 };
					inline constexpr Common::ID GetSafeFormEditorID_A{ 81623 };
					inline constexpr Common::ID GetSafeFormEditorID_B{ 489157 };
					inline constexpr Common::ID MakeEditorIDUnique{ 655843, 1618628 };
					inline constexpr Common::ID EnumFormIDs{ 1106870, 1618347 };
				}

				namespace BGSLayerDialog
				{
					inline constexpr Common::ID Constructor{ 635113, 1992398 };
					inline constexpr Common::ID GetSingleton{ 0, 1970093 };
					inline constexpr Common::ID GetActiveLayer{ 0, 1720120 };
					inline constexpr Common::ID OnRefLoadStateEvent{ 492351 };
					inline constexpr Common::ID DialogProc{ 84440, 1450938 };
				}

				namespace BGSObjectWindowLayoutManager
				{
					inline constexpr Common::ID GetSingleton{ 0, 1675541 };
				}

				namespace BGSGenericCategoryLayoutManager
				{
					inline constexpr Common::ID Show{ 0, 1459330 };
					inline constexpr Common::ID PopulateLayoutComboBox{ 0, 1620933 };
				}

				namespace DialogExtraLocalCopy
				{
					inline constexpr Common::ID Create{ 498332 };
				}

				namespace MemoryManager
				{
					namespace AutoScrapBuffer
					{
						inline constexpr Common::ID ctor{ 707709, 1494736 };
						inline constexpr Common::ID dtor{ 751485, 1662022 };
					}

					inline constexpr Common::ID InitDefaultHeap{ 394350 };
					inline constexpr Common::ID Singleton{ 618899, 1625025 };	// 1793486 AE memory address
					inline constexpr Common::ID Allocate{ 642906, 1670063 };
					inline constexpr Common::ID Deallocate{ 570472, 1372464 };
					inline constexpr Common::ID Reallocate{ 537295, 1684415 };
					inline constexpr Common::ID Size{ 446435, 1704908 };
					inline constexpr Common::ID GetThreadScrapHeap{ 679610, 1482136 };
					inline constexpr Common::ID RegisterMemoryManager{ 608113, 1424959 };
				}

				namespace BGSLocalizedString
				{
					inline constexpr Common::ID GetLength{ 409166, 1382155 };
					inline constexpr Common::ID GetStr{ 381562 };
				}

				namespace ScrapHeap
				{
					inline constexpr Common::ID ctor{ 525631, 1991938 };
					inline constexpr Common::ID Allocate{ 5005, 1994738 };
					inline constexpr Common::ID Deallocate{ 751586, 1994657 };
					inline constexpr Common::ID ReleasePages{ 540140, 1499067 };
				}				

				namespace TESFormEdit
				{
					inline constexpr Common::ID DialogProc{ 0, 1618641 };
				}

				namespace TreeListControl
				{
					inline constexpr Common::ID Create{ 646694, 1541828 };
					inline constexpr Common::ID Resize{ 646714, 1410439 };
				}

				namespace ResponseDialog
				{
					inline constexpr Common::ID AppendAudioRecord{ 0, 1436146 };
				}

				namespace TESWaterFormEditor
				{
					inline constexpr Common::ID ConstructPropertiesTab{ 129319 };
				}

				namespace BSStringCache
				{
					inline constexpr Common::ID GetSingleton{ 35135, 1626129 };
				}

				namespace Archive
				{
					inline constexpr Common::ID Constructor{ 599530, 1577865 };
				}

				namespace BGSLocAlias
				{
					inline constexpr Common::ID Constructor{ 374680 };
				}

				namespace BGSRefAlias
				{
					inline constexpr Common::ID Constructor{ 498231 };
				}

				namespace BGSRefCollectionAlias
				{
					inline constexpr Common::ID Constructor{ 226716 };
				}

				namespace BGSAttackData
				{
					inline constexpr Common::ID Constructor{ 92206 };
				}

				namespace TESCustomPackageData
				{
					inline constexpr Common::ID Constructor{ 372998 };
				}

				namespace bhkRefObject
				{
					inline constexpr Common::ID SetReferencedObject{ 725722, 1521302 };
				}

				namespace bhkWorld
				{
					inline constexpr Common::ID Update{ 753440, 1626434 };
					inline constexpr Common::ID Init{ 753447, 1505182 };
				}

				namespace bhkNPCollisionObject
				{
					inline constexpr Common::ID CreateInstance{ 753894, 1599525 };
					inline constexpr Common::ID AddToWorld{ 753901, 1334468 };
					inline constexpr Common::ID RemoveFromWorld{ 705852, 1599528 };
					inline constexpr Common::ID SetCollisionFilterInfo{ 108700, 1582399 };
					inline constexpr Common::ID GetBody{ 377675, 1626976 };
				}

				namespace bhkThreadMemorySource
				{
					inline constexpr Common::ID Constructor{ 724189, 1347142 };
				}

				namespace bhkCharacterController
				{
					inline constexpr Common::ID SetOrientationController{ 46640, 1838184 };
				}

				namespace hkaAnimationControl
				{
					inline constexpr Common::ID Ctor{ 742083, 1629421 };
					inline constexpr Common::ID InitWeights{ 106589, 1534443 };
				}

				namespace hkaDefaultAnimationControl
				{
					inline constexpr Common::ID Ctor{ 233793, 1628644 };
				}

				namespace TESBitArrayFile
				{
					inline constexpr Common::ID GetBit{ 278609, 1840653 };
					inline constexpr Common::ID SetBit{ 200021 };
					inline constexpr Common::ID GetByte{ 25027, 1618634 };
					inline constexpr Common::ID SetByte{ 43380, 1618639 };
					inline constexpr Common::ID GetNibble{ 643104, 1532694 };
					inline constexpr Common::ID SetNibble{ 226424, 1689550 };
				}

				namespace BSAnimationGraphManager
				{
					inline constexpr Common::ID NotifyAnimationGraph{ 405302, 1475908 };
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
					// 1.11.246.0 0x14261D020 BSIStream::BSIStream
					inline constexpr Common::ID BSIStream__BSIStream{ 774104, 1569121 };
					// 1.11.246.0 0x14261D020 BSIStream::BSIStream
					inline constexpr Common::ID BSIStream__BSIStream_2{ 1569121 };
					// 1.11.246.0 0x14248BA20 BSResource::GetLooseFileSize32
					inline constexpr Common::ID BSResource__GetLooseFileSize32{ 712081, 2009874 };
					// 1.11.246.0 0x14247BD40 BSResource::LooseFileStream_Create
					inline constexpr Common::ID BSResource__LooseFileStream_Create{ 193501, 1992729 };
					// 1.11.246.0 0x14247C6A0 BSResource::OpenLooseFile
					inline constexpr Common::ID BSResource__OpenLooseFile{ 767841, 1456433 };
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
					// 1.11.246.0 0x1408805B0 TESFile::LoadTESInfo
					inline constexpr Common::ID TESFile__LoadTESInfo{ 654208, 1514423 };
					// 1.11.246.0 0x1408810C0 TESFile::WriteTESInfo
					inline constexpr Common::ID TESFile__WriteTESInfo{ 532384 };
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
					// 1.11.246.0 0x143D27D80 
					inline constexpr Common::ID data_3D27D80{ 1591587 };
					// 1.11.246.0 0x143D2F9B4 
					inline constexpr Common::ID data_3D2F9B4{ 1435996 };
					// 1.11.246.0 0x143D2F9B8 
					inline constexpr Common::ID data_3D2F9B8{ 1873557 };
					// 1.11.246.0 0x143D49308 
					inline constexpr Common::ID data_3D49308{ 451607 };
					// 1.11.246.0 0x143D5E564 
					inline constexpr Common::ID data_3D5E564{ 365092, 1990122 };
					// 1.11.246.0 0x143D5E5B8 
					inline constexpr Common::ID data_3D5E5B8{ 1110197, 1493376 };
					// 1.11.246.0 0x143D6EC30 
					inline constexpr Common::ID data_3D6EC30{ 394722 };
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
					// 1.11.246.0 0x141137B70 CSScript::PickScriptsToCompileDlg::DlgProc
					inline constexpr Common::ID PapyrusCompileDialog_DlgProc{ 580037 };
					// 1.11.246.0 0x141137F50 sub_141137F50
					inline constexpr Common::ID PapyrusCompileDialog_UpdateButtons{ 522253 };
					// 1.11.246.0 0x142847CA0 BNet::Log
					inline constexpr Common::ID BNet_Log{ 200383, 1680890 };
					// 1.11.246.0 0x140488130 BGSArchiveBrowser stdout capture lambda 1
					inline constexpr Common::ID BGSArchiveBrowser_AppendOutput_1{ 428354, 2007543 };
					// 1.11.246.0 0x140488140 BGSArchiveBrowser stdout capture lambda 2
					inline constexpr Common::ID BGSArchiveBrowser_AppendOutput_2{ 632413, 1472816 };
					// 1.11.246.0 0x140488230 BGSArchiveBrowser stdout capture lambda 3
					inline constexpr Common::ID BGSArchiveBrowser_AppendOutput_3{ 118013, 1507206 };
					// 1.11.246.0 0x1404C8AA0 BGSErrorDlg::OnInitDialog
					inline constexpr Common::ID BGSErrorDlg_OnInitDialog{ 532471 };
					// 1.11.246.0 0x143BAA070 bPreviewShowLandPlane:PreviewMovement value byte
					inline constexpr Common::ID bPreviewShowLandPlane{ 7994 };
					// 1.11.246.0 0x14053AF30 ModelData::DlgProc
					inline constexpr Common::ID ModelData_DlgProc{ 436240, 1690230 };
					// 1.11.246.0 0x140068377 DialogExtraPreviewControl::Get
					inline constexpr Common::ID DialogExtraPreviewControl_Get{ 432191 };
					// 1.11.246.0 0x140729A30 TESPreviewControl::ToggleGroundPlane
					inline constexpr Common::ID TESPreviewControl_ToggleGroundPlane{ 76398, 1729980 };
					// 1.11.246.0 0x14113A6A0 sub_14113A6A0
					inline constexpr Common::ID PapyrusScriptEditWindow_Create{ 69589, 1343977 };
					// 1.11.246.0 0x1411425D0 sub_1411425D0
					inline constexpr Common::ID PapyrusScriptEditWindow_WndProc{ 697171, 1531107 };
					// 1.11.246.0 0x141143C30 sub_141143C30
					inline constexpr Common::ID PapyrusScriptEditWindow_StorePlacement{ 1515787 };
					// 1.11.246.0 0x141159DB0 sub_141159DB0
					inline constexpr Common::ID PapyrusPropertiesWindow_Create{ 70844, 1650572 };
					// 1.11.246.0 0x14117DB60 sub_14117DB60
					inline constexpr Common::ID sub_117DB60{ 580023 };
					// 1.11.246.0 0x141810B70 sub_141810B70
					inline constexpr Common::ID zlib_inflate{ 531888, 1656061 };
					// 1.11.246.0 0x141A2BB60 sub_141A2BB60
					inline constexpr Common::ID sub_1A2BB60{ 1990426 };
					// 1.11.246.0 0x141CC48D0 sub_141CC48D0
					inline constexpr Common::ID sub_1CC48D0{ 660208, 1990223 };
					// 1.11.246.0 0x141FFBDA0 sub_141FFBDA0
					inline constexpr Common::ID sub_1FFBDA0{ 650953, 1356979 };
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
					// 1.11.246.0 0x14201A140 sub_14201A140
					inline constexpr Common::ID sub_201A140{ 1352498 };
					// 1.11.246.0 0x14204C730 sub_14204C730
					inline constexpr Common::ID sub_204C730{ 1574397 };
					// 1.11.246.0 0x14204E890 sub_14204E890
					inline constexpr Common::ID set_purecall_handler_wrapper{ 1357980 };
					// 1.11.246.0 0x14204E8D0 sub_14204E8D0
					inline constexpr Common::ID sub_204E8D0{ 1362949 };
					// 1.11.246.0 0x142429820 sub_142429820
					inline constexpr Common::ID BSStringCache__Ref__Ref{ 766110, 1340478 };
					// 1.11.246.0 0x142429970 sub_142429970
					inline constexpr Common::ID BSStringCache__Ref__set{ 766111, 1698047 };
					// 1.11.246.0 0x14242A490 sub_14242A490
					inline constexpr Common::ID sub_242A490{ 650803, 1852383 };
					// 1.11.246.0 0x14242BA90 sub_14242BA90
					inline constexpr Common::ID BSStringCache__Ref__release{ 766148, 1624720 };
					// 1.11.246.0 0x14242EA10 sub_14242EA10
					inline constexpr Common::ID sub_242EA10{ 158625, 1627008 };
					// 1.11.246.0 0x14242F6B0 sub_14242F6B0
					inline constexpr Common::ID BSFile__BSFile{ 481720, 1519241 };
					// 1.11.246.0 0x14244BAD0 sub_14244BAD0
					inline constexpr Common::ID LoadPluginsTxt{ 84817, 1356072 };
					// 1.11.246.0 0x142464400 sub_142464400
					inline constexpr Common::ID sub_2464400{ 552578 };
					// 1.11.246.0 0x14246EC50 sub_14246EC50
					inline constexpr Common::ID sub_246EC50{ 767509, 1655730 };
					// 1.11.246.0 0x142475B90 sub_142475B90
					inline constexpr Common::ID sub_2475B90{ 767637, 1991833 };
					// 1.11.246.0 0x14247F9B0 sub_14247F9B0
					inline constexpr Common::ID LooseFileStream_DoRead{ 2203563 };
					// 1.11.246.0 0x14247FB80 sub_14247FB80
					inline constexpr Common::ID LooseFileStream_DoReadAlt{ 2129124 };
					// 1.11.246.0 0x14247FD60 sub_14247FD60
					inline constexpr Common::ID LooseFileStream_DoSeek{ 2203564 };
					// 1.11.246.0 0x1424802A0 sub_1424802A0
					inline constexpr Common::ID LooseFileStream_DoWrite{ 2147855 };
					// 1.11.246.0 0x1424AD530 sub_1424AD530
					inline constexpr Common::ID sub_24AD530{ 1471571 };
					// 1.11.246.0 0x1424BDB90 sub_1424BDB90
					inline constexpr Common::ID sub_24BDB90{ 768935, 1503558 };
					// 1.11.246.0 0x1424D00C0 sub_1424D00C0
					inline constexpr Common::ID TESObjectLAND_NormalizeLayers{ 1621802 };
					// 1.11.246.0 0x14253D7F0 sub_14253D7F0
					inline constexpr Common::ID NiControllerSequence__StoreTargets{ 142900, 1621640 };
					// 1.11.246.0 0x1425D1410 sub_1425D1410
					inline constexpr Common::ID sub_25D1410{ 305059, 1522044 };
					// 1.11.246.0 0x1427FA7B0 sub_1427FA7B0
					inline constexpr Common::ID sub_27FA7B0{ 103156, 1421864 };
					// 1.11.246.0 0x1402B4570 sub_1402B4570
					inline constexpr Common::ID BSGraphics_LoadD3DCompiler_A{ 2009774 };
					// 1.11.246.0 0x142B4CFF0 sub_142B4CFF0
					inline constexpr Common::ID BSGraphics_LoadD3DCompiler_B{ 1569538 };
					// 1.11.246.0 0x142B4F2A0 sub_142B4F2A0
					inline constexpr Common::ID BSGraphics_EndFrame{ 570159, 1989813 };
					// 1.11.246.0 0x142B5DE40 sub_142B5DE40
					inline constexpr Common::ID BSGraphics_LoadD3DCompiler_C{ 2009800 };
					// 1.11.246.0 0x142B5EAB0 sub_142B5EAB0
					inline constexpr Common::ID BSGraphics_LogOutOfUCodeSpace{ 700920, 1476321 };
					// 1.11.246.0 0x142B64610 sub_142B64610
					inline constexpr Common::ID BSGraphics_LogOutOfPixelShaders{ 795189, 1627205 };
					// 1.11.246.0 0x142B99AD0 sub_142B99AD0
					inline constexpr Common::ID sub_2B99AD0{ 1355670 };
					// 1.11.246.0 0x142BCC490 sub_142BCC490
					inline constexpr Common::ID sub_2BCC490{ 117776, 1666411 };
					// 1.11.246.0 0x142BD9C40 sub_142BD9C40
					inline constexpr Common::ID sub_2BD9C40{ 1619614 };
					// 1.11.246.0 0x142BDE840 sub_142BDE840
					inline constexpr Common::ID BSShaderManager_LoadShaderPackages{ 122998, 1575961 };
					// 1.11.246.0 0x142BDE840 sub_142BDE840
					inline constexpr Common::ID BSShaderManager_LoadShaderPackages_2{ 1575961 };
					// 1.11.246.0 0x142BDFF60 sub_142BDFF60
					inline constexpr Common::ID BSShaderManager__GetTexture{ 797202, 1623969 };
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
					inline constexpr Common::ID NiBSScrapAdapter_AllocateFromBSScrap{ 811328, 1621670 };
					// 1.11.246.0 0x1402E2B50 sub_1402E2B50
					inline constexpr Common::ID sub_2E2B50{ 620742, 1816981 };
					// 1.11.246.0 0x14032D9F0 sub_14032D9F0
					inline constexpr Common::ID sub_32D9F0{ 587727 };
					// 1.11.246.0 0x140347840 sub_140347840
					inline constexpr Common::ID sub_347840{ 623435, 1989512 };
					// 1.11.246.0 0x140359D00 sub_140359D00
					inline constexpr Common::ID sub_359D00{ 370815 };
					// 1.11.246.0 0x14003E2B1 sub_14003E2B1
					inline constexpr Common::ID sub_3E2B1{ 584687 };
					// 1.11.246.0 0x140489830 sub_140489830
					inline constexpr Common::ID sub_489830{ 632480, 1993061 };
					// 1.11.246.0 0x14048D8D0 sub_14048D8D0
					inline constexpr Common::ID sub_48D8D0{ 632497, 1994724 };
					// 1.11.246.0 0x140492110 sub_140492110
					inline constexpr Common::ID sub_492110{ 1462644 };
					// 1.11.246.0 0x140492E90 sub_140492E90
					inline constexpr Common::ID sub_492E90{ 632518, 1388320 };
					// 1.11.246.0 0x1404C7DB0 sub_1404C7DB0
					inline constexpr Common::ID ProgressWindow_Create{ 634156, 1725866 };
					// 1.11.246.0 0x1404EEB30 sub_1404EEB30
					inline constexpr Common::ID LayersWindow_LoadImageList{ 1844613 };
					// 1.11.246.0 0x140520350 sub_140520350
					inline constexpr Common::ID NavMeshWindow_WndProc{ 636009, 1498789 };
					// 1.11.246.0 0x1405214D0 sub_1405214D0
					inline constexpr Common::ID ObjectWindow_DlgProc{ 636042, 1990688 };
					// 1.11.246.0 0x140528670 sub_140528670
					inline constexpr Common::ID sub_528670{ 1528186 };
					// 1.11.246.0 0x140528B00 sub_140528B00
					inline constexpr Common::ID sub_528B00{ 636062, 1582406 };
					// 1.11.246.0 0x140536B10 sub_140536B10
					inline constexpr Common::ID sub_536B10{ 636530, 1675545 };
					// 1.11.246.0 0x140542280 sub_140542280
					inline constexpr Common::ID BSPointerHandleManagerInterface__GetCurrentHandle{ 1618981 };
					// 1.11.246.0 0x140542F30 sub_140542F30
					inline constexpr Common::ID BSUntypedPointerHandle__Set{ 505510 };
					// 1.11.246.0 0x14054DA60 sub_14054DA60
					inline constexpr Common::ID BSHandleRefObject__DecRefCount{ 314534 };
					// 1.11.246.0 0x140553D60 sub_140553D60
					inline constexpr Common::ID sub_553D60{ 1989539 };
					// 1.11.246.0 0x1405566F0 sub_1405566F0
					inline constexpr Common::ID sub_5566F0{ 1515129 };
					// 1.11.246.0 0x140558F90 sub_140558F90
					inline constexpr Common::ID sub_558F90{ 637365, 1698155 };
					// 1.11.246.0 0x140568A00 sub_140568A00
					inline constexpr Common::ID Hotkeys_LoadSettings{ 55199, 1619017 };
					// 1.11.246.0 0x14056E940 sub_14056E940
					inline constexpr Common::ID sub_56E940{ 548371 };
					// 1.11.246.0 0x14057F380 sub_14057F380
					inline constexpr Common::ID RenderWindow_RegisterHotkeyFunctions{ 111996 };
					// 1.11.246.0 0x1405CA7C0 sub_1405CA7C0
					inline constexpr Common::ID sub_5CA7C0{ 1341552 };
					// 1.11.246.0 0x1405CAA40 sub_1405CAA40
					inline constexpr Common::ID sub_5CAA40{ 1599037 };
					// 1.11.246.0 0x1405D1CD0 sub_1405D1CD0
					inline constexpr Common::ID sub_5D1CD0{ 1489339 };
					// 1.11.246.0 0x1405D2370 sub_1405D2370
					inline constexpr Common::ID sub_5D2370{ 639595, 1990553 };
					// 1.11.246.0 0x1405EB370 sub_1405EB370
					inline constexpr Common::ID TESObjectREFR__SetPosition{ 87136, 1671826 };
					// 1.11.246.0 0x140632EA0 sub_140632EA0
					inline constexpr Common::ID sub_632EA0{ 641592, 1843783 };
					// 1.11.246.0 0x14063EE20 sub_14063EE20
					inline constexpr Common::ID ComboBox_InsertItem{ 641924, 1752948 };
					// 1.11.246.0 0x140641B60 sub_140641B60
					inline constexpr Common::ID sub_641B60{ 642003, 1548425 };
					// 1.11.246.0 0x140642E40 sub_140642E40
					inline constexpr Common::ID sub_642E40{ 642042, 1457407 };
					// 1.11.246.0 0x140642FD0 sub_140642FD0
					inline constexpr Common::ID sub_642FD0{ 642044, 1651586 };
					// 1.11.246.0 0x140643750 sub_140643750
					inline constexpr Common::ID sub_643750{ 1750857 };
					// 1.11.246.0 0x14064C270 sub_14064C270
					inline constexpr Common::ID ResponseWindow_RunLipGenerator{ 1657523 };
					// 1.11.246.0 0x140667770 sub_140667770
					inline constexpr Common::ID ChooseSoundFile_Browse{ 1606845 };
					// 1.11.246.0 0x140667840 sub_140667840
					inline constexpr Common::ID ChooseSoundFile_BrowseAlt{ 1654850 };
					// 1.11.246.0 0x140667DE0 sub_140667DE0
					inline constexpr Common::ID ChooseSoundFile_MakeRelativePath{ 1654852 };
					// 1.11.246.0 0x140671DB0 sub_140671DB0
					inline constexpr Common::ID CellViewWindow_WndProc{ 643131, 1820720 };
					// 1.11.246.0 0x140676190 sub_140676190
					inline constexpr Common::ID sub_676190{ 206118, 1450783 };
					// 1.11.246.0 0x140677DA0 sub_140677DA0
					inline constexpr Common::ID sub_677DA0{ 1973863 };
					// 1.11.246.0 0x140677DA0 sub_140677DA0
					inline constexpr Common::ID sub_677DA0_2{ 643197, 1973863 };
					// 1.11.246.0 0x140678E00 sub_140678E00
					inline constexpr Common::ID sub_678E00{ 535497 };
					// 1.11.246.0 0x14068CBD0 sub_14068CBD0
					inline constexpr Common::ID DataWindow_WndProc{ 643283, 1992435 };
					// 1.11.246.0 0x14068D580 sub_14068D580
					inline constexpr Common::ID sub_68D580{ 643289, 1753394 };
					// 1.11.246.0 0x14068D6E0 sub_14068D6E0
					inline constexpr Common::ID DataWindow_UpdateFileInfo{ 643293, 1664109 };
					// 1.11.246.0 0x14069C240 sub_14069C240
					inline constexpr Common::ID MainWindow_WndProc{ 643475, 1634204 };
					// 1.11.246.0 0x14069C240 sub_14069C240
					inline constexpr Common::ID MainWindow_WndProc_2{ 1634204 };
					// 1.11.246.0 0x1406A67F0 sub_1406A67F0
					inline constexpr Common::ID MainWindow_UpdateTitle{ 643506, 2007835 };
					// 1.11.246.0 0x1406A7AD0 sub_1406A7AD0
					inline constexpr Common::ID sub_6A7AD0{ 405154, 1993129 };
					// 1.11.246.0 0x1406A8950 sub_1406A8950
					inline constexpr Common::ID VersionControl_PromptNetworkLocation{ 643528, 1351048 };
					// 1.11.246.0 0x1406BD1B0 sub_1406BD1B0
					inline constexpr Common::ID MainWindow_SaveActiveFile{ 613852, 1995935 };
					// 1.11.246.0 0x1406C83A0 sub_1406C83A0
					inline constexpr Common::ID TimeOfDayDialog_Initialize{ 466561, 1988708 };
					// 1.11.246.0 0x1406C8F50 sub_1406C8F50
					inline constexpr Common::ID TimeOfDayDialog_SetNewValue{ 637699, 1580232 };
					// 1.11.246.0 0x1406D6E80 sub_1406D6E80
					inline constexpr Common::ID TimeOfDayDialog_CreateControls{ 88741, 1654076 };
					// 1.11.246.0 0x1406D74F0 sub_1406D74F0
					inline constexpr Common::ID sub_6D74F0{ 644735, 1777801 };
					// 1.11.246.0 0x1406EF7C0 sub_1406EF7C0
					inline constexpr Common::ID sub_6EF7C0{ 76508, 1361378 };
					// 1.11.246.0 0x1406F1D90 sub_1406F1D90
					inline constexpr Common::ID TESBitArray_GetFormIDAsBitArrayIndex{ 49336 };
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
					inline constexpr Common::ID OpenPluginSaveDialog{ 462857, 1663801 };
					// 1.11.246.0 0x14073D090 sub_14073D090
					inline constexpr Common::ID TreeList_WndProc{ 1673982 };
					// 1.11.246.0 0x1407B48A0 sub_1407B48A0
					inline constexpr Common::ID sub_7B48A0{ 650645, 1337975 };
					// 1.11.246.0 0x140832E70 sub_140832E70
					inline constexpr Common::ID sub_832E70{ 1524313 };
					// 1.11.246.0 0x140863990 sub_140863990
					inline constexpr Common::ID sub_863990{ 653585, 1754118 };
					// 1.11.246.0 0x140866080 sub_140866080
					inline constexpr Common::ID TESDataHandler_ValidateForms{ 653598, 1484721 };
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
					// 1.11.246.0 0x140882210 sub_140882210
					inline constexpr Common::ID TESDataHandler_LoadFileDependencies{ 304833, 1995710 };
					// 1.11.246.0 0x140882210 sub_140882210
					inline constexpr Common::ID TESDataHandler_LoadFileDependencies_2{ 1995710 };
					// 1.11.246.0 0x140886890 sub_140886890
					inline constexpr Common::ID sub_886890{ 654263, 1994346 };
					// 1.11.246.0 0x140886930 sub_140886930
					inline constexpr Common::ID sub_886930{ 300396, 1818942 };
					// 1.11.246.0 0x1408BE210 sub_1408BE210
					inline constexpr Common::ID sub_8BE210{ 243708, 1596835 };
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
					inline constexpr Common::ID ExportFaceTintText{ 666256, 1990278 };
					// 1.11.246.0 0x140AEF0B0 sub_140AEF0B0
					inline constexpr Common::ID sub_AEF0B0{ 664457, 1894783 };
					// 1.11.246.0 0x140B37010 sub_140B37010
					inline constexpr Common::ID ResponseWindow_DlgProc{ 668460, 1645713 };
					// 1.11.246.0 0x140B3ABB0 sub_140B3ABB0
					inline constexpr Common::ID sub_B3ABB0{ 1992727 };
					// 1.11.246.0 0x140B3B3C0 sub_140B3B3C0
					inline constexpr Common::ID ResponseWindow_SelectIdle{ 492810, 1647136 };
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
					inline constexpr Common::ID ValidateBookText{ 1804550 };
					// 1.11.246.0 0x140CAC840 sub_140CAC840
					inline constexpr Common::ID sub_CAC840{ 501084 };
					// 1.11.246.0 0x140CD0870 sub_140CD0870
					inline constexpr Common::ID sub_CD0870{ 127277 };
					// 1.11.246.0 0x140D0F530 sub_140D0F530
					inline constexpr Common::ID sub_D0F530{ 288239 };
					// 1.11.246.0 0x140D267E0 sub_140D267E0
					inline constexpr Common::ID PreCombined_Generate{ 1990514 };
					// 1.11.246.0 0x140D496E0 sub_140D496E0
					inline constexpr Common::ID PreCombined_GetNifPath{ 677726, 1377514 };
					// 1.11.246.0 0x140DB5B60 sub_140DB5B60
					inline constexpr Common::ID sub_DB5B60{ 407591 };
					// 1.11.246.0 0x140E5CF80 sub_140E5CF80
					inline constexpr Common::ID sub_E5CF80{ 552257 };
					// 1.11.246.0 0x140E6B380 sub_140E6B380
					inline constexpr Common::ID Conditions_CreateControls{ 425067, 1669436 };
					// 1.11.246.0 0x140E96F00 sub_140E96F00
					inline constexpr Common::ID sub_E96F00{ 535695 };
					// 1.11.246.0 0x142F9D260 xmmword_142F9D260
					inline constexpr Common::ID xmmword_142F9D260{ 351273 };
				}
			}
		}
	}
}
