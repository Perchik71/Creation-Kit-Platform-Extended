// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseObject.h>
#include <EditorAPI/H/hkBaseTypes.h>
#include <EditorAPI/H/hkbBehaviorGraph.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkbAlignBoneModifier;
	class hkbAttachmentModifier;
	class hkbAttachmentSetup;
	class hkbAttributeModifier;
	class hkbAttributeModifierAssignment;
	class hkbBoneIndexArray;
	class hkbBoneWeightArray;
	class hkbBoolVariableSequencedData;
	class hkbCharacterControllerModifier;
	class hkbCharacterControllerModifierControlData;
	class hkbCombineTransformsModifier;
	class hkbComputeDirectionModifier;
	class hkbComputeRotationFromAxisAngleModifier;
	class hkbComputeRotationToTargetModifier;
	class hkbComputeWorldFromModelModifier;
	class hkbDampingModifier;
	class hkbDelayedModifier;
	class hkbDetectCloseToGroundModifier;
	class hkbEvaluateExpressionModifier;
	class hkbEvaluateHandleModifier;
	class hkbEventDrivenModifier;
	class hkbEventProperty;
	class hkbEventRangeDataArray;
	class hkbEventSequencedData;
	class hkbEventsFromRangeModifier;
	class hkbExpressionDataArray;
	class hkbExtractRagdollPoseModifier;
	class hkbFootIkControlData;
	class hkbFootIkControlsModifier;
	class hkbFootIkControlsModifierLeg;
	class hkbFootIkGains;
	class hkbFootIkModifier;
	class hkbFootIkModifierInternalLegData;
	class hkbFootIkModifierLeg;
	class hkbGetHandleOnBoneModifier;
	class hkbGetUpModifier;
	class hkbGetWorldFromModelModifier;
	class hkbGravityModifier;
	class hkbHandIkControlsModifier;
	class hkbHandIkControlsModifierHand;
	class hkbHandIkModifier;
	class hkbHandIkModifierHand;
	class hkbHandle;
	class hkbIntVariableSequencedData;
	class hkbJigglerGroup;
	class hkbJigglerModifier;
	class hkbKeyframeBonesModifier;
	class hkbKeyframeBonesModifierKeyframeInfo;
	class hkbLookAtModifier;
	class hkbMirrorModifier;
	class hkbModifier;
	class hkbModifierList;
	class hkbModifierWrapper;
	class hkbMoveBoneAttachmentModifier;
	class hkbMoveCharacterModifier;
	class hkbPoweredRagdollControlData;
	class hkbPoweredRagdollControlsModifier;
	class hkbRealVariableSequencedData;
	class hkbRigidBodyRagdollControlData;
	class hkbRigidBodyRagdollControlsModifier;
	class hkbRotateCharacterModifier;
	class hkbSenseHandleModifier;
	class hkbSenseHandleModifierRange;
	class hkbSequence;
	class hkbSequenceStringData;
	class hkbSetWorldFromModelModifier;
	class hkbStateDependentModifier;
	class hkbStateMachine;
	class hkbTimerModifier;
	class hkbTransformVectorModifier;
	class hkbTwistModifier;
	class hkbWorldFromModelModeData;

	class hkbAlignBoneModifier : public hkbModifier
	{
	public:
		enum class AlignModeABAM : std::int32_t
		{
			ALIGN_MODE_CHARACTER_WORLD_FROM_MODEL = 0,
			ALIGN_MODE_ANIMATION_SKELETON_BONE = 1,
		};

		enum class AlignTargetMode : std::int32_t
		{
			ALIGN_TARGET_MODE_CHARACTER_WORLD_FROM_MODEL = 0,
			ALIGN_TARGET_MODE_RAGDOLL_SKELETON_BONE = 1,
			ALIGN_TARGET_MODE_ANIMATION_SKELETON_BONE = 2,
			ALIGN_TARGET_MODE_USER_SPECIFIED_FRAME_OF_REFERENCE = 3,
		};

		/* 0x058 */ std::int8_t m_alignMode;
		/* 0x059 */ std::int8_t m_alignTargetMode;
		/* 0x05A */ hkBool m_alignSingleAxis;
		/* 0x05B */ std::uint8_t pad05B[0x5];
		/* 0x060 */ hkVector4f m_alignAxis;
		/* 0x070 */ hkVector4f m_alignTargetAxis;
		/* 0x080 */ hkQuaternionf m_frameOfReference;
		/* 0x090 */ float m_duration;
		/* 0x094 */ std::int32_t m_alignModeIndex;
		/* 0x098 */ std::int32_t m_alignTargetModeIndex;
		/* 0x09C */ float m_time;
		/* 0x0A0 */ float m_timeSinceLastModify;
		/* 0x0A4 */ std::uint8_t pad0A4[0xC];
	};
	static_assert(sizeof(hkbAlignBoneModifier) == 0xB0);
	static_assert(offsetof(hkbAlignBoneModifier, m_alignMode) == 0x58);
	static_assert(offsetof(hkbAlignBoneModifier, m_alignTargetMode) == 0x59);
	static_assert(offsetof(hkbAlignBoneModifier, m_alignSingleAxis) == 0x5A);
	static_assert(offsetof(hkbAlignBoneModifier, m_alignAxis) == 0x60);
	static_assert(offsetof(hkbAlignBoneModifier, m_alignTargetAxis) == 0x70);
	static_assert(offsetof(hkbAlignBoneModifier, m_frameOfReference) == 0x80);
	static_assert(offsetof(hkbAlignBoneModifier, m_duration) == 0x90);
	static_assert(offsetof(hkbAlignBoneModifier, m_alignModeIndex) == 0x94);
	static_assert(offsetof(hkbAlignBoneModifier, m_alignTargetModeIndex) == 0x98);
	static_assert(offsetof(hkbAlignBoneModifier, m_time) == 0x9C);
	static_assert(offsetof(hkbAlignBoneModifier, m_timeSinceLastModify) == 0xA0);

	class hkbAttachmentModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkbEventProperty m_sendToAttacherOnAttach;
		/* 0x068 */ hkbEventProperty m_sendToAttacheeOnAttach;
		/* 0x078 */ hkbEventProperty m_sendToAttacherOnDetach;
		/* 0x088 */ hkbEventProperty m_sendToAttacheeOnDetach;
		/* 0x098 */ hkbAttachmentSetup* m_attachmentSetup;
		/* 0x0A0 */ hkbHandle* m_attacherHandle;
		/* 0x0A8 */ hkbHandle* m_attacheeHandle;
		/* 0x0B0 */ std::int32_t m_attacheeLayer;
		/* 0x0B4 */ std::uint8_t pad0B4[0x4];
		/* 0x0B8 */ void* m_attacheeRB;
		/* 0x0C0 */ std::uint8_t m_oldMotionType;
		/* 0x0C1 */ std::uint8_t pad0C1[0x3];
		/* 0x0C4 */ std::int32_t m_oldFilterInfo;
		/* 0x0C8 */ void* m_attachment;
	};
	static_assert(sizeof(hkbAttachmentModifier) == 0xD0);
	static_assert(offsetof(hkbAttachmentModifier, m_sendToAttacherOnAttach) == 0x58);
	static_assert(offsetof(hkbAttachmentModifier, m_sendToAttacheeOnAttach) == 0x68);
	static_assert(offsetof(hkbAttachmentModifier, m_sendToAttacherOnDetach) == 0x78);
	static_assert(offsetof(hkbAttachmentModifier, m_sendToAttacheeOnDetach) == 0x88);
	static_assert(offsetof(hkbAttachmentModifier, m_attachmentSetup) == 0x98);
	static_assert(offsetof(hkbAttachmentModifier, m_attacherHandle) == 0xA0);
	static_assert(offsetof(hkbAttachmentModifier, m_attacheeHandle) == 0xA8);
	static_assert(offsetof(hkbAttachmentModifier, m_attacheeLayer) == 0xB0);
	static_assert(offsetof(hkbAttachmentModifier, m_attacheeRB) == 0xB8);
	static_assert(offsetof(hkbAttachmentModifier, m_oldMotionType) == 0xC0);
	static_assert(offsetof(hkbAttachmentModifier, m_oldFilterInfo) == 0xC4);
	static_assert(offsetof(hkbAttachmentModifier, m_attachment) == 0xC8);

	class hkbAttributeModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkArray<hkbAttributeModifierAssignment> m_assignments;
	};
	static_assert(sizeof(hkbAttributeModifier) == 0x68);
	static_assert(offsetof(hkbAttributeModifier, m_assignments) == 0x58);

	class hkbCharacterControllerModifier : public hkbModifier
	{
	public:
		enum class InitialVelocityCoordinates : std::int32_t
		{
			INITIAL_VELOCITY_IN_WORLD_COORDINATES = 0,
			INITIAL_VELOCITY_IN_MODEL_COORDINATES = 1,
		};

		enum class MotionMode : std::int32_t
		{
			MOTION_MODE_FOLLOW_ANIMATION = 0,
			MOTION_MODE_DYNAMIC = 1,
		};

		/* 0x058 */ hkbCharacterControllerModifierControlData m_controlData;
		/* 0x068 */ std::uint8_t pad068[0x8];
		/* 0x070 */ hkVector4f m_initialVelocity;
		/* 0x080 */ std::int8_t m_initialVelocityCoordinates;
		/* 0x081 */ std::int8_t m_motionMode;
		/* 0x082 */ hkBool m_forceDownwardMomentum;
		/* 0x083 */ hkBool m_applyGravity;
		/* 0x084 */ hkBool m_setInitialVelocity;
		/* 0x085 */ hkBool m_isTouchingGround;
		/* 0x086 */ std::uint8_t pad086[0xA];
		/* 0x090 */ hkVector4f m_gravity;
		/* 0x0A0 */ hkBool m_isInitialVelocityAdded;
		/* 0x0A1 */ std::uint8_t pad0A1[0xF];
	};
	static_assert(sizeof(hkbCharacterControllerModifier) == 0xB0);
	static_assert(offsetof(hkbCharacterControllerModifier, m_controlData) == 0x58);
	static_assert(offsetof(hkbCharacterControllerModifier, m_initialVelocity) == 0x70);
	static_assert(offsetof(hkbCharacterControllerModifier, m_initialVelocityCoordinates) == 0x80);
	static_assert(offsetof(hkbCharacterControllerModifier, m_motionMode) == 0x81);
	static_assert(offsetof(hkbCharacterControllerModifier, m_forceDownwardMomentum) == 0x82);
	static_assert(offsetof(hkbCharacterControllerModifier, m_applyGravity) == 0x83);
	static_assert(offsetof(hkbCharacterControllerModifier, m_setInitialVelocity) == 0x84);
	static_assert(offsetof(hkbCharacterControllerModifier, m_isTouchingGround) == 0x85);
	static_assert(offsetof(hkbCharacterControllerModifier, m_gravity) == 0x90);
	static_assert(offsetof(hkbCharacterControllerModifier, m_isInitialVelocityAdded) == 0xA0);

	class hkbCombineTransformsModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkVector4f m_translationOut;
		/* 0x070 */ hkQuaternionf m_rotationOut;
		/* 0x080 */ hkVector4f m_leftTranslation;
		/* 0x090 */ hkQuaternionf m_leftRotation;
		/* 0x0A0 */ hkVector4f m_rightTranslation;
		/* 0x0B0 */ hkQuaternionf m_rightRotation;
		/* 0x0C0 */ hkBool m_invertLeftTransform;
		/* 0x0C1 */ hkBool m_invertRightTransform;
		/* 0x0C2 */ hkBool m_invertResult;
		/* 0x0C3 */ std::uint8_t pad0C3[0xD];
	};
	static_assert(sizeof(hkbCombineTransformsModifier) == 0xD0);
	static_assert(offsetof(hkbCombineTransformsModifier, m_translationOut) == 0x60);
	static_assert(offsetof(hkbCombineTransformsModifier, m_rotationOut) == 0x70);
	static_assert(offsetof(hkbCombineTransformsModifier, m_leftTranslation) == 0x80);
	static_assert(offsetof(hkbCombineTransformsModifier, m_leftRotation) == 0x90);
	static_assert(offsetof(hkbCombineTransformsModifier, m_rightTranslation) == 0xA0);
	static_assert(offsetof(hkbCombineTransformsModifier, m_rightRotation) == 0xB0);
	static_assert(offsetof(hkbCombineTransformsModifier, m_invertLeftTransform) == 0xC0);
	static_assert(offsetof(hkbCombineTransformsModifier, m_invertRightTransform) == 0xC1);
	static_assert(offsetof(hkbCombineTransformsModifier, m_invertResult) == 0xC2);

	class hkbComputeDirectionModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkVector4f m_pointIn;
		/* 0x070 */ hkVector4f m_pointOut;
		/* 0x080 */ float m_groundAngleOut;
		/* 0x084 */ float m_upAngleOut;
		/* 0x088 */ float m_verticalOffset;
		/* 0x08C */ hkBool m_reverseGroundAngle;
		/* 0x08D */ hkBool m_reverseUpAngle;
		/* 0x08E */ hkBool m_projectPoint;
		/* 0x08F */ hkBool m_normalizePoint;
		/* 0x090 */ hkBool m_computeOnlyOnce;
		/* 0x091 */ hkBool m_computedOutput;
		/* 0x092 */ std::uint8_t pad092[0xE];
	};
	static_assert(sizeof(hkbComputeDirectionModifier) == 0xA0);
	static_assert(offsetof(hkbComputeDirectionModifier, m_pointIn) == 0x60);
	static_assert(offsetof(hkbComputeDirectionModifier, m_pointOut) == 0x70);
	static_assert(offsetof(hkbComputeDirectionModifier, m_groundAngleOut) == 0x80);
	static_assert(offsetof(hkbComputeDirectionModifier, m_upAngleOut) == 0x84);
	static_assert(offsetof(hkbComputeDirectionModifier, m_verticalOffset) == 0x88);
	static_assert(offsetof(hkbComputeDirectionModifier, m_reverseGroundAngle) == 0x8C);
	static_assert(offsetof(hkbComputeDirectionModifier, m_reverseUpAngle) == 0x8D);
	static_assert(offsetof(hkbComputeDirectionModifier, m_projectPoint) == 0x8E);
	static_assert(offsetof(hkbComputeDirectionModifier, m_normalizePoint) == 0x8F);
	static_assert(offsetof(hkbComputeDirectionModifier, m_computeOnlyOnce) == 0x90);
	static_assert(offsetof(hkbComputeDirectionModifier, m_computedOutput) == 0x91);

	class hkbComputeRotationFromAxisAngleModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkQuaternionf m_rotationOut;
		/* 0x070 */ hkVector4f m_axis;
		/* 0x080 */ float m_angleDegrees;
		/* 0x084 */ std::uint8_t pad084[0xC];
	};
	static_assert(sizeof(hkbComputeRotationFromAxisAngleModifier) == 0x90);
	static_assert(offsetof(hkbComputeRotationFromAxisAngleModifier, m_rotationOut) == 0x60);
	static_assert(offsetof(hkbComputeRotationFromAxisAngleModifier, m_axis) == 0x70);
	static_assert(offsetof(hkbComputeRotationFromAxisAngleModifier, m_angleDegrees) == 0x80);

	class hkbComputeRotationToTargetModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkQuaternionf m_rotationOut;
		/* 0x070 */ hkVector4f m_targetPosition;
		/* 0x080 */ hkVector4f m_currentPosition;
		/* 0x090 */ hkQuaternionf m_currentRotation;
		/* 0x0A0 */ hkVector4f m_localAxisOfRotation;
		/* 0x0B0 */ hkVector4f m_localFacingDirection;
		/* 0x0C0 */ hkBool m_resultIsDelta;
		/* 0x0C1 */ std::uint8_t pad0C1[0xF];
	};
	static_assert(sizeof(hkbComputeRotationToTargetModifier) == 0xD0);
	static_assert(offsetof(hkbComputeRotationToTargetModifier, m_rotationOut) == 0x60);
	static_assert(offsetof(hkbComputeRotationToTargetModifier, m_targetPosition) == 0x70);
	static_assert(offsetof(hkbComputeRotationToTargetModifier, m_currentPosition) == 0x80);
	static_assert(offsetof(hkbComputeRotationToTargetModifier, m_currentRotation) == 0x90);
	static_assert(offsetof(hkbComputeRotationToTargetModifier, m_localAxisOfRotation) == 0xA0);
	static_assert(offsetof(hkbComputeRotationToTargetModifier, m_localFacingDirection) == 0xB0);
	static_assert(offsetof(hkbComputeRotationToTargetModifier, m_resultIsDelta) == 0xC0);

	class hkbComputeWorldFromModelModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::int16_t m_poseMatchingRootBoneIndex;
		/* 0x05A */ std::int16_t m_poseMatchingOtherBoneIndex;
		/* 0x05C */ std::int16_t m_poseMatchingAnotherBoneIndex;
		/* 0x05E */ std::uint8_t pad05E[0x2];
	};
	static_assert(sizeof(hkbComputeWorldFromModelModifier) == 0x60);
	static_assert(offsetof(hkbComputeWorldFromModelModifier, m_poseMatchingRootBoneIndex) == 0x58);
	static_assert(offsetof(hkbComputeWorldFromModelModifier, m_poseMatchingOtherBoneIndex) == 0x5A);
	static_assert(offsetof(hkbComputeWorldFromModelModifier, m_poseMatchingAnotherBoneIndex) == 0x5C);

	class hkbDampingModifier : public hkbModifier
	{
	public:
		/* 0x058 */ float m_kP;
		/* 0x05C */ float m_kI;
		/* 0x060 */ float m_kD;
		/* 0x064 */ hkBool m_enableScalarDamping;
		/* 0x065 */ hkBool m_enableVectorDamping;
		/* 0x066 */ std::uint8_t pad066[0x2];
		/* 0x068 */ float m_rawValue;
		/* 0x06C */ float m_dampedValue;
		/* 0x070 */ hkVector4f m_rawVector;
		/* 0x080 */ hkVector4f m_dampedVector;
		/* 0x090 */ hkVector4f m_vecErrorSum;
		/* 0x0A0 */ hkVector4f m_vecPreviousError;
		/* 0x0B0 */ float m_errorSum;
		/* 0x0B4 */ float m_previousError;
		/* 0x0B8 */ std::uint8_t pad0B8[0x8];
	};
	static_assert(sizeof(hkbDampingModifier) == 0xC0);
	static_assert(offsetof(hkbDampingModifier, m_kP) == 0x58);
	static_assert(offsetof(hkbDampingModifier, m_kI) == 0x5C);
	static_assert(offsetof(hkbDampingModifier, m_kD) == 0x60);
	static_assert(offsetof(hkbDampingModifier, m_enableScalarDamping) == 0x64);
	static_assert(offsetof(hkbDampingModifier, m_enableVectorDamping) == 0x65);
	static_assert(offsetof(hkbDampingModifier, m_rawValue) == 0x68);
	static_assert(offsetof(hkbDampingModifier, m_dampedValue) == 0x6C);
	static_assert(offsetof(hkbDampingModifier, m_rawVector) == 0x70);
	static_assert(offsetof(hkbDampingModifier, m_dampedVector) == 0x80);
	static_assert(offsetof(hkbDampingModifier, m_vecErrorSum) == 0x90);
	static_assert(offsetof(hkbDampingModifier, m_vecPreviousError) == 0xA0);
	static_assert(offsetof(hkbDampingModifier, m_errorSum) == 0xB0);
	static_assert(offsetof(hkbDampingModifier, m_previousError) == 0xB4);

	class hkbModifierWrapper : public hkbModifier
	{
	public:
		/* 0x058 */ hkbModifier* m_modifier;
	};
	static_assert(sizeof(hkbModifierWrapper) == 0x60);
	static_assert(offsetof(hkbModifierWrapper, m_modifier) == 0x58);

	class hkbDelayedModifier : public hkbModifierWrapper
	{
	public:
		/* 0x060 */ float m_delaySeconds;
		/* 0x064 */ float m_durationSeconds;
		/* 0x068 */ float m_secondsElapsed;
		/* 0x06C */ hkBool m_isActive;
		/* 0x06D */ std::uint8_t pad06D[0x3];
	};
	static_assert(sizeof(hkbDelayedModifier) == 0x70);
	static_assert(offsetof(hkbDelayedModifier, m_delaySeconds) == 0x60);
	static_assert(offsetof(hkbDelayedModifier, m_durationSeconds) == 0x64);
	static_assert(offsetof(hkbDelayedModifier, m_secondsElapsed) == 0x68);
	static_assert(offsetof(hkbDelayedModifier, m_isActive) == 0x6C);

	class hkbDetectCloseToGroundModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkbEventProperty m_closeToGroundEvent;
		/* 0x068 */ float m_closeToGroundHeight;
		/* 0x06C */ float m_raycastDistanceDown;
		/* 0x070 */ std::uint32_t m_collisionFilterInfo;
		/* 0x074 */ std::int16_t m_boneIndex;
		/* 0x076 */ std::int16_t m_animBoneIndex;
		/* 0x078 */ hkBool m_isCloseToGround;
		/* 0x079 */ std::uint8_t pad079[0x7];
	};
	static_assert(sizeof(hkbDetectCloseToGroundModifier) == 0x80);
	static_assert(offsetof(hkbDetectCloseToGroundModifier, m_closeToGroundEvent) == 0x58);
	static_assert(offsetof(hkbDetectCloseToGroundModifier, m_closeToGroundHeight) == 0x68);
	static_assert(offsetof(hkbDetectCloseToGroundModifier, m_raycastDistanceDown) == 0x6C);
	static_assert(offsetof(hkbDetectCloseToGroundModifier, m_collisionFilterInfo) == 0x70);
	static_assert(offsetof(hkbDetectCloseToGroundModifier, m_boneIndex) == 0x74);
	static_assert(offsetof(hkbDetectCloseToGroundModifier, m_animBoneIndex) == 0x76);
	static_assert(offsetof(hkbDetectCloseToGroundModifier, m_isCloseToGround) == 0x78);

	class hkbEvaluateExpressionModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkbExpressionDataArray* m_expressions;
		/* 0x060 */ void* m_compiledExpressionSet;
		/* 0x068 */ hkArray<void> m_internalExpressionsData;
	};
	static_assert(sizeof(hkbEvaluateExpressionModifier) == 0x78);
	static_assert(offsetof(hkbEvaluateExpressionModifier, m_expressions) == 0x58);
	static_assert(offsetof(hkbEvaluateExpressionModifier, m_compiledExpressionSet) == 0x60);
	static_assert(offsetof(hkbEvaluateExpressionModifier, m_internalExpressionsData) == 0x68);

	class hkbEvaluateHandleModifier : public hkbModifier
	{
	public:
		enum class HandleChangeMode : std::int32_t
		{
			HANDLE_CHANGE_MODE_ABRUPT = 0,
			HANDLE_CHANGE_MODE_CONSTANT_VELOCITY = 1,
		};

		/* 0x058 */ hkbHandle* m_handle;
		/* 0x060 */ hkVector4f m_handlePositionOut;
		/* 0x070 */ hkQuaternionf m_handleRotationOut;
		/* 0x080 */ hkBool m_isValidOut;
		/* 0x081 */ std::uint8_t pad081[0x3];
		/* 0x084 */ float m_extrapolationTimeStep;
		/* 0x088 */ float m_handleChangeSpeed;
		/* 0x08C */ std::int8_t m_handleChangeMode;
		/* 0x08D */ std::uint8_t pad08D[0x3];
		/* 0x090 */ hkbHandle m_oldHandle;
		/* 0x0C0 */ hkVector4f m_oldHandlePosition;
		/* 0x0D0 */ hkQuaternionf m_oldHandleRotation;
		/* 0x0E0 */ float m_timeSinceLastModify;
		/* 0x0E4 */ hkBool m_smoothlyChangingHandles;
		/* 0x0E5 */ std::uint8_t pad0E5[0xB];
	};
	static_assert(sizeof(hkbEvaluateHandleModifier) == 0xF0);
	static_assert(offsetof(hkbEvaluateHandleModifier, m_handle) == 0x58);
	static_assert(offsetof(hkbEvaluateHandleModifier, m_handlePositionOut) == 0x60);
	static_assert(offsetof(hkbEvaluateHandleModifier, m_handleRotationOut) == 0x70);
	static_assert(offsetof(hkbEvaluateHandleModifier, m_isValidOut) == 0x80);
	static_assert(offsetof(hkbEvaluateHandleModifier, m_extrapolationTimeStep) == 0x84);
	static_assert(offsetof(hkbEvaluateHandleModifier, m_handleChangeSpeed) == 0x88);
	static_assert(offsetof(hkbEvaluateHandleModifier, m_handleChangeMode) == 0x8C);
	static_assert(offsetof(hkbEvaluateHandleModifier, m_oldHandle) == 0x90);
	static_assert(offsetof(hkbEvaluateHandleModifier, m_oldHandlePosition) == 0xC0);
	static_assert(offsetof(hkbEvaluateHandleModifier, m_oldHandleRotation) == 0xD0);
	static_assert(offsetof(hkbEvaluateHandleModifier, m_timeSinceLastModify) == 0xE0);
	static_assert(offsetof(hkbEvaluateHandleModifier, m_smoothlyChangingHandles) == 0xE4);

	class hkbEventDrivenModifier : public hkbModifierWrapper
	{
	public:
		/* 0x060 */ std::int32_t m_activateEventId;
		/* 0x064 */ std::int32_t m_deactivateEventId;
		/* 0x068 */ hkBool m_activeByDefault;
		/* 0x069 */ hkBool m_isActive;
		/* 0x06A */ std::uint8_t pad06A[0x6];
	};
	static_assert(sizeof(hkbEventDrivenModifier) == 0x70);
	static_assert(offsetof(hkbEventDrivenModifier, m_activateEventId) == 0x60);
	static_assert(offsetof(hkbEventDrivenModifier, m_deactivateEventId) == 0x64);
	static_assert(offsetof(hkbEventDrivenModifier, m_activeByDefault) == 0x68);
	static_assert(offsetof(hkbEventDrivenModifier, m_isActive) == 0x69);

	class hkbEventsFromRangeModifier : public hkbModifier
	{
	public:
		/* 0x058 */ float m_inputValue;
		/* 0x05C */ float m_lowerBound;
		/* 0x060 */ hkbEventRangeDataArray* m_eventRanges;
		/* 0x068 */ hkArray<void> m_wasActiveInPreviousFrame;
	};
	static_assert(sizeof(hkbEventsFromRangeModifier) == 0x78);
	static_assert(offsetof(hkbEventsFromRangeModifier, m_inputValue) == 0x58);
	static_assert(offsetof(hkbEventsFromRangeModifier, m_lowerBound) == 0x5C);
	static_assert(offsetof(hkbEventsFromRangeModifier, m_eventRanges) == 0x60);
	static_assert(offsetof(hkbEventsFromRangeModifier, m_wasActiveInPreviousFrame) == 0x68);

	class hkbExtractRagdollPoseModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::int16_t m_poseMatchingBone0;
		/* 0x05A */ std::int16_t m_poseMatchingBone1;
		/* 0x05C */ std::int16_t m_poseMatchingBone2;
		/* 0x05E */ hkBool m_enableComputeWorldFromModel;
		/* 0x05F */ std::uint8_t pad05F[0x1];
	};
	static_assert(sizeof(hkbExtractRagdollPoseModifier) == 0x60);
	static_assert(offsetof(hkbExtractRagdollPoseModifier, m_poseMatchingBone0) == 0x58);
	static_assert(offsetof(hkbExtractRagdollPoseModifier, m_poseMatchingBone1) == 0x5A);
	static_assert(offsetof(hkbExtractRagdollPoseModifier, m_poseMatchingBone2) == 0x5C);
	static_assert(offsetof(hkbExtractRagdollPoseModifier, m_enableComputeWorldFromModel) == 0x5E);

	class hkbFootIkControlsModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkbFootIkControlData m_controlData;
		/* 0x0B0 */ hkArray<hkbFootIkControlsModifierLeg> m_legs;
		/* 0x0C0 */ hkVector4f m_errorOutTranslation;
		/* 0x0D0 */ hkQuaternionf m_alignWithGroundRotation;
	};
	static_assert(sizeof(hkbFootIkControlsModifier) == 0xE0);
	static_assert(offsetof(hkbFootIkControlsModifier, m_controlData) == 0x60);
	static_assert(offsetof(hkbFootIkControlsModifier, m_legs) == 0xB0);
	static_assert(offsetof(hkbFootIkControlsModifier, m_errorOutTranslation) == 0xC0);
	static_assert(offsetof(hkbFootIkControlsModifier, m_alignWithGroundRotation) == 0xD0);

	class hkbFootIkModifier : public hkbModifier
	{
	public:
		enum class AlignMode : std::int32_t
		{
			ALIGN_MODE_FORWARD_RIGHT = 0,
			ALIGN_MODE_FORWARD = 1,
		};

		/* 0x058 */ hkbFootIkGains m_gains;
		/* 0x088 */ hkArray<hkbFootIkModifierLeg> m_legs;
		/* 0x098 */ float m_raycastDistanceUp;
		/* 0x09C */ float m_raycastDistanceDown;
		/* 0x0A0 */ float m_originalGroundHeightMS;
		/* 0x0A4 */ float m_errorOut;
		/* 0x0A8 */ float m_verticalOffset;
		/* 0x0AC */ std::uint32_t m_collisionFilterInfo;
		/* 0x0B0 */ float m_forwardAlignFraction;
		/* 0x0B4 */ float m_sidewaysAlignFraction;
		/* 0x0B8 */ float m_sidewaysSampleWidth;
		/* 0x0BC */ hkBool m_useTrackData;
		/* 0x0BD */ hkBool m_lockFeetWhenPlanted;
		/* 0x0BE */ hkBool m_useCharacterUpVector;
		/* 0x0BF */ hkBool m_keepSourceFootEndAboveGround;
		/* 0x0C0 */ std::int8_t m_alignMode;
		/* 0x0C1 */ std::uint8_t pad0C1[0x7];
		/* 0x0C8 */ hkArray<hkbFootIkModifierInternalLegData> m_internalLegData;
		/* 0x0D8 */ float m_prevIsFootIkEnabled;
		/* 0x0DC */ hkBool m_isSetUp;
		/* 0x0DD */ hkBool m_isGroundPositionValid;
		/* 0x0DE */ std::uint8_t pad0DE[0x2];
	};
	static_assert(sizeof(hkbFootIkModifier) == 0xE0);
	static_assert(offsetof(hkbFootIkModifier, m_gains) == 0x58);
	static_assert(offsetof(hkbFootIkModifier, m_legs) == 0x88);
	static_assert(offsetof(hkbFootIkModifier, m_raycastDistanceUp) == 0x98);
	static_assert(offsetof(hkbFootIkModifier, m_raycastDistanceDown) == 0x9C);
	static_assert(offsetof(hkbFootIkModifier, m_originalGroundHeightMS) == 0xA0);
	static_assert(offsetof(hkbFootIkModifier, m_errorOut) == 0xA4);
	static_assert(offsetof(hkbFootIkModifier, m_verticalOffset) == 0xA8);
	static_assert(offsetof(hkbFootIkModifier, m_collisionFilterInfo) == 0xAC);
	static_assert(offsetof(hkbFootIkModifier, m_forwardAlignFraction) == 0xB0);
	static_assert(offsetof(hkbFootIkModifier, m_sidewaysAlignFraction) == 0xB4);
	static_assert(offsetof(hkbFootIkModifier, m_sidewaysSampleWidth) == 0xB8);
	static_assert(offsetof(hkbFootIkModifier, m_useTrackData) == 0xBC);
	static_assert(offsetof(hkbFootIkModifier, m_lockFeetWhenPlanted) == 0xBD);
	static_assert(offsetof(hkbFootIkModifier, m_useCharacterUpVector) == 0xBE);
	static_assert(offsetof(hkbFootIkModifier, m_keepSourceFootEndAboveGround) == 0xBF);
	static_assert(offsetof(hkbFootIkModifier, m_alignMode) == 0xC0);
	static_assert(offsetof(hkbFootIkModifier, m_internalLegData) == 0xC8);
	static_assert(offsetof(hkbFootIkModifier, m_prevIsFootIkEnabled) == 0xD8);
	static_assert(offsetof(hkbFootIkModifier, m_isSetUp) == 0xDC);
	static_assert(offsetof(hkbFootIkModifier, m_isGroundPositionValid) == 0xDD);

	class hkbGetHandleOnBoneModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkbHandle* m_handleOut;
		/* 0x060 */ hkStringPtr m_localFrameName;
		/* 0x068 */ std::int16_t m_ragdollBoneIndex;
		/* 0x06A */ std::int16_t m_animationBoneIndex;
		/* 0x06C */ std::uint8_t pad06C[0x4];
	};
	static_assert(sizeof(hkbGetHandleOnBoneModifier) == 0x70);
	static_assert(offsetof(hkbGetHandleOnBoneModifier, m_handleOut) == 0x58);
	static_assert(offsetof(hkbGetHandleOnBoneModifier, m_localFrameName) == 0x60);
	static_assert(offsetof(hkbGetHandleOnBoneModifier, m_ragdollBoneIndex) == 0x68);
	static_assert(offsetof(hkbGetHandleOnBoneModifier, m_animationBoneIndex) == 0x6A);

	class hkbGetUpModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkVector4f m_groundNormal;
		/* 0x070 */ float m_duration;
		/* 0x074 */ float m_alignWithGroundDuration;
		/* 0x078 */ std::int16_t m_rootBoneIndex;
		/* 0x07A */ std::int16_t m_otherBoneIndex;
		/* 0x07C */ std::int16_t m_anotherBoneIndex;
		/* 0x07E */ std::uint8_t pad07E[0x2];
		/* 0x080 */ float m_timeSinceBegin;
		/* 0x084 */ float m_timeStep;
		/* 0x088 */ hkBool m_initNextModify;
		/* 0x089 */ std::uint8_t pad089[0x7];
	};
	static_assert(sizeof(hkbGetUpModifier) == 0x90);
	static_assert(offsetof(hkbGetUpModifier, m_groundNormal) == 0x60);
	static_assert(offsetof(hkbGetUpModifier, m_duration) == 0x70);
	static_assert(offsetof(hkbGetUpModifier, m_alignWithGroundDuration) == 0x74);
	static_assert(offsetof(hkbGetUpModifier, m_rootBoneIndex) == 0x78);
	static_assert(offsetof(hkbGetUpModifier, m_otherBoneIndex) == 0x7A);
	static_assert(offsetof(hkbGetUpModifier, m_anotherBoneIndex) == 0x7C);
	static_assert(offsetof(hkbGetUpModifier, m_timeSinceBegin) == 0x80);
	static_assert(offsetof(hkbGetUpModifier, m_timeStep) == 0x84);
	static_assert(offsetof(hkbGetUpModifier, m_initNextModify) == 0x88);

	class hkbGetWorldFromModelModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkVector4f m_translationOut;
		/* 0x070 */ hkQuaternionf m_rotationOut;
	};
	static_assert(sizeof(hkbGetWorldFromModelModifier) == 0x80);
	static_assert(offsetof(hkbGetWorldFromModelModifier, m_translationOut) == 0x60);
	static_assert(offsetof(hkbGetWorldFromModelModifier, m_rotationOut) == 0x70);

	class hkbGravityModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkVector4f m_initialVelocityInMS;
		/* 0x070 */ float m_gravityConstant;
		/* 0x074 */ std::uint8_t pad074[0xC];
		/* 0x080 */ hkVector4f m_currentVelocityInMS;
		/* 0x090 */ float m_secondsElapsed;
		/* 0x094 */ std::uint8_t pad094[0xC];
	};
	static_assert(sizeof(hkbGravityModifier) == 0xA0);
	static_assert(offsetof(hkbGravityModifier, m_initialVelocityInMS) == 0x60);
	static_assert(offsetof(hkbGravityModifier, m_gravityConstant) == 0x70);
	static_assert(offsetof(hkbGravityModifier, m_currentVelocityInMS) == 0x80);
	static_assert(offsetof(hkbGravityModifier, m_secondsElapsed) == 0x90);

	class hkbHandIkControlsModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkArray<hkbHandIkControlsModifierHand> m_hands;
	};
	static_assert(sizeof(hkbHandIkControlsModifier) == 0x68);
	static_assert(offsetof(hkbHandIkControlsModifier, m_hands) == 0x58);

	class hkbHandIkModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkArray<hkbHandIkModifierHand> m_hands;
		/* 0x068 */ std::int8_t m_fadeInOutCurve;
		/* 0x069 */ std::uint8_t pad069[0x7];
		/* 0x070 */ hkArray<void> m_internalHandData;
	};
	static_assert(sizeof(hkbHandIkModifier) == 0x80);
	static_assert(offsetof(hkbHandIkModifier, m_hands) == 0x58);
	static_assert(offsetof(hkbHandIkModifier, m_fadeInOutCurve) == 0x68);
	static_assert(offsetof(hkbHandIkModifier, m_internalHandData) == 0x70);

	class hkbJigglerModifier : public hkbModifier
	{
	public:
		enum class JiggleCoordinates : std::int32_t
		{
			JIGGLE_IN_WORLD_COORDINATES = 0,
			JIGGLE_IN_MODEL_COORDINATES = 1,
		};

		/* 0x058 */ hkArray<hkbJigglerGroup*> m_jigglerGroups;
		/* 0x068 */ std::int8_t m_jiggleCoordinates;
		/* 0x069 */ std::uint8_t pad069[0x7];
		/* 0x070 */ hkArray<void> m_currentVelocitiesWS;
		/* 0x080 */ hkArray<void> m_currentPositions;
		/* 0x090 */ float m_timeStep;
		/* 0x094 */ hkBool m_initNextModify;
		/* 0x095 */ std::uint8_t pad095[0x3];
	};
	static_assert(sizeof(hkbJigglerModifier) == 0x98);
	static_assert(offsetof(hkbJigglerModifier, m_jigglerGroups) == 0x58);
	static_assert(offsetof(hkbJigglerModifier, m_jiggleCoordinates) == 0x68);
	static_assert(offsetof(hkbJigglerModifier, m_currentVelocitiesWS) == 0x70);
	static_assert(offsetof(hkbJigglerModifier, m_currentPositions) == 0x80);
	static_assert(offsetof(hkbJigglerModifier, m_timeStep) == 0x90);
	static_assert(offsetof(hkbJigglerModifier, m_initNextModify) == 0x94);

	class hkbKeyframeBonesModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkArray<hkbKeyframeBonesModifierKeyframeInfo> m_keyframeInfo;
		/* 0x068 */ hkbBoneIndexArray* m_keyframedBonesList;
	};
	static_assert(sizeof(hkbKeyframeBonesModifier) == 0x70);
	static_assert(offsetof(hkbKeyframeBonesModifier, m_keyframeInfo) == 0x58);
	static_assert(offsetof(hkbKeyframeBonesModifier, m_keyframedBonesList) == 0x68);

	class hkbLookAtModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkVector4f m_targetWS;
		/* 0x070 */ hkVector4f m_headForwardLS;
		/* 0x080 */ hkVector4f m_neckForwardLS;
		/* 0x090 */ hkVector4f m_neckRightLS;
		/* 0x0A0 */ hkVector4f m_eyePositionHS;
		/* 0x0B0 */ float m_newTargetGain;
		/* 0x0B4 */ float m_onGain;
		/* 0x0B8 */ float m_offGain;
		/* 0x0BC */ float m_limitAngleDegrees;
		/* 0x0C0 */ float m_limitAngleLeft;
		/* 0x0C4 */ float m_limitAngleRight;
		/* 0x0C8 */ float m_limitAngleUp;
		/* 0x0CC */ float m_limitAngleDown;
		/* 0x0D0 */ std::int16_t m_headIndex;
		/* 0x0D2 */ std::int16_t m_neckIndex;
		/* 0x0D4 */ hkBool m_isOn;
		/* 0x0D5 */ hkBool m_individualLimitsOn;
		/* 0x0D6 */ hkBool m_isTargetInsideLimitCone;
		/* 0x0D7 */ std::uint8_t pad0D7[0x9];
		/* 0x0E0 */ hkVector4f m_lookAtLastTargetWS;
		/* 0x0F0 */ float m_lookAtWeight;
		/* 0x0F4 */ std::uint8_t pad0F4[0xC];
	};
	static_assert(sizeof(hkbLookAtModifier) == 0x100);
	static_assert(offsetof(hkbLookAtModifier, m_targetWS) == 0x60);
	static_assert(offsetof(hkbLookAtModifier, m_headForwardLS) == 0x70);
	static_assert(offsetof(hkbLookAtModifier, m_neckForwardLS) == 0x80);
	static_assert(offsetof(hkbLookAtModifier, m_neckRightLS) == 0x90);
	static_assert(offsetof(hkbLookAtModifier, m_eyePositionHS) == 0xA0);
	static_assert(offsetof(hkbLookAtModifier, m_newTargetGain) == 0xB0);
	static_assert(offsetof(hkbLookAtModifier, m_onGain) == 0xB4);
	static_assert(offsetof(hkbLookAtModifier, m_offGain) == 0xB8);
	static_assert(offsetof(hkbLookAtModifier, m_limitAngleDegrees) == 0xBC);
	static_assert(offsetof(hkbLookAtModifier, m_limitAngleLeft) == 0xC0);
	static_assert(offsetof(hkbLookAtModifier, m_limitAngleRight) == 0xC4);
	static_assert(offsetof(hkbLookAtModifier, m_limitAngleUp) == 0xC8);
	static_assert(offsetof(hkbLookAtModifier, m_limitAngleDown) == 0xCC);
	static_assert(offsetof(hkbLookAtModifier, m_headIndex) == 0xD0);
	static_assert(offsetof(hkbLookAtModifier, m_neckIndex) == 0xD2);
	static_assert(offsetof(hkbLookAtModifier, m_isOn) == 0xD4);
	static_assert(offsetof(hkbLookAtModifier, m_individualLimitsOn) == 0xD5);
	static_assert(offsetof(hkbLookAtModifier, m_isTargetInsideLimitCone) == 0xD6);
	static_assert(offsetof(hkbLookAtModifier, m_lookAtLastTargetWS) == 0xE0);
	static_assert(offsetof(hkbLookAtModifier, m_lookAtWeight) == 0xF0);

	class hkbMirrorModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkBool m_isAdditive;
		/* 0x059 */ std::uint8_t pad059[0x7];
	};
	static_assert(sizeof(hkbMirrorModifier) == 0x60);
	static_assert(offsetof(hkbMirrorModifier, m_isAdditive) == 0x58);

	class hkbModifierList : public hkbModifier
	{
	public:
		/* 0x058 */ hkArray<hkbModifier*> m_modifiers;
	};
	static_assert(sizeof(hkbModifierList) == 0x68);
	static_assert(offsetof(hkbModifierList, m_modifiers) == 0x58);

	class hkbMoveBoneAttachmentModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::int32_t m_activateEventId;
		/* 0x05C */ std::uint8_t pad05C[0x4];
		/* 0x060 */ hkStringPtr m_attachment;
		/* 0x068 */ hkStringPtr m_localFrame;
	};
	static_assert(sizeof(hkbMoveBoneAttachmentModifier) == 0x70);
	static_assert(offsetof(hkbMoveBoneAttachmentModifier, m_activateEventId) == 0x58);
	static_assert(offsetof(hkbMoveBoneAttachmentModifier, m_attachment) == 0x60);
	static_assert(offsetof(hkbMoveBoneAttachmentModifier, m_localFrame) == 0x68);

	class hkbMoveCharacterModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkVector4f m_offsetPerSecondMS;
		/* 0x070 */ float m_timeSinceLastModify;
		/* 0x074 */ std::uint8_t pad074[0xC];
	};
	static_assert(sizeof(hkbMoveCharacterModifier) == 0x80);
	static_assert(offsetof(hkbMoveCharacterModifier, m_offsetPerSecondMS) == 0x60);
	static_assert(offsetof(hkbMoveCharacterModifier, m_timeSinceLastModify) == 0x70);

	class hkbPoweredRagdollControlsModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkbPoweredRagdollControlData m_controlData;
		/* 0x080 */ hkbBoneIndexArray* m_bones;
		/* 0x088 */ hkbWorldFromModelModeData m_worldFromModelModeData;
		/* 0x090 */ hkbBoneWeightArray* m_boneWeights;
		/* 0x098 */ float m_animationBlendFraction;
		/* 0x09C */ std::uint8_t pad09C[0x4];
	};
	static_assert(sizeof(hkbPoweredRagdollControlsModifier) == 0xA0);
	static_assert(offsetof(hkbPoweredRagdollControlsModifier, m_controlData) == 0x60);
	static_assert(offsetof(hkbPoweredRagdollControlsModifier, m_bones) == 0x80);
	static_assert(offsetof(hkbPoweredRagdollControlsModifier, m_worldFromModelModeData) == 0x88);
	static_assert(offsetof(hkbPoweredRagdollControlsModifier, m_boneWeights) == 0x90);
	static_assert(offsetof(hkbPoweredRagdollControlsModifier, m_animationBlendFraction) == 0x98);

	class hkbRigidBodyRagdollControlsModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkbRigidBodyRagdollControlData m_controlData;
		/* 0x0A0 */ hkbBoneIndexArray* m_bones;
		/* 0x0A8 */ float m_animationBlendFraction;
		/* 0x0AC */ std::uint8_t pad0AC[0x4];
	};
	static_assert(sizeof(hkbRigidBodyRagdollControlsModifier) == 0xB0);
	static_assert(offsetof(hkbRigidBodyRagdollControlsModifier, m_controlData) == 0x60);
	static_assert(offsetof(hkbRigidBodyRagdollControlsModifier, m_bones) == 0xA0);
	static_assert(offsetof(hkbRigidBodyRagdollControlsModifier, m_animationBlendFraction) == 0xA8);

	class hkbRotateCharacterModifier : public hkbModifier
	{
	public:
		/* 0x058 */ float m_degreesPerSecond;
		/* 0x05C */ float m_speedMultiplier;
		/* 0x060 */ hkVector4f m_axisOfRotation;
		/* 0x070 */ float m_angle;
		/* 0x074 */ std::uint8_t pad074[0xC];
	};
	static_assert(sizeof(hkbRotateCharacterModifier) == 0x80);
	static_assert(offsetof(hkbRotateCharacterModifier, m_degreesPerSecond) == 0x58);
	static_assert(offsetof(hkbRotateCharacterModifier, m_speedMultiplier) == 0x5C);
	static_assert(offsetof(hkbRotateCharacterModifier, m_axisOfRotation) == 0x60);
	static_assert(offsetof(hkbRotateCharacterModifier, m_angle) == 0x70);

	class hkbSenseHandleModifier : public hkbModifier
	{
	public:
		enum class SensingMode : std::int32_t
		{
			SENSE_IN_NEARBY_RIGID_BODIES = 0,
			SENSE_IN_RIGID_BODIES_OUTSIDE_THIS_CHARACTER = 1,
			SENSE_IN_OTHER_CHARACTER_RIGID_BODIES = 2,
			SENSE_IN_THIS_CHARACTER_RIGID_BODIES = 3,
			SENSE_IN_GIVEN_CHARACTER_RIGID_BODIES = 4,
			SENSE_IN_GIVEN_RIGID_BODY = 5,
			SENSE_IN_OTHER_CHARACTER_SKELETON = 6,
			SENSE_IN_THIS_CHARACTER_SKELETON = 7,
			SENSE_IN_GIVEN_CHARACTER_SKELETON = 8,
			SENSE_IN_GIVEN_LOCAL_FRAME_GROUP = 9,
		};

		/* 0x058 */ hkbHandle m_handle;
		/* 0x088 */ std::uint8_t pad088[0x8];
		/* 0x090 */ hkVector4f m_sensorLocalOffset;
		/* 0x0A0 */ hkArray<hkbSenseHandleModifierRange> m_ranges;
		/* 0x0B0 */ hkbHandle* m_handleOut;
		/* 0x0B8 */ hkbHandle* m_handleIn;
		/* 0x0C0 */ hkStringPtr m_localFrameName;
		/* 0x0C8 */ hkStringPtr m_sensorLocalFrameName;
		/* 0x0D0 */ float m_minDistance;
		/* 0x0D4 */ float m_maxDistance;
		/* 0x0D8 */ float m_distanceOut;
		/* 0x0DC */ std::uint32_t m_collisionFilterInfo;
		/* 0x0E0 */ std::int16_t m_sensorRagdollBoneIndex;
		/* 0x0E2 */ std::int16_t m_sensorAnimationBoneIndex;
		/* 0x0E4 */ std::int8_t m_sensingMode;
		/* 0x0E5 */ hkBool m_extrapolateSensorPosition;
		/* 0x0E6 */ hkBool m_keepFirstSensedHandle;
		/* 0x0E7 */ hkBool m_foundHandleOut;
		/* 0x0E8 */ float m_timeSinceLastModify;
		/* 0x0EC */ std::int32_t m_rangeIndexForEventToSendNextUpdate;
	};
	static_assert(sizeof(hkbSenseHandleModifier) == 0xF0);
	static_assert(offsetof(hkbSenseHandleModifier, m_handle) == 0x58);
	static_assert(offsetof(hkbSenseHandleModifier, m_sensorLocalOffset) == 0x90);
	static_assert(offsetof(hkbSenseHandleModifier, m_ranges) == 0xA0);
	static_assert(offsetof(hkbSenseHandleModifier, m_handleOut) == 0xB0);
	static_assert(offsetof(hkbSenseHandleModifier, m_handleIn) == 0xB8);
	static_assert(offsetof(hkbSenseHandleModifier, m_localFrameName) == 0xC0);
	static_assert(offsetof(hkbSenseHandleModifier, m_sensorLocalFrameName) == 0xC8);
	static_assert(offsetof(hkbSenseHandleModifier, m_minDistance) == 0xD0);
	static_assert(offsetof(hkbSenseHandleModifier, m_maxDistance) == 0xD4);
	static_assert(offsetof(hkbSenseHandleModifier, m_distanceOut) == 0xD8);
	static_assert(offsetof(hkbSenseHandleModifier, m_collisionFilterInfo) == 0xDC);
	static_assert(offsetof(hkbSenseHandleModifier, m_sensorRagdollBoneIndex) == 0xE0);
	static_assert(offsetof(hkbSenseHandleModifier, m_sensorAnimationBoneIndex) == 0xE2);
	static_assert(offsetof(hkbSenseHandleModifier, m_sensingMode) == 0xE4);
	static_assert(offsetof(hkbSenseHandleModifier, m_extrapolateSensorPosition) == 0xE5);
	static_assert(offsetof(hkbSenseHandleModifier, m_keepFirstSensedHandle) == 0xE6);
	static_assert(offsetof(hkbSenseHandleModifier, m_foundHandleOut) == 0xE7);
	static_assert(offsetof(hkbSenseHandleModifier, m_timeSinceLastModify) == 0xE8);
	static_assert(offsetof(hkbSenseHandleModifier, m_rangeIndexForEventToSendNextUpdate) == 0xEC);

	class hkbSequence : public hkbModifier
	{
	public:
		/* 0x058 */ hkArray<hkbEventSequencedData*> m_eventSequencedData;
		/* 0x068 */ hkArray<hkbRealVariableSequencedData*> m_realVariableSequencedData;
		/* 0x078 */ hkArray<hkbBoolVariableSequencedData*> m_boolVariableSequencedData;
		/* 0x088 */ hkArray<hkbIntVariableSequencedData*> m_intVariableSequencedData;
		/* 0x098 */ std::int32_t m_enableEventId;
		/* 0x09C */ std::int32_t m_disableEventId;
		/* 0x0A0 */ hkbSequenceStringData* m_stringData;
		/* 0x0A8 */ void* m_variableIdMap;
		/* 0x0B0 */ void* m_eventIdMap;
		/* 0x0B8 */ hkArray<void> m_nextSampleEvents;
		/* 0x0C8 */ hkArray<void> m_nextSampleReals;
		/* 0x0D8 */ hkArray<void> m_nextSampleBools;
		/* 0x0E8 */ hkArray<void> m_nextSampleInts;
		/* 0x0F8 */ float m_time;
		/* 0x0FC */ hkBool m_isEnabled;
		/* 0x0FD */ std::uint8_t pad0FD[0x3];
	};
	static_assert(sizeof(hkbSequence) == 0x100);
	static_assert(offsetof(hkbSequence, m_eventSequencedData) == 0x58);
	static_assert(offsetof(hkbSequence, m_realVariableSequencedData) == 0x68);
	static_assert(offsetof(hkbSequence, m_boolVariableSequencedData) == 0x78);
	static_assert(offsetof(hkbSequence, m_intVariableSequencedData) == 0x88);
	static_assert(offsetof(hkbSequence, m_enableEventId) == 0x98);
	static_assert(offsetof(hkbSequence, m_disableEventId) == 0x9C);
	static_assert(offsetof(hkbSequence, m_stringData) == 0xA0);
	static_assert(offsetof(hkbSequence, m_variableIdMap) == 0xA8);
	static_assert(offsetof(hkbSequence, m_eventIdMap) == 0xB0);
	static_assert(offsetof(hkbSequence, m_nextSampleEvents) == 0xB8);
	static_assert(offsetof(hkbSequence, m_nextSampleReals) == 0xC8);
	static_assert(offsetof(hkbSequence, m_nextSampleBools) == 0xD8);
	static_assert(offsetof(hkbSequence, m_nextSampleInts) == 0xE8);
	static_assert(offsetof(hkbSequence, m_time) == 0xF8);
	static_assert(offsetof(hkbSequence, m_isEnabled) == 0xFC);

	class hkbSetWorldFromModelModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkVector4f m_translation;
		/* 0x070 */ hkQuaternionf m_rotation;
		/* 0x080 */ hkBool m_setTranslation;
		/* 0x081 */ hkBool m_setRotation;
		/* 0x082 */ std::uint8_t pad082[0xE];
	};
	static_assert(sizeof(hkbSetWorldFromModelModifier) == 0x90);
	static_assert(offsetof(hkbSetWorldFromModelModifier, m_translation) == 0x60);
	static_assert(offsetof(hkbSetWorldFromModelModifier, m_rotation) == 0x70);
	static_assert(offsetof(hkbSetWorldFromModelModifier, m_setTranslation) == 0x80);
	static_assert(offsetof(hkbSetWorldFromModelModifier, m_setRotation) == 0x81);

	class hkbStateDependentModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkBool m_applyModifierDuringTransition;
		/* 0x059 */ std::uint8_t pad059[0x7];
		/* 0x060 */ hkArray<std::int32_t> m_stateIds;
		/* 0x070 */ hkbModifier* m_modifier;
		/* 0x078 */ hkBool m_isActive;
		/* 0x079 */ std::uint8_t pad079[0x7];
		/* 0x080 */ hkbStateMachine* m_stateMachine;
	};
	static_assert(sizeof(hkbStateDependentModifier) == 0x88);
	static_assert(offsetof(hkbStateDependentModifier, m_applyModifierDuringTransition) == 0x58);
	static_assert(offsetof(hkbStateDependentModifier, m_stateIds) == 0x60);
	static_assert(offsetof(hkbStateDependentModifier, m_modifier) == 0x70);
	static_assert(offsetof(hkbStateDependentModifier, m_isActive) == 0x78);
	static_assert(offsetof(hkbStateDependentModifier, m_stateMachine) == 0x80);

	class hkbTimerModifier : public hkbModifier
	{
	public:
		/* 0x058 */ float m_alarmTimeSeconds;
		/* 0x05C */ std::uint8_t pad05C[0x4];
		/* 0x060 */ hkbEventProperty m_alarmEvent;
		/* 0x070 */ float m_secondsElapsed;
		/* 0x074 */ std::uint8_t pad074[0x4];
	};
	static_assert(sizeof(hkbTimerModifier) == 0x78);
	static_assert(offsetof(hkbTimerModifier, m_alarmTimeSeconds) == 0x58);
	static_assert(offsetof(hkbTimerModifier, m_alarmEvent) == 0x60);
	static_assert(offsetof(hkbTimerModifier, m_secondsElapsed) == 0x70);

	class hkbTransformVectorModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkQuaternionf m_rotation;
		/* 0x070 */ hkVector4f m_translation;
		/* 0x080 */ hkVector4f m_vectorIn;
		/* 0x090 */ hkVector4f m_vectorOut;
		/* 0x0A0 */ hkBool m_rotateOnly;
		/* 0x0A1 */ hkBool m_inverse;
		/* 0x0A2 */ hkBool m_computeOnActivate;
		/* 0x0A3 */ hkBool m_computeOnModify;
		/* 0x0A4 */ std::uint8_t pad0A4[0xC];
	};
	static_assert(sizeof(hkbTransformVectorModifier) == 0xB0);
	static_assert(offsetof(hkbTransformVectorModifier, m_rotation) == 0x60);
	static_assert(offsetof(hkbTransformVectorModifier, m_translation) == 0x70);
	static_assert(offsetof(hkbTransformVectorModifier, m_vectorIn) == 0x80);
	static_assert(offsetof(hkbTransformVectorModifier, m_vectorOut) == 0x90);
	static_assert(offsetof(hkbTransformVectorModifier, m_rotateOnly) == 0xA0);
	static_assert(offsetof(hkbTransformVectorModifier, m_inverse) == 0xA1);
	static_assert(offsetof(hkbTransformVectorModifier, m_computeOnActivate) == 0xA2);
	static_assert(offsetof(hkbTransformVectorModifier, m_computeOnModify) == 0xA3);

	class hkbTwistModifier : public hkbModifier
	{
	public:
		enum class SetAngleMethod : std::int32_t
		{
			LINEAR = 0,
			RAMPED = 1,
		};

		enum class RotationAxisCoordinates : std::int32_t
		{
			ROTATION_AXIS_IN_MODEL_COORDINATES = 0,
			ROTATION_AXIS_IN_PARENT_COORDINATES = 1,
			ROTATION_AXIS_IN_LOCAL_COORDINATES = 2,
		};

		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkVector4f m_axisOfRotation;
		/* 0x070 */ float m_twistAngle;
		/* 0x074 */ std::int16_t m_startBoneIndex;
		/* 0x076 */ std::int16_t m_endBoneIndex;
		/* 0x078 */ std::int8_t m_setAngleMethod;
		/* 0x079 */ std::int8_t m_rotationAxisCoordinates;
		/* 0x07A */ hkBool m_isAdditive;
		/* 0x07B */ std::uint8_t pad07B[0x5];
		/* 0x080 */ hkArray<void> m_boneChainIndices;
		/* 0x090 */ hkArray<void> m_parentBoneIndices;
	};
	static_assert(sizeof(hkbTwistModifier) == 0xA0);
	static_assert(offsetof(hkbTwistModifier, m_axisOfRotation) == 0x60);
	static_assert(offsetof(hkbTwistModifier, m_twistAngle) == 0x70);
	static_assert(offsetof(hkbTwistModifier, m_startBoneIndex) == 0x74);
	static_assert(offsetof(hkbTwistModifier, m_endBoneIndex) == 0x76);
	static_assert(offsetof(hkbTwistModifier, m_setAngleMethod) == 0x78);
	static_assert(offsetof(hkbTwistModifier, m_rotationAxisCoordinates) == 0x79);
	static_assert(offsetof(hkbTwistModifier, m_isAdditive) == 0x7A);
	static_assert(offsetof(hkbTwistModifier, m_boneChainIndices) == 0x80);
	static_assert(offsetof(hkbTwistModifier, m_parentBoneIndices) == 0x90);

}
