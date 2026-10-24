// Copyright © 2026 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <EditorAPI/H/hkInfrastructure.h>

namespace CKPE::Fallout4::EditorAPI
{
	class BSAnimationDBData
	{
	public:
		/* 0x000 */ virtual ~BSAnimationDBData();

	private:
		/* 0x008 */ virtual void sub_008();
		/* 0x010 */ virtual void sub_010();
	};
	static_assert(sizeof(BSAnimationDBData) == 0x8);

	class BShkbHkxDB
	{
	public:
		class ProjectDBData;

		class hkxDBData : public hkLoader, public BSAnimationDBData
		{
		public:
			/* 0x060 */ [[nodiscard]] virtual ProjectDBData* AsProjectDBData() noexcept(true);

			/* 0x028 */ void* loadedData;
			/* 0x030 */ std::uint32_t fileSize;
			/* 0x034 */ std::byte unk034;
			/* 0x035 */ std::byte pad035[0x3];
		};
		static_assert(sizeof(hkxDBData) == 0x38);
		static_assert(offsetof(hkxDBData, loadedData) == 0x28);
		static_assert(offsetof(hkxDBData, fileSize) == 0x30);

		class ProjectDBData : public hkxDBData
		{
		public:
			/* 0x038 */ std::byte unk038[0x190];
		};
		static_assert(sizeof(ProjectDBData) == 0x1C8);
		static_assert(offsetof(ProjectDBData, unk038) == 0x38);

		class QueuedHandles;
		class StreamAdaptor;
	};
}
