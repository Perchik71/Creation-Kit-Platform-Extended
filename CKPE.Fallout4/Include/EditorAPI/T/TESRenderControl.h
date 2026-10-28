// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <Windows.h>
#include <CKPE.Common.h>
#include <EditorAPI/N/NiNode.h>
#include <EditorAPI/N/NiCamera.h>
#include <EditorAPI/N/NiPointer.h>
#include <EditorAPI/N/NiTypes.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class TESRenderControl
			{
			public:
				/* 0x000 */ virtual ~TESRenderControl();

				constexpr static Common::ID VTABLE = IDs::VTABLE::TESRenderControl[0];

				/* 0x008 */ virtual void AttachObject(NiAPI::NiAVObject* Object) noexcept(true);
				/* 0x010 */ virtual void DetachObject(NiAPI::NiAVObject* Object) noexcept(true);

			protected:
				// Unknown signatures are reserved slots, not callable interfaces.
				/* 0x018 */ virtual void Unk_018() noexcept(true);
				/* 0x020 */ virtual void ResizeViewport() noexcept(true);
				/* 0x028 */ virtual void Unk_028() noexcept(true);
				/* 0x030 */ virtual void Unk_030() noexcept(true);
				/* 0x038 */ virtual void Unk_038() noexcept(true);
				/* 0x040 */ virtual void Unk_040() noexcept(true);
				/* 0x048 */ virtual void Unk_048() noexcept(true);

			public:
				[[nodiscard]] inline HWND GetDisplayWindow() const noexcept(true)
				{
					return GetDlgItem(DialogWindow, DisplayControlID);
				}

				CKPE_READ_PROPERTY(GetDisplayWindow) HWND DisplayWindow;

				/* 0x008 */ HWND DialogWindow;
				/* 0x010 */ std::uint32_t DisplayControlID;
				/* 0x014 */ char pad14[0xC];
				/* 0x020 */ NiAPI::NiPointer<NiAPI::NiNode> SceneRoot;
				/* 0x028 */ char pad28[0x28];
				/* 0x050 */ NiAPI::NiPointer<NiAPI::NiNode> CameraRoot;
				/* 0x058 */ NiAPI::NiPointer<NiAPI::NiCamera> Camera;
				/* 0x060 */ NiAPI::NiWindow* RenderWindow;
				/* 0x068 */ char pad68[0x48];
			};
			static_assert(sizeof(TESRenderControl) == 0xB0);
			static_assert(offsetof(TESRenderControl, DialogWindow) == 0x8);
			static_assert(offsetof(TESRenderControl, DisplayControlID) == 0x10);
			static_assert(offsetof(TESRenderControl, SceneRoot) == 0x20);
			static_assert(offsetof(TESRenderControl, CameraRoot) == 0x50);
			static_assert(offsetof(TESRenderControl, Camera) == 0x58);
			static_assert(offsetof(TESRenderControl, RenderWindow) == 0x60);
		}
	}
}
