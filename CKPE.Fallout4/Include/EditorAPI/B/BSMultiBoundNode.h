// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSNiNode.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSMultiBound;
			class BSMultiBoundRoom;

			class BSMultiBoundNode : public BSNiNode
			{
			public:
				/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
				/* 07 */ virtual BSMultiBoundNode* IsMultiBoundNode() noexcept(true) override;
				/* 1A */ virtual NiAPI::NiObject* CreateClone(NiAPI::NiCloningProcess& Process) noexcept(true) override;
				/* 1B */ virtual void LoadBinary(NiAPI::NiStream& Stream) noexcept(true) override;
				/* 1C */ virtual void LinkObject(NiAPI::NiStream& Stream) noexcept(true) override;
				/* 1D */ virtual bool RegisterStreamables(NiAPI::NiStream& Stream) noexcept(true) override;
				/* 1E */ virtual void SaveBinary(NiAPI::NiStream& Stream) noexcept(true) override;
				/* 1F */ virtual bool IsEqual(NiAPI::NiObject* Object) noexcept(true) override;
				/* 20 */ virtual void GetViewerStringsArray(void* Strings) noexcept(true) override;
				/* 32 */ virtual void UpdateDownwardPass(NiAPI::NiUpdateData& Data, std::uint32_t Flags) noexcept(true) override;
				/* 35 */ virtual void UpdateWorldBound() noexcept(true) override;
				/* 3B */ virtual void OnVisible(NiAPI::NiCullingProcess& Culler) noexcept(true) override;
				/* 44 */ virtual void UpdateUpwardPass(NiAPI::NiUpdateData& Data) noexcept(true) override;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSMultiBoundNode[0];

				/* 46 */ [[nodiscard]] virtual BSMultiBoundRoom* GetMultiBoundRoom() noexcept(true);
				/* 47 */ [[nodiscard]] virtual bool IsPointWithin(const NiAPI::NiPoint3* Point) noexcept(true);
				/* 48 */ [[nodiscard]] virtual std::uint32_t CheckMultiBound(const BSMultiBound* Bound) noexcept(true);
				/* 49 */ [[nodiscard]] virtual std::uint32_t CheckBound(const NiAPI::NiBound* Bound) noexcept(true);

				[[nodiscard]] inline const NiAPI::NiPointer<BSMultiBound>& GetMultiBound() const noexcept(true) { return _MultiBound; }
				[[nodiscard]] inline std::uint32_t GetCullingMode() const noexcept(true) { return _CullingMode; }
			private:
				/* 0x140 */ NiAPI::NiPointer<BSMultiBound> _MultiBound;
				/* 0x148 */ std::uint32_t _CullingMode;
				/* 0x14C */ float _LastAccumTime;
			public:
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BSMultiBoundNode, _MultiBound) == 0x140 &&
						offsetof(BSMultiBoundNode, _CullingMode) == 0x148 &&
						offsetof(BSMultiBoundNode, _LastAccumTime) == 0x14C;
				}
			};
			static_assert(sizeof(BSMultiBoundNode) == 0x150);
			static_assert(BSMultiBoundNode::VerifyLayout());
		}
	}
}
