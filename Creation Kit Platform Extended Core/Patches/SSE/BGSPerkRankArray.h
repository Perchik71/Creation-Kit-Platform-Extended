// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace CreationKitPlatformExtended::Core;

			class BGSPerkRankArrayPatch : public Module
			{
			public:
				struct PerkRankEntry
				{
					union
					{
						struct
						{
							uint32_t FormId;	// 0x0
							uint8_t Rank;		// 0x4
						};

						struct
						{
							uint64_t FormIdOrPointer;	// 0x0
							uint8_t NewRank;			// 0x8
						};
					};
				};
				static_assert(sizeof(PerkRankEntry) == 0x10);

				BGSPerkRankArrayPatch();

				virtual bool HasOption() const;
				virtual bool HasCanRuntimeDisabled() const;
				virtual const char* GetOptionName() const;
				virtual const char* GetName() const;
				virtual bool HasDependencies() const;
				virtual Array<String> GetDependencies() const;

				static bool sub1(PerkRankEntry* Entry, uint32_t* FormId, __int64 UnknownArray);
				static void sub2(__int64 ArrayHandle, PerkRankEntry*& Entry);
			protected:
				virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
					const char* lpcstrPlatformRuntimeVersion) const;
				virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
				virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			private:
				BGSPerkRankArrayPatch(const BGSPerkRankArrayPatch&) = default;
				BGSPerkRankArrayPatch& operator=(const BGSPerkRankArrayPatch&) = default;
			};
		}
	}
}