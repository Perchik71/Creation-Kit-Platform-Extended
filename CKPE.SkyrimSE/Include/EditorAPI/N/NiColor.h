// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiColor.h

#pragma once

#include <xmmintrin.h>

#include <algorithm>
#include <string>

#include <CKPE.Asserts.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{
				struct NiRGB;
				struct NiRGBA;
				struct NiColor;
				struct NiColorA;

				struct NiRGB
				{
					union
					{
						struct { std::uint8_t r, g, b; };
						std::uint8_t v[3];
					};

					enum : std::size_t
					{
						kRed,
						kGreen,
						kBlue,

						kTotal
					};

					constexpr NiRGB() noexcept(true) :
						r(0), g(0), b(0)
					{}

					constexpr NiRGB(const NiRGB& a_rhs) noexcept(true) :
						r(a_rhs.r), g(a_rhs.g), b(a_rhs.b)
					{}

					constexpr NiRGB(NiRGB&& a_rhs) noexcept(true) :
						r(std::move(a_rhs.r)), g(std::move(a_rhs.g)), b(std::move(a_rhs.b))
					{}

					constexpr NiRGB(std::uint8_t a_red, std::uint8_t a_green, std::uint8_t a_blue) noexcept(true) :
						r(a_red), g(a_green), b(a_blue)
					{}

					constexpr NiRGB(std::uint32_t a_hexValue) noexcept(true) :
						r((a_hexValue >> 16) & 0xFF),
						g((a_hexValue >> 8) & 0xFF),
						b((a_hexValue) & 0xFF)
					{}

					NiRGB(const NiColor& a_rhs);

					~NiRGB() noexcept = default;

					constexpr NiRGB& operator=(const NiRGB& a_rhs) noexcept(true)
					{
						if (this != std::addressof(a_rhs))
						{
							r = a_rhs.r;
							g = a_rhs.g;
							b = a_rhs.b;
						}
						return *this;
					}

					constexpr NiRGB& operator=(NiRGB&& a_rhs) noexcept
					{
						if (this != std::addressof(a_rhs))
						{
							r = std::move(a_rhs.r);
							g = std::move(a_rhs.g);
							b = std::move(a_rhs.b);
						}
						return *this;
					}

					[[nodiscard]] friend constexpr bool operator==(const NiRGB& a_lhs, const NiRGB& a_rhs) noexcept
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							if (a_lhs[i] != a_rhs[i])
								return false;
						return true;
					}

					[[nodiscard]] friend constexpr bool operator!=(const NiRGB& a_lhs, const NiRGB& a_rhs) noexcept
					{
						return !(a_lhs == a_rhs);
					}

					[[nodiscard]] constexpr std::uint8_t& operator[](std::size_t a_idx) noexcept
					{
						CKPE_ASSERT(a_idx < kTotal);
						return std::addressof(r)[a_idx];
					}

					[[nodiscard]] constexpr const std::uint8_t& operator[](std::size_t a_idx) const noexcept
					{
						CKPE_ASSERT(a_idx < kTotal);
						return std::addressof(r)[a_idx];
					}

					[[nodiscard]] inline NiRGB operator+(const NiRGB& a_rhs) const noexcept
					{
						NiRGB tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] += a_rhs[i];
						return tmp;
					}

					inline NiRGB& operator+=(const NiRGB& a_rhs) noexcept
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) += a_rhs[i];
						return *this;
					}

					[[nodiscard]] inline NiRGB operator-(const NiRGB& a_rhs) const noexcept
					{
						NiRGB tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] -= a_rhs[i];
						return tmp;
					}

					inline NiRGB& operator-=(const NiRGB& a_rhs) noexcept
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) -= a_rhs[i];
						return *this;
					}

					friend NiRGB operator-(std::uint8_t a_lhs, const NiRGB& a_rhs)
					{
						return NiRGB(a_lhs - a_rhs.r, a_lhs - a_rhs.g, a_lhs - a_rhs.b);
					}

					[[nodiscard]] inline NiRGB operator*(const NiRGB& a_rhs) const noexcept
					{
						NiRGB tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] *= a_rhs[i];
						return tmp;
					}

					inline NiRGB& operator*=(const NiRGB& a_rhs) noexcept
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) *= a_rhs[i];
						return *this;
					}

					friend NiRGB operator*(std::uint8_t a_lhs, const NiRGB& a_rhs)
					{
						return NiRGB(a_lhs * a_rhs.r, a_lhs * a_rhs.g, a_lhs * a_rhs.b);
					}

					[[nodiscard]] inline NiRGB operator/(const NiRGB& a_rhs) const noexcept
					{
						NiRGB tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] /= a_rhs[i];
						return tmp;
					}

					inline NiRGB& operator/=(const NiRGB& a_rhs) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) /= a_rhs[i];
						return *this;
					}

					friend NiRGB operator/(std::uint8_t a_lhs, const NiRGB& a_rhs) noexcept(true)
					{
						return NiRGB(a_lhs / a_rhs.r, a_lhs / a_rhs.g, a_lhs / a_rhs.b);
					}

					[[nodiscard]] inline NiRGB operator+(std::uint8_t a_value) const noexcept(true)
					{
						NiRGB tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] += a_value;
						return tmp;
					}

					inline NiRGB& operator+=(std::uint8_t a_value) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) += a_value;
						return *this;
					}

					[[nodiscard]] inline NiRGB operator-(std::uint8_t a_value) const noexcept(true)
					{
						NiRGB tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] -= a_value;
						return tmp;
					}

					inline NiRGB& operator-=(std::uint8_t a_value) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) -= a_value;
						return *this;
					}

					[[nodiscard]] inline NiRGB operator*(std::uint8_t a_value) const noexcept(true)
					{
						NiRGB tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] *= a_value;
						return tmp;
					}

					inline NiRGB& operator*=(std::uint8_t a_value) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) *= a_value;
						return *this;
					}

					[[nodiscard]] inline NiRGB operator/(std::uint8_t a_value) const noexcept(true)
					{
						NiRGB tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] /= a_value;
						return tmp;
					}

					inline NiRGB& operator/=(std::uint8_t a_value) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) /= a_value;
						return *this;
					}

					std::uint32_t ToInt() const noexcept(true);
					std::string   ToHex() const noexcept(true);

					[[nodiscard]] inline const std::uint8_t* Data() const noexcept(true) { return std::addressof(r); }
				};

				struct NiRGBA
				{
					union
					{
						struct { std::uint8_t r, g, b, a; };
						struct { std::uint8_t v[4]; };
						std::uint32_t c;
					};

					enum : std::size_t
					{
						kRed,
						kGreen,
						kBlue,
						kAlpha,

						kTotal
					};

					constexpr NiRGBA() noexcept(true) :
						r(0), g(0), b(0), a(0)
					{}

					constexpr NiRGBA(const NiRGBA& a_rhs) noexcept(true) :
						r(a_rhs.r), g(a_rhs.g), b(a_rhs.b), a(a_rhs.a)
					{}

					constexpr NiRGBA(NiRGBA&& a_rhs) noexcept(true) :
						r(std::move(a_rhs.r)), g(std::move(a_rhs.g)), b(std::move(a_rhs.b)), a(std::move(a_rhs.a))
					{}

					constexpr NiRGBA(std::uint8_t a_red, std::uint8_t a_green, std::uint8_t a_blue, std::uint8_t a_alpha) noexcept(true) :
						r(a_red), g(a_green), b(a_blue), a(a_alpha)
					{}

					constexpr NiRGBA(std::uint32_t a_hexValue) noexcept(true) :
						r((a_hexValue >> 24) & 0xFF),
						g((a_hexValue >> 16) & 0xFF),
						b((a_hexValue >> 8) & 0xFF),
						a(a_hexValue & 0xFF)
					{}

					NiRGBA(const NiColorA& a_rhs);

					~NiRGBA() noexcept = default;

					constexpr NiRGBA& operator=(const NiRGBA& a_rhs) noexcept(true)
					{
						if (this != std::addressof(a_rhs))
						{
							r = a_rhs.r;
							g = a_rhs.g;
							b = a_rhs.b;
							a = a_rhs.a;
						}
						return *this;
					}

					constexpr NiRGBA& operator=(NiRGBA&& a_rhs) noexcept(true)
					{
						if (this != std::addressof(a_rhs))
						{
							r = std::move(a_rhs.r);
							g = std::move(a_rhs.g);
							b = std::move(a_rhs.b);
							a = std::move(a_rhs.a);
						}
						return *this;
					}

					[[nodiscard]] friend constexpr bool operator==(const NiRGBA& a_lhs, const NiRGBA& a_rhs) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							if (a_lhs[i] != a_rhs[i])
								return false;
						return true;
					}

					[[nodiscard]] friend constexpr bool operator!=(const NiRGBA& a_lhs, const NiRGBA& a_rhs) noexcept(true)
					{
						return !(a_lhs == a_rhs);
					}

					[[nodiscard]] constexpr std::uint8_t& operator[](std::size_t a_idx) noexcept(true)
					{
						CKPE_ASSERT(a_idx < kTotal);
						return std::addressof(r)[a_idx];
					}

					[[nodiscard]] constexpr const std::uint8_t& operator[](std::size_t a_idx) const noexcept(true)
					{
						CKPE_ASSERT(a_idx < kTotal);
						return std::addressof(r)[a_idx];
					}

					[[nodiscard]] inline NiRGBA operator+(const NiRGBA& a_rhs) const noexcept(true)
					{
						NiRGBA tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] += a_rhs[i];
						return tmp;
					}

					inline NiRGBA& operator+=(const NiRGBA& a_rhs) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) += a_rhs[i];
						return *this;
					}

					[[nodiscard]] inline NiRGBA operator-(const NiRGBA& a_rhs) const noexcept(true)
					{
						NiRGBA tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] -= a_rhs[i];
						return tmp;
					}

					inline NiRGBA& operator-=(const NiRGBA& a_rhs) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) -= a_rhs[i];
						return *this;
					}

					friend NiRGBA operator-(std::uint8_t a_lhs, const NiRGBA& a_rhs) noexcept(true)
					{
						return NiRGBA(a_lhs - a_rhs.r, a_lhs - a_rhs.g, a_lhs - a_rhs.b, a_lhs - a_rhs.a);
					}

					[[nodiscard]] inline NiRGBA operator*(const NiRGBA& a_rhs) const noexcept(true)
					{
						NiRGBA tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] *= a_rhs[i];
						return tmp;
					}

					inline NiRGBA& operator*=(const NiRGBA& a_rhs) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) *= a_rhs[i];
						return *this;
					}

					friend NiRGBA operator*(std::uint8_t a_lhs, const NiRGBA& a_rhs)
					{
						return NiRGBA(a_lhs * a_rhs.r, a_lhs * a_rhs.g, a_lhs * a_rhs.b, a_lhs * a_rhs.a);
					}

					[[nodiscard]] inline NiRGBA operator/(const NiRGBA& a_rhs) const noexcept(true)
					{
						NiRGBA tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] /= a_rhs[i];
						return tmp;
					}

					inline NiRGBA& operator/=(const NiRGBA& a_rhs)  noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) /= a_rhs[i];
						return *this;
					}

					friend NiRGBA operator/(std::uint8_t a_lhs, const NiRGBA& a_rhs) noexcept(true)
					{
						return NiRGBA(a_lhs / a_rhs.r, a_lhs / a_rhs.g, a_lhs / a_rhs.b, a_lhs / a_rhs.a);
					}

					[[nodiscard]] inline NiRGBA operator+(std::uint8_t a_value) const noexcept(true)
					{
						NiRGBA tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] += a_value;
						return tmp;
					}

					inline NiRGBA& operator+=(std::uint8_t a_value) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) += a_value;
						return *this;
					}

					[[nodiscard]] inline NiRGBA operator-(std::uint8_t a_value) const noexcept(true)
					{
						NiRGBA tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] -= a_value;
						return tmp;
					}

					inline NiRGBA& operator-=(std::uint8_t a_value) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) -= a_value;
						return *this;
					}

					[[nodiscard]] inline NiRGBA operator*(std::uint8_t a_value) const noexcept(true)
					{
						NiRGBA tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] *= a_value;
						return tmp;
					}

					inline NiRGBA& operator*=(std::uint8_t a_value) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) *= a_value;
						return *this;
					}

					[[nodiscard]] inline NiRGBA operator/(std::uint8_t a_value) const noexcept(true)
					{
						NiRGBA tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] /= a_value;
						return tmp;
					}

					inline NiRGBA& operator/=(std::uint8_t a_value) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) /= a_value;
						return *this;
					}

					std::uint32_t ToInt() const noexcept(true);
					std::string   ToHex() const noexcept(true);

					[[nodiscard]] inline const std::uint8_t* Data() const noexcept(true) { return std::addressof(r); }
				};

				struct NiColor
				{
					union
					{
						struct { float r, g, b; };
						float v[3];
					};

					enum : std::size_t
					{
						kRed,
						kGreen,
						kBlue,
						kTotal
					};

					constexpr NiColor() noexcept(true) :
						r(.0f), g(.0f), b(.0f)
					{}

					constexpr NiColor(const NiColor& a_rhs) noexcept(true) :
						r(a_rhs.r), g(a_rhs.g), b(a_rhs.b)
					{}

					constexpr NiColor(NiColor&& a_rhs) noexcept(true) :
						r(std::move(a_rhs.r)), g(std::move(a_rhs.g)), b(std::move(a_rhs.b))
					{}

					constexpr NiColor(float a_red, float a_green, float a_blue) noexcept(true) :
						r(a_red), g(a_green), b(a_blue)
					{}

					constexpr NiColor(std::uint32_t a_hexValue) noexcept(true) :
						r(((a_hexValue >> 16) & 0xFF) / 255.0f),
						g(((a_hexValue >> 8) & 0xFF) / 255.0f),
						b(((a_hexValue) & 0xFF) / 255.0f)
					{}

					NiColor(const NiRGB& a_rhs) noexcept(true) :
						r(a_rhs.r / 255.0f), g(a_rhs.g / 255.0f), b(a_rhs.b / 255.0f)
					{}

					NiColor(const NiRGBA& a_rhs) noexcept(true) :
						r(a_rhs.r / 255.0f), g(a_rhs.g / 255.0f), b(a_rhs.b / 255.0f)
					{}

					~NiColor() noexcept(true) = default;

					constexpr NiColor& operator=(const NiColor& a_rhs) noexcept(true)
					{
						if (this != std::addressof(a_rhs)) 
						{
							r = a_rhs.r;
							g = a_rhs.g;
							b = a_rhs.b;
						}
						return *this;
					}

					constexpr NiColor& operator=(NiColor&& a_rhs) noexcept(true)
					{
						if (this != std::addressof(a_rhs))
						{
							r = std::move(a_rhs.r);
							g = std::move(a_rhs.g);
							b = std::move(a_rhs.b);
						}
						return *this;
					}

					constexpr NiColor& operator=(const NiColorA& a_rhs) noexcept(true);

					[[nodiscard]] friend constexpr bool operator==(const NiColor& a_lhs, const NiColor& a_rhs) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							if (a_lhs[i] != a_rhs[i])
								return false;
						return true;
					}

					[[nodiscard]] friend constexpr bool operator!=(const NiColor& a_lhs, const NiColor& a_rhs) noexcept(true)
					{
						return !(a_lhs == a_rhs);
					}

					[[nodiscard]] constexpr float& operator[](std::size_t a_idx) noexcept(true)
					{
						CKPE_ASSERT(a_idx < kTotal);
						return std::addressof(r)[a_idx];
					}

					[[nodiscard]] constexpr const float& operator[](std::size_t a_idx) const noexcept(true)
					{
						CKPE_ASSERT(a_idx < kTotal);
						return std::addressof(r)[a_idx];
					}

					[[nodiscard]] inline NiColor operator+(const NiColor& a_rhs) const noexcept(true)
					{
						NiColor tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] += a_rhs[i];
						return tmp;
					}

					inline NiColor& operator+=(const NiColor& a_rhs) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) += a_rhs[i];
						return *this;
					}

					[[nodiscard]] inline NiColor operator-(const NiColor& a_rhs) const noexcept(true)
					{
						NiColor tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] -= a_rhs[i];
						return tmp;
					}

					inline NiColor& operator-=(const NiColor& a_rhs) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) -= a_rhs[i];
						return *this;
					}

					friend NiColor operator-(float a_lhs, const NiColor& a_rhs) noexcept(true)
					{
						return NiColor(a_lhs - a_rhs.r, a_lhs - a_rhs.g, a_lhs - a_rhs.b);
					}

					[[nodiscard]] inline NiColor operator*(const NiColor& a_rhs) const noexcept(true)
					{
						NiColor tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] *= a_rhs[i];
						return tmp;
					}

					inline NiColor& operator*=(const NiColor& a_rhs) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i) {
							operator[](i) *= a_rhs[i];
						}
						return *this;
					}

					friend NiColor operator*(float a_lhs, const NiColor& a_rhs) noexcept(true)
					{
						return NiColor(a_lhs * a_rhs.r, a_lhs * a_rhs.g, a_lhs * a_rhs.b);
					}

					[[nodiscard]] inline NiColor operator/(const NiColor& a_rhs) const noexcept(true)
					{
						NiColor tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] /= a_rhs[i];
						return tmp;
					}

					inline NiColor& operator/=(const NiColor& a_rhs) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) /= a_rhs[i];
						return *this;
					}

					friend NiColor operator/(float a_lhs, const NiColor& a_rhs) noexcept(true)
					{
						return NiColor(a_lhs / a_rhs.r, a_lhs / a_rhs.g, a_lhs / a_rhs.b);
					}

					[[nodiscard]] inline NiColor operator+(float a_value) const noexcept(true)
					{
						NiColor tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] += a_value;
						return tmp;
					}

					inline NiColor& operator+=(float a_value) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) += a_value;
						return *this;
					}

					[[nodiscard]] inline NiColor operator-(float a_value) const noexcept(true)
					{
						NiColor tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] -= a_value;
						return tmp;
					}

					inline NiColor& operator-=(float a_value) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) -= a_value;
						return *this;
					}

					[[nodiscard]] inline NiColor operator*(float a_value) const noexcept(true)
					{
						NiColor tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] *= a_value;
						return tmp;
					}

					inline NiColor& operator*=(float a_value) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) *= a_value;
						return *this;
					}

					[[nodiscard]] inline NiColor operator/(float a_value) const noexcept(true)
					{
						NiColor tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] /= a_value;
						return tmp;
					}

					inline NiColor& operator/=(float a_value) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) /= a_value;
						return *this;
					}

					[[nodiscard]] std::uint32_t	ToInt() const noexcept(true);
					[[nodiscard]] std::string	ToHex() const noexcept(true);

					[[nodiscard]] inline const float* Data() const noexcept(true) { return std::addressof(r); }
				};
				static_assert(sizeof(NiColor) == 0xC);

				struct NiColorA
				{
					union
					{
						struct { float r, g, b, a; };
						float v[4];
					};

					enum : std::size_t
					{
						kRed,
						kGreen,
						kBlue,
						kAlpha,
						kTotal
					};

					constexpr NiColorA() noexcept(true) :
						r(0.0), g(0.0), b(0.0), a(0.0)
					{}

					constexpr NiColorA(const NiColorA& a_rhs) noexcept(true) :
						r(a_rhs.r), g(a_rhs.g), b(a_rhs.b), a(a_rhs.a)
					{}

					constexpr NiColorA(NiColorA&& a_rhs) noexcept(true) :
						r(std::move(a_rhs.r)), g(std::move(a_rhs.g)), b(std::move(a_rhs.b)), a(std::move(a_rhs.a))
					{}

					constexpr NiColorA(float a_red, float a_green, float a_blue, float a_alpha) noexcept(true) :
						r(a_red), g(a_green), b(a_blue), a(a_alpha)
					{}

					NiColorA(const NiRGB& a_rhs) noexcept(true) :
						r(a_rhs.r / 255.0f), g(a_rhs.g / 255.0f), b(a_rhs.b / 255.0f), a(1.f)
					{}

					NiColorA(const NiRGBA& a_rhs) noexcept(true) :
						r(a_rhs.r / 255.0f), g(a_rhs.g / 255.0f), b(a_rhs.b / 255.0f), a(a_rhs.a / 255.0f)
					{}

					~NiColorA() noexcept = default;

					constexpr NiColorA& operator=(const NiColorA& a_rhs) noexcept(true)
					{
						if (this != std::addressof(a_rhs))
						{
							r = a_rhs.r;
							g = a_rhs.g;
							b = a_rhs.b;
							a = a_rhs.a;
						}
						return *this;
					}

					constexpr NiColorA& operator=(NiColorA&& a_rhs) noexcept(true)
					{
						if (this != std::addressof(a_rhs))
						{
							r = std::move(a_rhs.r);
							g = std::move(a_rhs.g);
							b = std::move(a_rhs.b);
							a = std::move(a_rhs.a);
						}
						return *this;
					}

					constexpr NiColorA& operator=(const NiColor& a_rhs) noexcept(true);

					[[nodiscard]] friend constexpr bool operator==(const NiColorA& a_lhs, const NiColorA& a_rhs) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							if (a_lhs[i] != a_rhs[i])
								return false;
						return true;
					}

					[[nodiscard]] friend constexpr bool operator!=(const NiColorA& a_lhs, const NiColorA& a_rhs) noexcept(true)
					{
						return !(a_lhs == a_rhs);
					}

					[[nodiscard]] constexpr float& operator[](std::size_t a_idx) noexcept(true)
					{
						CKPE_ASSERT(a_idx < kTotal);
						return std::addressof(r)[a_idx];
					}

					[[nodiscard]] constexpr const float& operator[](std::size_t a_idx) const noexcept(true)
					{
						CKPE_ASSERT(a_idx < kTotal);
						return std::addressof(r)[a_idx];
					}

					[[nodiscard]] inline NiColorA operator+(float a_value) const noexcept(true)
					{
						NiColorA tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] += a_value;
						return tmp;
					}

					inline NiColorA& operator+=(float a_value) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) += a_value;
						return *this;
					}

					[[nodiscard]] inline NiColorA operator-(float a_value) const noexcept(true)
					{
						NiColorA tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] -= a_value;
						return tmp;
					}

					inline NiColorA& operator-=(float a_value) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) -= a_value;
						return *this;
					}

					[[nodiscard]] NiColorA operator*(float a_value) const noexcept(true)
					{
						NiColorA tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] *= a_value;
						return tmp;
					}

					NiColorA& operator*=(float a_value) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) *= a_value;
						return *this;
					}

					[[nodiscard]] NiColorA operator/(float a_value) const noexcept(true)
					{
						NiColorA tmp = *this;
						for (std::size_t i = 0; i < kTotal; ++i)
							tmp[i] /= a_value;
						return tmp;
					}

					NiColorA& operator/=(float a_value) noexcept(true)
					{
						for (std::size_t i = 0; i < kTotal; ++i)
							operator[](i) /= a_value;
						return *this;
					}

					[[nodiscard]] inline const float* Data() const noexcept(true) { return &r; }
					[[nodiscard]] inline __m128 AsXmm() const noexcept(true) { return _mm_load_ps(Data()); }
				};
				static_assert(sizeof(NiColorA) == 0x10);

				constexpr NiColor& NiColor::operator=(const NiColorA& a_rhs) noexcept(true)
				{
					r = a_rhs.r;
					g = a_rhs.g;
					b = a_rhs.b;
					return *this;
				}

				constexpr NiColorA& NiColorA::operator=(const NiColor& a_rhs) noexcept(true)
				{
					r = a_rhs.r;
					g = a_rhs.g;
					b = a_rhs.b;
					a = 1.f;

					return *this;
				}
			}
		}
	}
}