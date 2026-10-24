// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <Windows.h>
#include <EditorAPI/B/BSFile.h>
#include <EditorAPI/N/NiRefObject.h>

namespace CKPE::Fallout4::EditorAPI
{
	struct BSArchiveHeader
	{
		/* 0x000 */ std::uint32_t magic;
		/* 0x004 */ std::uint32_t version;
		/* 0x008 */ std::uint32_t offset;
		/* 0x00C */ std::uint32_t flags;
		/* 0x010 */ std::uint32_t folderCount;
		/* 0x014 */ std::uint32_t fileCount;
		/* 0x018 */ std::uint32_t folderNameLength;
		/* 0x01C */ std::uint32_t fileNameLength;
		/* 0x020 */ std::uint32_t fileFlags;
	};
	static_assert(sizeof(BSArchiveHeader) == 0x24);
	static_assert(offsetof(BSArchiveHeader, folderCount) == 0x10);

	class BSArchive : public BSArchiveHeader
	{
	public:
		/* 0x028 */ void* folders;
	};
	static_assert(sizeof(BSArchive) == 0x30);
	static_assert(offsetof(BSArchive, folders) == 0x28);

	// This is the legacy BSA reader, not the Archive2 BA2 index.
	class Archive : public BSFile, public NiAPI::NiRefObject, public BSArchive
	{
	public:
		virtual ~Archive() noexcept(true) override;

		/* 0x2F0 */ std::uint64_t unk2F0;
		/* 0x2F8 */ std::uint64_t unk2F8;
		/* 0x300 */ std::uint64_t unk300;
		/* 0x308 */ std::uint64_t unk308;
		/* 0x310 */ CRITICAL_SECTION criticalSection;
		/* 0x338 */ std::uint32_t owningThread;
		/* 0x33C */ std::uint32_t pad33C;
		/* 0x340 */ std::uint32_t lockCount;
		/* 0x344 */ std::uint32_t pad344;
		/* 0x348 */ std::uint8_t flags;
		/* 0x349 */ std::byte pad349[0x7];
		/* 0x350 */ char* folderNames;
		/* 0x358 */ char** folderNamePointers;
		/* 0x360 */ char* fileNames;
		/* 0x368 */ char** fileNamePointers;
	};
	static_assert(sizeof(Archive) == 0x370);
	static_assert(offsetof(Archive, magic) == 0x2C0);
	static_assert(offsetof(Archive, folders) == 0x2E8);
	static_assert(offsetof(Archive, criticalSection) == 0x310);
	static_assert(offsetof(Archive, folderNames) == 0x350);
}
