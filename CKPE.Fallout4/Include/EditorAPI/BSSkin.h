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
#include "BSTArray.h"

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSSkin
			{
			public:
				// 28
				class BoneData : public NiAPI::NiObject
				{
				public:
					// 50
					struct BoneTransforms
					{
						NiAPI::NiBound		BoundingSphere;	// 00
						NiAPI::NiTransform	Transform;		// 10
					};

					[[nodiscard]] inline BoneTransforms& GetAt(std::uint32_t Index) noexcept(true) { return Transforms[Index]; }
					[[nodiscard]] inline BoneTransforms GetAt(std::uint32_t Index) const noexcept(true) { return Transforms[Index]; }
					[[nodiscard]] inline uint16_t GetCount() const noexcept(true) { return Transforms.size(); }

					inline BoneTransforms& operator[](std::uint32_t Index) noexcept(true) { return GetAt(Index); }
					inline BoneTransforms operator[](std::uint32_t Index) const noexcept(true) { return GetAt(Index); }
				private:
					BSTArray<BoneTransforms> Transforms; // 10
				};

				// C0
				class Instance : public NiAPI::NiObject
				{
				public:
					[[nodiscard]] inline NiAPI::NiNode* GetBonesAt(std::uint32_t Index) const noexcept(true) { return Bones[Index]; }
					[[nodiscard]] inline std::uint32_t GetBonesCount() const noexcept(true) { return Bones.size(); }
					[[nodiscard]] inline NiAPI::NiTransform* GetTransformAt(std::uint32_t Index) const noexcept(true) { return WorldTransforms[Index]; }
					[[nodiscard]] inline std::uint32_t GetTransformCount() const noexcept(true) { return WorldTransforms.size(); }
					[[nodiscard]] inline BoneData* GetBoneData() const noexcept(true) { return BoneData; }
					[[nodiscard]] inline NiAPI::NiAVObject* GetRootNode() const noexcept(true) { return RootNode; }
				private:
					BSTArray<NiAPI::NiNode*>		Bones;				// 10
					BSTArray<NiAPI::NiTransform*>	WorldTransforms;	// 28
					BoneData*						BoneData;			// 40
					NiAPI::NiAVObject*				RootNode;			// 48
					char							Pad50[0x10];		// 50
					NiAPI::NiMatrix4				Unk60;				// Usually identity matrix
					char							PadA0[0x20];		// A0
				};
			};
			static_assert(sizeof(BSSkin::BoneData::BoneTransforms) == 0x50);
			static_assert(sizeof(BSSkin::BoneData) == 0x28);
			static_assert(sizeof(BSSkin::Instance) == 0xC0);
		}
	}
}