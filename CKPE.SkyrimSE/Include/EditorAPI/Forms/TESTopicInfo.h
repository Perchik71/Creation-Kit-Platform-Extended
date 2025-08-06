// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.h>
#include <EditorAPI/BSString.h>
#include <EditorAPI/BGSLocalizedString.h>
#include "TESForm.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESTopic;
				// size 0xB0
				// func 103
				class TESTopicInfo : public TESForm
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftTopicInfo;

					enum : std::uint16_t
					{
						tifGoodbye = 0x1,
						tifRandom = 0x2,
						tifSayOnce = 0x4,
						tifRandomEnd = 0x20,
						tifInvisibleContinue = 0x40,
						tifWalkAway = 0x80,
						tifWalkAwayInvisibleInMenu = 0x100,
						tifForceSubtitle = 0x200,
						tifCanMoveWhileGreeting = 0x400,
						tifNoHasLip = 0x800,
						tifRequiresPostProcessing = 0x1000,
						tifSpendsFavorPoints = 0x4000,
					};

					struct Condition
					{
						enum FunctionT : std::uint32_t
						{

						};

						Condition* Next;
						float Value;
						char padC[0xC];
						FunctionT Function;
						char pad1C[0x4];
						TESForm* Arg1;
						TESForm* Arg2;
						std::uint32_t Flags;
						char pad34[0x4];

						class Interator
						{
						public:
							inline Interator(Condition* Cond) noexcept(true) : _Cond(Cond) {}
							inline Interator(const Interator& It) noexcept(true) : _Cond(It._Cond) {}

							inline Interator& operator++() noexcept(true)
							{
								if (!End())
									_Cond = _Cond->Next;
								return *this;
							}
							inline Interator operator++(int) noexcept(true)
							{
								Interator It = *this;
								if (!End())
									_Cond = _Cond->Next;
								return It;
							}
							inline bool End() { return _Cond == nullptr; }

							inline Condition* operator->() const noexcept(true) { return _Cond; }
							inline Condition* operator*() const noexcept(true) { return _Cond; }
						private:
							Condition* _Cond;
						};
					};

					struct Response
					{
						enum EmotionT : std::uint32_t
						{
							etNeutral = 0,
							etAnger,
							etDisgust,
							etFear,
							etSad,
							etHappy,
							etSuprise,
							etPuzzled
						};

						EmotionT EmotionType;
						std::uint32_t EmotionValue;
						char pad8[0x8];
						std::uint32_t ResponseId;
						char pad14[0x4];
						TESForm* SoundFile;
						std::uint8_t UseEmotionAnimation;
						char pad21[0x7];
						BGSLocalizedString ResponseText;
						TESForm* Speaker;
						TESForm* Listener;
						Response* ResponseNext;
						BSString ScriptNotes;
						BSString Edits;
						char pad70[0x8];

						class Interator
						{
						public:
							inline Interator(Response* Resp) noexcept(true) : _Resp(Resp) {}
							inline Interator(const Interator& It) noexcept(true) : _Resp(It._Resp) {}

							inline Interator& operator++() noexcept(true)
							{
								if (!End())
									_Resp = _Resp->ResponseNext;
								return *this;
							}
							inline Interator operator++(int) noexcept(true)
							{
								Interator It = *this;
								if (!End())
									_Resp = _Resp->ResponseNext;
								return It;
							}
							inline bool End() noexcept(true) { return _Resp == nullptr; }

							inline Response* operator->() const noexcept(true) { return _Resp; }
							inline Response* operator*() const noexcept(true) { return _Resp; }
						private:
							Response* _Resp;
						};
					};
				public:
					virtual ~TESTopicInfo() = default;

					[[nodiscard]] inline TESTopic* GetParentTopic() const noexcept(true) { return _ParentTopic; }
					[[nodiscard]] inline TESTopicInfo* GetSharedTopicInfo() const noexcept(true) { return _SharedTopicInfo; }
					[[nodiscard]] inline TESForm* GetSpeaker() const noexcept(true) { return _Speaker; }
					[[nodiscard]] inline const char* GetPromptText() const noexcept(true) { return _PromptText.c_str(); }
					[[nodiscard]] inline float GetHoursUntilReset() const noexcept(true) { return (float)((double)(_HoursUntilReset) / 2730.625f); }
					[[nodiscard]] inline std::uint8_t GetFavorLevel() const noexcept(true) { return _FavorLevel; }
					[[nodiscard]] inline Response* GetFirstResponse() const noexcept(true) { return _ResponseFirst; }
					[[nodiscard]] inline Response::Interator GetFirstResponseIterator() const noexcept(true) { return _ResponseFirst; }
					[[nodiscard]] inline Condition* GetFirstCondition() const noexcept(true) { return _ConditionFirst; }
					[[nodiscard]] inline Condition::Interator GetFirstConditionIterator() const noexcept(true) { return _ConditionFirst; }

					[[nodiscard]] inline bool HasShared() const noexcept(true) { return _SharedTopicInfo != nullptr; }
					[[nodiscard]] inline bool HasGoodbye() const noexcept(true) { return (_Flags & tifGoodbye) == tifGoodbye; }
					[[nodiscard]] inline bool HasRandom() const noexcept(true) { return (_Flags & tifRandom) == tifRandom; }
					[[nodiscard]] inline bool HasSayOnce() const noexcept(true) { return (_Flags & tifSayOnce) == tifSayOnce; }
					[[nodiscard]] inline bool HasRandomEnd() const noexcept(true) { return (_Flags & tifRandomEnd) == tifRandomEnd; }
					[[nodiscard]] inline bool HasInvisibleContinue() const noexcept(true) { return (_Flags & tifInvisibleContinue) == tifInvisibleContinue; }
					[[nodiscard]] inline bool HasWalkAway() const noexcept(true) { return (_Flags & tifWalkAway) == tifWalkAway; }
					[[nodiscard]] inline bool HasWalkAwayInvisibleInMenu() const noexcept(true) { return (_Flags & tifWalkAwayInvisibleInMenu) == tifWalkAwayInvisibleInMenu; }
					[[nodiscard]] inline bool HasForceSubtitle() const noexcept(true) { return (_Flags & tifForceSubtitle) == tifForceSubtitle; }
					[[nodiscard]] inline bool HasCanMoveWhileGreeting() const noexcept(true) { return (_Flags & tifCanMoveWhileGreeting) == tifCanMoveWhileGreeting; }
					[[nodiscard]] inline bool HasNoHasLip() const { return (_Flags & tifNoHasLip) == tifNoHasLip; }
					[[nodiscard]] inline bool HasRequiresPostProcessing() const noexcept(true) { return (_Flags & tifRequiresPostProcessing) == tifRequiresPostProcessing; }
					[[nodiscard]] inline bool HasSpendsFavorPoints() const noexcept(true) { return (_Flags & tifSpendsFavorPoints) == tifSpendsFavorPoints; }

					CKPE_READ_PROPERTY(GetParentTopic) TESTopic* ParentTopic;
					CKPE_READ_PROPERTY(GetSharedTopicInfo) TESTopicInfo* SharedTopicInfo;
					CKPE_READ_PROPERTY(GetSpeaker) TESForm* Speaker;
					CKPE_READ_PROPERTY(GetPromptText) const char* PromptText;
					CKPE_READ_PROPERTY(GetHoursUntilReset) float HoursUntilReset;
					CKPE_READ_PROPERTY(GetFavorLevel) std::uint8_t FavorLevel;
				private:
					char pad28[0x8];
					TESTopic* _ParentTopic;
					TESTopicInfo* _SharedTopicInfo;
					Condition* _ConditionFirst;
					char pad48[0xA];
					std::uint8_t _FavorLevel;
					std::uint8_t _Unk53;
					std::uint16_t _Flags;
					// Bethesda is a lover of perversion (What's the point of such a type here?) 
					// 1.00 == 2730,625 (65535 max 16bit / 24 hours)
					std::uint16_t _HoursUntilReset;
					TESForm* _Speaker;
					char pad60[0x8];
					BGSLocalizedString _PromptText;
					Response* _ResponseFirst;
				};
				static_assert(sizeof(TESTopicInfo::Condition) == 0x38);
				static_assert(sizeof(TESTopicInfo::Response) == 0x78);
				static_assert(sizeof(TESTopicInfo) == 0x80);
			}
		}
	}
}