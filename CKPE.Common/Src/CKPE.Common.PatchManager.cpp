// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <memory>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.PatchManager.h>
#include <CKPE.PathUtils.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Exception.h>

namespace CKPE
{
	namespace Common
	{
		static PatchManager GlobalPatchManager;

		std::int32_t PatchManager::ActivePatchSafe(Entry& entry)
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

		std::int32_t PatchManager::QueryPatchSafe(Entry& entry)
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

			if (!entry.db)
			{
				_WARNING("The \"%s\" patch can't be installed, there is no data in the database",
					entry.patch->GetName().c_str());
				return false;
			}

			if (entry.patch->HasOption())
			{
				auto option_name = entry.patch->GetOptionName();
				if (!option_name)
				{
					_ERROR("The \"%s\" patch is a requirement for an option, but the option itself is specified as nullptr, skips",
						entry.patch->GetName().c_str());
					return false;
				}

				std::string section;
				std::string name;
				if (!gsettings->SplitOptionName(option_name, section, name) || !section.length() || !name.length())
				{
					_ERROR("The \"%s\" patch couldn't identify the section and the name of the option, skips",
						entry.patch->GetName().c_str());
					return false;
				}

				if (gsettings->GetOptionTypeByName(name) != SettingOptionType::sotBool)
				{
					_ERROR("The \"%s\" patch only logical option names are allowed, skips",
						entry.patch->GetName().c_str());
					return false;
				}

				if (!gsettings->ReadBool(section, name, false))
				{
					_MESSAGE("[%s]\tThe \"%s\" patch can't be installed, it is disabled by the option",
						game_short.c_str(), entry.patch->GetName().c_str());
					return false;
				}
			}

			if (entry.patch->HasDependencies())
			{
				auto depends = entry.patch->GetDependencies();
				if (!depends.size())
					_WARNING("The \"%s\" patch says that there are dependencies that for some reason don't exist",
						entry.patch->GetName().c_str());
				else
				{
					for (auto& depend : depends)
					{
						auto it = std::find_if(_entries->begin(), _entries->end(), [&depend](Entry& entry) -> bool
							{
								return !_stricmp(entry.patch->GetName().c_str(), depend.c_str());
							});

						if (it == _entries->end())
						{
							_ERROR("The \"%s\" patch has a dependency \"%s\" that is not in the database or is not registered, skips",
								entry.patch->GetName().c_str(), depend.c_str());
							return false;
						}
						
						if (it->patch->IsActive())
							continue;

						if (!ActivePatch(*it, game_short))
						{
							_ERROR("The \"%s\" patch has a dependency \"%s\" that has not been initialized, skips",
								entry.patch->GetName().c_str(), depend.c_str());
							return false;
						}
					}
				}
			}

			switch (ActivePatchSafe(entry))
			{
			case 0:
				_MESSAGE("[%s]\tThe \"%s\" patch has been initialized",
					game_short.c_str(), entry.patch->GetName().c_str());
				return true;
			case -1:
				_FATALERROR("The \"%s\" patch has not been fully installed, there may be errors",
					entry.patch->GetName().c_str());
				break;
			case -2:
				_FATALERROR("An internal error occurred while installing the \"%s\" patch",
					entry.patch->GetName().c_str());
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
				_ERROR("PatchManager::Register patch is nullptr");
				return;
			}
			
			auto name = patch->GetName();
			if (!name.length())
			{
				_ERROR("PatchManager::Register patch haven't name");
				return;
			}

			for (auto& s : *_blacklist)
			{
				if (!_stricmp(s.c_str(), name.c_str()))
				{
					_WARNING("PatchManager::Register \"%s\" is blacklisted", name.c_str());
					return;
				}
			}

			auto db = Relocator::GetSingleton()->GetByName(name);
			if (!db)
			{
				_ERROR("PatchManager::Register no found this patch name \"%s\" in db", name.c_str());
				return;
			}

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
					_WARNING("[%s]\tThe \"%s\" patch can't be installed for this version of the editor",
						gshort.c_str(), it->patch->GetName().c_str());
					break;
				case -2:
					_ERROR("[%s]\tAn internal error occurred while checking the \"%s\" patch",
						gshort.c_str(), it->patch->GetName().c_str());
					break;
				}

				if (result)
					skips.push_back(it);
			}

			for (auto& it : skips)
			{
				if (it->patch)
					delete it->patch;

				_entries->erase(it);
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
								_WARNING("PatchManager::OpenBlackList \"%s\" duplicate", str_line.c_str());
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
					_ERROR("PatchManager::OpenBlackList %s", e.what());
				}
			}
		}

		PatchManager* PatchManager::GetSingleton() noexcept(true)
		{
			return &GlobalPatchManager;
		}
	}
}