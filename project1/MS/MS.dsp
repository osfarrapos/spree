# Microsoft Developer Studio Project File - Name="MS" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MS - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MS.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MS.mak" CFG="MS - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MS - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MS - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "MS - Win32 Release_d" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/MS", NHCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MS - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_WIN32_DCOM" /D "_TRACE_" /D "_RELEASE_" /FR /YX /FD /D //_OR_MAIL" " /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 Kernel32.lib ws2_32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Iphlpapi.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "MS - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_CONSOLE" /D "_WIN32_DCOM" /D "_TRACE_" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Kernel32.lib ws2_32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib Iphlpapi.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "MS - Win32 Release_d"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MS___Win32_Release_d"
# PROP BASE Intermediate_Dir "MS___Win32_Release_d"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "MS___Win32_Release_d"
# PROP Intermediate_Dir "MS___Win32_Release_d"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MTd /W3 /GX /ZI /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_WIN32_DCOM" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 Kernel32.lib ws2_32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /incremental:yes /debug
# ADD LINK32 Kernel32.lib ws2_32.lib Iphlpapi.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "MS - Win32 Release"
# Name "MS - Win32 Debug"
# Name "MS - Win32 Release_d"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Network\Network.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\Network.h
# End Source File
# End Group
# Begin Group "VMManager"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\VMManager\VMManager.cpp
# End Source File
# Begin Source File

SOURCE=.\VMManager\VMManager.h
# End Source File
# End Group
# Begin Group "Global"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Global\Global_Define.h
# End Source File
# Begin Source File

SOURCE=.\Global\Global_Function.cpp
# End Source File
# Begin Source File

SOURCE=.\Global\Global_Function.h
# End Source File
# Begin Source File

SOURCE=.\Global\Global_Lock.cpp
# End Source File
# Begin Source File

SOURCE=.\Global\Global_Lock.h
# End Source File
# Begin Source File

SOURCE=.\Global\Global_LogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Global\Global_LogFile.h
# End Source File
# Begin Source File

SOURCE=.\Global\MSPacket_Msg.h
# End Source File
# Begin Source File

SOURCE=.\Global\Trace.h
# End Source File
# End Group
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Group
# Begin Group "Map"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Map\MAPManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Map\MAPManager.h
# End Source File
# End Group
# Begin Group "Queue"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Queue\Packet_Process.cpp
# End Source File
# Begin Source File

SOURCE=.\Queue\Packet_Queue.cpp
# End Source File
# Begin Source File

SOURCE=.\Queue\Packet_Queue.h
# End Source File
# End Group
# Begin Group "OLE-DB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\OLE-DB\Accessors.h"
# End Source File
# Begin Source File

SOURCE=".\OLE-DB\db_command.cpp"
# End Source File
# Begin Source File

SOURCE=".\OLE-DB\db_command.h"
# End Source File
# Begin Source File

SOURCE=".\OLE-DB\sql_convert.h"
# End Source File
# End Group
# Begin Group "Threads"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Threads\Bacup_Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\Threads\PING_Thread.cpp
# End Source File
# End Group
# Begin Group "lib_ND_Net"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lib_ND_Net\socket_pooling.cpp
# End Source File
# Begin Source File

SOURCE=.\lib_ND_Net\socket_pooling.h
# End Source File
# Begin Source File

SOURCE=.\lib_ND_Net\stdafx.h
# End Source File
# End Group
# Begin Group "lib_ND_Util"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\lib_ND_Util\ND_Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\lib_ND_Util\ND_Thread.h
# End Source File
# Begin Source File

SOURCE=.\lib_ND_Util\stdafx.h
# End Source File
# End Group
# Begin Group "User"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\User\User.cpp
# End Source File
# Begin Source File

SOURCE=.\User\User.h
# End Source File
# End Group
# Begin Group "MyLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MyLib\MY_CRYPTO.H
# End Source File
# Begin Source File

SOURCE=.\MyLib\MY_OLE_DB.H
# End Source File
# Begin Source File

SOURCE=.\MyLib\MY_QUEUE.H
# End Source File
# Begin Source File

SOURCE=.\MyLib\MY_TRACE.H
# End Source File
# Begin Source File

SOURCE=.\MyLib\NDUtilClass.h
# End Source File
# End Group
# End Target
# End Project
