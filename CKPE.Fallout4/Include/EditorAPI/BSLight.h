// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BSTArray.h"
#include "NiAPI/NiPoint.h"
#include "NiAPI/NiRefObject.h"
#include "NiAPI/NiPointer.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSMultiBoundRoom;
			class BSPortalGraph;

			namespace NiAPI
			{
				class NiPointLight;
				class NiAVObject;
			}

			class BSLight : public NiAPI::NiRefObject
			{
				char _pad10[0x80];
				NiAPI::NiPoint3 _LightPosition;							// 0x90
				float _unk9C;											// 0x9C
				NiAPI::NiPoint3 _LightBoxPosition;						// 0xA0
				NiAPI::NiPoint3 _Radius;								// 0xAC
				NiAPI::NiPointer<NiAPI::NiPointLight> _PointLight;		// 0xB8
				char _padC0[0x30];
				BSTArray<BSMultiBoundRoom*> _BoundRooms;				// 0xF0
				BSTArray<void*> _Portals;								// 0x108 BSPortal
				BSTArray<void*> _Unk1;									// 0x120
				NiAPI::NiPointer<BSPortalGraph> _PortalGraph;			// 0x138
				std::uint64_t _unk140;									// 0x140
				NiAPI::NiPointer<NiAPI::NiAVObject> _ObjectNode;		// 0x148
				char _Unk2[0x40];
			public:
				virtual ~BSLight() = default;
			};
			static_assert(sizeof(BSLight) == 0x190);
		}
	}
}
