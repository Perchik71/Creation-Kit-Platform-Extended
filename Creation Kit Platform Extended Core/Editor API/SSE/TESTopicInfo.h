// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "..\BGSLocalizedString.h"
#include "TESForm.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class TESTopic;
			// size 0xB0
			// func 103
			class TESTopicInfo : public TESForm
			{
			public:
				constexpr static uint8_t TYPE_ID = ftTopicInfo;
				
				enum : uint16_t
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
					enum FunctionT : uint32_t
					{

					};

					Condition* Next;
					float Value;
					char padC[0xC];
					FunctionT Function;
					char pad1C[0x4];
					TESForm* Arg1;
					TESForm* Arg2;
					uint32_t Flags;
					char pad34[0x4];

					class Interator
					{
					public:
						inline Interator(Condition* Cond) : _Cond(Cond) {}
						inline Interator(const Interator& It) : _Cond(It._Cond) {}

						inline Interator& operator++()
						{
							if (!End())
								_Cond = _Cond->Next;
							return *this;
						}
						inline Interator operator++(int)
						{
							Interator It = *this;
							if (!End())
								_Cond = _Cond->Next;
							return It;
						}
						inline bool End() { return _Cond == nullptr; }

						inline Condition* operator->() const { return _Cond; }
						inline Condition* operator*() const { return _Cond; }
					private:
						Condition* _Cond;
					};
				};

				struct Response
				{
					enum EmotionT : uint32_t
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
					uint32_t EmotionValue;
					char pad8[0x8];
					uint32_t ResponseId;
					char pad14[0x4];
					TESForm* SoundFile;
					uint8_t UseEmotionAnimation;
					char pad21[0x7];
					BGSLocalizedString<char> ResponseText;
					TESForm* Speaker;
					TESForm* Listener;
					Response* ResponseNext;
					BSString ScriptNotes;
					BSString Edits;
					char pad70[0x8];

					class Interator
					{
					public:
						inline Interator(Response* Resp) : _Resp(Resp) {}
						inline Interator(const Interator& It) : _Resp(It._Resp) {}

						inline Interator& operator++() 
						{
							if (!End())
								_Resp = _Resp->ResponseNext; 
							return *this; 
						}
						inline Interator operator++(int)
						{
							Interator It = *this;
							if (!End())
								_Resp = _Resp->ResponseNext;
							return It;
						}
						inline bool End() { return _Resp == nullptr; }

						inline Response* operator->() const { return _Resp; }
						inline Response* operator*() const { return _Resp; }
					private:
						Response* _Resp;
					};
				};
			public:
				virtual ~TESTopicInfo() = default;

				inline TESTopic* GetParentTopic() const { return _ParentTopic; }
				inline TESTopicInfo* GetSharedTopicInfo() const { return _SharedTopicInfo; }
				inline TESForm* GetSpeaker() const { return _Speaker; }
				inline const char* GetPromptText() const { return _PromptText.c_str(); }
				inline float GetHoursUntilReset() const { return (float)((double)(_HoursUntilReset) / 2730.625f); }
				inline uint8_t GetFavorLevel() const { return _FavorLevel; }
				inline Response* GetFirstResponse() const { return _ResponseFirst; }
				inline Response::Interator GetFirstResponseIterator() const { return _ResponseFirst; }
				inline Condition* GetFirstCondition() const { return _ConditionFirst; }
				inline Condition::Interator GetFirstConditionIterator() const { return _ConditionFirst; }

				inline bool HasShared() const { return _SharedTopicInfo != nullptr; }
				inline bool HasGoodbye() const { return (_Flags & tifGoodbye) == tifGoodbye; }
				inline bool HasRandom() const { return (_Flags & tifRandom) == tifRandom; }
				inline bool HasSayOnce() const { return (_Flags & tifSayOnce) == tifSayOnce; }
				inline bool HasRandomEnd() const { return (_Flags & tifRandomEnd) == tifRandomEnd; }
				inline bool HasInvisibleContinue() const { return (_Flags & tifInvisibleContinue) == tifInvisibleContinue; }
				inline bool HasWalkAway() const { return (_Flags & tifWalkAway) == tifWalkAway; }
				inline bool HasWalkAwayInvisibleInMenu() const { return (_Flags & tifWalkAwayInvisibleInMenu) == tifWalkAwayInvisibleInMenu; }
				inline bool HasForceSubtitle() const { return (_Flags & tifForceSubtitle) == tifForceSubtitle; }
				inline bool HasCanMoveWhileGreeting() const { return (_Flags & tifCanMoveWhileGreeting) == tifCanMoveWhileGreeting; }
				inline bool HasNoHasLip() const { return (_Flags & tifNoHasLip) == tifNoHasLip; }
				inline bool HasRequiresPostProcessing() const { return (_Flags & tifRequiresPostProcessing) == tifRequiresPostProcessing; }
				inline bool HasSpendsFavorPoints() const { return (_Flags & tifSpendsFavorPoints) == tifSpendsFavorPoints; }

				READ_PROPERTY(GetParentTopic) TESTopic* ParentTopic;
				READ_PROPERTY(GetSharedTopicInfo) TESTopicInfo* SharedTopicInfo;
				READ_PROPERTY(GetSpeaker) TESForm* Speaker;
				READ_PROPERTY(GetPromptText) const char* PromptText;
				READ_PROPERTY(GetHoursUntilReset) float HoursUntilReset;
				READ_PROPERTY(GetFavorLevel) uint8_t FavorLevel;
			private:
				char pad28[0x8];
				TESTopic* _ParentTopic;
				TESTopicInfo* _SharedTopicInfo;
				Condition* _ConditionFirst;
				char pad48[0xA];
				uint8_t _FavorLevel;
				uint8_t _Unk53;
				uint16_t _Flags;
				// Bethesda is a lover of perversion (What's the point of such a type here?) 
				// 1.00 == 2730,625 (65535 max 16bit / 24 hours)
				uint16_t _HoursUntilReset;
				TESForm* _Speaker;
				char pad60[0x8];
				BGSLocalizedString<char> _PromptText;
				Response* _ResponseFirst;
			};
			static_assert(sizeof(TESTopicInfo::Condition) == 0x38);
			static_assert(sizeof(TESTopicInfo::Response) == 0x78);
			static_assert(sizeof(TESTopicInfo) == 0x80);
		}
	}
}

#pragma pack(pop)