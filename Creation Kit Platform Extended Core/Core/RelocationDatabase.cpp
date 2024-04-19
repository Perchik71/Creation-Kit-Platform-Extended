// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Engine.h"
#include "RelocationDatabase.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		RelocationDatabaseItem::RelocationDatabaseItem() : patch(nullptr)
		{}

		RelocationDatabaseItem::RelocationDatabaseItem(voltek::reldb_stream* stm, voltek::reldb_patch* pch) : 
			stream(stm), patch(pch)
		{
			Assert(stream);
			Assert(patch);
		}

		void RelocationDatabaseItem::Clear()
		{
			if (patch)
				voltek::reldb_clear_signatures_in_patch(patch);
		}

		bool RelocationDatabaseItem::SaveToFileDeveloped(const char* filename)
		{
			if (!patch)
				return false;

			auto resultErrno = voltek::reldb_save_dev_file_patch(stream, patch, filename);
			if (resultErrno)
			{
				_ERROR("The file could not be created: \"%s\" message \"%s\"",
					filename,
					voltek::reldb_get_error_text(resultErrno));
				return false;
			}

			_MESSAGE("\tThe patch file has been saved: \"%s\" rva %u", 
				filename, 
				Count());

			return true;
		}

		String RelocationDatabaseItem::Name() const
		{ 
			if (!patch)
				return "";

			char szBuf[60];
			if (voltek::reldb_get_name_patch(patch, szBuf, 60) != 0)
				return "";
			
			return szBuf;
		}

		uint32_t RelocationDatabaseItem::At(uint32_t nId) const
		{ 
			voltek::reldb_signature* sign = nullptr;
			if (voltek::reldb_get_signature_patch(patch, &sign, nId) != 0)
				return 0;
			return voltek::reldb_get_rva_from_signature(sign); 
		}

		String RelocationDatabaseItem::PatternAt(uint32_t nId) const
		{ 
			voltek::reldb_signature* sign = nullptr;
			if (voltek::reldb_get_signature_patch(patch, &sign, nId) != 0)
				return "";
			
			auto len = voltek::reldb_get_pattern_length_from_signature(sign);
			if (!len) return "";

			String s;
			s.resize(len);
			voltek::reldb_get_pattern_from_signature(sign, s.data(), len);	
			return s; 
		}

		RelocationDatabase* GlobalRelocationDatabasePtr = nullptr;

		RelocationDatabase::RelocationDatabase(Engine* lpEngine) : _engine(lpEngine), _stm(nullptr)
		{}

		RelocationDatabase::~RelocationDatabase()
		{
			if (_stm)
				voltek::reldb_release_db(_stm);
		}

		bool RelocationDatabase::CreateDatabase()
		{
			if (!allowedDatabaseVersion.contains(_engine->GetEditorVersion()))
			{
				_ERROR("The database name for this version of Creation Kit is not in the list");
				return false;
			}

			auto fileName = allowedDatabaseVersion.at(_engine->GetEditorVersion());
			auto resultErrno = voltek::reldb_new_db(&_stm, fileName.data());
			if (resultErrno)
			{
				_ERROR("The database file could not be created: \"%s\" message \"%s\"", 
					fileName.data(), 
					voltek::reldb_get_error_text(resultErrno));
				return false;
			}

			_MESSAGE("\tAn empty database has been created: \"%s\"", fileName.data());

			return true;
		}

		bool RelocationDatabase::OpenDatabase()
		{
			if (_stm)
			{
				voltek::reldb_release_db(_stm);
				_stm = nullptr;
			}

			if (!allowedDatabaseVersion.contains(_engine->GetEditorVersion()))
			{
				_ERROR("The database name for this version of Creation Kit is not in the list");
				return false;
			}

			auto fileName = allowedDatabaseVersion.at(_engine->GetEditorVersion());
			auto resultErrno = voltek::reldb_open_db(&_stm, fileName.data());
			if (resultErrno)
			{
				_ERROR("The database file could not be opened: \"%s\" message \"%s\"",
					fileName.data(),
					voltek::reldb_get_error_text(resultErrno));
				return false;
			}

			_MESSAGE("Total patches: %u", voltek::reldb_count_patches(_stm));

			for (long i = 0; i < voltek::reldb_count_patches(_stm); i++)
			{
				voltek::reldb_patch* patch = nullptr;
				resultErrno = voltek::reldb_get_patch_by_id(_stm, &patch, i);
				if (resultErrno)
				{
					_ERROR("Couldn't get the patch: \"%s\" index %i",
						voltek::reldb_get_error_text(resultErrno),
						i);
					return false;
				}

				char szBuf[60];
				voltek::reldb_get_name_patch(patch, szBuf, 60);
				_MESSAGE("\t%s (version %i) -> signature count %i", 
					szBuf, 
					voltek::reldb_get_version_patch(patch),
					voltek::reldb_count_signatures_in_patch(patch));
			}

			return true;
		}

		bool RelocationDatabase::SaveDatabase()
		{
			if (!_stm)
				return false;

			auto resultErrno = voltek::reldb_save_db(_stm);
			if (resultErrno)
			{
				_ERROR("The database file could not be created: \"%s\"",
					voltek::reldb_get_error_text(resultErrno));
				return false;
			}

			_MESSAGE("\tThe database has been saved");

			return true;
		}
		
		SmartPointer<RelocationDatabaseItem> RelocationDatabase::GetByName(const char* name) const
		{
			voltek::reldb_patch* patch = nullptr;
			if (voltek::reldb_get_patch_by_name(_stm, &patch, name) != 0)
				return nullptr;

			return new RelocationDatabaseItem(_stm, patch);
		}

		SmartPointer<RelocationDatabaseItem> RelocationDatabase::Append(const char* filename)
		{
			voltek::reldb_patch* patch = nullptr;
			auto resultErrno = voltek::reldb_open_dev_file_patch(_stm, &patch, filename);
			if (resultErrno)
			{
				_ERROR("The file could not be opened: \"%s\" message \"%s\"",
					filename,
					voltek::reldb_get_error_text(resultErrno));
				return nullptr;
			}

			char szBuf[60];
			voltek::reldb_get_name_patch(patch, szBuf, 60);

			resultErrno = voltek::reldb_update_patch(_stm, patch);
			if (resultErrno)
			{
				_ERROR("The patch could not be added or updated: \"%s\" message \"%s\"",
					szBuf,
					voltek::reldb_get_error_text(resultErrno));
				return nullptr;
			}

			return GetByName(szBuf);
		}

		bool RelocationDatabase::Remove(const char* name)
		{
			auto resultErrno = voltek::reldb_remove_patch(_stm, name);
			if (resultErrno)
			{
				_ERROR("The patch could not be deleted: \"%s\" message \"%s\"",
					name,
					voltek::reldb_get_error_text(resultErrno));
				return false;
			}

			return true;
		}

		void RelocationDatabase::Clear()
		{
			auto resultErrno = voltek::reldb_clear_db(_stm);
			if (resultErrno)
			{
				_ERROR("Database cleanup failed: \"%s\"",
					voltek::reldb_get_error_text(resultErrno));
			}
		}
	}
}