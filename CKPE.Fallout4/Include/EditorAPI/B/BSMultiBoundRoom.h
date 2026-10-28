// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSMultiBoundNode.h>
#include <EditorAPI/N/NiTPointerList.h>
#include <EditorAPI/B/BSTArray.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSMultiBound;
			class BSLight;
			class BSPortal;
			class BSOcclusionShape;

			class BSMultiBoundRoom : public BSMultiBoundNode
			{
				/* 0x150 */ NiAPI::NiTPointerList<BSPortal*> _Portals;
				/* 0x168 */ NiAPI::NiTPointerList<BSOcclusionShape*> _Occluders;
				/* 0x180 */ NiAPI::NiTPointerList<NiAPI::NiPointer<BSMultiBound>> _JoinedMultiBounds;
				/* 0x198 */ BSTArray<BSLight*> _Lights;
			public:
				virtual ~BSMultiBoundRoom() = default;

				constexpr static Common::ID VTABLE = IDs::VTABLE::BSMultiBoundRoom[0];

				[[nodiscard]] inline const NiAPI::NiTPointerList<BSPortal*>& GetPortals() const noexcept(true) { return _Portals; }
				[[nodiscard]] inline const NiAPI::NiTPointerList<BSOcclusionShape*>& GetOccluders() const noexcept(true) { return _Occluders; }
				[[nodiscard]] inline const NiAPI::NiTPointerList<NiAPI::NiPointer<BSMultiBound>>& GetJoinedMultiBounds() const noexcept(true) { return _JoinedMultiBounds; }
				[[nodiscard]] inline BSTArray<BSLight*>& GetLights() noexcept(true) { return _Lights; }

				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BSMultiBoundRoom, _Portals) == 0x150 &&
						offsetof(BSMultiBoundRoom, _Occluders) == 0x168 &&
						offsetof(BSMultiBoundRoom, _JoinedMultiBounds) == 0x180 &&
						offsetof(BSMultiBoundRoom, _Lights) == 0x198;
				}
			};
			static_assert(sizeof(BSMultiBoundRoom) == 0x1B0);
			static_assert(BSMultiBoundRoom::VerifyLayout());
			static_assert(sizeof(NiAPI::NiTPointerList<BSPortal*>) == 0x18);
			static_assert(sizeof(NiAPI::NiTPointerList<BSOcclusionShape*>) == 0x18);
			static_assert(sizeof(NiAPI::NiTPointerList<NiAPI::NiPointer<BSMultiBound>>) == 0x18);
			static_assert(NiAPI::NiTPointerList<BSPortal*>::VerifyLayout());
			static_assert(NiAPI::NiTPointerList<BSOcclusionShape*>::VerifyLayout());
			static_assert(NiAPI::NiTPointerList<NiAPI::NiPointer<BSMultiBound>>::VerifyLayout());
		}
	}
}