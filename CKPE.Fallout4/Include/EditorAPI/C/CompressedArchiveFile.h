// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/A/ArchiveFile.h>

namespace CKPE::Fallout4::EditorAPI
{
	class CompressedArchiveFile : public ArchiveFile
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::CompressedArchiveFile[0];

		virtual ~CompressedArchiveFile() override;
		virtual void Seek(std::ptrdiff_t numBytes) override;
		virtual void Seek(std::int64_t offset, std::uint32_t mode) override;
		virtual std::uint64_t DoRead(void* buffer, std::uint64_t bytes) override;
		virtual std::uint64_t DoWrite(const void* buffer, std::uint64_t bytes) override;
		[[nodiscard]] virtual std::uint64_t GetSize() override;
		virtual bool Reopen() noexcept(true) override;
		virtual std::uint64_t RefreshFileSize() noexcept(true) override;
		virtual std::uint32_t ReadString(BSString& value,
			std::uint64_t maxBytes) noexcept(true) override;
		virtual std::uint32_t ReadWideString(void* value,
			std::uint64_t maxBytes) noexcept(true) override;
		[[nodiscard]] virtual bool Exists() noexcept(true) override;

		/* 0x2C0 */ void* inflateStream;
		/* 0x2C8 */ std::byte* decompressedBuffer;
		/* 0x2D0 */ std::uint64_t uncompressedSize;
		/* 0x2D8 */ std::uint64_t decompressedBufferSize;
		/* 0x2E0 */ std::uint64_t decompressedBufferPosition;
		/* 0x2E8 */ std::uint64_t decompressedBufferReadSize;
	};
	static_assert(sizeof(CompressedArchiveFile) == 0x2F0);
	static_assert(offsetof(CompressedArchiveFile, inflateStream) == 0x2C0);
	static_assert(offsetof(CompressedArchiveFile, uncompressedSize) == 0x2D0);
	static_assert(offsetof(CompressedArchiveFile, decompressedBufferPosition) == 0x2E0);
}
