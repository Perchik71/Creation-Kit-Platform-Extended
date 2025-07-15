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
			class BGSPerkRankArray : public Common::Patch
			{
				BGSPerkRankArray(const BGSPerkRankArray&) = delete;
				BGSPerkRankArray& operator=(const BGSPerkRankArray&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				BGSPerkRankArray();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				struct Entry
				{
					union
					{
						struct
						{
							std::uint32_t FormId;	// 0x0
							std::uint8_t Rank;		// 0x4
						};

						struct
						{
							std::uint64_t FormIdOrPointer;	// 0x0
							std::uint8_t NewRank;			// 0x8
						};
					};
				};
				static_assert(sizeof(Entry) == 0x10);

				static bool sub1(Entry* Entry, std::uint32_t* FormId, std::int64_t UnknownArray) noexcept(true);
				static void sub2(std::int64_t ArrayHandle, Entry*& Entry) noexcept(true);
			};
		}
	}
}