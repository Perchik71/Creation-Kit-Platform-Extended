// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "BNetConvertUnicodeString.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			BNetConvertUnicodeStringPatch::BNetConvertUnicodeStringPatch() : Module(GlobalEnginePtr)
			{}

			bool BNetConvertUnicodeStringPatch::HasOption() const
			{
				return false;
			}

			bool BNetConvertUnicodeStringPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* BNetConvertUnicodeStringPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* BNetConvertUnicodeStringPatch::GetName() const
			{
				return "BNet Convert Unicode String";
			}

			bool BNetConvertUnicodeStringPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> BNetConvertUnicodeStringPatch::GetDependencies() const
			{
				return {};
			}

			bool BNetConvertUnicodeStringPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_LAST;
			}

			bool BNetConvertUnicodeStringPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix crash when Unicode string conversion fails with bethesda.net http responses
					//
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&BNetConvertUnicodeString);

					return true;
				}

				return false;
			}

			bool BNetConvertUnicodeStringPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			size_t BNetConvertUnicodeStringPatch::BNetConvertUnicodeString(char* Destination, size_t DestSize,
				const wchar_t* Source, size_t SourceSize)
			{
				if (Destination)
					memset(Destination, 0, DestSize);

				size_t outLen;
				errno_t result = wcsrtombs_s(&outLen, Destination, DestSize, &Source, SourceSize, nullptr);

				if (result != 0)
				{
					// Try a fall back instead of normally returning -1
					outLen = WideCharToMultiByte(CP_UTF8, 0, Source, (int)SourceSize, Destination, 
						(int)DestSize, nullptr, nullptr);

					if (Destination && DestSize > 0)
						Destination[DestSize - 1] = '\0';

					AssertMsg(outLen != 0, "Unicode conversion failed");
				}

				return outLen - 1;
			}
		}
	}
}