// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include <CKPE.Common.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BGSRenderWindowBorder
			{
			public:
				enum class Color : std::uint32_t
				{
					Green = 0,
					Yellow = 1,
					Red = 2,
				};

				virtual ~BGSRenderWindowBorder() = default;

				[[nodiscard]] inline HWND GetWindowHandle() const noexcept(true) { return _WindowHandle; }
				[[nodiscard]] inline bool IsVisible() const noexcept(true) { return _Visible; }
				CKPE_READ_PROPERTY(GetWindowHandle) HWND WindowHandle;
				CKPE_READ_PROPERTY(IsVisible) bool Visible;

				[[nodiscard]] static constexpr bool VerifyLayout() noexcept(true)
				{
					return offsetof(BGSRenderWindowBorder, _WindowHandle) == 0x8 &&
						offsetof(BGSRenderWindowBorder, _GreenBrush) == 0x30 &&
						offsetof(BGSRenderWindowBorder, _Visible) == 0x4C;
				}
			private:
				/* 0x008 */ HWND _WindowHandle;
				/* 0x010 */ HWND _Edges[4];
				/* 0x030 */ HBRUSH _GreenBrush;
				/* 0x038 */ HBRUSH _YellowBrush;
				/* 0x040 */ HBRUSH _RedBrush;
				/* 0x048 */ Color _Color;
				/* 0x04C */ bool _Visible;
				/* 0x04D */ bool _NeedsRedraw;
				/* 0x04E */ char pad04E[0x2];
			};
			static_assert(sizeof(BGSRenderWindowBorder) == 0x50);
			static_assert(BGSRenderWindowBorder::VerifyLayout());
		}
	}
}
