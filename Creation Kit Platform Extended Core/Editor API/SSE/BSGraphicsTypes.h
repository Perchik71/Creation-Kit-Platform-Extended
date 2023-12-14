// Special thanks to Nukem: 
// https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/BSShader/BSShaderRenderTargets.h
// https://github.com/Nukem9/SkyrimSETest/blob/master/skyrim64_test/src/patches/TES/BSGraphics/BSGraphicsTypes.h

#pragma once

#define MAX_SHARED_PARTICLES_SIZE 2048
#define MAX_PARTICLE_STRIP_COUNT 51200

#define MAX_VS_CONSTANTS 20
#define MAX_PS_CONSTANTS 64
#define MAX_CS_CONSTANTS 32

namespace CreationKitPlatformExtended
{
	namespace EditorAPI
	{
		namespace SkyrimSpectialEdition
		{
			namespace BSGraphics
			{
				namespace BSShaderRenderTargets
				{
					const char* GetRenderTargetName(uint32_t Index);
					const char* GetRenderTargetName_1130(uint32_t Index);
					const char* GetCubemapName(uint32_t Index);
					const char* GetTexture3DName(uint32_t Index);
					const char* GetDepthStencilName(uint32_t Index);
				}

				enum : uint32_t
				{
					RENDER_TARGET_NONE = 0xFFFFFFFF,
					RENDER_TARGET_FRAMEBUFFER = 0,
					RENDER_TARGET_MAIN,
					RENDER_TARGET_MAIN_COPY,
					RENDER_TARGET_MAIN_ONLY_ALPHA,
					RENDER_TARGET_NORMAL_TAAMASK_SSRMASK,
					RENDER_TARGET_NORMAL_TAAMASK_SSRMASK_SWAP,
					RENDER_TARGET_NORMAL_TAAMASK_SSRMASK_DOWNSAMPLED,
					RENDER_TARGET_MOTION_VECTOR,
					RENDER_TARGET_WATER_DISPLACEMENT,
					RENDER_TARGET_WATER_DISPLACEMENT_SWAP,
					RENDER_TARGET_WATER_REFLECTIONS,
					RENDER_TARGET_WATER_FLOW,
					RENDER_TARGET_UNDERWATER_MASK,
					RENDER_TARGET_REFRACTION_NORMALS,
					RENDER_TARGET_MENUBG,
					RENDER_TARGET_PLAYER_FACEGEN_TINT,
					RENDER_TARGET_LOCAL_MAP,
					RENDER_TARGET_LOCAL_MAP_SWAP,
					RENDER_TARGET_SHADOW_MASK,
					RENDER_TARGET_GETHIT_BUFFER,
					RENDER_TARGET_GETHIT_BLURSWAP,
					RENDER_TARGET_BLURFULL_BUFFER,
					RENDER_TARGET_HDR_BLURSWAP,
					RENDER_TARGET_LDR_BLURSWAP,
					RENDER_TARGET_HDR_BLOOM,
					RENDER_TARGET_LDR_DOWNSAMPLE0,
					RENDER_TARGET_HDR_DOWNSAMPLE0,
					RENDER_TARGET_HDR_DOWNSAMPLE1,
					RENDER_TARGET_HDR_DOWNSAMPLE2,
					RENDER_TARGET_HDR_DOWNSAMPLE3,
					RENDER_TARGET_HDR_DOWNSAMPLE4,
					RENDER_TARGET_HDR_DOWNSAMPLE5,
					RENDER_TARGET_HDR_DOWNSAMPLE6,
					RENDER_TARGET_HDR_DOWNSAMPLE7,
					RENDER_TARGET_HDR_DOWNSAMPLE8,
					RENDER_TARGET_HDR_DOWNSAMPLE9,
					RENDER_TARGET_HDR_DOWNSAMPLE10,
					RENDER_TARGET_HDR_DOWNSAMPLE11,
					RENDER_TARGET_HDR_DOWNSAMPLE12,
					RENDER_TARGET_HDR_DOWNSAMPLE13,
					RENDER_TARGET_LENSFLAREVIS,
					RENDER_TARGET_IMAGESPACE_TEMP_COPY,
					RENDER_TARGET_IMAGESPACE_TEMP_COPY2,
					RENDER_TARGET_IMAGESPACE_VOLUMETRIC_LIGHTING,
					RENDER_TARGET_IMAGESPACE_VOLUMETRIC_LIGHTING_PREVIOUS,
					RENDER_TARGET_IMAGESPACE_VOLUMETRIC_LIGHTING_COPY,
					RENDER_TARGET_SAO,
					RENDER_TARGET_SAO_DOWNSCALED,
					RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_0_ESRAM,
					RENDER_TARGET_SAO_CAMERAZ,
					RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_0,
					RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_1,
					RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_2,
					RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_3,
					RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_4,
					RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_5,
					RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_6,
					RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_7,
					RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_8,
					RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_9,
					RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_10,
					RENDER_TARGET_SAO_CAMERAZ_MIP_LEVEL_11,
					RENDER_TARGET_SAO_RAWAO,
					RENDER_TARGET_SAO_RAWAO_DOWNSCALED,
					RENDER_TARGET_SAO_RAWAO_PREVIOUS,
					RENDER_TARGET_SAO_RAWAO_PREVIOUS_DOWNSCALED,
					RENDER_TARGET_SAO_TEMP_BLUR,
					RENDER_TARGET_SAO_TEMP_BLUR_DOWNSCALED,
					RENDER_TARGET_INDIRECT,
					RENDER_TARGET_INDIRECT_DOWNSCALED,
					RENDER_TARGET_RAWINDIRECT,
					RENDER_TARGET_RAWINDIRECT_DOWNSCALED,
					RENDER_TARGET_RAWINDIRECT_PREVIOUS,
					RENDER_TARGET_RAWINDIRECT_PREVIOUS_DOWNSCALED,
					RENDER_TARGET_RAWINDIRECT_SWAP,
					RENDER_TARGET_SAVE_GAME_SCREENSHOT,
					RENDER_TARGET_SCREENSHOT,
					RENDER_TARGET_VOLUMETRIC_LIGHTING_HALF_RES,
					RENDER_TARGET_VOLUMETRIC_LIGHTING_BLUR_HALF_RES,
					RENDER_TARGET_VOLUMETRIC_LIGHTING_QUARTER_RES,
					RENDER_TARGET_VOLUMETRIC_LIGHTING_BLUR_QUARTER_RES,
					RENDER_TARGET_TEMPORAL_AA_ACCUMULATION_1,
					RENDER_TARGET_TEMPORAL_AA_ACCUMULATION_2,
					RENDER_TARGET_TEMPORAL_AA_UI_ACCUMULATION_1,
					RENDER_TARGET_TEMPORAL_AA_UI_ACCUMULATION_2,
					RENDER_TARGET_TEMPORAL_AA_MASK,
					RENDER_TARGET_TEMPORAL_AA_WATER_1,
					RENDER_TARGET_TEMPORAL_AA_WATER_2,
					RENDER_TARGET_RAW_WATER,
					RENDER_TARGET_WATER_1,
					RENDER_TARGET_WATER_2,
					RENDER_TARGET_IBLENSFLARES_LIGHTS_FILTER,
					RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_4X_4X_PING,
					RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_4X_4X_PONG,
					RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_16X_4Y_PING,
					RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_16X_4Y_PONG,
					RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_16X_4Y_BLUR,
					RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_16X_4Y_BLUR_SWAP,
					RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_32X_4Y_PING,
					RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_32X_4Y_PONG,
					RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_32X_4Y_BLUR,
					RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_32X_4Y_BLUR_SWAP,
					RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_16X_16X_PING,
					RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_16X_16X_PONG,
					RENDER_TARGET_IBLENSFLARES_DOWNSAMPLE_16X_16X_SWAP,
					RENDER_TARGET_BOOK_TEXT_0,
					RENDER_TARGET_BOOK_TEXT_1,
					RENDER_TARGET_BOOK_TEXT_2,
					RENDER_TARGET_BOOK_TEXT_3,
					RENDER_TARGET_SSR,
					RENDER_TARGET_SSR_RAW,
					RENDER_TARGET_SSR_BLURRED0,
					RENDER_TARGET_SNOW_SPECALPHA,
					RENDER_TARGET_SNOW_SWAP,

					RENDER_TARGET_COUNT,
					RENDER_TARGET_FRAMEBUFFER_COUNT = 1,
				};

				enum : uint32_t
				{
					RENDER_TARGET_UNKNOWN_1 = RENDER_TARGET_SNOW_SWAP + 1,
					RENDER_TARGET_UNKNOWN_2,

					RENDER_TARGET_COUNT_1130,
				};

				enum : uint32_t
				{
					RENDER_TARGET_CUBEMAP_NONE = 0xFFFFFFFF,
					RENDER_TARGET_CUBEMAP_REFLECTIONS = 0,

					RENDER_TARGET_CUBEMAP_COUNT,
				};

				enum : uint32_t
				{
					TEXTURE3D_NONE = 0xFFFFFFFF,
					VOLUMETRIC_LIGHTING_ACCUMULATION = 0,
					VOLUMETRIC_LIGHTING_ACCUMULATION_COPY,
					VOLUMETRIC_LIGHTING_NOISE,

					TEXTURE3D_COUNT,
				};

				enum : uint32_t
				{
					DEPTH_STENCIL_TARGET_NONE = 0xFFFFFFFF,
					DEPTH_STENCIL_TARGET_MAIN = 0,
					DEPTH_STENCIL_TARGET_MAIN_COPY,
					DEPTH_STENCIL_TARGET_SHADOWMAPS_ESRAM,
					DEPTH_STENCIL_TARGET_VOLUMETRIC_LIGHTING_SHADOWMAPS_ESRAM,
					DEPTH_STENCIL_TARGET_SHADOWMAPS,
					DEPTH_STENCIL_DECAL_OCCLUSION,
					DEPTH_STENCIL_CUBEMAP_REFLECTIONS,
					DEPTH_STENCIL_POST_ZPREPASS_COPY,
					DEPTH_STENCIL_POST_WATER_COPY,
					DEPTH_STENCIL_BOOK_TEXT,
					DEPTH_STENCIL_TARGET_PRECIPITATION_OCCLUSION_MAP,
					DEPTH_STENCIL_TARGET_FOCUS_NEO,

					DEPTH_STENCIL_COUNT,
				};

				//
				// Renderer shadow state settings
				//
				enum
				{
					DIRTY_RENDERTARGET = 0x1,
					DIRTY_VIEWPORT = 0x2,
					DIRTY_DEPTH_MODE = 0x4,
					DIRTY_DEPTH_STENCILREF_MODE = 0x8,
					DIRTY_UNKNOWN1 = 0x10,
					DIRTY_RASTER_CULL_MODE = 0x20,
					DIRTY_RASTER_DEPTH_BIAS = 0x40,
					DIRTY_ALPHA_BLEND = 0x80,
					DIRTY_ALPHA_TEST_REF = 0x100,
					DIRTY_ALPHA_ENABLE_TEST = 0x200,
					DIRTY_VERTEX_DESC = 0x400,
					DIRTY_PRIMITIVE_TOPO = 0x800,
					DIRTY_UNKNOWN2 = 0x1000,
				};

				enum ClearDepthStencilTarget
				{
					CLEAR_DEPTH_STENCIL_TARGET_DEPTH = 1,
					CLEAR_DEPTH_STENCIL_TARGET_STENCIL = 2,
					CLEAR_DEPTH_STENCIL_TARGET_ALL = 3,
				};

				enum SetRenderTargetMode : uint32_t
				{
					SRTM_CLEAR = 0,
					SRTM_CLEAR_DEPTH = 1,
					SRTM_CLEAR_STENCIL = 2,
					SRTM_RESTORE = 3,
					SRTM_NO_CLEAR = 4,
					SRTM_FORCE_COPY_RESTORE = 5,
					SRTM_INIT = 6,
				};

				enum DepthStencilStencilMode
				{
					DEPTH_STENCIL_STENCIL_MODE_DISABLED = 0,

					DEPTH_STENCIL_STENCIL_MODE_DEFAULT = DEPTH_STENCIL_STENCIL_MODE_DISABLED,	// Used for BSShader::RestoreX
				};

				enum DepthStencilDepthMode
				{
					DEPTH_STENCIL_DEPTH_MODE_DISABLED = 0,
					DEPTH_STENCIL_DEPTH_MODE_TEST = 1,
					DEPTH_STENCIL_DEPTH_MODE_TEST_WRITE = 3,
					DEPTH_STENCIL_DEPTH_MODE_TESTEQUAL = 4,										// Unverified
					DEPTH_STENCIL_DEPTH_MODE_TESTGREATER = 6,									// Unverified

					DEPTH_STENCIL_DEPTH_MODE_DEFAULT = DEPTH_STENCIL_DEPTH_MODE_TEST_WRITE,		// Used for BSShader::RestoreX
				};

				enum RasterStateCullMode
				{
					RASTER_STATE_CULL_MODE_BACK = 1,

					RASTER_STATE_CULL_MODE_DEFAULT = RASTER_STATE_CULL_MODE_BACK,				// Used for BSShader::RestoreX
				};

				struct RenderTargetProperties
				{
					uint32_t uiWidth;
					uint32_t uiHeight;
					DXGI_FORMAT eFormat;
					bool bCopyable;
					bool bSupportUnorderedAccess;
					bool bAllowMipGeneration;
					int iMipLevel;
					uint32_t uiTextureTarget;
					uint32_t uiUnknown;
				};
				static_assert(sizeof(RenderTargetProperties) == 0x1C);

				struct DepthStencilTargetProperties
				{
					uint32_t uiWidth;
					uint32_t uiHeight;
					uint32_t uiArraySize;
					bool Unknown1;
					bool Stencil;
					bool Use16BitsDepth;
				};
				static_assert(sizeof(DepthStencilTargetProperties) == 0x10);

				struct CubeMapRenderTargetProperties
				{
					uint32_t uiWidth;
					uint32_t uiHeight;
					DXGI_FORMAT eFormat;
				};
				static_assert(sizeof(CubeMapRenderTargetProperties) == 0xC);

				struct RenderTargetData
				{
					ID3D11Texture2D* Texture;
					ID3D11Texture2D* TextureCopy;
					ID3D11RenderTargetView* RTV;		// For "Texture"
					ID3D11ShaderResourceView* SRV;		// For "Texture"
					ID3D11ShaderResourceView* SRVCopy;	// For "TextureCopy"
					ID3D11UnorderedAccessView* UAV;		// For "Texture"
				};
				static_assert(sizeof(RenderTargetData) == 0x30);

				struct DepthStencilData
				{
					ID3D11Texture2D* Texture;
					ID3D11DepthStencilView* Views[8];
					ID3D11DepthStencilView* ReadOnlyViews[8];
					ID3D11ShaderResourceView* DepthSRV;
					ID3D11ShaderResourceView* StencilSRV;
				};
				static_assert(sizeof(DepthStencilData) == 0x98);

				struct CubemapRenderTargetData
				{
					ID3D11Texture2D* Texture;
					ID3D11RenderTargetView* CubeSideRTV[6];
					ID3D11ShaderResourceView* SRV;
				};
				static_assert(sizeof(CubemapRenderTargetData) == 0x40);

				struct Texture3DTargetData
				{
					char _pad0[0x20];
				};
				static_assert(sizeof(Texture3DTargetData) == 0x20);

				//
				// General resources
				//
				struct Texture
				{
					ID3D11Texture2D* m_Texture;
					char _pad0[0x8];
					ID3D11ShaderResourceView* m_ResourceView;
				};

				struct Buffer
				{
					ID3D11Buffer* m_Buffer;	// Selected from pool in Load*ShaderFromFile()
					void* m_Data;			// m_Data = DeviceContext->Map(m_Buffer)

					// Based on shader load flags, these **can be null**. At least one of the
					// pointers is guaranteed to be non-null.
				};

				//
				// Constant group types used for shader parameters
				//
				const uint8_t INVALID_CONSTANT_BUFFER_OFFSET = 0xFF;

				enum ConstantGroupLevel
				{
					CONSTANT_GROUP_LEVEL_TECHNIQUE = 0x0,		// Varies between PS/VS shaders
					CONSTANT_GROUP_LEVEL_MATERIAL = 0x1,		// Varies between PS/VS shaders
					CONSTANT_GROUP_LEVEL_GEOMETRY = 0x2,		// Varies between PS/VS shaders
					CONSTANT_GROUP_LEVEL_COUNT = 0x3,

					// Slot 7 is used for grass but unknown
					CONSTANT_GROUP_LEVEL_INSTANCE = 0x8,		// Instanced geometry such as grass and trees
					CONSTANT_GROUP_LEVEL_PREVIOUS_BONES = 0x9,
					CONSTANT_GROUP_LEVEL_BONES = 0xA,
					CONSTANT_GROUP_LEVEL_ALPHA_TEST_REF = 0xB,	// PS/VS. Used as a single float value for alpha testing (16 bytes allocated)
					CONSTANT_GROUP_LEVEL_PERFRAME = 0xC,		// PS/VS. Per-frame constants. Contains view projections and some other variables.
				};

				//
				// Shaders
				//
#pragma warning(push)
#pragma warning(disable:4200) // MSVC
#pragma warning(disable:94)   // Intel C++ Compiler
				struct VertexShader
				{
					uint32_t m_TechniqueID;						// Bit flags
					ID3D11VertexShader* m_Shader;				// DirectX handle
					uint32_t m_ShaderLength;					// Raw bytecode length

					union
					{
						struct
						{
							Buffer m_PerTechnique;				// CONSTANT_GROUP_LEVEL_TECHNIQUE
							Buffer m_PerMaterial;				// CONSTANT_GROUP_LEVEL_MATERIAL
							Buffer m_PerGeometry;				// CONSTANT_GROUP_LEVEL_GEOMETRY
						};

						Buffer m_ConstantGroups[CONSTANT_GROUP_LEVEL_COUNT];
					};

					uint64_t m_VertexDescription;				// ID3D11Device::CreateInputLayout lookup (for VSMain)
					uint8_t m_ConstantOffsets[MAX_VS_CONSTANTS];// Actual offset is multiplied by 4
					uint8_t __padding[4];
					uint8_t m_RawBytecode[0];					// Raw bytecode
				};
				static_assert(sizeof(VertexShader) == 0x68);

				struct PixelShader
				{
					uint32_t m_TechniqueID;						// Bit flags
					ID3D11PixelShader* m_Shader;				// DirectX handle

					union
					{
						struct
						{
							Buffer m_PerTechnique;				// CONSTANT_GROUP_LEVEL_TECHNIQUE
							Buffer m_PerMaterial;				// CONSTANT_GROUP_LEVEL_MATERIAL
							Buffer m_PerGeometry;				// CONSTANT_GROUP_LEVEL_GEOMETRY
						};

						Buffer m_ConstantGroups[CONSTANT_GROUP_LEVEL_COUNT];
					};

					uint8_t m_ConstantOffsets[MAX_PS_CONSTANTS];// Actual offset is multiplied by 4
				};
				static_assert(sizeof(PixelShader) == 0x80);

				struct ComputeShader
				{
					char _pad0[0x8];
					Buffer m_PerTechnique;						// CONSTANT_GROUP_LEVEL_TECHNIQUE
					char _pad1[0xC];
					Buffer m_PerMaterial;						// CONSTANT_GROUP_LEVEL_MATERIAL
					char _pad2[0xC];
					Buffer m_PerGeometry;						// CONSTANT_GROUP_LEVEL_GEOMETRY
					char _pad3[0x4];
					ID3D11ComputeShader* m_Shader;				// DirectX handle
					uint32_t m_TechniqueID;						// Bit flags
					uint32_t m_ShaderLength;					// Raw bytecode length
					uint8_t m_ConstantOffsets[MAX_CS_CONSTANTS];// Actual offset is multiplied by 4
					uint8_t m_RawBytecode[0];					// Raw bytecode
				};
				static_assert(sizeof(ComputeShader) == 0x90);

				// Not part of the vanilla game
				struct HullShader
				{
					uint32_t m_TechniqueID;						// Bit flags
					ID3D11HullShader* m_Shader;					// DirectX handle
				};

				// Not part of the vanilla game
				struct DomainShader
				{
					uint32_t m_TechniqueID;						// Bit flags
					ID3D11DomainShader* m_Shader;				// DirectX handle
				};

#pragma warning(pop)

				//
				// Renderer-specific types to handle uploading raw data to the GPU
				//
				struct LineShape
				{
					ID3D11Buffer* m_VertexBuffer;
					ID3D11Buffer* m_IndexBuffer;
					uint64_t m_VertexDesc;
					uint32_t m_RefCount;
				};

				struct TriShape : LineShape
				{
					void* m_RawVertexData;
					void* m_RawIndexData;
				};

				struct DynamicTriShape
				{
					ID3D11Buffer* m_VertexBuffer;
					ID3D11Buffer* m_IndexBuffer;
					uint64_t m_VertexDesc;
					uint32_t m_VertexAllocationOffset;
					uint32_t m_VertexAllocationSize;
					uint32_t m_RefCount;
					void* m_RawVertexData;
					void* m_RawIndexData;
				};

				struct DynamicTriShapeData
				{
					ID3D11Buffer* m_VertexBuffer;
					ID3D11Buffer* m_IndexBuffer;
					uint64_t m_VertexDesc;
				};

				struct DynamicTriShapeDrawData
				{
					uint32_t m_Offset;
				};
			}
		}
	}
}