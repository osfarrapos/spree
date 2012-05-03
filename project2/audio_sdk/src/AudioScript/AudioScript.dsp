# Microsoft Developer Studio Project File - Name="AudioScript" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=AudioScript - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AudioScript.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AudioScript.mak" CFG="AudioScript - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AudioScript - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioScript - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioScript - Win32 Release_include_debugging_information" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioScript - Win32 test_Release_include_debugging_information" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioScript - Win32 test_Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioScript - Win32 VN_test_Release_include_debugging_information" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioScript - Win32 ENG_test_Release_include_debugging_information" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioScript - Win32 tw_Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioScript - Win32 vn_Release" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioScript - Win32 vn_Release_Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "AudioScript - Win32 tw_Release_Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Game_Audio/audio_sdk/src/AudioScript", OPDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AudioScript - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Compiled\release\AudioScript"
# PROP Intermediate_Dir "..\..\..\Compiled\release\AudioScript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\library\AudioScript.lib"

!ELSEIF  "$(CFG)" == "AudioScript - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\Compiled\Debug\AudioScript"
# PROP Intermediate_Dir "..\..\..\Compiled\Debug\AudioScript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\..\include" /I "..\..\..\Utility" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\library\AudioScript_dbg.lib"

!ELSEIF  "$(CFG)" == "AudioScript - Win32 Release_include_debugging_information"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioScript___Win32_Release_include_debugging_information"
# PROP BASE Intermediate_Dir "AudioScript___Win32_Release_include_debugging_information"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Compiled\ReleaseDebug\AudioScript"
# PROP Intermediate_Dir "..\..\..\Compiled\ReleaseDebug\AudioScript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\..\include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /YX /FD /c
# SUBTRACT BASE CPP /Fr
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /FAs /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioScript.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioScript_RD.lib"

!ELSEIF  "$(CFG)" == "AudioScript - Win32 test_Release_include_debugging_information"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioScript___Win32_test_Release_include_debugging_information"
# PROP BASE Intermediate_Dir "AudioScript___Win32_test_Release_include_debugging_information"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Compiled\TestReleaseDebug\AudioScript"
# PROP Intermediate_Dir "..\..\..\Compiled\TestReleaseDebug\AudioScript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /FAs /FR /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XTESTSERVER" /FAs /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioScript_RD.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioScriptTS_RD.lib"

!ELSEIF  "$(CFG)" == "AudioScript - Win32 test_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioScript___Win32_test_Release"
# PROP BASE Intermediate_Dir "AudioScript___Win32_test_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Compiled\testrelease\AudioScript"
# PROP Intermediate_Dir "..\..\..\Compiled\testrelease\AudioScript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XTESTSERVER" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioScript.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioScriptTS.lib"

!ELSEIF  "$(CFG)" == "AudioScript - Win32 VN_test_Release_include_debugging_information"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioScript___Win32_VN_test_Release_include_debugging_information"
# PROP BASE Intermediate_Dir "AudioScript___Win32_VN_test_Release_include_debugging_information"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Compiled\VNTestReleaseDebug\AudioScript"
# PROP Intermediate_Dir "..\..\..\Compiled\VNTestReleaseDebug\AudioScript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XTESTSERVER" /FAs /FR /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XTESTSERVER" /FAs /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioScriptTS_RD.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioScriptTS_RD.lib"

!ELSEIF  "$(CFG)" == "AudioScript - Win32 ENG_test_Release_include_debugging_information"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioScript___Win32_ENG_test_Release_include_debugging_information"
# PROP BASE Intermediate_Dir "AudioScript___Win32_ENG_test_Release_include_debugging_information"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "AudioScript___Win32_ENG_test_Release_include_debugging_information"
# PROP Intermediate_Dir "AudioScript___Win32_ENG_test_Release_include_debugging_information"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XTESTSERVER" /FAs /FR /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XDWDEBUG" /D "_XTESTSERVER" /FAs /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioScriptTS_RD.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioScriptTS_RD.lib"

!ELSEIF  "$(CFG)" == "AudioScript - Win32 tw_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioScript___Win32_tw_Release"
# PROP BASE Intermediate_Dir "AudioScript___Win32_tw_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\Compiled\tw_release\AudioScript"
# PROP Intermediate_Dir "..\..\..\Compiled\tw_release\AudioScript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XTAIWANESE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioScript.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioScript.lib"

!ELSEIF  "$(CFG)" == "AudioScript - Win32 vn_Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioScript___Win32_vn_Release"
# PROP BASE Intermediate_Dir "AudioScript___Win32_vn_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../Compiled/vn_release/AudioScript"
# PROP Intermediate_Dir "../Compiled/vn_release/AudioScript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XTAIWANESE" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XVIETNAMESE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioScript.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioScript.lib"

!ELSEIF  "$(CFG)" == "AudioScript - Win32 vn_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioScript___Win32_vn_Release_Debug"
# PROP BASE Intermediate_Dir "AudioScript___Win32_vn_Release_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/VN_release_debug/AudioScript"
# PROP Intermediate_Dir "Compiled/VN_release_debug/AudioScript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /O2 /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XVIETNAMESE" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XADMINISTRATORMODE" /D "_XDWDEBUG" /D "_XVIETNAMESE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioScript.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioScript.lib"

!ELSEIF  "$(CFG)" == "AudioScript - Win32 tw_Release_Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "AudioScript___Win32_tw_Release_Debug"
# PROP BASE Intermediate_Dir "AudioScript___Win32_tw_Release_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Compiled/tw_release_debug/AudioScript"
# PROP Intermediate_Dir "Compiled/tw_release_debug/AudioScript"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XTAIWANESE" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /GX /Zi /Od /I "..\..\include" /I "..\..\..\Utility" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "USE_VORBIS" /D "NOUSE_WMA" /D "_XADMINISTRATORMODE" /D "_XDWDEBUG" /D "_XTAIWANESE" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\library\AudioScript.lib"
# ADD LIB32 /nologo /out:"..\..\..\library\AudioScript.lib"

!ENDIF 

# Begin Target

# Name "AudioScript - Win32 Release"
# Name "AudioScript - Win32 Debug"
# Name "AudioScript - Win32 Release_include_debugging_information"
# Name "AudioScript - Win32 test_Release_include_debugging_information"
# Name "AudioScript - Win32 test_Release"
# Name "AudioScript - Win32 VN_test_Release_include_debugging_information"
# Name "AudioScript - Win32 ENG_test_Release_include_debugging_information"
# Name "AudioScript - Win32 tw_Release"
# Name "AudioScript - Win32 vn_Release"
# Name "AudioScript - Win32 vn_Release_Debug"
# Name "AudioScript - Win32 tw_Release_Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AudioScriptFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\Lexer.cpp
# End Source File
# Begin Source File

SOURCE=.\MusicMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\Script.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundScape.cpp
# End Source File
# Begin Source File

SOURCE=.\Theme.cpp
# End Source File
# Begin Source File

SOURCE=.\Token.cpp
# End Source File
# Begin Source File

SOURCE=.\TokenFile.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\include\Audio.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AudioScript\AudioScriptFactory.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AudioScript\IMusic.h
# End Source File
# Begin Source File

SOURCE=..\..\include\AudioScript\ISound.h
# End Source File
# Begin Source File

SOURCE=.\Lexer.h
# End Source File
# Begin Source File

SOURCE=.\MusicMgr.h
# End Source File
# Begin Source File

SOURCE=.\Script.h
# End Source File
# Begin Source File

SOURCE=.\SoundMgr.h
# End Source File
# Begin Source File

SOURCE=.\SoundScape.h
# End Source File
# Begin Source File

SOURCE=.\Theme.h
# End Source File
# Begin Source File

SOURCE=.\Token.h
# End Source File
# Begin Source File

SOURCE=.\TokenFile.h
# End Source File
# End Group
# End Target
# End Project
