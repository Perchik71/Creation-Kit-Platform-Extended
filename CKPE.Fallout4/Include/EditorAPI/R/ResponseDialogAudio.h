// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

namespace CKPE::Fallout4::EditorAPI
{
	namespace Forms { class BGSVoiceType; }

	struct ResponseDialogAudio
	{
		/* 0x000 */ Forms::BGSVoiceType* VoiceType;
		/* 0x008 */ std::uint32_t HasXWM;
		/* 0x00C */ char AudioFileName[260];
		/* 0x110 */ std::uint32_t HasWAV;
		/* 0x114 */ std::uint32_t HasLIP;
		/* 0x118 */ std::uint32_t HasLTF;
		/* 0x11C */ std::uint8_t pad11C[0x04];
	};
	static_assert(sizeof(ResponseDialogAudio) == 0x120);
	static_assert(offsetof(ResponseDialogAudio, AudioFileName) == 0x0C);
	static_assert(offsetof(ResponseDialogAudio, HasWAV) == 0x110);
	static_assert(offsetof(ResponseDialogAudio, HasLTF) == 0x118);
}
