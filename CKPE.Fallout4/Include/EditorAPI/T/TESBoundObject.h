// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/T/TESObject.h>
#include <EditorAPI/T/TESCellUseList.h>
#include <EditorAPI/B/BGSMod.h>
#include <EditorAPI/B/BGSPreviewTransform.h>
#include <EditorAPI/B/BGSSoundTagComponent.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class BGSVoiceType;
				// 0x90
				class TESBoundObject :
					public TESObject,        // 000
					public TESCellUseList    // 030
				{
				public:
					//constexpr static std::uint8_t TYPE_ID = ftVoice;			// need knows

					struct TESBound { std::int16_t x, y, z; };

					// CK 1.11.240.0: 154 virtual functions (TESObject has 136), the new ones start at 0x440
					virtual ~TESBoundObject() = default;
					virtual void* CreateInstanceData(const void* pData) const noexcept(true) { return nullptr; } // 136 - RVA 0x436830, return 0;
					virtual const void* GetBaseInstanceData() const noexcept(true) { return nullptr; } // 137 - RVA 0x436870, return 0;
					virtual bool GetAttachedMarkersVisible(void* pObj3D) const noexcept(true) { return false; } // 138 - RVA 0xC5A1A0, return 0;
					virtual void SetAttachedMarkersVisible(void* pObj3D, bool bVisible) const noexcept(true) { return; } // 139 - RVA 0xC5A1B0
					virtual bool QSupportsCountPerInstance() const noexcept(true) { return false; } // 140 - RVA 0x436980, return 0;
					virtual void SetObjectVoiceType(BGSVoiceType* pVoiceType) noexcept(true) { return; } // 141 - RVA 0x4369A0
					virtual BGSVoiceType* GetObjectVoiceType() const noexcept(true) { return nullptr; } // 142 - RVA 0x436890, return 0;
					virtual void sub_478() { return; } // 143 - RVA 0xC5A270, return sub_14009E585(a1 + 128);
					virtual void Clone3D(void* pRequester, void* pObj3D) noexcept(true) { return; } // 144 - RVA 0xC5A560, return (*(__int64 (__fastcall **)(__int64, __int64, __int64, _QWORD))(*(_QWORD *)a1 + 1024
					virtual bool ReplaceModel(const char* lpFileName) noexcept(true) { return false; } // 145 - RVA 0xC5A6B0
					virtual void sub_490() { return; } // 146 - RVA 0xC5A910
					virtual void sub_498() { return; } // 147 - RVA 0xC5B0B0
					virtual void sub_4A0() { return; } // 148 - RVA 0x436860
					virtual void sub_4A8() { return; } // 149 - RVA 0x4368D0
					virtual void sub_4B0() { return; } // 150 - RVA 0x436970
					virtual void sub_4B8() { return; } // 151 - RVA 0x436940
					virtual void sub_4C0() { return; } // 152 - RVA 0x436950
					virtual void sub_4C8() { return; } // 153 - RVA 0x436960

				private:
					TESBound _BoundMin; // 40
					TESBound _BoundMax; // 46
					char pad4C[4]; // 4C
					BGSMod::Template::Items _templateItems; // 50 - ???
					BGSPreviewTransform _PreviewTransform; // 70
					BGSSoundTagComponent _SoundTag; // 80 - 8 bytes in the CK
					std::uint32_t _unk88; // 88
					char pad8C[4]; // 8C
				public:
					[[nodiscard]] inline TESBound GetBoundMin() const noexcept(true) { return _BoundMin; }
					[[nodiscard]] inline TESBound GetBoundMax() const noexcept(true) { return _BoundMax; }

					/*
					Declare TESForm 
					[[nodiscard]] inline const BGSPreviewTransform* GetPreviewTransform() const noexcept(true) { return &_PreviewTransform; }
					[[nodiscard]] inline const BGSSoundTagComponent* GetSoundTagComponent() const noexcept(true) { return &_SoundTag; }*/
				public:
					CKPE_READ_PROPERTY(GetBoundMin) TESBound BoundMin;
					CKPE_READ_PROPERTY(GetBoundMax) TESBound BoundMax;
				};
				static_assert(sizeof(TESBoundObject) == 0x90);

				// 0x90
				class TESBoundAnimObject :
					public TESBoundObject   // 000
				{};
				static_assert(sizeof(TESBoundAnimObject) == 0x90);

				// 0x90
				class TESBoundTreeObject : public TESBoundObject 
				{};
				static_assert(sizeof(TESBoundTreeObject) == 0x90);
			}
		}
	}
}
