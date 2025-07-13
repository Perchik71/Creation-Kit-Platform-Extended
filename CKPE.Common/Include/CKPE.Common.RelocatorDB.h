// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Common.h>
#include <CKPE.Stream.h>
#include <CKPE.CriticalSection.h>
#include <string>
#include <cstdint>
#include <map>
#include <vector>

namespace CKPE
{
	namespace Common
	{
		class CKPE_COMMON_API RelocatorDB : public FileStreamIntf
		{
		public:
			class CKPE_COMMON_API PatchDB : public FileStreamIntf
			{
			public:
				struct CKPE_COMMON_API EntryDB
				{
					uint32_t Rva;
					std::string* Mask;
				};
			private:
				std::uint32_t _version{ 0 };
				std::string* _name{ nullptr };
				std::vector<EntryDB>* _entries{ nullptr };
				CriticalSection _locker;

				PatchDB(const PatchDB&) = delete;
				PatchDB& operator=(const PatchDB&) = delete;

				virtual std::int32_t OpenStream(Stream& stream) noexcept(true);
				virtual std::int32_t SaveStream(Stream& stream) const noexcept(true);
				virtual std::int32_t OpenDevStream(TextFileStream& stream) noexcept(true);
				virtual std::int32_t SaveDevStream(TextFileStream& stream, bool regen_sign) const noexcept(true);
			public:
				PatchDB() noexcept(true);
				virtual ~PatchDB() noexcept(true);

				virtual bool LoadFromStream(Stream& stream) noexcept(true);
				virtual bool SaveToStream(Stream& stream) const noexcept(true);

				virtual bool LoadDevFromFile(const std::string& fname) noexcept(true);
				virtual bool LoadDevFromFile(const std::wstring& fname) noexcept(true);
				virtual bool SaveDevToFile(const std::string& fname, bool regen_sign = false) const noexcept(true);
				virtual bool SaveDevToFile(const std::wstring& fname, bool regen_sign = false) const noexcept(true);

				virtual std::string GetName() const noexcept(true);
				virtual std::uint32_t GetVersion() const noexcept(true);
				virtual EntryDB GetAt(std::uint32_t id) const noexcept(true);
				virtual void Append(const EntryDB& name) noexcept(true);
				virtual void Insert(std::uint32_t id, const EntryDB& name) noexcept(true);
				virtual void Clear() noexcept(true);

				virtual std::uint32_t GetCount() const noexcept(true);
			};
		private:
			std::map<std::string, PatchDB*>* _db{ nullptr };
			CriticalSection _locker;

			RelocatorDB(const RelocatorDB&) = delete;
			RelocatorDB& operator=(const RelocatorDB&) = delete;

			virtual std::int32_t OpenStream(Stream& stream) noexcept(true);
			virtual std::int32_t SaveStream(Stream& stream) const noexcept(true);
		public:
			RelocatorDB() noexcept(true);
			virtual ~RelocatorDB() noexcept(true);

			virtual bool LoadFromStream(Stream& stream) noexcept(true);
			virtual bool SaveToStream(Stream& stream) const noexcept(true);

			virtual PatchDB* At(const std::string& name) noexcept(true);
			virtual PatchDB* AtByIndex(const std::uint32_t id) noexcept(true);

			virtual bool Add(PatchDB* patch) noexcept(true);
			virtual bool Delete(const std::string& name) noexcept(true);
			virtual bool DeleteByIndex(const std::uint32_t id) noexcept(true);
			virtual std::uint32_t GetCount() const noexcept(true);

			virtual void Clear() noexcept(true);
		};
	}
}

#define __CKPE_RVA(idx) (db->GetAt(idx).Rva)
#define __CKPE_OFFSET(idx) (base + db->GetAt(idx).Rva)