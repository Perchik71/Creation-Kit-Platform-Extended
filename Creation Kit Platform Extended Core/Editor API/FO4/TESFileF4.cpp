// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Patches/ConsolePatch.h"
#include "TESFileF4.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			uintptr_t pointer_TESFile_sub1 = 0;
			uintptr_t pointer_TESFile_sub2 = 0;
			uintptr_t pointer_TESFile_data = 0;

			int32_t TESFile::hk_LoadTESInfo()
			{
				int error = LoadTESInfo(this);

				if (error != 0)
					return error;

				// If it's an ESM being loaded as the active file, force it to act like a normal ESP
				if (AllowSaveESM)
				{
					if (IsMaster() && IsActive())
					{
						Patches::ConsolePatch::Log("Loading master file '%s' as a plugin\n", m_FileName);

						// Strip ESM flag, clear loaded ONAM data
						m_Flags &= ~FILE_RECORD_ESM;
						((void(__fastcall*)(TESFile*))pointer_TESFile_sub1)(this);
					}
				}

				// If loading ESP files as masters, flag everything except for the currently active plugin
				if (AllowMasterESP)
				{
					if (!IsMaster() && !IsActive() && IsSelected())
					{
						Patches::ConsolePatch::Log("Loading plugin file '%s' as a master\n", m_FileName);
						m_Flags |= FILE_RECORD_ESM;
					}
				}

				return 0;
			}

			int64_t TESFile::hk_WriteTESInfo()
			{
				bool resetEsmFlag = false;

				if (AllowSaveESM)
				{
					if (IsActive())
					{
						LPCSTR extension = strrchr(m_FileName, '.');

						// forced remove esm flag
						m_Flags &= ~FILE_RECORD_ESM;

						if (extension && !_stricmp(extension, ".esm"))
						{
							Patches::ConsolePatch::Log("Regenerating ONAM data for master file '%s'...\n", m_FileName);

							((void(__fastcall*)(TESFile*))pointer_TESFile_sub2)(this);
							resetEsmFlag = true;
						}
					}
				}

				auto form = WriteTESInfo(this);

				if (resetEsmFlag)
					m_Flags &= ~FILE_RECORD_ESM;

				return form;
			}

			bool TESFile::IsMasterFileToBlacklist() {
				if (IsMaster()) {
					auto str = FileName;

					if (!str.Compare("fallout4.esm") ||
						!str.Compare("dlcrobot.esm") ||
						!str.Compare("dlcnukaworld.esm") ||
						!str.Compare("dlccoast.esm") ||
						!str.Compare("dlcworkshop01.esm") ||
						!str.Compare("dlcworkshop02.esm") ||
						!str.Compare("dlcworkshop03.esm")) 
					{
						MessageBoxA(GetForegroundWindow(), 
							"Base game master files cannot be set as the active file.", 
							"Warning", 
							MB_OK | MB_ICONWARNING);

						return true;
					}

					return false;
				}

				return false;
			}

			bool TESFile::ReadFirstChunk(const char* fileName, TESChunk& chunk)
			{
				auto fileStream = _fsopen(fileName, "rb", _SH_DENYWR);
				if (!fileStream) return false;

				Utils::ScopeFileStream file(fileStream);
				Utils::FileReadBuffer(fileStream, &chunk);

				return true;
			}

			uint32_t TESFile::GetTypeFile(const char* fileName)
			{
				TESChunk chunk;
				if (ReadFirstChunk(fileName, chunk))
					return chunk.flags;
				return 0;
			}

			BSString TESFile::GetAuthorName() const 
			{
				if (IsMaster())
					return (LPCSTR)pointer_TESFile_data;

				return *m_authorName ? m_authorName : "";
			}

			SYSTEMTIME FileTimeToSystemTimeEx(const FILETIME& Time) 
			{
				SYSTEMTIME T;
				FILETIME Local_File_Time;

				FileTimeToLocalFileTime(&Time, &Local_File_Time);
				FileTimeToSystemTime(&Local_File_Time, &T);

				return T;
			}

			SYSTEMTIME TESFile::GetCreationTime() const 
			{
				return FileTimeToSystemTimeEx(m_findData.ftCreationTime);
			}

			SYSTEMTIME TESFile::GetLastWriteTime() const 
			{
				return FileTimeToSystemTimeEx(m_findData.ftLastWriteTime);
			}

			void TESFile::Dump() 
			{
				_CONSOLE("Address: %p", this);
				_CONSOLE("FileName: %s", *FileName);
				_CONSOLE("FilePath: %s", *FilePath);
				_CONSOLE("AuthorName: %s", *AuthorName);
				_CONSOLE("Description: %s", *Description);
				_CONSOLE("FileSize: %u", FileSize);

				_CONSOLE(" ");
				_CONSOLE("DependCount: %u", DependCount);
				uint32_t count = DependCount;
				for (uint32_t i = 0; i < count; i++) {
					auto file = DependArray[i];

					_CONSOLE("Address: %p", file);
					_CONSOLE("FileName: %s", *(file->FileName));
					_CONSOLE("FilePath: %s", *(file->FilePath));
					_CONSOLE("AuthorName: %s", *(file->AuthorName));
					_CONSOLE("Description: %s", *(file->Description));
					_CONSOLE("FileSize: %u", file->FileSize);
				}
			}
		}
	}
}