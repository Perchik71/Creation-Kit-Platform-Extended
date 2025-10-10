// Copyright � 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include "BGSUniqueObjectRef.h"
#include "BGSRenderWindow.h"
#include "BGSRenderWindowReferenceEditModule.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// Offset relative to the grid
			static NiPoint3 MoveSnapObjectIntermediate;

			void BGSRenderWindowReferenceEditModule::MoveSelectObjects(NiPoint3* NewPosition, int32_t Unk)
			{
				auto Picker = _Renderer->GetPickHandler();
				if (!Picker || !Picker->Count) return;
				// Reset var the relative grid offset 
				if (_TempPosition == ZERO_P3)
					MoveSnapObjectIntermediate = ZERO_P3;
				// If the offset is too small, then it is not needed
				/*if (NewPosition->Magnitude2() >= 0.001f)
					return;*/
				// Increase the offset while pull the mouse, this offset cannot be touched, it seems to be used to restore the cursor
				_TempPosition += *NewPosition;
				// Also for the relative grid
				MoveSnapObjectIntermediate += *NewPosition;
				
				auto It = Picker->Items->First;
				for (std::uint32_t i = 0; i < Picker->Count; i++, It = It->Next)
				{
					// Get a REFR to object and increase the counter
					auto FormRef = BGSUniqueObjectRef((Forms::TESObjectREFR_Original*)It->GetRef());
					if (FormRef.Empty()) continue;
					// I have no idea what for, but it is used in the original, also incrementing the counter
					auto NodeRef = FormRef->GetFadeNode();
					if (NodeRef.Empty()) continue;
					
					auto ObjectPosition = FormRef->GetPosition();
					if (BGSRenderWindow::HasSnapToGrid() || BGSRenderWindow::HasSnapToConnectPoints())
					{
						NiPoint3 NewPos;
						if (ComputeGridOrConnectPoint(ObjectPosition, *NewPosition, NewPos, i, Picker->Count))
							FormRef->SetPosition(FormRef.Get(), &NewPos);
					}
					else
					{
						ObjectPosition += *NewPosition;
						FormRef->SetPosition(FormRef.Get(), &ObjectPosition);
					}
				}
			}

			void BGSRenderWindowReferenceEditModule::MoveSelectObjectsExtremlyNG(NiPoint3* NewPosition, int32_t Unk)
			{
				auto Picker = _Renderer->GetPickHandler();
				if (!Picker || !Picker->Count) return;
				// Reset var the relative grid offset
				if (_TempPosition == ZERO_P3)
					MoveSnapObjectIntermediate = ZERO_P3;
				// If the offset is too small, then it is not needed
				/*if (NewPosition->Magnitude2() >= 0.001f)
					return;*/
					// Increase the offset while pull the mouse, this offset cannot be touched, it seems to be used to restore the cursor
				_TempPosition += *NewPosition;
				// Also for the relative grid
				MoveSnapObjectIntermediate += *NewPosition;

				auto It = Picker->Items->First;
				for (std::uint32_t i = 0; i < Picker->Count; i++, It = It->Next)
				{
					// Get a REFR to object and increase the counter
					auto FormRef = BGSUniqueObjectRef((Forms::TESObjectREFR_Extremly_NG*)It->GetRef());
					if (FormRef.Empty()) continue;
					// I have no idea what for, but it is used in the original, also incrementing the counter
					auto NodeRef = FormRef->GetFadeNode();
					if (NodeRef.Empty()) continue;

					auto ObjectPosition = FormRef->GetPosition();
					if (BGSRenderWindow::HasSnapToGrid() || BGSRenderWindow::HasSnapToConnectPoints())
					{
						NiPoint3 NewPos;
						if (ComputeGridOrConnectPoint(ObjectPosition, *NewPosition, NewPos, i, Picker->Count))
							FormRef->SetPosition(FormRef.Get(), &NewPos);
					}
					else
					{
						ObjectPosition += *NewPosition;
						FormRef->SetPosition(FormRef.Get(), &ObjectPosition);
					}
				}
			}

			void BGSRenderWindowReferenceEditModule::MoveSelectObjectsExtremly(NiPoint3* NewPosition, int32_t Unk)
			{
				auto Picker = _Renderer->GetPickHandler();
				if (!Picker || !Picker->Count) return;
				// Reset var the relative grid offset
				if (_TempPosition == ZERO_P3)
					MoveSnapObjectIntermediate = ZERO_P3;
				// If the offset is too small, then it is not needed
				/*if (NewPosition->Magnitude2() >= 0.001f)
					return;*/
				// Increase the offset while pull the mouse, this offset cannot be touched, it seems to be used to restore the cursor
				_TempPosition += *NewPosition;
				// Also for the relative grid
				MoveSnapObjectIntermediate += *NewPosition;

				auto It = Picker->Items->First;
				for (std::uint32_t i = 0; i < Picker->Count; i++, It = It->Next)
				{
					// Get a REFR to object and increase the counter
					auto FormRef = BGSUniqueObjectRef((Forms::TESObjectREFR_Extremly*)It->GetRef());
					if (FormRef.Empty()) continue;
					// I have no idea what for, but it is used in the original, also incrementing the counter
					auto NodeRef = FormRef->GetFadeNode();
					if (NodeRef.Empty()) continue;

					auto ObjectPosition = FormRef->GetPosition(); 
					if (BGSRenderWindow::HasSnapToGrid() || BGSRenderWindow::HasSnapToConnectPoints())
					{
						NiPoint3 NewPos;
						if (ComputeGridOrConnectPoint(ObjectPosition, *NewPosition, NewPos, i, Picker->Count))
							FormRef->SetPosition(FormRef.Get(), &NewPos);
					}
					else
					{
						ObjectPosition += *NewPosition;
						FormRef->SetPosition(FormRef.Get(), &ObjectPosition);
					}
				}
			}

			bool BGSRenderWindowReferenceEditModule::ComputeGridOrConnectPoint(const NiPoint3& Position, NiPoint3& OffsetInstant,
				NiPoint3& NewPosition, std::uint32_t Index, std::uint32_t Count) const
			{
				if (BGSRenderWindow::HasSnapToGrid())
				{
					// Getting the grid value in the user settings
					auto Grid = (float)BGSRenderWindow::Settings::Movement::GetSnapGrid();

					// Algorithm should be similar to the original

					float GridNormal = 1.f / Grid;
					float GridAverage = Grid * 0.5f;

					NewPosition = Position + MoveSnapObjectIntermediate;
					NewPosition.x += (NewPosition.x >= 0.f) ? GridAverage : -GridAverage;
					NewPosition.y += (NewPosition.y >= 0.f) ? GridAverage : -GridAverage;
					NewPosition.z += (NewPosition.z >= 0.f) ? GridAverage : -GridAverage;
					NewPosition.x = ((float)((int)(NewPosition.x * GridNormal))) * Grid;
					NewPosition.y = ((float)((int)(NewPosition.y * GridNormal))) * Grid;
					NewPosition.z = ((float)((int)(NewPosition.z * GridNormal))) * Grid;

					// If there is at least some offset, then we give the right to change,
					// and also reset the coordinate axis that has an offset

					bool Result = false;
					auto Offset = NewPosition - Position;
					if (Offset.x != 0.f)
					{
						if (Index == (Count - 1)) MoveSnapObjectIntermediate.x = 0.f;
						Result = true;
					}
					if (Offset.y != 0.f)
					{
						if (Index == (Count - 1)) MoveSnapObjectIntermediate.y = 0.f;
						Result = true;
					}
					if (Offset.z != 0.f)
					{
						if (Index == (Count - 1)) MoveSnapObjectIntermediate.z = 0.f;
						Result = true;
					}

					return Result;
				}
				else if (BGSRenderWindow::HasSnapToConnectPoints())
				{
					// This function also performs Snap To Grid, however, 
					// But I couldn't get it to work, so this part is implemented,
					// it does not matter, I know for sure that for the point binding function, 
					// only 2 arguments are needed.

					NewPosition = Position + OffsetInstant;
					return vtbl_call<bool>(0xB0, this, (NiPoint3*)&NewPosition);
				}

				return false;
			}
		}
	}
}