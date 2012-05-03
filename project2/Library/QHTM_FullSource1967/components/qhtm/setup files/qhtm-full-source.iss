[Setup]
AppName=QHTM
AppVerName=QHTM
AppCopyright=Copyright (C) Russ Freeman
AppPublisher=GipsySoft
AppPublisherURL=http://www.gipsysoft.com/
AppSupportURL=http://www.gipsysoft.com/
AppUpdatesURL=http://www.gipsysoft.com/qhtm/
LicenseFile=Source-licence.txt
DefaultDirName={pf}\GipsySoft\Components

OutputBaseFilename = QHTM_FullSource1967

DisableProgramGroupPage=yes

; ^ since no icons will be created in "{group}", we don't need the wizard
;   to ask for a group name.
UninstallDisplayIcon={app}\QHTM.ico

[Files]
Source: "D:\project zip files\QHTM\components\*.*"; DestDir: "{app}"; Flags: recursesubdirs

