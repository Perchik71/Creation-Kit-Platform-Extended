//////////////////////////////////////////
/*
* Copyright (c) 2020-2021 Perchik71 <email:perchik71@outlook.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this
* software and associated documentation files (the "Software"), to deal in the Software
* without restriction, including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or
* substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
* PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
* FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/
//////////////////////////////////////////

#include "UIGraphics.h"

namespace Core
{
	namespace Classes
	{
		namespace UI
		{
			// https://stackoverflow.com/questions/24720451/save-hbitmap-to-bmp-file-using-only-win32

			PBITMAPINFO __imCreateBitmapInfoStruct(HBITMAP hBmp)
			{
				BITMAP bmp = { 0 };
				PBITMAPINFO pbmi;
				WORD    cClrBits;

				// Retrieve the bitmap color format, width, and height.  
				Assert(GetObjectA(hBmp, sizeof(BITMAP), (LPSTR)& bmp));

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

			VOID __imCreateBMPFile(LPCSTR pszFile, HBITMAP hBMP)
			{
				HANDLE hf;						// file handle  
				BITMAPFILEHEADER hdr = { 0 };   // bitmap file-header  
				PBITMAPINFOHEADER pbih;			// bitmap info-header  
				LPBYTE lpBits;					// memory pointer  
				DWORD dwTotal;					// total count of bytes  
				DWORD cb;						// incremental count of bytes  
				BYTE* hp;						// byte pointer  
				DWORD dwTmp = 0;
				PBITMAPINFO pbi;
				HDC hDC;

				hDC = CreateCompatibleDC(GetWindowDC(GetDesktopWindow()));
				SelectObject(hDC, hBMP);

				pbi = __imCreateBitmapInfoStruct(hBMP);

				pbih = (PBITMAPINFOHEADER)pbi;
				lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

				Assert(lpBits);

				// Retrieve the color table (RGBQUAD array) and the bits  
				// (array of palette indices) from the DIB.  
				Assert(GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
					DIB_RGB_COLORS));

				// Create the .BMP file.  
				hf = CreateFileA(pszFile,
					GENERIC_READ | GENERIC_WRITE,
					(DWORD)0,
					NULL,
					CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL,
					(HANDLE)NULL);
				Assert(hf != INVALID_HANDLE_VALUE);

				hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"  
				// Compute the size of the entire file.  
				hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
					pbih->biSize + pbih->biClrUsed
					* sizeof(RGBQUAD) + pbih->biSizeImage);
				hdr.bfReserved1 = 0;
				hdr.bfReserved2 = 0;

				// Compute the offset to the array of color indices.  
				hdr.bfOffBits = (DWORD)((size_t) sizeof(BITMAPFILEHEADER) +
					pbih->biSize + pbih->biClrUsed
					* sizeof(RGBQUAD));

				// Copy the BITMAPFILEHEADER into the .BMP file.  
				Assert(WriteFile(hf, (LPVOID)& hdr, sizeof(BITMAPFILEHEADER),
					(LPDWORD)& dwTmp, NULL));

				// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.  
				Assert(WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
					+ pbih->biClrUsed * sizeof(RGBQUAD),
					(LPDWORD)& dwTmp, (NULL)));

				// Copy the array of color indices into the .BMP file.  
				dwTotal = cb = pbih->biSizeImage;
				hp = lpBits;
				Assert(WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)& dwTmp, NULL));

				// Close the .BMP file.  
				Assert(CloseHandle(hf));

				// Free memory.  
				GlobalFree((HGLOBAL)lpBits);
			}

			VOID __imGradientFill_V(HDC hdc, const COLORREF start_color, const COLORREF end_color, const LPRECT rc)
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

			VOID __imGradientFill_H(HDC hdc, const COLORREF start_color, const COLORREF end_color, const LPRECT rc)
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

			// CUIFont

			CUIFont::CUIFont(const HDC hDC) : CUIObjectGUI(4), m_lock(FALSE)
			{
				Recreate(hDC);
			}

			CUIFont::CUIFont(const std::string& name, const LONG size, const CUIFontStyles& styles, const ULONG ulCharSet, const CUIFontQuality quality,
				const CUIFontPitch pitch) : CUIObjectGUI(4), m_lock(FALSE), m_ulCharSet(ulCharSet),
				m_Name(name), m_Quality(quality), m_FontStyles(styles), m_Pitch(pitch)
			{
				Size = size;
			}

			VOID CUIFont::Recreate(const HDC hDC)
			{
				TEXTMETRICA metric;
				GetTextMetricsA(hDC, &metric);
				m_Name.resize(MAX_PATH);
				m_Name.resize((size_t)GetTextFaceA(hDC, MAX_PATH, &m_Name[0]) + 1);

				m_Quality = fqClearTypeNatural;

				if (metric.tmItalic)
					m_FontStyles.insert(fsItalic);
				if (metric.tmUnderlined)
					m_FontStyles.insert(fsUnderline);
				if (metric.tmWeight >= FW_BOLD)
					m_FontStyles.insert(fsBold);

				m_Pitch = fpDefault;
				m_Height = metric.tmHeight;

				Recreate();
			}

			VOID CUIFont::SetStyles(const CUIFontStyles & styles)
			{
				m_FontStyles = styles;
				Change();
			}

			VOID CUIFont::SetName(const std::string & name)
			{
				m_Name = name;
				Change();
			}

			LONG CUIFont::GetSize(VOID) const
			{
				HDC hDC = GetDC(0);
				LONG res = -MulDiv(m_Height, 72, GetDeviceCaps(hDC, LOGPIXELSY));
				ReleaseDC(0, hDC);
				return res;
			}

			VOID CUIFont::SetSize(const LONG value)
			{
				HDC hDC = GetDC(0);
				m_Height = -MulDiv(value, GetDeviceCaps(hDC, LOGPIXELSY), 72);
				ReleaseDC(0, hDC);
				Change();
			}

			VOID CUIFont::SetCharSet(const ULONG CharSet)
			{
				m_ulCharSet = CharSet;
				Change();
			}

			VOID CUIFont::SetHeight(const LONG value)
			{
				m_Height = value;
				Change();
			}

			VOID CUIFont::Assign(const CUIFont& font)
			{
				m_lock = TRUE;

				this->Name = font.Name;
				this->Size = font.Size;
				this->Pitch = font.Pitch;
				this->Styles = font.Styles;

				m_lock = FALSE;

				Change();
			}

			CUIFont& CUIFont::operator=(const CUIFont& font)
			{
				Assign(font);
				return *this;
			}

			VOID CUIFont::Apply(HWND window) const
			{
				SendMessageA(window, WM_SETFONT, (WPARAM)m_fHandle, 0);
			}

			VOID CUIFont::SetQuality(const CUIFontQuality quality)
			{
				m_Quality = quality;
				Change();
			}

			VOID CUIFont::SetPitch(const CUIFontPitch pitch)
			{
				m_Pitch = pitch;
				Change();
			}

			VOID CUIFont::Recreate(VOID)
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
					(int)m_FontStyles.count(fsBold) ? FW_BOLD : FW_NORMAL,
					(int)m_FontStyles.count(fsItalic),
					(int)m_FontStyles.count(fsUnderline),
					(int)m_FontStyles.count(fsStrikeOut),
					m_ulCharSet, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
					_Quality,
					_Pitch,
					m_Name.c_str());
			}

			VOID CUIFont::Change(VOID)
			{
				if (m_lock)
					return;

				Recreate();
				DoChange();
			}

			// CUIObjectGUI

			VOID CUIObjectGUI::Release(VOID)
			{
				if (!m_fHandle)
					return;

				if(DeleteObject((HGDIOBJ)m_fHandle))
					m_fHandle = NULL;
			}

			VOID CUIObjectGUI::DoChange(VOID)
			{
				if (OnChange)
					OnChange(this, Canvas);
			}

			HANDLE CUIObjectGUI::Select(const CUICanvas& canvas) const
			{
				return (HANDLE)SelectObject(canvas.Handle, m_fHandle);
			}

			HANDLE CUIObjectGUI::Select(VOID) const
			{
				if (Canvas)
					return (HANDLE)SelectObject(Canvas->Handle, m_fHandle);
				return m_fHandle;
			}

			CUIObjectGUI::~CUIObjectGUI(VOID)
			{
				Release();
			}

			// CUIBitmap

			VOID CUIBitmap::SaveToFile(LPCSTR fname) const
			{
				if (!Empty())
					__imCreateBMPFile(fname, (HBITMAP)m_fHandle);
			}

			VOID CUIBitmap::Create(INT width, INT height, INT bpp)
			{
				Assert((width > 0) && (height > 0) && ((bpp == 24) || (bpp == 32)));
				m_fHandle = CreateBitmap(width, height, 1, bpp, NULL);
				Assert(m_fHandle);
				DoChange();
			}

			VOID CUIBitmap::Create(LPCSTR fname)
			{
				m_fHandle = LoadImageA(NULL, fname, IMAGE_BITMAP, 0, 0, LR_VGACOLOR | LR_COLOR | LR_LOADFROMFILE | LR_CREATEDIBSECTION);
				Assert(m_fHandle);
				DoChange();
			}

			VOID CUIBitmap::Create(HINSTANCE hinst, LPCSTR name)
			{
				m_fHandle = LoadImageA(hinst, name, IMAGE_BITMAP, 0, 0, LR_VGACOLOR | LR_COLOR | LR_CREATEDIBSECTION);
				Assert(m_fHandle);
				DoChange();
			}

			VOID CUIBitmap::FreeImage(VOID)
			{
				Release();
				DoChange();
			}

			VOID CUIBitmap::Assign(const CUIBitmap& bitmap)
			{
				Release();
				if (!bitmap.Empty())
				{
					m_fHandle = CopyImage(bitmap.Handle, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
					Assert(m_fHandle);
				}
				DoChange();
			}

			CUIBitmap& CUIBitmap::operator=(const CUIBitmap& bitmap)
			{
				Assign(bitmap);
				return *this;
			}

			CUIBitmap::CUIBitmap(HBITMAP bitmap) : CUIObjectGUI(1)
			{ 
				if (bitmap)
				{
					m_fHandle = CopyImage(bitmap, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
					Assert(m_fHandle);
				}
			}

			CUIBitmap::CUIBitmap(const CUIBitmap& bitmap) : CUIObjectGUI(bitmap)
			{
				if (!bitmap.Empty())
				{
					m_fHandle = CopyImage(bitmap.Handle, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);
					Assert(m_fHandle);
				}
			}

			// CUIPen

			VOID CUIPen::Create(CUIPenStyle style, INT width, COLORREF color)
			{
				UINT flag = 0;

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
				Assert(m_fHandle);
				
				m_fStyle = style;
				m_fSize = width;
				m_fColor = color;

				DoChange();
			}

			VOID CUIPen::SetWidth(const INT width)
			{
				Release();
				Create(Style, (width > 0) ? width : 1, Color);
			}

			VOID CUIPen::SetColor(const COLORREF color)
			{
				Release();
				Create(Style, Width, color);
			}

			VOID CUIPen::SetStyle(const CUIPenStyle style)
			{
				Release();
				Create(style, Width, Color);
			}

			VOID CUIPen::Assign(const CUIPen& pen)
			{
				Release();
				Create(pen.Style, pen.Width, pen.Color);
			}

			CUIPen& CUIPen::operator=(const CUIPen& pen)
			{
				Assign(pen);
				return *this;
			}

			CUIPen WINAPI CreateSolidPen(INT width, COLORREF color)
			{
				return CUIPen(psSolid, width, color);
			}

			CUIPen WINAPI CreateDashPen(INT width, COLORREF color)
			{
				return CUIPen(psDash, width, color);
			}

			CUIPen WINAPI CreateDotPen(INT width, COLORREF color)
			{
				return CUIPen(psDot, width, color);
			}

			// CUIBrush

			VOID CUIBrush::Assign(const CUIBrush& brush)
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

			VOID CUIBrush::Create(const COLORREF color)
			{
				m_fHandle = ::CreateSolidBrush(color);
				Assert(m_fHandle);
				m_fColor = color;
				m_fStyle = bsSolid;
				DoChange();
			}

			VOID CUIBrush::Create(const INT iHatch, const COLORREF color)
			{
				m_fHandle = ::CreateHatchBrush(iHatch, color);
				Assert(m_fHandle);
				m_fHatch = iHatch;
				m_fColor = color;
				m_fStyle = bsHatch;
				DoChange();
			}

			VOID CUIBrush::Create(const CUIBitmap& bitmap)
			{
				Assert(!bitmap.Empty());
				m_fBitmap.Assign(bitmap);
				m_fHandle = ::CreatePatternBrush((HBITMAP)m_fBitmap.Handle);
				Assert(m_fHandle);
				m_fStyle = bsBitmap;
				DoChange();
			}

			VOID CUIBrush::SetHatch(const INT value)
			{
				if (m_fStyle == CUIBrushStyle::bsHatch)
				{
					Release();
					Create(value, m_fColor);
				}
				else m_fHatch = value;
			}

			VOID CUIBrush::SetColor(const COLORREF color)
			{
				if ((m_fStyle == CUIBrushStyle::bsHatch) || (m_fStyle == CUIBrushStyle::bsSolid))
				{
					Release();
					if (m_fStyle == CUIBrushStyle::bsHatch) 
						Create(m_fHatch, color);
					else
						Create(color);
				}
				else m_fColor = color;
			}

			VOID CUIBrush::SetStyle(const CUIBrushStyle style)
			{
				if (style == m_fStyle)
					return;
				
				if (m_fStyle == bsBitmap)
					m_fBitmap.FreeImage();

				Release();

				switch (style)
				{
				case CUIBrushStyle::bsBitmap:
					/* bitmap empty */
					break;
				case CUIBrushStyle::bsHatch:
					Create(m_fHatch, m_fColor);
					break;
				case CUIBrushStyle::bsClear:
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

			VOID CUIBrush::SetBitmap(const CUIBitmap& bitmap)
			{
				Release();
				Create(bitmap);
			}

			CUIBrush::CUIBrush(const CUIBrush& brush) : CUIObjectGUI(brush)
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

			CUIBrush& CUIBrush::operator=(const CUIBrush& brush)
			{
				Assign(brush);
				return *this;
			}

			CUIBrush WINAPI CreateSolidBrush(const COLORREF color)
			{
				return color;
			}

			CUIBrush WINAPI CreatePatternBrush(const CUIBitmap& bitmap)
			{
				return bitmap;
			}

			CUIBrush WINAPI CreateHatchBrush(const INT iHatch, const COLORREF color)
			{
				return CUIBrush(iHatch, color);
			}

			bool WINAPI CreateGradientBrush(CUIBrush& brush, const COLORREF start_color, const COLORREF end_color, 
				const INT size, const CUIGradientDirect direct)
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

			// CUICanvas

			VOID CUICanvas::SetPixel(INT x, INT y, const COLORREF color) const
			{
				::SetPixel(m_hDC, x, y, color);
			}

			VOID CUICanvas::SetPixel(const POINT& p, const COLORREF color) const
			{
				::SetPixel(m_hDC, p.x, p.y, color);
			}

			VOID CUICanvas::SetBrush(const CUIBrush& brush)
			{
				m_fBrush.Assign(brush);
			}

			VOID CUICanvas::SetPen(const CUIPen& pen)
			{
				m_fPen.Assign(pen);
			}

			VOID CUICanvas::SetFont(const CUIFont& font)
			{
				m_fFont.Assign(font);
			}

			VOID CUICanvas::MoveTo(INT x, INT y) const
			{
				::MoveToEx(m_hDC, x, y, NULL);
			}

			VOID CUICanvas::MoveTo(const POINT& p) const
			{
				MoveTo(p.x, p.y);
			}

			VOID CUICanvas::LineTo(INT x, INT y) const
			{
				::LineTo(m_hDC, x, y);
			}

			VOID CUICanvas::LineTo(const POINT& p) const
			{
				LineTo(p.x, p.y);
			}

			VOID CUICanvas::RoundRect(const RECT& area, const INT w, const INT h) const
			{
				::RoundRect(m_hDC, area.left, area.top, area.right, area.bottom, w, h);
			}

			VOID CUICanvas::RoundRect(const CRECT& area, const INT w, const INT h) const
			{
				::RoundRect(m_hDC, area.Left, area.Top, area.Right, area.Bottom, w, h);
			}

			VOID CUICanvas::Ellipse(const RECT& area) const
			{
				::Ellipse(m_hDC, area.left, area.top, area.right, area.bottom);
			}

			VOID CUICanvas::Ellipse(const CRECT& area) const
			{
				::Ellipse(m_hDC, area.Left, area.Top, area.Right, area.Bottom);
			}

			VOID CUICanvas::Rectangle(const RECT& area) const
			{
				::Rectangle(m_hDC, area.left, area.top, area.right, area.bottom);
			}

			VOID CUICanvas::Rectangle(const CRECT& area) const
			{
				::Rectangle(m_hDC, area.Left, area.Top, area.Right, area.Bottom);
			}

			VOID CUICanvas::Fill(const COLORREF color)
			{
				RECT rc;
				GetClipBox(m_hDC, &rc);
				Fill(rc, color);
			}

			VOID CUICanvas::Fill(const RECT& area, const COLORREF color)
			{
				m_fBrush.Color = color;
				m_fBrush.Style = bsSolid;
				FillRect(m_hDC, &area, (HBRUSH)m_fBrush.Handle);
			}

			VOID CUICanvas::Fill(const CRECT& area, const COLORREF color)
			{
				m_fBrush.Color = color;
				m_fBrush.Style = bsSolid;
				FillRect(m_hDC, (LPRECT)&area, (HBRUSH)m_fBrush.Handle);
			}

			VOID CUICanvas::FillWithTransparent(const RECT& area, const COLORREF color, BYTE percent)
			{
				HDC hMemDC = ::CreateCompatibleDC(m_hDC);
				if (!hMemDC) return;

				int w = area.right - area.left;
				int h = area.bottom - area.top;

				HBITMAP hMemBmp = ::CreateCompatibleBitmap(m_hDC, w, h);
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

				AlphaBlend(m_hDC,
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

			VOID CUICanvas::FillWithTransparent(const CRECT& area, const COLORREF color, BYTE percent)
			{
				HDC hMemDC = ::CreateCompatibleDC(m_hDC);
				if (!hMemDC) return;

				int w = area.Width;
				int h = area.Height;

				HBITMAP hMemBmp = ::CreateCompatibleBitmap(m_hDC, w, h);
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

				AlphaBlend(m_hDC,
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

			VOID CUICanvas::Fill(const LPCRECT area, const INT nCount, const COLORREF color)
			{
				m_fBrush.Color = color;
				m_fBrush.Style = bsSolid;

				for (INT i = 0; i < nCount; i++)
					FillRect(m_hDC, &area[i], (HBRUSH)m_fBrush.Handle);
			}

			VOID CUICanvas::Fill(const LPCCRECT area, const INT nCount, const COLORREF color)
			{
				m_fBrush.Color = color;
				m_fBrush.Style = bsSolid;

				for (INT i = 0; i < nCount; i++)
					FillRect(m_hDC, (LPCRECT)&area[i], (HBRUSH)m_fBrush.Handle);
			}

			VOID CUICanvas::EllipseFill(const RECT& area, const COLORREF color)
			{
				m_fPen.Style = psClear;
				m_fBrush.Color = color;
				m_fBrush.Style = bsSolid;

				::Ellipse(m_hDC, area.left, area.top, area.right, area.bottom);
			}

			VOID CUICanvas::EllipseFill(const CRECT& area, const COLORREF color)
			{
				m_fPen.Style = psClear;
				m_fBrush.Color = color;
				m_fBrush.Style = bsSolid;

				::Ellipse(m_hDC, area.Left, area.Top, area.Right, area.Bottom);
			}

			VOID CUICanvas::GradientFill(const RECT& area, const COLORREF start_color, const COLORREF end_color, const CUIGradientDirect direct)
			{
				if (direct == gdHorz)
					__imGradientFill_H(m_hDC, start_color, end_color, (LPRECT)&area);
				else
					__imGradientFill_V(m_hDC, start_color, end_color, (LPRECT)&area);
			}

			VOID CUICanvas::GradientFill(const CRECT& area, const COLORREF start_color, const COLORREF end_color, const CUIGradientDirect direct)
			{
				if (direct == gdHorz)
					__imGradientFill_H(m_hDC, start_color, end_color, (LPRECT)&area);
				else
					__imGradientFill_V(m_hDC, start_color, end_color, (LPRECT)&area);
			}

			VOID CUICanvas::Frame(const COLORREF color)
			{
				RECT rc;
				GetClipBox(m_hDC, &rc);
				Frame(rc, color);
			}

			VOID CUICanvas::Frame(const RECT& area, const COLORREF color)
			{
				m_fBrush.Color = color;
				m_fBrush.Style = bsSolid;
				FrameRect(m_hDC, &area, (HBRUSH)m_fBrush.Handle);
			}

			VOID CUICanvas::Frame(const CRECT& area, const COLORREF color)
			{
				m_fBrush.Color = color;
				m_fBrush.Style = bsSolid;
				FrameRect(m_hDC, (LPRECT)&area, (HBRUSH)m_fBrush.Handle);
			}

			VOID CUICanvas::GradientFrame(const RECT& area, const COLORREF start_color, const COLORREF end_color, const CUIGradientDirect direct)
			{
				if (direct == gdHorz)
					if (CreateGradientBrush(Brush, start_color, end_color, area.right - area.left, direct))
						FrameRect(m_hDC, (LPRECT)&area, (HBRUSH)Brush.Handle);
				else
					if (CreateGradientBrush(Brush, start_color, end_color, area.bottom - area.top, direct))
						FrameRect(m_hDC, (LPRECT)&area, (HBRUSH)Brush.Handle);
			}

			VOID CUICanvas::GradientFrame(const CRECT& area, const COLORREF start_color, const COLORREF end_color, const CUIGradientDirect direct)
			{
				if (direct == gdHorz)
					if (CreateGradientBrush(Brush, start_color, end_color, area.Width, direct))
						FrameRect(m_hDC, (LPRECT)&area, (HBRUSH)Brush.Handle);
				else
					if (CreateGradientBrush(Brush, start_color, end_color, area.Height, direct))
						FrameRect(m_hDC, (LPRECT)&area, (HBRUSH)Brush.Handle);
			}

			VOID CUICanvas::Polygon(const LPPOINT ps, INT count) const
			{
				::Polygon(m_hDC, ps, count);
			}

			VOID CUICanvas::Polygon(const std::vector<POINT>& p) const
			{
				::Polygon(m_hDC, p.data(), (int)p.size());
			}

			VOID CUICanvas::TextSize(LPCSTR text, INT& width, INT& height) const
			{
				if (text)
				{
					SIZE size = { 0 };
					GetTextExtentPoint32A(m_hDC, text, (int)strlen(text), &size);
					width = size.cx;
					height = size.cy;
				}
			}

			VOID CUICanvas::TextInput(INT x, INT y, LPCSTR text)
			{
				if (text)
					::TextOutA(m_hDC, x, y, text, (int)strlen(text));
			}

			VOID CUICanvas::DoChange(CUIObjectGUI* sender, CUICanvas* canvas)
			{
				if (sender->ObjectType == 3)
				{
					if (((CUIBrush*)sender)->Style == bsClear)
					{
						SelectObject(canvas->Handle, GetStockObject(NULL_BRUSH));
						return;
					}
				}

				sender->Select(*canvas);
			}

			VOID CUICanvas::Update(VOID) const
			{
				m_fPen.Select(*this);
				m_fBrush.Select(*this);
				m_fFont.Select(*this);
			}

			CRECT CUICanvas::GetClipRect(VOID) const
			{
				RECT rc;
				GetClipBox(m_hDC, &rc);
				return rc;
			}

			BOOL CUICanvas::GetTransparentMode(VOID) const
			{
				return GetBkMode(m_hDC) == TRANSPARENT;
			}

			VOID CUICanvas::SetTransparentMode(BOOL value)
			{
				SetBkMode(m_hDC, value ? TRANSPARENT : OPAQUE);
			}

			VOID CUICanvas::TextRect(RECT& area, LPCSTR text, UINT flags, UINT len) const
			{
				if (text)
					DrawTextA(m_hDC, text, !len ? (int)strlen(text) : len, &area, flags);
			}

			VOID CUICanvas::TextRect(CRECT& area, LPCSTR text, UINT flags, UINT len) const
			{
				if (text)
					DrawTextA(m_hDC, text, !len ? (int)strlen(text) : len, (LPRECT)&area, flags);
			}

			VOID CUICanvas::TextRect(RECT& area, LPCWSTR text, UINT flags, UINT len) const
			{
				if (text)
					DrawTextW(m_hDC, text, !len ? (int)wcslen(text) : len, &area, flags);
			}

			VOID CUICanvas::TextRect(CRECT& area, LPCWSTR text, UINT flags, UINT len) const
			{
				if (text)
					DrawTextW(m_hDC, text, !len ? (int)wcslen(text) : len, (LPRECT)&area, flags);
			}

			VOID CUICanvas::IncludeRect(const RECT& area) const
			{
				IntersectClipRect(m_hDC, area.left, area.top, area.right, area.bottom);
			}

			VOID CUICanvas::IncludeRect(const CRECT& area) const
			{
				IntersectClipRect(m_hDC, area.Left, area.Top, area.Right, area.Bottom);
			}

			VOID CUICanvas::ExcludeRect(const RECT& area) const
			{
				ExcludeClipRect(m_hDC, area.left, area.top, area.right, area.bottom);
			}

			VOID CUICanvas::ExcludeRect(const CRECT& area) const
			{
				ExcludeClipRect(m_hDC, area.Left, area.Top, area.Right, area.Bottom);
			}

			COLORREF CUICanvas::GetColorText(VOID) const
			{
				return ::GetTextColor(m_hDC);
			}

			VOID CUICanvas::SetColorText(COLORREF value)
			{
				::SetTextColor(m_hDC, value);
			}

			VOID CUICanvas::ToFileBitmap(LPCSTR fname)
			{
				RECT rc;
				::GetClipBox(m_hDC, &rc);
				
				BITMAP bmpScreen = { 0 };
				HBITMAP bm = ::CreateCompatibleBitmap(m_hDC, rc.right - rc.left, rc.bottom - rc.top);

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
				GetDIBits(m_hDC, bm, 0,
					(UINT)bmpScreen.bmHeight,
					lpbitmap,
					(BITMAPINFO*)&bi, DIB_RGB_COLORS);

				// A file is created, this is where we will save the screen capture.
				HANDLE hFile = CreateFileA(fname,
					GENERIC_WRITE,
					0,
					NULL,
					CREATE_ALWAYS,
					FILE_ATTRIBUTE_NORMAL, NULL);

				// Add the size of the headers to the size of the bitmap to get the total file size.
				DWORD dwSizeofDIB = dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

				// Offset to where the actual bitmap bits start.
				bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);
				// Size of the file.
				bmfHeader.bfSize = dwSizeofDIB;
				// bfType must always be BM for Bitmaps.
				bmfHeader.bfType = 0x4D42; // BM.

				WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
				WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
				WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

				// Unlock and Free the DIB from the heap.
				GlobalUnlock(hDIB);
				GlobalFree(hDIB);

				// Close the handle for the file that was created.
				CloseHandle(hFile);

				DeleteObject(bm);
			}

			CUICanvas::CUICanvas(HDC hDC) : m_hDC(hDC), m_fPen(psSolid, 1, RGB(255, 0, 0)), m_fBrush(RGB(255, 255, 255)), m_fFont(hDC)
			{
				m_fPen.Canvas = this;
				m_fPen.OnChange = DoChange;
				m_fBrush.Canvas = this;
				m_fBrush.OnChange = DoChange;
				m_fFont.Canvas = this;
				m_fFont.OnChange = DoChange;

				m_hOldPen = (HPEN)m_fPen.Select(*this);
				m_hOldBrush = (HBRUSH)m_fBrush.Select(*this);
				m_hOldFont = (HFONT)m_fFont.Select(*this);
			}

			CUICanvas::CUICanvas(const CUICanvas& canvas) : m_hDC(canvas.m_hDC), m_fPen(canvas.m_fPen), m_fBrush(canvas.m_fBrush), m_fFont(canvas.m_fFont)
			{
				m_fPen.Canvas = this;
				m_fPen.OnChange = DoChange;
				m_fBrush.Canvas = this;
				m_fBrush.OnChange = DoChange;
				m_fFont.Canvas = this;
				m_fFont.OnChange = DoChange;

				m_hOldPen = (HPEN)m_fPen.Select(*this);
				m_hOldBrush = (HBRUSH)m_fBrush.Select(*this);
				m_hOldFont = (HFONT)m_fFont.Select(*this);
			}

			CUICanvas::~CUICanvas(VOID)
			{
				::SelectObject(m_hDC, m_hOldPen);
				::SelectObject(m_hDC, m_hOldBrush);
				::SelectObject(m_hDC, m_hOldFont);
			}

			// CUIMonitor

			BOOL CUIMonitor::IsPrimary(VOID) const
			{
				MONITORINFO mi = { 0 };
				mi.cbSize = sizeof(MONITORINFO);
				if (GetMonitorInfoA(m_fHandle, &mi))
					return (mi.dwFlags & MONITORINFOF_PRIMARY) == MONITORINFOF_PRIMARY;
				return FALSE;
			}

			CRECT CUIMonitor::GetRect(VOID) const
			{
				MONITORINFO mi = { 0 };
				mi.cbSize = sizeof(MONITORINFO);
				if (GetMonitorInfoA(m_fHandle, &mi))
					return mi.rcMonitor;
				// empty
				return {};
			}

			CRECT CUIMonitor::GetWorkAreaRect(VOID) const
			{
				MONITORINFO mi = { 0 };
				mi.cbSize = sizeof(MONITORINFO);
				if (GetMonitorInfoA(m_fHandle, &mi))
					return mi.rcWork;
				// empty
				return {};
			}

			// CUIScreen

			CUIScreen Screen;

			CUIScreen::CUIScreen(VOID)
			{
				m_nVirtualWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
				m_nVirtualHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
			}

			CUIMonitor CUIScreen::MonitorFromPoint(const POINT& p) const
			{
				return ::MonitorFromPoint(p, MONITOR_DEFAULTTONULL);
			}

			CUIMonitor CUIScreen::MonitorFromRect(const RECT& rect) const
			{
				return ::MonitorFromRect(&rect, MONITOR_DEFAULTTONULL);
			}

			CUIMonitor CUIScreen::MonitorFromRect(const CRECT& rect) const
			{
				return ::MonitorFromRect((LPRECT)&rect, MONITOR_DEFAULTTONULL);
			}

			CUIMonitor CUIScreen::MonitorFromWindow(HWND handle) const
			{
				return ::MonitorFromWindow(handle, MONITOR_DEFAULTTONULL);
			}
		}
	}
}