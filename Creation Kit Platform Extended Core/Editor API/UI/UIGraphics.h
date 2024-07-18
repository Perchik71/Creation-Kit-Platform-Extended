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

#pragma once

namespace Core
{
	namespace Classes
	{
		namespace UI
		{
			class CRECT
			{
			private:
				LONG m_left;
				LONG m_top;
				LONG m_right;
				LONG m_bottom;
			public:
				CRECT() : m_left(0), m_top(0), m_right(0), m_bottom(0) {}
				CRECT(const LONG l, const LONG t, const LONG r, const LONG b) : m_left(l), m_top(t), m_right(r), m_bottom(b) {}
				CRECT(const RECT& r) : m_left(r.left), m_top(r.top), m_right(r.right), m_bottom(r.bottom) {}
				CRECT(const CRECT& r) : m_left(r.m_left), m_top(r.m_top), m_right(r.m_right), m_bottom(r.m_bottom) {}
			public:
				inline LONG GetWidth(VOID) const { return m_right - m_left; }
				inline LONG GetHeight(VOID) const { return m_bottom - m_top; }
				inline LONG GetLeft(VOID) const { return m_left; }
				inline LONG GetTop(VOID) const { return m_top; }
				inline LONG GetRight(VOID) const { return m_right; }
				inline LONG GetBottom(VOID) const { return m_bottom; }
				inline VOID SetWidth(const LONG value) { m_right = value + m_left; }
				inline VOID SetHeight(const LONG value) { m_bottom = value + m_top; }
				inline VOID SetLeft(const LONG value) { m_right = GetWidth() + value; m_left = value; }
				inline VOID SetTop(const LONG value) { m_bottom = GetHeight() + value; m_top = value; }
				inline VOID SetRight(const LONG value) { m_right = value; }
				inline VOID SetBottom(const LONG value) { m_bottom = value; }
			public:
				inline CRECT& Inflate(const LONG x, const LONG y) { m_left -= x; m_top -= y; m_right += x; m_bottom += y; return *this; }
				inline CRECT& Offset(const LONG x, const LONG y) { m_left += x; m_top += y; m_right += x; m_bottom += y; return *this; }
				inline BOOL IsEmpty(VOID) const { return GetWidth() == 0 || GetHeight() == 0; }
				inline CRECT Dublicate(VOID) const { return *this; }
			public:
				inline BOOL operator==(const RECT& r) const { return (r.left == Left) && (r.top == Top) && (r.right == Right) && (r.bottom == Bottom); }
				inline BOOL operator==(const CRECT& r) const { return (r.Left == Left) && (r.Top == Top) && (r.Right == Right) && (r.Bottom == Bottom); }
				inline BOOL operator!=(const RECT& r) const { return !(*this == r); }
				inline BOOL operator!=(const CRECT & r) const { return !(*this == r); }
			public:
				PROPERTY(GetWidth, SetWidth) LONG Width;
				PROPERTY(GetHeight, SetHeight) LONG Height;
				PROPERTY(GetLeft, SetLeft) LONG Left;
				PROPERTY(GetTop, SetTop) LONG Top;
				PROPERTY(GetRight, SetRight) LONG Right;
				PROPERTY(GetBottom, SetBottom) LONG Bottom;
			};

			typedef CRECT* LPCCRECT, PCCRECT;

			class CUICanvas;
			class CUIObjectGUI;

			enum CUIPenStyle
			{
				psClear,
				psSolid,
				psDash,
				psDot,
				psDashDot,
				psDashDotDot,
				psInsideFrame
			};

			typedef VOID (*CUIObjectGUIChangeEvent)(CUIObjectGUI* sender, CUICanvas* canvas);

			class CUIObjectGUI
			{
			protected:
				UINT m_fType;
				HANDLE m_fHandle;
			protected:
				virtual VOID Release(VOID);
				VOID DoChange(VOID);
			public:
				inline UINT GetObjectType(VOID) const { return m_fType; }
				inline HANDLE GetHandle(VOID) const { return m_fHandle; }
				HANDLE Select(const CUICanvas& canvas) const;
				HANDLE Select(VOID) const;
			public:
				CUIObjectGUI(UINT uType) : m_fType(uType), OnChange(NULL), m_fHandle(NULL), Canvas(NULL) {}
				CUIObjectGUI(const CUIObjectGUI &obj) : m_fType(obj.m_fType), OnChange(obj.OnChange), m_fHandle(obj.m_fHandle), Canvas(obj.Canvas) {}
				virtual ~CUIObjectGUI(VOID);
			public:
				CUICanvas* Canvas;
				CUIObjectGUIChangeEvent OnChange;
			public:
				LONG_PTR Tag;
			public:
				READ_PROPERTY(GetObjectType) UINT ObjectType;
				READ_PROPERTY(GetHandle) HANDLE Handle;
			};

			class CUIBitmap : public CUIObjectGUI
			{
			protected:
				VOID Create(INT width, INT height, INT bpp);
				VOID Create(LPCSTR fname);
				VOID Create(HINSTANCE hinst, LPCSTR name);
			public:
				VOID FreeImage(VOID);
				BOOL Empty(VOID) const { return !m_fHandle; };
				VOID SaveToFile(LPCSTR fname) const;
				VOID Assign(const CUIBitmap& bitmap);

				CUIBitmap& operator=(const CUIBitmap& bitmap);
			public:
				CUIBitmap(VOID) : CUIObjectGUI(1) {}
				CUIBitmap(HBITMAP bitmap);
				CUIBitmap(INT width, INT height, INT bpp) : CUIObjectGUI(1) { Create(width, height, bpp); }
				CUIBitmap(LPCSTR fname) : CUIObjectGUI(1) { Create(fname); }
				CUIBitmap(HINSTANCE hinst, LPCSTR name) : CUIObjectGUI(1) { Create(hinst, name); }
				CUIBitmap(const CUIBitmap& bitmap);
			};

			class CUIPen : public CUIObjectGUI
			{
			private:
				INT m_fSize;
				COLORREF m_fColor;
				CUIPenStyle m_fStyle;
			protected:
				VOID Create(CUIPenStyle style, INT width, COLORREF color);
			public:
				inline INT GetWidth(VOID) const { return m_fSize; }
				inline COLORREF GetColor(VOID) const { return m_fColor; }
				inline CUIPenStyle GetStyle(VOID) const { return m_fStyle; }
				VOID SetWidth(const INT width);
				VOID SetColor(const COLORREF color);
				VOID SetStyle(const CUIPenStyle style);
				VOID Assign(const CUIPen& pen);
			public:
				CUIPen(CUIPenStyle style, INT width, COLORREF color) : CUIObjectGUI(2) { Create(style, width, color); }
				CUIPen(const CUIPen& pen) : CUIObjectGUI(pen) { Create(pen.m_fStyle, pen.m_fSize, pen.m_fColor); }

				CUIPen& operator=(const CUIPen& pen);
			public:
				PROPERTY(GetWidth, SetWidth) const INT Width;
				PROPERTY(GetColor, SetColor) const COLORREF Color;
				PROPERTY(GetStyle, SetStyle) const CUIPenStyle Style;
			};

			CUIPen WINAPI CreateSolidPen(INT width, COLORREF color);
			CUIPen WINAPI CreateDashPen(INT width, COLORREF color);
			CUIPen WINAPI CreateDotPen(INT width, COLORREF color);

			enum CUIBrushStyle
			{
				bsClear,
				bsSolid,
				bsHatch,
				bsBitmap
			};

			class CUIBrush : public CUIObjectGUI
			{
			private:
				INT m_fHatch;
				COLORREF m_fColor;
				CUIBitmap m_fBitmap;
				CUIBrushStyle m_fStyle;
			protected:
				VOID Create(const COLORREF color);
				VOID Create(const INT iHatch, const COLORREF color);
				VOID Create(const CUIBitmap& bitmap);
			public:
				inline INT GetHatch(VOID) const { return m_fHatch; }
				inline COLORREF GetColor(VOID) const { return m_fColor; }
				inline CUIBrushStyle GetStyle(VOID) const { return m_fStyle; }
				inline const CUIBitmap& GetBitmap(VOID) const { return m_fBitmap; }
				VOID SetHatch(const INT value);
				VOID SetColor(const COLORREF color);
				VOID SetStyle(const CUIBrushStyle style);
				VOID SetBitmap(const CUIBitmap& bitmap);
				VOID Assign(const CUIBrush& brush);
			public:
				CUIBrush(const COLORREF color) : CUIObjectGUI(3) { Create(color); }
				CUIBrush(const INT iHatch, const COLORREF color) : CUIObjectGUI(3) { Create(iHatch, color); }
				CUIBrush(const CUIBitmap& bitmap) : CUIObjectGUI(3) { Create(bitmap); }
				CUIBrush(const CUIBrush& brush);

				CUIBrush& operator=(const CUIBrush& brush);
			public:
				PROPERTY(GetHatch, SetHatch) const INT Hatch;
				PROPERTY(GetColor, SetColor) const COLORREF Color;
				PROPERTY(GetStyle, SetStyle) const CUIBrushStyle Style;
				PROPERTY(GetBitmap, SetBitmap) const CUIBitmap& Bitmap;
			};

			CUIBrush WINAPI CreateSolidBrush(const COLORREF color);
			CUIBrush WINAPI CreateHatchBrush(const INT iHatch, const COLORREF color);
			CUIBrush WINAPI CreatePatternBrush(const CUIBitmap& bitmap);

			enum CUIGradientDirect {
				gdHorz,
				gdVert
			};

			bool WINAPI CreateGradientBrush(CUIBrush& brush, const COLORREF start_color, const COLORREF end_color, const INT size, const CUIGradientDirect direct);

			enum CUIFontStyle {
				fsBold,
				fsItalic,
				fsUnderline,
				fsStrikeOut
			};
			typedef std::set<CUIFontStyle> CUIFontStyles;
			enum CUIFontQuality {
				fqDefault,
				fqClearType,
				fqClearTypeNatural,
				fqProof,
				fqAntialiased,
				fqNoAntialiased,
			};
			enum CUIFontPitch {
				fpDefault,
				fpFixed,
				fpVariable,
				fpMono
			};

			class CUIFont : public CUIObjectGUI
			{
			private:
				BOOL m_lock;
				std::string m_Name;
				CUIFontStyles m_FontStyles;
				CUIFontQuality m_Quality;
				CUIFontPitch m_Pitch;
				LONG m_Height;
				ULONG m_ulCharSet;
			private:
				VOID Change(VOID);
			public:
				std::string GetName(VOID) const { return m_Name; }
				VOID SetName(const std::string& name);
				LONG GetSize(VOID) const;
				VOID SetSize(const LONG value);
				inline LONG GetHeight(VOID) const { return m_Height; }
				VOID SetHeight(const LONG value);
				inline CUIFontStyles GetStyles(VOID) const { return m_FontStyles; }
				VOID SetStyles(const CUIFontStyles& styles);
				inline CUIFontQuality GetQuality(VOID) const { return m_Quality; }
				VOID SetQuality(const CUIFontQuality quality);
				inline CUIFontPitch GetPitch(VOID) const { return m_Pitch; }
				VOID SetPitch(const CUIFontPitch pitch);
				inline ULONG GetCharSet(VOID) const { return m_ulCharSet; }
				VOID SetCharSet(const ULONG CharSet);
			private:
				VOID Recreate(VOID);
				VOID Recreate(const HDC hDC);
			public:
				VOID Apply(HWND window) const;
				VOID Assign(const CUIFont& font);

				CUIFont& operator=(const CUIFont& font);
			public:
				READ_PROPERTY(GetHandle) HFONT Handle;
				PROPERTY(GetName, SetName) const std::string Name;
				PROPERTY(GetSize, SetSize) const LONG Size;
				PROPERTY(GetHeight, SetHeight) const LONG Height;
				PROPERTY(GetStyles, SetStyles) const CUIFontStyles Styles;
				PROPERTY(GetQuality, SetQuality) const CUIFontQuality Quality;
				PROPERTY(GetPitch, SetPitch) const CUIFontPitch Pitch;
				PROPERTY(GetCharSet, SetCharSet) const ULONG CharSet;
			public:
				CUIFont(const std::string& name, const LONG size, const CUIFontStyles& styles = {}, const ULONG ulCharSet = DEFAULT_CHARSET, const CUIFontQuality quality = fqClearTypeNatural,
					const CUIFontPitch pitch = fpVariable);
				CUIFont(const HDC hDC);
				CUIFont(const CUIFont& parent) : CUIObjectGUI(parent), m_FontStyles(parent.m_FontStyles), m_Quality(parent.m_Quality), m_ulCharSet(parent.m_ulCharSet), m_Pitch(parent.m_Pitch),
					m_Name(parent.m_Name), m_Height(parent.m_Height), m_lock(FALSE) {
					Recreate();
				}
			};

			class CUICanvas
			{
			private:
				HDC m_hDC;
				CUIPen m_fPen;
				CUIBrush m_fBrush;
				CUIFont m_fFont;
				HPEN m_hOldPen;
				HBRUSH m_hOldBrush;
				HFONT m_hOldFont;
			private:
				static VOID DoChange(CUIObjectGUI* sender, CUICanvas* canvas);
			public:
				VOID SetPixel(INT x, INT y, const COLORREF color) const;
				VOID SetPixel(const POINT& p, const COLORREF color) const;
				VOID MoveTo(INT x, INT y) const;
				VOID MoveTo(const POINT& p) const;
				VOID LineTo(INT x, INT y) const;
				VOID LineTo(const POINT& p) const;
				VOID Rectangle(const RECT& area) const;
				VOID Rectangle(const CRECT& area) const;
				VOID RoundRect(const RECT& area, const INT w, const INT h) const;
				VOID RoundRect(const CRECT& area, const INT w, const INT h) const;
				VOID Ellipse(const RECT& area) const;
				VOID Ellipse(const CRECT& area) const;
				VOID Fill(const COLORREF color);
				VOID Fill(const RECT& area, const COLORREF color);
				VOID Fill(const CRECT& area, const COLORREF color);
				VOID FillWithTransparent(const RECT& area, const COLORREF color, BYTE percent);
				VOID FillWithTransparent(const CRECT& area, const COLORREF color, BYTE percent);
				VOID Fill(const LPCRECT area, const INT nCount, const COLORREF color);
				VOID Fill(const LPCCRECT area, const INT nCount, const COLORREF color);
				VOID EllipseFill(const RECT& area, const COLORREF color);
				VOID EllipseFill(const CRECT& area, const COLORREF color);
				VOID GradientFill(const RECT& area, const COLORREF start_color, const COLORREF end_color, const CUIGradientDirect direct);
				VOID GradientFill(const CRECT& area, const COLORREF start_color, const COLORREF end_color, const CUIGradientDirect direct);
				VOID Frame(const COLORREF color);
				VOID Frame(const RECT& area, const COLORREF color);
				VOID Frame(const CRECT& area, const COLORREF color);
				VOID GradientFrame(const RECT& area, const COLORREF start_color, const COLORREF end_color, const CUIGradientDirect direct);
				VOID GradientFrame(const CRECT& area, const COLORREF start_color, const COLORREF end_color, const CUIGradientDirect direct);
				VOID Polygon(const LPPOINT ps, INT count) const;
				VOID Polygon(const std::vector<POINT>& p) const;
				VOID TextSize(LPCSTR text, INT& width, INT& height) const;
				inline INT TextWidth(LPCSTR text) const { INT w, h; TextSize(text, w, h); return w; }
				inline INT TextHeight(LPCSTR text) const { INT w, h; TextSize(text, w, h); return h; }
				VOID TextRect(RECT& area, LPCSTR text, UINT flags, UINT len = 0) const;
				VOID TextRect(CRECT& area, LPCSTR text, UINT flags, UINT len = 0) const;
				VOID TextRect(RECT& area, LPCWSTR text, UINT flags, UINT len = 0) const;
				VOID TextRect(CRECT& area, LPCWSTR text, UINT flags, UINT len = 0) const;
				VOID TextInput(INT x, INT y, LPCSTR text);
			public:
				VOID ToFileBitmap(LPCSTR fname);
			public:
				VOID IncludeRect(const RECT& area) const;
				VOID IncludeRect(const CRECT& area) const;
				VOID ExcludeRect(const RECT& area) const;
				VOID ExcludeRect(const CRECT& area) const;
				VOID Update(VOID) const;
				CRECT GetClipRect(VOID) const;
				inline HDC GetHandle(VOID) const { return m_hDC; }
				inline CUIPen& GetPen(VOID) { return m_fPen; }
				inline CUIBrush& GetBrush(VOID) { return m_fBrush; }
				inline CUIFont& GetFont(VOID) { return m_fFont; }
				VOID SetPen(const CUIPen& pen);
				VOID SetBrush(const CUIBrush& brush);
				VOID SetFont(const CUIFont& font);
				BOOL GetTransparentMode(VOID) const;
				VOID SetTransparentMode(BOOL value);
				COLORREF GetColorText(VOID) const;
				VOID SetColorText(COLORREF value);
			public:
				CUICanvas(HDC hDC);
				CUICanvas(const CUICanvas& canvas);
				virtual ~CUICanvas(VOID);
			public:
				PROPERTY(GetTransparentMode, SetTransparentMode) BOOL TransparentMode;
				PROPERTY(GetColorText, SetColorText) COLORREF ColorText;
				READ_PROPERTY(GetHandle) HDC Handle;
				PROPERTY(GetPen, SetPen) CUIPen& Pen;
				PROPERTY(GetBrush, SetBrush) CUIBrush& Brush;
				PROPERTY(GetFont, SetFont) CUIFont& Font;
			};

			class CUIMonitor
			{
			private:
				HMONITOR m_fHandle;
			public:
				inline HMONITOR GetHandle(VOID) const { return m_fHandle; }
			public:
				BOOL IsPrimary(VOID) const;
				CRECT GetRect(VOID) const;
				CRECT GetWorkAreaRect(VOID) const;
			public:
				CUIMonitor(HMONITOR handle) : m_fHandle(handle) {}
				CUIMonitor(const CUIMonitor& monitor) : m_fHandle(monitor.m_fHandle) {}
			public:
				READ_PROPERTY(GetHandle) HMONITOR Handle;
				READ_PROPERTY(GetRect) CRECT Rect;
				READ_PROPERTY(GetWorkAreaRect) CRECT WorkAreaRect;
			};

			class CUIScreen
			{
			private:
				INT m_nVirtualWidth, m_nVirtualHeight;
			public:
				inline INT GetWidth(VOID) const { return m_nVirtualWidth; }
				inline INT GetHeight(VOID) const { return m_nVirtualHeight; }
				CUIMonitor MonitorFromPoint(const POINT& p) const;
				CUIMonitor MonitorFromRect(const RECT& rect) const;
				CUIMonitor MonitorFromRect(const CRECT& rect) const;
				CUIMonitor MonitorFromWindow(HWND handle) const;
			public:
				CUIScreen(VOID);
			public:
				READ_PROPERTY(GetWidth) INT Width;
				READ_PROPERTY(GetHeight) INT Height;
			};

			extern CUIScreen Screen;
		}
	}
}