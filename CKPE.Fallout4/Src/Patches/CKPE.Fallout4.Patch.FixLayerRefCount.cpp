// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Application.h>
#include <EditorAPI/IDs.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.Common.Relocation.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <Patches/CKPE.Fallout4.Patch.FixLayerRefCount.h>

#include <xbyak.h>

#include <cstring>

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			// TESObjectREFR::Set3D(this = rbp, node = rsi, 3D holder ExtraData = rdi): replace path, taken when
			// the reference already has a 3D (ExtraData 0x4A) and a new non-null node is set. It swaps the node
			// pointer, then falls into the shared tail that calls the refresh function and sends
			// NotifyLoadState3D(formID, true). The notify is skipped here: the reference was already announced
			// as loaded and nothing announces it as unloaded in between.
			class ReplaceNodeHook : public Xbyak::CodeGenerator
			{
			public:
				ReplaceNodeHook(std::uintptr_t Refresh, std::uintptr_t Epilogue) : Xbyak::CodeGenerator()
				{
					Xbyak::Label skip;

					mov(rcx, ptr[rdi + 0x18]);
					cmp(rcx, rsi);
					jz(skip);
					lock();
					inc(dword[rsi + 8]);
					mov(ptr[rdi + 0x18], rsi);
					test(rcx, rcx);
					jz(skip);
					mov(eax, 0xFFFFFFFF);
					lock();
					xadd(dword[rcx + 8], eax);
					cmp(eax, 1);
					jnz(skip);
					mov(rax, ptr[rcx]);
					call(qword[rax + 8]);
					L(skip);
					mov(rcx, rbp);
					mov(rax, Refresh);
					call(rax);
					mov(rax, Epilogue);
					jmp(rax);
				}
			};

			// Per CK runtime (1.10.162.0, 1.10.982.3, 1.11.137.0, 1.11.240.0), relative to Set3D:
			// start of the replace block, the call of the refresh function inside the shared tail and the epilogue.
			constexpr std::size_t Set3D_Block[] = { 0x162, 0x166, 0x166, 0x166 };
			constexpr std::size_t Set3D_Call[] = { 0x2F, 0x2E, 0x2E, 0x2E };
			constexpr std::size_t Set3D_Epilogue[] = { 0x42, 0x41, 0x41, 0x41 };

			FixLayerRefCount::FixLayerRefCount() : Common::Patch()
			{
				SetName("Fix layer reference count");
			}

			bool FixLayerRefCount::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* FixLayerRefCount::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool FixLayerRefCount::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> FixLayerRefCount::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool FixLayerRefCount::DoQuery() const noexcept(true)
			{
				auto ver = VersionLists::GetEditorVersion();
				return (ver != VersionLists::EDITOR_FALLOUT_C4_1_10_943_1) && (ver <= VersionLists::EDITOR_FALLOUT_C4_LAST);
			}

			bool FixLayerRefCount::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				// The same ID resolves to TESObjectREFR::Set3D in all four runtimes.
				const auto runtime = IDDB::GetSingleton()->GetRuntimeIndex();
				if (runtime >= SUPPORT_RUNTIMECOUNT)
					return false;

				const auto func = Relocation(EditorAPI::IDs::Functions::TESObjectREFR__Set3D).Address();
				const auto block = func + Set3D_Block[runtime];

				// mov rcx, [rdi+18h]; cmp rcx, rsi / call rel32 / mov rbx, [rsp+...]
				static constexpr std::uint8_t sig_block[] = { 0x48, 0x8B, 0x4F, 0x18, 0x48, 0x3B, 0xCE };
				static constexpr std::uint8_t sig_epilogue[] = { 0x48, 0x8B, 0x5C, 0x24 };

				if (std::memcmp((const void*)block, sig_block, sizeof(sig_block)) ||
					(*(const std::uint8_t*)(block + Set3D_Call[runtime]) != 0xE8) ||
					std::memcmp((const void*)(block + Set3D_Epilogue[runtime]), sig_epilogue, sizeof(sig_epilogue)))
					return false;

				const auto call = block + Set3D_Call[runtime];
				const auto refresh = call + 5 + *(const std::int32_t*)(call + 1);

				// Memory is leaked on purpose, the code lives as long as the process.
				auto hook = new ReplaceNodeHook(refresh, block + Set3D_Epilogue[runtime]);
				Relocation(block).WriteJump(hook->getCode());

				return true;
			}
		}
	}
}
