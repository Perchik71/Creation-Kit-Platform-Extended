// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseObject.h>
#include <EditorAPI/H/hkBaseTypes.h>
#include <EditorAPI/H/hkbBehaviorGraph.h>

namespace CKPE::Fallout4::EditorAPI
{
	class BSAlignBoneModifier;
	class BSAssignVariablesModifier;
	class BSBehaviorGraphSwapGenerator;
	class BSBoneSwitchGenerator;
	class BSBoneSwitchGeneratorBoneData;
	class BSComputeAddBoneAnimModifier;
	class BSCyclicBlendTransitionGenerator;
	class BSDecomposeVectorModifier;
	class BSDirectAtCapturePoseModifier;
	class BSDirectAtModifier;
	class BSDistTriggerModifier;
	class BSEventEveryNEventsModifier;
	class BSEventOnDeactivateModifier;
	class BSEventOnFalseToTrueModifier;
	class BSGetTimeStepModifier;
	class BSIStateManagerModifier;
	class BSIStateManagerModifierBSIStateManagerStateListener;
	class BSIStateManagerModifierBSiStateData;
	class BSInterpValueModifier;
	class BSIsActiveModifier;
	class BSLimbCycleModifier;
	class BSLimbIKModifier;
	class BSLookAtCapturePoseModifier;
	class BSLookAtModifier;
	class BSLookAtModifierBoneData;
	class BSModifyOnceModifier;
	class BSOffsetAnimationGenerator;
	class BSPassByTargetTriggerModifier;
	class BSRagdollContactListenerModifier;
	class BSRandomAlarmModifier;
	class BSRetargetBodyBlend;
	class BSReverseSpineTwistModifier;
	class BSRootTwistModifier;
	class BSSpeedSamplerModifier;
	class BSSpringState;
	class BSTimerModifier;
	class BSTweenerModifier;
	class BSiStateTaggingGenerator;
	class BSiStateTaggingStateListener;
	class hkbBoneIndexArray;
	class hkbBoneWeightArray;
	class hkbEventProperty;
	class hkbGenerator;
	class hkbModifier;

	class BSAlignBoneModifier : public hkbModifier
	{
	public:
		enum class CONTEXT : std::int32_t
		{
			None = 0,
			WeaponGrip = 1,
		};

		/* 0x058 */ std::int8_t m_context;
		/* 0x059 */ std::uint8_t pad059[0x1];
		/* 0x05A */ std::int16_t m_boneIndex;
		/* 0x05C */ std::uint8_t pad05C[0x4];
		/* 0x060 */ hkStringPtr m_targetName;
		/* 0x068 */ hkBool m_rotation;
		/* 0x069 */ hkBool m_translation;
		/* 0x06A */ hkBool m_mirrored;
		/* 0x06B */ std::uint8_t pad06B[0x1];
		/* 0x06C */ std::int16_t m_weaponBoneIndex;
		/* 0x06E */ std::int16_t m_weaponMirroredBoneIndex;
		/* 0x070 */ hkBool m_debugVisuals;
		/* 0x071 */ hkBool m_debugMode;
		/* 0x072 */ std::uint8_t pad072[0xE];
		/* 0x080 */ hkVector4f m_debugTargetLocation;
		/* 0x090 */ hkQuaternionf m_debugTargetRotation;
		/* 0x0A0 */ hkBool m_hasTarget;
		/* 0x0A1 */ std::uint8_t pad0A1[0xF];
		/* 0x0B0 */ hkQsTransformf m_targetMS;
	};
	static_assert(sizeof(BSAlignBoneModifier) == 0xE0);
	static_assert(offsetof(BSAlignBoneModifier, m_context) == 0x58);
	static_assert(offsetof(BSAlignBoneModifier, m_boneIndex) == 0x5A);
	static_assert(offsetof(BSAlignBoneModifier, m_targetName) == 0x60);
	static_assert(offsetof(BSAlignBoneModifier, m_rotation) == 0x68);
	static_assert(offsetof(BSAlignBoneModifier, m_translation) == 0x69);
	static_assert(offsetof(BSAlignBoneModifier, m_mirrored) == 0x6A);
	static_assert(offsetof(BSAlignBoneModifier, m_weaponBoneIndex) == 0x6C);
	static_assert(offsetof(BSAlignBoneModifier, m_weaponMirroredBoneIndex) == 0x6E);
	static_assert(offsetof(BSAlignBoneModifier, m_debugVisuals) == 0x70);
	static_assert(offsetof(BSAlignBoneModifier, m_debugMode) == 0x71);
	static_assert(offsetof(BSAlignBoneModifier, m_debugTargetLocation) == 0x80);
	static_assert(offsetof(BSAlignBoneModifier, m_debugTargetRotation) == 0x90);
	static_assert(offsetof(BSAlignBoneModifier, m_hasTarget) == 0xA0);
	static_assert(offsetof(BSAlignBoneModifier, m_targetMS) == 0xB0);

	class BSAssignVariablesModifier : public hkbModifier
	{
	public:
		/* 0x058 */ float m_floatVariable1;
		/* 0x05C */ float m_floatValue1;
		/* 0x060 */ float m_floatVariable2;
		/* 0x064 */ float m_floatValue2;
		/* 0x068 */ float m_floatVariable3;
		/* 0x06C */ float m_floatValue3;
		/* 0x070 */ float m_floatVariable4;
		/* 0x074 */ float m_floatValue4;
		/* 0x078 */ float m_floatVariable5;
		/* 0x07C */ float m_floatValue5;
		/* 0x080 */ float m_floatVariable6;
		/* 0x084 */ float m_floatValue6;
		/* 0x088 */ float m_floatVariable7;
		/* 0x08C */ float m_floatValue7;
		/* 0x090 */ float m_floatVariable8;
		/* 0x094 */ float m_floatValue8;
		/* 0x098 */ float m_floatVariable9;
		/* 0x09C */ float m_floatValue9;
		/* 0x0A0 */ float m_floatVariable10;
		/* 0x0A4 */ float m_floatValue10;
		/* 0x0A8 */ float m_floatVariable11;
		/* 0x0AC */ float m_floatValue11;
		/* 0x0B0 */ float m_floatVariable12;
		/* 0x0B4 */ float m_floatValue12;
		/* 0x0B8 */ float m_floatVariable13;
		/* 0x0BC */ float m_floatValue13;
		/* 0x0C0 */ float m_floatVariable14;
		/* 0x0C4 */ float m_floatValue14;
		/* 0x0C8 */ float m_floatVariable15;
		/* 0x0CC */ float m_floatValue15;
		/* 0x0D0 */ float m_floatVariable16;
		/* 0x0D4 */ float m_floatValue16;
		/* 0x0D8 */ float m_floatVariable17;
		/* 0x0DC */ float m_floatValue17;
		/* 0x0E0 */ float m_floatVariable18;
		/* 0x0E4 */ float m_floatValue18;
		/* 0x0E8 */ float m_floatVariable19;
		/* 0x0EC */ float m_floatValue19;
		/* 0x0F0 */ float m_floatVariable20;
		/* 0x0F4 */ float m_floatValue20;
		/* 0x0F8 */ std::int32_t m_intVariable1;
		/* 0x0FC */ std::int32_t m_intValue1;
		/* 0x100 */ std::int32_t m_intVariable2;
		/* 0x104 */ std::int32_t m_intValue2;
		/* 0x108 */ std::int32_t m_intVariable3;
		/* 0x10C */ std::int32_t m_intValue3;
		/* 0x110 */ std::int32_t m_intVariable4;
		/* 0x114 */ std::int32_t m_intValue4;
	};
	static_assert(sizeof(BSAssignVariablesModifier) == 0x118);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable1) == 0x58);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue1) == 0x5C);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable2) == 0x60);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue2) == 0x64);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable3) == 0x68);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue3) == 0x6C);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable4) == 0x70);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue4) == 0x74);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable5) == 0x78);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue5) == 0x7C);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable6) == 0x80);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue6) == 0x84);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable7) == 0x88);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue7) == 0x8C);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable8) == 0x90);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue8) == 0x94);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable9) == 0x98);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue9) == 0x9C);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable10) == 0xA0);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue10) == 0xA4);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable11) == 0xA8);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue11) == 0xAC);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable12) == 0xB0);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue12) == 0xB4);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable13) == 0xB8);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue13) == 0xBC);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable14) == 0xC0);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue14) == 0xC4);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable15) == 0xC8);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue15) == 0xCC);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable16) == 0xD0);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue16) == 0xD4);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable17) == 0xD8);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue17) == 0xDC);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable18) == 0xE0);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue18) == 0xE4);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable19) == 0xE8);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue19) == 0xEC);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatVariable20) == 0xF0);
	static_assert(offsetof(BSAssignVariablesModifier, m_floatValue20) == 0xF4);
	static_assert(offsetof(BSAssignVariablesModifier, m_intVariable1) == 0xF8);
	static_assert(offsetof(BSAssignVariablesModifier, m_intValue1) == 0xFC);
	static_assert(offsetof(BSAssignVariablesModifier, m_intVariable2) == 0x100);
	static_assert(offsetof(BSAssignVariablesModifier, m_intValue2) == 0x104);
	static_assert(offsetof(BSAssignVariablesModifier, m_intVariable3) == 0x108);
	static_assert(offsetof(BSAssignVariablesModifier, m_intValue3) == 0x10C);
	static_assert(offsetof(BSAssignVariablesModifier, m_intVariable4) == 0x110);
	static_assert(offsetof(BSAssignVariablesModifier, m_intValue4) == 0x114);

	class BSBehaviorGraphSwapGenerator : public hkbGenerator
	{
	public:
		/* 0x088 */ std::uint8_t pad088[0x8];
		/* 0x090 */ hkbGenerator* m_pDefaultGenerator;
		/* 0x098 */ std::uint8_t pad098[0x8];
	};
	static_assert(sizeof(BSBehaviorGraphSwapGenerator) == 0xA0);
	static_assert(offsetof(BSBehaviorGraphSwapGenerator, m_pDefaultGenerator) == 0x90);

	class BSBoneSwitchGenerator : public hkbGenerator
	{
	public:
		/* 0x088 */ std::uint8_t pad088[0x8];
		/* 0x090 */ hkbGenerator* m_pDefaultGenerator;
		/* 0x098 */ hkArray<BSBoneSwitchGeneratorBoneData*> m_ChildrenA;
		/* 0x0A8 */ std::uint8_t pad0A8[0x8];
	};
	static_assert(sizeof(BSBoneSwitchGenerator) == 0xB0);
	static_assert(offsetof(BSBoneSwitchGenerator, m_pDefaultGenerator) == 0x90);
	static_assert(offsetof(BSBoneSwitchGenerator, m_ChildrenA) == 0x98);

	class BSBoneSwitchGeneratorBoneData : public hkbBindable
	{
	public:
		/* 0x030 */ hkbGenerator* m_pGenerator;
		/* 0x038 */ hkbBoneWeightArray* m_spBoneWeight;
	};
	static_assert(sizeof(BSBoneSwitchGeneratorBoneData) == 0x40);
	static_assert(offsetof(BSBoneSwitchGeneratorBoneData, m_pGenerator) == 0x30);
	static_assert(offsetof(BSBoneSwitchGeneratorBoneData, m_spBoneWeight) == 0x38);

	class BSComputeAddBoneAnimModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::int16_t m_boneIndex;
		/* 0x05A */ std::uint8_t pad05A[0x6];
		/* 0x060 */ hkVector4f m_translationLSOut;
		/* 0x070 */ hkQuaternionf m_rotationLSOut;
		/* 0x080 */ hkVector4f m_scaleLSOut;
		/* 0x090 */ void* m_pSkeletonMemory;
		/* 0x098 */ std::uint8_t pad098[0x8];
	};
	static_assert(sizeof(BSComputeAddBoneAnimModifier) == 0xA0);
	static_assert(offsetof(BSComputeAddBoneAnimModifier, m_boneIndex) == 0x58);
	static_assert(offsetof(BSComputeAddBoneAnimModifier, m_translationLSOut) == 0x60);
	static_assert(offsetof(BSComputeAddBoneAnimModifier, m_rotationLSOut) == 0x70);
	static_assert(offsetof(BSComputeAddBoneAnimModifier, m_scaleLSOut) == 0x80);
	static_assert(offsetof(BSComputeAddBoneAnimModifier, m_pSkeletonMemory) == 0x90);

	class BSCyclicBlendTransitionGenerator : public hkbGenerator
	{
	public:
		enum class CurrentBlendMode : std::int32_t
		{
			MODE_INACTIVE = -1,
			MODE_DEFAULT = 0,
			MODE_FROZEN = 1,
			MODE_BLENDING = 2,
			MODE_WAITINGFORBLENDING = 3,
			MODE_LOCKED = 4,
			MODE_LOCKED_WAITINGFORBLENDING = 5,
		};

		/* 0x088 */ std::uint8_t pad088[0x8];
		/* 0x090 */ hkbGenerator* m_pBlenderGenerator;
		/* 0x098 */ hkbEventProperty m_EventToFreezeBlendValue;
		/* 0x0A8 */ hkbEventProperty m_EventToCrossBlend;
		/* 0x0B8 */ hkbEventProperty m_TransitionOutEvent;
		/* 0x0C8 */ hkbEventProperty m_TransitionInEvent;
		/* 0x0D8 */ float m_fBlendParameter;
		/* 0x0DC */ float m_fTransitionDuration;
		/* 0x0E0 */ std::int8_t m_eBlendCurve;
		/* 0x0E1 */ std::uint8_t pad0E1[0x7];
		/* 0x0E8 */ hkArray<void*> m_pTransitionBlenderGeneratorsA;
		/* 0x0F8 */ hkArray<void> m_sortedChildren;
		/* 0x108 */ void* m_pTempOutput;
		/* 0x110 */ std::int8_t m_currentMode;
		/* 0x111 */ std::uint8_t pad111[0xF];
	};
	static_assert(sizeof(BSCyclicBlendTransitionGenerator) == 0x120);
	static_assert(offsetof(BSCyclicBlendTransitionGenerator, m_pBlenderGenerator) == 0x90);
	static_assert(offsetof(BSCyclicBlendTransitionGenerator, m_EventToFreezeBlendValue) == 0x98);
	static_assert(offsetof(BSCyclicBlendTransitionGenerator, m_EventToCrossBlend) == 0xA8);
	static_assert(offsetof(BSCyclicBlendTransitionGenerator, m_TransitionOutEvent) == 0xB8);
	static_assert(offsetof(BSCyclicBlendTransitionGenerator, m_TransitionInEvent) == 0xC8);
	static_assert(offsetof(BSCyclicBlendTransitionGenerator, m_fBlendParameter) == 0xD8);
	static_assert(offsetof(BSCyclicBlendTransitionGenerator, m_fTransitionDuration) == 0xDC);
	static_assert(offsetof(BSCyclicBlendTransitionGenerator, m_eBlendCurve) == 0xE0);
	static_assert(offsetof(BSCyclicBlendTransitionGenerator, m_pTransitionBlenderGeneratorsA) == 0xE8);
	static_assert(offsetof(BSCyclicBlendTransitionGenerator, m_sortedChildren) == 0xF8);
	static_assert(offsetof(BSCyclicBlendTransitionGenerator, m_pTempOutput) == 0x108);
	static_assert(offsetof(BSCyclicBlendTransitionGenerator, m_currentMode) == 0x110);

	class BSDecomposeVectorModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkVector4f m_vector;
		/* 0x070 */ float m_x;
		/* 0x074 */ float m_y;
		/* 0x078 */ float m_z;
		/* 0x07C */ float m_w;
	};
	static_assert(sizeof(BSDecomposeVectorModifier) == 0x80);
	static_assert(offsetof(BSDecomposeVectorModifier, m_vector) == 0x60);
	static_assert(offsetof(BSDecomposeVectorModifier, m_x) == 0x70);
	static_assert(offsetof(BSDecomposeVectorModifier, m_y) == 0x74);
	static_assert(offsetof(BSDecomposeVectorModifier, m_z) == 0x78);
	static_assert(offsetof(BSDecomposeVectorModifier, m_w) == 0x7C);

	class BSDirectAtCapturePoseModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::int32_t m_capturePoseEventId;
		/* 0x05C */ hkBool m_capturePoseOnActivate;
		/* 0x05D */ hkBool m_capturePose;
		/* 0x05E */ std::uint8_t pad05E[0x2];
	};
	static_assert(sizeof(BSDirectAtCapturePoseModifier) == 0x60);
	static_assert(offsetof(BSDirectAtCapturePoseModifier, m_capturePoseEventId) == 0x58);
	static_assert(offsetof(BSDirectAtCapturePoseModifier, m_capturePoseOnActivate) == 0x5C);
	static_assert(offsetof(BSDirectAtCapturePoseModifier, m_capturePose) == 0x5D);

	class BSDirectAtModifier : public hkbModifier
	{
	public:
		enum class TARGET : std::int32_t
		{
			AppSupplied = 0,
			Camera = 1,
		};

		enum class CONTEXT : std::int32_t
		{
			None = 0,
			BlockAnticipate = 1,
			BowAttack = 2,
			GunAttack = 3,
			MagicAttack = 4,
		};

		/* 0x058 */ hkBool m_directAtOn;
		/* 0x059 */ std::int8_t m_target;
		/* 0x05A */ std::int8_t m_context;
		/* 0x05B */ std::uint8_t pad05B[0x1];
		/* 0x05C */ std::int16_t m_sourceBoneIndex;
		/* 0x05E */ hkBool m_offsetsCharacterRelative;
		/* 0x05F */ std::uint8_t pad05F[0x1];
		/* 0x060 */ std::int16_t m_startBoneIndex;
		/* 0x062 */ std::int16_t m_endBoneIndex;
		/* 0x064 */ float m_limitHeadingDegreesCCW;
		/* 0x068 */ float m_limitHeadingDegreesCW;
		/* 0x06C */ float m_limitPitchDegreesUp;
		/* 0x070 */ float m_limitPitchDegreesDown;
		/* 0x074 */ float m_onGain;
		/* 0x078 */ float m_offGain;
		/* 0x07C */ float m_onLeadIn;
		/* 0x080 */ float m_offLeadIn;
		/* 0x084 */ float m_initialHeadingOffset;
		/* 0x088 */ float m_initialPitchOffset;
		/* 0x08C */ float m_initialGain;
		/* 0x090 */ hkBool m_allowSourceBonePoseCapture;
		/* 0x091 */ std::uint8_t pad091[0x3];
		/* 0x094 */ float m_sourceBonePoseCaptureBlendGain;
		/* 0x098 */ std::int32_t m_useCurrentSourceBonePoseEventId;
		/* 0x09C */ hkBool m_counteractSourceBoneWorldSpaceDelta;
		/* 0x09D */ std::uint8_t pad09D[0x3];
		/* 0x0A0 */ std::int32_t m_snapToTargetEventId;
		/* 0x0A4 */ float m_directAtCameraX;
		/* 0x0A8 */ float m_directAtCameraY;
		/* 0x0AC */ float m_directAtCameraZ;
		/* 0x0B0 */ hkBool m_drawDebugInfo;
		/* 0x0B1 */ hkBool m_active;
		/* 0x0B2 */ std::uint8_t pad0B2[0x2];
		/* 0x0B4 */ float m_currentHeadingOffset;
		/* 0x0B8 */ float m_currentPitchOffset;
		/* 0x0BC */ float m_currentGain;
		/* 0x0C0 */ hkBool m_firstModify;
		/* 0x0C1 */ std::uint8_t pad0C1[0x3];
		/* 0x0C4 */ float m_headingOffset;
		/* 0x0C8 */ float m_pitchOffset;
		/* 0x0CC */ hkBool m_buseSourceBonePoseCapture;
		/* 0x0CD */ std::uint8_t pad0CD[0x3];
		/* 0x0D0 */ float m_sourceBonePoseGain;
		/* 0x0D4 */ std::uint8_t pad0D4[0xC];
		/* 0x0E0 */ hkQsTransformf m_sourceBonePoseCapture;
		/* 0x110 */ hkQsTransformf m_sourceBonePose;
		/* 0x140 */ hkVector4f m_sourceBoneDirWS;
		/* 0x150 */ hkBool m_snapToTarget;
		/* 0x151 */ std::uint8_t pad151[0x3];
		/* 0x154 */ float m_timeStep;
		/* 0x158 */ hkBool m_directAtTarget;
		/* 0x159 */ std::uint8_t pad159[0x7];
		/* 0x160 */ hkVector4f m_targetLocation;
		/* 0x170 */ hkQsTransformf m_worldTransform;
		/* 0x1A0 */ hkArray<void> m_boneChainIndices;
	};
	static_assert(sizeof(BSDirectAtModifier) == 0x1B0);
	static_assert(offsetof(BSDirectAtModifier, m_directAtOn) == 0x58);
	static_assert(offsetof(BSDirectAtModifier, m_target) == 0x59);
	static_assert(offsetof(BSDirectAtModifier, m_context) == 0x5A);
	static_assert(offsetof(BSDirectAtModifier, m_sourceBoneIndex) == 0x5C);
	static_assert(offsetof(BSDirectAtModifier, m_offsetsCharacterRelative) == 0x5E);
	static_assert(offsetof(BSDirectAtModifier, m_startBoneIndex) == 0x60);
	static_assert(offsetof(BSDirectAtModifier, m_endBoneIndex) == 0x62);
	static_assert(offsetof(BSDirectAtModifier, m_limitHeadingDegreesCCW) == 0x64);
	static_assert(offsetof(BSDirectAtModifier, m_limitHeadingDegreesCW) == 0x68);
	static_assert(offsetof(BSDirectAtModifier, m_limitPitchDegreesUp) == 0x6C);
	static_assert(offsetof(BSDirectAtModifier, m_limitPitchDegreesDown) == 0x70);
	static_assert(offsetof(BSDirectAtModifier, m_onGain) == 0x74);
	static_assert(offsetof(BSDirectAtModifier, m_offGain) == 0x78);
	static_assert(offsetof(BSDirectAtModifier, m_onLeadIn) == 0x7C);
	static_assert(offsetof(BSDirectAtModifier, m_offLeadIn) == 0x80);
	static_assert(offsetof(BSDirectAtModifier, m_initialHeadingOffset) == 0x84);
	static_assert(offsetof(BSDirectAtModifier, m_initialPitchOffset) == 0x88);
	static_assert(offsetof(BSDirectAtModifier, m_initialGain) == 0x8C);
	static_assert(offsetof(BSDirectAtModifier, m_allowSourceBonePoseCapture) == 0x90);
	static_assert(offsetof(BSDirectAtModifier, m_sourceBonePoseCaptureBlendGain) == 0x94);
	static_assert(offsetof(BSDirectAtModifier, m_useCurrentSourceBonePoseEventId) == 0x98);
	static_assert(offsetof(BSDirectAtModifier, m_counteractSourceBoneWorldSpaceDelta) == 0x9C);
	static_assert(offsetof(BSDirectAtModifier, m_snapToTargetEventId) == 0xA0);
	static_assert(offsetof(BSDirectAtModifier, m_directAtCameraX) == 0xA4);
	static_assert(offsetof(BSDirectAtModifier, m_directAtCameraY) == 0xA8);
	static_assert(offsetof(BSDirectAtModifier, m_directAtCameraZ) == 0xAC);
	static_assert(offsetof(BSDirectAtModifier, m_drawDebugInfo) == 0xB0);
	static_assert(offsetof(BSDirectAtModifier, m_active) == 0xB1);
	static_assert(offsetof(BSDirectAtModifier, m_currentHeadingOffset) == 0xB4);
	static_assert(offsetof(BSDirectAtModifier, m_currentPitchOffset) == 0xB8);
	static_assert(offsetof(BSDirectAtModifier, m_currentGain) == 0xBC);
	static_assert(offsetof(BSDirectAtModifier, m_firstModify) == 0xC0);
	static_assert(offsetof(BSDirectAtModifier, m_headingOffset) == 0xC4);
	static_assert(offsetof(BSDirectAtModifier, m_pitchOffset) == 0xC8);
	static_assert(offsetof(BSDirectAtModifier, m_buseSourceBonePoseCapture) == 0xCC);
	static_assert(offsetof(BSDirectAtModifier, m_sourceBonePoseGain) == 0xD0);
	static_assert(offsetof(BSDirectAtModifier, m_sourceBonePoseCapture) == 0xE0);
	static_assert(offsetof(BSDirectAtModifier, m_sourceBonePose) == 0x110);
	static_assert(offsetof(BSDirectAtModifier, m_sourceBoneDirWS) == 0x140);
	static_assert(offsetof(BSDirectAtModifier, m_snapToTarget) == 0x150);
	static_assert(offsetof(BSDirectAtModifier, m_timeStep) == 0x154);
	static_assert(offsetof(BSDirectAtModifier, m_directAtTarget) == 0x158);
	static_assert(offsetof(BSDirectAtModifier, m_targetLocation) == 0x160);
	static_assert(offsetof(BSDirectAtModifier, m_worldTransform) == 0x170);
	static_assert(offsetof(BSDirectAtModifier, m_boneChainIndices) == 0x1A0);

	class BSDistTriggerModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkVector4f m_targetPosition;
		/* 0x070 */ float m_distance;
		/* 0x074 */ float m_distanceTrigger;
		/* 0x078 */ hkbEventProperty m_triggerEvent;
		/* 0x088 */ std::uint8_t pad088[0x8];
	};
	static_assert(sizeof(BSDistTriggerModifier) == 0x90);
	static_assert(offsetof(BSDistTriggerModifier, m_targetPosition) == 0x60);
	static_assert(offsetof(BSDistTriggerModifier, m_distance) == 0x70);
	static_assert(offsetof(BSDistTriggerModifier, m_distanceTrigger) == 0x74);
	static_assert(offsetof(BSDistTriggerModifier, m_triggerEvent) == 0x78);

	class BSEventEveryNEventsModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkbEventProperty m_eventToCheckFor;
		/* 0x068 */ hkbEventProperty m_eventToSend;
		/* 0x078 */ std::int8_t m_numberOfEventsBeforeSend;
		/* 0x079 */ std::int8_t m_minimumNumberOfEventsBeforeSend;
		/* 0x07A */ hkBool m_randomizeNumberOfEvents;
		/* 0x07B */ std::uint8_t pad07B[0x1];
		/* 0x07C */ std::int32_t m_numberOfEventsSeen;
		/* 0x080 */ std::int8_t m_calculatedNumberOfEventsBeforeSend;
		/* 0x081 */ std::uint8_t pad081[0x7];
	};
	static_assert(sizeof(BSEventEveryNEventsModifier) == 0x88);
	static_assert(offsetof(BSEventEveryNEventsModifier, m_eventToCheckFor) == 0x58);
	static_assert(offsetof(BSEventEveryNEventsModifier, m_eventToSend) == 0x68);
	static_assert(offsetof(BSEventEveryNEventsModifier, m_numberOfEventsBeforeSend) == 0x78);
	static_assert(offsetof(BSEventEveryNEventsModifier, m_minimumNumberOfEventsBeforeSend) == 0x79);
	static_assert(offsetof(BSEventEveryNEventsModifier, m_randomizeNumberOfEvents) == 0x7A);
	static_assert(offsetof(BSEventEveryNEventsModifier, m_numberOfEventsSeen) == 0x7C);
	static_assert(offsetof(BSEventEveryNEventsModifier, m_calculatedNumberOfEventsBeforeSend) == 0x80);

	class BSEventOnDeactivateModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkbEventProperty m_event;
	};
	static_assert(sizeof(BSEventOnDeactivateModifier) == 0x68);
	static_assert(offsetof(BSEventOnDeactivateModifier, m_event) == 0x58);

	class BSEventOnFalseToTrueModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkBool m_bEnableEvent1;
		/* 0x059 */ hkBool m_bVariableToTest1;
		/* 0x05A */ std::uint8_t pad05A[0x6];
		/* 0x060 */ hkbEventProperty m_EventToSend1;
		/* 0x070 */ hkBool m_bEnableEvent2;
		/* 0x071 */ hkBool m_bVariableToTest2;
		/* 0x072 */ std::uint8_t pad072[0x6];
		/* 0x078 */ hkbEventProperty m_EventToSend2;
		/* 0x088 */ hkBool m_bEnableEvent3;
		/* 0x089 */ hkBool m_bVariableToTest3;
		/* 0x08A */ std::uint8_t pad08A[0x6];
		/* 0x090 */ hkbEventProperty m_EventToSend3;
		/* 0x0A0 */ hkBool m_bSlot1ActivatedLastFrame;
		/* 0x0A1 */ hkBool m_bSlot2ActivatedLastFrame;
		/* 0x0A2 */ hkBool m_bSlot3ActivatedLastFrame;
		/* 0x0A3 */ std::uint8_t pad0A3[0x5];
	};
	static_assert(sizeof(BSEventOnFalseToTrueModifier) == 0xA8);
	static_assert(offsetof(BSEventOnFalseToTrueModifier, m_bEnableEvent1) == 0x58);
	static_assert(offsetof(BSEventOnFalseToTrueModifier, m_bVariableToTest1) == 0x59);
	static_assert(offsetof(BSEventOnFalseToTrueModifier, m_EventToSend1) == 0x60);
	static_assert(offsetof(BSEventOnFalseToTrueModifier, m_bEnableEvent2) == 0x70);
	static_assert(offsetof(BSEventOnFalseToTrueModifier, m_bVariableToTest2) == 0x71);
	static_assert(offsetof(BSEventOnFalseToTrueModifier, m_EventToSend2) == 0x78);
	static_assert(offsetof(BSEventOnFalseToTrueModifier, m_bEnableEvent3) == 0x88);
	static_assert(offsetof(BSEventOnFalseToTrueModifier, m_bVariableToTest3) == 0x89);
	static_assert(offsetof(BSEventOnFalseToTrueModifier, m_EventToSend3) == 0x90);
	static_assert(offsetof(BSEventOnFalseToTrueModifier, m_bSlot1ActivatedLastFrame) == 0xA0);
	static_assert(offsetof(BSEventOnFalseToTrueModifier, m_bSlot2ActivatedLastFrame) == 0xA1);
	static_assert(offsetof(BSEventOnFalseToTrueModifier, m_bSlot3ActivatedLastFrame) == 0xA2);

	class BSGetTimeStepModifier : public hkbModifier
	{
	public:
		/* 0x058 */ float m_timeStep;
		/* 0x05C */ std::uint8_t pad05C[0x4];
	};
	static_assert(sizeof(BSGetTimeStepModifier) == 0x60);
	static_assert(offsetof(BSGetTimeStepModifier, m_timeStep) == 0x58);

	class BSIStateManagerModifierBSIStateManagerStateListener : public hkbStateListener
	{
	public:
		/* 0x010 */ void* m_pStateManager;
	};
	static_assert(sizeof(BSIStateManagerModifierBSIStateManagerStateListener) == 0x18);
	static_assert(offsetof(BSIStateManagerModifierBSIStateManagerStateListener, m_pStateManager) == 0x10);

	class BSIStateManagerModifier : public hkbModifier
	{
	public:
		using BSIStateManagerStateListener = BSIStateManagerModifierBSIStateManagerStateListener;
		/* 0x058 */ std::int32_t m_iStateVar;
		/* 0x05C */ std::uint8_t pad05C[0x4];
		/* 0x060 */ hkArray<BSIStateManagerModifierBSiStateData> m_stateData;
		/* 0x070 */ BSIStateManagerModifierBSIStateManagerStateListener m_myStateListener;
	};
	static_assert(sizeof(BSIStateManagerModifier) == 0x88);
	static_assert(offsetof(BSIStateManagerModifier, m_iStateVar) == 0x58);
	static_assert(offsetof(BSIStateManagerModifier, m_stateData) == 0x60);
	static_assert(offsetof(BSIStateManagerModifier, m_myStateListener) == 0x70);

	class BSInterpValueModifier : public hkbModifier
	{
	public:
		/* 0x058 */ float m_source;
		/* 0x05C */ float m_target;
		/* 0x060 */ float m_result;
		/* 0x064 */ float m_gain;
		/* 0x068 */ float m_timeStep;
		/* 0x06C */ std::uint8_t pad06C[0x4];
	};
	static_assert(sizeof(BSInterpValueModifier) == 0x70);
	static_assert(offsetof(BSInterpValueModifier, m_source) == 0x58);
	static_assert(offsetof(BSInterpValueModifier, m_target) == 0x5C);
	static_assert(offsetof(BSInterpValueModifier, m_result) == 0x60);
	static_assert(offsetof(BSInterpValueModifier, m_gain) == 0x64);
	static_assert(offsetof(BSInterpValueModifier, m_timeStep) == 0x68);

	class BSIsActiveModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkBool m_bIsActive0;
		/* 0x059 */ hkBool m_bInvertActive0;
		/* 0x05A */ hkBool m_bIsActive1;
		/* 0x05B */ hkBool m_bInvertActive1;
		/* 0x05C */ hkBool m_bIsActive2;
		/* 0x05D */ hkBool m_bInvertActive2;
		/* 0x05E */ hkBool m_bIsActive3;
		/* 0x05F */ hkBool m_bInvertActive3;
		/* 0x060 */ hkBool m_bIsActive4;
		/* 0x061 */ hkBool m_bInvertActive4;
		/* 0x062 */ std::uint8_t pad062[0x6];
	};
	static_assert(sizeof(BSIsActiveModifier) == 0x68);
	static_assert(offsetof(BSIsActiveModifier, m_bIsActive0) == 0x58);
	static_assert(offsetof(BSIsActiveModifier, m_bInvertActive0) == 0x59);
	static_assert(offsetof(BSIsActiveModifier, m_bIsActive1) == 0x5A);
	static_assert(offsetof(BSIsActiveModifier, m_bInvertActive1) == 0x5B);
	static_assert(offsetof(BSIsActiveModifier, m_bIsActive2) == 0x5C);
	static_assert(offsetof(BSIsActiveModifier, m_bInvertActive2) == 0x5D);
	static_assert(offsetof(BSIsActiveModifier, m_bIsActive3) == 0x5E);
	static_assert(offsetof(BSIsActiveModifier, m_bInvertActive3) == 0x5F);
	static_assert(offsetof(BSIsActiveModifier, m_bIsActive4) == 0x60);
	static_assert(offsetof(BSIsActiveModifier, m_bInvertActive4) == 0x61);

	class BSLimbCycleModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkbBoneIndexArray* m_limbBoneIndexA;
		/* 0x060 */ hkbEventProperty m_EventCycleLeft;
		/* 0x070 */ hkbEventProperty m_EventCycleRight;
		/* 0x080 */ hkbEventProperty m_EventCycleReset;
		/* 0x090 */ hkArray<void> m_boneChainIndexA;
		/* 0x0A0 */ std::int16_t m_iFrontIndex;
		/* 0x0A2 */ std::uint8_t pad0A2[0x6];
	};
	static_assert(sizeof(BSLimbCycleModifier) == 0xA8);
	static_assert(offsetof(BSLimbCycleModifier, m_limbBoneIndexA) == 0x58);
	static_assert(offsetof(BSLimbCycleModifier, m_EventCycleLeft) == 0x60);
	static_assert(offsetof(BSLimbCycleModifier, m_EventCycleRight) == 0x70);
	static_assert(offsetof(BSLimbCycleModifier, m_EventCycleReset) == 0x80);
	static_assert(offsetof(BSLimbCycleModifier, m_boneChainIndexA) == 0x90);
	static_assert(offsetof(BSLimbCycleModifier, m_iFrontIndex) == 0xA0);

	class BSLimbIKModifier : public hkbModifier
	{
	public:
		/* 0x058 */ float m_limitAngleDegrees;
		/* 0x05C */ float m_currentAngle;
		/* 0x060 */ std::int16_t m_startBoneIndex;
		/* 0x062 */ std::int16_t m_endBoneIndex;
		/* 0x064 */ float m_gain;
		/* 0x068 */ float m_boneRadius;
		/* 0x06C */ float m_castOffset;
		/* 0x070 */ float m_timeStep;
		/* 0x074 */ std::uint8_t pad074[0x4];
		/* 0x078 */ void* m_pSkeletonMemory;
	};
	static_assert(sizeof(BSLimbIKModifier) == 0x80);
	static_assert(offsetof(BSLimbIKModifier, m_limitAngleDegrees) == 0x58);
	static_assert(offsetof(BSLimbIKModifier, m_currentAngle) == 0x5C);
	static_assert(offsetof(BSLimbIKModifier, m_startBoneIndex) == 0x60);
	static_assert(offsetof(BSLimbIKModifier, m_endBoneIndex) == 0x62);
	static_assert(offsetof(BSLimbIKModifier, m_gain) == 0x64);
	static_assert(offsetof(BSLimbIKModifier, m_boneRadius) == 0x68);
	static_assert(offsetof(BSLimbIKModifier, m_castOffset) == 0x6C);
	static_assert(offsetof(BSLimbIKModifier, m_timeStep) == 0x70);
	static_assert(offsetof(BSLimbIKModifier, m_pSkeletonMemory) == 0x78);

	class BSLookAtCapturePoseModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::int32_t m_capturePoseEventId;
		/* 0x05C */ hkBool m_capturePoseOnActivate;
		/* 0x05D */ hkBool m_capturePose;
		/* 0x05E */ std::uint8_t pad05E[0x2];
	};
	static_assert(sizeof(BSLookAtCapturePoseModifier) == 0x60);
	static_assert(offsetof(BSLookAtCapturePoseModifier, m_capturePoseEventId) == 0x58);
	static_assert(offsetof(BSLookAtCapturePoseModifier, m_capturePoseOnActivate) == 0x5C);
	static_assert(offsetof(BSLookAtCapturePoseModifier, m_capturePose) == 0x5D);

	class BSLookAtModifierBoneData
	{
	public:
		enum class AXIS_COLOR : std::int32_t
		{
			Red = 0,
			Green = 1,
			Blue = 2,
		};

		/* 0x000 */ std::int16_t m_index;
		/* 0x002 */ std::uint8_t pad002[0xE];
		/* 0x010 */ hkVector4f m_fwdAxisLS;
		/* 0x020 */ hkVector4f m_upAxisLS;
		/* 0x030 */ hkBool m_specifyAxesByColor;
		/* 0x031 */ std::int8_t m_forwardAxisColor;
		/* 0x032 */ std::int8_t m_upAxisColor;
		/* 0x033 */ std::uint8_t pad033[0x1];
		/* 0x034 */ float m_limitAngleDegrees;
		/* 0x038 */ float m_limitAngleDegreesLeft;
		/* 0x03C */ float m_limitAngleDegreesRight;
		/* 0x040 */ float m_limitAngleDegreesUp;
		/* 0x044 */ float m_limitAngleDegreesDown;
		/* 0x048 */ float m_onGain;
		/* 0x04C */ float m_offGain;
		/* 0x050 */ float m_onLeadIn;
		/* 0x054 */ float m_offLeadIn;
		/* 0x058 */ hkBool m_rotateAboutAxes;
		/* 0x059 */ hkBool m_enabled;
		/* 0x05A */ hkBool m_drawDebugInfo;
		/* 0x05B */ std::uint8_t pad05B[0x5];
		/* 0x060 */ hkVector4f m_axisFwdLS;
		/* 0x070 */ hkVector4f m_axisUpLS;
		/* 0x080 */ hkVector4f m_axisSideLS;
		/* 0x090 */ hkVector4f m_currentFwdAxisLS;
		/* 0x0A0 */ hkQuaternionf m_currentRotMS;
		/* 0x0B0 */ float m_currentGain;
		/* 0x0B4 */ std::uint8_t pad0B4[0xC];
		/* 0x0C0 */ hkQsTransformf m_defaultRefTransformMS;
		/* 0x0F0 */ hkQsTransformf m_defaultRefTransformLS;
		/* 0x120 */ float m_defaultOffsetFwd;
		/* 0x124 */ float m_defaultOffsetVert;
		/* 0x128 */ std::uint8_t pad128[0x8];
		/* 0x130 */ hkQsTransformf m_customRefTransformMS;
		/* 0x160 */ hkQsTransformf m_customRefTransformLS;
		/* 0x190 */ float m_customOffsetFwd;
		/* 0x194 */ float m_customOffsetVert;
		/* 0x198 */ std::uint8_t pad198[0x8];
		/* 0x1A0 */ hkQsTransformf m_currentRefTransformMS;
		/* 0x1D0 */ hkQsTransformf m_currentRefTransformLS;
		/* 0x200 */ float m_currentOffsetFwd;
		/* 0x204 */ float m_currentOffsetVert;
		/* 0x208 */ std::uint8_t pad208[0x8];
	};
	static_assert(sizeof(BSLookAtModifierBoneData) == 0x210);
	static_assert(offsetof(BSLookAtModifierBoneData, m_index) == 0x0);
	static_assert(offsetof(BSLookAtModifierBoneData, m_fwdAxisLS) == 0x10);
	static_assert(offsetof(BSLookAtModifierBoneData, m_upAxisLS) == 0x20);
	static_assert(offsetof(BSLookAtModifierBoneData, m_specifyAxesByColor) == 0x30);
	static_assert(offsetof(BSLookAtModifierBoneData, m_forwardAxisColor) == 0x31);
	static_assert(offsetof(BSLookAtModifierBoneData, m_upAxisColor) == 0x32);
	static_assert(offsetof(BSLookAtModifierBoneData, m_limitAngleDegrees) == 0x34);
	static_assert(offsetof(BSLookAtModifierBoneData, m_limitAngleDegreesLeft) == 0x38);
	static_assert(offsetof(BSLookAtModifierBoneData, m_limitAngleDegreesRight) == 0x3C);
	static_assert(offsetof(BSLookAtModifierBoneData, m_limitAngleDegreesUp) == 0x40);
	static_assert(offsetof(BSLookAtModifierBoneData, m_limitAngleDegreesDown) == 0x44);
	static_assert(offsetof(BSLookAtModifierBoneData, m_onGain) == 0x48);
	static_assert(offsetof(BSLookAtModifierBoneData, m_offGain) == 0x4C);
	static_assert(offsetof(BSLookAtModifierBoneData, m_onLeadIn) == 0x50);
	static_assert(offsetof(BSLookAtModifierBoneData, m_offLeadIn) == 0x54);
	static_assert(offsetof(BSLookAtModifierBoneData, m_rotateAboutAxes) == 0x58);
	static_assert(offsetof(BSLookAtModifierBoneData, m_enabled) == 0x59);
	static_assert(offsetof(BSLookAtModifierBoneData, m_drawDebugInfo) == 0x5A);
	static_assert(offsetof(BSLookAtModifierBoneData, m_axisFwdLS) == 0x60);
	static_assert(offsetof(BSLookAtModifierBoneData, m_axisUpLS) == 0x70);
	static_assert(offsetof(BSLookAtModifierBoneData, m_axisSideLS) == 0x80);
	static_assert(offsetof(BSLookAtModifierBoneData, m_currentFwdAxisLS) == 0x90);
	static_assert(offsetof(BSLookAtModifierBoneData, m_currentRotMS) == 0xA0);
	static_assert(offsetof(BSLookAtModifierBoneData, m_currentGain) == 0xB0);
	static_assert(offsetof(BSLookAtModifierBoneData, m_defaultRefTransformMS) == 0xC0);
	static_assert(offsetof(BSLookAtModifierBoneData, m_defaultRefTransformLS) == 0xF0);
	static_assert(offsetof(BSLookAtModifierBoneData, m_defaultOffsetFwd) == 0x120);
	static_assert(offsetof(BSLookAtModifierBoneData, m_defaultOffsetVert) == 0x124);
	static_assert(offsetof(BSLookAtModifierBoneData, m_customRefTransformMS) == 0x130);
	static_assert(offsetof(BSLookAtModifierBoneData, m_customRefTransformLS) == 0x160);
	static_assert(offsetof(BSLookAtModifierBoneData, m_customOffsetFwd) == 0x190);
	static_assert(offsetof(BSLookAtModifierBoneData, m_customOffsetVert) == 0x194);
	static_assert(offsetof(BSLookAtModifierBoneData, m_currentRefTransformMS) == 0x1A0);
	static_assert(offsetof(BSLookAtModifierBoneData, m_currentRefTransformLS) == 0x1D0);
	static_assert(offsetof(BSLookAtModifierBoneData, m_currentOffsetFwd) == 0x200);
	static_assert(offsetof(BSLookAtModifierBoneData, m_currentOffsetVert) == 0x204);

	class BSLookAtModifier : public hkbModifier
	{
	public:
		enum class TARGET : std::int32_t
		{
			Default = 0,
			AppSupplied = 1,
			Camera = 2,
		};

		enum class CHARACTER_TYPE : std::int32_t
		{
			Generic = 0,
			Biped = 1,
		};

		/* 0x058 */ hkBool m_additive;
		/* 0x059 */ hkBool m_clampAdditive;
		/* 0x05A */ std::int8_t m_target;
		/* 0x05B */ hkBool m_HeadTrackingOn1;
		/* 0x05C */ hkBool m_HeadTrackingOn2;
		/* 0x05D */ std::int8_t m_characterType;
		/* 0x05E */ std::uint8_t pad05E[0x2];
		/* 0x060 */ BSLookAtModifierBoneData m_rootBone;
		/* 0x270 */ hkArray<BSLookAtModifierBoneData> m_bones;
		/* 0x280 */ hkArray<BSLookAtModifierBoneData> m_eyeBones;
		/* 0x290 */ hkBool m_useIndividualLimitAngles;
		/* 0x291 */ std::uint8_t pad291[0x3];
		/* 0x294 */ float m_limitAngleThresholdDegrees;
		/* 0x298 */ hkBool m_continueLookOutsideOfLimit;
		/* 0x299 */ std::uint8_t pad299[0x3];
		/* 0x29C */ float m_onGain;
		/* 0x2A0 */ float m_offGain;
		/* 0x2A4 */ float m_onLeadIn;
		/* 0x2A8 */ float m_offLeadIn;
		/* 0x2AC */ hkBool m_useBoneGains;
		/* 0x2AD */ std::uint8_t pad2AD[0x3];
		/* 0x2B0 */ float m_refPoseBlendGain;
		/* 0x2B4 */ std::int32_t m_restoreDefaultRefPosEventId;
		/* 0x2B8 */ std::uint8_t pad2B8[0x8];
		/* 0x2C0 */ hkVector4f m_targetLocation;
		/* 0x2D0 */ hkBool m_targetOutsideLimits;
		/* 0x2D1 */ std::uint8_t pad2D1[0x7];
		/* 0x2D8 */ hkbEventProperty m_targetOutOfLimitEvent;
		/* 0x2E8 */ std::int32_t m_snapToTargetEventId;
		/* 0x2EC */ hkBool m_lookActive;
		/* 0x2ED */ hkBool m_modificationActive;
		/* 0x2EE */ std::uint8_t pad2EE[0x2];
		/* 0x2F0 */ float m_lookAtCameraX;
		/* 0x2F4 */ float m_lookAtCameraY;
		/* 0x2F8 */ float m_lookAtCameraZ;
		/* 0x2FC */ hkBool m_drawDebugInfo;
		/* 0x2FD */ hkBool m_prevLookAtTarget;
		/* 0x2FE */ hkBool m_targetOutsideLimitsPrev;
		/* 0x2FF */ hkBool m_buseCustomRefPose;
		/* 0x300 */ float m_refPoseGain;
		/* 0x304 */ hkBool m_snapToTarget;
		/* 0x305 */ std::uint8_t pad305[0x3];
		/* 0x308 */ float m_timeStep;
		/* 0x30C */ hkBool m_curLookAtTarget;
		/* 0x30D */ std::uint8_t pad30D[0x3];
		/* 0x310 */ hkVector4f m_curTargetLocation;
		/* 0x320 */ hkBool m_ballBonesValid;
		/* 0x321 */ std::uint8_t pad321[0xF];
	};
	static_assert(sizeof(BSLookAtModifier) == 0x330);
	static_assert(offsetof(BSLookAtModifier, m_additive) == 0x58);
	static_assert(offsetof(BSLookAtModifier, m_clampAdditive) == 0x59);
	static_assert(offsetof(BSLookAtModifier, m_target) == 0x5A);
	static_assert(offsetof(BSLookAtModifier, m_HeadTrackingOn1) == 0x5B);
	static_assert(offsetof(BSLookAtModifier, m_HeadTrackingOn2) == 0x5C);
	static_assert(offsetof(BSLookAtModifier, m_characterType) == 0x5D);
	static_assert(offsetof(BSLookAtModifier, m_rootBone) == 0x60);
	static_assert(offsetof(BSLookAtModifier, m_bones) == 0x270);
	static_assert(offsetof(BSLookAtModifier, m_eyeBones) == 0x280);
	static_assert(offsetof(BSLookAtModifier, m_useIndividualLimitAngles) == 0x290);
	static_assert(offsetof(BSLookAtModifier, m_limitAngleThresholdDegrees) == 0x294);
	static_assert(offsetof(BSLookAtModifier, m_continueLookOutsideOfLimit) == 0x298);
	static_assert(offsetof(BSLookAtModifier, m_onGain) == 0x29C);
	static_assert(offsetof(BSLookAtModifier, m_offGain) == 0x2A0);
	static_assert(offsetof(BSLookAtModifier, m_onLeadIn) == 0x2A4);
	static_assert(offsetof(BSLookAtModifier, m_offLeadIn) == 0x2A8);
	static_assert(offsetof(BSLookAtModifier, m_useBoneGains) == 0x2AC);
	static_assert(offsetof(BSLookAtModifier, m_refPoseBlendGain) == 0x2B0);
	static_assert(offsetof(BSLookAtModifier, m_restoreDefaultRefPosEventId) == 0x2B4);
	static_assert(offsetof(BSLookAtModifier, m_targetLocation) == 0x2C0);
	static_assert(offsetof(BSLookAtModifier, m_targetOutsideLimits) == 0x2D0);
	static_assert(offsetof(BSLookAtModifier, m_targetOutOfLimitEvent) == 0x2D8);
	static_assert(offsetof(BSLookAtModifier, m_snapToTargetEventId) == 0x2E8);
	static_assert(offsetof(BSLookAtModifier, m_lookActive) == 0x2EC);
	static_assert(offsetof(BSLookAtModifier, m_modificationActive) == 0x2ED);
	static_assert(offsetof(BSLookAtModifier, m_lookAtCameraX) == 0x2F0);
	static_assert(offsetof(BSLookAtModifier, m_lookAtCameraY) == 0x2F4);
	static_assert(offsetof(BSLookAtModifier, m_lookAtCameraZ) == 0x2F8);
	static_assert(offsetof(BSLookAtModifier, m_drawDebugInfo) == 0x2FC);
	static_assert(offsetof(BSLookAtModifier, m_prevLookAtTarget) == 0x2FD);
	static_assert(offsetof(BSLookAtModifier, m_targetOutsideLimitsPrev) == 0x2FE);
	static_assert(offsetof(BSLookAtModifier, m_buseCustomRefPose) == 0x2FF);
	static_assert(offsetof(BSLookAtModifier, m_refPoseGain) == 0x300);
	static_assert(offsetof(BSLookAtModifier, m_snapToTarget) == 0x304);
	static_assert(offsetof(BSLookAtModifier, m_timeStep) == 0x308);
	static_assert(offsetof(BSLookAtModifier, m_curLookAtTarget) == 0x30C);
	static_assert(offsetof(BSLookAtModifier, m_curTargetLocation) == 0x310);
	static_assert(offsetof(BSLookAtModifier, m_ballBonesValid) == 0x320);

	class BSModifyOnceModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkbModifier* m_pOnActivateModifier;
		/* 0x068 */ std::uint8_t pad068[0x8];
		/* 0x070 */ hkbModifier* m_pOnDeactivateModifier;
		/* 0x078 */ std::uint8_t pad078[0x8];
	};
	static_assert(sizeof(BSModifyOnceModifier) == 0x80);
	static_assert(offsetof(BSModifyOnceModifier, m_pOnActivateModifier) == 0x60);
	static_assert(offsetof(BSModifyOnceModifier, m_pOnDeactivateModifier) == 0x70);

	class BSOffsetAnimationGenerator : public hkbGenerator
	{
	public:
		/* 0x088 */ std::uint8_t pad088[0x8];
		/* 0x090 */ hkbGenerator* m_pDefaultGenerator;
		/* 0x098 */ std::uint8_t pad098[0x8];
		/* 0x0A0 */ hkbGenerator* m_pOffsetClipGenerator;
		/* 0x0A8 */ float m_fOffsetVariable;
		/* 0x0AC */ float m_fOffsetRangeStart;
		/* 0x0B0 */ float m_fOffsetRangeEnd;
		/* 0x0B4 */ std::uint8_t pad0B4[0x4];
		/* 0x0B8 */ hkArray<void> m_BoneOffsetA;
		/* 0x0C8 */ hkArray<void> m_BoneIndexA;
		/* 0x0D8 */ float m_fCurrentPercentage;
		/* 0x0DC */ std::uint32_t m_iCurrentFrame;
		/* 0x0E0 */ hkBool m_bZeroOffset;
		/* 0x0E1 */ hkBool m_bOffsetValid;
		/* 0x0E2 */ std::uint8_t pad0E2[0xE];
	};
	static_assert(sizeof(BSOffsetAnimationGenerator) == 0xF0);
	static_assert(offsetof(BSOffsetAnimationGenerator, m_pDefaultGenerator) == 0x90);
	static_assert(offsetof(BSOffsetAnimationGenerator, m_pOffsetClipGenerator) == 0xA0);
	static_assert(offsetof(BSOffsetAnimationGenerator, m_fOffsetVariable) == 0xA8);
	static_assert(offsetof(BSOffsetAnimationGenerator, m_fOffsetRangeStart) == 0xAC);
	static_assert(offsetof(BSOffsetAnimationGenerator, m_fOffsetRangeEnd) == 0xB0);
	static_assert(offsetof(BSOffsetAnimationGenerator, m_BoneOffsetA) == 0xB8);
	static_assert(offsetof(BSOffsetAnimationGenerator, m_BoneIndexA) == 0xC8);
	static_assert(offsetof(BSOffsetAnimationGenerator, m_fCurrentPercentage) == 0xD8);
	static_assert(offsetof(BSOffsetAnimationGenerator, m_iCurrentFrame) == 0xDC);
	static_assert(offsetof(BSOffsetAnimationGenerator, m_bZeroOffset) == 0xE0);
	static_assert(offsetof(BSOffsetAnimationGenerator, m_bOffsetValid) == 0xE1);

	class BSPassByTargetTriggerModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::uint8_t pad058[0x8];
		/* 0x060 */ hkVector4f m_targetPosition;
		/* 0x070 */ float m_radius;
		/* 0x074 */ std::uint8_t pad074[0xC];
		/* 0x080 */ hkVector4f m_movementDirection;
		/* 0x090 */ hkbEventProperty m_triggerEvent;
		/* 0x0A0 */ hkBool m_targetPassed;
		/* 0x0A1 */ std::uint8_t pad0A1[0xF];
	};
	static_assert(sizeof(BSPassByTargetTriggerModifier) == 0xB0);
	static_assert(offsetof(BSPassByTargetTriggerModifier, m_targetPosition) == 0x60);
	static_assert(offsetof(BSPassByTargetTriggerModifier, m_radius) == 0x70);
	static_assert(offsetof(BSPassByTargetTriggerModifier, m_movementDirection) == 0x80);
	static_assert(offsetof(BSPassByTargetTriggerModifier, m_triggerEvent) == 0x90);
	static_assert(offsetof(BSPassByTargetTriggerModifier, m_targetPassed) == 0xA0);

	class BSRagdollContactListenerModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkbEventProperty m_contactEvent;
		/* 0x068 */ hkbBoneIndexArray* m_bones;
		/* 0x070 */ hkBool m_throwEvent;
		/* 0x071 */ std::uint8_t pad071[0x7];
	};
	static_assert(sizeof(BSRagdollContactListenerModifier) == 0x78);
	static_assert(offsetof(BSRagdollContactListenerModifier, m_contactEvent) == 0x58);
	static_assert(offsetof(BSRagdollContactListenerModifier, m_bones) == 0x68);
	static_assert(offsetof(BSRagdollContactListenerModifier, m_throwEvent) == 0x70);

	class BSRandomAlarmModifier : public hkbModifier
	{
	public:
		/* 0x058 */ float m_alarmTimeSecondsMin;
		/* 0x05C */ float m_alarmTimeSecondsMax;
		/* 0x060 */ hkbEventProperty m_alarmEvent;
		/* 0x070 */ hkBool m_resetAlarm;
		/* 0x071 */ std::uint8_t pad071[0x3];
		/* 0x074 */ float m_secondsRemaining;
	};
	static_assert(sizeof(BSRandomAlarmModifier) == 0x78);
	static_assert(offsetof(BSRandomAlarmModifier, m_alarmTimeSecondsMin) == 0x58);
	static_assert(offsetof(BSRandomAlarmModifier, m_alarmTimeSecondsMax) == 0x5C);
	static_assert(offsetof(BSRandomAlarmModifier, m_alarmEvent) == 0x60);
	static_assert(offsetof(BSRandomAlarmModifier, m_resetAlarm) == 0x70);
	static_assert(offsetof(BSRandomAlarmModifier, m_secondsRemaining) == 0x74);

	class BSRetargetBodyBlend : public hkbGenerator
	{
	public:
		/* 0x088 */ std::uint8_t pad088[0x8];
		/* 0x090 */ hkbGenerator* m_pDefaultGenerator;
		/* 0x098 */ std::uint8_t pad098[0x8];
		/* 0x0A0 */ hkbGenerator* m_pTargetGenerator;
		/* 0x0A8 */ std::uint8_t pad0A8[0x8];
	};
	static_assert(sizeof(BSRetargetBodyBlend) == 0xB0);
	static_assert(offsetof(BSRetargetBodyBlend, m_pDefaultGenerator) == 0x90);
	static_assert(offsetof(BSRetargetBodyBlend, m_pTargetGenerator) == 0xA0);

	class BSSpringState
	{
	public:
		/* 0x000 */ float m_SpringConstant;
		/* 0x004 */ float m_IN_ValueToSmooth;
		/* 0x008 */ float m_Current;
		/* 0x00C */ float m_Equilibrium;
		/* 0x010 */ float m_Velocity;
	};
	static_assert(sizeof(BSSpringState) == 0x14);
	static_assert(offsetof(BSSpringState, m_SpringConstant) == 0x0);
	static_assert(offsetof(BSSpringState, m_IN_ValueToSmooth) == 0x4);
	static_assert(offsetof(BSSpringState, m_Current) == 0x8);
	static_assert(offsetof(BSSpringState, m_Equilibrium) == 0xC);
	static_assert(offsetof(BSSpringState, m_Velocity) == 0x10);

	class BSReverseSpineTwistModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkBool m_isAdditive;
		/* 0x059 */ std::uint8_t pad059[0x1];
		/* 0x05A */ std::int16_t m_COMIndex;
		/* 0x05C */ std::int16_t m_Bone0Index;
		/* 0x05E */ std::uint8_t pad05E[0x2];
		/* 0x060 */ hkVector4f m_Bone0Translate;
		/* 0x070 */ hkQuaternionf m_Bone0Rotate;
		/* 0x080 */ std::int16_t m_Bone1Index;
		/* 0x082 */ std::uint8_t pad082[0xE];
		/* 0x090 */ hkVector4f m_Bone1Translate;
		/* 0x0A0 */ hkQuaternionf m_Bone1Rotate;
		/* 0x0B0 */ std::int16_t m_Bone2Index;
		/* 0x0B2 */ std::uint8_t pad0B2[0x2];
		/* 0x0B4 */ float m_RunSpeed;
		/* 0x0B8 */ float m_WalkSpeed;
		/* 0x0BC */ float m_MaxZRotationRun;
		/* 0x0C0 */ float m_MaxZRotationWalk;
		/* 0x0C4 */ float m_MaxTurnDeltaRun;
		/* 0x0C8 */ float m_MaxTurnDeltaWalk;
		/* 0x0CC */ float m_IN_Speed;
		/* 0x0D0 */ BSSpringState m_SpringZ;
		/* 0x0E4 */ std::uint8_t pad0E4[0x4];
		/* 0x0E8 */ hkArray<void> m_parentOfBone0ToCOMIndices;
		/* 0x0F8 */ std::uint8_t pad0F8[0x8];
	};
	static_assert(sizeof(BSReverseSpineTwistModifier) == 0x100);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_isAdditive) == 0x58);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_COMIndex) == 0x5A);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_Bone0Index) == 0x5C);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_Bone0Translate) == 0x60);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_Bone0Rotate) == 0x70);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_Bone1Index) == 0x80);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_Bone1Translate) == 0x90);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_Bone1Rotate) == 0xA0);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_Bone2Index) == 0xB0);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_RunSpeed) == 0xB4);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_WalkSpeed) == 0xB8);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_MaxZRotationRun) == 0xBC);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_MaxZRotationWalk) == 0xC0);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_MaxTurnDeltaRun) == 0xC4);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_MaxTurnDeltaWalk) == 0xC8);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_IN_Speed) == 0xCC);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_SpringZ) == 0xD0);
	static_assert(offsetof(BSReverseSpineTwistModifier, m_parentOfBone0ToCOMIndices) == 0xE8);

	class BSRootTwistModifier : public hkbModifier
	{
	public:
		enum class SetTwistMethod : std::int32_t
		{
			ANGLE_DEGREES = 0,
			DIRECTION = 1,
		};

		/* 0x058 */ float m_rootTwistAngle;
		/* 0x05C */ std::int8_t m_twistType;
		/* 0x05D */ std::uint8_t pad05D[0x3];
		/* 0x060 */ hkQsTransformf m_calculatedRotation;
	};
	static_assert(sizeof(BSRootTwistModifier) == 0x90);
	static_assert(offsetof(BSRootTwistModifier, m_rootTwistAngle) == 0x58);
	static_assert(offsetof(BSRootTwistModifier, m_twistType) == 0x5C);
	static_assert(offsetof(BSRootTwistModifier, m_calculatedRotation) == 0x60);

	class BSSpeedSamplerModifier : public hkbModifier
	{
	public:
		/* 0x058 */ std::int32_t m_state;
		/* 0x05C */ float m_direction;
		/* 0x060 */ float m_goalSpeed;
		/* 0x064 */ float m_speedOut;
	};
	static_assert(sizeof(BSSpeedSamplerModifier) == 0x68);
	static_assert(offsetof(BSSpeedSamplerModifier, m_state) == 0x58);
	static_assert(offsetof(BSSpeedSamplerModifier, m_direction) == 0x5C);
	static_assert(offsetof(BSSpeedSamplerModifier, m_goalSpeed) == 0x60);
	static_assert(offsetof(BSSpeedSamplerModifier, m_speedOut) == 0x64);

	class BSTimerModifier : public hkbModifier
	{
	public:
		/* 0x058 */ float m_alarmTimeSeconds;
		/* 0x05C */ std::uint8_t pad05C[0x4];
		/* 0x060 */ hkbEventProperty m_alarmEvent;
		/* 0x070 */ hkBool m_resetAlarm;
		/* 0x071 */ std::uint8_t pad071[0x3];
		/* 0x074 */ float m_secondsElapsed;
	};
	static_assert(sizeof(BSTimerModifier) == 0x78);
	static_assert(offsetof(BSTimerModifier, m_alarmTimeSeconds) == 0x58);
	static_assert(offsetof(BSTimerModifier, m_alarmEvent) == 0x60);
	static_assert(offsetof(BSTimerModifier, m_resetAlarm) == 0x70);
	static_assert(offsetof(BSTimerModifier, m_secondsElapsed) == 0x74);

	class BSTweenerModifier : public hkbModifier
	{
	public:
		/* 0x058 */ hkBool m_tweenPosition;
		/* 0x059 */ hkBool m_tweenRotation;
		/* 0x05A */ hkBool m_useTweenDuration;
		/* 0x05B */ std::uint8_t pad05B[0x1];
		/* 0x05C */ float m_tweenDuration;
		/* 0x060 */ hkVector4f m_targetPosition;
		/* 0x070 */ hkQuaternionf m_targetRotation;
		/* 0x080 */ float m_duration;
		/* 0x084 */ std::uint8_t pad084[0xC];
		/* 0x090 */ hkQsTransformf m_startTransform;
		/* 0x0C0 */ float m_time;
		/* 0x0C4 */ std::uint8_t pad0C4[0xC];
	};
	static_assert(sizeof(BSTweenerModifier) == 0xD0);
	static_assert(offsetof(BSTweenerModifier, m_tweenPosition) == 0x58);
	static_assert(offsetof(BSTweenerModifier, m_tweenRotation) == 0x59);
	static_assert(offsetof(BSTweenerModifier, m_useTweenDuration) == 0x5A);
	static_assert(offsetof(BSTweenerModifier, m_tweenDuration) == 0x5C);
	static_assert(offsetof(BSTweenerModifier, m_targetPosition) == 0x60);
	static_assert(offsetof(BSTweenerModifier, m_targetRotation) == 0x70);
	static_assert(offsetof(BSTweenerModifier, m_duration) == 0x80);
	static_assert(offsetof(BSTweenerModifier, m_startTransform) == 0x90);
	static_assert(offsetof(BSTweenerModifier, m_time) == 0xC0);

	class BSiStateTaggingGenerator : public hkbGenerator
	{
	public:
		/* 0x088 */ std::uint8_t pad088[0x8];
		/* 0x090 */ hkbGenerator* m_pDefaultGenerator;
		/* 0x098 */ std::int32_t m_iStateToSetAs;
		/* 0x09C */ std::int32_t m_iPriority;
	};
	static_assert(sizeof(BSiStateTaggingGenerator) == 0xA0);
	static_assert(offsetof(BSiStateTaggingGenerator, m_pDefaultGenerator) == 0x90);
	static_assert(offsetof(BSiStateTaggingGenerator, m_iStateToSetAs) == 0x98);
	static_assert(offsetof(BSiStateTaggingGenerator, m_iPriority) == 0x9C);

	class BSiStateTaggingStateListener : public hkbStateListener
	{
	public:
		/* 0x010 */ std::int32_t m_iStateToSetAs;
		/* 0x014 */ std::int32_t m_iPriority;
	};
	static_assert(sizeof(BSiStateTaggingStateListener) == 0x18);
	static_assert(offsetof(BSiStateTaggingStateListener, m_iStateToSetAs) == 0x10);
	static_assert(offsetof(BSiStateTaggingStateListener, m_iPriority) == 0x14);

}
