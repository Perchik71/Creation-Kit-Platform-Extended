// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <CKPE.Stream.h>
#include <CKPE.Exception.h>
#include <CKPE.Graphics.h>
#include <CKPE.GDIPlus.h>

namespace CKPE
{
	static void __imGradientFill_V(HDC hdc, const TColor start_color, const TColor end_color, const LPRECT rc)
	{
		GRADIENT_RECT rects = { 0, 1 };
		TRIVERTEX vertices[2];
		ZeroMemory(&vertices, sizeof(TRIVERTEX) << 1);

		vertices[0].Red = ((COLOR16)GetRValue(start_color)) << 8;
		vertices[0].Green = ((COLOR16)GetGValue(start_color)) << 8;
		vertices[0].Blue = ((COLOR16)GetBValue(start_color)) << 8;
		vertices[0].x = rc->left;
		vertices[0].y = rc->top;

		vertices[1].Red = ((COLOR16)GetRValue(end_color)) << 8;
		vertices[1].Green = ((COLOR16)GetGValue(end_color)) << 8;
		vertices[1].Blue = ((COLOR16)GetBValue(end_color)) << 8;
		vertices[1].x = rc->right;
		vertices[1].y = rc->bottom;

		::GradientFill(hdc, vertices, 2, &rects, 1, GRADIENT_FILL_RECT_V);
	}

	static void __imGradientFill_H(HDC hdc, const TColor start_color, const TColor end_color, const LPRECT rc)
	{
		GRADIENT_RECT rects = { 0, 1 };
		TRIVERTEX vertices[2];
		ZeroMemory(&vertices, sizeof(TRIVERTEX) << 1);

		vertices[0].Red = ((COLOR16)GetRValue(start_color)) << 8;
		vertices[0].Green = ((COLOR16)GetGValue(start_color)) << 8;
		vertices[0].Blue = ((COLOR16)GetBValue(start_color)) << 8;
		vertices[0].x = rc->left;
		vertices[0].y = rc->top;

		vertices[1].Red = ((COLOR16)GetRValue(end_color)) << 8;
		vertices[1].Green = ((COLOR16)GetGValue(end_color)) << 8;
		vertices[1].Blue = ((COLOR16)GetBValue(end_color)) << 8;
		vertices[1].x = rc->right;
		vertices[1].y = rc->bottom;

		::GradientFill(hdc, vertices, 2, &rects, 1, GRADIENT_FILL_RECT_H);
	}

	// ObjectGUI

	void ObjectGUI::Release() noexcept(true)
	{
		if (!m_fHandle)
			return;

		if (DeleteObject((HGDIOBJ)m_fHandle))
			m_fHandle = nullptr;
	}

	void ObjectGUI::DoChange() noexcept(true)
	{
		if (OnChange)
			OnChange(this, Canvas);
	}

	THandle ObjectGUI::Select(const _Canvas& canvas) const noexcept(true)
	{
		return (THandle)SelectObject((HDC)canvas.Handle, m_fHandle);
	}

	THandle ObjectGUI::Select() const noexcept(true)
	{
		if (Canvas)
			return (THandle)SelectObject((HDC)Canvas->Handle, m_fHandle);
		return m_fHandle;
	}

	ObjectGUI::~ObjectGUI() noexcept(true)
	{
		Release();
	}

	// Bitmap

	// https://stackoverflow.com/questions/24720451/save-hbitmap-to-bmp-file-using-only-win32
	static PBITMAPINFO __imCreateBitmapInfoStruct(HBITMAP hBmp)
	{
		BITMAP bmp = { 0 };
		PBITMAPINFO pbmi;
		WORD cClrBits;

		// Retrieve the bitmap color format, width, and height.  
		GetObjectA(hBmp, sizeof(BITMAP), (LPSTR)&bmp);

		// Convert the color format to a count of bits.  
		cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
		if (cClrBits == 1)
			cClrBits = 1;
		else if (cClrBits <= 4)
			cClrBits = 4;
		else if (cClrBits <= 8)
			cClrBits = 8;
		else if (cClrBits <= 16)
			cClrBits = 16;
		else if (cClrBits <= 24)
			cClrBits = 24;
		else cClrBits = 32;

		// Allocate memory for the BITMAPINFO structure. (This structure  
		// contains a BITMAPINFOHEADER structure and an array of RGBQUAD  
		// data structures.)  

		if (cClrBits < 24)
			pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
				sizeof(BITMAPINFOHEADER) +
				sizeof(RGBQUAD) * ((size_t)1 << cClrBits));

		// There is no RGBQUAD array for these formats: 24-bit-per-pixel or 32-bit-per-pixel 

		else
			pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
				sizeof(BITMAPINFOHEADER));

		// Initialize the fields in the BITMAPINFO structure.  

		pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pbmi->bmiHeader.biWidth = bmp.bmWidth;
		pbmi->bmiHeader.biHeight = bmp.bmHeight;
		pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
		pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
		if (cClrBits < 24)
			pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

		// If the bitmap is not compressed, set the BI_RGB flag.  
		pbmi->bmiHeader.biCompression = BI_RGB;

		// Compute the number of bytes in the array of color  
		// indices and store the result in biSizeImage.  
		// The width must be DWORD aligned unless the bitmap is RLE 
		// compressed. 
		pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
			* pbmi->bmiHeader.biHeight;
		// Set biClrImportant to 0, indicating that all of the  
		// device colors are important.  
		pbmi->bmiHeader.biClrImportant = 0;
		return pbmi;
	}

	static void __imCreateBMPFile(Stream& stream, HBITMAP hBMP)
	{
		BITMAPFILEHEADER hdr = { 0 };   // bitmap file-header  
		PBITMAPINFOHEADER pbih;			// bitmap info-header  
		LPBYTE lpBits;					// memory pointer  
		std::uint32_t dwTotal;			// total count of bytes  
		std::uint32_t cb;				// incremental count of bytes  
		BYTE* hp;						// byte pointer  
		DWORD dwTmp = 0;
		PBITMAPINFO pbi;
		HDC hDC;

		hDC = CreateCompatibleDC(GetWindowDC(GetDesktopWindow()));
		SelectObject(hDC, hBMP);

		pbi = __imCreateBitmapInfoStruct(hBMP);

		pbih = (PBITMAPINFOHEADER)pbi;
		lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);
		if (lpBits)
		{
			// Retrieve the color table (RGBQUAD array) and the bits  
			// (array of palette indices) from the DIB.  
			GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
				DIB_RGB_COLORS);

			try
			{
				hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
				// Compute the size of the entire file.  
				hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
					pbih->biSize + pbih->biClrUsed
					* sizeof(RGBQUAD) + pbih->biSizeImage);
				hdr.bfReserved1 = 0;
				hdr.bfReserved2 = 0;

				// Compute the offset to the array of color indices.  
				hdr.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD));

				stream.Write(&hdr, (std::uint32_t)sizeof(BITMAPFILEHEADER));
				stream.Write(pbih, (std::uint32_t)(sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD)));

				// Copy the array of color indices into the .BMP file.  
				dwTotal = cb = (std::uint32_t)pbih->biSizeImage;
				hp = lpBits;

				stream.Write(hp, cb);
			}
			catch (const std::exception& e)
			{
				ErrorHandler::Trigger(e.what());
			}

			// Free memory.  
			GlobalFree((HGLOBAL)lpBits);
		}
	}

	void Bitmap::Create(std::int32_t width, std::int32_t height, std::int32_t bpp)
	{
		if ((width > 0) && (height > 0) && ((bpp == 24) || (bpp == 32)))
		{
			m_fHandle = CreateBitmap(width, height, 1, bpp, nullptr);
			DoChange();
		}
		else
			throw RuntimeError("Bitmap::Create Invalid args");
	}

	void Bitmap::Create(const std::wstring& fname) noexcept(true)
	{
		m_fHandle = GDIPlus::LoadImageFromFile(fname);
		DoChange();
	}

	void Bitmap::Create(THandle hinst, std::uint32_t dwResId, const std::wstring& name) noexcept(true)
	{
		m_fHandle = GDIPlus::LoadImageFromResource(hinst, dwResId, name);
		DoChange();
	}

	void Bitmap::FreeImage() noexcept(true)
	{
		Release();
		DoChange();
	}

	bool Bitmap::LoadFromResource(THandle hinst, std::uint32_t dwResId, const std::wstring& name) noexcept(true)
	{
		Release();
		m_fHandle = GDIPlus::LoadImageFromResource(hinst, dwResId, name);
		DoChange();
		return true;
	}

	bool Bitmap::LoadFromStream(Stream& stream) noexcept(true)
	{
		Release();
		m_fHandle = GDIPlus::LoadImageFromStream(stream);
		DoChange();
		return true;
	}

	bool Bitmap::SaveToStream(Stream& stream) const noexcept(true)
	{
		if (!Empty())
			__imCreateBMPFile(stream, (HBITMAP)m_fHandle);
		return true;
	}

	void Bitmap::Assign(const Bitmap& bitmap) noexcept(true)
	{
		Release();
		if (!bitmap.Empty())
			m_fHandle = CopyImage(bitmap.Handle, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
		DoChange();
	}

	Bitmap& Bitmap::operator=(const Bitmap& bitmap) noexcept(true)
	{
		Assign(bitmap);
		return *this;
	}

	Bitmap::Bitmap() noexcept(true) : ObjectGUI(1), FileStreamIntf() 
	{}

	Bitmap::Bitmap(THandle bitmap) noexcept(true) : ObjectGUI(1), FileStreamIntf()
	{
		if (bitmap)
			m_fHandle = CopyImage(bitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	}

	Bitmap::Bitmap(std::int32_t width, std::int32_t height, std::int32_t bpp) : ObjectGUI(1), FileStreamIntf()
	{
		Create(width, height, bpp);
	}

	Bitmap::Bitmap(const std::wstring& fname) noexcept(true) : ObjectGUI(1), FileStreamIntf()
	{ 
		Create(fname); 
	}

	Bitmap::Bitmap(THandle hinst, std::uint32_t dwResId, const std::wstring& name) noexcept(true) : ObjectGUI(1), FileStreamIntf()
	{
		Create(hinst, dwResId, name);
	}

	Bitmap::Bitmap(const Bitmap& bitmap) noexcept(true) : ObjectGUI(bitmap), FileStreamIntf()
	{
		if (!bitmap.Empty())
			m_fHandle = CopyImage(bitmap.Handle, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
	}

	// Pen

	void Pen::Create(PenStyle style, std::int32_t width, TColor color) noexcept(true)
	{
		std::uint32_t flag = 0;

		switch (style)
		{
		case psSolid:
			flag = PS_SOLID;
			break;
		case psDash:
			flag = PS_DASH;
			break;
		case psDot:
			flag = PS_DOT;
			break;
		case psDashDot:
			flag = PS_DASHDOT;
			break;
		case psDashDotDot:
			flag = PS_DASHDOTDOT;
			break;
		case psInsideFrame:
			flag = PS_INSIDEFRAME;
			break;
		default:
			flag = PS_NULL;
			break;
		}

		m_fHandle = CreatePen(flag, width, color);
		m_fStyle = style;
		m_fSize = width;
		m_fColor = color;

		DoChange();
	}

	void Pen::SetWidth(const std::int32_t width) noexcept(true)
	{
		Release();
		Create(Style, (width > 0) ? width : 1, Color);
	}

	void Pen::SetColor(const TColor color) noexcept(true)
	{
		Release();
		Create(Style, Width, color);
	}

	void Pen::SetStyle(const PenStyle style) noexcept(true)
	{
		Release();
		Create(style, Width, Color);
	}

	void Pen::Assign(const Pen& pen) noexcept(true)
	{
		Release();
		Create(pen.Style, pen.Width, pen.Color);
	}

	Pen& Pen::operator=(const Pen& pen)
	{
		Assign(pen);
		return *this;
	}

	CKPE_API Pen CreateSolidPen(std::int32_t width, TColor color) noexcept(true)
	{
		return Pen(psSolid, width, color);
	}

	CKPE_API Pen CreateDashPen(std::int32_t width, TColor color) noexcept(true)
	{
		return Pen(psDash, width, color);
	}

	CKPE_API Pen CreateDotPen(std::int32_t width, TColor color) noexcept(true)
	{
		return Pen(psDot, width, color);
	}

	// Brush

	void Brush::Assign(const Brush& brush) noexcept(true)
	{
		Release();

		switch (brush.Style)
		{
		case bsClear:
		{
			m_fStyle = bsClear;
			DoChange();
		}
		break;
		case bsSolid:
			Create(brush.Color);
			break;
		case bsHatch:
			Create(brush.Hatch, brush.Color);
			break;
		case bsBitmap:
			Create(brush.Bitmap);
			break;
		}
	}

	void Brush::Create(const TColor color) noexcept(true)
	{
		m_fHandle = ::CreateSolidBrush(color);
		m_fColor = color;
		m_fStyle = bsSolid;
		DoChange();
	}

	void Brush::Create(const std::int32_t iHatch, const TColor color) noexcept(true)
	{
		m_fHandle = ::CreateHatchBrush(iHatch, color);
		m_fHatch = iHatch;
		m_fColor = color;
		m_fStyle = bsHatch;
		DoChange();
	}

	void Brush::Create(const _Bitmap& bitmap) noexcept(true)
	{
		m_fBitmap.Assign(bitmap);
		m_fHandle = ::CreatePatternBrush((HBITMAP)m_fBitmap.Handle);
		m_fStyle = bsBitmap;
		DoChange();
	}

	void Brush::SetHatch(const std::int32_t value) noexcept(true)
	{
		if (m_fStyle == BrushStyle::bsHatch)
		{
			Release();
			Create(value, m_fColor);
		}
		else m_fHatch = value;
	}

	void Brush::SetColor(const TColor color) noexcept(true)
	{
		if ((m_fStyle == BrushStyle::bsHatch) || (m_fStyle == BrushStyle::bsSolid))
		{
			Release();
			if (m_fStyle == BrushStyle::bsHatch)
				Create(m_fHatch, color);
			else
				Create(color);
		}
		else m_fColor = color;
	}

	void Brush::SetStyle(const BrushStyle style) noexcept(true)
	{
		if (style == m_fStyle)
			return;

		if (m_fStyle == bsBitmap)
			m_fBitmap.FreeImage();

		Release();

		switch (style)
		{
		case BrushStyle::bsBitmap:
			/* bitmap empty */
			break;
		case BrushStyle::bsHatch:
			Create(m_fHatch, m_fColor);
			break;
		case BrushStyle::bsClear:
		{
			m_fStyle = bsClear;
			DoChange();
		}
		break;
		default:
			Create(m_fColor);
			break;
		}
	}

	void Brush::SetBitmap(const _Bitmap& bitmap) noexcept(true)
	{
		Release();
		Create(bitmap);
	}

	Brush::Brush(const Brush& brush) : ObjectGUI(brush)
	{
		switch (brush.Style)
		{
		case bsClear:
			m_fStyle = bsClear;
			break;
		case bsSolid:
			Create(brush.Color);
			break;
		case bsHatch:
			Create(brush.Hatch, brush.Color);
			break;
		case bsBitmap:
			Create(brush.Bitmap);
			break;
		}
	}

	Brush& Brush::operator=(const Brush& brush)
	{
		Assign(brush);
		return *this;
	}

	CKPE_API Brush CreateSolidBrush(const TColor color) noexcept(true)
	{
		return color;
	}

	CKPE_API Brush CreatePatternBrush(const Bitmap& bitmap) noexcept(true)
	{
		return bitmap;
	}

	CKPE_API Brush CreateHatchBrush(const std::int32_t iHatch, const TColor color) noexcept(true)
	{
		return Brush(iHatch, color);
	}

	CKPE_API bool CreateGradientBrush(Brush& brush, const TColor start_color, const TColor end_color,
		const std::int32_t size, const CUIGradientDirect direct) noexcept(true)
	{
		// I'm just surprised at people who like to reduce everything in the world......
		//Assert(size > 0);
		auto nsize = std::max(size, 5);
		bool res = false;

		HWND hWnd = GetDesktopWindow();
		HDC hDC = GetDC(hWnd);
		HDC hDCMem = CreateCompatibleDC(hDC);

		RECT rc;
		memset(&rc, 0, sizeof(RECT));

		if (direct == gdHorz)
		{
			rc.right = nsize;
			rc.bottom = 1;
		}
		else
		{
			rc.right = 1;
			rc.bottom = nsize;
		}

		if (auto hBitmap = CreateCompatibleBitmap(hDC, rc.right, rc.bottom))
		{
			SelectObject(hDCMem, hBitmap);

			if (direct == gdHorz)
				__imGradientFill_H(hDCMem, start_color, end_color, (LPRECT)&rc);
			else
				__imGradientFill_V(hDCMem, start_color, end_color, (LPRECT)&rc);

			brush.Bitmap = hBitmap;
			res = true;
		}

		DeleteDC(hDCMem);
		ReleaseDC(hWnd, hDC);

		return res;
	}

	// Font

	Font::Font(const THandle hDC) noexcept(true) : ObjectGUI(4), m_lock(false), m_Name(new std::string)
	{
		m_FontStyles = new FontStyles();
		Recreate(hDC);
	}

	Font::Font(const std::string& name, const std::int32_t size, const FontStyles& styles,
		const std::uint32_t ulCharSet, const FontQuality quality, const FontPitch pitch) noexcept(true) :
		ObjectGUI(4), m_lock(false), m_ulCharSet(ulCharSet),
		m_Name(new std::string(name)), m_Quality(quality), m_Pitch(pitch)
	{
		m_FontStyles = new FontStyles();
		*m_FontStyles = styles;
		Size = size;
	}

	void Font::Recreate(const THandle hDC) noexcept(true)
	{
		TEXTMETRICA metric;
		GetTextMetricsA((HDC)hDC, &metric);
		m_Name->resize(MAX_PATH);
		m_Name->resize((size_t)GetTextFaceA((HDC)hDC, MAX_PATH + 1, m_Name->data()) + 1);

		m_Quality = fqClearTypeNatural;

		if (metric.tmItalic)
			m_FontStyles->insert(fsItalic);
		if (metric.tmUnderlined)
			m_FontStyles->insert(fsUnderline);
		if (metric.tmWeight >= FW_BOLD)
			m_FontStyles->insert(fsBold);

		m_Pitch = fpDefault;
		m_Height = metric.tmHeight;

		Recreate();
	}

	void Font::SetStyles(const FontStyles& styles) noexcept(true)
	{
		*m_FontStyles = styles;
		Change();
	}

	void Font::SetName(const std::string& name) noexcept(true)
	{
		*m_Name = name;
		Change();
	}

	std::int32_t Font::GetSize() const noexcept(true)
	{
		HDC hDC = GetDC(0);
		LONG res = -MulDiv(m_Height, 72, GetDeviceCaps(hDC, LOGPIXELSY));
		ReleaseDC(0, hDC);
		return res;
	}

	void Font::SetSize(const std::int32_t value) noexcept(true)
	{
		HDC hDC = GetDC(0);
		m_Height = -MulDiv(value, GetDeviceCaps(hDC, LOGPIXELSY), 72);
		ReleaseDC(0, hDC);
		Change();
	}

	void Font::SetCharSet(const std::uint32_t CharSet) noexcept(true)
	{
		m_ulCharSet = CharSet;
		Change();
	}

	void Font::SetHeight(const std::int32_t value) noexcept(true)
	{
		m_Height = value;
		Change();
	}

	void Font::Assign(const Font& font) noexcept(true)
	{
		m_lock = TRUE;

		this->SetName(font.GetName());
		this->Size = font.Size;
		this->Pitch = font.Pitch;
		this->SetStyles(font.GetStyles());

		m_lock = FALSE;

		Change();
	}

	Font& Font::operator=(const Font& font)
	{
		Assign(font);
		return *this;
	}

	void Font::Apply(THandle window) const noexcept(true)
	{
		SendMessageA((HWND)window, WM_SETFONT, (WPARAM)m_fHandle, 0);
	}

	void Font::SetQuality(const FontQuality quality) noexcept(true)
	{
		m_Quality = quality;
		Change();
	}

	void Font::SetPitch(const FontPitch pitch) noexcept(true)
	{
		m_Pitch = pitch;
		Change();
	}

	void Font::Recreate() noexcept(true)
	{
		DWORD _Quality = DEFAULT_QUALITY;
		DWORD _Pitch = DEFAULT_PITCH;

		switch (m_Quality)
		{
		case fqClearType:
			_Quality = CLEARTYPE_QUALITY;
			break;
		case fqClearTypeNatural:
			_Quality = CLEARTYPE_NATURAL_QUALITY;
			break;
		case fqProof:
			_Quality = PROOF_QUALITY;
			break;
		case fqAntialiased:
			_Quality = NONANTIALIASED_QUALITY;
			break;
		case fqNoAntialiased:
			_Quality = ANTIALIASED_QUALITY;
			break;
		}

		switch (m_Pitch)
		{
		case fpFixed:
			_Pitch = FIXED_PITCH;
			break;
		case fpVariable:
			_Pitch = VARIABLE_PITCH;
			break;
		case fpMono:
			_Pitch = MONO_FONT;
			break;
		}

		if (m_fHandle)
			DeleteObject(m_fHandle);

		m_fHandle = CreateFontA(Height, 0, 0, 0,
			(int)m_FontStyles->count(fsBold) ? FW_BOLD : FW_NORMAL,
			(int)m_FontStyles->count(fsItalic),
			(int)m_FontStyles->count(fsUnderline),
			(int)m_FontStyles->count(fsStrikeOut),
			m_ulCharSet, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			_Quality,
			_Pitch,
			m_Name->c_str());
	}

	void Font::Change() noexcept(true)
	{
		if (m_lock)
			return;
		
		Recreate();
		DoChange();
	}

	// Canvas

	void Canvas::SetPixel(std::int32_t x, std::int32_t y, const TColor color) const noexcept(true)
	{
		::SetPixel((HDC)m_hDC, x, y, color);
	}

	void Canvas::SetPixel(const POINT& p, const TColor color) const noexcept(true)
	{
		::SetPixel((HDC)m_hDC, p.x, p.y, color);
	}

	void Canvas::SetPixel(const CPOINT& p, const TColor color) const noexcept(true)
	{
		::SetPixel((HDC)m_hDC, p.X, p.Y, color);
	}

	void Canvas::SetBrush(const _Brush& brush) noexcept(true)
	{
		m_fBrush.Assign(brush);
	}

	void Canvas::SetPen(const _Pen& pen) noexcept(true)
	{
		m_fPen.Assign(pen);
	}

	void Canvas::SetFont(const _Font& font) noexcept(true)
	{
		m_fFont.Assign(font);
	}

	void Canvas::MoveTo(std::int32_t x, std::int32_t y) const noexcept(true)
	{
		::MoveToEx((HDC)m_hDC, x, y, nullptr);
	}

	void Canvas::MoveTo(const POINT& p) const noexcept(true)
	{
		MoveTo(p.x, p.y);
	}

	void Canvas::MoveTo(const CPOINT& p) const noexcept(true)
	{
		MoveTo(p.X, p.Y);
	}

	void Canvas::LineTo(std::int32_t x, std::int32_t y) const noexcept(true)
	{
		::LineTo((HDC)m_hDC, x, y);
	}

	void Canvas::LineTo(const POINT& p) const noexcept(true)
	{
		LineTo(p.x, p.y);
	}

	void Canvas::LineTo(const CPOINT& p) const noexcept(true)
	{
		LineTo(p.X, p.Y);
	}

	void Canvas::RoundRect(const RECT& area, const std::int32_t w, const std::int32_t h) const noexcept(true)
	{
		::RoundRect((HDC)m_hDC, area.left, area.top, area.right, area.bottom, w, h);
	}

	void Canvas::RoundRect(const CRECT& area, const std::int32_t w, const std::int32_t h) const noexcept(true)
	{
		::RoundRect((HDC)m_hDC, area.Left, area.Top, area.Right, area.Bottom, w, h);
	}

	void Canvas::Ellipse(const RECT& area) const noexcept(true)
	{
		::Ellipse((HDC)m_hDC, area.left, area.top, area.right, area.bottom);
	}

	void Canvas::Ellipse(const CRECT& area) const noexcept(true)
	{
		::Ellipse((HDC)m_hDC, area.Left, area.Top, area.Right, area.Bottom);
	}

	void Canvas::Rectangle(const RECT& area) const noexcept(true)
	{
		::Rectangle((HDC)m_hDC, area.left, area.top, area.right, area.bottom);
	}

	void Canvas::Rectangle(const CRECT& area) const noexcept(true)
	{
		::Rectangle((HDC)m_hDC, area.Left, area.Top, area.Right, area.Bottom);
	}

	void Canvas::Fill(const TColor color) noexcept(true)
	{
		RECT rc;
		GetClipBox((HDC)m_hDC, &rc);
		Fill(rc, color);
	}

	void Canvas::Fill(const RECT& area, const TColor color) noexcept(true)
	{
		m_fBrush.Color = color;
		m_fBrush.Style = bsSolid;
		FillRect((HDC)m_hDC, &area, (HBRUSH)m_fBrush.Handle);
	}

	void Canvas::Fill(const CRECT& area, const TColor color) noexcept(true)
	{
		m_fBrush.Color = color;
		m_fBrush.Style = bsSolid;
		FillRect((HDC)m_hDC, (LPRECT)&area, (HBRUSH)m_fBrush.Handle);
	}

	void Canvas::FillWithTransparent(const RECT& area, const TColor color, std::uint8_t percent) noexcept(true)
	{
		HDC hMemDC = ::CreateCompatibleDC((HDC)m_hDC);
		if (!hMemDC) return;

		int w = area.right - area.left;
		int h = area.bottom - area.top;

		HBITMAP hMemBmp = ::CreateCompatibleBitmap((HDC)m_hDC, w, h);
		if (!hMemBmp)
		{
			DeleteDC(hMemDC);
			return;
		}

		HBRUSH hMemBrush = ::CreateSolidBrush(color);
		CRECT clipRect = CRECT(0, 0, w, h);

		SelectObject(hMemDC, hMemBmp);
		FillRect(hMemDC, (LPRECT)&clipRect, hMemBrush);

		BLENDFUNCTION bf = { 0 };

		float p = std::min(percent, (BYTE)100);

		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = (BYTE)(255 * (p / 100));   // Range from 0 to 255
		bf.AlphaFormat = AC_SRC_OVER;

		AlphaBlend((HDC)m_hDC,
			area.left,
			area.top,
			area.right - area.left,
			area.bottom - area.top,
			hMemDC,
			0,
			0,
			clipRect.Width,
			clipRect.Height,
			bf);

		DeleteObject(hMemBrush);
		DeleteObject(hMemBmp);
		DeleteDC(hMemDC);
	}

	void Canvas::FillWithTransparent(const CRECT& area, const TColor color, std::uint8_t percent) noexcept(true)
	{
		HDC hMemDC = ::CreateCompatibleDC((HDC)m_hDC);
		if (!hMemDC) return;

		int w = area.Width;
		int h = area.Height;

		HBITMAP hMemBmp = ::CreateCompatibleBitmap((HDC)m_hDC, w, h);
		if (!hMemBmp)
		{
			DeleteDC(hMemDC);
			return;
		}

		HBRUSH hMemBrush = ::CreateSolidBrush(color);
		CRECT clipRect = CRECT(0, 0, w, h);

		SelectObject(hMemDC, hMemBmp);
		FillRect(hMemDC, (LPRECT)&clipRect, hMemBrush);

		BLENDFUNCTION bf = { 0 };

		float p = std::min(percent, (BYTE)100);

		bf.BlendOp = AC_SRC_OVER;
		bf.BlendFlags = 0;
		bf.SourceConstantAlpha = (BYTE)(255 * (p / 100));   // Range from 0 to 255
		bf.AlphaFormat = AC_SRC_OVER;

		AlphaBlend((HDC)m_hDC,
			area.Left,
			area.Top,
			area.Width,
			area.Height,
			hMemDC,
			0,
			0,
			clipRect.Width,
			clipRect.Height,
			bf);

		DeleteObject(hMemBrush);
		DeleteObject(hMemBmp);
		DeleteDC(hMemDC);
	}

	void Canvas::Fill(LPCRECT area, const std::int32_t nCount, const TColor color) noexcept(true)
	{
		m_fBrush.Color = color;
		m_fBrush.Style = bsSolid;

		for (INT i = 0; i < nCount; i++)
			FillRect((HDC)m_hDC, &area[i], (HBRUSH)m_fBrush.Handle);
	}

	void Canvas::Fill(const LPCCRECT area, const std::int32_t nCount, const TColor color) noexcept(true)
	{
		m_fBrush.Color = color;
		m_fBrush.Style = bsSolid;

		for (INT i = 0; i < nCount; i++)
			FillRect((HDC)m_hDC, (LPCRECT)&area[i], (HBRUSH)m_fBrush.Handle);
	}

	void Canvas::EllipseFill(const RECT& area, const TColor color) noexcept(true)
	{
		m_fPen.Style = psClear;
		m_fBrush.Color = color;
		m_fBrush.Style = bsSolid;

		::Ellipse((HDC)m_hDC, area.left, area.top, area.right, area.bottom);
	}

	void Canvas::EllipseFill(const CRECT& area, const TColor color) noexcept(true)
	{
		m_fPen.Style = psClear;
		m_fBrush.Color = color;
		m_fBrush.Style = bsSolid;

		::Ellipse((HDC)m_hDC, area.Left, area.Top, area.Right, area.Bottom);
	}

	void Canvas::GradientFill(const RECT& area, const TColor start_color, const TColor end_color,
		const CUIGradientDirect direct) noexcept(true)
	{
		if (direct == gdHorz)
			__imGradientFill_H((HDC)m_hDC, start_color, end_color, (LPRECT)&area);
		else
			__imGradientFill_V((HDC)m_hDC, start_color, end_color, (LPRECT)&area);
	}

	void Canvas::GradientFill(const CRECT& area, const TColor start_color, const TColor end_color,
		const CUIGradientDirect direct) noexcept(true)
	{
		if (direct == gdHorz)
			__imGradientFill_H((HDC)m_hDC, start_color, end_color, (LPRECT)&area);
		else
			__imGradientFill_V((HDC)m_hDC, start_color, end_color, (LPRECT)&area);
	}

	void Canvas::Frame(const TColor color) noexcept(true)
	{
		RECT rc;
		GetClipBox((HDC)m_hDC, &rc);
		Frame(rc, color);
	}

	void Canvas::Frame(const RECT& area, const TColor color) noexcept(true)
	{
		m_fBrush.Color = color;
		m_fBrush.Style = bsSolid;
		FrameRect((HDC)m_hDC, &area, (HBRUSH)m_fBrush.Handle);
	}

	void Canvas::Frame(const CRECT& area, const TColor color) noexcept(true)
	{
		m_fBrush.Color = color;
		m_fBrush.Style = bsSolid;
		FrameRect((HDC)m_hDC, (LPRECT)&area, (HBRUSH)m_fBrush.Handle);
	}

	void Canvas::GradientFrame(const RECT& area, const TColor start_color, const TColor end_color,
		const CUIGradientDirect direct) noexcept(true)
	{
		if (direct == gdHorz)
			if (CreateGradientBrush(Brush, start_color, end_color, area.right - area.left, direct))
				FrameRect((HDC)m_hDC, (LPRECT)&area, (HBRUSH)Brush.Handle);
			else
				if (CreateGradientBrush(Brush, start_color, end_color, area.bottom - area.top, direct))
					FrameRect((HDC)m_hDC, (LPRECT)&area, (HBRUSH)Brush.Handle);
	}

	void Canvas::GradientFrame(const CRECT& area, const TColor start_color, const TColor end_color,
		const CUIGradientDirect direct) noexcept(true)
	{
		if (direct == gdHorz)
			if (CreateGradientBrush(Brush, start_color, end_color, area.Width, direct))
				FrameRect((HDC)m_hDC, (LPRECT)&area, (HBRUSH)Brush.Handle);
			else
				if (CreateGradientBrush(Brush, start_color, end_color, area.Height, direct))
					FrameRect((HDC)m_hDC, (LPRECT)&area, (HBRUSH)Brush.Handle);
	}

	void Canvas::Polygon(const LPPOINT ps, std::int32_t count) const noexcept(true)
	{
		::Polygon((HDC)m_hDC, ps, count);
	}

	void Canvas::Polygon(const std::vector<POINT>& p) const noexcept(true)
	{
		::Polygon((HDC)m_hDC, p.data(), (int)p.size());
	}

	void Canvas::Polygon(const LPCCPOINT ps, std::int32_t count) const noexcept(true)
	{
		::Polygon((HDC)m_hDC, (LPPOINT)ps, count);
	}

	void Canvas::Polygon(const std::vector<CPOINT>& p) const noexcept(true)
	{
		::Polygon((HDC)m_hDC, (LPPOINT)p.data(), (int)p.size());
	}

	void Canvas::TextSize(const std::string& text, std::int32_t& width, std::int32_t& height) const noexcept(true)
	{
		if (!text.empty())
		{
			SIZE size = { 0 };
			GetTextExtentPoint32A((HDC)m_hDC, text.c_str(), (int)text.length(), &size);
			width = size.cx;
			height = size.cy;
		}
	}

	void Canvas::TextSize(const std::wstring& text, std::int32_t& width, std::int32_t& height) const noexcept(true)
	{
		if (!text.empty())
		{
			SIZE size = { 0 };
			GetTextExtentPoint32W((HDC)m_hDC, text.c_str(), (int)text.length(), &size);
			width = size.cx;
			height = size.cy;
		}
	}

	void Canvas::TextInput(std::int32_t x, std::int32_t y, const std::string& text) noexcept(true)
	{
		if (!text.empty())
			::TextOutA((HDC)m_hDC, x, y, text.c_str(), (int)text.length());
	}

	void Canvas::TextInput(std::int32_t x, std::int32_t y, const std::wstring& text) noexcept(true)
	{
		if (!text.empty())
			::TextOutW((HDC)m_hDC, x, y, text.c_str(), (int)text.length());
	}

	void Canvas::DoChange(ObjectGUI* sender, Canvas* canvas) noexcept(true)
	{
		if (sender->ObjectType == 3)
		{
			if (((_Brush*)sender)->Style == bsClear)
			{
				SelectObject((HDC)canvas->Handle, GetStockObject(NULL_BRUSH));
				return;
			}
		}

		sender->Select(*canvas);
	}

	void Canvas::Update() const noexcept(true)
	{
		m_fPen.Select(*this);
		m_fBrush.Select(*this);
		m_fFont.Select(*this);
	}

	CRECT Canvas::GetClipRect() const noexcept(true)
	{
		RECT rc;
		GetClipBox((HDC)m_hDC, &rc);
		return rc;
	}

	bool Canvas::GetTransparentMode() const noexcept(true)
	{
		return GetBkMode((HDC)m_hDC) == TRANSPARENT;
	}

	void Canvas::SetTransparentMode(bool value) noexcept(true)
	{
		SetBkMode((HDC)m_hDC, value ? TRANSPARENT : OPAQUE);
	}

	void Canvas::TextRect(RECT& area, const std::string& text, std::uint32_t flags, std::uint32_t len) const noexcept(true)
	{
		if (!text.empty())
			DrawTextA((HDC)m_hDC, text.c_str(), !len ? (int)text.length() : len, &area, flags);
	}

	void Canvas::TextRect(CRECT& area, const std::string& text, std::uint32_t flags, std::uint32_t len) const noexcept(true)
	{
		if (!text.empty())
			DrawTextA((HDC)m_hDC, text.c_str(), !len ? (int)text.length() : len, (LPRECT)&area, flags);
	}

	void Canvas::TextRect(RECT& area, const std::wstring& text, std::uint32_t flags, std::uint32_t len) const noexcept(true)
	{
		if (!text.empty())
			DrawTextW((HDC)m_hDC, text.c_str(), !len ? (int)text.length() : len, &area, flags);
	}

	void Canvas::TextRect(CRECT& area, const std::wstring& text, std::uint32_t flags, std::uint32_t len) const noexcept(true)
	{
		if (!text.empty())
			DrawTextW((HDC)m_hDC, text.c_str(), !len ? (int)text.length() : len, (LPRECT)&area, flags);
	}

	void Canvas::IncludeRect(const RECT& area) const noexcept(true)
	{
		IntersectClipRect((HDC)m_hDC, area.left, area.top, area.right, area.bottom);
	}

	void Canvas::IncludeRect(const CRECT& area) const noexcept(true)
	{
		IntersectClipRect((HDC)m_hDC, area.Left, area.Top, area.Right, area.Bottom);
	}

	void Canvas::ExcludeRect(const RECT& area) const noexcept(true)
	{
		ExcludeClipRect((HDC)m_hDC, area.left, area.top, area.right, area.bottom);
	}

	void Canvas::ExcludeRect(const CRECT& area) const noexcept(true)
	{
		ExcludeClipRect((HDC)m_hDC, area.Left, area.Top, area.Right, area.Bottom);
	}

	TColor Canvas::GetColorText() const noexcept(true)
	{
		return ::GetTextColor((HDC)m_hDC);
	}

	void Canvas::SetColorText(TColor value) noexcept(true)
	{
		::SetTextColor((HDC)m_hDC, value);
	}

	void Canvas::ToFileBitmap(const std::wstring& fname) noexcept(true)
	{
		RECT rc;
		::GetClipBox((HDC)m_hDC, &rc);

		BITMAP bmpScreen = { 0 };
		HBITMAP bm = ::CreateCompatibleBitmap((HDC)m_hDC, rc.right - rc.left, rc.bottom - rc.top);

		DWORD dwBytesWritten = 0;

		// Get the BITMAP from the HBITMAP.
		GetObject(bm, sizeof(BITMAP), &bmpScreen);

		BITMAPFILEHEADER   bmfHeader = { 0 };
		BITMAPINFOHEADER   bi = { 0 };

		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = bmpScreen.bmWidth;
		bi.biHeight = bmpScreen.bmHeight;
		bi.biPlanes = 1;
		bi.biBitCount = 32;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrUsed = 0;
		bi.biClrImportant = 0;

		DWORD dwBmpSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

		// Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that 
		// call HeapAlloc using a handle to the process's default heap. Therefore, GlobalAlloc and LocalAlloc 
		// have greater overhead than HeapAlloc.
		HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
		PCHAR lpbitmap = (PCHAR)GlobalLock(hDIB);

		// Gets the "bits" from the bitmap, and copies them into a buffer 
		// that's pointed to by lpbitmap.
		GetDIBits((HDC)m_hDC, bm, 0,
			(UINT)bmpScreen.bmHeight,
			lpbitmap,
			(BITMAPINFO*)&bi, DIB_RGB_COLORS);

		try
		{
			FileStream stream(fname, FileStream::fmCreate);

			// Add the size of the headers to the size of the bitmap to get the total file size.
			std::uint32_t dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
			// Offset to where the actual bitmap bits start.
			bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
			// Size of the file.
			bmfHeader.bfSize = dwSizeofDIB;
			// bfType must always be BM for Bitmaps.
			bmfHeader.bfType = 0x4D42; // BM.

			stream.Write(&bmfHeader, (std::uint32_t)sizeof(BITMAPFILEHEADER));
			stream.Write(&bi, (std::uint32_t)sizeof(BITMAPINFOHEADER));
			stream.Write(lpbitmap, dwBmpSize);
		}
		catch (const std::exception& e)
		{
			ErrorHandler::Trigger(e.what());
		}

		// Unlock and Free the DIB from the heap.
		GlobalUnlock(hDIB);
		GlobalFree(hDIB);

		DeleteObject(bm);
	}

	Canvas::Canvas(THandle hDC) : m_hDC(hDC), m_fPen(psSolid, 1, RGB(255, 0, 0)), m_fBrush(RGB(255, 255, 255)), 
		m_fFont(hDC)
	{
		m_fPen.Canvas = this;
		m_fPen.OnChange = DoChange;
		m_fBrush.Canvas = this;
		m_fBrush.OnChange = DoChange;
		m_fFont.Canvas = this;
		m_fFont.OnChange = DoChange;

		m_hOldPen = m_fPen.Select(*this);
		m_hOldBrush = m_fBrush.Select(*this);
		m_hOldFont = m_fFont.Select(*this);
	}

	Canvas::Canvas(const Canvas& canvas) : m_hDC(canvas.m_hDC)
		, m_fPen(canvas.m_fPen), m_fBrush(canvas.m_fBrush), m_fFont(canvas.m_fFont)
	{
		m_fPen.Canvas = this;
		m_fPen.OnChange = DoChange;
		m_fBrush.Canvas = this;
		m_fBrush.OnChange = DoChange;
		m_fFont.Canvas = this;
		m_fFont.OnChange = DoChange;

		m_hOldPen = m_fPen.Select(*this);
		m_hOldBrush = m_fBrush.Select(*this);
		m_hOldFont = m_fFont.Select(*this);
	}

	Canvas::~Canvas()
	{
		::SelectObject((HDC)m_hDC, m_hOldPen);
		::SelectObject((HDC)m_hDC, m_hOldBrush);
		::SelectObject((HDC)m_hDC, m_hOldFont);
	}

	// Monitor

	bool Monitor::IsPrimary() const noexcept(true)
	{
		MONITORINFO mi = { 0 };
		mi.cbSize = sizeof(MONITORINFO);
		if (GetMonitorInfoA((HMONITOR)m_fHandle, &mi))
			return (mi.dwFlags & MONITORINFOF_PRIMARY) == MONITORINFOF_PRIMARY;
		return FALSE;
	}

	CRECT Monitor::GetRect() const noexcept(true)
	{
		MONITORINFO mi = { 0 };
		mi.cbSize = sizeof(MONITORINFO);
		if (GetMonitorInfoA((HMONITOR)m_fHandle, &mi))
			return mi.rcMonitor;
		// empty
		return {};
	}

	CRECT Monitor::GetWorkAreaRect() const noexcept(true)
	{
		MONITORINFO mi = { 0 };
		mi.cbSize = sizeof(MONITORINFO);
		if (GetMonitorInfoA((HMONITOR)m_fHandle, &mi))
			return mi.rcWork;
		// empty
		return {};
	}

	// Screen

	Screen _sscreen;

	Screen::Screen()
	{
		m_nVirtualWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		m_nVirtualHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	}

	Monitor Screen::MonitorFromPoint(const POINT& p) const noexcept(true)
	{
		return ::MonitorFromPoint(p, MONITOR_DEFAULTTONULL);
	}

	Monitor Screen::MonitorFromRect(const RECT& rect) const noexcept(true)
	{
		return ::MonitorFromRect(&rect, MONITOR_DEFAULTTONULL);
	}

	Monitor Screen::MonitorFromRect(const CRECT& rect) const noexcept(true)
	{
		return ::MonitorFromRect((LPRECT)&rect, MONITOR_DEFAULTTONULL);
	}

	Monitor Screen::MonitorFromWindow(THandle handle) const noexcept(true)
	{
		return ::MonitorFromWindow((HWND)handle, MONITOR_DEFAULTTONULL);
	}

	Screen* Screen::GetSingleton() noexcept(true)
	{
		return &_sscreen;
	}
}