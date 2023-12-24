// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashCCCFiles.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			Array<String> g_CCEslNames;

			CrashCCCFilesPatch::CrashCCCFilesPatch() : Module(GlobalEnginePtr)
			{}

			bool CrashCCCFilesPatch::HasOption() const
			{
				return false;
			}

			bool CrashCCCFilesPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashCCCFilesPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CrashCCCFilesPatch::GetName() const
			{
				return "Crash CCC Files";
			}

			bool CrashCCCFilesPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> CrashCCCFilesPatch::GetDependencies() const
			{
				return {};
			}

			bool CrashCCCFilesPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				// Update 1.5.73 automatically parses the Skyrim.CCC file.
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_5_3;
			}

			bool CrashCCCFilesPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					//
					// Fix crash when loading new CC ESLs as master files. 
					//
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&GetCCFileCount);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(1), (uintptr_t)&GetCCFile);
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(2), (uintptr_t)&IsCCFile);

					return true;
				}

				return false;
			}

			bool CrashCCCFilesPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			void CrashCCCFilesPatch::ParseCreationClubContentFile()
			{
				static int unused = []
				{
					if (FILE* f; fopen_s(&f, "Skyrim.ccc", "rt") == 0)
					{
						char name[MAX_PATH];

						while (fgets(name, ARRAYSIZE(name), f))
						{
							if (strchr(name, '\n'))
								*strchr(name, '\n') = '\0';

							g_CCEslNames.push_back(name);
						}

						fclose(f);
					}

					return 0;
				}();
			}

			uint32_t CrashCCCFilesPatch::GetCCFileCount()
			{
				ParseCreationClubContentFile();

				return (uint32_t)g_CCEslNames.size();
			}

			const char* CrashCCCFilesPatch::GetCCFile(uint32_t Index)
			{
				ParseCreationClubContentFile();

				if (Index < g_CCEslNames.size())
					return g_CCEslNames[Index].c_str();

				return nullptr;
			}

			bool CrashCCCFilesPatch::IsCCFile(const char* Name)
			{
				ParseCreationClubContentFile();

				if (!Name || strlen(Name) <= 0)
					return false;

				for (String& s : g_CCEslNames)
				{
					if (!_stricmp(s.c_str(), Name))
						return true;
				}

				return false;
			}
		}
	}
}