// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <EditorAPI/N/NiColor.h>
#include <EditorAPI/N/NiObject.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiFloatData;
				class NiColorData;
				class NiObjectNET;
				class NiQuatTransform;
				class NiQuaternion;
				class NiPoint3;
				class NiAnimationKey;
				class NiAVObjectPalette;
				class NiBoolInterpolator;

				class NiInterpolator : public NiObject
				{
				public:
					/* 02 */ virtual NiRTTI* GetRTTI() const noexcept(true) override;
					/* 1B */ virtual void LoadBinary(NiStream& Stream) noexcept(true) override;
					/* 1C */ virtual void LinkObject(NiStream& Stream) noexcept(true) override;
					/* 1D */ virtual bool RegisterStreamables(NiStream& Stream) noexcept(true) override;
					/* 1E */ virtual void SaveBinary(NiStream& Stream) noexcept(true) override;
					/* 1F */ virtual bool IsEqual(NiObject* Object) noexcept(true) override;
					/* 20 */ virtual void GetViewerStringsArray(void* Strings) noexcept(true) override;
					/* 2A */ virtual bool UpdateTransform(float Time, NiObjectNET* Target, NiQuatTransform& Value) noexcept(true);
					/* 2B */ virtual bool UpdateColor(float Time, NiObjectNET* Target, NiColorA& Value) noexcept(true);
					/* 2C */ virtual bool UpdatePoint3(float Time, NiObjectNET* Target, NiPoint3& Value) noexcept(true);
					/* 2D */ virtual bool UpdateQuaternion(float Time, NiObjectNET* Target, NiQuaternion& Value) noexcept(true);
					/* 2E */ virtual bool UpdateFloat(float Time, NiObjectNET* Target, float& Value) noexcept(true);
					/* 2F */ virtual bool UpdateBool(float Time, NiObjectNET* Target, bool& Value) noexcept(true);
					/* 30 */ [[nodiscard]] virtual bool IsBoolValueSupported() const noexcept(true);
					/* 31 */ [[nodiscard]] virtual bool IsFloatValueSupported() const noexcept(true);
					/* 32 */ [[nodiscard]] virtual bool IsQuaternionValueSupported() const noexcept(true);
					/* 33 */ [[nodiscard]] virtual bool IsPoint3ValueSupported() const noexcept(true);
					/* 34 */ [[nodiscard]] virtual bool IsColorValueSupported() const noexcept(true);
					/* 35 */ [[nodiscard]] virtual bool IsTransformValueSupported() const noexcept(true);
					/* 36 */ virtual void Collapse() noexcept(true);
					/* 37 */ virtual void GuaranteeTimeRange(float Start, float End) noexcept(true);
					/* 38 */ [[nodiscard]] virtual NiInterpolator* GetSequenceInterpolator(float Start, float End) noexcept(true);
					/* 39 */ virtual bool ResolveDependencies(NiAVObjectPalette* Palette) noexcept(true);
					/* 3A */ virtual bool SetUpDependencies() noexcept(true);
					/* 3B */ [[nodiscard]] virtual bool AlwaysUpdate() const noexcept(true);
					/* 3C */ [[nodiscard]] virtual NiBoolInterpolator* IsBoolInterpolator() noexcept(true);
					/* 3D */ virtual void GetActiveTimeRange(float& Start, float& End) const noexcept(true) = 0;

					[[nodiscard]] inline float GetLastTime() const noexcept(true) { return _LastTime; }
				private:
					/* 0x10 */ float _LastTime;
					/* 0x14 */ std::uint32_t _pad14;
				public:
					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(NiInterpolator, _LastTime) == 0x10;
					}
				};
				static_assert(sizeof(NiInterpolator) == 0x18);
				static_assert(NiInterpolator::VerifyLayout());

				class NiKeyBasedInterpolator : public NiInterpolator
				{
				public:
					/* 02 */ virtual NiRTTI* GetRTTI() const noexcept(true) override;
					/* 1B */ virtual void LoadBinary(NiStream& Stream) noexcept(true) override;
					/* 1C */ virtual void LinkObject(NiStream& Stream) noexcept(true) override;
					/* 1D */ virtual bool RegisterStreamables(NiStream& Stream) noexcept(true) override;
					/* 1E */ virtual void SaveBinary(NiStream& Stream) noexcept(true) override;
					/* 1F */ virtual bool IsEqual(NiObject* Object) noexcept(true) override;
					/* 20 */ virtual void GetViewerStringsArray(void* Strings) noexcept(true) override;
					void GetActiveTimeRange(float& Start, float& End) const noexcept(true) override;
					/* 3E */ [[nodiscard]] virtual std::uint16_t GetKeyChannelCount() const noexcept(true) = 0;
					/* 3F */ [[nodiscard]] virtual std::uint32_t GetKeyCount(std::uint16_t Channel) const noexcept(true) = 0;
					/* 40 */ [[nodiscard]] virtual std::uint32_t GetKeyType(std::uint16_t Channel) const noexcept(true) = 0;
					/* 41 */ [[nodiscard]] virtual std::uint32_t GetKeyContent(std::uint16_t Channel) const noexcept(true) = 0;
					/* 42 */ [[nodiscard]] virtual NiAnimationKey* GetKeys(std::uint16_t Channel) const noexcept(true) = 0;
					/* 43 */ [[nodiscard]] virtual std::uint8_t GetKeyStride(std::uint16_t Channel) const noexcept(true) = 0;
					/* 44 */ [[nodiscard]] virtual bool IsPosed(std::uint16_t Channel) const noexcept(true) = 0;
				};
				static_assert(sizeof(NiKeyBasedInterpolator) == 0x18);

				// The CK embeds interpolators by value in TESImageSpaceModifier.
				class NiFloatInterpolator : public NiKeyBasedInterpolator
				{
				public:
					/* 02 */ virtual NiRTTI* GetRTTI() const noexcept(true) override;
					/* 1A */ virtual NiObject* CreateClone(NiCloningProcess& Process) noexcept(true) override;
					/* 1B */ virtual void LoadBinary(NiStream& Stream) noexcept(true) override;
					[[nodiscard]] std::uint16_t GetKeyChannelCount() const noexcept(true) override;
					[[nodiscard]] std::uint32_t GetKeyCount(std::uint16_t Channel) const noexcept(true) override;
					[[nodiscard]] std::uint32_t GetKeyType(std::uint16_t Channel) const noexcept(true) override;
					[[nodiscard]] std::uint32_t GetKeyContent(std::uint16_t Channel) const noexcept(true) override;
					[[nodiscard]] NiAnimationKey* GetKeys(std::uint16_t Channel) const noexcept(true) override;
					[[nodiscard]] std::uint8_t GetKeyStride(std::uint16_t Channel) const noexcept(true) override;
					[[nodiscard]] bool IsPosed(std::uint16_t Channel) const noexcept(true) override;

					float value; // 18 - -FLT_MAX (invalid) on construction
					std::uint32_t pad1C; // 1C
					NiFloatData* data; // 20 - reference counted (count at +8)
					std::uint32_t lastIndex; // 28
					std::uint32_t pad2C; // 2C
				};
				static_assert(offsetof(NiFloatInterpolator, value) == 0x18);
				static_assert(offsetof(NiFloatInterpolator, data) == 0x20);
				static_assert(sizeof(NiFloatInterpolator) == 0x30);

				class NiColorInterpolator : public NiKeyBasedInterpolator
				{
				public:
					/* 02 */ virtual NiRTTI* GetRTTI() const noexcept(true) override;
					/* 1A */ virtual NiObject* CreateClone(NiCloningProcess& Process) noexcept(true) override;
					/* 1B */ virtual void LoadBinary(NiStream& Stream) noexcept(true) override;
					/* 1C */ virtual void LinkObject(NiStream& Stream) noexcept(true) override;
					/* 1D */ virtual bool RegisterStreamables(NiStream& Stream) noexcept(true) override;
					/* 1E */ virtual void SaveBinary(NiStream& Stream) noexcept(true) override;
					/* 1F */ virtual bool IsEqual(NiObject* Object) noexcept(true) override;
					/* 20 */ virtual void GetViewerStringsArray(void* Strings) noexcept(true) override;
					[[nodiscard]] std::uint16_t GetKeyChannelCount() const noexcept(true) override;
					[[nodiscard]] std::uint32_t GetKeyCount(std::uint16_t Channel) const noexcept(true) override;
					[[nodiscard]] std::uint32_t GetKeyType(std::uint16_t Channel) const noexcept(true) override;
					[[nodiscard]] std::uint32_t GetKeyContent(std::uint16_t Channel) const noexcept(true) override;
					[[nodiscard]] NiAnimationKey* GetKeys(std::uint16_t Channel) const noexcept(true) override;
					[[nodiscard]] std::uint8_t GetKeyStride(std::uint16_t Channel) const noexcept(true) override;
					[[nodiscard]] bool IsPosed(std::uint16_t Channel) const noexcept(true) override;

					NiColorA value; // 18 - invalid marker on construction
					NiColorData* data; // 28 - reference counted (count at +8)
					std::uint32_t lastIndex; // 30
					std::uint32_t pad34; // 34
				};
				static_assert(offsetof(NiColorInterpolator, value) == 0x18);
				static_assert(offsetof(NiColorInterpolator, data) == 0x28);
				static_assert(sizeof(NiColorInterpolator) == 0x38);
			}
		}
	}
}
