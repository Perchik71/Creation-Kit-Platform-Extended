// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "SpellEnableCastingAndDeliveryAlways.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			SpellEnableCastingAndDeliveryAlwaysPatch::SpellEnableCastingAndDeliveryAlwaysPatch() : Module(GlobalEnginePtr)
			{}

			bool SpellEnableCastingAndDeliveryAlwaysPatch::HasOption() const
			{
				return false;
			}

			bool SpellEnableCastingAndDeliveryAlwaysPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* SpellEnableCastingAndDeliveryAlwaysPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* SpellEnableCastingAndDeliveryAlwaysPatch::GetName() const
			{
				return "Spell Enable Casting And Delivery Always";
			}

			bool SpellEnableCastingAndDeliveryAlwaysPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> SpellEnableCastingAndDeliveryAlwaysPatch::GetDependencies() const
			{
				return {};
			}

			bool SpellEnableCastingAndDeliveryAlwaysPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool SpellEnableCastingAndDeliveryAlwaysPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&HKEnableWindow);	// Spell fix disable Casting and Delivery
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(1), (uintptr_t)&HKEnableWindow);	// ^

					return true;
				}

				return false;
			}

			bool SpellEnableCastingAndDeliveryAlwaysPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}