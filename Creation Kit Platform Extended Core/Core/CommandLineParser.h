// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#pragma once

namespace CreationKitPlatformExtended
{
	namespace Core
	{
		class CommandLineParser
		{
		public:
			CommandLineParser();
			~CommandLineParser();
		
			inline String At(uint32_t nId) const { return _aArgs[nId]; }
			inline uint32_t Count() const { return (uint32_t)_aArgs.size(); }

			inline String operator[](uint32_t nId) const { return At(nId); }

			int IndexArgWithCommand() const;
			inline bool HasCommandRun() const { return IndexArgWithCommand() != -1; }
			String GetCommand() const;
		private:
			Array<String> _aArgs;
		};
	}
}