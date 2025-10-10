// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <d3d11_3.h>
#include <comdef.h>
#include <CKPE.Asserts.h>
#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.CreateDDS.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			std::uintptr_t pointer_CreateDDS_sub = 0;

            static const char* ArrayFormatDDS[] = {
                "DXGI_FORMAT_UNKNOWN",
                "DXGI_FORMAT_R32G32B32A32_TYPELESS",
                "DXGI_FORMAT_R32G32B32A32_FLOAT",
                "DXGI_FORMAT_R32G32B32A32_UINT",
                "DXGI_FORMAT_R32G32B32A32_SINT",
                "DXGI_FORMAT_R32G32B32_TYPELESS",
                "DXGI_FORMAT_R32G32B32_FLOAT",
                "DXGI_FORMAT_R32G32B32_UINT",
                "DXGI_FORMAT_R32G32B32_SINT",
                "DXGI_FORMAT_R16G16B16A16_TYPELESS",
                "DXGI_FORMAT_R16G16B16A16_FLOAT",
                "DXGI_FORMAT_R16G16B16A16_UNORM",
                "DXGI_FORMAT_R16G16B16A16_UINT",
                "DXGI_FORMAT_R16G16B16A16_SNORM",
                "DXGI_FORMAT_R16G16B16A16_SINT",
                "DXGI_FORMAT_R32G32_TYPELESS",
                "DXGI_FORMAT_R32G32_FLOAT",
                "DXGI_FORMAT_R32G32_UINT",
                "DXGI_FORMAT_R32G32_SINT",
                "DXGI_FORMAT_R32G8X24_TYPELESS",
                "DXGI_FORMAT_D32_FLOAT_S8X24_UINT",
                "DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS",
                "DXGI_FORMAT_X32_TYPELESS_G8X24_UINT",
                "DXGI_FORMAT_R10G10B10A2_TYPELESS",
                "DXGI_FORMAT_R10G10B10A2_UNORM",
                "DXGI_FORMAT_R10G10B10A2_UINT",
                "DXGI_FORMAT_R11G11B10_FLOAT",
                "DXGI_FORMAT_R8G8B8A8_TYPELESS",
                "DXGI_FORMAT_R8G8B8A8_UNORM",
                "DXGI_FORMAT_R8G8B8A8_UNORM_SRGB",
                "DXGI_FORMAT_R8G8B8A8_UINT",
                "DXGI_FORMAT_R8G8B8A8_SNORM",
                "DXGI_FORMAT_R8G8B8A8_SINT",
                "DXGI_FORMAT_R16G16_TYPELESS",
                "DXGI_FORMAT_R16G16_FLOAT",
                "DXGI_FORMAT_R16G16_UNORM",
                "DXGI_FORMAT_R16G16_UINT",
                "DXGI_FORMAT_R16G16_SNORM",
                "DXGI_FORMAT_R16G16_SINT",
                "DXGI_FORMAT_R32_TYPELESS",
                "DXGI_FORMAT_D32_FLOAT",
                "DXGI_FORMAT_R32_FLOAT",
                "DXGI_FORMAT_R32_UINT",
                "DXGI_FORMAT_R32_SINT",
                "DXGI_FORMAT_R24G8_TYPELESS",
                "DXGI_FORMAT_D24_UNORM_S8_UINT",
                "DXGI_FORMAT_R24_UNORM_X8_TYPELESS",
                "DXGI_FORMAT_X24_TYPELESS_G8_UINT",
                "DXGI_FORMAT_R8G8_TYPELESS",
                "DXGI_FORMAT_R8G8_UNORM",
                "DXGI_FORMAT_R8G8_UINT",
                "DXGI_FORMAT_R8G8_SNORM",
                "DXGI_FORMAT_R8G8_SINT",
                "DXGI_FORMAT_R16_TYPELESS",
                "DXGI_FORMAT_R16_FLOAT",
                "DXGI_FORMAT_D16_UNORM",
                "DXGI_FORMAT_R16_UNORM",
                "DXGI_FORMAT_R16_UINT",
                "DXGI_FORMAT_R16_SNORM",
                "DXGI_FORMAT_R16_SINT",
                "DXGI_FORMAT_R8_TYPELESS",
                "DXGI_FORMAT_R8_UNORM",
                "DXGI_FORMAT_R8_UINT",
                "DXGI_FORMAT_R8_SNORM",
                "DXGI_FORMAT_R8_SINT",
                "DXGI_FORMAT_A8_UNORM",
                "DXGI_FORMAT_R1_UNORM",
                "DXGI_FORMAT_R9G9B9E5_SHAREDEXP",
                "DXGI_FORMAT_R8G8_B8G8_UNORM",
                "DXGI_FORMAT_G8R8_G8B8_UNORM",
                "DXGI_FORMAT_BC1_TYPELESS",
                "DXGI_FORMAT_BC1_UNORM",
                "DXGI_FORMAT_BC1_UNORM_SRGB",
                "DXGI_FORMAT_BC2_TYPELESS",
                "DXGI_FORMAT_BC2_UNORM",
                "DXGI_FORMAT_BC2_UNORM_SRGB",
                "DXGI_FORMAT_BC3_TYPELESS",
                "DXGI_FORMAT_BC3_UNORM",
                "DXGI_FORMAT_BC3_UNORM_SRGB",
                "DXGI_FORMAT_BC4_TYPELESS",
                "DXGI_FORMAT_BC4_UNORM",
                "DXGI_FORMAT_BC4_SNORM",
                "DXGI_FORMAT_BC5_TYPELESS",
                "DXGI_FORMAT_BC5_UNORM",
                "DXGI_FORMAT_BC5_SNORM",
                "DXGI_FORMAT_B5G6R5_UNORM",
                "DXGI_FORMAT_B5G5R5A1_UNORM",
                "DXGI_FORMAT_B8G8R8A8_UNORM",
                "DXGI_FORMAT_B8G8R8X8_UNORM",
                "DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM",
                "DXGI_FORMAT_B8G8R8A8_TYPELESS",
                "DXGI_FORMAT_B8G8R8A8_UNORM_SRGB",
                "DXGI_FORMAT_B8G8R8X8_TYPELESS",
                "DXGI_FORMAT_B8G8R8X8_UNORM_SRGB",
                "DXGI_FORMAT_BC6H_TYPELESS",
                "DXGI_FORMAT_BC6H_UF16",
                "DXGI_FORMAT_BC6H_SF16",
                "DXGI_FORMAT_BC7_TYPELESS",
                "DXGI_FORMAT_BC7_UNORM",
                "DXGI_FORMAT_BC7_UNORM_SRGB",
                "DXGI_FORMAT_AYUV",
                "DXGI_FORMAT_Y410",
                "DXGI_FORMAT_Y416",
                "DXGI_FORMAT_NV12",
                "DXGI_FORMAT_P010",
                "DXGI_FORMAT_P016",
                "DXGI_FORMAT_420_OPAQUE",
                "DXGI_FORMAT_YUY2",
                "DXGI_FORMAT_Y210",
                "DXGI_FORMAT_Y216",
                "DXGI_FORMAT_NV11",
                "DXGI_FORMAT_AI44",
                "DXGI_FORMAT_IA44",
                "DXGI_FORMAT_P8",
                "DXGI_FORMAT_A8P8",
                "DXGI_FORMAT_B4G4R4A4_UNORM",
            };

            static void CreateDDSPatch_sub(ID3D11Device* pDevice, const D3D11_TEXTURE2D_DESC* pDesc,
                const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture)
            {
                // Verify our target format is supported by the current device

                UINT support = 0;
                auto Ret = pDevice->CheckFormatSupport(pDesc->Format, &support);
                if (FAILED(Ret) || !(support & D3D11_FORMAT_SUPPORT_TEXTURE2D))
                    CKPE_ASSERT_MSG_FMT(SUCCEEDED(Ret),
                        "ID3D11CreateTexture2D: The format \"%s\" is not supported by your device.",
                        (((int)pDesc->Format < _ARRAYSIZE(ArrayFormatDDS)) ? ArrayFormatDDS[(int)pDesc->Format] : ArrayFormatDDS[0]));

                Ret = pDevice->CreateTexture2D(pDesc, pInitialData, ppTexture);

                if (FAILED(Ret))
                {
                    _ERROR("Texture creation error");
                    _MESSAGE("Requested parameters:");

                    _MESSAGE("- Width %u", pDesc->Width);
                    _MESSAGE("- Height %u", pDesc->Height);
                    _MESSAGE("- MipLevels %u", pDesc->MipLevels);
                    _MESSAGE("- Format %s",
                        (((int)pDesc->Format < _ARRAYSIZE(ArrayFormatDDS)) ? ArrayFormatDDS[(int)pDesc->Format] : ArrayFormatDDS[0]));
                    _MESSAGE("- SampleDesc.Count %u", pDesc->SampleDesc.Count);
                    _MESSAGE("- SampleDesc.Quality %u", pDesc->SampleDesc.Quality);
                    _MESSAGE("- Usage %u", (uint32_t)pDesc->Usage);
                    _MESSAGE("- BindFlags %u", pDesc->BindFlags);
                    _MESSAGE("- CPUAccessFlags %u", (uint32_t)pDesc->CPUAccessFlags);
                    _MESSAGE("- MiscFlags %u", pDesc->MiscFlags);
                    _MESSAGE("- Result Code Error %u %s", Ret, _com_error(Ret).ErrorMessage());

                    CKPE_ASSERT_MSG(SUCCEEDED(Ret),
                        "ID3D11CreateTexture2D: A fatal error occurred while creating the texture. "
                        "For details, see the log.");
                }

                // This return value is ignored. If it fails it returns a null pointer (ppTexture) and crashes later on.
                fast_call<void>(pointer_CreateDDS_sub, *ppTexture);
            }

            static void CreateDDSPatch_sub2()
            {
                CKPE_ASSERT_MSG(0,
                    "ID3D11CreateTexture2D: A fatal error occurred while creating the texture. "
                    "Perhaps your device does not directly support texture formats.");
            }

			CreateDDS::CreateDDS() : Common::Patch()
			{
				SetName("Add check D3D11CreateTexture2D");
			}

			bool CreateDDS::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* CreateDDS::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bWarningCreateTexture2D";
			}

			bool CreateDDS::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CreateDDS::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CreateDDS::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool CreateDDS::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto _interface = CKPE::Common::Interface::GetSingleton();
				auto base = _interface->GetApplication()->GetBase();

				if (verPatch == 1)
				{
					auto rva = __CKPE_OFFSET(0);

					SafeWrite::WriteNop(rva, 0xC);
					Detours::DetourCall(rva, (std::uintptr_t)&CreateDDSPatch_sub);
					pointer_CreateDDS_sub = __CKPE_OFFSET(1);

					return true;
				}
				else if (verPatch == 2)
				{
					// Bethesda has finally added a check. Let's make it so that it causes a crash.
					Detours::DetourCall(__CKPE_OFFSET(0), (std::uintptr_t)&CreateDDSPatch_sub2);

					return true;
				}

				return true;
			}
		}
	}
}