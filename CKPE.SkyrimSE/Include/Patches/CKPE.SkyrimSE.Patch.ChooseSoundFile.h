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
			class ChooseSoundFile : public Common::Patch
			{
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

				static bool sub(std::int64_t iHandle, const char* lpPath, const char* lpFormat,
					const char* lpCaption, char* lpFileName, void* pHandler, std::int32_t iUnknown,
					bool bSaveDlg, const char* lpUnknownStr, std::uint32_t uUnknown, const char* lpBuffer,
					void* _CrtBuffer) noexcept(true);
			};
		}
	}
}