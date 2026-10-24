// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSTArray.h>
#include <EditorAPI/B/BSTSmartPointer.h>
#include <EditorAPI/T/TESObjectREFRFwd.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class ActorValueInfo;
				class BGSKeyword;
			}

			class TBO_InstanceData;
			class BSIAudioEffectVisitorBase;
			class BSNavmeshInfo;
			class BSAnimationGraphManager;
			class BShkbAnimationGraph;
			enum class ActorValueModifier : std::int32_t;

			namespace BSISoundCategoryUtils
			{
				enum class FadeType : std::int32_t;
			}

			// Interface slots verified in CK 1.10.162 through 1.11.246.
			// Some base data remains flattened into the concrete form declarations.

			// size 0x8, 3 virtual functions
			class IKeywordFormBase
			{
			public:
				virtual ~IKeywordFormBase() = default;
				[[nodiscard]] virtual bool HasKeyword(const Forms::BGSKeyword* keyword,
					const TBO_InstanceData* data) const noexcept(true) = 0; // 008
				// keywords points to BSScrapArray<const Forms::BGSKeyword*>.
				virtual void CollectAllKeywords(void* keywords,
					const TBO_InstanceData* data) const noexcept(true) = 0; // 010
			};
			static_assert(sizeof(IKeywordFormBase) == 0x8);

			// size 0x8, 11 virtual functions
			class ActorValueOwner
			{
			public:
				virtual ~ActorValueOwner() = default;
				[[nodiscard]] virtual float GetActorValue(const Forms::ActorValueInfo&) const noexcept(true) { return 0.0f; } // 008
				[[nodiscard]] virtual float GetPermanentActorValue(const Forms::ActorValueInfo&) const noexcept(true) { return 0.0f; } // 010
				[[nodiscard]] virtual float GetBaseActorValue(const Forms::ActorValueInfo&) const noexcept(true) { return 0.0f; } // 018
				virtual void SetBaseActorValue(const Forms::ActorValueInfo&, float) noexcept(true) { return; } // 020
				virtual void ModBaseActorValue(const Forms::ActorValueInfo&, float) noexcept(true) { return; } // 028
				virtual void ModActorValue(ActorValueModifier, const Forms::ActorValueInfo&, float) noexcept(true) { return; } // 030
				[[nodiscard]] virtual float GetModifier(ActorValueModifier, const Forms::ActorValueInfo&) const noexcept(true) { return 0.0f; } // 038
				virtual void RestoreActorValue(const Forms::ActorValueInfo&, float) noexcept(true) { return; } // 040
				virtual void SetActorValue(const Forms::ActorValueInfo& info, float value) noexcept(true) { SetBaseActorValue(info, value); } // 048
				[[nodiscard]] virtual bool GetIsPlayerOwner() const noexcept(true) { return false; } // 050
			};
			static_assert(sizeof(ActorValueOwner) == 0x8);

			// size 0x8, 5 virtual functions
			class BGSOpenCloseForm
			{
			public:
				virtual ~BGSOpenCloseForm() = default;
				virtual void HandleOpenStart(Forms::TESObjectREFR*, Forms::TESObjectREFR*, bool) noexcept(true) { return; } // 008
				virtual void HandleCloseStart(Forms::TESObjectREFR*, Forms::TESObjectREFR*, bool) noexcept(true) { return; } // 010
				virtual void HandleOpenFinish(Forms::TESObjectREFR*, Forms::TESObjectREFR*) noexcept(true) { return; } // 018
				virtual void HandleCloseFinish(Forms::TESObjectREFR*, Forms::TESObjectREFR*) noexcept(true) { return; } // 020
			};
			static_assert(sizeof(BGSOpenCloseForm) == 0x8);

			// size 0x8, 4 virtual functions
			class BSIAudioEffectChain
			{
			public:
				virtual ~BSIAudioEffectChain() = default;
				[[nodiscard]] virtual std::uint32_t QEffectCount() const noexcept(true) = 0; // 008
				[[nodiscard]] virtual const char* GetDebugID() const noexcept(true) = 0; // 010
				virtual void ForEachEffectImpl(const BSIAudioEffectVisitorBase& visitor) const noexcept(true) = 0; // 018
			};
			static_assert(sizeof(BSIAudioEffectChain) == 0x8);

			// size 0x10, 11 virtual functions
			class BSIMusicTrack
			{
			public:
				enum class MusicStatus : std::int32_t;

				virtual ~BSIMusicTrack() = default;
				virtual void DoUpdate() noexcept(true) = 0; // 008
				virtual void DoPlay() noexcept(true) = 0; // 010
				virtual void DoPause() noexcept(true) = 0; // 018
				virtual void DoFinish(bool immediate, float fadeTime) noexcept(true) = 0; // 020
				[[nodiscard]] virtual float GetDurationImpl() const noexcept(true) = 0; // 028
				[[nodiscard]] virtual std::uint32_t GetType() const noexcept(true) = 0; // 030
				[[nodiscard]] virtual bool TestCanPlay() const noexcept(true) { return true; } // 038
				[[nodiscard]] virtual MusicStatus GetMusicStatus() const noexcept(true) = 0; // 040
				virtual void DoSetDuckingAttenuation(std::uint16_t) noexcept(true) { return; } // 048
				virtual void DoClearDucking() noexcept(true) { return; } // 050

				std::uint8_t unk08[0x8];
			};
			static_assert(sizeof(BSIMusicTrack) == 0x10);

			// size 0x50, 9 virtual functions
			class BSIMusicType
			{
			public:
				// CK adds GetDebugID after the destructor, unlike the game.
				virtual void DoUpdate() noexcept(true) = 0; // 000
				virtual void DoPlay() noexcept(true) = 0; // 008
				virtual void DoPause() noexcept(true) = 0; // 010
				virtual void DoFinish(bool immediate) noexcept(true) = 0; // 018
				virtual void DoApplyDuckingAttenuation(std::uint16_t) noexcept(true) { return; } // 020
				virtual void DoClearDucking() noexcept(true) { return; } // 028
				virtual void DoPrepare() noexcept(true) { return; } // 030
				virtual ~BSIMusicType() = default; // 038
				[[nodiscard]] virtual const char* GetDebugID() const noexcept(true) = 0; // 040

				/* 0x08 */ std::uint32_t flags;
				/* 0x0C */ std::int8_t priority;
				/* 0x0D */ std::uint8_t pad35;
				/* 0x0E */ std::uint16_t ducksOtherMusicBy;
				/* 0x10 */ float fadeTime;
				/* 0x14 */ std::uint32_t currentTrackIndex;
				/* 0x18 */ BSTArray<void*> trackHistory;
				/* 0x30 */ BSTArray<void*> tracks;
				/* 0x48 */ std::int32_t typeStatus;
				/* 0x4C */ std::uint32_t pad74;
			};
			static_assert(sizeof(BSIMusicType) == 0x50);
			static_assert(offsetof(BSIMusicType, flags) == 0x8);
			static_assert(offsetof(BSIMusicType, trackHistory) == 0x18);
			static_assert(offsetof(BSIMusicType, tracks) == 0x30);
			static_assert(offsetof(BSIMusicType, typeStatus) == 0x48);

			// size 0x8, 11 virtual functions
			class BSIReverbType
			{
			public:
				// This interface has no virtual destructor.
				[[nodiscard]] virtual std::int32_t DoGetRoomLevel() const noexcept(true) = 0; // 000
				[[nodiscard]] virtual std::int32_t DoGetRoomHFLevel() const noexcept(true) = 0; // 008
				[[nodiscard]] virtual float DoGetDecayTime() const noexcept(true) = 0; // 010
				[[nodiscard]] virtual float DoGetDecayHFRatio() const noexcept(true) = 0; // 018
				[[nodiscard]] virtual std::int32_t DoGetReflectionLevel() const noexcept(true) = 0; // 020
				[[nodiscard]] virtual float DoGetReflectionDelay() const noexcept(true) = 0; // 028
				[[nodiscard]] virtual std::int32_t DoGetReverbLevel() const noexcept(true) = 0; // 030
				[[nodiscard]] virtual float DoGetReverbDelay() const noexcept(true) = 0; // 038
				[[nodiscard]] virtual float DoGetDiffusion() const noexcept(true) = 0; // 040
				[[nodiscard]] virtual float DoGetDensity() const noexcept(true) = 0; // 048
				[[nodiscard]] virtual float DoGetHFReference() const noexcept(true) = 0; // 050
			};
			static_assert(sizeof(BSIReverbType) == 0x8);

			// size 0x8, 16 virtual functions
			class BSISoundCategory
			{
			public:
				virtual ~BSISoundCategory() = default;
				[[nodiscard]] virtual bool Matches(BSISoundCategory* category, bool exclusive) const noexcept(true) = 0; // 008
				[[nodiscard]] virtual float GetCategoryVolume() const noexcept(true) = 0; // 010
				virtual void SetCategoryVolume(float volume) noexcept(true) = 0; // 018
				[[nodiscard]] virtual float GetCategoryFrequency() const noexcept(true) = 0; // 020
				virtual void SetCategoryFrequency(float frequency) noexcept(true) = 0; // 028
				[[nodiscard]] virtual std::uint16_t GetCategoryAttenuation(BSISoundCategoryUtils::FadeType type) const noexcept(true) = 0; // 030
				virtual void SetCategoryAttenuation(BSISoundCategoryUtils::FadeType type, std::uint16_t attenuation) noexcept(true) = 0; // 038
				[[nodiscard]] virtual bool CategoryPaused() const noexcept(true) = 0; // 040
				virtual void SetCategoryPaused(bool paused, bool exclusive) noexcept(true) = 0; // 048
				[[nodiscard]] virtual bool CategoryMute() const noexcept(true) = 0; // 050
				virtual void SetCategoryMute(bool mute, bool exclusive) noexcept(true) = 0; // 058
				[[nodiscard]] virtual float GetMinFrequencyMult() const noexcept(true) = 0; // 060
				[[nodiscard]] virtual bool GetBlockSpeedChange() const noexcept(true) = 0; // 068
				[[nodiscard]] virtual bool GetSkipOPMOverrides() const noexcept(true) = 0; // 070
				[[nodiscard]] virtual const char* GetDebugID() const noexcept(true) = 0; // 078
			};
			static_assert(sizeof(BSISoundCategory) == 0x8);

			// size 0x8, 5 virtual functions
			class BSISoundDescriptor
			{
			public:
				struct Resolution;
				struct ExtraResolutionData;

				virtual ~BSISoundDescriptor() = default;
				[[nodiscard]] virtual bool DoResolve(Resolution& resolution, float distance,
					ExtraResolutionData* data) const noexcept(true) = 0; // 008
				// resolutions points to BSScrapArray<Resolution>.
				[[nodiscard]] virtual bool DoMultiResolve(void* resolutions,
					ExtraResolutionData* data) const noexcept(true) = 0; // 010
				[[nodiscard]] virtual bool DoAudibilityTest(float distance) const noexcept(true) = 0; // 018
				[[nodiscard]] virtual bool DoGetMultiResolves() const noexcept(true) = 0; // 020
			};
			static_assert(sizeof(BSISoundDescriptor) == 0x8);

			// size 0x8, 16 virtual functions
			class BSISoundOutputModel
			{
			public:
				struct BSIAttenuationCharacteristics;

				virtual ~BSISoundOutputModel() = default;
				[[nodiscard]] virtual bool DoGetUsesHRTF() const noexcept(true) = 0; // 008
				[[nodiscard]] virtual bool DoGetHasSpeakerBias() const noexcept(true) = 0; // 010
				[[nodiscard]] virtual bool DoGetSpeakerBias(std::uint32_t srcChannels,
					std::uint32_t channel, float (&levels)[8]) const noexcept(true) = 0; // 018
				[[nodiscard]] virtual bool DoGetAttenuatesWithDistance() const noexcept(true) = 0; // 020
				[[nodiscard]] virtual bool DoGetUseDoppler() const noexcept(true) = 0; // 028
				[[nodiscard]] virtual bool DoGetUseSoSDelay() const noexcept(true) = 0; // 030
				[[nodiscard]] virtual bool DoGetAudibility(float distance) const noexcept(true) = 0; // 038
				[[nodiscard]] virtual std::uint32_t DoGetSupportedInputChannels() const noexcept(true) = 0; // 040
				[[nodiscard]] virtual const BSIAttenuationCharacteristics* DoGetAttenuation() const noexcept(true) = 0; // 048
				[[nodiscard]] virtual float DoGetReverbSendLevel() const noexcept(true) = 0; // 050
				[[nodiscard]] virtual bool DoGetSupportsMonitor(std::uint32_t monitorID) const noexcept(true) = 0; // 058
				[[nodiscard]] virtual std::uint16_t DoGetStaticAttenuation() const noexcept(true) = 0; // 060
				[[nodiscard]] virtual const BSIAudioEffectChain* DoGetEffectChain() const noexcept(true) = 0; // 068
				[[nodiscard]] virtual const char* DoGetDebugID() const noexcept(true) = 0; // 070
				[[nodiscard]] virtual bool DoGetTryPlayThroughController() const noexcept(true) = 0; // 078
			};
			static_assert(sizeof(BSISoundOutputModel) == 0x8);

			// size 0x50, 2 virtual functions
			class BSMaterialObject
			{
			public:
				virtual ~BSMaterialObject() = default;
				virtual void EnsureLoaded() noexcept(true) { return; } // 008

				std::uint8_t unk08[0x48];
			};
			static_assert(sizeof(BSMaterialObject) == 0x50);

			// size 0x228, 2 virtual functions
			class BSNavmesh
			{
			public:
				virtual ~BSNavmesh() = default;
				[[nodiscard]] virtual std::uint32_t QNavmeshID() const noexcept(true) = 0; // 008

				std::uint8_t unk08[0x220];
			};
			static_assert(sizeof(BSNavmesh) == 0x228);

			// size 0x8, 6 virtual functions
			class BSNavmeshInfoMap
			{
			public:
				struct IVisitor;

				virtual ~BSNavmeshInfoMap() = default;
				virtual BSNavmeshInfo* GetNavMeshInfoFixID(std::uint32_t navMeshID) noexcept(true) = 0; // 008
				virtual BSNavmeshInfo* GetNavmeshInfo(std::uint32_t navMeshID) noexcept(true) = 0; // 010
				virtual void GetAllNavMeshInfo(BSTArray<BSNavmeshInfo*>* result) noexcept(true) = 0; // 018
				virtual void BuildListOfConnectedInfos(const BSNavmeshInfo* info,
					BSTArray<BSNavmeshInfo*>* result) noexcept(true) = 0; // 020
				virtual void ForEach(IVisitor& visitor) noexcept(true) = 0; // 028

			};
			static_assert(sizeof(BSNavmeshInfoMap) == 0x8);

			// size 0x8, 26 virtual functions
			class IAnimationGraphManagerHolder
			{
			public:
				virtual ~IAnimationGraphManagerHolder() = default;
				virtual bool NotifyAnimationGraphImpl(const BSFixedString& eventName) noexcept(true) = 0; // 008
				virtual void EventSuccessfullyProcessed() noexcept(true) { return; } // 010
				virtual void ModifyInitialAnimationStateImpl(bool) noexcept(true) { return; } // 018
				[[nodiscard]] virtual bool GetAnimationGraphManagerImpl(
					BSTSmartPointer<BSAnimationGraphManager>& manager) const noexcept(true) = 0; // 020
				virtual bool SetAnimationGraphManagerImpl(
					const BSTSmartPointer<BSAnimationGraphManager>& manager) noexcept(true) = 0; // 028
				// nodesToAnimate points to BSScrapArray<NiAPI::NiAVObject*>.
				[[nodiscard]] virtual bool PopulateGraphNodesToTarget(void* nodesToAnimate) const noexcept(true) { return false; } // 030
				virtual bool ConstructAnimationGraph(BSTSmartPointer<BShkbAnimationGraph>& graph) noexcept(true) = 0; // 038
				[[nodiscard]] virtual bool InitializeAnimationGraphVariables(
					const BSTSmartPointer<BShkbAnimationGraph>& graph) const noexcept(true) { return true; } // 040
				virtual bool SetupAnimEventSinks(const BSTSmartPointer<BShkbAnimationGraph>& graph) noexcept(true) { return true; } // 048
				virtual void DoFailedToLoadGraph() const noexcept(true) { return; } // 050
				// channels points to BSScrapArray of animation-channel smart pointers.
				virtual bool CreateAnimationChannels(void* channels) noexcept(true) { return false; } // 058
				virtual void PostCreateAnimationGraphManager(
					BSTSmartPointer<BSAnimationGraphManager>& manager) noexcept(true) { return; } // 060
				virtual void PostChangeAnimationManager(
					const BSTSmartPointer<BSAnimationGraphManager>& newManager,
					const BSTSmartPointer<BSAnimationGraphManager>& oldManager) noexcept(true) { return; } // 068
				virtual bool ShouldUpdateAnimation() noexcept(true) { return true; } // 070
				[[nodiscard]] virtual std::uint32_t GetGraphVariableCacheSize() const noexcept(true) { return 0; } // 078
				// Unique names avoid MSVC reordering same-name virtual overloads.
				[[nodiscard]] virtual bool GetGraphVariableByIDInt(std::uint32_t graphVarID, std::int32_t& result) const noexcept(true) { return false; } // 080
				[[nodiscard]] virtual bool GetGraphVariableByIDBool(std::uint32_t graphVarID, bool& result) const noexcept(true) { return false; } // 088
				[[nodiscard]] virtual bool GetGraphVariableByIDFloat(std::uint32_t graphVarID, float& result) const noexcept(true) { return false; } // 090
				[[nodiscard]] virtual bool GetGraphVariableImplFloat(const BSFixedString& variable, float& result) const noexcept(true) = 0; // 098
				[[nodiscard]] virtual bool GetGraphVariableImplInt(const BSFixedString& variable, std::int32_t& result) const noexcept(true) = 0; // 0A0
				[[nodiscard]] virtual bool GetGraphVariableImplBool(const BSFixedString& variable, bool& result) const noexcept(true) = 0; // 0A8
				virtual void PreUpdateAnimationGraphManager(
					const BSTSmartPointer<BSAnimationGraphManager>& manager) const noexcept(true) { return; } // 0B0
				virtual void PostUpdateAnimationGraphManager(
					const BSTSmartPointer<BSAnimationGraphManager>& manager) const noexcept(true) { return; } // 0B8
				virtual void PreLoadAnimationGraphManager(
					const BSTSmartPointer<BSAnimationGraphManager>& manager) noexcept(true) { return; } // 0C0
				virtual void PostLoadAnimationGraphManager(
					const BSTSmartPointer<BSAnimationGraphManager>& manager) noexcept(true) { return; } // 0C8
			};
			static_assert(sizeof(IAnimationGraphManagerHolder) == 0x8);

			// Empty helper base (no data).
			class BGSNavmeshableObject {};

			// Empty helper base (no data).
			class TESMagicCasterForm {};

			// Empty helper base (no data).
			class TESMagicTargetForm {};

			// Empty helper base, singleton marker.
			template<class T>
			class BSTSingletonImplicit {};

		}
	}
}
