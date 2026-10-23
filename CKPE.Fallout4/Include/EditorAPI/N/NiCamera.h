// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/IDs.h>
#include <CKPE.Common.ID.h>
#include <CKPE.Utils.h>
#include <EditorAPI/N/NiAVObject.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiCamera : public NiAVObject
				{
				public:
					using WorldToCameraMatrix = float[4][4];
					inline static constexpr std::size_t VTableCount = 0x3C;

					virtual ~NiCamera() = default;

					[[nodiscard]] inline const WorldToCameraMatrix& GetWorldToCamera() const noexcept(true) { return _WorldToCamera; }
					[[nodiscard]] inline const NiFrustum& GetFrustum() const noexcept(true) { return _Frustum; }
					[[nodiscard]] inline const NiRect<float>& GetPort() const noexcept(true) { return _Port; }
					[[nodiscard]] inline float GetMinNearPlaneDist() const noexcept(true) { return _MinNearPlaneDist; }
					[[nodiscard]] inline float GetMaxFarNearRatio() const noexcept(true) { return _MaxFarNearRatio; }
					[[nodiscard]] inline float GetLODAdjust() const noexcept(true) { return _LODAdjust; }

					CKPE_READ_PROPERTY(GetFrustum) const NiFrustum& Frustum;
					CKPE_READ_PROPERTY(GetPort) const NiRect<float>& Port;
					CKPE_READ_PROPERTY(GetMinNearPlaneDist) float MinNearPlaneDist;
					CKPE_READ_PROPERTY(GetMaxFarNearRatio) float MaxFarNearRatio;
					CKPE_READ_PROPERTY(GetLODAdjust) float LODAdjust;

					inline void SetFrustum(const NiFrustum& Frustum) noexcept(true)
					{
						CKPE::fast_call<void>(EditorAPI::IDs::NiCamera::SetFrustum.Address(), this, &Frustum);
					}

					inline void UpdateWorldToCamera() noexcept(true)
					{
						CKPE::fast_call<void>(EditorAPI::IDs::NiCamera::UpdateWorldToCamera.Address(), this);
					}

					[[nodiscard]] inline bool LookAt(const NiPoint3& Target, const NiPoint3& Up) noexcept(true)
					{
						return CKPE::fast_call<bool>(EditorAPI::IDs::NiCamera::LookAt.Address(), this, &Target, &Up);
					}

					[[nodiscard]] inline bool FitNearFarToBound(const NiBound& Bound) noexcept(true)
					{
						return CKPE::fast_call<bool>(EditorAPI::IDs::NiCamera::FitNearFarToBound.Address(), this, &Bound);
					}

					[[nodiscard]] inline bool WindowPointToRay(std::int32_t X, std::int32_t Y,
						NiPoint3& Origin, NiPoint3& Direction, float Width, float Height) const noexcept(true)
					{
						return CKPE::fast_call<bool>(EditorAPI::IDs::NiCamera::WindowPointToRay.Address(),
							this, X, Y, &Origin, &Direction, Width, Height);
					}

					// X and Y are frustum-plane coordinates, not window pixels.
					inline void ScreenPtToWorldRay(float X, float Y,
						NiPoint3& Origin, NiPoint3& Direction) const noexcept(true)
					{
						CKPE::fast_call<void>(EditorAPI::IDs::NiCamera::ScreenPtToWorldRay.Address(),
							this, X, Y, &Origin, &Direction);
					}

					[[nodiscard]] inline bool WorldPtToScreenPt(const NiPoint3& Point,
						float& X, float& Y, float ZeroTolerance = 1.0e-5f) const noexcept(true)
					{
						return CKPE::fast_call<bool>(EditorAPI::IDs::NiCamera::WorldPtToScreenPt.Address(),
							this, &Point, &X, &Y, ZeroTolerance);
					}

					// This variant uses abs(W) and does not reject off-screen points.
					[[nodiscard]] inline bool WorldPtToScreenPt3(const NiPoint3& Point,
						float& X, float& Y, float& Z, float ZeroTolerance = 1.0e-5f) const noexcept(true)
					{
						return CKPE::fast_call<bool>(EditorAPI::IDs::NiCamera::WorldPtToScreenPt3.Address(),
							this, &Point, &X, &Y, &Z, ZeroTolerance);
					}

				private:
					/* 0x120 */ WorldToCameraMatrix _WorldToCamera;
					/* 0x160 */ NiFrustum _Frustum;
					/* 0x17C */ float _MinNearPlaneDist;
					/* 0x180 */ float _MaxFarNearRatio;
					/* 0x184 */ NiRect<float> _Port;
					/* 0x194 */ float _LODAdjust;
					/* 0x198 */ std::uint8_t _Pad198[0x8];

				public:
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(NiCamera, _WorldToCamera) == 0x120 &&
							offsetof(NiCamera, _Frustum) == 0x160 &&
							offsetof(NiCamera, _MinNearPlaneDist) == 0x17C &&
							offsetof(NiCamera, _MaxFarNearRatio) == 0x180 &&
							offsetof(NiCamera, _Port) == 0x184 &&
							offsetof(NiCamera, _LODAdjust) == 0x194;
					}
				};
				static_assert(sizeof(NiCamera) == 0x1A0);
				static_assert(NiCamera::VerifyLayout());
			}
		}
	}
}
