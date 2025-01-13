// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "TESFileSF.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			uintptr_t pointer_TESFile_sub1 = 0;
			uintptr_t pointer_TESFile_sub2 = 0;
			uintptr_t pointer_TESFile_sub3 = 0;
			uintptr_t pointer_TESFile_data = 0;
			uint32_t conversion_TESFile_data = 0;

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
						_CONSOLE("Loading master file '%s' as a plugin\n", m_FileName);

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
						_CONSOLE("Loading plugin file '%s' as a master\n", m_FileName);
						m_Flags |= FILE_RECORD_ESM;
					}
				}

				return 0;
			}

			int64_t TESFile::hk_WriteTESInfo()
			{
				if (AllowSaveESM)
				{
					bool resetEsmFlag = false;

					if (IsActive())
					{
						if (conversion_TESFile_data)
						{
							// The conversion takes place in 3 stages: saving the plugin,
							// changing the header and saving, returning the header and saving.
							// Plugin always with ext as .esp.

							if (IsMaster())
							{
								_CONSOLE("Conversion plugin: 0x%X", conversion_TESFile_data);

								// forced remove esm flag
								m_Flags &= ~FILE_RECORD_ESM;

								_CONSOLE("Regenerating ONAM data for master file '%s'...\n", m_FileName);
								((void(__fastcall*)(TESFile*))pointer_TESFile_sub2)(this);

								// returned esm flag and type
								m_Flags |= FILE_RECORD_ESM;
								m_Flags |= conversion_TESFile_data;
							}
						}
						else
						{
							LPCSTR extension = strrchr(m_FileName, '.');
							if (extension && !_stricmp(extension, ".esm"))
							{
								// forced remove esm flag
								m_Flags &= ~FILE_RECORD_ESM;

								_CONSOLE("Regenerating ONAM data for master file '%s'...\n", m_FileName);
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
				else
					return WriteTESInfo(this);
			}

			void TESFile::hk_ConversionPlugin(void* __This, uint32_t Type)
			{
				switch (Type)
				{
				case 1:
					conversion_TESFile_data = FILE_RECORD_LIGHT;
					break;
				case 2:
					conversion_TESFile_data = FILE_RECORD_MID;
					break;
				case 3:
					conversion_TESFile_data = FILE_RECORD_ESM;
					break;
				default:
					conversion_TESFile_data = 0;
					break;
				}

				// call native function
				Core::fastCall<void>(pointer_TESFile_sub3, __This, Type);
				// reset conversion flag
				conversion_TESFile_data = 0;
			}

			bool TESFile::IsMasterFileToBlacklist() {
				if (IsMaster()) {
					auto str = FileName;

					if (!str.Compare("starfield.esm") ||
						!str.Compare("constellation.esm") ||
						!str.Compare("oldmars.esm") ||
						!str.Compare("sfbgs003.esm") ||
						!str.Compare("sfbgs004.esm") ||
						!str.Compare("sfbgs006.esm") ||
						!str.Compare("sfbgs007.esm") ||
						!str.Compare("sfbgs008.esm") ||
						!str.Compare("sfbgs00a_a.esm") ||
						!str.Compare("sfbgs00e.esm") ||
						!str.Compare("sfbgs021.esm") ||
						!str.Compare("sfbgs023.esm") ||
						!str.Compare("sfta01.esm") ||
						!str.Compare("blueprintships - starfield.esm"))
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

			BSString TESFile::GetAuthorName() const
			{
				if (IsMaster())
					return (const char*)pointer_TESFile_data;

				return m_authorName.IsEmpty() ? m_authorName.c_str() : "";
			}

			BSString TESFile::GetDescription() const
			{
				return m_description.IsEmpty() ? m_description.c_str() : "";
			}
		}
	}
}