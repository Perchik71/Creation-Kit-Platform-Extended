// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "TESForm.h"
#include "TESActorValue.h"
//#include "TESObjectCELL.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// `class TESForm`: VTable[0x0000000002F83448, 0x0000000000000000 offset, 124 functions] `. ? AVTESForm@@`
				// `class TESObject`: VTable [0x0000000002FF3A20, 0x0000000000000000 offset, 136 functions] `.?AVTESObject@@`
				class TESObject : public TESForm
				{
				private:
					/*028*/ char pad28[8];
				public:
					virtual ~TESObject() = default;

					// add
					virtual void                        Unk_3B(void);                                // 3B - { return 0; }
					virtual bool                        IsBoundAnimObject();                         // 3C - { return false; }
					//[[nodiscard]] virtual TESWaterForm* GetWaterType() const;                        // 3D - { return 0; }
					[[nodiscard]] virtual bool          IsAutoCalc() const;                          // 3E - { return false; }
					virtual void                        SetAutoCalc(bool a_autoCalc);                // 3F - { return; }
					//virtual NiAVObject* Clone3D(TESObjectREFR* a_ref, bool a_arg3);  // 40 - { return 0; }
					virtual void                        UnClone3D(TESObjectREFR* a_ref);             // 41
					virtual bool                        IsMarker();                                  // 42
					virtual bool                        IsOcclusionMarker();                         // 43 - { return formType == FormType::Static && this == Plane/Room/PortalMarker; }
					virtual bool                        ReplaceModel();                              // 44
					virtual std::uint32_t               IncRef();                                    // 45 - { return 0; }
					virtual std::uint32_t               DecRef();                                    // 46 - { return 0; }
					//virtual NiAVObject* LoadGraphics(TESObjectREFR* a_ref);          // 47
				};
			}
		}
	}
}
