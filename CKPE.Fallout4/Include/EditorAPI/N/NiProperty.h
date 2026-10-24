// Copyright © 2020-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

// Special thanks to Nukem: https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/NiMain/NiAlphaProperty.h

#pragma once

#include <CKPE.Utils.h>
#include <EditorAPI/N/NiObjectNET.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class BSGeometry;

			namespace NiAPI
			{
				namespace Templates
				{
					template<typename T, std::uint32_t REL_GETTYPE>
					class NiPropertyT : public T
					{
					public:
						enum
						{
							kTypeAlpha = 0,
							kTypeShade = 1
						};

						[[nodiscard]] inline std::uint32_t GetType() const noexcept(true) { return CKPE::vtbl_call<std::uint32_t>(REL_GETTYPE, this); }
					};

					template<typename T>
					class NiAlphaPropertyT : public T
					{
					private:
						union
						{
							std::uint16_t Value;
							struct
							{
								std::uint16_t DoBlending : 1;
								std::uint16_t SrcBlend : 4;	// BlendFunction
								std::uint16_t DstBlend : 4;	// BlendFunction
								std::uint16_t DoTesting : 1;
								std::uint16_t TestFunc : 3;	// TestFunction
								std::uint16_t NoSorter : 1;
							};
						} AlphaFlags;
						std::uint8_t alphaThreshold;
						char pad04[0x4];
					public:
						bool GetAlphaBlending() const
						{
							return AlphaFlags.DoBlending;
						}

						bool GetAlphaTesting() const
						{
							return AlphaFlags.DoTesting;
						}

						std::uint8_t GetThreshold() const
						{
							return alphaThreshold;
						}

						void GetViewerStrings(void(*Callback)(const char*, ...)) const
						{
							Callback("-- NiAlphaProperty --\n");
							Callback("Alpha Blend: %s\n", GetAlphaBlending() ? "true" : "false");
							Callback("Alpha Test: %s\n", GetAlphaTesting() ? "true" : "false");
							Callback("Threshold: %u\n", (std::uint32_t)GetThreshold());
						}
					};
				}

				class NiUpdateData;

				class NiProperty : public Templates::NiPropertyT<NiObjectNET, 0x150>
				{
				public:
					/* 2A */ [[nodiscard]] virtual std::uint32_t GetType() const noexcept(true) = 0;
					/* 2B */ virtual void UpdateControllers(NiUpdateData& Data) noexcept(true);
				};
				static_assert(sizeof(NiProperty) == 0x28);

				class NiShadeProperty : public NiProperty
				{
				public:
					[[nodiscard]] std::uint32_t GetType() const noexcept(true) override;
				protected:
					/* 2C */ virtual bool Unk2C(BSGeometry* Geometry) noexcept(true);
				};
				static_assert(sizeof(NiShadeProperty) == 0x28);

				class NiAlphaProperty : public Templates::NiAlphaPropertyT<NiProperty>
				{
				public:
					[[nodiscard]] std::uint32_t GetType() const noexcept(true) override;
				};
				static_assert(sizeof(NiAlphaProperty) == 0x30);
			}
		}
	}
}