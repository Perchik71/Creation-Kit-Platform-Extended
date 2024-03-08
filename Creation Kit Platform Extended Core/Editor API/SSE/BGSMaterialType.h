// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "BGSImpactDataSet.h"

#pragma pack(push, 1)

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			// size 0x58
			// func 101
			class BGSMaterialType : public TESForm
			{
			public:
				constexpr static uint8_t TYPE_ID = ftMaterialType;
				
				enum
				{
					mtfStairMaterial	= 1,
					mtfArrowStick		= 2,
				};
			public:
				virtual ~BGSMaterialType() = default;

				inline BGSMaterialType* GetParent() const { return _Parent; }
				inline const char* GetName() const { return _Name; }
				inline NiAPI::NiColor GetHavokDisplayColor() const { return _HavokDisplayColor; }
				inline void SetHavokDisplayColor(const NiAPI::NiColor& v) { _HavokDisplayColor = v; }
				inline void SetHavokDisplayColorRGB(uint8_t r, uint8_t g, uint8_t b) 
				{
					_HavokDisplayColor = { r / 255.f , g / 255.f, b / 255.f };
				}
				inline float GetBouyancy() const { return _Bouyancy; }
				inline void SetBouyancy(float v) { _Bouyancy = v; }

				inline bool HasStairMaterial() const { return (_MatFlags & mtfStairMaterial) == mtfStairMaterial; }
				inline bool HasArrowStick() const { return (_MatFlags & mtfArrowStick) == mtfArrowStick; }
				inline void SetStairMaterial(bool v) 
				{ 
					_MatFlags &= ~mtfStairMaterial;
					if (v)
						_MatFlags |= mtfStairMaterial;
				}
				inline void SetArrowStick(bool v)
				{
					_MatFlags &= ~mtfArrowStick;
					if (v)
						_MatFlags |= mtfArrowStick;
				}
				inline BGSImpactDataSet* GetImpactDataSet() const { return _ImpactDataSet; }

				READ_PROPERTY(GetParent) BGSMaterialType* Parent;
				READ_PROPERTY(GetName) const char* Name;
				PROPERTY(GetHavokDisplayColor, SetHavokDisplayColor) NiAPI::NiColor HavokDisplayColor;
				PROPERTY(GetBouyancy, SetBouyancy) float Bouyancy;
				READ_PROPERTY(GetImpactDataSet) BGSImpactDataSet* ImpactDataSet;
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

#pragma pack(pop)