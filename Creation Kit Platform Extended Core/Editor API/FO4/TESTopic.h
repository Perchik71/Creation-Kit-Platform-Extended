// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "NiAPI/NiTSimpleArray.h"
#include "TESTopicInfo.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			class TESQuest;
			// size = 0x98
			// func = 124
			class TESTopic : public TESForm
			{
			public:
				constexpr static uint8_t TYPE_ID = ftTopic;

				enum SubtypeT : uint8_t
				{
					stObserveCombat				= 0x54,
					stNoticeCorpose				= 0x55,
					stTimeToGo					= 0x56,
					stGoodbye					= 0x57,
					stSwingMeleeWeapon			= 0x59,
					stShootBow					= 0x5A,
					stZKeyObject				= 0x5B,
					stJump						= 0x5C,
					stKnockOverObject			= 0x5D,
					stDestroyObject				= 0x5E,
					stStandonFurniture			= 0x5F,
					stLockedObject				= 0x60,
					stPickpocketTopic			= 0x61,
					stPursueIdleTopic			= 0x62,
					stSceneChoice				= 0x64,
					stPlayerCastProjectileSpell	= 0x65,
					stPlayerCastSelfSpell		= 0x66,
					stPlayerShout				= 0x67,
					stIdle						= 0x68,
					stEnterSprintBreath			= 0x69,
					stEnterBowZoomBreath		= 0x6A,
					stExitBowZoomBreath			= 0x6B,
					stActorCollidewithActor		= 0x6C,
					stPlayerinIronSights		= 0x6D,
					stOutofBreath				= 0x6E,
					stCombatGrunt				= 0x6F,
					stLeaveWaterBreath			= 0x70,
					stImpatientPositive			= 0x71,
					stImpatientNegative			= 0x72,
					stImpatientNatural			= 0x73,
					stImpatientQuestion			= 0x74,
					stGreeting					= 0x76,
					stPlayerActiveDoor			= 0x77,
					stPlayerActiveTerminals		= 0x78,
					stPlayerActiveFurniture		= 0x79,
					stPlayerActiveActivators	= 0x7A,
					stPlayerActiveContainer		= 0x7B,
					stPlayerAquireFeaturedItem	= 0x7C,
				};

				virtual ~TESTopic() = default;
			public:
				inline const char* GetTopicText() const { return _TopicText.c_str(); }
				inline SubtypeT GetSubtype() const { return _Subtype; }
				inline uint8_t GetPriority() const { return _Priority; }
				inline TESQuest* GetQuest() const { return _Quest[0]; }
				inline NiAPI::NiTSimpleArray<TESTopicInfo*>& GetInfos() { return _Infos; }

				READ_PROPERTY(GetTopicText) const char* TopicText;
				READ_PROPERTY(GetSubtype) SubtypeT Subtype;
				READ_PROPERTY(GetPriority) uint8_t Priority;
				READ_PROPERTY(GetQuest) TESQuest* Quest;
				READ_PROPERTY(GetInfos) NiAPI::NiTSimpleArray<TESTopicInfo*>& Infos;
			private:
				char pad28[0x8];
				BGSLocalizedStringIL _TopicText;
				char pad40[0x2];
				SubtypeT _Subtype;
				char pad43[0x4];
				uint8_t _Priority;
				char pad48[0x8];
				TESQuest* _Quest[2];
				char pad60[0x8];
				NiAPI::NiTSimpleArray<TESTopicInfo*> _Infos;
				char pad78[0x20];
			};
			static_assert(sizeof(TESTopic) == 0x98);
		}
	}
}

#pragma pack(pop)