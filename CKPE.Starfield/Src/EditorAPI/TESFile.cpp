// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.Stream.h>
#include <EditorAPI/TESFile.h>
#include <CKPE.MessageBox.h>
#include <CKPE.Common.Interface.h>

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			std::uintptr_t pointer_TESFile_sub1 = 0;
			std::uintptr_t pointer_TESFile_sub2 = 0;
			std::uintptr_t pointer_TESFile_sub3 = 0;
			std::uintptr_t pointer_TESFile_data = 0;
			std::uint32_t conversion_TESFile_data = 0;

			std::int32_t TESFile::hk_LoadTESInfo() noexcept(true)
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

			std::int64_t TESFile::hk_WriteTESInfo() noexcept(true)
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

			void TESFile::hk_ConversionPlugin(void* __This, std::uint32_t Type) noexcept(true)
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
				fast_call<void>(pointer_TESFile_sub3, __This, Type);
				// reset conversion flag
				conversion_TESFile_data = 0;
			}

			bool TESFile::IsMasterFileToBlacklist() const noexcept(true)
			{
				if (IsMaster()) 
				{
					auto str = FileName;

					if (!_stricmp(str, "starfield.esm") ||
						!_stricmp(str, "constellation.esm") ||
						!_stricmp(str, "oldmars.esm") ||
						!_stricmp(str, "sfbgs003.esm") ||
						!_stricmp(str, "sfbgs004.esm") ||
						!_stricmp(str, "sfbgs006.esm") ||
						!_stricmp(str, "sfbgs007.esm") ||
						!_stricmp(str, "sfbgs008.esm") ||
						!_stricmp(str, "sfbgs00a_a.esm") ||
						!_stricmp(str, "sfbgs00e.esm") ||
						!_stricmp(str, "sfbgs021.esm") ||
						!_stricmp(str, "sfbgs023.esm") ||
						!_stricmp(str, "sfta01.esm") ||
						!_stricmp(str, "blueprintships - starfield.esm"))
					{
						MessageBox::OpenWarning("Base game master files cannot be set as the active file.");
						return true;
					}

					return false;
				}

				return false;
			}

			bool TESFile::ReadFirstChunk(const char* fileName, TESChunk& chunk) noexcept(true)
			{
				ZeroMemory(&chunk, sizeof(TESChunk));

				try
				{
					FileStream fstm(fileName, FileStream::fmOpenRead);
					return fstm.Read(&chunk, sizeof(TESChunk)) == sizeof(TESChunk);
				}
				catch (const std::exception& e)
				{
					_ERROR("TESFile: %s", e.what());
					return false;
				}
			}

			std::uint32_t TESFile::GetTypeFile(const char* fileName) noexcept(true)
			{
				TESChunk chunk;
				if (ReadFirstChunk(fileName, chunk))
					return chunk.flags;
				return 0;
			}

			static SYSTEMTIME FileTimeToSystemTimeEx(const FILETIME& Time) noexcept(true)
			{
				SYSTEMTIME T;
				FILETIME Local_File_Time;

				FileTimeToLocalFileTime(&Time, &Local_File_Time);
				FileTimeToSystemTime(&Local_File_Time, &T);

				return T;
			}

			SYSTEMTIME TESFile::GetCreationTime() const noexcept(true)
			{
				return FileTimeToSystemTimeEx(m_findData.ftCreationTime);
			}

			SYSTEMTIME TESFile::GetLastWriteTime() const noexcept(true)
			{
				return FileTimeToSystemTimeEx(m_findData.ftLastWriteTime);
			}

			const char* TESFile::GetAuthorName() const noexcept(true)
			{
				if (IsMaster())
					return (const char*)pointer_TESFile_data;

				return m_authorName.c_str();
			}

			const char* TESFile::GetDescription() const noexcept(true)
			{
				return m_description.c_str();
			}
		}
	}
}