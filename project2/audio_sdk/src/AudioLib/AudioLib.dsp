# Microsoft Developer Studio Project File - Name="AudioLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=AudioLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AudioLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AudioLib.mak" CFG="AudioLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AudioLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioLib - Win32 Release_include_debugging_information" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioLib - Win32 test_Release_include_debugging_information" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioLib - Win32 test_Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioLib - Win32 VN_test_Release_include_debugging_information" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioLib - Win32 ENG_test_Release_include_debugging_information" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioLib - Win32 tw_Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioLib - Win32 vn_Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioLib - Win32 vn_Release_Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioLib - Win32 tw_Release_Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Game_Audio/audio_sdk/src/AudioLib", LNDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AudioLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Compiled\release\AudioLib"
# PROP Intermediate_Dir "..\..\..\Compiled\release\AudioLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\library\AudioLib.lib"

!ELSEIF  "$(CFG)" == "AudioLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\Compiled\Debug\AudioLib"
# PROP Intermediate_Dir "..\..\..\Compiled\Debug\AudioLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\library\AudioLib_dbg.lib"

!ELSEIF  "$(CFG)" == "AudioLib - Win32 Release_include_debugging_information"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioLib___Win32_Release_include_debugging_information"
# PROP BASE Intermediate_Dir "AudioLib___Win32_Release_include_debugging_information"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Compiled\ReleaseDebug\AudioLib"
# PROP Intermediate_Dir "..\..\..\Compiled\ReleaseDebug\AudioLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\..\include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /FAs /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioLib.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioLib_RD.lib"

!ELSEIF  "$(CFG)" == "AudioLib - Win32 test_Release_include_debugging_information"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioLib___Win32_test_Release_include_debugging_information"
# PROP BASE Intermediate_Dir "AudioLib___Win32_test_Release_include_debugging_information"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Compiled\TestReleaseDebug\AudioLib"
# PROP Intermediate_Dir "..\..\..\Compiled\TestReleaseDebug\AudioLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /FAs /FR /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XTESTSERVER" /FAs /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioLib_RD.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioLibTS_RD.lib"

!ELSEIF  "$(CFG)" == "AudioLib - Win32 test_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioLib___Win32_test_Release"
# PROP BASE Intermediate_Dir "AudioLib___Win32_test_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Compiled\testrelease\AudioLib"
# PROP Intermediate_Dir "..\..\..\Compiled\testrelease\AudioLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XTESTSERVER" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioLib.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioLibTS.lib"

!ELSEIF  "$(CFG)" == "AudioLib - Win32 VN_test_Release_include_debugging_information"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioLib___Win32_VN_test_Release_include_debugging_information"
# PROP BASE Intermediate_Dir "AudioLib___Win32_VN_test_Release_include_debugging_information"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Compiled\VNTestReleaseDebug\AudioLib"
# PROP Intermediate_Dir "..\..\..\Compiled\VNTestReleaseDebug\AudioLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XTESTSERVER" /FAs /FR /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XTESTSERVER" /FAs /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioLibTS_RD.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioLibTS_RD.lib"

!ELSEIF  "$(CFG)" == "AudioLib - Win32 ENG_test_Release_include_debugging_information"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioLib___Win32_ENG_test_Release_include_debugging_information"
# PROP BASE Intermediate_Dir "AudioLib___Win32_ENG_test_Release_include_debugging_information"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AudioLib___Win32_ENG_test_Release_include_debugging_information"
# PROP Intermediate_Dir "AudioLib___Win32_ENG_test_Release_include_debugging_information"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XTESTSERVER" /FAs /FR /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XTESTSERVER" /FAs /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioLibTS_RD.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioLibTS_RD.lib"

!ELSEIF  "$(CFG)" == "AudioLib - Win32 tw_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioLib___Win32_tw_Release"
# PROP BASE Intermediate_Dir "AudioLib___Win32_tw_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Compiled\tw_release\AudioLib"
# PROP Intermediate_Dir "..\..\..\Compiled\tw_release\AudioLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XTAIWANESE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioLib.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioLib.lib"

!ELSEIF  "$(CFG)" == "AudioLib - Win32 vn_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioLib___Win32_vn_Release"
# PROP BASE Intermediate_Dir "AudioLib___Win32_vn_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Compiled/vn_release/AudioLib"
# PROP Intermediate_Dir "../Compiled/vn_release/AudioLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XTAIWANESE" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XVIETNAMESE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioLib.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioLib.lib"

!ELSEIF  "$(CFG)" == "AudioLib - Win32 vn_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioLib___Win32_vn_Release_Debug"
# PROP BASE Intermediate_Dir "AudioLib___Win32_vn_Release_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/VN_release_debug/AudioLib"
# PROP Intermediate_Dir "Compiled/VN_release_debug/AudioLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XVIETNAMESE" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XADMINISTRATORMODE" /D "_XDWDEBUG" /D "_XVIETNAMESE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioLib.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioLib.lib"

!ELSEIF  "$(CFG)" == "AudioLib - Win32 tw_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioLib___Win32_tw_Release_Debug"
# PROP BASE Intermediate_Dir "AudioLib___Win32_tw_Release_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/tw_release_debug/AudioLib"
# PROP Intermediate_Dir "Compiled/tw_release_debug/AudioLib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XTAIWANESE" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\include\AudioLib" /I "..\..\..\Utility" /I "..\..\..\XKernel" /I "..\..\..\XGamebase\Include" /I "..\..\..\XGamebase\Include\Template" /I "..\..\..\XGamebase\src\libs\jpeg" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XADMINISTRATORMODE" /D "_XDWDEBUG" /D "_XTAIWANESE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioLib.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioLib.lib"

!ENDIF 

# Begin Target

# Name "AudioLib - Win32 Release"
# Name "AudioLib - Win32 Debug"
# Name "AudioLib - Win32 Release_include_debugging_information"
# Name "AudioLib - Win32 test_Release_include_debugging_information"
# Name "AudioLib - Win32 test_Release"
# Name "AudioLib - Win32 VN_test_Release_include_debugging_information"
# Name "AudioLib - Win32 ENG_test_Release_include_debugging_information"
# Name "AudioLib - Win32 tw_Release"
# Name "AudioLib - Win32 vn_Release"
# Name "AudioLib - Win32 vn_Release_Debug"
# Name "AudioLib - Win32 tw_Release_Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AudioLibFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\AudioMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\AudioScript.cpp
# End Source File
# Begin Source File

SOURCE=.\AudioStreamFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\BufferCache.cpp
# End Source File
# Begin Source File

SOURCE=.\CDPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Diagnostics.cpp
# End Source File
# Begin Source File

SOURCE=.\DLS.cpp
# End Source File
# Begin Source File

SOURCE=.\Errors.cpp
# End Source File
# Begin Source File

SOURCE=.\FileStream.cpp
# End Source File
# Begin Source File

SOURCE=.\Listener.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoryWave.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertySet.cpp
# End Source File
# Begin Source File

SOURCE=.\Segment.cpp
# End Source File
# Begin Source File

SOURCE=.\Sound.cpp
# End Source File
# Begin Source File

SOURCE=.\Sound3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Utilities.cpp
# End Source File
# Begin Source File

SOURCE=.\Vorbis.cpp
# End Source File
# Begin Source File

SOURCE=.\Wave.cpp
# End Source File
# Begin Source File

SOURCE=.\WMA.cpp
# End Source File
# Begin Source File

SOURCE=.\XAudioPackage.cpp
# End Source File
# Begin Source File

SOURCE=.\XPackageWave.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\AudioLib\3dl2.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AudioLib\3dl2help.h
# End Source File
# Begin Source File

SOURCE=..\..\include\Audio.h
# End Source File
# Begin Source File

SOURCE=.\AudioCommon.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AudioLib\AudioLibFactory.h
# End Source File
# Begin Source File

SOURCE=.\AudioMgr.h
# End Source File
# Begin Source File

SOURCE=.\AudioScript.h
# End Source File
# Begin Source File

SOURCE=.\AudioStreamFactory.h
# End Source File
# Begin Source File

SOURCE=.\BufferCache.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AudioLib\CDPlayer.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AudioLib\Common.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AudioLib\Diagnostics.h
# End Source File
# Begin Source File

SOURCE=.\DLS.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AudioLib\Errors.h
# End Source File
# Begin Source File

SOURCE=.\FileStream.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AudioLib\IAudio.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AudioLib\IAudioExt.h
# End Source File
# Begin Source File

SOURCE=.\IAudioLoader.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AudioLib\IAudioStream.h
# End Source File
# Begin Source File

SOURCE=.\Listener.h
# End Source File
# Begin Source File

SOURCE=.\MemoryWave.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AudioLib\ObjectPool.h
# End Source File
# Begin Source File

SOURCE=.\PropertySet.h
# End Source File
# Begin Source File

SOURCE=.\Segment.h
# End Source File
# Begin Source File

SOURCE=.\Sound.h
# End Source File
# Begin Source File

SOURCE=.\Sound3D.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AudioLib\Types.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AudioLib\Utilities.h
# End Source File
# Begin Source File

SOURCE=.\Vorbis.h
# End Source File
# Begin Source File

SOURCE=.\Wave.h
# End Source File
# Begin Source File

SOURCE=.\WMA.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AudioLib\XAudioPackage.h
# End Source File
# Begin Source File

SOURCE=.\XPackageWave.h
# End Source File
# End Group
# End Target
# End Project
