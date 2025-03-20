// Copyright © 2025 aka perchik71.All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <wrl/client.h>
#include <d3d11.h>

#include <DirectXMath.h>

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		using namespace Microsoft::WRL;

		enum D3DStreamOffset
		{
			kD3DStreamBegin = 0,
			kD3DStreamCurrent,
			kD3DStreamEnd
		};

		typedef void(*D3DStreamReadEvent)(void* Buf, int64_t Size);
		typedef void(*D3DStreamWriteEvent)(const void* Buf, int64_t Size);

		class D3DCustomStream
		{
			bool _abort;
		protected:
			void DoReadBuf(void* Buf, int64_t Size) const noexcept;
			void DoWriteBuf(const void* Buf, int64_t Size) const noexcept;
		public:
			D3DCustomStream();
			virtual ~D3DCustomStream() = default;

			D3DCustomStream(const D3DCustomStream& Rhs);
			D3DCustomStream& operator=(const D3DCustomStream& Rhs);

			D3DStreamReadEvent OnReadBuf;
			D3DStreamWriteEvent OnWriteBuf;

			virtual int32_t ReadBuf(void* Buf, int32_t Size) const = 0;
			virtual int32_t WriteBuf(const void* Buf, int32_t Size) = 0;
			virtual int64_t Seek(int64_t Offset, D3DStreamOffset Flag) const = 0;

			virtual int64_t CopyFrom(D3DCustomStream& Steam, int64_t Size = 0);
			inline void Assign(D3DCustomStream& Steam) { CopyFrom(Steam); }
			inline void Abort() noexcept { _abort = true; }

			[[nodiscard]] virtual int64_t GetPosition() const noexcept;
			virtual void SetPosition(int64_t Position) const noexcept;

			[[nodiscard]] virtual int64_t GetSize() const noexcept;

			[[nodiscard]] inline bool IsEmpty() const { return GetSize() == 0; }
			[[nodiscard]] inline bool Eof() const { return GetSize() == GetPosition(); }

			PROPERTY(GetPosition, SetPosition) int64_t Position;
			READ_PROPERTY(GetSize) int64_t Size;
		};

		enum class D3DFileStreamMode
		{
			kD3DStreamCreate = 0,
			kD3DStreamOpenRead,
			kD3DStreamOpenReadWrite
		};

		class D3DFileStream : public D3DCustomStream
		{
			void* _handle;
			String _FileName;
		public:
			D3DFileStream();
			explicit D3DFileStream(const char* FileName, D3DFileStreamMode Mode, bool Cache = true);
			explicit D3DFileStream(const wchar_t* FileName, D3DFileStreamMode Mode, bool Cache = true);
			D3DFileStream(const String& FileName, D3DFileStreamMode Mode, bool Cache = true);
			virtual ~D3DFileStream();

			D3DFileStream(const D3DFileStream& Rhs) = delete;
			D3DFileStream& operator=(const D3DFileStream& Rhs) = delete;

			virtual int32_t ReadBuf(void* Buf, int32_t Size) const;
			virtual int32_t WriteBuf(const void* Buf, int32_t Size);
			virtual int64_t Seek(int64_t Offset, D3DStreamOffset Flag) const;

			virtual bool Open(const char* FileName, D3DFileStreamMode Mode, bool Cache = true);
			virtual bool Open(const wchar_t* FileName, D3DFileStreamMode Mode, bool Cache = true);
			virtual bool Open(const String& FileName, D3DFileStreamMode Mode, bool Cache = true);
			[[nodiscard]] virtual bool IsOpen() const noexcept;
			virtual void Close();
			virtual void Flush() const noexcept;

			[[nodiscard]] virtual String GetFileName() const noexcept;

			READ_PROPERTY(GetFileName) String FileName;
		};

		class D3DFileStreamInterfaceMethods
		{
		public:
			bool LoadFromFile(const char* FileName);
			bool LoadFromFile(const wchar_t* FileName);
			bool SaveToFile(const char* FileName) const;
			bool SaveToFile(const wchar_t* FileName) const;
			virtual bool LoadFromStream(D3DCustomStream& Steam) = 0;
			virtual bool SaveToStream(D3DCustomStream& Steam) const = 0;
		};

		class D3DMemoryStream : public D3DCustomStream, public D3DFileStreamInterfaceMethods
		{
			void* _data;
			int64_t _pos;
			int64_t _size;
		protected:
			void* Allocate(int64_t NewSize);
			void Deallocate();
		public:
			D3DMemoryStream();
			D3DMemoryStream(const void* Buf, int32_t Size);
			virtual ~D3DMemoryStream();
			D3DMemoryStream(const D3DMemoryStream& Rhs);
			D3DMemoryStream& operator=(const D3DMemoryStream& Rhs);

			virtual int32_t ReadBuf(void* Buf, int32_t Size) const;
			virtual int32_t WriteBuf(const void* Buf, int32_t Size);
			virtual int64_t Seek(int64_t Offset, D3DStreamOffset Flag) const;
			virtual void SetSize(int64_t NewSize);

			virtual void Clear();
			inline void* Memory() const noexcept { return _data; }

			virtual bool LoadFromStream(D3DCustomStream& Steam);
			virtual bool SaveToStream(D3DCustomStream& Steam) const;
		};

		class D3D11ShaderEngine;

		enum : uint32_t
		{
			CKPE_PIXEL_SHADER = 1 << 0,
			CKPE_VERTEX_SHADER = 1 << 1,
			CKPE_COMPUTE_SHADER = 1 << 2,
		};

		class D3D11Object
		{
		public:
			D3D11Object(D3D11ShaderEngine* Engine, const char* Name);
			virtual ~D3D11Object() = default;

			D3D11Object(const D3D11Object&) = delete;
			D3D11Object& operator=(const D3D11Object&) = delete;

			[[nodiscard]] virtual inline D3D11ShaderEngine* Engine() const noexcept(true) { return _Engine; }
			[[nodiscard]] virtual inline String Name() const noexcept(true) { return _Name; }
		private:
			D3D11ShaderEngine* _Engine;
			String _Name;
		};

		// https://github.com/fholger/vrperfkit/blob/a52f8a45d330d0b66206aee85165db715e4482cd/src/d3d11/d3d11_helper.h#L22
		class D3D11State : public D3D11Object
		{
			ComPtr<ID3D11VertexShader> VertexShader;
			ComPtr<ID3D11PixelShader> PixelShader;
			ComPtr<ID3D11ComputeShader> ComputeShader;
			ComPtr<ID3D11InputLayout> InputLayout;
			D3D11_PRIMITIVE_TOPOLOGY Topology;
			ID3D11Buffer* VertexBuffers[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
			UINT Strides[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
			UINT Offsets[D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT];
			ComPtr<ID3D11Buffer> IndexBuffer;
			DXGI_FORMAT Format;
			UINT Offset;
			ID3D11RenderTargetView* RenderTargets[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
			ComPtr<ID3D11DepthStencilView> DepthStencil;
			ComPtr<ID3D11RasterizerState> RasterizerState;
			ComPtr<ID3D11DepthStencilState> DepthStencilState;
			UINT StencilRef;
			D3D11_VIEWPORT Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
			UINT NumViewports = 0;
			ComPtr<ID3D11Buffer> VSConstantBuffer;
			ComPtr<ID3D11Buffer> PSConstantBuffer;
			ComPtr<ID3D11Buffer> CSConstantBuffer;
			UINT NumScissorRects;
			D3D11_RECT RSScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
			ComPtr<ID3D11RasterizerState> RS;
			ComPtr<ID3D11BlendState> BlendState;
			UINT SampleMask;
			FLOAT BlendFactor[4];
			ID3D11ShaderResourceView* CSShaderResources[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT];
			ID3D11UnorderedAccessView* CSUavs[D3D11_1_UAV_SLOT_COUNT];
		public:
			D3D11State(D3D11ShaderEngine* Engine, const char* Name);
			virtual ~D3D11State() = default;

			virtual void PushState() noexcept(true);
			virtual void PopState() noexcept(true);

			D3D11State(const D3D11State&) = delete;
			D3D11State& operator=(const D3D11State&) = delete;
		};

		class D3D11ShaderTexture : public D3D11Object
		{
			D3D11_TEXTURE2D_DESC _Desc;
			ComPtr<ID3D11Texture2D> _Texture;
		public:
			D3D11ShaderTexture(D3D11ShaderEngine* Engine, const char* Name);
			D3D11ShaderTexture(D3D11ShaderEngine* Engine, const char* Name, const D3D11_TEXTURE2D_DESC* Desc);
			D3D11ShaderTexture(D3D11ShaderEngine* Engine, const char* Name, const ID3D11Texture2D* Texture,
				uint32_t BindFlags = D3D11_BIND_SHADER_RESOURCE);
			D3D11ShaderTexture(D3D11ShaderEngine* Engine, const char* Name, const ID3D11Resource* Resource, 
				uint32_t BindFlags = D3D11_BIND_SHADER_RESOURCE);

			D3D11ShaderTexture(const D3D11ShaderTexture&) = delete;
			D3D11ShaderTexture& operator=(const D3D11ShaderTexture&) = delete;

			virtual bool SaveTextureToFileAsDDS(const char* FileName) const noexcept(true);
			virtual void DebugInfo() const noexcept(true);
			virtual bool Create(const D3D11_TEXTURE2D_DESC* Desc) noexcept(true);
			virtual bool Create(const ID3D11Texture2D* Texture, uint32_t BindFlags = D3D11_BIND_SHADER_RESOURCE) noexcept(true);
			virtual bool Create(const ID3D11Resource* Resource, uint32_t BindFlags = D3D11_BIND_SHADER_RESOURCE) noexcept(true);

			virtual bool CopyFrom(const D3D11ShaderTexture* Texture) noexcept(true);
			virtual bool CopyFrom(const ID3D11Texture2D* Texture) noexcept(true);
			virtual bool CopyFrom(const ID3D11Resource* Resource) noexcept(true);

			[[nodiscard]] virtual inline bool IsEmpty() const noexcept(true) { return _Texture == nullptr; }
			[[nodiscard]] virtual inline ID3D11Texture2D* Get() const noexcept(true) { return _Texture.Get(); }
			[[nodiscard]] virtual inline ID3D11Texture2D** GetAddressOf() noexcept(true) { return _Texture.GetAddressOf(); }
			[[nodiscard]] virtual inline ID3D11Texture2D** ReleaseAndGetAddressOf() noexcept(true) { return _Texture.ReleaseAndGetAddressOf(); }
			[[nodiscard]] virtual inline const D3D11_TEXTURE2D_DESC* GetDesc() const noexcept(true) { return &_Desc; }
		};

		class D3D11ShaderBuffer : public D3D11Object
		{
			uint32_t IPType, BindId[3];
			D3D11_BUFFER_DESC _Desc;
			ComPtr<ID3D11Buffer> _Buffer;
		public:
			D3D11ShaderBuffer(D3D11ShaderEngine* Engine, const char* Name);
			D3D11ShaderBuffer(D3D11ShaderEngine* Engine, const char* Name, const void* Buffer, uint32_t Size, uint32_t BindFlags, 
				D3D11_USAGE Usage = D3D11_USAGE_DEFAULT);

			D3D11ShaderBuffer(const D3D11ShaderBuffer&) = delete;
			D3D11ShaderBuffer& operator=(const D3D11ShaderBuffer&) = delete;

			virtual bool Create(const void* Buffer, uint32_t Size, uint32_t BindFlags, D3D11_USAGE Usage = D3D11_USAGE_DEFAULT,
				uint32_t CPUAccess = 0) noexcept(true);
			
			virtual void Bind(uint32_t Type, uint32_t BindID) noexcept(true);
			virtual void Unbind() noexcept(true);

			[[nodiscard]] virtual inline bool IsEmpty() const noexcept(true) { return _Buffer == nullptr; }
			[[nodiscard]] virtual ID3D11Buffer* Get() const noexcept(true) { return _Buffer.Get(); }
			[[nodiscard]] virtual ID3D11Buffer** GetAddressOf() noexcept(true) { return _Buffer.GetAddressOf(); }
			[[nodiscard]] virtual ID3D11Buffer** ReleaseAndGetAddressOf() noexcept(true) { return _Buffer.ReleaseAndGetAddressOf(); }
			[[nodiscard]] virtual inline const D3D11_BUFFER_DESC* GetDesc() const noexcept(true) { return &_Desc; }
		};

		class D3D11UnorderedAccessView : public D3D11Object
		{
			UINT Counts, IPType, BindId;
			D3D11_UNORDERED_ACCESS_VIEW_DESC _Desc;
			ComPtr<ID3D11UnorderedAccessView> _View;
		public:
			D3D11UnorderedAccessView(D3D11ShaderEngine* Engine, const char* Name);
			D3D11UnorderedAccessView(D3D11ShaderEngine* Engine, const char* Name, const ID3D11Resource* Resource,
				const D3D11_UNORDERED_ACCESS_VIEW_DESC* Desc);
			D3D11UnorderedAccessView(D3D11ShaderEngine* Engine, const char* Name, const ID3D11UnorderedAccessView* View);

			D3D11UnorderedAccessView(const D3D11UnorderedAccessView&) = delete;
			D3D11UnorderedAccessView& operator=(const D3D11UnorderedAccessView&) = delete;

			virtual void DebugInfo() const noexcept(true);
			virtual bool Create(const ID3D11Resource* Resource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* Desc) noexcept(true);
			virtual bool Create(const ID3D11UnorderedAccessView* View) noexcept(true);
			virtual void Bind(uint32_t Type, uint32_t BindID) noexcept(true);
			virtual void Unbind() noexcept(true);

			[[nodiscard]] virtual inline bool IsEmpty() const noexcept(true) { return _View == nullptr; }
			[[nodiscard]] virtual inline ID3D11UnorderedAccessView* Get() const noexcept(true) { return _View.Get(); }
			[[nodiscard]] virtual inline ID3D11UnorderedAccessView** GetAddressOf() noexcept(true) { return _View.GetAddressOf(); }
			[[nodiscard]] virtual inline ID3D11UnorderedAccessView** ReleaseAndGetAddressOf() noexcept(true) { return _View.ReleaseAndGetAddressOf(); }
			[[nodiscard]] virtual inline const D3D11_UNORDERED_ACCESS_VIEW_DESC* GetDesc() const noexcept(true) { return &_Desc; }
		};

		class D3D11ShaderResourceView : public D3D11Object
		{
			uint32_t IPType, BindId[3];
			D3D11_SHADER_RESOURCE_VIEW_DESC _Desc;
			ComPtr<ID3D11ShaderResourceView> _ResourceView;
		public:
			D3D11ShaderResourceView(D3D11ShaderEngine* Engine, const char* Name);
			D3D11ShaderResourceView(D3D11ShaderEngine* Engine, const char* Name, const ID3D11Resource* Resource, 
				const D3D11_SHADER_RESOURCE_VIEW_DESC* Desc);
			D3D11ShaderResourceView(D3D11ShaderEngine* Engine, const char* Name, const ID3D11ShaderResourceView* View);

			D3D11ShaderResourceView(const D3D11ShaderResourceView&) = delete;
			D3D11ShaderResourceView& operator=(const D3D11ShaderResourceView&) = delete;

			virtual void DebugInfo() const noexcept(true);
			virtual bool Create(const ID3D11Resource* Resource, const D3D11_SHADER_RESOURCE_VIEW_DESC* Desc) noexcept(true);
			virtual bool Create(const ID3D11ShaderResourceView* View) noexcept(true);
			
			virtual void Bind(uint32_t Type, uint32_t BindID) noexcept(true);
			virtual void Unbind() noexcept(true);

			[[nodiscard]] virtual inline bool IsEmpty() const noexcept(true) { return _ResourceView == nullptr; }
			[[nodiscard]] virtual inline ID3D11ShaderResourceView* Get() const noexcept(true) { return _ResourceView.Get(); }
			[[nodiscard]] virtual inline ID3D11ShaderResourceView** GetAddressOf() noexcept(true) { return _ResourceView.GetAddressOf(); }
			[[nodiscard]] virtual inline ID3D11ShaderResourceView** ReleaseAndGetAddressOf() noexcept(true) { return _ResourceView.ReleaseAndGetAddressOf(); }
			[[nodiscard]] virtual inline const D3D11_SHADER_RESOURCE_VIEW_DESC* GetDesc() const noexcept(true) { return &_Desc; }
		};

		class D3D11SamplerState : public D3D11Object
		{
			uint32_t IPType, BindId[3];
			D3D11_SAMPLER_DESC _Desc;
			ComPtr<ID3D11SamplerState> _SamplerState;
		public:
			D3D11SamplerState(D3D11ShaderEngine* Engine, const char* Name);
			D3D11SamplerState(D3D11ShaderEngine* Engine, const char* Name, const D3D11_SAMPLER_DESC* Desc);

			D3D11SamplerState(const D3D11SamplerState&) = delete;
			D3D11SamplerState& operator=(const D3D11SamplerState&) = delete;

			virtual void DebugInfo() const noexcept(true);
			virtual bool Create(const D3D11_SAMPLER_DESC* Desc) noexcept(true);
			virtual void Bind(uint32_t Type, uint32_t BindID) noexcept(true);
			virtual void Unbind() noexcept(true);

			[[nodiscard]] virtual inline bool IsEmpty() const noexcept(true) { return _SamplerState == nullptr; }
			[[nodiscard]] virtual inline ID3D11SamplerState* Get() const noexcept(true) { return _SamplerState.Get(); }
			[[nodiscard]] virtual inline ID3D11SamplerState** GetAddressOf() noexcept(true) { return _SamplerState.GetAddressOf(); }
			[[nodiscard]] virtual inline ID3D11SamplerState** ReleaseAndGetAddressOf() noexcept(true) { return _SamplerState.ReleaseAndGetAddressOf(); }
			[[nodiscard]] virtual inline const D3D11_SAMPLER_DESC* GetDesc() const noexcept(true) { return &_Desc; }
		};

		class D3D11RenderTargetView : public D3D11Object
		{
			D3D11_RENDER_TARGET_VIEW_DESC _Desc;
			ComPtr<ID3D11RenderTargetView> _RenderTarget;
		public:
			D3D11RenderTargetView(D3D11ShaderEngine* Engine, const char* Name);
			D3D11RenderTargetView(D3D11ShaderEngine* Engine, const char* Name, const ID3D11Resource* Resource, 
				const D3D11_RENDER_TARGET_VIEW_DESC* Desc);
			D3D11RenderTargetView(D3D11ShaderEngine* Engine, const char* Name, const ID3D11RenderTargetView* View);

			D3D11RenderTargetView(const D3D11RenderTargetView&) = delete;
			D3D11RenderTargetView& operator=(const D3D11RenderTargetView&) = delete;

			virtual void DebugInfo() const noexcept(true);
			virtual bool Create(const ID3D11Resource* Resource, const D3D11_RENDER_TARGET_VIEW_DESC* Desc) noexcept(true);
			virtual bool Create(const ID3D11RenderTargetView* View) noexcept(true);

			[[nodiscard]] virtual inline bool IsEmpty() const noexcept(true) { return _RenderTarget == nullptr; }
			[[nodiscard]] virtual inline ID3D11RenderTargetView* Get() const noexcept(true) { return _RenderTarget.Get(); }
			[[nodiscard]] virtual inline ID3D11RenderTargetView** GetAddressOf() noexcept(true) { return _RenderTarget.GetAddressOf(); }
			[[nodiscard]] virtual inline ID3D11RenderTargetView** ReleaseAndGetAddressOf() noexcept(true) { return _RenderTarget.ReleaseAndGetAddressOf(); }
			[[nodiscard]] virtual inline const D3D11_RENDER_TARGET_VIEW_DESC* GetDesc() const noexcept(true) { return &_Desc; }
		};

		class D3D11DepthStencilView : public D3D11Object
		{
			D3D11_DEPTH_STENCIL_VIEW_DESC _Desc;
			ComPtr<ID3D11DepthStencilView> _DepthTarget;
		public:
			D3D11DepthStencilView(D3D11ShaderEngine* Engine, const char* Name);
			D3D11DepthStencilView(D3D11ShaderEngine* Engine, const char* Name, const ID3D11Resource* Resource, 
				const D3D11_DEPTH_STENCIL_VIEW_DESC* Desc);
			D3D11DepthStencilView(D3D11ShaderEngine* Engine, const char* Name, const ID3D11DepthStencilView* View);

			D3D11DepthStencilView(const D3D11DepthStencilView&) = delete;
			D3D11DepthStencilView& operator=(const D3D11DepthStencilView&) = delete;

			virtual void DebugInfo() const noexcept(true);
			virtual bool Create(const ID3D11Resource* Resource, const D3D11_DEPTH_STENCIL_VIEW_DESC* Desc) noexcept(true);
			virtual bool Create(const ID3D11DepthStencilView* View) noexcept(true);

			[[nodiscard]] virtual inline bool IsEmpty() const noexcept(true) { return _DepthTarget == nullptr; }
			[[nodiscard]] virtual inline ID3D11DepthStencilView* Get() const noexcept(true) { return _DepthTarget.Get(); }
			[[nodiscard]] virtual inline ID3D11DepthStencilView** GetAddressOf() noexcept(true) { return _DepthTarget.GetAddressOf(); }
			[[nodiscard]] virtual inline ID3D11DepthStencilView** ReleaseAndGetAddressOf() noexcept(true) { return _DepthTarget.ReleaseAndGetAddressOf(); }
			[[nodiscard]] virtual inline const D3D11_DEPTH_STENCIL_VIEW_DESC* GetDesc() const noexcept(true) { return &_Desc; }
		};

		class D3D11Shader : public D3D11Object
		{
		protected:
			D3DMemoryStream StreamBinary;
		public:
			D3D11Shader(D3D11ShaderEngine* Engine, const char* Name);
			
			D3D11Shader(const D3D11Shader&) = delete;
			D3D11Shader& operator=(const D3D11Shader&) = delete;

			virtual void Bind() noexcept(true) = 0;
			virtual void Unbind() noexcept(true) = 0;
			virtual bool Install() noexcept(true) = 0;
			virtual void Shutdown() noexcept(true) = 0;

			[[nodiscard]] virtual bool LoadFromResource(HMODULE Module, uint32_t ResourceID) noexcept(true);
			[[nodiscard]] virtual bool LoadFromResource(HMODULE Module, const char* ResourceName) noexcept(true);
			[[nodiscard]] virtual bool LoadFromStream(D3DCustomStream& Steam) noexcept(true);
			[[nodiscard]] virtual bool LoadFromMemory(const void* Buf, int32_t Size) noexcept(true);
			[[nodiscard]] virtual bool LoadFromFile(const char* FileName) noexcept(true);
			[[nodiscard]] virtual bool LoadFromFile(const wchar_t* FileName) noexcept(true);
			[[nodiscard]] virtual bool CompileFromFile(const char* FileName, const char* TypeShader, const char* EntryPoint) noexcept(true);
			[[nodiscard]] virtual bool CompileFromFile(const wchar_t* FileName, const char* TypeShader, const char* EntryPoint) noexcept(true);
			[[nodiscard]] virtual inline bool IsEmpty() const noexcept(true) { return StreamBinary.IsEmpty(); }
		};

		class D3D11PixelShader : public D3D11Shader
		{
			ComPtr<ID3D11PixelShader> _PixelShader;
		public:
			D3D11PixelShader(D3D11ShaderEngine* Engine, const char* Name);

			D3D11PixelShader(const D3D11PixelShader&) = delete;
			D3D11PixelShader& operator=(const D3D11PixelShader&) = delete;

			virtual void Bind() noexcept(true);
			virtual void Unbind() noexcept(true);
			virtual bool Install() noexcept(true);
			virtual void Shutdown() noexcept(true);
		};

		class D3D11VertexShader : public D3D11Shader
		{
			ComPtr<ID3D11VertexShader> _VertexShader;
			ComPtr<ID3D11InputLayout> _InputLayout;
			std::unique_ptr<D3D11ShaderBuffer> _InputBuffer;
			uint32_t _SizeBuffer;
			uint32_t _StrideBuffer;
			uint32_t _OffsetBuffer;
			D3D_PRIMITIVE_TOPOLOGY _Topology;
		public:
			D3D11VertexShader(D3D11ShaderEngine* Engine, const char* Name);
			
			D3D11VertexShader(const D3D11VertexShader&) = delete;
			D3D11VertexShader& operator=(const D3D11VertexShader&) = delete;

			virtual void Bind() noexcept(true);
			virtual void Unbind() noexcept(true);
			virtual bool Install() noexcept(true);
			virtual void Shutdown() noexcept(true);

			virtual bool InitData(D3D_PRIMITIVE_TOPOLOGY Topology, const std::initializer_list<DirectX::XMFLOAT2>& InputData) noexcept(true);
		};

		class D3D11ComputeShader : public D3D11Shader
		{
			ComPtr<ID3D11ComputeShader> _ComputeShader;
		public:
			D3D11ComputeShader(D3D11ShaderEngine* Engine, const char* Name);
			
			D3D11ComputeShader(const D3D11ComputeShader&) = delete;
			D3D11ComputeShader& operator=(const D3D11ComputeShader&) = delete;

			virtual void Dispatch(uint32_t ThreadGroupCountX, uint32_t ThreadGroupCountY, uint32_t ThreadGroupCountZ) noexcept(true);
			virtual void Bind() noexcept(true);
			virtual void Unbind() noexcept(true);
			virtual bool Install() noexcept(true);
			virtual void Shutdown() noexcept(true);
		};

		class D3D11ShaderEngine
		{
		public:
			D3D11ShaderEngine(ID3D11Device* Device, ID3D11DeviceContext* DeviceContext);
			virtual ~D3D11ShaderEngine() = default;

			[[nodiscard]] virtual inline ID3D11Device* Device() const noexcept(true) { return _Device; }
			[[nodiscard]] virtual inline ID3D11DeviceContext* DeviceContext() const noexcept(true) { return _DeviceContext; }

			virtual void UnbindResource(uint32_t Type, uint32_t BindID) noexcept(true);
		private:
			ID3D11Device* _Device;
			ID3D11DeviceContext* _DeviceContext;
			std::unique_ptr<D3D11ShaderTexture> _EmptyTexture;
			std::unique_ptr<D3D11ShaderResourceView> _EmptyResource;
		};

		extern D3D11ShaderEngine* GlobalD3D11ShaderEngine;
	}
}