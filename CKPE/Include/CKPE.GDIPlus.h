// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <string>
#include <Windows.h>
#include <CKPE.Stream.h>

namespace CKPE
{
#ifndef CKPE_GRAPHICS
	typedef void* THandle;
	typedef std::uint32_t TColor;
#endif // !CKPE_GRAPHICS

	class CKPE_API GDIPlus
	{
		GDIPlus(GDIPlus&&) = delete;
		GDIPlus(const GDIPlus&) = delete;
		GDIPlus& operator=(GDIPlus&&) = delete;
		GDIPlus& operator=(const GDIPlus&) = delete;
	public:
		GDIPlus() noexcept(true);
		virtual ~GDIPlus() noexcept(true);

		static THandle LoadImageFromFile(const std::wstring& fname) noexcept(true);
		static THandle LoadImageFromStream(Stream& stream) noexcept(true);
		static THandle LoadImageFromResource(THandle hInst, std::uint32_t dwResId,
			const wchar_t* ResType) noexcept(true);
		static THandle ResizeImage(THandle hBmp, std::int32_t newWidth, std::int32_t newHeight) noexcept(true);

		[[nodiscard]] static GDIPlus* GetSingleton() noexcept(true);
	};

	struct CKPE_API GDIPlusColor
	{
		std::uint8_t r;
		std::uint8_t g;
		std::uint8_t b;
		std::uint8_t a;

		GDIPlusColor() noexcept(true);
		GDIPlusColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255) noexcept(true);
		GDIPlusColor(const GDIPlusColor& color) noexcept(true);
		GDIPlusColor(TColor color) noexcept(true);

		GDIPlusColor& operator=(const GDIPlusColor& color) noexcept(true);
		GDIPlusColor& operator=(GDIPlusColor&&) noexcept = default;
		GDIPlusColor& operator=(TColor color) noexcept(true);

		GDIPlusColor operator*(float value) noexcept(true);
		GDIPlusColor operator/(float value) noexcept(true);
		GDIPlusColor& operator*=(float value) noexcept(true);
		GDIPlusColor& operator/=(float value) noexcept(true);
	};

	class CKPE_API GDIPlusRenderer
	{
		GDIPlusRenderer(GDIPlusRenderer&&) = delete;
		GDIPlusRenderer(const GDIPlusRenderer&) = delete;
		GDIPlusRenderer& operator=(GDIPlusRenderer&&) = delete;
		GDIPlusRenderer& operator=(const GDIPlusRenderer&) = delete;
	public:
		GDIPlusRenderer() noexcept(true) = default;
		virtual ~GDIPlusRenderer() noexcept(true) = default;

		static THandle DrawBegin(THandle hSrcDC, LPCRECT lpSrcRect = nullptr) noexcept(true);
		static void DrawEnd(THandle hRenderer) noexcept(true);

		static void SetClearColor(THandle hRenderer, std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept(true);

		static void Clear(THandle hRenderer) noexcept(true);
		static void FillSolidRect(THandle hRenderer, LPCRECT lpRect, const GDIPlusColor& color) noexcept(true);
		static void FillRoundedRect(THandle hRenderer, LPCRECT lpRect, std::int32_t radius, const GDIPlusColor& color) noexcept(true);
		static void FillRoundedRectGradient(THandle hRenderer, LPCRECT lpRect, std::int32_t radius, const GDIPlusColor& top, const GDIPlusColor& bottom) noexcept(true);
		static void FillRoundedRectGradientWithMiddle(THandle hRenderer, LPCRECT lpRect, std::int32_t radius, const GDIPlusColor& top,
			const GDIPlusColor& middle, float fMiddlePos, const GDIPlusColor& bottom) noexcept(true);
		static void OutlineRoundedRect(THandle hRenderer, LPCRECT lpRect, std::int32_t radius, const GDIPlusColor& color, float fWidth = 1.0f) noexcept(true);
		static void DrawRoundedRectShadow(THandle hRenderer, LPCRECT lpRect, std::int32_t radius, const GDIPlusColor& color, std::int32_t blur,
			std::int32_t offsetX, std::int32_t offsetY) noexcept(true);
	};

	class CKPE_API GDIPlusRendererHelpers
	{
	public:
		// Готовая фигура: панель
		static void DrawRoundedSquare(THandle hRenderer, LPCRECT lpRect,
			std::int32_t radius,
			const GDIPlusColor& bodyGradientTop,
			const GDIPlusColor& bodyGradientBottom,
			const GDIPlusColor& innerRing,
			const GDIPlusColor& background,
			const GDIPlusColor& highlightTop,
			const GDIPlusColor& highlightBottom,
			std::int32_t glowWidth,
			std::int32_t innerRingWidth) noexcept(true);
	};
}