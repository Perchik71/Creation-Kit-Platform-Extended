// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TESForm.h"
#include "..\BSTArray.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class BGSSoundOutput;
			class BGSSoundCategory;
			class BGSSoundDescriptorForm;

			// size 0x78
			// func 27
			class BGSStandardSoundDef
			{
			public:
				virtual ~BGSStandardSoundDef() = default;

				inline BGSSoundCategory* GetSoundCategory() const { return _SoundCategory; }
				inline BGSSoundOutput* GetSoundOutput() const { return _SoundOutput; }
				inline BGSSoundDescriptorForm* GetDescriptor() const { return _Descriptor; }
			private:
				BGSSoundCategory* _SoundCategory;
				char pad10[0x8];
				BSTArray<const char*> _SoundPaths;
				BSTArray<void*> _SoundCaches;
				char pad48[0x18];
				BGSSoundOutput* _SoundOutput;
				char pad68[0x8];
				BGSSoundDescriptorForm* _Descriptor;
			};
			static_assert(sizeof(BGSStandardSoundDef) == 0x78);

			// size 0x40
			// func 101
			class BGSSoundDescriptorForm : public TESForm
			{
			public:
				constexpr static uint8_t TYPE_ID = ftSoundDescriptor;
			public:
				virtual ~BGSSoundDescriptorForm() = default;

				inline BGSStandardSoundDef* GetSound() const { return _Sound; }
			private:
				char pad28[0x10];
				BGSStandardSoundDef* _Sound;
			};
			static_assert(sizeof(BGSSoundDescriptorForm) == 0x40);
		}
	}
}

#pragma pack(pop)