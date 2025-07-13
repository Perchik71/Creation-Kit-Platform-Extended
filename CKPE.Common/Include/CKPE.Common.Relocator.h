// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Common.h>
#include <CKPE.Common.RelocatorDB.h>
#include <string>
#include <cstdint>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API Relocator
		{
			RelocatorDB* _db{ nullptr };

			Relocator(const Relocator&) = delete;
			Relocator& operator=(const Relocator&) = delete;
		public:
			Relocator() noexcept(true);
			virtual ~Relocator() noexcept(true);

			virtual RelocatorDB* GetDB() noexcept(true);

			virtual bool Open(const std::wstring& fname_pak, const std::wstring& fname_db) noexcept(true);
			virtual bool Save(const std::wstring& fname_pak, const std::wstring& fname_db) noexcept(true);

			virtual const RelocatorDB::PatchDB* GetAtConst(std::uint32_t id) const noexcept(true);
			virtual const RelocatorDB::PatchDB* GetByNameConst(const std::string& name) const noexcept(true);
			virtual RelocatorDB::PatchDB* GetAt(std::uint32_t id) noexcept(true);
			virtual RelocatorDB::PatchDB* GetByName(const std::string& name) noexcept(true);
			virtual bool Add(RelocatorDB::PatchDB* patch) noexcept(true);
			virtual bool Delete(const std::string& name) noexcept(true);
			virtual bool DeleteByIndex(const std::uint32_t id) noexcept(true);
			virtual void Clear() noexcept(true);
			virtual std::uint32_t GetCount() const noexcept(true);

			static Relocator* GetSingleton() noexcept(true);
		};
	}
}