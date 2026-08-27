// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>

#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Application.h>
#include <CKPE.PathUtils.h>
#include <CKPE.HashUtils.h>
#include <CKPE.Zipper.h>
#include <CKPE.MessageBox.h>
#include <CKPE.Stream.h>
#include <CKPE.Exception.h>
#include <algorithm>
#include <format>


using namespace std::literals;

namespace CKPE
{
	namespace Common
	{
		enum class IDVersionDatabase
		{
			kVersionDatabase_Failed = 0,
			kVersionDatabase_0,
			kVersionDatabase_1,
		};

		bool IDDatabase::Load_v0(Stream& stm, const std::wstring& fname) noexcept(true)
		{
			try
			{
				stm.SetPosition(0);

				std::uint64_t count = 0;
				if (stm.Read(&count, sizeof(count)) != sizeof(count))
					throw RuntimeError(L"IDDatabase::Load_v0 file \"{}\" is broken (couldn't read header)"sv, fname);

				auto expected_size = static_cast<std::uint64_t>(sizeof(count) + count * sizeof(Entry_v0));
				if ((stm.GetSize()) != expected_size)
					throw RuntimeError(L"IDDatabase::Load_v0 file \"{}\" has an unexpected size ({}/{})"sv,
						fname, stm.GetSize(), expected_size);

				std::vector<Entry_v0> entries;
				entries.resize(static_cast<std::size_t>(count));

				if (count)
				{
					auto bytes_to_read	= static_cast<std::uint32_t>(count * sizeof(Entry_v0));
					auto bytes_reads	= stm.Read(entries.data(), bytes_to_read);
					if (bytes_reads != bytes_to_read)
						throw RuntimeError(L"IDDatabase::Load_v0 file \"{}\" is broken (short read) ({}/{})"sv, 
							fname, bytes_to_read, bytes_reads);
				}

				_entries->resize(static_cast<std::size_t>(count));

				(*_entries)[0].Id		= static_cast<std::uint32_t>(entries[0].Id);
				(*_entries)[0].Offset	= static_cast<std::uint32_t>(entries[0].Offset);

				for (std::size_t i = 1; i < entries.size(); i++)
				{
					auto& e = entries[i];
					if (e.Id <= entries[i - 1].Id)
						throw RuntimeError(L"IDDatabase::Load_v0 file \"{}\" entries aren't sorted/unique by id"sv, fname);

					auto& es	= (*_entries)[i];
					es.Id		= static_cast<std::uint32_t>(e.Id);
					es.Offset	= static_cast<std::uint32_t>(e.Offset);
				}

				return true;
			}
			catch (const std::exception& e)
			{
				_ERROR(e.what());
				Clear();
			}

			return false;
		}

		bool IDDatabase::Load_v1(Stream& stm, const Header& hdr, const std::wstring& fname) noexcept(true)
		{
			try
			{
				stm.SetPosition(0);

				auto expected_size = static_cast<std::uint64_t>(sizeof(Header) + hdr.count * sizeof(Entry_v1));
				if ((stm.GetSize()) != expected_size)
					throw RuntimeError(L"IDDatabase::Load_v1 file \"{}\" has an unexpected size ({}/{})"sv,
						fname, stm.GetSize(), expected_size);

				_entries->resize((std::size_t)hdr.count);
				stm.SetPosition(static_cast<std::uint64_t>(sizeof(Header)));

				if (hdr.count)
				{
					auto bytes_to_read = static_cast<std::uint32_t>(hdr.count * sizeof(Entry_v1));
					auto bytes_reads = stm.Read(_entries->data(), bytes_to_read);
					if (bytes_reads != bytes_to_read)
						throw RuntimeError(L"IDDatabase::Load_v1 file \"{}\" is broken (short read) ({}/{})",
							fname, bytes_to_read, bytes_reads);

					if (hdr.crc32 != HashUtils::CRC32Buffer(_entries->data(), bytes_to_read))
						throw RuntimeError(L"IDDatabase::Load_v1 file \"{}\" is broken", fname);
				}

				for (std::size_t i = 1; i < _entries->size(); i++)
					if ((*_entries)[i].Id <= (*_entries)[i - 1].Id)
						throw RuntimeError(L"IDDatabase::Load_v1 file \"{}\" entries aren't sorted/unique by id", fname);

				return true;
			}
			catch (const std::exception& e)
			{
				_ERROR(e.what());
				Clear();
			}

			return false;
		}
		
		IDDatabase::IDDatabase() :
			_entries(new std::vector<Entry_v1>)
		{}

		IDDatabase::~IDDatabase()
		{
			Clear();
		}

		bool IDDatabase::Load(const std::uint8_t a_runtime_index) noexcept(true)
		{
			Clear();

			try
			{
				auto app = Application::GetSingleton();
				auto ver = app->GetFileVersion();
				if (!ver.has_value())
					throw RuntimeError("IDDatabase::Load get version game failed"sv);

				_version = ver.value();
				const auto db_name = std::format(L"{}version-{}.bin"sv, 
					PathUtils::GetCKPEAddressLibraryPath(), _version.wstring(L"-"sv));
				if (!PathUtils::FileExists(db_name))
					throw RuntimeError(L"IDDatabase::Load file \"{}\" no found"sv, db_name);
				else
				{
					FileStream2 fstm(db_name, FileStream::fmOpenRead);
					Header header;

					if (fstm.Read(&header, sizeof(header)) != sizeof(header))
						throw RuntimeError(L"IDDatabase::Load unexpected end of file \"{}\""sv, db_name);

					IDVersionDatabase dbVer = IDVersionDatabase::kVersionDatabase_0;
					if (header.magick == Header::MAGICK)
						dbVer = header.version == Header::CURRENT_VERSION ? 
							IDVersionDatabase::kVersionDatabase_1 :
							IDVersionDatabase::kVersionDatabase_Failed;
					
					switch (dbVer)
					{
					case IDVersionDatabase::kVersionDatabase_0:
						_loaded = Load_v0(fstm, db_name);
						break;
					case IDVersionDatabase::kVersionDatabase_1:
						_loaded = Load_v1(fstm, header, db_name);
						break;
					default:
						break;
					}

					if (_loaded)
						_MESSAGE_EX(L"IDDatabase::Load \"{}\" loaded ({} entries) ({} runtime index)"sv, db_name,
							static_cast<std::uint32_t>(_entries->size()), _runtime);
					else
						_ERROR_EX(L"IDDatabase::Load \"{}\" file no found."sv, db_name);
				}
			}
			catch (const std::exception& e)
			{
				_ERROR(e.what());
			}

			if (_loaded)
				_runtime = a_runtime_index;

			return _loaded;
		}

		bool IDDatabase::Save(const std::wstring& fname) noexcept(true)
		{
			if (!_loaded || !_entries || _entries->empty())
				return false;

			try
			{
				FileStream2 fstm(fname, FileStream::fmCreate);
				Header header;

				header.count = static_cast<std::uint32_t>(_entries->size());
				auto bytes_to_write = static_cast<std::uint32_t>(header.count * sizeof(Entry_v1));
				header.crc32 = HashUtils::CRC32Buffer(_entries->data(), bytes_to_write);
				if (fstm.Write(&header, sizeof(header)) != sizeof(header))
					throw RuntimeError(L"IDDatabase::Save error writing header to a file \"{}\""sv, fname);

				auto bytes_wrote = fstm.Write(_entries->data(), bytes_to_write);
				if (bytes_to_write != bytes_wrote)
					throw RuntimeError(L"IDDatabase::Save error writing file to a \"{}\" ({}/{})"sv,
						fname, bytes_to_write, bytes_wrote);

				return true;
			}
			catch (const std::exception& e)
			{
				_ERROR(e.what());
			}

			return false;
		}

		void IDDatabase::Clear() noexcept(true)
		{
			if (_loaded)
			{
				_loaded = false;
				_runtime = 0xFF;

				if (_entries)
					_entries->clear();
			}
		}

		std::uint32_t IDDatabase::GetCount() const noexcept(true)
		{
			return _loaded && _entries ? static_cast<std::uint32_t>(_entries->size()) : 0;
		}

		IDDatabase::AddressOffset IDDatabase::ResolveOffset(AddressID id) const noexcept(true)
		{
			if (!_loaded || !_entries || _entries->empty())
				return 0;

			auto it = std::lower_bound(_entries->begin(), _entries->end(), id,
				[](const Entry_v1& e, AddressID value) noexcept(true) { return e.Id < value; });

			if (it == _entries->end() || it->Id != id)
			{
				auto app = Application::GetSingleton();
				auto msg = std::format(
					"Failed to find offset for Address Library ID!\n"
					"Invalid ID: {}\n"
					"Game Version: {}",
					id, app->GetFileVersion().value().string());
				
				if (!IsDebuggerPresent())
					CKPE::MessageBox::OpenError(msg);
				else
					OutputDebugStringA(msg.c_str());

				DebugBreak();
				app->Terminate();
			}

			return it->Offset;
		}

		std::uintptr_t IDDatabase::Resolve(AddressID id) const noexcept(true)
		{
			auto offset = ResolveOffset(id);
			if (!offset)
				return 0;

			auto base = Application::GetSingleton()->GetBase();
			return static_cast<std::uintptr_t>(base) + offset;
		}

		IDDatabase::AddressOffset IDDatabase::ResolveOffset(const VariantID& id) const noexcept(true)
		{
			const auto variant = id.Get(_version);
			const auto offset = ResolveOffset(variant.ID);
			if (!offset)
				return 0;

			return offset + variant.Offset;
		}

		std::uintptr_t IDDatabase::Resolve(const VariantID& id) const noexcept(true)
		{
			const auto offset = ResolveOffset(id);
			if (!offset)
				return 0;

			auto base = Application::GetSingleton()->GetBase();
			return static_cast<std::uintptr_t>(base) + static_cast<std::uintptr_t>(offset);
		}

		AddressLibrary GlobalAddressLibrary;

		AddressLibrary* AddressLibrary::GetSingleton() noexcept(true)
		{
			return &GlobalAddressLibrary;
		}
	}
}
