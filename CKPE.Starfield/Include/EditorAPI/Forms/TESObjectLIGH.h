// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include "TESBoundAnimObject.h"
#include "TESFullName.h"
#include "TESWeightForm.h"
#include "TESValueForm.h"
#include "BGSDefaultLayer.h"
#include "BGSModelMaterialSwap.h"
#include "BGSDestructibleObjectForm.h"
#include "BGSEquip.h"
#include "BGSPropertySheet.h"
#include "BGSOpenCloseForm.h"
#include "BGSKeywordForm.h"
#include "BGSPapyrusScriptsComponent.h"
#include "../NiAPI/NiTypes.h"

#pragma once

namespace CKPE
{
	namespace Starfield
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// 0x920
				class TESObjectLIGH : public TESBoundAnimObject, public BGSDefaultLayer, public TESFullName,
					public BGSModelMaterialSwap, public TESWeightForm, public TESValueForm, 
					public BGSDestructibleObjectForm, public BGSEquipType, public BGSPropertySheet,
					public BGSOpenCloseForm, public BGSKeywordForm, public BGSPapyrusScriptsComponent
				{
				private:
					char pad710[0x64];
				public:
					constexpr static std::uint8_t TYPE_ID = ftLight;

					enum : std::uint32_t
					{
						kLightCanBeCarried = 1 << 1,
						kOffByDefault = 1 << 4,
						kDisableSpecular = 1 << 5,
						kDisableDistanceAttenuation = 1 << 6,
						kIsDirectSpot = 1 << 7,
						kUsePBRValues = 1 << 8,
						kFocusSpotlingBean = 1 << 9,
						kExternallyControlle = 1 << 12,
					};

					enum Type : std::uint8_t
					{
						kOmnidirectional = 0,
						kShadowSpotling,
						kNonShadowSpotling,
					};

					enum Flicker : std::uint8_t
					{
						kNone = 0,
						kFlicker,
						kPulse,
					};

					float Radius;
					NiAPI::NiRGB SpecularColor;
					std::uint32_t Flags;
					float FallofExponent;
					float FOV;
					float a3;
					float Period;				// result to form = 1 / Period
					float IntensityAmplitude;
					float MovementAmplitude;
					float ShadowOffset;
					std::uint32_t u1;
					std::uint32_t u2;
					std::uint32_t u3;
					Type TypeLight;
					Flicker FlickerEffect;
				};
				static_assert(offsetof(TESObjectLIGH, SpecularColor) == 0x778);
				static_assert(sizeof(TESObjectLIGH) == 0x7B0);
			}
		}
	}
}