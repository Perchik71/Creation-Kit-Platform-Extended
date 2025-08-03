// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BSFixedString.h"

#pragma pack(push, 1)

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace BSResource
			{
				class LooseFileStream
				{
					char pad08[0x20];
					BSFixedString FileName;
				public:
					virtual ~LooseFileStream() = default;

					const BSFixedString GetFileName() const noexcept(true) { return FileName; }
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