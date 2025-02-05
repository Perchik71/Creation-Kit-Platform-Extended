// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "BSFixedString.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			namespace BSResource
			{
				class LooseFileStream
				{
					char pad08[0x20];
					EditorAPI::SkyrimSpectialEdition::BSFixedString FileName;
				public:
					virtual ~LooseFileStream();

					const EditorAPI::SkyrimSpectialEdition::BSFixedString GetFileName() const noexcept(true) { return FileName; }
				};
			}

			class BSResourceNiBinaryStream
			{
				char pad00[0x10];
				void* ReadFuncPtr;
				void* WriteFuncPtr;
				BSResource::LooseFileStream* Stream;
			public:
				inline const BSResource::LooseFileStream* GetStream() const noexcept(true) { return Stream; }
			};
			static_assert(sizeof(BSResourceNiBinaryStream) == 0x28);
		}
	}
}

#pragma pack(pop)