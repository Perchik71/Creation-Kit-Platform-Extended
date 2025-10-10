// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "NiAPI/NiPoint.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BGSRenderWindow;
			class BGSRenderWindowReferenceEditModule
			{
			public:
				virtual ~BGSRenderWindowReferenceEditModule() = default;

				void MoveSelectObjects(NiAPI::NiPoint3* NewPosition, std::int32_t Unk = 0);
				void MoveSelectObjectsExtremly(NiAPI::NiPoint3* NewPosition, std::int32_t Unk = 0);
				void MoveSelectObjectsExtremlyNG(NiAPI::NiPoint3* NewPosition, std::int32_t Unk = 0);

				bool ComputeGridOrConnectPoint(const NiAPI::NiPoint3& Position, NiAPI::NiPoint3& OffsetInstant,
					NiAPI::NiPoint3& NewPosition, uint32_t Index, uint32_t Count) const;
			private:
				//									00 - vtable
				BGSRenderWindow* _Renderer;		//	08 - render	
				char pad10[0x60];				//	10
				NiAPI::NiPoint3 _TempPosition;	//	70 - position	
				char pad7C[0x4];				//	7C
			};
			static_assert(sizeof(BGSRenderWindowReferenceEditModule) == 0x80);
		}
	}
}