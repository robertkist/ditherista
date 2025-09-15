# EDIT THE FOLLOWING 3 PATHS BELOW ON WINDOWS:
# paths are the default install locations for Qt/MingW and Nullsoft Installer Maker (NSIS)
WIN_QT_BIN_PATH=C:\\Qt\\6.9.2\\mingw_64\bin
WIN_MINGW_BIN_PATH=C:\\Qt\\Tools\\mingw1310_64\\bin
WIN_NSIS_EXE=C:\Program Files (x86)\NSIS\makensis.exe
# EDIT THE FOLLOWING PATH BELOW FOR MACOS:
MAC_QT_BIN_PATH=~/Qt/6.9.2/macos/bin

APPNAME=ditherista
APPNAME_CS=Ditherista
APP_YEAR=2025
APP_VERSION=$(APP_YEAR).09.14a.RC2
APP_HOMEPAGE=http://github.com/robertkist

BUILDDIR=build
DISTDIR=dist
DISTBUILDDIR=dist_build
LIBDITHER_ROOT=libdither

LIBNAME=libdither
QMAKEOPTS=-Wall -Wlogic

app_x64: QTARCHS=QMAKE_APPLE_DEVICE_ARCHS="x86_64"
app_arm64: QTARCHS=QMAKE_APPLE_DEVICE_ARCHS="arm64"
app_universal: QTARCHS=QMAKE_APPLE_DEVICE_ARCHS="x86_64 arm64"

ifdef OS  # Windows:
define fn_mkdir
	if not exist "$(1)" mkdir "$(1)"
endef
ABOUT_INI_PATH=src\\app\\resources\\about.ini
define fn_make_about_ini
	-del $(ABOUT_INI_PATH)
	echo [about]>$(ABOUT_INI_PATH)
	echo title=$(APPNAME_CS)>>$(ABOUT_INI_PATH)
	echo version=$(1)>>$(ABOUT_INI_PATH)
	echo homepage=$(2)>>$(ABOUT_INI_PATH)
	echo year=$(3)>>$(ABOUT_INI_PATH)
endef
	DEL=del
	DELTREE=rd /s /q
	QMAKE=SET PATH=$(WIN_MINGW_BIN_PATH);$(WIN_QT_BIN_PATH);%PATH% && qmake
	LUPDATE=SET PATH=$(WIN_MINGW_BIN_PATH);$(WIN_QT_BIN_PATH);%PATH% && lupdate
	LRELEASE=SET PATH=$(WIN_MINGW_BIN_PATH);$(WIN_QT_BIN_PATH);%PATH% && lrelease
	DEPLOYQT=SET PATH=$(WIN_MINGW_BIN_PATH);$(WIN_QT_BIN_PATH);%PATH% && windeployqt
	MAKE=SET PATH=$(WIN_MINGW_BIN_PATH);$(WIN_QT_BIN_PATH);%PATH% && make
	LIBDITHERPATH=$(LIBDITHER_ROOT)\\dist\\
	RUNCMD=$(DISTDIR)/$(APPNAME)/$(APPNAME).exe
	APPBUILD=app_win
	INSTALLERBUILD=installer_win
	INSTALLERNAME=$(APPNAME)_$(APP_VERSION)_setup.exe
	HELP_PATH=src\\app\\resources\\help
	HELP_COPY_CMD=xcopy src\\help\\en_win $(HELP_PATH) /I /Y
else  # Unix based platforms
define fn_mkdir
	@mkdir -p "$(1)"
endef
ABOUT_INI_PATH=src/app/resources/about.ini
define fn_make_about_ini
	-rm -f $(ABOUT_INI_PATH)
	echo "[about]">$(ABOUT_INI_PATH)
	echo "title=$(APPNAME_CS)">>$(ABOUT_INI_PATH)
	echo "version=$(1)">>$(ABOUT_INI_PATH)
	echo "homepage=$(2)">>$(ABOUT_INI_PATH)
	echo "year=$(3)">>$(ABOUT_INI_PATH)
endef
	DEL=rm
	DELTREE=rm -Rf
	MAKE=make
	LIBDITHERPATH=$(LIBDITHER_ROOT)/dist/
	HELP_PATH=./src/app/resources/help
	ifeq ($(shell uname), Darwin)  # macOS
		QMAKE=export LIBRARY_PATH=$$HOME:$$LIBRARY_PATH && $(MAC_QT_BIN_PATH)/qmake
		LUPDATE=$(MAC_QT_BIN_PATH)/lupdate
		LRELEASE=$(MAC_QT_BIN_PATH)/lrelease
		DEPLOYQT=$(MAC_QT_BIN_PATH)/macdeployqt
		RUNCMD=$(DISTDIR)/$(APPNAME_CS).app/Contents/MacOS/application
		APPBUILD=app_mac
		INSTALLERBUILD=installer_mac
		ifneq ("$(wildcard dist/Ditherista.app/Contents/MacOS/application)","")
			ARCH=$(shell lipo -archs dist/Ditherista.app/Contents/MacOS/application)
			ifeq ($(ARCH),x86_64 arm64)
				ARCH=universal
			endif
		endif
		DMGNAME=$(APPNAME)_$(APP_VERSION)_$(ARCH)
		HELP_COPY_CMD=cp -R ./src/help/en_macos/ $(HELP_PATH)
	else  # other Unix / Linux
		QMAKE=qmake
		LUPDATE=lupdate
		LRELEASE=lrelease
		DEPLOYQT=linuxdeployqt
		RUNCMD=$(DISTDIR)/$(APPNAME)/$(APPNAME)
		APPBUILD=app_deb
		INSTALLERBUILD=installer_deb
		ARCH=$(shell dpkg --print-architecture)
		DEBNAME=$(APPNAME)_$(APP_VERSION)-1_$(ARCH)
		HELP_COPY_CMD=cp -R ./src/help/en_linux/ $(HELP_PATH)
	endif
endif

.PHONY: all
all:
	@echo "Targets:"
	@echo "* app - builds the application for the current platform"
	@echo "* app_x64 - builds the application for the macOS x86 platform"
	@echo "* app_arm64 - builds the application for the macOS arm64 platform"
	@echo "* app_universal - builds a universal macOS application"
	@echo "* installer - builds an installer for the current platform"
	@echo "* installer_tgz_full - builds a Linux tar.gz archive including all dependencies"
	@echo "* appimage - builds a Linux AppImage"
	@echo "* run - for development: builds and runs the application"
	@echo "* clean"

.PHONY: app_x64
app_x64: app

.PHONY: app_arm64
app_arm64: app

.PHONY: app_universal
app_universal: app

.PHONY: app
app: $(APPBUILD)

.PHONY: installer
installer: $(INSTALLERBUILD)

.PHONY: app_build
app_build:
	$(call fn_mkdir,$(BUILDDIR))
	$(call fn_make_about_ini,$(APP_VERSION),$(APP_HOMEPAGE),$(APP_YEAR))
	$(HELP_COPY_CMD)
	$(LUPDATE) src/app/application.pro
	$(LRELEASE) src/app/application.pro
	$(QMAKE) $(QMAKEOPTS) $(QTARCHS) src/app/application.pro -o $(BUILDDIR)/Makefile.qmake
	@cd $(BUILDDIR) && $(MAKE) -f Makefile.qmake
	@echo "application executable built successfully"

.PHONY: run
run: $(APPBUILD)
	$(RUNCMD)
	@echo "done"

# WINDOWS SPECIFIC TARGETS post build steps
.PHONY: app_win
app_win: app_build
	$(call fn_mkdir,$(DISTDIR))
	$(call fn_mkdir,$(DISTDIR)\\$(APPNAME))
	copy $(BUILDDIR)\\release\\application.exe $(DISTDIR)\\$(APPNAME)\\$(APPNAME).exe
	copy $(LIBDITHERPATH)$(LIBNAME).dll $(DISTDIR)\\$(APPNAME)\\$(LIBNAME).dll
	$(DEPLOYQT) $(DISTDIR)\\$(APPNAME)\\$(APPNAME).exe

.PHONY: installer_win
installer_win:
	set APPNAME=$(APPNAME)&&set APPNAME_CS=$(APPNAME_CS)&&"$(WIN_NSIS_EXE)" src/win_installer/installscript.nsi
	@move src\\win_installer\\setup_out.exe $(DISTDIR)\\$(INSTALLERNAME)

# DEBIAN SPECIFIC TARGETS post build steps
.PHONY: app_deb
app_deb: app_build
	$(call fn_mkdir,$(DISTDIR)/$(APPNAME))
	cp $(BUILDDIR)/application $(DISTDIR)/$(APPNAME)/$(APPNAME)
	cp $(LIBDITHERPATH)/$(LIBNAME).so $(DISTDIR)/$(APPNAME)

.PHONY: installer_deb
installer_deb:
	$(call fn_mkdir,$(DISTBUILDDIR)/$(DEBNAME)/usr/local/bin)
	cp -R $(DISTDIR)/$(APPNAME) $(DISTBUILDDIR)/$(DEBNAME)/usr/local
	# copy and prepare debian specific control folder
	cp -R src/deb_installer/DEBIAN $(DISTBUILDDIR)/$(DEBNAME)
	sed -i 's/{{APPNAME}}/$(APPNAME)/g' $(DISTBUILDDIR)/$(DEBNAME)/DEBIAN/control
	sed -i 's/{{ARCH}}/$(ARCH)/g' $(DISTBUILDDIR)/$(DEBNAME)/DEBIAN/control
	sed -i 's/{{APP_VERSION}}/$(APP_VERSION)/g' $(DISTBUILDDIR)/$(DEBNAME)/DEBIAN/control
	sed -i 's,{{APP_HOMEPAGE}},$(APP_HOMEPAGE),g' $(DISTBUILDDIR)/$(DEBNAME)/DEBIAN/control
	sed -i 's/{{APPNAME}}/$(APPNAME)/g' $(DISTBUILDDIR)/$(DEBNAME)/DEBIAN/postinst
	sed -i 's/{{APPNAME_CS}}/$(APPNAME_CS)/g' $(DISTBUILDDIR)/$(DEBNAME)/DEBIAN/postinst
	sed -i 's/{{APPNAME_CS}}/$(APPNAME_CS)/g' $(DISTBUILDDIR)/$(DEBNAME)/DEBIAN/postrm
	sed -i 's/{{APPNAME_CS}}/$(APPNAME_CS)/g' $(DISTBUILDDIR)/$(DEBNAME)/DEBIAN/copyright
	sed -i 's,{{APP_HOMEPAGE}},$(APP_HOMEPAGE),g' $(DISTBUILDDIR)/$(DEBNAME)/DEBIAN/copyright
	sed -i 's,{{APP_YEAR}},$(APP_YEAR),g' $(DISTBUILDDIR)/$(DEBNAME)/DEBIAN/copyright
	chmod 775 $(DISTBUILDDIR)/$(DEBNAME)/DEBIAN/postrm
	chmod 775 $(DISTBUILDDIR)/$(DEBNAME)/DEBIAN/postinst
	# copy app and launcher script
	cp src/app/resources/appicon.png $(DISTBUILDDIR)/$(DEBNAME)/usr/local/$(APPNAME)
	cp src/deb_installer/launcher.sh $(DISTBUILDDIR)/$(DEBNAME)/usr/local/bin/$(APPNAME)
	sed -i 's/{{APPNAME}}/$(APPNAME)/g' $(DISTBUILDDIR)/$(DEBNAME)/usr/local/bin/$(APPNAME)
	chmod +x $(DISTBUILDDIR)/$(DEBNAME)/usr/local/bin/$(APPNAME)
	cp src/deb_installer/shortcut.desktop $(DISTBUILDDIR)/$(DEBNAME)/usr/local/$(APPNAME)/$(APPNAME_CS).desktop
	sed -i 's/{{APPNAME}}/$(APPNAME)/g' $(DISTBUILDDIR)/$(DEBNAME)/usr/local/$(APPNAME)/$(APPNAME_CS).desktop
	sed -i 's/{{APPNAME_CS}}/$(APPNAME_CS)/g' $(DISTBUILDDIR)/$(DEBNAME)/usr/local/$(APPNAME)/$(APPNAME_CS).desktop
	# list package dependencies
	echo "*** Package dependencies for $(APPNAME):"
	cp -R $(DISTBUILDDIR)/$(DEBNAME)/DEBIAN $(DISTBUILDDIR)/$(DEBNAME)/debian
	cd $(DISTBUILDDIR)/$(DEBNAME) && dpkg-shlibdeps -O --ignore-missing-info ./usr/local/$(APPNAME)/$(APPNAME)
	# build final .deb package
	dpkg-deb --build $(DISTBUILDDIR)/$(DEBNAME)
	cp $(DISTBUILDDIR)/$(DEBNAME).deb $(DISTDIR)
	# create a tarball
	cd $(DISTDIR) && tar -czvf $(DEBNAME).tar.gz $(APPNAME)

.PHONY: appimage
appimage:
	$(call fn_mkdir,$(DISTBUILDDIR)/appimage/usr/local/$(APPNAME))
	cp $(DISTDIR)/$(APPNAME)/$(APPNAME) $(DISTBUILDDIR)/appimage
	cp $(DISTDIR)/$(APPNAME)/libdither.so $(DISTBUILDDIR)/appimage
	cp src/deb_installer/shortcut.desktop $(DISTBUILDDIR)/appimage/$(APPNAME_CS).desktop
	sed -i 's/{{APPNAME}}/$(APPNAME)/g' $(DISTBUILDDIR)/appimage/$(APPNAME_CS).desktop
	sed -i 's/{{APPNAME_CS}}/$(APPNAME_CS)/g' $(DISTBUILDDIR)/appimage/$(APPNAME_CS).desktop
	cp src/app/resources/appicon.png $(DISTBUILDDIR)/appimage/usr/local/$(APPNAME)/appicon.png.png
	$(DEPLOYQT) $(DISTBUILDDIR)/appimage/$(APPNAME) -verbose=2 -always-overwrite -bundle-non-qt-libs -appimage -no-strip
	mv *.AppImage $(DISTDIR)

.PHONY: installer_tgz_full
installer_tgz_full:
	$(call fn_mkdir,$(DISTBUILDDIR)/full_tgz/$(APPNAME))
	cp $(DISTDIR)/$(APPNAME)/$(APPNAME) $(DISTBUILDDIR)/full_tgz/$(APPNAME)
	cp $(DISTDIR)/$(APPNAME)/libdither.so $(DISTBUILDDIR)/full_tgz/$(APPNAME)
	$(DEPLOYQT) $(DISTBUILDDIR)/full_tgz/$(APPNAME)/$(APPNAME) -verbose=2 -always-overwrite -bundle-non-qt-libs -no-strip
	cd $(DISTBUILDDIR)/full_tgz && tar -czvf $(DEBNAME)_full.tar.gz $(APPNAME)
	cp $(DISTBUILDDIR)/full_tgz/$(DEBNAME)_full.tar.gz $(DISTDIR)

# MAC SPECIFIC TARGETS post build steps
.PHONY: app_mac
app_mac: app_build
	-rm -Rf $(BUILDDIR)/$(APPNAME_CS).app
	mv $(BUILDDIR)/application.app $(BUILDDIR)/$(APPNAME_CS).app
	cp $(LIBDITHERPATH)$(LIBNAME).dylib $(BUILDDIR)/$(APPNAME_CS).app/Contents/MacOS
	# adjust library search path
	cd $(BUILDDIR) && install_name_tool -change $(LIBNAME).dylib @executable_path/$(LIBNAME).dylib $(APPNAME_CS).app/Contents/MacOS/application
	# add Qt libraries
	$(DEPLOYQT) $(BUILDDIR)/$(APPNAME_CS).app
	# add application icon
	cp src/mac_build/appicon.icns $(BUILDDIR)/$(APPNAME_CS).app/Contents/Resources
	cp src/mac_build/info.plist $(BUILDDIR)/$(APPNAME_CS).app/Contents
	sed -i '' -e 's/{{APPNAME_CS}}/$(APPNAME_CS)/g' $(BUILDDIR)/$(APPNAME_CS).app/Contents/info.plist
	sed -i '' -e 's/{{APPNAME}}/$(APPNAME)/g' $(BUILDDIR)/$(APPNAME_CS).app/Contents/info.plist
	-rm -Rf $(DISTDIR)
	$(call fn_mkdir,$(DISTDIR))
	cp -R $(BUILDDIR)/$(APPNAME_CS).app $(DISTDIR)

.PHONY: installer_mac
installer_mac:
	$(call fn_mkdir,$(DISTBUILDDIR))
	-rm -f $(DISTBUILDDIR)/$(APPNAME)_temp.dmg
	-rm -f $(DISTDIR)/$(DMGNAME).dmg
	-rm -Rf $(DISTBUILDDIR)/$(APPNAME)_temp_dmg
	-diskutil eject $(APPNAME_CS)
	$(call fn_mkdir,$(DISTBUILDDIR)/$(APPNAME)_temp_dmg)
	cp -R $(DISTDIR)/$(APPNAME_CS).app $(DISTBUILDDIR)/$(APPNAME)_temp_dmg
	# copy .dmg background image
	$(call fn_mkdir,$(DISTBUILDDIR)/$(APPNAME)_temp_dmg/.background)
	tiffutil -cathidpicheck src/mac_installer/background.png src/mac_installer/background@2x.png -out $(DISTBUILDDIR)/$(APPNAME)_temp_dmg/.background/background.tiff
	# create temporary installer dmg
	hdiutil create $(DISTBUILDDIR)/$(APPNAME)_temp.dmg -ov -format UDRW -volname "$(APPNAME_CS)" -fs HFS+ -srcfolder "$(DISTBUILDDIR)/$(APPNAME)_temp_dmg"
	# mount temporary installer dmg and run preparation script
	hdiutil attach "$(DISTBUILDDIR)/$(APPNAME)_temp.dmg"
	ls /Volumes/$(APPNAME_CS) | grep "$(APPNAME_CS).app"  # check if volume has been mounted successfully
	cp src/mac_installer/prepare.applescript $(DISTBUILDDIR)
	sed -i '' -e 's/{{APPNAME_CS}}/$(APPNAME_CS)/g' $(DISTBUILDDIR)/prepare.applescript
	osascript $(DISTBUILDDIR)/prepare.applescript
	-rm -f $(DISTBUILDDIR)/prepare.applescript
	chmod -Rf go-w /Volumes/$(APPNAME_CS)
	sync
	# unmount temporary installer dmg and convert to final read-only installer dmg
	hdiutil detach /Volumes/$(APPNAME_CS)
	hdiutil convert $(DISTBUILDDIR)/$(APPNAME)_temp.dmg -format UDZO -o $(DISTDIR)/$(DMGNAME).dmg

.PHONY: clean
clean:
	-@$(DELTREE) $(DISTDIR)
	-@$(DELTREE) $(BUILDDIR)
	-@$(DELTREE) $(DISTBUILDDIR)
	-@$(DELTREE) appimage
	-@$(DELTREE) $(HELP_PATH)
	-@$(DEL) $(ABOUT_INI_PATH)
