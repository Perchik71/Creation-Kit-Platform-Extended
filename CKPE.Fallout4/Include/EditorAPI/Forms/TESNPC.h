// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TESObject.h"
#include "BGSVoiceType.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			class TESNPC;

			// size = 0x40
			class TESActorBaseData
			{
			public:
				virtual ~TESActorBaseData() = default;

				enum TESActorFlags : uint64_t
				{
					nfFemale = 0x1,
					nfEssential = 0x2,
					nfCharGenFacePreset = 0x4,
					nfRespawn = 0x8,
					nfAutoCalcStats = 0x10,
					nfUnique = 0x20,
					nfDoesNotAffectStealthMeter = 0x40,
					nfPCLevelMult = 0x80,			// It is always set together with the parameter nfAutoCalcStats
					nfSummonable = 0x400,
					nfProtected = 0x800,
					nfBleedoutOverride = 0x4000,
					nfOppositeGenderAnims = 0x8000,
					nfDoesNotBleed = 0x10000,
					nfSimpleActor = 0x100000,
					nfNoActivationOrHellos = 0x800000,
					nfDiffuseAlphaTest = 0x1000000,
					nfGhost = 0x20000000,
					nfInvulnerable = 0x80000000,
				};

				enum TESActorTemplateFlags : uint64_t
				{
					tfTraits = 1 << 16,
					tfStats = 1 << 17,	
					tfFactions = 1 << 18,
					tfSpellList = 1 << 19,
					tfAIData = 1 << 20,
					tfAIPackage = 1 << 21,
					tfBaseData = 1 << 23,
					tfInventory = 1 << 24,
					tfScripts = 1 << 25,
					tfDefaultPackList = 1 << 26,
					tfAttackData = 1 << 27,
					tfKeywowrds = 1 << 28,
				};

				TESActorFlags Flags;					// 08
				TESActorTemplateFlags TemplateFlags;	// 10
				uint64_t unk18;							// 18
				uint64_t unk20;							// 20
				BGSVoiceType* VoiceType;				// 28
				TESNPC*	DefaultTemplate;				// 30
				
				struct TemplateInfo
				{
					TESNPC* Traits;				// 00
					TESNPC* Stats;				// 08			
					TESNPC* Factions;			// 10
					TESNPC* SpellList;			// 18
					TESNPC* AIData;				// 20
					TESNPC* AIPackage;			// 28
					char pad30[0x8];			// 30 ??? nullptr
					TESNPC* BaseData;			// 38
					TESNPC* Inventory;			// 40
					TESNPC* Scripts;			// 48
					TESNPC* DefaultPackList;	// 50
					TESNPC* AttackData;			// 58					
					TESNPC* Keywowrds;			// 60
				};

				TemplateInfo* Templates;
			};
			static_assert(sizeof(TESActorBaseData::TemplateInfo) == 0x68);
			static_assert(sizeof(TESActorBaseData) == 0x40);


			// size = 0x400
			// func = 158
			class TESNPC : public TESBoundAnimObject
			{
			public:
				constexpr static uint8_t TYPE_ID = ftActor;

				virtual ~TESNPC() = default;

				TESActorBaseData* GetActorData() noexcept(true) { return &ActorData; }
			
				[[nodiscard]] inline bool HasFemale() const noexcept(true) { return (ActorData.Flags & TESActorBaseData::nfFemale) == TESActorBaseData::nfFemale; }
				[[nodiscard]] inline bool HasOppositeGenderAnims() const noexcept(true) { return (ActorData.Flags & TESActorBaseData::nfOppositeGenderAnims) == TESActorBaseData::nfOppositeGenderAnims; }
				[[nodiscard]] inline bool HasAutoCalcStats() const noexcept(true) { return (ActorData.Flags & TESActorBaseData::nfAutoCalcStats) == TESActorBaseData::nfAutoCalcStats; }
				[[nodiscard]] inline bool HasCharGenFacePreset() const noexcept(true) { return (ActorData.Flags & TESActorBaseData::nfCharGenFacePreset) == TESActorBaseData::nfCharGenFacePreset; }
				[[nodiscard]] inline bool HasEssential() const noexcept(true) { return (ActorData.Flags & TESActorBaseData::nfEssential) == TESActorBaseData::nfEssential; }
				[[nodiscard]] inline bool HasProtected() const noexcept(true) { return (ActorData.Flags & TESActorBaseData::nfProtected) == TESActorBaseData::nfProtected; }
				[[nodiscard]] inline bool HasRespawn() const noexcept(true) { return (ActorData.Flags & TESActorBaseData::nfRespawn) == TESActorBaseData::nfRespawn; }
				[[nodiscard]] inline bool HasUnique() const noexcept(true) { return (ActorData.Flags & TESActorBaseData::nfUnique) == TESActorBaseData::nfUnique; }
				[[nodiscard]] inline bool HasDoesNotAffectStealthMeter() const noexcept(true) { return (ActorData.Flags & TESActorBaseData::nfDoesNotAffectStealthMeter) == TESActorBaseData::nfDoesNotAffectStealthMeter; }
				[[nodiscard]] inline bool HasDiffuseAlphaTest() const noexcept(true) { return (ActorData.Flags & TESActorBaseData::nfDiffuseAlphaTest) == TESActorBaseData::nfDiffuseAlphaTest; }
				[[nodiscard]] inline bool HasSummonable() const noexcept(true) { return (ActorData.Flags & TESActorBaseData::nfSummonable) == TESActorBaseData::nfSummonable; }
				[[nodiscard]] inline bool HasGhost() const noexcept(true) { return (ActorData.Flags & TESActorBaseData::nfGhost) == TESActorBaseData::nfGhost; }
				[[nodiscard]] inline bool HasInvulnerable() const noexcept(true) { return (ActorData.Flags & TESActorBaseData::nfInvulnerable) == TESActorBaseData::nfInvulnerable; }
				[[nodiscard]] inline bool HasDoesNotBleed() const noexcept(true) { return (ActorData.Flags & TESActorBaseData::nfDoesNotBleed) == TESActorBaseData::nfDoesNotBleed; }
				[[nodiscard]] inline bool HasSimpleActor() const noexcept(true) { return (ActorData.Flags & TESActorBaseData::nfSimpleActor) == TESActorBaseData::nfSimpleActor; }
				[[nodiscard]] inline bool HasNoActivationOrHellos() const noexcept(true) { return (ActorData.Flags & TESActorBaseData::nfNoActivationOrHellos) == TESActorBaseData::nfNoActivationOrHellos; }
			private:
				TESActorBaseData ActorData;
			};
			static_assert(sizeof(TESNPC) == 0xD0);
		}
	}
}
