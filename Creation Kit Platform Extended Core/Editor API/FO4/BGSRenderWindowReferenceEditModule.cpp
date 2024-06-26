#include "..\BGSUniqueObjectRef.h"
#include "TESObjectREFR.h"
#include "BGSRenderWindow.h"
#include "BGSRenderWindowReferenceEditModule.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			// Offset relative to the grid
			static NiPoint3 MoveSnapObjectIntermediate;

			void BGSRenderWindowReferenceEditModule::MoveSelectObjects(NiPoint3* NewPosition, int32_t Unk)
			{
				auto Picker = _Renderer->GetPickHandler();
				if (!Picker && !Picker->Count) return;
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
				for (uint32_t i = 0; i < Picker->Count; i++, It = It->Next)
				{
					// Get a REFR to object and increase the counter
					auto FormRef = BGSUniqueObjectRef((TESObjectREFR_Original*)It->GetRef());
					if (FormRef.Empty()) continue;
					// I have no idea what for, but it is used in the original, also incrementing the counter
					auto NodeRef = FormRef->GetFadeNode();
					if (NodeRef.Empty()) continue;
					
					auto ObjectPosition = FormRef->GetPosition();
					if (BGSRenderWindow::HasSnapToGrid() || BGSRenderWindow::HasSnapToConnectPoints())
					{
						NiPoint3 NewPos;
						if (ComputeGridOrConnectPoint(ObjectPosition, NewPos))
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
				if (!Picker && !Picker->Count) return;
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
				for (uint32_t i = 0; i < Picker->Count; i++, It = It->Next)
				{
					// Get a REFR to object and increase the counter
					auto FormRef = BGSUniqueObjectRef((TESObjectREFR_Extremly*)It->GetRef());
					if (FormRef.Empty()) continue;
					// I have no idea what for, but it is used in the original, also incrementing the counter
					auto NodeRef = FormRef->GetFadeNode();
					if (NodeRef.Empty()) continue;

					auto ObjectPosition = FormRef->GetPosition(); 
					if (BGSRenderWindow::HasSnapToGrid() || BGSRenderWindow::HasSnapToConnectPoints())
					{
						NiPoint3 NewPos;
						if (ComputeGridOrConnectPoint(ObjectPosition, NewPos))
							FormRef->SetPosition(FormRef.Get(), &NewPos);
					}
					else
					{
						ObjectPosition += *NewPosition;
						FormRef->SetPosition(FormRef.Get(), &ObjectPosition);
					}
				}
			}

			bool BGSRenderWindowReferenceEditModule::ComputeGridOrConnectPoint(const NiPoint3& Position, NiPoint3& NewPosition) const
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
						MoveSnapObjectIntermediate.x = 0.f;
						Result = true;
					}
					if (Offset.y != 0.f)
					{
						MoveSnapObjectIntermediate.y = 0.f;
						Result = true;
					}
					if (Offset.z != 0.f)
					{
						MoveSnapObjectIntermediate.z = 0.f;
						Result = true;
					}

					return Result;
				}
				else if (BGSRenderWindow::HasSnapToConnectPoints())
				{
					// DEV
				}

				return false;
			}
		}
	}
}