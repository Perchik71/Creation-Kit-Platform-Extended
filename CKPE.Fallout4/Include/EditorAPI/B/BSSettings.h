// Copyright © 2023-2025 aka CKPE team. All rights reserved.
// Contacts: <email:timencevaleksej@gmail.com>
// License: https://www.gnu.org/licenses/lgpl-3.0.html

#pragma once

#include <CKPE.Common.Relocation.h>
#include <EditorAPI/IDs.h>
#include <EditorAPI/B/BSString.h>
#include <EditorAPI/B/BSFixedString.h>
#include <EditorAPI/B/BSTList.h>
#include <EditorAPI/B/BSTBTree.h>

namespace CKPE
{
	namespace Fallout4
	{
		namespace EditorAPI
		{
			// Bethesda uses SettingT. 
			// This is a template class with type binding, I will not use it, as it greatly inflates the code.
			class Setting
			{
			public:
				enum Types : std::uint8_t
				{
					stBool = 0x0,			// 'b' size 1
					stChar = 0x1,			// 'c' size 1
					stUnsignedChar = 0x2,	// 'h' size 1
					stSignedInt = 0x3,		// 'i' size 4
					stUnsignedInt = 0x4,	// 'u' size 4
					stFloat = 0x5,			// 'f' size 4
					stString = 0x6,			// 'S'/'s' size indet.
					stRGB = 0x7,			// 'r' size 4, alpha byte set to 255
					stRGBA = 0x8,			// 'a' size 4
					stMAX = 0x9,
					stError = stMAX
				};
				// union for various value types
				union Value
				{
					bool			b;
					char			c;
					std::uint8_t	h;
					std::int32_t	i;
					std::uint32_t	u;
					float			f;
					char* s;
					struct Color {
						// alpha is least sig. std::uint8_t, *opposite* standard windows order
						std::uint8_t alpha;
						std::uint8_t blue;
						std::uint8_t green;
						std::uint8_t red;
					} rgba;
				};

				Setting(const char* name, const Value& value);
				Setting(const Setting& s);
				virtual ~Setting();
				// 1.11.246.0: vtable RVA 0x2F9F068, 2 virtual functions (the SettingT<...> vtables share this body, so it is never true in the CK)
				[[nodiscard]] virtual bool IsPrefSetting() const noexcept(true) { return false; } // 01 - RVA 0x25CCB70

				[[nodiscard]] Types GetValueType() const;
				[[nodiscard]] inline bool CheckValidValueType(Types t) const { return GetValueType() == t; }
				bool SetBool(bool value);
				bool SetChar(char value);
				bool SetUnsignedChar(std::uint8_t value);
				bool SetSignedInt(std::int32_t value);
				bool SetUnsignedInt(std::uint32_t value);
				bool SetFloat(float value);
				bool SetString(const char* value);
				inline bool SetString(const BSString& value) { return SetString(value.c_str()); }
				inline bool SetString(const std::string& value) { return SetString(value.c_str()); }
				bool SetRGB(std::uint8_t r, std::uint8_t g, std::uint8_t b);
				bool SetRGBA(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);

				[[nodiscard]] inline const char* GetName() const { return _Name; }
				[[nodiscard]] inline bool GetBool() const { return _Value.b; }
				[[nodiscard]] inline char GetChar() const { return _Value.c; }
				[[nodiscard]] inline std::uint8_t GetUnsignedChar() const { return _Value.h; }
				[[nodiscard]] inline std::int32_t GetSignedInt() const { return _Value.i; }
				[[nodiscard]] inline std::uint32_t GetUnsignedInt() const { return _Value.u; }
				[[nodiscard]] inline float GetFloat() const { return _Value.f; }
				[[nodiscard]] inline const char* GetString() const { return _Value.s; }
				[[nodiscard]] inline Value::Color GetRGB() const { return _Value.rgba; }
				[[nodiscard]] inline Value::Color GetRGBA() const { return _Value.rgba; }

				CKPE_READ_PROPERTY(GetName) const char* Name;
				CKPE_READ_PROPERTY(GetValueType) Types ValueType;

				void* operator new(std::size_t sz);
				void* operator new[](std::size_t sz);
				void operator delete(void* ptr) noexcept;

				static Setting* CreateInstance(const char* name, const Value& value);
				inline void DeleteInstance() { delete this; }
			private:
				/* 00 vtable */
				/* 08 */ Value _Value;
				/* 10 */ char* _Name;
				static void CheckLayout() noexcept(true)
				{
					static_assert(offsetof(Setting, _Value) == 0x8);
					static_assert(offsetof(Setting, _Name) == 0x10);
				}
			};
			static_assert(sizeof(Setting) == 0x18);

			// 1.11.246.0: SettingCollection<Setting> vtable RVA 0x2F9F088, 11 virtual functions. CommonLibF4 has no
			// equivalent of the DumpSettings virtual (03): the Creation Kit inserts it between Remove and WriteSetting.
			template <class T>
			class SettingCollection
			{
				/* 008 */ char filename[260];
				/* 10C */ std::uint8_t pad10C[4];
				/* 110 */ void* handle;
				static void CheckLayout() noexcept(true)
				{
					static_assert(offsetof(SettingCollection, filename) == 0x8);
					static_assert(offsetof(SettingCollection, handle) == 0x110);
				}
			public:
				[[nodiscard]] const char* GetFileName() const noexcept(true) { return filename; }
				[[nodiscard]] void* GetHandle() const noexcept(true) { return handle; }

				virtual ~SettingCollection();

				virtual void Add(T* setting) = 0;									// 01
				virtual void Remove(T* setting) = 0;								// 02
				// Appends every setting to the report array (a "<title>" entry first, then one dump per setting and a
				// "Total ...: %u" line for the INI/Game collections) and returns the number of settings.
				virtual std::uint32_t DumpSettings(void* Report) = 0;				// 03
				virtual bool WriteSetting(T* setting) = 0;							// 04
				virtual bool ReadSetting(T* setting) = 0;							// 05
				virtual bool Open([[maybe_unused]] bool write) { return false; }	// 06
				virtual bool Close() { return true; }                               // 07
				virtual bool ReadSettingsFromProfile() { return false; }            // 08
				virtual bool WriteSettings() { return handle != nullptr; }          // 09
				virtual bool ReadSettings() { return handle != nullptr; }           // 0A
			};
			static_assert(sizeof(SettingCollection<int>) == 0x118);

			// 1.11.246.0: vtable RVA 0x2F9F130
			template <class T>
			class SettingCollectionList : public SettingCollection<T>
			{
			public:
				BSSimpleList<T> settings;		// 118

				template <class Visitor>
				void ForEach(Visitor&& visitor) const
				{
					for (auto it = settings.Begin(); !it.End(); ++it)
						if (auto setting = it.Get()) visitor(setting);
				}

				[[nodiscard]] T* GetSetting(const char* name) const noexcept(true)
				{
					T* result = nullptr;
					ForEach([&](T* setting)
					{
						if (!result && !_stricmp(setting->GetName(), name))
							result = setting;
					});
					return result;
				}
			};
			static_assert(sizeof(SettingCollectionList<int>) == 0x128);

			// 1.11.246.0: size 0x128, vtable RVA 0x2F9F1A8. Open() makes the collection its own handle, WriteSetting/
			// ReadSetting go through the Windows profile (INI) API.
			class INISettingCollection : public SettingCollectionList<Setting>
			{
			public:
				[[nodiscard]] static INISettingCollection* GetSingleton() noexcept(true)
				{
					static Common::Relocation<INISettingCollection**> singleton{ IDs::Setting::INISettingCollection::Singleton };
					return *singleton.Get();
				}
			};
			static_assert(sizeof(INISettingCollection) == 0x128);
			static_assert(offsetof(INISettingCollection, settings) == 0x118);

			// 1.11.246.0: size 0x128, vtable RVA 0x2FA6AF0 (CreationKitPrefs.ini)
			class INIPrefSettingCollection : public INISettingCollection
			{
			public:
				[[nodiscard]] static INIPrefSettingCollection* GetSingleton() noexcept(true)
				{
					static Common::Relocation<INIPrefSettingCollection**> singleton{ IDs::Setting::INIPrefSettingCollection::Singleton };
					return *singleton.Get();
				}
			};
			static_assert(sizeof(INIPrefSettingCollection) == 0x128);

			// 1.11.246.0: vtable RVA 0x2FA8258. The settings are kept in a B-tree keyed by the setting name.
			template <class T>
			class SettingCollectionMap : public SettingCollection<T>
			{
			public:
				struct Entry
				{
					/* 00 */ BSFixedString key;
					/* 08 */ T* value;
				};
				struct Node
				{
					/* 00 */ Entry entries[4];
					/* 40 */ Node* children[5];
					/* 68 */ std::uint32_t count;
					/* 6C */ std::uint8_t pad6C[0x14];
				};
				static_assert(sizeof(Entry) == 0x10);
				static_assert(sizeof(Node) == 0x80);
				static_assert(offsetof(Node, children) == 0x40);
				static_assert(offsetof(Node, count) == 0x68);

				/* 118 */ BSTBTree<Entry> settings;

				[[nodiscard]] std::uint32_t size() const noexcept(true) { return settings.size(); }

				[[nodiscard]] T* GetSetting(const char* name) const noexcept(true)
				{
					BSAutoFixedString key(name);
					const auto address = reinterpret_cast<std::uintptr_t>(key.c_str());
					auto node = reinterpret_cast<const Node*>(settings.root);
					while (node)
					{
						std::uint32_t first = 0, last = node->count;
						while (first < last)
						{
							const auto mid = (first + last) / 2;
							if (address > reinterpret_cast<std::uintptr_t>(node->entries[mid].key.c_str()))
								first = mid + 1;
							else
								last = mid;
						}
						if (first < node->count && node->entries[first].key == key)
							return node->entries[first].value;
						node = node->children[first];
					}
					return nullptr;
				}

				template <class Visitor>
				void ForEach(Visitor&& visitor) const
				{
					VisitNode(reinterpret_cast<const Node*>(settings.root), visitor);
				}

			private:
				template <class Visitor>
				static void VisitNode(const Node* node, Visitor& visitor)
				{
					if (!node) return;
					for (std::uint32_t i = 0; i < node->count; ++i)
					{
						VisitNode(node->children[i], visitor);
						visitor(node->entries[i].value);
					}
					VisitNode(node->children[node->count], visitor);
				}
			};
			static_assert(sizeof(SettingCollectionMap<int>) == 0x138);

			// 1.11.246.0: size 0x138, vtable RVA 0x2FA82D0, 12 virtual functions
			class GameSettingCollection : public SettingCollectionMap<Setting>
			{
			public:
				// Opens the collection on Handle, reads the setting named Name and closes it again (0x14095C360).
				virtual bool ReadNamedSetting(void* Handle, const char* Name) = 0;	// 0B

				[[nodiscard]] static GameSettingCollection* GetSingleton() noexcept(true)
				{
					static Common::Relocation<GameSettingCollection**> singleton{ IDs::Setting::GameSettingCollection::Singleton };
					return *singleton.Get();
				}
			};
			static_assert(sizeof(GameSettingCollection) == 0x138);
			static_assert(offsetof(GameSettingCollection, settings) == 0x118);
		}
	}
}
