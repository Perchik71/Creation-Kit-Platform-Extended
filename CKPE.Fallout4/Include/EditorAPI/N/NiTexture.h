// Copyright © 2024 aka CKPE team.All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/N/NiObject.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiTexture : public NiObject
				{
				public:
					[[nodiscard]] inline BSFixedString GetFileNameFS() const noexcept(true) { return _FileName; }
					[[nodiscard]] inline const char* GetFileName() const noexcept(true) { return _FileName; }
					[[nodiscard]] inline NiTexture* GetPrev() const noexcept(true) { return _PrevTexture; }
					[[nodiscard]] inline NiTexture* GetNext() const noexcept(true) { return _NextTexture; }
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(NiTexture, _FileName) == 0x10 &&
							offsetof(NiTexture, _PrevTexture) == 0x20 &&
							offsetof(NiTexture, _NextTexture) == 0x28 &&
							offsetof(NiTexture, _RendererTexture) == 0x38;
					}
				private:
					// Likely texture-array cast; the base returns null.
					/* 0x150 */ virtual void sub_150();

					BSFixedString _FileName;			// 10
					std::uint32_t _Flags;			// 18
					std::uint32_t _Pad1C;			// 1C
					NiTexture* _PrevTexture;			// 20
					NiTexture* _NextTexture;			// 28
					void* _Stream;					// 30
					void* _RendererTexture;			// 38
					std::int8_t _DesiredDegradeLevel;	// 40
					std::int8_t _SavedDegradeLevel;	// 41
					std::uint8_t _TextureFlags;		// 42
					char _Pad43[5];					// 43
				};
				static_assert(sizeof(NiTexture) == 0x48);
				static_assert(NiTexture::VerifyLayout());
			}
		}
	}
}