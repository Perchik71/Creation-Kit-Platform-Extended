#pragma once

#include "NiAPI/NiPoint.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			using namespace CreationKitPlatformExtended::NiAPI;

			class BGSRenderWindow;
			class BGSRenderWindowReferenceEditModule
			{
			public:
				virtual ~BGSRenderWindowReferenceEditModule() = default;

				void MoveSelectObjects(NiPoint3* NewPosition, int32_t Unk = 0);
				void MoveSelectObjectsExtremly(NiPoint3* NewPosition, int32_t Unk = 0);
				void MoveSelectObjectsExtremlyNG(NiPoint3* NewPosition, int32_t Unk = 0);

				bool ComputeGridOrConnectPoint(const NiPoint3& Position, NiPoint3& OffsetInstant, 
					NiPoint3& NewPosition, uint32_t Index, uint32_t Count) const;
			private:
				//								00 - vtable
				BGSRenderWindow* _Renderer;	//	08 - render	
				char pad10[0x60];			//	10
				NiPoint3 _TempPosition;		//	70 - position	
				char pad7C[0x4];			//	7C
			};
			static_assert(sizeof(BGSRenderWindowReferenceEditModule) == 0x80);
		}
	}
}