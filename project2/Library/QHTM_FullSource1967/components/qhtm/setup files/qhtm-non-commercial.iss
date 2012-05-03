[Setup]
AppName=QHTM Non Commercial
AppVerName=QHTM Non Commercial
AppCopyright=Copyright (C) Russ Freeman
AppPublisher=GipsySoft
AppPublisherURL=http://www.gipsysoft.com/
AppSupportURL=http://www.gipsysoft.com/
AppUpdatesURL=http://www.gipsysoft.com/qhtm/

DefaultDirName={pf}\GipsySoft\Components\

OutputBaseFilename = QHTM_non_commercial

LicenseFile=non-commercial-licence.rtf

DisableProgramGroupPage=yes

; ^ since no icons will be created in "{group}", we don't need the wizard
;   to ask for a group name.
UninstallDisplayIcon={app}\QHTM.ico

[Files]
Source: "\gs-projects\bin\qhtm.lib"; DestDir: "{app}"
Source: "\gs-projects\bin\qhtm.DLL"; DestDir: "{app}"
Source: "\gs-projects\components\qhtm\qhtm.h"; DestDir: "{app}"
Source: "\gs-projects\components\qhtm\history.txt"; DestDir: "{app}"
