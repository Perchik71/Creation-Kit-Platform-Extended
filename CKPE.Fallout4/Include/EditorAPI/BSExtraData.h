// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Asserts.h>
#include <CKPE.Common.RTTI.h>
#include "NiAPI/NiFlags.h"
#include "NiAPI/NiPointer.h"
#include "NiAPI/NiTypes.h"
#include "NiAPI/NiAVObject.h"
#include "NiAPI/NiPointLight.h"
#include "BSLight.h"
#include "BSLines.h"
#include "BSTArray.h"
#include "ObjectRefHandle.h"
#include "BSLock.h"
#include "BSTSmartPointer.h"
#include "BSEntryString.h"
#include <algorithm>
#include <span>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace Forms
			{
				class TESForm;
				class BGSLayer;
				class BGSKeyword;
				class BGSMaterialSwap;
			}

			class BGSPrimitive;

			class BSExtraData
			{
			public:
				enum : std::uint8_t
				{
					kNone,						// BSExtraData
					kHavok,
					kCell3D,
					kWaterType,					// ExtraCellWaterType
					kRegionList,
					kSeenData,
					kEditorID,
					kCellMusicType,
					kSkyRegion,
					kCellMidLowProcess,
					kCellDetachTime,
					kPersistentCell,
					kKeywords,
					kAction,
					kStartingPosition,
					kAnim,
					kHavokAnim,
					kBiped,
					kUsedMarkers,
					kDistantData,
					kRagdollData,
					kCellPrevisRefs,
					kInitActions,
					kEssentialProtected,
					kPackagesStartLoc,
					kPackage,
					kTrespassPackage,
					kRunOncePackages,
					kReferenceHandle,			// ExtraReferenceHandles
					kFollower,
					kLevCreaMod,
					kGhost,
					kOriginalReference,
					kOwnership,
					kGlobal,
					kRank,
					kCount,
					kHealth,
					kRangeDistOverride,
					kTimeLeft,
					kCharge,
					kLight,
					kLock,
					kTeleport,
					kMapMarker,
					kLeveledCreature,
					kLevelItem,
					kScale,
					kSeed,
					kMagicCaster,
					kMagicTarget,
					kMasterFileCell,
					kPlayerCrimeList,
					kObjectInstance,			// BGSObjectInstanceExtra
					kEnableStateParent,
					kEnableStateChildren,
					kItemDropper,
					kDroppedItemList,
					kTeleportMarker,
					kInactivePowerArmor,
					kSavedHavokData,
					kCannotWear,
					kPoison,
					kMagicLight,
					kLastFinishedSequence,
					kSavedAnimation,
					kNorthRotation,
					kSpawnCount,
					kFriendHits,
					kHeadTrackTarget,
					kBoundArmor,
					kRefractionProperty,
					kStartingWorldOrCell,
					kFavorite,
					kEditorRef3DData,
					kEditorRefMoveData,
					kInfoGeneralTopic,
					kNoRumors,
					kSound,
					kTerminalState,
					kLinkedRef,
					kLinkedRefChildren,
					kActivateRef,
					kActivateRefChildren,
					kTalkToPlayer,
					kObjectHealth,
					kImageSpace,
					kNavmeshPortal,
					kModelSwap,
					kRadius,
					kRadiation,
					kFactionChanges,
					kDismemberedLimbs,
					kActorCause,
					kMultibound,
					kMultiboundData,
					kMultiboundRef,
					kReflectedRefs,
					kReflectorRefs,
					kEmittanceSource,
					kRadioData,
					kCombatStyle,
					kLinkColorPair,
					kPrimitive,
					kOpenCloseActivateRef,
					kAnimNoteReceiver,
					kAmmo,
					kPatrolRefData,
					kPackageData,
					kOcclusionShape,
					kCollisionData,
					kSayOnceADayTopicInfo,
					kEncounterZone,
					kSayToTopicInfo,
					kOcclusionPlaneRefData,
					kPortalRefData,
					kPortal,
					kRoom,
					kGodRays,
					kRoomRefData,
					kGuardedRefData,
					kCreatureAwakeSound,
					kWaterZoneMap,
					kHorse,
					kIgnoredBySandbox,
					kCellAcousticSpace,
					kReservedMarkers,
					kCellTransCount,
					kWaterLightRefs,
					kLitWaterRefs,
					kRadioRepeater,
					kActivateLoopSound,
					kPatrolRefInUseData,
					kAshpileRef,
					kCreatureMovementSound,
					kFollowerSwimBreadcrumbs,
					kAliasInstanceArray,		// ExtraAliasInstanceArray
					kLocation,					// ExtraLocation
					kMasterLocation,
					kLocationRefType,
					kPromotedRef,
					kAnimSequencer,
					kOutfitItem,
					kEditorLocation,
					kLeveledItemBase,
					kLightData,
					kScene,
					kBadPosition,
					kHeadTrackingWeight,
					kFromAlias,
					kShouldWear,
					kFavorCost,
					kAttachArrows3D,
					kTextDisplayData,			// ExtraTextDisplayData
					kAlphaCutoff,
					kEnchantment,
					kSoul,
					kForcedTarget,
					kSoundOutput,
					kUniqueID,					// ExtraUniqueID
					kFlags,
					kRefrPath,
					kDecalGroup,
					kLockList,
					kForcedLandingMarker,
					kLargeRefOwnerCells,
					kCellWaterEnvMap,
					kCellGrassData,
					kTeleportName,
					kActorInteraction,
					kWaterData,
					kWaterCurrentZoneData,
					kAttachRef,
					kAttachRefChildren,
					kGroupConstraint,
					kScriptedAnimDependence,
					kCachedScale,
					kRaceData,
					kGIDBuffer,
					kMissingRefIDs,
					kBendableSplineParams,
					kReferenceGroup,
					kReferenceCharacterController,	
					kMovementController,
					kLayer,
					kMaterialSwap,				// ExtraMaterialSwap
					kInstanceData,				// ExtraInstanceData
					kPowerArmor,
					kAcousticParent,
					kInputEnableLayer,
					kProjectedDecalRef,
					kWorkshop,					// Workshop::ExtraData
					kRadioReceiver,
					kCulledBone,
					kActorValueStorage,
					kDirectAtTarget,
					kActivateNext,
					kCellCombinedRefs,
					kObjectBreakable,
					kSavedDynamicIdles,
					kIgnoredAttractKeywords,
					kModRank,
					kInteriorLODWorldspace,
					kActorBoneScaleMap,
					kActorFXPickNodes,
					kPowerArmorPreload,
					kAnimGraphPreload,
					kAnimSounds,
					kPowerLinks,				// ExtraPowerLinks
					kSavedUnrecoverableSubgraphs,
					kRefWeaponSounds,
					kRefInvestedGold,
					kRefFurnitureEntryData,
					kVoiceType,
					kTotal
				};

				static constexpr auto TYPE{ kNone };

				BSExtraData() : BSExtraData(TYPE) {}
				BSExtraData(std::uint8_t type) : _type(type) {}
				virtual ~BSExtraData() = default;

				virtual bool CompareImpl([[maybe_unused]] const BSExtraData& rhs) const noexcept(true) { return false; }
				virtual bool CompareForUI(const BSExtraData* rhs) const noexcept(true)
				{
					if (rhs && (_type == rhs->_type))
						return CompareImpl(*rhs);
					return true;
				}

				[[nodiscard]] inline bool HasFlag(std::uint16_t f) const noexcept(true) { return _flags.Has(f); }
				inline void SetFlag(std::uint16_t f) noexcept(true) { _flags.Set(f); }
				inline void UnsetFlag(std::uint16_t f) noexcept(true) { _flags.Unset(f); }

				inline void SetNext(BSExtraData* extra) noexcept(true) { _next = extra; }
				[[nodiscard]] inline BSExtraData* GetNext() const noexcept(true) { return _next; }
				[[nodiscard]] inline BSExtraData** GetNextAddressOf() noexcept(true) { return std::addressof(_next); }
				[[nodiscard]] inline std::uint8_t GetType() const noexcept(true) { return _type; }
			private:
				BSExtraData* _next{ nullptr };
				NiAPI::NiTFlags<std::uint16_t, BSExtraData> _flags;
				std::uint8_t _type{ kNone };
			};
			static_assert(sizeof(BSExtraData) == 0x18);

			class ExtraCellWaterType : public BSExtraData
			{
				Forms::TESForm* _Water{ nullptr };  // TESWaterForm
			public:
				static constexpr auto TYPE{ kWaterType };

				ExtraCellWaterType() : BSExtraData(TYPE) {}
				ExtraCellWaterType(Forms::TESForm* Water) : BSExtraData(TYPE), _Water(Water) {}
				virtual ~ExtraCellWaterType() = default;

				[[nodiscard]] inline Forms::TESForm* GetWaterForm() const noexcept(true) { return _Water; }
			};
			static_assert(sizeof(ExtraCellWaterType) == 0x20);

			class ExtraStartingPosition : public BSExtraData
			{
				NiAPI::NiPoint3A _position;
				NiAPI::NiPoint3A _rotate;
			public:
				static constexpr auto TYPE{ kStartingPosition };

				ExtraStartingPosition() : BSExtraData(TYPE) {}
				ExtraStartingPosition(const NiAPI::NiPoint3A& Position, const NiAPI::NiPoint3A& Rotate) :
					BSExtraData(TYPE), _position(Position), _rotate(Rotate) {}
				virtual ~ExtraStartingPosition() = default;

				[[nodiscard]] inline NiAPI::NiPoint3A GetPosition() const noexcept(true) { return _position; }
				[[nodiscard]] inline NiAPI::NiPoint3A GetRotate() const noexcept(true) { return _rotate; }
				[[nodiscard]] inline NiAPI::NiPoint3A& GetPosition() noexcept(true) { return _position; }
				[[nodiscard]] inline NiAPI::NiPoint3A& GetRotate() noexcept(true) { return _rotate; }
			};
			static_assert(sizeof(ExtraStartingPosition) == 0x38);

			class ExtraEditorRef3DData : public BSExtraData
			{
				NiAPI::NiPointer<NiAPI::NiAVObject> _3D;	// BSFadeNode by RTTI
				char unk[0x10];
			public:
				static constexpr auto TYPE{ kEditorRef3DData };

				ExtraEditorRef3DData(const NiAPI::NiPointer<NiAPI::NiAVObject>& Model3D) : BSExtraData(TYPE), _3D(Model3D) {}
				virtual ~ExtraEditorRef3DData() = default;

				[[nodiscard]] inline NiAPI::NiPointer<NiAPI::NiAVObject> Get3D() const noexcept(true) { return _3D; }
			};
			static_assert(sizeof(ExtraEditorRef3DData) == 0x30);

			class ExtraEditorRefMoveData : public BSExtraData
			{
				NiAPI::NiPoint3 _Angle;
				NiAPI::NiPoint3 _Position;
				NiAPI::NiPoint3 _OldPosition;
			public:
				static constexpr auto TYPE{ kEditorRefMoveData };

				ExtraEditorRefMoveData() : BSExtraData(TYPE) {}
				ExtraEditorRefMoveData(NiAPI::NiPoint3 Angle, NiAPI::NiPoint3 OldPosition, NiAPI::NiPoint3 Position) :
					BSExtraData(TYPE), _Angle(Angle), _OldPosition(OldPosition), _Position(Position) {}
				virtual ~ExtraEditorRefMoveData() = default;

				[[nodiscard]] inline NiAPI::NiPoint3 GetAngle() const noexcept(true) { return _Angle; }
				[[nodiscard]] inline NiAPI::NiPoint3 GetOldPosition() const noexcept(true) { return _OldPosition; }
				[[nodiscard]] inline NiAPI::NiPoint3 GetPosition() const noexcept(true) { return _Position; }
				[[nodiscard]] inline NiAPI::NiPoint3& GetAngle() noexcept(true) { return _Angle; }
				[[nodiscard]] inline NiAPI::NiPoint3& GetOldPosition() noexcept(true) { return _OldPosition; }
				[[nodiscard]] inline NiAPI::NiPoint3& GetPosition() noexcept(true) { return _Position; }
			};
			static_assert(sizeof(ExtraEditorRefMoveData) == 0x40);

			class ExtraLight : public BSExtraData
			{
				NiAPI::NiPointer<NiAPI::NiPointLight> _PointLight;
				NiAPI::NiPointer<BSLight> _Light;
				float _WantDimmer{ 0.0f };
			public:
				static constexpr auto TYPE{ kLight };

				ExtraLight() : BSExtraData(TYPE) {}
				ExtraLight(const NiAPI::NiPointer<NiAPI::NiPointLight>& PointLight) : BSExtraData(TYPE), _PointLight(PointLight) {}
				virtual ~ExtraLight() = default;

				[[nodiscard]] inline NiAPI::NiPointer<NiAPI::NiPointLight> GetPointLight() const noexcept(true) { return _PointLight; }
				[[nodiscard]] inline NiAPI::NiPointer<BSLight> GetLightData() const noexcept(true) { return _Light; }
				[[nodiscard]] inline float GetWantDimmer() const noexcept(true) { return _WantDimmer; }
			};
			static_assert(sizeof(ExtraLight) == 0x30);
			
			class ExtraLightData : public BSExtraData
			{
				float _fov{ 0.0f };
				float _fade{ 0.0f };
				float _endDistanceCap{ 0.0f };
				float _shadowDepthBias{ 0.0f };
				std::uint64_t _unk{ 0 };
			public:
				static constexpr auto TYPE{ kLightData };

				ExtraLightData() : BSExtraData(TYPE) {}
				ExtraLightData(float Fov, float Fade, float EndDistanceCap, float ShadowDepthBias) : 
					BSExtraData(TYPE), _fov(Fov), _fade(Fade), _endDistanceCap(EndDistanceCap), _shadowDepthBias(ShadowDepthBias) {}
				virtual ~ExtraLightData() = default;

				[[nodiscard]] inline float GetFov() const noexcept(true) { return _fov; }
				inline void SetFov(float Fov) noexcept(true) { _fov = Fov; }
				[[nodiscard]] inline float GetFade() const noexcept(true) { return _fade; }
				inline void SetFade(float Fade) noexcept(true) { _fade = Fade; }
				[[nodiscard]] inline float GetEndDistanceCap() const noexcept(true) { return _endDistanceCap; }
				inline void SetEndDistanceCap(float EndDistanceCap) noexcept(true) { _endDistanceCap = EndDistanceCap; }
				[[nodiscard]] inline float GetShadowDepthBias() const noexcept(true) { return _shadowDepthBias; }
				inline void SetShadowDepthBias(float ShadowDepthBias) noexcept(true) { _shadowDepthBias = ShadowDepthBias; }

				CKPE_PROPERTY(GetFov, SetFov) float Fov;
				CKPE_PROPERTY(GetFade, SetFade) float Fade;
				CKPE_PROPERTY(GetEndDistanceCap, SetEndDistanceCap) float EndDistanceCap;
				CKPE_PROPERTY(GetShadowDepthBias, SetShadowDepthBias) float ShadowDepthBias;
			};
			static_assert(sizeof(ExtraLightData) == 0x30);

			class ExtraLayer : public BSExtraData
			{
				Forms::BGSLayer* _Layer{ nullptr };
			public:
				static constexpr auto TYPE{ kLayer };	// B8

				ExtraLayer() : BSExtraData(TYPE) {}
				ExtraLayer(Forms::BGSLayer* Layer) : BSExtraData(TYPE), _Layer(Layer) {}
				virtual ~ExtraLayer() = default;

				[[nodiscard]] inline Forms::BGSLayer* Get() const noexcept(true) { return _Layer; }
			};
			static_assert(sizeof(ExtraLayer) == 0x20);

			class ExtraLinkedRef : public BSExtraData
			{
				char _unk00[8];
				Forms::BGSKeyword* _keyword{ nullptr };
				Forms::TESForm* _refr{ nullptr };
				char _unk18[16];
			public:
				static constexpr auto TYPE{ kLinkedRef };

				ExtraLinkedRef() : BSExtraData(TYPE) {}
				ExtraLinkedRef(Forms::BGSKeyword* Keyword, Forms::TESForm* Refr) :
					BSExtraData(TYPE), _keyword(Keyword), _refr(Refr) {}
				virtual ~ExtraLinkedRef() = default;

				[[nodiscard]] Forms::BGSKeyword* GetKeyword() const noexcept(true) { return _keyword; }
				[[nodiscard]] Forms::TESForm* GetRefr() const noexcept(true) { return _refr; }
			};
			static_assert(sizeof(ExtraLinkedRef) == 0x40);

			class ExtraLinkedRefChildren : public BSExtraData
			{
			public:
				struct Children
				{
					Forms::BGSKeyword* Keyword;		// 00
					ObjectRefHandle Refr;			// 08
				};
			private:
				char _unk00[8];
				Children* _Childrens{ nullptr };
				ObjectRefHandle _BaseRefr;
				char _pad[4];
				std::uint32_t _Count{ 0 };
			public:
				static constexpr auto TYPE{ kLinkedRefChildren };

				ExtraLinkedRefChildren() : BSExtraData(TYPE) {}
				ExtraLinkedRefChildren(Children* Childrens, std::uint32_t ChildrenCount, const ObjectRefHandle& ObjRefr) :
					BSExtraData(TYPE), _Childrens(Childrens), _BaseRefr(ObjRefr), _Count(ChildrenCount) {}
				virtual ~ExtraLinkedRefChildren() = default;

				[[nodiscard]] Children* GetChildrens() const noexcept(true) { return _Childrens; }
				[[nodiscard]] std::uint32_t GetCount() const noexcept(true) { return _Count; }
				[[nodiscard]] ObjectRefHandle GetBaseObjectRefr() const noexcept(true) { return _BaseRefr; }
			};
			static_assert(sizeof(ExtraLinkedRefChildren) == 0x38);

			class ExtraRadius : public BSExtraData
			{
				float _radius{ 0.0f };
			public:
				static constexpr auto TYPE{ kRadius };

				ExtraRadius() : BSExtraData(TYPE) {}
				ExtraRadius(float Radius) : BSExtraData(TYPE), _radius(Radius) {}
				virtual ~ExtraRadius() = default;

				[[nodiscard]] inline float Get() const noexcept(true) { return _radius; }
				inline void Set(float Radius) noexcept(true) { _radius = Radius; }

				CKPE_PROPERTY(Get, Set) float Value;
			};
			static_assert(sizeof(ExtraRadius) == 0x20);

			class ExtraEnableStateParent : public BSExtraData
			{
				NiAPI::NiTFlags<std::int8_t, ExtraEnableStateParent> _flags;
				std::uint8_t pad11{ 0 };
				std::uint16_t pad12{ 0 };
				ObjectRefHandle _parent;
			public:
				static constexpr auto TYPE{ kEnableStateParent };

				ExtraEnableStateParent() : BSExtraData(TYPE) {}
				ExtraEnableStateParent(std::int8_t Flags, const ObjectRefHandle& ObjRef) : 
					BSExtraData(TYPE), _parent(ObjRef) { _flags.Flags = Flags; }
				virtual ~ExtraEnableStateParent() = default;

				[[nodiscard]] inline bool HasFlag(std::uint8_t f) const noexcept(true) { return _flags.Has(f); }
				inline void SetFlag(std::uint8_t f) noexcept(true) { _flags.Set(f); }
				inline void UnsetFlag(std::uint8_t f) noexcept(true) { _flags.Unset(f); }
				[[nodiscard]] inline ObjectRefHandle GetParentObjectRef() const noexcept(true) { return _parent; }
			};
			static_assert(sizeof(ExtraEnableStateParent) == 0x20);

			class ExtraPrimitive : public BSExtraData
			{
				BGSPrimitive* _Primitive{ nullptr };
			public:
				static constexpr auto TYPE{ kPrimitive };

				ExtraPrimitive(BGSPrimitive* Primitive) : BSExtraData(TYPE), _Primitive(Primitive) {}
				virtual ~ExtraPrimitive() = default;

				[[nodiscard]] BGSPrimitive* Get() const noexcept(true) { return _Primitive; }
			};
			static_assert(sizeof(ExtraPrimitive) == 0x20);

			class ExtraReferenceHandles : public BSExtraData
			{
				ObjectRefHandle _OriginalRef;
				ObjectRefHandle _ContainerRef;
			public:
				static constexpr auto TYPE{ kReferenceHandle };

				ExtraReferenceHandles() : BSExtraData(TYPE) {}
				ExtraReferenceHandles(const ObjectRefHandle& OriginalRef, const ObjectRefHandle& ContainerRef) :
					BSExtraData(TYPE), _OriginalRef(OriginalRef), _ContainerRef(ContainerRef) {}
				virtual ~ExtraReferenceHandles() = default;

				[[nodiscard]] ObjectRefHandle GetOriginalObjectRef() const noexcept(true) { return _OriginalRef; }
				[[nodiscard]] ObjectRefHandle GetContainerObjectRef() const noexcept(true) { return _ContainerRef; }
			};
			static_assert(sizeof(ExtraReferenceHandles) == 0x20);

			class ExtraMaterialSwap : public BSExtraData
			{
				Forms::BGSMaterialSwap* _Swap{ nullptr };
			public:
				static constexpr auto TYPE{ kMaterialSwap };

				ExtraMaterialSwap(Forms::BGSMaterialSwap* MaterialSwap) :
					BSExtraData(TYPE), _Swap(MaterialSwap) {}
				virtual ~ExtraMaterialSwap() = default;

				[[nodiscard]] Forms::BGSMaterialSwap* GetForm() const noexcept(true) { return _Swap; }
			};
			static_assert(sizeof(ExtraMaterialSwap) == 0x20);

			class ExtraUniqueID : public BSExtraData
			{
				std::uint16_t _uniqueID{ 0 };
				std::uint32_t _baseID{ 0 };
			public:
				static constexpr auto TYPE{ kUniqueID };

				ExtraUniqueID() : BSExtraData(TYPE) {}
				ExtraUniqueID(std::uint16_t UniqueID, std::uint32_t BaseID) :
					BSExtraData(TYPE), _uniqueID(UniqueID), _baseID(BaseID) {}
				virtual ~ExtraUniqueID() = default;

				[[nodiscard]] std::uint16_t GetUniqueID() const noexcept(true) { return _uniqueID; }
				[[nodiscard]] std::uint32_t GetBaseID() const noexcept(true) { return _baseID; }
			};
			static_assert(sizeof(ExtraUniqueID) == 0x20);

			class ExtraPowerLinks : public BSExtraData
			{
			public:
				static constexpr auto TYPE{ kPowerLinks };

				struct Element
				{
					std::uint32_t FormID;    // 0
					std::uint32_t LinkType;  // 4
				};

				BSTArray<Element> PowerLinks;

				ExtraPowerLinks() : BSExtraData(TYPE) {}
				virtual ~ExtraPowerLinks() = default;
			};
			static_assert(sizeof(ExtraPowerLinks::Element) == 0x8);
			static_assert(sizeof(ExtraPowerLinks) == 0x30);

			class BaseExtraList
			{
				BSExtraData* _head{ nullptr };
				BSExtraData** _tail{ std::addressof(_head) };
				std::uint8_t* _flags{ nullptr };				// looks like always nullptr for ck
			public:
				void AddExtra(BSExtraData* Extra) noexcept(true)
				{
					CKPE_ASSERT(Extra != nullptr);
					CKPE_ASSERT(Extra->GetNext() == nullptr);

					const auto type = Extra->GetType();
					CKPE_ASSERT(!HasType(type));

					if (!_head || !IsHighUseExtra(type))
					{
						CKPE_ASSERT(_tail != nullptr);
						*_tail = Extra;
						_tail = Extra->GetNextAddressOf();
					}
					else 
					{
						Extra->SetNext(_head);
						_head = Extra;
					}

					// remove for ck
					// MarkType(type, true);
				}

				[[nodiscard]] BSExtraData* GetByType(std::uint8_t Type) const noexcept(true)
				{
					CKPE_ASSERT(Type < BSExtraData::kTotal);

					for (auto iter = _head; iter; iter = iter->GetNext())
						if (iter->GetType() == Type)
							return iter;

					return nullptr;
				}

				[[nodiscard]] bool HasType(std::uint8_t Type) const noexcept(true)
				{
					// No work for ck
					/*const auto idx = Type / 8;
					const auto flags = GetFlags();
					
					if (!flags.empty() && idx < flags.size()) 
					{
						const auto pos = static_cast<std::uint8_t>(1u << (Type % 8));
						return (flags[idx] & pos) != 0;
					}*/
					
					return GetByType(Type) ? true : false;
				}

				std::unique_ptr<BSExtraData> RemoveExtra(std::uint8_t Type) noexcept(true)
				{
					BSExtraData* prev = nullptr;
					for (auto iter = _head; iter; prev = iter, iter = iter->GetNext()) 
					{
						if (iter->GetType() == Type) 
						{
							if (prev)
								prev->SetNext(iter->GetNext());
							else
								_head = iter->GetNext();

							if (!_tail || *_tail == iter)
								_tail = prev ? prev->GetNextAddressOf() : std::addressof(_head);

							// remove for ck
							// MarkType(Type, false);
							return std::unique_ptr<BSExtraData>{ iter };
						}
					}

					return nullptr;
				}

				void Dump(void(*Callback)(const char*, ...)) const noexcept(true)
				{
					for (auto iter = _head; iter; iter = iter->GetNext())
					{
						auto rtti = Common::RTTI::GetSingleton()->Find(*((std::uintptr_t*)iter));
						if (rtti)
							Callback("\tExtra: type %X \"%s\" %p", (std::uint32_t)iter->GetType(), rtti->Name, iter);
						else
							Callback("\tExtra: type %X \"Unknow\" %p", (std::uint32_t)iter->GetType(), iter);
					}
				}
			private:
				//static constexpr std::size_t N = (BSExtraData::kTotal / 8) + 1;

				[[nodiscard]] static bool IsHighUseExtra(std::uint8_t Type) noexcept(true)
				{
					return !((Type - 11) & ~0x22u) && (Type != BSExtraData::kLeveledCreature);
				}

				/*void CreateFlags() noexcept(true)
				{ 
					_flags = (std::uint8_t*)CKPE::Common::MemoryManager::GetSingleton()->MemAlloc(N, 8, true);
				}

				[[nodiscard]] std::span<std::uint8_t> GetFlags() const noexcept(true)
				{
					if (_flags)
						return { _flags, N };
					return {};
				}

				[[nodiscard]] std::span<std::uint8_t, N> GetOrCreateFlags() noexcept(true)
				{
					if (!_flags)
						CreateFlags();

					return std::span{ reinterpret_cast<std::uint8_t(&)[N]>(*_flags) };
				}

				void MarkType(std::uint8_t Type, bool Set)
				{
					CKPE_ASSERT(Type < BSExtraData::kTotal);

					const auto idx = Type / 8;
					const auto pos = static_cast<std::uint8_t>(1u << (Type % 8));
					const auto flags = GetOrCreateFlags();
					
					if (Set) flags[idx] |= pos;
					else flags[idx] &= ~pos;
				}*/

				[[nodiscard]] BSExtraData* GetHead() const noexcept(true) { return _head; }
				[[nodiscard]] BSExtraData** GetTail() const noexcept(true) { return _tail; }
			};
			static_assert(sizeof(BaseExtraList) == 0x18);

			namespace Detail
			{
				template<class T>
				concept ExtraDataListConstraint =
					std::derived_from<T, BSExtraData> && !std::is_pointer_v<T> && !std::is_reference_v<T>;
			}

			class ExtraDataList : public BSIntrusiveRefCounted
			{
			public:
				inline void AddExtra(BSExtraData* a_extra)
				{
					const BSAutoWriteLock l{ extraRWLock };
					extraData.AddExtra(a_extra);
				}

				[[nodiscard]] inline BSExtraData* GetByType(std::uint8_t Type) const noexcept(true)
				{
					const BSAutoReadLock l{ extraRWLock };
					return extraData.GetByType(Type);
				}

				template <Detail::ExtraDataListConstraint T>
				[[nodiscard]] inline T* GetByType() const noexcept(true)
				{
					return static_cast<T*>(GetByType(T::TYPE));
				}

				[[nodiscard]] inline bool HasType(std::uint8_t Type) const noexcept(true)
				{
					const BSAutoReadLock l{ extraRWLock };
					return extraData.HasType(Type);
				}

				template <Detail::ExtraDataListConstraint T>
				[[nodiscard]] inline bool HasType() const noexcept(true)
				{
					return HasType(T::TYPE);
				}

				inline std::unique_ptr<BSExtraData> RemoveExtra(std::uint8_t Type)
				{
					const BSAutoWriteLock l{ extraRWLock };
					return extraData.RemoveExtra(Type);
				}

				template <Detail::ExtraDataListConstraint T>
				inline std::unique_ptr<T> RemoveExtra()
				{
					return std::unique_ptr<T>{ static_cast<T*>(RemoveExtra(T::TYPE).release()) };
				}

				inline void Dump(void(*Callback)(const char*, ...)) const noexcept(true)
				{
					extraData.Dump(Callback);
				}

				// members
				BaseExtraList extraData;              // 08
				mutable BSReadWriteLock extraRWLock;  // 20
			};
			static_assert(sizeof(ExtraDataList) == 0x28);
		}
	}
}