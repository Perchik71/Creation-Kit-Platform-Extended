// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			class BSArchiveManager : public Common::Patch
			{
				BSArchiveManager(const BSArchiveManager&) = delete;
				BSArchiveManager& operator=(const BSArchiveManager&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				BSArchiveManager();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static void AttachBSAFile(const char* _filename) noexcept(true);
				static void LoadTesFile(const void* load_file) noexcept(true);
				static void LoadTesFileFinal() noexcept(true);
			};
		}
	}
}