// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <CKPE.Stream.h>
#include <CKPE.SmartPointer.h>
#include <CKPE.CriticalSection.h>

namespace CKPE
{
	class CKPE_API TZipObject
	{
	protected:
		bool _stream_init{ false };
		void* _handle{ nullptr };
		CriticalSection _section;
	public:
		TZipObject() noexcept(true) = default;
		virtual ~TZipObject() noexcept(true);

		template<typename T>
		[[nodiscard]] inline constexpr T* GetHandle() const noexcept(true) { return (T*)_handle; }

		std::int32_t LastError{ 0 };
		[[nodiscard]] virtual const char* LastErrorByString() const noexcept(true);
	};

	class CKPE_API Zipper;
	class CKPE_API UnZipper;
	class CKPE_API ZipFileEntry;
	class CKPE_API ZipFileEntries;

	class CKPE_API ZipFileScopeEntry
	{
		ZipFileEntry* _entry{ nullptr };

		ZipFileScopeEntry(const ZipFileScopeEntry&) = delete;
		ZipFileScopeEntry& operator=(const ZipFileScopeEntry&) = delete;
	public:
		ZipFileScopeEntry(ZipFileEntry* entry) noexcept(true);
		~ZipFileScopeEntry() noexcept(true);

		[[nodiscard]] inline ZipFileEntry* Get() const noexcept(true) { return _entry; }
	};

	class CKPE_API ZipFileEntry
	{
	public:
		constexpr static std::size_t InvalidIndex = (std::size_t)-1;
	private:
		const ZipFileEntries* _entries{ nullptr };
		std::size_t _idx{ InvalidIndex };

		ZipFileEntry(const ZipFileEntry&) = delete;
		ZipFileEntry& operator=(const ZipFileEntry&) = delete;
	public:
		friend class ZipFileScopeEntry;

		ZipFileEntry(const ZipFileEntries* entries, std::size_t idx) noexcept(true);
		virtual ~ZipFileEntry() noexcept(true) = default;

		[[nodiscard]] virtual bool IsDir() const noexcept(true);
		[[nodiscard]] virtual std::string GetName() const noexcept(true);
		[[nodiscard]] virtual std::size_t GetSize() const noexcept(true);
		[[nodiscard]] virtual std::size_t GetSizeInArchive() const noexcept(true);
		[[nodiscard]] virtual std::uint32_t GetCrc32() const noexcept(true);
		virtual bool ReadToStream(Stream& stream) const noexcept(true);
	protected:
		virtual void Close() const noexcept(true);
	};

	class CKPE_API ZipFileEntries
	{
		const TZipObject* _class_owner{ nullptr };
		std::vector<ZipFileEntry*>* _items{ nullptr };
		CriticalSection _section;

		ZipFileEntries(const ZipFileEntries&) = delete;
		ZipFileEntries& operator=(const ZipFileEntries&) = delete;
	public:
		ZipFileEntries(const TZipObject* class_owner) noexcept(true);
		virtual ~ZipFileEntries() noexcept(true);

		[[nodiscard]] virtual std::size_t Count() const noexcept(true);
		[[nodiscard]] virtual inline const TZipObject* Zip() const noexcept(true) { return _class_owner; }

		[[nodiscard]] virtual SmartPointer<ZipFileScopeEntry> At(std::size_t idx) const noexcept(true);
		
		virtual bool AddFile(const char* fname) noexcept(true);
		virtual bool AddFile(const wchar_t* fname) noexcept(true);
		virtual bool AddFile(const std::string& fname) noexcept(true);
		virtual bool AddFile(const std::wstring& fname) noexcept(true);

		virtual bool AddStream(const char* fname, MemoryStream& stm) noexcept(true);
		virtual bool AddStream(const wchar_t* fname, MemoryStream& stm) noexcept(true);
		virtual bool AddStream(const std::string& fname, MemoryStream& stm) noexcept(true);
		virtual bool AddStream(const std::wstring& fname, MemoryStream& stm) noexcept(true);
	};

	class CKPE_API UnZipper : public TZipObject
	{
		std::wstring* _fname{ nullptr };
		ZipFileEntries* _entries{ nullptr };
		MemoryStream _stream;

		UnZipper(const UnZipper&) = delete;
		UnZipper& operator=(const UnZipper&) = delete;
	protected:
		virtual bool OpenStream(const wchar_t* fname, MemoryStream& stm) noexcept(true);
	public:
		UnZipper() noexcept(true);
		UnZipper(const char* fname) noexcept(true);
		UnZipper(const wchar_t* fname) noexcept(true);
		UnZipper(const std::string& fname) noexcept(true);
		UnZipper(const std::wstring& fname) noexcept(true);
		virtual ~UnZipper() noexcept(true);

		[[nodiscard]] constexpr virtual inline bool HasOpen() const noexcept(true) { return _entries != nullptr; }

		virtual bool OpenFile(const char* fname) noexcept(true);
		virtual bool OpenFile(const wchar_t* fname) noexcept(true);
		virtual bool OpenFile(const std::string& fname) noexcept(true);
		virtual bool OpenFile(const std::wstring& fname) noexcept(true);

		virtual void Close() noexcept(true);

		[[nodiscard]] virtual ZipFileEntries* GetEntries() noexcept(true) { return _entries; }
		[[nodiscard]] virtual std::wstring GetFileName() const noexcept(true);
		
		[[nodiscard]] virtual std::size_t IndexOf(const char* fname) const noexcept(true);
		[[nodiscard]] virtual std::size_t IndexOf(const wchar_t* fname) const noexcept(true);
		[[nodiscard]] virtual std::size_t IndexOf(const std::string& fname) const noexcept(true);
		[[nodiscard]] virtual std::size_t IndexOf(const std::wstring& fname) const noexcept(true);

		virtual bool UnZipFile(const char* fname, const char* path) const noexcept(true);
		virtual bool UnZipFile(const wchar_t* fname, const wchar_t* path) const noexcept(true);
		virtual bool UnZipFile(const std::string& fname, const std::string& path) const noexcept(true);
		virtual bool UnZipFile(const std::wstring& fname, const std::wstring& path) const noexcept(true);

		static bool UnZipAllFiles(const char* fname, const char* path) noexcept(true);
		static bool UnZipAllFiles(const wchar_t* fname, const wchar_t* path) noexcept(true);
		static bool UnZipAllFiles(const std::string& fname, const std::string& path) noexcept(true);
		static bool UnZipAllFiles(const std::wstring& fname, const std::wstring& path) noexcept(true);
	};

	class CKPE_API Zipper : public TZipObject
	{
		std::wstring* _fname{ nullptr };
		ZipFileEntries* _entries{ nullptr };

		Zipper(const Zipper&) = delete;
		Zipper& operator=(const Zipper&) = delete;
	public:
		Zipper() noexcept(true);
		Zipper(const char* fname) noexcept(true);
		Zipper(const wchar_t* fname) noexcept(true);
		Zipper(const std::string& fname) noexcept(true);
		Zipper(const std::wstring& fname) noexcept(true);
		virtual ~Zipper() noexcept(true);

		[[nodiscard]] constexpr virtual inline bool HasOpen() const noexcept(true) { return _entries != nullptr; }
		
		virtual bool OpenFile(const char* fname) noexcept(true);
		virtual bool OpenFile(const wchar_t* fname) noexcept(true);
		virtual bool OpenFile(const std::string& fname) noexcept(true);
		virtual bool OpenFile(const std::wstring& fname) noexcept(true);

		virtual bool OpenStream() noexcept(true);

		virtual void Close() noexcept(true);
		
		[[nodiscard]] virtual ZipFileEntries* GetEntries() noexcept(true) { return _entries; }
		[[nodiscard]] virtual std::wstring GetFileName() const noexcept(true);
		
		virtual bool DeleteFile(const char* fname) noexcept(true);
		virtual bool DeleteFile(const wchar_t* fname) noexcept(true);
		virtual bool DeleteFile(const std::string& fname) noexcept(true);
		virtual bool DeleteFile(const std::wstring& fname) noexcept(true);

		virtual bool ZipFile(const char* fname) noexcept(true);
		virtual bool ZipFile(const wchar_t* fname) noexcept(true);
		virtual bool ZipFile(const std::string& fname) noexcept(true);
		virtual bool ZipFile(const std::wstring& fname) noexcept(true);

		virtual bool ZipFileAsStream(const char* fname, MemoryStream& stream) noexcept(true);
		virtual bool ZipFileAsStream(const wchar_t* fname, MemoryStream& stream) noexcept(true);
		virtual bool ZipFileAsStream(const std::string& fname, MemoryStream& stream) noexcept(true);
		virtual bool ZipFileAsStream(const std::wstring& fname, MemoryStream& stream) noexcept(true);
		
		static bool ZipFiles(const std::string& fname,
			const std::vector<std::string>& flist) noexcept(true);
		static bool ZipFiles(const std::wstring& fname,
			const std::vector<std::wstring>& flist) noexcept(true);
	};
}