// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Common.Interface.h>
#include <CKPE.Fallout4.Runner.h>
#include <CKPE.Common.RuntimeOptimization.h>

namespace CKPE
{
	namespace Fallout4
	{
		static Runner _srunner;

		bool Runner::InstallPatches() noexcept(true)
		{
			return true;
		}

		Runner* Runner::GetSingleton() noexcept(true)
		{
			return &_srunner;
		}

		bool Runner::Install() noexcept(true)
		{
			try
			{
				_MESSAGE("[FO4] Install patches...");
				if (InstallPatches())
				{
					// Important: this end operation
					Common::RuntimeOptimization ro;
					ro.Apply();
				}
				else
					return false;
			}
			catch (const std::exception& e)
			{
				_ERROR(e.what());
			}

			return true;
		}
	}
}