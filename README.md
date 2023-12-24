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
The platform will support all known versions of Creation Kit in the future, but the focus is first on the Creation Kit version SSE 1.6.1130. This is a continuation of two previously arhived projects, as I am already confused in the code. The new implementation is based on the principle of modularity, which will allow for better control of development.

# Fixed or added
:white_check_mark: Fast file loading  
:white_check_mark: Interception of warning messages in the log  
:white_check_mark: Unicode support  
:white_check_mark: Multiple UI Fix and etc  
:white_check_mark: Dark theme  
:white_check_mark: Create master files and open them as plugins  
:white_check_mark: Opening plugins as master files if they are in dependencies  
:white_check_mark: Many other fixes are also from the author of [@Nukem9](https://github.com/Nukem9)

# Demonstration
<p align="center">
  <img src="https://github.com/Perchik71/Creation-Kit-Platform-Extended/blob/master/Resources/darktheme.png" width="640px" alt="Creation Kit Platform Extended" border="0">
</p>

# ENB
**CK** does not work well with **ENB**.  

# Mod Settings
To hide errors, create a file *CreationKitPlatformExtendedMessagesBlacklist.txt*.  
**IMPORTANT**: Errors should be copied from the mod's Log window and one error is one line. The file must be in **ANSI** encoding.  
*CreationKitPlatformExtended.ini* the main file with the settings and comes with the code, be sure to read.

# Unicode
**ONLY SURROGATE**: Support for English and native languages without special characters.
For understanding, the application uses the ancient **ANSI** type string representation. The idea is to feed the text already in your native encoding. When loading .esp, .esl, .esm files all strings are translated from UTF-8 to WinCP, and when saved back. WinCP is the current encoding in your operating system.  
**IMPORTANT**:  
In **Win10**, in the language settings, there is now an option to work with UTF-8, you need to turn it off otherwise there will be only "?".

# Dependencies
[detours](https://github.com/Nukem9/detours.git) URL **https://github.com/Nukem9/detours.git** IN **Dependencies/detours**  
[mINI](https://github.com/Perchik71/mINI.git) URL **https://github.com/Perchik71/mINI.git** IN **Dependencies/mINI**  
[Zydis](https://github.com/zyantific/zydis.git) URL **https://github.com/zyantific/zydis.git** IN **Dependencies/zydis**  
[zipper](https://github.com/kuba--/zip.git) URL **https://github.com/kuba--/zip.git** IN **Dependencies/zipper**  
[libdeflate](https://github.com/ebiggers/libdeflate.git) URL **https://github.com/ebiggers/libdeflate.git** IN **Dependencies/libdeflate**  
[xbyak](https://github.com/herumi/xbyak.git) URL **https://github.com/herumi/xbyak.git** IN **Dependencies/xbyak**  
[jDialogs](https://github.com/Perchik71/jDialogs.git) URL **https://github.com/Perchik71/jDialogs.git** IN **Dependencies/jDialogs** *(recursive)*

# Compilation
Open a project in **Visual Studio 2019** or later. Build the project in **Release** mode.  
Compile the Unicode Plugin to **Lazarus 1.8.2** or later. Take .dll's from the **x64** folder and move it to the folder with the game. 
Take the database **CreationKitPlatformExtended_SSE_1_6_1130.database** from the folder **Database/1_6_1130/**. 
You will also need **Stuffs/SSE/CreationKitPlatformExtendedMessagesBlacklist.txt** and **Stuffs/SSE/CreationKitPlatformExtended.ini** to the game folder. 

# Credits
[Nukem9](https://github.com/Nukem9) (for experimental functions, hash and so on, very useful)  
[adzm](https://github.com/adzm) (I really appreciate your work, you have [done](https://github.com/adzm/win32-custom-menubar-aero-theme) what I have long wanted to do myself)  

# License
**GPLv3**: <a>https://www.gnu.org/licenses/gpl-3.0.html</a>. <br />
Copyright © 2023-2024 aka perchik71. All rights reserved. <br />
The memory manager is licensed under **LGPLv3**: <a>https://www.gnu.org/licenses/lgpl-3.0.html</a>. <br />
Copyright © 2023 aka perchik71. All rights reserved.
