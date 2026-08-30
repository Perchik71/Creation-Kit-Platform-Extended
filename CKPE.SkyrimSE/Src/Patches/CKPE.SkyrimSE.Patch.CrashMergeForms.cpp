// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Utils.h>
#include <CKPE.CriticalSection.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.SkyrimSE.VersionLists.h>
#include <EditorAPI/Forms/TESForm.h>
#include <EditorAPI/TESFile.h>
#include <EditorAPI/BSTArray.h>
#include <Patches/CKPE.SkyrimSE.Patch.CrashMergeForms.h>

namespace CKPE
{
	namespace SkyrimSE
	{
		namespace Patch
		{
			std::uintptr_t pointer_CrashMergeForms_data = 0;
			std::uintptr_t pointer_CrashMergeForms_sub1 = 0;
			std::uintptr_t pointer_CrashMergeForms_sub2 = 0;
			std::uintptr_t pointer_CrashMergeForms_sub3 = 0;
			std::uintptr_t pointer_CrashMergeForms_sub4 = 0;
			std::uintptr_t pointer_CrashMergeForms_sub5 = 0;
			std::uintptr_t pointer_CrashMergeForms_sub6 = 0;
			std::uintptr_t pointer_CrashMergeForms_sub7 = 0;
			std::uintptr_t pointer_CrashMergeForms_sub8 = 0;
			std::uintptr_t pointer_CrashMergeForms_sub9 = 0;
			std::uintptr_t pointer_CrashMergeForms_subA = 0;
			CriticalSection lock_CrashMergeForms;

			CrashMergeForms::CrashMergeForms() : Common::Patch()
			{
				SetName("Crash Merge Forms");
			}

			bool CrashMergeForms::HasOption() const noexcept(true)
			{
				return true;
			}

			const char* CrashMergeForms::GetOptionName() const noexcept(true)
			{
				return "CreationKit:bOverlapsGenerateONAM";
			}

			bool CrashMergeForms::HasDependencies() const noexcept(true)
			{
				return false;
			}

			std::vector<std::string> CrashMergeForms::GetDependencies() const noexcept(true)
			{
				return {};
			}

			bool CrashMergeForms::SupportsAddressLibrary() const noexcept(true)
			{
				return true;
			}

			bool CrashMergeForms::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() <= VersionLists::EDITOR_SKYRIM_SE_LAST;
			}

			bool CrashMergeForms::DoActive([[maybe_unused]] Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				using namespace Common;

				Relocation(ID(539487)).WriteJump(&sub);

				pointer_CrashMergeForms_data = ID(191036).Address();
				pointer_CrashMergeForms_sub1 = ID(569310).Address();
				pointer_CrashMergeForms_sub2 = ID(65013).Address() ;
				pointer_CrashMergeForms_sub3 = ID(346842).Address();
				pointer_CrashMergeForms_sub4 = ID(218849).Address();
				pointer_CrashMergeForms_sub5 = ID(1420).Address();
				pointer_CrashMergeForms_sub6 = ID(283793).Address();
				pointer_CrashMergeForms_sub7 = ID(351541).Address();
				pointer_CrashMergeForms_sub8 = ID(569205).Address();
				pointer_CrashMergeForms_sub9 = ID(196467).Address();
				pointer_CrashMergeForms_subA = ID(363840).Address();

				return true;
			}

			void CrashMergeForms::sub(void* PluginFile) noexcept(true)
			{
				auto plugin = (EditorAPI::TESFile*)PluginFile;

				struct IteratorT
				{
					std::uint32_t* FormId;
					char pad[0x18];			// Unk
				};

				// Redoing the master files merge function.
				// In the original, it is allowed no more than 65536 ids to array [ONAM].

				// block for thread
				ScopeCriticalSection guard(lock_CrashMergeForms);
				{
					// Erasing the previous array form ids
					((void(__fastcall*)(void*))pointer_CrashMergeForms_sub1)(PluginFile);

					// I will assume that I am working with a temp plugin object, but I need to get parent object
					auto SomePluginFile = plugin->GetParentFile();
					if (!SomePluginFile)
						SomePluginFile = plugin;

					_CONSOLE("......[CKPE] Updating owned ID array (File: %s)", plugin->GetFileName().c_str());

					// I refuse from NiArray in favor of BSTArray (limited to 4 billion elements)
					EditorAPI::BSTArray<std::uint32_t, 128, 128> FormIds;

					// I have no idea, I think these are iterators
					//

					auto UnkClass1 = (uintptr_t*)pointer_CrashMergeForms_data;
					IteratorT Iterator;
					IteratorT IteratorEnd;
					memset(&Iterator, 0, sizeof(IteratorT));
					memset(&IteratorEnd, 0, sizeof(IteratorT));

					fast_call<void>(pointer_CrashMergeForms_sub2, *UnkClass1, &Iterator);
					fast_call<void>(pointer_CrashMergeForms_sub3, *UnkClass1, &IteratorEnd);

					SomePluginFile->CleanCountOwnedIds();
					// Go through to the end
					while (fast_call<bool>(pointer_CrashMergeForms_sub4, &Iterator, &IteratorEnd))
					{
						std::uint32_t FormId = 0;
						EditorAPI::Forms::TESForm* Form = nullptr;

						// Getting a form by its iterator, where exactly are its Id
						fast_call<void>(pointer_CrashMergeForms_sub5, *UnkClass1, &Iterator, &FormId, &Form);
						// Next
						fast_call<void>(pointer_CrashMergeForms_sub6, &Iterator);

						// Lots of checks
						if (FormId && Form &&
							fast_call<bool>(pointer_CrashMergeForms_sub7, Form, SomePluginFile) &&
							(fast_call<EditorAPI::TESFile*>(pointer_CrashMergeForms_sub8, Form, 0) != SomePluginFile))
						{
							// All conditions are original

							bool NeedAdd = false;
							auto FType = Form->Type;

							if ((FType >= EditorAPI::Forms::TESForm::ftReference) && (FType <= EditorAPI::Forms::TESForm::ftNavMesh))
							{
								if ((FType != EditorAPI::Forms::TESForm::ftCharacter) && (FType != EditorAPI::Forms::TESForm::ftWorldSpace))
								{
									if (FType >= EditorAPI::Forms::TESForm::ftLandspace)
										NeedAdd = true;
									else
										NeedAdd = !fast_call<bool>(pointer_CrashMergeForms_sub9, Form);
								}
							}

							// If the form needs to be added...
							if (NeedAdd)
							{
								// Added
								FormIds.push_back(FormId);
								// Inc count to temp object
								SomePluginFile->IncCountOwnedIds();
							}
						}
					}

					// Get count total
					size_t size = SomePluginFile->CountOwnedIds();
					_CONSOLE("......[CKPE] Total owned ID (%llu)", size);

					if (size)
					{
						// Copying the entire buffer to a new one
						size *= sizeof(std::uint32_t);
						auto data = fast_call<std::uint32_t*>(pointer_CrashMergeForms_subA, size);
						if (data)
						{
							memcpy(data, FormIds.data(), size);

							// Set new array
							plugin->SetArrayOwnedIds(data, SomePluginFile->CountOwnedIds());
							_CONSOLE("......[CKPE] The array has been successfully update", size);
						}
						else
							_CONSOLE("......[CKPE] Failure to update owned ID array, lack of memory.", size);

						FormIds.clear();
					}
				}
			}
		}
	}
}