# Microsoft Developer Studio Project File - Name="ImgLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ImgLib - Win32 Debug UNICODE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ImgLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ImgLib.mak" CFG="ImgLib - Win32 Debug UNICODE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ImgLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ImgLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "ImgLib - Win32 Debug UNICODE" (based on "Win32 (x86) Static Library")
!MESSAGE "ImgLib - Win32 Release UNICODE" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Components/ImgLib", EPKAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ImgLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /O2 /I "..\ZLib" /I "..\\" /D "_LIB" /D "NDEBUG" /D "WIN32" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\bin\ImgLib.lib"

!ELSEIF  "$(CFG)" == "ImgLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /ZI /Od /I "..\ZLib" /I "..\\" /D "_LIB" /D "_MBCS" /D "WIN32" /D "_DEBUG" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\bin\ImgLibD.lib"

!ELSEIF  "$(CFG)" == "ImgLib - Win32 Debug UNICODE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ImgLib___Win32_Debug_UNICODE"
# PROP BASE Intermediate_Dir "ImgLib___Win32_Debug_UNICODE"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ImgLib___Win32_Debug_UNICODE"
# PROP Intermediate_Dir "ImgLib___Win32_Debug_UNICODE"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "..\ZLib" /I "..\\" /D "_LIB" /D "_MBCS" /D "WIN32" /D "_DEBUG" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /ZI /Od /I "..\ZLib" /I "..\\" /D "_DEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\bin\ImgLibD.lib"
# ADD LIB32 /nologo /out:"..\..\bin\ImgLibUD.lib"

!ELSEIF  "$(CFG)" == "ImgLib - Win32 Release UNICODE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ImgLib___Win32_Release_UNICODE"
# PROP BASE Intermediate_Dir "ImgLib___Win32_Release_UNICODE"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ImgLib___Win32_Release_UNICODE"
# PROP Intermediate_Dir "ImgLib___Win32_Release_UNICODE"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /O2 /I "..\ZLib" /I "..\\" /D "_LIB" /D "NDEBUG" /D "WIN32" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /O2 /I "..\ZLib" /I "..\\" /D "NDEBUG" /D "_LIB" /D "WIN32" /D "_MBCS" /D "UNICODE" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\bin\ImgLib.lib"
# ADD LIB32 /nologo /out:"..\..\bin\ImgLibU.lib"

!ENDIF 

# Begin Target

# Name "ImgLib - Win32 Release"
# Name "ImgLib - Win32 Debug"
# Name "ImgLib - Win32 Debug UNICODE"
# Name "ImgLib - Win32 Release UNICODE"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Decoders"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DecodeBMP.cpp
# End Source File
# Begin Source File

SOURCE=.\DecodeGIF.cpp
# End Source File
# Begin Source File

SOURCE=.\DecodeICO.cpp
# End Source File
# Begin Source File

SOURCE=.\DecodeJPEG.cpp
# End Source File
# Begin Source File

SOURCE=.\DecodeMNG.cpp
# End Source File
# Begin Source File

SOURCE=.\DecodePCX.cpp
# End Source File
# Begin Source File

SOURCE=.\DecodePNG.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\Config.h
# End Source File
# Begin Source File

SOURCE=.\DibLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\DibLoader.h
# End Source File
# Begin Source File

SOURCE=.\Frame.cpp
# End Source File
# Begin Source File

SOURCE=.\gif.h
# End Source File
# Begin Source File

SOURCE=.\Image.cpp
# End Source File
# Begin Source File

SOURCE=.\imglib.h
# End Source File
# Begin Source File

SOURCE=.\ImgLibInt.h
# End Source File
# Begin Source File

SOURCE=.\RIFFormat.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Array.h
# End Source File
# Begin Source File

SOURCE=..\Array.inl
# End Source File
# Begin Source File

SOURCE=.\CompatibleDC.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ImagLib.txt
# End Source File
# Begin Source File

SOURCE=.\licence.txt
# End Source File
# End Target
# End Project
