// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <CKPE.Utils.h>
#include <EditorAPI/NiAPI/NiNode.h>
#include <EditorAPI/BSHandleRefObject.h>
#include "TESForm.h"
#include "TESObjectCELL.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESChildCell
				{
				public:
					virtual ~TESChildCell() = default;
					virtual void* GetSaveParentCell();
				};
				static_assert(sizeof(TESChildCell) == 0x8);

				class TESObjectREFR_Original : public TESForm, public TESChildCell, public BSHandleRefObject_Original
				{
				public:
					virtual ~TESObjectREFR_Original() = default;
					virtual void OtherTestFunction2();
					char _pad0[0x10];
				};
				static_assert(sizeof(TESObjectREFR_Original) == 0x50);

				class TESObjectREFR_Extremly : public TESForm, public TESChildCell, public BSHandleRefObject_Extremly
				{
				public:
					virtual ~TESObjectREFR_Extremly() = default;
					virtual void OtherTestFunction2();
					char _pad0[0x10];
				};
				static_assert(sizeof(TESObjectREFR_Extremly) == 0x50);

				// This class is just a shell over the original class, can't use it to change and god forbid - duplication.
				// Because CKPE replaces with many different BSPointerHandle. Depending on how many valid refs are needed.
				class TESObjectREFR : public TESObjectREFR_Original
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftReference;
					enum SpecialFlagsForm
					{
						fs3DInvisible = 1 << 31,
						fsChildren3DInvisible = 1 << 30,
						fsFrozen = 1 << 29,
					};
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
					inline NiAPI::NiNode* GetFadeNode() const { return vtbl_call<NiAPI::NiNode*>(0x458, this); }
					inline TESForm* GetParent() const { return _Parent; }
					inline void SetParent(TESForm* NewParentForm) { vtbl_call<void>(0x4F8, this, NewParentForm); MarkAsChanged(); }
					inline NiAPI::NiPoint3 GetRotate() const { return _Rotate; }
					inline NiAPI::NiPoint3 GetPosition() const { return _Position; }
					inline float GetScale() const { return (float)_Scale / 100; }
					inline TESObjectCELL* GetParentCell() const { return _ParentCell; }
					inline bool IsInvisible() const { return (_FormFlags & SpecialFlagsForm::fs3DInvisible); }
					inline bool IsChildrenInvisible() const { return (_FormFlags & SpecialFlagsForm::fsChildren3DInvisible); }
					inline bool IsFrozen() const { return (_FormFlags & SpecialFlagsForm::fsFrozen); }

					// Added patch "Add Change Ref" (only 1.6 and newer)
					inline static void (*SetParentWithRedraw)(TESObjectREFR*, TESForm*);
				private:
					TESForm* _Parent;
					NiAPI::NiPoint3 _Rotate;
					NiAPI::NiPoint3 _Position;
					TESObjectCELL* _ParentCell;
					void* UnkPtr;
					ExtraDataList ExtraData;
					char pad90[0x10];
					std::uint16_t _Scale;
					std::uint16_t UnkNum;
				};
				static_assert(sizeof(TESObjectREFR) == 0xA8);
			}
		}
	}
}