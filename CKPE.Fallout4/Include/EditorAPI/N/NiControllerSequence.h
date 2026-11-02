// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/N/NiObject.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/IDs_RTTI.h>
#include <EditorAPI/IDs_VTABLE.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiAVObject;
				class NiBlendInterpolator;
				class NiControllerManager;
				class NiInterpController;
				class NiInterpolator;
				class NiTextKeyExtraData;
				class NiStringPalette;

				// Layout and size (0xB8, the allocation size at every constructor call site) are the same in 1.10.162.0,
				// 1.10.982.3, 1.11.137.0, 1.11.240.0 and 1.11.246.0.
				class NiControllerSequence : public NiObject
				{
				public:
					constexpr static Common::ID RTTI = IDs::RTTI::NiControllerSequence;
					constexpr static Common::ID VTABLE = IDs::VTABLE::NiControllerSequence[0];

					enum CycleType : std::uint32_t
					{
						ctLoop = 0,
						ctReverse,
						ctClamp,
					};

					enum State : std::uint32_t
					{
						sInactive = 0,
						sAnimating,
						sEaseIn,
						sEaseOut,
						sTransitionSource,
						sTransitionDestination,
						sMorphSource,
					};

					// One per controlled block, the interpolator and the controller are streamed.
					struct InterpArrayItem
					{
						NiInterpolator* Interpolator;			// 00
						NiInterpController* Controller;			// 08
						NiBlendInterpolator* BlendInterpolator;	// 10 - runtime only
						std::uint8_t BlendIndex;				// 18
						std::uint8_t Priority;					// 19
						std::uint8_t _pad1A[6];					// 1A
					};
					static_assert(sizeof(InterpArrayItem) == 0x20);

					// Names that identify a controlled block (the target node, the property and the controller).
					struct IDTag
					{
						BSFixedString AVObjectName;				// 00
						BSFixedString PropertyType;				// 08
						BSFixedString ControllerType;			// 10
						BSFixedString ControllerID;				// 18
						BSFixedString InterpolatorID;			// 20
					};
					static_assert(sizeof(IDTag) == 0x28);

					/* 02 */ virtual NiRTTI* GetRTTI() const noexcept(true) override;
					/* 1A */ virtual NiObject* CreateClone(NiCloningProcess& Process) noexcept(true) override;
					/* 1B */ virtual void LoadBinary(NiStream& Stream) noexcept(true) override;
					/* 1C */ virtual void LinkObject(NiStream& Stream) noexcept(true) override;
					/* 1D */ virtual bool RegisterStreamables(NiStream& Stream) noexcept(true) override;
					/* 1E */ virtual void SaveBinary(NiStream& Stream) noexcept(true) override;
					/* 1F */ virtual bool IsEqual(NiObject* Object) noexcept(true) override;
					/* 20 */ virtual void GetViewerStringsArray(void* Strings) noexcept(true) override;
					/* 21 */ virtual void ProcessClone(NiCloningProcess& Process) noexcept(true) override;
					/* 22 */ virtual void PostLinkObject(NiStream& Stream) noexcept(true) override;
					// Ends the sequence at once (EaseOutTime <= 0), or starts the ease out (EASEOUT, or TRANSITION SOURCE with Transition).
					/* 2A */ virtual bool Deactivate(float EaseOutTime, bool Transition) noexcept(true);

					// Starts the sequence on its manager; false when it cannot be activated.
					bool Activate(std::uint8_t Priority, bool StartOver, float Weight, float EaseInTime,
						NiControllerSequence* TimeSync, bool Transition) noexcept(true);

					[[nodiscard]] inline const char* GetName() const noexcept(true) { return _Name; }
					[[nodiscard]] inline std::uint32_t GetControlledBlockCount() const noexcept(true) { return _ArraySize; }
					[[nodiscard]] inline float GetWeight() const noexcept(true) { return _Weight; }
					[[nodiscard]] inline CycleType GetCycleType() const noexcept(true) { return _CycleType; }
					[[nodiscard]] inline float GetFrequency() const noexcept(true) { return _Frequency; }
					[[nodiscard]] inline float GetBeginKeyTime() const noexcept(true) { return _BeginKeyTime; }
					[[nodiscard]] inline float GetEndKeyTime() const noexcept(true) { return _EndKeyTime; }
					[[nodiscard]] inline NiControllerManager* GetOwner() const noexcept(true) { return _Owner; }
					[[nodiscard]] inline State GetState() const noexcept(true) { return _State; }
					[[nodiscard]] inline bool IsInactive() const noexcept(true) { return _State == State::sInactive; }
					[[nodiscard]] inline NiControllerSequence* GetPartnerSequence() const noexcept(true) { return _PartnerSequence; }

					// The offset between the manager time and the sequence time is computed again on the next update;
					// InitNonNPCAnimation stores this value after activating a sequence.
					inline void ResetOffset() noexcept(true) { _Offset = -3.402823466e+38F; }

					BSFixedString _Name;								// 10
					std::uint32_t _ArraySize;							// 18 - number of controlled blocks
					std::uint32_t _ArrayGrowBy;							// 1C
					InterpArrayItem* _InterpArray;						// 20
					IDTag* _IDTagArray;									// 28
					float _Weight;										// 30
					std::uint32_t _pad34;								// 34
					NiTextKeyExtraData* _TextKeys;						// 38 - reference counted
					CycleType _CycleType;								// 40
					float _Frequency;									// 44
					float _BeginKeyTime;								// 48 - FLT_MAX until set
					float _EndKeyTime;									// 4C
					float _LastTime;									// 50
					float _WeightedLastTime;							// 54
					float _LastScaledTime;								// 58
					std::uint32_t _pad5C;								// 5C
					NiControllerManager* _Owner;						// 60
					State _State;										// 68
					float _Offset;										// 6C
					float _StartTime;									// 70
					float _EndTime;										// 74
					float _DestFrame;									// 78
					std::uint32_t _pad7C;								// 7C
					NiControllerSequence* _PartnerSequence;				// 80 - time sync sequence
					BSFixedString _AccumRootName;						// 88
					NiAVObject* _AccumRoot;								// 90
					NiStringPalette* _StringPalette;					// 98 - old files only, released in PostLinkObject
					std::uint16_t _unkA0;								// A0 - initialized to 0xFFFF, not used by the functions recovered
					std::uint8_t _padA2[6];								// A2
					NiObject** _AnimNotes;								// A8 - reference counted, _AnimNoteCount entries
					std::uint16_t _AnimNoteCount;						// B0
					std::uint8_t _unkB2;								// B2 - initialized to 0, not used by the functions recovered
					std::uint8_t _padB3[5];								// B3
				};
				static_assert(sizeof(NiControllerSequence) == 0xB8);
				static_assert(offsetof(NiControllerSequence, _Name) == 0x10);
				static_assert(offsetof(NiControllerSequence, _ArraySize) == 0x18);
				static_assert(offsetof(NiControllerSequence, _InterpArray) == 0x20);
				static_assert(offsetof(NiControllerSequence, _IDTagArray) == 0x28);
				static_assert(offsetof(NiControllerSequence, _Weight) == 0x30);
				static_assert(offsetof(NiControllerSequence, _TextKeys) == 0x38);
				static_assert(offsetof(NiControllerSequence, _CycleType) == 0x40);
				static_assert(offsetof(NiControllerSequence, _Frequency) == 0x44);
				static_assert(offsetof(NiControllerSequence, _BeginKeyTime) == 0x48);
				static_assert(offsetof(NiControllerSequence, _EndKeyTime) == 0x4C);
				static_assert(offsetof(NiControllerSequence, _LastTime) == 0x50);
				static_assert(offsetof(NiControllerSequence, _WeightedLastTime) == 0x54);
				static_assert(offsetof(NiControllerSequence, _LastScaledTime) == 0x58);
				static_assert(offsetof(NiControllerSequence, _Owner) == 0x60);
				static_assert(offsetof(NiControllerSequence, _State) == 0x68);
				static_assert(offsetof(NiControllerSequence, _Offset) == 0x6C);
				static_assert(offsetof(NiControllerSequence, _StartTime) == 0x70);
				static_assert(offsetof(NiControllerSequence, _EndTime) == 0x74);
				static_assert(offsetof(NiControllerSequence, _DestFrame) == 0x78);
				static_assert(offsetof(NiControllerSequence, _PartnerSequence) == 0x80);
				static_assert(offsetof(NiControllerSequence, _AccumRootName) == 0x88);
				static_assert(offsetof(NiControllerSequence, _AccumRoot) == 0x90);
				static_assert(offsetof(NiControllerSequence, _StringPalette) == 0x98);
				static_assert(offsetof(NiControllerSequence, _unkA0) == 0xA0);
				static_assert(offsetof(NiControllerSequence, _AnimNotes) == 0xA8);
				static_assert(offsetof(NiControllerSequence, _AnimNoteCount) == 0xB0);
				static_assert(offsetof(NiControllerSequence, _unkB2) == 0xB2);
			}
		}
	}
}
