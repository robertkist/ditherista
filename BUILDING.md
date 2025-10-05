Building Ditherista
===================

Ditherista is a GUI for the [libdither C library](https://github.com/robertkist/libdither).

For each platform, there are 3 basic steps involved:
1. building libdither
2. building the Ditherista application
3. optional: building an installer

Minimum Versions
----------------

* all systems: Qt 6.6.x (RECOMMENDED for Win, Mac: 6.9.2 or newer; RECOMMENDED for Linux: 6.8.2 for widest compatibility)
* all systems: GNU make 3.8.1 (RECOMMENDED 4.3 or newer)
* Windows: Visual Studio 2022
* Windows: MinGW 13.10 (included in Qt Online installer)
* Windows: Nullsoft Installer System (NSIS) any version
* Mac: XCode toolchain or command-line tools for macOS Mojave (RECOMMENDED: macOS Sonoma or newer)
* Linux: GCC 11.4 or newer 

Building for macOS
------------------

Pre-Requisites:
* Install Qt via the official Qt Online Installer. Do not use homebrew's Qt version -
  its deployment tool (```macdeployqt```) doesn't work properly, and you won't be able to build a fully
  functioning application bundle.
* Downloads:
  * [Qt Online Installer](https://www.qt.io/download-open-source)

Notes on Qt Version:
* If a newer Qt release is available, I recommend using the newest release, and only falling back to the
  release specified above if there are problems. Why? Qt keeps fixing bugs regularly, and the Qt features 
  used by Ditherista are relatively stable, so there is little risk for things to break when using a newer
  Qt version.
* On MacOS, newer Qt releases also work better with the newest features released by Apple for MacOS.

Building:
1. clone the ditherista git repository: ```git clone --recursive https://github.com/robertkist/ditherista.git```
2. change to the libdither directory: ```cd ditherista/libdither```
3. build libdither library: ```make libdither_universal``` (run ```make libdither_arm64``` or ```make libdither_x64``` to build a pure Apple Silicon or Intel x64 version of the libdither library)
4. change back to ditherista repository root: ```cd ..```
5. open Ditherista's Makefile in a text editor and check that ```MAC_QT_BIN_PATH``` points to your Qt installation.
6. build ditherista: ```make app_universal``` (run ```make app_arm64``` or ```make app_x64``` to build a pure Apple Silicon or Intel x64 version of Ditherista)
7. optional: ```make installer``` to build a macOS .dmg image. The .dmg can be found in the \dist directory.
8. You can find Ditherista.app and the installer .dmg in the ```dist``` directory.

Tip: for development and testing you can just run ```make libdither``` and ```make app```, which is faster than 
building a universal app every time.

Building for Linux
------------------

Pre-Requisites:
* You need to have Qt6 installed, with the ```qmake``` command of the desired Qt version in the path.
* Ubuntu: for building you will need to use the offical Qt Online installer
* Ubuntu: you also need to install the following pre-requisites: ```sudo apt install libgl1-mesa-dev```
* Fedora: the latest Fedora distribution offers all necessary Qt6 packages for building

Building:
1. clone the ditherista git repository: ```git clone --recursive https://github.com/robertkist/ditherista.git```
2. make sure you have ```qmake``` in your ```$PATH``` 
3. change to the libdither directory: ```cd ditherista/libdither```
4. build libdither library: ```make libdither```
5. change back to ditherista repository root: ```cd ..```
6. build ditherista: ```make app```
7. optional: ```make installer```. This builds a Debian (.deb) installer package and a .tar.gz archive, which can be found in the ```dist``` directory.
8. You can find the ditherista application directory, the Debian installer package and the .tar.gz archive in the ```dist``` directory.

There are 2 additional installer targets:
* AppImage - It is recommended to build this Ubuntu 22.04 using 
  Qt 6.8.2 for widest compatibility with different Linux distributions.
* .tar.gz archive which includes all library dependencies - i.e. Qt and a few others

Building for Windows (MinGW)
----------------------------

This is the recommended way to build Ditherista on Windows.

Pre-Requisites:
* Libdither on Windows is built using Qt's MingW compiler. Use Qt's official Online Installer to install
Qt and the MingW compiler. You will also need to install the GNU ```make``` tool via Chocolatey. 
* If you plan on building an installer (i.e. setup.exe), you 
will need to install the latest version of the Nullsoft Scriptable Installer System (NSIS). 
* Downloads:
  * [Qt Online Installer (includes MinGW)](https://www.qt.io/download-open-source)
  * [Chocolatey](https://chocolatey.org/install) 
  * [GNU make via Chocolatey](https://community.chocolatey.org/packages/make)
  * [NSIS](https://nsis.sourceforge.io/Download)

Building:
1. clone the ditherista git repository: ```git clone --recursive https://github.com/robertkist/ditherista.git```
2. change to the libdither directory: ```cd ditherista/libdither```
3. open libdither's Makefile in a text editor and check that ```WIN_MINGW_BIN_PATH``` points to Qt's MingW compiler.
4. build libdither library: ```make libdither```
5. change back to ditherista repository root: ```cd ..```
6. open Ditherista's Makefile in a text editor and check that ```WIN_QT_BIN_PATH``` points to your Qt installation, 
   ```WIN_MINGW_BIN_PATH``` points to Qt's MingW compiler, and ```WIN_NSIS_EXE``` points to ```makensis.exe```.
7. build ditherista: ```make app```
8. run ```make installer``` to build a windows installer (setup.exe). The installer can be found in the ```dist``` directory.
9. You can find the ditherista application directory and the installer in the ```dist``` directory.

Building for Windows (MS Visual Studio)
---------------------------------------

Pre-Requisites:
* Libdither on Windows can also be built with Microsoft's Visual Studio. Use Qt's official Online Installer to 
  install Qt. You should also need to install the GNU ```make``` tool via Chocolatey. 
* If you plan on building an installer (i.e. setup.exe), you will need to install the latest version of the Nullsoft 
  Scriptable Installer System (NSIS).
* Downloads:
  * [Visual Studio 2022](https://visualstudio.microsoft.com/vs/)
  * [Qt Online Installer](https://www.qt.io/download-open-source)
  * [Chocolatey](https://chocolatey.org/install)
  * [GNU make via Chocolatey](https://community.chocolatey.org/packages/make)
  * [NSIS](https://nsis.sourceforge.io/Download)

Building:
1. clone the ditherista git repository: ```git clone --recursive https://github.com/robertkist/ditherista.git```
2. change to the libdither directory: ```cd ditherista/libdither```
3. run Visual Studio's ```vcvars64.bat``` command to set up the build environment.
4. build libdither library: ```make libdither_msvc```
5. change back to ditherista repository root: ```cd ..```
6. open Ditherista's Makefile in a text editor and check that ```WIN_NSIS_EXE``` points to ```makensis.exe```.
7. build ditherista: ```make app_msvc```
8. run ```make installer``` to build a windows installer (setup.exe). The installer can be found in the ```dist``` directory.
9. You can find the Ditherista application directory and the installer in the ```dist``` directory.

Making Changes
--------------

Help files exist in `src/help`. When adding, removing, or renaming help content, be sure to update the 
`src/app/resources.qrc` file accordingly.
