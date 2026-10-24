// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/IDs.h>
#include <EditorAPI/B/BSSpinLock.h>
#include <EditorAPI/B/BSTEvents.h>
#include <EditorAPI/B/BSTSmartPointer.h>
#include <CKPE.Common.Relocation.h>
#include <cstddef>
#include <cstring>

namespace CKPE::Fallout4::EditorAPI
{
	class BSAnimationGraphChannel;
	class BShkbAnimationGraph;
	class hkbVariableValue;

	class AnimVariableCacheInfo
	{
	public:
		/* 0x000 */ BSFixedString name;
		/* 0x008 */ hkbVariableValue* value;
	};
	static_assert(sizeof(AnimVariableCacheInfo) == 0x10);
	static_assert(offsetof(AnimVariableCacheInfo, value) == 0x8);

	class BSAnimationGraphVariableCache
	{
	public:
		/* 0x000 */ BSTArray<AnimVariableCacheInfo> variableCache;
		/* 0x018 */ BSTArray<hkbVariableValue*> variableQuickLookup;
		/* 0x030 */ BSSpinLock* lock;
		/* 0x038 */ BSTSmartPointer<BShkbAnimationGraph> graphToCacheFor;
	};
	static_assert(sizeof(BSAnimationGraphVariableCache) == 0x40);
	static_assert(offsetof(BSAnimationGraphVariableCache, variableQuickLookup) == 0x18);
	static_assert(offsetof(BSAnimationGraphVariableCache, lock) == 0x30);
	static_assert(offsetof(BSAnimationGraphVariableCache, graphToCacheFor) == 0x38);

	class BSAnimationGraphManager :
		public BSTEventSink<BSAnimationGraphEvent>,
		public BSIntrusiveRefCounted
	{
	public:
		// Native tagged-pointer view; the containing manager owns the reference.
		class DependentManagerSmartPtr
		{
		public:
			/* 0x000 */ std::uintptr_t ptrAndFlagsStorage;

			[[nodiscard]] BSAnimationGraphManager* Get() const noexcept(true)
			{
				return reinterpret_cast<BSAnimationGraphManager*>(
					ptrAndFlagsStorage & ~std::uintptr_t{ 1 });
			}
		};
		static_assert(sizeof(DependentManagerSmartPtr) == 0x8);

		struct QueuedEvent
		{
			/* 0x000 */ BSFixedString name;
			/* 0x008 */ BSFixedString payload;
		};
		static_assert(sizeof(QueuedEvent) == 0x10);
		static_assert(offsetof(QueuedEvent, payload) == 0x8);

		virtual ~BSAnimationGraphManager();
		EventResult ReceiveEvent(BSAnimationGraphEvent* event,
			void* dispatcher) override;

		[[nodiscard]] bool NotifyAnimationGraph(const BSFixedString& eventName)
		{
			using Function = bool (*)(BSAnimationGraphManager*, const BSFixedString&);
			static Common::Relocation<Function> function{
				EditorAPI::IDs::BSAnimationGraphManager::NotifyAnimationGraph };
			return function(this, eventName);
		}

		[[nodiscard]] std::uint32_t GetGraphCount() const noexcept(true)
		{
			std::uint32_t count;
			std::memcpy(&count, graphStorage + 0x10, sizeof(count));
			return count;
		}

		[[nodiscard]] BShkbAnimationGraph* GetGraph(std::uint32_t index) const noexcept(true)
		{
			if (index >= GetGraphCount())
				return nullptr;

			std::int32_t capacityAndFlags;
			std::memcpy(&capacityAndFlags, graphStorage, sizeof(capacityAndFlags));
			const std::byte* data = graphStorage + 0x8;
			if (capacityAndFlags >= 0)
				std::memcpy(&data, graphStorage + 0x8, sizeof(data));
			BShkbAnimationGraph* graph;
			std::memcpy(&graph, data + index * sizeof(graph), sizeof(graph));
			return graph;
		}

		[[nodiscard]] BShkbAnimationGraph* GetActiveGraph() const noexcept(true)
		{
			return GetGraph(activeGraph);
		}

		/* 0x00C */ std::byte pad00C[0x4];
		/* 0x010 */ std::byte unk010[0x30];
		// Native BSTSmallArray<BSTSmartPointer<BShkbAnimationGraph>, 1>.
		/* 0x040 */ std::byte graphStorage[0x18];
		/* 0x058 */ BSTArray<DependentManagerSmartPtr> subManagers;
		/* 0x070 */ BSTArray<QueuedEvent> eventQueue;
		/* 0x088 */ BSAnimationGraphVariableCache variableCache;
		/* 0x0C8 */ BSSpinLock updateLock;
		/* 0x0D0 */ BSSpinLock dependentManagerLock;
		/* 0x0D8 */ std::uint32_t activeGraph;
		/* 0x0DC */ std::uint32_t generateDepth;
	};
	static_assert(sizeof(BSAnimationGraphManager) == 0xE0);
	static_assert(offsetof(BSAnimationGraphManager, graphStorage) == 0x40);
	static_assert(offsetof(BSAnimationGraphManager, subManagers) == 0x58);
	static_assert(offsetof(BSAnimationGraphManager, eventQueue) == 0x70);
	static_assert(offsetof(BSAnimationGraphManager, variableCache) == 0x88);
	static_assert(offsetof(BSAnimationGraphManager, updateLock) == 0xC8);
	static_assert(offsetof(BSAnimationGraphManager, dependentManagerLock) == 0xD0);
	static_assert(offsetof(BSAnimationGraphManager, activeGraph) == 0xD8);
	static_assert(offsetof(BSAnimationGraphManager, generateDepth) == 0xDC);
}
