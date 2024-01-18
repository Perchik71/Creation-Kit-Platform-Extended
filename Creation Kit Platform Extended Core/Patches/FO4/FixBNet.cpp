// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "FixBNet.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			FixBNetPatch::FixBNetPatch() : Module(GlobalEnginePtr)
			{}

			bool FixBNetPatch::HasOption() const
			{
				return false;
			}

			bool FixBNetPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixBNetPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixBNetPatch::GetName() const
			{
				return "Fix crash when Unicode string BNet";
			}

			bool FixBNetPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixBNetPatch::GetDependencies() const
			{
				return {};
			}

			bool FixBNetPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool FixBNetPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Fix crash when Unicode string conversion fails with bethesda.net http responses
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);

					return true;
				}

				return false;
			}

			bool FixBNetPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			size_t FixBNetPatch::sub(char* Destination, size_t DestSize, const wchar_t* Source, size_t SourceSize)
			{
				if (Destination)
					memset(Destination, 0, DestSize);

				size_t outLen;
				errno_t result = wcsrtombs_s(&outLen, Destination, DestSize, &Source, SourceSize, nullptr);

				if (result != 0)
				{
					// Try a fall back instead of normally returning -1
					outLen = WideCharToMultiByte(CP_UTF8, 0, Source, (int)SourceSize, Destination, (int)DestSize, nullptr, nullptr);

					if (Destination && DestSize > 0)
						Destination[DestSize - 1] = '\0';

					AssertMsg(outLen != 0, "Unicode conversion failed");
				}

				return outLen - 1;
			}
		}
	}
}