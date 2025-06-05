// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Stream.h>
#include <CKPE.PathUtils.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Exception.h>
#include <cstdarg>
#include <memory>

namespace CKPE
{
	std::uint64_t Stream::CopyFrom(Stream& stream, std::uint64_t count) noexcept(true)
	{
		ScopeCriticalSection guard{ _section };

		constexpr static std::uint32_t BUF_SIZE = 1024 * 64;

		std::uint64_t dw64Pos = 0;
		std::uint64_t dw64Size = count;
		std::uint32_t dwBuf = BUF_SIZE;

		if (!count)
		{
			dw64Size = stream.GetSize();
			stream.SetPosition(0);
		}

		auto szBuf = std::make_unique<unsigned char[]>(BUF_SIZE);

		do {
			dwBuf = (dw64Size - dw64Pos) > BUF_SIZE ? BUF_SIZE : (std::uint32_t)(dw64Size - dw64Pos);
			dw64Pos += stream.Read(szBuf.get(), dwBuf);

			Write(szBuf.get(), dwBuf);
		} while (dw64Pos < dw64Size);

		return dw64Pos;
	}

	std::uint32_t FileStream::Read(void* buf, std::uint32_t size) const noexcept(true)
	{
		ScopeCriticalSection guard{ _section };
		return (std::uint32_t)fread(buf, 1, size, _Handle);
	}

	std::uint32_t FileStream::Write(const void* buf, std::uint32_t size) noexcept(true)
	{
		ScopeCriticalSection guard{ _section };
		return (std::uint32_t)fwrite(buf, 1, size, _Handle);
	}

	std::uint64_t FileStream::Offset(std::int64_t offset, OffsetStream flag) noexcept(true)
	{
		ScopeCriticalSection guard{ _section };
		_fseeki64(_Handle, offset, flag);
		return _ftelli64(_Handle);
	}

	std::uint64_t FileStream::GetSize() const noexcept(true)
	{
		ScopeCriticalSection guard{ _section };
		auto _This = const_cast<FileStream*>(this);
		auto safep = _This->Offset(0);
		auto size = _This->Offset(0, Stream::ofEnd);
		_This->Offset(safep, Stream::ofBegin);
		return size;
	}

	std::wstring FileStream::GetFileName() const noexcept(true)
	{
		return _FileName ? *_FileName : L"";
	}

	void FileStream::Flush() const noexcept(true)
	{
		if (_Handle)
			fflush(_Handle);
	}

	bool FileStream::Eof() const noexcept(true)
	{
		return _Handle ? feof(_Handle) : false;
	}

	FileStream::FileStream(const std::string& fname, FileOpen _open, FileMode _mode) :
		FileStream(StringUtils::Utf8ToUtf16(fname), _open, _mode)
	{}

	FileStream::FileStream(const std::wstring& fname, FileOpen _open, FileMode _mode) : Stream()
	{
		int right = _SH_DENYNO;
		std::wstring m;
		
		switch (_open)
		{
		case CKPE::FileStream::fmOpenRead:
			m = L"r";
			m += _mode == FileMode::fmBinary ? L"b" : L"t";
			right = _SH_DENYWR;
			break;
		case CKPE::FileStream::fmOpenReadWrite:
			m = L"r";
			m += _mode == FileMode::fmBinary ? L"b" : L"t";
			m += L"+";
			right = _SH_DENYRW;
			break;
		default:
			m = L"w";
			m += _mode == FileMode::fmBinary ? L"b" : L"t";
			m += L"+";
			right = _SH_DENYRW;
			break;
		}

		_Handle = _wfsopen(fname.c_str(), m.c_str(), right);
		if (!_Handle)
			throw SystemError(errno, "FileStream \"{}\"", StringUtils::Utf16ToWinCP(fname));

		_FileName = new std::wstring(fname);
	}

	FileStream::~FileStream() noexcept(true)
	{
		if (_Handle)
		{
			fflush(_Handle);
			fclose(_Handle);
			_Handle = nullptr;
		}

		if (_FileName)
		{
			delete _FileName;
			_FileName = nullptr;
		}
	}

	void TextFileStream::WriteLineStr(const std::string& string) const noexcept(true)
	{
		if (!_Handle || string.empty())
			return;

		fputs(string.c_str(), _Handle);
		fputc('\n', _Handle);
	}

	bool TextFileStream::ReadLine(std::string& string, std::uint32_t maxsize) const noexcept(true)
	{
		if (!_Handle || !maxsize)
			return false;

		fgets(string.data(), (int)maxsize, _Handle);
		return true;
	}

	void TextFileStream::WriteLine(const std::string_view& formatted_string, ...) const noexcept(true)
	{
		va_list ap;
		va_start(ap, &formatted_string);
		auto len = _vscprintf(formatted_string.data(), (va_list)ap);
		if (len < 1) return;

		std::string string_done;
		string_done.resize((std::size_t)len);
		if (string_done.empty()) return;
		vsprintf(string_done.data(), formatted_string.data(), (va_list)ap);
		va_end(ap);

		WriteLineStr(string_done);
	}

	void TextFileStream::WriteLine(const std::wstring_view& formatted_string, ...) const noexcept(true)
	{
		va_list ap;
		va_start(ap, &formatted_string);
		auto len = _vscwprintf(formatted_string.data(), (va_list)ap);
		if (len < 1) return;

		std::wstring string_done;
		string_done.resize((std::size_t)len);
		if (string_done.empty()) return;
		_vswprintf(string_done.data(), formatted_string.data(), (va_list)ap);
		va_end(ap);

		WriteLineStr(StringUtils::Utf16ToUtf8(string_done));
	}

	TextFileStream::TextFileStream(const std::string& fname, FileOpen _open) : 
		FileStream(fname, _open, FileMode::fmText)
	{}

	TextFileStream::TextFileStream(const std::wstring& fname, FileOpen _open) :
		FileStream(fname, _open, FileMode::fmText)
	{}

	std::uint32_t MapFileStream::Read(void* buf, std::uint32_t size) const noexcept(true)
	{
		ScopeCriticalSection guard{ _section };

		LARGE_INTEGER offset
		{
			.QuadPart = (LONGLONG)_caret
		};
		
		if ((_caret + size) > _size) 
			size = (std::uint32_t)(_size - _caret);

		auto Ptr = ::MapViewOfFileEx(_handle_map, FILE_MAP_READ, offset.HighPart, offset.LowPart, 
			size, nullptr);
		if (!Ptr)
			return 0;

		memcpy(buf, Ptr, size);
		UnmapViewOfFile(Ptr);

		const_cast<MapFileStream*>(this)->_caret += size;
		return size;
	}

	std::uint32_t MapFileStream::Write(const void* buf, std::uint32_t size) noexcept(true)
	{
		ScopeCriticalSection guard{ _section };

		LARGE_INTEGER offset
		{
			.QuadPart = (LONGLONG)_caret
		};
		LARGE_INTEGER new_offset;

		SetFilePointerEx(_handle, offset, &new_offset, FILE_BEGIN);

		DWORD dwWritten;
		if (WriteFile(_handle, buf, size, &dwWritten, nullptr))
			_caret += dwWritten;

		return dwWritten;
	}

	std::uint64_t MapFileStream::Offset(std::int64_t offset, OffsetStream flag) noexcept(true)
	{
		ScopeCriticalSection guard{ _section };

		switch (flag)
		{
		case Stream::ofBegin:
			_caret = (std::uint64_t)offset;
			break;
		case Stream::ofCurrent:
			_caret += offset;
			break;
		case Stream::ofEnd:
			_caret = _size + offset;
			break;
		}

		return _caret;
	}

	std::uint64_t MapFileStream::GetSize() const noexcept(true)
	{
		return _size;
	}

	std::wstring MapFileStream::GetFileName() const noexcept(true)
	{
		return _FileName ? *_FileName : L"";
	}

	bool MapFileStream::Eof() const noexcept(true)
	{
		return _caret >= _size;
	}

	MapFileStream::MapFileStream(const std::string& fname, FileStream::FileOpen _open, bool UseCache) :
		MapFileStream(StringUtils::Utf8ToUtf16(fname), _open)
	{}

	MapFileStream::MapFileStream(const std::wstring& fname, FileStream::FileOpen _open, bool UseCache) : Stream()
	{
		_handle = CreateFileW(
			fname.c_str(),
			(_open == FileStream::FileOpen::fmOpenRead) ? GENERIC_READ : (GENERIC_READ | GENERIC_WRITE),
			(_open == FileStream::FileOpen::fmOpenRead) ? FILE_SHARE_WRITE : 0,
			nullptr,
			(_open == FileStream::FileOpen::fmCreate) ? CREATE_NEW : OPEN_EXISTING,
			UseCache ? FILE_FLAG_SEQUENTIAL_SCAN : 0,
			nullptr);
		if (_handle == INVALID_HANDLE_VALUE)
			throw SystemError(GetLastError(), "MapFileStream_CreateFileW \"{}\"", 
				StringUtils::Utf16ToWinCP(fname));

		LARGE_INTEGER size;
		if (!GetFileSizeEx(_handle, &size))
		{
			CloseHandle(_handle);
			_handle = INVALID_HANDLE_VALUE;

			throw SystemError(GetLastError(), "MapFileStream_CreateFileW \"{}\"",
				StringUtils::Utf16ToWinCP(fname));
		}
		_size = size.QuadPart;

		_handle_map = CreateFileMappingW(
			_handle, 
			nullptr, 
			(_open == FileStream::FileOpen::fmOpenRead) ? PAGE_READONLY : PAGE_READWRITE,
			0, 
			0, 
			nullptr);
		if (!_handle_map)
		{
			CloseHandle(_handle);
			_handle = INVALID_HANDLE_VALUE;

			throw SystemError(GetLastError(), "MapFileStream_CreateFileMappingW \"{}\"",
				StringUtils::Utf16ToWinCP(fname));
		}

		_FileName = new std::wstring(fname);
	}

	MapFileStream::~MapFileStream() noexcept(true)
	{
		if (_FileName)
		{
			delete _FileName;
			_FileName = nullptr;
		}

		if (_handle_map)
		{
			CloseHandle(_handle_map);
			_handle_map = nullptr;
		}

		if (_handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(_handle);
			_handle = INVALID_HANDLE_VALUE;
		}
	}

	bool FileStreamIntf::LoadFromFile(const std::string& fname) noexcept(true)
	{
		return LoadFromFile(StringUtils::Utf8ToUtf16(fname));
	}

	bool FileStreamIntf::LoadFromFile(const std::wstring& fname) noexcept(true)
	{
		if (fname.empty() || !PathUtils::FileExists(fname))
			return false;

		FileStream stream(fname, FileStream::fmOpenRead);
		return LoadFromStream(stream);
	}

	bool FileStreamIntf::SaveToFile(const std::string& fname) const noexcept(true)
	{
		return SaveToFile(StringUtils::Utf8ToUtf16(fname));
	}

	bool FileStreamIntf::SaveToFile(const std::wstring& fname) const noexcept(true)
	{
		FileStream stream(fname, FileStream::fmCreate);
		return SaveToStream(stream);
	}

	bool MemoryStream::Allocate(std::uint64_t newsize) noexcept(true)
	{
		ScopeCriticalSection guard{ _section };

		if (!_data)
			_data = (std::uint8_t*)std::malloc(newsize);
		else
		{
			auto d = _data;
			_data = (std::uint8_t*)std::realloc(d, newsize);
		}
			
		if (_data)
		{
			_size = newsize;
			if (_size < _caret)
				_caret = _size;

			return true;
		}

		_size = 0;
		_caret = 0;

		return false;
	}

	void MemoryStream::Deallocate() noexcept(true)
	{
		ScopeCriticalSection guard{ _section };

		if (_data)
		{
			std::free(_data);

			_size = 0;
			_caret = 0;
			_data = nullptr;
		}
	}

	void MemoryStream::Set(void* mem, std::uint64_t size) noexcept(true)
	{
		ScopeCriticalSection guard{ _section };
		if (mem && !size && Allocate(size))
			memcpy(_data, mem, size);
	}

	std::uint32_t MemoryStream::Read(void* buf, std::uint32_t size) const noexcept(true)
	{
		ScopeCriticalSection guard{ _section };

		std::uint32_t Ret = size;

		if (Ret > (std::uint32_t)(_size - _caret))
			Ret = (std::uint32_t)(_size - _caret);

		if (!Ret)
			return Ret;

		memcpy(buf, ((char*)_data) + _caret, Ret);
		const_cast<MemoryStream*>(this)->_caret += Ret;

		return Ret;
	}

	std::uint32_t MemoryStream::Write(const void* buf, std::uint32_t size) noexcept(true)
	{
		ScopeCriticalSection guard{ _section };

		std::uint32_t Ret = size;

		if (Ret > (_size - _caret))
			if (!Allocate(_size + (Ret - (_size - _caret))))
				return 0;

		memcpy(((char*)_data) + _caret, buf, Ret);
		_caret += Ret;

		return Ret;
	}

	std::uint64_t MemoryStream::GetSize() const noexcept(true)
	{
		ScopeCriticalSection guard{ _section };
		return _size;
	}

	std::uint64_t MemoryStream::Offset(std::int64_t offset, OffsetStream flag) noexcept(true)
	{
		ScopeCriticalSection guard{ _section };

		switch (flag)
		{
		case Stream::ofBegin:
			_caret = (std::uint64_t)offset;
			break;
		case Stream::ofCurrent:
			_caret += offset;
			break;
		case Stream::ofEnd:
			_caret = _size + offset;
			break;
		}

		return _caret;
	}

	void MemoryStream::SetSize(std::uint64_t newsize)
	{
		ScopeCriticalSection guard{ _section };

		if (!Allocate(newsize))
			RuntimeError("MemoryStream: Out of memory");
	}

	void MemoryStream::Clear() noexcept(true)
	{
		Deallocate();
	}

	bool MemoryStream::LoadFromStream(Stream& stream) noexcept(true)
	{
		Assign(stream);
		return true;
	}

	bool MemoryStream::SaveToStream(Stream& stream) const noexcept(true)
	{
		stream.Assign(*const_cast<MemoryStream*>(this));
		return true;
	}

	MemoryStream::MemoryStream(void* mem, std::uint64_t size) noexcept(true) : Stream(), FileStreamIntf()
	{
		Set(mem, size);
	}

	MemoryStream::MemoryStream(Stream& Stream) noexcept(true) : Stream(), FileStreamIntf()
	{
		CopyFrom(Stream);
	}

	MemoryStream::MemoryStream(const MemoryStream& Stream) noexcept(true) : Stream(), FileStreamIntf()
	{
		Set(Stream._data, Stream._size);
	}

	MemoryStream::~MemoryStream() noexcept(true)
	{
		Deallocate();
	}
}