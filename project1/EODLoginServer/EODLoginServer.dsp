# Microsoft Developer Studio Project File - Name="EODLoginServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=EODLoginServer - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EODLoginServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EODLoginServer.mak" CFG="EODLoginServer - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EODLoginServer - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "EODLoginServer - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "EODLoginServer - Win32 METEL" (based on "Win32 (x86) Console Application")
!MESSAGE "EODLoginServer - Win32 ACCLAIM" (based on "Win32 (x86) Console Application")
!MESSAGE "EODLoginServer - Win32 Vina" (based on "Win32 (x86) Console Application")
!MESSAGE "EODLoginServer - Win32 PWC" (based on "Win32 (x86) Console Application")
!MESSAGE "EODLoginServer - Win32 JAPAN" (based on "Win32 (x86) Console Application")
!MESSAGE "EODLoginServer - Win32 RUSSIA" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/EODLoginServer", YDCAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EODLoginServer - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "_RELEASE" /D "_PD_COMPANY_NEXON_" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 winmm.lib ws2_32.lib Iphlpapi.lib odbc32.lib /nologo /subsystem:console /machine:I386 /out:"ExeFiles/NDLogin_R.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "EODLoginServer - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib ws2_32.lib Iphlpapi.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"ExeFiles/NDLogin_D.exe" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "EODLoginServer - Win32 METEL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EODLoginServer___Win32_METEL"
# PROP BASE Intermediate_Dir "EODLoginServer___Win32_METEL"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EODLoginServer___Win32_METEL"
# PROP Intermediate_Dir "EODLoginServer___Win32_METEL"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "_RELEASE" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "_RELEASE" /D "_PD_COMPANY_METEL_" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib ws2_32.lib Iphlpapi.lib /nologo /subsystem:console /machine:I386 /out:"ExeFiles/NDLogin_R.exe"
# ADD LINK32 winmm.lib ws2_32.lib Iphlpapi.lib odbc32.lib /nologo /subsystem:console /incremental:yes /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"ExeFiles/NDLogin_METEL.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "EODLoginServer - Win32 ACCLAIM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EODLoginServer___Win32_ACCLAIM"
# PROP BASE Intermediate_Dir "EODLoginServer___Win32_ACCLAIM"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EODLoginServer___Win32_ACCLAIM"
# PROP Intermediate_Dir "EODLoginServer___Win32_ACCLAIM"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "_RELEASE" /D "_PD_COMPANY_METEL_" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "_RELEASE" /D "_PD_COMPANY_ACCLAIM_" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib ws2_32.lib Iphlpapi.lib /nologo /subsystem:console /machine:I386 /out:"ExeFiles/NDLogin_METEL.exe"
# ADD LINK32 winmm.lib ws2_32.lib Iphlpapi.lib odbc32.lib /nologo /subsystem:console /machine:I386 /out:"ExeFiles/NDLogin_Acclaim.exe"

!ELSEIF  "$(CFG)" == "EODLoginServer - Win32 Vina"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EODLoginServer___Win32_Vina"
# PROP BASE Intermediate_Dir "EODLoginServer___Win32_Vina"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EODLoginServer___Win32_Vina"
# PROP Intermediate_Dir "EODLoginServer___Win32_Vina"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "_RELEASE" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "_RELEASE" /D "_PD_COMPANY_VINA_" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib ws2_32.lib Iphlpapi.lib odbc32.lib /nologo /subsystem:console /machine:I386 /out:"ExeFiles/NDLogin_R.exe"
# ADD LINK32 winmm.lib ws2_32.lib Iphlpapi.lib odbc32.lib /nologo /subsystem:console /machine:I386 /out:"ExeFiles/NDLogin_Vina.exe"

!ELSEIF  "$(CFG)" == "EODLoginServer - Win32 PWC"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EODLoginServer___Win32_PWC"
# PROP BASE Intermediate_Dir "EODLoginServer___Win32_PWC"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EODLoginServer___Win32_PWC"
# PROP Intermediate_Dir "EODLoginServer___Win32_PWC"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "_RELEASE" /D "_PD_COMPANY_ACCLAIM_" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "_RELEASE" /D "_PD_COMPANY_PWC_" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib ws2_32.lib Iphlpapi.lib odbc32.lib /nologo /subsystem:console /machine:I386 /out:"ExeFiles/NDLogin_Acclaim.exe"
# ADD LINK32 winmm.lib ws2_32.lib Iphlpapi.lib odbc32.lib /nologo /subsystem:console /machine:I386 /out:"ExeFiles/NDLogin_PWC.exe"

!ELSEIF  "$(CFG)" == "EODLoginServer - Win32 JAPAN"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EODLoginServer___Win32_JAPAN"
# PROP BASE Intermediate_Dir "EODLoginServer___Win32_JAPAN"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EODLoginServer___Win32_JAPAN"
# PROP Intermediate_Dir "EODLoginServer___Win32_JAPAN"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "_RELEASE" /D "_PD_COMPANY_ACCLAIM_" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "_RELEASE" /D "_PD_COMPANY_JAPAN_" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib ws2_32.lib Iphlpapi.lib odbc32.lib /nologo /subsystem:console /machine:I386 /out:"ExeFiles/NDLogin_Acclaim.exe"
# ADD LINK32 ND_MD5R.lib winmm.lib ws2_32.lib Iphlpapi.lib odbc32.lib /nologo /subsystem:console /machine:I386 /out:"ExeFiles/NDLogin_RocWorks.exe"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "EODLoginServer - Win32 RUSSIA"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "EODLoginServer___Win32_RUSSIA"
# PROP BASE Intermediate_Dir "EODLoginServer___Win32_RUSSIA"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "EODLoginServer___Win32_RUSSIA"
# PROP Intermediate_Dir "EODLoginServer___Win32_RUSSIA"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "_RELEASE" /D "_PD_COMPANY_ACCLAIM_" /FR /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_WIN32_DCOM" /D "_RELEASE" /D "_PD_COMPANY_RUSSIA_" /FR /YX /FD /c
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib ws2_32.lib Iphlpapi.lib odbc32.lib /nologo /subsystem:console /machine:I386 /out:"ExeFiles/NDLogin_Acclaim.exe"
# ADD LINK32 winmm.lib ws2_32.lib Iphlpapi.lib odbc32.lib /nologo /subsystem:console /machine:I386 /out:"ExeFiles/NDLogin_Russia.exe"

!ENDIF 

# Begin Target

# Name "EODLoginServer - Win32 Release"
# Name "EODLoginServer - Win32 Debug"
# Name "EODLoginServer - Win32 METEL"
# Name "EODLoginServer - Win32 ACCLAIM"
# Name "EODLoginServer - Win32 Vina"
# Name "EODLoginServer - Win32 PWC"
# Name "EODLoginServer - Win32 JAPAN"
# Name "EODLoginServer - Win32 RUSSIA"
# Begin Group "Global"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Global\ComUdp_packets.h
# End Source File
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

SOURCE=.\Global\IP_Filter.cpp
# End Source File
# Begin Source File

SOURCE=.\Global\IP_Filter.h
# End Source File
# Begin Source File

SOURCE=.\Global\Login_packets.h
# End Source File
# Begin Source File

SOURCE=.\Global\NDTrace.h
# End Source File
# Begin Source File

SOURCE=.\Global\SystemMessage_ref.h
# End Source File
# End Group
# Begin Group "Environment"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Environment\Environment_Read.cpp
# End Source File
# Begin Source File

SOURCE=.\Environment\Environment_Read.h
# End Source File
# End Group
# Begin Group "OLE-DB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Ole-DB\_prLS_OpenBeta_Login.h"
# End Source File
# Begin Source File

SOURCE=".\Ole-DB\db_command.cpp"
# End Source File
# Begin Source File

SOURCE=".\Ole-DB\db_command.h"
# End Source File
# Begin Source File

SOURCE=".\Ole-DB\sql_convert.h"
# End Source File
# End Group
# Begin Group "Queue"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Packet_Queue\Packet_Queue.cpp
# End Source File
# Begin Source File

SOURCE=.\Packet_Queue\Packet_Queue.h
# End Source File
# End Group
# Begin Group "UDPThread"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\UDP_Thread\PING_Thread.cpp
# End Source File
# Begin Source File

SOURCE=.\UDP_Thread\UDP_Threads.cpp
# End Source File
# End Group
# Begin Group "TCPThread"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\TCP_Thread\TCP_Thread.cpp
# End Source File
# End Group
# Begin Group "Timer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Timer\TimeOut.cpp
# End Source File
# Begin Source File

SOURCE=.\Timer\TimeOut.h
# End Source File
# End Group
# Begin Group "MyServer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\myServer\CriticalSection.h
# End Source File
# Begin Source File

SOURCE=.\myServer\FileLog.h
# End Source File
# Begin Source File

SOURCE=.\myServer\gDefine.h
# End Source File
# Begin Source File

SOURCE=.\myServer\gGlobal.h
# End Source File
# Begin Source File

SOURCE=.\myServer\MY_CRYPTO.H
# End Source File
# Begin Source File

SOURCE=.\myServer\MY_FILE.H
# End Source File
# Begin Source File

SOURCE=.\myServer\MY_OLE_DB.H
# End Source File
# Begin Source File

SOURCE=.\myServer\MY_QUEUE.H
# End Source File
# Begin Source File

SOURCE=.\myServer\MY_TRACE.H
# End Source File
# Begin Source File

SOURCE=.\myServer\myServer.cpp
# End Source File
# Begin Source File

SOURCE=.\myServer\myServer.h
# End Source File
# Begin Source File

SOURCE=.\myServer\ND_MD5Dll.h
# End Source File
# Begin Source File

SOURCE=.\myServer\NDUtilClass.h
# End Source File
# Begin Source File

SOURCE=.\myServer\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\myServer\StdAfx.h
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

SOURCE=.\lib_ND_Util\NDQueue.cpp
# End Source File
# Begin Source File

SOURCE=.\lib_ND_Util\NDQueue.h
# End Source File
# Begin Source File

SOURCE=.\lib_ND_Util\stdafx.h
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
# Begin Group "Billing"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Billing\Billing_Process.cpp
# End Source File
# Begin Source File

SOURCE=.\Billing\BillingSvrLink.cpp
# End Source File
# Begin Source File

SOURCE=.\Billing\BillingSvrLink.h
# End Source File
# Begin Source File

SOURCE=.\Billing\GameAPI.h
# End Source File
# Begin Source File

SOURCE=.\Billing\MetelBilling.cpp
# End Source File
# End Group
# Begin Group "Odbc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Odbc\ODBC_.cpp
# End Source File
# Begin Source File

SOURCE=.\Odbc\ODBC_.h
# End Source File
# End Group
# Begin Group "RocWorks"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RocWorks\RocAuthSession.cpp
# End Source File
# Begin Source File

SOURCE=.\RocWorks\RocAuthSession.h
# End Source File
# End Group
# Begin Source File

SOURCE="C:\Program Files\Microsoft Visual Studio\VC98\Include\ASSERT.H"
# End Source File
# Begin Source File

SOURCE=.\EODMain.cpp
# End Source File
# Begin Source File

SOURCE=.\mFileLog.dll
# End Source File
# Begin Source File

SOURCE=.\SmallNet\Sock.cpp
# End Source File
# Begin Source File

SOURCE=.\SmallNet\Sock.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\GameAPI.lib
# End Source File
# Begin Source File

SOURCE=.\mFileLog.lib
# End Source File
# End Target
# End Project
