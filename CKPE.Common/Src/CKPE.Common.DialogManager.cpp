// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <vector>
#include <CKPE.Common.DialogManager.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Exception.h>
#include <CKPE.Zipper.h>
#include <CKPE.StringUtils.h>
#include <CKPE.PathUtils.h>

namespace CKPE
{
	namespace Common
	{
		static DialogManager* sDialogManager = nullptr;

		bool DialogManager::UnSafeAddDialog(const std::string& json_file, const std::uintptr_t uid) noexcept(true)
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

			m_items->insert(std::pair(uid, dialog));
			return true;
		}

		bool DialogManager::UnSafeAddDialogByCode(const std::string& json_code, const std::uintptr_t uid) noexcept(true)
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

			m_items->insert(std::pair(uid, dialog));
			return true;
		}

		DialogManager::DialogManager(const std::string& fname) noexcept(true)
		{
			m_items = new DialogList;
			LoadFromFilePackage(fname);
		}

		DialogManager* DialogManager::GetSingleton() noexcept(true)
		{
			return sDialogManager;
		}

		bool DialogManager::Initialize(const std::string& fname) noexcept(true)
		{
			try
			{
				sDialogManager = new DialogManager(fname);
				return true;
			}
			catch (const std::exception&)
			{
				return false;
			}
		}

		bool DialogManager::Initialize(const std::wstring& fname) noexcept(true)
		{
			return Initialize(StringUtils::Utf16ToWinCP(fname));
		}

		bool DialogManager::HasDialog(const std::uintptr_t uid) const noexcept(true)
		{ 
			return m_items->find(uid) != m_items->end(); 
		}

		bool DialogManager::AddDialog(const std::string& json_file, const std::uintptr_t uid) noexcept(true)
		{
			__try
			{
				return UnSafeAddDialog(json_file, uid);
			}
			__except (1)
			{
				return false;
			}
		}

		bool DialogManager::AddDialogByCode(const std::string& json_code, const std::uintptr_t uid) noexcept(true)
		{
			__try
			{
				return UnSafeAddDialogByCode(json_code, uid);
			}
			__except (1)
			{
				return false;
			}
		}

		jDialog* DialogManager::GetDialog(const std::uintptr_t uid) noexcept(true)
		{
			auto it = m_items->find(uid);
			return (it == m_items->end()) ? nullptr : it->second;
		}

		bool DialogManager::Empty() const noexcept(true)
		{ 
			return !m_items || !m_items->size();
		}

		void DialogManager::LoadFromFilePackage(const std::string& fname)
		{
			try
			{
				std::string sName, sId;
				UnZipper zipper(fname);

				if (!zipper.HasOpen())
					throw RuntimeError("DIALOG: Can't opened file \"{}\"", fname);

				_MESSAGE("DIALOG: Open archive \"%s\"", fname.c_str());

				for (std::size_t i = 0; i < zipper.GetEntries()->Count(); ++i)
				{
					auto entry = zipper.GetEntries()->At(i);
					if (entry.Empty() || !entry->Get())
						throw std::runtime_error(zipper.LastErrorByString());

					if (entry->Get()->IsDir())
						continue;

					sName = entry->Get()->GetName();
					// only .json 
					if (_stricmp(PathUtils::ExtractFileExt(sName).c_str(), ".json"))
						continue;

					auto AtuID = sName.find_first_of('.');
					if (AtuID == std::string::npos)
						continue;

					sId = sName.substr(0, AtuID);

					MemoryStream stream;
					if (!entry->Get()->ReadToStream(stream))
						throw RuntimeError("DIALOG: Failed read file \"{}\" {}", sName, zipper.LastErrorByString());
					else
					{
						if (AddDialogByCode((const char*)stream.Data(), strtoul(sId.data(), nullptr, 10)))
							_MESSAGE("DIALOG: The dialog has been added: \"%s\"", sName.c_str());
						else
							throw RuntimeError("Error adding a dialog: \"{}\"", sName);
					}
				}
			}
			catch (const std::exception& e)
			{
				_ERROR(e.what());
			}
		}

		void DialogManager::PackToFilePackage(const std::string& fname, const std::string& dir)
		{
			if (dir.empty() || fname.empty())
				return;

			try
			{
				if (!PathUtils::DirExists(dir))
					throw RuntimeError("DIALOG: The specified directory \"{}\" does not exist", dir);
				
				std::vector<std::string> flist;

				WIN32_FIND_DATA FindFileData;
				HANDLE hFind;
				std::string path = dir;


				if (hFind = FindFirstFileA((path + "\\*.json").c_str(), &FindFileData); hFind != INVALID_HANDLE_VALUE)
				{
					std::string sName;

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

						flist.push_back(path + FindFileData.cFileName);
					} while (FindNextFileA(hFind, &FindFileData));

					FindClose(hFind);

					if (flist.size())
						Zipper::ZipFiles(fname, flist);
				}

				_MESSAGE("DIALOG: New archive created: \"%s\"", fname);
			}
			catch (const std::exception& e)
			{
				_ERROR(e.what());
			}
		}
	}
}