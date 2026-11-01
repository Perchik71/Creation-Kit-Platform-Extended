// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkBaseObject.h>
#include <EditorAPI/H/hkBaseTypes.h>
#include <EditorAPI/H/hkReflectionTypes.h>

namespace CKPE::Fallout4::EditorAPI
{
	class hkLocalFrame;
	class hkMultiThreadCheck;
	class hkReferencedObject;
	class hkaSkeleton;
	class hkaSkeletonMapper;
	class hkbAiControlCancelPathCommand;
	class hkbAiControlControlData;
	class hkbAiControlControlDataBlendable;
	class hkbAiControlControlDataNonBlendable;
	class hkbAiControlPathToCommand;
	class hkbAssetBundle;
	class hkbAssetBundleStringData;
	class hkbAttachmentSetup;
	class hkbAttributeModifierAssignment;
	class hkbAuxiliaryNodeInfo;
	class hkbBehaviorEventsInfo;
	class hkbBehaviorGraph;
	class hkbBehaviorGraphData;
	class hkbBehaviorGraphInternalState;
	class hkbBehaviorGraphInternalStateInfo;
	class hkbBehaviorGraphStringData;
	class hkbBehaviorInfo;
	class hkbBehaviorInfoIdToNamePair;
	class hkbBindable;
	class hkbBlenderGeneratorChild;
	class hkbBlenderGeneratorChildInternalState;
	class hkbBlenderGeneratorInternalState;
	class hkbBlendingTransitionEffectInternalState;
	class hkbBoneIndexArray;
	class hkbBoneWeightArray;
	class hkbBoolVariableSequencedData;
	class hkbBoolVariableSequencedDataSample;
	class hkbCameraShakeEventPayload;
	class hkbCameraVariablesChangedCommand;
	class hkbCharacter;
	class hkbCharacterAddedInfo;
	class hkbCharacterControlCommand;
	class hkbCharacterControllerModifierControlData;
	class hkbCharacterControllerModifierInternalState;
	class hkbCharacterControllerSetup;
	class hkbCharacterData;
	class hkbCharacterInfo;
	class hkbCharacterSelectedInfo;
	class hkbCharacterSetup;
	class hkbCharacterSkinInfo;
	class hkbCharacterSteppedInfo;
	class hkbCharacterStringData;
	class hkbCharacterStringDataFileNameMeshNamePair;
	class hkbClientCharacterState;
	class hkbClipGeneratorEcho;
	class hkbClipGeneratorInternalState;
	class hkbClipTrigger;
	class hkbClipTriggerArray;
	class hkbCombineTransformsModifierInternalState;
	class hkbCompiledExpressionSet;
	class hkbCompiledExpressionSetToken;
	class hkbComputeDirectionModifierInternalState;
	class hkbComputeRotationFromAxisAngleModifierInternalState;
	class hkbComputeRotationToTargetModifierInternalState;
	class hkbCondition;
	class hkbConstraintSetup;
	class hkbContext;
	class hkbCustomIdSelector;
	class hkbDampingModifierInternalState;
	class hkbDefaultMessageLog;
	class hkbDelayedModifierInternalState;
	class hkbDetectCloseToGroundModifierInternalState;
	class hkbDockingGeneratorInternalState;
	class hkbEvaluateExpressionModifierInternalExpressionData;
	class hkbEvaluateExpressionModifierInternalState;
	class hkbEvent;
	class hkbEventBase;
	class hkbEventDrivenModifierInternalState;
	class hkbEventInfo;
	class hkbEventPayload;
	class hkbEventPayloadList;
	class hkbEventProperty;
	class hkbEventRaisedInfo;
	class hkbEventRangeData;
	class hkbEventRangeDataArray;
	class hkbEventSequencedData;
	class hkbEventSequencedDataSequencedEvent;
	class hkbEventsFromRangeModifierInternalState;
	class hkbExpressionCondition;
	class hkbExpressionData;
	class hkbExpressionDataArray;
	class hkbExtrapolatingTransitionEffectInternalState;
	class hkbFootIkControlData;
	class hkbFootIkControlsModifierLeg;
	class hkbFootIkDriverInfo;
	class hkbFootIkDriverInfoLeg;
	class hkbFootIkGains;
	class hkbFootIkModifierInternalLegData;
	class hkbFootIkModifierLeg;
	class hkbGenerator;
	class hkbGeneratorOutputListener;
	class hkbGeneratorPartitionInfo;
	class hkbGeneratorSyncInfo;
	class hkbGeneratorSyncInfoActiveInterval;
	class hkbGeneratorSyncInfoSyncPoint;
	class hkbGeneratorTransitionEffectInternalState;
	class hkbGetUpModifierInternalState;
	class hkbGetWorldFromModelModifierInternalState;
	class hkbHandIkControlData;
	class hkbHandIkControlsModifierHand;
	class hkbHandIkDriverInfo;
	class hkbHandIkDriverInfoHand;
	class hkbHandIkModifierHand;
	class hkbHandle;
	class hkbIntEventPayload;
	class hkbIntVariableSequencedData;
	class hkbIntVariableSequencedDataSample;
	class hkbJigglerGroup;
	class hkbJigglerModifierInternalState;
	class hkbKeyFrameControlData;
	class hkbKeyframeBonesModifierKeyframeInfo;
	class hkbLayer;
	class hkbLayerGeneratorInternalState;
	class hkbLayerGeneratorLayerInternalState;
	class hkbLinkedSymbolInfo;
	class hkbLookAtModifierInternalState;
	class hkbManualSelectorGeneratorInternalState;
	class hkbManualSelectorTransitionEffectInternalState;
	class hkbMessageLog;
	class hkbMirroredSkeletonInfo;
	class hkbModifier;
	class hkbMoveCharacterModifierInternalState;
	class hkbNamedEventPayload;
	class hkbNamedIntEventPayload;
	class hkbNamedRealEventPayload;
	class hkbNamedStringEventPayload;
	class hkbNode;
	class hkbNodeInternalStateInfo;
	class hkbNullPhysicsInterface;
	class hkbParticleSystemEventPayload;
	class hkbPhysicsInterface;
	class hkbPoseMatchingGeneratorInternalState;
	class hkbPoseStoringGeneratorOutputListener;
	class hkbPoseStoringGeneratorOutputListenerStoredPose;
	class hkbPoweredRagdollControlData;
	class hkbProjectData;
	class hkbProjectStringData;
	class hkbRadialSelectorGeneratorGeneratorInfo;
	class hkbRadialSelectorGeneratorGeneratorPair;
	class hkbRagdollControllerSetup;
	class hkbRagdollInterface;
	class hkbRaiseEventCommand;
	class hkbRealEventPayload;
	class hkbRealVariableSequencedData;
	class hkbRealVariableSequencedDataSample;
	class hkbRebuildScriptsCommand;
	class hkbReferencedGeneratorSyncInfo;
	class hkbRigidBodyRagdollControlData;
	class hkbRigidBodySetup;
	class hkbRoleAttribute;
	class hkbRotateCharacterModifierInternalState;
	class hkbScriptCondition;
	class hkbSenseHandleModifierRange;
	class hkbSequenceInternalState;
	class hkbSequenceStringData;
	class hkbSequencedData;
	class hkbSetBehaviorCommand;
	class hkbSetLocalTimeOfClipGeneratorCommand;
	class hkbSetNodePropertyCommand;
	class hkbSetSelectedCharacterCommand;
	class hkbSetWordVariableCommand;
	class hkbShapeSetup;
	class hkbSimulationControlCommand;
	class hkbSimulationStateInfo;
	class hkbStateChooser;
	class hkbStateChooserWrapper;
	class hkbStateListener;
	class hkbStateMachineActiveTransitionInfo;
	class hkbStateMachineDelayedTransitionInfo;
	class hkbStateMachineEventPropertyArray;
	class hkbStateMachineInternalState;
	class hkbStateMachineNestedStateMachineData;
	class hkbStateMachineProspectiveTransitionInfo;
	class hkbStateMachineStateInfo;
	class hkbStateMachineTimeInterval;
	class hkbStateMachineTransitionInfo;
	class hkbStateMachineTransitionInfoArray;
	class hkbStateMachineTransitionInfoReference;
	class hkbStringCondition;
	class hkbStringEventPayload;
	class hkbTestIdSelector;
	class hkbTimerModifierInternalState;
	class hkbTransformVectorModifierInternalState;
	class hkbTransitionEffect;
	class hkbVariableBindingSet;
	class hkbVariableBindingSetBinding;
	class hkbVariableBounds;
	class hkbVariableInfo;
	class hkbVariableValue;
	class hkbVariableValueSet;
	class hkbWorldFromModelModeData;
	class hkbnpPhysicsInterface;
	class hkbnpRagdollInterface;

	class hkbAiControlCancelPathCommand : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
	};
	static_assert(sizeof(hkbAiControlCancelPathCommand) == 0x18);
	static_assert(offsetof(hkbAiControlCancelPathCommand, m_characterId) == 0x10);

	class hkbAiControlControlDataBlendable
	{
	public:
		/* 0x000 */ float m_desiredSpeed;
		/* 0x004 */ float m_maximumSpeed;
	};
	static_assert(sizeof(hkbAiControlControlDataBlendable) == 0x8);
	static_assert(offsetof(hkbAiControlControlDataBlendable, m_desiredSpeed) == 0x0);
	static_assert(offsetof(hkbAiControlControlDataBlendable, m_maximumSpeed) == 0x4);

	class hkbAiControlControlDataNonBlendable
	{
	public:
		/* 0x000 */ hkBool m_canControl;
		/* 0x001 */ std::uint8_t m_padding[3];
	};
	static_assert(sizeof(hkbAiControlControlDataNonBlendable) == 0x4);
	static_assert(offsetof(hkbAiControlControlDataNonBlendable, m_canControl) == 0x0);
	static_assert(offsetof(hkbAiControlControlDataNonBlendable, m_padding) == 0x1);

	class hkbAiControlControlData
	{
	public:
		/* 0x000 */ hkbAiControlControlDataBlendable m_blendable;
		/* 0x008 */ hkbAiControlControlDataNonBlendable m_nonBlendable;
	};
	static_assert(sizeof(hkbAiControlControlData) == 0xC);
	static_assert(offsetof(hkbAiControlControlData, m_blendable) == 0x0);
	static_assert(offsetof(hkbAiControlControlData, m_nonBlendable) == 0x8);

	class hkbAiControlPathToCommand : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
		/* 0x018 */ std::uint8_t pad018[0x8];
		/* 0x020 */ hkVector4f m_goalPoint;
	};
	static_assert(sizeof(hkbAiControlPathToCommand) == 0x30);
	static_assert(offsetof(hkbAiControlPathToCommand, m_characterId) == 0x10);
	static_assert(offsetof(hkbAiControlPathToCommand, m_goalPoint) == 0x20);

	class hkbAssetBundle : public hkReferencedObject
	{
	public:
		enum class BundleType : std::int32_t
		{
			ANIMATION_BUNDLE = 0,
		};

		/* 0x010 */ hkArray<hkReferencedObject*> m_assets;
		/* 0x020 */ hkStringPtr m_name;
		/* 0x028 */ std::int8_t m_type;
		/* 0x029 */ std::uint8_t pad029[0x7];
	};
	static_assert(sizeof(hkbAssetBundle) == 0x30);
	static_assert(offsetof(hkbAssetBundle, m_assets) == 0x10);
	static_assert(offsetof(hkbAssetBundle, m_name) == 0x20);
	static_assert(offsetof(hkbAssetBundle, m_type) == 0x28);

	class hkbAssetBundleStringData
	{
	public:
		/* 0x000 */ hkStringPtr m_bundleName;
		/* 0x008 */ hkArray<hkStringPtr> m_assetNames;
	};
	static_assert(sizeof(hkbAssetBundleStringData) == 0x18);
	static_assert(offsetof(hkbAssetBundleStringData, m_bundleName) == 0x0);
	static_assert(offsetof(hkbAssetBundleStringData, m_assetNames) == 0x8);

	class hkbAttachmentSetup : public hkReferencedObject
	{
	public:
		enum class AttachmentType : std::int32_t
		{
			ATTACHMENT_TYPE_KEYFRAME_RIGID_BODY = 0,
			ATTACHMENT_TYPE_BALL_SOCKET_CONSTRAINT = 1,
			ATTACHMENT_TYPE_RAGDOLL_CONSTRAINT = 2,
			ATTACHMENT_TYPE_SET_WORLD_FROM_MODEL = 3,
			ATTACHMENT_TYPE_NONE = 4,
		};

		/* 0x010 */ float m_blendInTime;
		/* 0x014 */ float m_moveAttacherFraction;
		/* 0x018 */ float m_gain;
		/* 0x01C */ float m_extrapolationTimeStep;
		/* 0x020 */ float m_fixUpGain;
		/* 0x024 */ float m_maxLinearDistance;
		/* 0x028 */ float m_maxAngularDistance;
		/* 0x02C */ std::int8_t m_attachmentType;
		/* 0x02D */ std::uint8_t pad02D[0x3];
	};
	static_assert(sizeof(hkbAttachmentSetup) == 0x30);
	static_assert(offsetof(hkbAttachmentSetup, m_blendInTime) == 0x10);
	static_assert(offsetof(hkbAttachmentSetup, m_moveAttacherFraction) == 0x14);
	static_assert(offsetof(hkbAttachmentSetup, m_gain) == 0x18);
	static_assert(offsetof(hkbAttachmentSetup, m_extrapolationTimeStep) == 0x1C);
	static_assert(offsetof(hkbAttachmentSetup, m_fixUpGain) == 0x20);
	static_assert(offsetof(hkbAttachmentSetup, m_maxLinearDistance) == 0x24);
	static_assert(offsetof(hkbAttachmentSetup, m_maxAngularDistance) == 0x28);
	static_assert(offsetof(hkbAttachmentSetup, m_attachmentType) == 0x2C);

	class hkbAttributeModifierAssignment
	{
	public:
		/* 0x000 */ std::int32_t m_attributeIndex;
		/* 0x004 */ float m_attributeValue;
	};
	static_assert(sizeof(hkbAttributeModifierAssignment) == 0x8);
	static_assert(offsetof(hkbAttributeModifierAssignment, m_attributeIndex) == 0x0);
	static_assert(offsetof(hkbAttributeModifierAssignment, m_attributeValue) == 0x4);

	class hkbAuxiliaryNodeInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint16_t m_type;
		/* 0x012 */ std::uint8_t m_depth;
		/* 0x013 */ std::uint8_t pad013[0x5];
		/* 0x018 */ hkStringPtr m_referenceBehaviorName;
		/* 0x020 */ hkArray<hkStringPtr> m_selfTransitionNames;
	};
	static_assert(sizeof(hkbAuxiliaryNodeInfo) == 0x30);
	static_assert(offsetof(hkbAuxiliaryNodeInfo, m_type) == 0x10);
	static_assert(offsetof(hkbAuxiliaryNodeInfo, m_depth) == 0x12);
	static_assert(offsetof(hkbAuxiliaryNodeInfo, m_referenceBehaviorName) == 0x18);
	static_assert(offsetof(hkbAuxiliaryNodeInfo, m_selfTransitionNames) == 0x20);

	class hkbBehaviorEventsInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
		/* 0x018 */ hkArray<std::int16_t> m_externalEventIds;
		/* 0x028 */ std::int32_t m_padding;
		/* 0x02C */ std::uint8_t pad02C[0x4];
	};
	static_assert(sizeof(hkbBehaviorEventsInfo) == 0x30);
	static_assert(offsetof(hkbBehaviorEventsInfo, m_characterId) == 0x10);
	static_assert(offsetof(hkbBehaviorEventsInfo, m_externalEventIds) == 0x18);
	static_assert(offsetof(hkbBehaviorEventsInfo, m_padding) == 0x28);

	class hkbBehaviorGraphData : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<float> m_attributeDefaults;
		/* 0x020 */ hkArray<hkbVariableInfo> m_variableInfos;
		/* 0x030 */ hkArray<hkbVariableInfo> m_characterPropertyInfos;
		/* 0x040 */ hkArray<hkbEventInfo> m_eventInfos;
		/* 0x050 */ hkArray<hkbVariableBounds> m_variableBounds;
		/* 0x060 */ hkbVariableValueSet* m_variableInitialValues;
		/* 0x068 */ hkbBehaviorGraphStringData* m_stringData;
	};
	static_assert(sizeof(hkbBehaviorGraphData) == 0x70);
	static_assert(offsetof(hkbBehaviorGraphData, m_attributeDefaults) == 0x10);
	static_assert(offsetof(hkbBehaviorGraphData, m_variableInfos) == 0x20);
	static_assert(offsetof(hkbBehaviorGraphData, m_characterPropertyInfos) == 0x30);
	static_assert(offsetof(hkbBehaviorGraphData, m_eventInfos) == 0x40);
	static_assert(offsetof(hkbBehaviorGraphData, m_variableBounds) == 0x50);
	static_assert(offsetof(hkbBehaviorGraphData, m_variableInitialValues) == 0x60);
	static_assert(offsetof(hkbBehaviorGraphData, m_stringData) == 0x68);

	class hkbBehaviorGraphInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkbNodeInternalStateInfo*> m_nodeInternalStateInfos;
		/* 0x020 */ hkbVariableValueSet* m_variableValueSet;
	};
	static_assert(sizeof(hkbBehaviorGraphInternalState) == 0x28);
	static_assert(offsetof(hkbBehaviorGraphInternalState, m_nodeInternalStateInfos) == 0x10);
	static_assert(offsetof(hkbBehaviorGraphInternalState, m_variableValueSet) == 0x20);

	class hkbBehaviorGraphInternalStateInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
		/* 0x018 */ hkbBehaviorGraphInternalState* m_internalState;
		/* 0x020 */ hkArray<hkbAuxiliaryNodeInfo*> m_auxiliaryNodeInfo;
		/* 0x030 */ hkArray<std::int16_t> m_activeEventIds;
		/* 0x040 */ hkArray<std::int16_t> m_activeVariableIds;
	};
	static_assert(sizeof(hkbBehaviorGraphInternalStateInfo) == 0x50);
	static_assert(offsetof(hkbBehaviorGraphInternalStateInfo, m_characterId) == 0x10);
	static_assert(offsetof(hkbBehaviorGraphInternalStateInfo, m_internalState) == 0x18);
	static_assert(offsetof(hkbBehaviorGraphInternalStateInfo, m_auxiliaryNodeInfo) == 0x20);
	static_assert(offsetof(hkbBehaviorGraphInternalStateInfo, m_activeEventIds) == 0x30);
	static_assert(offsetof(hkbBehaviorGraphInternalStateInfo, m_activeVariableIds) == 0x40);

	class hkbBehaviorGraphStringData : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkStringPtr> m_eventNames;
		/* 0x020 */ hkArray<hkStringPtr> m_attributeNames;
		/* 0x030 */ hkArray<hkStringPtr> m_variableNames;
		/* 0x040 */ hkArray<hkStringPtr> m_characterPropertyNames;
	};
	static_assert(sizeof(hkbBehaviorGraphStringData) == 0x50);
	static_assert(offsetof(hkbBehaviorGraphStringData, m_eventNames) == 0x10);
	static_assert(offsetof(hkbBehaviorGraphStringData, m_attributeNames) == 0x20);
	static_assert(offsetof(hkbBehaviorGraphStringData, m_variableNames) == 0x30);
	static_assert(offsetof(hkbBehaviorGraphStringData, m_characterPropertyNames) == 0x40);

	class hkbBehaviorInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
		/* 0x018 */ hkbBehaviorGraphData* m_data;
		/* 0x020 */ hkArray<hkbBehaviorInfoIdToNamePair> m_idToNamePairs;
	};
	static_assert(sizeof(hkbBehaviorInfo) == 0x30);
	static_assert(offsetof(hkbBehaviorInfo, m_characterId) == 0x10);
	static_assert(offsetof(hkbBehaviorInfo, m_data) == 0x18);
	static_assert(offsetof(hkbBehaviorInfo, m_idToNamePairs) == 0x20);

	class hkbBehaviorInfoIdToNamePair
	{
	public:
		/* 0x000 */ hkStringPtr m_behaviorName;
		/* 0x008 */ hkStringPtr m_nodeName;
		/* 0x010 */ std::uint16_t m_toolType;
		/* 0x012 */ std::uint16_t m_id;
		/* 0x014 */ std::uint8_t pad014[0x4];
	};
	static_assert(sizeof(hkbBehaviorInfoIdToNamePair) == 0x18);
	static_assert(offsetof(hkbBehaviorInfoIdToNamePair, m_behaviorName) == 0x0);
	static_assert(offsetof(hkbBehaviorInfoIdToNamePair, m_nodeName) == 0x8);
	static_assert(offsetof(hkbBehaviorInfoIdToNamePair, m_toolType) == 0x10);
	static_assert(offsetof(hkbBehaviorInfoIdToNamePair, m_id) == 0x12);

	class hkbBindable : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkbVariableBindingSet* m_variableBindingSet;
		/* 0x018 */ hkArray<void*> m_cachedBindables;
		/* 0x028 */ hkBool m_areBindablesCached;
		/* 0x029 */ hkBool m_hasEnableChanged;
		/* 0x02A */ std::uint8_t pad02A[0x6];

	private:
		/* 04 */ virtual void sub_hkbBindable_04();
		/* 05 */ virtual void sub_hkbBindable_05();
		/* 06 */ virtual void sub_hkbBindable_06();
	};
	static_assert(sizeof(hkbBindable) == 0x30);
	static_assert(offsetof(hkbBindable, m_variableBindingSet) == 0x10);
	static_assert(offsetof(hkbBindable, m_cachedBindables) == 0x18);
	static_assert(offsetof(hkbBindable, m_areBindablesCached) == 0x28);
	static_assert(offsetof(hkbBindable, m_hasEnableChanged) == 0x29);

	class hkbBlenderGeneratorChild : public hkbBindable
	{
	public:
		/* 0x030 */ hkbGenerator* m_generator;
		/* 0x038 */ hkbBoneWeightArray* m_boneWeights;
		/* 0x040 */ float m_weight;
		/* 0x044 */ float m_worldFromModelWeight;
		/* 0x048 */ std::uint8_t pad048[0x8];
	};
	static_assert(sizeof(hkbBlenderGeneratorChild) == 0x50);
	static_assert(offsetof(hkbBlenderGeneratorChild, m_generator) == 0x30);
	static_assert(offsetof(hkbBlenderGeneratorChild, m_boneWeights) == 0x38);
	static_assert(offsetof(hkbBlenderGeneratorChild, m_weight) == 0x40);
	static_assert(offsetof(hkbBlenderGeneratorChild, m_worldFromModelWeight) == 0x44);

	class hkbBlenderGeneratorChildInternalState
	{
	public:
		/* 0x000 */ hkBool m_isActive;
		/* 0x001 */ hkBool m_syncNextFrame;
	};
	static_assert(sizeof(hkbBlenderGeneratorChildInternalState) == 0x2);
	static_assert(offsetof(hkbBlenderGeneratorChildInternalState, m_isActive) == 0x0);
	static_assert(offsetof(hkbBlenderGeneratorChildInternalState, m_syncNextFrame) == 0x1);

	class hkbBlenderGeneratorInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkbBlenderGeneratorChildInternalState> m_childrenInternalStates;
		/* 0x020 */ hkArray<std::int16_t> m_sortedChildren;
		/* 0x030 */ float m_endIntervalWeight;
		/* 0x034 */ std::int32_t m_numActiveChildren;
		/* 0x038 */ std::int16_t m_beginIntervalIndex;
		/* 0x03A */ std::int16_t m_endIntervalIndex;
		/* 0x03C */ hkBool m_initSync;
		/* 0x03D */ hkBool m_doSubtractiveBlend;
		/* 0x03E */ std::uint8_t pad03E[0x2];
	};
	static_assert(sizeof(hkbBlenderGeneratorInternalState) == 0x40);
	static_assert(offsetof(hkbBlenderGeneratorInternalState, m_childrenInternalStates) == 0x10);
	static_assert(offsetof(hkbBlenderGeneratorInternalState, m_sortedChildren) == 0x20);
	static_assert(offsetof(hkbBlenderGeneratorInternalState, m_endIntervalWeight) == 0x30);
	static_assert(offsetof(hkbBlenderGeneratorInternalState, m_numActiveChildren) == 0x34);
	static_assert(offsetof(hkbBlenderGeneratorInternalState, m_beginIntervalIndex) == 0x38);
	static_assert(offsetof(hkbBlenderGeneratorInternalState, m_endIntervalIndex) == 0x3A);
	static_assert(offsetof(hkbBlenderGeneratorInternalState, m_initSync) == 0x3C);
	static_assert(offsetof(hkbBlenderGeneratorInternalState, m_doSubtractiveBlend) == 0x3D);

	class hkbBlendingTransitionEffectInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkVector4f m_fromPos;
		/* 0x020 */ hkQuaternionf m_fromRot;
		/* 0x030 */ hkVector4f m_toPos;
		/* 0x040 */ hkQuaternionf m_toRot;
		/* 0x050 */ hkVector4f m_lastPos;
		/* 0x060 */ hkQuaternionf m_lastRot;
		/* 0x070 */ hkArray<hkQsTransformf> m_characterPoseAtBeginningOfTransition;
		/* 0x080 */ float m_timeRemaining;
		/* 0x084 */ float m_timeInTransition;
		/* 0x088 */ std::int8_t m_toGeneratorSelfTranstitionMode;
		/* 0x089 */ hkBool m_initializeCharacterPose;
		/* 0x08A */ hkBool m_alignThisFrame;
		/* 0x08B */ hkBool m_alignmentFinished;
		/* 0x08C */ std::uint8_t pad08C[0x4];
	};
	static_assert(sizeof(hkbBlendingTransitionEffectInternalState) == 0x90);
	static_assert(offsetof(hkbBlendingTransitionEffectInternalState, m_fromPos) == 0x10);
	static_assert(offsetof(hkbBlendingTransitionEffectInternalState, m_fromRot) == 0x20);
	static_assert(offsetof(hkbBlendingTransitionEffectInternalState, m_toPos) == 0x30);
	static_assert(offsetof(hkbBlendingTransitionEffectInternalState, m_toRot) == 0x40);
	static_assert(offsetof(hkbBlendingTransitionEffectInternalState, m_lastPos) == 0x50);
	static_assert(offsetof(hkbBlendingTransitionEffectInternalState, m_lastRot) == 0x60);
	static_assert(offsetof(hkbBlendingTransitionEffectInternalState, m_characterPoseAtBeginningOfTransition) == 0x70);
	static_assert(offsetof(hkbBlendingTransitionEffectInternalState, m_timeRemaining) == 0x80);
	static_assert(offsetof(hkbBlendingTransitionEffectInternalState, m_timeInTransition) == 0x84);
	static_assert(offsetof(hkbBlendingTransitionEffectInternalState, m_toGeneratorSelfTranstitionMode) == 0x88);
	static_assert(offsetof(hkbBlendingTransitionEffectInternalState, m_initializeCharacterPose) == 0x89);
	static_assert(offsetof(hkbBlendingTransitionEffectInternalState, m_alignThisFrame) == 0x8A);
	static_assert(offsetof(hkbBlendingTransitionEffectInternalState, m_alignmentFinished) == 0x8B);

	class hkbBoneIndexArray : public hkbBindable
	{
	public:
		/* 0x030 */ hkArray<std::int16_t> m_boneIndices;
	};
	static_assert(sizeof(hkbBoneIndexArray) == 0x40);
	static_assert(offsetof(hkbBoneIndexArray, m_boneIndices) == 0x30);

	class hkbBoneWeightArray : public hkbBindable
	{
	public:
		/* 0x030 */ hkArray<float> m_boneWeights;
	};
	static_assert(sizeof(hkbBoneWeightArray) == 0x40);
	static_assert(offsetof(hkbBoneWeightArray, m_boneWeights) == 0x30);

	class hkbSequencedData : public hkReferencedObject
	{
	public:

	private:
		/* 04 */ virtual void sub_hkbSequencedData_04();
	};
	static_assert(sizeof(hkbSequencedData) == 0x10);

	class hkbBoolVariableSequencedData : public hkbSequencedData
	{
	public:
		/* 0x010 */ hkArray<hkbBoolVariableSequencedDataSample> m_samples;
		/* 0x020 */ std::int32_t m_variableIndex;
		/* 0x024 */ std::uint8_t pad024[0x4];
	};
	static_assert(sizeof(hkbBoolVariableSequencedData) == 0x28);
	static_assert(offsetof(hkbBoolVariableSequencedData, m_samples) == 0x10);
	static_assert(offsetof(hkbBoolVariableSequencedData, m_variableIndex) == 0x20);

	class hkbBoolVariableSequencedDataSample
	{
	public:
		/* 0x000 */ float m_time;
		/* 0x004 */ hkBool m_value;
		/* 0x005 */ std::uint8_t pad005[0x3];
	};
	static_assert(sizeof(hkbBoolVariableSequencedDataSample) == 0x8);
	static_assert(offsetof(hkbBoolVariableSequencedDataSample, m_time) == 0x0);
	static_assert(offsetof(hkbBoolVariableSequencedDataSample, m_value) == 0x4);

	class hkbEventPayload : public hkReferencedObject
	{
	public:
	};
	static_assert(sizeof(hkbEventPayload) == 0x10);

	class hkbCameraShakeEventPayload : public hkbEventPayload
	{
	public:
		/* 0x010 */ float m_amplitude;
		/* 0x014 */ float m_halfLife;
	};
	static_assert(sizeof(hkbCameraShakeEventPayload) == 0x18);
	static_assert(offsetof(hkbCameraShakeEventPayload, m_amplitude) == 0x10);
	static_assert(offsetof(hkbCameraShakeEventPayload, m_halfLife) == 0x14);

	class hkbCameraVariablesChangedCommand : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkStringPtr> m_cameraVariableFloatNames;
		/* 0x020 */ hkArray<float> m_cameraFloatValues;
		/* 0x030 */ hkArray<hkStringPtr> m_cameraVariableVectorNames;
		/* 0x040 */ hkArray<hkVector4f> m_cameraVectorValues;
	};
	static_assert(sizeof(hkbCameraVariablesChangedCommand) == 0x50);
	static_assert(offsetof(hkbCameraVariablesChangedCommand, m_cameraVariableFloatNames) == 0x10);
	static_assert(offsetof(hkbCameraVariablesChangedCommand, m_cameraFloatValues) == 0x20);
	static_assert(offsetof(hkbCameraVariablesChangedCommand, m_cameraVariableVectorNames) == 0x30);
	static_assert(offsetof(hkbCameraVariablesChangedCommand, m_cameraVectorValues) == 0x40);

	class hkbCharacter : public hkReferencedObject
	{
	public:
		enum class CapabilityTypes : std::int32_t
		{
			FULL_POSE = 1,
			MIRRORING = 2,
			DOCKING = 4,
			HAND_IK = 8,
			CHARACTER_CONTROL = 16,
			RAGDOLL = 32,
			FOOT_IK = 64,
			AI_CONTROL = 128,
			BASIC_CAPABILITIES = 7,
			PHYSICS_CAPABILITIES = 120,
			STANDARD_CAPABILITIES = 127,
		};

		/* 0x010 */ hkArray<hkbCharacter*> m_nearbyCharacters;
		/* 0x020 */ std::uintptr_t m_userData;
		/* 0x028 */ std::int16_t m_currentLod;
		/* 0x02A */ std::int16_t m_numTracksInLod;
		/* 0x02C */ std::uint8_t pad02C[0x4];
		/* 0x030 */ void* m_generatorOutput;
		/* 0x038 */ hkStringPtr m_name;
		/* 0x040 */ void* m_ragdollDriver;
		/* 0x048 */ void* m_ragdollInterface;
		/* 0x050 */ void* m_characterControllerDriver;
		/* 0x058 */ void* m_footIkDriver;
		/* 0x060 */ void* m_handIkDriver;
		/* 0x068 */ void* m_dockingDriver;
		/* 0x070 */ void* m_aiControlDriver;
		/* 0x078 */ hkbCharacterSetup* m_setup;
		/* 0x080 */ hkbBehaviorGraph* m_behaviorGraph;
		/* 0x088 */ hkbProjectData* m_projectData;
		/* 0x090 */ void* m_animationBindingSet;
		/* 0x098 */ void* m_spatialQueryInterface;
		/* 0x0A0 */ void* m_world;
		/* 0x0A8 */ hkArray<void*> m_boneAttachments;
		/* 0x0B8 */ void* m_eventQueue;
		/* 0x0C0 */ void* m_characterLuaState;
		/* 0x0C8 */ void* m_assetManager;
		/* 0x0D0 */ std::int32_t m_capabilities;
		/* 0x0D4 */ std::int32_t m_effectiveCapabilities;

	private:
		/* 04 */ virtual void sub_hkbCharacter_04();
		/* 05 */ virtual void sub_hkbCharacter_05();
	};
	static_assert(sizeof(hkbCharacter) == 0xD8);
	static_assert(offsetof(hkbCharacter, m_nearbyCharacters) == 0x10);
	static_assert(offsetof(hkbCharacter, m_userData) == 0x20);
	static_assert(offsetof(hkbCharacter, m_currentLod) == 0x28);
	static_assert(offsetof(hkbCharacter, m_numTracksInLod) == 0x2A);
	static_assert(offsetof(hkbCharacter, m_generatorOutput) == 0x30);
	static_assert(offsetof(hkbCharacter, m_name) == 0x38);
	static_assert(offsetof(hkbCharacter, m_ragdollDriver) == 0x40);
	static_assert(offsetof(hkbCharacter, m_ragdollInterface) == 0x48);
	static_assert(offsetof(hkbCharacter, m_characterControllerDriver) == 0x50);
	static_assert(offsetof(hkbCharacter, m_footIkDriver) == 0x58);
	static_assert(offsetof(hkbCharacter, m_handIkDriver) == 0x60);
	static_assert(offsetof(hkbCharacter, m_dockingDriver) == 0x68);
	static_assert(offsetof(hkbCharacter, m_aiControlDriver) == 0x70);
	static_assert(offsetof(hkbCharacter, m_setup) == 0x78);
	static_assert(offsetof(hkbCharacter, m_behaviorGraph) == 0x80);
	static_assert(offsetof(hkbCharacter, m_projectData) == 0x88);
	static_assert(offsetof(hkbCharacter, m_animationBindingSet) == 0x90);
	static_assert(offsetof(hkbCharacter, m_spatialQueryInterface) == 0x98);
	static_assert(offsetof(hkbCharacter, m_world) == 0xA0);
	static_assert(offsetof(hkbCharacter, m_boneAttachments) == 0xA8);
	static_assert(offsetof(hkbCharacter, m_eventQueue) == 0xB8);
	static_assert(offsetof(hkbCharacter, m_characterLuaState) == 0xC0);
	static_assert(offsetof(hkbCharacter, m_assetManager) == 0xC8);
	static_assert(offsetof(hkbCharacter, m_capabilities) == 0xD0);
	static_assert(offsetof(hkbCharacter, m_effectiveCapabilities) == 0xD4);

	class hkbCharacterAddedInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
		/* 0x018 */ hkStringPtr m_instanceName;
		/* 0x020 */ hkStringPtr m_templateName;
		/* 0x028 */ hkStringPtr m_fullPathToProject;
		/* 0x030 */ hkStringPtr m_localScriptsPath;
		/* 0x038 */ hkStringPtr m_remoteScriptsPath;
		/* 0x040 */ hkaSkeleton* m_skeleton;
		/* 0x048 */ std::uint8_t pad048[0x8];
		/* 0x050 */ hkQsTransformf m_worldFromModel;
		/* 0x080 */ hkArray<hkQsTransformf> m_poseModelSpace;
	};
	static_assert(sizeof(hkbCharacterAddedInfo) == 0x90);
	static_assert(offsetof(hkbCharacterAddedInfo, m_characterId) == 0x10);
	static_assert(offsetof(hkbCharacterAddedInfo, m_instanceName) == 0x18);
	static_assert(offsetof(hkbCharacterAddedInfo, m_templateName) == 0x20);
	static_assert(offsetof(hkbCharacterAddedInfo, m_fullPathToProject) == 0x28);
	static_assert(offsetof(hkbCharacterAddedInfo, m_localScriptsPath) == 0x30);
	static_assert(offsetof(hkbCharacterAddedInfo, m_remoteScriptsPath) == 0x38);
	static_assert(offsetof(hkbCharacterAddedInfo, m_skeleton) == 0x40);
	static_assert(offsetof(hkbCharacterAddedInfo, m_worldFromModel) == 0x50);
	static_assert(offsetof(hkbCharacterAddedInfo, m_poseModelSpace) == 0x80);

	class hkbCharacterControlCommand : public hkReferencedObject
	{
	public:
		enum class CharacterControlCommand : std::int32_t
		{
			COMMAND_HIDE = 0,
			COMMAND_SHOW = 1,
		};

		/* 0x010 */ std::uint64_t m_characterId;
		/* 0x018 */ std::uint8_t m_command;
		/* 0x019 */ std::uint8_t pad019[0x3];
		/* 0x01C */ std::int32_t m_padding;
	};
	static_assert(sizeof(hkbCharacterControlCommand) == 0x20);
	static_assert(offsetof(hkbCharacterControlCommand, m_characterId) == 0x10);
	static_assert(offsetof(hkbCharacterControlCommand, m_command) == 0x18);
	static_assert(offsetof(hkbCharacterControlCommand, m_padding) == 0x1C);

	class hkbCharacterControllerModifierControlData
	{
	public:
		/* 0x000 */ float m_verticalGain;
		/* 0x004 */ float m_horizontalCatchUpGain;
		/* 0x008 */ float m_maxVerticalSeparation;
		/* 0x00C */ float m_maxHorizontalSeparation;
	};
	static_assert(sizeof(hkbCharacterControllerModifierControlData) == 0x10);
	static_assert(offsetof(hkbCharacterControllerModifierControlData, m_verticalGain) == 0x0);
	static_assert(offsetof(hkbCharacterControllerModifierControlData, m_horizontalCatchUpGain) == 0x4);
	static_assert(offsetof(hkbCharacterControllerModifierControlData, m_maxVerticalSeparation) == 0x8);
	static_assert(offsetof(hkbCharacterControllerModifierControlData, m_maxHorizontalSeparation) == 0xC);

	class hkbCharacterControllerModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkVector4f m_gravity;
		/* 0x020 */ hkBool m_isInitialVelocityAdded;
		/* 0x021 */ hkBool m_isTouchingGround;
		/* 0x022 */ std::uint8_t pad022[0xE];
	};
	static_assert(sizeof(hkbCharacterControllerModifierInternalState) == 0x30);
	static_assert(offsetof(hkbCharacterControllerModifierInternalState, m_gravity) == 0x10);
	static_assert(offsetof(hkbCharacterControllerModifierInternalState, m_isInitialVelocityAdded) == 0x20);
	static_assert(offsetof(hkbCharacterControllerModifierInternalState, m_isTouchingGround) == 0x21);

	class hkbShapeSetup
	{
	public:
		enum class Type : std::int32_t
		{
			CAPSULE = 0,
			FILE = 1,
		};

		/* 0x000 */ float m_capsuleHeight;
		/* 0x004 */ float m_capsuleRadius;
		/* 0x008 */ hkStringPtr m_fileName;
		/* 0x010 */ std::int8_t m_type;
		/* 0x011 */ std::uint8_t pad011[0x7];
	};
	static_assert(sizeof(hkbShapeSetup) == 0x18);
	static_assert(offsetof(hkbShapeSetup, m_capsuleHeight) == 0x0);
	static_assert(offsetof(hkbShapeSetup, m_capsuleRadius) == 0x4);
	static_assert(offsetof(hkbShapeSetup, m_fileName) == 0x8);
	static_assert(offsetof(hkbShapeSetup, m_type) == 0x10);

	class hkbRigidBodySetup
	{
	public:
		enum class Type : std::int32_t
		{
			INVALID = -1,
			KEYFRAMED = 0,
			DYNAMIC = 1,
			FIXED = 2,
		};

		/* 0x000 */ std::uint32_t m_collisionFilterInfo;
		/* 0x004 */ std::int8_t m_type;
		/* 0x005 */ std::uint8_t pad005[0x3];
		/* 0x008 */ hkbShapeSetup m_shapeSetup;
	};
	static_assert(sizeof(hkbRigidBodySetup) == 0x20);
	static_assert(offsetof(hkbRigidBodySetup, m_collisionFilterInfo) == 0x0);
	static_assert(offsetof(hkbRigidBodySetup, m_type) == 0x4);
	static_assert(offsetof(hkbRigidBodySetup, m_shapeSetup) == 0x8);

	class hkbCharacterControllerSetup
	{
	public:
		/* 0x000 */ hkbRigidBodySetup m_rigidBodySetup;
		/* 0x020 */ hkReferencedObject* m_controllerCinfo;
	};
	static_assert(sizeof(hkbCharacterControllerSetup) == 0x28);
	static_assert(offsetof(hkbCharacterControllerSetup, m_rigidBodySetup) == 0x0);
	static_assert(offsetof(hkbCharacterControllerSetup, m_controllerCinfo) == 0x20);

	class hkbCharacterData : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkbCharacterControllerSetup m_characterControllerSetup;
		/* 0x038 */ std::uint8_t pad038[0x8];
		/* 0x040 */ hkVector4f m_modelUpMS;
		/* 0x050 */ hkVector4f m_modelForwardMS;
		/* 0x060 */ hkVector4f m_modelRightMS;
		/* 0x070 */ hkArray<hkbVariableInfo> m_characterPropertyInfos;
		/* 0x080 */ hkArray<std::int32_t> m_numBonesPerLod;
		/* 0x090 */ hkbVariableValueSet* m_characterPropertyValues;
		/* 0x098 */ hkbFootIkDriverInfo* m_footIkDriverInfo;
		/* 0x0A0 */ hkbHandIkDriverInfo* m_handIkDriverInfo;
		/* 0x0A8 */ hkReferencedObject* m_aiControlDriverInfo;
		/* 0x0B0 */ hkbCharacterStringData* m_stringData;
		/* 0x0B8 */ hkbMirroredSkeletonInfo* m_mirroredSkeletonInfo;
		/* 0x0C0 */ hkArray<std::int16_t> m_boneAttachmentBoneIndices;
		/* 0x0D0 */ hkArray<hkMatrix4f> m_boneAttachmentTransforms;
		/* 0x0E0 */ float m_scale;
		/* 0x0E4 */ std::int16_t m_numHands;
		/* 0x0E6 */ std::int16_t m_numFloatSlots;
		/* 0x0E8 */ std::uint8_t pad0E8[0x8];
	};
	static_assert(sizeof(hkbCharacterData) == 0xF0);
	static_assert(offsetof(hkbCharacterData, m_characterControllerSetup) == 0x10);
	static_assert(offsetof(hkbCharacterData, m_modelUpMS) == 0x40);
	static_assert(offsetof(hkbCharacterData, m_modelForwardMS) == 0x50);
	static_assert(offsetof(hkbCharacterData, m_modelRightMS) == 0x60);
	static_assert(offsetof(hkbCharacterData, m_characterPropertyInfos) == 0x70);
	static_assert(offsetof(hkbCharacterData, m_numBonesPerLod) == 0x80);
	static_assert(offsetof(hkbCharacterData, m_characterPropertyValues) == 0x90);
	static_assert(offsetof(hkbCharacterData, m_footIkDriverInfo) == 0x98);
	static_assert(offsetof(hkbCharacterData, m_handIkDriverInfo) == 0xA0);
	static_assert(offsetof(hkbCharacterData, m_aiControlDriverInfo) == 0xA8);
	static_assert(offsetof(hkbCharacterData, m_stringData) == 0xB0);
	static_assert(offsetof(hkbCharacterData, m_mirroredSkeletonInfo) == 0xB8);
	static_assert(offsetof(hkbCharacterData, m_boneAttachmentBoneIndices) == 0xC0);
	static_assert(offsetof(hkbCharacterData, m_boneAttachmentTransforms) == 0xD0);
	static_assert(offsetof(hkbCharacterData, m_scale) == 0xE0);
	static_assert(offsetof(hkbCharacterData, m_numHands) == 0xE4);
	static_assert(offsetof(hkbCharacterData, m_numFloatSlots) == 0xE6);

	class hkbCharacterInfo : public hkReferencedObject
	{
	public:
		enum class Event : std::int32_t
		{
			REMOVED_FROM_WORLD = 0,
			SHOWN = 1,
			HIDDEN = 2,
			ACTIVATED = 3,
			DEACTIVATED = 4,
		};

		/* 0x010 */ std::uint64_t m_characterId;
		/* 0x018 */ std::uint8_t m_event;
		/* 0x019 */ std::uint8_t pad019[0x3];
		/* 0x01C */ std::int32_t m_padding;
	};
	static_assert(sizeof(hkbCharacterInfo) == 0x20);
	static_assert(offsetof(hkbCharacterInfo, m_characterId) == 0x10);
	static_assert(offsetof(hkbCharacterInfo, m_event) == 0x18);
	static_assert(offsetof(hkbCharacterInfo, m_padding) == 0x1C);

	class hkbCharacterSelectedInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
		/* 0x018 */ std::int32_t m_scriptDebuggingPort;
		/* 0x01C */ std::int32_t m_padding;
	};
	static_assert(sizeof(hkbCharacterSelectedInfo) == 0x20);
	static_assert(offsetof(hkbCharacterSelectedInfo, m_characterId) == 0x10);
	static_assert(offsetof(hkbCharacterSelectedInfo, m_scriptDebuggingPort) == 0x18);
	static_assert(offsetof(hkbCharacterSelectedInfo, m_padding) == 0x1C);

	class hkbCharacterSetup : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkaSkeletonMapper*> m_retargetingSkeletonMappers;
		/* 0x020 */ hkaSkeleton* m_animationSkeleton;
		/* 0x028 */ hkaSkeletonMapper* m_ragdollToAnimationSkeletonMapper;
		/* 0x030 */ hkaSkeletonMapper* m_animationToRagdollSkeletonMapper;
		/* 0x038 */ void* m_animationBindingSet;
		/* 0x040 */ hkbCharacterData* m_data;
		/* 0x048 */ void* m_unscaledAnimationSkeleton;
		/* 0x050 */ void* m_mirroredSkeleton;
		/* 0x058 */ void* m_characterPropertyIdMap;
		/* 0x060 */ void* m_criticalSection;
	};
	static_assert(sizeof(hkbCharacterSetup) == 0x68);
	static_assert(offsetof(hkbCharacterSetup, m_retargetingSkeletonMappers) == 0x10);
	static_assert(offsetof(hkbCharacterSetup, m_animationSkeleton) == 0x20);
	static_assert(offsetof(hkbCharacterSetup, m_ragdollToAnimationSkeletonMapper) == 0x28);
	static_assert(offsetof(hkbCharacterSetup, m_animationToRagdollSkeletonMapper) == 0x30);
	static_assert(offsetof(hkbCharacterSetup, m_animationBindingSet) == 0x38);
	static_assert(offsetof(hkbCharacterSetup, m_data) == 0x40);
	static_assert(offsetof(hkbCharacterSetup, m_unscaledAnimationSkeleton) == 0x48);
	static_assert(offsetof(hkbCharacterSetup, m_mirroredSkeleton) == 0x50);
	static_assert(offsetof(hkbCharacterSetup, m_characterPropertyIdMap) == 0x58);
	static_assert(offsetof(hkbCharacterSetup, m_criticalSection) == 0x60);

	class hkbCharacterSkinInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
		/* 0x018 */ hkArray<std::uint64_t> m_deformableSkins;
		/* 0x028 */ hkArray<std::uint64_t> m_rigidSkins;
	};
	static_assert(sizeof(hkbCharacterSkinInfo) == 0x38);
	static_assert(offsetof(hkbCharacterSkinInfo, m_characterId) == 0x10);
	static_assert(offsetof(hkbCharacterSkinInfo, m_deformableSkins) == 0x18);
	static_assert(offsetof(hkbCharacterSkinInfo, m_rigidSkins) == 0x28);

	class hkbCharacterSteppedInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
		/* 0x018 */ float m_deltaTime;
		/* 0x01C */ std::uint8_t pad01C[0x4];
		/* 0x020 */ hkQsTransformf m_worldFromModel;
		/* 0x050 */ hkArray<hkQsTransformf> m_poseModelSpace;
		/* 0x060 */ hkArray<hkQsTransformf> m_rigidAttachmentTransforms;
	};
	static_assert(sizeof(hkbCharacterSteppedInfo) == 0x70);
	static_assert(offsetof(hkbCharacterSteppedInfo, m_characterId) == 0x10);
	static_assert(offsetof(hkbCharacterSteppedInfo, m_deltaTime) == 0x18);
	static_assert(offsetof(hkbCharacterSteppedInfo, m_worldFromModel) == 0x20);
	static_assert(offsetof(hkbCharacterSteppedInfo, m_poseModelSpace) == 0x50);
	static_assert(offsetof(hkbCharacterSteppedInfo, m_rigidAttachmentTransforms) == 0x60);

	class hkbCharacterStringData : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkbCharacterStringDataFileNameMeshNamePair> m_skinNames;
		/* 0x020 */ hkArray<hkbCharacterStringDataFileNameMeshNamePair> m_boneAttachmentNames;
		/* 0x030 */ hkArray<hkbAssetBundleStringData> m_animationBundleNameData;
		/* 0x040 */ hkArray<hkbAssetBundleStringData> m_animationBundleFilenameData;
		/* 0x050 */ hkArray<hkStringPtr> m_characterPropertyNames;
		/* 0x060 */ hkArray<hkStringPtr> m_retargetingSkeletonMapperFilenames;
		/* 0x070 */ hkArray<hkStringPtr> m_lodNames;
		/* 0x080 */ hkArray<hkStringPtr> m_mirroredSyncPointSubstringsA;
		/* 0x090 */ hkArray<hkStringPtr> m_mirroredSyncPointSubstringsB;
		/* 0x0A0 */ hkStringPtr m_name;
		/* 0x0A8 */ hkStringPtr m_rigName;
		/* 0x0B0 */ hkStringPtr m_ragdollName;
		/* 0x0B8 */ hkStringPtr m_behaviorFilename;
		/* 0x0C0 */ hkStringPtr m_luaScriptOnCharacterActivated;
		/* 0x0C8 */ hkStringPtr m_luaScriptOnCharacterDeactivated;
		/* 0x0D0 */ hkArray<hkStringPtr> m_luaFiles;
	};
	static_assert(sizeof(hkbCharacterStringData) == 0xE0);
	static_assert(offsetof(hkbCharacterStringData, m_skinNames) == 0x10);
	static_assert(offsetof(hkbCharacterStringData, m_boneAttachmentNames) == 0x20);
	static_assert(offsetof(hkbCharacterStringData, m_animationBundleNameData) == 0x30);
	static_assert(offsetof(hkbCharacterStringData, m_animationBundleFilenameData) == 0x40);
	static_assert(offsetof(hkbCharacterStringData, m_characterPropertyNames) == 0x50);
	static_assert(offsetof(hkbCharacterStringData, m_retargetingSkeletonMapperFilenames) == 0x60);
	static_assert(offsetof(hkbCharacterStringData, m_lodNames) == 0x70);
	static_assert(offsetof(hkbCharacterStringData, m_mirroredSyncPointSubstringsA) == 0x80);
	static_assert(offsetof(hkbCharacterStringData, m_mirroredSyncPointSubstringsB) == 0x90);
	static_assert(offsetof(hkbCharacterStringData, m_name) == 0xA0);
	static_assert(offsetof(hkbCharacterStringData, m_rigName) == 0xA8);
	static_assert(offsetof(hkbCharacterStringData, m_ragdollName) == 0xB0);
	static_assert(offsetof(hkbCharacterStringData, m_behaviorFilename) == 0xB8);
	static_assert(offsetof(hkbCharacterStringData, m_luaScriptOnCharacterActivated) == 0xC0);
	static_assert(offsetof(hkbCharacterStringData, m_luaScriptOnCharacterDeactivated) == 0xC8);
	static_assert(offsetof(hkbCharacterStringData, m_luaFiles) == 0xD0);

	class hkbCharacterStringDataFileNameMeshNamePair
	{
	public:
		/* 0x000 */ hkStringPtr m_fileName;
		/* 0x008 */ hkStringPtr m_meshName;
	};
	static_assert(sizeof(hkbCharacterStringDataFileNameMeshNamePair) == 0x10);
	static_assert(offsetof(hkbCharacterStringDataFileNameMeshNamePair, m_fileName) == 0x0);
	static_assert(offsetof(hkbCharacterStringDataFileNameMeshNamePair, m_meshName) == 0x8);

	class hkbClientCharacterState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<std::uint64_t> m_deformableSkinIds;
		/* 0x020 */ hkArray<std::uint64_t> m_rigidSkinIds;
		/* 0x030 */ hkArray<std::int16_t> m_externalEventIds;
		/* 0x040 */ hkArray<hkbAuxiliaryNodeInfo*> m_auxiliaryInfo;
		/* 0x050 */ hkArray<std::int16_t> m_activeEventIds;
		/* 0x060 */ hkArray<std::int16_t> m_activeVariableIds;
		/* 0x070 */ std::uint64_t m_characterId;
		/* 0x078 */ hkStringPtr m_instanceName;
		/* 0x080 */ hkStringPtr m_templateName;
		/* 0x088 */ hkStringPtr m_fullPathToProject;
		/* 0x090 */ hkStringPtr m_localScriptsPath;
		/* 0x098 */ hkStringPtr m_remoteScriptsPath;
		/* 0x0A0 */ hkbBehaviorGraphData* m_behaviorData;
		/* 0x0A8 */ hkbBehaviorGraphInternalState* m_behaviorInternalState;
		/* 0x0B0 */ void* m_nodeIdToInternalStateMap;
		/* 0x0B8 */ hkBool m_visible;
		/* 0x0B9 */ std::uint8_t pad0B9[0x3];
		/* 0x0BC */ float m_elapsedSimulationTime;
		/* 0x0C0 */ hkaSkeleton* m_skeleton;
		/* 0x0C8 */ std::uint8_t pad0C8[0x8];
		/* 0x0D0 */ hkQsTransformf m_worldFromModel;
		/* 0x100 */ hkArray<hkQsTransformf> m_poseModelSpace;
		/* 0x110 */ hkArray<hkQsTransformf> m_rigidAttachmentTransforms;
	};
	static_assert(sizeof(hkbClientCharacterState) == 0x120);
	static_assert(offsetof(hkbClientCharacterState, m_deformableSkinIds) == 0x10);
	static_assert(offsetof(hkbClientCharacterState, m_rigidSkinIds) == 0x20);
	static_assert(offsetof(hkbClientCharacterState, m_externalEventIds) == 0x30);
	static_assert(offsetof(hkbClientCharacterState, m_auxiliaryInfo) == 0x40);
	static_assert(offsetof(hkbClientCharacterState, m_activeEventIds) == 0x50);
	static_assert(offsetof(hkbClientCharacterState, m_activeVariableIds) == 0x60);
	static_assert(offsetof(hkbClientCharacterState, m_characterId) == 0x70);
	static_assert(offsetof(hkbClientCharacterState, m_instanceName) == 0x78);
	static_assert(offsetof(hkbClientCharacterState, m_templateName) == 0x80);
	static_assert(offsetof(hkbClientCharacterState, m_fullPathToProject) == 0x88);
	static_assert(offsetof(hkbClientCharacterState, m_localScriptsPath) == 0x90);
	static_assert(offsetof(hkbClientCharacterState, m_remoteScriptsPath) == 0x98);
	static_assert(offsetof(hkbClientCharacterState, m_behaviorData) == 0xA0);
	static_assert(offsetof(hkbClientCharacterState, m_behaviorInternalState) == 0xA8);
	static_assert(offsetof(hkbClientCharacterState, m_nodeIdToInternalStateMap) == 0xB0);
	static_assert(offsetof(hkbClientCharacterState, m_visible) == 0xB8);
	static_assert(offsetof(hkbClientCharacterState, m_elapsedSimulationTime) == 0xBC);
	static_assert(offsetof(hkbClientCharacterState, m_skeleton) == 0xC0);
	static_assert(offsetof(hkbClientCharacterState, m_worldFromModel) == 0xD0);
	static_assert(offsetof(hkbClientCharacterState, m_poseModelSpace) == 0x100);
	static_assert(offsetof(hkbClientCharacterState, m_rigidAttachmentTransforms) == 0x110);

	class hkbClipGeneratorEcho
	{
	public:
		/* 0x000 */ float m_offsetLocalTime;
		/* 0x004 */ float m_weight;
		/* 0x008 */ float m_dwdt;
		/* 0x00C */ std::uint8_t pad00C[0x4];
	};
	static_assert(sizeof(hkbClipGeneratorEcho) == 0x10);
	static_assert(offsetof(hkbClipGeneratorEcho, m_offsetLocalTime) == 0x0);
	static_assert(offsetof(hkbClipGeneratorEcho, m_weight) == 0x4);
	static_assert(offsetof(hkbClipGeneratorEcho, m_dwdt) == 0x8);

	class hkbClipGeneratorInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkQsTransformf m_extractedMotion;
		/* 0x040 */ hkArray<hkbClipGeneratorEcho> m_echos;
		/* 0x050 */ float m_localTime;
		/* 0x054 */ float m_time;
		/* 0x058 */ float m_previousUserControlledTimeFraction;
		/* 0x05C */ std::int32_t m_bufferSize;
		/* 0x060 */ hkBool m_atEnd;
		/* 0x061 */ hkBool m_ignoreStartTime;
		/* 0x062 */ hkBool m_pingPongBackward;
		/* 0x063 */ std::uint8_t pad063[0xD];
	};
	static_assert(sizeof(hkbClipGeneratorInternalState) == 0x70);
	static_assert(offsetof(hkbClipGeneratorInternalState, m_extractedMotion) == 0x10);
	static_assert(offsetof(hkbClipGeneratorInternalState, m_echos) == 0x40);
	static_assert(offsetof(hkbClipGeneratorInternalState, m_localTime) == 0x50);
	static_assert(offsetof(hkbClipGeneratorInternalState, m_time) == 0x54);
	static_assert(offsetof(hkbClipGeneratorInternalState, m_previousUserControlledTimeFraction) == 0x58);
	static_assert(offsetof(hkbClipGeneratorInternalState, m_bufferSize) == 0x5C);
	static_assert(offsetof(hkbClipGeneratorInternalState, m_atEnd) == 0x60);
	static_assert(offsetof(hkbClipGeneratorInternalState, m_ignoreStartTime) == 0x61);
	static_assert(offsetof(hkbClipGeneratorInternalState, m_pingPongBackward) == 0x62);

	class hkbEventBase
	{
	public:
		enum class SystemEventIds : std::int32_t
		{
			EVENT_ID_NULL = -1,
		};

		/* 0x000 */ std::int32_t m_id;
		/* 0x004 */ std::uint8_t pad004[0x4];
		/* 0x008 */ hkbEventPayload* m_payload;
	};
	static_assert(sizeof(hkbEventBase) == 0x10);
	static_assert(offsetof(hkbEventBase, m_id) == 0x0);
	static_assert(offsetof(hkbEventBase, m_payload) == 0x8);

	class hkbEventProperty : public hkbEventBase
	{
	public:
	};
	static_assert(sizeof(hkbEventProperty) == 0x10);

	class hkbClipTrigger
	{
	public:
		/* 0x000 */ float m_localTime;
		/* 0x004 */ std::uint8_t pad004[0x4];
		/* 0x008 */ hkbEventProperty m_event;
		/* 0x018 */ hkBool m_relativeToEndOfClip;
		/* 0x019 */ hkBool m_acyclic;
		/* 0x01A */ hkBool m_isAnnotation;
		/* 0x01B */ std::uint8_t pad01B[0x5];
	};
	static_assert(sizeof(hkbClipTrigger) == 0x20);
	static_assert(offsetof(hkbClipTrigger, m_localTime) == 0x0);
	static_assert(offsetof(hkbClipTrigger, m_event) == 0x8);
	static_assert(offsetof(hkbClipTrigger, m_relativeToEndOfClip) == 0x18);
	static_assert(offsetof(hkbClipTrigger, m_acyclic) == 0x19);
	static_assert(offsetof(hkbClipTrigger, m_isAnnotation) == 0x1A);

	class hkbClipTriggerArray : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkbClipTrigger> m_triggers;
	};
	static_assert(sizeof(hkbClipTriggerArray) == 0x20);
	static_assert(offsetof(hkbClipTriggerArray, m_triggers) == 0x10);

	class hkbCombineTransformsModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkVector4f m_translationOut;
		/* 0x020 */ hkQuaternionf m_rotationOut;
	};
	static_assert(sizeof(hkbCombineTransformsModifierInternalState) == 0x30);
	static_assert(offsetof(hkbCombineTransformsModifierInternalState, m_translationOut) == 0x10);
	static_assert(offsetof(hkbCombineTransformsModifierInternalState, m_rotationOut) == 0x20);

	class hkbCompiledExpressionSet : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkbCompiledExpressionSetToken> m_rpn;
		/* 0x020 */ hkArray<std::int32_t> m_expressionToRpnIndex;
		/* 0x030 */ std::int8_t m_numExpressions;
		/* 0x031 */ std::uint8_t pad031[0x7];
	};
	static_assert(sizeof(hkbCompiledExpressionSet) == 0x38);
	static_assert(offsetof(hkbCompiledExpressionSet, m_rpn) == 0x10);
	static_assert(offsetof(hkbCompiledExpressionSet, m_expressionToRpnIndex) == 0x20);
	static_assert(offsetof(hkbCompiledExpressionSet, m_numExpressions) == 0x30);

	class hkbCompiledExpressionSetToken
	{
	public:
		enum class TokenType : std::int32_t
		{
			TOKEN_TYPE_NONE = 0,
			TOKEN_TYPE_OPERATOR = 1,
			TOKEN_TYPE_NUMBER = 2,
			TOKEN_TYPE_VARIABLE_INDEX = 3,
			TOKEN_TYPE_OPENING_PAREN = 4,
			TOKEN_TYPE_CLOSING_PAREN = 5,
			TOKEN_TYPE_COMMA = 6,
			TOKEN_TYPE_CHARACTER_PROPERTY_INDEX = 7,
		};

		enum class Operator : std::int32_t
		{
			OP_NOP = 0,
			OP_RAND01 = 1,
			OP_LOGICAL_NOT = 2,
			OP_UNARY_MINUS = 3,
			OP_UNARY_PLUS = 4,
			OP_SIN = 5,
			OP_COS = 6,
			OP_ASIN = 7,
			OP_ACOS = 8,
			OP_SQRT = 9,
			OP_FABS = 10,
			OP_CEIL = 11,
			OP_FLOOR = 12,
			OP_SQRTINV = 13,
			OP_MUL = 14,
			OP_DIV = 15,
			OP_ADD = 16,
			OP_SUB = 17,
			OP_LOGICAL_OR = 18,
			OP_LOGICAL_AND = 19,
			OP_EQ = 20,
			OP_NEQ = 21,
			OP_LT = 22,
			OP_GT = 23,
			OP_LEQ = 24,
			OP_GEQ = 25,
			OP_POW = 26,
			OP_MAX2 = 27,
			OP_MIN2 = 28,
			OP_RANDRANGE = 29,
			OP_ATAN2 = 30,
			OP_CLAMP = 31,
			OP_MOD = 32,
			OP_DEG2RAD = 33,
			OP_RAD2DEG = 34,
			OP_COSD = 35,
			OP_SIND = 36,
			OP_ACOSD = 37,
			OP_ASIND = 38,
			OP_ATAN2D = 39,
			OP_SIGN = 40,
			OP_LERP = 41,
			OP_CLERP = 42,
			OP_COND = 43,
		};

		/* 0x000 */ float m_data;
		/* 0x004 */ std::int8_t m_type;
		/* 0x005 */ std::int8_t m_operator;
		/* 0x006 */ std::uint8_t pad006[0x2];
	};
	static_assert(sizeof(hkbCompiledExpressionSetToken) == 0x8);
	static_assert(offsetof(hkbCompiledExpressionSetToken, m_data) == 0x0);
	static_assert(offsetof(hkbCompiledExpressionSetToken, m_type) == 0x4);
	static_assert(offsetof(hkbCompiledExpressionSetToken, m_operator) == 0x5);

	class hkbComputeDirectionModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkVector4f m_pointOut;
		/* 0x020 */ float m_groundAngleOut;
		/* 0x024 */ float m_upAngleOut;
		/* 0x028 */ hkBool m_computedOutput;
		/* 0x029 */ std::uint8_t pad029[0x7];
	};
	static_assert(sizeof(hkbComputeDirectionModifierInternalState) == 0x30);
	static_assert(offsetof(hkbComputeDirectionModifierInternalState, m_pointOut) == 0x10);
	static_assert(offsetof(hkbComputeDirectionModifierInternalState, m_groundAngleOut) == 0x20);
	static_assert(offsetof(hkbComputeDirectionModifierInternalState, m_upAngleOut) == 0x24);
	static_assert(offsetof(hkbComputeDirectionModifierInternalState, m_computedOutput) == 0x28);

	class hkbComputeRotationFromAxisAngleModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkQuaternionf m_rotationOut;
	};
	static_assert(sizeof(hkbComputeRotationFromAxisAngleModifierInternalState) == 0x20);
	static_assert(offsetof(hkbComputeRotationFromAxisAngleModifierInternalState, m_rotationOut) == 0x10);

	class hkbComputeRotationToTargetModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkQuaternionf m_rotationOut;
	};
	static_assert(sizeof(hkbComputeRotationToTargetModifierInternalState) == 0x20);
	static_assert(offsetof(hkbComputeRotationToTargetModifierInternalState, m_rotationOut) == 0x10);

	class hkbCondition : public hkReferencedObject
	{
	public:

	private:
		/* 04 */ virtual void sub_hkbCondition_04();
		/* 05 */ virtual void sub_hkbCondition_05();
	};
	static_assert(sizeof(hkbCondition) == 0x10);

	class hkbConstraintSetup
	{
	public:
		enum class Type : std::int32_t
		{
			BALL_AND_SOCKET = 0,
			RAGDOLL = 1,
		};

		/* 0x000 */ std::int8_t m_type;
	};
	static_assert(sizeof(hkbConstraintSetup) == 0x1);
	static_assert(offsetof(hkbConstraintSetup, m_type) == 0x0);

	class hkbContext
	{
	public:
		/* 0x000 */ void* m_character;
		/* 0x008 */ hkbBehaviorGraph* m_rootBehavior;
		/* 0x010 */ void* m_behavior;
		/* 0x018 */ void* m_nodeToIndexMap;
		/* 0x020 */ void* m_eventQueue;
		/* 0x028 */ void* m_sharedEventQueue;
		/* 0x030 */ hkbGeneratorOutputListener* m_generatorOutputListener;
		/* 0x038 */ void* m_syncVariableSetup;
		/* 0x040 */ hkBool m_eventTriggeredTransition;
		/* 0x041 */ std::uint8_t pad041[0x7];
		/* 0x048 */ void* m_nodeIdToInternalState;
		/* 0x050 */ void* m_physicsInterface;
		/* 0x058 */ void* m_attachmentManager;
	};
	static_assert(sizeof(hkbContext) == 0x60);
	static_assert(offsetof(hkbContext, m_character) == 0x0);
	static_assert(offsetof(hkbContext, m_rootBehavior) == 0x8);
	static_assert(offsetof(hkbContext, m_behavior) == 0x10);
	static_assert(offsetof(hkbContext, m_nodeToIndexMap) == 0x18);
	static_assert(offsetof(hkbContext, m_eventQueue) == 0x20);
	static_assert(offsetof(hkbContext, m_sharedEventQueue) == 0x28);
	static_assert(offsetof(hkbContext, m_generatorOutputListener) == 0x30);
	static_assert(offsetof(hkbContext, m_syncVariableSetup) == 0x38);
	static_assert(offsetof(hkbContext, m_eventTriggeredTransition) == 0x40);
	static_assert(offsetof(hkbContext, m_nodeIdToInternalState) == 0x48);
	static_assert(offsetof(hkbContext, m_physicsInterface) == 0x50);
	static_assert(offsetof(hkbContext, m_attachmentManager) == 0x58);

	class hkbCustomIdSelector : public hkReferencedObject
	{
	public:

	private:
		/* 04 */ virtual void sub_hkbCustomIdSelector_04();
		/* 05 */ virtual void sub_hkbCustomIdSelector_05();
	};
	static_assert(sizeof(hkbCustomIdSelector) == 0x10);

	class hkbDampingModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkVector4f m_dampedVector;
		/* 0x020 */ hkVector4f m_vecErrorSum;
		/* 0x030 */ hkVector4f m_vecPreviousError;
		/* 0x040 */ float m_dampedValue;
		/* 0x044 */ float m_errorSum;
		/* 0x048 */ float m_previousError;
		/* 0x04C */ std::uint8_t pad04C[0x4];
	};
	static_assert(sizeof(hkbDampingModifierInternalState) == 0x50);
	static_assert(offsetof(hkbDampingModifierInternalState, m_dampedVector) == 0x10);
	static_assert(offsetof(hkbDampingModifierInternalState, m_vecErrorSum) == 0x20);
	static_assert(offsetof(hkbDampingModifierInternalState, m_vecPreviousError) == 0x30);
	static_assert(offsetof(hkbDampingModifierInternalState, m_dampedValue) == 0x40);
	static_assert(offsetof(hkbDampingModifierInternalState, m_errorSum) == 0x44);
	static_assert(offsetof(hkbDampingModifierInternalState, m_previousError) == 0x48);

	class hkbDefaultMessageLog
	{
	public:
		/* 0x000 */ std::uint8_t pad000[0x1];
	};
	static_assert(sizeof(hkbDefaultMessageLog) == 0x1);

	class hkbDelayedModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ float m_secondsElapsed;
		/* 0x014 */ hkBool m_isActive;
		/* 0x015 */ std::uint8_t pad015[0x3];
	};
	static_assert(sizeof(hkbDelayedModifierInternalState) == 0x18);
	static_assert(offsetof(hkbDelayedModifierInternalState, m_secondsElapsed) == 0x10);
	static_assert(offsetof(hkbDelayedModifierInternalState, m_isActive) == 0x14);

	class hkbDetectCloseToGroundModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkBool m_isCloseToGround;
		/* 0x011 */ std::uint8_t pad011[0x7];
	};
	static_assert(sizeof(hkbDetectCloseToGroundModifierInternalState) == 0x18);
	static_assert(offsetof(hkbDetectCloseToGroundModifierInternalState, m_isCloseToGround) == 0x10);

	class hkbDockingGeneratorInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ float m_localTime;
		/* 0x014 */ float m_previousLocalTime;
		/* 0x018 */ float m_intervalStartLocalTime;
		/* 0x01C */ float m_intervalEndLocalTime;
	};
	static_assert(sizeof(hkbDockingGeneratorInternalState) == 0x20);
	static_assert(offsetof(hkbDockingGeneratorInternalState, m_localTime) == 0x10);
	static_assert(offsetof(hkbDockingGeneratorInternalState, m_previousLocalTime) == 0x14);
	static_assert(offsetof(hkbDockingGeneratorInternalState, m_intervalStartLocalTime) == 0x18);
	static_assert(offsetof(hkbDockingGeneratorInternalState, m_intervalEndLocalTime) == 0x1C);

	class hkbEvaluateExpressionModifierInternalExpressionData
	{
	public:
		/* 0x000 */ hkBool m_raisedEvent;
		/* 0x001 */ hkBool m_wasTrueInPreviousFrame;
	};
	static_assert(sizeof(hkbEvaluateExpressionModifierInternalExpressionData) == 0x2);
	static_assert(offsetof(hkbEvaluateExpressionModifierInternalExpressionData, m_raisedEvent) == 0x0);
	static_assert(offsetof(hkbEvaluateExpressionModifierInternalExpressionData, m_wasTrueInPreviousFrame) == 0x1);

	class hkbEvaluateExpressionModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkbEvaluateExpressionModifierInternalExpressionData> m_internalExpressionsData;
	};
	static_assert(sizeof(hkbEvaluateExpressionModifierInternalState) == 0x20);
	static_assert(offsetof(hkbEvaluateExpressionModifierInternalState, m_internalExpressionsData) == 0x10);

	class hkbEvent : public hkbEventBase
	{
	public:
		/* 0x010 */ void* m_sender;
	};
	static_assert(sizeof(hkbEvent) == 0x18);
	static_assert(offsetof(hkbEvent, m_sender) == 0x10);

	class hkbEventDrivenModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkBool m_isActive;
		/* 0x011 */ std::uint8_t pad011[0x7];
	};
	static_assert(sizeof(hkbEventDrivenModifierInternalState) == 0x18);
	static_assert(offsetof(hkbEventDrivenModifierInternalState, m_isActive) == 0x10);

	class hkbEventInfo
	{
	public:
		enum class Flags : std::int32_t
		{
			FLAG_SILENT = 1,
			FLAG_SYNC_POINT = 2,
		};

		/* 0x000 */ std::uint32_t m_flags;
	};
	static_assert(sizeof(hkbEventInfo) == 0x4);
	static_assert(offsetof(hkbEventInfo, m_flags) == 0x0);

	class hkbEventPayloadList : public hkbEventPayload
	{
	public:
		/* 0x010 */ hkArray<hkbEventPayload*> m_payloads;
	};
	static_assert(sizeof(hkbEventPayloadList) == 0x20);
	static_assert(offsetof(hkbEventPayloadList, m_payloads) == 0x10);

	class hkbEventRaisedInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
		/* 0x018 */ hkStringPtr m_eventName;
		/* 0x020 */ hkBool m_raisedBySdk;
		/* 0x021 */ std::uint8_t pad021[0x3];
		/* 0x024 */ std::int32_t m_senderId;
		/* 0x028 */ std::int32_t m_padding;
		/* 0x02C */ std::uint8_t pad02C[0x4];
	};
	static_assert(sizeof(hkbEventRaisedInfo) == 0x30);
	static_assert(offsetof(hkbEventRaisedInfo, m_characterId) == 0x10);
	static_assert(offsetof(hkbEventRaisedInfo, m_eventName) == 0x18);
	static_assert(offsetof(hkbEventRaisedInfo, m_raisedBySdk) == 0x20);
	static_assert(offsetof(hkbEventRaisedInfo, m_senderId) == 0x24);
	static_assert(offsetof(hkbEventRaisedInfo, m_padding) == 0x28);

	class hkbEventRangeData
	{
	public:
		enum class EventRangeMode : std::int32_t
		{
			EVENT_MODE_SEND_ON_ENTER_RANGE = 0,
			EVENT_MODE_SEND_WHEN_IN_RANGE = 1,
		};

		/* 0x000 */ float m_upperBound;
		/* 0x004 */ std::uint8_t pad004[0x4];
		/* 0x008 */ hkbEventProperty m_event;
		/* 0x018 */ std::int8_t m_eventMode;
		/* 0x019 */ std::uint8_t pad019[0x7];
	};
	static_assert(sizeof(hkbEventRangeData) == 0x20);
	static_assert(offsetof(hkbEventRangeData, m_upperBound) == 0x0);
	static_assert(offsetof(hkbEventRangeData, m_event) == 0x8);
	static_assert(offsetof(hkbEventRangeData, m_eventMode) == 0x18);

	class hkbEventRangeDataArray : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkbEventRangeData> m_eventData;
	};
	static_assert(sizeof(hkbEventRangeDataArray) == 0x20);
	static_assert(offsetof(hkbEventRangeDataArray, m_eventData) == 0x10);

	class hkbEventSequencedData : public hkbSequencedData
	{
	public:
		/* 0x010 */ hkArray<hkbEventSequencedDataSequencedEvent> m_events;
	};
	static_assert(sizeof(hkbEventSequencedData) == 0x20);
	static_assert(offsetof(hkbEventSequencedData, m_events) == 0x10);

	class hkbEventSequencedDataSequencedEvent
	{
	public:
		/* 0x000 */ hkbEvent m_event;
		/* 0x018 */ float m_time;
		/* 0x01C */ std::uint8_t pad01C[0x4];
	};
	static_assert(sizeof(hkbEventSequencedDataSequencedEvent) == 0x20);
	static_assert(offsetof(hkbEventSequencedDataSequencedEvent, m_event) == 0x0);
	static_assert(offsetof(hkbEventSequencedDataSequencedEvent, m_time) == 0x18);

	class hkbEventsFromRangeModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkBool> m_wasActiveInPreviousFrame;
	};
	static_assert(sizeof(hkbEventsFromRangeModifierInternalState) == 0x20);
	static_assert(offsetof(hkbEventsFromRangeModifierInternalState, m_wasActiveInPreviousFrame) == 0x10);

	class hkbExpressionCondition : public hkbCondition
	{
	public:
		/* 0x010 */ hkStringPtr m_expression;
		/* 0x018 */ void* m_compiledExpressionSet;
	};
	static_assert(sizeof(hkbExpressionCondition) == 0x20);
	static_assert(offsetof(hkbExpressionCondition, m_expression) == 0x10);
	static_assert(offsetof(hkbExpressionCondition, m_compiledExpressionSet) == 0x18);

	class hkbExpressionData
	{
	public:
		enum class ExpressionEventMode : std::int32_t
		{
			EVENT_MODE_SEND_ONCE = 0,
			EVENT_MODE_SEND_ON_TRUE = 1,
			EVENT_MODE_SEND_ON_FALSE_TO_TRUE = 2,
			EVENT_MODE_SEND_EVERY_FRAME_ONCE_TRUE = 3,
		};

		/* 0x000 */ hkStringPtr m_expression;
		/* 0x008 */ std::int32_t m_assignmentVariableIndex;
		/* 0x00C */ std::int32_t m_assignmentEventIndex;
		/* 0x010 */ std::int8_t m_eventMode;
		/* 0x011 */ hkBool m_raisedEvent;
		/* 0x012 */ hkBool m_wasTrueInPreviousFrame;
		/* 0x013 */ std::uint8_t pad013[0x5];
	};
	static_assert(sizeof(hkbExpressionData) == 0x18);
	static_assert(offsetof(hkbExpressionData, m_expression) == 0x0);
	static_assert(offsetof(hkbExpressionData, m_assignmentVariableIndex) == 0x8);
	static_assert(offsetof(hkbExpressionData, m_assignmentEventIndex) == 0xC);
	static_assert(offsetof(hkbExpressionData, m_eventMode) == 0x10);
	static_assert(offsetof(hkbExpressionData, m_raisedEvent) == 0x11);
	static_assert(offsetof(hkbExpressionData, m_wasTrueInPreviousFrame) == 0x12);

	class hkbExpressionDataArray : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkbExpressionData> m_expressionsData;
	};
	static_assert(sizeof(hkbExpressionDataArray) == 0x20);
	static_assert(offsetof(hkbExpressionDataArray, m_expressionsData) == 0x10);

	class hkbGeneratorPartitionInfo
	{
	public:
		/* 0x000 */ std::uint32_t m_boneMask[8];
		/* 0x020 */ std::uint32_t m_partitionMask[1];
		/* 0x024 */ std::int16_t m_numBones;
		/* 0x026 */ std::int16_t m_numMaxPartitions;
	};
	static_assert(sizeof(hkbGeneratorPartitionInfo) == 0x28);
	static_assert(offsetof(hkbGeneratorPartitionInfo, m_boneMask) == 0x0);
	static_assert(offsetof(hkbGeneratorPartitionInfo, m_partitionMask) == 0x20);
	static_assert(offsetof(hkbGeneratorPartitionInfo, m_numBones) == 0x24);
	static_assert(offsetof(hkbGeneratorPartitionInfo, m_numMaxPartitions) == 0x26);

	class hkbGeneratorSyncInfoSyncPoint
	{
	public:
		/* 0x000 */ std::int32_t m_id;
		/* 0x004 */ float m_time;
	};
	static_assert(sizeof(hkbGeneratorSyncInfoSyncPoint) == 0x8);
	static_assert(offsetof(hkbGeneratorSyncInfoSyncPoint, m_id) == 0x0);
	static_assert(offsetof(hkbGeneratorSyncInfoSyncPoint, m_time) == 0x4);

	class hkbGeneratorSyncInfoActiveInterval
	{
	public:
		/* 0x000 */ hkbGeneratorSyncInfoSyncPoint m_syncPoints[2];
		/* 0x010 */ float m_fraction;
	};
	static_assert(sizeof(hkbGeneratorSyncInfoActiveInterval) == 0x14);
	static_assert(offsetof(hkbGeneratorSyncInfoActiveInterval, m_syncPoints) == 0x0);
	static_assert(offsetof(hkbGeneratorSyncInfoActiveInterval, m_fraction) == 0x10);

	class hkbGeneratorSyncInfo
	{
	public:
		/* 0x000 */ hkbGeneratorSyncInfoSyncPoint m_syncPoints[16];
		/* 0x080 */ float m_duration;
		/* 0x084 */ float m_localTime;
		/* 0x088 */ float m_playbackSpeed;
		/* 0x08C */ std::int8_t m_numSyncPoints;
		/* 0x08D */ hkBool m_isCyclic;
		/* 0x08E */ hkBool m_isMirrored;
		/* 0x08F */ hkBool m_isAdditive;
		/* 0x090 */ hkbGeneratorSyncInfoActiveInterval m_activeInterval;
		/* 0x0A4 */ float m_nativePlaybackSpeed;
	};
	static_assert(sizeof(hkbGeneratorSyncInfo) == 0xA8);
	static_assert(offsetof(hkbGeneratorSyncInfo, m_syncPoints) == 0x0);
	static_assert(offsetof(hkbGeneratorSyncInfo, m_duration) == 0x80);
	static_assert(offsetof(hkbGeneratorSyncInfo, m_localTime) == 0x84);
	static_assert(offsetof(hkbGeneratorSyncInfo, m_playbackSpeed) == 0x88);
	static_assert(offsetof(hkbGeneratorSyncInfo, m_numSyncPoints) == 0x8C);
	static_assert(offsetof(hkbGeneratorSyncInfo, m_isCyclic) == 0x8D);
	static_assert(offsetof(hkbGeneratorSyncInfo, m_isMirrored) == 0x8E);
	static_assert(offsetof(hkbGeneratorSyncInfo, m_isAdditive) == 0x8F);
	static_assert(offsetof(hkbGeneratorSyncInfo, m_activeInterval) == 0x90);
	static_assert(offsetof(hkbGeneratorSyncInfo, m_nativePlaybackSpeed) == 0xA4);

	class hkbExtrapolatingTransitionEffectInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkbGeneratorSyncInfo m_fromGeneratorSyncInfo;
		/* 0x0B8 */ hkbGeneratorPartitionInfo m_fromGeneratorPartitionInfo;
		/* 0x0E0 */ hkQsTransformf m_worldFromModel;
		/* 0x110 */ hkQsTransformf m_motion;
		/* 0x140 */ hkArray<hkQsTransformf> m_pose;
		/* 0x150 */ hkArray<hkQsTransformf> m_additivePose;
		/* 0x160 */ hkArray<float> m_boneWeights;
		/* 0x170 */ float m_toGeneratorDuration;
		/* 0x174 */ hkBool m_isFromGeneratorActive;
		/* 0x175 */ hkBool m_gotPose;
		/* 0x176 */ hkBool m_gotAdditivePose;
		/* 0x177 */ std::uint8_t pad177[0x9];
	};
	static_assert(sizeof(hkbExtrapolatingTransitionEffectInternalState) == 0x180);
	static_assert(offsetof(hkbExtrapolatingTransitionEffectInternalState, m_fromGeneratorSyncInfo) == 0x10);
	static_assert(offsetof(hkbExtrapolatingTransitionEffectInternalState, m_fromGeneratorPartitionInfo) == 0xB8);
	static_assert(offsetof(hkbExtrapolatingTransitionEffectInternalState, m_worldFromModel) == 0xE0);
	static_assert(offsetof(hkbExtrapolatingTransitionEffectInternalState, m_motion) == 0x110);
	static_assert(offsetof(hkbExtrapolatingTransitionEffectInternalState, m_pose) == 0x140);
	static_assert(offsetof(hkbExtrapolatingTransitionEffectInternalState, m_additivePose) == 0x150);
	static_assert(offsetof(hkbExtrapolatingTransitionEffectInternalState, m_boneWeights) == 0x160);
	static_assert(offsetof(hkbExtrapolatingTransitionEffectInternalState, m_toGeneratorDuration) == 0x170);
	static_assert(offsetof(hkbExtrapolatingTransitionEffectInternalState, m_isFromGeneratorActive) == 0x174);
	static_assert(offsetof(hkbExtrapolatingTransitionEffectInternalState, m_gotPose) == 0x175);
	static_assert(offsetof(hkbExtrapolatingTransitionEffectInternalState, m_gotAdditivePose) == 0x176);

	class hkbFootIkGains
	{
	public:
		/* 0x000 */ float m_onOffGain;
		/* 0x004 */ float m_groundAscendingGain;
		/* 0x008 */ float m_groundDescendingGain;
		/* 0x00C */ float m_footPlantedGain;
		/* 0x010 */ float m_footRaisedGain;
		/* 0x014 */ float m_footLockingGain;
		/* 0x018 */ float m_worldFromModelFeedbackGain;
		/* 0x01C */ float m_errorUpDownBias;
		/* 0x020 */ float m_alignWorldFromModelGain;
		/* 0x024 */ float m_hipOrientationGain;
		/* 0x028 */ float m_maxKneeAngleDifference;
		/* 0x02C */ float m_ankleOrientationGain;
	};
	static_assert(sizeof(hkbFootIkGains) == 0x30);
	static_assert(offsetof(hkbFootIkGains, m_onOffGain) == 0x0);
	static_assert(offsetof(hkbFootIkGains, m_groundAscendingGain) == 0x4);
	static_assert(offsetof(hkbFootIkGains, m_groundDescendingGain) == 0x8);
	static_assert(offsetof(hkbFootIkGains, m_footPlantedGain) == 0xC);
	static_assert(offsetof(hkbFootIkGains, m_footRaisedGain) == 0x10);
	static_assert(offsetof(hkbFootIkGains, m_footLockingGain) == 0x14);
	static_assert(offsetof(hkbFootIkGains, m_worldFromModelFeedbackGain) == 0x18);
	static_assert(offsetof(hkbFootIkGains, m_errorUpDownBias) == 0x1C);
	static_assert(offsetof(hkbFootIkGains, m_alignWorldFromModelGain) == 0x20);
	static_assert(offsetof(hkbFootIkGains, m_hipOrientationGain) == 0x24);
	static_assert(offsetof(hkbFootIkGains, m_maxKneeAngleDifference) == 0x28);
	static_assert(offsetof(hkbFootIkGains, m_ankleOrientationGain) == 0x2C);

	class hkbFootIkControlData
	{
	public:
		/* 0x000 */ hkbFootIkGains m_gains;
		/* 0x030 */ float m_enabled[8];
	};
	static_assert(sizeof(hkbFootIkControlData) == 0x50);
	static_assert(offsetof(hkbFootIkControlData, m_gains) == 0x0);
	static_assert(offsetof(hkbFootIkControlData, m_enabled) == 0x30);

	class hkbFootIkControlsModifierLeg
	{
	public:
		/* 0x000 */ hkVector4f m_groundPosition;
		/* 0x010 */ hkbEventProperty m_ungroundedEvent;
		/* 0x020 */ float m_verticalError;
		/* 0x024 */ hkBool m_hitSomething;
		/* 0x025 */ hkBool m_isPlantedMS;
		/* 0x026 */ hkBool m_enabled;
		/* 0x027 */ std::uint8_t pad027[0x9];
	};
	static_assert(sizeof(hkbFootIkControlsModifierLeg) == 0x30);
	static_assert(offsetof(hkbFootIkControlsModifierLeg, m_groundPosition) == 0x0);
	static_assert(offsetof(hkbFootIkControlsModifierLeg, m_ungroundedEvent) == 0x10);
	static_assert(offsetof(hkbFootIkControlsModifierLeg, m_verticalError) == 0x20);
	static_assert(offsetof(hkbFootIkControlsModifierLeg, m_hitSomething) == 0x24);
	static_assert(offsetof(hkbFootIkControlsModifierLeg, m_isPlantedMS) == 0x25);
	static_assert(offsetof(hkbFootIkControlsModifierLeg, m_enabled) == 0x26);

	class hkbFootIkDriverInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkbFootIkDriverInfoLeg> m_legs;
		/* 0x020 */ float m_raycastDistanceUp;
		/* 0x024 */ float m_raycastDistanceDown;
		/* 0x028 */ float m_originalGroundHeightMS;
		/* 0x02C */ float m_verticalOffset;
		/* 0x030 */ std::uint32_t m_collisionFilterInfo;
		/* 0x034 */ float m_forwardAlignFraction;
		/* 0x038 */ float m_sidewaysAlignFraction;
		/* 0x03C */ float m_sidewaysSampleWidth;
		/* 0x040 */ hkBool m_lockFeetWhenPlanted;
		/* 0x041 */ hkBool m_useCharacterUpVector;
		/* 0x042 */ hkBool m_isQuadrupedNarrow;
		/* 0x043 */ hkBool m_keepSourceFootEndAboveGround;
		/* 0x044 */ std::uint8_t pad044[0x4];
	};
	static_assert(sizeof(hkbFootIkDriverInfo) == 0x48);
	static_assert(offsetof(hkbFootIkDriverInfo, m_legs) == 0x10);
	static_assert(offsetof(hkbFootIkDriverInfo, m_raycastDistanceUp) == 0x20);
	static_assert(offsetof(hkbFootIkDriverInfo, m_raycastDistanceDown) == 0x24);
	static_assert(offsetof(hkbFootIkDriverInfo, m_originalGroundHeightMS) == 0x28);
	static_assert(offsetof(hkbFootIkDriverInfo, m_verticalOffset) == 0x2C);
	static_assert(offsetof(hkbFootIkDriverInfo, m_collisionFilterInfo) == 0x30);
	static_assert(offsetof(hkbFootIkDriverInfo, m_forwardAlignFraction) == 0x34);
	static_assert(offsetof(hkbFootIkDriverInfo, m_sidewaysAlignFraction) == 0x38);
	static_assert(offsetof(hkbFootIkDriverInfo, m_sidewaysSampleWidth) == 0x3C);
	static_assert(offsetof(hkbFootIkDriverInfo, m_lockFeetWhenPlanted) == 0x40);
	static_assert(offsetof(hkbFootIkDriverInfo, m_useCharacterUpVector) == 0x41);
	static_assert(offsetof(hkbFootIkDriverInfo, m_isQuadrupedNarrow) == 0x42);
	static_assert(offsetof(hkbFootIkDriverInfo, m_keepSourceFootEndAboveGround) == 0x43);

	class hkbFootIkDriverInfoLeg
	{
	public:
		/* 0x000 */ hkVector4f m_kneeAxisLS;
		/* 0x010 */ hkVector4f m_footEndLS;
		/* 0x020 */ float m_footPlantedAnkleHeightMS;
		/* 0x024 */ float m_footRaisedAnkleHeightMS;
		/* 0x028 */ float m_maxAnkleHeightMS;
		/* 0x02C */ float m_minAnkleHeightMS;
		/* 0x030 */ float m_maxKneeAngleDegrees;
		/* 0x034 */ float m_minKneeAngleDegrees;
		/* 0x038 */ float m_maxAnkleAngleDegrees;
		/* 0x03C */ std::int16_t m_hipIndex;
		/* 0x03E */ std::int16_t m_hipSiblingIndex;
		/* 0x040 */ std::int16_t m_kneeIndex;
		/* 0x042 */ std::int16_t m_kneeSiblingIndex;
		/* 0x044 */ std::int16_t m_ankleIndex;
		/* 0x046 */ std::uint8_t pad046[0xA];
	};
	static_assert(sizeof(hkbFootIkDriverInfoLeg) == 0x50);
	static_assert(offsetof(hkbFootIkDriverInfoLeg, m_kneeAxisLS) == 0x0);
	static_assert(offsetof(hkbFootIkDriverInfoLeg, m_footEndLS) == 0x10);
	static_assert(offsetof(hkbFootIkDriverInfoLeg, m_footPlantedAnkleHeightMS) == 0x20);
	static_assert(offsetof(hkbFootIkDriverInfoLeg, m_footRaisedAnkleHeightMS) == 0x24);
	static_assert(offsetof(hkbFootIkDriverInfoLeg, m_maxAnkleHeightMS) == 0x28);
	static_assert(offsetof(hkbFootIkDriverInfoLeg, m_minAnkleHeightMS) == 0x2C);
	static_assert(offsetof(hkbFootIkDriverInfoLeg, m_maxKneeAngleDegrees) == 0x30);
	static_assert(offsetof(hkbFootIkDriverInfoLeg, m_minKneeAngleDegrees) == 0x34);
	static_assert(offsetof(hkbFootIkDriverInfoLeg, m_maxAnkleAngleDegrees) == 0x38);
	static_assert(offsetof(hkbFootIkDriverInfoLeg, m_hipIndex) == 0x3C);
	static_assert(offsetof(hkbFootIkDriverInfoLeg, m_hipSiblingIndex) == 0x3E);
	static_assert(offsetof(hkbFootIkDriverInfoLeg, m_kneeIndex) == 0x40);
	static_assert(offsetof(hkbFootIkDriverInfoLeg, m_kneeSiblingIndex) == 0x42);
	static_assert(offsetof(hkbFootIkDriverInfoLeg, m_ankleIndex) == 0x44);

	class hkbFootIkModifierInternalLegData
	{
	public:
		/* 0x000 */ hkVector4f m_groundPosition;
		/* 0x010 */ void* m_footIkSolver;
		/* 0x018 */ std::uint8_t pad018[0x8];
	};
	static_assert(sizeof(hkbFootIkModifierInternalLegData) == 0x20);
	static_assert(offsetof(hkbFootIkModifierInternalLegData, m_groundPosition) == 0x0);
	static_assert(offsetof(hkbFootIkModifierInternalLegData, m_footIkSolver) == 0x10);

	class hkbFootIkModifierLeg
	{
	public:
		/* 0x000 */ hkQsTransformf m_originalAnkleTransformMS;
		/* 0x030 */ hkVector4f m_kneeAxisLS;
		/* 0x040 */ hkVector4f m_footEndLS;
		/* 0x050 */ hkbEventProperty m_ungroundedEvent;
		/* 0x060 */ float m_footPlantedAnkleHeightMS;
		/* 0x064 */ float m_footRaisedAnkleHeightMS;
		/* 0x068 */ float m_maxAnkleHeightMS;
		/* 0x06C */ float m_minAnkleHeightMS;
		/* 0x070 */ float m_maxKneeAngleDegrees;
		/* 0x074 */ float m_minKneeAngleDegrees;
		/* 0x078 */ float m_verticalError;
		/* 0x07C */ std::int16_t m_hipIndex;
		/* 0x07E */ std::int16_t m_kneeIndex;
		/* 0x080 */ std::int16_t m_ankleIndex;
		/* 0x082 */ hkBool m_hitSomething;
		/* 0x083 */ hkBool m_isPlantedMS;
		/* 0x084 */ hkBool m_isOriginalAnkleTransformMSSet;
		/* 0x085 */ std::uint8_t pad085[0xB];
	};
	static_assert(sizeof(hkbFootIkModifierLeg) == 0x90);
	static_assert(offsetof(hkbFootIkModifierLeg, m_originalAnkleTransformMS) == 0x0);
	static_assert(offsetof(hkbFootIkModifierLeg, m_kneeAxisLS) == 0x30);
	static_assert(offsetof(hkbFootIkModifierLeg, m_footEndLS) == 0x40);
	static_assert(offsetof(hkbFootIkModifierLeg, m_ungroundedEvent) == 0x50);
	static_assert(offsetof(hkbFootIkModifierLeg, m_footPlantedAnkleHeightMS) == 0x60);
	static_assert(offsetof(hkbFootIkModifierLeg, m_footRaisedAnkleHeightMS) == 0x64);
	static_assert(offsetof(hkbFootIkModifierLeg, m_maxAnkleHeightMS) == 0x68);
	static_assert(offsetof(hkbFootIkModifierLeg, m_minAnkleHeightMS) == 0x6C);
	static_assert(offsetof(hkbFootIkModifierLeg, m_maxKneeAngleDegrees) == 0x70);
	static_assert(offsetof(hkbFootIkModifierLeg, m_minKneeAngleDegrees) == 0x74);
	static_assert(offsetof(hkbFootIkModifierLeg, m_verticalError) == 0x78);
	static_assert(offsetof(hkbFootIkModifierLeg, m_hipIndex) == 0x7C);
	static_assert(offsetof(hkbFootIkModifierLeg, m_kneeIndex) == 0x7E);
	static_assert(offsetof(hkbFootIkModifierLeg, m_ankleIndex) == 0x80);
	static_assert(offsetof(hkbFootIkModifierLeg, m_hitSomething) == 0x82);
	static_assert(offsetof(hkbFootIkModifierLeg, m_isPlantedMS) == 0x83);
	static_assert(offsetof(hkbFootIkModifierLeg, m_isOriginalAnkleTransformMSSet) == 0x84);

	class hkbNode : public hkbBindable
	{
	public:
		enum class GetChildrenFlagBits : std::int32_t
		{
			FLAG_ACTIVE_ONLY = 1,
			FLAG_GENERATORS_ONLY = 4,
			FLAG_IGNORE_REFERENCED_BEHAVIORS = 8,
		};

		enum class CloneState : std::int32_t
		{
			CLONE_STATE_DEFAULT = 0,
			CLONE_STATE_TEMPLATE = 1,
			CLONE_STATE_CLONE = 2,
		};

		enum class TemplateOrClone : std::int32_t
		{
			NODE_IS_TEMPLATE = 0,
			NODE_IS_CLONE = 1,
		};

		/* 0x030 */ std::uintptr_t m_userData;
		/* 0x038 */ hkStringPtr m_name;
		/* 0x040 */ std::uint16_t m_id;
		/* 0x042 */ std::int8_t m_cloneState;
		/* 0x043 */ std::uint8_t m_type;
		/* 0x044 */ std::uint8_t pad044[0x4];
		/* 0x048 */ void* m_nodeInfo;

	private:
		/* 07 */ virtual void activate(const void* Context); // 0x38 - hkbContext
		/* 08 */ virtual void update(const void* Context, float Timestep); // 0x40
		/* 09 */ virtual void sub_hkbNode_09();
		/* 0A */ virtual void deactivate(const void* Context); // 0x50
		/* 0B */ virtual void sub_hkbNode_0B();
		/* 0C */ virtual void sub_hkbNode_0C();
		/* 0D */ virtual void sub_hkbNode_0D();
		/* 0E */ virtual void sub_hkbNode_0E();
		/* 0F */ virtual void sub_hkbNode_0F();
		/* 10 */ virtual void sub_hkbNode_10();
		/* 11 */ virtual void sub_hkbNode_11();
		/* 12 */ virtual void sub_hkbNode_12();
		/* 13 */ virtual void sub_hkbNode_13();
		/* 14 */ virtual void sub_hkbNode_14();
		/* 15 */ virtual void sub_hkbNode_15();
		/* 16 */ virtual void sub_hkbNode_16();
	};
	static_assert(sizeof(hkbNode) == 0x50);
	static_assert(offsetof(hkbNode, m_userData) == 0x30);
	static_assert(offsetof(hkbNode, m_name) == 0x38);
	static_assert(offsetof(hkbNode, m_id) == 0x40);
	static_assert(offsetof(hkbNode, m_cloneState) == 0x42);
	static_assert(offsetof(hkbNode, m_type) == 0x43);
	static_assert(offsetof(hkbNode, m_nodeInfo) == 0x48);

	class hkbGenerator : public hkbNode
	{
	public:
		/* 0x050 */ hkbGeneratorPartitionInfo m_partitionInfo;
		/* 0x078 */ void* m_syncInfo;
		/* 0x080 */ std::int8_t m_pad[4];
		/* 0x084 */ std::uint8_t pad084[0x4];

	private:
		/* 17 */ virtual void sub_hkbGenerator_17();
		/* 18 */ virtual void sub_hkbGenerator_18();
		/* 19 */ virtual void sub_hkbGenerator_19();
		/* 1A */ virtual void sub_hkbGenerator_1A();
		/* 1B */ virtual void sub_hkbGenerator_1B();
		/* 1C */ virtual void sub_hkbGenerator_1C();
		/* 1D */ virtual void sub_hkbGenerator_1D();
	};
	static_assert(sizeof(hkbGenerator) == 0x88);
	static_assert(offsetof(hkbGenerator, m_partitionInfo) == 0x50);
	static_assert(offsetof(hkbGenerator, m_syncInfo) == 0x78);
	static_assert(offsetof(hkbGenerator, m_pad) == 0x80);

	class hkbGeneratorOutputListener : public hkReferencedObject
	{
	public:

	private:
		/* 04 */ virtual void sub_hkbGeneratorOutputListener_04();
	};
	static_assert(sizeof(hkbGeneratorOutputListener) == 0x10);

	class hkbGeneratorTransitionEffectInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ float m_timeInTransition;
		/* 0x014 */ float m_duration;
		/* 0x018 */ float m_effectiveBlendInDuration;
		/* 0x01C */ float m_effectiveBlendOutDuration;
		/* 0x020 */ std::int8_t m_toGeneratorState;
		/* 0x021 */ hkBool m_echoTransitionGenerator;
		/* 0x022 */ std::int8_t m_toGeneratorSelfTransitionMode;
		/* 0x023 */ hkBool m_justActivated;
		/* 0x024 */ hkBool m_updateActiveNodes;
		/* 0x025 */ std::int8_t m_stage;
		/* 0x026 */ std::uint8_t pad026[0x2];
	};
	static_assert(sizeof(hkbGeneratorTransitionEffectInternalState) == 0x28);
	static_assert(offsetof(hkbGeneratorTransitionEffectInternalState, m_timeInTransition) == 0x10);
	static_assert(offsetof(hkbGeneratorTransitionEffectInternalState, m_duration) == 0x14);
	static_assert(offsetof(hkbGeneratorTransitionEffectInternalState, m_effectiveBlendInDuration) == 0x18);
	static_assert(offsetof(hkbGeneratorTransitionEffectInternalState, m_effectiveBlendOutDuration) == 0x1C);
	static_assert(offsetof(hkbGeneratorTransitionEffectInternalState, m_toGeneratorState) == 0x20);
	static_assert(offsetof(hkbGeneratorTransitionEffectInternalState, m_echoTransitionGenerator) == 0x21);
	static_assert(offsetof(hkbGeneratorTransitionEffectInternalState, m_toGeneratorSelfTransitionMode) == 0x22);
	static_assert(offsetof(hkbGeneratorTransitionEffectInternalState, m_justActivated) == 0x23);
	static_assert(offsetof(hkbGeneratorTransitionEffectInternalState, m_updateActiveNodes) == 0x24);
	static_assert(offsetof(hkbGeneratorTransitionEffectInternalState, m_stage) == 0x25);

	class hkbGetUpModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ float m_timeSinceBegin;
		/* 0x014 */ float m_timeStep;
		/* 0x018 */ hkBool m_initNextModify;
		/* 0x019 */ std::uint8_t pad019[0x7];
	};
	static_assert(sizeof(hkbGetUpModifierInternalState) == 0x20);
	static_assert(offsetof(hkbGetUpModifierInternalState, m_timeSinceBegin) == 0x10);
	static_assert(offsetof(hkbGetUpModifierInternalState, m_timeStep) == 0x14);
	static_assert(offsetof(hkbGetUpModifierInternalState, m_initNextModify) == 0x18);

	class hkbGetWorldFromModelModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkVector4f m_translationOut;
		/* 0x020 */ hkQuaternionf m_rotationOut;
	};
	static_assert(sizeof(hkbGetWorldFromModelModifierInternalState) == 0x30);
	static_assert(offsetof(hkbGetWorldFromModelModifierInternalState, m_translationOut) == 0x10);
	static_assert(offsetof(hkbGetWorldFromModelModifierInternalState, m_rotationOut) == 0x20);

	class hkbHandIkControlData
	{
	public:
		enum class HandleChangeMode : std::int32_t
		{
			HANDLE_CHANGE_MODE_ABRUPT = 0,
			HANDLE_CHANGE_MODE_CONSTANT_VELOCITY = 1,
		};

		/* 0x000 */ hkVector4f m_targetPosition;
		/* 0x010 */ hkQuaternionf m_targetRotation;
		/* 0x020 */ hkVector4f m_targetNormal;
		/* 0x030 */ hkbHandle* m_targetHandle;
		/* 0x038 */ float m_transformOnFraction;
		/* 0x03C */ float m_normalOnFraction;
		/* 0x040 */ float m_fadeInDuration;
		/* 0x044 */ float m_fadeOutDuration;
		/* 0x048 */ float m_extrapolationTimeStep;
		/* 0x04C */ float m_handleChangeSpeed;
		/* 0x050 */ std::int8_t m_handleChangeMode;
		/* 0x051 */ hkBool m_fixUp;
		/* 0x052 */ std::uint8_t pad052[0xE];
	};
	static_assert(sizeof(hkbHandIkControlData) == 0x60);
	static_assert(offsetof(hkbHandIkControlData, m_targetPosition) == 0x0);
	static_assert(offsetof(hkbHandIkControlData, m_targetRotation) == 0x10);
	static_assert(offsetof(hkbHandIkControlData, m_targetNormal) == 0x20);
	static_assert(offsetof(hkbHandIkControlData, m_targetHandle) == 0x30);
	static_assert(offsetof(hkbHandIkControlData, m_transformOnFraction) == 0x38);
	static_assert(offsetof(hkbHandIkControlData, m_normalOnFraction) == 0x3C);
	static_assert(offsetof(hkbHandIkControlData, m_fadeInDuration) == 0x40);
	static_assert(offsetof(hkbHandIkControlData, m_fadeOutDuration) == 0x44);
	static_assert(offsetof(hkbHandIkControlData, m_extrapolationTimeStep) == 0x48);
	static_assert(offsetof(hkbHandIkControlData, m_handleChangeSpeed) == 0x4C);
	static_assert(offsetof(hkbHandIkControlData, m_handleChangeMode) == 0x50);
	static_assert(offsetof(hkbHandIkControlData, m_fixUp) == 0x51);

	class hkbHandIkControlsModifierHand
	{
	public:
		/* 0x000 */ hkbHandIkControlData m_controlData;
		/* 0x060 */ std::int32_t m_handIndex;
		/* 0x064 */ hkBool m_enable;
		/* 0x065 */ std::uint8_t pad065[0xB];
	};
	static_assert(sizeof(hkbHandIkControlsModifierHand) == 0x70);
	static_assert(offsetof(hkbHandIkControlsModifierHand, m_controlData) == 0x0);
	static_assert(offsetof(hkbHandIkControlsModifierHand, m_handIndex) == 0x60);
	static_assert(offsetof(hkbHandIkControlsModifierHand, m_enable) == 0x64);

	class hkbHandIkDriverInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkbHandIkDriverInfoHand> m_hands;
		/* 0x020 */ std::int8_t m_fadeInOutCurve;
		/* 0x021 */ std::uint8_t pad021[0x7];
	};
	static_assert(sizeof(hkbHandIkDriverInfo) == 0x28);
	static_assert(offsetof(hkbHandIkDriverInfo, m_hands) == 0x10);
	static_assert(offsetof(hkbHandIkDriverInfo, m_fadeInOutCurve) == 0x20);

	class hkbHandIkDriverInfoHand
	{
	public:
		/* 0x000 */ hkVector4f m_elbowAxisLS;
		/* 0x010 */ hkVector4f m_backHandNormalLS;
		/* 0x020 */ hkVector4f m_handOffsetLS;
		/* 0x030 */ hkQuaternionf m_handOrienationOffsetLS;
		/* 0x040 */ float m_maxElbowAngleDegrees;
		/* 0x044 */ float m_minElbowAngleDegrees;
		/* 0x048 */ std::int16_t m_shoulderIndex;
		/* 0x04A */ std::int16_t m_shoulderSiblingIndex;
		/* 0x04C */ std::int16_t m_elbowIndex;
		/* 0x04E */ std::int16_t m_elbowSiblingIndex;
		/* 0x050 */ std::int16_t m_wristIndex;
		/* 0x052 */ hkBool m_enforceEndPosition;
		/* 0x053 */ hkBool m_enforceEndRotation;
		/* 0x054 */ std::uint8_t pad054[0x4];
		/* 0x058 */ hkStringPtr m_localFrameName;
	};
	static_assert(sizeof(hkbHandIkDriverInfoHand) == 0x60);
	static_assert(offsetof(hkbHandIkDriverInfoHand, m_elbowAxisLS) == 0x0);
	static_assert(offsetof(hkbHandIkDriverInfoHand, m_backHandNormalLS) == 0x10);
	static_assert(offsetof(hkbHandIkDriverInfoHand, m_handOffsetLS) == 0x20);
	static_assert(offsetof(hkbHandIkDriverInfoHand, m_handOrienationOffsetLS) == 0x30);
	static_assert(offsetof(hkbHandIkDriverInfoHand, m_maxElbowAngleDegrees) == 0x40);
	static_assert(offsetof(hkbHandIkDriverInfoHand, m_minElbowAngleDegrees) == 0x44);
	static_assert(offsetof(hkbHandIkDriverInfoHand, m_shoulderIndex) == 0x48);
	static_assert(offsetof(hkbHandIkDriverInfoHand, m_shoulderSiblingIndex) == 0x4A);
	static_assert(offsetof(hkbHandIkDriverInfoHand, m_elbowIndex) == 0x4C);
	static_assert(offsetof(hkbHandIkDriverInfoHand, m_elbowSiblingIndex) == 0x4E);
	static_assert(offsetof(hkbHandIkDriverInfoHand, m_wristIndex) == 0x50);
	static_assert(offsetof(hkbHandIkDriverInfoHand, m_enforceEndPosition) == 0x52);
	static_assert(offsetof(hkbHandIkDriverInfoHand, m_enforceEndRotation) == 0x53);
	static_assert(offsetof(hkbHandIkDriverInfoHand, m_localFrameName) == 0x58);

	class hkbHandIkModifierHand
	{
	public:
		/* 0x000 */ hkVector4f m_elbowAxisLS;
		/* 0x010 */ hkVector4f m_backHandNormalLS;
		/* 0x020 */ hkVector4f m_handOffsetLS;
		/* 0x030 */ hkQuaternionf m_handOrientationOffsetLS;
		/* 0x040 */ float m_maxElbowAngleDegrees;
		/* 0x044 */ float m_minElbowAngleDegrees;
		/* 0x048 */ std::int16_t m_shoulderIndex;
		/* 0x04A */ std::int16_t m_shoulderSiblingIndex;
		/* 0x04C */ std::int16_t m_elbowIndex;
		/* 0x04E */ std::int16_t m_elbowSiblingIndex;
		/* 0x050 */ std::int16_t m_wristIndex;
		/* 0x052 */ hkBool m_enforceEndPosition;
		/* 0x053 */ hkBool m_enforceEndRotation;
		/* 0x054 */ std::uint8_t pad054[0x4];
		/* 0x058 */ hkStringPtr m_localFrameName;
	};
	static_assert(sizeof(hkbHandIkModifierHand) == 0x60);
	static_assert(offsetof(hkbHandIkModifierHand, m_elbowAxisLS) == 0x0);
	static_assert(offsetof(hkbHandIkModifierHand, m_backHandNormalLS) == 0x10);
	static_assert(offsetof(hkbHandIkModifierHand, m_handOffsetLS) == 0x20);
	static_assert(offsetof(hkbHandIkModifierHand, m_handOrientationOffsetLS) == 0x30);
	static_assert(offsetof(hkbHandIkModifierHand, m_maxElbowAngleDegrees) == 0x40);
	static_assert(offsetof(hkbHandIkModifierHand, m_minElbowAngleDegrees) == 0x44);
	static_assert(offsetof(hkbHandIkModifierHand, m_shoulderIndex) == 0x48);
	static_assert(offsetof(hkbHandIkModifierHand, m_shoulderSiblingIndex) == 0x4A);
	static_assert(offsetof(hkbHandIkModifierHand, m_elbowIndex) == 0x4C);
	static_assert(offsetof(hkbHandIkModifierHand, m_elbowSiblingIndex) == 0x4E);
	static_assert(offsetof(hkbHandIkModifierHand, m_wristIndex) == 0x50);
	static_assert(offsetof(hkbHandIkModifierHand, m_enforceEndPosition) == 0x52);
	static_assert(offsetof(hkbHandIkModifierHand, m_enforceEndRotation) == 0x53);
	static_assert(offsetof(hkbHandIkModifierHand, m_localFrameName) == 0x58);

	class hkbHandle : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkLocalFrame* m_frame;
		/* 0x018 */ hkReferencedObject* m_rigidBody;
		/* 0x020 */ hkbCharacter* m_character;
		/* 0x028 */ std::int16_t m_animationBoneIndex;
		/* 0x02A */ std::uint8_t pad02A[0x6];
	};
	static_assert(sizeof(hkbHandle) == 0x30);
	static_assert(offsetof(hkbHandle, m_frame) == 0x10);
	static_assert(offsetof(hkbHandle, m_rigidBody) == 0x18);
	static_assert(offsetof(hkbHandle, m_character) == 0x20);
	static_assert(offsetof(hkbHandle, m_animationBoneIndex) == 0x28);

	class hkbIntEventPayload : public hkbEventPayload
	{
	public:
		/* 0x010 */ std::int32_t m_data;
		/* 0x014 */ std::uint8_t pad014[0x4];
	};
	static_assert(sizeof(hkbIntEventPayload) == 0x18);
	static_assert(offsetof(hkbIntEventPayload, m_data) == 0x10);

	class hkbIntVariableSequencedData : public hkbSequencedData
	{
	public:
		/* 0x010 */ hkArray<hkbIntVariableSequencedDataSample> m_samples;
		/* 0x020 */ std::int32_t m_variableIndex;
		/* 0x024 */ std::uint8_t pad024[0x4];
	};
	static_assert(sizeof(hkbIntVariableSequencedData) == 0x28);
	static_assert(offsetof(hkbIntVariableSequencedData, m_samples) == 0x10);
	static_assert(offsetof(hkbIntVariableSequencedData, m_variableIndex) == 0x20);

	class hkbIntVariableSequencedDataSample
	{
	public:
		/* 0x000 */ float m_time;
		/* 0x004 */ std::int32_t m_value;
	};
	static_assert(sizeof(hkbIntVariableSequencedDataSample) == 0x8);
	static_assert(offsetof(hkbIntVariableSequencedDataSample, m_time) == 0x0);
	static_assert(offsetof(hkbIntVariableSequencedDataSample, m_value) == 0x4);

	class hkbJigglerGroup : public hkbBindable
	{
	public:
		/* 0x030 */ hkbBoneIndexArray* m_boneIndices;
		/* 0x038 */ float m_mass;
		/* 0x03C */ float m_stiffness;
		/* 0x040 */ float m_damping;
		/* 0x044 */ float m_maxElongation;
		/* 0x048 */ float m_maxCompression;
		/* 0x04C */ hkBool m_propagateToChildren;
		/* 0x04D */ hkBool m_affectSiblings;
		/* 0x04E */ hkBool m_rotateBonesForSkinning;
		/* 0x04F */ hkBool m_pad[5];
		/* 0x054 */ std::uint8_t pad054[0x4];
	};
	static_assert(sizeof(hkbJigglerGroup) == 0x58);
	static_assert(offsetof(hkbJigglerGroup, m_boneIndices) == 0x30);
	static_assert(offsetof(hkbJigglerGroup, m_mass) == 0x38);
	static_assert(offsetof(hkbJigglerGroup, m_stiffness) == 0x3C);
	static_assert(offsetof(hkbJigglerGroup, m_damping) == 0x40);
	static_assert(offsetof(hkbJigglerGroup, m_maxElongation) == 0x44);
	static_assert(offsetof(hkbJigglerGroup, m_maxCompression) == 0x48);
	static_assert(offsetof(hkbJigglerGroup, m_propagateToChildren) == 0x4C);
	static_assert(offsetof(hkbJigglerGroup, m_affectSiblings) == 0x4D);
	static_assert(offsetof(hkbJigglerGroup, m_rotateBonesForSkinning) == 0x4E);
	static_assert(offsetof(hkbJigglerGroup, m_pad) == 0x4F);

	class hkbJigglerModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkVector4f> m_currentVelocitiesWS;
		/* 0x020 */ hkArray<hkVector4f> m_currentPositions;
		/* 0x030 */ float m_timeStep;
		/* 0x034 */ hkBool m_initNextModify;
		/* 0x035 */ std::uint8_t pad035[0x3];
	};
	static_assert(sizeof(hkbJigglerModifierInternalState) == 0x38);
	static_assert(offsetof(hkbJigglerModifierInternalState, m_currentVelocitiesWS) == 0x10);
	static_assert(offsetof(hkbJigglerModifierInternalState, m_currentPositions) == 0x20);
	static_assert(offsetof(hkbJigglerModifierInternalState, m_timeStep) == 0x30);
	static_assert(offsetof(hkbJigglerModifierInternalState, m_initNextModify) == 0x34);

	class hkbKeyFrameControlData
	{
	public:
		/* 0x000 */ float m_hierarchyGain;
		/* 0x004 */ float m_velocityDamping;
		/* 0x008 */ float m_accelerationGain;
		/* 0x00C */ float m_velocityGain;
		/* 0x010 */ float m_positionGain;
		/* 0x014 */ float m_positionMaxLinearVelocity;
		/* 0x018 */ float m_positionMaxAngularVelocity;
		/* 0x01C */ float m_snapGain;
		/* 0x020 */ float m_snapMaxLinearVelocity;
		/* 0x024 */ float m_snapMaxAngularVelocity;
		/* 0x028 */ float m_snapMaxLinearDistance;
		/* 0x02C */ float m_snapMaxAngularDistance;
	};
	static_assert(sizeof(hkbKeyFrameControlData) == 0x30);
	static_assert(offsetof(hkbKeyFrameControlData, m_hierarchyGain) == 0x0);
	static_assert(offsetof(hkbKeyFrameControlData, m_velocityDamping) == 0x4);
	static_assert(offsetof(hkbKeyFrameControlData, m_accelerationGain) == 0x8);
	static_assert(offsetof(hkbKeyFrameControlData, m_velocityGain) == 0xC);
	static_assert(offsetof(hkbKeyFrameControlData, m_positionGain) == 0x10);
	static_assert(offsetof(hkbKeyFrameControlData, m_positionMaxLinearVelocity) == 0x14);
	static_assert(offsetof(hkbKeyFrameControlData, m_positionMaxAngularVelocity) == 0x18);
	static_assert(offsetof(hkbKeyFrameControlData, m_snapGain) == 0x1C);
	static_assert(offsetof(hkbKeyFrameControlData, m_snapMaxLinearVelocity) == 0x20);
	static_assert(offsetof(hkbKeyFrameControlData, m_snapMaxAngularVelocity) == 0x24);
	static_assert(offsetof(hkbKeyFrameControlData, m_snapMaxLinearDistance) == 0x28);
	static_assert(offsetof(hkbKeyFrameControlData, m_snapMaxAngularDistance) == 0x2C);

	class hkbKeyframeBonesModifierKeyframeInfo
	{
	public:
		/* 0x000 */ hkVector4f m_keyframedPosition;
		/* 0x010 */ hkQuaternionf m_keyframedRotation;
		/* 0x020 */ std::int16_t m_boneIndex;
		/* 0x022 */ hkBool m_isValid;
		/* 0x023 */ std::uint8_t pad023[0xD];
	};
	static_assert(sizeof(hkbKeyframeBonesModifierKeyframeInfo) == 0x30);
	static_assert(offsetof(hkbKeyframeBonesModifierKeyframeInfo, m_keyframedPosition) == 0x0);
	static_assert(offsetof(hkbKeyframeBonesModifierKeyframeInfo, m_keyframedRotation) == 0x10);
	static_assert(offsetof(hkbKeyframeBonesModifierKeyframeInfo, m_boneIndex) == 0x20);
	static_assert(offsetof(hkbKeyframeBonesModifierKeyframeInfo, m_isValid) == 0x22);

	class hkbLayer : public hkbBindable
	{
	public:
		/* 0x030 */ hkbGenerator* m_generator;
		/* 0x038 */ float m_weight;
		/* 0x03C */ std::uint8_t pad03C[0x4];
		/* 0x040 */ hkbBoneWeightArray* m_boneWeights;
		/* 0x048 */ float m_fadeInDuration;
		/* 0x04C */ float m_fadeOutDuration;
		/* 0x050 */ std::int32_t m_onEventId;
		/* 0x054 */ std::int32_t m_offEventId;
		/* 0x058 */ hkBool m_onByDefault;
		/* 0x059 */ hkBool m_useMotion;
		/* 0x05A */ hkBool m_forceFullFadeDurations;
		/* 0x05B */ std::uint8_t pad05B[0x5];
	};
	static_assert(sizeof(hkbLayer) == 0x60);
	static_assert(offsetof(hkbLayer, m_generator) == 0x30);
	static_assert(offsetof(hkbLayer, m_weight) == 0x38);
	static_assert(offsetof(hkbLayer, m_boneWeights) == 0x40);
	static_assert(offsetof(hkbLayer, m_fadeInDuration) == 0x48);
	static_assert(offsetof(hkbLayer, m_fadeOutDuration) == 0x4C);
	static_assert(offsetof(hkbLayer, m_onEventId) == 0x50);
	static_assert(offsetof(hkbLayer, m_offEventId) == 0x54);
	static_assert(offsetof(hkbLayer, m_onByDefault) == 0x58);
	static_assert(offsetof(hkbLayer, m_useMotion) == 0x59);
	static_assert(offsetof(hkbLayer, m_forceFullFadeDurations) == 0x5A);

	class hkbLayerGeneratorInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::int32_t m_numActiveLayers;
		/* 0x014 */ std::uint8_t pad014[0x4];
		/* 0x018 */ hkArray<hkbLayerGeneratorLayerInternalState> m_layerInternalStates;
		/* 0x028 */ hkBool m_initSync;
		/* 0x029 */ std::uint8_t pad029[0x7];
	};
	static_assert(sizeof(hkbLayerGeneratorInternalState) == 0x30);
	static_assert(offsetof(hkbLayerGeneratorInternalState, m_numActiveLayers) == 0x10);
	static_assert(offsetof(hkbLayerGeneratorInternalState, m_layerInternalStates) == 0x18);
	static_assert(offsetof(hkbLayerGeneratorInternalState, m_initSync) == 0x28);

	class hkbLayerGeneratorLayerInternalState
	{
	public:
		enum class FadingState : std::int32_t
		{
			FADING_STATE_NONE = 0,
			FADING_STATE_IN = 1,
			FADING_STATE_OUT = 2,
		};

		/* 0x000 */ float m_weight;
		/* 0x004 */ float m_timeElapsed;
		/* 0x008 */ float m_onFraction;
		/* 0x00C */ std::int8_t m_fadingState;
		/* 0x00D */ hkBool m_useMotion;
		/* 0x00E */ hkBool m_syncNextFrame;
		/* 0x00F */ hkBool m_isActive;
	};
	static_assert(sizeof(hkbLayerGeneratorLayerInternalState) == 0x10);
	static_assert(offsetof(hkbLayerGeneratorLayerInternalState, m_weight) == 0x0);
	static_assert(offsetof(hkbLayerGeneratorLayerInternalState, m_timeElapsed) == 0x4);
	static_assert(offsetof(hkbLayerGeneratorLayerInternalState, m_onFraction) == 0x8);
	static_assert(offsetof(hkbLayerGeneratorLayerInternalState, m_fadingState) == 0xC);
	static_assert(offsetof(hkbLayerGeneratorLayerInternalState, m_useMotion) == 0xD);
	static_assert(offsetof(hkbLayerGeneratorLayerInternalState, m_syncNextFrame) == 0xE);
	static_assert(offsetof(hkbLayerGeneratorLayerInternalState, m_isActive) == 0xF);

	class hkbLinkedSymbolInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkStringPtr> m_eventNames;
		/* 0x020 */ hkArray<hkStringPtr> m_variableNames;
	};
	static_assert(sizeof(hkbLinkedSymbolInfo) == 0x30);
	static_assert(offsetof(hkbLinkedSymbolInfo, m_eventNames) == 0x10);
	static_assert(offsetof(hkbLinkedSymbolInfo, m_variableNames) == 0x20);

	class hkbLookAtModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkVector4f m_lookAtLastTargetWS;
		/* 0x020 */ float m_lookAtWeight;
		/* 0x024 */ hkBool m_isTargetInsideLimitCone;
		/* 0x025 */ std::uint8_t pad025[0xB];
	};
	static_assert(sizeof(hkbLookAtModifierInternalState) == 0x30);
	static_assert(offsetof(hkbLookAtModifierInternalState, m_lookAtLastTargetWS) == 0x10);
	static_assert(offsetof(hkbLookAtModifierInternalState, m_lookAtWeight) == 0x20);
	static_assert(offsetof(hkbLookAtModifierInternalState, m_isTargetInsideLimitCone) == 0x24);

	class hkbManualSelectorGeneratorInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::int16_t m_currentGeneratorIndex;
		/* 0x012 */ std::int16_t m_generatorIndexAtActivate;
		/* 0x014 */ std::uint8_t pad014[0x4];
		/* 0x018 */ hkArray<hkbStateMachineActiveTransitionInfo> m_activeTransitions;
	};
	static_assert(sizeof(hkbManualSelectorGeneratorInternalState) == 0x28);
	static_assert(offsetof(hkbManualSelectorGeneratorInternalState, m_currentGeneratorIndex) == 0x10);
	static_assert(offsetof(hkbManualSelectorGeneratorInternalState, m_generatorIndexAtActivate) == 0x12);
	static_assert(offsetof(hkbManualSelectorGeneratorInternalState, m_activeTransitions) == 0x18);

	class hkbManualSelectorTransitionEffectInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint8_t m_currentTransitionEffectIndex;
		/* 0x011 */ std::uint8_t pad011[0x7];
	};
	static_assert(sizeof(hkbManualSelectorTransitionEffectInternalState) == 0x18);
	static_assert(offsetof(hkbManualSelectorTransitionEffectInternalState, m_currentTransitionEffectIndex) == 0x10);

	class hkbMessageLog
	{
	public:
		/* 0x000 */ void* m_messages;
		/* 0x008 */ std::int32_t m_maxMessages;
		/* 0x00C */ std::uint8_t pad00C[0x4];
	};
	static_assert(sizeof(hkbMessageLog) == 0x10);
	static_assert(offsetof(hkbMessageLog, m_messages) == 0x0);
	static_assert(offsetof(hkbMessageLog, m_maxMessages) == 0x8);

	class hkbMirroredSkeletonInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkVector4f m_mirrorAxis;
		/* 0x020 */ hkArray<std::int16_t> m_bonePairMap;
		/* 0x030 */ hkArray<std::int16_t> m_partitionPairMap;
	};
	static_assert(sizeof(hkbMirroredSkeletonInfo) == 0x40);
	static_assert(offsetof(hkbMirroredSkeletonInfo, m_mirrorAxis) == 0x10);
	static_assert(offsetof(hkbMirroredSkeletonInfo, m_bonePairMap) == 0x20);
	static_assert(offsetof(hkbMirroredSkeletonInfo, m_partitionPairMap) == 0x30);

	class hkbModifier : public hkbNode
	{
	public:
		/* 0x050 */ hkBool m_enable;
		/* 0x051 */ hkBool m_padModifier[3];
		/* 0x054 */ std::uint8_t pad054[0x4];

	private:
		/* 17 */ virtual void sub_hkbModifier_17();
		/* 18 */ virtual void sub_hkbModifier_18();
		/* 19 */ virtual void sub_hkbModifier_19();
		/* 1A */ virtual void sub_hkbModifier_1A();
	};
	static_assert(sizeof(hkbModifier) == 0x58);
	static_assert(offsetof(hkbModifier, m_enable) == 0x50);
	static_assert(offsetof(hkbModifier, m_padModifier) == 0x51);

	class hkbMoveCharacterModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ float m_timeSinceLastModify;
		/* 0x014 */ std::uint8_t pad014[0x4];
	};
	static_assert(sizeof(hkbMoveCharacterModifierInternalState) == 0x18);
	static_assert(offsetof(hkbMoveCharacterModifierInternalState, m_timeSinceLastModify) == 0x10);

	class hkbNamedEventPayload : public hkbEventPayload
	{
	public:
		/* 0x010 */ hkStringPtr m_name;
	};
	static_assert(sizeof(hkbNamedEventPayload) == 0x18);
	static_assert(offsetof(hkbNamedEventPayload, m_name) == 0x10);

	class hkbNamedIntEventPayload : public hkbNamedEventPayload
	{
	public:
		/* 0x018 */ std::int32_t m_data;
		/* 0x01C */ std::uint8_t pad01C[0x4];
	};
	static_assert(sizeof(hkbNamedIntEventPayload) == 0x20);
	static_assert(offsetof(hkbNamedIntEventPayload, m_data) == 0x18);

	class hkbNamedRealEventPayload : public hkbNamedEventPayload
	{
	public:
		/* 0x018 */ float m_data;
		/* 0x01C */ std::uint8_t pad01C[0x4];
	};
	static_assert(sizeof(hkbNamedRealEventPayload) == 0x20);
	static_assert(offsetof(hkbNamedRealEventPayload, m_data) == 0x18);

	class hkbNamedStringEventPayload : public hkbNamedEventPayload
	{
	public:
		/* 0x018 */ hkStringPtr m_data;
	};
	static_assert(sizeof(hkbNamedStringEventPayload) == 0x20);
	static_assert(offsetof(hkbNamedStringEventPayload, m_data) == 0x18);

	class hkbNodeInternalStateInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkbReferencedGeneratorSyncInfo* m_syncInfo;
		/* 0x018 */ hkStringPtr m_name;
		/* 0x020 */ hkReferencedObject* m_internalState;
		/* 0x028 */ std::uint16_t m_nodeId;
		/* 0x02A */ hkBool m_hasActivateBeenCalled;
		/* 0x02B */ hkBool m_isModifierEnabled;
		/* 0x02C */ std::uint8_t pad02C[0x4];
	};
	static_assert(sizeof(hkbNodeInternalStateInfo) == 0x30);
	static_assert(offsetof(hkbNodeInternalStateInfo, m_syncInfo) == 0x10);
	static_assert(offsetof(hkbNodeInternalStateInfo, m_name) == 0x18);
	static_assert(offsetof(hkbNodeInternalStateInfo, m_internalState) == 0x20);
	static_assert(offsetof(hkbNodeInternalStateInfo, m_nodeId) == 0x28);
	static_assert(offsetof(hkbNodeInternalStateInfo, m_hasActivateBeenCalled) == 0x2A);
	static_assert(offsetof(hkbNodeInternalStateInfo, m_isModifierEnabled) == 0x2B);

	class hkbPhysicsInterface : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint8_t pad010[0x8];

	private:
		/* 04 */ virtual void sub_hkbPhysicsInterface_04();
		/* 05 */ virtual void sub_hkbPhysicsInterface_05();
		/* 06 */ virtual void sub_hkbPhysicsInterface_06();
		/* 07 */ virtual void sub_hkbPhysicsInterface_07();
		/* 08 */ virtual void sub_hkbPhysicsInterface_08();
		/* 09 */ virtual void sub_hkbPhysicsInterface_09();
		/* 0A */ virtual void sub_hkbPhysicsInterface_0A();
		/* 0B */ virtual void sub_hkbPhysicsInterface_0B();
		/* 0C */ virtual void sub_hkbPhysicsInterface_0C();
		/* 0D */ virtual void sub_hkbPhysicsInterface_0D();
		/* 0E */ virtual void sub_hkbPhysicsInterface_0E();
		/* 0F */ virtual void sub_hkbPhysicsInterface_0F();
		/* 10 */ virtual void sub_hkbPhysicsInterface_10();
		/* 11 */ virtual void sub_hkbPhysicsInterface_11();
		/* 12 */ virtual void sub_hkbPhysicsInterface_12();
		/* 13 */ virtual void sub_hkbPhysicsInterface_13();
		/* 14 */ virtual void sub_hkbPhysicsInterface_14();
		/* 15 */ virtual void sub_hkbPhysicsInterface_15();
		/* 16 */ virtual void sub_hkbPhysicsInterface_16();
		/* 17 */ virtual void sub_hkbPhysicsInterface_17();
		/* 18 */ virtual void sub_hkbPhysicsInterface_18();
		/* 19 */ virtual void sub_hkbPhysicsInterface_19();
		/* 1A */ virtual void sub_hkbPhysicsInterface_1A();
		/* 1B */ virtual void sub_hkbPhysicsInterface_1B();
		/* 1C */ virtual void sub_hkbPhysicsInterface_1C();
		/* 1D */ virtual void sub_hkbPhysicsInterface_1D();
	};
	static_assert(sizeof(hkbPhysicsInterface) == 0x18);

	class hkbNullPhysicsInterface : public hkbPhysicsInterface
	{
	public:
	};
	static_assert(sizeof(hkbNullPhysicsInterface) == 0x18);

	class hkbParticleSystemEventPayload : public hkbEventPayload
	{
	public:
		enum class SystemType : std::int32_t
		{
			DEBRIS = 0,
			DUST = 1,
			EXPLOSION = 2,
			SMOKE = 3,
			SPARKS = 4,
		};

		/* 0x010 */ std::uint8_t m_type;
		/* 0x011 */ std::uint8_t pad011[0x1];
		/* 0x012 */ std::int16_t m_emitBoneIndex;
		/* 0x014 */ std::uint8_t pad014[0xC];
		/* 0x020 */ hkVector4f m_offset;
		/* 0x030 */ hkVector4f m_direction;
		/* 0x040 */ std::int32_t m_numParticles;
		/* 0x044 */ float m_speed;
		/* 0x048 */ std::uint8_t pad048[0x8];
	};
	static_assert(sizeof(hkbParticleSystemEventPayload) == 0x50);
	static_assert(offsetof(hkbParticleSystemEventPayload, m_type) == 0x10);
	static_assert(offsetof(hkbParticleSystemEventPayload, m_emitBoneIndex) == 0x12);
	static_assert(offsetof(hkbParticleSystemEventPayload, m_offset) == 0x20);
	static_assert(offsetof(hkbParticleSystemEventPayload, m_direction) == 0x30);
	static_assert(offsetof(hkbParticleSystemEventPayload, m_numParticles) == 0x40);
	static_assert(offsetof(hkbParticleSystemEventPayload, m_speed) == 0x44);

	class hkbPoseMatchingGeneratorInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::int32_t m_currentMatch;
		/* 0x014 */ std::int32_t m_bestMatch;
		/* 0x018 */ float m_timeSinceBetterMatch;
		/* 0x01C */ float m_error;
		/* 0x020 */ hkBool m_resetCurrentMatchLocalTime;
		/* 0x021 */ std::uint8_t pad021[0x7];
	};
	static_assert(sizeof(hkbPoseMatchingGeneratorInternalState) == 0x28);
	static_assert(offsetof(hkbPoseMatchingGeneratorInternalState, m_currentMatch) == 0x10);
	static_assert(offsetof(hkbPoseMatchingGeneratorInternalState, m_bestMatch) == 0x14);
	static_assert(offsetof(hkbPoseMatchingGeneratorInternalState, m_timeSinceBetterMatch) == 0x18);
	static_assert(offsetof(hkbPoseMatchingGeneratorInternalState, m_error) == 0x1C);
	static_assert(offsetof(hkbPoseMatchingGeneratorInternalState, m_resetCurrentMatchLocalTime) == 0x20);

	class hkbPoseStoringGeneratorOutputListener : public hkbGeneratorOutputListener
	{
	public:
		using StoredPose = hkbPoseStoringGeneratorOutputListenerStoredPose;
		/* 0x010 */ hkArray<hkbPoseStoringGeneratorOutputListenerStoredPose*> m_storedPoses;
		/* 0x020 */ hkBool m_dirty;
		/* 0x021 */ std::uint8_t pad021[0x7];
		/* 0x028 */ void* m_nodeToIndexMap;
	};
	static_assert(sizeof(hkbPoseStoringGeneratorOutputListener) == 0x30);
	static_assert(offsetof(hkbPoseStoringGeneratorOutputListener, m_storedPoses) == 0x10);
	static_assert(offsetof(hkbPoseStoringGeneratorOutputListener, m_dirty) == 0x20);
	static_assert(offsetof(hkbPoseStoringGeneratorOutputListener, m_nodeToIndexMap) == 0x28);

	class hkbPoseStoringGeneratorOutputListenerStoredPose : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkbNode* m_node;
		/* 0x018 */ hkArray<hkQsTransformf> m_pose;
		/* 0x028 */ std::uint8_t pad028[0x8];
		/* 0x030 */ hkQsTransformf m_worldFromModel;
		/* 0x060 */ hkBool m_isPoseValid;
		/* 0x061 */ std::uint8_t pad061[0xF];
	};
	static_assert(sizeof(hkbPoseStoringGeneratorOutputListenerStoredPose) == 0x70);
	static_assert(offsetof(hkbPoseStoringGeneratorOutputListenerStoredPose, m_node) == 0x10);
	static_assert(offsetof(hkbPoseStoringGeneratorOutputListenerStoredPose, m_pose) == 0x18);
	static_assert(offsetof(hkbPoseStoringGeneratorOutputListenerStoredPose, m_worldFromModel) == 0x30);
	static_assert(offsetof(hkbPoseStoringGeneratorOutputListenerStoredPose, m_isPoseValid) == 0x60);

	class hkbPoweredRagdollControlData
	{
	public:
		/* 0x000 */ float m_maxForce;
		/* 0x004 */ float m_tau;
		/* 0x008 */ float m_damping;
		/* 0x00C */ float m_proportionalRecoveryVelocity;
		/* 0x010 */ float m_constantRecoveryVelocity;
		/* 0x014 */ std::uint8_t pad014[0xC];
	};
	static_assert(sizeof(hkbPoweredRagdollControlData) == 0x20);
	static_assert(offsetof(hkbPoweredRagdollControlData, m_maxForce) == 0x0);
	static_assert(offsetof(hkbPoweredRagdollControlData, m_tau) == 0x4);
	static_assert(offsetof(hkbPoweredRagdollControlData, m_damping) == 0x8);
	static_assert(offsetof(hkbPoweredRagdollControlData, m_proportionalRecoveryVelocity) == 0xC);
	static_assert(offsetof(hkbPoweredRagdollControlData, m_constantRecoveryVelocity) == 0x10);

	class hkbProjectData : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkVector4f m_worldUpWS;
		/* 0x020 */ hkbProjectStringData* m_stringData;
		/* 0x028 */ std::int8_t m_defaultEventMode;
		/* 0x029 */ std::uint8_t pad029[0x7];
	};
	static_assert(sizeof(hkbProjectData) == 0x30);
	static_assert(offsetof(hkbProjectData, m_worldUpWS) == 0x10);
	static_assert(offsetof(hkbProjectData, m_stringData) == 0x20);
	static_assert(offsetof(hkbProjectData, m_defaultEventMode) == 0x28);

	class hkbProjectStringData : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkStringPtr> m_animationFilenames;
		/* 0x020 */ hkArray<hkStringPtr> m_behaviorFilenames;
		/* 0x030 */ hkArray<hkStringPtr> m_characterFilenames;
		/* 0x040 */ hkArray<hkStringPtr> m_eventNames;
		/* 0x050 */ hkStringPtr m_animationPath;
		/* 0x058 */ hkStringPtr m_behaviorPath;
		/* 0x060 */ hkStringPtr m_characterPath;
		/* 0x068 */ hkStringPtr m_scriptsPath;
		/* 0x070 */ hkStringPtr m_fullPathToSource;
		/* 0x078 */ hkStringPtr m_rootPath;
	};
	static_assert(sizeof(hkbProjectStringData) == 0x80);
	static_assert(offsetof(hkbProjectStringData, m_animationFilenames) == 0x10);
	static_assert(offsetof(hkbProjectStringData, m_behaviorFilenames) == 0x20);
	static_assert(offsetof(hkbProjectStringData, m_characterFilenames) == 0x30);
	static_assert(offsetof(hkbProjectStringData, m_eventNames) == 0x40);
	static_assert(offsetof(hkbProjectStringData, m_animationPath) == 0x50);
	static_assert(offsetof(hkbProjectStringData, m_behaviorPath) == 0x58);
	static_assert(offsetof(hkbProjectStringData, m_characterPath) == 0x60);
	static_assert(offsetof(hkbProjectStringData, m_scriptsPath) == 0x68);
	static_assert(offsetof(hkbProjectStringData, m_fullPathToSource) == 0x70);
	static_assert(offsetof(hkbProjectStringData, m_rootPath) == 0x78);

	class hkbRadialSelectorGeneratorGeneratorInfo
	{
	public:
		/* 0x000 */ hkbGenerator* m_generator;
		/* 0x008 */ float m_angle;
		/* 0x00C */ float m_radialSpeed;
	};
	static_assert(sizeof(hkbRadialSelectorGeneratorGeneratorInfo) == 0x10);
	static_assert(offsetof(hkbRadialSelectorGeneratorGeneratorInfo, m_generator) == 0x0);
	static_assert(offsetof(hkbRadialSelectorGeneratorGeneratorInfo, m_angle) == 0x8);
	static_assert(offsetof(hkbRadialSelectorGeneratorGeneratorInfo, m_radialSpeed) == 0xC);

	class hkbRadialSelectorGeneratorGeneratorPair
	{
	public:
		/* 0x000 */ hkbRadialSelectorGeneratorGeneratorInfo m_generators[2];
		/* 0x020 */ float m_minAngle;
		/* 0x024 */ float m_maxAngle;
	};
	static_assert(sizeof(hkbRadialSelectorGeneratorGeneratorPair) == 0x28);
	static_assert(offsetof(hkbRadialSelectorGeneratorGeneratorPair, m_generators) == 0x0);
	static_assert(offsetof(hkbRadialSelectorGeneratorGeneratorPair, m_minAngle) == 0x20);
	static_assert(offsetof(hkbRadialSelectorGeneratorGeneratorPair, m_maxAngle) == 0x24);

	class hkbRagdollControllerSetup
	{
	public:
		enum class Type : std::int32_t
		{
			POWERED = 1,
			RIGID_BODY = 2,
		};

		/* 0x000 */ std::int8_t m_type;
	};
	static_assert(sizeof(hkbRagdollControllerSetup) == 0x1);
	static_assert(offsetof(hkbRagdollControllerSetup, m_type) == 0x0);

	class hkbRagdollInterface : public hkReferencedObject
	{
	public:

	private:
		/* 04 */ virtual void sub_hkbRagdollInterface_04();
		/* 05 */ virtual void sub_hkbRagdollInterface_05();
		/* 06 */ virtual void sub_hkbRagdollInterface_06();
		/* 07 */ virtual void sub_hkbRagdollInterface_07();
		/* 08 */ virtual void sub_hkbRagdollInterface_08();
		/* 09 */ virtual void sub_hkbRagdollInterface_09();
		/* 0A */ virtual void sub_hkbRagdollInterface_0A();
		/* 0B */ virtual void sub_hkbRagdollInterface_0B();
		/* 0C */ virtual void sub_hkbRagdollInterface_0C();
		/* 0D */ virtual void sub_hkbRagdollInterface_0D();
		/* 0E */ virtual void sub_hkbRagdollInterface_0E();
		/* 0F */ virtual void sub_hkbRagdollInterface_0F();
		/* 10 */ virtual void sub_hkbRagdollInterface_10();
		/* 11 */ virtual void sub_hkbRagdollInterface_11();
		/* 12 */ virtual void sub_hkbRagdollInterface_12();
		/* 13 */ virtual void sub_hkbRagdollInterface_13();
		/* 14 */ virtual void sub_hkbRagdollInterface_14();
		/* 15 */ virtual void sub_hkbRagdollInterface_15();
		/* 16 */ virtual void sub_hkbRagdollInterface_16();
	};
	static_assert(sizeof(hkbRagdollInterface) == 0x10);

	class hkbRaiseEventCommand : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
		/* 0x018 */ hkBool m_global;
		/* 0x019 */ std::uint8_t pad019[0x3];
		/* 0x01C */ std::int32_t m_externalId;
	};
	static_assert(sizeof(hkbRaiseEventCommand) == 0x20);
	static_assert(offsetof(hkbRaiseEventCommand, m_characterId) == 0x10);
	static_assert(offsetof(hkbRaiseEventCommand, m_global) == 0x18);
	static_assert(offsetof(hkbRaiseEventCommand, m_externalId) == 0x1C);

	class hkbRealEventPayload : public hkbEventPayload
	{
	public:
		/* 0x010 */ float m_data;
		/* 0x014 */ std::uint8_t pad014[0x4];
	};
	static_assert(sizeof(hkbRealEventPayload) == 0x18);
	static_assert(offsetof(hkbRealEventPayload, m_data) == 0x10);

	class hkbRealVariableSequencedData : public hkbSequencedData
	{
	public:
		/* 0x010 */ hkArray<hkbRealVariableSequencedDataSample> m_samples;
		/* 0x020 */ std::int32_t m_variableIndex;
		/* 0x024 */ std::uint8_t pad024[0x4];
	};
	static_assert(sizeof(hkbRealVariableSequencedData) == 0x28);
	static_assert(offsetof(hkbRealVariableSequencedData, m_samples) == 0x10);
	static_assert(offsetof(hkbRealVariableSequencedData, m_variableIndex) == 0x20);

	class hkbRealVariableSequencedDataSample
	{
	public:
		/* 0x000 */ float m_time;
		/* 0x004 */ float m_value;
	};
	static_assert(sizeof(hkbRealVariableSequencedDataSample) == 0x8);
	static_assert(offsetof(hkbRealVariableSequencedDataSample, m_time) == 0x0);
	static_assert(offsetof(hkbRealVariableSequencedDataSample, m_value) == 0x4);

	class hkbRebuildScriptsCommand : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
	};
	static_assert(sizeof(hkbRebuildScriptsCommand) == 0x18);
	static_assert(offsetof(hkbRebuildScriptsCommand, m_characterId) == 0x10);

	class hkbReferencedGeneratorSyncInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkbGeneratorSyncInfo m_syncInfo;
	};
	static_assert(sizeof(hkbReferencedGeneratorSyncInfo) == 0xB8);
	static_assert(offsetof(hkbReferencedGeneratorSyncInfo, m_syncInfo) == 0x10);

	class hkbRigidBodyRagdollControlData
	{
	public:
		/* 0x000 */ hkbKeyFrameControlData m_keyFrameControlData;
		/* 0x030 */ float m_durationToBlend;
		/* 0x034 */ std::uint8_t pad034[0xC];
	};
	static_assert(sizeof(hkbRigidBodyRagdollControlData) == 0x40);
	static_assert(offsetof(hkbRigidBodyRagdollControlData, m_keyFrameControlData) == 0x0);
	static_assert(offsetof(hkbRigidBodyRagdollControlData, m_durationToBlend) == 0x30);

	class hkbRoleAttribute
	{
	public:
		enum class Role : std::int32_t
		{
			ROLE_DEFAULT = 0,
			ROLE_FILE_NAME = 1,
			ROLE_BONE_INDEX = 2,
			ROLE_EVENT_ID = 3,
			ROLE_VARIABLE_INDEX = 4,
			ROLE_ATTRIBUTE_INDEX = 5,
			ROLE_TIME = 6,
			ROLE_SCRIPT = 7,
			ROLE_LOCAL_FRAME = 8,
			ROLE_BONE_ATTACHMENT = 9,
		};

		enum class RoleFlags : std::int32_t
		{
			FLAG_NONE = 0,
			FLAG_RAGDOLL = 1,
			FLAG_NORMALIZED = 2,
			FLAG_NOT_VARIABLE = 4,
			FLAG_HIDDEN = 8,
			FLAG_OUTPUT = 16,
			FLAG_NOT_CHARACTER_PROPERTY = 32,
			FLAG_CHAIN = 64,
		};

		/* 0x000 */ std::int16_t m_role;
		/* 0x002 */ std::int16_t m_flags;
	};
	static_assert(sizeof(hkbRoleAttribute) == 0x4);
	static_assert(offsetof(hkbRoleAttribute, m_role) == 0x0);
	static_assert(offsetof(hkbRoleAttribute, m_flags) == 0x2);

	class hkbRotateCharacterModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ float m_angle;
		/* 0x014 */ std::uint8_t pad014[0x4];
	};
	static_assert(sizeof(hkbRotateCharacterModifierInternalState) == 0x18);
	static_assert(offsetof(hkbRotateCharacterModifierInternalState, m_angle) == 0x10);

	class hkbScriptCondition : public hkbCondition
	{
	public:
		/* 0x010 */ hkStringPtr m_conditionScript;
	};
	static_assert(sizeof(hkbScriptCondition) == 0x18);
	static_assert(offsetof(hkbScriptCondition, m_conditionScript) == 0x10);

	class hkbSenseHandleModifierRange
	{
	public:
		/* 0x000 */ hkbEventProperty m_event;
		/* 0x010 */ float m_minDistance;
		/* 0x014 */ float m_maxDistance;
		/* 0x018 */ hkBool m_ignoreHandle;
		/* 0x019 */ std::uint8_t pad019[0x7];
	};
	static_assert(sizeof(hkbSenseHandleModifierRange) == 0x20);
	static_assert(offsetof(hkbSenseHandleModifierRange, m_event) == 0x0);
	static_assert(offsetof(hkbSenseHandleModifierRange, m_minDistance) == 0x10);
	static_assert(offsetof(hkbSenseHandleModifierRange, m_maxDistance) == 0x14);
	static_assert(offsetof(hkbSenseHandleModifierRange, m_ignoreHandle) == 0x18);

	class hkbSequenceInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<std::int32_t> m_nextSampleEvents;
		/* 0x020 */ hkArray<std::int32_t> m_nextSampleReals;
		/* 0x030 */ hkArray<std::int32_t> m_nextSampleBools;
		/* 0x040 */ hkArray<std::int32_t> m_nextSampleInts;
		/* 0x050 */ float m_time;
		/* 0x054 */ hkBool m_isEnabled;
		/* 0x055 */ std::uint8_t pad055[0x3];
	};
	static_assert(sizeof(hkbSequenceInternalState) == 0x58);
	static_assert(offsetof(hkbSequenceInternalState, m_nextSampleEvents) == 0x10);
	static_assert(offsetof(hkbSequenceInternalState, m_nextSampleReals) == 0x20);
	static_assert(offsetof(hkbSequenceInternalState, m_nextSampleBools) == 0x30);
	static_assert(offsetof(hkbSequenceInternalState, m_nextSampleInts) == 0x40);
	static_assert(offsetof(hkbSequenceInternalState, m_time) == 0x50);
	static_assert(offsetof(hkbSequenceInternalState, m_isEnabled) == 0x54);

	class hkbSequenceStringData : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkStringPtr> m_eventNames;
		/* 0x020 */ hkArray<hkStringPtr> m_variableNames;
	};
	static_assert(sizeof(hkbSequenceStringData) == 0x30);
	static_assert(offsetof(hkbSequenceStringData, m_eventNames) == 0x10);
	static_assert(offsetof(hkbSequenceStringData, m_variableNames) == 0x20);

	class hkbSetBehaviorCommand : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
		/* 0x018 */ hkbBehaviorGraph* m_behavior;
		/* 0x020 */ hkbGenerator* m_rootGenerator;
		/* 0x028 */ hkArray<hkbBehaviorGraph*> m_referencedBehaviors;
		/* 0x038 */ std::int32_t m_startStateIndex;
		/* 0x03C */ hkBool m_randomizeSimulation;
		/* 0x03D */ std::uint8_t pad03D[0x3];
		/* 0x040 */ std::int32_t m_padding;
		/* 0x044 */ std::uint8_t pad044[0x4];
	};
	static_assert(sizeof(hkbSetBehaviorCommand) == 0x48);
	static_assert(offsetof(hkbSetBehaviorCommand, m_characterId) == 0x10);
	static_assert(offsetof(hkbSetBehaviorCommand, m_behavior) == 0x18);
	static_assert(offsetof(hkbSetBehaviorCommand, m_rootGenerator) == 0x20);
	static_assert(offsetof(hkbSetBehaviorCommand, m_referencedBehaviors) == 0x28);
	static_assert(offsetof(hkbSetBehaviorCommand, m_startStateIndex) == 0x38);
	static_assert(offsetof(hkbSetBehaviorCommand, m_randomizeSimulation) == 0x3C);
	static_assert(offsetof(hkbSetBehaviorCommand, m_padding) == 0x40);

	class hkbSetLocalTimeOfClipGeneratorCommand : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
		/* 0x018 */ float m_localTime;
		/* 0x01C */ std::uint16_t m_nodeId;
		/* 0x01E */ std::uint8_t pad01E[0x2];
	};
	static_assert(sizeof(hkbSetLocalTimeOfClipGeneratorCommand) == 0x20);
	static_assert(offsetof(hkbSetLocalTimeOfClipGeneratorCommand, m_characterId) == 0x10);
	static_assert(offsetof(hkbSetLocalTimeOfClipGeneratorCommand, m_localTime) == 0x18);
	static_assert(offsetof(hkbSetLocalTimeOfClipGeneratorCommand, m_nodeId) == 0x1C);

	class hkbVariableValue
	{
	public:
		/* 0x000 */ std::int32_t m_value;
	};
	static_assert(sizeof(hkbVariableValue) == 0x4);
	static_assert(offsetof(hkbVariableValue, m_value) == 0x0);

	class hkbSetNodePropertyCommand : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
		/* 0x018 */ hkStringPtr m_nodeName;
		/* 0x020 */ hkStringPtr m_propertyName;
		/* 0x028 */ hkbVariableValue m_propertyValue;
		/* 0x02C */ std::int32_t m_padding;
	};
	static_assert(sizeof(hkbSetNodePropertyCommand) == 0x30);
	static_assert(offsetof(hkbSetNodePropertyCommand, m_characterId) == 0x10);
	static_assert(offsetof(hkbSetNodePropertyCommand, m_nodeName) == 0x18);
	static_assert(offsetof(hkbSetNodePropertyCommand, m_propertyName) == 0x20);
	static_assert(offsetof(hkbSetNodePropertyCommand, m_propertyValue) == 0x28);
	static_assert(offsetof(hkbSetNodePropertyCommand, m_padding) == 0x2C);

	class hkbSetSelectedCharacterCommand : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
	};
	static_assert(sizeof(hkbSetSelectedCharacterCommand) == 0x18);
	static_assert(offsetof(hkbSetSelectedCharacterCommand, m_characterId) == 0x10);

	class hkbSetWordVariableCommand : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint64_t m_characterId;
		/* 0x018 */ hkStringPtr m_variableName;
		/* 0x020 */ hkbVariableValue m_value;
		/* 0x024 */ std::uint8_t pad024[0xC];
		/* 0x030 */ hkVector4f m_quadValue;
		/* 0x040 */ std::uint8_t m_type;
		/* 0x041 */ hkBool m_global;
		/* 0x042 */ std::uint8_t pad042[0xE];
	};
	static_assert(sizeof(hkbSetWordVariableCommand) == 0x50);
	static_assert(offsetof(hkbSetWordVariableCommand, m_characterId) == 0x10);
	static_assert(offsetof(hkbSetWordVariableCommand, m_variableName) == 0x18);
	static_assert(offsetof(hkbSetWordVariableCommand, m_value) == 0x20);
	static_assert(offsetof(hkbSetWordVariableCommand, m_quadValue) == 0x30);
	static_assert(offsetof(hkbSetWordVariableCommand, m_type) == 0x40);
	static_assert(offsetof(hkbSetWordVariableCommand, m_global) == 0x41);

	class hkbSimulationControlCommand : public hkReferencedObject
	{
	public:
		enum class SimulationControlCommand : std::int32_t
		{
			COMMAND_PLAY = 0,
			COMMAND_PAUSE = 1,
			COMMAND_STEP = 2,
			COMMAND_STOP = 3,
			COMMAND_ACCUMULATE_MOTION = 4,
			COMMAND_DO_NOT_ACCUMULATE_MOTION = 5,
		};

		/* 0x010 */ std::uint8_t m_command;
		/* 0x011 */ std::uint8_t pad011[0x7];
	};
	static_assert(sizeof(hkbSimulationControlCommand) == 0x18);
	static_assert(offsetof(hkbSimulationControlCommand, m_command) == 0x10);

	class hkbSimulationStateInfo : public hkReferencedObject
	{
	public:
		/* 0x010 */ std::uint8_t m_simulationState;
		/* 0x011 */ std::uint8_t pad011[0x7];
	};
	static_assert(sizeof(hkbSimulationStateInfo) == 0x18);
	static_assert(offsetof(hkbSimulationStateInfo, m_simulationState) == 0x10);

	class hkbStateChooser : public hkReferencedObject
	{
	public:
	};
	static_assert(sizeof(hkbStateChooser) == 0x10);

	class hkbStateChooserWrapper : public hkbCustomIdSelector
	{
	public:
		/* 0x010 */ hkbStateChooser* m_wrappedChooser;
	};
	static_assert(sizeof(hkbStateChooserWrapper) == 0x18);
	static_assert(offsetof(hkbStateChooserWrapper, m_wrappedChooser) == 0x10);

	class hkbStateListener : public hkReferencedObject
	{
	public:

	private:
		/* 04 */ virtual void sub_hkbStateListener_04();
		/* 05 */ virtual void sub_hkbStateListener_05();
		/* 06 */ virtual void sub_hkbStateListener_06();
		/* 07 */ virtual void sub_hkbStateListener_07();
	};
	static_assert(sizeof(hkbStateListener) == 0x10);

	class hkbStateMachineTransitionInfoReference
	{
	public:
		/* 0x000 */ std::int16_t m_fromStateIndex;
		/* 0x002 */ std::int16_t m_transitionIndex;
		/* 0x004 */ std::int16_t m_stateMachineId;
	};
	static_assert(sizeof(hkbStateMachineTransitionInfoReference) == 0x6);
	static_assert(offsetof(hkbStateMachineTransitionInfoReference, m_fromStateIndex) == 0x0);
	static_assert(offsetof(hkbStateMachineTransitionInfoReference, m_transitionIndex) == 0x2);
	static_assert(offsetof(hkbStateMachineTransitionInfoReference, m_stateMachineId) == 0x4);

	class hkbStateMachineActiveTransitionInfo
	{
	public:
		/* 0x000 */ void* m_transitionEffect;
		/* 0x008 */ hkbNodeInternalStateInfo* m_transitionEffectInternalStateInfo;
		/* 0x010 */ hkbStateMachineTransitionInfoReference m_transitionInfoReference;
		/* 0x016 */ hkbStateMachineTransitionInfoReference m_transitionInfoReferenceForTE;
		/* 0x01C */ std::int32_t m_fromStateId;
		/* 0x020 */ std::int32_t m_toStateId;
		/* 0x024 */ hkBool m_isReturnToPreviousState;
		/* 0x025 */ std::uint8_t pad025[0x3];
	};
	static_assert(sizeof(hkbStateMachineActiveTransitionInfo) == 0x28);
	static_assert(offsetof(hkbStateMachineActiveTransitionInfo, m_transitionEffect) == 0x0);
	static_assert(offsetof(hkbStateMachineActiveTransitionInfo, m_transitionEffectInternalStateInfo) == 0x8);
	static_assert(offsetof(hkbStateMachineActiveTransitionInfo, m_transitionInfoReference) == 0x10);
	static_assert(offsetof(hkbStateMachineActiveTransitionInfo, m_transitionInfoReferenceForTE) == 0x16);
	static_assert(offsetof(hkbStateMachineActiveTransitionInfo, m_fromStateId) == 0x1C);
	static_assert(offsetof(hkbStateMachineActiveTransitionInfo, m_toStateId) == 0x20);
	static_assert(offsetof(hkbStateMachineActiveTransitionInfo, m_isReturnToPreviousState) == 0x24);

	class hkbStateMachineProspectiveTransitionInfo
	{
	public:
		/* 0x000 */ hkbStateMachineTransitionInfoReference m_transitionInfoReference;
		/* 0x006 */ hkbStateMachineTransitionInfoReference m_transitionInfoReferenceForTE;
		/* 0x00C */ std::int32_t m_toStateId;
	};
	static_assert(sizeof(hkbStateMachineProspectiveTransitionInfo) == 0x10);
	static_assert(offsetof(hkbStateMachineProspectiveTransitionInfo, m_transitionInfoReference) == 0x0);
	static_assert(offsetof(hkbStateMachineProspectiveTransitionInfo, m_transitionInfoReferenceForTE) == 0x6);
	static_assert(offsetof(hkbStateMachineProspectiveTransitionInfo, m_toStateId) == 0xC);

	class hkbStateMachineDelayedTransitionInfo
	{
	public:
		/* 0x000 */ hkbStateMachineProspectiveTransitionInfo m_delayedTransition;
		/* 0x010 */ float m_timeDelayed;
		/* 0x014 */ hkBool m_isDelayedTransitionReturnToPreviousState;
		/* 0x015 */ hkBool m_wasInAbutRangeLastFrame;
		/* 0x016 */ std::uint8_t pad016[0x2];
	};
	static_assert(sizeof(hkbStateMachineDelayedTransitionInfo) == 0x18);
	static_assert(offsetof(hkbStateMachineDelayedTransitionInfo, m_delayedTransition) == 0x0);
	static_assert(offsetof(hkbStateMachineDelayedTransitionInfo, m_timeDelayed) == 0x10);
	static_assert(offsetof(hkbStateMachineDelayedTransitionInfo, m_isDelayedTransitionReturnToPreviousState) == 0x14);
	static_assert(offsetof(hkbStateMachineDelayedTransitionInfo, m_wasInAbutRangeLastFrame) == 0x15);

	class hkbStateMachineEventPropertyArray : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkbEventProperty> m_events;
	};
	static_assert(sizeof(hkbStateMachineEventPropertyArray) == 0x20);
	static_assert(offsetof(hkbStateMachineEventPropertyArray, m_events) == 0x10);

	class hkbStateMachineInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkbStateMachineActiveTransitionInfo> m_activeTransitions;
		/* 0x020 */ hkArray<std::uint8_t> m_transitionFlags;
		/* 0x030 */ hkArray<std::uint8_t> m_wildcardTransitionFlags;
		/* 0x040 */ hkArray<hkbStateMachineDelayedTransitionInfo> m_delayedTransitions;
		/* 0x050 */ float m_timeInState;
		/* 0x054 */ float m_lastLocalTime;
		/* 0x058 */ std::int32_t m_currentStateId;
		/* 0x05C */ std::int32_t m_previousStateId;
		/* 0x060 */ std::int32_t m_nextStartStateIndexOverride;
		/* 0x064 */ hkBool m_stateOrTransitionChanged;
		/* 0x065 */ hkBool m_echoNextUpdate;
		/* 0x066 */ hkBool m_hasEventlessWildcardTransitions;
		/* 0x067 */ std::uint8_t pad067[0x1];
	};
	static_assert(sizeof(hkbStateMachineInternalState) == 0x68);
	static_assert(offsetof(hkbStateMachineInternalState, m_activeTransitions) == 0x10);
	static_assert(offsetof(hkbStateMachineInternalState, m_transitionFlags) == 0x20);
	static_assert(offsetof(hkbStateMachineInternalState, m_wildcardTransitionFlags) == 0x30);
	static_assert(offsetof(hkbStateMachineInternalState, m_delayedTransitions) == 0x40);
	static_assert(offsetof(hkbStateMachineInternalState, m_timeInState) == 0x50);
	static_assert(offsetof(hkbStateMachineInternalState, m_lastLocalTime) == 0x54);
	static_assert(offsetof(hkbStateMachineInternalState, m_currentStateId) == 0x58);
	static_assert(offsetof(hkbStateMachineInternalState, m_previousStateId) == 0x5C);
	static_assert(offsetof(hkbStateMachineInternalState, m_nextStartStateIndexOverride) == 0x60);
	static_assert(offsetof(hkbStateMachineInternalState, m_stateOrTransitionChanged) == 0x64);
	static_assert(offsetof(hkbStateMachineInternalState, m_echoNextUpdate) == 0x65);
	static_assert(offsetof(hkbStateMachineInternalState, m_hasEventlessWildcardTransitions) == 0x66);

	class hkbStateMachineNestedStateMachineData
	{
	public:
		/* 0x000 */ void* m_nestedStateMachine;
		/* 0x008 */ void* m_eventIdMap;
	};
	static_assert(sizeof(hkbStateMachineNestedStateMachineData) == 0x10);
	static_assert(offsetof(hkbStateMachineNestedStateMachineData, m_nestedStateMachine) == 0x0);
	static_assert(offsetof(hkbStateMachineNestedStateMachineData, m_eventIdMap) == 0x8);

	class hkbStateMachineStateInfo : public hkbBindable
	{
	public:
		/* 0x030 */ hkArray<hkbStateListener*> m_listeners;
		/* 0x040 */ hkbStateMachineEventPropertyArray* m_enterNotifyEvents;
		/* 0x048 */ hkbStateMachineEventPropertyArray* m_exitNotifyEvents;
		/* 0x050 */ hkbStateMachineTransitionInfoArray* m_transitions;
		/* 0x058 */ hkbGenerator* m_generator;
		/* 0x060 */ hkStringPtr m_name;
		/* 0x068 */ std::int32_t m_stateId;
		/* 0x06C */ float m_probability;
		/* 0x070 */ hkBool m_enable;
		/* 0x071 */ hkBool m_hasEventlessTransitions;
		/* 0x072 */ std::uint8_t pad072[0x6];
	};
	static_assert(sizeof(hkbStateMachineStateInfo) == 0x78);
	static_assert(offsetof(hkbStateMachineStateInfo, m_listeners) == 0x30);
	static_assert(offsetof(hkbStateMachineStateInfo, m_enterNotifyEvents) == 0x40);
	static_assert(offsetof(hkbStateMachineStateInfo, m_exitNotifyEvents) == 0x48);
	static_assert(offsetof(hkbStateMachineStateInfo, m_transitions) == 0x50);
	static_assert(offsetof(hkbStateMachineStateInfo, m_generator) == 0x58);
	static_assert(offsetof(hkbStateMachineStateInfo, m_name) == 0x60);
	static_assert(offsetof(hkbStateMachineStateInfo, m_stateId) == 0x68);
	static_assert(offsetof(hkbStateMachineStateInfo, m_probability) == 0x6C);
	static_assert(offsetof(hkbStateMachineStateInfo, m_enable) == 0x70);
	static_assert(offsetof(hkbStateMachineStateInfo, m_hasEventlessTransitions) == 0x71);

	class hkbStateMachineTimeInterval
	{
	public:
		/* 0x000 */ std::int32_t m_enterEventId;
		/* 0x004 */ std::int32_t m_exitEventId;
		/* 0x008 */ float m_enterTime;
		/* 0x00C */ float m_exitTime;
	};
	static_assert(sizeof(hkbStateMachineTimeInterval) == 0x10);
	static_assert(offsetof(hkbStateMachineTimeInterval, m_enterEventId) == 0x0);
	static_assert(offsetof(hkbStateMachineTimeInterval, m_exitEventId) == 0x4);
	static_assert(offsetof(hkbStateMachineTimeInterval, m_enterTime) == 0x8);
	static_assert(offsetof(hkbStateMachineTimeInterval, m_exitTime) == 0xC);

	class hkbStateMachineTransitionInfo
	{
	public:
		enum class TransitionFlags : std::int32_t
		{
			FLAG_USE_TRIGGER_INTERVAL = 1,
			FLAG_USE_INITIATE_INTERVAL = 2,
			FLAG_UNINTERRUPTIBLE_WHILE_PLAYING = 4,
			FLAG_UNINTERRUPTIBLE_WHILE_DELAYED = 8,
			FLAG_DELAY_STATE_CHANGE = 16,
			FLAG_DISABLED = 32,
			FLAG_DISALLOW_RETURN_TO_PREVIOUS_STATE = 64,
			FLAG_DISALLOW_RANDOM_TRANSITION = 128,
			FLAG_DISABLE_CONDITION = 256,
			FLAG_ALLOW_SELF_TRANSITION_BY_TRANSITION_FROM_ANY_STATE = 512,
			FLAG_IS_GLOBAL_WILDCARD = 1024,
			FLAG_IS_LOCAL_WILDCARD = 2048,
			FLAG_FROM_NESTED_STATE_ID_IS_VALID = 4096,
			FLAG_TO_NESTED_STATE_ID_IS_VALID = 8192,
			FLAG_ABUT_AT_END_OF_FROM_GENERATOR = 16384,
		};

		enum class InternalFlagBits : std::int32_t
		{
			FLAG_INTERNAL_IN_TRIGGER_INTERVAL = 1,
			FLAG_INTERNAL_IN_INITIATE_INTERVAL = 2,
		};

		/* 0x000 */ hkbStateMachineTimeInterval m_triggerInterval;
		/* 0x010 */ hkbStateMachineTimeInterval m_initiateInterval;
		/* 0x020 */ hkbTransitionEffect* m_transition;
		/* 0x028 */ hkbCondition* m_condition;
		/* 0x030 */ std::int32_t m_eventId;
		/* 0x034 */ std::int32_t m_toStateId;
		/* 0x038 */ std::int32_t m_fromNestedStateId;
		/* 0x03C */ std::int32_t m_toNestedStateId;
		/* 0x040 */ std::int16_t m_priority;
		/* 0x042 */ std::int16_t m_flags;
		/* 0x044 */ std::uint8_t pad044[0x4];
	};
	static_assert(sizeof(hkbStateMachineTransitionInfo) == 0x48);
	static_assert(offsetof(hkbStateMachineTransitionInfo, m_triggerInterval) == 0x0);
	static_assert(offsetof(hkbStateMachineTransitionInfo, m_initiateInterval) == 0x10);
	static_assert(offsetof(hkbStateMachineTransitionInfo, m_transition) == 0x20);
	static_assert(offsetof(hkbStateMachineTransitionInfo, m_condition) == 0x28);
	static_assert(offsetof(hkbStateMachineTransitionInfo, m_eventId) == 0x30);
	static_assert(offsetof(hkbStateMachineTransitionInfo, m_toStateId) == 0x34);
	static_assert(offsetof(hkbStateMachineTransitionInfo, m_fromNestedStateId) == 0x38);
	static_assert(offsetof(hkbStateMachineTransitionInfo, m_toNestedStateId) == 0x3C);
	static_assert(offsetof(hkbStateMachineTransitionInfo, m_priority) == 0x40);
	static_assert(offsetof(hkbStateMachineTransitionInfo, m_flags) == 0x42);

	class hkbStateMachineTransitionInfoArray : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkbStateMachineTransitionInfo> m_transitions;
		/* 0x020 */ hkBool m_hasEventlessTransitions;
		/* 0x021 */ hkBool m_hasTimeBoundedTransitions;
		/* 0x022 */ std::uint8_t pad022[0x6];
	};
	static_assert(sizeof(hkbStateMachineTransitionInfoArray) == 0x28);
	static_assert(offsetof(hkbStateMachineTransitionInfoArray, m_transitions) == 0x10);
	static_assert(offsetof(hkbStateMachineTransitionInfoArray, m_hasEventlessTransitions) == 0x20);
	static_assert(offsetof(hkbStateMachineTransitionInfoArray, m_hasTimeBoundedTransitions) == 0x21);

	class hkbStringCondition : public hkbCondition
	{
	public:
		/* 0x010 */ hkStringPtr m_conditionString;
	};
	static_assert(sizeof(hkbStringCondition) == 0x18);
	static_assert(offsetof(hkbStringCondition, m_conditionString) == 0x10);

	class hkbStringEventPayload : public hkbEventPayload
	{
	public:
		/* 0x010 */ hkStringPtr m_data;
	};
	static_assert(sizeof(hkbStringEventPayload) == 0x18);
	static_assert(offsetof(hkbStringEventPayload, m_data) == 0x10);

	class hkbTestIdSelector : public hkbCustomIdSelector
	{
	public:
		/* 0x010 */ std::int32_t m_int;
		/* 0x014 */ float m_real;
		/* 0x018 */ hkStringPtr m_string;
	};
	static_assert(sizeof(hkbTestIdSelector) == 0x20);
	static_assert(offsetof(hkbTestIdSelector, m_int) == 0x10);
	static_assert(offsetof(hkbTestIdSelector, m_real) == 0x14);
	static_assert(offsetof(hkbTestIdSelector, m_string) == 0x18);

	class hkbTimerModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ float m_secondsElapsed;
		/* 0x014 */ std::uint8_t pad014[0x4];
	};
	static_assert(sizeof(hkbTimerModifierInternalState) == 0x18);
	static_assert(offsetof(hkbTimerModifierInternalState, m_secondsElapsed) == 0x10);

	class hkbTransformVectorModifierInternalState : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkVector4f m_vectorOut;
	};
	static_assert(sizeof(hkbTransformVectorModifierInternalState) == 0x20);
	static_assert(offsetof(hkbTransformVectorModifierInternalState, m_vectorOut) == 0x10);

	class hkbVariableBindingSet : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkbVariableBindingSetBinding> m_bindings;
		/* 0x020 */ std::int32_t m_indexOfBindingToEnable;
		/* 0x024 */ hkBool m_hasOutputBinding;
		/* 0x025 */ hkBool m_initializedOffsets;
		/* 0x026 */ std::uint8_t pad026[0x2];
	};
	static_assert(sizeof(hkbVariableBindingSet) == 0x28);
	static_assert(offsetof(hkbVariableBindingSet, m_bindings) == 0x10);
	static_assert(offsetof(hkbVariableBindingSet, m_indexOfBindingToEnable) == 0x20);
	static_assert(offsetof(hkbVariableBindingSet, m_hasOutputBinding) == 0x24);
	static_assert(offsetof(hkbVariableBindingSet, m_initializedOffsets) == 0x25);

	class hkbVariableBindingSetBinding
	{
	public:
		enum class BindingType : std::int32_t
		{
			BINDING_TYPE_VARIABLE = 0,
			BINDING_TYPE_CHARACTER_PROPERTY = 1,
		};

		enum class InternalBindingFlags : std::int32_t
		{
			FLAG_NONE = 0,
			FLAG_OUTPUT = 1,
		};

		/* 0x000 */ hkStringPtr m_memberPath;
		/* 0x008 */ void* m_memberClass;
		/* 0x010 */ std::int32_t m_offsetInObjectPlusOne;
		/* 0x014 */ std::int32_t m_offsetInArrayPlusOne;
		/* 0x018 */ std::int32_t m_rootVariableIndex;
		/* 0x01C */ std::int32_t m_variableIndex;
		/* 0x020 */ std::int8_t m_bitIndex;
		/* 0x021 */ std::int8_t m_bindingType;
		/* 0x022 */ std::uint8_t m_memberType;
		/* 0x023 */ std::int8_t m_variableType;
		/* 0x024 */ std::int8_t m_flags;
		/* 0x025 */ std::uint8_t pad025[0x3];
	};
	static_assert(sizeof(hkbVariableBindingSetBinding) == 0x28);
	static_assert(offsetof(hkbVariableBindingSetBinding, m_memberPath) == 0x0);
	static_assert(offsetof(hkbVariableBindingSetBinding, m_memberClass) == 0x8);
	static_assert(offsetof(hkbVariableBindingSetBinding, m_offsetInObjectPlusOne) == 0x10);
	static_assert(offsetof(hkbVariableBindingSetBinding, m_offsetInArrayPlusOne) == 0x14);
	static_assert(offsetof(hkbVariableBindingSetBinding, m_rootVariableIndex) == 0x18);
	static_assert(offsetof(hkbVariableBindingSetBinding, m_variableIndex) == 0x1C);
	static_assert(offsetof(hkbVariableBindingSetBinding, m_bitIndex) == 0x20);
	static_assert(offsetof(hkbVariableBindingSetBinding, m_bindingType) == 0x21);
	static_assert(offsetof(hkbVariableBindingSetBinding, m_memberType) == 0x22);
	static_assert(offsetof(hkbVariableBindingSetBinding, m_variableType) == 0x23);
	static_assert(offsetof(hkbVariableBindingSetBinding, m_flags) == 0x24);

	class hkbVariableBounds
	{
	public:
		/* 0x000 */ hkbVariableValue m_min;
		/* 0x004 */ hkbVariableValue m_max;
	};
	static_assert(sizeof(hkbVariableBounds) == 0x8);
	static_assert(offsetof(hkbVariableBounds, m_min) == 0x0);
	static_assert(offsetof(hkbVariableBounds, m_max) == 0x4);

	class hkbVariableInfo
	{
	public:
		enum class VariableType : std::int32_t
		{
			VARIABLE_TYPE_INVALID = -1,
			VARIABLE_TYPE_BOOL = 0,
			VARIABLE_TYPE_INT8 = 1,
			VARIABLE_TYPE_INT16 = 2,
			VARIABLE_TYPE_INT32 = 3,
			VARIABLE_TYPE_REAL = 4,
			VARIABLE_TYPE_POINTER = 5,
			VARIABLE_TYPE_VECTOR3 = 6,
			VARIABLE_TYPE_VECTOR4 = 7,
			VARIABLE_TYPE_QUATERNION = 8,
		};

		/* 0x000 */ hkbRoleAttribute m_role;
		/* 0x004 */ std::int8_t m_type;
		/* 0x005 */ std::uint8_t pad005[0x1];
	};
	static_assert(sizeof(hkbVariableInfo) == 0x6);
	static_assert(offsetof(hkbVariableInfo, m_role) == 0x0);
	static_assert(offsetof(hkbVariableInfo, m_type) == 0x4);

	class hkbVariableValueSet : public hkReferencedObject
	{
	public:
		/* 0x010 */ hkArray<hkbVariableValue> m_wordVariableValues;
		/* 0x020 */ hkArray<hkVector4f> m_quadVariableValues;
		/* 0x030 */ hkArray<hkReferencedObject*> m_variantVariableValues;
	};
	static_assert(sizeof(hkbVariableValueSet) == 0x40);
	static_assert(offsetof(hkbVariableValueSet, m_wordVariableValues) == 0x10);
	static_assert(offsetof(hkbVariableValueSet, m_quadVariableValues) == 0x20);
	static_assert(offsetof(hkbVariableValueSet, m_variantVariableValues) == 0x30);

	class hkbWorldFromModelModeData
	{
	public:
		enum class WorldFromModelMode : std::int32_t
		{
			WORLD_FROM_MODEL_MODE_USE_OLD = 0,
			WORLD_FROM_MODEL_MODE_USE_INPUT = 1,
			WORLD_FROM_MODEL_MODE_COMPUTE = 2,
			WORLD_FROM_MODEL_MODE_NONE = 3,
			WORLD_FROM_MODEL_MODE_RAGDOLL = 4,
		};

		/* 0x000 */ std::int16_t m_poseMatchingBone0;
		/* 0x002 */ std::int16_t m_poseMatchingBone1;
		/* 0x004 */ std::int16_t m_poseMatchingBone2;
		/* 0x006 */ std::int8_t m_mode;
		/* 0x007 */ std::uint8_t pad007[0x1];
	};
	static_assert(sizeof(hkbWorldFromModelModeData) == 0x8);
	static_assert(offsetof(hkbWorldFromModelModeData, m_poseMatchingBone0) == 0x0);
	static_assert(offsetof(hkbWorldFromModelModeData, m_poseMatchingBone1) == 0x2);
	static_assert(offsetof(hkbWorldFromModelModeData, m_poseMatchingBone2) == 0x4);
	static_assert(offsetof(hkbWorldFromModelModeData, m_mode) == 0x6);

	class hkbnpPhysicsInterface : public hkbPhysicsInterface
	{
	public:
		/* 0x018 */ void* m_world;
		/* 0x020 */ void* m_taskQueue;
		/* 0x028 */ void* m_threadPool;
		/* 0x030 */ void* m_stepLocalAllocator;
		/* 0x038 */ hkMultiThreadCheck m_multiThreadCheck;
		/* 0x044 */ std::uint8_t pad044[0x4];
		/* 0x048 */ void* m_criticalSection;
		/* 0x050 */ hkArray<void> m_newRbHandles;
		/* 0x060 */ hkArray<void> m_newConstraintHandles;
		/* 0x070 */ hkArray<void> m_referencedRbHandles;
		/* 0x080 */ hkArray<void> m_referencedConstraintHandles;
		/* 0x090 */ hkArray<void*> m_referencedBodiesLocalFrames;

	private:
		/* 1E */ virtual void sub_hkbnpPhysicsInterface_1E();
		/* 1F */ virtual void sub_hkbnpPhysicsInterface_1F();
		/* 20 */ virtual void sub_hkbnpPhysicsInterface_20();
	};
	static_assert(sizeof(hkbnpPhysicsInterface) == 0xA0);
	static_assert(offsetof(hkbnpPhysicsInterface, m_world) == 0x18);
	static_assert(offsetof(hkbnpPhysicsInterface, m_taskQueue) == 0x20);
	static_assert(offsetof(hkbnpPhysicsInterface, m_threadPool) == 0x28);
	static_assert(offsetof(hkbnpPhysicsInterface, m_stepLocalAllocator) == 0x30);
	static_assert(offsetof(hkbnpPhysicsInterface, m_multiThreadCheck) == 0x38);
	static_assert(offsetof(hkbnpPhysicsInterface, m_criticalSection) == 0x48);
	static_assert(offsetof(hkbnpPhysicsInterface, m_newRbHandles) == 0x50);
	static_assert(offsetof(hkbnpPhysicsInterface, m_newConstraintHandles) == 0x60);
	static_assert(offsetof(hkbnpPhysicsInterface, m_referencedRbHandles) == 0x70);
	static_assert(offsetof(hkbnpPhysicsInterface, m_referencedConstraintHandles) == 0x80);
	static_assert(offsetof(hkbnpPhysicsInterface, m_referencedBodiesLocalFrames) == 0x90);

	class hkbnpRagdollInterface : public hkbRagdollInterface
	{
	public:
		/* 0x010 */ void* m_physicsInterface;
		/* 0x018 */ void* m_ragdoll;
		/* 0x020 */ hkArray<void*> m_scaledObjects;
		/* 0x030 */ hkArray<void*> m_rigidBodyHandles;
		/* 0x040 */ std::uint8_t pad040[0x18];
		/* 0x058 */ hkBool m_addedToWorld;
		/* 0x059 */ std::uint8_t pad059[0x7];
	};
	static_assert(sizeof(hkbnpRagdollInterface) == 0x60);
	static_assert(offsetof(hkbnpRagdollInterface, m_physicsInterface) == 0x10);
	static_assert(offsetof(hkbnpRagdollInterface, m_ragdoll) == 0x18);
	static_assert(offsetof(hkbnpRagdollInterface, m_scaledObjects) == 0x20);
	static_assert(offsetof(hkbnpRagdollInterface, m_rigidBodyHandles) == 0x30);
	static_assert(offsetof(hkbnpRagdollInterface, m_addedToWorld) == 0x58);

}
