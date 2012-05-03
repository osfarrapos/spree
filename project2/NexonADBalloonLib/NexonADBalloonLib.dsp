# Microsoft Developer Studio Project File - Name="NexonADBalloonLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=NexonADBalloonLib - Win32 KR_Release_Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NexonADBalloonLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NexonADBalloonLib.mak" CFG="NexonADBalloonLib - Win32 KR_Release_Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NexonADBalloonLib - Win32 KR_Release_Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "NexonADBalloonLib - Win32 KR_Release" (based on "Win32 (x86) Static Library")
!MESSAGE "NexonADBalloonLib - Win32 KR_Release_GM" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/EmperorOfDragons/NexonADBalloonLib", GCAFAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NexonADBalloonLib - Win32 KR_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "NexonADBalloonLib___Win32_KR_Release_Debug"
# PROP BASE Intermediate_Dir "NexonADBalloonLib___Win32_KR_Release_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Compiled/KR/release_debug/NexonADBalloonLib"
# PROP Intermediate_Dir "../Compiled/KR/release_debug/NexonADBalloonLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../Library/KR/NexonADBalloonLib_D.lib"
# ADD LIB32 /nologo /out:"../Library/KR/NexonADBalloonLib_RD.lib"

!ELSEIF  "$(CFG)" == "NexonADBalloonLib - Win32 KR_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "NexonADBalloonLib___Win32_KR_Release"
# PROP BASE Intermediate_Dir "NexonADBalloonLib___Win32_KR_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Compiled/KR/release/NexonADBalloonLib"
# PROP Intermediate_Dir "../Compiled/KR/release/NexonADBalloonLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../Library/KR/NexonADBalloonLib.lib"
# ADD LIB32 /nologo /out:"../Library/KR/NexonADBalloonLib.lib"

!ELSEIF  "$(CFG)" == "NexonADBalloonLib - Win32 KR_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "NexonADBalloonLib___Win32_KR_Release_GM"
# PROP BASE Intermediate_Dir "NexonADBalloonLib___Win32_KR_Release_GM"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Compiled/KR/release_gm/NexonADBalloonLib"
# PROP Intermediate_Dir "../Compiled/KR/release_gm/NexonADBalloonLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"../Library/KR/NexonADBalloonLib.lib"
# ADD LIB32 /nologo /out:"../Library/KR/NexonADBalloonLib_GM.lib"

!ENDIF 

# Begin Target

# Name "NexonADBalloonLib - Win32 KR_Release_Debug"
# Name "NexonADBalloonLib - Win32 KR_Release"
# Name "NexonADBalloonLib - Win32 KR_Release_GM"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ADBalloon.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ADBalloon.h
# End Source File
# End Group
# End Target
# End Project
