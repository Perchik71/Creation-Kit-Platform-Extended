// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <EditorAPI/NiAPI/NiColor.h>
#include <format>

std::uint32_t CKPE::SkyrimSE::EditorAPI::NiAPI::NiColor::ToInt() const noexcept(true)
{
	auto red	= static_cast<std::uint32_t>(r * 255);
	auto green	= static_cast<std::uint32_t>(g * 255);
	auto blue	= static_cast<std::uint32_t>(b * 255);

	return ((red & 0xFF) << 16) + ((green & 0xFF) << 8) + (blue & 0xFF);
}

std::string CKPE::SkyrimSE::EditorAPI::NiAPI::NiColor::ToHex() const noexcept(true)
{
	auto red	= static_cast<std::uint32_t>(r * 255);
	auto green	= static_cast<std::uint32_t>(g * 255);
	auto blue	= static_cast<std::uint32_t>(b * 255);

	return std::format("{:X}{:X}{:X}", red, green, blue);
}

CKPE::SkyrimSE::EditorAPI::NiAPI::NiRGBA::NiRGBA(const NiColorA& a_rhs) :
	r(static_cast<std::uint8_t>(255.0f * a_rhs.r)),
	g(static_cast<std::uint8_t>(255.0f * a_rhs.g)),
	b(static_cast<std::uint8_t>(255.0f * a_rhs.b)),
	a(static_cast<std::uint8_t>(255.0f * a_rhs.a))
{}

std::uint32_t CKPE::SkyrimSE::EditorAPI::NiAPI::NiRGBA::ToInt() const noexcept(true)
{
	return ((r & 0xFF) << 24) + ((g & 0xFF) << 16) + ((b & 0xFF) << 8) + (a & 0xFF);
}

std::string CKPE::SkyrimSE::EditorAPI::NiAPI::NiRGBA::ToHex() const noexcept(true)
{
	return std::format("{:X}{:X}{:X}{:X}", r, g, b, a);
}

CKPE::SkyrimSE::EditorAPI::NiAPI::NiRGB::NiRGB(const NiColor& a_rhs) :
	r(static_cast<std::uint8_t>(255.0f * a_rhs.r)),
	g(static_cast<std::uint8_t>(255.0f * a_rhs.g)),
	b(static_cast<std::uint8_t>(255.0f * a_rhs.b))
{}

std::uint32_t CKPE::SkyrimSE::EditorAPI::NiAPI::NiRGB::ToInt() const noexcept(true)
{
	return ((r & 0xFF) << 16) + ((g & 0xFF) << 8) + (b & 0xFF);
}

std::string CKPE::SkyrimSE::EditorAPI::NiAPI::NiRGB::ToHex() const noexcept(true)
{
	return std::format("{:X}{:X}{:X}", r, g, b);
}
