// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSResourceArchive2.h>
#include <EditorAPI/N/NiPointer.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace OG_NG
			{
				// 0x20
				class BSIStream
				{
					/* 0x008 */ BSResource::OG_NG::Stream* Stream;
					/* 0x010 */ bool opened;
					/* 0x011 */ std::byte pad011[0x7];
					/* 0x018 */ BSFixedString fileName;
				public:
					virtual ~BSIStream();

					inline void NewStream(BSResource::Archive2::OG_NG::ReaderStream* s) { Stream = s; }
					[[nodiscard]] inline const BSResource::OG_NG::Stream* GetStream() const noexcept(true) { return Stream; }
					[[nodiscard]] inline bool IsOpen() const noexcept(true) { return opened; }
					[[nodiscard]] inline const BSFixedString& GetFileName() const noexcept(true) { return fileName; }
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(BSIStream, Stream) == 0x8 &&
							offsetof(BSIStream, opened) == 0x10 &&
							offsetof(BSIStream, fileName) == 0x18;
					}
				};
				static_assert(sizeof(BSIStream) == 0x20);
				static_assert(BSIStream::VerifyLayout());
			}

			namespace AE
			{
				// 0x20
				class BSIStream
				{
					/* 0x008 */ BSResource::AE::Stream* Stream;
					/* 0x010 */ bool opened;
					/* 0x011 */ std::byte pad011[0x7];
					/* 0x018 */ BSFixedString fileName;
				public:
					virtual ~BSIStream();

					inline void NewStream(BSResource::Archive2::AE::ReaderStream* s) { Stream = s; }
					[[nodiscard]] inline const BSResource::AE::Stream* GetStream() const noexcept(true) { return Stream; }
					[[nodiscard]] inline bool IsOpen() const noexcept(true) { return opened; }
					[[nodiscard]] inline const BSFixedString& GetFileName() const noexcept(true) { return fileName; }
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(BSIStream, Stream) == 0x8 &&
							offsetof(BSIStream, opened) == 0x10 &&
							offsetof(BSIStream, fileName) == 0x18;
					}
				};
				static_assert(sizeof(BSIStream) == 0x20);
				static_assert(BSIStream::VerifyLayout());
			}
		}
	}
}