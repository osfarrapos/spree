# Microsoft Developer Studio Project File - Name="XPacker" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=XPacker - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XPacker.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XPacker.mak" CFG="XPacker - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XPacker - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "XPacker - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/EmperorOfDragons/XPacker", RVEBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XPacker - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Imagehlp.lib shlwapi.lib Zip/Zip.lib winmm.lib dsound.lib dxguid.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBC"

!ELSEIF  "$(CFG)" == "XPacker - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Imagehlp.lib shlwapi.lib Zip/ZipD.lib dsound.lib winmm.lib dxguid.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "XPacker - Win32 Release"
# Name "XPacker - Win32 Debug"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cpt.ico
# End Source File
# Begin Source File

SOURCE=".\res\Eudora 3 black.ico"
# End Source File
# Begin Source File

SOURCE=.\res\folderopen.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=".\res\Simple Remix Hard Drive.ico"
# End Source File
# Begin Source File

SOURCE=.\res\XPacker.ico
# End Source File
# Begin Source File

SOURCE=.\res\XPacker.rc2
# End Source File
# End Group
# Begin Group "Window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\XPacker.cpp
# End Source File
# Begin Source File

SOURCE=.\XPacker.h
# End Source File
# Begin Source File

SOURCE=.\XPacker.rc
# End Source File
# Begin Source File

SOURCE=.\XPackerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XPackerDlg.h
# End Source File
# End Group
# Begin Group "Zip"

# PROP Default_Filter ""
# End Group
# Begin Group "Dialog"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DirDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\DirDialog.h
# End Source File
# Begin Source File

SOURCE=.\EditSoundListItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditSoundListItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\ModifySoundSceneItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ModifySoundSceneItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\MywndTab.cpp
# End Source File
# Begin Source File

SOURCE=.\MywndTab.h
# End Source File
# Begin Source File

SOURCE=.\NewPackageFileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewPackageFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewSectionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewSectionDlg.h
# End Source File
# Begin Source File

SOURCE=.\PackingView.cpp
# End Source File
# Begin Source File

SOURCE=.\PackingView.h
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressDlg.h
# End Source File
# Begin Source File

SOURCE=.\SetRootPathDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SetRootPathDlg.h
# End Source File
# Begin Source File

SOURCE=.\WavePackageView.cpp
# End Source File
# Begin Source File

SOURCE=.\WavePackageView.h
# End Source File
# Begin Source File

SOURCE=.\wndTab.cpp
# End Source File
# Begin Source File

SOURCE=.\wndTab.h
# End Source File
# Begin Source File

SOURCE=.\xShadeButton.cpp
# End Source File
# Begin Source File

SOURCE=.\xShadeButton.h
# End Source File
# Begin Source File

SOURCE=.\XTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\XTreeCtrl.h
# End Source File
# End Group
# Begin Group "PackageCore"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Package.cpp
# End Source File
# Begin Source File

SOURCE=.\Package.h
# End Source File
# Begin Source File

SOURCE=.\PackFileItem.cpp
# End Source File
# Begin Source File

SOURCE=.\PackFileItem.h
# End Source File
# Begin Source File

SOURCE=.\SectionList.cpp
# End Source File
# Begin Source File

SOURCE=.\SectionList.h
# End Source File
# Begin Source File

SOURCE=.\SndStructure.h
# End Source File
# Begin Source File

SOURCE=.\SndTableList.cpp
# End Source File
# Begin Source File

SOURCE=.\SndTableList.h
# End Source File
# Begin Source File

SOURCE=.\WavePackage.cpp
# End Source File
# Begin Source File

SOURCE=.\WavePackage.h
# End Source File
# Begin Source File

SOURCE=.\XDSound.cpp
# End Source File
# Begin Source File

SOURCE=.\XDSound.h
# End Source File
# Begin Source File

SOURCE=.\Xitem.cpp
# End Source File
# Begin Source File

SOURCE=.\Xitem.h
# End Source File
# Begin Source File

SOURCE=.\Xlist.cpp
# End Source File
# Begin Source File

SOURCE=.\Xlist.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\XPacker.reg
# End Source File
# End Target
# End Project
