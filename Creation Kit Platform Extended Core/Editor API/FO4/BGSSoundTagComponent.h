// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once
#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			// size = 0x10
			// func = 21
			class BGSSoundTagComponent
			{
			public:
				virtual ~BGSSoundTagComponent() = default;
			public:
				inline uint64_t GetTag() const { return _Tag; }
			public:
				READ_PROPERTY(GetTag) uint64_t Tag;
			private:
				uint64_t _Tag;
			};
			static_assert(sizeof(BGSSoundTagComponent) == 0x10);
		}
	}
}

#pragma pack(pop)
