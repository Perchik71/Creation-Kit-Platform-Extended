// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiObject.h

#pragma once

#include "NiRefObject.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		class __declspec(align(8)) NiObject : public NiRefObject
		{
		public:
			virtual const void* GetRTTI() const;
			virtual const void* IsNode() const;
			virtual class NiSwitchNode* IsSwitchNode() const;
			virtual class BSFadeNode* IsFadeNode() const;
			virtual class BSMultiBoundNode* IsMultiBoundNode() const;
			virtual class BSGeometry* IsGeometry() const;
			virtual class NiTriStrips* IsTriStrips() const;
			virtual class BSTriShape* IsTriShape() const;
			virtual class BSSegmentedTriShape* IsSegmentedTriShape() const;
			virtual class BSSubIndexTriShape* IsSubIndexTriShape() const;
			virtual class BSDynamicTriShape* IsDynamicTriShape() const;
			virtual class NiGeometry* IsNiGeometry() const;
			virtual class NiTriBasedGeom* IsNiTriBasedGeom() const;
			virtual class NiTriShape* IsNiTriShape() const;
			virtual class NiParticles* IsParticlesGeom() const;
			virtual class BSLines* IsLinesGeom() const;
			virtual class bhkNiCollisionObject* IsBhkNiCollisionObject() const;
			virtual class bhkBlendCollisionObject* IsBhkBlendCollisionObject() const;
			virtual class bhkAttachmentCollisionObject* IsBhkAttachmentCollisionObject() const;
			virtual class bhkRigidBody* IsBhkRigidBody() const;
			virtual class bhkLimitedHingeConstraint* IsBhkLimitedHingeConstraint() const;

			virtual NiObject* CreateClone(void*);
			virtual void LoadBinary(void*);
			virtual void LinkObject(void*);
			virtual bool RegisterStreamables(void*);
			virtual void SaveBinary(void*);
			virtual bool IsEqual(NiObject* Other);
			virtual void ProcessClone(void*);
			virtual void PostLinkObject(void*);
			virtual bool StreamCanSkip();
			virtual const void* GetStreamableRTTI();
			virtual unsigned int GetBlockAllocationSize();
			virtual void* GetGroup();
			virtual void* SetGroup(void*);
			virtual class NiControllerManager* IsNiControllerManager() const;

			void GetViewerStrings(void(*Callback)(const char*, ...), bool Recursive) const
			{
				if (Recursive)
					__super::GetViewerStrings(Callback, Recursive);

				/*Callback("-- NiObject --\n");
				Callback("NiRTTI = %s\n", GetRTTI()->GetName());*/
			}

			bool IsExactKindOf(void* RTTI) const
			{
				return GetRTTI() == RTTI;
			}

			bool IsKindOf(void* RTTI) const
			{
				/*for (auto currentRTTI = GetRTTI(); currentRTTI; currentRTTI = currentRTTI->GetBaseRTTI())
				{
					if (currentRTTI == RTTI)
						return true;
				}*/

				return false;
			}
		};
		static_assert(sizeof(NiObject) == 0x10);
	}
}