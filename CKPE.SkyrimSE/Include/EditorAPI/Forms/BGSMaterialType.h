// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "BGSImpactDataSet.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// size 0x58
				// func 101
				class BGSMaterialType : public TESForm
				{
				public:
					constexpr static uint8_t TYPE_ID = ftMaterialType;

					enum
					{
						mtfStairMaterial = 1,
						mtfArrowStick = 2,
					};
				public:
					virtual ~BGSMaterialType() = default;

					inline BGSMaterialType* GetParent() const noexcept(true) { return _Parent; }
					inline const char* GetName() const noexcept(true) { return _Name; }
					inline NiAPI::NiColor GetHavokDisplayColor() const noexcept(true) { return _HavokDisplayColor; }
					inline void SetHavokDisplayColor(const NiAPI::NiColor& v) noexcept(true) { _HavokDisplayColor = v; }
					inline void SetHavokDisplayColorRGB(uint8_t r, uint8_t g, uint8_t b) noexcept(true)
					{ _HavokDisplayColor = { r / 255.f , g / 255.f, b / 255.f }; }
					inline float GetBouyancy() const noexcept(true) { return _Bouyancy; }
					inline void SetBouyancy(float v) noexcept(true) { _Bouyancy = v; }
					inline bool HasStairMaterial() const noexcept(true) { return (_MatFlags & mtfStairMaterial) == mtfStairMaterial; }
					inline bool HasArrowStick() const noexcept(true) { return (_MatFlags & mtfArrowStick) == mtfArrowStick; }
					inline void SetStairMaterial(bool v) noexcept(true)
					{
						_MatFlags &= ~mtfStairMaterial;
						if (v)
							_MatFlags |= mtfStairMaterial;
					}
					inline void SetArrowStick(bool v) noexcept(true)
					{
						_MatFlags &= ~mtfArrowStick;
						if (v)
							_MatFlags |= mtfArrowStick;
					}
					inline BGSImpactDataSet* GetImpactDataSet() const noexcept(true) { return _ImpactDataSet; }

					CKPE_READ_PROPERTY(GetParent) BGSMaterialType* Parent;
					CKPE_READ_PROPERTY(GetName) const char* Name;
					CKPE_PROPERTY(GetHavokDisplayColor, SetHavokDisplayColor) NiAPI::NiColor HavokDisplayColor;
					CKPE_PROPERTY(GetBouyancy, SetBouyancy) float Bouyancy;
					CKPE_READ_PROPERTY(GetImpactDataSet) BGSImpactDataSet* ImpactDataSet;
				private:
					BGSMaterialType* _Parent;
					const char* _Name;
					float _Unk;
					NiAPI::NiColor _HavokDisplayColor;
					float _Bouyancy;
					uint32_t _MatFlags;
					BGSImpactDataSet* _ImpactDataSet;
				};
				static_assert(sizeof(BGSMaterialType) == 0x58);
			}
		}
	}
}