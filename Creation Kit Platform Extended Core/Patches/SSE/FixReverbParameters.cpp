// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Core/TypeInfo/ms_rtti.h"
#include "FixReverbParameters.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			FixReverbParametersPatch::FixReverbParametersPatch() : Module(GlobalEnginePtr)
			{}

			bool FixReverbParametersPatch::HasOption() const
			{
				return false;
			}

			bool FixReverbParametersPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixReverbParametersPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixReverbParametersPatch::GetName() const
			{
				return "Fix Reverb Parameters";
			}

			bool FixReverbParametersPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> FixReverbParametersPatch::GetDependencies() const
			{
				return {};
			}

			bool FixReverbParametersPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion <= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_438;
			}

			bool FixReverbParametersPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Bethesda pointed to a function that is not WndProc
					// According to RTTI readings, this object has one less function compared to CK 1.6.1130
					// and the last function is exactly what is needed.

					auto __BGSReverbParametersRTTI = Core::MSRTTI::FindAll("class BGSReverbParameters");
					
					for (auto it = __BGSReverbParametersRTTI.begin(); it != __BGSReverbParametersRTTI.end(); it++)
					{
						if ((*it)->VFunctionCount == 102)
						{
							ScopeRelocator vtable((*it)->VTableAddress, (*it)->VFunctionCount * 8);
							auto Class = (uint64_t*)(*it)->VTableAddress;
							Class[84] = Class[101];

							return true;
						}
					}
				}

				return false;
			}

			bool FixReverbParametersPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}
		}
	}
}