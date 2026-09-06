// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <d3d11_2.h>
#include <DirectXTex.h>
#include <commctrl.h>

#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.SafeWrite.h>
#include <CKPE.StringUtils.h>
#include <CKPE.PathUtils.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.Console.h>
#include <Patches/CKPE.Fallout4.Patch.Facegen.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			std::uintptr_t pointer_FaceGen_sub1 = 0;
			std::uintptr_t pointer_FaceGen_sub2 = 0;
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

			bool Facegen::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_FALLOUT_C4_LAST;
			}

			bool Facegen::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// Cutting a lot is faster this way
				auto stext = Application::GetSingleton()->GetSegment(Segment::text);
				ScopeSafeWrite text(stext.GetAddress(), stext.GetSize());

				pointer_FaceGen_sub1 = Relocation(ID{ 75665, 1589229 }).Address();
				pointer_FaceGen_sub2 = Relocation(ID{ 632518, 1388320 }).Address();

				const auto target1 = ID{ 666256, 1990278 };

				// Don't produce DDS files
				if (_READ_OPTION_BOOL("FaceGen", "bDisableExportDDS", false))
				{
					text.WriteNop(Relocation(target1, 0x163).Address(), 0x5);
					text.WriteNop(Relocation(target1, 0x183).Address(), 0x5);
					text.WriteNop(Relocation(target1, 0x1A8).Address(), 0x5);
					text.WriteNop(Relocation(target1, 0x1C8).Address(), 0x5);
					text.WriteNop(Relocation(target1, 0x1ED).Address(), 0x5);
					text.WriteNop(Relocation(target1, 0x20D).Address(), 0x5);
					_MESSAGE("Disabling export FaceGen .DDS files");
				}
				else if (_READ_OPTION_BOOL("FaceGen", "bAutoCompressionDDS", false))
				{
					Detours::DetourCall(Relocation(target1, 0x183).Address(), reinterpret_cast<std::uintptr_t>(&CreateDiffuseCompressDDS));
					Detours::DetourCall(Relocation(target1, 0x1C8).Address(), reinterpret_cast<std::uintptr_t>(&CreateNormalsCompressDDS));
					Detours::DetourCall(Relocation(target1, 0x20D).Address(), reinterpret_cast<std::uintptr_t>(&CreateSpecularCompressDDS));
				}

				// Don't produce TGA files
				if (_READ_OPTION_BOOL("FaceGen", "bDisableExportTGA", false))
				{
					text.WriteNop(Relocation(target1, 0x231).Address(), 0x5);
					text.WriteNop(Relocation(target1, 0x250).Address(), 0x5);
					text.WriteNop(Relocation(target1, 0x274).Address(), 0x5);
					text.WriteNop(Relocation(target1, 0x293).Address(), 0x5);
					text.WriteNop(Relocation(target1, 0x2B7).Address(), 0x5);
					text.WriteNop(Relocation(target1, 0x2D6).Address(), 0x5);
					_MESSAGE("Disabling export FaceGen .TGA files");
				}

				// Don't produce NIF files
				if (_READ_OPTION_BOOL("FaceGen", "bDisableExportNIF", false))
				{
					text.Write(Relocation(ID{ 50814, 1938871 }).Address(), { 0xC3 });
					_MESSAGE("Disabling export FaceGen .NIF files");
				}

				bUseCompresionAsBC7U = _READ_OPTION_BOOL("FaceGen", "bUseCompressionAsBC7U", false);

				// Allow variable tint mask resolution
				const auto target2 = ID{ 800387, 1690631 };
				std::uint32_t tintResolution = _READ_OPTION_UINT("FaceGen", "uTintMaskResolution", 1024);
				text.Write(Relocation(target2, Offset{ 0xB9C, 0xBDD }).Address(),
					(std::uint8_t*)&tintResolution, sizeof(std::uint32_t));
				text.Write(Relocation(target2, Offset{ 0xBA3, 0xBE8 }).Address(),
					(std::uint8_t*)&tintResolution, sizeof(std::uint32_t));

				// remove check format
				text.WriteNop(Relocation(target1, 0x13D).Address(), 6);

				return true;
			}

			void Facegen::CreateDiffuseCompressDDS(std::int64_t lpThis, std::uint32_t TextureId, const char* lpFileName,
				std::int32_t Unk1, bool Unk2) noexcept(true)
			{
				fast_call<void>(pointer_FaceGen_sub1, lpThis, TextureId, lpFileName, Unk1, Unk2);
				if (!CompressionDDSFile(lpFileName, pointer_d3d11DeviceIntf ?
					(bUseCompresionAsBC7U ? DDS_COMPRESSION::BC7_UNORM : DDS_COMPRESSION::BC3_UNORM) :
					DDS_COMPRESSION::BC3_UNORM))
					_CONSOLE("FACEGEN: Compression texture \"%s\" error has occurred", lpFileName);
			}

			void Facegen::CreateNormalsCompressDDS(std::int64_t lpThis, std::uint32_t TextureId, const char* lpFileName,
				std::int32_t Unk1, bool Unk2) noexcept(true)
			{
				fast_call<void>(pointer_FaceGen_sub1, lpThis, TextureId, lpFileName, Unk1, Unk2);
				if (!CompressionDDSFile(lpFileName, DDS_COMPRESSION::BC5_UNORM))
					_CONSOLE("FACEGEN: Compression texture \"%s\" error has occurred", lpFileName);
			}

			void Facegen::CreateSpecularCompressDDS(std::int64_t lpThis, std::uint32_t TextureId, const char* lpFileName,
				std::int32_t Unk1, bool Unk2) noexcept(true)
			{
				fast_call<void>(pointer_FaceGen_sub1, lpThis, TextureId, lpFileName, Unk1, Unk2);
				if (!CompressionDDSFile(lpFileName, DDS_COMPRESSION::BC5_UNORM))
					_CONSOLE("FACEGEN: Compression texture \"%s\" error has occurred", lpFileName);
			}
		}
	}
}