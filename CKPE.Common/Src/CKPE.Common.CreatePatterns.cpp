// Copyright © 2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <Zydis/Zydis.h>
#include <CKPE.Application.h>
#include <CKPE.StringUtils.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.CreatePatterns.h>

#include <memory>

namespace CKPE
{
	namespace Common
	{
		CKPE_COMMON_API std::string ZydisCreateMask(std::uintptr_t start_address, std::size_t size,
			Patterns::CreateFlag flag) noexcept(true)
		{
			auto s_text = Interface::GetSingleton()->GetApplication()->GetSegment(Segment::text);
			if ((s_text.GetAddress() > start_address) || (s_text.GetEndAddress() <= start_address))
				return "";

			// Init threadsafe instruction decoder
			ZydisDecoder decoder;
			if (ZYDIS_SUCCESS(ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64)))
			{
				auto bssize = size * 2;
				auto buffer = std::make_unique<std::uint8_t[]>(bssize);
				memcpy(buffer.get(), (void*)start_address, bssize);

				for (std::uint32_t offset = 0; offset < size;)
				{
					const std::uintptr_t ip = start_address + offset;
					const std::uint8_t opcode = *(std::uint8_t*)ip;
					ZydisDecodedInstruction instruction;

					if (!ZYDIS_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (void*)ip, ZYDIS_MAX_INSTRUCTION_LENGTH, ip,
						&instruction)))
					{
						// Decode failed. Always increase byte offset by 1.
						offset += 1;
						if (offset >= size) break;

						continue;
					}

					// ret
					if ((opcode == 0xC3) || (opcode == 0xCC))
					{
						size = (std::size_t)offset;
						break;
					}

					if (instruction.attributes & ZYDIS_ATTRIB_IS_RELATIVE)
					{
						// remove reloff

						switch (instruction.opcode)
						{
							// weird cmds begin
						case 0x00:
						case 0x01:
						case 0x02:
						case 0x03:
						case 0x0A:
						case 0x0B:
						case 0x1A:
						case 0x1B:
						case 0x32:
						case 0xD8:
						case 0xDD:
						case 0xDF:
							// weird cmds end
						case 0x10:
						case 0x11:
						case 0x15:
						case 0x23:
						case 0x28:
						case 0x29:
						case 0x2B:
						case 0x2C:
						case 0x2E:
						case 0x2F:
						case 0x33:
						case 0x38:
						case 0x39:
						case 0x3A:
						case 0x3B:
						case 0x44:
						case 0x45:
						case 0x54:
						case 0x57:
						case 0x58:
						case 0x59:
						case 0x5A:
						case 0x5C:
						case 0x5D:
						case 0x5E:
						case 0x5F:
						case 0x63:
						case 0x6F:
						case 0x82:
						case 0x84:
						case 0x85:
						case 0x86:
						case 0x87:
						case 0x88:
						case 0x89:
						case 0x8A:
						case 0x8B:
						case 0x8C:
						case 0x8D:
						case 0x8E:
						case 0x8F:
						case 0xB0:
						case 0xB1:
						case 0xB6:
						case 0xB7:
						case 0xBE:
						case 0xC1:
						case 0xE8:
						case 0xE9:
						case 0xDE:
						case 0xFF:
							memset(buffer.get() + offset + (instruction.length - 4), 0, 4);
							break;
						case 0x70:
						case 0x71:
						case 0x72:
						case 0x73:
						case 0x74:
						case 0x75:
						case 0x76:
						case 0x77:
						case 0x78:
						case 0x79:
						case 0x7A:
						case 0x7B:
						case 0x7C:
						case 0x7D:
						case 0x7E:
						case 0x7F:
						case 0xE0:
						case 0xE1:
						case 0xE2:
						case 0xE3:
						case 0xEB:
							buffer[(std::size_t)offset + 1] = 0x0;
							break;
						case 0x80:
						case 0x83:
						case 0xC0:
						case 0xC2:
						case 0xC6:
						case 0xF6:
							memset(buffer.get() + offset + (instruction.length - 5), 0, 4);
							break;
						case 0x69:
						case 0x81:
						case 0xC7:
							memset(buffer.get() + offset + (instruction.length - 8), 0, 4);
							break;
						default:
							//_MESSAGE("Dbg rel %llX %02X %u", ip, instruction.opcode, instruction.length);
							break;
						}
					}

					offset += instruction.length;
					if (offset >= size)
					{
						size = offset;
						break;
					}
				}

				if (size > 6)
				{
					auto smask = Patterns::CreateMask((std::uintptr_t)buffer.get(), size);
					auto sfind = Patterns::FindsByMask(s_text.GetAddress(), s_text.GetSize(), smask);
					smask = Patterns::CreateMask((std::uintptr_t)buffer.get(), size, flag);

					if (sfind.size() == 1)
						return smask;
					else if (sfind.size())
					{
						auto it = std::find(sfind.begin(), sfind.end(), start_address);
						if (it == sfind.end())
							return smask;
						else
							return StringUtils::FormatString("v%llu_s%llu_%s", 
								std::distance(sfind.begin(), it),
								sfind.size(), smask.c_str());
					}
				}
			}

			return "";
		}
	}
}