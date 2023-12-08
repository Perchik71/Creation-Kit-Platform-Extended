// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Engine.h"
#include "RelocationDatabase.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		struct RelocationDatabaseItemFileChunk
		{
			uint32_t u32Id;
			uint32_t u32Version;
			uint32_t u32Size;
			uint32_t u32Reserved;
		};

		constexpr static uint32_t RELOCATION_DB_CHUNK_ID = MAKEFOURCC('R', 'E', 'L', 'B');
		constexpr static uint32_t RELOCATION_DB_CHUNK_VERSION = 1;
		constexpr static uint32_t RELOCATION_DB_DATA_CHUNK_ID = MAKEFOURCC('R', 'E', 'L', 'D');
		constexpr static uint32_t RELOCATION_DB_DATA_CHUNK_VERSION = 1;
		constexpr static uint32_t RELOCATION_DB_ITEM_CHUNK_ID = MAKEFOURCC('R', 'L', 'B', 'I');
		constexpr static uint32_t RELOCATION_DB_ITEM_CHUNK_VERSION = 1;
		constexpr static uint32_t RELOCATION_DB_ITEM_DATA_CHUNK_ID = MAKEFOURCC('R', 'L', 'B', 'D');
		constexpr static uint32_t RELOCATION_DB_ITEM_DATA_CHUNK_VERSION = 1;

		constexpr static const char* MESSAGE_FAILED_READ2 = "RelocationDatabase: The end of the file was reached unexpectedly";
		constexpr static const char* MESSAGE_FAILED_WRITE2 = "RelocationDatabase: Failed to write to a file";
		constexpr static const char* MESSAGE_FAILED_READ = "RelocationDatabaseItem: The end of the file was reached unexpectedly";
		constexpr static const char* MESSAGE_FAILED_WRITE = "RelocationDatabaseItem: Failed to write to a file";

		RelocationDatabaseItem::RelocationDatabaseItem() : _version(1)
		{}

		RelocationDatabaseItem::RelocationDatabaseItem(const char* lpcstrName, uint32_t nVersion, 
			std::initializer_list<uint32_t> rvaList) : _version(nVersion), _name(lpcstrName)
		{
			for (auto It = rvaList.begin(); It != rvaList.end(); It++)
				_rvaList.push_back(*It);
		}

		RelocationDatabaseItem::~RelocationDatabaseItem()
		{
			Clear();
		}

		bool RelocationDatabaseItem::LoadFromFileStream(FILE* fileStream)
		{
			_rvaList.clear();

			RelocationDatabaseItemFileChunk Chunk = { 0 };

			if (!CreationKitPlatformExtended::Utils::FileReadBuffer(fileStream, &Chunk))
			{
				_ERROR(MESSAGE_FAILED_READ);
				return false;
			}

			if (Chunk.u32Id != RELOCATION_DB_ITEM_CHUNK_ID)
			{
				_ERROR("RelocationDatabaseItem: This is not patch");
				return false;
			}

			if (Chunk.u32Version != RELOCATION_DB_ITEM_CHUNK_VERSION)
			{
				_ERROR("RelocationDatabaseItem: The patch version is not supported, please update the platform");
				return false;
			}

			if (!Chunk.u32Size)
			{
				_WARNING("RelocationDatabaseItem: There is no patch data");
				return false;
			}

			// Версия патча
			if (!CreationKitPlatformExtended::Utils::FileReadBuffer(fileStream, &_version))
			{
				_ERROR(MESSAGE_FAILED_READ);
				return false;
			}

			// Имя патча
			if (!CreationKitPlatformExtended::Utils::FileReadString(fileStream, _name))
			{
				_ERROR(MESSAGE_FAILED_READ);
				return false;
			}

			if (!CreationKitPlatformExtended::Utils::FileReadBuffer(fileStream, &Chunk))
			{
				_ERROR(MESSAGE_FAILED_READ);
				return false;
			}

			if (Chunk.u32Id != RELOCATION_DB_ITEM_DATA_CHUNK_ID)
			{
				_ERROR("RelocationDatabaseItem: This is not patch data");
				return false;
			}

			if (Chunk.u32Version != RELOCATION_DB_ITEM_CHUNK_VERSION)
			{
				_ERROR("RelocationDatabaseItem: The patch data version is not supported, please update the platform");
				return false;
			}

			// Чтение данных
			_rvaList.resize(Chunk.u32Size >> 2);	
			if (!CreationKitPlatformExtended::Utils::FileReadBuffer(fileStream, _rvaList.data(), (uint32_t)_rvaList.size()))
			{
				_ERROR(MESSAGE_FAILED_READ);
				return false;
			}

			return true;
		}

		bool RelocationDatabaseItem::SaveToFileStream(FILE* fileStream)
		{
			RelocationDatabaseItemFileChunk Chunk = { 
				RELOCATION_DB_ITEM_CHUNK_ID,
				RELOCATION_DB_ITEM_CHUNK_VERSION,
				sizeof(_version) + sizeof(uint16_t) + (uint32_t)_name.length() +
				sizeof(RelocationDatabaseItemFileChunk) + (uint32_t)(_rvaList.size() << 2),
				0 
			};

			if (!CreationKitPlatformExtended::Utils::FileWriteBuffer(fileStream, Chunk))
			{
				_ERROR(MESSAGE_FAILED_WRITE);
				return false;
			}

			// Версия патча
			if (!CreationKitPlatformExtended::Utils::FileWriteBuffer(fileStream, _version))
			{
				_ERROR(MESSAGE_FAILED_WRITE);
				return false;
			}

			// Имя патча
			if (!CreationKitPlatformExtended::Utils::FileWriteString(fileStream, _name))
			{
				_ERROR(MESSAGE_FAILED_WRITE);
				return false;
			}

			Chunk.u32Id = RELOCATION_DB_ITEM_DATA_CHUNK_ID;
			Chunk.u32Version = RELOCATION_DB_ITEM_DATA_CHUNK_VERSION;
			Chunk.u32Size = (uint32_t)(_rvaList.size() << 2);

			if (!CreationKitPlatformExtended::Utils::FileWriteBuffer(fileStream, Chunk))
			{
				_ERROR(MESSAGE_FAILED_WRITE);
				return false;
			}

			// Запись данных
			for (auto It = _rvaList.begin(); It != _rvaList.end(); It++)
			{
				if (!CreationKitPlatformExtended::Utils::FileWriteBuffer(fileStream, *It))
				{
					_ERROR(MESSAGE_FAILED_WRITE);
					return false;
				}
			}

			return true;
		}

		void RelocationDatabaseItem::Clear()
		{
			_rvaList.clear();
		}

		bool RelocationDatabaseItem::LoadFromFileDeveloped(const char* filename)
		{
			FILE* fileStream = _fsopen(filename, "rt", _SH_DENYWR);
			if (!fileStream)
			{
				_MESSAGE("\tThe file could not be opened: \"%s\"", filename);
				return false;
			}

			Clear();
			CreationKitPlatformExtended::Utils::ScopeFileStream file(fileStream);
			
			_MESSAGE("\tThe beginning of file analysis: \"%s\"", filename);

			char szBuf[256] = { 0 };
			if (!fgets(szBuf, 256, fileStream))
			{
				_ERROR(MESSAGE_FAILED_READ);
				return false;
			}

			szBuf[255] = '\0';
			_name = CreationKitPlatformExtended::Utils::Trim(szBuf);

			_MESSAGE("\tThe patch name: \"%s\"", _name.c_str());

			if (fscanf(fileStream, "%u", &_version) != 1)
			{
				_ERROR(MESSAGE_FAILED_READ);
				return false;
			}

			_MESSAGE("\tThe patch version: %u", _version);

			uint32_t rva;
			while (!feof(fileStream))
			{
				rva = 0;
				if (fscanf(fileStream, "%X", &rva) == 1)
					_rvaList.push_back(rva);
			}

			_MESSAGE("\tThe patch file has been processed: \"%s\" rva %u", filename, _rvaList.size());

			return true;
		}

		bool RelocationDatabaseItem::SaveToFileDeveloped(const char* filename)
		{
			if (!_rvaList.size())
				return false;

			FILE* fileStream = _fsopen(filename, "wt", _SH_DENYRW);
			if (!fileStream)
			{
				_MESSAGE("\tThe file could not be created: \"%s\"", filename);
				return false;
			}

			CreationKitPlatformExtended::Utils::ScopeFileStream file(fileStream);

			_MESSAGE("\tThe beginning of unpacking the patch into a file: \"%s\"", filename);

			// Запись имени патча
			fputs(_name.c_str(), fileStream);
			fputc('\n', fileStream);
			// Запись версии патча
			fprintf(fileStream, "%u\n", _version);
			// Запись данных
			for (auto It = _rvaList.begin(); It != _rvaList.end(); It++)
				fprintf(fileStream, "%X\n", *It);

			_MESSAGE("\tThe patch file has been saved: \"%s\" rva %u", filename, _rvaList.size());

			return true;
		}

		RelocationDatabase* GlobalRelocationDatabasePtr = nullptr;

		RelocationDatabase::RelocationDatabase(Engine* lpEngine) : _engine(lpEngine)
		{}

		RelocationDatabase::~RelocationDatabase()
		{
			Clear();
		}

		bool RelocationDatabase::CreateDatabase()
		{
			if (!allowedDatabaseVersion.contains(_engine->GetEditorVersion()))
			{
				_ERROR("The database name for this version of Creation Kit is not in the list");
				return false;
			}

			auto fileName = allowedDatabaseVersion.at(_engine->GetEditorVersion());
			FILE* fileStream = _fsopen(fileName.data(), "wb", _SH_DENYRW);
			if (!fileStream)
			{
				_ERROR("The database file could not be created: \"%s\"", fileName.data());
				return false;
			}

			CreationKitPlatformExtended::Utils::ScopeFileStream file(fileStream);
			RelocationDatabaseItemFileChunk Chunk = {
				RELOCATION_DB_CHUNK_ID,
				RELOCATION_DB_CHUNK_VERSION,
				sizeof(RelocationDatabaseItemFileChunk) + sizeof(uint32_t),
				0
			};

			if (!CreationKitPlatformExtended::Utils::FileWriteBuffer(fileStream, Chunk))
			{
				_ERROR(MESSAGE_FAILED_WRITE2);
				return false;
			}

			Chunk.u32Id = RELOCATION_DB_DATA_CHUNK_ID;
			Chunk.u32Version = RELOCATION_DB_DATA_CHUNK_VERSION;
			Chunk.u32Size = sizeof(uint32_t);

			if (!CreationKitPlatformExtended::Utils::FileWriteBuffer(fileStream, Chunk))
			{
				_ERROR(MESSAGE_FAILED_WRITE2);
				return false;
			}

			uint32_t nCount = (uint32_t)_patches.size();
			if (!CreationKitPlatformExtended::Utils::FileWriteBuffer(fileStream, nCount))
			{
				_ERROR(MESSAGE_FAILED_WRITE2);
				return false;
			}

			_MESSAGE("\tAn empty database has been created: \"%s\"", fileName.data());

			return true;
		}

		bool RelocationDatabase::OpenDatabase()
		{
			Clear();

			if (!allowedDatabaseVersion.contains(_engine->GetEditorVersion()))
			{
				_ERROR("The database name for this version of Creation Kit is not in the list");
				return false;
			}

			auto fileName = allowedDatabaseVersion.at(_engine->GetEditorVersion());
			FILE* fileStream = _fsopen(fileName.data(), "rb", _SH_DENYWR);
			if (!fileStream)
			{
				_ERROR("The database file could not be opened: \"%s\"", fileName.data());
				return false;
			}

			CreationKitPlatformExtended::Utils::ScopeFileStream file(fileStream);
			RelocationDatabaseItemFileChunk Chunk = { 0 };

			if (!CreationKitPlatformExtended::Utils::FileReadBuffer(fileStream, &Chunk))
			{
				_ERROR(MESSAGE_FAILED_READ2);
				return false;
			}

			if (Chunk.u32Id != RELOCATION_DB_CHUNK_ID)
			{
				_ERROR("RelocationDatabase: This is not database");
				return false;
			}

			if (Chunk.u32Version != RELOCATION_DB_CHUNK_VERSION)
			{
				_ERROR("RelocationDatabase: The database version is not supported, please update the platform");
				return false;
			}

			if (!Chunk.u32Size)
			{
				_ERROR("RelocationDatabase: There is no database data");
				return false;
			}

			if (!CreationKitPlatformExtended::Utils::FileReadBuffer(fileStream, &Chunk))
			{
				_ERROR(MESSAGE_FAILED_READ2);
				return false;
			}

			if (Chunk.u32Id != RELOCATION_DB_DATA_CHUNK_ID)
			{
				_ERROR("RelocationDatabase: This is not database data");
				return false;
			}

			if (Chunk.u32Version != RELOCATION_DB_DATA_CHUNK_VERSION)
			{
				_ERROR("RelocationDatabase: The database data version is not supported, please update the platform");
				return false;
			}

			if (Chunk.u32Size < sizeof(uint32_t))
			{
				_ERROR("RelocationDatabase: Chunk.u32Size < sizeof(uint32_t)");
				return false;
			}

			// Кол-во патчей в базе
			uint32_t nCount = 0;
			if (!CreationKitPlatformExtended::Utils::FileReadBuffer(fileStream, &nCount))
			{
				_ERROR(MESSAGE_FAILED_READ2);
				return false;
			}

			RelocationDatabaseItem* Patch;
			for (uint32_t nId = 0; nId < nCount; nId++)
			{
				Patch = new RelocationDatabaseItem();
				if (Patch && Patch->LoadFromFileStream(fileStream))
					_patches.insert(std::make_pair<String, SmartPointer<RelocationDatabaseItem>>(Patch->Name(), Patch));
				else
				{
					_ERROR("Database read error: \"%s\"", fileName.data());
					return false;
				}
			}

			return true;
		}

		bool RelocationDatabase::SaveDatabase()
		{
			if (!allowedDatabaseVersion.contains(_engine->GetEditorVersion()))
			{
				_ERROR("The database name for this version of Creation Kit is not in the list");
				return false;
			}

			auto fileName = allowedDatabaseVersion.at(_engine->GetEditorVersion());
			FILE* fileStream = _fsopen(fileName.data(), "wb", _SH_DENYRW);
			if (!fileStream)
			{
				_ERROR("The database file could not be created: \"%s\"", fileName.data());
				return false;
			}

			CreationKitPlatformExtended::Utils::ScopeFileStream file(fileStream);
			RelocationDatabaseItemFileChunk Chunk = {
				RELOCATION_DB_CHUNK_ID,
				RELOCATION_DB_CHUNK_VERSION,
				sizeof(RelocationDatabaseItemFileChunk) + sizeof(uint32_t),
				0
			};

			if (!CreationKitPlatformExtended::Utils::FileWriteBuffer(fileStream, Chunk))
			{
				_ERROR(MESSAGE_FAILED_WRITE2);
				return false;
			}

			Chunk.u32Id = RELOCATION_DB_DATA_CHUNK_ID;
			Chunk.u32Version = RELOCATION_DB_DATA_CHUNK_VERSION;
			Chunk.u32Size = sizeof(uint32_t);

			if (!CreationKitPlatformExtended::Utils::FileWriteBuffer(fileStream, Chunk))
			{
				_ERROR(MESSAGE_FAILED_WRITE2);
				return false;
			}

			uint32_t nCount = (uint32_t)_patches.size();
			if (!CreationKitPlatformExtended::Utils::FileWriteBuffer(fileStream, nCount))
			{
				_ERROR(MESSAGE_FAILED_WRITE2);
				return false;
			}

			for (auto It = _patches.begin(); It != _patches.end(); It++)
			{
				if (!It->second->SaveToFileStream(fileStream))
				{
					_ERROR("Error writing database: \"%s\" patch \"%s\"", fileName.data(), It->first.c_str());
					return false;
				}
				else
					_MESSAGE("\tThe patch saved: \"%s\"", It->first.c_str());
			}

			_MESSAGE("\tThe database has been saved: \"%s\"", fileName.data());

			return true;
		}
		
		SmartPointer<RelocationDatabaseItem> RelocationDatabase::GetByName(const char* name) const
		{
			for (auto It = _patches.begin(); It != _patches.end(); It++)
			{
				if (!_stricmp(It->first.c_str(), name))
					return It->second;
			}

			return nullptr;
		}

		bool RelocationDatabase::Has(const char* name) const
		{
			for (auto It = _patches.begin(); It != _patches.end(); It++)
			{
				if (!_stricmp(It->first.c_str(), name))
					return true;
			}

			return false;
		}

		SmartPointer<RelocationDatabaseItem> RelocationDatabase::Append(const char* name, RelocationDatabaseItem* Patch)
		{
			if (name && Patch && !Has(name))
			{
				auto pair = std::make_pair<String, SmartPointer<RelocationDatabaseItem>>(name, Patch);
				_patches.insert(pair);
				return pair.second;
			}

			return nullptr;
		}

		void RelocationDatabase::Clear()
		{
			_patches.clear();
		}
	}
}