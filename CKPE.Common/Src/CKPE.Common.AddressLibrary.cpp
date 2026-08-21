// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Application.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Stream.h>
#include <CKPE.Exception.h>
#include <algorithm>

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

		bool AddressLibrary::Load(const std::wstring& fname) noexcept(true)
		{
			Clear();

			try
			{
				if (!PathUtils::FileExists(fname))
					throw RuntimeError(L"AddressLibrary::Load file \"{}\" no found", fname);

				FileStream stm(fname, FileStream::fmOpenRead);

				std::uint64_t count = 0;
				if (stm.Read(&count, sizeof(count)) != sizeof(count))
					throw RuntimeError(L"AddressLibrary::Load file \"{}\" is broken (couldn't read header)", fname);

				auto expected_size = static_cast<std::uint64_t>(sizeof(count) + count * sizeof(Entry));
				if (stm.GetSize() != expected_size)
					throw RuntimeError(L"AddressLibrary::Load file \"{}\" has an unexpected size", fname);

				_entries->resize((std::size_t)count);

				if (count)
				{
					auto bytes_to_read = static_cast<std::uint32_t>(count * sizeof(Entry));
					if (stm.Read(_entries->data(), bytes_to_read) != bytes_to_read)
						throw RuntimeError(L"AddressLibrary::Load file \"{}\" is broken (short read)", fname);
				}

				for (std::size_t i = 1; i < _entries->size(); i++)
				{
					if ((*_entries)[i].Id <= (*_entries)[i - 1].Id)
						throw RuntimeError(L"AddressLibrary::Load file \"{}\" entries aren't sorted/unique by id", fname);
				}

				_loaded = true;
				_MESSAGE(L"\tAddress Library \"%s\" loaded (%u entries)"sv, fname.c_str(), static_cast<std::uint32_t>(_entries->size()));

				return true;
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
