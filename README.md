[![MSBuild](https://github.com/Perchik71/Creation-Kit-Platform-Extended/actions/workflows/msbuild.yml/badge.svg?branch=master)](https://github.com/Perchik71/Creation-Kit-Platform-Extended/actions/workflows/msbuild.yml) 
![GitHub License](https://img.shields.io/github/license/perchik71/Creation-Kit-Platform-Extended) 
![GitHub top language](https://img.shields.io/github/languages/top/perchik71/Creation-Kit-Platform-Extended)
![GitHub last commit (branch)](https://img.shields.io/github/last-commit/perchik71/Creation-Kit-Platform-Extended/master)
![GitHub repo size](https://img.shields.io/github/repo-size/perchik71/Creation-Kit-Platform-Extended?color=rgb(170%2C40%2C80))
![GitHub Downloads (all assets, latest release)](https://img.shields.io/github/downloads/perchik71/Creation-Kit-Platform-Extended/latest/total?color=rgb(00%2C150%2C100))  

<p align="center">
  <img src="https://github.com/Perchik71/Creation-Kit-Platform-Extended/blob/master/Resources/logo.png" alt="Creation Kit Platform Extended" border="0">
</p>

<p align="center">
  A collection of modifications, enhancements, and reverse engineered resources for Creation Kit by Bethesda.
</p>

# Description
<p align="left">
<img src="https://github.com/Perchik71/Creation-Kit-Platform-Extended/blob/master/Resources/ckpe_ai.png" alt="Creation Kit Platform Extended" border="0" align="left" height=393>
</p>

The powerful open-source platform makes numerous fixes and improvements to the **Creation Kit** editor from **Bethesda**. This is a continuation of two previously archived projects, as I am already confused in the code. The new implementation is based on the principle of modularity, which will allow for better control of development.  
  
The important thing that CKPE adds and fixes:  
:white_check_mark: Fast file loading  
:white_check_mark: Interception of warning messages in the log  
:white_check_mark: Unicode support  
:white_check_mark: Multiple UI Fix and etc  
:white_check_mark: Dark theme  
:white_check_mark: Create master files and open them as plugins  
:white_check_mark: Opening plugins as master files if they are in dependencies  
:white_check_mark: Filtering by active forms and cell  
:white_check_mark: Filtering mods in the Data dialog  
:white_check_mark: Many other fixes are also from the author of [@Nukem9](https://github.com/Nukem9)  
This is not a complete list of improvements.

<table>
  <caption>
    At the moment, the platform supports <b>CK</b> version:
  </caption>
  <thead>
    <tr>
      <th scope="col">Version</th>
      <th scope="col">State</th>
      <th scope="col">Version</th>
      <th scope="col">State</th>
      <th scope="col">Version</th>
      <th scope="col">State</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <th scope="row" colspan="2">Skyrim SE</th>
      <th scope="row" colspan="2">Fallout 4</th>
      <th scope="row" colspan="2">Starfield</th>
    </tr>
    <tr>
      <td>1.5.3.0</td>
      <td>Active</td>
      <td>1.10.162.0</td>
      <td>Active</td>
      <td>1.13.61.0</td>
      <td>Deprecated</td>
    </tr>
    <tr>
      <td>1.5.73.0</td>
      <td>Active</td>
      <td>1.10.943.1</td>
      <td>Deprecated</td>
      <td>1.14.70.0</td>
      <td>Active</td>
    </tr>
    <tr>
      <td>1.6.438.0</td>
      <td>Active</td>
      <td>1.10.982.3</td>
      <td>Active</td>
      <td></td>
      <td></td>
    </tr>
    <tr>
      <td>1.6.1130.0</td>
      <td>Active</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
    </tr>
  </tbody>
  <tfoot>
    <tr>
      <th scope="row" colspan="2">Status platform</th>
      <th scope="row" colspan="2">Status platform</th>
      <th scope="row" colspan="2">Status platform</th>
    </tr>
    <tr>
      <th scope="row" colspan="2">Stable</th>
      <th scope="row" colspan="2">Stable</th>
      <th scope="row" colspan="2">Stable</th>
    </tr>
  </tfoot>
</table>
Marked as <b>deprecated</b>, means that there will be nothing new for these versions, older versions, as an option, are more stable. Attention detailed information is relevant for the current commit.  

# 
> [!IMPORTANT] 
> **CK** does not work well with **ENB**. I wrote **ckpe_loader**. This is console app, for 15 seconds renames from the known to me .dll files that harm the work of CK this time is enough to load the CK itself. All renames .dll files will be restored name after this time.

> [!NOTE]
> You can remove *Steam* from the **Creation Kit**.  
> You just need to manually replace the string *`steam_api64.dll`* on *`winhttp.dll`* in any **HEX editor**. Since the new string is four bytes shorter, the remaining bytes are set to **`0x00`**. Or compile the **NoSteam Patcher** in **Lazarus** and use it.  
> For versions **`1.6.438.0`** and **`1.10.162.0`**, you first need to remove **`Steam DRM`**, for example, [Steamless](https://github.com/atom0s/Steamless).  

# Demonstration
<p align="center">
  <img src="https://github.com/Perchik71/Creation-Kit-Platform-Extended/blob/master/Resources/darktheme.png" width="640px" alt="Creation Kit Platform Extended" border="0">
</p>

# Mod Settings
To hide errors, create a file *CreationKitPlatformExtendedMessagesBlacklist.txt*.  
**IMPORTANT**: Errors should be copied from the mod's Log window and one error is one line. The file must be in **ANSI** encoding.  
*CreationKitPlatformExtended.ini* the main file with the settings and comes with the code, be sure to read.

# Unicode
**`ONLY SURROGATE`**: Support for English and native languages without special characters.
For understanding, the application uses the ancient **`ANSI`** type string representation. The idea is to feed the text already in your native encoding. When loading `.esp`, `.esl`, `.esm` files all strings are translated from **`UTF-8`** to **`WinCP`**, and when saved back. **`WinCP`** is the current encoding in your operating system.  

> [!IMPORTANT] 
> In **`Win10`**, in the language settings, there is now an option to work with **`UTF-8`**, you need to turn it off otherwise there will be only **`?`**.

# Dependencies
[VoltekLib](https://github.com/Perchik71/VoltekLib.git) by `perchik71` *(recursive)*  
[mINI](https://github.com/Perchik71/mINI.git) by `perchik71` and `metayeti`  
[Zydis](https://github.com/zyantific/zydis.git) by `zyantifi` and other  
[zipper](https://github.com/kuba--/zip.git) by `kuba--`  
[libdeflate](https://github.com/ebiggers/libdeflate.git) by `ebiggers` and other  
[xbyak](https://github.com/herumi/xbyak.git) by `herumi` and other    
[DirectXTex](https://github.com/microsoft/DirectXTex.git) by `microsoft`  
[jDialogs](https://github.com/Perchik71/jDialogs.git) by `perchik71` *(recursive)*

# Compilation
Open a project in **`Visual Studio 2022`** or later. Build the project in **`Release`** mode or **`Release-Qt`** for `Starfield`.  
You can take compiled ones automatically, they are of decent quality.

# Credits
[Nukem9](https://github.com/Nukem9) (for experimental functions, hash and so on, very useful)  
[adzm](https://github.com/adzm) (I really appreciate your work, you have [done](https://github.com/adzm/win32-custom-menubar-aero-theme) what I have long wanted to do myself)  
[yak3d](https://github.com/yak3d) (For the contribution and development of workflow)  

# License
**GPLv3**: <a>https://www.gnu.org/licenses/gpl-3.0.html</a>. <br />
Copyright © 2023-2024 aka perchik71. All rights reserved. <br />
File `Stuffs\FO4\CreationKitPlatformExtended_FO4_Resources.pak` contains files are proprietary and not licensed. <br />
Dependencies are under their respective licenses. 
