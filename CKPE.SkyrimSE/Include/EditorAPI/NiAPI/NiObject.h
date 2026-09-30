// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Utils.h>
#include <EditorAPI/NiAPI/NiRTTI.h>
#include <EditorAPI/NiAPI/NiRefObject.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			class BSDynamicTriShape;
			class BSFadeNode;
			class BSGeometry;
			class bhkAttachmentCollisionObject;
			class bhkBlendCollisionObject;
			class bhkLimitedHingeConstraint;
			class bhkNiCollisionObject;
			class bhkRigidBody;
			class BSLines;
			class BSMultiBoundNode;
			class BSSegmentedTriShape;
			class BSSubIndexTriShape;
			class BSTriShape;

			namespace NiAPI
			{
				class NiCloningProcess;
				class NiControllerManager;
				class NiGeometry;
				class NiNode;
				class NiObjectGroup;
				class NiParticles;
				class NiRTTI;
				class NiStream;
				class NiSwitchNode;
				class NiTriBasedGeom;
				class NiTriShape;
				class NiTriStrips;

				namespace Templates
				{
					// 10
					template<typename T>
					class NiObjectT : public T
					{
					public:
						virtual ~NiObjectT() = default;

						// add
						[[nodiscard]] virtual const NiRTTI* GetRTTI() const noexcept(true);
						[[nodiscard]] virtual NiNode* AsNode() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual NiSwitchNode* AsSwitchNode() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual BSFadeNode* AsFadeNode() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual BSMultiBoundNode* AsMultiBoundNode() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual BSGeometry* AsGeometry() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual NiTriStrips* AsTriStrips() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual BSTriShape* AsTriShape() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual BSSegmentedTriShape* AsSegmentedTriShape() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual BSSubIndexTriShape* AsSubIndexTriShape() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual BSDynamicTriShape* AsDynamicTriShape() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual NiGeometry* AsNiGeometry() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual NiTriBasedGeom* AsNiTriBasedGeom() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual NiTriShape* AsNiTriShape() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual NiParticles* AsParticlesGeom() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual BSLines* AsLinesGeom() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual bhkNiCollisionObject* AsBhkNiCollisionObject() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual bhkBlendCollisionObject* AsBhkBlendCollisionObject() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual bhkAttachmentCollisionObject* AsBhkAttachmentCollisionObject() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual bhkRigidBody* AsBhkRigidBody() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual bhkLimitedHingeConstraint* AsBhkLimitedHingeConstraint() noexcept(true) { return nullptr; }
						[[nodiscard]] virtual NiObjectT* CreateClone([[maybe_unused]] NiCloningProcess& a_cloning) { return this; }
						virtual void LoadBinary([[maybe_unused]] NiStream& a_stream) noexcept(true) { return; }
						virtual void LinkObject([[maybe_unused]] NiStream& a_stream) noexcept(true) { return; }
						virtual bool RegisterStreamables(NiStream& a_stream) noexcept(true);
						virtual void SaveBinary([[maybe_unused]] NiStream& a_stream) noexcept(true) { return; }
						[[nodiscard]] virtual bool IsEqual(NiObjectT* a_object) const noexcept(true);	// 27
						virtual void ProcessClone(NiCloningProcess& a_cloning) noexcept(true);
						virtual void subF0() noexcept(true);
						virtual void PostLinkObject([[maybe_unused]] NiStream& a_stream) noexcept(true) { return; }
						[[nodiscard]] virtual bool StreamCanSkip() noexcept(true) { return false; }
						[[nodiscard]] virtual const NiRTTI* GetStreamableRTTI() const noexcept(true) { return GetRTTI(); }
						[[nodiscard]] virtual std::uint32_t GetBlockAllocationSize() const noexcept(true) { return 0; }
						[[nodiscard]] virtual NiObjectGroup* GetGroup() const noexcept(true) { return nullptr; }
						virtual void SetGroup([[maybe_unused]] NiObjectGroup* a_group) noexcept(true) { return; }
						virtual NiControllerManager* AsNiControllerManager() noexcept(true) { return nullptr; }
						virtual void sub138() noexcept(true) { return; }

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

				class NiObject : public Templates::NiObjectT<NiRefObject64> {};
				static_assert(sizeof(NiObject) == 0x10);
			}
		}
	}
}