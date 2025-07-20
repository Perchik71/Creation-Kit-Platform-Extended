// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Common.h>
#include <CKPE.Common.Patch.h>
#include <CKPE.Common.Relocator.h>
#include <CKPE.CriticalSection.h>
#include <vector>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API PatchManager
		{
			struct CKPE_COMMON_API Entry
			{
				RelocatorDB::PatchDB* db;
				Patch* patch;
			};

			std::vector<Entry>* _entries{ nullptr };
			CriticalSection _locker;

			[[nodiscard]] std::int32_t ActivePatchSafe(Entry& entry);
			[[nodiscard]] std::int32_t QueryPatchSafe(Entry& entry);
			bool ActivePatch(Entry& entry, const std::string& game_short) noexcept(true);

			PatchManager(const PatchManager&) = delete;
			PatchManager& operator=(const PatchManager&) = delete;
		public:
			PatchManager() noexcept(true);
			virtual ~PatchManager() noexcept(true);

			virtual void Register(Patch* patch) noexcept(true);
			virtual void Clear() noexcept(true);
			virtual std::uint32_t GetCount() noexcept(true);

			virtual void ActiveAll(const std::wstring& game_short) noexcept(true);
			virtual void QueryAll(const std::wstring& game_short) noexcept(true);

			static PatchManager* GetSingleton() noexcept(true);
		};
	}
}