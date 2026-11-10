#include <CKPE.Common.ID.h>

#pragma once

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace IDs
			{
				namespace StringCache
				{
					inline constexpr Common::ID StringPool_AnsiRelease{ 216797, 1016197 };
					inline constexpr Common::ID StringPool_WideRelease{ 758662, 1016177 };

					inline constexpr Common::ID BucketTable_Singleton{ 591027 };

					inline constexpr Common::ID FixedString_AnsiCreate{ 604959 };
					inline constexpr Common::ID FixedString_WideCreate{ 519648 };
				}

				namespace BSResource
				{
					namespace ID
					{
						inline constexpr Common::ID GenerateFromPath{ 758809, 1014466 };
					}
				}

				namespace TESDataHandler
				{
					inline constexpr Common::ID Instance{ 476691 };
					inline constexpr Common::ID UserModded{ 223341 };
				}

				namespace TESForm
				{
					inline constexpr Common::ID GetFile{ 569205 };
					inline constexpr Common::ID GetFormByNumericID{ 76804 };
				}

				namespace TESObjectDOOR
				{
					inline constexpr Common::ID Save{ 341948 };
				}

				namespace BGSRenderWindowCamera
				{
					inline constexpr Common::ID Ctor{ 253786 };
					inline constexpr Common::ID SetViewMode{ 343459 };
				}

				namespace BGSRenderWindow
				{
					inline constexpr Common::ID Singleton{ 78593 };
					inline constexpr Common::ID WndProc{ 287695 };
					inline constexpr Common::ID UpdateTitle{ 65157 };
					inline constexpr Common::ID HKFunc_ToggleOrthographicPerspective{ 336765 };
				}

				namespace MainWindow
				{
					inline constexpr Common::ID WndProc{ 465745 };
				}

				namespace bhkThreadMemorySource
				{
					inline constexpr Common::ID ctor{ 758593, 948874 };
				}

				namespace NiMemoryManager
				{
					inline constexpr Common::ID Singleton{ 457123 };
				}

				namespace MemoryManager
				{
					namespace AutoScrapBuffer
					{
						inline constexpr Common::ID ctor{ 508555 };
						inline constexpr Common::ID dtor{ 31234 };
					}

					inline constexpr Common::ID Singleton{ 571890 };
					inline constexpr Common::ID InitDefaultHeap{ 28172 };
					inline constexpr Common::ID Allocate{ 105314 };
					inline constexpr Common::ID Reallocate{ 187127 };
					inline constexpr Common::ID Deallocate{ 253947 };
					inline constexpr Common::ID Size{ 657554 };
					inline constexpr Common::ID RegisterMemoryManager{ 385186 };		
					inline constexpr Common::ID GetThreadScrapHeap{ 162230, 918799 };
				}

				namespace BSSmallBlockAllocator
				{
					inline constexpr Common::ID Init{ 599362, 1025020 };
				}

				namespace ScrapHeap
				{
					inline constexpr Common::ID ctor{ 757575, 1016911 };
					inline constexpr Common::ID dtor{ 607309 };
					inline constexpr Common::ID Allocate{ 317676 };
					inline constexpr Common::ID Deallocate{ 757576, 974040 };
					inline constexpr Common::ID ReleasePages{ 605622 };
				}

				namespace Setting
				{
					namespace INISettingCollection
					{
						inline constexpr Common::ID Singleton{ 366656, 1023008 };
						inline constexpr Common::ID GetSingleton{ 563946 };
					}

					namespace INIPrefSettingCollection
					{
						inline constexpr Common::ID Singleton{ 239737, 1025711 };
						inline constexpr Common::ID GetSingleton{ 476720 };
					}

					namespace GameSettingCollection
					{
						inline constexpr Common::ID Singleton{ 361977 };
						inline constexpr Common::ID GetSingleton{ 247182 };
					}
				}

				namespace BGSRenderShowHideDlg
				{
					inline constexpr Common::ID RefreshChecks{ 289812 };

					namespace Settings
					{
						inline constexpr Common::ID ShowMarkers{ 274506 };
						inline constexpr Common::ID ShowLightMarkers{ 197370 };
						inline constexpr Common::ID ShowHelperObjects{ 11713 };
						inline constexpr Common::ID ShowActivators{ 191326 };
						inline constexpr Common::ID ShowDoors{ 232996 };
						inline constexpr Common::ID ShowTerrain{ 272963 };
						inline constexpr Common::ID ShowWater{ 103293 };
						inline constexpr Common::ID ShowStatics{ 274529 };
						inline constexpr Common::ID ShowActors{ 251591 };
						inline constexpr Common::ID ShowMultibounds{ 45024 };
						inline constexpr Common::ID ShowDynamics{ 138838 };
						inline constexpr Common::ID ShowOcclusionPlanes{ 121906 };
						inline constexpr Common::ID ShowPortalsAndRooms{ 274518 };
						inline constexpr Common::ID ShowSoundMarkers{ 159141 };
						inline constexpr Common::ID ShowCollisionObjects{ 19146 };
						inline constexpr Common::ID ShowNonCollectibleObjects{ 217534 };
						inline constexpr Common::ID ShowDecals{ 251593 };
					}
				}
			}
		}
	}
}


