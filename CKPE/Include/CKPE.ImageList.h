// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdint>
#include <CKPE.Graphics.h>

namespace CKPE
{
	enum ImageListColorType : std::uint32_t
	{
		ilct8Bit = 0,
		ilct24Bit,
		ilct32Bit,
	};

	class CKPE_API ImageList
	{
		std::int32_t m_nSizeX, m_nSizeY;
		ImageListColorType m_eColorType;
		THandle m_hHandle;
		bool m_bMasked;
	protected:
		void CreateImgList(std::int32_t nSizeX, std::int32_t nSizeY, bool bMasked = false, 
			ImageListColorType eColorType = ilct32Bit);
		void ReCreateImgList(std::int32_t nSizeX, std::int32_t nSizeY, bool bMasked = false,
			ImageListColorType eColorType = ilct32Bit);
		void ReleaseImgList() noexcept(true);
	public:
		[[nodiscard]] inline virtual THandle GetHandle() const noexcept(true) { return m_hHandle; }
		[[nodiscard]] inline virtual std::int32_t GetWidth() const noexcept(true) { return m_nSizeX; }
		[[nodiscard]] inline virtual std::int32_t GetHeight() const noexcept(true) { return m_nSizeY; }
		[[nodiscard]] inline virtual bool IsMasked() const noexcept(true) { return m_bMasked; }
		[[nodiscard]] inline virtual ImageListColorType GetColorType() const noexcept(true) { return m_eColorType; }
		virtual void SetWidth(std::int32_t width);
		virtual void SetHeight(std::int32_t height);
		virtual void SetMasked(bool value);
		virtual void SetColorType(ImageListColorType etype);
		virtual void ReSize(std::int32_t width, std::int32_t height);
		virtual void ReCreate(std::int32_t width, std::int32_t height, bool bMasked,
			ImageListColorType eColorType);
		virtual void Assign(const THandle imglst, std::int32_t width, std::int32_t height, bool bMasked,
			ImageListColorType eColorType) noexcept(true);
		virtual void Assign(const ImageList* imglst) noexcept(true);
		virtual void AssignTo(ImageList*imglst) const noexcept(true);
		virtual ImageList Dublicate() const noexcept(true);
		[[nodiscard]] virtual bool IsCreated() const noexcept(true);
	public:
		virtual bool GetBitmap(std::int32_t idx, THandle& bitmap) const noexcept(true);
		virtual bool ExtractToFile(std::int32_t idx, const std::wstring& fname) const noexcept(true);
	public:
		[[nodiscard]] virtual std::int32_t GetCount() const noexcept(true);
		virtual std::int32_t Add(THandle bitmap, THandle hmask = nullptr) noexcept(true);
		virtual std::int32_t Add(THandle bitmap, TColor cmask) noexcept(true);
		virtual std::int32_t AddFromFile(const std::wstring& fname, const std::wstring& fname_mask = L"") noexcept(true);
		virtual std::int32_t AddFromFile(const std::wstring& fname, TColor cmask) noexcept(true);
		virtual std::int32_t AddFromResource(THandle hinst, std::uint32_t dwResId, std::uint32_t dwResMaskId = 0) noexcept(true);
		virtual std::int32_t AddFromResource(THandle hinst, std::uint32_t dwResId, const wchar_t* name,
			std::uint32_t dwResMaskId = 0) noexcept(true);
		virtual std::int32_t AddFromResourceMasked(THandle hinst, std::uint32_t dwResId, const wchar_t* name,
			TColor cmask) noexcept(true);
		virtual std::int32_t AddFromResourceMasked(THandle hinst, std::uint32_t dwResId, TColor cmask) noexcept(true);
		virtual bool Replace(std::int32_t idx, THandle bitmap, THandle hmask = nullptr) noexcept(true);
		virtual bool ReplaceFromFile(std::int32_t idx, const std::wstring& fname, 
			const std::wstring& fname_mask = L"") noexcept(true);
		virtual bool ReplaceFromResource(std::int32_t idx, THandle hinst, std::uint32_t dwResId, 
			const wchar_t* name, std::uint32_t dwResMaskId = 0) noexcept(true);
		virtual bool Remove(std::int32_t idx) noexcept(true);
		virtual void Clear() noexcept(true);
	public:
		ImageList() noexcept(true);
		ImageList(std::int32_t nSizeX, std::int32_t nSizeY);
		ImageList(std::int32_t nSizeX, std::int32_t nSizeY, ImageListColorType eColorType);
		ImageList(const ImageList& imglst) noexcept(true);
		virtual ~ImageList() noexcept(true);
	public:
		CKPE_READ_PROPERTY(GetHandle) THandle Handle;
		CKPE_PROPERTY(GetWidth, SetWidth) const std::int32_t Width;
		CKPE_PROPERTY(GetHeight, SetHeight) const std::int32_t Height;
		CKPE_PROPERTY(IsMasked, SetMasked) const bool Masked;
		CKPE_PROPERTY(GetColorType, SetColorType) const ImageListColorType ColorType;
	};
}