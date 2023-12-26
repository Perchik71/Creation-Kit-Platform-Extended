// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		using jDialog = perchik71::jDialogs::jDialogA;

		class DialogManager
		{
		private:
			using DialogList = ConcurrencyMap<ULONG_PTR, jDialog*>;
			DialogList m_items;
		public:
			inline bool HasDialog(const LONG_PTR uid) const { return m_items.find(uid) != m_items.end(); }
			bool AddDialog(const String& json_file, const ULONG_PTR uid);
			bool AddDialogByCode(const String& json_code, const ULONG_PTR uid);
			jDialog* GetDialog(const ULONG_PTR uid);
			inline bool Empty() const { return !m_items.size(); }

			void LoadFromFilePackage(const char* fname);
			void PackToFilePackage(const char* fname, const char* dir);
	
			DialogManager() = default;
		};

		extern DialogManager* GlobalDialogManagerPtr;
	}
}