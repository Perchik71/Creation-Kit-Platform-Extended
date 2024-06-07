// Copyright © 2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <xmmintrin.h>
#include "NiAPI/NiNode.h"
#include "NiAPI/NiTypes.h"
#include "NiAPI/NiPoint.h"
#include "NiAPI/NiMatrix.h"
#include "NiAPI/NiObject.h"
#include "..\BSTArray.h"

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace Fallout4
		{
			using namespace NiAPI::Fallout4;

			class BSSkin
			{
			public:
				// 28
				class BoneData : public NiObject
				{
				public:
					// 50
					struct BoneTransforms
					{
						NiAPI::NiBound	BoundingSphere;	// 00
						NiTransform		Transform;		// 10
					};

					inline BoneTransforms& GetAt(uint32_t Index) { return Transforms[Index]; }
					inline BoneTransforms GetAt(uint32_t Index) const { return Transforms[Index]; }
					inline uint16_t GetCount() const { return Transforms.QSize(); }

					inline BoneTransforms& operator[](uint32_t Index) { return GetAt(Index); }
					inline BoneTransforms operator[](uint32_t Index) const { return GetAt(Index); }
				private:
					BSTArray<BoneTransforms> Transforms; // 10
				};

				// C0
				class Instance : public NiObject
				{
				public:
					inline NiNode* GetBonesAt(uint32_t Index) const { return Bones[Index]; }
					inline uint32_t GetBonesCount() const { return Bones.QSize(); }
					inline NiTransform* GetTransformAt(uint32_t Index) const { return WorldTransforms[Index]; }
					inline uint32_t GetTransformCount() const { return WorldTransforms.QSize(); }
					inline BoneData* GetBoneData() const { return BoneData; }
					inline NiAVObject* GetRootNode() const { return RootNode; }
				private:
					BSTArray<NiNode*>		Bones;				// 10
					BSTArray<NiTransform*>	WorldTransforms;	// 28
					BoneData*				BoneData;			// 40
					NiAVObject*				RootNode;			// 48
					char					Pad50[0x10];		// 50
					NiAPI::NiMatrix4		Unk60;				// Usually identity matrix
					char					PadA0[0x20];		// A0
				};
			};
			static_assert(sizeof(BSSkin::BoneData::BoneTransforms) == 0x50);
			static_assert(sizeof(BSSkin::BoneData) == 0x28);
			static_assert(sizeof(BSSkin::Instance) == 0xC0);
		}
	}
}