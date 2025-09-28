// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Stream.h>
#include <EditorAPI/TESFile.h>
#include <CKPE.MessageBox.h>
#include <CKPE.Common.Interface.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			std::uintptr_t pointer_TESFile_sub1 = 0;
			std::uintptr_t pointer_TESFile_sub2 = 0;

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
						_CONSOLE("Loading master file '%s' as a plugin\n", m_FileName);

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
						_CONSOLE("Loading plugin file '%s' as a master\n", m_FileName);
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
							_CONSOLE("Regenerating ONAM data for master file '%s'...\n", m_FileName);

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

			bool TESFile::IsActiveFileBlacklist(bool showDialog) const noexcept(true)
			{
				if (IsMaster())
				{
					if (!_stricmp(m_FileName, "fallout4.esm") ||
						!_stricmp(m_FileName, "dlcrobot.esm") ||
						!_stricmp(m_FileName, "dlcnukaworld.esm") ||
						!_stricmp(m_FileName, "dlccoast.esm") ||
						!_stricmp(m_FileName, "dlcworkshop01.esm") ||
						!_stricmp(m_FileName, "dlcworkshop02.esm") ||
						!_stricmp(m_FileName, "dlcworkshop03.esm"))
					{
						if (showDialog)
							MessageBox::OpenWarning("Base game master files cannot be set as the active file.");
						return true;
					}
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

			SYSTEMTIME TESFile::GetCreateTime() const noexcept(true)
			{
				return FileTimeToSystemTimeEx(m_findData.CreateTime);
			}

			SYSTEMTIME TESFile::GetModifyTime() const noexcept(true)
			{
				return FileTimeToSystemTimeEx(m_findData.ModifyTime);
			}
		}
	}
}