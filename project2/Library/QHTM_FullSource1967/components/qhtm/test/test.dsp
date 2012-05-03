# Microsoft Developer Studio Project File - Name="Test" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Test - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Test.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Test.mak" CFG="Test - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Test - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Test - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/components/QHTM/Test", NVHAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Test - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "QHTM_NO_LINK_LIBS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"../../../bin/QHTM_MFC_Test.exe" /libpath:"../../../bin/"

!ELSEIF  "$(CFG)" == "Test - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "QHTM_NO_LINK_LIBS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"../../../bin/QHTM_MFC_TestD.exe" /pdbtype:sept /libpath:"../../../bin/"

!ENDIF 

# Begin Target

# Name "Test - Win32 Release"
# Name "Test - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "docking control bars"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\docking control bar\scbarcf.cpp"
# End Source File
# Begin Source File

SOURCE=".\docking control bar\scbarcf.h"
# End Source File
# Begin Source File

SOURCE=".\docking control bar\scbarg.cpp"
# End Source File
# Begin Source File

SOURCE=".\docking control bar\scbarg.h"
# End Source File
# Begin Source File

SOURCE=".\docking control bar\sizecbar.cpp"
# End Source File
# Begin Source File

SOURCE=".\docking control bar\sizecbar.h"
# End Source File
# End Group
# Begin Group "Test Files"

# PROP Default_Filter ""
# Begin Group "forms"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Test files\forms\FORM-in-table.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\forms\form-ISINDEX.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\forms\form-label.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\forms\form-select-listbox.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\forms\form-select.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\forms\form-within-table.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\forms\form1.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\forms\form2.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\forms\form3.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\forms\form4.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\forms\form5.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\forms\form6.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\forms\form7.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\forms\form8.html"
# End Source File
# End Group
# Begin Group "misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Test files\misc\ansi_document.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\blockquote.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\bold.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\Break.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\center.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\Comment-test-WordHtml.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\comment2.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\Comments.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\control-edit.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\dir.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\DL.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\font-background-color.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\FONT-plus-minus.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\fonts.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\frames1.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\frames2.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\frames3.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\frames4.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\Headings.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\HR.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\hypenation.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\image with percent width.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\Link-ID.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\link-tip-test.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\long-links-focus.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\MainMenu.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\Measuring-bug.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\menu.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\multiline-edit.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\Para-test.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\Paragraph.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\PRE.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\quoteapp1.htm"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\scrolling links test.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\slow-test.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\styles.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\transcript1.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\TT.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\UL.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\unicode_document.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\UTF-8 Sampler.htm"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\we come in peace.htm"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\welcome.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\whitespace-begining.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\misc\whitespace-between-inputs.html"
# End Source File
# End Group
# Begin Group "scrambled foreign chars"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Test files\scrambled foreign chars\qhtmtest.bmp"
# End Source File
# Begin Source File

SOURCE=".\Test files\scrambled foreign chars\qhtmtest.html"
# End Source File
# End Group
# Begin Group "style"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Test files\style\font-size-percent.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\style\font-size.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\style\list.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\style\margins.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\style\pseudo-elements.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\style\selectors.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\style\table borders.html"
# End Source File
# End Group
# Begin Group "tables"

# PROP Default_Filter ""
# Begin Group "colspan"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\Test files\tables\COLSPAN\colspan.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\tables\COLSPAN\colspan1.html"
# End Source File
# End Group
# Begin Source File

SOURCE=".\Test files\tables\Background image.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\tables\table content alignment.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\tables\table without TR crash.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\tables\table-alignment.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\tables\table-colours.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\tables\Table-height.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\tables\table-row-height.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\tables\table-within-table.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\tables\table-wrap.html"
# End Source File
# Begin Source File

SOURCE=".\Test files\tables\TableCellColor.html"
# End Source File
# End Group
# End Group
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Test.cpp
# End Source File
# Begin Source File

SOURCE=.\Test.rc
# End Source File
# Begin Source File

SOURCE=.\TestDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\TestList.cpp
# End Source File
# Begin Source File

SOURCE=.\TestList.h
# End Source File
# Begin Source File

SOURCE=.\TestView.cpp
# End Source File
# Begin Source File

SOURCE=.\TestView.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Test.h
# End Source File
# Begin Source File

SOURCE=.\TestDoc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Test.ico
# End Source File
# Begin Source File

SOURCE=.\res\Test.rc2
# End Source File
# Begin Source File

SOURCE=.\res\TestDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
