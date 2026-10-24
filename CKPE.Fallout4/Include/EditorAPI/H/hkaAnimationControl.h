// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkaAnimation.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkaAnimationControlListener;
	class hkaDefaultAnimationControlListener;
	class hkaDefaultAnimationControlMapperData;

	class hkaAnimationControl : public hkReferencedObject
	{
	public:
		/* 04 */ virtual void Update(float StepDelta) = 0;
		/* 05 */ virtual void GetFutureTime(float StepDelta, float& LocalTimeOut,
			std::int32_t& LoopsOut) const = 0;
		/* 06 */ virtual void SampleTracks(hkQsTransformf* TransformTracksOut,
			float* FloatTracksOut) const;
		/* 07 */ virtual void SamplePartialTracks(std::uint32_t MaxTransformTracks,
			hkQsTransformf* TransformTracksOut, std::uint32_t MaxFloatTracks,
			float* FloatTracksOut) const;
		/* 08 */ virtual void GetExtractedMotionDeltaReferenceFrame(float DeltaTime,
			hkQsTransformf& DeltaMotionOut) const;
		/* 09 */ [[nodiscard]] virtual std::int32_t GetNumberOfTransformTracks() const;
		/* 0A */ [[nodiscard]] virtual std::int32_t GetNumberOfFloatTracks() const;
		/* 0B */ [[nodiscard]] virtual std::int32_t GetNumTransformTrackToBoneIndices() const;
		/* 0C */ [[nodiscard]] virtual const std::int16_t* GetTransformTrackToBoneIndices() const;
		/* 0D */ [[nodiscard]] virtual std::int32_t GetNumFloatTrackToFloatSlotIndices() const;
		/* 0E */ [[nodiscard]] virtual const std::int16_t* GetFloatTrackToFloatSlotIndices() const;
		/* 0F */ [[nodiscard]] virtual std::int32_t GetNumPartitionIndices() const;
		/* 10 */ [[nodiscard]] virtual const std::int16_t* GetPartitionIndices() const;
		/* 11 */ [[nodiscard]] virtual const hkaSkeletonMapper* GetSkeletonMapper() const;

		/* 0x010 */ float m_localTime;
		/* 0x014 */ float m_weight;
		/* 0x018 */ hkArray<std::uint8_t> m_transformTrackWeights;
		/* 0x028 */ hkArray<std::uint8_t> m_floatTrackWeights;
		/* 0x038 */ const hkaAnimationBinding* m_binding;
		/* 0x040 */ hkArray<hkaAnimationControlListener*> m_listeners;
		/* 0x050 */ float unk050;
		/* 0x054 */ std::uint8_t pad054[0x4];
	};
	static_assert(sizeof(hkaAnimationControl) == 0x58);
	static_assert(offsetof(hkaAnimationControl, m_localTime) == 0x10);
	static_assert(offsetof(hkaAnimationControl, m_weight) == 0x14);
	static_assert(offsetof(hkaAnimationControl, m_transformTrackWeights) == 0x18);
	static_assert(offsetof(hkaAnimationControl, m_floatTrackWeights) == 0x28);
	static_assert(offsetof(hkaAnimationControl, m_binding) == 0x38);
	static_assert(offsetof(hkaAnimationControl, m_listeners) == 0x40);

	class hkaDefaultAnimationControl : public hkaAnimationControl
	{
	public:
		/* 0x058 */ float m_masterWeight;
		/* 0x05C */ float m_playbackSpeed;
		/* 0x060 */ std::uint8_t pad060[0x3C];
		/* 0x09C */ float m_cropStartAmountLocalTime;
		/* 0x0A0 */ float m_cropEndAmountLocalTime;
		/* 0x0A4 */ std::uint8_t pad0A4[0x4];
		/* 0x0A8 */ hkArray<hkaDefaultAnimationControlListener*> m_defaultListeners;
		/* 0x0B8 */ hkaDefaultAnimationControlMapperData* m_mapper;

		/* 04 */ void Update(float StepDelta) override;
		/* 05 */ void GetFutureTime(float StepDelta, float& LocalTimeOut,
			std::int32_t& LoopsOut) const override;
	};
	static_assert(sizeof(hkaDefaultAnimationControl) == 0xC0);
	static_assert(offsetof(hkaDefaultAnimationControl, m_masterWeight) == 0x58);
	static_assert(offsetof(hkaDefaultAnimationControl, m_playbackSpeed) == 0x5C);
	static_assert(offsetof(hkaDefaultAnimationControl, m_cropStartAmountLocalTime) == 0x9C);
	static_assert(offsetof(hkaDefaultAnimationControl, m_cropEndAmountLocalTime) == 0xA0);
	static_assert(offsetof(hkaDefaultAnimationControl, m_defaultListeners) == 0xA8);
	static_assert(offsetof(hkaDefaultAnimationControl, m_mapper) == 0xB8);
}
