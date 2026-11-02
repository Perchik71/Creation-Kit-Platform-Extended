// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSTHashMap.h>
#include <EditorAPI/N/NiTimeController.h>
#include <EditorAPI/N/NiControllerSequence.h>
#include <EditorAPI/N/NiTObjectArray.h>
#include <EditorAPI/N/NiPointer.h>
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
				class NiDefaultAVObjectPalette;

				// Layout and size (0xD0, the allocation size at every constructor call site) are the same in 1.10.162.0,
				// 1.10.982.3, 1.11.137.0, 1.11.240.0 and 1.11.246.0.
				class NiControllerManager : public NiTimeController
				{
				public:
					constexpr static Common::ID RTTI = IDs::RTTI::NiControllerManager;
					constexpr static Common::ID VTABLE = IDs::VTABLE::NiControllerManager[0];

					/* 02 */ virtual NiRTTI* GetRTTI() const noexcept(true) override;
					/* 1A */ virtual NiObject* CreateClone(NiCloningProcess& Process) noexcept(true) override;
					/* 1B */ virtual void LoadBinary(NiStream& Stream) noexcept(true) override;
					/* 1C */ virtual void LinkObject(NiStream& Stream) noexcept(true) override;
					/* 1D */ virtual bool RegisterStreamables(NiStream& Stream) noexcept(true) override;
					/* 1E */ virtual void SaveBinary(NiStream& Stream) noexcept(true) override;
					/* 1F */ virtual bool IsEqual(NiObject* Object) noexcept(true) override;
					/* 21 */ virtual void ProcessClone(NiCloningProcess& Process) noexcept(true) override;
					/* 22 */ virtual void PostLinkObject(NiStream& Stream) noexcept(true) override;
					/* 28 */ virtual NiControllerManager* IsNiControllerManager() noexcept(true) override;
					/* 2A */ virtual void Start(float Time) noexcept(true) override;
					/* 2B */ virtual void Stop() noexcept(true) override;
					// Updates every active sequence while the controller is active (flag 0x8) and drops the ones that became inactive.
					/* 2C */ virtual void Update(NiUpdateData& Data) noexcept(true) override;
					/* 2D */ virtual void SetTarget(NiObjectNET* Target) noexcept(true) override;
					/* 33 */ virtual bool TargetIsRequiredType() const noexcept(true) override;
					// Empty in this build, arguments not recovered.
					/* 34 */ virtual void UnkControllerManager34() noexcept(true);

					// The array is sparse, iterate from 0 to GetSequenceCount() - 1 and skip null entries.
					[[nodiscard]] inline std::uint16_t GetSequenceCount() const noexcept(true) { return _Sequences.EmptyRunStart; }
					[[nodiscard]] inline NiControllerSequence* GetSequence(std::uint16_t Index) const noexcept(true)
					{
						return (Index < _Sequences.EmptyRunStart) && _Sequences.Data ? (NiControllerSequence*)_Sequences.Data[Index] : nullptr;
					}
					[[nodiscard]] inline std::uint32_t GetActiveSequenceCount() const noexcept(true) { return _ActiveSequencesCount; }
					[[nodiscard]] inline NiControllerSequence* GetActiveSequence(std::uint32_t Index) const noexcept(true)
					{
						return (Index < _ActiveSequencesCount) && _ActiveSequences ? _ActiveSequences[Index] : nullptr;
					}
					[[nodiscard]] inline bool IsCumulative() const noexcept(true) { return _Cumulative; }

					NiTObjectArray<NiPointer<NiControllerSequence>> _Sequences;				// 48
					NiControllerSequence** _ActiveSequences;								// 60 - animating sequences, see Update
					std::uint32_t _ActiveSequencesCapacity;									// 68
					std::uint32_t _ActiveSequencesCount;									// 6C
					BSTHashMap<BSFixedString, NiControllerSequence*> _SequenceMap;			// 70 - by sequence name
					NiObject* _AnimNoteReceivers;											// A8 - NiTPrimitiveArray<BSAnimNoteListener::BSAnimReceiverType*> (0x18 bytes)
					bool _Cumulative;														// B0 - InitNonNPCAnimation reports a cumulative manager as an error
					std::uint8_t _padB1[7];													// B1
					NiPointer<NiObject>* _ClonedObjects;									// B8 - reference counted, replaced by their clones in ProcessClone
					std::uint32_t _ClonedObjectsCapacity;									// C0
					std::uint32_t _ClonedObjectsCount;										// C4
					NiDefaultAVObjectPalette* _ObjectPalette;								// C8 - reference counted
				};
				static_assert(sizeof(NiControllerManager) == 0xD0);
				static_assert(offsetof(NiControllerManager, _Sequences) == 0x48);
				static_assert(offsetof(NiControllerManager, _ActiveSequences) == 0x60);
				static_assert(offsetof(NiControllerManager, _ActiveSequencesCapacity) == 0x68);
				static_assert(offsetof(NiControllerManager, _ActiveSequencesCount) == 0x6C);
				static_assert(offsetof(NiControllerManager, _SequenceMap) == 0x70);
				static_assert(offsetof(NiControllerManager, _AnimNoteReceivers) == 0xA8);
				static_assert(offsetof(NiControllerManager, _Cumulative) == 0xB0);
				static_assert(offsetof(NiControllerManager, _ClonedObjects) == 0xB8);
				static_assert(offsetof(NiControllerManager, _ClonedObjectsCapacity) == 0xC0);
				static_assert(offsetof(NiControllerManager, _ClonedObjectsCount) == 0xC4);
				static_assert(offsetof(NiControllerManager, _ObjectPalette) == 0xC8);
			}
		}
	}
}
