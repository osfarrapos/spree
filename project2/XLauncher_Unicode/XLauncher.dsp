# Microsoft Developer Studio Project File - Name="XLauncher" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=XLauncher - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XLauncher.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XLauncher.mak" CFG="XLauncher - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XLauncher - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "XLauncher - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "XLauncher - Win32 Release_include_debugging_information" (based on "Win32 (x86) Application")
!MESSAGE "XLauncher - Win32 test_Release" (based on "Win32 (x86) Application")
!MESSAGE "XLauncher - Win32 test_Debug" (based on "Win32 (x86) Application")
!MESSAGE "XLauncher - Win32 tw_Release_include_debugging_information" (based on "Win32 (x86) Application")
!MESSAGE "XLauncher - Win32 vn_Release" (based on "Win32 (x86) Application")
!MESSAGE "XLauncher - Win32 tw_Release" (based on "Win32 (x86) Application")
!MESSAGE "XLauncher - Win32 eu_Release" (based on "Win32 (x86) Application")
!MESSAGE "XLauncher - Win32 us_Release" (based on "Win32 (x86) Application")
!MESSAGE "XLauncher - Win32 tw_Release_TestServer" (based on "Win32 (x86) Application")
!MESSAGE "XLauncher - Win32 jp_Release" (based on "Win32 (x86) Application")
!MESSAGE "XLauncher - Win32 rs_Release" (based on "Win32 (x86) Application")
!MESSAGE "XLauncher - Win32 Release_newtest" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/EmperorOfDragons/XLauncher", ISEBAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XLauncher - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XKOREAN" /D "_XNEXON_WEBLOGIN" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 unicows.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib dxguid.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"C:\Program Files\Nexon\NineDragons\NDLauncher.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "XLauncher - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XUSEXPIDXTABLE_NO" /D "_UNICODE" /D "UNICODE" /D "_XKOREAN" /D "_XNEXON_WEBLOGIN" /FR /FD /GZ /Zm150 /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x412 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib wininet.lib Version.lib imagehlp.lib mp3decoder.lib winmm.lib ./../Library/FMod/fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /out:"..\Game\KR\NDLauncher.exe" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "XLauncher - Win32 Release_include_debugging_information"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XLauncher___Win32_Release_include_debugging_information0"
# PROP BASE Intermediate_Dir "XLauncher___Win32_Release_include_debugging_information0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "XLauncher___Win32_Release_include_debugging_information0"
# PROP Intermediate_Dir "XLauncher___Win32_Release_include_debugging_information0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_XRELEASEDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XKOREAN" /D "_XNEXON_WEBLOGIN" /FR /FD /Zm150 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib wininet.lib Version.lib imagehlp.lib Zip/Zip.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBC" /out:"..\Game\EODLauncher.exe"
# ADD LINK32 Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib dxguid.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:yes /debug /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"..\Game\KR\NDLauncher.exe"

!ELSEIF  "$(CFG)" == "XLauncher - Win32 test_Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XLauncher___Win32_test_Release"
# PROP BASE Intermediate_Dir "XLauncher___Win32_test_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "XLauncher___Win32_test_Release"
# PROP Intermediate_Dir "XLauncher___Win32_test_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /FR /FD /Zm150 /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XKOREAN" /D "_XTESTSERVER" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib wininet.lib Version.lib imagehlp.lib mp3decoder.lib winmm.lib ./../Library/FMod/fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /out:"..\Game\NDLauncher.exe"
# ADD LINK32 unicows.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib dxguid.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"..\Game\KR\tsNDLauncher.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "XLauncher - Win32 test_Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XLauncher___Win32_test_Debug"
# PROP BASE Intermediate_Dir "XLauncher___Win32_test_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "XLauncher___Win32_test_Debug"
# PROP Intermediate_Dir "XLauncher___Win32_test_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_XTESTSERVER" /FR /FD /Zm150 /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MTd /W3 /GX /ZI /Od /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XKOREAN" /D "_XTESTSERVER" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib wininet.lib Version.lib imagehlp.lib mp3decoder.lib winmm.lib ./../Library/FMod/fmodvc.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /out:"..\Game\tsNDLauncher.exe"
# ADD LINK32 Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib dxguid.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"..\Game\KR\tsNDLauncher.exe"

!ELSEIF  "$(CFG)" == "XLauncher - Win32 tw_Release_include_debugging_information"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XLauncher___Win32_tw_Release_include_debugging_information"
# PROP BASE Intermediate_Dir "XLauncher___Win32_tw_Release_include_debugging_information"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "XLauncher___Win32_tw_Release_include_debugging_information"
# PROP Intermediate_Dir "XLauncher___Win32_tw_Release_include_debugging_information"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /ZI /Od /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_XRELEASEDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XTAIWANESE" /FR /FD /Zm150 /c
# ADD CPP /nologo /MT /W3 /GX /ZI /Od /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_XRELEASEDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XTAIWANESE" /FR /FD /Zm150 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib wininet.lib Version.lib imagehlp.lib mp3decoder.lib winmm.lib uuid.lib ./../Library/FMod/fmodvc.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:yes /debug /machine:I386 /nodefaultlib:"LIBC" /out:"..\Game\NDLauncher.exe"
# ADD LINK32 ws2_32.lib wininet.lib Version.lib imagehlp.lib mp3decoder.lib winmm.lib uuid.lib ./../Library/FMod/fmodvc.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /incremental:yes /debug /machine:I386 /nodefaultlib:"LIBC" /out:"..\Game\NDLauncher.exe"

!ELSEIF  "$(CFG)" == "XLauncher - Win32 vn_Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XLauncher___Win32_vn_Release"
# PROP BASE Intermediate_Dir "XLauncher___Win32_vn_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "XLauncher___Win32_vn_Release"
# PROP Intermediate_Dir "XLauncher___Win32_vn_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /FR /FD /Zm150 /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XVIETNAMESE" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib wininet.lib Version.lib imagehlp.lib mp3decoder.lib winmm.lib ./../Library/FMod/fmodvc.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /out:"..\Game\NDLauncher.exe"
# ADD LINK32 UnicoWS.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"..\Game\VN\NDLauncher.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "XLauncher - Win32 tw_Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XLauncher___Win32_tw_Release"
# PROP BASE Intermediate_Dir "XLauncher___Win32_tw_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "XLauncher___Win32_tw_Release"
# PROP Intermediate_Dir "XLauncher___Win32_tw_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XVIETNAMESE" /FR /FD /Zm150 /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XTAIWANESE" /D "_XTW_CSRELEASEVERSION" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ws2_32.lib wininet.lib Version.lib imagehlp.lib mp3decoder.lib winmm.lib ./../Library/FMod/fmodvc.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBC" /out:"..\Game\NDLauncher.exe"
# ADD LINK32 UnicoWS.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"..\Game\TW\NDLauncher.exe"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "XLauncher - Win32 eu_Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XLauncher___Win32_eu_Release"
# PROP BASE Intermediate_Dir "XLauncher___Win32_eu_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "XLauncher___Win32_eu_Release"
# PROP Intermediate_Dir "XLauncher___Win32_eu_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XENGLISH" /D "_XACCLAIM" /FR /FD /Zm150 /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XENGLISH" /D "_XPWC" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 UnicoWS.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"..\Game\NDLauncher.exe"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 UnicoWS.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"..\Game\NDLauncher.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "XLauncher - Win32 us_Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XLauncher___Win32_us_Release0"
# PROP BASE Intermediate_Dir "XLauncher___Win32_us_Release0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "XLauncher___Win32_us_Release"
# PROP Intermediate_Dir "XLauncher___Win32_us_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XVIETNAMESE" /FR /FD /Zm150 /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XENGLISH" /D "_XACCLAIM" /D "_XACCLAIM_NEWWEBPAGE" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 UnicoWS.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"..\Game\VN\NDLauncher.exe"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 UnicoWS.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"..\Game\US\NDLauncher.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "XLauncher - Win32 tw_Release_TestServer"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XLauncher___Win32_tw_Release_TestServer"
# PROP BASE Intermediate_Dir "XLauncher___Win32_tw_Release_TestServer"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "XLauncher___Win32_tw_Release_TestServer"
# PROP Intermediate_Dir "XLauncher___Win32_tw_Release_TestServer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XTAIWANESE" /D "_XTW_CSRELEASEVERSION" /FR /FD /Zm150 /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XTAIWANESE" /D "_XTW_CSTESTVERSION" /D "_XTW_CSRELEASEVERSION" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 UnicoWS.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"..\Game\TW\NDLauncher.exe"
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 UnicoWS.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"..\Game\TW\NDLauncher.exe"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "XLauncher - Win32 jp_Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XLauncher___Win32_jp_Release"
# PROP BASE Intermediate_Dir "XLauncher___Win32_jp_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "XLauncher___Win32_jp_Release"
# PROP Intermediate_Dir "XLauncher___Win32_jp_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XTAIWANESE" /D "_XTW_CSRELEASEVERSION" /FR /FD /Zm150 /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XJAPANESE" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 UnicoWS.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"..\Game\TW\NDLauncher.exe"
# SUBTRACT BASE LINK32 /pdb:none /debug
# ADD LINK32 UnicoWS.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"..\Game\JP\NDLauncher.exe"
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "XLauncher - Win32 rs_Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XLauncher___Win32_rs_Release"
# PROP BASE Intermediate_Dir "XLauncher___Win32_rs_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "XLauncher___Win32_rs_Release"
# PROP Intermediate_Dir "XLauncher___Win32_rs_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XENGLISH" /D "_XACCLAIM" /D "_XACCLAIM_NEWWEBPAGE" /FR /FD /Zm150 /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XRUSSIAN" /D "_XACCLAIM_NEWWEBPAGE" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 UnicoWS.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"..\Game\US\NDLauncher.exe"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 UnicoWS.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"..\Game\RS\NDLauncher.exe"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "XLauncher - Win32 Release_newtest"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XLauncher___Win32_Release_newtest"
# PROP BASE Intermediate_Dir "XLauncher___Win32_Release_newtest"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "XLauncher___Win32_Release_newtest"
# PROP Intermediate_Dir "XLauncher___Win32_Release_newtest"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /Zi /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XKOREAN" /D "_XNEXON_WEBLOGIN" /FR /FD /Zm150 /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "./" /I "./FlashCtrl" /I "./FlashCtrl/Jpeg" /I "./FlashCtrl/Win32" /I "./FlashCtrl/Zlib" /I "./Zip" /I "./WebController" /I "./Library/FMod" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "FSCONTROL_EXPORTS" /D "USE_JPEG" /D "FRAMESPERSEC" /D "SOUND" /D "EDITTEXT" /D "FLASHMP3" /D "FIXEDSIZE" /D "_UNICODE" /D "UNICODE" /D "_XKOREAN" /D "_XNEXON_WEBLOGIN" /D "_XNEXON_NEWTESTVERSION" /FR /FD /Zm150 /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x412 /d "NDEBUG"
# ADD RSC /l 0x412 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 unicows.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib dxguid.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"C:\Program Files\Nexon\NineDragons\NDLauncher.exe"
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 unicows.lib Kernel32.lib Advapi32.lib User32.lib Gdi32.lib Shell32.lib Comdlg32.lib Version.lib Mpr.lib Rasapi32.lib Winmm.lib Winspool.lib Vfw32.lib Secur32.lib Oleacc.lib Oledlg.lib Sensapi.lib ws2_32.lib wininet.lib imagehlp.lib mp3decoder.lib ./../Library/FMod/fmodvc.lib dxguid.lib d3d9.lib d3dx9.lib /nologo /entry:"wWinMainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"LIBC" /out:"C:\Program Files\Nexon\NineDragons\NDLauncher.exe"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "XLauncher - Win32 Release"
# Name "XLauncher - Win32 Debug"
# Name "XLauncher - Win32 Release_include_debugging_information"
# Name "XLauncher - Win32 test_Release"
# Name "XLauncher - Win32 test_Debug"
# Name "XLauncher - Win32 tw_Release_include_debugging_information"
# Name "XLauncher - Win32 vn_Release"
# Name "XLauncher - Win32 tw_Release"
# Name "XLauncher - Win32 eu_Release"
# Name "XLauncher - Win32 us_Release"
# Name "XLauncher - Win32 tw_Release_TestServer"
# Name "XLauncher - Win32 jp_Release"
# Name "XLauncher - Win32 rs_Release"
# Name "XLauncher - Win32 Release_newtest"
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\center.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Close.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CoLogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ColumnHeaderEnd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ColumnHeaderSpan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ColumnHeaderStart.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\eu_CoLogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\highquality_highlight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\highquality_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HorizontalScrollBarLeftArrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HorizontalScrollBarRightArrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HorizontalScrollBarSpan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HorizontalScrollBarThumb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\jp_CoLogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\kr_CoLogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\launcher_button_connect_clicked.bmp
# End Source File
# Begin Source File

SOURCE=.\res\launcher_button_connect_highlight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\launcher_button_connect_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\launcher_button_end_highlight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\launcher_button_end_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\launcher_button_join_highlight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\launcher_button_join_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\launcher_button_option_highlight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\launcher_button_option_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\left.bmp
# End Source File
# Begin Source File

SOURCE=.\res\list_bot.bmp
# End Source File
# Begin Source File

SOURCE=.\res\list_left.bmp
# End Source File
# Begin Source File

SOURCE=.\res\list_right.bmp
# End Source File
# Begin Source File

SOURCE=.\res\list_top.bmp
# End Source File
# Begin Source File

SOURCE=.\res\lowquality_highlight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\lowquality_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MainIcon.ico
# End Source File
# Begin Source File

SOURCE=.\res\midquality_highlight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\midquality_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\miniclose_highlight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\miniclose_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\notice_button_close_highlight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\notice_button_close_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\notice_small.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NoticeCloseButton.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NoticeTitle.bmp
# End Source File
# Begin Source File

SOURCE=.\IMageResource\OptionBorder.bmp
# End Source File
# Begin Source File

SOURCE=.\res\OptionBorder.bmp
# End Source File
# Begin Source File

SOURCE=.\res\optionCancel_Hightlight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\optionCancel_Normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\optionOK_Hightlight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\optionOK_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\patchslider.bmp
# End Source File
# Begin Source File

SOURCE=.\res\patchsliderback.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PopupBorder.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ProgressbarBackBorder.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ProgressbarBackBorderTitle.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Resolution.bmp
# End Source File
# Begin Source File

SOURCE=.\res\right.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rightdown.bmp
# End Source File
# Begin Source File

SOURCE=.\res\rs_CoLogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\selectserverbtn_highlight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\selectserverbtn_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ServerSelect_CloseHighlight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ServerSelect_CloseNormal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ServerSelect_RefreshHighlight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ServerSelect_RefreshNormal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ShowPatchInfo_Hightlight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ShowPatchInfo_Normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tw_colog.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tw_CoLogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\update_small.bmp
# End Source File
# Begin Source File

SOURCE=.\res\UpdateBorder.bmp
# End Source File
# Begin Source File

SOURCE=.\res\UpdateTitle.bmp
# End Source File
# Begin Source File

SOURCE=.\res\us_CoLogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VerticleScrollbarBottom.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VerticleScrollBarDownArrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VerticleScrollBarSpan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VerticleScrollBarThumb.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VerticleScrollbarTop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\VerticleScrollBarUpArrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\vn_CoLogo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WebBorder.bmp
# End Source File
# Begin Source File

SOURCE=.\res\XLauncher.ico
# End Source File
# Begin Source File

SOURCE=.\res\XLauncher.rc2
# End Source File
# End Group
# Begin Group "Windows"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CheckVersionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckVersionDlg.h
# End Source File
# Begin Source File

SOURCE=.\noticedialog.cpp
# End Source File
# Begin Source File

SOURCE=.\noticedialog.h
# End Source File
# Begin Source File

SOURCE=.\NotInstalledDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\NotInstalledDialog.h
# End Source File
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

SOURCE=.\XLauncher.cpp
# End Source File
# Begin Source File

SOURCE=.\XLauncher.h
# End Source File
# Begin Source File

SOURCE=.\XLauncher.rc
# End Source File
# Begin Source File

SOURCE=.\XLauncherDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\XLauncherDlg.h
# End Source File
# Begin Source File

SOURCE=.\XRegistry.cpp
# End Source File
# Begin Source File

SOURCE=.\XRegistry.h
# End Source File
# Begin Source File

SOURCE=.\XRegistryDef.h
# End Source File
# End Group
# Begin Group "MFC Class"

# PROP Default_Filter ""
# Begin Group "WebController"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\WebController\custsite.cpp
# End Source File
# Begin Source File

SOURCE=.\WebController\custsite.h
# End Source File
# Begin Source File

SOURCE=.\WebController\idispimp.cpp
# End Source File
# Begin Source File

SOURCE=.\WebController\idispimp.h
# End Source File
# Begin Source File

SOURCE=.\WebController\webbro.cpp
# End Source File
# Begin Source File

SOURCE=.\WebController\webbro.h
# End Source File
# End Group
# Begin Group "MarkUp"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Markup.cpp
# End Source File
# Begin Source File

SOURCE=.\Markup.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BCMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\BCMenu.h
# End Source File
# Begin Source File

SOURCE=.\BitComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\BitComboBox.h
# End Source File
# Begin Source File

SOURCE=.\BitListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\BitListBox.h
# End Source File
# Begin Source File

SOURCE=.\BitmapDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapDialog.h
# End Source File
# Begin Source File

SOURCE=.\BitmapSlider.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapSlider.h
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

SOURCE=.\GradientProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GradientProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\KCBusyProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\KCBusyProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Label.cpp
# End Source File
# Begin Source File

SOURCE=.\Label.h
# End Source File
# Begin Source File

SOURCE=.\MacButtons.cpp
# End Source File
# Begin Source File

SOURCE=.\MacButtons.h
# End Source File
# Begin Source File

SOURCE=.\memdc.h
# End Source File
# Begin Source File

SOURCE=.\SeverMessageDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SeverMessageDialog.h
# End Source File
# Begin Source File

SOURCE=.\ShadeButtonST.cpp
# End Source File
# Begin Source File

SOURCE=.\ShadeButtonST.h
# End Source File
# Begin Source File

SOURCE=.\SkinHeaderCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SkinHorizontalScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinHorizontalScrollbar.h
# End Source File
# Begin Source File

SOURCE=.\SkinListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SkinVerticleScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinVerticleScrollbar.h
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\TextProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\XBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\XBrowser.h
# End Source File
# Begin Source File

SOURCE=.\xDib.cpp
# End Source File
# Begin Source File

SOURCE=.\xDib.h
# End Source File
# Begin Source File

SOURCE=.\XProgressBar.cpp
# End Source File
# Begin Source File

SOURCE=.\XProgressBar.h
# End Source File
# End Group
# Begin Group "Serial"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EnterSerialDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EnterSerialDlg.h
# End Source File
# Begin Source File

SOURCE=.\XEncryption.cpp
# End Source File
# Begin Source File

SOURCE=.\XEncryption.h
# End Source File
# Begin Source File

SOURCE=.\XEncryptiondata.h
# End Source File
# End Group
# Begin Group "Zip"

# PROP Default_Filter ""
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
# Begin Group "FTP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FTPOptionsDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\FTPOptionsDialog.h
# End Source File
# Begin Source File

SOURCE=.\FTPTransferDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FTPTransferDlg.h
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
# Begin Group "HTML View"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DLGHTML.cpp
# End Source File
# Begin Source File

SOURCE=.\DLGHTML.h
# End Source File
# End Group
# Begin Group "HTTPDownload"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ProxyIBindStatusCallback.h
# End Source File
# Begin Source File

SOURCE=.\XFileDownload.cpp
# End Source File
# Begin Source File

SOURCE=.\XFileDownload.h
# End Source File
# End Group
# Begin Group "FlashControl"

# PROP Default_Filter ""
# Begin Group "ZLib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FlashCtrl\zlib\adler32.c
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\compress.c
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\crc32.c
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\crc32.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\deflate.c
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\deflate.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\gzio.c
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\infback.c
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\inffast.c
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\inffast.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\inffixed.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\inflate.c
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\inflate.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\inftrees.c
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\trees.c
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\trees.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\uncompr.c
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\zconf.in.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\zutil.c
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\zlib\zutil.h
# End Source File
# End Group
# Begin Group "Jpeg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\cderror.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\cdjpeg.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jchuff.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jcomapi.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jconfig.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jdapimin.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jdapistd.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jdcoefct.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jdcolor.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jdct.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jddctmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jdhuff.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jdhuff.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jdinput.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jdmainct.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jdmarker.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jdmaster.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jdpostct.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jdsample.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jfdctint.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jidctint.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jmemmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jmemnobs.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jmemsys.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jpegint.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jpeglib.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jutils.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\JPEG\jversion.h
# End Source File
# End Group
# Begin Group "FlashSource"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FlashCtrl\win32\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\win32\Display.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\FlashControlImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\FlashStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\win32\FSControl.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\win32\palette.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\win32\sound.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\win32\util.cpp
# End Source File
# End Group
# Begin Group "FlashHeader"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FlashCtrl\win32\debug.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\win32\display.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\win32\fixed.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\FlashControl.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\FlashControlImpl.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\FlashStatic.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\win32\FSControl.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\win32\palette.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\win32\Sound.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\win32\util.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\win32\version.h
# End Source File
# End Group
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FlashCtrl\bitbuf.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\Bitbuf.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\bitstream.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\curve.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\curve.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\edittext.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\edittext.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\fixed.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\fixed.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\genericfonts.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\geom.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\geom.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\global.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\global.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\jpeg.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\jpeg.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\mcontext.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\memcop.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\memcop.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\morphinter.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\morphinter.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\mp3decifc.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\mp3decoder.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\mp3decoder.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\mp3sscdef.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\mp3streaminfo.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\mpegbitstream.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\mpegheader.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\raster.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\raster.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\sbitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\sbitmap.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\sdisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\sdisplay.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\shape.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\sndcodec.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\sndcodec.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\sndmix.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\sndmix.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\sobject.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\sobject.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\splay.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\splay.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\splayer.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\splayer.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\sstroker.cpp
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\sstroker.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\stags.h
# End Source File
# Begin Source File

SOURCE=.\FlashCtrl\unixfonts.cpp
# End Source File
# End Group
# End Group
# Begin Group "D3DClass"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\d3denumeration.cpp
# End Source File
# Begin Source File

SOURCE=.\d3denumeration.h
# End Source File
# Begin Source File

SOURCE=.\d3dutil.cpp
# End Source File
# Begin Source File

SOURCE=.\d3dutil.h
# End Source File
# Begin Source File

SOURCE=.\dxutil.cpp
# End Source File
# Begin Source File

SOURCE=.\dxutil.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\eu_interface.txt
# End Source File
# Begin Source File

SOURCE=.\res\jp_interface.txt
# End Source File
# Begin Source File

SOURCE=.\res\kr_interface.txt
# End Source File
# Begin Source File

SOURCE=.\res\Launcherbutton.wav
# End Source File
# Begin Source File

SOURCE=.\res\luncherStart.wav
# End Source File
# Begin Source File

SOURCE=.\res\PatchComplete.wav
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\ResChangeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ResChangeDialog.h
# End Source File
# Begin Source File

SOURCE=.\res\rs_interface.txt
# End Source File
# Begin Source File

SOURCE=.\SelectServer.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectServer.h
# End Source File
# Begin Source File

SOURCE=.\res\StartGame.wav
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
# Section XLauncher : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowser2
# 	2:10:HeaderFile:webbrowser2.h
# 	2:8:ImplFile:webbrowser2.cpp
# End Section
# Section XLauncher : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser2.h
# 	2:16:DefaultSinkClass:CWebBrowser2
# End Section
