// Copyright © 2024 aka perchik71.All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <EditorAPI/BSFixedString.h>
#include "NiObject.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace BSGraphics
			{
				struct Texture;
			}

			namespace NiAPI
			{
				class NiTexture : public NiObject
				{
				public:
					[[nodiscard]] inline BSFixedString GetFileNameFS() const noexcept { return _FileName; }
					[[nodiscard]] inline const char* GetFileName() const noexcept { return _FileName.c_str(); }
					[[nodiscard]] inline NiTexture* GetPrev() const noexcept { return _PrevTexture; }
					[[nodiscard]] inline NiTexture* GetNext() const noexcept { return _NextTexture; }
					[[nodiscard]] inline BSGraphics::Texture* GetTexture() const noexcept { return _Texture; }
					[[nodiscard]] inline void SetTexture(BSGraphics::Texture* Texture) noexcept { _Texture = Texture; }
				private:
					std::uint32_t Unk10[4];			// 10
					BSFixedString _FileName;		// 20
					char _Pad28[8];					// 28
					NiTexture* _PrevTexture;		// 30
					NiTexture* _NextTexture;		// 38
					char _Pad30[8];					// 40
					BSGraphics::Texture* _Texture;	// 48
					char _Pad40[8];					// 50
				};
				static_assert(sizeof(NiTexture) == 0x58);
			}
		}
	}
}