// Copyright © 2023-2024 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/gpl-3.0.html

#include "Core/Engine.h"
#include "Editor API/BSString.h"
#include "Editor API/FO4/TESNPC.h"
#include "Editor API/FO4/BSDynamicTriShape.h"
#include "Editor API/FO4/BSLightingShaderProperty.h"
#include "Editor API/FO4/BSResourceEntryDB.h"
#include "FixUsesReadyFaceGen.h"

namespace CreationKitPlatformExtended
{
	namespace Patches
	{
		namespace Fallout4
		{
			uintptr_t pointer_FixUsesReadyFaceGen_sub0 = 0;
			uintptr_t pointer_FixUsesReadyFaceGen_data = 0;
			uintptr_t pointer_FixUsesReadyFaceGen_sub1 = 0;
			uintptr_t pointer_FixUsesReadyFaceGen_sub2 = 0;
			uintptr_t pointer_FixUsesReadyFaceGen_sub3 = 0;
			uintptr_t pointer_FixUsesReadyFaceGen_sub4 = 0;
			uintptr_t pointer_FixUsesReadyFaceGen_sub5 = 0;
			uintptr_t pointer_FixUsesReadyFaceGen_sub6 = 0;
			uintptr_t pointer_FixUsesReadyFaceGen_sub7 = 0;

			using namespace EditorAPI;

			FixUsesReadyFaceGenPatch::FixUsesReadyFaceGenPatch() : Module(GlobalEnginePtr)
			{}

			bool FixUsesReadyFaceGenPatch::HasOption() const
			{
				return false;
			}

			bool FixUsesReadyFaceGenPatch::HasCanRuntimeDisabled() const
			{
				return false;
			}

			const char* FixUsesReadyFaceGenPatch::GetOptionName() const
			{
				return nullptr;
			}

			const char* FixUsesReadyFaceGenPatch::GetName() const
			{
				return "Fix Uses Ready FaceGen";
			}

			bool FixUsesReadyFaceGenPatch::HasDependencies() const
			{
				return true;
			}

			Array<String> FixUsesReadyFaceGenPatch::GetDependencies() const
			{
				return { "BSResource Texture DB RE" };
			}

			bool FixUsesReadyFaceGenPatch::QueryFromPlatform(EDITOR_EXECUTABLE_TYPE eEditorCurrentVersion,
				const char* lpcstrPlatformRuntimeVersion) const
			{
				return eEditorCurrentVersion >= EDITOR_EXECUTABLE_TYPE::EDITOR_FALLOUT_C4_1_10_982_3;
			}

			bool FixUsesReadyFaceGenPatch::Activate(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				if (lpRelocationDatabaseItem->Version() == 1)
				{
					// Replacing the original function
					lpRelocator->DetourJump(_RELDATA_RAV(0), (uintptr_t)&sub);
					// RTTI address BSLightingShaderProperty
					BSLightingShaderProperty::NIRTTI_BSLightingShaderProperty = (NiAPI::NiRTTI*)_RELDATA_ADDR(1);

					pointer_FixUsesReadyFaceGen_sub0 = _RELDATA_ADDR(2);
					pointer_FixUsesReadyFaceGen_data = _RELDATA_ADDR(3);
					pointer_FixUsesReadyFaceGen_sub1 = _RELDATA_ADDR(4);
					pointer_FixUsesReadyFaceGen_sub2 = _RELDATA_ADDR(5);
					pointer_FixUsesReadyFaceGen_sub4 = _RELDATA_ADDR(7);
					pointer_FixUsesReadyFaceGen_sub5 = _RELDATA_ADDR(8);
					pointer_FixUsesReadyFaceGen_sub6 = _RELDATA_ADDR(9);
					pointer_FixUsesReadyFaceGen_sub3 = _RELDATA_ADDR(10);
					pointer_FixUsesReadyFaceGen_sub7 = _RELDATA_ADDR(11);

					// The functions create a new texture (I linked from to NiTexture, but I'm not sure about the original)
					*(uintptr_t*)&NiTexture::CreateTexture = _RELDATA_ADDR(6);
					*(uintptr_t*)&NiTexture::CreateTexture2 = _RELDATA_ADDR(12);

					return true;
				}

				return false;
			}

			bool FixUsesReadyFaceGenPatch::Shutdown(const Relocator* lpRelocator,
				const RelocationDatabaseItem* lpRelocationDatabaseItem)
			{
				return false;
			}

			bool FixUsesReadyFaceGenPatch::sub(ActorData* Data, BSDynamicTriShape* TriShape)
			{
				constexpr static uint32_t DEFFUSE_TYPE = 0x10;
				constexpr static uint32_t NORMALS_TYPE = 0x11;
				constexpr static uint32_t SMOOTHS_TYPE = 0x12;

				static const char* NAMES_TYPE[3] =
				{
					"FaceCustomizationDiffuse",
					"FaceCustomizationNormals",
					"FaceCustomizationSmoothSpec"
				};

				static const char* FORMAT_PATH_FACEGEN[3] =
				{
					"Data\\Textures\\Actors\\Character\\FaceCustomization\\%s\\%08X_d.dds",
					"Data\\Textures\\Actors\\Character\\FaceCustomization\\%s\\%08X_msn.dds",
					"Data\\Textures\\Actors\\Character\\FaceCustomization\\%s\\%08X_s.dds"
				};

				if (!TriShape || !Data)
					return false;

				auto ShaderProperty = TriShape->GetShaderProperty();
				if (ShaderProperty.Empty() || ShaderProperty->GetRTTI() != BSLightingShaderProperty::NIRTTI_BSLightingShaderProperty)
					return false;

				auto LightingShaderProperty = ShaderProperty.GetPtr<BSLightingShaderProperty>();
				auto LightingShaderMaterial = LightingShaderProperty->GetShaderMaterial();
				if (!LightingShaderMaterial)
					return false;

				// Only need a face
				if (LightingShaderMaterial->GetType() == BSShaderMaterial::Face)
				{
					auto NPC = Data->Actor;
					auto ShaderFace = (BSLightingShaderMaterialFace*)LightingShaderMaterial;

					if (Data->Count > 0)
					{
						_CONSOLE("There should not be more than one material in the hierarchy with BSLightingShaderUniqueFeatures::FACE."
							" Actor: ""%s"" (%08X).", NPC->GetEditorID_NoVTable(), NPC->FormID);
						return false;
					}

					if (!Data->NoUseGenerator)
					{
						// WaitFor ???
						fastCall<void>(pointer_FixUsesReadyFaceGen_sub0, LightingShaderProperty);

						// Cleanup
						fastCall<void>(pointer_FixUsesReadyFaceGen_sub1, pointer_FixUsesReadyFaceGen_data, DEFFUSE_TYPE);
						fastCall<void>(pointer_FixUsesReadyFaceGen_sub1, pointer_FixUsesReadyFaceGen_data, NORMALS_TYPE);
						fastCall<void>(pointer_FixUsesReadyFaceGen_sub1, pointer_FixUsesReadyFaceGen_data, SMOOTHS_TYPE);

						// This is a buffer for a string, since Bethesda inserted a BSFixedString there
						char BufferFoName[0x80];

						if (Data->GenerationAgain)
						{
							for (auto i = DEFFUSE_TYPE; i <= SMOOTHS_TYPE; i++)
							{
								auto Index = i - DEFFUSE_TYPE;
								// Creating a new string with name for texture, BSFixedString okay...
								// Only diffuse texture have second parameter is 1
								auto NewTexture =
									NiTexture::CreateTexture(
										fastCall<BSEntryString*>(pointer_FixUsesReadyFaceGen_sub2, BufferFoName, NAMES_TYPE[Index]),
										i == DEFFUSE_TYPE, 0);
								// Idk, it is very difficult for my brain. 
								// Fuck. Is it really difficult to interact as with a class and not a structure?
								if (i == SMOOTHS_TYPE)
									fastCall<void>(pointer_FixUsesReadyFaceGen_sub4, ((uint8_t*)ShaderFace + 0x60), NewTexture);
								else
									fastCall<void>(pointer_FixUsesReadyFaceGen_sub4, ((uint8_t*)ShaderFace + (0x48 + (Index * 8))), NewTexture);
								// Remove memory by string
								fastCall<void>(pointer_FixUsesReadyFaceGen_sub5, BufferFoName);
								// Generation texture
								auto BinaryData = fastCall<void*>(pointer_FixUsesReadyFaceGen_sub6, pointer_FixUsesReadyFaceGen_data,
									i, i == DEFFUSE_TYPE, 1, 0, NAMES_TYPE[Index]);
								if (BinaryData)
								{
									// Nothing is clear, but quickly
									// Setting binary data into one of the texture
									if (i == SMOOTHS_TYPE)
										((NiTexture*)*((uint8_t**)((uintptr_t)ShaderFace + 0x60)))->SetBinaryData(BinaryData);
									else
										((NiTexture*)*((uint8_t**)((uintptr_t)ShaderFace + (0x48 + (Index * 8)))))->SetBinaryData(BinaryData);
								}
								else
									_CONSOLE("Failed to generate %s!", NAMES_TYPE[Index]);
							}
						}
						else
						{
							auto Texture = ShaderFace->GetDiffuseTexture();
							if (Texture)
							{
								auto BinaryData = Texture->GetBinaryData();
								fastCall<BSEntryString*>(pointer_FixUsesReadyFaceGen_sub7,
									pointer_FixUsesReadyFaceGen_data, DEFFUSE_TYPE, BinaryData, 1, 1);
							}

							Texture = ShaderFace->GetNormalTexture();
							if (Texture)
							{
								auto BinaryData = Texture->GetBinaryData();
								fastCall<BSEntryString*>(pointer_FixUsesReadyFaceGen_sub7,
									pointer_FixUsesReadyFaceGen_data, NORMALS_TYPE, BinaryData, 1, 0);
							}
							
							Texture = ShaderFace->GetSmoothnessSpecMaskTexture();
							if (Texture)
							{
								auto BinaryData = Texture->GetBinaryData();
								fastCall<BSEntryString*>(pointer_FixUsesReadyFaceGen_sub7,
									pointer_FixUsesReadyFaceGen_data, SMOOTHS_TYPE, BinaryData, 1, 0);
							}
						}
					}

					// Skip npc with protect
					if (!NPC->HasCharGenFacePreset())
					{
						auto PluginFile = NPC->GetBelongsToPlugin();
						if (PluginFile)
						{
							bool Done = true;
							// Getting an identifier for a name
							auto FormID = NPC->FormID;
							FormID = PluginFile->IsSmallMaster() ? FormID & 0xFFF : FormID & 0xFFFFFF;
							auto TextureSet = ShaderFace->GetTextureSet();

							for (uint32_t i = 0; i < 3; i++)
							{
								auto FileName = BSString::FormatString(FORMAT_PATH_FACEGEN[i], PluginFile->FileName.c_str(), FormID);

								if (!BSResource::BSTextureDB::Exists(FileName.c_str()))
								{
									_CONSOLE("[FACEGEN] Unable to find texture set '%s' on '%s' (%08X)", FileName.c_str(),
										NPC->GetEditorID_NoVTable(), NPC->FormID);
									Done = false;

									break;
								}

								TextureSet->SetTextureFilename((i == 2) ? 7 : i, FileName.c_str());
							}

							// Loading textures
							if (Done)
								ShaderFace->LoadTextureSet(TextureSet);
						}
					}

					// Apply
					fastCall<void>(pointer_FixUsesReadyFaceGen_sub3, pointer_FixUsesReadyFaceGen_data, DEFFUSE_TYPE);
					fastCall<void>(pointer_FixUsesReadyFaceGen_sub3, pointer_FixUsesReadyFaceGen_data, NORMALS_TYPE);
					fastCall<void>(pointer_FixUsesReadyFaceGen_sub3, pointer_FixUsesReadyFaceGen_data, SMOOTHS_TYPE);

					return true;
				}

				return false;
			}
		}
	}
}