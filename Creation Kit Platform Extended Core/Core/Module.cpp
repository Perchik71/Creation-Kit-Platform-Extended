// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Version/resource_version2.h"
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
			auto Name = GetName();
			if (!Ret && Name)
				_MESSAGE("Module \"%s\" rejected the query, most likely does not support the version of the editor or platform.", Name);
			return Ret;
		}

		void Module::Enable(const Relocator* lpRelocator, const RelocationDatabaseItem* lpRelocationDatabaseItem)
		{
			if (_Active || !lpRelocator || !lpRelocationDatabaseItem) return;

			if (HasDependencies())
			{
				auto PatchesManager = _engine->GetPatchesManager();
				if (PatchesManager && (PatchesManager->Count() > 0))
				{
					auto Dependencies = GetDependencies();
					for (auto it = Dependencies.begin(); it != Dependencies.end(); it++)
					{
						auto Depend = PatchesManager->GetByName(it->c_str());
						if (Depend.Empty())
						{
							_ERROR("The dependent module \"%s\" was not detected: \"%s\"", it->c_str(), GetName());
							return;
						}
						else
						{
							if (Depend->HasActive())
								continue;

							if (!Depend->Query(_engine->GetEditorVersion(), VER_FILE_VERSION_STR))
							{
								_ERROR("The dependent module \"%s\" has rejected its enabling, it may not support the current version of the editor or platform: \"%s\"", it->c_str(), GetName());
								PatchesManager->Remove(it->c_str());
								return;
							}

							Depend->Enable(lpRelocator, *Core::GlobalRelocationDatabasePtr->GetByName(it->c_str()));
							if (!Depend->HasActive())
							{
								_ERROR("the dependent module \"%s\" has not been enabled, this module will not be enabled: \"%s\"", it->c_str(), GetName());
								PatchesManager->Remove(it->c_str());
								return;
							}
						}
					}
				}
				else
				{
					_ERROR("The module indicated that it has a dependency, but did not specify which modules are needed, the module will not be enabled: \"%s\"", GetName());
					return;
				}
			}

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