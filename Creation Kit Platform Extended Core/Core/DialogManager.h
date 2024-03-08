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
			virtual bool HasDialog(const LONG_PTR uid) const;
			virtual bool AddDialog(const String& json_file, const ULONG_PTR uid);
			virtual bool AddDialogByCode(const String& json_code, const ULONG_PTR uid);
			virtual jDialog* GetDialog(const ULONG_PTR uid);
			virtual bool Empty() const;

			virtual void LoadFromFilePackage(const char* fname);
			virtual void PackToFilePackage(const char* fname, const char* dir);
	
			DialogManager() = default;
		};

		extern DialogManager* GlobalDialogManagerPtr;
	}
}