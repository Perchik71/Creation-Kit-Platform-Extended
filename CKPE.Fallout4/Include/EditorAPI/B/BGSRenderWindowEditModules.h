// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/B/BGSRenderWindowReferenceEditModule.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// 1.11.246.0: size 0xD0 (the deleting destructor frees 208 bytes). Overrides 0x08, 0x10, 0x60, 0xD0 and 0x238 of the base module;
			// only the named ones are declared here.
			class BGSRenderWindowPrimitiveEditModule : public BGSRenderWindowEditModule
			{
			public:
				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSRenderWindowPrimitiveEditModule[0];

				virtual ~BGSRenderWindowPrimitiveEditModule() = default;
				/* 0x008 */ void BuildContextMenu() override;
				/* 0x010 */ void RegisterHotkeyFunctions() override;
				/* 0x060 */ void OnLButtonUp() override;

				/* 0x0B0 */ std::byte unk0B0[0x20];
			};
			static_assert(sizeof(BGSRenderWindowPrimitiveEditModule) == 0xD0);

			// 1.11.246.0: size 0x358 (the deleting destructor frees 856 bytes). 35 slots of the base module are overridden; only the named ones are declared.
			class BGSRenderWindowNavMeshEditModule : public BGSRenderWindowEditModule
			{
			public:
				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSRenderWindowNavMeshEditModule[0];

				virtual ~BGSRenderWindowNavMeshEditModule() = default;
				/* 0x010 */ void RegisterHotkeyFunctions() override;
				/* 0x058 */ void OnLButtonDown() override;
				/* 0x068 */ void OnLButtonDoubleClick() override;
				/* 0x070 */ void OnRButtonDown() override;
				/* 0x078 */ void OnRButtonUp() override;
				/* 0x080 */ void OnMouseMove() override;
				/* 0x0B0 */ void SnapPositionToGrid(float* Position, float GridSize, const void* ReferenceHandle) override;
				/* 0x140 */ void UpdateStatusBarText() override;

				/* 0x0B0 */ std::byte unk0B0[0x358 - 0xB0];
			};
			static_assert(sizeof(BGSRenderWindowNavMeshEditModule) == 0x358);

			// 1.11.246.0: size 0x110 (the deleting destructor frees 272 bytes). The constructor creates the landscape dialog (template 0xCB) and keeps its
			// window handle at 0xB8 and a smart pointer to the landscape editing data at 0xB0.
			class BGSRenderWindowLandscapeEditModule : public BGSRenderWindowEditModule
			{
			public:
				constexpr static Common::ID VTABLE = IDs::VTABLE::BGSRenderWindowLandscapeEditModule[0];

				virtual ~BGSRenderWindowLandscapeEditModule() = default;
				/* 0x008 */ void BuildContextMenu() override;
				/* 0x010 */ void RegisterHotkeyFunctions() override;
				/* 0x058 */ void OnLButtonDown() override;
				/* 0x060 */ void OnLButtonUp() override;
				/* 0x070 */ void OnRButtonDown() override;
				/* 0x078 */ void OnRButtonUp() override;
				/* 0x080 */ void OnMouseMove() override;
				/* 0x088 */ void OnMouseWheel() override;

				/* 0x0B0 */ void* landscapeData;
				/* 0x0B8 */ void* dialog; // HWND
				/* 0x0C0 */ std::byte unk0C0[0x110 - 0xC0];
			};
			static_assert(sizeof(BGSRenderWindowLandscapeEditModule) == 0x110);
			static_assert(offsetof(BGSRenderWindowLandscapeEditModule, dialog) == 0xB8);
		}
	}
}
