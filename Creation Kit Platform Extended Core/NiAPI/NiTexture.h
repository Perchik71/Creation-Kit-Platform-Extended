// Copyright © 2024 aka perchik71.All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "NiObject.h"
#include "Editor API/FO4/BSFixedString.h"
#include "Editor API/FO4/BSEntryString.h"

#pragma pack(push, 8)

namespace CreationKitPlatformExtended
{
	namespace NiAPI
	{
		class NiTexture : public NiObject
		{
		public:
			NiTexture() = default;
			virtual ~NiTexture() = default;

			char _pad[0x30];
		};
		static_assert(sizeof(NiTexture) == 0x40);

		namespace Fallout4
		{
			using namespace EditorAPI::Fallout4;

			class NiTexture : public NiObject
			{
			public:
				inline BSFixedString GetFileNameFS() const { return _FileName; }
				inline const char* GetFileName() const { return _FileName.c_str(); }
				inline NiTexture* GetPrev() const { return _PrevTexture; }
				inline NiTexture* GetNext() const { return _NextTexture; }
				inline void* GetBinaryData() const { return _BinaryData; }
				inline void SetBinaryData(void* Buffer) { _BinaryData = Buffer; }

				inline static NiTexture* (*CreateTexture)(BSEntryString*, uint32_t, uint32_t);
				inline static void (*CreateTexture2)(const char*, NiTexture*&, uint32_t);
			private:
				BSFixedString	_FileName;		// 10
				char			_Pad18[8];		// 18
				NiTexture*		_PrevTexture;	// 20
				NiTexture*		_NextTexture;	// 28
				char			_Pad30[8];		// 30
				void*			_BinaryData;	// 38
				char			_Pad40[8];		// 40
			};
			static_assert(sizeof(NiTexture) == 0x48);
		}
	}
}

#pragma pack(pop)