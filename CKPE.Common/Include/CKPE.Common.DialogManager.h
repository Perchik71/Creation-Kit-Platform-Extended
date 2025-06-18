// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

#include <CKPE.Common.Common.h>
#include <jdialogs.h>
#include <concurrent_unordered_map.h>
#include <string>
#include <cstdint>

namespace CKPE
{
	namespace Common
	{
		using jDialog = perchik71::jDialogs::jDialogA;

		class DialogManager
		{
			using DialogList = concurrency::concurrent_unordered_map<ULONG_PTR, jDialog*>;
			DialogList* m_items{ nullptr };

			DialogManager(const DialogManager&) = delete;
			DialogManager& operator=(const DialogManager&) = delete;

			bool UnSafeAddDialog(const std::string& json_file, const std::uintptr_t uid) noexcept(true);
			bool UnSafeAddDialogByCode(const std::string& json_code, const std::uintptr_t uid) noexcept(true);
		public:
			constexpr DialogManager() noexcept(true) = default;
			DialogManager(const std::string& fname) noexcept(true);

			virtual bool HasDialog(const std::uintptr_t uid) const noexcept(true);
			virtual bool AddDialog(const std::string& json_file, const std::uintptr_t uid) noexcept(true);
			virtual bool AddDialogByCode(const std::string& json_code, const std::uintptr_t uid) noexcept(true);
			virtual jDialog* GetDialog(const std::uintptr_t uid) noexcept(true);
			virtual bool Empty() const noexcept(true);

			virtual void LoadFromFilePackage(const std::string& fname);
			virtual void PackToFilePackage(const std::string& fname, const std::string& dir);
	
			static DialogManager* GetSingleton() noexcept(true);
			static bool Initialize(const std::string& fname) noexcept(true);
			static bool Initialize(const std::wstring& fname) noexcept(true);
		};

		extern DialogManager* GlobalDialogManagerPtr;
	}
}