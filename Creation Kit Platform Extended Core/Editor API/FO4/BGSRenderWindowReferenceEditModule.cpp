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
			void BGSRenderWindowReferenceEditModule::MoveSelectObjects(NiPoint3* NewPosition)
			{
				auto Picker = _Renderer->GetPickHandler();
				if (!Picker && !Picker->Count) return;

				_TempPosition += *NewPosition;

				auto It = Picker->Items->First;
				for (uint32_t i = 0; i < Picker->Count; i++, It = It->Next)
				{
					auto FormRef = BGSUniqueObjectRef((TESObjectREFR_Original*)It->GetRef());
					if (FormRef.Empty()) continue;
					auto NodeRef = BGSUniqueObjectRef(FormRef->GetFadeNode());
					if (NodeRef.Empty()) continue;
		
					if (_TempPosition.Magnitude2() >= 0.001f)
					{
						auto NewPos = FormRef->GetPosition();
						NewPos += *NewPosition;

						FormRef->SetPosition(FormRef.Get(), &NewPos);
					}
				}
			}

			void BGSRenderWindowReferenceEditModule::MoveSelectObjectsExtremly(NiPoint3* NewPosition)
			{
				auto Picker = _Renderer->GetPickHandler();
				if (!Picker && !Picker->Count) return;

				_TempPosition += *NewPosition;

				auto It = Picker->Items->First;
				for (uint32_t i = 0; i < Picker->Count; i++, It = It->Next)
				{
					auto FormRef = BGSUniqueObjectRef((TESObjectREFR_Extremly*)It->GetRef());
					if (FormRef.Empty()) continue;
					auto NodeRef = BGSUniqueObjectRef(FormRef->GetFadeNode());
					if (NodeRef.Empty()) continue;

					if (_TempPosition.Magnitude2() >= 0.001f)
					{
						auto NewPos = FormRef->GetPosition();
						NewPos += *NewPosition;

						FormRef->SetPosition(FormRef.Get(), &NewPos);
					}
				}
			}
		}
	}
}