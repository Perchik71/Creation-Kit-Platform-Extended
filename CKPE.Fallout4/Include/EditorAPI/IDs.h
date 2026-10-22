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
			}
		}
	}
}
