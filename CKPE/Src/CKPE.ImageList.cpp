// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <commctrl.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Exception.h>
#include <CKPE.ImageList.h>

namespace CKPE
{
	namespace Common
	{
		namespace UI
		{
			void ImageList::CreateImgList(std::int32_t nSizeX, std::int32_t nSizeY, bool bMasked, 
				ImageListColorType eColorType)
			{
				if (m_hHandle || !nSizeX || !nSizeY)
					return;

				UINT uFlag = 0;

				if (eColorType == ilct24Bit)
					uFlag = ILC_COLOR24;
				else if (eColorType == ilct32Bit)
					uFlag = ILC_COLOR32;
				else
					uFlag = ILC_COLOR;

				m_hHandle = (THandle)ImageList_Create(nSizeX, nSizeY, uFlag | (bMasked ? ILC_MASK : 0), 0, 0);
				if (!m_hHandle) RuntimeError("ImageList_Create return failed");

				m_nSizeX = nSizeX;
				m_nSizeY = nSizeY;
				m_eColorType = eColorType;
				m_bMasked = bMasked;
			}

			void ImageList::ReCreateImgList(std::int32_t nSizeX, std::int32_t nSizeY, bool bMasked, 
				ImageListColorType eColorType)
			{
				ReleaseImgList();
				CreateImgList(nSizeX, nSizeY, bMasked, eColorType);
			}


			void ImageList::ReleaseImgList() noexcept(true)
			{
				if (IsCreated())
				{
					ImageList_Destroy((HIMAGELIST)m_hHandle);
					m_hHandle = nullptr;
					m_nSizeX = 0;
					m_nSizeY = 0;
				}
			}

			void ImageList::SetWidth(std::int32_t width)
			{
				ReCreateImgList(width, m_nSizeY, m_bMasked, m_eColorType);
			}

			void ImageList::SetHeight(std::int32_t height)
			{
				ReCreateImgList(m_nSizeX, height, m_bMasked, m_eColorType);
			}

			void ImageList::SetMasked(bool value)
			{
				ReCreateImgList(m_nSizeX, m_nSizeY, value, m_eColorType);
			}

			void ImageList::SetColorType(ImageListColorType etype)
			{
				ReCreateImgList(m_nSizeX, m_nSizeY, m_bMasked, etype);
			}

			void ImageList::ReSize(std::int32_t width, std::int32_t height)
			{
				ReCreateImgList(width, height, m_bMasked, m_eColorType);
			}

			void ImageList::ReCreate(std::int32_t width, std::int32_t height, bool bMasked, ImageListColorType eColorType)
			{
				ReCreateImgList(width, height, bMasked, eColorType);
			}

			void ImageList::Assign(const THandle imglst, std::int32_t width, std::int32_t height,
				bool bMasked, ImageListColorType eColorType) noexcept(true)
			{
				ReleaseImgList();

				if (imglst)
				{
					m_nSizeX = width;
					m_nSizeY = height;
					m_eColorType = eColorType;
					m_bMasked = bMasked;
					m_hHandle = (THandle)ImageList_Duplicate((HIMAGELIST)imglst);
				}
			}

			void ImageList::Assign(const ImageList* imglst) noexcept(true)
			{
				ReleaseImgList();

				if (imglst && imglst->IsCreated())
				{
					m_nSizeX = imglst->Width;
					m_nSizeY = imglst->Height;
					m_eColorType = imglst->ColorType;
					m_bMasked = imglst->Masked;
					m_hHandle = (THandle)ImageList_Duplicate((HIMAGELIST)imglst->Handle);
				}
			}

			void ImageList::AssignTo(ImageList* imglst) const noexcept(true)
			{
				imglst->Assign(this);
			}

			ImageList ImageList::Dublicate() const noexcept(true)
			{
				return *this;
			}

			bool ImageList::IsCreated() const noexcept(true)
			{
				return (m_hHandle) && (m_nSizeX > 0) && (m_nSizeY > 0);
			}

			bool ImageList::GetBitmap(std::int32_t idx, THandle& bitmap) const noexcept(true)
			{
				HDC hDTDC = GetDC(NULL);
				HDC hDestDC = CreateCompatibleDC(hDTDC);
				bitmap = (THandle)CreateCompatibleBitmap(hDTDC, Width, Height);
				bitmap = (THandle)SelectObject(hDestDC, bitmap);
				bool bRes = (bool)ImageList_Draw((HIMAGELIST)m_hHandle, idx, hDestDC, 0, 0, ILD_NORMAL);
				bitmap = (THandle)SelectObject(hDestDC, bitmap);
				DeleteDC(hDestDC);
				ReleaseDC(NULL, hDTDC);
				return bRes;
			}

			bool ImageList::ExtractToFile(std::int32_t idx, const std::wstring& fname) const noexcept(true)
			{
				THandle hBitmapRet;
				bool bRes = GetBitmap(idx, hBitmapRet);
				if (bRes && hBitmapRet)
				{
					Bitmap bm(hBitmapRet);
					bm.SaveToFile(fname);
				}
				return bRes;
			}

			std::int32_t ImageList::GetCount() const noexcept(true)
			{
				return IsCreated() ? ImageList_GetImageCount((HIMAGELIST)m_hHandle) : 0;
			}

			std::int32_t ImageList::Add(THandle bitmap, THandle hmask) noexcept(true)
			{
				if (!IsCreated()) return -1;
				return (std::int32_t)ImageList_Add((HIMAGELIST)m_hHandle, (HBITMAP)bitmap, (HBITMAP)hmask);
			}

			std::int32_t ImageList::Add(THandle bitmap, TColor cmask) noexcept(true)
			{
				if (!IsCreated()) return -1;
				return (std::int32_t)ImageList_AddMasked((HIMAGELIST)m_hHandle, (HBITMAP)bitmap, cmask);
			}

			std::int32_t ImageList::AddFromFile(const std::wstring& fname, const std::wstring& fname_mask) noexcept(true)
			{
				if (!IsCreated() || fname.empty() || !PathUtils::FileExists(fname)) return -1;

				Bitmap bm(fname);
				Bitmap bmmask;
				
				if ((fname_mask.length() > 0) && PathUtils::FileExists(fname_mask))
					bmmask.LoadFromFile(fname_mask);

				return (!bm.Empty()) ? (std::int32_t)ImageList_Add((HIMAGELIST)m_hHandle, (HBITMAP)bm.Handle, 
					(HBITMAP)bmmask.Handle) : -1;
			}

			std::int32_t ImageList::AddFromFile(const std::wstring& fname, TColor cmask) noexcept(true)
			{
				if (!IsCreated() || fname.empty() || !PathUtils::FileExists(fname)) return -1;

				Bitmap bm(fname);
				return (!bm.Empty()) ? (std::int32_t)ImageList_AddMasked((HIMAGELIST)m_hHandle, (HBITMAP)bm.Handle, 
					cmask) : -1;
			}

			std::int32_t ImageList::AddFromResource(THandle hinst, std::uint32_t dwResId, const std::wstring& name,
				std::uint32_t dwResMaskId) noexcept(true)
			{
				if (!IsCreated()) return -1;

				Bitmap bm(hinst, dwResId, name);
				Bitmap bmmask;

				if (dwResMaskId)
					bmmask.LoadFromResource(hinst, dwResMaskId, name);

				return (!bm.Empty()) ? (std::int32_t)ImageList_Add((HIMAGELIST)m_hHandle, (HBITMAP)bm.Handle,
					(HBITMAP)bmmask.Handle) : -1;
			}

			std::int32_t ImageList::AddFromResourceMasked(THandle hinst, std::uint32_t dwResId, const std::wstring& name,
				TColor cmask) noexcept(true)
			{
				if (!IsCreated()) return -1;

				Bitmap bm(hinst, dwResId, name);
				return (!bm.Empty()) ? (std::int32_t)ImageList_AddMasked((HIMAGELIST)m_hHandle, (HBITMAP)bm.Handle, 
					cmask) : -1;
			}

			bool ImageList::Replace(std::int32_t idx, THandle bitmap, THandle hmask) noexcept(true)
			{
				if (!IsCreated()) return false;
				return ImageList_Replace((HIMAGELIST)m_hHandle, idx, (HBITMAP)bitmap, (HBITMAP)hmask);
			}

			bool ImageList::ReplaceFromFile(std::int32_t idx, const std::wstring& fname, 
				const std::wstring& fname_mask) noexcept(true)
			{
				if (!IsCreated() || fname.empty() || !PathUtils::FileExists(fname)) return false;

				Bitmap bm(fname);
				Bitmap bmmask;

				if ((fname_mask.length() > 0) && PathUtils::FileExists(fname_mask))
					bmmask.LoadFromFile(fname_mask);

				return (!bm.Empty()) ? (std::int32_t)ImageList_Replace((HIMAGELIST)m_hHandle, idx, (HBITMAP)bm.Handle,
					(HBITMAP)bmmask.Handle) : -1;
			}

			bool ImageList::ReplaceFromResource(std::int32_t idx, THandle hinst, std::uint32_t dwResId,
				const std::wstring& name, std::uint32_t dwResMaskId) noexcept(true)
			{
				if (!IsCreated()) return false;

				Bitmap bm(hinst, dwResId, name);
				Bitmap bmmask;

				if (dwResMaskId)
					bmmask.LoadFromResource(hinst, dwResMaskId, name);

				return (!bm.Empty()) ? (std::int32_t)ImageList_Replace((HIMAGELIST)m_hHandle, idx, (HBITMAP)bm.Handle,
					(HBITMAP)bmmask.Handle) : -1;
			}

			bool ImageList::Remove(std::int32_t idx) noexcept(true)
			{
				if (!IsCreated()) return false;
				return ImageList_Remove((HIMAGELIST)m_hHandle, idx);
			}

			void ImageList::Clear() noexcept(true)
			{
				if (!IsCreated()) return;
				ImageList_RemoveAll((HIMAGELIST)m_hHandle);
			}

			ImageList::ImageList() noexcept(true) : m_hHandle(nullptr), m_nSizeX(0), m_nSizeY(0),
				m_eColorType(ilct32Bit), m_bMasked(false)
			{}

			ImageList::ImageList(std::int32_t nSizeX, std::int32_t nSizeY) : m_hHandle(nullptr), m_nSizeX(0), m_nSizeY(0),
				m_eColorType(ilct32Bit), m_bMasked(false)
			{ 
				CreateImgList(nSizeX, nSizeY, false, ilct32Bit);
			}

			ImageList::ImageList(std::int32_t nSizeX, std::int32_t nSizeY, ImageListColorType eColorType) : m_hHandle(nullptr),
				m_nSizeX(0), m_nSizeY(0), m_eColorType(eColorType), m_bMasked(false)
			{ 
				CreateImgList(nSizeX, nSizeY, false, eColorType);
			}

			ImageList::ImageList(const ImageList& imglst) noexcept(true) : m_hHandle(nullptr), m_nSizeX(imglst.Width),
				m_nSizeY(imglst.Height), m_eColorType(imglst.ColorType), m_bMasked(imglst.Masked)
			{
				if (imglst.IsCreated())
				  m_hHandle = ImageList_Duplicate((HIMAGELIST)imglst.Handle);
			}

			ImageList::~ImageList() noexcept(true)
			{ 
				ReleaseImgList();
			}
		}
	}
}