// Copyright © 2023-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Detours.h>
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

			bool CrashMergeForms::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() >= VersionLists::EDITOR_SKYRIM_SE_1_6_1130;
			}

			bool CrashMergeForms::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				if (db->GetVersion() != 1)
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				Detours::DetourJump(__CKPE_OFFSET(0), (std::uintptr_t)&sub);

				pointer_CrashMergeForms_data = __CKPE_OFFSET(1);
				pointer_CrashMergeForms_sub1 = __CKPE_OFFSET(2);
				pointer_CrashMergeForms_sub2 = __CKPE_OFFSET(3);
				pointer_CrashMergeForms_sub3 = __CKPE_OFFSET(4);
				pointer_CrashMergeForms_sub4 = __CKPE_OFFSET(5);
				pointer_CrashMergeForms_sub5 = __CKPE_OFFSET(6);
				pointer_CrashMergeForms_sub6 = __CKPE_OFFSET(7);
				pointer_CrashMergeForms_sub7 = __CKPE_OFFSET(8);
				pointer_CrashMergeForms_sub8 = __CKPE_OFFSET(9);
				pointer_CrashMergeForms_sub9 = __CKPE_OFFSET(10);
				pointer_CrashMergeForms_subA = __CKPE_OFFSET(11);

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