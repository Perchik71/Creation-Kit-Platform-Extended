// Copyright © 2024-2025 aka CKPE team. All rights reserved.
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
			class ChooseSoundFile : public Common::Patch
			{
				static bool sub(std::int64_t unknown, const char* lpPath, const char* lpFormat, const char* lpCaption,
					char* lpFileName, void* pHandler, std::int32_t iUnknown, bool bSaveDlg, const char* lpUnknownStr, 
					std::uint32_t uUnknown, const char* lpBuffer, void* _CrtBuffer, std::int64_t unk02,
					std::int64_t unk03, std::int64_t unk04, std::int64_t unk05) noexcept(true);

				ChooseSoundFile(const ChooseSoundFile&) = delete;
				ChooseSoundFile& operator=(const ChooseSoundFile&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				ChooseSoundFile();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
				[[nodiscard]] bool SupportsAddressLibrary() const noexcept(true) override { return true; }
			};
		}
	}
}