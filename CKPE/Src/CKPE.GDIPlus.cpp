// Copyright © 2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>

// Microsoft: It's probably cool when you need to make several includes instead of one 
// Perchik71: NO

#include <shlobj_core.h>
#include <gdiplusenums.h>
#include <gdiplustypes.h>
#include <gdiplus.h>

#include <CKPE.GDIPlus.h>
#include <CKPE.Stream.h>
#include <CKPE.PathUtils.h>

#include <format>

namespace CKPE
{
	static GDIPlus _sgdiplus;
	static Gdiplus::GdiplusStartupInput	_sgdiplusStartupInput;
	static ULONG_PTR _sgdiplusToken;

	GDIPlus::GDIPlus() noexcept(true)
	{
		Gdiplus::GdiplusStartup(&_sgdiplusToken, &_sgdiplusStartupInput, nullptr);
	}

	GDIPlus::~GDIPlus() noexcept(true)
	{
		Gdiplus::GdiplusShutdown(_sgdiplusToken);
	}

	THandle GDIPlus::LoadImageFromFile(const std::wstring& fname) noexcept(true)
	{
		if (!PathUtils::FileExists(fname))
			return nullptr;

		HBITMAP Result = nullptr;

		IStream* pStream = nullptr;
		Gdiplus::Bitmap* pBmp = nullptr;
		HGLOBAL hGlobal = nullptr;

		try
		{
			MemoryStream stream;
			stream.LoadFromFile(fname);

			// copy image bytes into a real hglobal memory handle
			hGlobal = ::GlobalAlloc(GHND, stream.GetSize());
			if (hGlobal)
			{
				void* pBuffer = ::GlobalLock(hGlobal);
				if (pBuffer)
				{
					memcpy(pBuffer, stream.Data(), stream.GetSize());
					HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
					if (SUCCEEDED(hr))
					{
						// pStream now owns the global handle and will invoke GlobalFree on release
						hGlobal = nullptr;
						pBmp = new Gdiplus::Bitmap(pStream);
					}
				}
			}
		}
		catch (const std::exception&)
		{}

		if (pStream)
		{
			pStream->Release();
			pStream = nullptr;
		}
		if (hGlobal)
		{
			GlobalFree(hGlobal);
			hGlobal = nullptr;
		}

		if (!pBmp)
			return 0;
		else
		{
			// Passing a transparent background (Color(0,0,0,0)) forces an alpha channel 
			// to be preserved if the HBITMAP format supports it.
			pBmp->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &Result);
			delete pBmp;
		}

		return Result;
	}

	THandle GDIPlus::LoadImageFromStream(Stream& stream) noexcept(true)
	{
		HBITMAP Result = nullptr;

		IStream* pStream = nullptr;
		Gdiplus::Bitmap* pBmp = nullptr;
		HGLOBAL hGlobal = nullptr;
		try
		{
			MemoryStream memstream;
			memstream.LoadFromStream(stream);

			// copy image bytes into a real hglobal memory handle
			hGlobal = ::GlobalAlloc(GHND, memstream.GetSize());
			if (hGlobal)
			{
				void* pBuffer = ::GlobalLock(hGlobal);
				if (pBuffer)
				{
					memcpy(pBuffer, memstream.Data(), memstream.GetSize());
					HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);

					if (SUCCEEDED(hr))
					{
						// pStream now owns the global handle and will invoke GlobalFree on release
						hGlobal = nullptr;
						pBmp = new Gdiplus::Bitmap(pStream);
					}
				}
			}
		}
		catch (const std::exception&)
		{}

		if (pStream)
		{
			pStream->Release();
			pStream = nullptr;
		}
		if (hGlobal)
		{
			GlobalFree(hGlobal);
			hGlobal = nullptr;
		}

		if (!pBmp)
			return 0;
		else
		{
			pBmp->GetHBITMAP(0, &Result);
			delete pBmp;
		}

		return Result;
	}

	THandle GDIPlus::LoadImageFromResource(THandle hInst, std::uint32_t dwResId,
		const wchar_t* ResType) noexcept(true)
	{
		// https://code911.top/howto/c-gdi-how-to-get-and-load-image-from-resource

		HBITMAP Result;

		IStream* pStream = nullptr;
		Gdiplus::Bitmap* pBmp = nullptr;
		HGLOBAL hGlobal = nullptr;
		// get the handle to the resource
		HRSRC hrsrc = FindResourceW((HINSTANCE)hInst, MAKEINTRESOURCEW(dwResId), ResType);
		if (hrsrc)
		{
			DWORD dwResourceSize = SizeofResource((HINSTANCE)hInst, hrsrc);
			if (dwResourceSize > 0)
			{
				HGLOBAL hGlobalResource = LoadResource((HINSTANCE)hInst, hrsrc); // load it
				if (hGlobalResource)
				{
					void* imagebytes = LockResource(hGlobalResource); // get a pointer to the file bytes
					// copy image bytes into a real hglobal memory handle
					hGlobal = ::GlobalAlloc(GHND, dwResourceSize);
					if (hGlobal)
					{
						void* pBuffer = ::GlobalLock(hGlobal);
						if (pBuffer)
						{
							memcpy(pBuffer, imagebytes, dwResourceSize);
							HRESULT hr = CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);
							if (SUCCEEDED(hr))
							{
								// pStream now owns the global handle and will invoke GlobalFree on release
								hGlobal = nullptr;
								pBmp = new Gdiplus::Bitmap(pStream);
							}
						}
					}
				}
			}
		}
		if (pStream)
		{
			pStream->Release();
			pStream = nullptr;
		}
		if (hGlobal)
		{
			GlobalFree(hGlobal);
			hGlobal = nullptr;
		}

		if (!pBmp)
			return 0;
		else
		{
			// Passing a transparent background (Color(0,0,0,0)) forces an alpha channel 
			// to be preserved if the HBITMAP format supports it.
			pBmp->GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), &Result);
			delete pBmp;
		}

		return Result;
	}

	THandle GDIPlus::ResizeImage(THandle hBmp, std::int32_t newWidth, std::int32_t newHeight) noexcept(true)
	{
		if (!hBmp || (newWidth <= 0) || (newHeight <= 0))
			return 0;

		// Для проверки корректности HBITMAP
		BITMAP bmInfo;
		if (!GetObjectA((HBITMAP)hBmp, sizeof(BITMAP), (LPSTR)std::addressof(bmInfo)))
			return 0;

		Gdiplus::Bitmap src((HBITMAP)hBmp, nullptr);

		if ((src.GetWidth() <= 0) || (src.GetHeight() <= 0))
			return 0;

		Gdiplus::Bitmap dst(newWidth, newHeight, src.GetPixelFormat());
		Gdiplus::Graphics g(&dst);

		// Критично для альфа-канала: фон не должен влиять на результат
		g.SetInterpolationMode(InterpolationMode::InterpolationModeHighQualityBicubic);
		g.SetPixelOffsetMode(PixelOffsetMode::PixelOffsetModeHalf);
		g.SetCompositingMode(CompositingMode::CompositingModeSourceCopy);
		// полностью прозрачный фон
		g.Clear(Gdiplus::Color(0, 0, 0, 0)); 

		Rect destRect(0, 0, newWidth, newHeight);
		Status st = g.DrawImage(&src, destRect, 0, 0, src.GetWidth(), src.GetHeight(), UnitPixel);
		if (st != Status::Ok)
			return 0;

		HBITMAP Result;
		if (dst.GetHBITMAP(Gdiplus::Color(0, 0, 0, 0), std::addressof(Result)) != Status::Ok)
			return 0;

		return Result;
	}

	//// ---------- Скруглённый прямоугольник ----------
	//GraphicsPath* MakeRoundedRect(const RectF& r, REAL rad)
	//{
	//	GraphicsPath* p = new GraphicsPath();
	//	REAL d = rad * 2;
	//	p->AddArc(r.X, r.Y, d, d, 180, 90);
	//	p->AddArc(r.X + r.Width - d, r.Y, d, d, 270, 90);
	//	p->AddArc(r.X + r.Width - d, r.Y + r.Height - d, d, d, 0, 90);
	//	p->AddArc(r.X, r.Y + r.Height - d, d, d, 90, 90);
	//	p->CloseFigure();
	//	return p;
	//}

	//// ---------- Тень: несколько полупрозрачных слоёв со сдвигом ----------
	//void DrawShadow(Graphics& g, GraphicsPath* body, REAL blur, REAL dx, REAL dy)
	//{
	//	const int layers = 6;
	//	for (int i = layers; i >= 1; --i)
	//	{
	//		REAL grow = blur * i / layers;          // насколько расширяем тень
	//		GraphicsPath* s = body->Clone();
	//		Matrix m;
	//		// расширяем контур и сдвигаем вниз-вправо
	//		m.Translate(dx * i / layers, dy * i / layers);
	//		s->Transform(&m);
	//		GraphicsPath* wider = MakeRoundedRect(
	//			RectF(s->GetBounds().X - grow, s->GetBounds().Y - grow,
	//				s->GetBounds().Width + grow * 2,
	//				s->GetBounds().Height + grow * 2), 14);
	//		SolidBrush b(Color(28 * i / layers, 0, 0, 0)); // альфа ~28..2
	//		g.FillPath(&b, wider);
	//		delete wider;
	//		delete s;
	//	}
	//}

	GDIPlus* GDIPlus::GetSingleton() noexcept(true)
	{
		return &_sgdiplus;
	}

	struct GDIPlusRendererHandle
	{
		HDC src{ nullptr };
		HDC compatible{ nullptr };
		HBITMAP compatibleBitmap{ nullptr };
		HGDIOBJ oldSelectBitmap{ nullptr };
		RECT srcRect{};
		Gdiplus::Graphics* g{ nullptr };
		GDIPlusColor clearColor{};

		~GDIPlusRendererHandle() noexcept(true)
		{
			if (g)
			{
				g = nullptr;
				delete g;
			}

			if (compatible && oldSelectBitmap)
			{
				SelectObject(compatible, oldSelectBitmap);
				oldSelectBitmap = nullptr;
			}

			if (compatibleBitmap)
			{
				DeleteObject(compatibleBitmap);
				compatibleBitmap = nullptr;
			}

			if (compatible)
			{
				DeleteDC(compatible);
				compatible = nullptr;
			}
		}

		constexpr std::int32_t GetSrcWidth() const noexcept(true) { return srcRect.right - srcRect.left; }
		constexpr std::int32_t GetSrcHeight() const noexcept(true) { return srcRect.bottom - srcRect.top; }
	};

	namespace
	{
		static GDIPlusRendererHandle* AsHandle(THandle hRenderer) noexcept(true)
		{
			return reinterpret_cast<GDIPlusRendererHandle*>(hRenderer);
		}

		// ---------- Скруглённый прямоугольник: путь из четырёх дуг ----------
		static Gdiplus::GraphicsPath* CreateRoundedRectPath(const RectF& r, REAL rad) noexcept(true)
		{
			REAL d = rad * 2.f;
			if (d > r.Width) d = r.Width;
			if (d > r.Height) d = r.Height;
			if (d < .0f) d = .0f;

			Gdiplus::GraphicsPath* p = new Gdiplus::GraphicsPath();
			p->AddArc(r.X, r.Y, d, d, 180.f, 90.f);
			p->AddArc(r.X + r.Width - d, r.Y, d, d, 270.f, 90.f);
			p->AddArc(r.X + r.Width - d, r.Y + r.Height - d, d, d, .0f, 90.f);
			p->AddArc(r.X, r.Y + r.Height - d, d, d, 90.f, 90.f);
			p->CloseFigure();
			return p;
		}

		constexpr std::uint8_t GET_R_VALUE(TColor color) noexcept(true)
		{
			return static_cast<uint8_t>((color >> 16) & 0xFF);
		}

		constexpr std::uint8_t GET_G_VALUE(TColor color) noexcept(true)
		{
			return static_cast<uint8_t>((color >> 8) & 0xFF);
		}

		constexpr std::uint8_t GET_B_VALUE(TColor color) noexcept(true)
		{
			return static_cast<uint8_t>(color & 0xFF);
		}
	}


	GDIPlusColor::GDIPlusColor() noexcept(true) :
		r(0), g(0), b(0), a(0)
	{}

	GDIPlusColor::GDIPlusColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept(true) :
		r(r), g(g), b(b), a(a)
	{}

	GDIPlusColor::GDIPlusColor(const GDIPlusColor& color) noexcept(true) :
		r(color.r), g(color.g), b(color.b), a(color.a)
	{}

	GDIPlusColor::GDIPlusColor(TColor color) noexcept(true) :
		r(GET_R_VALUE(color)), g(GET_G_VALUE(color)), b(GET_B_VALUE(color)), a(255)
	{}

	GDIPlusColor& GDIPlusColor::operator=(const GDIPlusColor& color) noexcept(true)
	{
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;

		return *this;
	}

	GDIPlusColor& GDIPlusColor::operator=(TColor color) noexcept(true)
	{
		r = GET_R_VALUE(color);
		g = GET_G_VALUE(color);
		b = GET_B_VALUE(color);
		a = 255;

		return *this;
	}

	GDIPlusColor GDIPlusColor::operator*(float value) noexcept(true)
	{
		return GDIPlusColor(
			static_cast<std::uint8_t>(std::min(static_cast<std::int32_t>(r) * value, 255.f)),
			static_cast<std::uint8_t>(std::min(static_cast<std::int32_t>(g) * value, 255.f)),
			static_cast<std::uint8_t>(std::min(static_cast<std::int32_t>(b) * value, 255.f)));
	}

	GDIPlusColor GDIPlusColor::operator/(float value) noexcept(true)
	{
		return GDIPlusColor(
			static_cast<std::uint8_t>(!r ? 0 : std::max(static_cast<std::int32_t>(r) / value, .0f)),
			static_cast<std::uint8_t>(!g ? 0 : std::max(static_cast<std::int32_t>(g) / value, .0f)),
			static_cast<std::uint8_t>(!b ? 0 : std::max(static_cast<std::int32_t>(b) / value, .0f)));
	}

	GDIPlusColor& GDIPlusColor::operator*=(float value) noexcept(true)
	{
		r = static_cast<std::uint8_t>(std::min(static_cast<std::int32_t>(r) * value, 255.f));
		g = static_cast<std::uint8_t>(std::min(static_cast<std::int32_t>(g) * value, 255.f));
		b = static_cast<std::uint8_t>(std::min(static_cast<std::int32_t>(b) * value, 255.f));

		return *this;
	}

	GDIPlusColor& GDIPlusColor::operator/=(float value) noexcept(true)
	{
		r = static_cast<std::uint8_t>(!r ? 0 : std::max(static_cast<std::int32_t>(r) / value, .0f));
		g = static_cast<std::uint8_t>(!g ? 0 : std::max(static_cast<std::int32_t>(g) / value, .0f));
		b = static_cast<std::uint8_t>(!b ? 0 : std::max(static_cast<std::int32_t>(b) / value, .0f));

		return *this;
	}

	THandle GDIPlusRenderer::DrawBegin(THandle hSrcDC, LPCRECT lpSrcRect) noexcept(true)
	{
		auto handle = new GDIPlusRendererHandle;
		if (!handle)
			return nullptr;

		handle->src = (HDC)hSrcDC;
		handle->compatible = CreateCompatibleDC(handle->src);
		if (!handle->compatible)
		{
			delete handle;
			return nullptr;
		}

		if (!lpSrcRect)
		{
			if (GetClipBox(handle->src, std::addressof(handle->srcRect)) == NULLREGION)
			{
				delete handle;
				return nullptr;
			}
		}
		else
			handle->srcRect = *lpSrcRect;

		handle->compatibleBitmap = CreateCompatibleBitmap(handle->src, handle->GetSrcWidth(), handle->GetSrcHeight());
		if (!handle->compatibleBitmap)
		{
			delete handle;
			return nullptr;
		}

		handle->oldSelectBitmap = SelectObject(handle->compatible, handle->compatibleBitmap);
		handle->g = new Gdiplus::Graphics(handle->compatible);
		if (!handle->g)
		{
			delete handle;
			return nullptr;
		}

		handle->g->SetSmoothingMode(SmoothingModeAntiAlias);

		return handle;
	}

	void GDIPlusRenderer::DrawEnd(THandle hRenderer) noexcept(true)
	{
		auto handle = AsHandle(hRenderer);
		if (!handle)
			return;

		BitBlt(handle->src, handle->srcRect.left, handle->srcRect.top, handle->srcRect.right, handle->srcRect.bottom,
			handle->compatible, 0, 0, SRCCOPY);

		delete handle;
	}

	void GDIPlusRenderer::SetClearColor(THandle hRenderer, std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a) noexcept(true)
	{
		auto handle = AsHandle(hRenderer);
		if (!handle)
			return;

		handle->clearColor = { r, g, b, a };
	}

	void GDIPlusRenderer::Clear(THandle hRenderer) noexcept(true)
	{
		auto handle = AsHandle(hRenderer);
		if (!handle || !handle->g)
			return;

		handle->g->Clear(Gdiplus::Color(handle->clearColor.a, handle->clearColor.r, handle->clearColor.g, handle->clearColor.b));
	}

	void GDIPlusRenderer::FillSolidRect(THandle hRenderer, LPCRECT lpRect,
		const GDIPlusColor& color) noexcept(true)
	{
		auto handle = AsHandle(hRenderer);
		if (!handle || !handle->g || !lpRect)
			return;

		Gdiplus::SolidBrush brush(Gdiplus::Color(color.a, color.r, color.g, color.b));
		handle->g->FillRectangle(std::addressof(brush),
			static_cast<REAL>(lpRect->left),
			static_cast<REAL>(lpRect->top),
			static_cast<REAL>(lpRect->right - lpRect->left),
			static_cast<REAL>(lpRect->bottom - lpRect->top));
	}

	void GDIPlusRenderer::FillRoundedRect(THandle hRenderer, LPCRECT lpRect,
		std::int32_t radius, const GDIPlusColor& color) noexcept(true)
	{
		auto handle = AsHandle(hRenderer);
		if (!handle || !handle->g || !lpRect)
			return;

		RectF rect(
			static_cast<REAL>(lpRect->left),
			static_cast<REAL>(lpRect->top),
			static_cast<REAL>(lpRect->right - lpRect->left),
			static_cast<REAL>(lpRect->bottom - lpRect->top));

		Gdiplus::GraphicsPath* path = CreateRoundedRectPath(rect, static_cast<REAL>(radius));
		if (!path)
			return;

		Gdiplus::SolidBrush brush(Gdiplus::Color(color.a, color.r, color.g, color.b));
		handle->g->FillPath(std::addressof(brush), path);
		delete path;
	}

	void GDIPlusRenderer::FillRoundedRectGradient(THandle hRenderer, LPCRECT lpRect, std::int32_t radius, const GDIPlusColor& top,
		const GDIPlusColor& bottom) noexcept(true)
	{
		auto handle = AsHandle(hRenderer);
		if (!handle || !handle->g || !lpRect)
			return;

		RectF rect(
			static_cast<REAL>(lpRect->left),
			static_cast<REAL>(lpRect->top),
			static_cast<REAL>(lpRect->right - lpRect->left),
			static_cast<REAL>(lpRect->bottom - lpRect->top));

		if (rect.Height <= .0f)
			return;

		Gdiplus::GraphicsPath* path = CreateRoundedRectPath(rect, static_cast<REAL>(radius));
		if (!path)
			return;

		Gdiplus::LinearGradientBrush brush(rect,
			Gdiplus::Color(top.a, top.r, top.g, top.b),
			Gdiplus::Color(bottom.a, bottom.r, bottom.g, bottom.b),
			LinearGradientModeVertical);

		handle->g->FillPath(std::addressof(brush), path);
		delete path;
	}

	void GDIPlusRenderer::FillRoundedRectGradientWithMiddle(THandle hRenderer, LPCRECT lpRect, std::int32_t radius, const GDIPlusColor& top, 
		const GDIPlusColor& middle, float fMiddlePos, const GDIPlusColor& bottom) noexcept(true)
	{
		auto handle = AsHandle(hRenderer);
		if (!handle || !handle->g || !lpRect)
			return;

		RectF rect(
			static_cast<REAL>(lpRect->left),
			static_cast<REAL>(lpRect->top),
			static_cast<REAL>(lpRect->right - lpRect->left),
			static_cast<REAL>(lpRect->bottom - lpRect->top));

		if (rect.Height <= .0f)
			return;

		if (fMiddlePos < .01f) fMiddlePos = .01f;
		if (fMiddlePos > .99f) fMiddlePos = .99f;

		Gdiplus::GraphicsPath* path = CreateRoundedRectPath(rect, static_cast<REAL>(radius));
		if (!path)
			return;

		Gdiplus::LinearGradientBrush brush(rect,
			Gdiplus::Color(0, 0, 0, 0), Gdiplus::Color(0, 0, 0, 0),
			LinearGradientModeVertical);

		Gdiplus::Color colors[3] = {
			Gdiplus::Color(top.a, top.r, top.g, top.b),
			Gdiplus::Color(middle.a, middle.r, middle.g, middle.b),
			Gdiplus::Color(bottom.a, bottom.r, bottom.g, bottom.b)
		};

		REAL positions[3] = { .0f, fMiddlePos, 1.f };
		brush.SetInterpolationColors(colors, positions, 3);

		handle->g->FillPath(std::addressof(brush), path);
		delete path;
	}

	void GDIPlusRenderer::OutlineRoundedRect(THandle hRenderer, LPCRECT lpRect,
		std::int32_t radius, const GDIPlusColor& color, float fWidth) noexcept(true)
	{
		auto handle = AsHandle(hRenderer);
		if (!handle || !handle->g || !lpRect)
			return;

		RectF rect(
			static_cast<REAL>(lpRect->left),
			static_cast<REAL>(lpRect->top),
			static_cast<REAL>(lpRect->right - lpRect->left),
			static_cast<REAL>(lpRect->bottom - lpRect->top));

		Gdiplus::GraphicsPath* path = CreateRoundedRectPath(rect, static_cast<REAL>(radius));
		if (!path)
			return;

		Gdiplus::Pen pen(Gdiplus::Color(color.a, color.r, color.g, color.b), fWidth);
		handle->g->DrawPath(std::addressof(pen), path);
		delete path;
	}

	// ---------- Тень: несколько полупрозрачных слоёв со сдвигом ----------
	// Честного blur в GDI+ нет, поэтому мягкость имитируем слоями:
	// контур расширяется, альфа убывает от внешнего слоя к внутреннему.
	void GDIPlusRenderer::DrawRoundedRectShadow(THandle hRenderer, LPCRECT lpRect,
		std::int32_t radius, const GDIPlusColor& color, std::int32_t blur,
		std::int32_t offsetX, std::int32_t offsetY) noexcept(true)
	{
		auto handle = AsHandle(hRenderer);
		if (!handle || !handle->g || !lpRect || blur <= 0)
			return;

		RectF rect(
			static_cast<REAL>(lpRect->left),
			static_cast<REAL>(lpRect->top),
			static_cast<REAL>(lpRect->right - lpRect->left),
			static_cast<REAL>(lpRect->bottom - lpRect->top));

		Gdiplus::GraphicsPath* body = CreateRoundedRectPath(rect, static_cast<REAL>(radius));
		if (!body)
			return;

		const std::int8_t layers = 6;
		for (std::int8_t i = layers; i >= 1; --i)
		{
			REAL grow = static_cast<REAL>(blur) * i / layers;      // насколько расширяем тень
			REAL shift = static_cast<REAL>(i) / layers;

			RectF bounds;
			body->GetBounds(std::addressof(bounds));

			Gdiplus::GraphicsPath* wider = CreateRoundedRectPath(
				RectF(bounds.X - grow + static_cast<REAL>(offsetX) * shift,
					bounds.Y - grow + static_cast<REAL>(offsetY) * shift,
					bounds.Width + grow * 2.f,
					bounds.Height + grow * 2.f), static_cast<REAL>(radius));

			if (wider)
			{
				Gdiplus::SolidBrush brush(Gdiplus::Color(
					static_cast<std::uint8_t>(color.a * i / layers),
					color.r, color.g, color.b));
				handle->g->FillPath(std::addressof(brush), wider);
				delete wider;
			}
		}

		delete body;
	}

	void GDIPlusRendererHelpers::DrawRoundedSquare(THandle hRenderer, LPCRECT lpRect,
		std::int32_t radius,
		const GDIPlusColor& bodyGradientTop,
		const GDIPlusColor& bodyGradientBottom,
		const GDIPlusColor& innerRing,
		const GDIPlusColor& background,
		const GDIPlusColor& highlightTop,
		const GDIPlusColor& highlightBottom,
		std::int32_t glowWidth,
		std::int32_t innerRingWidth) noexcept(true)
	{
		auto handle = AsHandle(hRenderer);
		if (!handle || !handle->g || !lpRect)
			return;

		if (((lpRect->right - lpRect->left) <= 0) || ((lpRect->bottom - lpRect->top) <= 0))
			return;

		// Внешняя подсветка: расширенный контур заливаем градиентом
		// "темнее фона -> фон -> светлее фона". Тело, нарисованное поверх,
		// скроет внутреннюю часть, останется только кольцо снаружи.
		if (glowWidth > 0 && (highlightTop.a || highlightBottom.a))
		{
			RECT rcGlow{ lpRect->left - glowWidth, lpRect->top - glowWidth,
				lpRect->right + glowWidth, lpRect->bottom + glowWidth };

			// радиус растим вместе с контуром, чтобы углы оставались концентричными
			GDIPlusRenderer::FillRoundedRectGradientWithMiddle(hRenderer, std::addressof(rcGlow),
				radius + glowWidth, highlightTop, background, .5f, highlightBottom);
		}

		GDIPlusRenderer::FillRoundedRectGradient(hRenderer, lpRect, radius,
			bodyGradientTop, bodyGradientBottom);

		// Тёмное кольцо ВНУТРИ кромки: обводки слоями с затуханием вглубь
		if (innerRingWidth > 0 && innerRing.a)
		{
			for (std::int32_t i = 0; i < innerRingWidth; ++i)
			{
				RECT rc{ lpRect->left + i, lpRect->top + i,
					lpRect->right - i, lpRect->bottom - i };

				GDIPlusColor c = innerRing;
				c.a = (std::uint8_t)((std::uint32_t)innerRing.a *
					(innerRingWidth - i) / innerRingWidth);
				GDIPlusRenderer::OutlineRoundedRect(hRenderer, std::addressof(rc), radius, c, 1.f);
			}
		}


		//GDIPlusRenderer::FillRoundedRectGradient(hRenderer, lpRect, radius, frameGradientTop, frameGradientBottom);
		//RECT rcInner{ lpRect->left + inset, lpRect->top + inset, lpRect->right - inset, lpRect->bottom - inset };

		//// Подсветка нижней кромки: сначала заливаем цветом подсветки,
		//// затем градиент с подъеденным низом оставляет 1px светлой полоски
		//GDIPlusRenderer::FillRoundedRect(hRenderer, std::addressof(rcInner), radius, innerHighlight);

		//RECT rcGrad = rcInner;
		//rcGrad.bottom -= 1;
		//GDIPlusRenderer::FillRoundedRectGradient(hRenderer, std::addressof(rcGrad), radius, innerGradientTop, innerGradientBottom);

		//// Внутренняя тень: слои обводки с убывающей альфой от кромки вглубь — сильнее у края, мягко тает
		//if ((innerShadowDepth > 0) && innerShadow.a)
		//{
		//	for (std::int32_t i = innerShadowDepth; i >= 1; --i)
		//	{
		//		RECT rc{ 
		//			rcInner.left + i - 1,	rcInner.top + i - 1,
		//			rcInner.right - i + 1,	rcInner.bottom - i + 1 };

		//		GDIPlusColor c = innerShadow;
		//		c.a = (std::uint8_t)((std::uint32_t)innerShadow.a * i / innerShadowDepth);
		//		GDIPlusRenderer::OutlineRoundedRect(hRenderer, std::addressof(rc), radius, c, 1.0f);
		//	}
		//}

		//// Внешняя окантовка
		//GDIPlusRenderer::OutlineRoundedRect(hRenderer, lpRect, radius, frameOutline, 1.0f);
	}
}