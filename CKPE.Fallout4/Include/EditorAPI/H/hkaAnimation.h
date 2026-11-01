// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseObject.h>
#include <EditorAPI/H/hkBaseTypes.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkLocalFrame;
	class hkReferencedObject;
	class hkaAngularReferenceFrame;
	class hkaAnimatedReferenceFrame;
	class hkaAnimation;
	class hkaAnimationBinding;
	class hkaAnimationContainer;
	class hkaAnimationPreviewColorContainer;
	class hkaAnnotationTrack;
	class hkaAnnotationTrackAnnotation;
	class hkaBone;
	class hkaBoneAttachment;
	class hkaDefaultAnimatedReferenceFrame;
	class hkaDirectionalReferenceFrame;
	class hkaFootstepAnalysisInfo;
	class hkaFootstepAnalysisInfoContainer;
	class hkaInterleavedUncompressedAnimation;
	class hkaLosslessCompressedAnimation;
	class hkaMeshBinding;
	class hkaMeshBindingMapping;
	class hkaParameterizedAnimationReferenceFrame;
	class hkaParameterizedReferenceFrame;
	class hkaPredictiveCompressedAnimation;
	class hkaPredictiveCompressedAnimationTrackCompressionParams;
	class hkaQuantizedAnimation;
	class hkaQuantizedAnimationTrackCompressionParams;
	class hkaReferencePoseAnimation;
	class hkaSkeleton;
	class hkaSkeletonLocalFrameOnBone;
	class hkaSkeletonMapper;
	class hkaSkeletonMapperData;
	class hkaSkeletonMapperDataChainMapping;
	class hkaSkeletonMapperDataPartitionMappingRange;
	class hkaSkeletonMapperDataSimpleMapping;
	class hkaSkeletonPartition;
	class hkaSplineCompressedAnimation;
	class hkaSplineCompressedAnimationAnimationCompressionParams;
	class hkaSplineCompressedAnimationTrackCompressionParams;
	class hkxMesh;

	class hkaAnimatedReferenceFrame : public hkReferencedObject
	{
	public:
		enum class hkaReferenceFrameTypeEnum : std::int32_t
		{
			REFERENCE_FRAME_UNKNOWN = 0,
			REFERENCE_FRAME_DEFAULT = 1,
			REFERENCE_FRAME_PARAMETRIC = 2,
		};

		/* 0x010 */ std::int8_t m_frameType;
		/* 0x011 */ std::uint8_t pad011[0x7];

	private:
		/* 04 */ virtual void sub_hkaAnimatedReferenceFrame_04();
		/* 05 */ virtual void sub_hkaAnimatedReferenceFrame_05();
		/* 06 */ virtual void sub_hkaAnimatedReferenceFrame_06();
	};
	static_assert(sizeof(hkaAnimatedReferenceFrame) == 0x18);
	static_assert(offsetof(hkaAnimatedReferenceFrame, m_frameType) == 0x10);

	class hkaDefaultAnimatedReferenceFrame : public hkaAnimatedReferenceFrame
	{
	public:
		/* 0x018 */ std::uint8_t pad018[0x8];
		/* 0x020 */ hkVector4f m_up;
		/* 0x030 */ hkVector4f m_forward;
		/* 0x040 */ float m_duration;
		/* 0x044 */ std::uint8_t pad044[0x4];
		/* 0x048 */ hkArray<hkVector4f> m_referenceFrameSamples;
		/* 0x058 */ std::uint8_t pad058[0x8];
	};
	static_assert(sizeof(hkaDefaultAnimatedReferenceFrame) == 0x60);
	static_assert(offsetof(hkaDefaultAnimatedReferenceFrame, m_up) == 0x20);
	static_assert(offsetof(hkaDefaultAnimatedReferenceFrame, m_forward) == 0x30);
	static_assert(offsetof(hkaDefaultAnimatedReferenceFrame, m_duration) == 0x40);
	static_assert(offsetof(hkaDefaultAnimatedReferenceFrame, m_referenceFrameSamples) == 0x48);

	class hkaParameterizedReferenceFrame : public hkaDefaultAnimatedReferenceFrame
	{
	public:

	private:
		/* 07 */ virtual void sub_hkaParameterizedReferenceFrame_07();
		/* 08 */ virtual void sub_hkaParameterizedReferenceFrame_08();
	};
	static_assert(sizeof(hkaParameterizedReferenceFrame) == 0x60);

	class hkaAngularReferenceFrame : public hkaParameterizedReferenceFrame
	{
	public:
		/* 0x060 */ float m_topAngle;
		/* 0x064 */ float m_radius;
		/* 0x068 */ std::uint8_t pad068[0x8];
	};
	static_assert(sizeof(hkaAngularReferenceFrame) == 0x70);
	static_assert(offsetof(hkaAngularReferenceFrame, m_topAngle) == 0x60);
	static_assert(offsetof(hkaAngularReferenceFrame, m_radius) == 0x64);

	class hkaAnimation : public hkReferencedObject
	{
	public:
		enum class AnimationType : std::int32_t
		{
			HK_UNKNOWN_ANIMATION = 0,
			HK_INTERLEAVED_ANIMATION = 1,
			HK_MIRRORED_ANIMATION = 2,
			HK_SPLINE_COMPRESSED_ANIMATION = 3,
			HK_QUANTIZED_COMPRESSED_ANIMATION = 4,
			HK_PREDICTIVE_COMPRESSED_ANIMATION = 5,
			HK_REFERENCE_POSE_ANIMATION = 6,
		};

		/* 0x010 */ std::int32_t m_type;
		/* 0x014 */ float m_duration;
		/* 0x018 */ std::int32_t m_numberOfTransformTracks;
		/* 0x01C */ std::int32_t m_numberOfFloatTracks;
		/* 0x020 */ hkaAnimatedReferenceFrame* m_extractedMotion;
		/* 0x028 */ hkArray<hkaAnnotationTrack> m_annotationTracks;

	private:
		// Wrapper that calls samplePartialTracks with the animation's own track counts
		/* 04 */ virtual void sampleTracks(float Time, void* TransformTracksOut, float* FloatTracksOut, void* ChunkCache) const;
		/* 05 */ virtual void samplePartialTracks(float Time, std::uint32_t NumTransformTracks, void* TransformTracksOut, std::uint32_t NumFloatTracks, float* FloatTracksOut, void* ChunkCache) const;
		/* 06 */ virtual void sampleIndividualTransformTracks(); // arguments not recovered
		/* 07 */ virtual void sampleIndividualFloatTracks(); // arguments not recovered
		/* 08 */ virtual void sub_hkaAnimation_08();
		[[nodiscard]] virtual std::int32_t getNumOriginalFrames() const; /* 09 */
		[[nodiscard]] virtual std::int32_t getNumDataChunks(std::uint32_t Frame, float Delta) const; /* 0A */
		virtual void getDataChunk(std::uint32_t Frame, float Delta, std::int32_t ChunkIndex, void* ChunkOut) const; /* 0B - fills the data pointer and its size */
		[[nodiscard]] virtual std::int32_t getMaxSizeOfCombinedDataChunks() const; /* 0C */
		/* 0D */ virtual void sub_hkaAnimation_0D();
		/* 0E */ virtual void sub_hkaAnimation_0E();
		/* 0F */ virtual void sub_hkaAnimation_0F();
		/* 10 */ virtual void sub_hkaAnimation_10();
		/* 11 */ virtual void sub_hkaAnimation_11();
		/* 12 */ virtual void sub_hkaAnimation_12();
		/* 13 */ virtual void sub_hkaAnimation_13();
		/* 14 */ virtual void sub_hkaAnimation_14();
	};
	static_assert(sizeof(hkaAnimation) == 0x38);
	static_assert(offsetof(hkaAnimation, m_type) == 0x10);
	static_assert(offsetof(hkaAnimation, m_duration) == 0x14);
	static_assert(offsetof(hkaAnimation, m_numberOfTransformTracks) == 0x18);
	static_assert(offsetof(hkaAnimation, m_numberOfFloatTracks) == 0x1C);
	static_assert(offsetof(hkaAnimation, m_extractedMotion) == 0x20);
	static_assert(offsetof(hkaAnimation, m_annotationTracks) == 0x28);

	class hkaAnimationBinding : public hkReferencedObject
	{
	public:
		enum class BlendHint : std::int32_t
		{
			NORMAL = 0,
			ADDITIVE_DEPRECATED = 1,
			ADDITIVE = 2,
		};

		/* 0x010 */ hkStringPtr m_originalSkeletonName;
		/* 0x018 */ hkaAnimation* m_animation;
		/* 0x020 */ hkArray<std::int16_t> m_transformTrackToBoneIndices;
		/* 0x030 */ hkArray<std::int16_t> m_floatTrackToFloatSlotIndices;
		/* 0x040 */ hkArray<std::int16_t> m_partitionIndices;
		/* 0x050 */ std::int8_t m_blendHint;
		/* 0x051 */ std::uint8_t pad051[0x7];
	};
	static_assert(sizeof(hkaAnimationBinding) == 0x58);
	static_assert(offsetof(hkaAnimationBinding, m_originalSkeletonName) == 0x10);
	static_assert(offsetof(hkaAnimationBinding, m_animation) == 0x18);
	static_assert(offsetof(hkaAnimationBinding, m_transformTrackToBoneIndices) == 0x20);
	static_assert(offsetof(hkaAnimationBinding, m_floatTrackToFloatSlotIndices) == 0x30);
	static_assert(offsetof(hkaAnimationBinding, m_partitionIndices) == 0x40);
	static_assert(offsetof(hkaAnimationBinding, m_blendHint) == 0x50);

	class hkaAnimationContainer : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkaSkeleton*> m_skeletons;
		/* 0x020 */ hkArray<hkaAnimation*> m_animations;
		/* 0x030 */ hkArray<hkaAnimationBinding*> m_bindings;
		/* 0x040 */ hkArray<hkaBoneAttachment*> m_attachments;
		/* 0x050 */ hkArray<hkaMeshBinding*> m_skins;
	};
	static_assert(sizeof(hkaAnimationContainer) == 0x60);
	static_assert(offsetof(hkaAnimationContainer, m_skeletons) == 0x10);
	static_assert(offsetof(hkaAnimationContainer, m_animations) == 0x20);
	static_assert(offsetof(hkaAnimationContainer, m_bindings) == 0x30);
	static_assert(offsetof(hkaAnimationContainer, m_attachments) == 0x40);
	static_assert(offsetof(hkaAnimationContainer, m_skins) == 0x50);

	class hkaAnimationPreviewColorContainer : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<std::uint32_t> m_previewColor;
	};
	static_assert(sizeof(hkaAnimationPreviewColorContainer) == 0x20);
	static_assert(offsetof(hkaAnimationPreviewColorContainer, m_previewColor) == 0x10);

	class hkaAnnotationTrack
	{
	public:
		/* 0x000 */ hkStringPtr m_trackName;
		/* 0x008 */ hkArray<hkaAnnotationTrackAnnotation> m_annotations;
	};
	static_assert(sizeof(hkaAnnotationTrack) == 0x18);
	static_assert(offsetof(hkaAnnotationTrack, m_trackName) == 0x0);
	static_assert(offsetof(hkaAnnotationTrack, m_annotations) == 0x8);

	class hkaAnnotationTrackAnnotation
	{
	public:
		/* 0x000 */ float m_time;
		/* 0x004 */ std::uint8_t pad004[0x4];
		/* 0x008 */ hkStringPtr m_text;
	};
	static_assert(sizeof(hkaAnnotationTrackAnnotation) == 0x10);
	static_assert(offsetof(hkaAnnotationTrackAnnotation, m_time) == 0x0);
	static_assert(offsetof(hkaAnnotationTrackAnnotation, m_text) == 0x8);

	class hkaBone
	{
	public:
		/* 0x000 */ hkStringPtr m_name;
		/* 0x008 */ hkBool m_lockTranslation;
		/* 0x009 */ std::uint8_t pad009[0x7];
	};
	static_assert(sizeof(hkaBone) == 0x10);
	static_assert(offsetof(hkaBone, m_name) == 0x0);
	static_assert(offsetof(hkaBone, m_lockTranslation) == 0x8);

	class hkaBoneAttachment : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkStringPtr m_originalSkeletonName;
		/* 0x018 */ std::uint8_t pad018[0x8];
		/* 0x020 */ hkMatrix4f m_boneFromAttachment;
		/* 0x060 */ hkReferencedObject* m_attachment;
		/* 0x068 */ hkStringPtr m_name;
		/* 0x070 */ std::int16_t m_boneIndex;
		/* 0x072 */ std::uint8_t pad072[0xE];
	};
	static_assert(sizeof(hkaBoneAttachment) == 0x80);
	static_assert(offsetof(hkaBoneAttachment, m_originalSkeletonName) == 0x10);
	static_assert(offsetof(hkaBoneAttachment, m_boneFromAttachment) == 0x20);
	static_assert(offsetof(hkaBoneAttachment, m_attachment) == 0x60);
	static_assert(offsetof(hkaBoneAttachment, m_name) == 0x68);
	static_assert(offsetof(hkaBoneAttachment, m_boneIndex) == 0x70);

	class hkaDirectionalReferenceFrame : public hkaParameterizedReferenceFrame
	{
	public:
		/* 0x060 */ hkVector4f m_movementDir;
	};
	static_assert(sizeof(hkaDirectionalReferenceFrame) == 0x70);
	static_assert(offsetof(hkaDirectionalReferenceFrame, m_movementDir) == 0x60);

	class hkaFootstepAnalysisInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<char> m_name;
		/* 0x020 */ hkArray<char> m_nameStrike;
		/* 0x030 */ hkArray<char> m_nameLift;
		/* 0x040 */ hkArray<char> m_nameLock;
		/* 0x050 */ hkArray<char> m_nameUnlock;
		/* 0x060 */ hkArray<float> m_minPos;
		/* 0x070 */ hkArray<float> m_maxPos;
		/* 0x080 */ hkArray<float> m_minVel;
		/* 0x090 */ hkArray<float> m_maxVel;
		/* 0x0A0 */ hkArray<float> m_allBonesDown;
		/* 0x0B0 */ hkArray<float> m_anyBonesDown;
		/* 0x0C0 */ float m_posTol;
		/* 0x0C4 */ float m_velTol;
		/* 0x0C8 */ float m_duration;
		/* 0x0CC */ std::uint8_t pad0CC[0x4];
	};
	static_assert(sizeof(hkaFootstepAnalysisInfo) == 0xD0);
	static_assert(offsetof(hkaFootstepAnalysisInfo, m_name) == 0x10);
	static_assert(offsetof(hkaFootstepAnalysisInfo, m_nameStrike) == 0x20);
	static_assert(offsetof(hkaFootstepAnalysisInfo, m_nameLift) == 0x30);
	static_assert(offsetof(hkaFootstepAnalysisInfo, m_nameLock) == 0x40);
	static_assert(offsetof(hkaFootstepAnalysisInfo, m_nameUnlock) == 0x50);
	static_assert(offsetof(hkaFootstepAnalysisInfo, m_minPos) == 0x60);
	static_assert(offsetof(hkaFootstepAnalysisInfo, m_maxPos) == 0x70);
	static_assert(offsetof(hkaFootstepAnalysisInfo, m_minVel) == 0x80);
	static_assert(offsetof(hkaFootstepAnalysisInfo, m_maxVel) == 0x90);
	static_assert(offsetof(hkaFootstepAnalysisInfo, m_allBonesDown) == 0xA0);
	static_assert(offsetof(hkaFootstepAnalysisInfo, m_anyBonesDown) == 0xB0);
	static_assert(offsetof(hkaFootstepAnalysisInfo, m_posTol) == 0xC0);
	static_assert(offsetof(hkaFootstepAnalysisInfo, m_velTol) == 0xC4);
	static_assert(offsetof(hkaFootstepAnalysisInfo, m_duration) == 0xC8);

	class hkaFootstepAnalysisInfoContainer : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkaFootstepAnalysisInfo*> m_previewInfo;
	};
	static_assert(sizeof(hkaFootstepAnalysisInfoContainer) == 0x20);
	static_assert(offsetof(hkaFootstepAnalysisInfoContainer, m_previewInfo) == 0x10);

	class hkaInterleavedUncompressedAnimation : public hkaAnimation
	{
	public:
		/* 0x038 */ hkArray<hkQsTransformf> m_transforms;
		/* 0x048 */ hkArray<float> m_floats;
	};
	static_assert(sizeof(hkaInterleavedUncompressedAnimation) == 0x58);
	static_assert(offsetof(hkaInterleavedUncompressedAnimation, m_transforms) == 0x38);
	static_assert(offsetof(hkaInterleavedUncompressedAnimation, m_floats) == 0x48);

	class hkaLosslessCompressedAnimation : public hkaAnimation
	{
	public:
		enum class LosslessTrackType : std::int32_t
		{
			TRACK_TYPE_CLEAR = 0,
			TRACK_TYPE_STATIC = 1,
			TRACK_TYPE_DYNAMIC = 2,
		};

		/* 0x038 */ hkArray<float> m_dynamicTranslations;
		/* 0x048 */ hkArray<float> m_staticTranslations;
		/* 0x058 */ hkArray<std::uint64_t> m_translationTypeAndOffsets;
		/* 0x068 */ hkArray<hkQuaternionf> m_dynamicRotations;
		/* 0x078 */ hkArray<hkQuaternionf> m_staticRotations;
		/* 0x088 */ hkArray<std::uint16_t> m_rotationTypeAndOffsets;
		/* 0x098 */ hkArray<float> m_dynamicScales;
		/* 0x0A8 */ hkArray<float> m_staticScales;
		/* 0x0B8 */ hkArray<std::uint64_t> m_scaleTypeAndOffsets;
		/* 0x0C8 */ hkArray<float> m_floats;
		/* 0x0D8 */ std::int32_t m_numFrames;
		/* 0x0DC */ std::uint8_t pad0DC[0x4];
	};
	static_assert(sizeof(hkaLosslessCompressedAnimation) == 0xE0);
	static_assert(offsetof(hkaLosslessCompressedAnimation, m_dynamicTranslations) == 0x38);
	static_assert(offsetof(hkaLosslessCompressedAnimation, m_staticTranslations) == 0x48);
	static_assert(offsetof(hkaLosslessCompressedAnimation, m_translationTypeAndOffsets) == 0x58);
	static_assert(offsetof(hkaLosslessCompressedAnimation, m_dynamicRotations) == 0x68);
	static_assert(offsetof(hkaLosslessCompressedAnimation, m_staticRotations) == 0x78);
	static_assert(offsetof(hkaLosslessCompressedAnimation, m_rotationTypeAndOffsets) == 0x88);
	static_assert(offsetof(hkaLosslessCompressedAnimation, m_dynamicScales) == 0x98);
	static_assert(offsetof(hkaLosslessCompressedAnimation, m_staticScales) == 0xA8);
	static_assert(offsetof(hkaLosslessCompressedAnimation, m_scaleTypeAndOffsets) == 0xB8);
	static_assert(offsetof(hkaLosslessCompressedAnimation, m_floats) == 0xC8);
	static_assert(offsetof(hkaLosslessCompressedAnimation, m_numFrames) == 0xD8);

	class hkaMeshBinding : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkxMesh* m_mesh;
		/* 0x018 */ hkStringPtr m_originalSkeletonName;
		/* 0x020 */ hkStringPtr m_name;
		/* 0x028 */ hkaSkeleton* m_skeleton;
		/* 0x030 */ hkArray<hkaMeshBindingMapping> m_mappings;
		/* 0x040 */ hkArray<hkTransformf> m_boneFromSkinMeshTransforms;
	};
	static_assert(sizeof(hkaMeshBinding) == 0x50);
	static_assert(offsetof(hkaMeshBinding, m_mesh) == 0x10);
	static_assert(offsetof(hkaMeshBinding, m_originalSkeletonName) == 0x18);
	static_assert(offsetof(hkaMeshBinding, m_name) == 0x20);
	static_assert(offsetof(hkaMeshBinding, m_skeleton) == 0x28);
	static_assert(offsetof(hkaMeshBinding, m_mappings) == 0x30);
	static_assert(offsetof(hkaMeshBinding, m_boneFromSkinMeshTransforms) == 0x40);

	class hkaMeshBindingMapping
	{
	public:
		/* 0x000 */ hkArray<std::int16_t> m_mapping;
	};
	static_assert(sizeof(hkaMeshBindingMapping) == 0x10);
	static_assert(offsetof(hkaMeshBindingMapping, m_mapping) == 0x0);

	class hkaParameterizedAnimationReferenceFrame : public hkaDefaultAnimatedReferenceFrame
	{
	public:
		enum class ParameterType : std::int32_t
		{
			UNKNOWN = 0,
			LINEAR_SPEED = 1,
			LINEAR_DIRECTION = 2,
			TURN_SPEED = 3,
		};

		/* 0x060 */ hkArray<float> m_parameterValues;
		/* 0x070 */ hkArray<std::int32_t> m_parameterTypes;
	};
	static_assert(sizeof(hkaParameterizedAnimationReferenceFrame) == 0x80);
	static_assert(offsetof(hkaParameterizedAnimationReferenceFrame, m_parameterValues) == 0x60);
	static_assert(offsetof(hkaParameterizedAnimationReferenceFrame, m_parameterTypes) == 0x70);

	class hkaPredictiveCompressedAnimation : public hkaAnimation
	{
	public:
		enum class StorageClass : std::int32_t
		{
			STORAGE_STATIC = 0,
			STORAGE_REFERENCE = 1,
			STORAGE_DYNAMIC_RANGE = 2,
			STORAGE_DYNAMIC_FIXED = 3,
		};

		enum class IntArrayID : std::int32_t
		{
			BLOCK_OFFSETS = 0,
			FIRST_FLOAT_BLOCK_OFFSETS = 1,
			IS_ANIMATED_BITMAP = 2,
			IS_FIXED_RANGE_BITMAP = 3,
			DYNAMIC_BONE_TRACK_INDEX = 4,
			DYNAMIC_FLOAT_TRACK_INDEX = 5,
			STATIC_BONE_TRACK_INDEX = 6,
			STATIC_FLOAT_TRACK_INDEX = 7,
			RENORM_QUATERNION_INDEX = 8,
			NUM_INT_ARRAYS = 9,
		};

		enum class FloatArrayID : std::int32_t
		{
			STATIC_VALUES = 0,
			DYNAMIC_SCALES = 1,
			DYNAMIC_OFFSETS = 2,
			NUM_FLOAT_ARRAYS = 3,
		};

		/* 0x038 */ hkArray<std::uint8_t> m_compressedData;
		/* 0x048 */ hkArray<std::uint16_t> m_intData;
		/* 0x058 */ std::int32_t m_intArrayOffsets[9];
		/* 0x07C */ std::uint8_t pad07C[0x4];
		/* 0x080 */ hkArray<float> m_floatData;
		/* 0x090 */ std::int32_t m_floatArrayOffsets[3];
		/* 0x09C */ std::int32_t m_numBones;
		/* 0x0A0 */ std::int32_t m_numFloatSlots;
		/* 0x0A4 */ std::int32_t m_numFrames;
		/* 0x0A8 */ std::int32_t m_firstFloatBlockScaleAndOffsetIndex;
		/* 0x0AC */ std::uint8_t pad0AC[0x4];
		/* 0x0B0 */ void* m_skeleton;
		/* 0x0B8 */ std::int32_t m_maxCompressedBytesPerFrame;
		/* 0x0BC */ std::uint8_t pad0BC[0x4];
	};
	static_assert(sizeof(hkaPredictiveCompressedAnimation) == 0xC0);
	static_assert(offsetof(hkaPredictiveCompressedAnimation, m_compressedData) == 0x38);
	static_assert(offsetof(hkaPredictiveCompressedAnimation, m_intData) == 0x48);
	static_assert(offsetof(hkaPredictiveCompressedAnimation, m_intArrayOffsets) == 0x58);
	static_assert(offsetof(hkaPredictiveCompressedAnimation, m_floatData) == 0x80);
	static_assert(offsetof(hkaPredictiveCompressedAnimation, m_floatArrayOffsets) == 0x90);
	static_assert(offsetof(hkaPredictiveCompressedAnimation, m_numBones) == 0x9C);
	static_assert(offsetof(hkaPredictiveCompressedAnimation, m_numFloatSlots) == 0xA0);
	static_assert(offsetof(hkaPredictiveCompressedAnimation, m_numFrames) == 0xA4);
	static_assert(offsetof(hkaPredictiveCompressedAnimation, m_firstFloatBlockScaleAndOffsetIndex) == 0xA8);
	static_assert(offsetof(hkaPredictiveCompressedAnimation, m_skeleton) == 0xB0);
	static_assert(offsetof(hkaPredictiveCompressedAnimation, m_maxCompressedBytesPerFrame) == 0xB8);

	class hkaPredictiveCompressedAnimationTrackCompressionParams
	{
	public:
		/* 0x000 */ float m_staticTranslationTolerance;
		/* 0x004 */ float m_staticRotationTolerance;
		/* 0x008 */ float m_staticScaleTolerance;
		/* 0x00C */ float m_staticFloatTolerance;
		/* 0x010 */ float m_dynamicTranslationTolerance;
		/* 0x014 */ float m_dynamicRotationTolerance;
		/* 0x018 */ float m_dynamicScaleTolerance;
		/* 0x01C */ float m_dynamicFloatTolerance;
	};
	static_assert(sizeof(hkaPredictiveCompressedAnimationTrackCompressionParams) == 0x20);
	static_assert(offsetof(hkaPredictiveCompressedAnimationTrackCompressionParams, m_staticTranslationTolerance) == 0x0);
	static_assert(offsetof(hkaPredictiveCompressedAnimationTrackCompressionParams, m_staticRotationTolerance) == 0x4);
	static_assert(offsetof(hkaPredictiveCompressedAnimationTrackCompressionParams, m_staticScaleTolerance) == 0x8);
	static_assert(offsetof(hkaPredictiveCompressedAnimationTrackCompressionParams, m_staticFloatTolerance) == 0xC);
	static_assert(offsetof(hkaPredictiveCompressedAnimationTrackCompressionParams, m_dynamicTranslationTolerance) == 0x10);
	static_assert(offsetof(hkaPredictiveCompressedAnimationTrackCompressionParams, m_dynamicRotationTolerance) == 0x14);
	static_assert(offsetof(hkaPredictiveCompressedAnimationTrackCompressionParams, m_dynamicScaleTolerance) == 0x18);
	static_assert(offsetof(hkaPredictiveCompressedAnimationTrackCompressionParams, m_dynamicFloatTolerance) == 0x1C);

	class hkaQuantizedAnimation : public hkaAnimation
	{
	public:
		/* 0x038 */ hkArray<std::uint8_t> m_data;
		/* 0x048 */ std::uint32_t m_endian;
		/* 0x04C */ std::uint8_t pad04C[0x4];
		/* 0x050 */ void* m_skeleton;
	};
	static_assert(sizeof(hkaQuantizedAnimation) == 0x58);
	static_assert(offsetof(hkaQuantizedAnimation, m_data) == 0x38);
	static_assert(offsetof(hkaQuantizedAnimation, m_endian) == 0x48);
	static_assert(offsetof(hkaQuantizedAnimation, m_skeleton) == 0x50);

	class hkaQuantizedAnimationTrackCompressionParams
	{
	public:
		/* 0x000 */ float m_rotationTolerance;
		/* 0x004 */ float m_translationTolerance;
		/* 0x008 */ float m_scaleTolerance;
		/* 0x00C */ float m_floatingTolerance;
	};
	static_assert(sizeof(hkaQuantizedAnimationTrackCompressionParams) == 0x10);
	static_assert(offsetof(hkaQuantizedAnimationTrackCompressionParams, m_rotationTolerance) == 0x0);
	static_assert(offsetof(hkaQuantizedAnimationTrackCompressionParams, m_translationTolerance) == 0x4);
	static_assert(offsetof(hkaQuantizedAnimationTrackCompressionParams, m_scaleTolerance) == 0x8);
	static_assert(offsetof(hkaQuantizedAnimationTrackCompressionParams, m_floatingTolerance) == 0xC);

	class hkaReferencePoseAnimation : public hkaAnimation
	{
	public:
		/* 0x038 */ hkaSkeleton* m_skeleton;
	};
	static_assert(sizeof(hkaReferencePoseAnimation) == 0x40);
	static_assert(offsetof(hkaReferencePoseAnimation, m_skeleton) == 0x38);

	class hkaSkeleton : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkStringPtr m_name;
		/* 0x018 */ hkArray<std::int16_t> m_parentIndices;
		/* 0x028 */ hkArray<hkaBone> m_bones;
		/* 0x038 */ hkArray<hkQsTransformf> m_referencePose;
		/* 0x048 */ hkArray<float> m_referenceFloats;
		/* 0x058 */ hkArray<hkStringPtr> m_floatSlots;
		/* 0x068 */ hkArray<hkaSkeletonLocalFrameOnBone> m_localFrames;
		/* 0x078 */ hkArray<hkaSkeletonPartition> m_partitions;
	};
	static_assert(sizeof(hkaSkeleton) == 0x88);
	static_assert(offsetof(hkaSkeleton, m_name) == 0x10);
	static_assert(offsetof(hkaSkeleton, m_parentIndices) == 0x18);
	static_assert(offsetof(hkaSkeleton, m_bones) == 0x28);
	static_assert(offsetof(hkaSkeleton, m_referencePose) == 0x38);
	static_assert(offsetof(hkaSkeleton, m_referenceFloats) == 0x48);
	static_assert(offsetof(hkaSkeleton, m_floatSlots) == 0x58);
	static_assert(offsetof(hkaSkeleton, m_localFrames) == 0x68);
	static_assert(offsetof(hkaSkeleton, m_partitions) == 0x78);

	class hkaSkeletonLocalFrameOnBone
	{
	public:
		/* 0x000 */ hkLocalFrame* m_localFrame;
		/* 0x008 */ std::int16_t m_boneIndex;
		/* 0x00A */ std::uint8_t pad00A[0x6];
	};
	static_assert(sizeof(hkaSkeletonLocalFrameOnBone) == 0x10);
	static_assert(offsetof(hkaSkeletonLocalFrameOnBone, m_localFrame) == 0x0);
	static_assert(offsetof(hkaSkeletonLocalFrameOnBone, m_boneIndex) == 0x8);

	class hkaSkeletonMapperData
	{
	public:
		enum class MappingType : std::int32_t
		{
			HK_RAGDOLL_MAPPING = 0,
			HK_RETARGETING_MAPPING = 1,
		};

		/* 0x000 */ hkaSkeleton* m_skeletonA;
		/* 0x008 */ hkaSkeleton* m_skeletonB;
		/* 0x010 */ hkArray<std::int16_t> m_partitionMap;
		/* 0x020 */ hkArray<hkaSkeletonMapperDataPartitionMappingRange> m_simpleMappingPartitionRanges;
		/* 0x030 */ hkArray<hkaSkeletonMapperDataPartitionMappingRange> m_chainMappingPartitionRanges;
		/* 0x040 */ hkArray<hkaSkeletonMapperDataSimpleMapping> m_simpleMappings;
		/* 0x050 */ hkArray<hkaSkeletonMapperDataChainMapping> m_chainMappings;
		/* 0x060 */ hkArray<std::int16_t> m_unmappedBones;
		/* 0x070 */ hkQsTransformf m_extractedMotionMapping;
		/* 0x0A0 */ hkBool m_keepUnmappedLocal;
		/* 0x0A1 */ std::uint8_t pad0A1[0x3];
		/* 0x0A4 */ std::int32_t m_mappingType;
		/* 0x0A8 */ std::uint8_t pad0A8[0x8];
	};
	static_assert(sizeof(hkaSkeletonMapperData) == 0xB0);
	static_assert(offsetof(hkaSkeletonMapperData, m_skeletonA) == 0x0);
	static_assert(offsetof(hkaSkeletonMapperData, m_skeletonB) == 0x8);
	static_assert(offsetof(hkaSkeletonMapperData, m_partitionMap) == 0x10);
	static_assert(offsetof(hkaSkeletonMapperData, m_simpleMappingPartitionRanges) == 0x20);
	static_assert(offsetof(hkaSkeletonMapperData, m_chainMappingPartitionRanges) == 0x30);
	static_assert(offsetof(hkaSkeletonMapperData, m_simpleMappings) == 0x40);
	static_assert(offsetof(hkaSkeletonMapperData, m_chainMappings) == 0x50);
	static_assert(offsetof(hkaSkeletonMapperData, m_unmappedBones) == 0x60);
	static_assert(offsetof(hkaSkeletonMapperData, m_extractedMotionMapping) == 0x70);
	static_assert(offsetof(hkaSkeletonMapperData, m_keepUnmappedLocal) == 0xA0);
	static_assert(offsetof(hkaSkeletonMapperData, m_mappingType) == 0xA4);

	class hkaSkeletonMapper : public hkReferencedObject
	{
	public:
		enum class ConstraintSource : std::int32_t
		{
			NO_CONSTRAINTS = 0,
			REFERENCE_POSE = 1,
			CURRENT_POSE = 2,
		};

		/* 0x010 */ hkaSkeletonMapperData m_mapping;
	};
	static_assert(sizeof(hkaSkeletonMapper) == 0xC0);
	static_assert(offsetof(hkaSkeletonMapper, m_mapping) == 0x10);

	class hkaSkeletonMapperDataChainMapping
	{
	public:
		/* 0x000 */ std::int16_t m_startBoneA;
		/* 0x002 */ std::int16_t m_endBoneA;
		/* 0x004 */ std::int16_t m_startBoneB;
		/* 0x006 */ std::int16_t m_endBoneB;
		/* 0x008 */ std::uint8_t pad008[0x8];
		/* 0x010 */ hkQsTransformf m_startAFromBTransform;
		/* 0x040 */ hkQsTransformf m_endAFromBTransform;
	};
	static_assert(sizeof(hkaSkeletonMapperDataChainMapping) == 0x70);
	static_assert(offsetof(hkaSkeletonMapperDataChainMapping, m_startBoneA) == 0x0);
	static_assert(offsetof(hkaSkeletonMapperDataChainMapping, m_endBoneA) == 0x2);
	static_assert(offsetof(hkaSkeletonMapperDataChainMapping, m_startBoneB) == 0x4);
	static_assert(offsetof(hkaSkeletonMapperDataChainMapping, m_endBoneB) == 0x6);
	static_assert(offsetof(hkaSkeletonMapperDataChainMapping, m_startAFromBTransform) == 0x10);
	static_assert(offsetof(hkaSkeletonMapperDataChainMapping, m_endAFromBTransform) == 0x40);

	class hkaSkeletonMapperDataPartitionMappingRange
	{
	public:
		/* 0x000 */ std::int32_t m_startMappingIndex;
		/* 0x004 */ std::int32_t m_numMappings;
	};
	static_assert(sizeof(hkaSkeletonMapperDataPartitionMappingRange) == 0x8);
	static_assert(offsetof(hkaSkeletonMapperDataPartitionMappingRange, m_startMappingIndex) == 0x0);
	static_assert(offsetof(hkaSkeletonMapperDataPartitionMappingRange, m_numMappings) == 0x4);

	class hkaSkeletonMapperDataSimpleMapping
	{
	public:
		/* 0x000 */ std::int16_t m_boneA;
		/* 0x002 */ std::int16_t m_boneB;
		/* 0x004 */ std::uint8_t pad004[0xC];
		/* 0x010 */ hkQsTransformf m_aFromBTransform;
	};
	static_assert(sizeof(hkaSkeletonMapperDataSimpleMapping) == 0x40);
	static_assert(offsetof(hkaSkeletonMapperDataSimpleMapping, m_boneA) == 0x0);
	static_assert(offsetof(hkaSkeletonMapperDataSimpleMapping, m_boneB) == 0x2);
	static_assert(offsetof(hkaSkeletonMapperDataSimpleMapping, m_aFromBTransform) == 0x10);

	class hkaSkeletonPartition
	{
	public:
		/* 0x000 */ hkStringPtr m_name;
		/* 0x008 */ std::int16_t m_startBoneIndex;
		/* 0x00A */ std::int16_t m_numBones;
		/* 0x00C */ std::uint8_t pad00C[0x4];
	};
	static_assert(sizeof(hkaSkeletonPartition) == 0x10);
	static_assert(offsetof(hkaSkeletonPartition, m_name) == 0x0);
	static_assert(offsetof(hkaSkeletonPartition, m_startBoneIndex) == 0x8);
	static_assert(offsetof(hkaSkeletonPartition, m_numBones) == 0xA);

	class hkaSplineCompressedAnimation : public hkaAnimation
	{
	public:
		/* 0x038 */ std::int32_t m_numFrames;
		/* 0x03C */ std::int32_t m_numBlocks;
		/* 0x040 */ std::int32_t m_maxFramesPerBlock;
		/* 0x044 */ std::int32_t m_maskAndQuantizationSize;
		/* 0x048 */ float m_blockDuration;
		/* 0x04C */ float m_blockInverseDuration;
		/* 0x050 */ float m_frameDuration;
		/* 0x054 */ std::uint8_t pad054[0x4];
		/* 0x058 */ hkArray<std::uint32_t> m_blockOffsets;
		/* 0x068 */ hkArray<std::uint32_t> m_floatBlockOffsets;
		/* 0x078 */ hkArray<std::uint32_t> m_transformOffsets;
		/* 0x088 */ hkArray<std::uint32_t> m_floatOffsets;
		/* 0x098 */ hkArray<std::uint8_t> m_data;
		/* 0x0A8 */ std::int32_t m_endian;
		/* 0x0AC */ std::uint8_t pad0AC[0x4];
	};
	static_assert(sizeof(hkaSplineCompressedAnimation) == 0xB0);
	static_assert(offsetof(hkaSplineCompressedAnimation, m_numFrames) == 0x38);
	static_assert(offsetof(hkaSplineCompressedAnimation, m_numBlocks) == 0x3C);
	static_assert(offsetof(hkaSplineCompressedAnimation, m_maxFramesPerBlock) == 0x40);
	static_assert(offsetof(hkaSplineCompressedAnimation, m_maskAndQuantizationSize) == 0x44);
	static_assert(offsetof(hkaSplineCompressedAnimation, m_blockDuration) == 0x48);
	static_assert(offsetof(hkaSplineCompressedAnimation, m_blockInverseDuration) == 0x4C);
	static_assert(offsetof(hkaSplineCompressedAnimation, m_frameDuration) == 0x50);
	static_assert(offsetof(hkaSplineCompressedAnimation, m_blockOffsets) == 0x58);
	static_assert(offsetof(hkaSplineCompressedAnimation, m_floatBlockOffsets) == 0x68);
	static_assert(offsetof(hkaSplineCompressedAnimation, m_transformOffsets) == 0x78);
	static_assert(offsetof(hkaSplineCompressedAnimation, m_floatOffsets) == 0x88);
	static_assert(offsetof(hkaSplineCompressedAnimation, m_data) == 0x98);
	static_assert(offsetof(hkaSplineCompressedAnimation, m_endian) == 0xA8);

	class hkaSplineCompressedAnimationAnimationCompressionParams
	{
	public:
		/* 0x000 */ std::uint16_t m_maxFramesPerBlock;
		/* 0x002 */ hkBool m_enableSampleSingleTracks;
		/* 0x003 */ std::uint8_t pad003[0x1];
	};
	static_assert(sizeof(hkaSplineCompressedAnimationAnimationCompressionParams) == 0x4);
	static_assert(offsetof(hkaSplineCompressedAnimationAnimationCompressionParams, m_maxFramesPerBlock) == 0x0);
	static_assert(offsetof(hkaSplineCompressedAnimationAnimationCompressionParams, m_enableSampleSingleTracks) == 0x2);

	class hkaSplineCompressedAnimationTrackCompressionParams
	{
	public:
		enum class RotationQuantization : std::int32_t
		{
			POLAR32 = 0,
			THREECOMP40 = 1,
			THREECOMP48 = 2,
			THREECOMP24 = 3,
			STRAIGHT16 = 4,
			UNCOMPRESSED = 5,
		};

		enum class ScalarQuantization : std::int32_t
		{
			BITS8 = 0,
			BITS16 = 1,
		};

		/* 0x000 */ float m_rotationTolerance;
		/* 0x004 */ float m_translationTolerance;
		/* 0x008 */ float m_scaleTolerance;
		/* 0x00C */ float m_floatingTolerance;
		/* 0x010 */ std::uint16_t m_rotationDegree;
		/* 0x012 */ std::uint16_t m_translationDegree;
		/* 0x014 */ std::uint16_t m_scaleDegree;
		/* 0x016 */ std::uint16_t m_floatingDegree;
		/* 0x018 */ std::uint8_t m_rotationQuantizationType;
		/* 0x019 */ std::uint8_t m_translationQuantizationType;
		/* 0x01A */ std::uint8_t m_scaleQuantizationType;
		/* 0x01B */ std::uint8_t m_floatQuantizationType;
	};
	static_assert(sizeof(hkaSplineCompressedAnimationTrackCompressionParams) == 0x1C);
	static_assert(offsetof(hkaSplineCompressedAnimationTrackCompressionParams, m_rotationTolerance) == 0x0);
	static_assert(offsetof(hkaSplineCompressedAnimationTrackCompressionParams, m_translationTolerance) == 0x4);
	static_assert(offsetof(hkaSplineCompressedAnimationTrackCompressionParams, m_scaleTolerance) == 0x8);
	static_assert(offsetof(hkaSplineCompressedAnimationTrackCompressionParams, m_floatingTolerance) == 0xC);
	static_assert(offsetof(hkaSplineCompressedAnimationTrackCompressionParams, m_rotationDegree) == 0x10);
	static_assert(offsetof(hkaSplineCompressedAnimationTrackCompressionParams, m_translationDegree) == 0x12);
	static_assert(offsetof(hkaSplineCompressedAnimationTrackCompressionParams, m_scaleDegree) == 0x14);
	static_assert(offsetof(hkaSplineCompressedAnimationTrackCompressionParams, m_floatingDegree) == 0x16);
	static_assert(offsetof(hkaSplineCompressedAnimationTrackCompressionParams, m_rotationQuantizationType) == 0x18);
	static_assert(offsetof(hkaSplineCompressedAnimationTrackCompressionParams, m_translationQuantizationType) == 0x19);
	static_assert(offsetof(hkaSplineCompressedAnimationTrackCompressionParams, m_scaleQuantizationType) == 0x1A);
	static_assert(offsetof(hkaSplineCompressedAnimationTrackCompressionParams, m_floatQuantizationType) == 0x1B);

}
