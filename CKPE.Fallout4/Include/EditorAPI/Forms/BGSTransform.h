// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <EditorAPI/Forms/TESForm.h>
#include <EditorAPI/NiAPI/NiTypes.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				// size = 0x50
				// func = 124
				class BGSTransform : public TESForm
				{
				public:
					constexpr static std::uint8_t TYPE_ID = ftTransform;

					virtual ~BGSTransform() = default;
		
					[[nodiscard]] inline NiAPI::NiPoint3 GetPosition() const noexcept(true) { return _Position; }
					inline void SetPosition(const NiAPI::NiPoint3& p) noexcept(true) { _Position = p; }
					// in radians
					[[nodiscard]] inline NiAPI::NiPoint3 GetRotation() const noexcept(true) { return _Rotation; }
					// in radians
					inline void SetRotation(const NiAPI::NiPoint3& r) noexcept(true) { _Rotation = r; }

					inline void SetPositionXYZ(float x, float y, float z) noexcept(true) { _Position = { x, y, z }; }
					// in radians
					inline void SetRotationXYZ(float x, float y, float z) noexcept(true) { _Rotation = { x, y, z }; }

					inline void GetRotationXYZ_InDegrees(float& x, float& y, float& z) const noexcept(true)
					{
						float degre = (float)(180.0f / M_PI);

						x = _Rotation.x * degre;
						y = _Rotation.y * degre;
						z = _Rotation.z * degre;
					}
					inline void SetRotationXYZ_InDegrees(float x, float y, float z) noexcept(true)
					{
						float r = (float)(M_PI / 180.0f);
						_Rotation = { x * r, y * r, z * r };
					}

					[[nodiscard]] inline float GetScale() const noexcept(true) { return _Scale; }
					inline void SetScale(float v) noexcept(true) { _Scale = v; }
					[[nodiscard]] inline float GetZoomAmountsMin() const noexcept(true) { return _ZoomAmountsMin; }
					inline void SetZoomAmountsMin(float v) noexcept(true) { _ZoomAmountsMin = v; }
					[[nodiscard]] inline float GetZoomAmountsMax() const noexcept(true) { return _ZoomAmountsMax; }
					inline void SetZoomAmountsMax(float v) noexcept(true) { _ZoomAmountsMax = v; }
				public:
					CKPE_PROPERTY(GetPosition, SetPosition) NiAPI::NiPoint3 Position;
					CKPE_PROPERTY(GetRotation, SetRotation) NiAPI::NiPoint3 Rotation;
					CKPE_PROPERTY(GetScale, SetScale) float Scale;
					CKPE_PROPERTY(GetZoomAmountsMin, SetZoomAmountsMin) float ZoomAmountsMin;
					CKPE_PROPERTY(GetZoomAmountsMax, SetZoomAmountsMax) float ZoomAmountsMax;
				private:
					NiAPI::NiPoint3 _Position;
					NiAPI::NiPoint3 _Rotation;
					float _Scale;
					float _ZoomAmountsMin;
					float _ZoomAmountsMax;
				};
				static_assert(sizeof(BGSTransform) == 0x50);
			}
		}
	}
}
