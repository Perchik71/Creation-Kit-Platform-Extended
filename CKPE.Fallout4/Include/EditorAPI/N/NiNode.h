// Copyright © 2023-2024 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <EditorAPI/N/NiAVObject.h>
#include <EditorAPI/N/NiTObjectArray.h>
#include <EditorAPI/B/BSAtomicValue.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				class NiNode : public NiAVObject
				{
					NiTObjectArray<NiPointer<NiAVObject>> _array_objs;	// 0x120
					BSTAtomicValue<std::uint32_t> _dirtyState;			// 0x138
					float _meshLODFadeAmount;							// 0x13C
				public:
					virtual ~NiNode() = default;

					/* 3C */ virtual void AttachChild(NiAVObject* Child, bool FirstAvailable) noexcept(true);
					/* 3D */ virtual void InsertChildAt(std::uint32_t Index, NiAVObject* Child) noexcept(true);
					/* 3E */ virtual void DetachChildAndGet(NiAVObject* Child, NiPointer<NiAVObject>& Result) noexcept(true);
					/* 3F */ virtual void DetachChild(NiAVObject* Child) noexcept(true);
					/* 40 */ virtual void DetachChildAtAndGet(std::uint32_t Index, NiPointer<NiAVObject>& Result) noexcept(true);
					/* 41 */ virtual void DetachChildAt(std::uint32_t Index) noexcept(true);
					/* 42 */ virtual void SetAtAndGet(std::uint32_t Index, NiAVObject* Child, NiPointer<NiAVObject>& Result) noexcept(true);
					/* 43 */ virtual void SetAt(std::uint32_t Index, NiAVObject* Child) noexcept(true);
					/* 44 */ virtual void UpdateUpwardPass(NiUpdateData& Data) noexcept(true);
					/* 45 */ [[nodiscard]] virtual bool IsCombinedNode() const noexcept(true);
				public:
					// CommonLibF4: NiNode::children
					[[nodiscard]] inline const NiTObjectArray<NiPointer<NiAVObject>>& GetChildren() const noexcept(true) { return _array_objs; }

					[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
					{
						return offsetof(NiNode, _array_objs) == 0x120 &&
							offsetof(NiNode, _dirtyState) == 0x138 &&
							offsetof(NiNode, _meshLODFadeAmount) == 0x13C;
					}
				};
				static_assert(sizeof(NiNode) == 0x140);
				static_assert(NiNode::VerifyLayout());
			}
		}
	}
}