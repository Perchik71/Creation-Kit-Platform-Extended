// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Utils.h>
#include <EditorAPI/N/NiRTTI.h>
#include <EditorAPI/N/NiRefObject.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSGeometry;
			class BSTriShape;
			class BSDynamicTriShape;
			class BSFadeNode;
			class BSMultiBoundNode;
			class BSSegmentedTriShape;
			class BSSubIndexTriShape;
			class BSLines;

			namespace NiAPI
			{
				class NiStream;
				class NiCloningProcess;
				class NiNode;
				class NiSwitchNode;
				class NiGeometry;
				class NiTriBasedGeom;
				class NiTriShape;
				class NiTriStrips;
				class NiParticles;
				class NiParticleSystem;
				class NiLight;
				class NiObjectGroup;
				class NiControllerManager;
				class bhkNiCollisionObject;
				class bhkBlendCollisionObject;
				class bhkRigidBody;
				class bhkLimitedHingeConstraint;
				class bhkNPCollisionObject;

				namespace Templates
				{
					// 10
					template<
						typename T,
						std::uint32_t REL_RTTI,
						std::uint32_t REL_CREATECLONE,
						std::uint32_t REL_LOADFROMSTREAM,
						std::uint32_t REL_SAVETOSTREAM,
						std::uint32_t REL_LINKOBJECT,
						std::uint32_t REL_REGSTREAM>
					class NiObjectT : public T
					{
					public:
						// It's everywhere
						inline NiRTTI* GetRTTI() const noexcept(true) { return CKPE::vtbl_call<NiRTTI*>(REL_RTTI, this); }

						// Creation of a clone object is mentioned in SKSE and F4SE, however, in SKSE it is specified without a refs, I think this is a mistake
						inline NiObject* CreateClone(NiCloningProcess* Process) const noexcept(true) { return CKPE::vtbl_call<NiObject*>(REL_CREATECLONE, this, Process); }

						// There is a mention of these functions in SKSE and F4SE, actually this is no exception for CK
						inline void LoadFromStream(NiStream* Stream) const noexcept(true) { CKPE::vtbl_call<void>(REL_LOADFROMSTREAM, this, Stream); }
						inline void LinkObject(NiStream* Stream) const noexcept(true) { CKPE::vtbl_call<void>(REL_LINKOBJECT, this, Stream); }
						inline bool RegisterStreamables(NiStream* Stream) const noexcept(true) { return CKPE::vtbl_call<bool>(REL_REGSTREAM, this, Stream); }
						inline void SaveToStream(NiStream* Stream) const noexcept(true) { CKPE::vtbl_call<void>(REL_SAVETOSTREAM, this, Stream); }

						inline void GetViewerRTTI(void(*Callback)(const char*, ...), bool Fully = false) const noexcept(true)
						{
							auto Info = GetRTTI();

							if (Fully)
								for (auto iter = this->GetRTTI()->GetParent(); iter; iter = iter->GetParent())
									Callback("---- %s ----\n", iter->GetNameClass());

							Callback("-- %s --\n", Info->GetNameClass());
						}
					};
				}

				class NiObject : public Templates::NiObjectT<NiRefObject, 0x10, 0xD0, 0xD8, 0xF0, 0xE0, 0xE8>
				{
				public:
					using Templates::NiObjectT<NiRefObject, 0x10, 0xD0, 0xD8, 0xF0, 0xE0, 0xE8>::CreateClone;
					using Templates::NiObjectT<NiRefObject, 0x10, 0xD0, 0xD8, 0xF0, 0xE0, 0xE8>::LinkObject;
					using Templates::NiObjectT<NiRefObject, 0x10, 0xD0, 0xD8, 0xF0, 0xE0, 0xE8>::RegisterStreamables;

					/* 02 */ virtual NiRTTI* GetRTTI() const noexcept(true);
					/* 03 */ virtual const NiNode* IsNodeConst() const noexcept(true);
					/* 04 */ virtual NiNode* IsNode() noexcept(true);
					/* 05 */ virtual NiSwitchNode* IsSwitchNode() noexcept(true);
					/* 06 */ virtual BSFadeNode* IsFadeNode() noexcept(true);
					/* 07 */ virtual BSMultiBoundNode* IsMultiBoundNode() noexcept(true);
					/* 08 */ virtual BSGeometry* IsGeometry() noexcept(true);
					/* 09 */ virtual NiTriStrips* IsTriStrips() noexcept(true);
					/* 0A */ virtual BSTriShape* IsTriShape() noexcept(true);
					/* 0B */ virtual BSDynamicTriShape* IsDynamicTriShape() noexcept(true);
					/* 0C */ virtual BSSegmentedTriShape* IsSegmentedTriShape() noexcept(true);
					/* 0D */ virtual BSSubIndexTriShape* IsSubIndexTriShape() noexcept(true);
					/* 0E */ virtual NiGeometry* IsNiGeometry() noexcept(true);
					/* 0F */ virtual NiTriBasedGeom* IsNiTriBasedGeom() noexcept(true);
					/* 10 */ virtual NiTriShape* IsNiTriShape() noexcept(true);
					/* 11 */ virtual NiParticles* IsParticlesGeom() noexcept(true);
					/* 12 */ virtual NiParticleSystem* IsParticleSystem() noexcept(true);
					/* 13 */ virtual BSLines* IsLinesGeom() noexcept(true);
					/* 14 */ virtual NiLight* IsLight() noexcept(true);
					/* 15 */ virtual bhkNiCollisionObject* IsBhkNiCollisionObject() noexcept(true);
					/* 16 */ virtual bhkBlendCollisionObject* IsBhkBlendCollisionObject() noexcept(true);
					/* 17 */ virtual bhkRigidBody* IsBhkRigidBody() noexcept(true);
					/* 18 */ virtual bhkLimitedHingeConstraint* IsBhkLimitedHingeConstraint() noexcept(true);
					/* 19 */ virtual bhkNPCollisionObject* IsbhkNPCollisionObject() noexcept(true);
					/* 1A */ virtual NiObject* CreateClone(NiCloningProcess& Process) noexcept(true);
					/* 1B */ virtual void LoadBinary(NiStream& Stream) noexcept(true);
					/* 1C */ virtual void LinkObject(NiStream& Stream) noexcept(true);
					/* 1D */ virtual bool RegisterStreamables(NiStream& Stream) noexcept(true);
					/* 1E */ virtual void SaveBinary(NiStream& Stream) noexcept(true);
					/* 1F */ virtual bool IsEqual(NiObject* Object) noexcept(true);
					/* 20 */ virtual void GetViewerStringsArray(void* Strings) noexcept(true);
					/* 21 */ virtual void ProcessClone(NiCloningProcess& Process) noexcept(true);
					/* 22 */ virtual void PostLinkObject(NiStream& Stream) noexcept(true);
					/* 23 */ virtual bool StreamCanSkip() noexcept(true);
					/* 24 */ virtual const NiRTTI* GetStreamableRTTI() noexcept(true);
					/* 25 */ virtual std::uint32_t GetBlockAllocationSize() const noexcept(true);
					/* 26 */ virtual NiObjectGroup* GetGroup() const noexcept(true);
					/* 27 */ virtual void SetGroup(NiObjectGroup* Group) noexcept(true);
					/* 28 */ virtual NiControllerManager* IsNiControllerManager() noexcept(true);
					/* 29 */ virtual void GetAdditionalViewerStrings(void* Strings) noexcept(true);
				};
				static_assert(sizeof(NiObject) == 0x10);
			}
		}
	}
}