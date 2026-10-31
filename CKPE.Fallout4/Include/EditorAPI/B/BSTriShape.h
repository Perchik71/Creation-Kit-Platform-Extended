// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BSGeometry.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 170
			class BSTriShape : public BSGeometry
			{
			public:
				/* 02 */ virtual NiAPI::NiRTTI* GetRTTI() const noexcept(true) override;
				/* 0A */ virtual BSTriShape* IsTriShape() noexcept(true) override;
				/* 1A */ virtual NiAPI::NiObject* CreateClone(NiAPI::NiCloningProcess& Process) noexcept(true) override;
				/* 1B */ virtual void LoadBinary(NiAPI::NiStream& Stream) noexcept(true) override;
				/* 1C */ virtual void LinkObject(NiAPI::NiStream& Stream) noexcept(true) override;
				/* 1D */ virtual bool RegisterStreamables(NiAPI::NiStream& Stream) noexcept(true) override;
				/* 1E */ virtual void SaveBinary(NiAPI::NiStream& Stream) noexcept(true) override;
				/* 1F */ virtual bool IsEqual(NiAPI::NiObject* Object) noexcept(true) override;
				/* 20 */ virtual void GetViewerStringsArray(void* Strings) noexcept(true) override;
				constexpr static Common::ID VTABLE = IDs::VTABLE::BSTriShape[0];

				[[nodiscard]] inline std::uint32_t GetTriangles() const noexcept(true) { return Triangles; }
				[[nodiscard]] inline std::uint32_t GetVertices() const noexcept(true) { return Vertices; }
			private:
				std::uint32_t Triangles;	// 160
				std::uint16_t Vertices;		// 164
				char Pad166[0xA];			// 166
			public:
				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BSTriShape, Triangles) == 0x160 &&
						offsetof(BSTriShape, Vertices) == 0x164;
				}
			};
			static_assert(sizeof(BSTriShape) == 0x170);
			static_assert(BSTriShape::VerifyLayout());
		}
	}
}