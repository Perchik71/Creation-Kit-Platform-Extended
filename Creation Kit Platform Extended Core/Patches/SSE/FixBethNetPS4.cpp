// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixBethNetPS4.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixBethNetPS4Patch::FixBethNetPS4Patch() : Module(GlobalEnginePtr)
			{}

			bool FixBethNetPS4Patch::HasOption() const
			{
				return false;
			}

			bool FixBethNetPS4Patch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixBethNetPS4Patch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixBethNetPS4Patch::GetName() const
			{
				return "Upload PS4 modded without assets";
			}

			bool FixBethNetPS4Patch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixBethNetPS4Patch::GetDependencies() const
			{
				return {};
			}

			bool FixBethNetPS4Patch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion == EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_1130;
			}

			bool FixBethNetPS4Patch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Эта функция, что-то долго ищет, судя по всему, сбор данных, анализ.
					// На этапе её выполнения предусмотрено смена курсора, на загрузку.
					// ВЫРЕЖЕМ ЕЁ.
					//
					lpRelocator->PatchNop(lpRelocationDatabaseItem->At(0), 5);

					return true;
				}

				return false;
			}

			bool FixBethNetPS4Patch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}