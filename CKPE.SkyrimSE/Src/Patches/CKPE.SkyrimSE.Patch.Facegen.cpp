// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <d3d11_2.h>
#include <DirectXTex.h>
#include <commctrl.h>

#include <CKPE.StringUtils.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <Patches/CKPE.SkyrimSE.Patch.Console.h>
#include <Patches/CKPE.SkyrimSE.Patch.Facegen.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			using TFaceGen_sub1 = bool();
			using TFaceGen_sub2 = void();
			using TFaceGen_sub3 = std::int64_t(HWND, std::int64_t);
			using TFaceGen_sub4 = bool(std::int64_t, std::int64_t);
			using TFaceGen_sub5 = void(std::int64_t);

			static std::function<TFaceGen_sub1> FaceGen_sub1;
			static std::function<TFaceGen_sub2> FaceGen_sub2;
			static std::function<TFaceGen_sub3> FaceGen_sub3;
			static std::function<TFaceGen_sub4> FaceGen_sub4;
			static std::function<TFaceGen_sub5> FaceGen_sub5;

			decltype(&Facegen::CreateDiffuseCompressDDS) FaceGen_sub6 = nullptr;
			std::uintptr_t pointer_FaceGen_data = 0;
			bool bUseCompresionAsBC7U = false;

			extern ID3D11Device* pointer_d3d11DeviceIntf;

			enum class DDS_COMPRESSION
			{
				BC3_UNORM = 0,
				BC5_UNORM,
				BC7_UNORM
			};

			// I'm pretty tired of crashes when working with texconv.
			// So I'm embedding compression into the code.

			static bool CompressionDDSFile(const char* FileName, DDS_COMPRESSION Flag)
			{
				// Checking the existence of the file
				if (!PathUtils::FileExists(FileName))
				{
					Console::LogWarning(Console::FACEGEN, "File was \"%s\" not found", FileName);
					return false;
				}
				// Opening a .dds file
				auto WFileName = StringUtils::WinCPToUtf16(FileName);
				DirectX::TexMetadata info;
				auto image = std::make_unique<DirectX::ScratchImage>();
				HRESULT hr = DirectX::LoadFromDDSFile(WFileName.c_str(), DirectX::DDS_FLAGS_NONE, &info, *image);
				if (FAILED(hr))
				{
					Console::LogWarning(Console::FACEGEN, "Can't open the file \"%s\"", FileName);
					return false;
				}
				// Compression to the desired format
				DirectX::ScratchImage bcImage;

				switch (Flag)
				{
				case DDS_COMPRESSION::BC3_UNORM:
					hr = DirectX::Compress(image->GetImages(), image->GetImageCount(), image->GetMetadata(),
						DXGI_FORMAT_BC3_UNORM, DirectX::TEX_COMPRESS_DEFAULT, DirectX::TEX_ALPHA_WEIGHT_DEFAULT, bcImage);
					break;
				case DDS_COMPRESSION::BC5_UNORM:
					hr = DirectX::Compress(image->GetImages(), image->GetImageCount(), image->GetMetadata(),
						DXGI_FORMAT_BC5_UNORM, DirectX::TEX_COMPRESS_DEFAULT, DirectX::TEX_ALPHA_WEIGHT_DEFAULT, bcImage);
					break;
				case DDS_COMPRESSION::BC7_UNORM:
					{
						if (pointer_d3d11DeviceIntf)
							hr = DirectX::Compress(pointer_d3d11DeviceIntf, image->GetImages(), image->GetImageCount(), image->GetMetadata(),
								DXGI_FORMAT_BC7_UNORM, DirectX::TEX_COMPRESS_DEFAULT, DirectX::TEX_ALPHA_WEIGHT_DEFAULT, bcImage);
						else
							// DXGI_FORMAT_BC7_UNORM on CPU very slower
							hr = DirectX::Compress(image->GetImages(), image->GetImageCount(), image->GetMetadata(),
								DXGI_FORMAT_BC3_UNORM, DirectX::TEX_COMPRESS_DEFAULT, DirectX::TEX_ALPHA_WEIGHT_DEFAULT, bcImage);
					}
					break;
				default:
					return false;
				}

				if (FAILED(hr))
				{
					Console::LogWarning(Console::FACEGEN, "Can't compression the file \"%s\"", FileName);
					return false;
				}
				// Saving to the same file
				hr = DirectX::SaveToDDSFile(bcImage.GetImages(), bcImage.GetImageCount(), bcImage.GetMetadata(),
					DirectX::DDS_FLAGS_NONE, WFileName.c_str());
				if (FAILED(hr))
				{
					Console::LogWarning(Console::FACEGEN, "Can't save to file \"%s\"", FileName);
					return false;
				}
				// Inform that everything went great.
				return true;
			}

			Facegen::Facegen() : Common::Patch()
			{
				SetName("Facegen");
			}

			bool Facegen::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* Facegen::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool Facegen::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> Facegen::GetDependencies() const noexcept(true)
			{
				return { "Console", "D3D11 Patch" };
			}

			bool Facegen::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool Facegen::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool Facegen::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// FaceGen
				//
				
				// Disable automatic FaceGen on save
				if (_READ_OPTION_BOOL("FaceGen", "bDisableAutoFaceGen", false))
				{
					Relocation(ID(70976)).Write(RET);
					_MESSAGE("Disabling automatic calling FaceGen");
				}

				auto target1 = ID(553555);

				// Don't produce DDS files
				if (_READ_OPTION_BOOL("FaceGen", "bDisableExportDDS", false))
				{
					Relocation(target1, 0x248).WriteFill(NOP, 5);
					_MESSAGE("Disabling export FaceGen .DDS files");
				}
				else if (_READ_OPTION_BOOL("FaceGen", "bAutoCompressionDDS", false))
					Relocation(target1, 0x248).WriteCall(&CreateDiffuseCompressDDS);

				// Don't produce TGA files
				if (_READ_OPTION_BOOL("FaceGen", "bDisableExportTGA", false))
				{
					Relocation(target1, 0x29B).WriteFill(NOP, 5);
					_MESSAGE("Disabling export FaceGen .TGA files");
				}
				
				// Don't produce NIF files
				if (_READ_OPTION_BOOL("FaceGen", "bDisableExportNIF", false))
				{
					Relocation(ID(553557)).Write(RET);
					_MESSAGE("Disabling export FaceGen .NIF files");
				}

				// Allow variable tint mask resolution
				auto target2 = ID(647183);
				auto tintResolution = _READ_OPTION_UINT("FaceGen", "uTintMaskResolution", 512);
				Relocation(target2, 0x5CB).Write(&tintResolution, sizeof(std::uint32_t));
				Relocation(target2, 0x5D8).Write(&tintResolution, sizeof(std::uint32_t));
				
				bUseCompresionAsBC7U = _READ_OPTION_BOOL("FaceGen", "bUseCompressionAsBC7U", false);

				// Prevent internal filesystem reloads when exporting FaceGen for many NPCs
				Relocation(ID(439682)).WriteJump(&sub);
				Relocation(ID(295619), 0x11A).WriteFill(NOP, 5);
				
				pointer_FaceGen_data = ID(43514).Address();
				FaceGen_sub1 = Relocation<TFaceGen_sub1>(ID(181472)).Get();
				FaceGen_sub2 = Relocation<TFaceGen_sub2>(ID(293309)).Get();
				FaceGen_sub3 = Relocation<TFaceGen_sub3>(ID(58196)).Get();
				FaceGen_sub4 = Relocation<TFaceGen_sub4>(ID(39410)).Get();
				FaceGen_sub5 = Relocation<TFaceGen_sub5>(ID(199017)).Get();
				FaceGen_sub6 = (decltype(&CreateDiffuseCompressDDS))ID(557372).Address();

				return true;
			}

			void Facegen::sub(std::int64_t a1, std::int64_t a2) noexcept(true)
			{
				// Display confirmation message box first
				if (!FaceGen_sub1())
					return;

				HWND listHandle = *(HWND*)(a1 + 16);
				auto itemIndex = ListView_GetNextItem(listHandle, -1, LVNI_SELECTED);
				int itemCount = 0;

				for (bool flag = true; itemIndex >= 0 && flag; itemCount++)
				{
					flag = FaceGen_sub4(a2, FaceGen_sub3(listHandle, itemIndex));
					if (flag)
					{
						int oldIndex = itemIndex;
						itemIndex = ListView_GetNextItem(listHandle, itemIndex, LVNI_SELECTED);

						if (itemIndex == oldIndex)
							itemIndex = -1;
					}
				}

				// Reload loose file paths manually since it's patched out
				Console::Log("Exported FaceGen for %d NPCs. Reloading loose file paths...", itemCount);
				FaceGen_sub5(*(std::int64_t*)pointer_FaceGen_data);

				FaceGen_sub2();
			}

			void Facegen::CreateDiffuseCompressDDS(std::int64_t lpThis, std::uint32_t TextureId, const char* lpFileName,
				std::int32_t Unk1, bool Unk2) noexcept(true)
			{
				FaceGen_sub6(lpThis, TextureId, lpFileName, Unk1, Unk2);
				if (!CompressionDDSFile(lpFileName, pointer_d3d11DeviceIntf ? 
					(bUseCompresionAsBC7U ? DDS_COMPRESSION::BC7_UNORM : DDS_COMPRESSION::BC3_UNORM) : 
					DDS_COMPRESSION::BC3_UNORM))
					Console::LogWarning(Console::FACEGEN, "Compression texture \"%s\" error has occurred", lpFileName);
			}
		}
	}
}