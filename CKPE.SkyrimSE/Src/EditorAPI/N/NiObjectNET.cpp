#include <CKPE.Asserts.h>
#include <EditorAPI/N/NiObjectNET.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace EditorAPI
		{
			namespace NiAPI
			{

				NiExtraData* NiExtraData::Create(std::size_t a_size, std::uintptr_t a_vtbl)
				{
					auto memory = malloc(a_size);
					if (memory)
					{
						std::memset(memory, 0, a_size);
						reinterpret_cast<std::uintptr_t*>(memory)[0] = a_vtbl;
					}
					return static_cast<NiExtraData*>(memory);
				}

				NiExtraData* NiObjectNET::GetExtraData(const BSFixedString& a_key) const noexcept(true)
				{
					if (a_key.empty())
						return nullptr;

					std::int16_t bottom = 0;
					std::int16_t top = static_cast<std::int16_t>(_ExtraData.size() - 1);
					std::int16_t middle = 0;

					while (bottom <= top)
					{
						middle = (top + bottom) >> 1;

						std::ptrdiff_t compare = a_key.c_str() - _ExtraData[middle]->GetName();

						if (!compare)
							return _ExtraData[middle];
						else if (compare > 0)
							bottom = middle + 1;
						else
							top = middle - 1;
					}

					return nullptr;
				}

				NiExtraData* NiObjectNET::GetExtraDataAt(std::uint16_t a_extraDataIndex) const noexcept(true)
				{
					CKPE_ASSERT(a_extraDataIndex < _ExtraData.size());
					return _ExtraData[a_extraDataIndex];
				}

				std::uint16_t NiObjectNET::GetExtraDataSize() const noexcept(true)
				{
					return _ExtraData.size();
				}

				bool NiObjectNET::HasExtraData(const BSFixedString& a_key) const noexcept(true)
				{
					if (a_key.empty() || _ExtraData.empty())
						return false;

					for (auto& extra : _ExtraData)
						if (const auto extraData = extra; extraData && !_stricmp(extraData->GetName(), a_key))
							return true;

					return false;
				}
			}
		}
	}
} // 260BC00