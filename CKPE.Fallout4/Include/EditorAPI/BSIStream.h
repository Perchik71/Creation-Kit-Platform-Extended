// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/BSResourceArchive2.h>
#include <EditorAPI/NiAPI/NiPointer.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 0x20
			class BSIStream
			{
				BSResource::Archive2::ReaderStream* Stream;
				char pad[0x8];			// it looks like this is the data of the list, how much data is in it
				BSEntryString* list;	// maybe list
			public:
				virtual ~BSIStream();

				inline void NewStream(BSResource::Archive2::ReaderStream* s) { Stream = s; }
			};
			static_assert(sizeof(BSIStream) == 0x20);
		}
	}
}