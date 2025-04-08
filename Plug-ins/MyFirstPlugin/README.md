# Description
This is an example of a simple plugin for **CKPE** and serves solely to start your own plugin.  

> [!NOTE]
> All plugins must be in the folder: *`<GAMEDIR>\CKPEPlugins\`*.
> Your debug log is there too.

> [!IMPORTANT]
> All functions must be implemented, read the comments.

# What does the plugin do?
![What does the plugin do?](https://github.com/Perchik71/Creation-Kit-Platform-Extended/blob/master/Plug-ins/MyFirstPlugin/What.png)

# Briefly
1. If you don't need a menu, don't create one. `*Data->SubMenu = nullptr;`
2. To create a menu, familiarize yourself with the function of creating it.
3. There are only 5 indexes available to you, if you take more, it will lead to an error with other plugins.
4. There is an API version in the structure. You are guided by it. In the future, the API will be updated only with the addition of new features to the core.
5. Creating plugins requires basic knowledge in RE. You can use it to gain practical knowledge.

# Compilation
The code itself was developed in **`Visual Studio 2019`**.
If you are going to make your own project, keep in mind that the **`Common.h`** file should be added as a must for everyone.

# License
This file for a test project, and serves as a guide for creating plugins.

You can also use the assets of this test project, some platform classes are passed with their interfaces 
without implementation, so that they can be used inside the plugin, while you will not necessarily need 
to open the code to the public. But you cannot use CKPE assets, in which case you must disclose 
the code and indicate authorship. And also you can't re-license, only GPLv3.
