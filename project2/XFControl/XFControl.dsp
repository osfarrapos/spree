# Microsoft Developer Studio Project File - Name="XFControl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=XFControl - Win32 TH_Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XFControl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XFControl.mak" CFG="XFControl - Win32 TH_Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XFControl - Win32 KR_Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 KR_Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 KR_Release_Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 KR_Release_GM" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 VN_Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 VN_Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 VN_Release_Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 VN_Release_GM" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 TW_Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 TW_Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 TW_Release_Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 TW_Release_GM" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 US_Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 US_Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 US_Release_Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 US_Release_GM" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 CN_Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 CN_Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 CN_Release_Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 CN_Release_GM" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 JP_Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 JP_Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 JP_Release_Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 JP_Release_GM" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 RS_Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 RS_Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 RS_Release_Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 RS_Release_GM" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 TH_Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 TH_Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 TH_Release_Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XFControl - Win32 TH_Release_GM" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/EmperorOfDragons/XFControl", PLLDAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XFControl - Win32 KR_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/KR"
# PROP BASE Intermediate_Dir "../Compiled/KR/release/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/KR"
# PROP Intermediate_Dir "../Compiled/KR/release/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/KR/XGameBase.Lib /nologo /dll /pdb:"../Compiled/KR/release/XFControl/XFControl.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/KR/XFControl.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 KR_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../Library/KR"
# PROP BASE Intermediate_Dir "../Compiled/KR/debug/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Library/KR"
# PROP Intermediate_Dir "../Compiled/KR/debug/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /FR /FD /GZ /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/KR/XGameBase_D.Lib /nologo /dll /map /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"../Game/KR/XFControl_D.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 KR_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/KR"
# PROP BASE Intermediate_Dir "../Compiled/KR/release_debug/XFControl"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/KR"
# PROP Intermediate_Dir "../Compiled/KR/release_debug/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /FR /YX /FD /Zm150 /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/KR/XGameBase_RD.Lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/KR/XGameBase_RD.Lib /nologo /dll /incremental:yes /map /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/KR/XFControl_RD.dll"

!ELSEIF  "$(CFG)" == "XFControl - Win32 KR_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/KR"
# PROP BASE Intermediate_Dir "../Compiled/KR/release_GM/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/KR"
# PROP Intermediate_Dir "../Compiled/KR/release_GM/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XGMCLIENT" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/KR/XGameBase_GM.Lib /nologo /dll /pdb:"../Compiled/KR/release_gm/XFControl/XFControl.pdb" /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/KR/XFControl_GM.dll"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "XFControl - Win32 VN_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/VN"
# PROP BASE Intermediate_Dir "../Compiled/VN/release/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/VN"
# PROP Intermediate_Dir "../Compiled/VN/release/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XVIETNAMESE" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/VN/XGameBase.Lib /nologo /dll /pdb:"../Compiled/VN/release/XFControl/XFControl.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/VN/XFControl.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 VN_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../Library/VN"
# PROP BASE Intermediate_Dir "../Compiled/VN/debug/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Library/VN"
# PROP Intermediate_Dir "../Compiled/VN/debug/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XVIETNAMESE" /FR /FD /GZ /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/VN/XGameBase_D.Lib /nologo /dll /map /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"../Game/VN/XFControl_D.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 VN_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/VN"
# PROP BASE Intermediate_Dir "../Compiled/VN/release_debug/XFControl"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/VN"
# PROP Intermediate_Dir "../Compiled/VN/release_debug/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /FR /YX /FD /Zm150 /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XVIETNAMESE" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/VN/XGameBase_RD.Lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/VN/XGameBase_RD.Lib /nologo /dll /incremental:yes /map /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/VN/XFControl_RD.dll"

!ELSEIF  "$(CFG)" == "XFControl - Win32 VN_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/VN"
# PROP BASE Intermediate_Dir "../Compiled/VN/release_GM/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/VN"
# PROP Intermediate_Dir "../Compiled/VN/release_GM/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XGMCLIENT" /D "_XVIETNAMESE" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/VN/XGameBase_GM.Lib /nologo /dll /pdb:"../Compiled/VN/release_gm/XFControl/XFControl.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/VN/XFControl_GM.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 TW_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/TW"
# PROP BASE Intermediate_Dir "../Compiled/TW/release/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/TW"
# PROP Intermediate_Dir "../Compiled/TW/release/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XTAIWANESE" /D "_XTW_CSRELEASEVERSION" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/TW/XGameBase.Lib /nologo /dll /pdb:"../Compiled/TW/release/XFControl/XFControl.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/TW/XFControl.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 TW_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../Library/TW"
# PROP BASE Intermediate_Dir "../Compiled/TW/debug/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Library/TW"
# PROP Intermediate_Dir "../Compiled/TW/debug/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XTAIWANESE" /D "_XTW_CSRELEASEVERSION" /FR /FD /GZ /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/TW/XGameBase_D.Lib /nologo /dll /map /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"../Game/TW/XFControl_D.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 TW_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/TW"
# PROP BASE Intermediate_Dir "../Compiled/TW/release_debug/XFControl"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/TW"
# PROP Intermediate_Dir "../Compiled/TW/release_debug/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /FR /YX /FD /Zm150 /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XTAIWANESE" /D "_XTW_CSRELEASEVERSION" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/TW/XGameBase_RD.Lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/TW/XGameBase_RD.Lib /nologo /dll /incremental:yes /map /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/TW/XFControl_RD.dll"

!ELSEIF  "$(CFG)" == "XFControl - Win32 TW_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/TW"
# PROP BASE Intermediate_Dir "../Compiled/TW/release_GM/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/TW"
# PROP Intermediate_Dir "../Compiled/TW/release_GM/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XGMCLIENT" /D "_XTAIWANESE" /D "_XTW_CSRELEASEVERSION" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/TW/XGameBase_GM.Lib /nologo /dll /pdb:"../Compiled/TW/release_gm/XFControl/XFControl.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/TW/XFControl_GM.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 US_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/US"
# PROP BASE Intermediate_Dir "../Compiled/US/release/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/US"
# PROP Intermediate_Dir "../Compiled/US/release/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_ACCLAIM_VERSION" /D "_XENGLISH" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/US/XGameBase.Lib /nologo /dll /pdb:"../Compiled/US/release/XFControl/XFControl.pdb" /machine:I386 /nodefaultlib:"libcmt.lib msvcrt.lib" /out:"../Game/US/XFControl.dll"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "XFControl - Win32 US_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../Library/US"
# PROP BASE Intermediate_Dir "../Compiled/US/debug/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Library/US"
# PROP Intermediate_Dir "../Compiled/US/debug/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_ACCLAIM_VERSION" /D "_XENGLISH" /FR /FD /GZ /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/US/XGameBase_D.Lib /nologo /dll /map /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB MSVCRT.LIB" /out:"../Game/US/XFControl_D.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 US_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/US"
# PROP BASE Intermediate_Dir "../Compiled/US/release_debug/XFControl"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/US"
# PROP Intermediate_Dir "../Compiled/US/release_debug/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /FR /YX /FD /Zm150 /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_ACCLAIM_VERSION" /D "_XENGLISH" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/US/XGameBase_RD.Lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/US/XGameBase_RD.Lib /nologo /dll /profile /map:"../Game/US/XFControl_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/US/XFControl_RD.dll"

!ELSEIF  "$(CFG)" == "XFControl - Win32 US_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/US"
# PROP BASE Intermediate_Dir "../Compiled/US/release_GM/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/US"
# PROP Intermediate_Dir "../Compiled/US/release_GM/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XGMCLIENT" /D "_ACCLAIM_VERSION" /D "_XENGLISH" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/US/XGameBase_GM.Lib /nologo /dll /pdb:"../Compiled/US/release_gm/XFControl/XFControl.pdb" /machine:I386 /nodefaultlib:"LIBCMT.LIB MSVCRT.LIB" /out:"../Game/US/XFControl_GM.dll"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "XFControl - Win32 CN_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/CN"
# PROP BASE Intermediate_Dir "../Compiled/CN/release/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/CN"
# PROP Intermediate_Dir "../Compiled/CN/release/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XCHINESE" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/CN/XGameBase.Lib /nologo /dll /pdb:"../Compiled/CN/release/XFControl/XFControl.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/CN/XFControl.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 CN_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../Library/CN"
# PROP BASE Intermediate_Dir "../Compiled/CN/debug/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Library/CN"
# PROP Intermediate_Dir "../Compiled/CN/debug/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XCHINESE" /FR /FD /GZ /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/CN/XGameBase_D.Lib /nologo /dll /map /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"../Game/CN/XFControl_D.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 CN_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/CN"
# PROP BASE Intermediate_Dir "../Compiled/CN/release_debug/XFControl"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/CN"
# PROP Intermediate_Dir "../Compiled/CN/release_debug/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /FR /YX /FD /Zm150 /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XCHINESE" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/CN/XGameBase_RD.Lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/CN/XGameBase_RD.Lib /nologo /dll /incremental:yes /map /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/CN/XFControl_RD.dll"

!ELSEIF  "$(CFG)" == "XFControl - Win32 CN_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/CN"
# PROP BASE Intermediate_Dir "../Compiled/CN/release_GM/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/CN"
# PROP Intermediate_Dir "../Compiled/CN/release_GM/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XGMCLIENT" /D "_XCHINESE" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/CN/XGameBase_GM.Lib /nologo /dll /pdb:"../Compiled/CN/release_gm/XFControl/XFControl.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/CN/XFControl_GM.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 JP_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/JP"
# PROP BASE Intermediate_Dir "../Compiled/JP/release/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/JP"
# PROP Intermediate_Dir "../Compiled/JP/release/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XJAPANESE" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/JP/XGameBase.Lib /nologo /dll /pdb:"../Compiled/JP/release/XFControl/XFControl.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/JP/XFControl.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 JP_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../Library/JP"
# PROP BASE Intermediate_Dir "../Compiled/JP/debug/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Library/JP"
# PROP Intermediate_Dir "../Compiled/JP/debug/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XJAPANESE" /FR /FD /GZ /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/JP/XGameBase_D.Lib /nologo /dll /map /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"../Game/JP/XFControl_D.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 JP_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/JP"
# PROP BASE Intermediate_Dir "../Compiled/JP/release_debug/XFControl"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/JP"
# PROP Intermediate_Dir "../Compiled/JP/release_debug/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /FR /YX /FD /Zm150 /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XJAPANESE" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/JP/XGameBase_RD.Lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/JP/XGameBase_RD.Lib /nologo /dll /incremental:yes /map /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/JP/XFControl_RD.dll"

!ELSEIF  "$(CFG)" == "XFControl - Win32 JP_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/JP"
# PROP BASE Intermediate_Dir "../Compiled/JP/release_GM/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/JP"
# PROP Intermediate_Dir "../Compiled/JP/release_GM/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XGMCLIENT" /D "_XJAPANESE" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/JP/XGameBase_GM.Lib /nologo /dll /pdb:"../Compiled/JP/release_gm/XFControl/XFControl.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/JP/XFControl_GM.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 RS_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/RS"
# PROP BASE Intermediate_Dir "../Compiled/RS/release/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/RS"
# PROP Intermediate_Dir "../Compiled/RS/release/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XRUSSIAN" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/RS/XGameBase.Lib /nologo /dll /pdb:"../Compiled/RS/release/XFControl/XFControl.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/RS/XFControl.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 RS_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../Library/RS"
# PROP BASE Intermediate_Dir "../Compiled/RS/debug/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Library/RS"
# PROP Intermediate_Dir "../Compiled/RS/debug/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XRUSSIAN" /FR /FD /GZ /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/RS/XGameBase_D.Lib /nologo /dll /map /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"../Game/RS/XFControl_D.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 RS_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/RS"
# PROP BASE Intermediate_Dir "../Compiled/RS/release_debug/XFControl"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/RS"
# PROP Intermediate_Dir "../Compiled/RS/release_debug/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /FR /YX /FD /Zm150 /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XRUSSIAN" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/RS/XGameBase_RD.Lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/RS/XGameBase_RD.Lib /nologo /dll /incremental:yes /map /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/RS/XFControl_RD.dll"

!ELSEIF  "$(CFG)" == "XFControl - Win32 RS_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/RS"
# PROP BASE Intermediate_Dir "../Compiled/RS/release_GM/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/RS"
# PROP Intermediate_Dir "../Compiled/RS/release_GM/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XGMCLIENT" /D "_XRUSSIAN" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/RS/XGameBase_GM.Lib /nologo /dll /pdb:"../Compiled/RS/release_gm/XFControl/XFControl.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/RS/XFControl_GM.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 TH_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/TH"
# PROP BASE Intermediate_Dir "../Compiled/TH/release/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/TH"
# PROP Intermediate_Dir "../Compiled/TH/release/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XTHAI" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/TH/XGameBase.Lib /nologo /dll /pdb:"../Compiled/TH/release/XFControl/XFControl.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/TH/XFControl.dll"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 TH_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../Library/TH"
# PROP BASE Intermediate_Dir "../Compiled/TH/debug/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../Library/TH"
# PROP Intermediate_Dir "../Compiled/TH/debug/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XTHAI" /FR /FD /GZ /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/TH/XGameBase_D.Lib /nologo /dll /map /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"../Game/TH/XFControl_D.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XFControl - Win32 TH_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/TH"
# PROP BASE Intermediate_Dir "../Compiled/TH/release_debug/XFControl"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/TH"
# PROP Intermediate_Dir "../Compiled/TH/release_debug/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /FR /YX /FD /Zm150 /c
# ADD CPP /nologo /MDd /W3 /GX /ZI /Od /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XTHAI" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/TH/XGameBase_RD.Lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/TH/XGameBase_RD.Lib /nologo /dll /incremental:yes /map /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/TH/XFControl_RD.dll"

!ELSEIF  "$(CFG)" == "XFControl - Win32 TH_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../Library/TH"
# PROP BASE Intermediate_Dir "../Compiled/TH/release_GM/XFControl"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Library/TH"
# PROP Intermediate_Dir "../Compiled/TH/release_GM/XFControl"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./../xgamebase/src/libs/jpeg" /I "./../xgamebase/src/libs/ZLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XFCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "NOEDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XGMCLIENT" /D "_XTHAI" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib wsock32.lib winmm.lib advapi32.lib imm32.lib mp3decoder.lib ./../Library/TH/XGameBase_GM.Lib /nologo /dll /pdb:"../Compiled/TH/release_gm/XFControl/XFControl.pdb" /debug /machine:I386 /nodefaultlib:"LIBCMT.LIB" /out:"../Game/TH/XFControl_GM.dll"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "XFControl - Win32 KR_Release"
# Name "XFControl - Win32 KR_Debug"
# Name "XFControl - Win32 KR_Release_Debug"
# Name "XFControl - Win32 KR_Release_GM"
# Name "XFControl - Win32 VN_Release"
# Name "XFControl - Win32 VN_Debug"
# Name "XFControl - Win32 VN_Release_Debug"
# Name "XFControl - Win32 VN_Release_GM"
# Name "XFControl - Win32 TW_Release"
# Name "XFControl - Win32 TW_Debug"
# Name "XFControl - Win32 TW_Release_Debug"
# Name "XFControl - Win32 TW_Release_GM"
# Name "XFControl - Win32 US_Release"
# Name "XFControl - Win32 US_Debug"
# Name "XFControl - Win32 US_Release_Debug"
# Name "XFControl - Win32 US_Release_GM"
# Name "XFControl - Win32 CN_Release"
# Name "XFControl - Win32 CN_Debug"
# Name "XFControl - Win32 CN_Release_Debug"
# Name "XFControl - Win32 CN_Release_GM"
# Name "XFControl - Win32 JP_Release"
# Name "XFControl - Win32 JP_Debug"
# Name "XFControl - Win32 JP_Release_Debug"
# Name "XFControl - Win32 JP_Release_GM"
# Name "XFControl - Win32 RS_Release"
# Name "XFControl - Win32 RS_Debug"
# Name "XFControl - Win32 RS_Release_Debug"
# Name "XFControl - Win32 RS_Release_GM"
# Name "XFControl - Win32 TH_Release"
# Name "XFControl - Win32 TH_Debug"
# Name "XFControl - Win32 TH_Release_Debug"
# Name "XFControl - Win32 TH_Release_GM"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Win32\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\Win32\Display.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashControlImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\win32\FSControl.cpp
# End Source File
# Begin Source File

SOURCE=.\Win32\palette.cpp
# End Source File
# Begin Source File

SOURCE=.\Win32\sound.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Win32\util.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Win32\debug.h
# End Source File
# Begin Source File

SOURCE=.\Win32\display.h
# End Source File
# Begin Source File

SOURCE=.\Win32\fixed.h
# End Source File
# Begin Source File

SOURCE=.\FlashControl.h
# End Source File
# Begin Source File

SOURCE=.\FlashControlImpl.h
# End Source File
# Begin Source File

SOURCE=.\win32\FSControl.h
# End Source File
# Begin Source File

SOURCE=.\Win32\palette.h
# End Source File
# Begin Source File

SOURCE=.\Win32\Sound.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Win32\util.h
# End Source File
# Begin Source File

SOURCE=.\Win32\version.h
# End Source File
# End Group
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bitbuf.cpp
# End Source File
# Begin Source File

SOURCE=.\Bitbuf.h
# End Source File
# Begin Source File

SOURCE=.\bitstream.h
# End Source File
# Begin Source File

SOURCE=.\curve.cpp
# End Source File
# Begin Source File

SOURCE=.\curve.h
# End Source File
# Begin Source File

SOURCE=.\edittext.cpp
# End Source File
# Begin Source File

SOURCE=.\edittext.h
# End Source File
# Begin Source File

SOURCE=.\fixed.cpp
# End Source File
# Begin Source File

SOURCE=.\fixed.h
# End Source File
# Begin Source File

SOURCE=.\genericfonts.h
# End Source File
# Begin Source File

SOURCE=.\geom.cpp
# End Source File
# Begin Source File

SOURCE=.\geom.h
# End Source File
# Begin Source File

SOURCE=.\global.cpp
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\jpeg.cpp
# End Source File
# Begin Source File

SOURCE=.\jpeg.h
# End Source File
# Begin Source File

SOURCE=.\mcontext.h
# End Source File
# Begin Source File

SOURCE=.\memcop.cpp
# End Source File
# Begin Source File

SOURCE=.\memcop.h
# End Source File
# Begin Source File

SOURCE=.\morphinter.cpp
# End Source File
# Begin Source File

SOURCE=.\morphinter.h
# End Source File
# Begin Source File

SOURCE=.\mp3decifc.h
# End Source File
# Begin Source File

SOURCE=.\mp3decoder.cpp
# End Source File
# Begin Source File

SOURCE=.\mp3decoder.h
# End Source File
# Begin Source File

SOURCE=.\mp3sscdef.h
# End Source File
# Begin Source File

SOURCE=.\mp3streaminfo.h
# End Source File
# Begin Source File

SOURCE=.\mpegbitstream.h
# End Source File
# Begin Source File

SOURCE=.\mpegheader.h
# End Source File
# Begin Source File

SOURCE=.\raster.cpp
# End Source File
# Begin Source File

SOURCE=.\raster.h
# End Source File
# Begin Source File

SOURCE=.\sbitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\sbitmap.h
# End Source File
# Begin Source File

SOURCE=.\sdisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\sdisplay.h
# End Source File
# Begin Source File

SOURCE=.\shape.h
# End Source File
# Begin Source File

SOURCE=.\sndcodec.cpp
# End Source File
# Begin Source File

SOURCE=.\sndcodec.h
# End Source File
# Begin Source File

SOURCE=.\sndmix.cpp
# End Source File
# Begin Source File

SOURCE=.\sndmix.h
# End Source File
# Begin Source File

SOURCE=.\sobject.cpp
# End Source File
# Begin Source File

SOURCE=.\sobject.h
# End Source File
# Begin Source File

SOURCE=.\splay.cpp
# End Source File
# Begin Source File

SOURCE=.\splay.h
# End Source File
# Begin Source File

SOURCE=.\splayer.cpp
# End Source File
# Begin Source File

SOURCE=.\splayer.h
# End Source File
# Begin Source File

SOURCE=.\sstroker.cpp
# End Source File
# Begin Source File

SOURCE=.\sstroker.h
# End Source File
# Begin Source File

SOURCE=.\stags.h
# End Source File
# Begin Source File

SOURCE=.\unixfonts.cpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\win32\ReadMe.txt
# End Source File
# End Target
# End Project
