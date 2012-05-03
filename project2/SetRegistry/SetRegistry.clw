; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAddServerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "setregistry.h"
LastPage=0

ClassCount=4
Class1=CAddServerDlg
Class2=CSetRegistryApp
Class3=CAboutDlg
Class4=CSetRegistryDlg

ResourceCount=3
Resource1=IDD_SETREGISTRY_DIALOG
Resource2=IDD_ABOUTBOX
Resource3=IDD_ADDSERVERITEM

[CLS:CAddServerDlg]
Type=0
BaseClass=CDialog
HeaderFile=AddServerDlg.h
ImplementationFile=AddServerDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_DOWNLOADADRESSEDIT

[CLS:CSetRegistryApp]
Type=0
BaseClass=CWinApp
HeaderFile=SetRegistry.h
ImplementationFile=SetRegistry.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=SetRegistryDlg.cpp
ImplementationFile=SetRegistryDlg.cpp

[CLS:CSetRegistryDlg]
Type=0
BaseClass=CDialog
HeaderFile=SetRegistryDlg.h
ImplementationFile=SetRegistryDlg.cpp
LastObject=IDC_TESTSERVERCHECK
Filter=D
VirtualFilter=dWC

[DLG:IDD_ADDSERVERITEM]
Type=1
Class=CAddServerDlg
ControlCount=10
Control1=IDC_SERVERNAMEEDIT,edit,1350631552
Control2=IDC_SERVERADDRESSEDIT,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242817
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_PATCHSERVERADDRESSEDIT,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_DOWNLOADADRESSEDIT,edit,1350631552
Control10=IDC_STATIC,static,1342308352

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SETREGISTRY_DIALOG]
Type=1
Class=CSetRegistryDlg
ControlCount=8
Control1=IDC_SERVERLIST,SysListView32,1350631437
Control2=IDADDSERVERADDRESS,button,1342242816
Control3=IDDELETESERVERADDRESS,button,1342242816
Control4=IDOK,button,1342242816
Control5=IDCANCEL,button,1342242817
Control6=IDMODIFYSERVERADDRESS,button,1342242816
Control7=IDMANAGERWINDOW,button,1476460544
Control8=IDC_TESTSERVERCHECK,button,1342242819

