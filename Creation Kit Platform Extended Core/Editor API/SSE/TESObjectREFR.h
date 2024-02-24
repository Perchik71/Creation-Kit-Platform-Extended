// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TESForm.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			// This class is just a shell over the original class, can't use it to change and god forbid - duplication.
			// Because CKPE replaces with many different BSPointerHandle. Depending on how many valid refs are needed.
			class TESObjectREFR : public TESObjectREFR_Original
			{
			public:
				constexpr static uint8_t TYPE_ID = 0x3D;
			public:
				class ExtraDataList
				{
				public:
					virtual ~ExtraDataList() = default;
					char pad08[0x8];
				};
			public:
				virtual ~TESObjectREFR() = default;

				// The function returns BSFadeNode, it has not been studied, I use parent class
				inline NiAPI::NiNode* GetFadeNode() const { return thisVirtualCall<NiAPI::NiNode*>(0x458, this); }
				inline TESForm* GetParent() const { return _Parent; }
				inline NiAPI::NiPoint3 GetRotate() const { return _Rotate; }
				inline NiAPI::NiPoint3 GetPosition() const { return _Position; }
				inline TESForm* GetParentCell() const { return _ParentCell; }
			private:
				TESForm* _Parent;
				NiAPI::NiPoint3 _Rotate;
				NiAPI::NiPoint3 _Position;
				TESForm* _ParentCell;
				void* UnkPtr;
				ExtraDataList ExtraData;
			};
			static_assert(sizeof(TESObjectREFR) == 0x90);
		}
	}
}