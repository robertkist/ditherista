Building Ditherista
===================

Ditherista is a GUI for the libdither C library (https://github.com/robertkist/libdither).

For each platform, there are 3 basic steps involved:
1. building libdither
2. building the Ditherista application
3. optional: building an installer

Other Notes:
* The recommended Qt version for Ditherista is Qt6, but it is also possible to build Ditherista for Qt5.

Building for macOS
------------------

Pre-Requisites:
* Install Qt 6.6.1 or newer via the official Qt Online Installer. Do not use homebrew's Qt version -
  its deployment tool (```macdeployqt```) doesn't work properly, and you won't be able to build a fully
  functioning application bundle.
* Downloads:
  * Qt Online Installer: https://www.qt.io/download-open-source

Notes on Qt Version:
* If a newer 6.x release is available, I recommend using the newest release, and only falling back to the
  release specified above if there are problems. Why? Qt keeps fixing bugs regularly, and the Qt features 
  used by Ditherista are relatively stable, so there is little risk for things to break when using a newer
  Qt version.

Building:
1. clone the ditherista git repository: ```git clone --recursive https://github.com/robertkist/ditherista.git```
2. change to the libdither directory: ```cd ditherista/libdither```
3. build libdither library: ```make libdither_universal``` (run ```make libdither_arm64``` or ```make libdither_x64``` to build a pure Apple Silicon or Intel x64 version of the libdither library)
4. change back to ditherista repository root: ```cd ..```
5. open Ditherista's Makefile in a text editor and check that ```MAC_QT_BIN_PATH``` points to your Qt installation.
6. build ditherista: ```make app_universal``` (run ```make app_arm64``` or ```make app_x64``` to build a pure Apple Silicon or Intel x64 version of Ditherista)
7. optional: ```make installer``` to build a macOS .dmg image. The .dmg can be found in the \dist directory.
8. You can find Ditherista.app and the installer .dmg in the ```dist``` directory.

The oldest version of MacOS supported is 10.14 Mojave.

Building for Linux
------------------

Pre-Requisites:
* You need to have Qt6 or Qt5 installed, with the ```qmake``` command of the desired Qt version in the path. 
* E.g. on Ubuntu, you should install the following pre-requisites:
```sudo apt install qtbase5-dev qttools5-dev-tools libqt5svg5 libqt5svg5-dev qt5-default libgl1-mesa-dev```

Building:
1. clone the ditherista git repository: ```git clone --recursive https://github.com/robertkist/ditherista.git```
2. change to the libdither directory: ```cd ditherista/libdither```
3. build libdither library: ```make libdither```
4. change back to ditherista repository root: ```cd ..```
5. build ditherista: ```make app```
6. optional: ```make installer```. This builds a Debian (.deb) installer package and a .tar.gz archive, which can be found in the ```dist``` directory.
7. You can find the ditherista application directory, the Debian installer package and the .tar.gz archive in the ```dist``` directory.

There are 2 additional installer targets:
* AppImage
* .tar.gz archive which include all library dependencies - i.e. Qt and a few others

Caveat: To make those 2 targets, you will need 'linuxdeployqt'. Linuxdeployqt (https://github.com/probonopd/linuxdeployqt) only runs on certain Linux versions in order to maximize
compatibility for the created AppImages. For example, to build an AppImage for all current Ubuntu versions (as of writing this readme, the most 
current is 22.04), linuxdeployqt requires you to build on Ubuntu 18.04. If you fulfill this requirement, all you need is to have linuxdeployqt in the path
and you can build those targets. You can also change the path to linuxdeployqt in the Makefile.

Building for Windows
--------------------

Pre-Requisites:
* Libdither on Windows is built using Qt's MingW compiler. Use Qt's official Online Installer to install
Qt 6.3.1 or newer and the MingW compiler. You will also need to install the GNU ```make``` tool, 
version 4.3 or newer, via Chocolatey. If you plan on building an installer (i.e. setup.exe), you 
will need to install the latest version of the Nullsoft Scriptable Installer System (NSIS). 
* Downloads:
  * Qt Online Installer: https://www.qt.io/download-open-source
  * Chocolatey: https://chocolatey.org/install 
  * GNU make 4.3, via Chocolatey: https://community.chocolatey.org/packages/make
  * NSIS: https://nsis.sourceforge.io/Download).

Building:
1. clone the ditherista git repository: ```git clone --recursive https://github.com/robertkist/ditherista.git```
2. change to the libdither directory: ```cd ditherista/libdither```
3. open libdither's Makefile in a text editor and check that ```WIN_MINGW_BIN_PATH``` points to Qt's MingW compiler
4build libdither library: ```make libdither```
4. change back to ditherista repository root: ```cd ..```
5. open Ditherista's Makefile in a text editor and check that ```WIN_QT_BIN_PATH``` points to your Qt installation, ```WIN_MINGW_BIN_PATH``` points to Qt's MingW compiler, and ```WIN_NSIS_EXE``` to the
5. build ditherista: ```make app```
6. run ```make installer``` to build a windows installer (setup.exe). The installer can be found in the ```dist``` directory.
7. You can find the ditherista application directory and the installer in the ```dist``` directory.

Making Changes
--------------

Help files exist in `src/help`. When adding, removing, or renaming help content, be sure to update the 
`src/app/resources.qrc` file accordingly.