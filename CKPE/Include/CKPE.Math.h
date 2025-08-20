// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstdio>
#include <cstdint>
#include <string>
#include <CKPE.Common.h>

namespace CKPE
{
#ifndef _WINDEF_
	typedef struct tagRECT
	{
		std::int32_t left;
		std::int32_t top;
		std::int32_t right;
		std::int32_t bottom;
	} 	RECT;

	typedef struct tagRECT* PRECT;
	typedef struct tagRECT* LPRECT;
	typedef const RECT* LPCRECT;

	typedef struct tagPOINT
	{
		std::int32_t x;
		std::int32_t y;
	} POINT, *PPOINT, *LPPOINT;
#endif

	class CKPE_API CPOINT
	{
		std::int32_t m_x{ 0 };
		std::int32_t m_y{ 0 };
	public:
		constexpr CPOINT() noexcept(true) = default;
		CPOINT(const std::int32_t x, const std::int32_t y) noexcept(true) : m_x(x), m_y(y) {}
		CPOINT(const POINT& p) noexcept(true) : m_x(p.x), m_y(p.y) {}
		CPOINT(const CPOINT& p) : m_x(p.m_x), m_y(p.m_y) {}
		CPOINT& operator=(const POINT& p)
		{
			m_x = p.x; m_y = p.y; return *this;
		}
		CPOINT& operator=(const CPOINT& p)
		{
			m_x = p.m_x; m_y = p.m_y; return *this;
		}
	public:
		[[nodiscard]] inline std::int32_t GetX() const noexcept(true) { return m_x; }
		[[nodiscard]] inline std::int32_t GetY() const noexcept(true) { return m_y; }
		inline void SetX(const std::int32_t value) noexcept(true) { m_x = value; }
		inline void SetY(const std::int32_t value) noexcept(true) { m_y = value; }
	public:
		[[nodiscard]] inline bool operator==(const POINT& p) const noexcept(true)
		{
			return (p.x == m_x) && (p.y == m_y);
		}
		[[nodiscard]] inline bool operator==(const CPOINT& p) const noexcept(true)
		{
			return (p.m_x == m_x) && (p.m_y == m_y);
		}
		[[nodiscard]] inline bool operator!=(const POINT& p) const noexcept(true) { return !(*this == p); }
		[[nodiscard]] inline bool operator!=(const CPOINT& p) const noexcept(true) { return !(*this == p); }
	public:
		CKPE_PROPERTY(GetX, SetX) std::int32_t X;
		CKPE_PROPERTY(GetY, SetY) std::int32_t Y;
	};

	typedef CPOINT *LPCCPOINT, *PCCPOINT;

	class CKPE_API CRECT
	{
		std::int32_t m_left{ 0 };
		std::int32_t m_top{ 0 };
		std::int32_t m_right{ 0 };
		std::int32_t m_bottom{ 0 };
	public:
		constexpr CRECT() noexcept(true) = default;
		CRECT(const std::int32_t l, const std::int32_t t, const std::int32_t r, const std::int32_t b) noexcept(true) :
			m_left(l), m_top(t), m_right(r), m_bottom(b) {}
		CRECT(const RECT& r) : m_left(r.left), m_top(r.top), m_right(r.right), m_bottom(r.bottom) {}
		CRECT(const CRECT& r) : m_left(r.m_left), m_top(r.m_top), m_right(r.m_right), m_bottom(r.m_bottom) {}
		CRECT& operator=(const RECT& r)
		{
			m_left = r.left; m_top = r.top; m_right = r.right; m_bottom = r.bottom; return *this;
		}
		CRECT& operator=(const CRECT& r)
		{
			m_left = r.m_left; m_top = r.m_top; m_right = r.m_right; m_bottom = r.m_bottom; return *this;
		}
	public:
		[[nodiscard]] inline std::int32_t GetWidth() const noexcept(true) { return m_right - m_left; }
		[[nodiscard]] inline std::int32_t GetHeight() const noexcept(true) { return m_bottom - m_top; }
		[[nodiscard]] inline std::int32_t GetLeft() const noexcept(true) { return m_left; }
		[[nodiscard]] inline std::int32_t GetTop() const noexcept(true) { return m_top; }
		[[nodiscard]] inline std::int32_t GetRight() const noexcept(true) { return m_right; }
		[[nodiscard]] inline std::int32_t GetBottom() const noexcept(true) { return m_bottom; }
		inline void SetWidth(const std::int32_t value) noexcept(true) { m_right = value + m_left; }
		inline void SetHeight(const std::int32_t value) noexcept(true) { m_bottom = value + m_top; }
		inline void SetLeft(const std::int32_t value) noexcept(true) { m_right = GetWidth() + value; m_left = value; }
		inline void SetTop(const std::int32_t value) noexcept(true) { m_bottom = GetHeight() + value; m_top = value; }
		inline void SetRight(const std::int32_t value) noexcept(true) { m_right = value; }
		inline void SetBottom(const std::int32_t value) noexcept(true) { m_bottom = value; }
	public:
		inline CRECT& Inflate(const std::int32_t x, const std::int32_t y) noexcept(true)
		{
			m_left -= x; m_top -= y; m_right += x; m_bottom += y; return *this;
		}
		inline CRECT& Offset(const std::int32_t x, const std::int32_t y) noexcept(true)
		{
			m_left += x; m_top += y; m_right += x; m_bottom += y; return *this;
		}
		[[nodiscard]] inline bool IsEmpty() const noexcept(true) { return GetWidth() == 0 || GetHeight() == 0; }
		inline CRECT Dublicate() const noexcept(true) { return *this; }
	public:
		[[nodiscard]] inline bool operator==(const RECT& r) const noexcept(true)
		{
			return (r.left == Left) && (r.top == Top) && (r.right == Right) && (r.bottom == Bottom);
		}
		[[nodiscard]] inline bool operator==(const CRECT& r) const noexcept(true)
		{
			return (r.Left == Left) && (r.Top == Top) && (r.Right == Right) && (r.Bottom == Bottom);
		}
		[[nodiscard]] inline bool operator!=(const RECT& r) const noexcept(true) { return !(*this == r); }
		[[nodiscard]] inline bool operator!=(const CRECT& r) const noexcept(true) { return !(*this == r); }
	public:
		CKPE_PROPERTY(GetWidth, SetWidth) std::int32_t Width;
		CKPE_PROPERTY(GetHeight, SetHeight) std::int32_t Height;
		CKPE_PROPERTY(GetLeft, SetLeft) std::int32_t Left;
		CKPE_PROPERTY(GetTop, SetTop) std::int32_t Top;
		CKPE_PROPERTY(GetRight, SetRight) std::int32_t Right;
		CKPE_PROPERTY(GetBottom, SetBottom) std::int32_t Bottom;
	};

	typedef CRECT *LPCCRECT, *PCCRECT;

	class CKPE_API Math
	{
		constexpr Math() noexcept(true) = default;
		Math(const Math&) = delete;
		Math& operator=(const Math&) = delete;
	public:
		[[nodiscard]] inline static double Rad2Deg(double rad) noexcept(true) { return rad * 180. / 3.1415926536; }
		[[nodiscard]] inline static double Deg2Rad(double deg) noexcept(true) { return deg * 3.1415926536 / 180.; }
	};
}