# Microsoft Developer Studio Project File - Name="LOG_SVR" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=LOG_SVR - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "LOG_SVR.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "LOG_SVR.mak" CFG="LOG_SVR - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "LOG_SVR - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "LOG_SVR - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/LOG_SVR", CGCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "LOG_SVR - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /O2 /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "_SAVE_VIEW_" /D "_TRACE_" /D "_OLE_DB_ERROR_TRACE_" /D "_RELEASE_" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 Kernel32.lib ws2_32.lib winmm.lib ole32.lib Iphlpapi.lib /nologo /subsystem:console /machine:I386
# SUBTRACT LINK32 /incremental:yes /debug

!ELSEIF  "$(CFG)" == "LOG_SVR - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "_SAVE_VIEW_" /D "_TRACE_" /D "_OLE_DB_ERROR_TRACE_" /Fr /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Kernel32.lib ws2_32.lib winmm.lib ole32.lib Iphlpapi.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "LOG_SVR - Win32 Release"
# Name "LOG_SVR - Win32 Debug"
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

SOURCE=.\Global\Global_Log_Define.h
# End Source File
# Begin Source File

SOURCE=.\Global\Global_Log_Packet.h
# End Source File
# Begin Source File

SOURCE=.\Global\Global_LogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Global\Global_LogFile.h
# End Source File
# Begin Source File

SOURCE=.\Global\Trace.h
# End Source File
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

SOURCE=".\OLE-DB\db_com_execute.h"
# End Source File
# Begin Source File

SOURCE=".\OLE-DB\db_command.h"
# End Source File
# Begin Source File

SOURCE=".\OLE-DB\db_Con.cpp"
# End Source File
# Begin Source File

SOURCE=".\OLE-DB\db_Con.h"
# End Source File
# Begin Source File

SOURCE=".\OLE-DB\sql_convert.h"
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
# Begin Group "Threads"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Threads\Backup_Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\Threads\Backup_Thread.h
# End Source File
# Begin Source File

SOURCE=.\Threads\PING_Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\Threads\TCP_Threads.cpp
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
# Begin Group "MyLibrary"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MyLibrary\MY_CRYPTO.H
# End Source File
# Begin Source File

SOURCE=.\MyLibrary\MY_OLE_DB.H
# End Source File
# Begin Source File

SOURCE=.\MyLibrary\MY_QUEUE.H
# End Source File
# Begin Source File

SOURCE=.\MyLibrary\MY_TRACE.H
# End Source File
# Begin Source File

SOURCE=.\MyLibrary\NDUtilClass.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Target
# End Project
