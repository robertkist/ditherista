!include "FileAssociation.nsh"
!include "NTProfiles.nsh"

Name $%APPNAME_CS%
OutFile "setup_out.exe"
Unicode True
Icon "..\..\src\app\resources\appicon.ico"
InstallDir $PROGRAMFILES64\$%APPNAME_CS%
InstallDirRegKey HKLM "Software\$%APPNAME_CS%" "Install_Dir"
RequestExecutionLevel admin ;Require admin rights on NT6+ (When UAC is turned on)

; Pages
Page directory
Page instfiles
UninstPage uninstConfirm
UninstPage instfiles

!include LogicLib.nsh

Function .onInit
  setShellVarContext all
FunctionEnd

Section "Install"
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  File /r ..\..\dist\$%APPNAME%\*.*

  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\$%APPNAME_CS% "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$%APPNAME_CS%" "DisplayName" "$%APPNAME_CS%"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$%APPNAME_CS%" "UninstallString" "$\"$INSTDIR\uninstaller.exe$\""
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$%APPNAME_CS%" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$%APPNAME_CS%" "NoRepair" 1
  WriteUninstaller "$INSTDIR\uninstaller.exe"
SectionEnd


; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"
  SetOutPath $INSTDIR
  CreateDirectory "$SMPROGRAMS\$%APPNAME_CS%"
  CreateShortcut "$SMPROGRAMS\$%APPNAME_CS%\Uninstall $%APPNAME_CS%.lnk" "$INSTDIR\uninstaller.exe" "" "$INSTDIR\uninstaller.exe" 0
  CreateShortcut "$SMPROGRAMS\$%APPNAME_CS%\$%APPNAME_CS%.lnk" "$INSTDIR\$%APPNAME%.exe" "" "$INSTDIR\$%APPNAME%.exe" 0
  CreateShortcut "$DESKTOP\$%APPNAME_CS%.lnk" "$INSTDIR\$%APPNAME%.exe" "" "$INSTDIR\$%APPNAME%.exe" 0
SectionEnd


; Uninstaller
Section "Uninstall"
  setShellVarContext all
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\$%APPNAME_CS%"
  DeleteRegKey HKLM SOFTWARE\$%APPNAME_CS%
  DeleteRegKey HKCU SOFTWARE\$%APPNAME_CS%

  ; Remove files and uninstaller
  ;RMDir /r "$INSTDIR\lib"
  ;Delete "$INSTDIR\lib"
  ;Delete "$INSTDIR\*.*"
  ;Delete "$INSTDIR\*"
  Delete $INSTDIR\uninstaller.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\$%APPNAME_CS%\*.*"
  Delete "$DESKTOP\$%APPNAME_CS%.lnk"

  ; Remove directories used
  RMDir /r /REBOOTOK "$SMPROGRAMS\$%APPNAME_CS%"
  RMDir /r /REBOOTOK $INSTDIR
SectionEnd
