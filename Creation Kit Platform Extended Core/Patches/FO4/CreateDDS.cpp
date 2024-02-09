// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CreateDDS.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			uintptr_t pointer_CreateDDS_sub = 0;

			const char* ArrayFormatDDS[] = {
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

			CreateDDSPatch::CreateDDSPatch() : Module(GlobalEnginePtr)
			{}

			bool CreateDDSPatch::HasOption() const
			{
				return false;
			}

			bool CreateDDSPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CreateDDSPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* CreateDDSPatch::GetName() const
			{
				return "Add check D3D11CreateTexture2D";
			}

			bool CreateDDSPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> CreateDDSPatch::GetDependencies() const
			{
				return {};
			}

			bool CreateDDSPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_LAST;
			}

			bool CreateDDSPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
                    auto rva = lpRelocationDatabaseItem->At(0);

                    lpRelocator->PatchNop(rva, 0xC);
					lpRelocator->DetourCall(rva, (uintptr_t)&sub);
					pointer_CreateDDS_sub = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));

					return true;
				}

				return false;
			}

			bool CreateDDSPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

            void CreateDDSPatch::sub(ID3D11Device* pContext, const D3D11_TEXTURE2D_DESC* pDesc,
				const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture)
			{
				auto Ret = pContext->CreateTexture2D(pDesc, pInitialData, ppTexture);
				
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

					AssertMsg(SUCCEEDED(Ret),
						"ID3D11CreateTexture2D: A fatal error occurred while creating the texture. "
						"For details, see the log.");
				}
                
                // This return value is ignored. If it fails it returns a null pointer (ppTexture) and crashes later on.
                fastCall<void>(pointer_CreateDDS_sub, *ppTexture);
			}
		}
	}
}