// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Common.h>
#include <string>
#include <vector>
#include <cstdint>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API AddressLibrary
		{
		public:
			using AddressID = std::uint64_t;
		private:
			struct Entry
			{
				std::uint64_t Id;
				std::uint64_t Offset;
			};

			std::vector<Entry>* _entries{ nullptr };
			bool _loaded{ false };

			AddressLibrary(const AddressLibrary&) = delete;
			AddressLibrary& operator=(const AddressLibrary&) = delete;
		public:
			AddressLibrary() noexcept(true);
			virtual ~AddressLibrary() noexcept(true);

			// Returns false (and logs the reason) on any missing/malformed/unsorted file - never throws
			virtual bool Load(const std::wstring& fname) noexcept(true);
			virtual void Clear() noexcept(true);

			[[nodiscard]] constexpr inline bool IsLoaded() const noexcept(true) { return _loaded; }
			[[nodiscard]] virtual std::uint32_t GetCount() const noexcept(true);

			// Raw RVA for the given stable id, or 0 if unknown / not loaded.
			[[nodiscard]] virtual std::uint64_t ResolveOffset(AddressID id) const noexcept(true);
			// Absolute address (CreationKit.exe module base + RVA), or 0 if unknown / not loaded.
			[[nodiscard]] virtual std::uintptr_t Resolve(AddressID id) const noexcept(true);

			static AddressLibrary* GetSingleton() noexcept(true);
		};
	}
}
