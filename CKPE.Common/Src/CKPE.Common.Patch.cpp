// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.Interface.h>
#include <CKPE.Common.Patch.h>

namespace CKPE
{
	namespace Common
	{
		void Patch::SetName(const char* name) noexcept(true)
		{
			if (_name)
				*_name = name;
		}

		void Patch::SetName(const std::string& name) noexcept(true)
		{
			if (_name)
				*_name = name;
		}

		Patch::Patch() noexcept(true) :
			_name(new std::string)
		{}

		Patch::~Patch() noexcept(true)
		{
			if (_name)
			{
				delete _name;
				_name = nullptr;
			}
		}

		bool Patch::Active(RelocatorDB::PatchDB* db) noexcept(true)
		{
			if (!db) return false;	
			_active = DoActive(db);
			return _active;
		}

		bool Patch::Query() const noexcept(true)
		{
			return DoQuery();
		}

		std::string Patch::GetName() const noexcept(true)
		{
			return _name ? *_name : "";
		}

		bool Patch::IsActive() const noexcept(true)
		{
			return _active;
		}
	}
}