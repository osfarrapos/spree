# Microsoft Developer Studio Project File - Name="XPatch" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=XPatch - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XPatch.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XPatch.mak" CFG="XPatch - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XPatch - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "XPatch - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "XPatch - Win32 vn_Release_Unicode" (based on "Win32 (x86) Application")
!MESSAGE "XPatch - Win32 tw_Release_Unicode" (based on "Win32 (x86) Application")
!MESSAGE "XPatch - Win32 us_Release_Unicode" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/EmperorOfDragons/XPatch", DYEBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XPatch - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 imagehlp.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /out:"..\Game\XPatch.exe"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XPatch - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 imagehlp.lib Zip/ZipD.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /out:"..\Game\XPatch.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XPatch - Win32 vn_Release_Unicode"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XPatch___Win32_vn_Release_Unicode"
# PROP BASE Intermediate_Dir "XPatch___Win32_vn_Release_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "XPatch___Win32_vn_Release_Unicode"
# PROP Intermediate_Dir "XPatch___Win32_vn_Release_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_XVIETNAMESE" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 imagehlp.lib Zip/Zip.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBC" /out:"..\Game\XPatch.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 UnicoWS.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib imagehlp.lib Version.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib LIBC.LIB" /out:"..\Game\XPatch.exe"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "XPatch - Win32 tw_Release_Unicode"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XPatch___Win32_tw_Release_Unicode"
# PROP BASE Intermediate_Dir "XPatch___Win32_tw_Release_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "XPatch___Win32_tw_Release_Unicode"
# PROP Intermediate_Dir "XPatch___Win32_tw_Release_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_XVIETNAMESE" /FR /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_XTAIWANESE" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 imagehlp.lib Version.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /out:"..\Game\XPatch.exe"
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 UnicoWS.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib imagehlp.lib Version.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib LIBC.LIB" /out:"..\Game\XPatch.exe"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "XPatch - Win32 us_Release_Unicode"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XPatch___Win32_us_Release_Unicode"
# PROP BASE Intermediate_Dir "XPatch___Win32_us_Release_Unicode"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "XPatch___Win32_us_Release_Unicode"
# PROP Intermediate_Dir "XPatch___Win32_us_Release_Unicode"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_XTAIWANESE" /FR /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "_XENGLISH" /D "_XACCLAIM" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 UnicoWS.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib imagehlp.lib Version.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib LIBC.LIB" /out:"..\Game\XPatch.exe"
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 UnicoWS.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib imagehlp.lib Version.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib LIBC.LIB" /out:"..\Game\XPatch.exe"
# SUBTRACT LINK32 /pdb:none /debug

!ENDIF 

# Begin Target

# Name "XPatch - Win32 Release"
# Name "XPatch - Win32 Debug"
# Name "XPatch - Win32 vn_Release_Unicode"
# Name "XPatch - Win32 tw_Release_Unicode"
# Name "XPatch - Win32 us_Release_Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\SeverMessageDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\XPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\XPatch.rc
# End Source File
# Begin Source File

SOURCE=.\XPatchDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SeverMessageDialog.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\XPatch.h
# End Source File
# Begin Source File

SOURCE=.\XPatchDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Border.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Close.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\M0Q1ILL5.BMP
# End Source File
# Begin Source File

SOURCE=.\res\Minimize.bmp
# End Source File
# Begin Source File

SOURCE=.\res\minimize.ico
# End Source File
# Begin Source File

SOURCE=.\res\Open.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Upgrade.bmp
# End Source File
# Begin Source File

SOURCE=.\res\us_Border.bmp
# End Source File
# Begin Source File

SOURCE=.\res\vn_Border.bmp
# End Source File
# Begin Source File

SOURCE=.\res\XPactchMainIcon.ico
# End Source File
# Begin Source File

SOURCE=.\res\XPatch.ico
# End Source File
# Begin Source File

SOURCE=.\res\XPatch.rc2
# End Source File
# End Group
# Begin Group "Registry"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RegStringDefine.h
# End Source File
# Begin Source File

SOURCE=.\XRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\XRegistry.h
# End Source File
# End Group
# Begin Group "Package"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Package.cpp
# End Source File
# Begin Source File

SOURCE=.\Package.h
# End Source File
# End Group
# Begin Group "MFC Engine"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BitmapDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapDialog.h
# End Source File
# Begin Source File

SOURCE=.\BtnST.cpp
# End Source File
# Begin Source File

SOURCE=.\BtnST.h
# End Source File
# Begin Source File

SOURCE=.\CeXDib.cpp
# End Source File
# Begin Source File

SOURCE=.\CeXDib.h
# End Source File
# Begin Source File

SOURCE=.\Dib.cpp
# End Source File
# Begin Source File

SOURCE=.\DIB.h
# End Source File
# Begin Source File

SOURCE=.\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\Label.h
# End Source File
# Begin Source File

SOURCE=.\ShadeButtonST.cpp
# End Source File
# Begin Source File

SOURCE=.\ShadeButtonST.h
# End Source File
# Begin Source File

SOURCE=.\xDib.cpp
# End Source File
# Begin Source File

SOURCE=.\xDib.h
# End Source File
# End Group
# Begin Group "Ziplib"

# PROP Default_Filter ""
# Begin Group "ZipCore"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Zip\adler32.c
# End Source File
# Begin Source File

SOURCE=.\Zip\compress.c
# End Source File
# Begin Source File

SOURCE=.\Zip\crc32.c
# End Source File
# Begin Source File

SOURCE=.\Zip\crc32.h
# End Source File
# Begin Source File

SOURCE=.\Zip\deflate.c
# End Source File
# Begin Source File

SOURCE=.\Zip\deflate.h
# End Source File
# Begin Source File

SOURCE=.\Zip\gzio.c
# End Source File
# Begin Source File

SOURCE=.\Zip\infback.c
# End Source File
# Begin Source File

SOURCE=.\Zip\inffast.c
# End Source File
# Begin Source File

SOURCE=.\Zip\inffast.h
# End Source File
# Begin Source File

SOURCE=.\Zip\inffixed.h
# End Source File
# Begin Source File

SOURCE=.\Zip\inflate.c
# End Source File
# Begin Source File

SOURCE=.\Zip\inflate.h
# End Source File
# Begin Source File

SOURCE=.\Zip\inftrees.c
# End Source File
# Begin Source File

SOURCE=.\Zip\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\Zip\trees.c
# End Source File
# Begin Source File

SOURCE=.\Zip\trees.h
# End Source File
# Begin Source File

SOURCE=.\Zip\uncompr.c
# End Source File
# Begin Source File

SOURCE=.\Zip\zconf.h
# End Source File
# Begin Source File

SOURCE=.\Zip\zconf.in.h
# End Source File
# Begin Source File

SOURCE=.\Zip\zlib.h
# End Source File
# Begin Source File

SOURCE=.\Zip\zutil.c
# End Source File
# Begin Source File

SOURCE=.\Zip\zutil.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Zip\AutoBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\Zip\AutoBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Zip\BigFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Zip\BigFile.h
# End Source File
# Begin Source File

SOURCE=.\Zip\CentralDir.cpp
# End Source File
# Begin Source File

SOURCE=.\Zip\CentralDir.h
# End Source File
# Begin Source File

SOURCE=.\Zip\FileHeader.cpp
# End Source File
# Begin Source File

SOURCE=.\Zip\FileHeader.h
# End Source File
# Begin Source File

SOURCE=.\Zip\ZipArchive.cpp
# End Source File
# Begin Source File

SOURCE=.\Zip\ZipArchive.h
# End Source File
# Begin Source File

SOURCE=.\Zip\ZipException.cpp
# End Source File
# Begin Source File

SOURCE=.\Zip\ZipException.h
# End Source File
# Begin Source File

SOURCE=.\Zip\ZipInternalInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Zip\ZipInternalInfo.h
# End Source File
# Begin Source File

SOURCE=.\Zip\ZipStorage.cpp
# End Source File
# Begin Source File

SOURCE=.\Zip\ZipStorage.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\Border.rgn
# End Source File
# Begin Source File

SOURCE=.\res\kr_interface.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\tw_interface.txt
# End Source File
# Begin Source File

SOURCE=.\res\us_interface.txt
# End Source File
# Begin Source File

SOURCE=.\res\vn_interface.txt
# End Source File
# End Target
# End Project
