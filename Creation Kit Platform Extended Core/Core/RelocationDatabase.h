// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class Engine;

		class RelocationDatabaseItem
		{
		public:
			RelocationDatabaseItem();
			RelocationDatabaseItem(const char* lpcstrName, uint32_t nVersion, std::initializer_list<uint32_t> rvaList);
			~RelocationDatabaseItem();
		public:
			bool LoadFromFileStream(FILE* fileStream);
			bool SaveToFileStream(FILE* fileStream);
			void Clear();

			bool LoadFromFileDeveloped(const char* filename);
			bool SaveToFileDeveloped(const char* filename);

			inline uint32_t Version() const { return _version; }
			inline String Name() const { return _name; }
			inline uint32_t At(uint32_t nId) const { return _rvaList[nId]; }
			inline uint32_t Count() const { return (uint32_t)_rvaList.size(); }

			inline uint32_t operator[](uint32_t nId) const { return At(nId); }
		private:
			RelocationDatabaseItem(const RelocationDatabaseItem&) = default;
			RelocationDatabaseItem& operator=(const RelocationDatabaseItem&) = default;

			uint32_t _version;
			String _name;
			Array<uint32_t> _rvaList;
		};

		class RelocationDatabase
		{
		public:
			RelocationDatabase(Engine* lpEngine);
			~RelocationDatabase();

			bool CreateDatabase();
			bool OpenDatabase();
			bool SaveDatabase();

			bool Has(const char* name) const;
			SmartPointer<RelocationDatabaseItem> Append(const char* name, RelocationDatabaseItem* Patch);
			SmartPointer<RelocationDatabaseItem> GetByName(const char* name) const;
			bool Remove(const char* name);

			void Clear();
		private:
			Engine* _engine;
			Map<String, SmartPointer<RelocationDatabaseItem>> _patches;
		};

		extern RelocationDatabase* GlobalRelocationDatabasePtr;
	}
}
