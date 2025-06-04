// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <string>
#include <cstdint>
#include <cstdio>
#include <string>
#include <CKPE.CriticalSection.h>

namespace CKPE
{
	class CKPE_API Stream
	{
	protected:
		CriticalSection _section;
	public:
		enum OffsetStream : std::uint32_t
		{
			ofBegin = 0,
			ofCurrent,
			ofEnd,
		};

		virtual std::uint32_t Read(void* buf, std::uint32_t size) const noexcept(true) = 0;
		virtual std::uint32_t Write(const void* buf, std::uint32_t size) noexcept(true) = 0;
		[[nodiscard]] virtual std::uint64_t GetSize() const noexcept(true) = 0;
		virtual std::uint64_t Offset(std::int64_t offset, OffsetStream flag = ofCurrent) noexcept(true) = 0;
	
		inline void Assign(Stream& stream) noexcept(true) { CopyFrom(stream); }
		std::uint64_t CopyFrom(Stream& stream, std::uint64_t count = 0) noexcept(true);

		[[nodiscard]] inline std::uint64_t GetPosition() noexcept(true) { return Offset(0, ofCurrent); }
		inline std::uint64_t SetPosition(std::uint64_t pos) noexcept(true) { return Offset(pos, ofBegin); }
	public:
		Stream() = default;
	};

	class CKPE_API FileStream : public Stream
	{
	public:
		enum FileMode : std::uint32_t
		{
			fmOpenRead = 0,
			fmOpenReadWrite,
			fmCreate,
		};
	protected:
		std::FILE* _Handle{ nullptr };
		std::wstring* _FileName{ nullptr };
	public:
		virtual std::uint32_t Read(void* buf, std::uint32_t size) const noexcept(true);
		virtual std::uint32_t Write(const void* buf, std::uint32_t size) noexcept(true);
		virtual std::uint64_t Offset(std::int64_t offset, OffsetStream flag = ofCurrent) noexcept(true);
		[[nodiscard]] virtual std::uint64_t GetSize() const noexcept(true);
		[[nodiscard]] virtual std::wstring GetFileName() const noexcept(true);
		[[nodiscard]] virtual inline std::FILE* GetHandle() const noexcept(true) { return _Handle; }
		virtual void Flush() const noexcept(true);
	public:
		FileStream(const std::string& fname, FileMode _mode);
		FileStream(const std::wstring& fname, FileMode _mode);
		virtual ~FileStream() noexcept(true);
	};

	class CKPE_API FileStreamIntf
	{
	public:
		virtual bool LoadFromStream(Stream& stream) noexcept(true) = 0;
		bool LoadFromFile(const std::string& fname) noexcept(true);
		bool LoadFromFile(const std::wstring& fname) noexcept(true);
		virtual bool SaveToStream(Stream& stream) const noexcept(true) = 0;
		bool SaveToFile(const std::string& fname) const noexcept(true);
		bool SaveToFile(const std::wstring& fname) const noexcept(true);
	};

	class CKPE_API MemoryStream : public Stream, public FileStreamIntf
	{
	protected:
		std::uint8_t* _data{ nullptr };
		std::uint64_t _size{ 0 };
		std::uint64_t _caret{ 0 };
	protected:
		bool Allocate(std::uint64_t newsize) noexcept(true);
		void Deallocate() noexcept(true);
	public:
		virtual std::uint32_t Read(void* buf, std::uint32_t size) const noexcept(true);
		virtual std::uint32_t Write(const void* buf, std::uint32_t size) noexcept(true);
		[[nodiscard]] virtual std::uint64_t GetSize() const noexcept(true);
		virtual std::uint64_t Offset(std::int64_t offset, OffsetStream flag = ofCurrent) noexcept(true);
	public:
		void Set(void* mem, std::uint64_t size) noexcept(true);
		void SetSize(std::uint64_t newsize);
		void Clear() noexcept(true);
		[[nodiscard]] constexpr inline bool Empty() const noexcept(true) { return _data == nullptr; }
		[[nodiscard]] constexpr inline std::uint8_t* Data() const noexcept(true) { return _data; }
	public:
		virtual bool LoadFromStream(Stream& stream) noexcept(true);
		virtual bool SaveToStream(Stream& stream) const noexcept(true);
	public:
		MemoryStream() noexcept(true) = default;
		MemoryStream(void* mem, std::uint64_t size) noexcept(true);
		MemoryStream(Stream& Stream) noexcept(true);
		MemoryStream(const MemoryStream& Stream) noexcept(true);
		virtual ~MemoryStream() noexcept(true);
	};
}