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

namespace CKPE
{
	bool Zipper::Entry::OpenEntryById(const Zipper* zipper, std::uint32_t id) noexcept(true)
	{
		if (!zipper || !zipper->_zip) 
			return false;
		
		auto r = zip_entry_openbyindex(zipper->_zip, id);
		if (r < 0)
		{
			const_cast<Zipper*>(zipper)->_lasterr = (std::int32_t)r;
			return false;
		}

		_zipper = zipper;
		_id = id;

		return true;
	}

	void Zipper::Entry::CloseEntry() noexcept(true)
	{
		if (!_zipper || !_zipper->_zip)
			return;

		auto r = zip_entry_close(_zipper->_zip);
		if (r < 0)
			const_cast<Zipper*>(_zipper)->_lasterr = (std::int32_t)r;
		else
		{
			_zipper = nullptr;
			_id = 0;
		}
	}

	Zipper::Entry::Entry(const Zipper* zipper, std::uint32_t id)
	{
		if (!OpenEntryById(zipper, id))
			throw RuntimeError("Zipper::Entry failed open by id {}", id);
	}

	Zipper::Entry::~Entry() noexcept(true)
	{
		CloseEntry();
	}

	bool Zipper::Entry::IsDir() const noexcept(true)
	{
		if (!_zipper || !_zipper->_zip)
			return false;

		return zip_entry_isdir(_zipper->_zip) > 0;
	}

	bool Zipper::Entry::GetName(std::string& name) const noexcept(true)
	{
		if (!_zipper || !_zipper->_zip)
			return false;

		auto s = zip_entry_name(_zipper->_zip);
		if (!s) return false;
		name = s;
		return true;
	}

	bool Zipper::Entry::GetSize(std::uint32_t& size) const noexcept(true)
	{
		if (!_zipper || !_zipper->_zip)
			return false;

		auto s = zip_entry_size(_zipper->_zip);
		if (!s) return false;
		size = (std::uint32_t)s;
		return true;
	}

	bool Zipper::Entry::GetSizeInArchive(std::uint32_t& size) const noexcept(true)
	{
		if (!_zipper || !_zipper->_zip)
			return false;

		auto s = zip_entry_comp_size(_zipper->_zip);
		if (!s) return false;
		size = (std::uint32_t)s;
		return true;
	}

	bool Zipper::Entry::GetCrc32(std::uint32_t& hash) const noexcept(true)
	{
		if (!_zipper || !_zipper->_zip)
			return false;

		auto s = zip_entry_crc32(_zipper->_zip);
		if (!s) return false;
		hash = s;
		return true;
	}

	bool Zipper::Entry::Read(Stream& stream) const noexcept(true)
	{
		if (!_zipper || !_zipper->_zip || IsDir())
			return false;

		std::uint32_t fileSize = (std::uint32_t)zip_entry_size(_zipper->_zip);
		auto fileBuffer = std::make_unique<char[]>((std::size_t)fileSize + 1);
		if (!fileBuffer)
		{
			const_cast<Zipper*>(_zipper)->_lasterr = ZIP_EOOMEM;
			return false;
		}

		auto r = zip_entry_noallocread(_zipper->_zip, (void*)fileBuffer.get(), fileSize);
		if (r < 0)
		{
			const_cast<Zipper*>(_zipper)->_lasterr = (std::int32_t)r;
			return false;
		}

		// sets EOF
		fileBuffer.get()[fileSize] = '\0';
		fileSize++;

		return stream.Write((void*)fileBuffer.get(), fileSize) == fileSize;
	}

	Zipper::Zipper(const std::string& fname)
	{
		if (!Open(fname))
			throw std::runtime_error(LastErrorAsString());
	}

	Zipper::Zipper(const std::wstring& fname)
	{
		if (!Open(fname))
			throw std::runtime_error(LastErrorAsString());
	}

	Zipper::~Zipper() noexcept(true)
	{
		Close();
	}

	bool Zipper::Create(const std::string& fname) noexcept(true)
	{
		_zip = zip_openwitherror(fname.c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w',
			(int*)&_lasterr);
		return _zip != nullptr;
	}

	bool Zipper::Create(const std::wstring& fname) noexcept(true)
	{
		_zip = zip_openwitherror(StringUtils::Utf16ToWinCP(fname).c_str(), ZIP_DEFAULT_COMPRESSION_LEVEL, 'w',
			(int*)&_lasterr);
		return _zip != nullptr;
	}

	bool Zipper::Open(const std::string& fname, bool write_access) noexcept(true)
	{
		_zip = zip_openwitherror(fname.c_str(),
			write_access ? ZIP_DEFAULT_COMPRESSION_LEVEL : 0,
			write_access ? 'a' : 'r', (int*)&_lasterr);
		return _zip != nullptr;
	}

	bool Zipper::Open(const std::wstring& fname, bool write_access) noexcept(true)
	{
		_zip = zip_openwitherror(StringUtils::Utf16ToWinCP(fname).c_str(), 
			write_access ? ZIP_DEFAULT_COMPRESSION_LEVEL : 0,
			write_access ? 'a' : 'r', (int*)&_lasterr);
		return _zip != nullptr;
	}

	void Zipper::Close() noexcept(true)
	{
		if (_zip)
		{
			zip_close(_zip);
			_zip = nullptr;
			_lasterr = 0;
		}
	}

	std::uint32_t Zipper::Count() const
	{
		if (!_zip) return 0;
		auto r = zip_entries_total(_zip);
		if (r < 0)
		{
			const_cast<Zipper*>(this)->_lasterr = (std::int32_t)r;
			throw std::runtime_error(LastErrorAsString());
			return 0;
		}
		return (std::uint32_t)r;
	}

	bool Zipper::PackFromFile(const std::string& fname) noexcept(true)
	{
		if (!_zip) return false;

		try
		{
			MemoryStream stream;
			if (!stream.LoadFromFile(fname))
				return false;

			return PackFromStream(fname, stream);
		}
		catch (const std::exception&)
		{
			return false;
		}
	}

	bool Zipper::PackFromFile(const std::wstring& fname) noexcept(true)
	{
		if (!_zip) return false;

		try
		{
			MemoryStream stream;
			if (!stream.LoadFromFile(fname))
				return false;

			return PackFromStream(fname, stream);
		}
		catch (const std::exception&)
		{
			return false;
		}
	}

	bool Zipper::PackFromStream(const std::string& fname, MemoryStream& stream) noexcept(true)
	{
		if (!_zip) return false;

		try
		{
			std::string sfname = fname;
			char* entry[1] = { sfname.data() };

			zip_entries_delete(_zip, entry, 1);

			auto r = zip_entry_open(_zip, entry[0]);
			if (r < 0)
			{
				const_cast<Zipper*>(this)->_lasterr = (std::int32_t)r;
				return false;
			}

			r = zip_entry_write(_zip, stream.Data(), stream.GetSize());
			if (r < 0)
			{
				const_cast<Zipper*>(this)->_lasterr = (std::int32_t)r;
				return false;
			}

			zip_entry_close(_zip);
			return true;
		}
		catch (const std::exception&)
		{
			return false;
		}
	}

	bool Zipper::PackFromStream(const std::wstring& fname, MemoryStream& stream) noexcept(true)
	{
		if (!_zip) return false;

		try
		{
			auto sfname = StringUtils::Utf16ToWinCP(PathUtils::ExtractFileName(fname));
			char* entry[1] = { sfname.data() };
			
			zip_entries_delete(_zip, entry, 1);

			auto r = zip_entry_open(_zip, entry[0]);
			if (r < 0)
			{
				const_cast<Zipper*>(this)->_lasterr = (std::int32_t)r;
				return false;
			}

			r = zip_entry_write(_zip, stream.Data(), stream.GetSize());
			if (r < 0)
			{
				const_cast<Zipper*>(this)->_lasterr = (std::int32_t)r;
				return false;
			}

			zip_entry_close(_zip);
			return true;
		}
		catch (const std::exception&)
		{
			return false;
		}
	}

	std::string Zipper::LastErrorAsString() const noexcept(true)
	{
		std::string r("Zipper: ");

		switch (LastError())
		{
		case 0:
			return r + "No Error";
		case ZIP_ENOINIT:
			return r + "Not initialized";
		case ZIP_EINVENTNAME:
			return r + "Invalid entry name";
		case ZIP_ENOENT:
			return r + "Entry not found";
		case ZIP_EINVMODE:
			return r + "Invalid zip mode";
		case ZIP_EINVLVL:
			return r + "Invalid compression level";
		case ZIP_ENOSUP64:
			return r + "No zip 64 support";
		case ZIP_EMEMSET:
			return r + "Memset error";
		case ZIP_EWRTENT:
			return r + "Cannot write data to entry";
		case ZIP_ETDEFLINIT:
			return r + "Cannot initialize tdefl compressor";
		case ZIP_EINVIDX:
			return r + "Invalid index";
		case ZIP_ENOHDR:
			return r + "Header not found";
		case ZIP_ETDEFLBUF:
			return r + "Cannot flush tdefl buffer";
		case ZIP_ECRTHDR:
			return r + "Cannot create entry header";
		case ZIP_EWRTHDR:
			return r + "Cannot write entry header";
		case ZIP_EWRTDIR:
			return r + "Cannot write to central dir";
		case ZIP_EOPNFILE:
			return r + "Cannot open file";
		case ZIP_EINVENTTYPE:
			return r + "Invalid entry typer";
		case ZIP_EMEMNOALLOC:
			return r + "Extracting data using no memory allocation";
		case ZIP_ENOFILE:
			return r + "File not found";
		case ZIP_ENOPERM:
			return r + "No permission";
		case ZIP_EOOMEM:
			return r + "Out of memory";
		case ZIP_EINVZIPNAME:
			return r + "Invalid zip archive name";
		case ZIP_EMKDIR:
			return r + "Make dir error";
		case ZIP_ESYMLINK:
			return r + "Symlink error";
		case ZIP_ECLSZIP:
			return r + "Close archive error";
		case ZIP_ECAPSIZE:
			return r + "Capacity size too small";
		case ZIP_EFSEEK:
			return r + "fseek error";
		case ZIP_EFREAD:
			return r + "fread error";
		case ZIP_EFWRITE:
			return r + "fwrite error";
		case ZIP_ERINIT:
			return r + "Cannot initialize reader";
		case ZIP_EWINIT:
			return r + "Cannot initialize writer";
		case ZIP_EWRINIT:
			return r + "Cannot initialize writer from reader";
		default:
			return r + "Unknown";
		}
	}
}