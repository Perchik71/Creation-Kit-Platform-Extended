// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <string>
#include <CKPE.Stream.h>

namespace CKPE
{
	class CKPE_API Zipper
	{
		std::int32_t _lasterr{ 0 };
		struct zip_t* _zip{ nullptr };

		Zipper(const Zipper&) = delete;
		Zipper& operator=(const Zipper&) = delete;
	public:
		class CKPE_API Entry
		{
			const Zipper* _zipper{ nullptr };
			std::uint32_t _id{ 0 };

			Entry(const Entry&) = delete;
			Entry& operator=(const Entry&) = delete;

			bool OpenEntryById(const Zipper* zipper, std::uint32_t id) noexcept(true);
			void CloseEntry() noexcept(true);
		public:
			Entry(const Zipper* zipper, std::uint32_t id);
			virtual ~Entry() noexcept(true);

			virtual bool IsDir() const noexcept(true);
			virtual bool GetName(std::string& name) const noexcept(true);
			virtual bool GetSize(std::uint32_t& size) const noexcept(true);
			virtual bool GetSizeInArchive(std::uint32_t& size) const noexcept(true);
			virtual bool GetCrc32(std::uint32_t& hash) const noexcept(true);
			virtual bool Read(Stream& stream) const noexcept(true);
		};

		Zipper() noexcept(true) = default;
		Zipper(const std::string& fname);
		Zipper(const std::wstring& fname);
		virtual ~Zipper() noexcept(true);

		virtual bool Create(const std::string& fname) noexcept(true);
		virtual bool Create(const std::wstring& fname) noexcept(true);
		virtual bool Open(const std::string& fname, bool write_access = false) noexcept(true);
		virtual bool Open(const std::wstring& fname, bool write_access = false) noexcept(true);
		virtual void Close() noexcept(true);

		virtual std::uint32_t Count() const;
		
		virtual bool PackFromFile(const std::string& fname) noexcept(true);
		virtual bool PackFromFile(const std::wstring& fname) noexcept(true);
		virtual bool PackFromStream(const std::string& fname, MemoryStream& stream) noexcept(true);
		virtual bool PackFromStream(const std::wstring& fname, MemoryStream& stream) noexcept(true);
		
		constexpr virtual std::int32_t LastError() const noexcept(true) { return _lasterr; }
		virtual std::string LastErrorAsString() const noexcept(true);
	};
}