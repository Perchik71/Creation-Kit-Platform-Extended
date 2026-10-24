// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSTList.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BSLock.h>
#include <EditorAPI/T/TESObjectREFRFwd.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			enum EventResult
			{
				kEvent_Continue = 0,
				kEvent_Abort
			};

			// 08
			template <typename T>
			class BSTEventSink
			{
			public:
				virtual ~BSTEventSink() { };
				virtual	EventResult	ReceiveEvent(T* evn, void* dispatcher) { return kEvent_Continue; }; // pure
			//	void	** _vtbl;	// 00
			};

			struct BGSInventoryListEvent
			{
				struct Event
				{

				};
			};

			struct MenuOpenCloseEvent
			{
				BSFixedString	menuName;
				bool			isOpen;
			};

			struct MenuModeChangeEvent
			{

			};

			struct UserEventEnabledEvent
			{

			};

			struct RequestHUDModesEvent
			{

			};

			struct TESHitEvent
			{

			};

			struct PerkEntryUpdatedEvent
			{
				struct PerkValueEvents
				{

				};
			};

			struct ApplyColorUpdateEvent
			{

			};

			struct BSMovementDataChangedEvent
			{

			};

			struct BSTransformDeltaEvent
			{

			};

			struct BSActiveGraphIfInactiveEvent
			{

			};

			struct BSAnimationGraphEvent
			{
				// Native event prefix; no closed total-size witness.
				/* 0x00 */ Forms::TESObjectREFR* holder;
				/* 0x08 */ BSFixedString tag;
				/* 0x10 */ BSFixedString payload;
			};
			static_assert(sizeof(BSAnimationGraphEvent) == 0x18);
			static_assert(offsetof(BSAnimationGraphEvent, holder) == 0x0);
			static_assert(offsetof(BSAnimationGraphEvent, tag) == 0x8);
			static_assert(offsetof(BSAnimationGraphEvent, payload) == 0x10);

			struct BSSubGraphActivationUpdate
			{

			};

			struct bhkCharacterMoveFinishEvent
			{

			};

			struct bhkNonSupportContactEvent
			{

			};

			struct bhkCharacterStateChangeEvent
			{

			};

			struct ChargenCharacterUpdateEvent
			{

			};

			struct QuickContainerStateEvent
			{

			};

			struct TESObjectLoadedEvent
			{
				std::uint32_t	formId;
				std::uint8_t	loaded; // 01 - loaded, 00 - unloaded
			};

			struct TESLoadGameEvent
			{

			};

			// 58
			template <typename EventT>
			class BSTEventDispatcher
			{
			public:
				typedef BSTEventSink<EventT> SinkT;

				bool AddEventSink(SinkT* sink)
				{
					BSAutoLock locker(&lock);

					// Check for duplicate first
					for (std::uint32_t i = 0; i < eventSinks.size(); i++)
					{
						if (eventSinks[i] == sink)
							return false;
					}

					eventSinks.Insert(0, sink);
					return true;
				}

				void RemoveEventSink(SinkT* sink)
				{
					BSAutoLock locker(&lock);

					for (std::uint32_t i = 0; i < eventSinks.size(); i++)
					{
						if (eventSinks[i] == sink) {
							eventSinks.Remove(i);
							break;
						}
					}
				}

				BSSpinLock			lock;				// 000
				BSTArray<SinkT*>	eventSinks;			// 008
				BSTArray<SinkT*>	addBuffer;			// 020
				BSTArray<SinkT*>	removeBuffer;		// 038
				bool				stateFlag;			// 050
				char				pad[7];
			};

			template <typename EventT>
			using BSTEventSource = BSTEventDispatcher<EventT>;

			static_assert(sizeof(BSTEventSink<TESObjectLoadedEvent>) == 0x8);
			static_assert(sizeof(BSTEventDispatcher<TESObjectLoadedEvent>) == 0x58);
			static_assert(offsetof(BSTEventDispatcher<TESObjectLoadedEvent>, eventSinks) == 0x8);
			static_assert(offsetof(BSTEventDispatcher<TESObjectLoadedEvent>, addBuffer) == 0x20);
			static_assert(offsetof(BSTEventDispatcher<TESObjectLoadedEvent>, removeBuffer) == 0x38);
			static_assert(offsetof(BSTEventDispatcher<TESObjectLoadedEvent>, stateFlag) == 0x50);

			class BSTGlobalEvent
			{
			public:
				virtual ~BSTGlobalEvent();

				template <typename T>
				class EventSource
				{
				public:
					virtual ~EventSource();

					// void ** _vtbl;                           // 00
					std::uint64_t           unk08;				// 08
					BSTEventDispatcher<T>   eventDispatcher;    // 10
				};

				// void ** _vtbl;                               // 00
				std::uint64_t unk08;									// 08
				std::uint64_t unk10;									// 10
				BSTArray<EventSource<void*>*> eventSources;     // 18
			};
		}
	}
}