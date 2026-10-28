// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <EditorAPI/IDs_VTABLE.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BSRenderPass.h>

namespace CKPE::Fallout4::EditorAPI
{
	class BSBatchRenderer
	{
	public:
		struct PassGroup
		{
			/* 0x000 */ BSRenderPass* passHead;
			/* 0x008 */ std::uint32_t passEnum;
			/* 0x00C */ std::uint16_t nextPassGroup;
			/* 0x00E */ std::uint16_t commandBufferPasses;
		};
		static_assert(sizeof(PassGroup) == 0x10);
		static_assert(offsetof(PassGroup, nextPassGroup) == 0xC);

		struct GeometryGroup
		{
			/* 0x000 */ BSBatchRenderer* batchRenderer;
			/* 0x008 */ std::byte _pad008[0x10];
			/* 0x018 */ float depth;
			/* 0x01C */ std::uint32_t count;
			/* 0x020 */ std::uint8_t flags;
			/* 0x021 */ std::int8_t group;
			/* 0x022 */ std::byte _pad022[6];
		};
		static_assert(sizeof(GeometryGroup) == 0x28);
		static_assert(offsetof(GeometryGroup, group) == 0x21);

		struct CommandBufferPassesData
		{
			/* 0x00000 */ std::byte* buffer[8192];
			/* 0x10000 */ std::byte** currentBuffer;
			/* 0x10008 */ CommandBufferPassesData* next;
			/* 0x10010 */ std::uint32_t frame;
			/* 0x10014 */ std::uint32_t ownerIndex;
		};
		static_assert(sizeof(CommandBufferPassesData) == 0x10018);
		static_assert(offsetof(CommandBufferPassesData, next) == 0x10008);

		struct CommandBufferPassesDataEntry
		{
			/* 0x000 */ CommandBufferPassesData* data;
			/* 0x008 */ std::uint64_t groupAndPass;
		};
		static_assert(sizeof(CommandBufferPassesDataEntry) == 0x10);

		BSBatchRenderer() = delete;
		virtual ~BSBatchRenderer();

		constexpr static Common::ID VTABLE = IDs::VTABLE::BSBatchRenderer[0];

		/* 0x008 */ BSTArray<PassGroup> renderPasses[13];
		/* 0x140 */ std::byte _renderPassMaps[13][0x20];
		/* 0x2E0 */ std::uint32_t activePassIndexLists[13][2];
		/* 0x348 */ std::uint32_t currentFirstPass;
		/* 0x34C */ std::uint32_t currentLastPass;
		/* 0x350 */ bool autoClearPasses;
		/* 0x351 */ std::byte _pad351[7];
		/* 0x358 */ GeometryGroup* geometryGroups[24];
		/* 0x418 */ GeometryGroup* alphaGroup;
		/* 0x420 */ std::uint32_t groupingAlphasGroupCount;
		/* 0x424 */ std::byte _pad424[4];
		/* 0x428 */ GeometryGroup* groupingAlphasGroup;
		/* 0x430 */ BSTArray<CommandBufferPassesDataEntry> commandBufferPasses;
		/* 0x448 */ std::byte _commandBufferPassMap[0x20];
		/* 0x468 */ CommandBufferPassesData* bufferPassesCurrent;
		/* 0x470 */ CommandBufferPassesData* bufferPassesHead;
		/* 0x478 */ CommandBufferPassesData** bufferPassesTail;
		/* 0x480 */ std::uint32_t bufferPassesCurrentAllocFrame;
		/* 0x484 */ std::byte _pad484[4];
	};
	static_assert(sizeof(BSBatchRenderer) == 0x488);
	static_assert(offsetof(BSBatchRenderer, renderPasses) == 0x8);
	static_assert(offsetof(BSBatchRenderer, activePassIndexLists) == 0x2E0);
	static_assert(offsetof(BSBatchRenderer, geometryGroups) == 0x358);
	static_assert(offsetof(BSBatchRenderer, commandBufferPasses) == 0x430);
	static_assert(offsetof(BSBatchRenderer, bufferPassesTail) == 0x478);
}
