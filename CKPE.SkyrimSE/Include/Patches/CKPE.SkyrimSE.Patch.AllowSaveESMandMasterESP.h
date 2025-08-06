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
			class AllowSaveESMandMasterESP : public Common::Patch
			{
				AllowSaveESMandMasterESP(const AllowSaveESMandMasterESP&) = delete;
				AllowSaveESMandMasterESP& operator=(const AllowSaveESMandMasterESP&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				AllowSaveESMandMasterESP();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static bool OpenPluginSaveDialog(std::uintptr_t ParentWindow, const char* BasePath, bool IsESM,
					char* Buffer, std::uint32_t BufferSize, const char* Directory);
			};
		}
	}
}