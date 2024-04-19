// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <Voltek.RelocationDatabase.h>

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class Engine;

		class RelocationDatabaseItem
		{
		public:
			RelocationDatabaseItem();
			RelocationDatabaseItem(voltek::reldb_stream* stm, voltek::reldb_patch* pch);
			~RelocationDatabaseItem() = default;
		public:
			void Clear();

			bool SaveToFileDeveloped(const char* filename);

			inline uint32_t Version() const { return (uint32_t)voltek::reldb_get_version_patch(patch); }
			String Name() const;
			uint32_t At(uint32_t nId) const;
			String PatternAt(uint32_t nId) const;
			inline uint32_t Count() const { return (uint32_t)voltek::reldb_count_signatures_in_patch(patch); }

			inline uint32_t operator[](uint32_t nId) const { return At(nId); }
		private:
			RelocationDatabaseItem(const RelocationDatabaseItem&) = default;
			RelocationDatabaseItem& operator=(const RelocationDatabaseItem&) = default;

			voltek::reldb_stream* stream;
			voltek::reldb_patch* patch;
		};

		class RelocationDatabase
		{
		public:
			RelocationDatabase(Engine* lpEngine);
			~RelocationDatabase();

			bool CreateDatabase();
			bool OpenDatabase();
			bool SaveDatabase();

			inline bool Has(const char* name) const { return voltek::reldb_has_patch(_stm, name); }
			SmartPointer<RelocationDatabaseItem> Append(const char* filename);
			SmartPointer<RelocationDatabaseItem> GetByName(const char* name) const;
			bool Remove(const char* name);

			void Clear();
		private:
			Engine* _engine;
			voltek::reldb_stream* _stm;
		};

		extern RelocationDatabase* GlobalRelocationDatabasePtr;
	}
}
