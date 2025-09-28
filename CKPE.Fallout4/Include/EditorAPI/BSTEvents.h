// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BSFixedString.h"
#include "BSTList.h"
#include "BSTArray.h"
#include "BSSimpleLock.h"

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

			};

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

			// 08
			template <typename EventT>
			class BSTEventDispatcher
			{
			public:
				typedef BSTEventSink<EventT> SinkT;

				bool AddEventSink(SinkT* sink)
				{
					SimpleLock locker(&lock);

					// Check for duplicate first
					for (int i = 0; i < eventSinks.count; i++)
					{
						if (eventSinks[i] == sink)
							return false;
					}

					eventSinks.Insert(0, sink);
					return true;
				}

				void RemoveEventSink(SinkT* sink)
				{
					SimpleLock locker(&lock);

					for (int i = 0; i < eventSinks.count; i++)
					{
						if (eventSinks[i] == sink) {
							eventSinks.Remove(i);
							break;
						}
					}
				}

				SimpleLock			lock;				// 000
				BSTArray<SinkT*>	eventSinks;			// 008
				BSTArray<SinkT*>	addBuffer;			// 020
				BSTArray<SinkT*>	removeBuffer;		// 038
				bool				stateFlag;			// 050
				char				pad[3];
			};

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