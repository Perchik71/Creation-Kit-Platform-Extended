// Copyright © 2024-2025 aka perchik71. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#include <CKPE.Asserts.h>
#include <CKPE.Exception.h>
#include <CKPE.Zipper.h>
#include <CKPE.PathUtils.h>
#include <CKPE.StringUtils.h>
#include <CKPE.FileUtils.h>
#include <CKPE.Utils.h>
#include <CKPE.Detours.h>
#include <CKPE.Application.h>
#include <CKPE.Common.Interface.h>
#include <CKPE.Common.RTTI.h>
#include <CKPE.Fallout4.VersionLists.h>
#include <EditorAPI/BSResourceArchive2.h>
#include <Patches/CKPE.Fallout4.Patch.Console.h>
#include <Patches/CKPE.Fallout4.Patch.LoadShaders.h>

#define CKPE_GEN_ENCRYPT 0

namespace CKPE
{
	namespace Fallout4
	{
		namespace Patch
		{
			std::uintptr_t pointer_LoadShaders_sub = 0;
			static MemoryStream PackageShaders;
			static std::uint8_t KeyEncrypt = 0xDE;

			namespace OG_NG
			{
				class ResourceArchiveShader : public EditorAPI::BSResource::Archive2::OG_NG::ReaderStream
				{
				public:
					ResourceArchiveShader();

					virtual EditorAPI::BSResource::EResultError DoOpen() noexcept(true) override;
					virtual void DoClose() noexcept(true) override;
					virtual EditorAPI::BSResource::EResultError DoRead(void* Buffer, std::uint64_t Bytes,
						std::uint64_t& Read) const noexcept(true) override;
					virtual EditorAPI::BSResource::EResultError DoSeek(std::int64_t p, EditorAPI::BSResource::SeekMode m,
						std::uint64_t& np) const noexcept(true) override;
					virtual EditorAPI::BSResource::EResultError DoPrefetchAll(std::uint32_t) const noexcept(true) override;
					virtual EditorAPI::BSResource::EResultError DoCreateAsync(void* unk) const noexcept(true) override;

				};

				ResourceArchiveShader::ResourceArchiveShader()
				{
					IncRef();

					Console::LogWarning(Console::EDITOR, "Load shaders OG/NG");

					auto filename = PathUtils::GetApplicationPath() + VersionLists::GetExternalResourcePackageFileName();
					CKPE_ASSERT_MSG_FMT(PathUtils::FileExists(filename), "The resource file \"%s\" was not found",
						StringUtils::Utf16ToWinCP(filename).c_str());

					Console::LogWarning(Console::EDITOR, "Open resource file: \"%s\"", StringUtils::Utf16ToWinCP(filename).c_str());

					try
					{
						UnZipper zipper(filename);
						auto entries = zipper.GetEntries();

						if (!zipper.HasOpen())
							throw RuntimeError(L"Can't opened file \"{}\"", filename);

						std::string needfile;
						if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
							needfile = "Shaders011 - OG.fxp";
						else
							needfile = "Shaders011 - NG.fxp";

						for (std::size_t i = 0; i < zipper.GetEntries()->Count(); i++)
						{
							auto entry = zipper.GetEntries()->At(i);
							if (entry.Empty() || !entry->Get())
								throw std::runtime_error(zipper.LastErrorByString());

							if (entry->Get()->IsDir())
								continue;

							auto sName = entry->Get()->GetName();
							if (_stricmp(sName.c_str(), needfile.c_str()))
								continue;

							if (!entry->Get()->ReadToStream(PackageShaders))
								throw RuntimeError("File reading error!");

#if CKPE_GEN_ENCRYPT
							auto mem = PackageShaders.Data();
							for (std::uint64_t i = 0; i < PackageShaders.GetSize(); i++)
								mem[i] ^= KeyEncrypt;
							PackageShaders.SaveToFile(needfile);
#endif

							PackageShaders.SetPosition(0);
							break;
						}
					}
					catch (const std::exception& e)
					{
						CKPE_ASSERT_MSG_FMT(PathUtils::FileExists(filename), "Fatal error open resource file \"%s\"\n\n%s",
							StringUtils::Utf16ToWinCP(filename).c_str(), e.what());
					}
				}

				EditorAPI::BSResource::EResultError ResourceArchiveShader::DoOpen() noexcept(true)
				{
					return EditorAPI::BSResource::EResultError::kNone;
				}

				void ResourceArchiveShader::DoClose() noexcept(true)
				{}

				EditorAPI::BSResource::EResultError ResourceArchiveShader::DoRead(void* Buffer, std::uint64_t Bytes,
					std::uint64_t& Read) const noexcept(true)
				{
					Read = PackageShaders.Read(Buffer, (std::uint32_t)Bytes);
					for (std::uint32_t i = 0; i < Read; i++)
						((std::uint8_t*)Buffer)[i] ^= KeyEncrypt;
					return EditorAPI::BSResource::EResultError::kNone;
				}

				EditorAPI::BSResource::EResultError ResourceArchiveShader::DoSeek(std::int64_t p, EditorAPI::BSResource::SeekMode m,
					std::uint64_t& np) const noexcept(true)
				{
					np = PackageShaders.Offset(p, (CKPE::Stream::OffsetStream)m);
					return EditorAPI::BSResource::EResultError::kNone;
				}

				EditorAPI::BSResource::EResultError ResourceArchiveShader::DoPrefetchAll(std::uint32_t) const noexcept(true)
				{
					return EditorAPI::BSResource::EResultError::kUnsupported;
				}

				EditorAPI::BSResource::EResultError ResourceArchiveShader::DoCreateAsync(void* unk) const noexcept(true)
				{
					return EditorAPI::BSResource::EResultError::kUnsupported;
				}
			}

			namespace AE
			{
				class ResourceArchiveShader : public EditorAPI::BSResource::Archive2::AE::ReaderStream
				{
				public:
					ResourceArchiveShader();

					virtual EditorAPI::BSResource::EResultError DoOpen() noexcept(true) override;
					virtual void DoClose() noexcept(true) override;
					virtual EditorAPI::BSResource::EResultError DoRead(void* Buffer, std::uint64_t Bytes,
						std::uint64_t& Read) const noexcept(true) override;
					virtual EditorAPI::BSResource::EResultError DoSeek(std::int64_t p, EditorAPI::BSResource::SeekMode m,
						std::uint64_t& np) const noexcept(true) override;
					virtual EditorAPI::BSResource::EResultError DoPrefetchAll(std::uint32_t) const noexcept(true) override;
					virtual EditorAPI::BSResource::EResultError DoCreateAsync(void* unk) const noexcept(true) override;

				};

				ResourceArchiveShader::ResourceArchiveShader()
				{
					IncRef();

					Console::LogWarning(Console::EDITOR, "Load shaders AE");

					auto filename = PathUtils::GetApplicationPath() + VersionLists::GetExternalResourcePackageFileName();
					CKPE_ASSERT_MSG_FMT(PathUtils::FileExists(filename), "The resource file \"%s\" was not found",
						StringUtils::Utf16ToWinCP(filename).c_str());

					Console::LogWarning(Console::EDITOR, "Open resource file: \"%s\"", StringUtils::Utf16ToWinCP(filename).c_str());

					try
					{
						UnZipper zipper(filename);
						auto entries = zipper.GetEntries();

						if (!zipper.HasOpen())
							throw RuntimeError(L"Can't opened file \"{}\"", filename);

						std::string needfile;
						if (VersionLists::GetEditorVersion() == VersionLists::EDITOR_FALLOUT_C4_1_10_162_0)
							needfile = "Shaders011 - OG.fxp";
						else
							needfile = "Shaders011 - NG.fxp";

						for (std::size_t i = 0; i < zipper.GetEntries()->Count(); i++)
						{
							auto entry = zipper.GetEntries()->At(i);
							if (entry.Empty() || !entry->Get())
								throw std::runtime_error(zipper.LastErrorByString());

							if (entry->Get()->IsDir())
								continue;

							auto sName = entry->Get()->GetName();
							if (_stricmp(sName.c_str(), needfile.c_str()))
								continue;

							if (!entry->Get()->ReadToStream(PackageShaders))
								throw RuntimeError("File reading error!");

#if CKPE_GEN_ENCRYPT
							auto mem = PackageShaders.Data();
							for (std::uint64_t i = 0; i < PackageShaders.GetSize(); i++)
								mem[i] ^= KeyEncrypt;
							PackageShaders.SaveToFile(needfile);
#endif

							PackageShaders.SetPosition(0);
							break;
						}
					}
					catch (const std::exception& e)
					{
						CKPE_ASSERT_MSG_FMT(PathUtils::FileExists(filename), "Fatal error open resource file \"%s\"\n\n%s",
							StringUtils::Utf16ToWinCP(filename).c_str(), e.what());
					}
				}

				EditorAPI::BSResource::EResultError ResourceArchiveShader::DoOpen() noexcept(true)
				{
					return EditorAPI::BSResource::EResultError::kNone;
				}

				void ResourceArchiveShader::DoClose() noexcept(true)
				{}

				EditorAPI::BSResource::EResultError ResourceArchiveShader::DoRead(void* Buffer, std::uint64_t Bytes,
					std::uint64_t& Read) const noexcept(true)
				{
					Read = PackageShaders.Read(Buffer, (std::uint32_t)Bytes);
					for (std::uint32_t i = 0; i < Read; i++)
						((std::uint8_t*)Buffer)[i] ^= KeyEncrypt;
					return EditorAPI::BSResource::EResultError::kNone;
				}

				EditorAPI::BSResource::EResultError ResourceArchiveShader::DoSeek(std::int64_t p, EditorAPI::BSResource::SeekMode m,
					std::uint64_t& np) const noexcept(true)
				{
					np = PackageShaders.Offset(p, (CKPE::Stream::OffsetStream)m);
					return EditorAPI::BSResource::EResultError::kNone;
				}

				EditorAPI::BSResource::EResultError ResourceArchiveShader::DoPrefetchAll(std::uint32_t) const noexcept(true)
				{
					return EditorAPI::BSResource::EResultError::kUnsupported;
				}

				EditorAPI::BSResource::EResultError ResourceArchiveShader::DoCreateAsync(void* unk) const noexcept(true)
				{
					return EditorAPI::BSResource::EResultError::kUnsupported;
				}
			}

			LoadShaders::LoadShaders() : Common::Patch()
			{
				SetName("Load Shaders");
			}

			bool LoadShaders::HasOption() const noexcept(true)
			{
				return false;
			}

			const char* LoadShaders::GetOptionName() const noexcept(true)
			{
				return nullptr;
			}

			bool LoadShaders::HasDependencies() const noexcept(true)
			{
				return true;
			}

			std::vector<std::string> LoadShaders::GetDependencies() const noexcept(true)
			{
				return { "Console" };
			}

			bool LoadShaders::DoQuery() const noexcept(true)
			{
				return VersionLists::GetEditorVersion() != VersionLists::EDITOR_FALLOUT_C4_1_10_943_1;
			}

			bool LoadShaders::DoActive(Common::RelocatorDB::PatchDB* db) noexcept(true)
			{
				auto verPatch = db->GetVersion();
				if ((verPatch != 1) && (verPatch != 2))
					return false;

				auto interface = CKPE::Common::Interface::GetSingleton();
				auto base = interface->GetApplication()->GetBase();

				if (VersionLists::GetEditorVersion() < VersionLists::EDITOR_FALLOUT_C4_1_11_137_0)
				{
					EditorAPI::BSResource::Archive2::OG_NG::ReaderStream::RTTI =
						Common::RTTI::GetSingleton()->Find("class BSResource::Archive2::ReaderStream")->VTableAddress;

					Detours::DetourCall(__CKPE_OFFSET(0) + ((verPatch == 1) ? 0x88 : 0x8C), (std::uintptr_t)&CreateStream_OG_NG);
					pointer_LoadShaders_sub = __CKPE_OFFSET(1);
				}
				else
				{
					EditorAPI::BSResource::Archive2::AE::ReaderStream::RTTI =
						Common::RTTI::GetSingleton()->Find("class BSResource::Archive2::ReaderStream")->VTableAddress;

					Detours::DetourCall(__CKPE_OFFSET(0) + 0x8C, (std::uintptr_t)&CreateStream_AE);
					pointer_LoadShaders_sub = __CKPE_OFFSET(1);
				}

				return true;
			}

			EditorAPI::OG_NG::BSIStream* LoadShaders::CreateStream_OG_NG(EditorAPI::OG_NG::BSIStream* Stream, const char* FileName,
				std::uint32_t unk1, bool unk2) noexcept(true)
			{
				auto streamShaders = fast_call<EditorAPI::OG_NG::BSIStream*>(pointer_LoadShaders_sub, Stream, FileName, unk1, unk2);
				streamShaders->NewStream(new OG_NG::ResourceArchiveShader());
				return streamShaders;
			}

			EditorAPI::AE::BSIStream* LoadShaders::CreateStream_AE(EditorAPI::AE::BSIStream* Stream, const char* FileName,
				std::uint32_t unk1, bool unk2) noexcept(true)
			{
				auto streamShaders = fast_call<EditorAPI::AE::BSIStream*>(pointer_LoadShaders_sub, Stream, FileName, unk1, unk2);
				streamShaders->NewStream(new AE::ResourceArchiveShader());
				return streamShaders;
			}
		}
	}
}