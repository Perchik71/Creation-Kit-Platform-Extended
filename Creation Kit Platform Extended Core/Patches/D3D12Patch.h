// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include "Core/Module.h"
#include "Core/Relocator.h"
#include "Core/RelocationDatabase.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		using namespace CreationKitPlatformExtended::Core;

		class D3D12Patch : public Module
		{
		public:
			D3D12Patch();

			virtual bool HasOption() const;
			virtual bool HasCanRuntimeDisabled() const;
			virtual const char* GetOptionName() const;
			virtual const char* GetName() const;
			virtual bool HasDependencies() const;
			virtual Array<String> GetDependencies() const;

			static HRESULT WINAPI HKCreateDXGIFactory(REFIID riid, void** ppFactory);
			static HRESULT WINAPI HKD3D12CreateDevice(IUnknown* pAdapter, D3D_FEATURE_LEVEL MinimumFeatureLevel,
				REFIID riid, void** ppDevice);
		protected:
			virtual bool QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const;
			virtual bool Activate(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
			virtual bool Shutdown(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem);
		private:
			D3D12Patch(const D3D12Patch&) = default;
			D3D12Patch& operator=(const D3D12Patch&) = default;

			HMODULE moduleDXGI;
			HMODULE moduleD3D12;
		};
	}
}