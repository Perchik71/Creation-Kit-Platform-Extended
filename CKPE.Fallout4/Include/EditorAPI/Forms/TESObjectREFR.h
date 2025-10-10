// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Utils.h>
#include "..\\BSTEvents.h"
#include "..\\BSFadeNode.h"
#include <EditorAPI/NiAPI/NiTypes.h>
#include <EditorAPI/NiAPI/NiPointer.h>
#include "..\\BSHandleRefObject.h"
#include "BGSPrimitive.h"
#include "TESForm.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObjectCELL;

				template<class _Ty>
				class TESObjectREFR_base : public TESForm, public BSTEventSink<void*>, public _Ty, 
					public BSTEventSink<BSActiveGraphIfInactiveEvent>, public BSTEventSink<BSAnimationGraphEvent>, 
					public BSTEventSink<BGSInventoryListEvent::Event>
				{
				public:
					constexpr static uint8_t TYPE_ID = TESForm::FormType::ftReference;
					enum SpecialFlagsForm 
					{
						// https://github.com/shadeMe/Construction-Set-Extender/blob/master/EditorAPI/TESObjectREFR.h
						fs3DInvisible = 1 << 31,
						fsChildren3DInvisible = 1 << 30,
						fsFrozen = 1 << 29,
					};
				protected:
					char _pad58[0x70];						// 0x58
					TESObjectCELL* _parentCell;				// 0xC8
					NiAPI::NiPoint3 _rotate;				// 0xD0, 0xD4, 0xD8
					char _padEC[0x4];						// 0xDC
					NiAPI::NiPoint3 _position;				// 0xE0, 0xE4, 0xE8
					char _padDC[0x4];						// 0xEC
					TESForm* _baseForm;						// 0xF0
					void* _unkB8;							// 0xF8
					void* _unkC0;							// 0x100
					void* _unkC8;							// 0x108
					void* _extraData;						// 0x110
					std::uint16_t _scale;					// 0x118
					char _padDA[0x26];						// 0x11A
				public:
					virtual ~TESObjectREFR_base() = default;

					inline static void (*SetParentWithRedraw)(void*, TESForm*);
					inline static void (*SetPosition)(void*, const NiAPI::NiPoint3* val);
					inline NiAPI::NiPointer<BSFadeNode> GetFadeNode() const { return vtbl_call<BSFadeNode*>(0x558, this); }
					inline NiAPI::NiPointer<BSFadeNode> GetFadeNode2(uint32_t unk = 0) const { return vtbl_call<BSFadeNode*>(0x528, this, unk); }

					inline NiAPI::NiPoint3 GetPosition() const { return _position; }
					inline NiAPI::NiPoint3 GetRotate() const { return _rotate; }
					inline void SetRotate(const NiAPI::NiPoint3& val) { _rotate = val; }
					inline std::uint16_t GetScale() const { return *(std::uint16_t*)(((uintptr_t)this) + 0x118); }
					inline void SetScale(std::uint16_t val) { *(std::uint16_t*)(((uintptr_t)this) + 0x118) = val; }
					inline float GetScaleFloat() const { return ((float)GetScale() / 100); }
					inline void SetScaleFloat(float val) { SetScale(std::uint16_t(val * 100)); }
					inline TESObjectCELL* GetParentCell() const { return *(TESObjectCELL**)(((uintptr_t)this) + 0xC8); }
					inline TESForm* GetParent() const { return *(TESForm**)(((uintptr_t)this) + 0xF0); }
					inline bool IsInvisible() const { return _FormFlags.Has(fs3DInvisible); }
					inline bool IsChildrenInvisible() const { return _FormFlags.Has(fsChildren3DInvisible); }
					inline bool IsFrozen() const { return _FormFlags.Has(fsFrozen); }

					CKPE_READ_PROPERTY(GetParentCell) TESObjectCELL* ParentCell;
					CKPE_READ_PROPERTY(GetParent) TESForm* Parent;
					CKPE_PROPERTY(GetPosition, SetPosition) NiAPI::NiPoint3 Position;
					CKPE_PROPERTY(GetRotate, SetRotate) NiAPI::NiPoint3 Rotate;
					CKPE_PROPERTY(GetScale, SetScale) std::uint16_t Scale;
					CKPE_PROPERTY(GetScaleFloat, SetScaleFloat) float ScaleFloat;
				};

				typedef TESObjectREFR_base<BSHandleRefObject_Original> TESObjectREFR_Original;
				typedef TESObjectREFR_base<BSHandleRefObject_Extremly> TESObjectREFR_Extremly_NG;
				typedef TESObjectREFR_base<BSHandleRefObject_64_Extremly> TESObjectREFR_Extremly;

				static_assert(sizeof(TESObjectREFR_Original) == 0x140);

				// IncRef and DefRef - NO USE!!!
				typedef TESObjectREFR_Original TESObjectREFR;
			}
		}
	}
}