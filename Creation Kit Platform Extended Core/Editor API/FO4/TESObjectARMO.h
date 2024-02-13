// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "BSEntryString.h"
#include "BGSPreviewTransform.h"
#include "BGSSoundTagComponent.h"
#include "BGSModelMaterialSwap.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			// size = 0x400
			// func = 154
			class TESObjectARMO : public TESForm
			{
			public:
				virtual ~TESObjectARMO() = default;
			public:
				inline const char* Name() const { return _Name ? _Name->Get<char>(true) : nullptr; }
				inline BGSPreviewTransform& GetPreviewTransform() { return _PreviewTransform; }
				inline BGSSoundTagComponent& GetSoundTag() { return _SoundTag; }
				inline BGSModelMaterialSwap& GetMaleModel() { return _MaleModel; }
				inline BGSModelMaterialSwap& GetWomanModel() { return _WomanModel; }
				inline TESForm* GetRace() { return _RaceForm; }
				inline TESForm** GetKeywords() { return _Keywords; }
				inline uint32_t GetKeywordCount() const { return _KeywordCount; }
			private:
				char pad28[0x48];
				BGSPreviewTransform _PreviewTransform;
				BGSSoundTagComponent _SoundTag;
				char pad90[0x10];
				BSEntryString* _Name;
				char padA8[0x8];
				TESForm* _RaceForm;
				char padB8[0x48];
				BGSModelMaterialSwap _MaleModel;
				BGSModelMaterialSwap _WomanModel;
				// Old data (Message icons and model)
				char pad1A0[0xB0];
				// TESModelRDT
				char pad250[0x80];
				TESForm** _Keywords;
				uint32_t _KeywordCount;
				char pad2DC[0x124];
			};
			static_assert(sizeof(TESObjectARMO) == 0x400);
		}
	}
}

#pragma pack(pop)