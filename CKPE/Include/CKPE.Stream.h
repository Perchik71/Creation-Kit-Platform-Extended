// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <string>
#include <cstdint>
#include <cstdio>
#include <string_view>
#include <sstream>
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

		Stream() = default;
	};

	class CKPE_API FileStream : public Stream
	{
	public:
		enum FileOpen : std::uint32_t
		{
			fmOpenRead = 0,
			fmOpenReadWrite,
			fmCreate,
		};

		enum FileMode : std::uint32_t
		{
			fmBinary = 0,
			fmText,
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
		virtual void Flush() const noexcept(true);
		[[nodiscard]] virtual bool Eof() const noexcept(true);

		FileStream(const std::string& fname, FileOpen _open, FileMode _mode = FileMode::fmBinary);
		FileStream(const std::wstring& fname, FileOpen _open, FileMode _mode = FileMode::fmBinary);
		virtual ~FileStream() noexcept(true);
	};

	// Only Utf8 or Ansi codepages
	class CKPE_API TextFileStream : public FileStream
	{
	protected:
		void WriteLineStr(const std::string& string) const noexcept(true);
	public:
		virtual bool ReadLine(char* string, std::uint32_t maxsize) const noexcept(true);
		virtual void WriteString(const std::string_view& formatted_string, ...) const noexcept(true);
		virtual void WriteString(const std::wstring_view& formatted_string, ...) const noexcept(true);
		virtual void WriteString(const char* formatted_string, ...) const noexcept(true);
		virtual void WriteString(const wchar_t* formatted_string, ...) const noexcept(true);
		virtual void WriteLine(const std::string_view& formatted_string, ...) const noexcept(true);
		virtual void WriteLine(const std::wstring_view& formatted_string, ...) const noexcept(true);
		virtual void WriteLine(const char* formatted_string, ...) const noexcept(true);
		virtual void WriteLine(const wchar_t* formatted_string, ...) const noexcept(true);

		TextFileStream(const std::string& fname, FileOpen _open);
		TextFileStream(const std::wstring& fname, FileOpen _open);
	};

	class CKPE_API FileStream2 : public Stream
	{
	protected:
		void* _handle{ nullptr };
		std::wstring* _FileName{ nullptr };
	public:
		virtual std::uint32_t Read(void* buf, std::uint32_t size) const noexcept(true);
		virtual std::uint32_t Write(const void* buf, std::uint32_t size) noexcept(true);
		virtual std::uint64_t Offset(std::int64_t offset, OffsetStream flag = ofCurrent) noexcept(true);
		[[nodiscard]] virtual std::uint64_t GetSize() const noexcept(true);
		[[nodiscard]] virtual std::wstring GetFileName() const noexcept(true);
		virtual void Flush() const noexcept(true);

		FileStream2(const std::string& fname, FileStream::FileOpen _open, bool UseCache = true);
		FileStream2(const std::wstring& fname, FileStream::FileOpen _open, bool UseCache = true);
		virtual ~FileStream2() noexcept(true);
	};

	class CKPE_API MapFileStream : public Stream
	{
		void* _handle{ (void*)-1 };
		void* _handle_map{ nullptr };
		std::uint64_t _size{ 0 };
		std::uint64_t _caret{ 0 };
		std::wstring* _FileName{ nullptr };
	public:
		virtual std::uint32_t Read(void* buf, std::uint32_t size) const noexcept(true);
		virtual std::uint32_t Write(const void* buf, std::uint32_t size) noexcept(true);
		virtual std::uint64_t Offset(std::int64_t offset, OffsetStream flag = ofCurrent) noexcept(true);
		[[nodiscard]] virtual std::uint64_t GetSize() const noexcept(true);
		[[nodiscard]] virtual std::wstring GetFileName() const noexcept(true);
		[[nodiscard]] virtual bool Eof() const noexcept(true);

		MapFileStream(const std::string& fname, FileStream::FileOpen _open, bool UseCache = true);
		MapFileStream(const std::wstring& fname, FileStream::FileOpen _open, bool UseCache = true);
		virtual ~MapFileStream() noexcept(true);
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

		void Set(void* mem, std::uint64_t size) noexcept(true);
		void SetSize(std::uint64_t newsize);
		void Clear() noexcept(true);
		[[nodiscard]] constexpr bool Empty() const noexcept(true) { return _data == nullptr; }
		[[nodiscard]] constexpr std::uint8_t* Data() const noexcept(true) { return _data; }

		virtual bool LoadFromStream(Stream& stream) noexcept(true);
		virtual bool SaveToStream(Stream& stream) const noexcept(true);

		MemoryStream() noexcept(true) = default;
		MemoryStream(void* mem, std::uint64_t size) noexcept(true);
		MemoryStream(Stream& Stream) noexcept(true);
		MemoryStream(const MemoryStream& Stream) noexcept(true);
		virtual ~MemoryStream() noexcept(true);
	};

	class CKPE_API StringStream : public Stream, public FileStreamIntf
	{
	protected:
		char* _data{ nullptr };
		std::uint64_t _size{ 0 };
		std::uint64_t _caret{ 0 };

		bool Allocate(std::uint64_t newsize) noexcept(true);
		void Deallocate() noexcept(true);
	public:
		StringStream() noexcept(true) = default;
		StringStream(const char* str, std::size_t size) noexcept(true);
		StringStream(Stream& Stream) noexcept(true);
		StringStream(const StringStream& Stream) noexcept(true);
		virtual ~StringStream() noexcept(true);

		virtual std::uint32_t Read(void* buf, std::uint32_t size) const noexcept(true);
		virtual std::uint32_t Write(const void* buf, std::uint32_t size) noexcept(true);
		virtual std::uint32_t ReadString(char* buf, std::uint32_t size) const noexcept(true);
		virtual std::uint32_t WriteString(const char* buf, std::uint32_t size) noexcept(true);
		[[nodiscard]] std::uint64_t GetSize() const noexcept(true) override;
		std::uint64_t Offset(std::int64_t offset, OffsetStream flag = ofCurrent) noexcept(true) override;

		bool LoadFromStream(Stream& stream) noexcept(true) override;
		bool SaveToStream(Stream& stream) const noexcept(true) override;

		void Set(const char* str, std::size_t size) noexcept(true);
		void SetSize(std::size_t newsize);
		void Clear() noexcept(true);
		[[nodiscard]] constexpr bool Empty() const noexcept(true) { return _data == nullptr; }
		[[nodiscard]] constexpr const char* String() const noexcept(true) { return _data; }
		[[nodiscard]] constexpr std::string GetStdString() const noexcept(true) { return _data; }
	};

	/**
	 * @brief Overload for writing const char* literals/pointers into a StringStream.
	 */
	inline CKPE::StringStream& operator<<(StringStream& stream, const char* c_str) noexcept(true)
	{
		if (!c_str) return stream;
		stream.WriteString(c_str, static_cast<std::uint32_t>(std::strlen(c_str)));
		return stream;
	}

	/**
	 * @brief Template overload for writing primitives and formatted types T into a StringStream.
	 */
	template <typename T>
	inline CKPE::StringStream& operator<<(StringStream& stream, const T& value) noexcept(true)
	{
		// Use stringstream to safely format the type into a string representation.
		std::stringstream ss;
		ss << value;
		stream.WriteString(ss.str().c_str(), static_cast<std::uint32_t>(ss.str().length()));
		return stream;
	}
}