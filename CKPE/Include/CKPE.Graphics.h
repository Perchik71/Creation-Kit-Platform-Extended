// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once
#define CKPE_GRAPHICS

#include <cstdint>
#include <string>
#include <vector>
#include <set>

#include <CKPE.Stream.h>
#include <CKPE.Math.h>

namespace CKPE
{
	class CKPE_API Canvas;
	class CKPE_API ObjectGUI;

	typedef void (*OnObjectGUIChangeEvent)(ObjectGUI* sender, Canvas* canvas);
	typedef void* THandle;
	typedef std::uint32_t TColor;

	class CKPE_API ObjectGUI
	{
	public:
		using _Canvas = Canvas;
	protected:
		std::uint32_t m_fType{ 0 };
		THandle m_fHandle{ nullptr };
	protected:
		virtual void Release() noexcept(true);
		void DoChange() noexcept(true);
	public:
		[[nodiscard]] inline std::uint32_t GetObjectType() const noexcept(true) { return m_fType; }
		[[nodiscard]] inline THandle GetHandle() const noexcept(true) { return m_fHandle; }
		virtual THandle Select(const _Canvas& canvas) const noexcept(true);
		virtual THandle Select() const noexcept(true);
	public:
		ObjectGUI(std::uint32_t uType) noexcept(true) : m_fType(uType) {}
		ObjectGUI(const ObjectGUI &obj) noexcept(true) : m_fType(obj.m_fType), OnChange(obj.OnChange), m_fHandle(obj.m_fHandle),
			Canvas(obj.Canvas) {}
		virtual ~ObjectGUI() noexcept(true);
	public:
		_Canvas* Canvas{ nullptr };
		OnObjectGUIChangeEvent OnChange{ nullptr };
	public:
		std::ptrdiff_t Tag{ 0 };
	public:
		CKPE_READ_PROPERTY(GetObjectType) std::uint32_t ObjectType;
		CKPE_READ_PROPERTY(GetHandle) THandle Handle;
	};

	class CKPE_API Bitmap : public ObjectGUI, public FileStreamIntf
	{
	protected:
		void Create(std::int32_t width, std::int32_t height, std::int32_t bpp);
		void Create(const std::wstring& fname) noexcept(true);
		void Create(THandle hinst, std::uint32_t dwResId) noexcept(true);
		void Create(THandle hinst, std::uint32_t dwResId, const wchar_t* name) noexcept(true);
	public:
		virtual void FreeImage() noexcept(true);
		virtual bool Empty() const noexcept(true) { return !m_fHandle; };
		virtual bool LoadFromResource(THandle hinst, std::uint32_t dwResId) noexcept(true);
		virtual bool LoadFromResource(THandle hinst, std::uint32_t dwResId, const wchar_t* name) noexcept(true);
		virtual bool LoadFromStream(Stream& stream) noexcept(true);
		virtual bool SaveToStream(Stream& stream) const noexcept(true);
		virtual void Assign(const Bitmap& bitmap) noexcept(true);

		Bitmap& operator=(const Bitmap& bitmap) noexcept(true);
	public:
		Bitmap() noexcept(true);
		Bitmap(THandle bitmap) noexcept(true);
		Bitmap(std::int32_t width, std::int32_t height, std::int32_t bpp);
		Bitmap(const std::wstring& fname) noexcept(true);
		Bitmap(THandle hinst, std::uint32_t dwResId) noexcept(true);
		Bitmap(THandle hinst, std::uint32_t dwResId, const wchar_t* name) noexcept(true);
		Bitmap(const Bitmap& bitmap) noexcept(true);
	};

	enum PenStyle : std::uint32_t
	{
		psClear = 0,
		psSolid,
		psDash,
		psDot,
		psDashDot,
		psDashDotDot,
		psInsideFrame,
	};

	class CKPE_API Pen : public ObjectGUI
	{
	private:
		std::int32_t m_fSize{ 0 };
		TColor m_fColor{ 0 };
		PenStyle m_fStyle{ PenStyle::psSolid };
	protected:
		void Create(PenStyle style, std::int32_t width, TColor color) noexcept(true);
	public:
		[[nodiscard]] inline virtual std::int32_t GetWidth() const noexcept(true) { return m_fSize; }
		[[nodiscard]] inline virtual TColor GetColor() const noexcept(true) { return m_fColor; }
		[[nodiscard]] inline virtual PenStyle GetStyle() const noexcept(true) { return m_fStyle; }
		virtual void SetWidth(const std::int32_t width) noexcept(true);
		virtual void SetColor(const TColor color) noexcept(true);
		virtual void SetStyle(const PenStyle style) noexcept(true);
		virtual void Assign(const Pen& pen) noexcept(true);
	public:
		Pen(PenStyle style, std::int32_t width, TColor color) : ObjectGUI(2) { Create(style, width, color); }
		Pen(const Pen& pen) : ObjectGUI(pen) { Create(pen.m_fStyle, pen.m_fSize, pen.m_fColor); }

		Pen& operator=(const Pen& pen);
	public:
		CKPE_PROPERTY(GetWidth, SetWidth) const std::int32_t Width;
		CKPE_PROPERTY(GetColor, SetColor) const TColor Color;
		CKPE_PROPERTY(GetStyle, SetStyle) const PenStyle Style;
	};

	CKPE_API Pen CreateSolidPen(std::int32_t width, TColor color) noexcept(true);
	CKPE_API Pen CreateDashPen(std::int32_t width, TColor color) noexcept(true);
	CKPE_API Pen CreateDotPen(std::int32_t width, TColor color) noexcept(true);

	enum BrushStyle : std::uint32_t
	{
		bsClear = 0,
		bsSolid,
		bsHatch,
		bsBitmap,
	};

	class CKPE_API Brush : public ObjectGUI
	{
	public:
		using _Bitmap = Bitmap;
	private:
		std::int32_t m_fHatch{ 0 };
		TColor m_fColor{ 0 };
		Bitmap m_fBitmap;
		BrushStyle m_fStyle{ BrushStyle::bsSolid };
	protected:
		void Create(const TColor color) noexcept(true);
		void Create(const std::int32_t iHatch, const TColor color) noexcept(true);
		void Create(const _Bitmap& bitmap) noexcept(true);
	public:
		[[nodiscard]] inline virtual std::int32_t GetHatch() const noexcept(true) { return m_fHatch; }
		[[nodiscard]] inline virtual TColor GetColor() const noexcept(true) { return m_fColor; }
		[[nodiscard]] inline virtual BrushStyle GetStyle() const noexcept(true) { return m_fStyle; }
		[[nodiscard]] inline virtual const _Bitmap& GetBitmap() const noexcept(true) { return m_fBitmap; }
		virtual void SetHatch(const std::int32_t value) noexcept(true);
		virtual void SetColor(const TColor color) noexcept(true);
		virtual void SetStyle(const BrushStyle style) noexcept(true);
		virtual void SetBitmap(const _Bitmap& bitmap) noexcept(true);
		virtual void Assign(const Brush& brush) noexcept(true);
	public:
		Brush(const TColor color) : ObjectGUI(3) { Create(color); }
		Brush(const std::int32_t iHatch, const TColor color) : ObjectGUI(3) { Create(iHatch, color); }
		Brush(const _Bitmap& bitmap) : ObjectGUI(3) { Create(bitmap); }
		Brush(const Brush& brush);

		Brush& operator=(const Brush& brush);
	public:
		CKPE_PROPERTY(GetHatch, SetHatch) const std::int32_t Hatch;
		CKPE_PROPERTY(GetColor, SetColor) const TColor Color;
		CKPE_PROPERTY(GetStyle, SetStyle) const BrushStyle Style;
		CKPE_PROPERTY(GetBitmap, SetBitmap) const Bitmap& Bitmap;
	};

	CKPE_API Brush CreateSolidBrush(const TColor color) noexcept(true);
	CKPE_API Brush CreateHatchBrush(const std::int32_t iHatch, const TColor color) noexcept(true);
	CKPE_API Brush CreatePatternBrush(const Bitmap& bitmap) noexcept(true);

	enum CUIGradientDirect : std::uint32_t
	{
		gdHorz = 0,
		gdVert,
	};

	CKPE_API bool CreateGradientBrush(Brush& brush, const TColor start_color,
		const TColor end_color, const std::int32_t size, const CUIGradientDirect direct) noexcept(true);

	enum FontStyle : std::uint32_t
	{
		fsBold = 0,
		fsItalic,
		fsUnderline,
		fsStrikeOut,
	};

	typedef std::set<FontStyle> FontStyles;

	enum FontQuality : std::uint32_t
	{
		fqDefault = 0,
		fqClearType,
		fqClearTypeNatural,
		fqProof,
		fqAntialiased,
		fqNoAntialiased,
	};

	enum FontPitch : std::uint32_t
	{
		fpDefault = 0,
		fpFixed,
		fpVariable,
		fpMono,
	};

	class CKPE_API Font : public ObjectGUI
	{
	private:
		bool m_lock{ false };
		std::string* m_Name{ nullptr };
		FontStyles* m_FontStyles{ nullptr };
		FontQuality m_Quality{ FontQuality::fqClearType };
		FontPitch m_Pitch{ FontPitch::fpDefault };
		std::int32_t m_Height{ 0 };
		std::uint32_t m_ulCharSet{ 0 };
	private:
		void Change() noexcept(true);
	public:
		[[nodiscard]] virtual std::string GetName() const noexcept(true) { return m_Name ? *m_Name : ""; }
		virtual void SetName(const std::string& name) noexcept(true);
		[[nodiscard]] virtual std::int32_t GetSize() const noexcept(true);
		virtual void SetSize(const std::int32_t value) noexcept(true);
		[[nodiscard]] inline virtual std::int32_t GetHeight() const noexcept(true) { return m_Height; }
		virtual void SetHeight(const std::int32_t value) noexcept(true);
		[[nodiscard]] inline virtual FontStyles GetStyles() const noexcept(true) { return *m_FontStyles; }
		virtual void SetStyles(const FontStyles& styles) noexcept(true);
		[[nodiscard]] inline virtual FontQuality GetQuality() const noexcept(true) { return m_Quality; }
		virtual void SetQuality(const FontQuality quality) noexcept(true);
		[[nodiscard]] inline virtual FontPitch GetPitch() const noexcept(true) { return m_Pitch; }
		virtual void SetPitch(const FontPitch pitch) noexcept(true);
		[[nodiscard]] inline virtual std::uint32_t GetCharSet() const noexcept(true) { return m_ulCharSet; }
		virtual void SetCharSet(const std::uint32_t CharSet) noexcept(true);
	private:
		void Recreate() noexcept(true);
		void Recreate(const THandle hDC) noexcept(true);
	public:
		virtual void Apply(THandle window) const noexcept(true);
		virtual void Assign(const Font& font) noexcept(true);

		Font& operator=(const Font& font);
	public:
		CKPE_READ_PROPERTY(GetHandle) THandle Handle;
		// no dll interface (what??? okay)
		//CKPE_PROPERTY(GetName, SetName) const std::string Name;
		CKPE_PROPERTY(GetSize, SetSize) const std::int32_t Size;
		CKPE_PROPERTY(GetHeight, SetHeight) const std::int32_t Height;
		// no dll interface (what??? okay)
		//CKPE_PROPERTY(GetStyles, SetStyles) const FontStyles Styles;
		CKPE_PROPERTY(GetQuality, SetQuality) const FontQuality Quality;
		CKPE_PROPERTY(GetPitch, SetPitch) const FontPitch Pitch;
		CKPE_PROPERTY(GetCharSet, SetCharSet) const std::uint32_t CharSet;
	public:
		Font(const std::string& name, const std::int32_t size, const FontStyles& styles = {}, 
			const std::uint32_t ulCharSet = 1, const FontQuality quality = fqClearTypeNatural,
			const FontPitch pitch = fpVariable) noexcept(true);
		Font(const THandle hDC) noexcept(true);
		Font(const Font& parent) : ObjectGUI(parent), m_FontStyles(parent.m_FontStyles), 
			m_Quality(parent.m_Quality), m_ulCharSet(parent.m_ulCharSet), m_Pitch(parent.m_Pitch),
			m_Name(parent.m_Name), m_Height(parent.m_Height), m_lock(false) 
		{ Recreate(); }
	};

	class CKPE_API Canvas
	{
	public:
		using _Pen = Pen;
		using _Brush = Brush;
		using _Font = Font;
	private:
		THandle m_hDC{ nullptr };
		_Pen m_fPen;
		_Brush m_fBrush;
		_Font m_fFont;
		THandle m_hOldPen{ nullptr };
		THandle m_hOldBrush{ nullptr };
		THandle m_hOldFont{ nullptr };
	private:
		static void DoChange(ObjectGUI* sender, Canvas* canvas) noexcept(true);
	public:
		virtual void SetPixel(std::int32_t x, std::int32_t y, const TColor color) const noexcept(true);
		virtual void SetPixel(const POINT& p, const TColor color) const noexcept(true);
		virtual void SetPixel(const CPOINT& p, const TColor color) const noexcept(true);
		virtual void MoveTo(std::int32_t x, std::int32_t y) const noexcept(true);
		virtual void MoveTo(const POINT& p) const noexcept(true);
		virtual void MoveTo(const CPOINT& p) const noexcept(true);
		virtual void LineTo(std::int32_t x, std::int32_t y) const noexcept(true);
		virtual void LineTo(const POINT& p) const noexcept(true);
		virtual void LineTo(const CPOINT& p) const noexcept(true);
		virtual void Rectangle(const RECT& area) const noexcept(true);
		virtual void Rectangle(const CRECT& area) const noexcept(true);
		virtual void RoundRect(const RECT& area, const std::int32_t w, const std::int32_t h) const noexcept(true);
		virtual void RoundRect(const CRECT& area, const std::int32_t w, const std::int32_t h) const noexcept(true);
		virtual void Ellipse(const RECT& area) const noexcept(true);
		virtual void Ellipse(const CRECT& area) const noexcept(true);
		virtual void Fill(const TColor color) noexcept(true);
		virtual void Fill(const RECT& area, const TColor color) noexcept(true);
		virtual void Fill(const CRECT& area, const TColor color) noexcept(true);
		virtual void FillWithTransparent(const RECT& area, const TColor color, std::uint8_t percent) noexcept(true);
		virtual void FillWithTransparent(const CRECT& area, const TColor color, std::uint8_t percent) noexcept(true);
		virtual void Fill(LPCRECT area, const std::int32_t nCount, const TColor color) noexcept(true);
		virtual void Fill(const LPCCRECT area, const std::int32_t nCount, const TColor color) noexcept(true);
		virtual void EllipseFill(const RECT& area, const TColor color) noexcept(true);
		virtual void EllipseFill(const CRECT& area, const TColor color) noexcept(true);
		virtual void GradientFill(const RECT& area, const TColor start_color, const TColor end_color, 
			const CUIGradientDirect direct) noexcept(true);
		virtual void GradientFill(const CRECT& area, const TColor start_color, const TColor end_color,
			const CUIGradientDirect direct) noexcept(true);
		virtual void Frame(const TColor color) noexcept(true);
		virtual void Frame(const RECT& area, const TColor color) noexcept(true);
		virtual void Frame(const CRECT& area, const TColor color) noexcept(true);
		virtual void GradientFrame(const RECT& area, const TColor start_color, const TColor end_color, 
			const CUIGradientDirect direct) noexcept(true);
		virtual void GradientFrame(const CRECT& area, const TColor start_color, const TColor end_color,
			const CUIGradientDirect direct) noexcept(true);
		virtual void Polygon(const LPPOINT ps, std::int32_t count) const noexcept(true);
		virtual void Polygon(const std::vector<POINT>& p) const noexcept(true);
		virtual void Polygon(const LPCCPOINT ps, std::int32_t count) const noexcept(true);
		virtual void Polygon(const std::vector<CPOINT>& p) const noexcept(true);
		virtual void TextSize(const std::string& text, std::int32_t& width, std::int32_t& height) const noexcept(true);
		[[nodiscard]] inline virtual std::int32_t TextWidth(const std::string& text) const noexcept(true)
		{ std::int32_t w, h; TextSize(text, w, h); return w; }
		[[nodiscard]] inline virtual std::int32_t TextHeight(const std::string& text) const noexcept(true)
		{ std::int32_t w, h; TextSize(text, w, h); return h; }
		virtual void TextSize(const std::wstring& text, std::int32_t& width, std::int32_t& height) const noexcept(true);
		[[nodiscard]] inline virtual std::int32_t TextWidth(const std::wstring& text) const noexcept(true)
		{
			std::int32_t w, h; TextSize(text, w, h); return w;
		}
		[[nodiscard]] inline virtual std::int32_t TextHeight(const std::wstring& text) const noexcept(true)
		{
			std::int32_t w, h; TextSize(text, w, h); return h;
		}
		virtual void TextRect(RECT& area, const std::string& text, std::uint32_t flags, std::uint32_t len = 0) const noexcept(true);
		virtual void TextRect(CRECT& area, const std::string& text, std::uint32_t flags, std::uint32_t len = 0) const noexcept(true);
		virtual void TextRect(RECT& area, const std::wstring& text, std::uint32_t flags, std::uint32_t len = 0) const noexcept(true);
		virtual void TextRect(CRECT& area, const std::wstring& text, std::uint32_t flags, std::uint32_t len = 0) const noexcept(true);
		virtual void TextInput(std::int32_t x, std::int32_t y, const std::string& text) noexcept(true);
		virtual void TextInput(std::int32_t x, std::int32_t y, const std::wstring& text) noexcept(true);
	public:
		virtual void ToFileBitmap(const std::wstring& fname) noexcept(true);
	public:
		virtual void IncludeRect(const RECT& area) const noexcept(true);
		virtual void IncludeRect(const CRECT& area) const noexcept(true);
		virtual void ExcludeRect(const RECT& area) const noexcept(true);
		virtual void ExcludeRect(const CRECT& area) const noexcept(true);
		virtual void Update() const noexcept(true);
		[[nodiscard]] virtual CRECT GetClipRect() const noexcept(true);
		[[nodiscard]] inline virtual THandle GetHandle() const noexcept(true) { return m_hDC; }
		[[nodiscard]] inline virtual _Pen& GetPen() noexcept(true) { return m_fPen; }
		[[nodiscard]] inline virtual _Brush& GetBrush() noexcept(true) { return m_fBrush; }
		[[nodiscard]] inline virtual _Font& GetFont() noexcept(true) { return m_fFont; }
		virtual void SetPen(const _Pen& pen) noexcept(true);
		virtual void SetBrush(const _Brush& brush) noexcept(true);
		virtual void SetFont(const _Font& font) noexcept(true);
		[[nodiscard]] virtual bool GetTransparentMode() const noexcept(true);
		virtual void SetTransparentMode(bool value) noexcept(true);
		[[nodiscard]] virtual TColor GetColorText() const noexcept(true);
		virtual void SetColorText(TColor value) noexcept(true);
	public:
		Canvas(THandle hDC);
		Canvas(const Canvas& canvas);
		virtual ~Canvas();
	public:
		CKPE_PROPERTY(GetTransparentMode, SetTransparentMode) bool TransparentMode;
		CKPE_PROPERTY(GetColorText, SetColorText) TColor ColorText;
		CKPE_READ_PROPERTY(GetHandle) THandle Handle;
		CKPE_PROPERTY(GetPen, SetPen) Pen& Pen;
		CKPE_PROPERTY(GetBrush, SetBrush) Brush& Brush;
		CKPE_PROPERTY(GetFont, SetFont) Font& Font;
	};

	class CKPE_API Monitor
	{
	private:
		THandle m_fHandle;
	public:
		[[nodiscard]] inline virtual THandle GetHandle() const noexcept(true) { return m_fHandle; }
	public:
		[[nodiscard]] virtual bool IsPrimary() const noexcept(true);
		[[nodiscard]] virtual CRECT GetRect() const noexcept(true);
		[[nodiscard]] virtual CRECT GetWorkAreaRect() const noexcept(true);
	public:
		Monitor(THandle handle) : m_fHandle(handle) {}
		Monitor(const Monitor& monitor) : m_fHandle(monitor.m_fHandle) {}
	public:
		CKPE_READ_PROPERTY(GetHandle) THandle Handle;
		CKPE_READ_PROPERTY(GetRect) CRECT Rect;
		CKPE_READ_PROPERTY(GetWorkAreaRect) CRECT WorkAreaRect;
	};

	class CKPE_API Screen
	{
	private:
		std::int32_t m_nVirtualWidth, m_nVirtualHeight;
	public:
		[[nodiscard]] inline virtual std::int32_t GetWidth() const noexcept(true) { return m_nVirtualWidth; }
		[[nodiscard]] inline virtual std::int32_t GetHeight() const noexcept(true) { return m_nVirtualHeight; }
		[[nodiscard]] virtual Monitor MonitorFromPoint(const POINT& p) const noexcept(true);
		[[nodiscard]] virtual Monitor MonitorFromRect(const RECT& rect) const noexcept(true);
		[[nodiscard]] virtual Monitor MonitorFromRect(const CRECT& rect) const noexcept(true);
		[[nodiscard]] virtual Monitor MonitorFromWindow(THandle handle) const noexcept(true);
	public:
		Screen();
		static Screen* GetSingleton() noexcept(true);
	public:
		CKPE_READ_PROPERTY(GetWidth) std::int32_t Width;
		CKPE_READ_PROPERTY(GetHeight) std::int32_t Height;
	};
}