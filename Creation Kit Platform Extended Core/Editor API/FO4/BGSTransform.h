// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "TESFormF4.h"
#include "NiAPI\NiTypes.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			// size = 0x80
			// func = 124
			class BGSTransform : public TESForm
			{
			public:
				virtual ~BGSTransform() = default;
			public:
				inline NiPoint3 GetPosition() { return _Position; }
				inline void SetPosition(const NiPoint3& p) { _Position = p; }
				// in radians
				inline NiPoint3 GetRotation() { return _Rotation; }
				// in radians
				inline void SetRotation(const NiPoint3& r) { _Rotation = r; }

				inline void SetPositionXYZ(float x, float y, float z) { _Position = { x, y, z }; }
				// in radians
				inline void SetRotationXYZ(float x, float y, float z) { _Rotation = { x, y, z }; }

				inline void GetRotationXYZ_InDegrees(float& x, float& y, float& z) 
				{ 
					float degre = (float)(180.0f / M_PI);
					
					x = _Rotation.x * degre;
					y = _Rotation.y * degre;
					z = _Rotation.z * degre;
				}
				inline void SetRotationXYZ_InDegrees(float x, float y, float z) 
				{ 
					float r = (float)(M_PI / 180.0f);
					_Rotation = { x * r, y * r, z * r };
				}

				inline float GetScale() const { return _Scale; }
				inline void SetScale(float v) { _Scale = v; }
				inline float GetZoomAmountsMin() const { return _ZoomAmountsMin; }
				inline void SetZoomAmountsMin(float v) { _ZoomAmountsMin = v; }
				inline float GetZoomAmountsMax() const { return _ZoomAmountsMax; }
				inline void SetZoomAmountsMax(float v) { _ZoomAmountsMax = v; }
			public:
				PROPERTY(GetPosition, SetPosition) NiPoint3 Position;
				PROPERTY(GetRotation, SetRotation) NiPoint3 Rotation;
				PROPERTY(GetScale, SetScale) float Scale;
				PROPERTY(GetZoomAmountsMin, SetZoomAmountsMin) float ZoomAmountsMin;
				PROPERTY(GetZoomAmountsMax, SetZoomAmountsMax) float ZoomAmountsMax;
			private:
				NiPoint3 _Position;
				NiPoint3 _Rotation;
				float _Scale;
				float _ZoomAmountsMin;
				float _ZoomAmountsMax;
				char pad4C[0x34];
			};
			static_assert(sizeof(BGSTransform) == 0x80);
		}
	}
}
