// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once
#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			struct TESChunk
			{
				char type[4];
				uint32_t size;
				uint32_t flags;
				uint32_t identifier;
				uint16_t timestamp;
				uint16_t vercontrol;
				uint16_t internalver;
				uint16_t unk;
			};

			class TESFile
			{
			private:
				TESFile() = default;
				~TESFile() = default;

			public:
				enum : uint32_t
				{
					FILE_RECORD_ESM = 0x1,			// Master plugin
					FILE_RECORD_LOCALIZED = 0x80,	// Strings removed
					FILE_RECORD_LIGHT = 0x100,		// Master plugin (Light)
					FILE_RECORD_MID = 0x400,		// Master plugin (Mid)
				};

				static bool ReadFirstChunk(const char* fileName, TESChunk& chunk);
				static uint32_t GetTypeFile(const char* fileName);

				/*SYSTEMTIME GetCreationTime() const;
				SYSTEMTIME GetLastWriteTime() const;*/
			};
		}
	}
}

#pragma pack(pop)
