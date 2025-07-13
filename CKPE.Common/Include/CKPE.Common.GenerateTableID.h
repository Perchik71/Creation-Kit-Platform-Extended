// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.TableID.h>
#include <CKPE.CriticalSection.h>
#include <CKPE.Stream.h>
#include <CKPE.Module.h>
#include <string>
#include <cstdint>
#include <ctime>
#include <vector>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API GenerateTableID : public FileStreamIntf
		{
		public:
			struct CKPE_COMMON_API Entry
			{
				std::uint32_t rva;
				std::uint32_t real_size;
				char mask[240];
				std::uint32_t mask_len;
			};
		private:
			CriticalSection _locker;
			Module* _mbase{ nullptr };
			std::vector<Entry>* _entries{ nullptr };
			std::uint64_t _version_file{ 0 };
			std::uint32_t _game{ 0 };

			void Analize() noexcept(true);

			GenerateTableID(const GenerateTableID&) = delete;
			GenerateTableID& operator=(const GenerateTableID&) = delete;
		public:
			GenerateTableID() noexcept(true);
			GenerateTableID(const void* base) noexcept(true);
			virtual ~GenerateTableID() noexcept(true);

			virtual void Rebase(const void* base = nullptr) noexcept(true);
			virtual void Merging(GenerateTableID& tableid) noexcept(true);
			virtual bool LoadFromStream(Stream& stm) noexcept(true);
			virtual bool SaveToStream(Stream& stm) const noexcept(true);
			virtual void Close();

			virtual void Dump(const std::string& fname) const noexcept(true);
			virtual void Dump(const std::wstring& fname) const noexcept(true);
		};
	}
}