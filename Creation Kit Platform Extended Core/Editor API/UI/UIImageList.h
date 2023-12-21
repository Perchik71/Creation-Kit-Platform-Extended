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
			enum CUIImageListColorType {
				ilct8Bit,
				ilct24Bit,
				ilct32Bit
			};

			class CUIImageList
			{
			private:
				INT m_nSizeX, m_nSizeY;
				CUIImageListColorType m_eColorType;
				HIMAGELIST m_hHandle;
				BOOL m_bMasked;
			protected:
				VOID CreateImgList(INT nSizeX, INT nSizeY, BOOL bMasked = FALSE, CUIImageListColorType eColorType = ilct32Bit);
				VOID ReCreateImgList(INT nSizeX, INT nSizeY, BOOL bMasked = FALSE, CUIImageListColorType eColorType = ilct32Bit);
				VOID ReleaseImgList(VOID);
				HBITMAP LoadBitmapFromFile(LPCSTR fname);
				HBITMAP LoadBitmapFromResource(HINSTANCE instance, LPCSTR name);
			public:
				inline HIMAGELIST GetHandle(VOID) const { return m_hHandle; }
				inline INT GetWidth(VOID) const { return m_nSizeX; }
				inline INT GetHeight(VOID) const { return m_nSizeY; }
				inline BOOL IsMasked(VOID) const { return m_bMasked; }
				inline CUIImageListColorType GetColorType(VOID) const { return m_eColorType; }
				VOID SetWidth(INT width);
				VOID SetHeight(INT height);
				VOID SetMasked(BOOL value);
				VOID SetColorType(CUIImageListColorType etype);
				VOID ReSize(INT width, INT height);
				VOID ReCreate(INT width, INT height, BOOL bMasked, CUIImageListColorType eColorType);
				VOID Assign(const HIMAGELIST imglst, INT width, INT height, BOOL bMasked, CUIImageListColorType eColorType);
				VOID Assign(const CUIImageList* imglst);
				VOID AssignTo(CUIImageList *imglst) const;
				CUIImageList Dublicate(VOID) const;
				BOOL IsCreated(VOID) const;
			public:
				BOOL GetBitmap(INT idx, HBITMAP &bitmap) const;
				BOOL ExtractToFile(INT idx, LPCSTR fname) const;
			public:
				INT GetCount(VOID);
				INT Add(HBITMAP bitmap, HBITMAP hmask = NULL);
				INT Add(HBITMAP bitmap, COLORREF cmask);
				INT AddFromFile(LPCSTR bitmap, LPCSTR mask = NULL);
				INT AddFromFile(LPCSTR bitmap, COLORREF cmask);
				INT AddFromResource(HINSTANCE instance, LPCSTR bitmap, LPCSTR mask = NULL);
				INT AddFromResource(HINSTANCE instance, LPCSTR bitmap, COLORREF cmask);
				BOOL Replace(INT idx, HBITMAP bitmap, HBITMAP hmask = NULL);
				BOOL ReplaceFromFile(INT idx, LPCSTR bitmap, LPCSTR mask = NULL);
				BOOL ReplaceFromResource(INT idx, HINSTANCE instance, LPCSTR bitmap, LPCSTR mask = NULL);
				BOOL Remove(INT idx);
				VOID Clear(VOID);
			public:
				CUIImageList(VOID);
				CUIImageList(INT nSizeX, INT nSizeY);
				CUIImageList(INT nSizeX, INT nSizeY, CUIImageListColorType eColorType);
				CUIImageList(const CUIImageList &imglst);
				virtual ~CUIImageList(VOID);
			public:
				READ_PROPERTY(GetHandle) HIMAGELIST Handle;
				PROPERTY(GetWidth, SetWidth) const INT Width;
				PROPERTY(GetHeight, SetHeight) const INT Height;
				PROPERTY(IsMasked, SetMasked) const BOOL Masked;
				PROPERTY(GetColorType, SetColorType) const CUIImageListColorType ColorType;
			};
		}
	}
}