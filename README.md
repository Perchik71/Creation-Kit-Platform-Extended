<p align="center">
  <img src="https://github.com/Perchik71/Creation-Kit-Platform-Extended/blob/master/Resources/logo.png" alt="Creation Kit Platform Extended" border="0">
</p>

<p align="center">
  A collection of modifications, enhancements, and reverse engineered resources for Creation Kit by Bethesda.
</p>

<p align="center">
  <a href="https://github.com/Perchik71/Creation-Kit-Platform-Extended/blob/master/LICENSE"><img src="https://img.shields.io/github/license/Perchik71/Creation-Kit-Platform-Extended?style=for-the-badge&color=007d34"></img></a>
  <img src="https://img.shields.io/github/languages/top/perchik71/Creation-Kit-Platform-Extended?style=for-the-badge&color=e8793e"></img> 
  <img src="https://img.shields.io/github/repo-size/Perchik71/Creation-Kit-Platform-Extended?style=for-the-badge"></img> 
  <img src="https://img.shields.io/github/last-commit/perchik71/Creation-Kit-Platform-Extended?style=for-the-badge"></img> 
</p>

# Description
**The mod is in the early stages of development.** The platform will support all known versions of Creation Kit in the future, but the focus is first on the Creation Kit version SSE 1.6.1130. This is a continuation of two previously arhived projects, as I am already confused in the code. The new implementation is based on the principle of modularity, which will allow for better control of development.

# Fixed or added
:white_check_mark: Fast file loading  
:white_check_mark: Interception of warning messages in the console  
:white_check_mark: Unicode support  

# Dependencies
[detours](https://github.com/Nukem9/detours.git) URL **https://github.com/Nukem9/detours.git** IN **Dependencies/detours**  
[mINI](https://github.com/Perchik71/mINI.git) URL **https://github.com/Perchik71/mINI.git** IN **Dependencies/mINI**  
[Zydis](https://github.com/zyantific/zydis.git) URL **https://github.com/zyantific/zydis.git** IN **Dependencies/zydis**  
[zipper](https://github.com/kuba--/zip.git) URL **https://github.com/kuba--/zip.git** IN **Dependencies/zipper**  
[libdeflate](https://github.com/ebiggers/libdeflate.git) URL **https://github.com/ebiggers/libdeflate.git** IN **Dependencies/libdeflate**  
[xbyak](https://github.com/herumi/xbyak.git) URL **https://github.com/herumi/xbyak.git** IN **Dependencies/xbyak**  

# Compilation
Open a project in **Visual Studio 2019** or later. Build the project in **Release** mode.  
Compile the Unicode Plugin to **Lazarus 1.8.2** or later. Take .dll's from the **x64** folder and move it to the folder with the game. 
Take the database **CreationKitPlatformExtended_SSE_1_6_1130.database** from the folder **Database/1_6_1130/**. 
You will also need **CreationKitPlatformExtendedMessagesBlacklist.txt** and **CreationKitPlatformExtended.ini**. 

# Credits
[Nukem9](https://github.com/Nukem9) (for experimental functions, hash and so on, very useful)  

# License
**GPLv3**: <a>https://www.gnu.org/licenses/gpl-3.0.html</a>. <br />
Copyright © 2023-2024 aka perchik71. All rights reserved. <br />
The memory manager is licensed under **LGPLv3**: <a>https://www.gnu.org/licenses/lgpl-3.0.html</a>. <br />
Copyright © 2023 aka perchik71. All rights reserved.
