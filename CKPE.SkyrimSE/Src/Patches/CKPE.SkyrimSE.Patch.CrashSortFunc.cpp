// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <algorithm>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/Forms/TESForm.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashSortFunc.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			template<typename T, bool Stable = false>
			static void CrashSortFuncSub(EditorAPI::BSTArray<T>& Array, int(*SortFunction)(const void*, const void*))
			{
				auto compare = [SortFunction](const T& A, const T& B)
					{
						return SortFunction(A, B) == -1;
					};

				if constexpr (Stable)
					std::stable_sort(&Array[0], &Array[Array.size()], compare);
				else
					std::sort(&Array[0], &Array[Array.size()], compare);
			}

			CrashSortFunc::CrashSortFunc() : Common::Patch()
			{
				SetName("Crash Sort Function");
			}

			bool CrashSortFunc::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* CrashSortFunc::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool CrashSortFunc::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CrashSortFunc::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CrashSortFunc::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool CrashSortFunc::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashSortFunc::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				//
				// Fix for crash (recursive sorting function stack overflow) when saving certain ESP files (i.e 3DNPC.esp)
				//
				Relocation(ID(52794)).WriteJump(&CrashSortFuncSub<EditorAPI::Forms::TESForm*>);

				return true;
			}
		}
	}
}