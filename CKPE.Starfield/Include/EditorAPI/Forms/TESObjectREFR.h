// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "TESForm.h"
#include "NiAPI/NiPoint.h"

#pragma once
#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Starfield
		{
			class TESObjectREFR : public TESForm
			{
			private:
				char pad8A[0xF6];
				NiAPI::NiPoint3 _Rotate;
				NiAPI::NiPoint3 _Position;
				TESForm* _Parent;
				char pad1A0[0x20];
				TESForm* _ParentCell;
				void* _ResourceList;
				char pad1D0[0x10];
				uint16_t _Scale;
				uint16_t UnkNum;
			public:
				// The function returns BSFadeNode, it has not been studied, I use parent class
				inline NiAPI::Starfield::NiNode* GetFadeNode() const
				{
					NiAPI::Starfield::NiNode* pReturn = nullptr;
					thisVirtualCall<NiAPI::Starfield::NiNode**>(0x708, this, &pReturn);
					return pReturn;
				}
				inline TESForm* GetParent() const { return _Parent; }
				inline NiAPI::NiPoint3 GetRotate() const { return _Rotate; }
				inline NiAPI::NiPoint3 GetPosition() const { return _Position; }
				inline float GetScale() const { return (float)_Scale / 100; }
				inline TESForm* GetParentCell() const { return _ParentCell; }
			};
		}
	}
}

#pragma pack(pop)