// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "ChooseSoundFileF4.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			uintptr_t pointer_ChooseSoundFile_sub = 0;

			ChooseSoundFilePatch::ChooseSoundFilePatch() : Module(GlobalEnginePtr)
			{}

			bool ChooseSoundFilePatch::HasOption() const
			{
				return false;
			}

			bool ChooseSoundFilePatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* ChooseSoundFilePatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* ChooseSoundFilePatch::GetName() const
			{
				return "Choose Sound File";
			}

			bool ChooseSoundFilePatch::HasDependencies() const
			{
				return false;
			}

			Array<String> ChooseSoundFilePatch::GetDependencies() const
			{
				return {};
			}

			bool ChooseSoundFilePatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool ChooseSoundFilePatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// The fixed is to select an audio file, also *.xwm.
					//
					lpRelocator->DetourCall(_RELDATA_RAV(0), (uintptr_t)&sub);
					lpRelocator->DetourCall(_RELDATA_RAV(1), (uintptr_t)&sub);
					lpRelocator->DetourCall(_RELDATA_RAV(2), (uintptr_t)&sub);
					lpRelocator->DetourCall(_RELDATA_RAV(3), (uintptr_t)&sub);
					pointer_ChooseSoundFile_sub = _RELDATA_ADDR(4);

					return true;
				}

				return false;
			}

			bool ChooseSoundFilePatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			bool ChooseSoundFilePatch::sub(__int64 unknown, const char* lpPath, const char* lpFormat, const char* lpCaption, 
				char* lpFileName)
			{
				return fastCall<bool>(pointer_ChooseSoundFile_sub, unknown, lpPath,
					"xWMA Files\0*.xwm\0Wave PCM\0*.wav\0All Files\0*.*\0", 
					lpCaption, lpFileName);
			}
		}
	}
}