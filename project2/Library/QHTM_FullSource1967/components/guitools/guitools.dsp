# Microsoft Developer Studio Project File - Name="guitools" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=guitools - Win32 Debug UNICODE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "guitools.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "guitools.mak" CFG="guitools - Win32 Debug UNICODE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "guitools - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "guitools - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "guitools - Win32 Debug UNICODE" (based on "Win32 (x86) Static Library")
!MESSAGE "guitools - Win32 Release UNICODE" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/components/guitools", NDHAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "guitools - Win32 Release"

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
# ADD CPP /nologo /MD /W4 /Ox /Oa /Ow /Og /Oi /Os /Op /I "../" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /FAs /FR /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\bin\\guitools.lib"

!ELSEIF  "$(CFG)" == "guitools - Win32 Debug"

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
# ADD CPP /nologo /MTd /W4 /Gm /ZI /Od /I "../" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\bin\\guitoolsD.lib"

!ELSEIF  "$(CFG)" == "guitools - Win32 Debug UNICODE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "guitools___Win32_Debug_UNICODE"
# PROP BASE Intermediate_Dir "guitools___Win32_Debug_UNICODE"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "guitools___Win32_Debug_UNICODE"
# PROP Intermediate_Dir "guitools___Win32_Debug_UNICODE"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /Gm /GX /ZI /Od /I "../" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /Gm /ZI /Od /I "../" /D "_DEBUG" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\bin\\guitoolsD.lib"
# ADD LIB32 /nologo /out:"..\..\bin\\guitoolsUD.lib"

!ELSEIF  "$(CFG)" == "guitools - Win32 Release UNICODE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "guitools___Win32_Release_UNICODE"
# PROP BASE Intermediate_Dir "guitools___Win32_Release_UNICODE"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "guitools___Win32_Release_UNICODE"
# PROP Intermediate_Dir "guitools___Win32_Release_UNICODE"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /GX /O2 /I "../" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W4 /O2 /I "../" /D "NDEBUG" /D "UNICODE" /D "_UNICODE" /D "WIN32" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\bin\\guitools.lib"
# ADD LIB32 /nologo /out:"..\..\bin\\guitoolsU.lib"

!ENDIF 

# Begin Target

# Name "guitools - Win32 Release"
# Name "guitools - Win32 Debug"
# Name "guitools - Win32 Debug UNICODE"
# Name "guitools - Win32 Release UNICODE"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CompatibleDC.h
# End Source File
# Begin Source File

SOURCE=.\CreateRoundCornerRegion.cpp
# End Source File
# Begin Source File

SOURCE=.\DIB.cpp
# End Source File
# Begin Source File

SOURCE=.\DIB.h
# End Source File
# Begin Source File

SOURCE=.\DIBAlphaBlend.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBBlur.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBClearToColour.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBConvertToGrayScale.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBCreateCopy.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBCreateRegionFromTransparency.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBCreateResized.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBDrawBitmapOntoUs.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBDrawLine.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBDrawTransparent.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBDrawWithAlphaChannel.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBFillRect.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBGetPixel.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBGradientFillLeftToRight.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBGradientFillTopToBottom.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBPutPixel.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBPutPixelBlend.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBRectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBSetColourTransparent.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawBitmapShadow.cpp
# End Source File
# Begin Source File

SOURCE=.\drawcontext.cpp
# End Source File
# Begin Source File

SOURCE=.\drawcontext.h
# End Source File
# Begin Source File

SOURCE=.\DrawContext.inl
# End Source File
# Begin Source File

SOURCE=.\FontDef.h
# End Source File
# Begin Source File

SOURCE=.\GradientFill.cpp
# End Source File
# Begin Source File

SOURCE=.\GradientFill.h
# End Source File
# Begin Source File

SOURCE=.\GuiBaseObject.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiBaseObject.h
# End Source File
# Begin Source File

SOURCE=.\GuiBaseWindowObject.cpp
# End Source File
# Begin Source File

SOURCE=.\GuiBaseWindowObject.h
# End Source File
# Begin Source File

SOURCE=.\GuiHotMixin.h
# End Source File
# Begin Source File

SOURCE=.\GuiTools.h
# End Source File
# Begin Source File

SOURCE=.\Guitooltip.cpp
# End Source File
# Begin Source File

SOURCE=.\Guitooltip.h
# End Source File
# Begin Source File

SOURCE=.\HandleMouseWheelRegisteredMessage.cpp
# End Source File
# Begin Source File

SOURCE=.\palette.cpp
# End Source File
# Begin Source File

SOURCE=.\palette.h
# End Source File
# Begin Source File

SOURCE=.\RadialGradient.cpp
# End Source File
# Begin Source File

SOURCE=.\SmoothAnimationCalc.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Readme.txt
# End Source File
# End Target
# End Project
