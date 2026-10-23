// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/N/NiPointer.h>
#include <EditorAPI/T/TESObjectREFRFwd.h>
#include <EditorAPI/T/TESForm.h>
#include <EditorAPI/T/TESActorValue.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiAVObject;
			}

			namespace Forms
			{
				class TESWaterForm;

				// CK 1.11.240.0: size 0x30, vtable RVA 0x2FF3A20, 136 virtual functions (TESForm has 124)
				// `class TESObject`: `.?AVTESObject@@`
				class TESObject :
					public TESForm   // 000
				{
				private:
					char pad28[8]; // 28
				public:
					virtual ~TESObject() = default;

					virtual bool IsBoundAnimObject() noexcept(true) { return false; } // 124 - { return 0; }
					virtual Forms::TESWaterForm* GetWaterType() const noexcept(true) { return nullptr; } // 125 - { return 0; }
					virtual bool IsAutoCalc() const noexcept(true) { return false; } // 126 - { return 0; }
					virtual void SetAutoCalc(bool bAutoCalc) noexcept(true) { return; } // 127
					virtual void Clone3D(TESObjectREFR* pRequester, NiAPI::NiPointer<NiAPI::NiAVObject>* pObj3D, bool bBackground) noexcept(true) { return; } // 128
					virtual void UnClone3D(void* pRequester) noexcept(true) { return; } // 129
					virtual bool IsMarker() noexcept(true) { return false; } // 130 - tests TESForm flag 0x800000
					virtual bool IsOcclusionMarker() noexcept(true) { return false; } // 131 - verified: formType == ftStatic && this is one of the Plane/Room/Portal marker statics
					virtual bool ReplaceModel() noexcept(true) { return false; } // 132
					virtual std::uint32_t IncRef() noexcept(true) { return 0; } // 133 - { return 0; }
					virtual std::uint32_t DecRef() noexcept(true) { return 0; } // 134 - { return 0; }
					virtual void* LoadGraphics(void* pRequester) noexcept(true) { return nullptr; } // 135
				};
				static_assert(sizeof(TESObject) == 0x30);
			}
		}
	}
}
