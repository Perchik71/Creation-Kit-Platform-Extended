// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "CrashMergeForms.h"
#include "Editor API/BSTArray.h"
#include "Editor API/SSE/TESForm.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace SkyrimSpectialEdition
		{
			using namespace EditorAPI;
			using namespace EditorAPI::SkyrimSpectialEdition;

			uintptr_t pointer_CrashMergeForms_data = 0;
			uintptr_t pointer_CrashMergeForms_sub1 = 0;
			uintptr_t pointer_CrashMergeForms_sub2 = 0;
			uintptr_t pointer_CrashMergeForms_sub3 = 0;
			uintptr_t pointer_CrashMergeForms_sub4 = 0;
			uintptr_t pointer_CrashMergeForms_sub5 = 0;
			uintptr_t pointer_CrashMergeForms_sub6 = 0;
			uintptr_t pointer_CrashMergeForms_sub7 = 0;
			uintptr_t pointer_CrashMergeForms_sub8 = 0;
			uintptr_t pointer_CrashMergeForms_sub9 = 0;
			uintptr_t pointer_CrashMergeForms_subA = 0;

			CrashMergeFormsPatch::CrashMergeFormsPatch() : Module(GlobalEnginePtr)
			{}

			bool CrashMergeFormsPatch::HasOption() const
			{
				return true;
			}

			bool CrashMergeFormsPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* CrashMergeFormsPatch::GetOptionName() const
			{
				return "CreationKit:bOverlapsGenerateONAM";
			}

			const char* CrashMergeFormsPatch::GetName() const
			{
				return "Crash Merge Forms";
			}

			bool CrashMergeFormsPatch::HasDependencies() const
			{
				return false;
			}

			Array<String> CrashMergeFormsPatch::GetDependencies() const
			{
				return {};
			}

			bool CrashMergeFormsPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_SKYRIM_SE_1_6_1130;
			}

			bool CrashMergeFormsPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					lpRelocator->DetourJump(lpRelocationDatabaseItem->At(0), (uintptr_t)&sub);
					
					pointer_CrashMergeForms_data = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(1));
					pointer_CrashMergeForms_sub1 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(2));
					pointer_CrashMergeForms_sub2 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(3));
					pointer_CrashMergeForms_sub3 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(4));
					pointer_CrashMergeForms_sub4 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(5));
					pointer_CrashMergeForms_sub5 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(6));
					pointer_CrashMergeForms_sub6 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(7));
					pointer_CrashMergeForms_sub7 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(8));
					pointer_CrashMergeForms_sub8 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(9));
					pointer_CrashMergeForms_sub9 = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(10));
					pointer_CrashMergeForms_subA = lpRelocator->Rav2Off(lpRelocationDatabaseItem->At(11));

					return true;
				}

				return false;
			}

			bool CrashMergeFormsPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			std::recursive_mutex lock;

			void CrashMergeFormsPatch::sub(TESFile* PluginFile)
			{
				struct IteratorT
				{
					uint32_t* FormId;
					char pad[0x18];		// Unk
				};

				// Redoing the master files merge function.
				// In the original, it is allowed no more than 65536 ids to array [ONAM].

				// block for thread
				std::lock_guard locker(lock);
				{
					// Erasing the previous array form ids
					fastCall<void>(pointer_CrashMergeForms_sub1, PluginFile);

					// I will assume that I am working with a temp plugin object, but I need to get parent object
					TESFile* SomePluginFile = PluginFile->GetParentFile();
					if (!SomePluginFile)
						SomePluginFile = PluginFile;

					_CONSOLE("......[CKPE] Updating owned ID array (File: %s)", PluginFile->GetFileName().c_str());

					// I refuse from NiArray in favor of BSTArray (limited to 4 billion elements)
					BSTArray<uint32_t, 128, 128> FormIds;

					// I have no idea, I think these are iterators
					//

					auto UnkClass1 = (uintptr_t*)pointer_CrashMergeForms_data;
					IteratorT Iterator;
					IteratorT IteratorEnd;
					memset(&Iterator, 0, sizeof(IteratorT));
					memset(&IteratorEnd, 0, sizeof(IteratorT));

					fastCall<void>(pointer_CrashMergeForms_sub2, *UnkClass1, &Iterator);
					fastCall<void>(pointer_CrashMergeForms_sub3, *UnkClass1, &IteratorEnd);

					SomePluginFile->CleanCountOwnedIds();
					// Go through to the end
					while (fastCall<bool>(pointer_CrashMergeForms_sub4, &Iterator, &IteratorEnd))
					{
						uint32_t FormId = 0;
						TESForm* Form = nullptr;

						// Getting a form by its iterator, where exactly are its Id
						fastCall<void>(pointer_CrashMergeForms_sub5, *UnkClass1, &Iterator, &FormId, &Form);
						// Next
						fastCall<void>(pointer_CrashMergeForms_sub6, &Iterator);

						// Lots of checks
						if (FormId && Form &&
							fastCall<bool>(pointer_CrashMergeForms_sub7, Form, SomePluginFile) &&
							(fastCall<TESFile*>(pointer_CrashMergeForms_sub8, Form, 0) != SomePluginFile))
						{
							// All conditions are original

							bool NeedAdd= false;
							auto FType = Form->Type;

							if ((FType >= TESForm::ftReference) && (FType <= TESForm::ftNavMesh))
							{
								if ((FType != TESForm::ftCharacter) && (FType != TESForm::ftWorldSpace))
								{
									if (FType >= TESForm::ftLandspace)
										NeedAdd = true;
									else
										NeedAdd = !fastCall<bool>(pointer_CrashMergeForms_sub9, Form);
								}
							}

							// If the form needs to be added...
							if (NeedAdd)
							{
								// Added
								FormIds.Push(FormId);
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
						size *= sizeof(uint32_t);
						auto data = fastCall<uint32_t*>(pointer_CrashMergeForms_subA, size);
						if (data)
						{
							memcpy(data, FormIds.QBuffer(), size);
	
							// Set new array
							PluginFile->SetArrayOwnedIds(data, SomePluginFile->CountOwnedIds());
							_CONSOLE("......[CKPE] The array has been successfully update", size);
						}
						else
							_CONSOLE("......[CKPE] Failure to update owned ID array, lack of memory.", size);

						FormIds.Clear();
					}
				}
			}
		}
	}
}