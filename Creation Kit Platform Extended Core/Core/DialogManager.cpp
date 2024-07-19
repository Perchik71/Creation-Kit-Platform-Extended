// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "DialogManager.h"
#include "Editor API/BSString.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		using namespace CreationKitPlatformExtended::EditorAPI;

		DialogManager* GlobalDialogManagerPtr = nullptr;

		bool DialogManager::HasDialog(const LONG_PTR uid) const 
		{ 
			return m_items.find(uid) != m_items.end(); 
		}

		bool DialogManager::AddDialog(const String& json_file, const ULONG_PTR uid)
		{
			if (HasDialog(uid))
				return false;

			jDialog* dialog = new jDialog();
			if (!dialog)
				return false;

			if (!dialog->LoadFromFile(json_file.c_str()))
			{
				delete dialog;
				return false;
			}

			m_items.insert(std::pair(uid, dialog));
			return true;
		}

		bool DialogManager::AddDialogByCode(const String& json_code, const ULONG_PTR uid)
		{
			if (HasDialog(uid))
				return false;

			jDialog* dialog = new jDialog();
			if (!dialog)
				return false;

			if (!dialog->ParseJSON(json_code.c_str()))
			{
				delete dialog;
				return false;
			}

			m_items.insert(std::pair(uid, dialog));
			return true;
		}

		jDialog* DialogManager::GetDialog(const ULONG_PTR uid)
		{
			auto it = m_items.find(uid);
			return (it == m_items.end()) ? NULL : it->second;
		}

		bool DialogManager::Empty() const
		{ 
			return !m_items.size(); 
		}

		void DialogManager::LoadFromFilePackage(const char* fname)
		{
			// I didn't understand it, but it crashes when calling json::parser().
			// I will make the load via a file.

			CHAR szBuf[MAX_PATH] = { 0 };
			Assert(GetTempPathA(MAX_PATH, szBuf));
			String path_temp = szBuf;

			// open archive
			struct zip_t* zip = zip_open(fname, 0, 'r');
			if (!zip)
			{
				_FATALERROR("DIALOG: Failed open archive \"%s\"", fname);
				return;
			}

			_MESSAGE("DIALOG: Open archive \"%s\"", fname);

			String sName, sId, extract_fname;

			INT nCount = zip_entries_total(zip);
			for (INT i = 0; i < nCount; ++i)
			{
				zip_entry_openbyindex(zip, i);
				
				// skip dirs
				if (zip_entry_isdir(zip) != 0)
				{
					zip_entry_close(zip);
					continue;
				}

				sName = zip_entry_name(zip);

				// only .json 
				if (BSString::Utils::ExtractFileExt(sName.c_str()) != ".json")
					continue;

				auto AtuID = sName.find_first_of('.');
				if (AtuID == String::npos)
					continue;

				sId = sName.substr(0, AtuID);
				extract_fname = path_temp + sId + ".json";
				if (zip_entry_fread(zip, extract_fname.c_str()))
					_FATALERROR("DIALOG: Failed read file \"%s\"", sName.c_str());
				else
				{
					if (AddDialog(extract_fname, strtoul(sId.data(), NULL, 10)))
						_MESSAGE("The dialog has been added: \"%s\"", sName.c_str());
					else
						_ERROR("Error adding a dialog: \"%s\"", sName.c_str());
				}

				DeleteFileA(extract_fname.c_str());
				zip_entry_close(zip);
			}

			zip_close(zip);
		}

		void DialogManager::PackToFilePackage(const char* fname, const char* dir)
		{
			if (!dir || !fname)
				return;

			if (!BSString::Utils::DirectoryExists(dir))
			{
				_ERROR("DIALOG: The specified directory \"%s\" does not exist", dir);
				return;
			}

			struct zip_t* zip = zip_open(fname, ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
			if (!zip)
			{
				_FATALERROR("DIALOG: Failed open archive \"%s\"", fname);
				return;
			}

			WIN32_FIND_DATA FindFileData;
			HANDLE hFind;
			String path = dir;

			if (hFind = FindFirstFileA((path + "\\*.json").c_str(), &FindFileData); hFind != INVALID_HANDLE_VALUE)
			{
				String sName;

				do
				{
					if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
						continue;

					// skip hidden files (I prefer to see the files)
					if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN)
						continue;

					// skip encrypted files (Encrypted file, problematic file)
					if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED) == FILE_ATTRIBUTE_ENCRYPTED)
						continue;

					// skip offline files
					if ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE) == FILE_ATTRIBUTE_OFFLINE)
						continue;

					zip_entry_open(zip, FindFileData.cFileName);
					zip_entry_fwrite(zip, (path + FindFileData.cFileName).c_str());
					zip_entry_close(zip);
				} while (FindNextFileA(hFind, &FindFileData));

				FindClose(hFind);
			}

			zip_close(zip);

			_MESSAGE("DIALOG: New archive created: \"%s\"", fname);
		}
	}
}