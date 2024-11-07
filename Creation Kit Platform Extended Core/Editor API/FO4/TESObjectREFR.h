// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "NiEvents.h"
#include "BSFadeNode.h"
#include "NiAPI/NiPoint.h"
#include "NiAPI/NiPointer.h"
#include "..\\BSHandleRefObject.h"
#include "BGSPrimitive.h"
#include "TESFormF4.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			using namespace NiAPI;

			class TESObjectCELL;

			template<class _Ty>
			class TESObjectREFR_base : public TESForm, public BSTEventSink<LPVOID>, public _Ty
			{
			public:
				virtual ~TESObjectREFR_base() = default;

				inline static void (*SetParentWithRedraw)(void*, TESForm*);
				inline static void (*SetPosition)(void*, const NiPoint3* val);
				inline NiAPI::NiPointer<BSFadeNode> GetFadeNode() const { return thisVirtualCall<BSFadeNode*>(0x558, this); }
				inline NiAPI::NiPointer<BSFadeNode> GetFadeNode2(uint32_t unk = 0) const { return thisVirtualCall<BSFadeNode*>(0x528, this, unk); }
			};

			typedef TESObjectREFR_base<BSHandleRefObject_Original> TESObjectREFR_base_Original;
			typedef TESObjectREFR_base<BSHandleRefObject_Extremly> TESObjectREFR_base_Extremly_NG;
			typedef TESObjectREFR_base<BSHandleRefObject_64_Extremly> TESObjectREFR_base_Extremly;

			static_assert(sizeof(TESObjectREFR_base_Original) == 0x40);
			static_assert(sizeof(TESObjectREFR_base_Extremly) == 0x40);

			class TESObjectREFR_Original : public TESObjectREFR_base_Original
			{
			public:
				constexpr static uint8_t TYPE_ID = FormType::ftReference;
				enum SpecialFlagsForm 
				{
					fs3DInvisible = 1 << 31,
					fsChildren3DInvisible = 1 << 30,
					fsFrozen = 1 << 29,
				};
			protected:
				BSTEventSink<void*> _unkEventSink02;	// 0x040
				BSTEventSink<void*> _unkEventSink03;	// 0x048
				BSTEventSink<void*> _unkEventSink04;	// 0x050
				char _pad0[0x70];						// 0x058
				TESObjectCELL* _parentCell;				// 0x0C8
				NiPoint3 _rotate;						// 0x0D0, 0x0D4, 0x0D8
				float _unkDC;							// 0x0DC
				NiPoint3 _position;						// 0x0E0, 0x0E4, 0x0E8
				float _unkEC;							// 0x0EC
				TESForm* _baseForm;						// 0x0F0
				void* _unkE8;							// 0x0F8
				void* _unk100;							// 0x100
				void* _unk108;							// 0x108
				void* _extraData;						// 0x110
				uint16_t _scale;						// 0x118
				char _pad118[0x26];						// 0x11A
			public:
				inline NiPoint3 GetPosition() const { return _position; }
				inline NiPoint3 GetRotate() const { return _rotate; }
				inline void SetRotate(const NiPoint3& val) { _rotate = val; }
				inline uint16_t GetScale() const { return _scale; }
				inline void SetScale(uint16_t val) { _scale = val; }
				inline float GetScaleFloat() const { return ((float)_scale / 100); }
				inline void SetScaleFloat(float val) { _scale = uint16_t(val * 100); }
				inline bool IsInvisible() const { return (_FormFlags & SpecialFlagsForm::fs3DInvisible); }
				inline bool IsChildrenInvisible() const { return (_FormFlags & SpecialFlagsForm::fsChildren3DInvisible); }
				inline bool IsFrozen() const { return (_FormFlags & SpecialFlagsForm::fsFrozen); }

				READ_PROPERTY2(TESObjectCELL*, ParentCell, _parentCell);
				READ_PROPERTY2(TESForm*, Parent, _baseForm);
				PROPERTY(GetPosition, SetPosition) NiPoint3 Position;
				PROPERTY(GetRotate, SetRotate) NiPoint3 Rotate;
				PROPERTY(GetScale, SetScale) uint16_t Scale;
				PROPERTY(GetScaleFloat, SetScaleFloat) float ScaleFloat;
			};
			static_assert(sizeof(TESObjectREFR_Original) == 0x140);

			class TESObjectREFR_Extremly_NG : public TESObjectREFR_base_Extremly_NG
			{
			public:
				constexpr static uint8_t TYPE_ID = FormType::ftReference;
				enum SpecialFlagsForm {
					// https://github.com/shadeMe/Construction-Set-Extender/blob/master/EditorAPI/TESObjectREFR.h
					fs3DInvisible = 1 << 31,
					fsChildren3DInvisible = 1 << 30,
					fsFrozen = 1 << 29,
				};
			protected:
				BSTEventSink<void*> _unkEventSink02;	// 0x040
				BSTEventSink<void*> _unkEventSink03;	// 0x048
				BSTEventSink<void*> _unkEventSink04;	// 0x050
				char _pad0[0x70];						// 0x058
				TESObjectCELL* _parentCell;				// 0x0C8
				NiPoint3 _rotate;						// 0x0D0, 0x0D4, 0x0D8
				float _unkDC;							// 0x0DC
				NiPoint3 _position;						// 0x0E0, 0x0E4, 0x0E8
				float _unkEC;							// 0x0EC
				TESForm* _baseForm;						// 0x0F0
				void* _unkE8;							// 0x0F8
				void* _unk100;							// 0x100
				void* _unk108;							// 0x108
				void* _extraData;						// 0x110
				uint16_t _scale;						// 0x118
				char _pad118[0x26];						// 0x11A
			public:
				inline NiPoint3 GetPosition() const { return _position; }
				inline NiPoint3 GetRotate() const { return _rotate; }
				inline void SetRotate(const NiPoint3& val) { _rotate = val; }
				inline uint16_t GetScale() const { return _scale; }
				inline void SetScale(uint16_t val) { _scale = val; }
				inline float GetScaleFloat() const { return ((float)_scale / 100); }
				inline void SetScaleFloat(float val) { _scale = uint16_t(val * 100); }
				inline bool IsInvisible() const { return (_FormFlags & SpecialFlagsForm::fs3DInvisible); }
				inline bool IsChildrenInvisible() const { return (_FormFlags & SpecialFlagsForm::fsChildren3DInvisible); }
				inline bool IsFrozen() const { return (_FormFlags & SpecialFlagsForm::fsFrozen); }

				READ_PROPERTY2(TESObjectCELL*, ParentCell, _parentCell);
				READ_PROPERTY2(TESForm*, Parent, _baseForm);
				PROPERTY(GetPosition, SetPosition) NiPoint3 Position;
				PROPERTY(GetRotate, SetRotate) NiPoint3 Rotate;
				PROPERTY(GetScale, SetScale) uint16_t Scale;
				PROPERTY(GetScaleFloat, SetScaleFloat) float ScaleFloat;
			};
			static_assert(sizeof(TESObjectREFR_Extremly_NG) == 0x140);

			class TESObjectREFR_Extremly : public TESObjectREFR_base_Extremly
			{
			public:
				constexpr static uint8_t TYPE_ID = FormType::ftReference;
				enum SpecialFlagsForm {
					// https://github.com/shadeMe/Construction-Set-Extender/blob/master/EditorAPI/TESObjectREFR.h
					fs3DInvisible = 1 << 31,
					fsChildren3DInvisible = 1 << 30,
					fsFrozen = 1 << 29,
				};
			protected:
				BSTEventSink<void*> _unkEventSink02;	// 0x040
				BSTEventSink<void*> _unkEventSink03;	// 0x048
				BSTEventSink<void*> _unkEventSink04;	// 0x050
				char _pad0[0x70];						// 0x058
				TESObjectCELL* _parentCell;				// 0x0C8
				NiPoint3 _rotate;						// 0x0D0, 0x0D4, 0x0D8
				float _unkDC;							// 0x0DC
				NiPoint3 _position;						// 0x0E0, 0x0E4, 0x0E8
				float _unkEC;							// 0x0EC
				TESForm* _baseForm;						// 0x0F0
				void* _unkE8;							// 0x0F8
				void* _unk100;							// 0x100
				void* _unk108;							// 0x108
				void* _extraData;						// 0x110
				uint16_t _scale;						// 0x118
				char _pad118[0x26];						// 0x11A
			public:
				inline NiPoint3 GetPosition() const { return _position; }
				inline NiPoint3 GetRotate() const { return _rotate; }
				inline void SetRotate(const NiPoint3& val) { _rotate = val; }
				inline uint16_t GetScale() const { return _scale; }
				inline void SetScale(uint16_t val) { _scale = val; }
				inline float GetScaleFloat() const { return ((float)_scale / 100); }
				inline void SetScaleFloat(float val) { _scale = uint16_t(val * 100); }
				inline bool IsInvisible() const { return (_FormFlags & SpecialFlagsForm::fs3DInvisible); }
				inline bool IsChildrenInvisible() const { return (_FormFlags & SpecialFlagsForm::fsChildren3DInvisible); }
				inline bool IsFrozen() const { return (_FormFlags & SpecialFlagsForm::fsFrozen); }

				READ_PROPERTY2(TESObjectCELL*, ParentCell, _parentCell);
				READ_PROPERTY2(TESForm*, Parent, _baseForm);
				PROPERTY(GetPosition, SetPosition) NiPoint3 Position;
				PROPERTY(GetRotate, SetRotate) NiPoint3 Rotate;
				PROPERTY(GetScale, SetScale) uint16_t Scale;
				PROPERTY(GetScaleFloat, SetScaleFloat) float ScaleFloat;
			};
			static_assert(sizeof(TESObjectREFR_Extremly) == 0x140);

			// IncRef and DefRef - NO USE!!!
			typedef TESObjectREFR_Original TESObjectREFR;
		}
	}
}

#pragma pack(pop)