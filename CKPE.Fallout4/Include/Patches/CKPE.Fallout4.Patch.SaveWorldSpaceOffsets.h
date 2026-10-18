// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			// The Creation Kit only writes the WRLD "OFST" (cell offset table) and "CLSZ" (cell size table)
			// subrecords in its internal version control check-in. The regular Save never does, so master
			// files saved by the CK have no fast cell lookup tables. After a successful Save of an active
			// master file this patch runs the CK's own offset updater (the same sequence the CK's
			// delocalization tool uses without version control) on the saved file.
			class SaveWorldSpaceOffsets : public Common::Patch
			{
				static bool hk_Save(void* handler, const char* file_name) noexcept(true);
				static void UpdateOffsets() noexcept(true);

				SaveWorldSpaceOffsets(const SaveWorldSpaceOffsets&) = delete;
				SaveWorldSpaceOffsets& operator=(const SaveWorldSpaceOffsets&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				SaveWorldSpaceOffsets();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
				[[nodiscard]] bool SupportsAddressLibrary() const noexcept(true) override { return true; }
			};
		}
	}
}
