; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{A41B450C-8A95-369E-B9ED08BBF0D6}
AppName=filedisk_v1.0Build42
AppVerName= v1.0Build42
VersionInfoTextVersion=v1.0Build42
AppPublisher=
AppPublisherURL=
AppSupportURL=
AppUpdatesURL=
DefaultDirName={pf}/Richtech/filedisk
DefaultGroupName=filedisk
OutputDir=output_iss_package
;VersionInfoProductVersion=sf_win_explorer.exe
;VersionInfoVersion=2.3.SVN_BUILD_NO1
OutputBaseFilename=filedisk_Build42
SourceDir=     .
Compression=lzma
SolidCompression=yes
AlwaysRestart=no
UninstallRestartComputer=no


[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "filedisk\*.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "filedisk\*.pdb"; DestDir: "{app}"; Flags: ignoreversion

Source: "sys\src\objfre_wxp_x86\i386\*.sys"; DestDir: "{app}"; Flags: ignoreversion
Source: "sys\src\objfre_wxp_x86\i386\*.pdb"; DestDir: "{app}"; Flags: ignoreversion



Source: "版本说明.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "bug*.txt"; DestDir: "{app}"; Flags: ignoreversion



Source: "E:\svn_working_path\system_initialization_win\vc_2003_dll\*.dll"; DestDir: "C:\WINDOWS\"; Flags: ignoreversion


[Registry]
Root: HKLM; Subkey: "Software\Microsoft\Windows\CurrentVersion\Run"; ValueType: string; ValueName: ""; ValueData: """{app}\"""


[Icons]
Name: "{commondesktop}\filedisk"; Filename: "{app}\"; Tasks: desktopicon ;WorkingDir:"{app}";  Flags:closeonexit
Name: "{group}\filedisk"; Filename: "{app}\" ; WorkingDir:"{app}"; Flags:closeonexit
Name: "{group}\卸载"; Filename: "{uninstallexe}"

[Run]
Filename: "{app}\install_cli.bat"; StatusMsg: "安装服务";

[UninstallRun]
Filename: "{app}\uninstall_cli.bat"; StatusMsg: "卸载服务";


