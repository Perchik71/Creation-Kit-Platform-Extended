// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Common.h>
#include <CKPE.Stream.h>
#include <string>
#include <cstdint>
#include <ctime>
#include <vector>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API TableID
		{
		public:
			struct CKPE_COMMON_API TableIDHeader
			{
				std::uint32_t fourcc;
				std::uint32_t version;
				std::uint32_t crc32;
				char game[16];
				std::uint64_t game_version;
				std::uint32_t count;
				time_t create_time;
				std::uint32_t reserved0;
				std::uint32_t reserved1;
			};
		private:
			std::vector<std::uint32_t>* _table{ nullptr };
			TableIDHeader _header{ 0 };
			std::uintptr_t _base{ 0 };

			TableID(const TableID&) = delete;
			TableID& operator=(const TableID&) = delete;
		public:
			TableID(std::uintptr_t base = 0) noexcept(true);
			virtual ~TableID() noexcept(true);

			virtual void Rebase(std::uintptr_t base = 0) noexcept(true);
			virtual bool Open(const std::string& fname) noexcept(true);
			virtual bool Open(const std::wstring& fname) noexcept(true);
			virtual bool Open(Stream& stm) noexcept(true);
			virtual void Clear() noexcept(true);

			[[nodiscard]] constexpr virtual std::uint32_t Crc32() const noexcept(true) { return _header.crc32; }
			[[nodiscard]] constexpr virtual std::uint32_t Count() const noexcept(true) { return _header.count; }
			[[nodiscard]] constexpr virtual std::string Game() const noexcept(true) { return _header.game; }
			[[nodiscard]] constexpr virtual std::uint64_t GameVersion() const noexcept(true) { return _header.game_version; }
			[[nodiscard]] constexpr virtual time_t CreateTime() const noexcept(true) { return _header.create_time; }

			virtual std::uint32_t Rva(std::uint32_t id) const noexcept(true);
			virtual std::uintptr_t Offset(std::uint32_t id) const noexcept(true);

			static TableID* GetSingleton() noexcept(true);
		};
	}
}