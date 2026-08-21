// Copyright © 2026 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.AddressLibrary.h>
#include <CKPE.Module.h>

namespace CKPE
{
    namespace Fallout4
    {
        namespace AddressVersions
        {
            inline constexpr Common::AddressLibrary::VersionID CK162 =
                MAKE_EXE_VERSION_EX(1, 10, 162, 0);

            inline constexpr Common::AddressLibrary::VersionID CK982 =
                MAKE_EXE_VERSION_EX(1, 10, 982, 3);

            inline constexpr Common::AddressLibrary::VersionID CK137 =
                MAKE_EXE_VERSION_EX(1, 11, 137, 0);

            inline constexpr Common::AddressLibrary::VersionID CK240 =
                MAKE_EXE_VERSION_EX(1, 11, 240, 0);
        }

        using AddressID = Common::AddressLibrary::AddressID;

        using VariantID = Common::AddressLibrary::VariantID;
    }
}