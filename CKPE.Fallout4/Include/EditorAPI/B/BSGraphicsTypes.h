// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <cstddef>
#include <cstdint>
#include <d3d11.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			namespace BSGraphics
			{
				enum class RENDER_TARGET : std::int32_t
				{
					kNONE = -1,
					kFRAMEBUFFER = 0,
					kMAIN = 1,
					kMAIN_COPY = 2,
					kMAIN_SAVE_RES = 3,
					kSSR_PREPASS = 4,
					kSSR = 5,
					kSSR_BLURRED_0 = 6,
					kSSR_BLURRED_1 = 7,
					kSSR_DEPTH = 8,
					kSSR_BLEND = 9,
					kWATER_DISPLACEMENT = 10,
					kWATER_DISPLACEMENT_SWAP = 11,
					kWATER_REFLECTIONS = 12,
					kWATER_REFRACTION = 13,
					kREFRACTION_NORMALS = 14,
					kMENU_BG = 15,
					kFACE_CUSTOMIZATION_DIFFUSE = 16,
					kFACE_CUSTOMIZATION_NORMALS = 17,
					kFACE_CUSTOMIZATION_SMOOTH_SPEC = 18,
					kLOCAL_MAP = 19,
					kCOMPANION_LOCAL_MAP = 20,
					kCOMPANION_LOCAL_MAP_NORMALS = 21,
					kFOG_OF_WAR = 22,
					kCOMPANION_FOG_OF_WAR = 23,
					kHUD_GLASS = 24,
					kHUD_GLASS_SWAP = 25,
					kGB_ALBEDO_SPEC = 26,
					kGB_NORMAL = 27,
					kGB_NORMAL_COPY = 28,
					kGB_ENVMAP = 29,
					kGB_AMBIENT = 30,
					kGB_EMIT = 31,
					kGB_MOTION_VECTORS = 32,
					kDF_LIGHT = 33,
					kDF_SPECULAR = 34,
					kDF_LIGHT_TILED = 35,
					kDF_SPECULAR_TILED = 36,
					kAO = 37,
					kAO_SWAP_0 = 38,
					kAO_2X = 39,
					kSAO_CAMERA_Z = 40,
					kSAO_MIP_LEVEL_0 = 41,
					kSAO_MIP_LEVEL_1 = 42,
					kSAO_MIP_LEVEL_2 = 43,
					kSAO_MIP_LEVEL_3 = 44,
					kSAO_MIP_LEVEL_4 = 45,
					kSAO_MIP_LEVEL_5 = 46,
					kSAO_MIP_LEVEL_6 = 47,
					kSAO_MIP_LEVEL_7 = 48,
					kSAO_MIP_LEVEL_8 = 49,
					kSAO_MIP_LEVEL_9 = 50,
					kSAO_MIP_LEVEL_10 = 51,
					kSAO_RAW_AO = 52,
					kSAO_RAW_AO_2 = 53,
					kSAO_TEMP_BLUR = 54,
					kSAO_RAW_AO_2X = 55,
					kSAO_RAW_AO_2_2X = 56,
					kSAO_TEMP_BLUR_2X = 57,
					kTEMPORAL_AA_ACCUMULATION_1 = 58,
					kTEMPORAL_AA_ACCUMULATION_2 = 59,
					kPIPBOY = 60,
					kPIPBOY_SWAP = 61,
					kMENU_SCREEN_FULL_SIZE = 62,
					kMENU_SCREEN_FULL_SIZE_SWAP = 63,
					kMENU_TEXT = 64,
					kLENS_FLARE = 65,
					kGET_HIT = 66,
					kBLUR_FULL = 67,
					kHDR_BLUR_SWAP = 68,
					kHDR_BLOOM = 69,
					kHDR_DOWNSAMPLE_0 = 70,
					kHDR_DOWNSAMPLE_1 = 71,
					kHDR_DOWNSAMPLE_2 = 72,
					kHDR_DOWNSAMPLE_3 = 73,
					kHDR_DOWNSAMPLE_4 = 74,
					kHDR_DOWNSAMPLE_5 = 75,
					kHDR_DOWNSAMPLE_6 = 76,
					kHDR_DOWNSAMPLE_7 = 77,
					kHDR_DOWNSAMPLE_8 = 78,
					kHDR_DOWNSAMPLE_9 = 79,
					kHDR_DOWNSAMPLE_10 = 80,
					kHDR_DOWNSAMPLE_11 = 81,
					kHDR_DOWNSAMPLE_12 = 82,
					kHDR_DOWNSAMPLE_13 = 83,
					kDOF_1 = 84,
					kDOF_2 = 85,
					kRAIN_OCCLUSION = 86,
					kTERRAIN_TEXTURE_GENERATION = 87,
					kTERRAIN_TEXTURE_GENERATION_FINAL = 88,
					kEDITOR_MAP_MAKER = 89,
					kEDITOR_MAP_MAKER_SWAP = 90,
					kGODRAYS_PHASE_LOOKUP = 91,
					kGODRAYS_ACCUMULATION = 92,
					kGODRAYS_RESOLVED = 93,
					kGODRAYS_RESOLVED_DEPTH = 94,
					kGODRAYS_FILTERED_0 = 95,
					kGODRAYS_FILTERED_1 = 96,
					kGODRAYS_FILTERED_DEPTH_0 = 97,
					kGODRAYS_FILTERED_DEPTH_1 = 98,
					kMONITOR_SCREENSHOT = 99,
					kCOUNT = 100
				};

				enum class DEPTH_STENCIL_TARGET : std::int32_t
				{
					kUNBOUND = -1,
					kNONE = 0,
					kMAIN = 1,
					kMAIN_HALF_RES = 2,
					kPIPBOY = 3,
					kVATS = 4,
					kSHADOW_MAP = 5,
					kSHADOW_MAP_ARRAY = 6,
					kCUBEMAP_REFLECTIONS = 7,
					kRAIN_OCCLUSION = 8,
					kVLS_SLICE_STENCIL = 9,
					kGODRAYS = 10,
					kCOMPANION_LOCAL_MAP = 11,
					kCOUNT = 12
				};

				enum class CUBEMAP_RENDER_TARGET : std::int32_t
				{
					kNONE = -1,
					kREFLECTIONS = 0,
					kCOUNT = 1
				};

				enum class RenderTargetCreationMode : std::uint8_t
				{
					kPooled = 0,
					kPersistent = 1,
					kOnDemand = 2
				};

				struct RenderTarget
				{
					/* 0x000 */ ID3D11Texture2D* texture;
					/* 0x008 */ ID3D11Texture2D* copyTexture;
					/* 0x010 */ ID3D11RenderTargetView* rtView;
					/* 0x018 */ ID3D11ShaderResourceView* srView;
					/* 0x020 */ ID3D11ShaderResourceView* copySRView;
					/* 0x028 */ ID3D11UnorderedAccessView* uaView;
				};
				static_assert(sizeof(RenderTarget) == 0x30);
				static_assert(offsetof(RenderTarget, uaView) == 0x28);

				struct DepthStencilTarget
				{
					/* 0x000 */ ID3D11Texture2D* texture;
					/* 0x008 */ ID3D11DepthStencilView* dsView[4];
					/* 0x028 */ ID3D11DepthStencilView* dsViewReadOnlyDepth[4];
					/* 0x048 */ ID3D11DepthStencilView* dsViewReadOnlyStencil[4];
					/* 0x068 */ ID3D11DepthStencilView* dsViewReadOnlyDepthStencil[4];
					/* 0x088 */ ID3D11ShaderResourceView* srViewDepth;
					/* 0x090 */ ID3D11ShaderResourceView* srViewStencil;
				};
				static_assert(sizeof(DepthStencilTarget) == 0x98);
				static_assert(offsetof(DepthStencilTarget, srViewDepth) == 0x88);

				struct CubeMapRenderTarget
				{
					/* 0x000 */ ID3D11Texture2D* texture;
					/* 0x008 */ ID3D11RenderTargetView* rtView[6];
					/* 0x038 */ ID3D11ShaderResourceView* srView;
				};
				static_assert(sizeof(CubeMapRenderTarget) == 0x40);
				static_assert(offsetof(CubeMapRenderTarget, srView) == 0x38);

				// Value-initialize properties; pool matching includes unnamed bytes.
				struct RenderTargetProperties
				{
					/* 0x000 */ std::uint32_t width;
					/* 0x004 */ std::uint32_t height;
					/* 0x008 */ DXGI_FORMAT format;
					/* 0x00C */ std::byte _pad00C[4];
					/* 0x010 */ bool copyable;
					/* 0x011 */ bool supportUnorderedAccess;
					/* 0x012 */ bool allowMipGeneration;
					/* 0x013 */ std::byte _pad013;
					/* 0x014 */ std::int32_t mipLevel;
					/* 0x018 */ std::uint32_t textureTarget;
					/* 0x01C */ std::byte _pad01C[4];
				};
				static_assert(sizeof(RenderTargetProperties) == 0x20);
				static_assert(offsetof(RenderTargetProperties, mipLevel) == 0x14);

				struct DepthStencilTargetProperties
				{
					/* 0x000 */ std::uint32_t width;
					/* 0x004 */ std::uint32_t height;
					/* 0x008 */ std::uint32_t arraySize;
					/* 0x00C */ std::byte _pad00C[0xA];
					/* 0x016 */ bool stencil;
					/* 0x017 */ bool use16BitsDepth;
					/* 0x018 */ std::byte _pad018[4];
				};
				static_assert(sizeof(DepthStencilTargetProperties) == 0x1C);
				static_assert(offsetof(DepthStencilTargetProperties, stencil) == 0x16);

				struct CubeMapRenderTargetProperties
				{
					/* 0x000 */ std::uint32_t width;
					/* 0x004 */ std::uint32_t height;
					/* 0x008 */ DXGI_FORMAT format;
					/* 0x00C */ std::byte _pad00C[0x18];
				};
				static_assert(sizeof(CubeMapRenderTargetProperties) == 0x24);
			}
		}
	}
}
