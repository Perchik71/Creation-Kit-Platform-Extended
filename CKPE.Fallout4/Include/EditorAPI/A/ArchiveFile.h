// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BSFile.h>

namespace CKPE::Fallout4::EditorAPI
{
	class Archive;

	class ArchiveFile : public BSFile
	{
	public:
		constexpr static Common::ID VTABLE = IDs::VTABLE::ArchiveFile[0];

		virtual ~ArchiveFile() override;
		virtual void Seek(std::ptrdiff_t numBytes) override;
		virtual void Seek(std::int64_t offset, std::uint32_t mode) override;
		virtual std::uint64_t DoRead(void* buffer, std::uint64_t bytes) override;
		virtual std::uint64_t DoWrite(const void* buffer, std::uint64_t bytes) override;
		virtual bool Reopen() noexcept(true) override;
		virtual std::uint64_t RefreshFileSize() noexcept(true) override;
		virtual std::uint32_t ReadString(BSString& value,
			std::uint64_t maxBytes) noexcept(true) override;
		virtual std::uint32_t ReadWideString(void* value,
			std::uint64_t maxBytes) noexcept(true) override;
		[[nodiscard]] virtual bool Exists() noexcept(true) override;

		/* 0x2B0 */ Archive* archive;
		/* 0x2B8 */ std::uint64_t archiveOffset;
	};
	static_assert(sizeof(ArchiveFile) == 0x2C0);
	static_assert(offsetof(ArchiveFile, archive) == 0x2B0);
	static_assert(offsetof(ArchiveFile, archiveOffset) == 0x2B8);
}
