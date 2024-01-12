//////////////////////////////////////////
/*
* Fallout 4 Script Extender (F4SE)
* by Ian Patterson, Stephen Abel, and Brendan Borthwick (ianpatt, behippo, and purplelunchbox)
*
* Contact the F4SE Team
*
* Entire Team
* Send email to team [at] f4se [dot] silverlock [dot] org
*
* Ian (ianpatt)
* Send email to ianpatt+f4se [at] gmail [dot] com
*
* Stephen (behippo)
* Send email to gamer [at] silverlock [dot] org
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

//////////////////////////////////////////
/*
* Copyright (c) 2022 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#pragma once
#pragma pack(push, 1)

#include "NiClassesF4.h"
#include "BSFixedString.h"
#include "..\BSTList.h"
#include "..\BSTArray.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
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
				uint32_t	formId;
				uint8_t		loaded; // 01 - loaded, 00 - unloaded
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
					SimpleLocker locker(&lock);

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
					SimpleLocker locker(&lock);

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
					uint64_t                unk08;				// 08
					BSTEventDispatcher<T>   eventDispatcher;    // 10
				};

				// void ** _vtbl;                               // 00
				uint64_t unk08;									// 08
				uint64_t unk10;									// 10
				BSTArray<EventSource<void*>*> eventSources;     // 18
			};
		}
	}
}

#pragma pack(pop)