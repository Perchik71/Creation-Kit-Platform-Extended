// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.h>
#include "TESFullName.h"
#include "TESTopicInfo.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// size 0x80
				// func 101
				class TESTopic : public TESForm, public TESFullName
				{
				public:
					constexpr static uint8_t TYPE_ID = ftTopic;

				public:
					virtual ~TESTopic() = default;

					[[nodiscard]] inline const char* GetTopicText() const noexcept(true) { return GetFullName(); }
					[[nodiscard]] inline std::uint16_t GetSubtype() const noexcept(true) { return _Subtype; }
					[[nodiscard]] inline std::uint32_t GetPriority() const noexcept(true) { return _Priority; }
					[[nodiscard]] inline TESForm* GetQuest() const noexcept(true) { return _Quest; }
					[[nodiscard]] inline TESTopicInfo** GetInfoList() const noexcept(true) { return _InfoList; }
					[[nodiscard]] inline std::uint32_t GetInfoCount() const noexcept(true) { return _InfoCount; }

					[[nodiscard]] inline TESTopicInfo* GetInfoAt(std::uint32_t Id) const noexcept(true) { return _InfoList[Id]; }
					[[nodiscard]] inline TESTopicInfo* operator[](std::uint32_t Id) const noexcept(true) { return GetInfoAt(Id); }

					CKPE_READ_PROPERTY(GetTopicText) const char* TopicText;
					CKPE_READ_PROPERTY(GetSubtype) std::uint16_t Subtype;
					CKPE_READ_PROPERTY(GetPriority) std::uint32_t Priority;
					CKPE_READ_PROPERTY(GetQuest) TESForm* Quest;
					CKPE_READ_PROPERTY(GetInfoList) TESTopicInfo** InfoList;
					CKPE_READ_PROPERTY(GetInfoCount) std::uint32_t InfoCount;
				private:
					char pad40[0x2];
					std::uint16_t _Subtype;
					// The data is stored in integers, but without multiplication by 100, in fact, 
					// the fractional part in the interface is always discarded, why would it be done?
					// Such data can then be stored in a byte.
					std::uint32_t _Priority;
					char pad48[0x8];
					TESForm* _Quest;
					TESTopicInfo** _InfoList;
					std::uint32_t _InfoCount;
					char pad64[0x1C];
				};
				static_assert(sizeof(TESTopic) == 0x80);
			}
		}
	}
}