// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/BSTArray.h>
#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			class OptimizationLoad : public Common::Patch
			{
				struct z_stream_s
				{
					const void* next_in;
					std::uint32_t avail_in;
					std::uint32_t total_in;
					void* next_out;
					std::uint32_t avail_out;
					std::uint32_t total_out;
					const char* msg;
					struct internal_state* state;
				};

				static void sub(std::int64_t a1) noexcept(true);
				static int HKInflateInit(z_stream_s* Stream, const char* Version, std::int32_t Mode) noexcept(true);
				static int HKInflate(z_stream_s* Stream, std::int32_t Flush) noexcept(true);
				static HANDLE HKFindFirstFileA(LPCSTR lpFileName, LPWIN32_FIND_DATAA lpFindFileData) noexcept(true);

				static std::uint32_t HKSearchIndexOffset32(EditorAPI::BSTArray<std::uint32_t>& _array,
					std::uint32_t& _target, std::uint32_t _start_index) noexcept(true);
				inline static std::uint32_t HKSearchIndex32(EditorAPI::BSTArray<std::uint32_t>& _array,
					std::uint32_t& _target) noexcept(true)
				{
					return HKSearchIndexOffset32(_array, _target, 0);
				}
				static std::uint32_t HKSearchIndexOffset64(EditorAPI::BSTArray<std::uint64_t>& _array,
					std::uint64_t& _target, std::uint32_t _start_index) noexcept(true);
				inline static std::uint32_t HKSearchIndex64(EditorAPI::BSTArray<std::uint64_t>& _array,
					std::uint64_t& _target) noexcept(true)
				{
					return HKSearchIndexOffset64(_array, _target, 0);
				}

				OptimizationLoad(const OptimizationLoad&) = delete;
				OptimizationLoad& operator=(const OptimizationLoad&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				OptimizationLoad();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);
			};
		}
	}
}