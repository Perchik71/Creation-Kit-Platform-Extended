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

#include "UIImageList.h"

namespace Core
{
	namespace Classes
	{
		namespace UI
		{
			// https://stackoverflow.com/questions/24720451/save-hbitmap-to-bmp-file-using-only-win32

			PBITMAPINFO CreateBitmapInfoStruct(HBITMAP hBmp)
			{
				BITMAP bmp = { 0 };
				PBITMAPINFO pbmi;
				WORD    cClrBits;

				// Retrieve the bitmap color format, width, and height.  
				Assert(GetObjectA(hBmp, sizeof(BITMAP), (LPSTR)&bmp));

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

			void CreateBMPFile(LPCSTR pszFile, HBITMAP hBMP)
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

				pbi = CreateBitmapInfoStruct(hBMP);

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
				hdr.bfOffBits = (DWORD)((size_t)sizeof(BITMAPFILEHEADER) +
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


			VOID CUIImageList::ReleaseImgList(VOID)
			{
				if (IsCreated())
				{
					ImageList_Destroy(m_hHandle);
					m_hHandle = NULL;
					m_nSizeX = 0;
					m_nSizeY = 0;
				}
			}

			VOID CUIImageList::CreateImgList(INT nSizeX, INT nSizeY, BOOL bMasked, CUIImageListColorType eColorType)
			{
				Assert(!m_hHandle);
				Assert(((nSizeX > 0) && (nSizeY > 0)));

				UINT uFlag = 0;

				if (eColorType == ilct24Bit)
					uFlag = ILC_COLOR24;
				else if (eColorType == ilct32Bit)
					uFlag = ILC_COLOR32;
				else
					uFlag = ILC_COLOR;

				m_hHandle = ImageList_Create(nSizeX, nSizeY, uFlag | (bMasked ? ILC_MASK : 0), 0, 0);
				Assert(m_hHandle);

				m_nSizeX = nSizeX;
				m_nSizeY = nSizeY;
				m_eColorType = eColorType;
				m_bMasked = bMasked;
			}

			VOID CUIImageList::ReCreateImgList(INT nSizeX, INT nSizeY, BOOL bMasked, CUIImageListColorType eColorType)
			{
				ReleaseImgList();
				CreateImgList(nSizeX, nSizeY, bMasked, eColorType);
			}

			VOID CUIImageList::ReCreate(INT width, INT height, BOOL bMasked, CUIImageListColorType eColorType)
			{
				ReCreateImgList(width, height, bMasked, eColorType);
			}

			HBITMAP CUIImageList::LoadBitmapFromFile(LPCSTR fname)
			{
				return (HBITMAP)LoadImageA(NULL, fname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			}

			HBITMAP CUIImageList::LoadBitmapFromResource(HINSTANCE instance, LPCSTR name)
			{
				return (HBITMAP)LoadImageA(instance, name, IMAGE_BITMAP, 0, 0, 0);
			}

			BOOL CUIImageList::GetBitmap(INT idx, HBITMAP& bitmap) const
			{
				HDC hDTDC = GetDC(NULL);
				HDC hDestDC = CreateCompatibleDC(hDTDC);
				bitmap = CreateCompatibleBitmap(hDTDC, Width, Height);
				bitmap = (HBITMAP)SelectObject(hDestDC, bitmap);
				BOOL bRes = ImageList_Draw(m_hHandle, idx, hDestDC, 0, 0, ILD_NORMAL);
				bitmap = (HBITMAP)SelectObject(hDestDC, bitmap);
				DeleteDC(hDestDC);
				ReleaseDC(NULL, hDTDC);
				return bRes;
			}

			BOOL CUIImageList::ExtractToFile(INT idx, LPCSTR fname) const
			{
				HBITMAP hBitmapRet;
				BOOL bRes = GetBitmap(idx, hBitmapRet);
				if (bRes) CreateBMPFile(fname, hBitmapRet);
				DeleteObject(hBitmapRet);
				return bRes;
			}

			VOID CUIImageList::Assign(const HIMAGELIST imglst, INT width, INT height, BOOL bMasked, CUIImageListColorType eColorType)
			{
				ReleaseImgList();

				if (imglst)
				{
					m_nSizeX = width;
					m_nSizeY = height;
					m_eColorType = eColorType;
					m_bMasked = bMasked;
					m_hHandle = ImageList_Duplicate(imglst);
				}
			}

			VOID CUIImageList::Assign(const CUIImageList* imglst)
			{
				ReleaseImgList();

				if (imglst && imglst->IsCreated())
				{
					m_nSizeX = imglst->Width;
					m_nSizeY = imglst->Height;
					m_eColorType = imglst->ColorType;
					m_bMasked = imglst->Masked;
					m_hHandle = ImageList_Duplicate(imglst->Handle);
				}
			}

			VOID CUIImageList::AssignTo(CUIImageList* imglst) const
			{
				imglst->Assign(this);
			}

			CUIImageList CUIImageList::Dublicate(VOID) const
			{
				return *this;
			}

			INT CUIImageList::GetCount(VOID)
			{
				if (IsCreated())
					return ImageList_GetImageCount(m_hHandle);
				else
					return 0;
			}

			VOID CUIImageList::Clear(VOID)
			{
				Assert(IsCreated());
				ImageList_RemoveAll(m_hHandle);
			}

			INT CUIImageList::Add(HBITMAP bitmap, HBITMAP hmask)
			{
				Assert(IsCreated());
				return ImageList_Add(m_hHandle, bitmap, hmask);
			}

			INT CUIImageList::Add(HBITMAP bitmap, COLORREF cmask)
			{
				Assert(IsCreated());
				return ImageList_AddMasked(m_hHandle, bitmap, cmask);
			}

			INT CUIImageList::AddFromFile(LPCSTR bitmap, LPCSTR mask)
			{
				Assert(IsCreated());
				Assert(bitmap);

				HBITMAP hMask = NULL;
				HBITMAP hBitmap = LoadBitmapFromFile(bitmap);
				if (!hBitmap) return -1;

				if (mask)
					hMask = LoadBitmapFromFile(mask);
				
				INT nRes = ImageList_Add(m_hHandle, hBitmap, hMask);

				DeleteObject(hBitmap);
				DeleteObject(hMask);
				return nRes;
			}

			INT CUIImageList::AddFromFile(LPCSTR bitmap, COLORREF cmask)
			{
				Assert(IsCreated());
				Assert(bitmap);

				HBITMAP hBitmap = LoadBitmapFromFile(bitmap);
				if (!hBitmap) return -1;

				INT nRes = ImageList_AddMasked(m_hHandle, hBitmap, cmask);

				DeleteObject(hBitmap);
				return nRes;
			}

			INT CUIImageList::AddFromResource(HINSTANCE instance, LPCSTR bitmap, LPCSTR mask)
			{
				Assert(IsCreated());
				Assert(bitmap);

				HBITMAP hMask = NULL;
				HBITMAP hBitmap = LoadBitmapFromResource(instance, bitmap);
				if (!hBitmap) return -1;

				if (mask)
					hMask = LoadBitmapFromResource(instance, mask);

				INT nRes = ImageList_Add(m_hHandle, hBitmap, hMask);

				DeleteObject(hBitmap);
				DeleteObject(hMask);
				return nRes;
			}

			INT CUIImageList::AddFromResource(HINSTANCE instance, LPCSTR bitmap, COLORREF cmask)
			{
				Assert(IsCreated());
				Assert(bitmap);

				HBITMAP hBitmap = LoadBitmapFromResource(instance, bitmap);
				if (!hBitmap) return -1;

				INT nRes = ImageList_AddMasked(m_hHandle, hBitmap, cmask);

				DeleteObject(hBitmap);
				return nRes;
			}

			BOOL CUIImageList::Replace(INT idx, HBITMAP bitmap, HBITMAP hmask)
			{
				Assert(IsCreated());
				return ImageList_Replace(m_hHandle, idx, bitmap, hmask);
			}

			BOOL CUIImageList::ReplaceFromFile(INT idx, LPCSTR bitmap, LPCSTR mask)
			{
				Assert(IsCreated());
				Assert(bitmap);

				HBITMAP hMask = NULL;
				HBITMAP hBitmap = LoadBitmapFromFile(bitmap);
				if (!hBitmap) return -1;

				if (mask)
					hMask = LoadBitmapFromFile(mask);

				INT nRes = ImageList_Replace(m_hHandle, idx, hBitmap, hMask);

				DeleteObject(hBitmap);
				DeleteObject(hMask);
				return nRes;
			}

			BOOL CUIImageList::ReplaceFromResource(INT idx, HINSTANCE instance, LPCSTR bitmap, LPCSTR mask)
			{
				Assert(IsCreated());
				Assert(bitmap);

				HBITMAP hMask = NULL;
				HBITMAP hBitmap = LoadBitmapFromResource(instance, bitmap);
				if (!hBitmap) return -1;

				if (mask)
					hMask = LoadBitmapFromResource(instance, mask);

				INT nRes = ImageList_Replace(m_hHandle, idx, hBitmap, hMask);

				DeleteObject(hBitmap);
				DeleteObject(hMask);
				return nRes;
			}

			BOOL CUIImageList::Remove(INT idx)
			{
				Assert(IsCreated());
				return ImageList_Remove(m_hHandle, idx);
			}

			BOOL CUIImageList::IsCreated(VOID) const 
			{ 
				return (m_hHandle) && (m_nSizeX > 0) && (m_nSizeY > 0); 
			}

			VOID CUIImageList::ReSize(INT width, INT height)
			{
				ReCreateImgList(width, height, m_bMasked, m_eColorType);
			}

			VOID CUIImageList::SetWidth(INT width)
			{
				ReCreateImgList(width, m_nSizeY, m_bMasked, m_eColorType);
			}

			VOID CUIImageList::SetHeight(INT height)
			{
				ReCreateImgList(m_nSizeX, height, m_bMasked, m_eColorType);
			}

			VOID CUIImageList::SetMasked(BOOL value)
			{
				ReCreateImgList(m_nSizeX, m_nSizeY, value, m_eColorType);
			}

			VOID CUIImageList::SetColorType(CUIImageListColorType etype)
			{
				ReCreateImgList(m_nSizeX, m_nSizeY, m_bMasked, etype);
			}

			CUIImageList::CUIImageList(VOID) : m_hHandle(NULL), m_nSizeX(0), m_nSizeY(0), m_eColorType(ilct32Bit), m_bMasked(FALSE)
			{}

			CUIImageList::CUIImageList(INT nSizeX, INT nSizeY) : m_hHandle(NULL), m_nSizeX(0), m_nSizeY(0), m_eColorType(ilct32Bit), m_bMasked(FALSE)
			{ 
				CreateImgList(nSizeX, nSizeY, FALSE, ilct32Bit);
			}

			CUIImageList::CUIImageList(INT nSizeX, INT nSizeY, CUIImageListColorType eColorType) : m_hHandle(NULL), m_nSizeX(0), m_nSizeY(0), m_eColorType(eColorType), m_bMasked(FALSE)
			{ 
				CreateImgList(nSizeX, nSizeY, FALSE, eColorType);
			}

			CUIImageList::CUIImageList(const CUIImageList& imglst) : m_hHandle(NULL), m_nSizeX(imglst.Width), m_nSizeY(imglst.Height), m_eColorType(imglst.ColorType), m_bMasked(imglst.Masked)
			{
				if (imglst.IsCreated())
				  m_hHandle = ImageList_Duplicate(imglst.Handle);
			}

			CUIImageList::~CUIImageList(VOID)
			{ 
				ReleaseImgList();
			}
		}
	}
}