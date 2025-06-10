// Copyright © 2025 aka perchik71. All rights reserved.
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
			pBmp->GetHBITMAP(0, &Result);
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
		const std::wstring& ResType) noexcept(true)
	{
		// https://code911.top/howto/c-gdi-how-to-get-and-load-image-from-resource

		HBITMAP Result;

		IStream* pStream = nullptr;
		Gdiplus::Bitmap* pBmp = nullptr;
		HGLOBAL hGlobal = nullptr;
		// get the handle to the resource
		HRSRC hrsrc = FindResourceW((HINSTANCE)hInst, MAKEINTRESOURCEW(dwResId), ResType.c_str());
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
			pBmp->GetHBITMAP(0, &Result);
			delete pBmp;
		}

		return Result;
	}

	GDIPlus* GDIPlus::GetSingleton() noexcept(true)
	{
		return &_sgdiplus;
	}
}