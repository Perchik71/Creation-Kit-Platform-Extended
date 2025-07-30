// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.Relocator.h>
#include <CKPE.StringUtils.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Zipper.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Exception.h>

#include <CKPE.MessageBox.h>

namespace CKPE
{
	namespace Common
	{
		Relocator GlobalRelocator;

		Relocator::Relocator() noexcept(true) :
			_db(new RelocatorDB)
		{}

		Relocator::~Relocator() noexcept(true)
		{
			if (_db)
			{
				delete _db;
				_db = nullptr;
			}
		}

		RelocatorDB* Relocator::GetDB() noexcept(true)
		{
			return _db;
		}

		bool Relocator::Open(const std::wstring& fname_pak, const std::wstring& fname_db) noexcept(true)
		{
			if (!_db)
				return false;

			try
			{
				if (!PathUtils::FileExists(fname_pak))
					throw RuntimeError(L"Relocator::Open file \"{}\" no found", fname_pak);

				UnZipper zip(fname_pak);
				if (!zip.HasOpen())
					throw RuntimeError(L"Relocator::Open file \"{}\" can't opened", fname_pak);

				auto db_name = StringUtils::Utf16ToWinCP(fname_db);

				for (std::uint32_t i = 0; i < zip.GetEntries()->Count(); i++)
				{
					auto entry = zip.GetEntries()->At(i);
					if (entry.Empty() || !entry->Get()) continue;

					auto sname = entry->Get()->GetName();
					if (!_stricmp(sname.c_str(), db_name.c_str()))
					{
						MemoryStream mstm;
						if (!entry->Get()->ReadToStream(mstm))
							throw RuntimeError(L"Relocator::Open file \"{}\" in \"{}\" is broken", fname_db, fname_pak);

						// sets begin
						mstm.SetPosition(0);
						// read stream
						bool result = _db->LoadFromStream(mstm);
#if 0
						if (result)
						{
							_MESSAGE(L"The database file opened \"%s\"\nTotal patches: %i",
								fname_db.c_str(), _db->GetCount());
							for (std::uint32_t i = 0; i < _db->GetCount(); i++)
							{
								auto patch = _db->AtByIndex(i);
								if (!patch) break;

								_MESSAGE("\t[%u] \"%s\"", i, patch->GetName().c_str());
							}
						}
#endif

						return result;
					}
				}

				throw RuntimeError(L"Relocator::Open file \"{}\" in \"{}\" no found", fname_db, fname_pak);
			}
			catch (const std::exception& e)
			{
				_ERROR(e.what());

				return false;
			}

			return false;
		}

		bool Relocator::Save(const std::wstring& fname_pak, const std::wstring& fname_db) noexcept(true)
		{
			try
			{
				MemoryStream mstm;
				_db->SaveToStream(mstm);
				mstm.SetPosition(0);

				Zipper zip(fname_pak);
				return zip.ZipFileAsStream(fname_db, mstm);
			}
			catch (const std::exception& e)
			{
				_ERROR(e.what());

				return false;
			}

			return false;
		}

		const RelocatorDB::PatchDB* Relocator::GetAtConst(std::uint32_t id) const noexcept(true)
		{
			if (!_db || (_db->GetCount() >= id)) return nullptr;
			return _db->AtByIndex(id);
		}

		const RelocatorDB::PatchDB* Relocator::GetByNameConst(const std::string& name) const noexcept(true)
		{
			if (!_db) return nullptr;
			return _db->At(name);
		}

		RelocatorDB::PatchDB* Relocator::GetAt(std::uint32_t id) noexcept(true)
		{
			if (!_db || (_db->GetCount() >= id)) return nullptr;
			return _db->AtByIndex(id);
		}

		RelocatorDB::PatchDB* Relocator::GetByName(const std::string& name) noexcept(true)
		{
			if (!_db) return nullptr;
			return _db->At(name);
		}

		bool Relocator::Add(RelocatorDB::PatchDB* patch) noexcept(true)
		{
			if (!_db) return false;
			return _db->Add(patch);
		}

		bool Relocator::Delete(const std::string& name) noexcept(true)
		{
			if (!_db) return false;
			return _db->Delete(name);
		}

		bool Relocator::DeleteByIndex(const std::uint32_t id) noexcept(true)
		{
			if (!_db) return false;
			return _db->DeleteByIndex(id);
		}

		void Relocator::Clear() noexcept(true)
		{
			if (_db) _db->Clear();
		}

		std::uint32_t Relocator::GetCount() const noexcept(true)
		{
			return _db ? _db->GetCount() : 0;
		}

		Relocator* Relocator::GetSingleton() noexcept(true)
		{
			return &GlobalRelocator;
		}
	}
}