// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixMusicTrackForm.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixMusicTrackFormPatch::FixMusicTrackFormPatch() : Module(GlobalEnginePtr)
			{}

			bool FixMusicTrackFormPatch::HasOption() const
			{
				return false;
			}

			bool FixMusicTrackFormPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixMusicTrackFormPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixMusicTrackFormPatch::GetName() const
			{
				return "Fix MusicTrack Form";
			}

			bool FixMusicTrackFormPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool FixMusicTrackFormPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix for crash when saving a plugin with an empty single track file path in a Music Track form. 
					// Null pointer dereference.
					//
					lpRelocator->DetourCall(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);

					return true;
				}

				return false;
			}

			bool FixMusicTrackFormPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			size_t FixMusicTrackFormPatch::sub(const char* String)
			{
				return String ? strlen(String) : 0;
			}
		}
	}
}