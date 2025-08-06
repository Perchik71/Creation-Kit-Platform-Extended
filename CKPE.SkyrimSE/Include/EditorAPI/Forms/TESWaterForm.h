#pragma once

#include <EditorAPI/NiAPI/NiTypes.h>
#include "TESForm.h"
#include "TESFullName.h"

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESWaterForm : public TESForm, public TESFullName
				{
				public:
					virtual ~TESWaterForm() = default;

					inline std::uint32_t GetDamagePerSecond() const { return _DamagePerSecond; }

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

					CKPE_READ_PROPERTY(GetName) const char* Name;
					CKPE_READ_PROPERTY(GetDamagePerSecond) std::uint32_t DamagePerSecond;

					CKPE_READ_PROPERTY(GetShallowColor) NiAPI::NiRGBA ShallowColor;
					CKPE_READ_PROPERTY(GetDeepColor) NiAPI::NiRGBA DeepColor;
					CKPE_READ_PROPERTY(GetReflectionColor) NiAPI::NiRGBA ReflectionColor;

					CKPE_READ_PROPERTY(GetDSStartingSize) float DSStartingSize;
					CKPE_READ_PROPERTY(GetDSForce) float DSForce;
					CKPE_READ_PROPERTY(GetDSVelocity) float DSVelocity;
					CKPE_READ_PROPERTY(GetDSFalloff) float DSFalloff;
					CKPE_READ_PROPERTY(GetDSDampner) float DSDampner;

					CKPE_READ_PROPERTY(GetLinerVelocity) NiAPI::NiPoint3 LinerVelocity;
					CKPE_READ_PROPERTY(GetAngularVelocity) NiAPI::NiPoint3 AngularVelocity;
				private:
					char pad40[8];
					std::uint32_t _DamagePerSecond;
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
}

