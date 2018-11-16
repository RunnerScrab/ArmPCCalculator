!include MUI2.nsh

!define ICON_NAME "zalgear.ico"
!define MUI_ICON "..\ArmSkills\${ICON_NAME}"
!define MUI_PRODUCT "Arm PC Calculator"
!define MUI_UNICON "..\ArmSkills\${ICON_NAME}"

!define APP_NAME "Arm_PC_Calculator.exe"
!define DB_NAME "armskills.db"
!define APP_LOC "..\Release\${APP_NAME}"
!define DB_LOC "..\ArmSkills\${DB_NAME}"

!define RegKey "Software\ArmCharacterCalculator"
!define UninstRegKey "Software\Microsoft\Windows\CurrentVersion\Uninstall\${MUI_PRODUCT}"

!define Uninstaller "uninstall.exe"


Name "Arm PC Calculator Installer"
Outfile "ArmCalcInstall.exe"

InstallDir "$PROGRAMFILES\${MUI_PRODUCT}\"
InstallDirRegKey HKLM "${RegKey}" ""

!insertmacro MUI_PAGE_LICENSE "..\ArmSkills\license.txt"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"

Section "Arm PC Calculator" install
	SetOutPath "$INSTDIR"
	WriteRegStr HKCU "${RegKey}" "" $INSTDIR
	WriteRegStr HKCU "${UninstRegKey}" "DisplayName" "${MUI_PRODUCT}"
	WriteRegStr HKCU "${UninstRegKey}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
	WriteRegStr HKCU "${UninstRegKey}" "NoModify" 1
	WriteRegStr HKCU "${UninstRegKey}" "NoRepair" 1
	File ${APP_LOC}
	File ${DB_LOC}
	File ${MUI_ICON}
	WriteRegStr HKCU "${UninstRegKey}" "DisplayIcon" '"$INSTDIR\${ICON_NAME}"'
	
	SectionGetSize ${install} $0
	IntFmt $0 "0x%08X" $0
	
	WriteRegDWORD HKCU "${UninstRegKey}" "EstimatedSize" "$0"
	WriteUninstaller "$INSTDIR\Uninstall.exe"
SectionEnd

Section "Start Menu Shortcuts"
CreateDirectory "$SMPROGRAMS\${MUI_PRODUCT}"
CreateShortcut "$SMPROGRAMS\${MUI_PRODUCT}\${MUI_PRODUCT}.lnk" "$INSTDIR\${APP_NAME}" ""  "$INSTDIR\${APP_NAME}"  0
SectionEnd

Section "Uninstall"
	Delete "$INSTDIR\${APP_NAME}"
	Delete "$INSTDIR\${DB_NAME}"
	Delete "$INSTDIR\${ICON_NAME}"
	Delete "$INSTDIR\Uninstall.exe"
	Delete "$SMPROGRAMS\${MUI_PRODUCT}\*.*"
	RMDir "$SMPROGRAMS\${MUI_PRODUCT}"
	RMDir "$INSTDIR"
	DeleteRegKey HKCU "${RegKey}"
	DeleteRegKey HKCU "${UninstRegKey}"
SectionEnd