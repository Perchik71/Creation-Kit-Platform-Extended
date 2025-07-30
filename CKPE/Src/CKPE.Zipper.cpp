// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <zip.h>
#include <memory>
#include <format>
#include <CKPE.StringUtils.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Zipper.h>
#include <CKPE.Exception.h>

#include <CKPE.MessageBox.h>

#define CKPE_MZ_OPENARCHIVE_FROM_STREAM 1

namespace CKPE
{
	TZipObject::~TZipObject() noexcept(true)
	{
		if (_handle)
		{
			if (_stream_init)
				zip_stream_close(GetHandle<zip_t>());
			else
				zip_close(GetHandle<zip_t>());

			_handle = nullptr;
		}
	}

	const char* TZipObject::LastErrorByString() const noexcept(true)
	{
		switch (LastError)
		{
		case 0:
			return "No Error";
		case ZIP_ENOINIT:
			return "Not initialized";
		case ZIP_EINVENTNAME:
			return "Invalid entry name";
		case ZIP_ENOENT:
			return "Entry not found";
		case ZIP_EINVMODE:
			return "Invalid zip mode";
		case ZIP_EINVLVL:
			return "Invalid compression level";
		case ZIP_ENOSUP64:
			return "No zip 64 support";
		case ZIP_EMEMSET:
			return "Memset error";
		case ZIP_EWRTENT:
			return "Cannot write data to entry";
		case ZIP_ETDEFLINIT:
			return "Cannot initialize tdefl compressor";
		case ZIP_EINVIDX:
			return "Invalid index";
		case ZIP_ENOHDR:
			return "Header not found";
		case ZIP_ETDEFLBUF:
			return "Cannot flush tdefl buffer";
		case ZIP_ECRTHDR:
			return "Cannot create entry header";
		case ZIP_EWRTHDR:
			return "Cannot write entry header";
		case ZIP_EWRTDIR:
			return "Cannot write to central dir";
		case ZIP_EOPNFILE:
			return "Cannot open file";
		case ZIP_EINVENTTYPE:
			return "Invalid entry typer";
		case ZIP_EMEMNOALLOC:
			return "Extracting data using no memory allocation";
		case ZIP_ENOFILE:
			return "File not found";
		case ZIP_ENOPERM:
			return "No permission";
		case ZIP_EOOMEM:
			return "Out of memory";
		case ZIP_EINVZIPNAME:
			return "Invalid zip archive name";
		case ZIP_EMKDIR:
			return "Make dir error";
		case ZIP_ESYMLINK:
			return "Symlink error";
		case ZIP_ECLSZIP:
			return "Close archive error";
		case ZIP_ECAPSIZE:
			return "Capacity size too small";
		case ZIP_EFSEEK:
			return "fseek error";
		case ZIP_EFREAD:
			return "fread error";
		case ZIP_EFWRITE:
			return "fwrite error";
		case ZIP_ERINIT:
			return "Cannot initialize reader";
		case ZIP_EWINIT:
			return "Cannot initialize writer";
		case ZIP_EWRINIT:
			return "Cannot initialize writer from reader";
		default:
			return "Unknown";
		}
	}

	ZipFileScopeEntry::ZipFileScopeEntry(ZipFileEntry* entry) noexcept(true) :
		_entry(entry)
	{}

	ZipFileScopeEntry::~ZipFileScopeEntry()
	{
		if (_entry)
		{
			_entry->Close();
			_entry = nullptr;
		}
	}

	ZipFileEntry::ZipFileEntry(const ZipFileEntries* entries, std::size_t idx) noexcept(true) :
		_entries(entries), _idx(idx)
	{}

	bool ZipFileEntry::IsDir() const noexcept(true)
	{
		if (!_entries || (_idx == InvalidIndex) || (_entries->Count() <= _idx))
			return false;

		return zip_entry_isdir(_entries->Zip()->GetHandle<zip_t>()) > 0;
	}

	std::string ZipFileEntry::GetName() const noexcept(true)
	{
		if (!_entries || (_idx == InvalidIndex) || (_entries->Count() <= _idx))
			return "";

		auto s = zip_entry_name(_entries->Zip()->GetHandle<zip_t>());
		return s ? s : "";
	}

	std::size_t ZipFileEntry::GetSize() const noexcept(true)
	{
		if (!_entries || (_idx == InvalidIndex) || (_entries->Count() <= _idx))
			return 0;

		return zip_entry_size(_entries->Zip()->GetHandle<zip_t>());
	}

	std::size_t ZipFileEntry::GetSizeInArchive() const noexcept(true)
	{
		if (!_entries || (_idx == InvalidIndex) || (_entries->Count() <= _idx))
			return 0;

		return zip_entry_comp_size(_entries->Zip()->GetHandle<zip_t>());
	}

	std::uint32_t ZipFileEntry::GetCrc32() const noexcept(true)
	{
		if (!_entries || (_idx == InvalidIndex) || (_entries->Count() <= _idx))
			return std::numeric_limits<std::uint32_t>::max();

		return zip_entry_crc32(_entries->Zip()->GetHandle<zip_t>());
	}

	bool ZipFileEntry::ReadToStream(Stream& stream) const noexcept(true)
	{
		if (!_entries || (_idx == InvalidIndex) || (_entries->Count() <= _idx) || IsDir() ||
			!dynamic_cast<const UnZipper*>(_entries->Zip()))
			return false;

		auto fileSize = zip_entry_size(_entries->Zip()->GetHandle<zip_t>());
		auto fileBuffer = std::make_unique<char[]>((std::size_t)fileSize + 1);
		if (!fileBuffer)
		{
			const_cast<TZipObject*>(_entries->Zip())->LastError = ZIP_EOOMEM;
			return false;
		}

		auto r = zip_entry_noallocread(_entries->Zip()->GetHandle<zip_t>(), (void*)fileBuffer.get(), fileSize);
		if (r < 0)
		{
			const_cast<TZipObject*>(_entries->Zip())->LastError = (std::int32_t)r;
			return false;
		}

		// sets EOF
		fileBuffer.get()[fileSize] = '\0';
		fileSize++;

		return stream.Write((void*)fileBuffer.get(), (std::uint32_t)fileSize) == (std::uint32_t)fileSize;
	}

	void ZipFileEntry::Close() const noexcept(true)
	{
		zip_entry_close(_entries->Zip()->GetHandle<zip_t>());
	}

	ZipFileEntries::ZipFileEntries(const TZipObject* class_owner) noexcept(true) :
		_class_owner(class_owner), _items(new std::vector<ZipFileEntry*>)
	{
		if (_class_owner && _items && dynamic_cast<const UnZipper*>(_class_owner))
		{
			for (size_t i = 0; i < Count(); i++)
				_items->push_back(new ZipFileEntry(this, i));
		}
	}

	ZipFileEntries::~ZipFileEntries() noexcept(true)
	{
		if (_items)
		{
			delete _items;
			_items = nullptr;
		}
	}

	std::size_t ZipFileEntries::Count() const noexcept(true)
	{
		if (!_class_owner)
			return 0;

		ScopeCriticalSection guard{ _section };

		auto r = zip_entries_total(_class_owner->GetHandle<zip_t>());
		if (r < 0) const_cast<TZipObject*>(_class_owner)->LastError = (std::int32_t)r;
		return (size_t)r;
	}

	SmartPointer<ZipFileScopeEntry> ZipFileEntries::At(std::size_t idx) const noexcept(true)
	{
		if (!_class_owner)
			return nullptr;

		if (!dynamic_cast<const UnZipper*>(_class_owner))
		{
			const_cast<TZipObject*>(_class_owner)->LastError = ZIP_EINVMODE;
			return nullptr;
		}

		ScopeCriticalSection guard{ _section };

		auto r = zip_entry_openbyindex(_class_owner->GetHandle<zip_t>(), idx);
		if (r < 0)
		{
			const_cast<TZipObject*>(_class_owner)->LastError = (std::int32_t)r;
			return nullptr;
		}

		if (!_items || (idx >= Count()))
		{
			const_cast<TZipObject*>(_class_owner)->LastError = ZIP_EINVIDX;
			return nullptr;
		}

		return MakeSmartPointer<ZipFileScopeEntry>(_items->at(idx));
	}

	bool ZipFileEntries::AddFile(const char* fname) noexcept(true)
	{
		if (!fname || !fname[0])
			return false;
		return AddFile(StringUtils::Utf8ToUtf16(fname));
	}

	bool ZipFileEntries::AddFile(const wchar_t* fname) noexcept(true)
	{
		if (!_class_owner || !fname || !fname[0])
			return false;

		ScopeCriticalSection guard{ _section };

		if (dynamic_cast<const Zipper*>(_class_owner))
		{
			try
			{
				if (!PathUtils::FileExists(fname))
				{
					const_cast<TZipObject*>(_class_owner)->LastError = ZIP_ENOFILE;
					return false;
				}

				MemoryStream stream;
				if (!stream.LoadFromFile(fname))
				{
					const_cast<TZipObject*>(_class_owner)->LastError = ZIP_EFREAD;
					return false;
				}

				return AddStream(fname, stream);
			}
			catch (const std::exception&)
			{
				const_cast<TZipObject*>(_class_owner)->LastError = ZIP_EOPNFILE;
				return false;
			}
		}

		const_cast<TZipObject*>(_class_owner)->LastError = ZIP_EINVMODE;
		return false;
	}

	bool ZipFileEntries::AddFile(const std::string& fname) noexcept(true)
	{
		return AddFile(StringUtils::Utf8ToUtf16(fname));
	}

	bool ZipFileEntries::AddFile(const std::wstring& fname) noexcept(true)
	{
		return AddFile(fname.c_str());
	}

	bool ZipFileEntries::AddStream(const char* fname, MemoryStream& stm) noexcept(true)
	{
		if (!_class_owner || stm.Empty() || !fname || !fname[0])
			return false;

		ScopeCriticalSection guard{ _section };

		if (dynamic_cast<const Zipper*>(_class_owner))
		{
			auto r = zip_entry_open(_class_owner->GetHandle<zip_t>(), fname);
			if (r < 0)
			{
				const_cast<TZipObject*>(_class_owner)->LastError = (std::int32_t)r;
				return false;
			}

			r = zip_entry_write(_class_owner->GetHandle<zip_t>(), stm.Data(), stm.GetSize());
			if (r < 0)
			{
				const_cast<TZipObject*>(_class_owner)->LastError = (std::int32_t)r;
				zip_entry_close(_class_owner->GetHandle<zip_t>());

				return false;
			}
			zip_entry_close(_class_owner->GetHandle<zip_t>());
		}

		const_cast<TZipObject*>(_class_owner)->LastError = ZIP_EINVMODE;
		return false;
	}

	bool ZipFileEntries::AddStream(const wchar_t* fname, MemoryStream& stm) noexcept(true)
	{
		if (!fname || !fname[0])
			return false;
		return AddStream(StringUtils::Utf16ToUtf8(fname), stm);
	}

	bool ZipFileEntries::AddStream(const std::string& fname, MemoryStream& stm) noexcept(true)
	{
		return AddStream(fname.c_str(), stm);
	}

	bool ZipFileEntries::AddStream(const std::wstring& fname, MemoryStream& stm) noexcept(true)
	{
		return AddStream(StringUtils::Utf16ToUtf8(fname), stm);
	}

	UnZipper::UnZipper() noexcept(true) :
		TZipObject(), _fname(new std::wstring)
	{}

	UnZipper::UnZipper(const char* fname) noexcept(true) :
		TZipObject(), _fname(new std::wstring)
	{
		OpenFile(fname);
	}

	UnZipper::UnZipper(const wchar_t* fname) noexcept(true) :
		TZipObject(), _fname(new std::wstring)
	{
		OpenFile(fname);
	}

	UnZipper::UnZipper(const std::string& fname) noexcept(true) :
		TZipObject(), _fname(new std::wstring)
	{
		OpenFile(fname);
	}

	UnZipper::UnZipper(const std::wstring& fname) noexcept(true) :
		TZipObject(), _fname(new std::wstring)
	{
		OpenFile(fname);
	}

	UnZipper::~UnZipper() noexcept(true)
	{
		Close();

		if (_fname)
		{
			delete _fname;
			_fname = nullptr;
		}
	}

	bool UnZipper::OpenFile(const char* fname) noexcept(true)
	{
		return OpenFile(StringUtils::Utf8ToUtf16(fname));
	}

	bool UnZipper::OpenFile(const wchar_t* fname) noexcept(true)
	{
		Close();

		ScopeCriticalSection guard{ _section };

		try
		{
			if (!PathUtils::FileExists(fname))
			{
				LastError = ZIP_ENOFILE;
				return false;
			}

#if (CKPE_MZ_OPENARCHIVE_FROM_STREAM == 1)
			_stream.Clear();
			if (!_stream.LoadFromFile(fname))
			{
				LastError = ZIP_EFREAD;
				return false;
			}
			
			return OpenStream(fname, _stream);
#else
			_handle = (void*)zip_openwitherror(StringUtils::Utf16ToWinCP(fname).c_str(), 0, 'r',
				(int*)&LastError);

			if (!_handle)
				return false;

			*_fname = fname;
			_entries = new ZipFileEntries(this);
			if (!_entries)
			{
				LastError = ZIP_EOOMEM;
				zip_close(GetHandle<zip_t>());
				_handle = nullptr;
				return false;
			}

			return true;
#endif // CKPE_MZ_OPENARCHIVE_FROM_STREAM
		}
		catch (const std::exception&)
		{
			LastError = ZIP_EOPNFILE;
			return false;
		}
	}

	bool UnZipper::OpenFile(const std::string& fname) noexcept(true)
	{
		return OpenFile(StringUtils::Utf8ToUtf16(fname));
	}

	bool UnZipper::OpenFile(const std::wstring& fname) noexcept(true)
	{
		return OpenFile(fname.c_str());
	}

	bool UnZipper::OpenStream(const wchar_t* fname, MemoryStream& stm) noexcept(true)
	{
#if (CKPE_MZ_OPENARCHIVE_FROM_STREAM == 1)
		if (stm.Empty())
			return false;

		Close();

		ScopeCriticalSection guard{ _section };

		_handle = (void*)zip_stream_openwitherror((const char*)stm.Data(), stm.GetSize(), 0, 'r', 
			(int*)&LastError);
		
		if (!_handle)
			return false;

		*_fname = fname;
		_stream_init = true;
		_entries = new ZipFileEntries(this);
		if (!_entries)
		{
			LastError = ZIP_EOOMEM;
			zip_stream_close(GetHandle<zip_t>());
			_handle = nullptr;

			return false;
		}

		return true;
#else
		return false;
#endif // CKPE_MZ_OPENARCHIVE_FROM_STREAM
	}

	void UnZipper::Close() noexcept(true)
	{
		if (!HasOpen())
			return;

		ScopeCriticalSection guard{ _section };

		// if open then _entries exists
		delete _entries;
		_entries = nullptr;

		if (_handle)
		{
			if (_stream_init)
				zip_stream_close(GetHandle<zip_t>());
			else
				zip_close(GetHandle<zip_t>());

			_handle = nullptr;
		}

		_stream_init = false;
	}

	std::wstring UnZipper::GetFileName() const noexcept(true)
	{
		return _fname ? *_fname : L"";
	}

	std::size_t UnZipper::IndexOf(const char* fname) const noexcept(true)
	{
		if (!HasOpen() || !fname || !fname[0])
			return ZipFileEntry::InvalidIndex;

		for (size_t i = 0; i < _entries->Count(); i++)
		{
			auto entry = _entries->At(i);
			if (!entry.Empty() && entry->Get() && !_stricmp(entry->Get()->GetName().c_str(), fname))
				return i;
		}

		return ZipFileEntry::InvalidIndex;
	}

	std::size_t UnZipper::IndexOf(const wchar_t* fname) const noexcept(true)
	{
		if (!fname || !fname[0])
			return ZipFileEntry::InvalidIndex;

		return IndexOf(StringUtils::Utf16ToUtf8(fname));
	}

	std::size_t UnZipper::IndexOf(const std::string& fname) const noexcept(true)
	{
		return IndexOf(fname.c_str());
	}

	std::size_t UnZipper::IndexOf(const std::wstring& fname) const noexcept(true)
	{
		return IndexOf(StringUtils::Utf16ToUtf8(fname));
	}

	bool UnZipper::UnZipFile(const char* fname, const char* path) const noexcept(true)
	{
		if (!fname || !fname[0] || !path || !path[0])
			return false;

		return UnZipFile(StringUtils::Utf8ToUtf16(fname), StringUtils::Utf8ToUtf16(path));
	}

	bool UnZipper::UnZipFile(const wchar_t* fname, const wchar_t* path) const noexcept(true)
	{
		if (!fname || !fname[0] || !path || !path[0])
			return false;

		try
		{
			auto idx = IndexOf(fname);
			if (idx == ZipFileEntry::InvalidIndex)
				return false;

			auto entry = _entries->At(idx);
			if (entry.Empty() || !entry->Get())
				return false;

			std::wstring spath = path;
			PathUtils::CreateFolder(spath);

			FileStream stream(PathUtils::IncludeTrailingPathDelimiter(spath) + fname, FileStream::fmCreate);
			return entry->Get()->ReadToStream(stream);
		}
		catch (const std::exception&)
		{
			return false;
		}
	}

	bool UnZipper::UnZipFile(const std::string& fname, const std::string& path) const noexcept(true)
	{
		return UnZipFile(StringUtils::Utf8ToUtf16(fname), StringUtils::Utf8ToUtf16(path));
	}

	bool UnZipper::UnZipFile(const std::wstring& fname, const std::wstring& path) const noexcept(true)
	{
		return UnZipFile(fname.c_str(), path.c_str());
	}

	bool UnZipper::UnZipAllFiles(const char* fname, const char* path) noexcept(true)
	{
		if (!fname || !fname[0] || !path || !path[0])
			return false;

		return UnZipAllFiles(StringUtils::Utf8ToUtf16(fname), StringUtils::Utf8ToUtf16(path));
	}

	bool UnZipper::UnZipAllFiles(const wchar_t* fname, const wchar_t* path) noexcept(true)
	{
		if (!fname || !fname[0] || !path || !path[0])
			return false;

		try
		{
			if (!PathUtils::FileExists(fname))
				return false;

			MemoryStream stream;
			if (!stream.LoadFromFile(fname))
				return false;

			auto on_extract_entry = [](const char* filename, void* arg) -> int
				{
					//static int i = 0;
					//int n = *(int*)arg;
					//printf("Extracted: %s (%d of %d)\n", filename, ++i, n);

					return 0;
				};

			PathUtils::CreateFolder(path);

			int arg = 2;
			return zip_stream_extract((const char*)stream.Data(), stream.GetSize(), 
				StringUtils::Utf16ToWinCP(path).c_str(), on_extract_entry, &arg);
		}
		catch (const std::exception&)
		{
			return false;
		}
	}

	bool UnZipper::UnZipAllFiles(const std::string& fname, const std::string& path) noexcept(true)
	{
		return UnZipAllFiles(StringUtils::Utf8ToUtf16(fname), StringUtils::Utf8ToUtf16(path));
	}

	bool UnZipper::UnZipAllFiles(const std::wstring& fname, const std::wstring& path) noexcept(true)
	{
		return UnZipAllFiles(fname.c_str(), path.c_str());
	}

	Zipper::Zipper() noexcept(true) :
		TZipObject(), _fname(new std::wstring)
	{
		OpenStream();
	}

	Zipper::Zipper(const char* fname) noexcept(true) :
		TZipObject(), _fname(new std::wstring)
	{
		OpenFile(fname);
	}

	Zipper::Zipper(const wchar_t* fname) noexcept(true) :
		TZipObject(), _fname(new std::wstring)
	{
		OpenFile(fname);
	}

	Zipper::Zipper(const std::string& fname) noexcept(true) :
		TZipObject(), _fname(new std::wstring)
	{
		OpenFile(fname);
	}

	Zipper::Zipper(const std::wstring& fname) noexcept(true) :
		TZipObject(), _fname(new std::wstring)
	{
		OpenFile(fname);
	}

	Zipper::~Zipper() noexcept(true)
	{
		Close();

		if (_fname)
		{
			delete _fname;
			_fname = nullptr;
		}
	}

	bool Zipper::OpenFile(const char* fname) noexcept(true)
	{
		if (!_fname)
			return false;

		return OpenFile(StringUtils::Utf8ToUtf16(fname));
	}

	bool Zipper::OpenFile(const wchar_t* fname) noexcept(true)
	{
		if (!_fname)
			return false;

		Close();

		ScopeCriticalSection guard{ _section };

		try
		{
			char wm = 'w';
			if (PathUtils::FileExists(fname))
				wm = 'a';

			_handle = (void*)zip_openwitherror(StringUtils::Utf16ToWinCP(fname).c_str(),
				ZIP_DEFAULT_COMPRESSION_LEVEL, wm, (int*)&LastError);

			if (!_handle)
				return false;

			*_fname = fname;
			_entries = new ZipFileEntries(this);
			if (!_entries)
			{
				LastError = ZIP_EOOMEM;
				zip_close(GetHandle<zip_t>());
				return false;
			}

			return true;
		}
		catch (const std::exception&)
		{
			LastError = ZIP_EOPNFILE;
			return false;
		}
	}

	bool Zipper::OpenFile(const std::string& fname) noexcept(true)
	{
		return OpenFile(StringUtils::Utf8ToUtf16(fname));
	}

	bool Zipper::OpenFile(const std::wstring& fname) noexcept(true)
	{
		return OpenFile(fname.c_str());
	}

	bool Zipper::OpenStream() noexcept(true)
	{
		if (!_fname)
			return false;

		Close();

		ScopeCriticalSection guard{ _section };

		_handle = (void*)zip_stream_openwitherror(nullptr, 0, ZIP_DEFAULT_COMPRESSION_LEVEL, 'w',
			(int*)&LastError);

		if (!_handle)
			return false;

		_stream_init = true;
		_fname->clear();
		_entries = new ZipFileEntries(this);
		if (!_entries)
		{
			LastError = ZIP_EOOMEM;
			zip_stream_close(GetHandle<zip_t>());
			return false;
		}

		return true;
	}

	void Zipper::Close() noexcept(true)
	{
		if (!HasOpen())
			return;

		ScopeCriticalSection guard{ _section };

		LastError = 0;

		if (_entries)
		{
			delete _entries;
			_entries = nullptr;
		}

		if (_handle)
		{
			if (_stream_init)
				zip_stream_close(GetHandle<zip_t>());
			else
				zip_close(GetHandle<zip_t>());

			_handle = nullptr;
		}

		_stream_init = false;
	}

	std::wstring Zipper::GetFileName() const noexcept(true)
	{
		return _fname ? *_fname : L"";
	}

	bool Zipper::DeleteFile(const char* fname) noexcept(true)
	{
		if (!fname || !fname[0] || !HasOpen())
			return false;

		ScopeCriticalSection guard{ _section };
		std::wstring safe_n = *_fname;

		Close();

		auto h = zip_openwitherror(StringUtils::Utf16ToWinCP(safe_n).c_str(), 0, 'd', (int*)&LastError);
		if (!h)
		{
			OpenFile(safe_n);
			return false;
		}

		auto r = zip_entries_delete(h, (char**)&fname, 1);
		if (r < 0)
		{
			LastError = (std::int32_t)r;
			
			zip_close(h);
			OpenFile(safe_n);

			return false;
		}

		zip_close(h);
		OpenFile(safe_n);

		return true;
	}

	bool Zipper::DeleteFile(const wchar_t* fname) noexcept(true)
	{
		if (!fname || !fname[0] || !HasOpen())
			return false;

		return DeleteFile(StringUtils::Utf16ToUtf8(fname));
	}

	bool Zipper::DeleteFile(const std::string& fname) noexcept(true)
	{
		return DeleteFile(fname.c_str());
	}

	bool Zipper::DeleteFile(const std::wstring& fname) noexcept(true)
	{
		return DeleteFile(StringUtils::Utf16ToUtf8(fname));
	}

	bool Zipper::ZipFile(const char* fname) noexcept(true)
	{
		if (!fname || !fname[0] || !HasOpen())
			return false;

		return ZipFile(StringUtils::Utf8ToUtf16(fname));
	}

	bool Zipper::ZipFile(const wchar_t* fname) noexcept(true)
	{
		if (!fname || !fname[0] || !HasOpen())
			return false;

		DeleteFile(fname);

		ScopeCriticalSection guard{ _section };

		return _entries->AddFile(fname);
	}

	bool Zipper::ZipFile(const std::string& fname) noexcept(true)
	{
		return ZipFile(StringUtils::Utf8ToUtf16(fname));
	}

	bool Zipper::ZipFile(const std::wstring& fname) noexcept(true)
	{
		return ZipFile(fname.c_str());
	}

	bool Zipper::ZipFileAsStream(const char* fname, MemoryStream& stream) noexcept(true)
	{
		if (!fname || !fname[0] || !HasOpen())
			return false;

		return ZipFileAsStream(StringUtils::Utf8ToUtf16(fname), stream);
	}

	bool Zipper::ZipFileAsStream(const wchar_t* fname, MemoryStream& stream) noexcept(true)
	{
		if (!fname || !fname[0] || !HasOpen())
			return false;

		DeleteFile(fname);

		ScopeCriticalSection guard{ _section };

		return _entries->AddStream(fname, stream);
	}

	bool Zipper::ZipFileAsStream(const std::string& fname, MemoryStream& stream) noexcept(true)
	{
		return ZipFileAsStream(StringUtils::Utf8ToUtf16(fname), stream);
	}

	bool Zipper::ZipFileAsStream(const std::wstring& fname, MemoryStream& stream) noexcept(true)
	{
		return ZipFileAsStream(fname.c_str(), stream);
	}
	
	bool Zipper::ZipFiles(const std::string& fname,
		const std::vector<std::string>& flist) noexcept(true)
	{
		auto list = std::make_unique<char* []>(flist.size());
		if (!list) return false;
		for (size_t i = 0; i < flist.size(); i++)
		{
			auto it = flist.begin();
			std::advance(it, i);
			list[i] = const_cast<char*>(it->data());
		}
		return zip_create(fname.c_str(), (const char**)list.get(), flist.size()) >= 0;
	}

	bool Zipper::ZipFiles(const std::wstring& fname,
		const std::vector<std::wstring>& flist) noexcept(true)
	{
		std::vector<std::string> list;
		for (auto& s : flist)
			list.push_back(StringUtils::Utf16ToWinCP(s));

		return ZipFiles(StringUtils::Utf16ToWinCP(fname), list);
	}
}