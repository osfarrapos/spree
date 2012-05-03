# Microsoft Developer Studio Project File - Name="EmperorOfDragons" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=EmperorOfDragons - Win32 TH_Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EmperorOfDragons.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EmperorOfDragons.mak" CFG="EmperorOfDragons - Win32 TH_Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EmperorOfDragons - Win32 KR_Release" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 KR_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 KR_Release_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 KR_Release_GM" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 KR_Test_Release" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 KR_Test_Release_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 KR_Test_Release_GM" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 VN_Release" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 VN_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 VN_Release_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 VN_Release_GM" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 TW_Release" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 TW_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 TW_Release_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 TW_Release_GM" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 US_Release" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 US_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 US_Release_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 US_Release_GM" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 CN_Release" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 CN_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 CN_Release_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 CN_Release_GM" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 JP_Release" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 JP_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 JP_Release_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 JP_Release_GM" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 RS_Release" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 RS_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 RS_Release_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 RS_Release_GM" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 TH_Release" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 TH_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 TH_Release_Debug" (based on "Win32 (x86) Application")
!MESSAGE "EmperorOfDragons - Win32 TH_Release_GM" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 1
# PROP Scc_ProjName ""$/EmperorOfDragons", ZEAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EmperorOfDragons - Win32 KR_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/KR/release/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/KR/release/EmperorOfDragons"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/KR/release/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/KR/release/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_NM_DYNAMIC_LOAD" /D "_XKOREAN" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ./Library/KR/XGamebase.lib ./Library/KR/XKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/KR/XFControl.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib ./Library/KR/NexonADBalloonLib.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/KR/NineDragons.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 KR_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Compiled/KR/debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/KR/debug/EmperorOfDragons"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Compiled/KR/debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/KR/debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "ITEM_VER2" /D "_XESTABLISHEDSERVER" /D "_XKOREAN" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ./Library/KR/XGamebase_D.lib ./Library/KR/XKernel_D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/KR/XFControl_D.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/KR/NexonADBalloonLib_D.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"./Game/KR/NineDragons_D.exe" /pdbtype:sept /OPT:REF
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 KR_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/KR/release_debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/KR/release_debug/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/KR/release_debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/KR/release_debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./ErrorHandler" /I "./XWControl" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_XDWDEBUG" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XGMCLIENT" /D "_XKOREAN" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/XGamebase_RD.lib ./Library/XKernel_RD.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib d3dx8.lib dxguid.lib ws2_32.lib imagehlp.lib /nologo /subsystem:windows /machine:I386 /out:"./Game/KR/EmperorOfDragons_RD.exe"
# SUBTRACT BASE LINK32 /profile /pdb:none
# ADD LINK32 ./Library/KR/XKernel_RD.lib ./Library/KR/XGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/KR/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib ./Library/KR/NexonADBalloonLib_RD.lib /nologo /subsystem:windows /map:"./Game/KR/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/KR/NineDragons_RD.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /incremental:yes /nodefaultlib

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 KR_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/KR/release_gm/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/KR/release_gm/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/KR/release_gm/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/KR/release_gm/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./audio_sdk/include" /I "./audio_sdk/include/AudioLib" /I "./audio_sdk/include/AudioScript" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XGMCLIENT" /D "_XDEF_VIEW_TESTSERVER_RELEASE" /D "_XKOREAN" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/KR/XGamebase_GM.lib ./Library/KR/XKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/KR/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/KR/NineDragons_GM.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/KR/XGamebase_GM.lib ./Library/KR/XKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/KR/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib ./Library/KR/NexonADBalloonLib_GM.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/KR/NineDragons_GM.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 KR_Test_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/KR/test_release/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/KR/test_release/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/KR/test_release/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/KR/test_release/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XTESTSERVER" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_NM_DYNAMIC_LOAD" /D "_XTESTSERVER" /D "_XKOREAN" /D "_XUSEAPEXMODULE" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/KR/tsXKernel.lib ./Library/KR/tsXGamebase.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/KR/XFControl.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /map:"./Game/KR/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/KR/tsNineDragons.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none /incremental:yes
# ADD LINK32 ./Library/KR/tsXGamebase.lib ./Library/KR/tsXKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/KR/XFControl.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib ./Library/KR/NexonADBalloonLib.lib /nologo /subsystem:windows /map:"./Game/KR/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/KR/tsNineDragons.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 KR_Test_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/KR/test_release_debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/KR/test_release_debug/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/KR/test_release_debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/KR/test_release_debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XGMCLIENT" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XGMCLIENT" /D "_XTESTSERVER" /D "_XKOREAN" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/KR/tsXKernel_RD.lib ./Library/KR/tsXGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/KR/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /map:"./Game/KR/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/KR/tsNineDragons_RD.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none /incremental:yes
# ADD LINK32 ./Library/KR/tsXKernel_RD.lib ./Library/KR/tsXGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/KR/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib ./Library/KR/NexonADBalloonLib_D.lib /nologo /subsystem:windows /map:"./Game/KR/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/KR/tsNineDragons_RD.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 KR_Test_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/KR/test_release_gm/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/KR/test_release_gm/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/KR/test_release_gm/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/KR/test_release_gm/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./audio_sdk/include" /I "./audio_sdk/include/AudioLib" /I "./audio_sdk/include/AudioScript" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XGMCLIENT" /D "_XTESTSERVER" /D "_XDEF_VIEW_TESTSERVER_RELEASE" /D "_XKOREAN" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/KR/tsXGamebase_GM.lib ./Library/KR/tsXKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/KR/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/KR/tsNineDragons_GM.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/KR/tsXGamebase_GM.lib ./Library/KR/tsXKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/KR/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib ./Library/KR/NexonADBalloonLib.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/KR/tsNineDragons_GM.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 VN_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/VN/release/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/VN/release/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/VN/release/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/VN/release/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XVIETNAMESE" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/VNXGamebase.lib ./Library/VN/XKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/VN/XFControl.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/VN/NineDragons.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/VN/XGamebase.lib ./Library/VN/XKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/VN/XFControl.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/VN/NineDragons.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 VN_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Compiled/VN/debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/VN/debug/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Compiled/VN/debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/VN/debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "ITEM_VER2" /D "_XESTABLISHEDSERVER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "ITEM_VER2" /D "_XESTABLISHEDSERVER" /D "_XVIETNAMESE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/VN/XGamebase_D.lib ./Library/VN/XKernel_D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/VN/XFControl_D.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"./Game/VN/NineDragons_D.exe" /pdbtype:sept /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/VN/XGamebase_D.lib ./Library/VN/XKernel_D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/VN/XFControl_D.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"./Game/VN/NineDragons_D.exe" /pdbtype:sept /OPT:REF
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 VN_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/VN/release_debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/VN/release_debug/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/VN/release_debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/VN/release_debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XVIETNAMESE" /D "_XDEF_MEMORYHACKDEFENCE" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/VN/XKernel_RD.lib ./Library/VN/XGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/VN/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /map:"./Game/VN/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/VN/NineDragons_RD.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none /incremental:yes
# ADD LINK32 ./Library/VN/XKernel_RD.lib ./Library/VN/XGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/VN/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /map:"./Game/VN/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/VN/NineDragons_RD.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 VN_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/VN/release_gm/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/VN/release_gm/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/VN/release_gm/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/VN/release_gm/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XGMCLIENT" /D "_XVIETNAMESE" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/VN/XGamebase_GM.lib ./Library/VN/XKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/VN/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/VN/NineDragons_GM.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/VN/XGamebase_GM.lib ./Library/VN/XKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/VN/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/VN/NineDragons_GM.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 TW_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/TW/release/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/TW/release/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/TW/release/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/TW/release/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XTAIWANESE" /D "_XTW_CSRELEASEVERSION" /D "_XUSEAPEXMODULE" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/TW/XGamebase.lib ./Library/TW/XKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/TW/XFControl.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/TW/NineDragons.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/TW/XGamebase.lib ./Library/TW/XKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/TW/XFControl.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/TW/NineDragons.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 TW_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Compiled/TW/debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/TW/debug/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Compiled/TW/debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/TW/debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "ITEM_VER2" /D "_XESTABLISHEDSERVER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "ITEM_VER2" /D "_XESTABLISHEDSERVER" /D "_XTAIWANESE" /D "_XTW_CSRELEASEVERSION" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/TW/XGamebase_D.lib ./Library/TW/XKernel_D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/TW/XFControl_D.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"./Game/TW/NineDragons_D.exe" /pdbtype:sept /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/TW/XGamebase_D.lib ./Library/TW/XKernel_D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/TW/XFControl_D.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"./Game/TW/NineDragons_D.exe" /pdbtype:sept /OPT:REF
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 TW_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/TW/release_debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/TW/release_debug/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/TW/release_debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/TW/release_debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XTAIWANESE" /D "_XGMCLIENT" /D "_XTW_CSRELEASEVERSION" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/TW/XKernel_RD.lib ./Library/TW/XGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/TW/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /map:"./Game/TW/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/TW/NineDragons_RD.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none /incremental:yes
# ADD LINK32 ./Library/TW/XKernel_RD.lib ./Library/TW/XGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/TW/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /map:"./Game/TW/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/TW/NineDragons_RD.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 TW_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/TW/release_gm/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/TW/release_gm/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/TW/release_gm/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/TW/release_gm/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XGMCLIENT" /D "_XTAIWANESE" /D "_XTW_CSRELEASEVERSION" /D "_XUSEAPEXMODULE" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/TW/XGamebase_GM.lib ./Library/TW/XKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/TW/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/TW/NineDragons_GM.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/TW/XGamebase_GM.lib ./Library/TW/XKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/TW/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/TW/NineDragons_GM.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 US_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/US/release/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/US/release/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/US/release/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/US/release/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /I "./XIGAADWrapper" /I "./Library/radsdk" /I "./Library/BinkSDK" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_ACCLAIM_VERSION" /D "_XENGLISH" /D "_ACCLAIM_RUBICONADSYSTEM" /D "_X_US_EXPANDSERVERLIST" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/US/XGamebase.lib ./Library/US/XKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/US/XFControl.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/US/NineDragons.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 msvcrt.lib libcmt.lib ./Library/US/XGamebase.lib ./Library/US/XKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/US/XFControl.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./Library/BinkSDK/binkw32.lib ./Library/radsdk/radsdk6.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"msvcrt.lib libcmt.lib" /out:"./Game/US/NineDragons.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 US_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Compiled/US/debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/US/debug/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Compiled/US/debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/US/debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "ITEM_VER2" /D "_XESTABLISHEDSERVER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "ITEM_VER2" /D "_XESTABLISHEDSERVER" /D "_ACCLAIM_VERSION" /D "_XENGLISH" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/US/XGamebase_D.lib ./Library/US/XKernel_D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/US/XFControl_D.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"./Game/US/NineDragons_D.exe" /pdbtype:sept /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 MSVCRT.LIB LIBCMT.LIB ./Library/US/XGamebase_D.lib ./Library/US/XKernel_D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/US/XFControl_D.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/BinkSDK/binkw32.lib ./Library/radsdk/radsdk6.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"MSVCRT.LIB LIBCMT.LIB" /out:"./Game/US/NineDragons_D.exe" /pdbtype:sept /OPT:REF
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 US_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/US/release_debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/US/release_debug/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/US/release_debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/US/release_debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XGMCLIENT" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./XIGAADWrapper" /I "./Library/radsdk" /I "./Library/BinkSDK" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_ACCLAIM_VERSION" /D "_XENGLISH" /D "_XGMCLIENT" /D "_ACCLAIM_RUBICONADSYSTEM" /D "_X_US_EXPANDSERVERLIST" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/US/XKernel_RD.lib ./Library/US/XGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/US/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /map:"./Game/US/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/US/NineDragons_RD.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none /incremental:yes
# ADD LINK32 ./Library/US/XKernel_RD.lib ./Library/US/XGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/US/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./Library/BinkSDK/binkw32.lib ./Library/radsdk/radsdk6.lib /nologo /subsystem:windows /profile /map:"./Game/US/NineDragons_RD.map" /debug /machine:I386 /out:"./Game/US/NineDragons_RD.exe" /OPT:REF

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 US_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/US/release_gm/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/US/release_gm/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/US/release_gm/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/US/release_gm/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /I "./XIGAADWrapper" /I "./Library/radsdk" /I "./Library/BinkSDK" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XGMCLIENT" /D "_ACCLAIM_VERSION" /D "_XENGLISH" /D "_ACCLAIM_RUBICONADSYSTEM" /D "_X_US_EXPANDSERVERLIST" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/US/XGamebase_GM.lib ./Library/US/XKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/US/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/US/NineDragons_GM.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 MSVCRT.LIB LIBCMT.LIB ./Library/US/XGamebase_GM.lib ./Library/US/XKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/US/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./Library/BinkSDK/binkw32.lib ./Library/radsdk/radsdk6.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"MSVCRT.LIB LIBCMT.LIB" /out:"./Game/US/NineDragons_GM.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 CN_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/CN/release/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/CN/release/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/CN/release/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/CN/release/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XCHINESE" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/CN/XGamebase.lib ./Library/CN/XKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/CN/XFControl.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/CN/NineDragons.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/CN/XGamebase.lib ./Library/CN/XKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/CN/XFControl.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/CN/NineDragons.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 CN_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Compiled/CN/debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/CN/debug/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Compiled/CN/debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/CN/debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "ITEM_VER2" /D "_XESTABLISHEDSERVER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "ITEM_VER2" /D "_XESTABLISHEDSERVER" /D "_XCHINESE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/CN/XGamebase_D.lib ./Library/CN/XKernel_D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/CN/XFControl_D.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"./Game/CN/NineDragons_D.exe" /pdbtype:sept /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/CN/XGamebase_D.lib ./Library/CN/XKernel_D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/CN/XFControl_D.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"./Game/CN/NineDragons_D.exe" /pdbtype:sept /OPT:REF
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 CN_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/CN/release_debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/CN/release_debug/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/CN/release_debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/CN/release_debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XCHINESE" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/CN/XKernel_RD.lib ./Library/CN/XGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/CN/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /map:"./Game/CN/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/CN/NineDragons_RD.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none /incremental:yes
# ADD LINK32 ./Library/CN/XKernel_RD.lib ./Library/CN/XGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/CN/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /map:"./Game/CN/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/CN/NineDragons_RD.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 CN_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/CN/release_gm/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/CN/release_gm/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/CN/release_gm/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/CN/release_gm/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XGMCLIENT" /D "_XCHINESE" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/CN/XGamebase_GM.lib ./Library/CN/XKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/CN/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/CN/NineDragons_GM.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/CN/XGamebase_GM.lib ./Library/CN/XKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/CN/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/CN/NineDragons_GM.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 JP_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/JP/release/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/JP/release/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/JP/release/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/JP/release/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XJAPANESE" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/JP/XGamebase.lib ./Library/JP/XKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/JP/XFControl.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/JP/NineDragons.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/JP/XGamebase.lib ./Library/JP/XKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/JP/XFControl.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/JP/NineDragons.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 JP_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Compiled/JP/debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/JP/debug/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Compiled/JP/debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/JP/debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "ITEM_VER2" /D "_XESTABLISHEDSERVER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "ITEM_VER2" /D "_XESTABLISHEDSERVER" /D "_XJAPANESE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/JP/XGamebase_D.lib ./Library/JP/XKernel_D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/JP/XFControl_D.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"./Game/JP/NineDragons_D.exe" /pdbtype:sept /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/JP/XGamebase_D.lib ./Library/JP/XKernel_D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/JP/XFControl_D.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"./Game/JP/NineDragons_D.exe" /pdbtype:sept /OPT:REF
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 JP_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/JP/release_debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/JP/release_debug/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/JP/release_debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/JP/release_debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XGMCLIENT" /D "_XJAPANESE" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/JP/XKernel_RD.lib ./Library/JP/XGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/JP/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /map:"./Game/JP/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/JP/NineDragons_RD.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none /incremental:yes
# ADD LINK32 ./Library/JP/XKernel_RD.lib ./Library/JP/XGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/JP/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /map:"./Game/JP/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/JP/NineDragons_RD.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 JP_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/JP/release_gm/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/JP/release_gm/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/JP/release_gm/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/JP/release_gm/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XGMCLIENT" /D "_XJAPANESE" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/JP/XGamebase_GM.lib ./Library/JP/XKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/JP/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/JP/NineDragons_GM.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/JP/XGamebase_GM.lib ./Library/JP/XKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/JP/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/JP/NineDragons_GM.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 RS_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/RS/release/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/RS/release/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/RS/release/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/RS/release/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XRUSSIAN" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/RS/XGamebase.lib ./Library/RS/XKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/RS/XFControl.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/RS/NineDragons.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/RS/XGamebase.lib ./Library/RS/XKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/RS/XFControl.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/RS/NineDragons.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 RS_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Compiled/RS/debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/RS/debug/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Compiled/RS/debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/RS/debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "ITEM_VER2" /D "_XESTABLISHEDSERVER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "ITEM_VER2" /D "_XESTABLISHEDSERVER" /D "_XRUSSIAN" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/RS/XGamebase_D.lib ./Library/RS/XKernel_D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/RS/XFControl_D.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"./Game/RS/NineDragons_D.exe" /pdbtype:sept /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/RS/XGamebase_D.lib ./Library/RS/XKernel_D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/RS/XFControl_D.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"./Game/RS/NineDragons_D.exe" /pdbtype:sept /OPT:REF
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 RS_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/RS/release_debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/RS/release_debug/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/RS/release_debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/RS/release_debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XRUSSIAN" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/RS/XKernel_RD.lib ./Library/RS/XGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/RS/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /map:"./Game/RS/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/RS/NineDragons_RD.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none /incremental:yes
# ADD LINK32 ./Library/RS/XKernel_RD.lib ./Library/RS/XGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/RS/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /map:"./Game/RS/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/RS/NineDragons_RD.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 RS_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/RS/release_gm/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/RS/release_gm/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/RS/release_gm/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/RS/release_gm/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XGMCLIENT" /D "_XRUSSIAN" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/RS/XGamebase_GM.lib ./Library/RS/XKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/RS/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/RS/NineDragons_GM.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/RS/XGamebase_GM.lib ./Library/RS/XKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/RS/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/RS/NineDragons_GM.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 TH_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/TH/release/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/TH/release/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/TH/release/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/TH/release/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XTHAI" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/TH/XGamebase.lib ./Library/TH/XKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/TH/XFControl.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/TH/NineDragons.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/TH/XGamebase.lib ./Library/TH/XKernel.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/TH/XFControl.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/TH/NineDragons.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 TH_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Compiled/TH/debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/TH/debug/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Compiled/TH/debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/TH/debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "ITEM_VER2" /D "_XESTABLISHEDSERVER" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./XSound" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "ITEM_VER2" /D "_XESTABLISHEDSERVER" /D "_XTHAI" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/TH/XGamebase_D.lib ./Library/TH/XKernel_D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/TH/XFControl_D.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"./Game/TH/NineDragons_D.exe" /pdbtype:sept /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/TH/XGamebase_D.lib ./Library/TH/XKernel_D.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/TH/XFControl_D.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"./Game/TH/NineDragons_D.exe" /pdbtype:sept /OPT:REF
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 TH_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/TH/release_debug/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/TH/release_debug/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/TH/release_debug/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/TH/release_debug/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XADMINISTRATORMODE" /D "_XKERYGMASETTING" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XTHAI" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_XDWDEBUG" /d "_XADMINISTRATORMODE"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/TH/XKernel_RD.lib ./Library/TH/XGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/TH/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /map:"./Game/TH/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/TH/NineDragons_RD.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none /incremental:yes
# ADD LINK32 ./Library/TH/XKernel_RD.lib ./Library/TH/XGamebase_RD.lib LIBCPMT.LIB kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/TH/XFControl_RD.lib version.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /map:"./Game/TH/EmperorOfDragons_RD.map" /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"./Game/TH/NineDragons_RD.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /incremental:yes

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 TH_Release_GM"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Compiled/TH/release_gm/EmperorOfDragons"
# PROP BASE Intermediate_Dir "Compiled/TH/release_gm/EmperorOfDragons"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/TH/release_gm/EmperorOfDragons"
# PROP Intermediate_Dir "Compiled/TH/release_gm/EmperorOfDragons"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "./" /I "./XGamebase/Include" /I "./XGamebase/Include/Template" /I "./XKernel" /I "./XProcess" /I "./DataStructure" /I "./XControl" /I "./Utility" /I "./Network" /I "./MeshControl" /I "./ObjectControl" /I "./PathFinding" /I "./Effect/Particle" /I "./Effect/Script" /I "./Script" /I "./XSound" /I "./Filtering" /I "./XGamebase/src/libs/jpeg" /I "./XSTreeWrapper" /I "./XParticleCore" /I "./XFControl" /I "./HackDetector/HackShield" /I "./HackDetector/GameGuard" /I "./Library" /I "./Library/QHTM" /I "./Library/FMod" /I "./XMADWrapper" /I "./NMClass" /I "./HackDetector/XTrap" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "NOUSE_VORBIS" /D "NOUSE_WMA" /D "_XUSEFMOD" /D "_XNOCHECKMEMORYUSAGE" /D "_XESTABLISHEDSERVER" /D "_NEW_TYPE" /D "_XGMCLIENT" /D "_XTHAI" /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ./Library/TH/XGamebase_GM.lib ./Library/TH/XKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/TH/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib ./NMClass/nmcogame.lib ./NMClass/nmconew.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/TH/NineDragons_GM.exe" /OPT:REF
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 ./Library/TH/XGamebase_GM.lib ./Library/TH/XKernel_GM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib dxguid.lib d3d9.lib d3dx9dt.lib d3dx9.lib dsound.lib dinput8.lib dxerr9.lib d3dxof.lib ws2_32.lib imagehlp.lib Imm32.lib msacm32.lib SpeedTreeRT.lib ./Library/dbghelp.lib ./Library/TH/XFControl_GM.lib ./Library/QHTM/QHTM.lib ./Library/FMod/fmodvc.lib ./Library/CWebPage/XWebPage.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libcmt.lib" /out:"./Game/TH/NineDragons_GM.exe" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ENDIF 

# Begin Target

# Name "EmperorOfDragons - Win32 KR_Release"
# Name "EmperorOfDragons - Win32 KR_Debug"
# Name "EmperorOfDragons - Win32 KR_Release_Debug"
# Name "EmperorOfDragons - Win32 KR_Release_GM"
# Name "EmperorOfDragons - Win32 KR_Test_Release"
# Name "EmperorOfDragons - Win32 KR_Test_Release_Debug"
# Name "EmperorOfDragons - Win32 KR_Test_Release_GM"
# Name "EmperorOfDragons - Win32 VN_Release"
# Name "EmperorOfDragons - Win32 VN_Debug"
# Name "EmperorOfDragons - Win32 VN_Release_Debug"
# Name "EmperorOfDragons - Win32 VN_Release_GM"
# Name "EmperorOfDragons - Win32 TW_Release"
# Name "EmperorOfDragons - Win32 TW_Debug"
# Name "EmperorOfDragons - Win32 TW_Release_Debug"
# Name "EmperorOfDragons - Win32 TW_Release_GM"
# Name "EmperorOfDragons - Win32 US_Release"
# Name "EmperorOfDragons - Win32 US_Debug"
# Name "EmperorOfDragons - Win32 US_Release_Debug"
# Name "EmperorOfDragons - Win32 US_Release_GM"
# Name "EmperorOfDragons - Win32 CN_Release"
# Name "EmperorOfDragons - Win32 CN_Debug"
# Name "EmperorOfDragons - Win32 CN_Release_Debug"
# Name "EmperorOfDragons - Win32 CN_Release_GM"
# Name "EmperorOfDragons - Win32 JP_Release"
# Name "EmperorOfDragons - Win32 JP_Debug"
# Name "EmperorOfDragons - Win32 JP_Release_Debug"
# Name "EmperorOfDragons - Win32 JP_Release_GM"
# Name "EmperorOfDragons - Win32 RS_Release"
# Name "EmperorOfDragons - Win32 RS_Debug"
# Name "EmperorOfDragons - Win32 RS_Release_Debug"
# Name "EmperorOfDragons - Win32 RS_Release_GM"
# Name "EmperorOfDragons - Win32 TH_Release"
# Name "EmperorOfDragons - Win32 TH_Debug"
# Name "EmperorOfDragons - Win32 TH_Release_Debug"
# Name "EmperorOfDragons - Win32 TH_Release_GM"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Resource\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\EmperorOfDragons.ico
# End Source File
# Begin Source File

SOURCE=.\res\GameMain.ico
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_arrow_diagonal_left.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_arrow_diagonal_right.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_arrow_rightandleft.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_arrow_topandbottom.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_changeview.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_characterselect.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_chatresizeclick.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_chatresizeover.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_choice.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_choice_push.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_combat.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_combat_push.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_CONVERSATION.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_gatecursor.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_lpsclick.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_lpsover.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_mobover.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_mobover_click.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_noway.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_peace.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_peace_push.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_pickup_shut.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_pickup_spread.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_screenmove_down.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_screenmove_downleft.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_screenmove_downright.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_screenmove_left.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_screenmove_right.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_screenmove_up.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_screenmove_upleft.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_screenmove_upright.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_status.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_statusclick.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\mp_warpgate.cur
# End Source File
# Begin Source File

SOURCE=.\res\NineDragons.ico
# End Source File
# Begin Source File

SOURCE=.\res\pause.ico
# End Source File
# Begin Source File

SOURCE=.\res\play.ico
# End Source File
# Begin Source File

SOURCE=.\Resource\reborncl.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\reborncu.cur
# End Source File
# Begin Source File

SOURCE=.\Resource\SandGlass1.cur
# End Source File
# Begin Source File

SOURCE=.\res\stop.ico
# End Source File
# End Group
# Begin Group "Windows"

# PROP Default_Filter ""
# Begin Group "Application Base"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EmperorOfDragons.rc

!IF  "$(CFG)" == "EmperorOfDragons - Win32 KR_Release"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 KR_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 KR_Release_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 KR_Release_GM"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 KR_Test_Release"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 KR_Test_Release_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 KR_Test_Release_GM"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 VN_Release"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 VN_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 VN_Release_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 VN_Release_GM"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 TW_Release"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 TW_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 TW_Release_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 TW_Release_GM"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 US_Release"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 US_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 US_Release_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 US_Release_GM"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 CN_Release"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 CN_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 CN_Release_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 CN_Release_GM"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 JP_Release"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 JP_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 JP_Release_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 JP_Release_GM"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 RS_Release"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 RS_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 RS_Release_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 RS_Release_GM"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 TH_Release"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 TH_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 TH_Release_Debug"

!ELSEIF  "$(CFG)" == "EmperorOfDragons - Win32 TH_Release_GM"

!ENDIF 

# End Source File
# End Group
# Begin Group "Global Define"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XProcess\EODDEF_GlobalObject.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\EODDEF_ModelData.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\EODEXTDEF_GlobalObject.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\EODEXTDEF_ModelData.h
# End Source File
# Begin Source File

SOURCE=.\XSound\SoundEffectList.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XRegistryDef.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\EmperorOfDragons.cpp
# End Source File
# Begin Source File

SOURCE=.\EmperorOfDragons.h
# End Source File
# End Group
# Begin Group "Documents"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Group
# Begin Group "ProcessHandler"

# PROP Default_Filter ""
# Begin Group "ProcessKernel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XProcess\XProc_ProcessDefine.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindowObjectDefine.h
# End Source File
# End Group
# Begin Group "LoginServer"

# PROP Default_Filter ""
# Begin Group "Title"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XProcess\XProc_NineDragonsTitle.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XProc_NineDragonsTitle.h
# End Source File
# End Group
# Begin Group "AcclaimMD5"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Network\XMD5.H
# End Source File
# Begin Source File

SOURCE=.\Network\XMD5c.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\XMD5Global.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\XProcess\XProc_LoginServer.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XProc_LoginServer.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_DrawKeyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_DrawKeyboard.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Login.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Login.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_SelectServerForest.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_SelectServerForest.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_TestServerNotice.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_TestServerNotice.h
# End Source File
# End Group
# Begin Group "Create&SelectCharacter"

# PROP Default_Filter ""
# Begin Group "SubWindow"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XProcess\XWindow_ChangeName.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ChangeName.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CreateCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CreateCharacter.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ExplainCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ExplainCharacter.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_SelectCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_SelectCharacter.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ServerUnification.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ServerUnification.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\XProcess\XProc_CreateCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XProc_CreateCharacter.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XProc_SelectCharacter.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XProc_SelectCharacter.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XProc_ServerUnification.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XProc_ServerUnification.h
# End Source File
# End Group
# Begin Group "Game main"

# PROP Default_Filter ""
# Begin Group "Window"

# PROP Default_Filter ""
# Begin Group "Messenger window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\xprocess\XWindow_Email.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_Email.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Messenger.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Messenger.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_MessengerDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_MessengerDialog.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MessengerTooltip.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MessengerTooltip.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NameInput.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NameInput.h
# End Source File
# End Group
# Begin Group "MessageBox"

# PROP Default_Filter ""
# Begin Group "Training Window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\xprocess\XWindow_Training.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_Training.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_TrainingComplete.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_TrainingComplete.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_TrainingDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_TrainingDummy.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_TrainingHelp.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_TrainingHelp.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_TrainingMed.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_TrainingMed.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\xwindow_trainingspell.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\xwindow_trainingspell.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\XProcess\XMessageWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XMessageWindow.h
# End Source File
# End Group
# Begin Group "Option Window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XProcess\XWindow_Option.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Option.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_OptionCommunity.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_OptionCommunity.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_OptionGame.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_OptionGame.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_OptionGraphic.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_OptionGraphic.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_OptionMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_OptionMenu.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_OptionSound.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_OptionSound.h
# End Source File
# End Group
# Begin Group "HelpSystem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XProcess\XWindow_GuideLine.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_GuideLine.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_Hedek.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_Hedek.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_Help.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_Help.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_HelpBook.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_HelpBook.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_HelpScreen.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_HelpScreen.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Tutorial.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_Tutorial.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_TutorialIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_TutorialIcon.h
# End Source File
# End Group
# Begin Group "GM_Window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XProcess\XWindow_GM_EventManager.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_GM_EventManager.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_GM_MonsterManage.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_GM_MonsterManage.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_GM_SendNoticeMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_GM_SendNoticeMessage.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_GM_StatusControl.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_GM_StatusControl.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_GM_UserCoordination.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_GM_UserCoordination.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_GMCommand.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_GMCommand.h
# End Source File
# End Group
# Begin Group "Party Window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\xprocess\xwindow_partymasterminibar.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_PartyMasterMiniBar.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\xwindow_partymemberminibar.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_PartyMemberMiniBar.h
# End Source File
# End Group
# Begin Group "Match window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XProcess\XWindow_FreeMatchInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_FreeMatchInfo.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MatchEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MatchEvent.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_MatchMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_MatchMessage.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_MatchNoticeLarge.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_MatchNoticeLarge.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_MatchNoticeSmall.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_MatchNoticeSmall.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MatchPR.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MatchPR.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_MatchRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_MatchRecord.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_MatchTablePVP.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_MatchTablePVP.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_PVPBroker.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_PVPBroker.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_RewardNick.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_RewardNick.h
# End Source File
# End Group
# Begin Group "Payment"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XProcess\XWindow_AccountItemUse.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_AccountItemUse.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_PaymentWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_PaymentWindow.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_PCRPremium.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_PCRPremium.h
# End Source File
# End Group
# Begin Group "Cash Window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XProcess\XWindow_CashBox.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CashBox.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CashMall.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CashMall.h
# End Source File
# End Group
# Begin Group "Castle window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XProcess\XWindow_CastleBattlePersonalHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CastleBattlePersonalHistory.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CastleBattleRewardList.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CastleBattleRewardList.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CastleBattleShout.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CastleBattleShout.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CastleBattleTimeChange.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CastleBattleTimeChange.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CastleInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CastleInfo.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CBOrgRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CBOrgRecord.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CBRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CBRecord.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CBResultNotice.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CBResultNotice.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CBRewardSmall.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CBRewardSmall.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ChallengerList.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ChallengerList.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Info.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Info.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Mark.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Mark.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ThreatenGauge.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ThreatenGauge.h
# End Source File
# End Group
# Begin Group "MasternDisciple Window"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XProcess\XMnDStructInfo.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MnDCurrentPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MnDCurrentPoint.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MnDGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MnDGroup.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MnDTotalPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MnDTotalPoint.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCMnDInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCMnDInfo.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\xprocess\xwindow_addsaveitem.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_AddSaveItem.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_AdminNotice.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_AdminNotice.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_AgeGrade.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_AgeGrade.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_AreaSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_AreaSelect.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_BlackList.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_BlackList.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_BloodPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_BloodPoint.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_BookOpen.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_BookOpen.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_CashItem.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_CashItem.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ChargeGauge.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ChargeGauge.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ChatBalloon.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ChatBalloon.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ChattingDefault.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ChattingDefault.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_CInfo.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_Contribution.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_Contribution.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_DeathBlow.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_DeathBlow.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Debug.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Debug.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_DefaultTooltip.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_DefaultTooltip.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_EnterBossRoom.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_EnterBossRoom.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Escape.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Escape.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Event.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Event.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_EventIcon.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_EventIcon.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_EventNotice.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_EventNotice.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Exp.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Exp.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_Gamble.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_Gamble.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_Group.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_Group.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Inchant.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Inchant.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Inventory.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Inventory.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_JinStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_JinStatus.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_LifeSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_LifeSkill.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MainFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MainFrame.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MainMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MainMenu.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MasterySkill.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MasterySkill.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MaximumMap.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MaximumMap.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_MiniTooltip.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_MiniTooltip.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MixWeapon.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MixWeapon.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MonsterStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MonsterStatus.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MoveVillage.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_MoveVillage.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_NickName.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_NickName.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_NPCBuff.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_NPCBuff.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCChatBalloon.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCChatBalloon.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCCollection.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCCollection.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCName.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCName.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCResource.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCResource.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCResource_New.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCResource_New.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCScript.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCScript.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCSearch.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCSkill.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCSkill.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCTrade.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_NPCTrade.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Party.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Party.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_PCTrade.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_PCTrade.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_PersonalStore.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_PersonalStore.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_PK.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_PK.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_PK_ListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_PK_ListBox.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_PKRecord.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_PKRecord.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_PKTracingRequest.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_PKTracingRequest.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_Pocket.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_Pocket.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Poison.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Poison.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_QuestMail.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_QuestMail.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_QuestRank.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_QuestRank.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_QuestWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_QuestWindow.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_QuickSlot.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_QuickSlot.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Rebirth.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Rebirth.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_RecallItem.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_RecallItem.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_RepairItem.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_RepairItem.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_RequestParty.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_RequestParty.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Skill.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Skill.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_SmeltItem.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_SmeltItem.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_SocketDetach.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_SocketDetach.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_SocketPlugIn.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_SocketPlugIn.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_SocketPlugInExp.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_SocketPlugInExp.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_State.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_State.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_SystemMsgDefault.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XWindow_SystemMsgDefault.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_TargetInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_TargetInfo.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_TargetInfoSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_TargetInfoSelect.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_TradeConfirm.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_TradeConfirm.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Warehouse.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_Warehouse.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_WebBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_WebBoard.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_WorldMap.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_WorldMap.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_WorldMinimap.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_WorldMinimap.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ZoneName.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XWindow_ZoneName.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\XProcess\XProc_Loading.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XProc_Loading.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XProc_MainGame.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XProc_MainGame.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XProc_MainGameCallBackFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XProc_MainGameDef.h
# End Source File
# Begin Source File

SOURCE=.\XProcess\XProc_MainGameMessageHandler.cpp
# End Source File
# End Group
# Begin Group "FirstLoad"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XProcess\XProc_FirstLoad.cpp
# End Source File
# Begin Source File

SOURCE=.\XProcess\XProc_FirstLoad.h
# End Source File
# End Group
# Begin Group "Flash"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\datastructure\FlashControlInterface.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XProc_Flash.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XProc_Flash.h
# End Source File
# End Group
# Begin Group "Zero Level"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\xprocess\XProc_SelectHero.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XProc_SelectHero.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XProc_ZeroLevel.cpp
# End Source File
# Begin Source File

SOURCE=.\xprocess\XProc_ZeroLevel.h
# End Source File
# Begin Source File

SOURCE=.\xprocess\XProc_ZeroLevelMessageHandler.cpp
# End Source File
# End Group
# End Group
# Begin Group "DataStructure"

# PROP Default_Filter ""
# Begin Group "Character"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataStructure\XActionTable.h
# End Source File
# Begin Source File

SOURCE=.\datastructure\XCharacterState.cpp
# End Source File
# Begin Source File

SOURCE=.\datastructure\XCharacterState.h
# End Source File
# Begin Source File

SOURCE=.\datastructure\XHeroActionTable.h
# End Source File
# Begin Source File

SOURCE=.\datastructure\XHeroLocalUser.cpp
# End Source File
# Begin Source File

SOURCE=.\datastructure\XHeroLocalUser.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XLifeEventManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XLifeEventManager.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XLocalUser.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XLocalUser.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XLocalUser_Battle.cpp
# End Source File
# Begin Source File

SOURCE=.\datastructure\XLocalUser_Training.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XLocalUser_VirtualMode.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XMob.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XMob.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XModelManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XModelManager.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XNPC.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XNPC.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XUser.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XUser.h
# End Source File
# End Group
# Begin Group "Item"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataStructure\XGameItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XGameItem.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XGameItemDefine.h
# End Source File
# End Group
# Begin Group "Messenger"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataStructure\XMessengerFriend.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XMessengerFriend.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XMessengerGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XMessengerGroup.h
# End Source File
# End Group
# Begin Group "Object"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\datastructure\xdummy.cpp
# End Source File
# Begin Source File

SOURCE=.\datastructure\xdummy.h
# End Source File
# Begin Source File

SOURCE=.\datastructure\XFunctionalObject.cpp
# End Source File
# Begin Source File

SOURCE=.\datastructure\XFunctionalObject.h
# End Source File
# Begin Source File

SOURCE=.\datastructure\xseat.cpp
# End Source File
# Begin Source File

SOURCE=.\datastructure\xseat.h
# End Source File
# End Group
# Begin Group "Notice"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataStructure\Notice\XNoticeProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\Notice\XNoticeProcess.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\Notice\XNoticeScript.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\Notice\XNoticeScript.h
# End Source File
# End Group
# Begin Group "Castle"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataStructure\XCastleBaseType.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XCastleInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XCastleInfo.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XCastleManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XCastleManager.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XCastleScheduler.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XCastleScheduler.h
# End Source File
# End Group
# Begin Group "XML"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataStructure\XMLPaser.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XMLPaser.h
# End Source File
# End Group
# Begin Group "TextureAnimation"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataStructure\XTextureAnimation.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XTextureAnimation.h
# End Source File
# End Group
# Begin Group "TagString"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataStructure\XTagString.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XTagString.h
# End Source File
# End Group
# Begin Group "MatchEvent"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataStructure\XMatchEventBaseType.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XMatchEventManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XMatchEventManager.h
# End Source File
# End Group
# Begin Group "SetItem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataStructure\XSetItemInfomation.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XSetItemInfomation.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\DataStructure\XArea.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XArea.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XAreaManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XAreaManager.h
# End Source File
# Begin Source File

SOURCE=.\datastructure\XBattleLog.cpp
# End Source File
# Begin Source File

SOURCE=.\datastructure\XBattleLog.h
# End Source File
# Begin Source File

SOURCE=.\datastructure\XFame.cpp
# End Source File
# Begin Source File

SOURCE=.\datastructure\XFame.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XFindFileManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XFindFileManager.h
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XObjectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XObjectManager.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XSkillItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XSkillItem.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XSkillMastery.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XSkillMastery.h
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XSR_STRINGHEADER.H
# End Source File
# End Group
# Begin Group "XUtility"

# PROP Default_Filter ""
# Begin Group "Word filtering"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Filtering\XFileEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Filtering\XFileEx.h
# End Source File
# Begin Source File

SOURCE=.\Filtering\XWordFilteringList.cpp
# End Source File
# Begin Source File

SOURCE=.\Filtering\XWordFilteringList.h
# End Source File
# Begin Source File

SOURCE=.\Filtering\XWordFilteringManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Filtering\XWordFilteringManager.h
# End Source File
# End Group
# Begin Group "String Resource control"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataStructure\XStringResourceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XStringResourceManager.h
# End Source File
# End Group
# End Group
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Group "NMClass"

# PROP Default_Filter ""
# Begin Group "ADBalloon"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NMClass\ADBalloon.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\NMClass\NMCOClientObject.h
# End Source File
# Begin Source File

SOURCE=.\NMClass\NMCOHelpers.h
# End Source File
# Begin Source File

SOURCE=.\NMClass\NMCONew.h
# End Source File
# Begin Source File

SOURCE=.\NMClass\NMCRChatRoom.h
# End Source File
# Begin Source File

SOURCE=.\NMClass\NMCSChatSession.h
# End Source File
# Begin Source File

SOURCE=.\NMClass\NMDataObject.h
# End Source File
# Begin Source File

SOURCE=.\NMClass\NMEventHandler.h
# End Source File
# Begin Source File

SOURCE=.\NMClass\NMEventObject.h
# End Source File
# Begin Source File

SOURCE=.\NMClass\NMFDSFileDownloadSession.h
# End Source File
# Begin Source File

SOURCE=.\NMClass\NMFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\NMClass\NMFunctionObject.h
# End Source File
# Begin Source File

SOURCE=.\NMClass\NMFUSFileUploadSession.h
# End Source File
# Begin Source File

SOURCE=.\NMClass\NMGeneral.h
# End Source File
# Begin Source File

SOURCE=.\NMClass\NMGSGeneralSession.h
# End Source File
# Begin Source File

SOURCE=./NMClass\NMManager.h
# End Source File
# Begin Source File

SOURCE=.\NMClass\NMPCH.h
# End Source File
# Begin Source File

SOURCE=.\NMClass\NMSerializable.h
# End Source File
# Begin Source File

SOURCE=.\NMClass\NMWSWBSession.h
# End Source File
# Begin Source File

SOURCE=.\NMClass\PCH.h
# End Source File
# End Group
# Begin Group "HTMLRequest"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Network\Request.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\Request.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Network\Game_packets.h
# End Source File
# Begin Source File

SOURCE=.\Network\netdefine.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_battle.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_boss.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_cashitem.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_cb.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_collection.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_ctrl_gms.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_event.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_items_trade.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_level.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_lobby.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_login.h
# End Source File
# Begin Source File

SOURCE=.\network\packet_for_match.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_messenger.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_MnD.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_nickname.h
# End Source File
# Begin Source File

SOURCE=.\network\packet_for_organization.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_party.h
# End Source File
# Begin Source File

SOURCE=.\network\packet_for_personalstore.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_pk.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_quest.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_skill.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_svrmove.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_unigrouplobby.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_xtrap.h
# End Source File
# Begin Source File

SOURCE=.\Network\packet_for_zone_monster.h
# End Source File
# Begin Source File

SOURCE=.\Network\XNetwork.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\XNetwork.h
# End Source File
# Begin Source File

SOURCE=.\network\XNetwork_Battle.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\XNetwork_Boss.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\XNetwork_cb.cpp
# End Source File
# Begin Source File

SOURCE=.\network\XNetwork_charac_monster.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\XNetwork_Collection.cpp
# End Source File
# Begin Source File

SOURCE=.\network\XNetwork_ctrl_gms.cpp
# End Source File
# Begin Source File

SOURCE=.\network\XNetwork_item_trade.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\XNetwork_MasternDisciple.cpp
# End Source File
# Begin Source File

SOURCE=.\network\XNetwork_Match.cpp
# End Source File
# Begin Source File

SOURCE=.\network\XNetwork_Messenger.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\XNetwork_Nickname.cpp
# End Source File
# Begin Source File

SOURCE=.\network\XNetwork_Organization.cpp
# End Source File
# Begin Source File

SOURCE=.\network\XNetwork_Party.cpp
# End Source File
# Begin Source File

SOURCE=.\network\XNetwork_PersonalStore.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\XNetwork_PKPenalty.cpp
# End Source File
# Begin Source File

SOURCE=.\network\XNetwork_quest.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\XNetwork_UniGroupLobby.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\XNetwork_XTrap.cpp
# End Source File
# Begin Source File

SOURCE=.\Network\XNetworkDefine.h
# End Source File
# End Group
# Begin Group "X3DKernel"

# PROP Default_Filter ""
# Begin Group "Billboard control"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MeshControl\XLensFlare.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XLensFlare.h
# End Source File
# End Group
# Begin Group "XBiped controller"

# PROP Default_Filter ""
# Begin Group "AnimModelContainer"

# PROP Default_Filter ""
# Begin Group "ShaderCode"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ShaderCode\Model.vsh
# End Source File
# Begin Source File

SOURCE=.\ShaderCode\ModelShadow.vsh
# End Source File
# Begin Source File

SOURCE=.\ShaderCode\Normal.vsh
# End Source File
# Begin Source File

SOURCE=.\ShaderCode\NormalPaletteSkin.vsh
# End Source File
# Begin Source File

SOURCE=.\ShaderCode\PaletteSkin.vsh
# End Source File
# Begin Source File

SOURCE=.\ShaderCode\PaletteSkinBumpMap.vsh
# End Source File
# Begin Source File

SOURCE=.\ShaderCode\PaletteSkinShadow.vsh
# End Source File
# Begin Source File

SOURCE=.\ShaderCode\RenderByNormalValue.vsh
# End Source File
# Begin Source File

SOURCE=.\ShaderCode\RigidAniEdge.vsh
# End Source File
# Begin Source File

SOURCE=.\ShaderCode\SilhouetteEdge.vsh
# End Source File
# Begin Source File

SOURCE=.\ShaderCode\SkinEdge.vsh
# End Source File
# Begin Source File

SOURCE=.\ShaderCode\Toon.vsh
# End Source File
# Begin Source File

SOURCE=.\ShaderCode\ToonPaletteSkin.vsh
# End Source File
# Begin Source File

SOURCE=.\ShaderCode\ToonRigidAni.vsh
# End Source File
# End Group
# Begin Source File

SOURCE=.\MeshControl\XAnimMeshContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XAnimMeshContainer.h
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XVSAConstants.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\MeshControl\XBipedController.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XBipedController.h
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XKeyframeController.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XKeyframeController.h
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XKeyframeController_Load.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XKeyframeDescriptor.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XKeyframeDescriptor.h
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XMeshContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XMeshContainer.h
# End Source File
# End Group
# Begin Group "Terrain"

# PROP Default_Filter ""
# Begin Group "ShaderWaterSystem"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=.\MeshControl\XDecalObject.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XDecalObject.h
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XEnvironmentManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XEnvironmentManager.h
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XGrassManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XGrassManager.h
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XLodTerrain.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XLodTerrain.h
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XLodTerrain_TileQuad.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XLodTerrainDefine.h
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XPathObject.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XPathObject.h
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XWater.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XWater.h
# End Source File
# End Group
# Begin Group "Camera"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XControl\X3PCamera.cpp
# End Source File
# Begin Source File

SOURCE=.\XControl\X3PCamera.h
# End Source File
# End Group
# Begin Group "Render"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MeshControl\XRenderStateControl.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XRenderStateControl.h
# End Source File
# End Group
# Begin Group "XEffect"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MeshControl\XEffect_SwordTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XEffect_SwordTrace.h
# End Source File
# End Group
# End Group
# Begin Group "AI Handler"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DataStructure\XPathFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\DataStructure\XPathFinder.h
# End Source File
# End Group
# Begin Group "Script"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Script\XNPCScript.cpp
# End Source File
# Begin Source File

SOURCE=.\Script\XNPCScript.h
# End Source File
# Begin Source File

SOURCE=.\Script\XNPCScriptConditionItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Script\XNPCScriptConditionItem.h
# End Source File
# End Group
# Begin Group "QuestScript"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Script\XQuestScriptBaseType.h
# End Source File
# Begin Source File

SOURCE=.\Script\XQuestScriptDefine.h
# End Source File
# Begin Source File

SOURCE=.\Script\XQuestScriptItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Script\XQuestScriptItem.h
# End Source File
# Begin Source File

SOURCE=.\script\XQuestScriptItemGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\script\XQuestScriptItemGroup.h
# End Source File
# Begin Source File

SOURCE=.\Script\XQuestScriptManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Script\XQuestScriptManager.h
# End Source File
# Begin Source File

SOURCE=.\Script\XTriggerActionItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Script\XTriggerActionItem.h
# End Source File
# Begin Source File

SOURCE=.\Script\XTriggerConditionItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Script\XTriggerConditionItem.h
# End Source File
# Begin Source File

SOURCE=.\Script\XTriggerItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Script\XTriggerItem.h
# End Source File
# Begin Source File

SOURCE=.\script\XTriggerTreeItem.cpp
# End Source File
# Begin Source File

SOURCE=.\script\XTriggerTreeItem.h
# End Source File
# End Group
# Begin Group "XAudioKernel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XSound\XAudioSystem.cpp
# End Source File
# Begin Source File

SOURCE=.\XSound\XAudioSystem.h
# End Source File
# Begin Source File

SOURCE=.\XControl\XFMODWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\XControl\XFMODWrapper.h
# End Source File
# End Group
# Begin Group "XGUI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XControl\XHTMLWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\XControl\XHTMLWrapper.h
# End Source File
# Begin Source File

SOURCE=.\XControl\XMessageBox.cpp
# End Source File
# Begin Source File

SOURCE=.\XControl\XMessageBox.h
# End Source File
# End Group
# Begin Group "SpeedTreeWrapper"

# PROP Default_Filter ""
# Begin Group "SpeedWind"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XSTreeWrapper\PIDController.h
# End Source File
# Begin Source File

SOURCE=.\XSTreeWrapper\SpeedWind.cpp
# End Source File
# Begin Source File

SOURCE=.\XSTreeWrapper\SpeedWind.h
# End Source File
# Begin Source File

SOURCE=.\XSTreeWrapper\SpeedWindBlend.cpp
# End Source File
# Begin Source File

SOURCE=.\XSTreeWrapper\SpeedWindBlend.h
# End Source File
# Begin Source File

SOURCE=.\XSTreeWrapper\SpeedWindParser.cpp
# End Source File
# Begin Source File

SOURCE=.\XSTreeWrapper\SpeedWindParser.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\XSTreeWrapper\XSTShader.h
# End Source File
# Begin Source File

SOURCE=.\XSTreeWrapper\XSTWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\XSTreeWrapper\XSTWrapper.h
# End Source File
# Begin Source File

SOURCE=.\XSTreeWrapper\XSTWrapperConfig.h
# End Source File
# Begin Source File

SOURCE=.\XSTreeWrapper\XSTWrapperCore.cpp
# End Source File
# Begin Source File

SOURCE=.\XSTreeWrapper\XSTWrapperCore.h
# End Source File
# End Group
# Begin Group "Effect kernel"

# PROP Default_Filter ""
# Begin Group "Particle core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XParticleCore\XParticleAction.cpp
# End Source File
# Begin Source File

SOURCE=.\XParticleCore\XParticleAction.h
# End Source File
# Begin Source File

SOURCE=.\XParticleCore\XParticleDefine.h
# End Source File
# Begin Source File

SOURCE=.\xparticlecore\XParticleEffectManager.cpp
# End Source File
# Begin Source File

SOURCE=.\xparticlecore\XParticleEffectManager.h
# End Source File
# Begin Source File

SOURCE=.\XParticleCore\XParticleElement.cpp
# End Source File
# Begin Source File

SOURCE=.\XParticleCore\XParticleElement.h
# End Source File
# Begin Source File

SOURCE=.\XParticleCore\XParticleEmitter.cpp
# End Source File
# Begin Source File

SOURCE=.\XParticleCore\XParticleEmitter.h
# End Source File
# Begin Source File

SOURCE=.\XParticleCore\XParticleIDDefine.h
# End Source File
# Begin Source File

SOURCE=.\XParticleCore\XParticleList.cpp
# End Source File
# Begin Source File

SOURCE=.\XParticleCore\XParticleList.h
# End Source File
# Begin Source File

SOURCE=.\XParticleCore\XParticleMeshObject.cpp
# End Source File
# Begin Source File

SOURCE=.\XParticleCore\XParticleMeshObject.h
# End Source File
# Begin Source File

SOURCE=.\XParticleCore\XParticleRenderManager.cpp
# End Source File
# Begin Source File

SOURCE=.\XParticleCore\XParticleRenderManager.h
# End Source File
# End Group
# Begin Group "Spline container"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XParticleCore\XSplineContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\XParticleCore\XSplineContainer.h
# End Source File
# End Group
# Begin Group "Recycling array"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XParticleCore\XRecyclingArray.h
# End Source File
# End Group
# Begin Group "PostScreenEffect"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MeshControl\XPostProcessCore.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshControl\XPostProcessCore.h
# End Source File
# Begin Source File

SOURCE=.\XParticleCore\XScreenMotionBlur.cpp
# End Source File
# Begin Source File

SOURCE=.\XParticleCore\XScreenMotionBlur.h
# End Source File
# End Group
# End Group
# Begin Group "FunctionalObjectScript"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\script\XFunctionalObjectActionItem.cpp
# End Source File
# Begin Source File

SOURCE=.\script\XFunctionalObjectActionItem.h
# End Source File
# Begin Source File

SOURCE=.\script\XFunctionalObjectConditionItem.cpp
# End Source File
# Begin Source File

SOURCE=.\script\XFunctionalObjectConditionItem.h
# End Source File
# Begin Source File

SOURCE=.\script\XFunctionalObjectScriptDefine.h
# End Source File
# Begin Source File

SOURCE=.\script\XFunctionalObjectTriggerItem.cpp
# End Source File
# Begin Source File

SOURCE=.\script\XFunctionalObjectTriggerItem.h
# End Source File
# End Group
# Begin Group "LifeSkillScript"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Script\XCollectionItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Script\XCollectionItem.h
# End Source File
# Begin Source File

SOURCE=.\Script\XCollectionManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Script\XCollectionManager.h
# End Source File
# Begin Source File

SOURCE=.\Script\XResourceItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Script\XResourceItem.h
# End Source File
# Begin Source File

SOURCE=.\Script\XResourceManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Script\XResourceManager.h
# End Source File
# End Group
# Begin Group "MADWrapper"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\XMADWrapper\MassiveAdObjectSubscriber.h
# End Source File
# Begin Source File

SOURCE=.\XMADWrapper\MassiveClientCore.h
# End Source File
# Begin Source File

SOURCE=.\XMADWrapper\MassiveMediaTypes.h
# End Source File
# Begin Source File

SOURCE=.\XMADWrapper\XMADWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\XMADWrapper\XMADWrapper.h
# End Source File
# Begin Source File

SOURCE=.\XMADWrapper\XMVTSubscriberObject.cpp
# End Source File
# Begin Source File

SOURCE=.\XMADWrapper\XMVTSubscriberObject.h
# End Source File
# End Group
# Begin Group "IGAADWrapper"

# PROP Default_Filter ""
# Begin Group "Bink"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Library\BinkSDK\bink.h
# End Source File
# Begin Source File

SOURCE=.\Library\BinkSDK\binktextures.h
# End Source File
# Begin Source File

SOURCE=.\Library\BinkSDK\dx9rad3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Library\BinkSDK\rad3d.h
# End Source File
# Begin Source File

SOURCE=.\Library\BinkSDK\radbase.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\XIGAADWrapper\Registry.h
# End Source File
# Begin Source File

SOURCE=.\XIGAADWrapper\XIGAADWrapperCore.cpp
# End Source File
# Begin Source File

SOURCE=.\XIGAADWrapper\XIGAADWrapperCore.h
# End Source File
# Begin Source File

SOURCE=.\XIGAADWrapper\XWindow_BinkStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\XIGAADWrapper\XWindow_BinkStatic.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\NineDragons.exe.manifest
# End Source File
# End Target
# End Project
