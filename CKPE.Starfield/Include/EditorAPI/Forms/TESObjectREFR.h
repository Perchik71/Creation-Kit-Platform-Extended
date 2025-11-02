// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>

#include "TESForm.h"
#include "../NiAPI/NiPoint.h"
#include "../NiAPI/NiNode.h"

#pragma once

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESObjectREFR : public TESForm
				{
				private:
					char pad118[0x68];
					NiAPI::NiPoint3 _Rotate;
					NiAPI::NiPoint3 _Position;
					TESForm* _Parent;
					char pad1A0[0x20];
					TESForm* _ParentCell;
					void* _ResourceList;
					char pad1D0[0x10];
					std::uint16_t _Scale;
					std::uint16_t UnkNum;
				public:
					constexpr static uint8_t TYPE_ID = TESForm::FormType::ftReference;

					// The function returns BSFadeNode, it has not been studied, I use parent class
					[[nodiscard]] inline NiAPI::NiNode* GetFadeNode() const noexcept(true)
					{
						NiAPI::NiNode* pReturn = nullptr;
						vtbl_call<NiAPI::NiNode**>(0x708, this, &pReturn);
						return pReturn;
					}
					[[nodiscard]] inline TESForm* GetParent() const noexcept(true) { return _Parent; }
					[[nodiscard]] inline NiAPI::NiPoint3 GetRotate() const noexcept(true) { return _Rotate; }
					[[nodiscard]] inline NiAPI::NiPoint3 GetPosition() const noexcept(true) { return _Position; }
					[[nodiscard]] inline float GetScale() const noexcept(true) { return (float)_Scale / 100; }
					[[nodiscard]] inline TESForm* GetParentCell() const noexcept(true) { return _ParentCell; }
				};
				static_assert(sizeof(TESObjectREFR) == 0x1E8);
			}
		}
	}
}