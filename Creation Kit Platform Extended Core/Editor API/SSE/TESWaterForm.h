#pragma once

#include "TESForm.h"
#include "../BGSLocalizedString.h"
#include "NiAPI/NiTypes.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			class TESWaterForm : public TESForm
			{
			public:
				virtual ~TESWaterForm() = default;

				inline const char* GetName() const { return _Name.c_str(); }
				inline uint32_t GetDamagePerSecond() const { return _DamagePerSecond; }

				inline NiAPI::NiRGBA GetShallowColor() const { return _Colors[0]; }
				inline NiAPI::NiRGBA GetDeepColor() const { return _Colors[1]; }
				inline NiAPI::NiRGBA GetReflectionColor() const { return _Colors[2]; }

				inline float GetDSStartingSize() const { return _DisplacementSimulator[0]; }
				inline float GetDSForce() const { return _DisplacementSimulator[1]; }
				inline float GetDSVelocity() const { return _DisplacementSimulator[2]; }
				inline float GetDSFalloff() const { return _DisplacementSimulator[3]; }
				inline float GetDSDampner() const { return _DisplacementSimulator[4]; }

				inline NiAPI::NiPoint3 GetLinerVelocity() const { return _LinerVelocity; }
				inline NiAPI::NiPoint3 GetAngularVelocity() const { return _AngularVelocity; }

				READ_PROPERTY(GetName) const char* Name;
				READ_PROPERTY(GetDamagePerSecond) uint32_t DamagePerSecond;

				READ_PROPERTY(GetShallowColor) NiAPI::NiRGBA ShallowColor;
				READ_PROPERTY(GetDeepColor) NiAPI::NiRGBA DeepColor;
				READ_PROPERTY(GetReflectionColor) NiAPI::NiRGBA ReflectionColor;

				READ_PROPERTY(GetDSStartingSize) float DSStartingSize;
				READ_PROPERTY(GetDSForce) float DSForce;
				READ_PROPERTY(GetDSVelocity) float DSVelocity;
				READ_PROPERTY(GetDSFalloff) float DSFalloff;
				READ_PROPERTY(GetDSDampner) float DSDampner;

				READ_PROPERTY(GetLinerVelocity) NiAPI::NiPoint3 LinerVelocity;
				READ_PROPERTY(GetAngularVelocity) NiAPI::NiPoint3 AngularVelocity;
			private:
				char pad28[8];
				BGSLocalizedString<char> _Name;
				char pad40[8];
				uint32_t _DamagePerSecond;
				char pad4C[0x3C];
				char pad88[0xE0];
				NiAPI::NiRGBA _Colors[3];
				char pad174[0x14];
				float _DisplacementSimulator[5];
				char pad19C[0x12C];
				NiAPI::NiPoint3 _LinerVelocity;
				NiAPI::NiPoint3 _AngularVelocity;
			};
			static_assert(sizeof(TESWaterForm) == 0x2E0);
		}
	}
}

