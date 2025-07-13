// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.Interface.h>
#include <CKPE.Common.PatchManager.h>
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

		PatchManager::PatchManager() noexcept(true) :
			_entries(new std::vector<Entry>)
		{}

		PatchManager::~PatchManager() noexcept(true)
		{
			Clear();

			if (_entries)
			{
				delete _entries;
				_entries = nullptr;
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

			auto db = Relocator::GetSingleton()->GetByName(name);
			if (!db)
			{
				_ERROR("PatchManager::Register no found this patch name in db");
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

		void PatchManager::ActiveAll(const std::wstring& game_short) noexcept(true)
		{
			if (!_entries)
				return;

			ScopeCriticalSection lock(_locker);
			auto gshort = StringUtils::Utf16ToUtf8(game_short);

			for (auto& entry : *_entries)
			{
				if (!entry.patch || entry.patch->IsActive())
					continue;

				if (!entry.db)
				{
					_WARNING("[%s]\tThe \"%s\" patch can't be installed, there is no data in the database", 
						gshort.c_str(), entry.patch->GetName().c_str());
					continue;
				}

				switch (ActivePatchSafe(entry))
				{
				
				case -1:
					_FATALERROR("[%s]\tThe \"%s\" patch has not been fully installed, there may be errors", 
						gshort.c_str(), entry.patch->GetName().c_str());
					break;
				case -2:
					_FATALERROR("[%s]\tAn internal error occurred while installing the \"%s\" patch",
						gshort.c_str(), entry.patch->GetName().c_str());
					break;
				}
			}
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

		PatchManager* PatchManager::GetSingleton() noexcept(true)
		{
			return &GlobalPatchManager;
		}
	}
}