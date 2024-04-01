// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TESTopicInfo.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			// size 0x80
			// func 101
			class TESTopic : public TESForm
			{
			public:
				constexpr static uint8_t TYPE_ID = ftTopic;
				
			public:
				virtual ~TESTopic() = default;

				inline const char* GetTopicText() const { return _TopicText.c_str(); }
				inline uint16_t GetSubtype() const { return _Subtype; }
				inline uint32_t GetPriority() const { return _Priority; }
				inline TESForm* GetQuest() const { return _Quest; }
				inline TESTopicInfo** GetInfoList() const { return _InfoList; }
				inline uint32_t GetInfoCount() const { return _InfoCount; }

				inline TESTopicInfo* GetInfoAt(uint32_t Id) const { return _InfoList[Id]; }
				inline TESTopicInfo* operator[](uint32_t Id) const { return GetInfoAt(Id); }

				READ_PROPERTY(GetTopicText) const char* TopicText;
				READ_PROPERTY(GetSubtype) uint16_t Subtype;
				READ_PROPERTY(GetPriority) uint32_t Priority;
				READ_PROPERTY(GetQuest) TESForm* Quest;
				READ_PROPERTY(GetInfoList) TESTopicInfo** InfoList;
				READ_PROPERTY(GetInfoCount) uint32_t InfoCount;
			private:
				char pad28[0x8];
				BGSLocalizedString<char> _TopicText;
				char pad40[0x2];
				uint16_t _Subtype;
				// The data is stored in integers, but without multiplication by 100, in fact, 
				// the fractional part in the interface is always discarded, why would it be done?
				// Such data can then be stored in a byte.
				uint32_t _Priority;
				char pad48[0x8];
				TESForm* _Quest;
				TESTopicInfo** _InfoList;
				uint32_t _InfoCount;
				char pad64[0x1C];
			};
			static_assert(sizeof(TESTopic) == 0x80);
		}
	}
}

#pragma pack(pop)