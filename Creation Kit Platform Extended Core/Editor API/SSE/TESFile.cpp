//////////////////////////////////////////
/*
* Copyright (c) 2020 Nukem9 <email:Nukem@outlook.com>
* Copyright (c) 2022-2023 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#include "TESFile.h"
#include "Patches/ConsolePatch.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			using namespace Patches;

			uintptr_t pointer_TESFile_sub1 = 0;
			uintptr_t pointer_TESFile_sub2 = 0;
			uintptr_t pointer_TESFile_sub3 = 0;

			int TESFile::hk_LoadTESInfo()
			{
				int error = LoadTESInfo(this);

				if (error != 0)
					return error;

				const bool masterFile = (m_RecordFlags & FILE_RECORD_ESM) == FILE_RECORD_ESM;
				const bool activeFile = (m_RecordFlags & FILE_RECORD_ACTIVE) == FILE_RECORD_ACTIVE;

				// If it's an ESM being loaded as the active file, force it to act like a normal ESP
				if (AllowSaveESM)
				{
					if (masterFile && activeFile)
					{
						ConsolePatch::Log("Loading master file '%s' as a plugin\n", m_FileName);

						// Strip ESM flag, clear loaded ONAM data
						m_RecordFlags &= ~FILE_RECORD_ESM;
						((void(__fastcall*)(TESFile*))pointer_TESFile_sub1)(this);
					}
				}

				// If loading ESP files as masters, flag everything except for the currently active plugin
				if (AllowMasterESP)
				{
					if (!masterFile && !activeFile && (m_RecordFlags & FILE_RECORD_CHECKED) == FILE_RECORD_CHECKED)
					{
						ConsolePatch::Log("Loading plugin file '%s' as a master\n", m_FileName);
						m_RecordFlags |= FILE_RECORD_ESM;
					}
				}

				return 0;
			}

			__int64 TESFile::hk_WriteTESInfo()
			{
				bool resetEsmFlag = false;

				if (AllowSaveESM)
				{
					if ((m_RecordFlags & FILE_RECORD_ACTIVE) == FILE_RECORD_ACTIVE)
					{
						const char* extension = strrchr(m_FileName, '.');

						// forced remove esm flag
						m_RecordFlags &= ~FILE_RECORD_ESM;

						if (extension && !_stricmp(extension, ".esm"))
						{
							ConsolePatch::Log("Regenerating ONAM data for master file '%s'...\n", m_FileName);

							((void(__fastcall*)(TESFile*))pointer_TESFile_sub2)(this);
							resetEsmFlag = true;
						}
					}
				}

				auto form = WriteTESInfo(this);

				if (resetEsmFlag)
					m_RecordFlags &= ~FILE_RECORD_ESM;

				return form;
			}

			bool TESFile::IsActiveFileBlacklist()
			{
				if ((m_RecordFlags & FILE_RECORD_ESM) == FILE_RECORD_ESM)
				{
					if (!_stricmp(m_FileName, "Skyrim.esm") ||
						!_stricmp(m_FileName, "Update.esm") ||
						!_stricmp(m_FileName, "Dawnguard.esm") ||
						!_stricmp(m_FileName, "HearthFires.esm") ||
						!_stricmp(m_FileName, "Dragonborn.esm"))
					{
						MessageBoxA(GetForegroundWindow(), 
							"Base game master files cannot be set as the active file.",
							"Warning", MB_ICONWARNING);
						return true;
					}
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

			uint32_t TESFile::GetIndexLoader() const 
			{
				return ((uint32_t(__fastcall*)(const TESFile*))pointer_TESFile_sub3)(this);
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
		}
	}
}