// Copyright © 2023-2024 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <memory>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.PatchManager.h>
#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.PathUtils.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Exception.h>

namespace CKPE
{
	namespace Common
	{
		static PatchManager GlobalPatchManager;

		std::int32_t PatchManager::ActivePatchSafe(Entry& entry) const
		{
			__try
			{
				if (!entry.patch->Active(entry.db))
					return -1;

				return 0;
			}
			__except (1)
			{
				return -2;
			}
		}

		std::int32_t PatchManager::QueryPatchSafe(Entry& entry) const
		{
			__try
			{
				if (!entry.patch->Query())
					return -1;

				return 0;
			}
			__except (1)
			{
				return -2;
			}
		}

		bool PatchManager::ActivePatch(Entry& entry, const std::string& game_short) noexcept(true)
		{
			auto gsettings = Interface::GetSingleton()->GetSettings();

			if (!entry.patch)
				return false;

			if (entry.patch->IsActive())
				return true;

			if (entry.patch->GetMethods() == Patch::Method::kUseAddressLibraryOrDatabased)
			{
				if (!entry.db && !(entry.patch->SupportsAddressLibrary() && AddressLibrary::GetSingleton()->IsLoaded()))
				{
					_WARNING_EX("The \"{}\" patch can't be installed, there is no data in the database"sv,
						entry.patch->GetName());
					return false;
				}
			}

			if (entry.patch->HasOption())
			{
				auto option_name = entry.patch->GetOptionName();
				if (!option_name)
				{
					_ERROR_EX("The \"{}\" patch is a requirement for an option, but the option itself is specified as nullptr, skips"sv,
						entry.patch->GetName());
					return false;
				}

				std::string section;
				std::string name;
				if (!gsettings->SplitOptionName(option_name, section, name) || !section.length() || !name.length())
				{
					_ERROR_EX("The \"{}\" patch couldn't identify the section and the name of the option, skips"sv,
						entry.patch->GetName());
					return false;
				}

				if (gsettings->GetOptionTypeByName(name) != SettingOptionType::sotBool)
				{
					_ERROR_EX("The \"{}\" patch only logical option names are allowed, skips"sv,
						entry.patch->GetName());
					return false;
				}

				if (!gsettings->ReadBool(section, name, false))
				{
					_MESSAGE_EX("[{}]\tThe \"{}\" patch can't be installed, it is disabled by the option"sv,
						game_short, entry.patch->GetName());
					return false;
				}
			}

			if (entry.patch->HasDependencies())
			{
				auto depends = entry.patch->GetDependencies();
				if (!depends.size())
					_WARNING_EX("The \"{}\" patch says that there are dependencies that for some reason don't exist"sv,
						entry.patch->GetName());
				else
				{
					for (auto& depend : depends)
					{
						auto it = std::find_if(_entries->begin(), _entries->end(), [&depend](Entry& entry)
							{
								return !_stricmp(entry.patch->GetName().c_str(), depend.c_str());
							});

						if (it == _entries->end())
						{
							_ERROR_EX("The \"{}\" patch has a dependency \"{}\" that is not in the database or is not registered, skips"sv,
								entry.patch->GetName(), depend);
							return false;
						}
						
						if (it->patch->IsActive())
							continue;

						if (!ActivePatch(*it, game_short))
						{
							_ERROR_EX("The \"{}\" patch has a dependency \"{}\" that has not been initialized, skips"sv,
								entry.patch->GetName(), depend);
							return false;
						}
					}
				}
			}

			switch (ActivePatchSafe(entry))
			{
			case 0:
				_MESSAGE_EX("[{}]\tThe \"{}\" patch has been initialized"sv, game_short, entry.patch->GetName());
				return true;
			case -1:
				_FATALERROR_EX("The \"{}\" patch has not been fully installed, there may be errors"sv, entry.patch->GetName());
				break;
			case -2:
				_FATALERROR_EX("An internal error occurred while installing the \"{}\" patch"sv, entry.patch->GetName());
				break;
			}

			return false;
		}

		PatchManager::PatchManager() noexcept(true) :
			_entries(new std::vector<Entry>), _blacklist(new std::vector<std::string>)
		{}

		PatchManager::~PatchManager() noexcept(true)
		{
			Clear();

			if (_entries)
			{
				delete _entries;
				_entries = nullptr;
			}

			if (_blacklist)
			{
				delete _blacklist;
				_blacklist = nullptr;
			}
		}

		void PatchManager::Register(Patch* patch) noexcept(true)
		{
			ScopeCriticalSection lock(_locker);

			if (!patch)
			{
				_ERROR("PatchManager::Register patch is nullptr"sv);
				return;
			}
			
			auto name = patch->GetName();
			if (!name.length())
			{
				_ERROR("PatchManager::Register patch haven't name"sv);
				return;
			}

			for (auto& s : *_blacklist)
			{
				if (!_stricmp(s.c_str(), name.c_str()))
				{
					_WARNING_EX("PatchManager::Register \"%s\" is blacklisted"sv, name);
					return;
				}
			}

			auto entry_exist = std::find_if(_entries->begin(), _entries->end(), [&name](const Entry& e) {
				return !_stricmp(e.patch->GetName().c_str(), name.c_str());
				});

			if (entry_exist != _entries->end())
			{
				_ERROR_EX("PatchManager::Register found this patch with same name \"{}\" in db class \"{}\" and \"{}\""sv,
					name, typeid(patch).name(), typeid(entry_exist->patch).name());
				return;
			}

			auto db = Relocator::GetSingleton()->GetByName(name);
			bool viaAddressLibrary = patch->SupportsAddressLibrary() && AddressLibrary::GetSingleton()->IsLoaded();
			if ((patch->GetMethods() == Patch::Method::kUseAddressLibraryOrDatabased) && (!db && !viaAddressLibrary))
			{
				_ERROR_EX("PatchManager::Register no found this patch name \"{}\" in db"sv, name);
				return;
			}

			// db may be nullptr here 
			// only for a patch that opted in via SupportsAddressLibrary() 
			_entries->push_back({ db, patch });
		}

		void PatchManager::Clear() noexcept(true)
		{
			if (!_entries)
				return;

			ScopeCriticalSection lock(_locker);

			for (auto& entry : *_entries)
			{
				if (entry.db)
				{
					delete entry.db;
					entry.db = nullptr;
				}

				if (entry.patch)
				{
					delete entry.patch;
					entry.patch = nullptr;
				}
			}

			_entries->clear();
		}

		std::uint32_t PatchManager::GetCount() noexcept(true)
		{
			return _entries ? (std::uint32_t)_entries->size() : 0ul;
		}

		std::vector<PatchManager::Entry>* PatchManager::GetEntries() noexcept(true)
		{
			return _entries;
		}

		void PatchManager::ActiveAll(const std::wstring& game_short) noexcept(true)
		{
			if (!_entries)
				return;

			ScopeCriticalSection lock(_locker);
			auto gshort = StringUtils::Utf16ToUtf8(game_short);	

			for (auto& entry : *_entries)
				ActivePatch(entry, gshort);
		}

		void PatchManager::QueryAll(const std::wstring& game_short) noexcept(true)
		{
			if (!_entries)
				return;

			ScopeCriticalSection lock(_locker);
			std::vector<std::vector<Entry>::iterator> skips;
			auto gshort = StringUtils::Utf16ToUtf8(game_short);

			for (auto it = _entries->begin(); it != _entries->end(); it++)
			{
				if (!it->patch)
				{
					skips.push_back(it);
					continue;
				}

				if (it->patch->IsActive())
					continue;

				auto result = QueryPatchSafe(*it);
				switch (result)
				{
				case -1:
					_WARNING_EX("[{}]\tThe \"{}\" patch can't be installed for this version of the editor"sv,
						gshort, it->patch->GetName());
					break;
				case -2:
					_ERROR_EX("[{}]\tAn internal error occurred while checking the \"{}\" patch"sv,
						gshort, it->patch->GetName());
					break;
				}

				if (result)
					skips.push_back(it);
			}

			while(!skips.empty())
			{
				auto& it = skips[skips.size() - 1];
				if (it->patch)
					delete it->patch;

				_entries->erase(it);
				skips.pop_back();
			}
		}

		void PatchManager::OpenBlackList() noexcept(true)
		{
			constexpr auto fname = "CreationKitPlatformExtendedFilter.txt";
			if (PathUtils::FileExists(fname))
			{
				try
				{
					TextFileStream fstm(fname, FileStream::fmOpenRead);

					auto line = std::make_unique<char[]>(260);
					if (!line)
						throw RuntimeError("Out of memory");

					while (!fstm.Eof())
					{
						fstm.ReadLine(line.get(), 260);
						std::string str_line = StringUtils::Trim(line.get());
						bool append = true;

						for (auto& s : *_blacklist)
						{
							if (!_stricmp(s.c_str(), str_line.c_str()))
							{
								_WARNING_EX("PatchManager::OpenBlackList \"{}\" duplicate"sv, str_line);
								append = false;
								break;
							}
						}

						if (append)
							_blacklist->push_back(str_line);
					}
				}
				catch (const std::exception& e)
				{
					_ERROR("PatchManager::OpenBlackList %s"sv, e.what());
				}
			}
		}

		PatchManager* PatchManager::GetSingleton() noexcept(true)
		{
			return &GlobalPatchManager;
		}
	}
}