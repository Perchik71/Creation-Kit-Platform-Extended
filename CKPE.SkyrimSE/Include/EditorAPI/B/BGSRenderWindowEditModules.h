// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			// Render window edit modules and gizmos (1.7.99.0). Sizes from the allocations; members come from the constructors and the virtuals that read them.

			class BGSRenderWindow;

			// vtable RVA 0x30179F0, 46 virtual functions
			class BGSRenderWindowEditModule
			{
			public:
				virtual ~BGSRenderWindowEditModule() = default;

				BGSRenderWindow* renderWindow;	// 08 - constructor argument
				std::uint8_t unk10[0x8];		// 10
				std::uint8_t unk18;				// 18 - 0 on construction
				std::uint8_t pad19[3];			// 19
				std::uint32_t unk1C;			// 1C - 0 on construction
				std::uint8_t unk20[0x28];		// 20 - helper-constructed members (@20 and @2C are 12 byte objects)
			};
			static_assert(offsetof(BGSRenderWindowEditModule, renderWindow) == 0x08);
			static_assert(offsetof(BGSRenderWindowEditModule, unk18) == 0x18);
			static_assert(sizeof(BGSRenderWindowEditModule) == 0x48);

			// vtable RVA 0x3019278
			class BGSRenderWindowLandscapeEditModule : public BGSRenderWindowEditModule
			{
			public:
				std::uint8_t unk48[0x78];	// 48
				void* unkC0;				// C0 - window handle (DestroyWindow in the destructor)
				void* unkC8;				// C8 - set by the virtual at 70
				std::uint8_t unkD0[0x20];	// D0
				void* unkF0;				// F0
				void* unkF8;				// F8
			};
			static_assert(offsetof(BGSRenderWindowLandscapeEditModule, unkC0) == 0xC0);
			static_assert(sizeof(BGSRenderWindowLandscapeEditModule) == 0x100);

			// vtable RVA 0x301C748
			class BGSRenderWindowNavMeshEditModule : public BGSRenderWindowEditModule
			{
			public:
				std::uint8_t unk48[0x2D8];	// 48 - layout not resolved
			};
			static_assert(sizeof(BGSRenderWindowNavMeshEditModule) == 0x320);

			// vtable RVA 0x301E3D8
			class BGSRenderWindowPrimitiveEditModule : public BGSRenderWindowEditModule
			{
			public:
				std::uint8_t unk48[0x78];	// 48
				std::uint32_t unkC0;		// C0
				std::uint32_t unkC4;		// C4
				std::uint32_t unkC8;		// C8 - 2 on construction
				std::uint8_t unkCC[0xC];	// CC
			};
			static_assert(offsetof(BGSRenderWindowPrimitiveEditModule, unkC0) == 0xC0);
			static_assert(sizeof(BGSRenderWindowPrimitiveEditModule) == 0xD8);

			// vtable RVA 0x301FB50, 71 virtual functions
			class BGSRenderWindowReferenceEditModule : public BGSRenderWindowEditModule
			{
			public:
				std::uint8_t unk48[0xB8];	// 48 - layout not resolved
			};
			static_assert(sizeof(BGSRenderWindowReferenceEditModule) == 0x100);

			// vtable RVA 0x3018CD0, 9 virtual functions. The four concrete gizmos add no members.
			class BGSRenderWindowGizmo
			{
			public:
				virtual ~BGSRenderWindowGizmo() = default;

				void* unk08;				// 08 - helper-constructed (reference handle)
				void* unk10;				// 10 - helper-constructed
				void* unk18;				// 18 - helper-constructed
				std::uint8_t unk20;			// 20 - 1 on construction
				std::uint8_t pad21[3];		// 21
				std::uint32_t unk24;		// 24 - 10 on construction
				std::uint32_t unk28;		// 28 - 10 on construction
				std::uint8_t unk2C[0x74];	// 2C - helper-constructed objects at 30 and 68
			};
			static_assert(offsetof(BGSRenderWindowGizmo, unk20) == 0x20);
			static_assert(sizeof(BGSRenderWindowGizmo) == 0xA0);

			// vtable RVA 0x301E850
			class BGSRenderWindowPrimitiveGizmo : public BGSRenderWindowGizmo {};
			static_assert(sizeof(BGSRenderWindowPrimitiveGizmo) == 0xA0);
			// vtable RVA 0x3021180
			class BGSRenderWindowRotationGizmo : public BGSRenderWindowGizmo {};
			static_assert(sizeof(BGSRenderWindowRotationGizmo) == 0xA0);
			// vtable RVA 0x3021230
			class BGSRenderWindowScaleGizmo : public BGSRenderWindowGizmo {};
			static_assert(sizeof(BGSRenderWindowScaleGizmo) == 0xA0);
			// vtable RVA 0x30212E0
			class BGSRenderWindowTranslationGizmo : public BGSRenderWindowGizmo {};
			static_assert(sizeof(BGSRenderWindowTranslationGizmo) == 0xA0);
		}
	}
}