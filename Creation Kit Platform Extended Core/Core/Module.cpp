// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Relocator.h"
#include "RelocationDatabase.h"
#include "Module.h"

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		Module::Module(Engine* lpEngine) : _engine(lpEngine), _relocator(nullptr), 
			_relocationDatabaseItem(nullptr), _Active(false)
		{}

		Module::~Module()
		{
			Disable();
		}

		bool Module::Query(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion, const char* lpcstrPlatformRuntimeVersion) const
		{
			bool Ret = QueryFromPlatform(eEditorCurrentVersion, lpcstrPlatformRuntimeVersion);
			if (!Ret)
				_MESSAGE("Module \"%s\" rejected the query, most likely does not support the version of the editor or platform.", GetName());
			return Ret;
		}

		void Module::Enable(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			if (_Active || !lpRelocator || !lpRelocationDatabaseItem) return;

			if (HasOption())
			{
				String Section, Option;
				INIConfig::SplitOptionName(GetOptionName(), Section, Option);

				if (Option.empty() || !Option.length())
				{
					_ERROR("The patch indicated that it has an option, but there is no option name: \"%s\"", GetName());
					return;
				}

				if (Section.empty() || !Section.length())
					Section = "CreationKit";

				_MESSAGE("Reading an option in the configuration: %s:%s", Section.c_str(), Option.c_str());

				switch (INIConfig::GetOptionTypeByName(Option.c_str()))
				{
				case cotBool:
					if (!GlobalINIConfigPtr->ReadBool(Section.c_str(), Option.c_str(), false))
					{
						_MESSAGE("The option is disabled in the configuration: %s:%s", Section.c_str(), Option.c_str());
						return;
					}
					break;
				case cotInteger:
					if (!GlobalINIConfigPtr->ReadInt(Section.c_str(), Option.c_str(), false))
					{
						_MESSAGE("The option is disabled in the configuration: %s:%s", Section.c_str(), Option.c_str());
						return;
					}
					break;
				case cotUnsignedInteger:
					if (!GlobalINIConfigPtr->ReadUInt(Section.c_str(), Option.c_str(), false))
					{
						_MESSAGE("The option is disabled in the configuration: %s:%s", Section.c_str(), Option.c_str());
						return;
					}
					break;
				default:
					_ERROR("The patch indicated that an option was needed and even gave it a name,"
						   " but it has an unsupported type: \"%s\"", GetName());
					return;
				}
			}

			if (Activate(lpRelocator, lpRelocationDatabaseItem))
			{
				_Active = true;
				_relocator = lpRelocator;
				_relocationDatabaseItem = lpRelocationDatabaseItem;

				_MESSAGE("Module \"%s\" initializing success", GetName());
			}
			else
				_MESSAGE("Module \"%s\" initializing failed", GetName());
		}

		void Module::Disable()
		{
			if (_Active && HasCanRuntimeDisabled() && Shutdown(_relocator, _relocationDatabaseItem))
			{
				_Active = false;
				_relocator = nullptr;
				_relocationDatabaseItem = nullptr;

				_MESSAGE("Module \"%s\" released", GetName());
			}
		}
	}
}