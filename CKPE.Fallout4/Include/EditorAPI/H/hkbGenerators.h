// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseObject.h>
#include <EditorAPI/H/hkBaseTypes.h>
#include <EditorAPI/H/hkReflectionTypes.h>
#include <EditorAPI/H/hkbBehaviorGraph.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkAabb;
	class hkbAnimatedSkeletonGenerator;
	class hkbBehaviorGraph;
	class hkbBehaviorGraphData;
	class hkbBehaviorReferenceGenerator;
	class hkbBlenderGenerator;
	class hkbBlenderGeneratorChild;
	class hkbBlendingTransitionEffect;
	class hkbBoneIndexArray;
	class hkbClipGenerator;
	class hkbClipTriggerArray;
	class hkbCustomIdSelector;
	class hkbDockingGenerator;
	class hkbEvent;
	class hkbExtrapolatingTransitionEffect;
	class hkbGenerator;
	class hkbGeneratorPartitionInfo;
	class hkbGeneratorSyncInfo;
	class hkbGeneratorTransitionEffect;
	class hkbHoldFromBlendingTransitionEffect;
	class hkbLayer;
	class hkbLayerGenerator;
	class hkbManualSelectorGenerator;
	class hkbManualSelectorTransitionEffect;
	class hkbModifier;
	class hkbModifierGenerator;
	class hkbParametricMotionGenerator;
	class hkbPinBonesGenerator;
	class hkbPoseMatchingGenerator;
	class hkbProceduralBlenderGenerator;
	class hkbRadialSelectorGenerator;
	class hkbRadialSelectorGeneratorGeneratorPair;
	class hkbReferencePoseGenerator;
	class hkbScriptGenerator;
	class hkbStateMachine;
	class hkbStateMachineStateInfo;
	class hkbStateMachineTransitionInfoArray;
	class hkbTransitionEffect;

	class hkbAnimatedSkeletonGenerator : public hkbGenerator
	{
	public:
		/* 0x088 */ void* m_animatedSkeleton;
	};
	static_assert(sizeof(hkbAnimatedSkeletonGenerator) == 0x90);
	static_assert(offsetof(hkbAnimatedSkeletonGenerator, m_animatedSkeleton) == 0x88);

	class hkbBehaviorGraph : public hkbGenerator
	{
	public:
		enum class VariableMode : std::int32_t
		{
			VARIABLE_MODE_DISCARD_WHEN_INACTIVE = 0,
			VARIABLE_MODE_MAINTAIN_VALUES_WHEN_INACTIVE = 1,
		};

		/* 0x088 */ std::int8_t m_variableMode;
		/* 0x089 */ std::uint8_t pad089[0x7];
		/* 0x090 */ hkArray<void> m_uniqueIdPool;
		/* 0x0A0 */ void* m_idToStateMachineTemplateMap;
		/* 0x0A8 */ hkArray<void> m_mirroredExternalIdMap;
		/* 0x0B8 */ void* m_pseudoRandomGenerator;
		/* 0x0C0 */ hkbGenerator* m_rootGenerator;
		/* 0x0C8 */ hkbBehaviorGraphData* m_data;
		/* 0x0D0 */ void* m_template;
		/* 0x0D8 */ void* m_rootGeneratorClone;
		/* 0x0E0 */ void* m_activeNodes;
		/* 0x0E8 */ void* m_globalTransitionData;
		/* 0x0F0 */ void* m_eventIdMap;
		/* 0x0F8 */ void* m_attributeIdMap;
		/* 0x100 */ void* m_variableIdMap;
		/* 0x108 */ void* m_characterPropertyIdMap;
		/* 0x110 */ void* m_variableValueSet;
		/* 0x118 */ void* m_nodeTemplateToCloneMap;
		/* 0x120 */ void* m_stateListenerTemplateToCloneMap;
		/* 0x128 */ hkArray<void*> m_recentlyCreatedClones;
		/* 0x138 */ void* m_nodePartitionInfo;
		/* 0x140 */ std::int32_t m_numIntermediateOutputs;
		/* 0x144 */ std::uint8_t pad144[0x4];
		/* 0x148 */ hkArray<void> m_intermediateOutputSizes;
		/* 0x158 */ hkArray<void*> m_jobs;
		/* 0x168 */ hkArray<void*> m_allPartitionMemory;
		/* 0x178 */ hkArray<void> m_internalToRootVariableIdMap;
		/* 0x188 */ hkArray<void> m_internalToCharacterCharacterPropertyIdMap;
		/* 0x198 */ hkArray<void> m_internalToRootAttributeIdMap;
		/* 0x1A8 */ std::uint16_t m_nextUniqueId;
		/* 0x1AA */ hkBool m_isActive;
		/* 0x1AB */ hkBool m_isLinked;
		/* 0x1AC */ hkBool m_updateActiveNodes;
		/* 0x1AD */ hkBool m_updateActiveNodesForEnable;
		/* 0x1AE */ hkBool m_checkNodeValidity;
		/* 0x1AF */ hkBool m_stateOrTransitionChanged;
	};
	static_assert(sizeof(hkbBehaviorGraph) == 0x1B0);
	static_assert(offsetof(hkbBehaviorGraph, m_variableMode) == 0x88);
	static_assert(offsetof(hkbBehaviorGraph, m_uniqueIdPool) == 0x90);
	static_assert(offsetof(hkbBehaviorGraph, m_idToStateMachineTemplateMap) == 0xA0);
	static_assert(offsetof(hkbBehaviorGraph, m_mirroredExternalIdMap) == 0xA8);
	static_assert(offsetof(hkbBehaviorGraph, m_pseudoRandomGenerator) == 0xB8);
	static_assert(offsetof(hkbBehaviorGraph, m_rootGenerator) == 0xC0);
	static_assert(offsetof(hkbBehaviorGraph, m_data) == 0xC8);
	static_assert(offsetof(hkbBehaviorGraph, m_template) == 0xD0);
	static_assert(offsetof(hkbBehaviorGraph, m_rootGeneratorClone) == 0xD8);
	static_assert(offsetof(hkbBehaviorGraph, m_activeNodes) == 0xE0);
	static_assert(offsetof(hkbBehaviorGraph, m_globalTransitionData) == 0xE8);
	static_assert(offsetof(hkbBehaviorGraph, m_eventIdMap) == 0xF0);
	static_assert(offsetof(hkbBehaviorGraph, m_attributeIdMap) == 0xF8);
	static_assert(offsetof(hkbBehaviorGraph, m_variableIdMap) == 0x100);
	static_assert(offsetof(hkbBehaviorGraph, m_characterPropertyIdMap) == 0x108);
	static_assert(offsetof(hkbBehaviorGraph, m_variableValueSet) == 0x110);
	static_assert(offsetof(hkbBehaviorGraph, m_nodeTemplateToCloneMap) == 0x118);
	static_assert(offsetof(hkbBehaviorGraph, m_stateListenerTemplateToCloneMap) == 0x120);
	static_assert(offsetof(hkbBehaviorGraph, m_recentlyCreatedClones) == 0x128);
	static_assert(offsetof(hkbBehaviorGraph, m_nodePartitionInfo) == 0x138);
	static_assert(offsetof(hkbBehaviorGraph, m_numIntermediateOutputs) == 0x140);
	static_assert(offsetof(hkbBehaviorGraph, m_intermediateOutputSizes) == 0x148);
	static_assert(offsetof(hkbBehaviorGraph, m_jobs) == 0x158);
	static_assert(offsetof(hkbBehaviorGraph, m_allPartitionMemory) == 0x168);
	static_assert(offsetof(hkbBehaviorGraph, m_internalToRootVariableIdMap) == 0x178);
	static_assert(offsetof(hkbBehaviorGraph, m_internalToCharacterCharacterPropertyIdMap) == 0x188);
	static_assert(offsetof(hkbBehaviorGraph, m_internalToRootAttributeIdMap) == 0x198);
	static_assert(offsetof(hkbBehaviorGraph, m_nextUniqueId) == 0x1A8);
	static_assert(offsetof(hkbBehaviorGraph, m_isActive) == 0x1AA);
	static_assert(offsetof(hkbBehaviorGraph, m_isLinked) == 0x1AB);
	static_assert(offsetof(hkbBehaviorGraph, m_updateActiveNodes) == 0x1AC);
	static_assert(offsetof(hkbBehaviorGraph, m_updateActiveNodesForEnable) == 0x1AD);
	static_assert(offsetof(hkbBehaviorGraph, m_checkNodeValidity) == 0x1AE);
	static_assert(offsetof(hkbBehaviorGraph, m_stateOrTransitionChanged) == 0x1AF);

	class hkbBehaviorReferenceGenerator : public hkbGenerator
	{
	public:
		/* 0x088 */ hkStringPtr m_behaviorName;
		/* 0x090 */ void* m_behavior;
	};
	static_assert(sizeof(hkbBehaviorReferenceGenerator) == 0x98);
	static_assert(offsetof(hkbBehaviorReferenceGenerator, m_behaviorName) == 0x88);
	static_assert(offsetof(hkbBehaviorReferenceGenerator, m_behavior) == 0x90);

	class hkbBlenderGenerator : public hkbGenerator
	{
	public:
		enum class BlenderFlags : std::int32_t
		{
			FLAG_SYNC = 1,
			FLAG_SMOOTH_GENERATOR_WEIGHTS = 4,
			FLAG_DONT_DEACTIVATE_CHILDREN_WITH_ZERO_WEIGHTS = 8,
			FLAG_PARAMETRIC_BLEND = 16,
			FLAG_IS_PARAMETRIC_BLEND_CYCLIC = 32,
			FLAG_FORCE_DENSE_POSE = 64,
			FLAG_BLEND_MOTION_OF_ADDITIVE_ANIMATIONS = 128,
			FLAG_USE_VELOCITY_SYNCHRONIZATION = 256,
		};

		/* 0x088 */ float m_referencePoseWeightThreshold;
		/* 0x08C */ float m_blendParameter;
		/* 0x090 */ float m_minCyclicBlendParameter;
		/* 0x094 */ float m_maxCyclicBlendParameter;
		/* 0x098 */ std::int16_t m_indexOfSyncMasterChild;
		/* 0x09A */ std::int16_t m_flags;
		/* 0x09C */ hkBool m_subtractLastChild;
		/* 0x09D */ std::uint8_t pad09D[0x3];
		/* 0x0A0 */ hkArray<hkbBlenderGeneratorChild*> m_children;
		/* 0x0B0 */ hkArray<void> m_childrenInternalStates;
		/* 0x0C0 */ hkArray<void> m_sortedChildren;
		/* 0x0D0 */ float m_endIntervalWeight;
		/* 0x0D4 */ std::int32_t m_numActiveChildren;
		/* 0x0D8 */ std::int16_t m_beginIntervalIndex;
		/* 0x0DA */ std::int16_t m_endIntervalIndex;
		/* 0x0DC */ hkBool m_initSync;
		/* 0x0DD */ hkBool m_doSubtractiveBlend;
		/* 0x0DE */ std::uint8_t pad0DE[0x2];
	};
	static_assert(sizeof(hkbBlenderGenerator) == 0xE0);
	static_assert(offsetof(hkbBlenderGenerator, m_referencePoseWeightThreshold) == 0x88);
	static_assert(offsetof(hkbBlenderGenerator, m_blendParameter) == 0x8C);
	static_assert(offsetof(hkbBlenderGenerator, m_minCyclicBlendParameter) == 0x90);
	static_assert(offsetof(hkbBlenderGenerator, m_maxCyclicBlendParameter) == 0x94);
	static_assert(offsetof(hkbBlenderGenerator, m_indexOfSyncMasterChild) == 0x98);
	static_assert(offsetof(hkbBlenderGenerator, m_flags) == 0x9A);
	static_assert(offsetof(hkbBlenderGenerator, m_subtractLastChild) == 0x9C);
	static_assert(offsetof(hkbBlenderGenerator, m_children) == 0xA0);
	static_assert(offsetof(hkbBlenderGenerator, m_childrenInternalStates) == 0xB0);
	static_assert(offsetof(hkbBlenderGenerator, m_sortedChildren) == 0xC0);
	static_assert(offsetof(hkbBlenderGenerator, m_endIntervalWeight) == 0xD0);
	static_assert(offsetof(hkbBlenderGenerator, m_numActiveChildren) == 0xD4);
	static_assert(offsetof(hkbBlenderGenerator, m_beginIntervalIndex) == 0xD8);
	static_assert(offsetof(hkbBlenderGenerator, m_endIntervalIndex) == 0xDA);
	static_assert(offsetof(hkbBlenderGenerator, m_initSync) == 0xDC);
	static_assert(offsetof(hkbBlenderGenerator, m_doSubtractiveBlend) == 0xDD);

	class hkbTransitionEffect : public hkbGenerator
	{
	public:
		enum class SelfTransitionMode : std::int32_t
		{
			SELF_TRANSITION_MODE_CONTINUE_IF_CYCLIC_BLEND_IF_ACYCLIC = 0,
			SELF_TRANSITION_MODE_CONTINUE = 1,
			SELF_TRANSITION_MODE_RESET = 2,
			SELF_TRANSITION_MODE_BLEND = 3,
		};

		enum class EventMode : std::int32_t
		{
			EVENT_MODE_DEFAULT = 0,
			EVENT_MODE_PROCESS_ALL = 1,
			EVENT_MODE_IGNORE_FROM_GENERATOR = 2,
			EVENT_MODE_IGNORE_TO_GENERATOR = 3,
		};

		/* 0x088 */ std::int8_t m_selfTransitionMode;
		/* 0x089 */ std::int8_t m_eventMode;
		/* 0x08A */ std::int8_t m_defaultEventMode;
		/* 0x08B */ std::uint8_t pad08B[0x5];
		/* 0x090 */ void* m_patchedBindingInfo;
		/* 0x098 */ void* m_fromGenerator;
		/* 0x0A0 */ void* m_toGenerator;

	private:
		/* 1E */ virtual void sub_hkbTransitionEffect_1E();
		/* 1F */ virtual void sub_hkbTransitionEffect_1F();
		/* 20 */ virtual void sub_hkbTransitionEffect_20();
		/* 21 */ virtual void sub_hkbTransitionEffect_21();
		/* 22 */ virtual void sub_hkbTransitionEffect_22();
	};
	static_assert(sizeof(hkbTransitionEffect) == 0xA8);
	static_assert(offsetof(hkbTransitionEffect, m_selfTransitionMode) == 0x88);
	static_assert(offsetof(hkbTransitionEffect, m_eventMode) == 0x89);
	static_assert(offsetof(hkbTransitionEffect, m_defaultEventMode) == 0x8A);
	static_assert(offsetof(hkbTransitionEffect, m_patchedBindingInfo) == 0x90);
	static_assert(offsetof(hkbTransitionEffect, m_fromGenerator) == 0x98);
	static_assert(offsetof(hkbTransitionEffect, m_toGenerator) == 0xA0);

	class hkbBlendingTransitionEffect : public hkbTransitionEffect
	{
	public:
		enum class FlagBits : std::int32_t
		{
			FLAG_NONE = 0,
			FLAG_IGNORE_FROM_WORLD_FROM_MODEL = 1,
			FLAG_SYNC = 2,
			FLAG_IGNORE_TO_WORLD_FROM_MODEL = 4,
			FLAG_IGNORE_TO_WORLD_FROM_MODEL_ROTATION = 8,
		};

		enum class EndMode : std::int32_t
		{
			END_MODE_NONE = 0,
			END_MODE_TRANSITION_UNTIL_END_OF_FROM_GENERATOR = 1,
			END_MODE_CAP_DURATION_AT_END_OF_FROM_GENERATOR = 2,
		};

		/* 0x0A8 */ float m_duration;
		/* 0x0AC */ float m_toGeneratorStartTimeFraction;
		/* 0x0B0 */ std::uint16_t m_flags;
		/* 0x0B2 */ std::int8_t m_endMode;
		/* 0x0B3 */ std::int8_t m_blendCurve;
		/* 0x0B4 */ std::int16_t m_alignmentBone;
		/* 0x0B6 */ std::uint8_t pad0B6[0xA];
		/* 0x0C0 */ hkVector4f m_fromPos;
		/* 0x0D0 */ hkQuaternionf m_fromRot;
		/* 0x0E0 */ hkVector4f m_toPos;
		/* 0x0F0 */ hkQuaternionf m_toRot;
		/* 0x100 */ hkVector4f m_lastPos;
		/* 0x110 */ hkQuaternionf m_lastRot;
		/* 0x120 */ hkArray<void> m_characterPoseAtBeginningOfTransition;
		/* 0x130 */ float m_timeRemaining;
		/* 0x134 */ float m_timeInTransition;
		/* 0x138 */ std::int8_t m_toGeneratorSelfTranstitionMode;
		/* 0x139 */ hkBool m_initializeCharacterPose;
		/* 0x13A */ hkBool m_alignThisFrame;
		/* 0x13B */ hkBool m_alignmentFinished;
		/* 0x13C */ std::uint8_t pad13C[0x4];

	private:
		/* 23 */ virtual void sub_hkbBlendingTransitionEffect_23();
		/* 24 */ virtual void sub_hkbBlendingTransitionEffect_24();
		/* 25 */ virtual void sub_hkbBlendingTransitionEffect_25();
		/* 26 */ virtual void sub_hkbBlendingTransitionEffect_26();
		/* 27 */ virtual void sub_hkbBlendingTransitionEffect_27();
	};
	static_assert(sizeof(hkbBlendingTransitionEffect) == 0x140);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_duration) == 0xA8);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_toGeneratorStartTimeFraction) == 0xAC);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_flags) == 0xB0);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_endMode) == 0xB2);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_blendCurve) == 0xB3);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_alignmentBone) == 0xB4);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_fromPos) == 0xC0);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_fromRot) == 0xD0);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_toPos) == 0xE0);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_toRot) == 0xF0);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_lastPos) == 0x100);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_lastRot) == 0x110);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_characterPoseAtBeginningOfTransition) == 0x120);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_timeRemaining) == 0x130);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_timeInTransition) == 0x134);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_toGeneratorSelfTranstitionMode) == 0x138);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_initializeCharacterPose) == 0x139);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_alignThisFrame) == 0x13A);
	static_assert(offsetof(hkbBlendingTransitionEffect, m_alignmentFinished) == 0x13B);

	class hkbClipGenerator : public hkbGenerator
	{
	public:
		enum class PlaybackMode : std::int32_t
		{
			MODE_SINGLE_PLAY = 0,
			MODE_LOOPING = 1,
			MODE_USER_CONTROLLED = 2,
			MODE_PING_PONG = 3,
			MODE_COUNT = 4,
		};

		enum class ClipFlags : std::int32_t
		{
			FLAG_CONTINUE_MOTION_AT_END = 1,
			FLAG_SYNC_HALF_CYCLE_IN_PING_PONG_MODE = 2,
			FLAG_MIRROR = 4,
			FLAG_FORCE_DENSE_POSE = 8,
			FLAG_DONT_CONVERT_ANNOTATIONS_TO_TRIGGERS = 16,
			FLAG_IGNORE_MOTION = 32,
		};

		/* 0x088 */ hkStringPtr m_animationBundleName;
		/* 0x090 */ hkStringPtr m_animationName;
		/* 0x098 */ hkbClipTriggerArray* m_triggers;
		/* 0x0A0 */ std::uint32_t m_userPartitionMask;
		/* 0x0A4 */ float m_cropStartAmountLocalTime;
		/* 0x0A8 */ float m_cropEndAmountLocalTime;
		/* 0x0AC */ float m_startTime;
		/* 0x0B0 */ float m_playbackSpeed;
		/* 0x0B4 */ float m_enforcedDuration;
		/* 0x0B8 */ float m_userControlledTimeFraction;
		/* 0x0BC */ std::int16_t m_animationBindingIndex;
		/* 0x0BE */ std::int8_t m_mode;
		/* 0x0BF */ std::int8_t m_flags;
		/* 0x0C0 */ hkArray<void> m_animDatas;
		/* 0x0D0 */ void* m_animationControl;
		/* 0x0D8 */ void* m_originalTriggers;
		/* 0x0E0 */ void* m_mapperData;
		/* 0x0E8 */ void* m_binding;
		/* 0x0F0 */ std::int32_t m_numAnimationTracks;
		/* 0x0F4 */ std::uint8_t pad0F4[0xC];
		/* 0x100 */ hkQsTransformf m_extractedMotion;
		/* 0x130 */ hkArray<void> m_echos;
		/* 0x140 */ float m_localTime;
		/* 0x144 */ float m_time;
		/* 0x148 */ float m_previousUserControlledTimeFraction;
		/* 0x14C */ std::int32_t m_bufferSize;
		/* 0x150 */ hkBool m_atEnd;
		/* 0x151 */ hkBool m_ignoreStartTime;
		/* 0x152 */ hkBool m_pingPongBackward;
		/* 0x153 */ std::uint8_t pad153[0xD];
	};
	static_assert(sizeof(hkbClipGenerator) == 0x160);
	static_assert(offsetof(hkbClipGenerator, m_animationBundleName) == 0x88);
	static_assert(offsetof(hkbClipGenerator, m_animationName) == 0x90);
	static_assert(offsetof(hkbClipGenerator, m_triggers) == 0x98);
	static_assert(offsetof(hkbClipGenerator, m_userPartitionMask) == 0xA0);
	static_assert(offsetof(hkbClipGenerator, m_cropStartAmountLocalTime) == 0xA4);
	static_assert(offsetof(hkbClipGenerator, m_cropEndAmountLocalTime) == 0xA8);
	static_assert(offsetof(hkbClipGenerator, m_startTime) == 0xAC);
	static_assert(offsetof(hkbClipGenerator, m_playbackSpeed) == 0xB0);
	static_assert(offsetof(hkbClipGenerator, m_enforcedDuration) == 0xB4);
	static_assert(offsetof(hkbClipGenerator, m_userControlledTimeFraction) == 0xB8);
	static_assert(offsetof(hkbClipGenerator, m_animationBindingIndex) == 0xBC);
	static_assert(offsetof(hkbClipGenerator, m_mode) == 0xBE);
	static_assert(offsetof(hkbClipGenerator, m_flags) == 0xBF);
	static_assert(offsetof(hkbClipGenerator, m_animDatas) == 0xC0);
	static_assert(offsetof(hkbClipGenerator, m_animationControl) == 0xD0);
	static_assert(offsetof(hkbClipGenerator, m_originalTriggers) == 0xD8);
	static_assert(offsetof(hkbClipGenerator, m_mapperData) == 0xE0);
	static_assert(offsetof(hkbClipGenerator, m_binding) == 0xE8);
	static_assert(offsetof(hkbClipGenerator, m_numAnimationTracks) == 0xF0);
	static_assert(offsetof(hkbClipGenerator, m_extractedMotion) == 0x100);
	static_assert(offsetof(hkbClipGenerator, m_echos) == 0x130);
	static_assert(offsetof(hkbClipGenerator, m_localTime) == 0x140);
	static_assert(offsetof(hkbClipGenerator, m_time) == 0x144);
	static_assert(offsetof(hkbClipGenerator, m_previousUserControlledTimeFraction) == 0x148);
	static_assert(offsetof(hkbClipGenerator, m_bufferSize) == 0x14C);
	static_assert(offsetof(hkbClipGenerator, m_atEnd) == 0x150);
	static_assert(offsetof(hkbClipGenerator, m_ignoreStartTime) == 0x151);
	static_assert(offsetof(hkbClipGenerator, m_pingPongBackward) == 0x152);

	class hkbDockingGenerator : public hkbGenerator
	{
	public:
		enum class BlendType : std::int32_t
		{
			BLEND_TYPE_BLEND_IN = 0,
			BLEND_TYPE_FULL_ON = 1,
		};

		enum class DockingFlagBits : std::int32_t
		{
			FLAG_NONE = 0,
			FLAG_DOCK_TO_FUTURE_POSITION = 1,
			FLAG_OVERRIDE_MOTION = 2,
		};

		/* 0x088 */ std::int16_t m_dockingBone;
		/* 0x08A */ std::uint8_t pad08A[0x6];
		/* 0x090 */ hkVector4f m_translationOffset;
		/* 0x0A0 */ hkQuaternionf m_rotationOffset;
		/* 0x0B0 */ std::int8_t m_blendType;
		/* 0x0B1 */ std::uint8_t pad0B1[0x1];
		/* 0x0B2 */ std::uint16_t m_flags;
		/* 0x0B4 */ std::uint8_t pad0B4[0x4];
		/* 0x0B8 */ hkbGenerator* m_child;
		/* 0x0C0 */ std::int32_t m_intervalStart;
		/* 0x0C4 */ std::int32_t m_intervalEnd;
		/* 0x0C8 */ float m_localTime;
		/* 0x0CC */ float m_previousLocalTime;
		/* 0x0D0 */ float m_intervalStartLocalTime;
		/* 0x0D4 */ float m_intervalEndLocalTime;
		/* 0x0D8 */ std::uint8_t pad0D8[0x8];
	};
	static_assert(sizeof(hkbDockingGenerator) == 0xE0);
	static_assert(offsetof(hkbDockingGenerator, m_dockingBone) == 0x88);
	static_assert(offsetof(hkbDockingGenerator, m_translationOffset) == 0x90);
	static_assert(offsetof(hkbDockingGenerator, m_rotationOffset) == 0xA0);
	static_assert(offsetof(hkbDockingGenerator, m_blendType) == 0xB0);
	static_assert(offsetof(hkbDockingGenerator, m_flags) == 0xB2);
	static_assert(offsetof(hkbDockingGenerator, m_child) == 0xB8);
	static_assert(offsetof(hkbDockingGenerator, m_intervalStart) == 0xC0);
	static_assert(offsetof(hkbDockingGenerator, m_intervalEnd) == 0xC4);
	static_assert(offsetof(hkbDockingGenerator, m_localTime) == 0xC8);
	static_assert(offsetof(hkbDockingGenerator, m_previousLocalTime) == 0xCC);
	static_assert(offsetof(hkbDockingGenerator, m_intervalStartLocalTime) == 0xD0);
	static_assert(offsetof(hkbDockingGenerator, m_intervalEndLocalTime) == 0xD4);

	class hkbExtrapolatingTransitionEffect : public hkbBlendingTransitionEffect
	{
	public:
		/* 0x140 */ hkbGeneratorSyncInfo m_fromGeneratorSyncInfo;
		/* 0x1E8 */ hkbGeneratorPartitionInfo m_fromGeneratorPartitionInfo;
		/* 0x210 */ hkQsTransformf m_worldFromModel;
		/* 0x240 */ hkQsTransformf m_motion;
		/* 0x270 */ hkArray<void> m_pose;
		/* 0x280 */ hkArray<void> m_additivePose;
		/* 0x290 */ hkArray<void> m_boneWeights;
		/* 0x2A0 */ float m_toGeneratorDuration;
		/* 0x2A4 */ hkBool m_isFromGeneratorActive;
		/* 0x2A5 */ hkBool m_gotPose;
		/* 0x2A6 */ hkBool m_gotAdditivePose;
		/* 0x2A7 */ std::uint8_t pad2A7[0x9];
	};
	static_assert(sizeof(hkbExtrapolatingTransitionEffect) == 0x2B0);
	static_assert(offsetof(hkbExtrapolatingTransitionEffect, m_fromGeneratorSyncInfo) == 0x140);
	static_assert(offsetof(hkbExtrapolatingTransitionEffect, m_fromGeneratorPartitionInfo) == 0x1E8);
	static_assert(offsetof(hkbExtrapolatingTransitionEffect, m_worldFromModel) == 0x210);
	static_assert(offsetof(hkbExtrapolatingTransitionEffect, m_motion) == 0x240);
	static_assert(offsetof(hkbExtrapolatingTransitionEffect, m_pose) == 0x270);
	static_assert(offsetof(hkbExtrapolatingTransitionEffect, m_additivePose) == 0x280);
	static_assert(offsetof(hkbExtrapolatingTransitionEffect, m_boneWeights) == 0x290);
	static_assert(offsetof(hkbExtrapolatingTransitionEffect, m_toGeneratorDuration) == 0x2A0);
	static_assert(offsetof(hkbExtrapolatingTransitionEffect, m_isFromGeneratorActive) == 0x2A4);
	static_assert(offsetof(hkbExtrapolatingTransitionEffect, m_gotPose) == 0x2A5);
	static_assert(offsetof(hkbExtrapolatingTransitionEffect, m_gotAdditivePose) == 0x2A6);

	class hkbGeneratorTransitionEffect : public hkbTransitionEffect
	{
	public:
		enum class ToGeneratorState : std::int32_t
		{
			STATE_INACTIVE = 0,
			STATE_READY_FOR_SET_LOCAL_TIME = 1,
			STATE_READY_FOR_APPLY_SELF_TRANSITION_MODE = 2,
			STATE_ACTIVE = 3,
		};

		enum class Stage : std::int32_t
		{
			STAGE_BLENDING_IN = 0,
			STAGE_PLAYING_TRANSITION_GENERATOR = 1,
			STAGE_BLENDING_OUT = 2,
		};

		enum class ChildState : std::int32_t
		{
			CHILD_FROM_GENERATOR = 0,
			CHILD_TRANSITION_GENERATOR = 1,
			CHILD_TO_GENERATOR = 2,
			CHILD_NONE = 3,
		};

		/* 0x0A8 */ hkbGenerator* m_transitionGenerator;
		/* 0x0B0 */ float m_blendInDuration;
		/* 0x0B4 */ float m_blendOutDuration;
		/* 0x0B8 */ hkBool m_syncToGeneratorStartTime;
		/* 0x0B9 */ std::int8_t m_childState;
		/* 0x0BA */ std::uint8_t pad0BA[0x2];
		/* 0x0BC */ float m_timeInTransition;
		/* 0x0C0 */ float m_duration;
		/* 0x0C4 */ float m_effectiveBlendInDuration;
		/* 0x0C8 */ float m_effectiveBlendOutDuration;
		/* 0x0CC */ std::int8_t m_toGeneratorState;
		/* 0x0CD */ hkBool m_echoTransitionGenerator;
		/* 0x0CE */ std::int8_t m_toGeneratorSelfTransitionMode;
		/* 0x0CF */ hkBool m_justActivated;
		/* 0x0D0 */ hkBool m_updateActiveNodes;
		/* 0x0D1 */ std::int8_t m_stage;
		/* 0x0D2 */ std::uint8_t pad0D2[0x6];
	};
	static_assert(sizeof(hkbGeneratorTransitionEffect) == 0xD8);
	static_assert(offsetof(hkbGeneratorTransitionEffect, m_transitionGenerator) == 0xA8);
	static_assert(offsetof(hkbGeneratorTransitionEffect, m_blendInDuration) == 0xB0);
	static_assert(offsetof(hkbGeneratorTransitionEffect, m_blendOutDuration) == 0xB4);
	static_assert(offsetof(hkbGeneratorTransitionEffect, m_syncToGeneratorStartTime) == 0xB8);
	static_assert(offsetof(hkbGeneratorTransitionEffect, m_childState) == 0xB9);
	static_assert(offsetof(hkbGeneratorTransitionEffect, m_timeInTransition) == 0xBC);
	static_assert(offsetof(hkbGeneratorTransitionEffect, m_duration) == 0xC0);
	static_assert(offsetof(hkbGeneratorTransitionEffect, m_effectiveBlendInDuration) == 0xC4);
	static_assert(offsetof(hkbGeneratorTransitionEffect, m_effectiveBlendOutDuration) == 0xC8);
	static_assert(offsetof(hkbGeneratorTransitionEffect, m_toGeneratorState) == 0xCC);
	static_assert(offsetof(hkbGeneratorTransitionEffect, m_echoTransitionGenerator) == 0xCD);
	static_assert(offsetof(hkbGeneratorTransitionEffect, m_toGeneratorSelfTransitionMode) == 0xCE);
	static_assert(offsetof(hkbGeneratorTransitionEffect, m_justActivated) == 0xCF);
	static_assert(offsetof(hkbGeneratorTransitionEffect, m_updateActiveNodes) == 0xD0);
	static_assert(offsetof(hkbGeneratorTransitionEffect, m_stage) == 0xD1);

	class hkbHoldFromBlendingTransitionEffect : public hkbBlendingTransitionEffect
	{
	public:
		/* 0x140 */ void* m_heldFromPose;
		/* 0x148 */ std::int32_t m_heldFromPoseSize;
		/* 0x14C */ std::uint8_t pad14C[0x4];
		/* 0x150 */ hkQsTransformf m_heldWorldFromModel;
		/* 0x180 */ void* m_heldFromSkeleton;
		/* 0x188 */ std::uint8_t pad188[0x8];
	};
	static_assert(sizeof(hkbHoldFromBlendingTransitionEffect) == 0x190);
	static_assert(offsetof(hkbHoldFromBlendingTransitionEffect, m_heldFromPose) == 0x140);
	static_assert(offsetof(hkbHoldFromBlendingTransitionEffect, m_heldFromPoseSize) == 0x148);
	static_assert(offsetof(hkbHoldFromBlendingTransitionEffect, m_heldWorldFromModel) == 0x150);
	static_assert(offsetof(hkbHoldFromBlendingTransitionEffect, m_heldFromSkeleton) == 0x180);

	class hkbLayerGenerator : public hkbGenerator
	{
	public:
		enum class LayerFlagBits : std::int32_t
		{
			FLAG_SYNC = 1,
		};

		/* 0x088 */ hkArray<hkbLayer*> m_layers;
		/* 0x098 */ std::int16_t m_indexOfSyncMasterChild;
		/* 0x09A */ std::uint16_t m_flags;
		/* 0x09C */ std::int32_t m_numActiveLayers;
		/* 0x0A0 */ hkArray<void> m_layerInternalStates;
		/* 0x0B0 */ hkBool m_initSync;
		/* 0x0B1 */ std::uint8_t pad0B1[0x7];
	};
	static_assert(sizeof(hkbLayerGenerator) == 0xB8);
	static_assert(offsetof(hkbLayerGenerator, m_layers) == 0x88);
	static_assert(offsetof(hkbLayerGenerator, m_indexOfSyncMasterChild) == 0x98);
	static_assert(offsetof(hkbLayerGenerator, m_flags) == 0x9A);
	static_assert(offsetof(hkbLayerGenerator, m_numActiveLayers) == 0x9C);
	static_assert(offsetof(hkbLayerGenerator, m_layerInternalStates) == 0xA0);
	static_assert(offsetof(hkbLayerGenerator, m_initSync) == 0xB0);

	class hkbManualSelectorGenerator : public hkbGenerator
	{
	public:
		/* 0x088 */ hkArray<hkbGenerator*> m_generators;
		/* 0x098 */ std::int16_t m_selectedGeneratorIndex;
		/* 0x09A */ std::uint8_t pad09A[0x6];
		/* 0x0A0 */ hkbCustomIdSelector* m_indexSelector;
		/* 0x0A8 */ hkBool m_selectedIndexCanChangeAfterActivate;
		/* 0x0A9 */ std::uint8_t pad0A9[0x7];
		/* 0x0B0 */ hkbTransitionEffect* m_generatorChangedTransitionEffect;
		/* 0x0B8 */ std::int16_t m_currentGeneratorIndex;
		/* 0x0BA */ std::int16_t m_generatorIndexAtActivate;
		/* 0x0BC */ std::uint8_t pad0BC[0x4];
		/* 0x0C0 */ hkArray<void> m_activeTransitions;
	};
	static_assert(sizeof(hkbManualSelectorGenerator) == 0xD0);
	static_assert(offsetof(hkbManualSelectorGenerator, m_generators) == 0x88);
	static_assert(offsetof(hkbManualSelectorGenerator, m_selectedGeneratorIndex) == 0x98);
	static_assert(offsetof(hkbManualSelectorGenerator, m_indexSelector) == 0xA0);
	static_assert(offsetof(hkbManualSelectorGenerator, m_selectedIndexCanChangeAfterActivate) == 0xA8);
	static_assert(offsetof(hkbManualSelectorGenerator, m_generatorChangedTransitionEffect) == 0xB0);
	static_assert(offsetof(hkbManualSelectorGenerator, m_currentGeneratorIndex) == 0xB8);
	static_assert(offsetof(hkbManualSelectorGenerator, m_generatorIndexAtActivate) == 0xBA);
	static_assert(offsetof(hkbManualSelectorGenerator, m_activeTransitions) == 0xC0);

	class hkbManualSelectorTransitionEffect : public hkbTransitionEffect
	{
	public:
		/* 0x0A8 */ hkArray<hkbTransitionEffect*> m_transitionEffects;
		/* 0x0B8 */ std::uint8_t m_selectedIndex;
		/* 0x0B9 */ std::uint8_t pad0B9[0x7];
		/* 0x0C0 */ hkbCustomIdSelector* m_indexSelector;
		/* 0x0C8 */ std::uint8_t m_currentTransitionEffectIndex;
		/* 0x0C9 */ std::uint8_t pad0C9[0x7];
		/* 0x0D0 */ void* m_currentTransitionEffect;
		/* 0x0D8 */ void* m_parentStateMachine;
		/* 0x0E0 */ void* m_referenceBehavior;
	};
	static_assert(sizeof(hkbManualSelectorTransitionEffect) == 0xE8);
	static_assert(offsetof(hkbManualSelectorTransitionEffect, m_transitionEffects) == 0xA8);
	static_assert(offsetof(hkbManualSelectorTransitionEffect, m_selectedIndex) == 0xB8);
	static_assert(offsetof(hkbManualSelectorTransitionEffect, m_indexSelector) == 0xC0);
	static_assert(offsetof(hkbManualSelectorTransitionEffect, m_currentTransitionEffectIndex) == 0xC8);
	static_assert(offsetof(hkbManualSelectorTransitionEffect, m_currentTransitionEffect) == 0xD0);
	static_assert(offsetof(hkbManualSelectorTransitionEffect, m_parentStateMachine) == 0xD8);
	static_assert(offsetof(hkbManualSelectorTransitionEffect, m_referenceBehavior) == 0xE0);

	class hkbModifierGenerator : public hkbGenerator
	{
	public:
		/* 0x088 */ hkbModifier* m_modifier;
		/* 0x090 */ hkbGenerator* m_generator;
	};
	static_assert(sizeof(hkbModifierGenerator) == 0x98);
	static_assert(offsetof(hkbModifierGenerator, m_modifier) == 0x88);
	static_assert(offsetof(hkbModifierGenerator, m_generator) == 0x90);

	class hkbProceduralBlenderGenerator : public hkbGenerator
	{
	public:
		/* 0x088 */ void* m_blender;

	private:
		/* 1E */ virtual void sub_hkbProceduralBlenderGenerator_1E();
		/* 1F */ virtual void sub_hkbProceduralBlenderGenerator_1F();
		/* 20 */ virtual void sub_hkbProceduralBlenderGenerator_20();
		/* 21 */ virtual void sub_hkbProceduralBlenderGenerator_21();
	};
	static_assert(sizeof(hkbProceduralBlenderGenerator) == 0x90);
	static_assert(offsetof(hkbProceduralBlenderGenerator, m_blender) == 0x88);

	class hkbParametricMotionGenerator : public hkbProceduralBlenderGenerator
	{
	public:
		enum class MotionSpaceType : std::int32_t
		{
			MST_UNKNOWN = 0,
			MST_ANGULAR = 1,
			MST_DIRECTIONAL = 2,
		};

		/* 0x090 */ std::int8_t m_motionSpace;
		/* 0x091 */ std::uint8_t pad091[0x7];
		/* 0x098 */ hkArray<hkbGenerator*> m_generators;
		/* 0x0A8 */ float m_xAxisParameterValue;
		/* 0x0AC */ float m_yAxisParameterValue;
		/* 0x0B0 */ hkAabb m_paramsDomain;
		/* 0x0D0 */ void* m_animationSpace;
		/* 0x0D8 */ hkArray<void> m_clipDurations;
		/* 0x0E8 */ std::uint8_t pad0E8[0x8];
	};
	static_assert(sizeof(hkbParametricMotionGenerator) == 0xF0);
	static_assert(offsetof(hkbParametricMotionGenerator, m_motionSpace) == 0x90);
	static_assert(offsetof(hkbParametricMotionGenerator, m_generators) == 0x98);
	static_assert(offsetof(hkbParametricMotionGenerator, m_xAxisParameterValue) == 0xA8);
	static_assert(offsetof(hkbParametricMotionGenerator, m_yAxisParameterValue) == 0xAC);
	static_assert(offsetof(hkbParametricMotionGenerator, m_paramsDomain) == 0xB0);
	static_assert(offsetof(hkbParametricMotionGenerator, m_animationSpace) == 0xD0);
	static_assert(offsetof(hkbParametricMotionGenerator, m_clipDurations) == 0xD8);

	class hkbPinBonesGenerator : public hkbGenerator
	{
	public:
		/* 0x088 */ hkbGenerator* m_referenceFrameGenerator;
		/* 0x090 */ hkbGenerator* m_pinnedGenerator;
		/* 0x098 */ hkbBoneIndexArray* m_boneIndices;
		/* 0x0A0 */ float m_fraction;
		/* 0x0A4 */ std::uint8_t pad0A4[0x4];
	};
	static_assert(sizeof(hkbPinBonesGenerator) == 0xA8);
	static_assert(offsetof(hkbPinBonesGenerator, m_referenceFrameGenerator) == 0x88);
	static_assert(offsetof(hkbPinBonesGenerator, m_pinnedGenerator) == 0x90);
	static_assert(offsetof(hkbPinBonesGenerator, m_boneIndices) == 0x98);
	static_assert(offsetof(hkbPinBonesGenerator, m_fraction) == 0xA0);

	class hkbPoseMatchingGenerator : public hkbBlenderGenerator
	{
	public:
		enum class Mode : std::int32_t
		{
			MODE_MATCH = 0,
			MODE_PLAY = 1,
		};

		/* 0x0E0 */ hkQuaternionf m_worldFromModelRotation;
		/* 0x0F0 */ float m_blendSpeed;
		/* 0x0F4 */ float m_minSpeedToSwitch;
		/* 0x0F8 */ float m_minSwitchTimeNoError;
		/* 0x0FC */ float m_minSwitchTimeFullError;
		/* 0x100 */ std::int32_t m_startPlayingEventId;
		/* 0x104 */ std::int32_t m_startMatchingEventId;
		/* 0x108 */ std::int16_t m_rootBoneIndex;
		/* 0x10A */ std::int16_t m_otherBoneIndex;
		/* 0x10C */ std::int16_t m_anotherBoneIndex;
		/* 0x10E */ std::int16_t m_pelvisIndex;
		/* 0x110 */ std::int8_t m_mode;
		/* 0x111 */ std::uint8_t pad111[0x3];
		/* 0x114 */ std::int32_t m_currentMatch;
		/* 0x118 */ std::int32_t m_bestMatch;
		/* 0x11C */ float m_timeSinceBetterMatch;
		/* 0x120 */ float m_error;
		/* 0x124 */ hkBool m_resetCurrentMatchLocalTime;
		/* 0x125 */ std::uint8_t pad125[0x3];
		/* 0x128 */ void* m_poseMatchingUtility;
	};
	static_assert(sizeof(hkbPoseMatchingGenerator) == 0x130);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_worldFromModelRotation) == 0xE0);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_blendSpeed) == 0xF0);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_minSpeedToSwitch) == 0xF4);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_minSwitchTimeNoError) == 0xF8);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_minSwitchTimeFullError) == 0xFC);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_startPlayingEventId) == 0x100);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_startMatchingEventId) == 0x104);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_rootBoneIndex) == 0x108);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_otherBoneIndex) == 0x10A);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_anotherBoneIndex) == 0x10C);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_pelvisIndex) == 0x10E);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_mode) == 0x110);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_currentMatch) == 0x114);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_bestMatch) == 0x118);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_timeSinceBetterMatch) == 0x11C);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_error) == 0x120);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_resetCurrentMatchLocalTime) == 0x124);
	static_assert(offsetof(hkbPoseMatchingGenerator, m_poseMatchingUtility) == 0x128);

	class hkbRadialSelectorGenerator : public hkbGenerator
	{
	public:
		/* 0x088 */ hkArray<hkbRadialSelectorGeneratorGeneratorPair> m_generatorPairs;
		/* 0x098 */ float m_angle;
		/* 0x09C */ float m_radius;
		/* 0x0A0 */ std::int32_t m_currentGeneratorPairIndex;
		/* 0x0A4 */ std::int32_t m_currentEndpointIndex;
		/* 0x0A8 */ float m_currentFraction;
		/* 0x0AC */ hkBool m_hasSetLocalTime;
		/* 0x0AD */ std::int8_t m_pad[4];
		/* 0x0B1 */ std::uint8_t pad0B1[0x7];
	};
	static_assert(sizeof(hkbRadialSelectorGenerator) == 0xB8);
	static_assert(offsetof(hkbRadialSelectorGenerator, m_generatorPairs) == 0x88);
	static_assert(offsetof(hkbRadialSelectorGenerator, m_angle) == 0x98);
	static_assert(offsetof(hkbRadialSelectorGenerator, m_radius) == 0x9C);
	static_assert(offsetof(hkbRadialSelectorGenerator, m_currentGeneratorPairIndex) == 0xA0);
	static_assert(offsetof(hkbRadialSelectorGenerator, m_currentEndpointIndex) == 0xA4);
	static_assert(offsetof(hkbRadialSelectorGenerator, m_currentFraction) == 0xA8);
	static_assert(offsetof(hkbRadialSelectorGenerator, m_hasSetLocalTime) == 0xAC);
	static_assert(offsetof(hkbRadialSelectorGenerator, m_pad) == 0xAD);

	class hkbReferencePoseGenerator : public hkbGenerator
	{
	public:
		/* 0x088 */ void* m_skeleton;
	};
	static_assert(sizeof(hkbReferencePoseGenerator) == 0x90);
	static_assert(offsetof(hkbReferencePoseGenerator, m_skeleton) == 0x88);

	class hkbScriptGenerator : public hkbGenerator
	{
	public:
		/* 0x088 */ hkbGenerator* m_child;
		/* 0x090 */ hkStringPtr m_onActivateScript;
		/* 0x098 */ hkStringPtr m_onPreUpdateScript;
		/* 0x0A0 */ hkStringPtr m_onGenerateScript;
		/* 0x0A8 */ hkStringPtr m_onHandleEventScript;
		/* 0x0B0 */ hkStringPtr m_onDeactivateScript;
		/* 0x0B8 */ std::int32_t m_refOnActivate;
		/* 0x0BC */ std::int32_t m_refOnPreUpdate;
		/* 0x0C0 */ std::int32_t m_refOnGenerate;
		/* 0x0C4 */ std::int32_t m_refOnHandleEvent;
		/* 0x0C8 */ std::int32_t m_refOnDeactivate;
		/* 0x0CC */ float m_timeStep;
	};
	static_assert(sizeof(hkbScriptGenerator) == 0xD0);
	static_assert(offsetof(hkbScriptGenerator, m_child) == 0x88);
	static_assert(offsetof(hkbScriptGenerator, m_onActivateScript) == 0x90);
	static_assert(offsetof(hkbScriptGenerator, m_onPreUpdateScript) == 0x98);
	static_assert(offsetof(hkbScriptGenerator, m_onGenerateScript) == 0xA0);
	static_assert(offsetof(hkbScriptGenerator, m_onHandleEventScript) == 0xA8);
	static_assert(offsetof(hkbScriptGenerator, m_onDeactivateScript) == 0xB0);
	static_assert(offsetof(hkbScriptGenerator, m_refOnActivate) == 0xB8);
	static_assert(offsetof(hkbScriptGenerator, m_refOnPreUpdate) == 0xBC);
	static_assert(offsetof(hkbScriptGenerator, m_refOnGenerate) == 0xC0);
	static_assert(offsetof(hkbScriptGenerator, m_refOnHandleEvent) == 0xC4);
	static_assert(offsetof(hkbScriptGenerator, m_refOnDeactivate) == 0xC8);
	static_assert(offsetof(hkbScriptGenerator, m_timeStep) == 0xCC);

	class hkbStateMachine : public hkbGenerator
	{
	public:
		using EventPropertyArray = hkbStateMachineEventPropertyArray;
		using StateInfo = hkbStateMachineStateInfo;
		using TransitionInfoArray = hkbStateMachineTransitionInfoArray;
		enum class StartStateMode : std::int32_t
		{
			START_STATE_MODE_DEFAULT = 0,
			START_STATE_MODE_SYNC = 1,
			START_STATE_MODE_RANDOM = 2,
			START_STATE_MODE_CHOOSER = 3,
		};

		enum class StateMachineSelfTransitionMode : std::int32_t
		{
			SELF_TRANSITION_MODE_NO_TRANSITION = 0,
			SELF_TRANSITION_MODE_TRANSITION_TO_START_STATE = 1,
			SELF_TRANSITION_MODE_FORCE_TRANSITION_TO_START_STATE = 2,
		};

		/* 0x088 */ hkbEvent m_eventToSendWhenStateOrTransitionChanges;
		/* 0x0A0 */ hkbCustomIdSelector* m_startStateIdSelector;
		/* 0x0A8 */ std::int32_t m_startStateId;
		/* 0x0AC */ std::int32_t m_returnToPreviousStateEventId;
		/* 0x0B0 */ std::int32_t m_randomTransitionEventId;
		/* 0x0B4 */ std::int32_t m_transitionToNextHigherStateEventId;
		/* 0x0B8 */ std::int32_t m_transitionToNextLowerStateEventId;
		/* 0x0BC */ std::int32_t m_syncVariableIndex;
		/* 0x0C0 */ std::int32_t m_currentStateId;
		/* 0x0C4 */ hkBool m_wrapAroundStateId;
		/* 0x0C5 */ std::int8_t m_maxSimultaneousTransitions;
		/* 0x0C6 */ std::int8_t m_startStateMode;
		/* 0x0C7 */ std::int8_t m_selfTransitionMode;
		/* 0x0C8 */ hkBool m_isActive;
		/* 0x0C9 */ std::uint8_t pad0C9[0x7];
		/* 0x0D0 */ hkArray<hkbStateMachineStateInfo*> m_states;
		/* 0x0E0 */ hkbStateMachineTransitionInfoArray* m_wildcardTransitions;
		/* 0x0E8 */ void* m_stateIdToIndexMap;
		/* 0x0F0 */ hkArray<void> m_activeTransitions;
		/* 0x100 */ hkArray<void> m_transitionFlags;
		/* 0x110 */ hkArray<void> m_wildcardTransitionFlags;
		/* 0x120 */ hkArray<void> m_delayedTransitions;
		/* 0x130 */ float m_timeInState;
		/* 0x134 */ float m_lastLocalTime;
		/* 0x138 */ std::int32_t m_previousStateId;
		/* 0x13C */ std::int32_t m_nextStartStateIndexOverride;
		/* 0x140 */ hkBool m_stateOrTransitionChanged;
		/* 0x141 */ hkBool m_echoNextUpdate;
		/* 0x142 */ hkBool m_hasEventlessWildcardTransitions;
		/* 0x143 */ std::uint8_t pad143[0x1];
		/* 0x144 */ std::uint16_t m_sCurrentStateIndexAndEntered;
		/* 0x146 */ std::uint8_t pad146[0x2];
	};
	static_assert(sizeof(hkbStateMachine) == 0x148);
	static_assert(offsetof(hkbStateMachine, m_eventToSendWhenStateOrTransitionChanges) == 0x88);
	static_assert(offsetof(hkbStateMachine, m_startStateIdSelector) == 0xA0);
	static_assert(offsetof(hkbStateMachine, m_startStateId) == 0xA8);
	static_assert(offsetof(hkbStateMachine, m_returnToPreviousStateEventId) == 0xAC);
	static_assert(offsetof(hkbStateMachine, m_randomTransitionEventId) == 0xB0);
	static_assert(offsetof(hkbStateMachine, m_transitionToNextHigherStateEventId) == 0xB4);
	static_assert(offsetof(hkbStateMachine, m_transitionToNextLowerStateEventId) == 0xB8);
	static_assert(offsetof(hkbStateMachine, m_syncVariableIndex) == 0xBC);
	static_assert(offsetof(hkbStateMachine, m_currentStateId) == 0xC0);
	static_assert(offsetof(hkbStateMachine, m_wrapAroundStateId) == 0xC4);
	static_assert(offsetof(hkbStateMachine, m_maxSimultaneousTransitions) == 0xC5);
	static_assert(offsetof(hkbStateMachine, m_startStateMode) == 0xC6);
	static_assert(offsetof(hkbStateMachine, m_selfTransitionMode) == 0xC7);
	static_assert(offsetof(hkbStateMachine, m_isActive) == 0xC8);
	static_assert(offsetof(hkbStateMachine, m_states) == 0xD0);
	static_assert(offsetof(hkbStateMachine, m_wildcardTransitions) == 0xE0);
	static_assert(offsetof(hkbStateMachine, m_stateIdToIndexMap) == 0xE8);
	static_assert(offsetof(hkbStateMachine, m_activeTransitions) == 0xF0);
	static_assert(offsetof(hkbStateMachine, m_transitionFlags) == 0x100);
	static_assert(offsetof(hkbStateMachine, m_wildcardTransitionFlags) == 0x110);
	static_assert(offsetof(hkbStateMachine, m_delayedTransitions) == 0x120);
	static_assert(offsetof(hkbStateMachine, m_timeInState) == 0x130);
	static_assert(offsetof(hkbStateMachine, m_lastLocalTime) == 0x134);
	static_assert(offsetof(hkbStateMachine, m_previousStateId) == 0x138);
	static_assert(offsetof(hkbStateMachine, m_nextStartStateIndexOverride) == 0x13C);
	static_assert(offsetof(hkbStateMachine, m_stateOrTransitionChanged) == 0x140);
	static_assert(offsetof(hkbStateMachine, m_echoNextUpdate) == 0x141);
	static_assert(offsetof(hkbStateMachine, m_hasEventlessWildcardTransitions) == 0x142);
	static_assert(offsetof(hkbStateMachine, m_sCurrentStateIndexAndEntered) == 0x144);

}
