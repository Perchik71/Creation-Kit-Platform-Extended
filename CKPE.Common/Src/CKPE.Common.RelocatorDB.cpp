// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <mmsystem.h>

#include <CKPE.Common.Interface.h>

#include <CKPE.Common.RelocatorDB.h>
#include <CKPE.Common.CreatePatterns.h>
#include <CKPE.StringUtils.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Zipper.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Exception.h>

#include <algorithm>

namespace CKPE
{
	namespace Common
	{
		constexpr static std::int32_t NO_ERR = 0;
		constexpr static std::int32_t ERR_NO_STREAM = -1;
		constexpr static std::int32_t ERR_INCORRECT_FNAME = -2;
		constexpr static std::int32_t ERR_NOACCESS_FILE = -3;
		constexpr static std::int32_t ERR_STDIO_FAILED = -4;
		constexpr static std::int32_t ERR_OUT_OF_MEMORY = -5;
		constexpr static std::int32_t ERR_FILE_NO_DATABASE = -6;
		constexpr static std::int32_t ERR_DATABASE_VERSION_NO_SUPPORTED = -7;
		constexpr static std::int32_t ERR_DATABASE_NO_INFO = -8;
		constexpr static std::int32_t ERR_INCORRECT_CHUNK_SIZE = -9;
		constexpr static std::int32_t ERR_FILE_ID_CORRUPTED = -10;
		constexpr static std::int32_t ERR_PATCH_NO_INFO = -11;
		constexpr static std::int32_t ERR_INDEX_OUT_OF_RANGE = -12;
		constexpr static std::int32_t ERR_INVALID_ARGUMENTS = -13;
		constexpr static std::int32_t ERR_NO_FOUND = -14;
		constexpr static std::int32_t ERR_PATCH_NAME_ALREADY_EXISTS = -15;
		constexpr static std::int32_t ERR_RVA_IN_PATCH_ALREADY_EXISTS = -16;

		constexpr static std::uint32_t RELOCATION_DB_CHUNK_ID = MAKEFOURCC('R', 'E', 'L', 'B');
		constexpr static std::uint32_t RELOCATION_DB_CHUNK_VERSION = 1;
		constexpr static std::uint32_t RELOCATION_DB_DATA_CHUNK_ID = MAKEFOURCC('R', 'E', 'L', 'D');
		constexpr static std::uint32_t RELOCATION_DB_DATA_CHUNK_VERSION = 1;
		constexpr static std::uint32_t RELOCATION_DB_ITEM_CHUNK_ID = MAKEFOURCC('R', 'L', 'B', 'I');
		constexpr static std::uint32_t RELOCATION_DB_ITEM_CHUNK_VERSION = 1;
		constexpr static std::uint32_t RELOCATION_DB_ITEM_DATA_CHUNK_ID = MAKEFOURCC('R', 'L', 'B', 'D');
		constexpr static std::uint32_t RELOCATION_DB_ITEM_DATA_CHUNK_VERSION = 2;

		constexpr static const char* EXTENDED_FORMAT = "extended";

		struct RelocatorDB_Chunk
		{
			std::uint32_t Id;
			std::uint32_t Version;
			std::uint32_t Size;
			std::uint32_t Reserved;
		};

		static std::string RELDB__ErrorToText(std::int32_t err) noexcept(true)
		{
			switch (err)
			{
			case NO_ERR:
				return "Ok";
			case ERR_NO_STREAM:
				return "Stream is nullptr";
			case ERR_INCORRECT_FNAME:
				return "Incorrect file name";
			case ERR_NOACCESS_FILE:
				return "Permission denied";
			case ERR_STDIO_FAILED:
				return "Error I/O";
			case ERR_OUT_OF_MEMORY:
				return "Out of memory";
			case ERR_FILE_NO_DATABASE:
				return "File is not a database";
			case ERR_DATABASE_VERSION_NO_SUPPORTED:
				return "The file version is not supported";
			case ERR_DATABASE_NO_INFO:
				return "There is no information in the database section";
			case ERR_INCORRECT_CHUNK_SIZE:
				return "Incorrect chunk size";
			case ERR_FILE_ID_CORRUPTED:
				return "File is corrupted";
			case ERR_PATCH_NO_INFO:
				return "There is no information in the patch section";
			case ERR_INDEX_OUT_OF_RANGE:
				return "Index is out of range";
			case ERR_INVALID_ARGUMENTS:
				return "The arguments of the function are set incorrectly";
			case ERR_NO_FOUND:
				return "Not found";
			case ERR_PATCH_NAME_ALREADY_EXISTS:
				return "The patch with that name already exists";
			case ERR_RVA_IN_PATCH_ALREADY_EXISTS:
				return "The specified offset is already available";
			default:
				return "Unknown error";
			}
		}

		std::int32_t RelocatorDB::PatchDB::OpenStream(Stream& stream) noexcept(true)
		{
			RelocatorDB_Chunk Chunk;
			if (stream.Read(&Chunk, sizeof(RelocatorDB_Chunk)) != (std::uint32_t)sizeof(RelocatorDB_Chunk))
				return ERR_STDIO_FAILED;

			if (Chunk.Id != RELOCATION_DB_ITEM_CHUNK_ID)
				return ERR_FILE_ID_CORRUPTED;

			if (Chunk.Version != RELOCATION_DB_ITEM_CHUNK_VERSION)
				return ERR_DATABASE_VERSION_NO_SUPPORTED;

			if (!Chunk.Size)
				return ERR_PATCH_NO_INFO;

			// Версия патча
			if (stream.Read(&_version, sizeof(std::uint32_t)) != (std::uint32_t)sizeof(std::uint32_t))
				return ERR_STDIO_FAILED;

			// Имя патча
			std::uint16_t nLen;
			if (stream.Read(&nLen, sizeof(std::uint16_t)) != (std::uint32_t)sizeof(std::uint16_t))
				return ERR_STDIO_FAILED;

			if (nLen)
			{
				_name->resize((std::size_t)nLen);
				if (stream.Read(_name->data(), nLen) != nLen)
					return ERR_STDIO_FAILED;
			}

			StringUtils::Trim(*_name);

			// Чтение данных
			if (stream.Read(&Chunk, sizeof(RelocatorDB_Chunk)) != (std::uint32_t)sizeof(RelocatorDB_Chunk))
				return ERR_STDIO_FAILED;

			if (Chunk.Id != RELOCATION_DB_ITEM_DATA_CHUNK_ID)
				return ERR_PATCH_NO_INFO;

			if (Chunk.Version == 1)
			{
				auto Count = Chunk.Size >> 2;
				if (Count)
				{
					auto Pattern = std::make_unique<std::uint32_t[]>(Count);
					if (!Pattern)
						return ERR_OUT_OF_MEMORY;
					
					_entries->resize(Count);
					if (_entries->empty())
						return ERR_OUT_OF_MEMORY;

					if (stream.Read(Pattern.get(), Chunk.Size) != Chunk.Size)
					{
						Clear();

						return ERR_STDIO_FAILED;
					}

					for (std::uint32_t i = 0; i < Count; i++)
					{
						auto& entry = _entries->at(i);
						entry.Rva = Pattern.get()[i];
						entry.Mask = new std::string;
					}
				}
			}
			else
			{
				if (Chunk.Version != RELOCATION_DB_ITEM_DATA_CHUNK_VERSION)
					return ERR_DATABASE_VERSION_NO_SUPPORTED;

				std::uint32_t Count;
				if (stream.Read(&Count, sizeof(std::uint32_t)) != (std::uint32_t)sizeof(std::uint32_t))
					return ERR_STDIO_FAILED;

				if (Count)
				{
					_entries->resize(Count);
					if (_entries->empty())
						return ERR_OUT_OF_MEMORY;

					for (std::uint32_t i = 0; i < Count; i++)
					{
						auto& entry = _entries->at(i);
						entry.Mask = new std::string;

						if (stream.Read(&entry.Rva, sizeof(std::uint32_t)) != (std::uint32_t)sizeof(std::uint32_t))
						{
							Clear();

							return ERR_STDIO_FAILED;
						}

						std::uint16_t nLen;
						if (stream.Read(&nLen, sizeof(std::uint16_t)) != (std::uint32_t)sizeof(std::uint16_t))
							return ERR_STDIO_FAILED;

						if (nLen)
						{
							entry.Mask->resize((std::size_t)nLen);
							if (stream.Read(entry.Mask->data(), nLen) != (std::uint32_t)nLen)
							{
								Clear();

								return ERR_STDIO_FAILED;
							}
						}
					}
				}
			}

			return NO_ERR;
		}

		std::int32_t RelocatorDB::PatchDB::SaveStream(Stream& stream) const noexcept(true)
		{
			auto pos_safe = stream.GetPosition();

			RelocatorDB_Chunk Chunk = {
				RELOCATION_DB_ITEM_CHUNK_ID,
				RELOCATION_DB_ITEM_CHUNK_VERSION,
				0,
				0
			};

			if (stream.Write(&Chunk, sizeof(Chunk)) != (std::uint32_t)sizeof(Chunk))
				return ERR_STDIO_FAILED;

			// Версия патча
			if (stream.Write(&_version, sizeof(std::uint32_t)) != (std::uint32_t)sizeof(std::uint32_t))
				return ERR_STDIO_FAILED;

			// Имя патча
			std::uint16_t nLen = (std::uint16_t)_name->length();
			if (stream.Write(&nLen, sizeof(std::uint16_t)) != (std::uint32_t)sizeof(std::uint16_t))
				return ERR_STDIO_FAILED;

			if (stream.Write(_name->c_str(), nLen) != nLen)
				return ERR_STDIO_FAILED;

			auto pos_safe_2 = stream.GetPosition();

			Chunk.Id = RELOCATION_DB_ITEM_DATA_CHUNK_ID;
			Chunk.Version = RELOCATION_DB_ITEM_DATA_CHUNK_VERSION;
			Chunk.Size = 0;

			if (stream.Write(&Chunk, sizeof(Chunk)) != (std::uint32_t)sizeof(Chunk))
				return ERR_STDIO_FAILED;

			std::uint32_t nSize = (std::uint32_t)_entries->size();
			if (stream.Write(&nSize, sizeof(std::uint32_t)) != (std::uint32_t)sizeof(std::uint32_t))
				return ERR_STDIO_FAILED;

			// Запись данных
			for (auto& entry : *_entries)
			{
				if (stream.Write(&entry.Rva, sizeof(std::uint32_t)) != (std::uint32_t)sizeof(std::uint32_t))
					return ERR_STDIO_FAILED;

				if (entry.Mask && !entry.Mask->empty())
				{
					std::uint16_t nLen = (std::uint16_t)entry.Mask->length();
					if (stream.Write(&nLen, sizeof(std::uint16_t)) != (std::uint32_t)sizeof(std::uint16_t))
						return ERR_STDIO_FAILED;

					if (stream.Write(entry.Mask->c_str(), nLen) != (std::uint32_t)nLen)
						return ERR_STDIO_FAILED;
				}
				else
				{
					std::uint16_t nData = (std::uint16_t)0;
					if (stream.Write(&nData, sizeof(std::uint16_t)) != (std::uint32_t)sizeof(std::uint16_t))
						return ERR_STDIO_FAILED;
				}
			}

			auto pos_end = stream.GetPosition();

			stream.SetPosition(pos_safe);

			Chunk.Id = RELOCATION_DB_ITEM_CHUNK_ID;
			Chunk.Version = RELOCATION_DB_ITEM_CHUNK_VERSION;
			Chunk.Size = pos_end - pos_safe;

			if (stream.Write(&Chunk, sizeof(Chunk)) != (std::uint32_t)sizeof(Chunk))
				return ERR_STDIO_FAILED;

			stream.SetPosition(pos_safe_2);

			Chunk.Id = RELOCATION_DB_ITEM_DATA_CHUNK_ID;
			Chunk.Version = RELOCATION_DB_ITEM_DATA_CHUNK_VERSION;
			Chunk.Size = pos_end - pos_safe_2;

			if (stream.Write(&Chunk, sizeof(Chunk)) != (std::uint32_t)sizeof(Chunk))
				return ERR_STDIO_FAILED;

			stream.SetPosition(pos_end);

			return NO_ERR;
		}

		std::int32_t RelocatorDB::PatchDB::OpenDevStream(TextFileStream& stream) noexcept(true)
		{
			auto line = std::make_unique<char[]>(1025);
			if (!line)
				return ERR_OUT_OF_MEMORY;

			line.get()[1024] = 0;
			if (!stream.ReadLine(line.get(), 1024))
				return ERR_STDIO_FAILED;

			*_name = StringUtils::Trim(line.get());

			if (!stream.ReadLine(line.get(), 1024))
				return ERR_STDIO_FAILED;

			char* EndPtr = nullptr;
			_version = strtoul(line.get(), &EndPtr, 10);

			auto pos_safe = stream.GetPosition();
			if (!stream.ReadLine(line.get(), 1024))
				return ERR_STDIO_FAILED;

			if (!_stricmp(StringUtils::Trim(line.get()).c_str(), EXTENDED_FORMAT))
			{
				uint32_t rva;
				auto pattern = std::make_unique<char[]>(256);
				uint32_t pattern_len;

				while (!stream.Eof())
				{
					if (!stream.ReadLine(line.get(), 1024))
						return ERR_STDIO_FAILED;

					auto Row = StringUtils::Trim(line.get());
					if (Row.length() < 2) break;

					if (!Row.empty() && (sscanf(Row.c_str(), "%X %u %s", &rva, &pattern_len, pattern.get()) == 3))
					{
						EntryDB entry{
							rva,
							new std::string,
						};

						if (entry.Mask)
							*entry.Mask = pattern.get();
						
						_entries->push_back(entry);
					}
				}
			}
			else
			{
				stream.SetPosition(pos_safe);

				uint32_t rva;
				while (!stream.Eof())
				{
					if (!stream.ReadLine(line.get(), 1024))
						return ERR_STDIO_FAILED;

					auto Row = StringUtils::Trim(line.get());
					if (!Row.empty() && (sscanf(Row.c_str(), "%X", &rva) == 1))
					{
						EntryDB entry{
							rva,
							new std::string,
						};

						_entries->push_back(entry);
					}
				}
			}

			return NO_ERR;
		}

		std::int32_t RelocatorDB::PatchDB::SaveDevStream(TextFileStream& stream, bool regen_sign) const noexcept(true)
		{
			// Запись имени, версии и формата патча
			stream.WriteLine("%s\n%u\n%s", _name->c_str(), _version, EXTENDED_FORMAT);

			size_t c = 0;
			// Запись данных
			for (auto& entry : *_entries)
			{
				if (regen_sign)
				{
					if (entry.Rva)
						*entry.Mask = ZydisCreateMask((std::uintptr_t)GetModuleHandleA(nullptr) + entry.Rva, 64, Patterns::XDBG64_MASK);
					else
						entry.Mask->clear();
				}

				auto l = entry.Mask->length();
				if ((c + 1) < _entries->size())
					stream.WriteLine("%X %u %s", entry.Rva, l, (l < 1) ? "<nope>" : entry.Mask->c_str());
				else
					stream.WriteString("%X %u %s", entry.Rva, l, (l < 1) ? "<nope>" : entry.Mask->c_str());
			}

			return NO_ERR;
		}

		RelocatorDB::PatchDB::PatchDB() noexcept(true) :
			_name(new std::string), _entries(new std::vector<EntryDB>)
		{}

		RelocatorDB::PatchDB::~PatchDB() noexcept(true)
		{
			Clear();

			if (_name)
			{
				delete _name;
				_name = nullptr;
			}

			if (_entries)
			{
				delete _entries;
				_entries = nullptr;
			}
		}

		bool RelocatorDB::PatchDB::LoadFromStream(Stream& stream) noexcept(true)
		{
			ScopeCriticalSection lock(_locker);

			auto err = OpenStream(stream);
			if (err != NO_ERR)
			{
				_ERROR_EX("RelocatorDB::PatchDB::LoadFromStream returned failed \"{}\"", RELDB__ErrorToText(err));

				return false;
			}

			return true;
		}

		bool RelocatorDB::PatchDB::SaveToStream(Stream& stream) const noexcept(true)
		{
			ScopeCriticalSection lock(_locker);

			auto err = SaveStream(stream);
			if (err != NO_ERR)
			{
				_ERROR_EX("RelocatorDB::PatchDB::SaveToStream returned failed \"{}\"", RELDB__ErrorToText(err));

				return false;
			}

			return true;
		}

		bool RelocatorDB::PatchDB::LoadDevFromFile(const std::string& fname) noexcept(true)
		{
			ScopeCriticalSection lock(_locker);

			TextFileStream fstm(fname, FileStream::fmOpenRead);
			auto err = OpenDevStream(fstm);
			if (err != NO_ERR)
			{
				_ERROR_EX("RelocatorDB::PatchDB::LoadDevFromFile returned failed \"{}\"", RELDB__ErrorToText(err));

				return false;
			}

			return true;
		}

		bool RelocatorDB::PatchDB::LoadDevFromFile(const std::wstring& fname) noexcept(true)
		{
			ScopeCriticalSection lock(_locker);

			TextFileStream fstm(fname, FileStream::fmOpenRead);
			auto err = OpenDevStream(fstm);
			if (err != NO_ERR)
			{
				_ERROR_EX("RelocatorDB::PatchDB::LoadDevFromFile returned failed \"{}\"", RELDB__ErrorToText(err));

				return false;
			}

			return true;
		}

		bool RelocatorDB::PatchDB::SaveDevToFile(const std::string& fname, bool regen_sign) const noexcept(true)
		{
			ScopeCriticalSection lock(_locker);

			TextFileStream fstm(fname, FileStream::fmCreate);
			auto err = SaveDevStream(fstm, regen_sign);
			if (err != NO_ERR)
			{
				_ERROR_EX("RelocatorDB::PatchDB::SaveDevToFile returned failed \"{}\"", RELDB__ErrorToText(err));

				return false;
			}

			return true;
		}

		bool RelocatorDB::PatchDB::SaveDevToFile(const std::wstring& fname, bool regen_sign) const noexcept(true)
		{
			ScopeCriticalSection lock(_locker);

			TextFileStream fstm(fname, FileStream::fmCreate);
			auto err = SaveDevStream(fstm, regen_sign);
			if (err != NO_ERR)
			{
				_ERROR_EX("RelocatorDB::PatchDB::SaveDevToFile returned failed \"{}\"", RELDB__ErrorToText(err));

				return false;
			}

			return true;
		}

		std::string RelocatorDB::PatchDB::GetName() const noexcept(true)
		{
			return _name ? *_name : "";
		}

		std::uint32_t RelocatorDB::PatchDB::GetVersion() const noexcept(true)
		{
			return _version;
		}

		RelocatorDB::PatchDB::EntryDB RelocatorDB::PatchDB::GetAt(std::uint32_t id) const noexcept(true)
		{
			RelocatorDB::PatchDB::EntryDB entry{0};
			if (_entries && (id < _entries->size()))
				entry = _entries->at(id);
			return entry;
		}

		void RelocatorDB::PatchDB::Append(const EntryDB& name) noexcept(true)
		{
			ScopeCriticalSection lock(_locker);

			if (_entries)
				_entries->push_back(name);
		}

		void RelocatorDB::PatchDB::Insert(std::uint32_t id, const EntryDB& name) noexcept(true)
		{
			ScopeCriticalSection lock(_locker);

			if (_entries)
			{
				if (id < _entries->size())
				{
					auto it = _entries->begin();
					std::advance(it, id);
					_entries->insert(it, name);
				}
				else
					_entries->push_back(name);
			}
		}

		void RelocatorDB::PatchDB::Clear() noexcept(true)
		{
			ScopeCriticalSection lock(_locker);

			if (_entries)
			{
				for (auto& entry : *_entries)
					if (entry.Mask)
						delete entry.Mask;
				_entries->clear();
			}
		}

		std::uint32_t RelocatorDB::PatchDB::GetCount() const noexcept(true)
		{
			return _entries ? (std::uint32_t)_entries->size() : 0;
		}

		std::int32_t RelocatorDB::OpenStream(Stream& stream) noexcept(true)
		{
			if (!_db)
				return ERR_OUT_OF_MEMORY;

			Clear();

			RelocatorDB_Chunk Chunk;
			if (stream.Read(&Chunk, sizeof(RelocatorDB_Chunk)) != (std::uint32_t)sizeof(RelocatorDB_Chunk))
				return ERR_STDIO_FAILED;

			if (Chunk.Id != RELOCATION_DB_CHUNK_ID)
				return ERR_FILE_NO_DATABASE;

			if (Chunk.Version != RELOCATION_DB_CHUNK_VERSION)
				return ERR_DATABASE_VERSION_NO_SUPPORTED;

			if (!Chunk.Size)
				return ERR_DATABASE_NO_INFO;

			if (stream.Read(&Chunk, sizeof(RelocatorDB_Chunk)) != (std::uint32_t)sizeof(RelocatorDB_Chunk))
				return ERR_STDIO_FAILED;

			if (Chunk.Id != RELOCATION_DB_DATA_CHUNK_ID)
				return ERR_DATABASE_NO_INFO;

			if (Chunk.Version != RELOCATION_DB_DATA_CHUNK_VERSION)
				return ERR_DATABASE_VERSION_NO_SUPPORTED;

			if (Chunk.Size < sizeof(std::uint32_t))
				return ERR_INCORRECT_CHUNK_SIZE;

			// Кол-во патчей в базе
			std::uint32_t count;
			if (stream.Read(&count, sizeof(std::uint32_t)) != (std::uint32_t)sizeof(std::uint32_t))
				return ERR_STDIO_FAILED;

			for (std::uint32_t nId = 0; nId < count; nId++)
			{
				PatchDB* patch = new PatchDB;
				if (patch && patch->LoadFromStream(stream) && !Add(patch))
					return ERR_PATCH_NAME_ALREADY_EXISTS;
			}

			return NO_ERR;
		}

		std::int32_t RelocatorDB::SaveStream(Stream& stream) const noexcept(true)
		{
			if (!_db)
				return ERR_OUT_OF_MEMORY;

			RelocatorDB_Chunk Chunk = {
				RELOCATION_DB_CHUNK_ID,
				RELOCATION_DB_CHUNK_VERSION,
				sizeof(RelocatorDB_Chunk) + sizeof(std::uint32_t),
				0
			};

			if (stream.Write(&Chunk, sizeof(RelocatorDB_Chunk)) != (std::uint32_t)sizeof(RelocatorDB_Chunk))
				return ERR_STDIO_FAILED;

			Chunk.Id = RELOCATION_DB_DATA_CHUNK_ID;
			Chunk.Version = RELOCATION_DB_DATA_CHUNK_VERSION;
			Chunk.Size = sizeof(std::uint32_t);

			if (stream.Write(&Chunk, sizeof(RelocatorDB_Chunk)) != (std::uint32_t)sizeof(RelocatorDB_Chunk))
				return ERR_STDIO_FAILED;

			std::uint32_t nSize = (std::uint32_t)_db->size();
			if (stream.Write(&nSize, sizeof(std::uint32_t)) != (std::uint32_t)sizeof(std::uint32_t))
				return ERR_STDIO_FAILED;

			for (auto& patches : *_db)
				if (patches.second && !patches.second->SaveToStream(stream))
					return ERR_STDIO_FAILED;

			return NO_ERR;
		}

		RelocatorDB::RelocatorDB() noexcept(true) :
			_db(new std::map<std::string, PatchDB*>)
		{}

		RelocatorDB::~RelocatorDB() noexcept(true)
		{
			Clear();

			if (_db)
			{
				delete _db;
				_db = nullptr;
			}
		}

		bool RelocatorDB::LoadFromStream(Stream& stream) noexcept(true)
		{
			auto err = OpenStream(stream);
			if (err != NO_ERR)
			{
				_ERROR_EX("RelocatorDB::LoadFromStream returned failed \"{}\"", RELDB__ErrorToText(err));

				return false;
			}

			return true;
		}

		bool RelocatorDB::SaveToStream(Stream& stream) const noexcept(true)
		{
			auto err = SaveStream(stream);
			if (err != NO_ERR)
			{
				_ERROR_EX("RelocatorDB::SaveToStream returned failed \"{}\"", RELDB__ErrorToText(err));

				return false;
			}

			return true;
		}

		RelocatorDB::PatchDB* RelocatorDB::At(const std::string& name) noexcept(true)
		{
			if (name.empty() || !name.length())
				return nullptr;

			auto it = _db->find(StringUtils::ToLowerUTF8(name));
			return it == _db->end() ? nullptr : it->second;
		}

		RelocatorDB::PatchDB* RelocatorDB::AtByIndex(const std::uint32_t id) noexcept(true)
		{
			auto it = _db->begin();
			std::advance(it, id);
			return it == _db->end() ? nullptr : it->second;
		}

		bool RelocatorDB::Add(PatchDB* patch) noexcept(true)
		{
			if (!patch)
				return false;

			ScopeCriticalSection lock(_locker);

			if (At(patch->GetName()))
			{
				_ERROR_EX("RelocatorDB::Add patch this name is exist \"{}\"", patch->GetName());

				return false;
			}

			_db->insert({ StringUtils::ToLowerUTF8(patch->GetName()), patch });

			return true;
		}

		bool RelocatorDB::Delete(const std::string& name) noexcept(true)
		{
			if (!_db)
				return false;

			ScopeCriticalSection lock(_locker);

			auto it = _db->find(StringUtils::ToLowerUTF8(name));
			if (it == _db->end())
				return false;

			if (it->second)
				delete it->second;

			_db->erase(it);

			return true;
		}

		bool RelocatorDB::DeleteByIndex(const std::uint32_t id) noexcept(true)
		{
			if (!_db)
				return false;

			ScopeCriticalSection lock(_locker);

			auto it = _db->begin();
			std::advance(it, id);

			if (it == _db->end())
				return false;

			if (it->second)
				delete it->second;

			_db->erase(it);

			return true;
		}

		std::uint32_t RelocatorDB::GetCount() const noexcept(true)
		{
			return _db ? (std::uint32_t)_db->size() : 0;
		}

		void RelocatorDB::Clear() noexcept(true)
		{
			ScopeCriticalSection lock(_locker);

			if (_db)
				for (auto& it : *_db)
					if (it.second)
					{
						delete it.second;
						it.second = nullptr;
					}
		}
	}
}