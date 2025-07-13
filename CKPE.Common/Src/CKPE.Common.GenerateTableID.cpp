// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <windows.h>
#include <mmsystem.h>
#include <Zydis/Zydis.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.GenerateTableID.h>
#include <CKPE.FileUtils.h>
#include <CKPE.Patterns.h>
#include <algorithm>
#include <execution>
#include <map>
#include <unordered_map>

namespace CKPE
{
	namespace Common
	{
		void GenerateTableID::Analize() noexcept(true)
		{
			if (!_mbase || !_entries)
				return;

			ScopeCriticalSection guard(_locker);

			_entries->clear();

			try
			{
				if (!_mbase->Is64())
					throw std::runtime_error("GenerateTableID::Analize() support only x64");

				// Enumerate all functions present in the x64 exception directory section
				const auto dir_exception = _mbase->GetPEDirectory(PEDirectory::e_exception);
				if (!dir_exception.GetAddress() || !dir_exception.GetSize()) 
					throw std::runtime_error("GenerateTableID::Analize() no exception directory");

				const auto functionEntries = dir_exception.GetPointer<RUNTIME_FUNCTION>();
				const auto functionEntryCount = dir_exception.GetSize() / sizeof(RUNTIME_FUNCTION);

				auto base = _mbase->GetBase();

				// Init threadsafe instruction decoder
				ZydisDecoder decoder;
				if (ZYDIS_SUCCESS(ZydisDecoderInit(&decoder, ZYDIS_MACHINE_MODE_LONG_64, ZYDIS_ADDRESS_WIDTH_64)))
				{
					std::unordered_map<std::uint32_t, std::uint32_t> unordered;

					for (auto Function = functionEntries; Function < (functionEntries + functionEntryCount); Function++)
					{
						if (!Function->BeginAddress)
							continue;

						unordered.insert({ Function->BeginAddress, Function->EndAddress - Function->BeginAddress });
					}
					
					std::map<std::uint32_t, std::uint32_t> ordered(unordered.begin(), unordered.end());
					_entries->reserve(unordered.size());

					_MESSAGE("\tTotal functions: %llu", unordered.size());

					for (auto& Function : ordered)
					{
						std::uint32_t size = Function.second;
						if (size > 64) size = 64;

						std::uint8_t buffer[128];
						ZeroMemory(buffer, 128);
						memcpy(buffer, (const void*)(base + Function.first), (Function.second > 127) ? 127 : Function.second);

						for (std::uint32_t offset = Function.first; offset < (Function.first + Function.second);)
						{
							const std::uintptr_t ip = base + offset;
							const std::uint8_t opcode = *(std::uint8_t*)ip;
							ZydisDecodedInstruction instruction;

							if (!ZYDIS_SUCCESS(ZydisDecoderDecodeBuffer(&decoder, (void*)ip, ZYDIS_MAX_INSTRUCTION_LENGTH, ip,
								&instruction)))
							{
								// Decode failed. Always increase byte offset by 1.
								offset += 1;

								if (offset >= (Function.first + size))
									break;

								continue;
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
									ZeroMemory(buffer + (offset - Function.first) + (instruction.length - 4), 4);
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
									buffer[(offset - Function.first) + 1] = 0x0;	
									break;
								case 0x80:
								case 0x83:
								case 0xC0:
								case 0xC2:
								case 0xC6:
								case 0xF6:
									ZeroMemory(buffer + (offset - Function.first) + (instruction.length - 5), 4);
									break;
								case 0x69:
								case 0x81:
								case 0xC7:
									ZeroMemory(buffer + (offset - Function.first) + (instruction.length - 8), 4);
									break;
								default:
									_MESSAGE("Dbg rel %llX %02X %u", ip, instruction.opcode, instruction.length);
									break;
								}								
							}

							offset += instruction.length;
							if (offset >= (Function.first + size))
							{
								size = (offset - Function.first);
								break;
							}
						}

						auto pattern = Patterns::CreateMask((std::uintptr_t)buffer, size);

						Entry entry;
						ZeroMemory(&entry, sizeof(Entry));

						entry.rva = Function.first;
						entry.real_size = Function.second;
						entry.mask_len = (std::uint32_t)pattern.length();
						memcpy(entry.mask, pattern.c_str(), entry.mask_len);
						entry.mask[entry.mask_len] = '\0';

						_entries->push_back(entry);
					}
				}
			}
			catch (const std::exception& e)
			{
				_ERROR(e.what());
			}
		}

		GenerateTableID::GenerateTableID() noexcept(true) :
			_entries(new std::vector<Entry>)
		{}

		GenerateTableID::GenerateTableID(const void* base) noexcept(true) :
			_entries(new std::vector<Entry>)
		{
			Rebase(base);
		}

		GenerateTableID::~GenerateTableID() noexcept(true)
		{
			Close();

			if (_entries)
			{
				delete _entries;
				_entries = nullptr;
			}
		}

		void GenerateTableID::Rebase(const void* base) noexcept(true)
		{
			Close();

			ScopeCriticalSection guard(_locker);

			if (!base)
				_mbase = new Module(GetModuleHandleA(nullptr));
			else
				_mbase = new Module(base);

			_mbase->LoadInfo();
			_version_file = FileUtils::GetFileVersion(_mbase->GetFileName());

			auto seg_rdata = _mbase->GetSegment(Segment::rdata);
			if (Patterns::FindByMask(seg_rdata.GetAddress(), seg_rdata.GetSize(),
				Patterns::ASCIIStringToMask("Skyrim.esm")))
				_game = MAKEFOURCC('S', 'S', 'E', '_');
			else if (Patterns::FindByMask(seg_rdata.GetAddress(), seg_rdata.GetSize(),
				Patterns::ASCIIStringToMask("Fallout4.esm")))
				_game = MAKEFOURCC('F', 'O', '4', '_');
			else if (Patterns::FindByMask(seg_rdata.GetAddress(), seg_rdata.GetSize(),
				Patterns::ASCIIStringToMask("Starfield.esm")))
				_game = MAKEFOURCC('S', 'F', '_', '_');
			else
				_game = MAKEFOURCC('U', 'N', 'K', 'W');

			Analize();
		}

		void GenerateTableID::Merging(GenerateTableID& tableid) noexcept(true)
		{
			auto dst = tableid._entries;
			auto src = _entries;

			std::size_t remove_func = 0;

			try
			{
				if (!dst || !src)
					return;

				if (tableid._game != _game)
					throw std::runtime_error("GenerateTableID::Merging game no matches");

				if (tableid._version_file == _version_file)
					throw std::runtime_error("GenerateTableID::Merging same game version");

				//auto merging_size = std::max(dst->size(), src->size());
				std::vector<Entry> new_entries, 
					merging_entries(dst->begin(), dst->end());

				auto current_it = src->begin();
				for (auto merging_it = merging_entries.begin(); merging_it != merging_entries.end(); merging_it++, current_it++)
				{
					if ((merging_it->mask_len == current_it->mask_len) &&
						!memcmp(merging_it->mask, current_it->mask, current_it->mask_len))
						// same
						continue;
					else
					{
						auto find_entry = std::find_if(merging_it, dst->end(),
							[&current_it](const Entry& entry) -> bool {
								if (entry.mask_len != current_it->mask_len)
									return false;
								else
									return !memcmp(entry.mask, current_it->mask, current_it->mask_len);
							});

						if (find_entry == dst->end())
							new_entries.push_back(*current_it);
						else
						{

						}
					}
				}


				////merging_entries.resize(merging_size);

				//auto dst_current = dst->begin();

				////tableid.Dump("dst_dump.txt");
				////Dump("src_dump.txt");

				//for (auto src_current = src->begin(); src_current != src->end(); src_current++)
				//{
				//	if (dst_current == dst->end())
				//		break;

				//	if (src_current->mask_len == dst_current->mask_len)
				//	{
				//		if (!memcmp(src_current->mask, dst_current->mask, dst_current->mask_len))
				//		{
				//			merging_entries[std::distance(dst->begin(), dst_current)] = *src_current;
				//			dst_current++;
				//		}
				//		else
				//			goto no_matches;
				//	}
				//	else
				//	{
				//	no_matches:
				//		// find function current

				

				//		if (find_entry == dst->end())
				//		{
				//			// New function
				//			new_entries.push_back(*src_current);
				//			// This function has not been found, I assume that it has been removed
				//			auto idx = std::distance(dst->begin(), dst_current);
				//			_MESSAGE("The function %lld was not found and appears to have been removed\n\t\"%s\"",
				//				idx, dst_current->mask);

				//			merging_entries[idx].rva = 0;
				//			dst_current++;
				//			remove_func++;
				//		}
				//		else
				//		{
				//			// The function was found, but not at this index, which means that the functions in between were removed			
				//			auto dist = std::distance(dst_current, find_entry);
				//			auto beg = std::distance(dst->begin(), dst_current);

				//			_MESSAGE("Starting from index %lld to %lld, the functions were presumably deleted", 
				//				beg, beg + (dist - 1));
				//			
				//			// Assume that some functions have been removed from the code section

				//			std::ptrdiff_t i = 0;
				//			while (i++ < dist)
				//			{
				//				if (dst_current == dst->end())
				//					break;

				//				merging_entries[std::distance(dst->begin(), dst_current)].rva = 0;
				//				dst_current++;
				//				remove_func++;
				//			}
				//		}
				//	}
				//}

				//merging_entries.erase(dst_current, merging_entries.end());
				//merging_entries.insert(merging_entries.end(), new_entries.begin(), new_entries.end());

				/*_MESSAGE("\tFind fuctions: %u\n\tRemoved fuctions: %u\n\tNew fuctions: %u",
					merging_entries.size(), remove_func, new_entries.size());*/
			}
			catch (const std::exception& e)
			{
				_ERROR(e.what());
			}
		}

		struct RELLIB_HEADER
		{
			std::uint32_t fourcc;
			std::uint32_t version;
			std::uint64_t version_file;
			std::uint32_t game;
			std::uint32_t count;
		};

		constexpr static std::uint32_t RELLIB_VERSION = 1;

		bool GenerateTableID::LoadFromStream(Stream& stm) noexcept(true)
		{
			if (!_entries)
				return false;

			try
			{
				RELLIB_HEADER header;
				stm.Read(&header, sizeof(RELLIB_HEADER));

				if (header.fourcc != MAKEFOURCC('C', 'K', 'R', 'L'))
					throw std::runtime_error("GenerateTableID::LoadFromStream no .relib file");

				if (header.version != RELLIB_VERSION)
					throw std::runtime_error("GenerateTableID::LoadFromStream invalid version file");

				_game = header.game;
				_version_file = header.version_file;

				_entries->resize(header.count);
				stm.Read(_entries->data(), (std::uint32_t)((std::size_t)header.count * sizeof(Entry)));
			}
			catch (const std::exception& e)
			{
				_ERROR(e.what());
				return false;
			}

			return true;
		}

		bool GenerateTableID::SaveToStream(Stream& stm) const noexcept(true)
		{
			if (!_entries || !_mbase)
				return false;

			RELLIB_HEADER header;
			ZeroMemory(&header, sizeof(RELLIB_HEADER));

			header.fourcc = MAKEFOURCC('C', 'K', 'R', 'L');
			header.version = RELLIB_VERSION;
			header.count = (std::uint32_t)_entries->size();
			header.version_file = _version_file;
			header.game = _game;

			stm.Write(&header, sizeof(RELLIB_HEADER));
			stm.Write(_entries->data(), (std::uint32_t)(_entries->size() * sizeof(Entry)));

			return true;
		}

		void GenerateTableID::Close()
		{
			ScopeCriticalSection guard(_locker);

			if (_mbase)
			{
				delete _mbase;
				_mbase = nullptr;
			}
		}

		void GenerateTableID::Dump(const std::string& fname) const noexcept(true)
		{
			if (!_entries)
				return;

			try
			{
				TextFileStream fstm(fname, FileStream::fmCreate);
				for (auto& entry : *_entries)
					fstm.WriteLine("0x%llX \"%s\"", entry.rva + 0x140000000, entry.mask);
			}
			catch (const std::exception&)
			{
			}
		}

		void GenerateTableID::Dump(const std::wstring& fname) const noexcept(true)
		{
			if (!_entries)
				return;

			try
			{
				TextFileStream fstm(fname, FileStream::fmCreate);
				for (auto& entry : *_entries)
					fstm.WriteLine("0x%llX \"%s\"", entry.rva + 0x140000000, entry.mask);
			}
			catch (const std::exception&)
			{
			}
		}
	}
}