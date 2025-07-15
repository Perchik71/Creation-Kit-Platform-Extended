// Copyright © 2023-2024 aka perchik71. All rights reserved.
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
			class BNetConvertUnicodeString : public Common::Patch
			{
				BNetConvertUnicodeString(const BNetConvertUnicodeString&) = delete;
				BNetConvertUnicodeString& operator=(const BNetConvertUnicodeString&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				BNetConvertUnicodeString();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static std::size_t Convert(char* Destination, std::size_t DestSize,
					const wchar_t* Source, std::size_t SourceSize) noexcept(true);
			};
		}
	}
}