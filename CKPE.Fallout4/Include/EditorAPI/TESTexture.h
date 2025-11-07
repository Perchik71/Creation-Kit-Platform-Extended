// Copyright © 2020-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include "BSString.h"
#include "BaseFormComponent.h"
#include "BSFixedString.h"
#include "NiAPI/NiTypes.h"
#include <dxgi.h>
#include <d3d11.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			class TESTexture : public BaseFormComponent
			{
			public:
				class TEXTURE_RAWDATA
				{
					DXGI_FORMAT _Format{ DXGI_FORMAT_R8G8B8A8_UNORM };
					DXGI_SAMPLE_DESC _SampleDesc{ 0, 0xFFFFCF };
					std::uint16_t _Usage{ 0 };							// D3D11_USAGE
					std::uint16_t _Bind{ 0 };							// D3D11_BIND_FLAG
					NiAPI::NiRGBA _Raw[1];
				public:
					constexpr TEXTURE_RAWDATA() = default;

					[[nodiscard]] inline DXGI_FORMAT GetFormat() const noexcept(true) { return _Format; }
					[[nodiscard]] inline DXGI_SAMPLE_DESC GetSampleDesc() const noexcept(true) { return _SampleDesc; }
					[[nodiscard]] inline D3D11_USAGE GetUsage() const noexcept(true) { return (D3D11_USAGE)_Usage; }
					[[nodiscard]] inline D3D11_BIND_FLAG GetBind() const noexcept(true) { return (D3D11_BIND_FLAG)_Bind; }
					[[nodiscard]] inline const NiAPI::NiRGBA* GetRaw() const noexcept(true) { return _Raw; }
				};

				class TEXTURE_INFO
				{
					std::uint32_t _Width{ 0 };
					std::uint32_t _Height{ 0 };
					TEXTURE_RAWDATA* _RawData{ nullptr };
				public:
					constexpr TEXTURE_INFO() = default;

					[[nodiscard]] inline std::uint32_t GetWidth() const noexcept(true) { return _Width; }
					[[nodiscard]] inline std::uint32_t GetHeight() const noexcept(true) { return _Height; }
					[[nodiscard]] inline const TEXTURE_RAWDATA* GetRawData() const noexcept(true) { return _RawData; }
				};
			private:
				BSFixedString _FileName;
				TEXTURE_INFO* _Info;
				char Unk[0x10];
			public:
				virtual ~TESTexture() = default;

				virtual void SetTextureFromDialogBox(HWND hDialogBox, std::uint32_t CtrlID) noexcept(true);
				[[nodiscard]] virtual std::uint32_t GetSlotIndex() const noexcept(true);
				virtual const char* GetAsNormalFile(BSString& sOut) const noexcept(true);
				[[nodiscard]] virtual const char* GetDefaultPath() const noexcept(true);
				[[nodiscard]] virtual const char* GetActionName() const noexcept(true);
				virtual void SetTextureFromFileName(const char* lpstrFileName) noexcept(true);

				[[nodiscard]] inline const char* GetFileName() const noexcept(true) { return _FileName.empty() ? "" : _FileName.c_str(); }
				[[nodiscard]] inline TEXTURE_INFO* GetDataInfo() const noexcept(true) { return _Info; }
			};
			static_assert(sizeof(TESTexture) == 0x28);
		}
	}
}