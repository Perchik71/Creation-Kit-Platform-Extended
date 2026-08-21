// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Application.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Zipper.h>
#include <CKPE.Stream.h>
#include <CKPE.Exception.h>
#include <algorithm>
#include <format>

using namespace std::literals;

namespace CKPE
{
	namespace Common
	{
		AddressLibrary GlobalAddressLibrary;

		AddressLibrary::AddressLibrary() noexcept(true) :
			_entries(new std::vector<Entry>)
		{}

		AddressLibrary::~AddressLibrary() noexcept(true)
		{
			if (_entries)
			{
				delete _entries;
				_entries = nullptr;
			}
		}

		void AddressLibrary::Clear() noexcept(true)
		{
			if (_entries)
				_entries->clear();
			_loaded = false;
		}

		bool AddressLibrary::Load(const std::wstring& fname_pak) noexcept(true)
		{
			Clear();

			try
			{
				if (!PathUtils::FileExists(fname_pak))
					throw RuntimeError(L"AddressLibrary::Load file \"{}\" no found", fname_pak);

				auto ver = Application::GetSingleton()->GetFileVersion();
				if (!ver.has_value())
					throw RuntimeError(L"AddressLibrary::Load get version game failed");

				_version = ver.value();
				const auto db_name = std::format("version-{}.bin", _version.string("-"));

				UnZipper zip(fname_pak);
				if (!zip.HasOpen())
					throw RuntimeError(L"AddressLibrary::Load file \"{}\" can't opened", fname_pak);

				for (std::uint32_t i = 0; i < zip.GetEntries()->Count(); i++)
				{
					auto entry = zip.GetEntries()->At(i);
					if (entry.Empty() || !entry->Get()) continue;

					auto sname = entry->Get()->GetName();
					if (!_stricmp(sname.c_str(), db_name.c_str()))
					{
						MemoryStream mstm;
						if (!entry->Get()->ReadToStream(mstm))
							throw RuntimeError(L"Relocator::Open file \"{}\" in \"{}\" is broken", StringUtils::WinCPToUtf16(db_name), fname_pak);
							
						// sets begin
						mstm.SetPosition(0);

						std::uint64_t count = 0;
						if (mstm.Read(&count, sizeof(count)) != sizeof(count))
							throw RuntimeError("AddressLibrary::Load file \"{}\" is broken (couldn't read header)", sname);

						auto expected_size = static_cast<std::uint64_t>(sizeof(count) + count * sizeof(Entry));
						// 0 bytes end so -1
						if ((mstm.GetSize() - 1) != expected_size)
							throw RuntimeError("AddressLibrary::Load file \"{}\" has an unexpected size ({}/{})", sname, mstm.GetSize(), expected_size);

						_entries->resize((std::size_t)count);

						if (count)
						{
							auto bytes_to_read = static_cast<std::uint32_t>(count * sizeof(Entry));
							if (mstm.Read(_entries->data(), bytes_to_read) != bytes_to_read)
								throw RuntimeError("AddressLibrary::Load file \"{}\" is broken (short read)", sname);
						}

						for (std::size_t i = 1; i < _entries->size(); i++)
							if ((*_entries)[i].Id <= (*_entries)[i - 1].Id)
								throw RuntimeError("AddressLibrary::Load file \"{}\" entries aren't sorted/unique by id", sname);

						_loaded = true;
					}
				}
				
				if (_loaded)
					_MESSAGE("Address Library \"%s\" loaded (%u entries)"sv, db_name.c_str(), static_cast<std::uint32_t>(_entries->size()));
				else
					_ERROR("Address Library \"%s\" file no found."sv, db_name.c_str());

				return _loaded;
			}
			catch (const std::exception& e)
			{
				_ERROR(e.what());

				Clear();
				return false;
			}
		}

		std::uint32_t AddressLibrary::GetCount() const noexcept(true)
		{
			return _entries ? (std::uint32_t)_entries->size() : 0;
		}

		std::uint64_t AddressLibrary::ResolveOffset(AddressID id) const noexcept(true)
		{
			if (!_loaded || !_entries || _entries->empty())
				return 0;

			auto it = std::lower_bound(_entries->begin(), _entries->end(), id,
				[](const Entry& e, AddressID value) noexcept(true) { return e.Id < value; });

			if (it == _entries->end() || it->Id != id)
				return 0;

			return it->Offset;
		}

		std::uintptr_t AddressLibrary::Resolve(AddressID id) const noexcept(true)
		{
			auto offset = ResolveOffset(id);
			if (!offset)
				return 0;

			auto base = Interface::GetSingleton()->GetApplication()->GetBase();
			return (std::uintptr_t)base + (std::uintptr_t)offset;
		}

		void AddressLibrary::SetVersion(const CKPE::Version& version) noexcept(true)
		{
			_version = version;
		}

		CKPE::Version AddressLibrary::GetVersion() const noexcept(true)
		{
			return _version;
		}

		std::uint64_t AddressLibrary::ResolveOffset(const VariantID& id) const noexcept(true)
		{
			const auto variant = id.Get(_version);

			const auto offset = ResolveOffset(variant.ID);
			if (!offset)
				return 0;

			return offset + variant.Offset;
		}

		std::uintptr_t AddressLibrary::Resolve(const VariantID& id) const noexcept(true)
		{
			const auto offset = ResolveOffset(id);
			if (!offset)
				return 0;

			auto base = Interface::GetSingleton()->GetApplication()->GetBase();

			return static_cast<std::uintptr_t>(base) + static_cast<std::uintptr_t>(offset);
		}
			
		AddressLibrary* AddressLibrary::GetSingleton() noexcept(true)
		{
			return &GlobalAddressLibrary;
		}
	}
}
