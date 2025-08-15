// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <windows.h>
#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			class LipGen : public Common::Patch
			{
				LipGen(const LipGen&) = delete;
				LipGen& operator=(const LipGen&) = delete;
			protected:
				virtual bool DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true);
				virtual bool DoQuery() const noexcept(true);
			public:
				LipGen();

				virtual bool HasOption() const noexcept(true);
				virtual const char* GetOptionName() const noexcept(true);
				virtual bool HasDependencies() const noexcept(true);
				virtual std::vector<std::string> GetDependencies() const noexcept(true);

				static void CreateLipGenProcess(std::int64_t a1) noexcept(true);
				static bool IsLipDataPresent(void* Thisptr) noexcept(true);
				static bool WriteLipData(void* Thisptr, const char* Path, std::int32_t Unkown1, bool Unknown2,
					bool Unknown3) noexcept(true);
				static std::int32_t IsWavDataPresent(const char* Path, std::int64_t a2, std::int64_t a3,
					std::int64_t a4) noexcept(true);
				static INT_PTR CALLBACK LipRecordDialogProc(HWND DialogHwnd, UINT Message, WPARAM wParam, 
					LPARAM lParam) noexcept(true);
			};
		}
	}
}