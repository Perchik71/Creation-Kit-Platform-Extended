<div align="right">
	<details>
		<summary>🌐 Language</summary>
		<div>
			<div align="center">
				<a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=en">English</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=zh-CN">简体中文</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=zh-TW">繁體中文</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=ja">日本語</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=ko">한국어</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=hi">हिन्दी</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=th">ไทย</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=fr">Français</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=de">Deutsch</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=es">Español</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=it">Italiano</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=ru">Русский</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=pt">Português</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=nl">Nederlands</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=pl">Polski</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=ar">العربية</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=fa">فارسی</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=tr">Türkçe</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=vi">Tiếng Việt</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=id">Bahasa Indonesia</a>
				| <a href="https://openaitx.github.io/view.html?user=Perchik71&project=Creation-Kit-Platform-Extended&lang=as">অসমীয়া</a>
			</div>
		</div>
	</details>
</div>
<br>
<div align="center">

<img src="Resources/logo.png" alt="Creation Kit Platform Extended" width="400px">

A collection of modifications, enhancements, and reverse engineered resources for Bethesda's Creation Kit.

[![CI](https://img.shields.io/github/actions/workflow/status/Perchik71/Creation-Kit-Platform-Extended/msbuild.yml?branch=master&style=for-the-badge&label=CI&logo=githubactions&logoColor=white)](https://github.com/Perchik71/Creation-Kit-Platform-Extended/actions/workflows/msbuild.yml)
[![License](https://img.shields.io/badge/License-LGPL%203.0-blue?style=for-the-badge)](LICENSE)
[![Release](https://img.shields.io/github/v/release/Perchik71/Creation-Kit-Platform-Extended?style=for-the-badge&label=release&color=orange)](https://github.com/Perchik71/Creation-Kit-Platform-Extended/releases/latest)
[![Editors](https://img.shields.io/badge/Editors-Skyrim%20SE%20%C2%B7%20Fallout%204%20%C2%B7%20Starfield-3a7d44?style=for-the-badge)](#supported-editors)

</div>
<br>

## Description

**Creation Kit Platform Extended (CKPE)** is a powerful shared-source platform that provides numerous fixes and improvements to **Bethesda's Creation Kit**, including support for **Skyrim Special Edition**, **Fallout 4** and **Starfield**.

CKPE is a direct successor to the projects that preceded it, including [SSE CKFixes](https://github.com/Nukem9/skyrimse-test), [SSE CKFixes Update](https://github.com/Perchik71/SkyrimSETest), [FO4 CKFixes](https://github.com/Perchik71/Fallout4Test) and [Unicode Patch for SSE CK 1.5.73](https://github.com/Perchik71/usse_test), which is the project that started it all for **CKPE**'s author, **perchik71**.

For more information, please take a look at [CKPE's Wiki](https://github.com/Perchik71/Creation-Kit-Platform-Extended/wiki).

## Supported Editors

| Game Editor | Version |
|---|---|
| **Skyrim Special Edition** | **1.5.73.0** and later |
| **Fallout 4** | **1.10.162.0** and later |
| **Starfield** | **1.15.222.0** and later |

For more information, please check the brief section in the [Wiki page](https://github.com/Perchik71/Creation-Kit-Platform-Extended/wiki#brief).

## Installation

>[!IMPORTANT]
>**CKPE** has a **Regular (AVX2)** and a **No AVX2** release depending on your CPU.<br>
>To determine which version of **CKPE** you should install, you can use **Steam**:
>1. Open **Steam** → Click on **Help** in the top menu bar → **System Information**.<br>
>2. Find the **Processor Information** section and check what it says for **AVX2**.
>
>If it says `AVX2: Supported`, then use the **Regular (AVX2)** Release.<br>
>If it says `AVX2: Unsupported`, then use the **No AVX2** Release.

>[!NOTE]
>After a fresh install or update, **ckpe_loader.exe** should be in the same folder as **CreationKit.exe**.

### Fresh Install

1. Install a [supported game](#supported-editors) and its **Creation Kit** from **Steam**.
2. Download the [latest release](https://github.com/Perchik71/Creation-Kit-Platform-Extended/releases) of **CKPE** for your game and CPU.
3. Extract all files into the game's **root** directory, alongside **CreationKit.exe**.

### Updating

1. Download the [latest release](https://github.com/Perchik71/Creation-Kit-Platform-Extended/releases) of **CKPE** for your game and CPU.
2. Extract all files into the game's **root** directory, alongside **CreationKit.exe**.
3. Agree to replace all files when asked.

### Beta Builds

You can also test the latest development builds of **CKPE** for your game.<br>
Be warned that these are in-development builds and may have issues.
1. Download the [latest development build](https://github.com/Perchik71/Creation-Kit-Platform-Extended/actions) of **CKPE** for your game and CPU.
2. Extract all files into the game's **root** directory, alongside **CreationKit.exe**.
3. Agree to replace all files if asked.

## Building

### Requirements

* Visual Studio 2022 or later.

### Clone the Repository

```bash
git clone --recurse-submodules https://github.com/Perchik71/Creation-Kit-Platform-Extended.git
```

### Opening and Building the Solution

1. Open the **`Creation Kit Platform Extended.sln`** solution in **Visual Studio 2022** or later.
2. Select either the **`Release`** or **`Release-NoAVX2`** build configuration depending on your processor.
3. Build the solution and copy the **Build Output** into your game's **root** directory next to **CreationKit.exe**.
4. Agree to replace all files if asked.

## Dependencies

| Dependency | Authors |
|---|---|
| **[toml11](https://github.com/ToruNiina/toml11)** | ToruNiina and contributors |
| **[Zydis](https://github.com/zyantific/zydis.git)** | zyantifi and contributors |
| **[zip](https://github.com/kuba--/zip.git)** | kuba-- |
| **[libdeflate](https://github.com/ebiggers/libdeflate.git)** | ebiggers and contributors |
| **[xbyak](https://github.com/herumi/xbyak.git)** | herumi and contributors |
| **[DirectXTex](https://github.com/microsoft/DirectXTex.git)** | Microsoft |
| **[jDialogs](https://github.com/Perchik71/jDialogs.git)** | perchik71 |

## Credits

| Contributor | Contribution |
|---|---|
| **[Nukem9](https://github.com/Nukem9)** | Experimental Functions, Hashes, and many other useful information. |
| **[adzm](https://github.com/adzm)** | Really appreciated work for the `win32-custom-menubar-aero-theme` project. |
| **[yak3d](https://github.com/yak3d)** | Contributions, development of Workflows and support for the latest Starfield. |
| **[FlenarnDev](https://github.com/FlenarnDev)** | Contributions, core and maintenance Fallout 4. |
| **[frakkin64](https://github.com/frakkin64)**, **[northaxosky/muteptr](https://github.com/northaxosky) | Contributions, maintenance AI Assistant. |
| **[GammaMetroid](https://github.com/GammaMetroid)** | Contributions, maintenance SSE branch. |
| **[DioKyrie-Git](https://github.com/DioKyrie-Git) and [TheGamerX20](https://github.com/TheGamerX20)** | Design of the README.md file. |

## License

**Creation Kit Platform Extended** is licensed under the [LGPLv3](https://www.gnu.org/licenses/lgpl-3.0.html) License starting with v0.6 (commit [9d93970](https://github.com/Perchik71/Creation-Kit-Platform-Extended/commit/9d93970cc3918099c895872d46a24aa29a34db11)).<br>
Earlier versions of **Creation Kit Platform Extended** were licensed under the [GPLv3](https://www.gnu.org/licenses/gpl-3.0.html) License.<br>
Copyright © 2023-2026 aka perchik71. All rights reserved.

>[!WARNING]
>`Stuffs\FO4\CreationKitPlatformExtended_FO4_Resources.pak` contains unlicensed proprietary files.<br>
>Similarly the `d3dcompiler` folder contains unlicensed proprietary files.<br>
>Dependencies are licensed under their own respective licenses.
