// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "BGSVoiceType.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			// size = 0x400
			// func = 158
			class TESNPC : public TESForm
			{
			private:
				enum : uint32_t
				{
					nfFemale					= 0x1,
					nfEssential					= 0x2,
					nfCharGenFacePreset			= 0x4,
					nfRespawn					= 0x8,
					nfAutoCalcStats				= 0x10,
					nfUnique					= 0x20,
					nfDoesNotAffectStealthMeter = 0x40,
					nfPCLevelMult				= 0x80,			// It is always set together with the parameter nfAutoCalcStats
					nfSummonable				= 0x400,
					nfProtected					= 0x800,
					nfBleedoutOverride			= 0x4000,
					nfOppositeGenderAnims		= 0x8000,
					nfDoesNotBleed				= 0x10000,
					nfSimpleActor				= 0x100000,
					nfNoActivationOrHellos		= 0x800000,
					nfDiffuseAlphaTest			= 0x1000000,		
					nfGhost						= 0x20000000,
					nfInvulnerable				= 0x80000000,
				};
			public:
				constexpr static uint8_t TYPE_ID = ftActor;

				virtual ~TESNPC() = default;
			
				inline bool HasFemale() const { return (_NpcFlags & nfFemale) == nfFemale; }
				inline bool HasOppositeGenderAnims() const { return (_NpcFlags & nfOppositeGenderAnims) == nfOppositeGenderAnims; }
				inline bool HasAutoCalcStats() const { return (_NpcFlags & nfAutoCalcStats) == nfAutoCalcStats; }
				inline bool HasCharGenFacePreset() const { return (_NpcFlags & nfCharGenFacePreset) == nfCharGenFacePreset; }
				inline bool HasEssential() const { return (_NpcFlags & nfEssential) == nfEssential; }
				inline bool HasProtected() const { return (_NpcFlags & nfProtected) == nfProtected; }
				inline bool HasRespawn() const { return (_NpcFlags & nfRespawn) == nfRespawn; }
				inline bool HasUnique() const { return (_NpcFlags & nfUnique) == nfUnique; }
				inline bool HasDoesNotAffectStealthMeter() const { return (_NpcFlags & nfDoesNotAffectStealthMeter) == nfDoesNotAffectStealthMeter; }
				inline bool HasDiffuseAlphaTest() const { return (_NpcFlags & nfDiffuseAlphaTest) == nfDiffuseAlphaTest; }
				inline bool HasSummonable() const { return (_NpcFlags & nfSummonable) == nfSummonable; }
				inline bool HasGhost() const { return (_NpcFlags & nfGhost) == nfGhost; }
				inline bool HasInvulnerable() const { return (_NpcFlags & nfInvulnerable) == nfInvulnerable; }
				inline bool HasDoesNotBleed() const { return (_NpcFlags & nfDoesNotBleed) == nfDoesNotBleed; }
				inline bool HasSimpleActor() const { return (_NpcFlags & nfSimpleActor) == nfSimpleActor; }
				inline bool HasNoActivationOrHellos() const { return (_NpcFlags & nfNoActivationOrHellos) == nfNoActivationOrHellos; }
			private:
				char _Pad28[0x70];
				uint32_t _NpcFlags;
				uint32_t _Unk9C;
			};
			static_assert(sizeof(TESNPC) == 0xA0);
		}
	}
}
