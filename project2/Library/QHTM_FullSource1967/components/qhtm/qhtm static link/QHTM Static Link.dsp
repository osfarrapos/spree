# Microsoft Developer Studio Project File - Name="QHTM Static Link" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=QHTM Static Link - Win32 Debug UNICODE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "QHTM Static Link.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "QHTM Static Link.mak" CFG="QHTM Static Link - Win32 Debug UNICODE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "QHTM Static Link - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "QHTM Static Link - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "QHTM Static Link - Win32 Debug UNICODE" (based on "Win32 (x86) Static Library")
!MESSAGE "QHTM Static Link - Win32 Release UNICODE" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/components/QHTM/QHTM Static Link", KJDAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "QHTM Static Link - Win32 Release"

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
# ADD CPP /nologo /MD /W4 /WX /Ox /Ot /Oa /Ow /Og /Oi /Op /I "..\\" /I "..\..\\" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "QHTM_NO_LINK_LIBS" /FR /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Os
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\bin\QHTMs.lib"

!ELSEIF  "$(CFG)" == "QHTM Static Link - Win32 Debug"

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
# ADD CPP /nologo /MTd /W4 /WX /Gm /ZI /Od /I "..\..\\" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "WIN32" /D "QHTM_NO_LINK_LIBS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\..\bin\QHTMsd.lib"

!ELSEIF  "$(CFG)" == "QHTM Static Link - Win32 Debug UNICODE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "QHTM_Static_Link___Win32_Debug_UNICODE"
# PROP BASE Intermediate_Dir "QHTM_Static_Link___Win32_Debug_UNICODE"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "QHTM_Static_Link___Win32_Debug_UNICODE"
# PROP Intermediate_Dir "QHTM_Static_Link___Win32_Debug_UNICODE"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W4 /WX /Gm /GX /ZI /Od /I "..\..\\" /D "_MBCS" /D "_LIB" /D "WIN32" /D "_DEBUG" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W4 /WX /Gm /ZI /Od /I "..\..\\" /D "_DEBUG" /D "UNICODE" /D "_UNICODE" /D "_MBCS" /D "_LIB" /D "WIN32" /D "QHTM_NO_LINK_LIBS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\bin\QHTMsd.lib"
# ADD LIB32 /nologo /out:"..\..\..\bin\QHTMUsd.lib"

!ELSEIF  "$(CFG)" == "QHTM Static Link - Win32 Release UNICODE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "QHTM_Static_Link___Win32_Release_UNICODE"
# PROP BASE Intermediate_Dir "QHTM_Static_Link___Win32_Release_UNICODE"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "QHTM_Static_Link___Win32_Release_UNICODE"
# PROP Intermediate_Dir "QHTM_Static_Link___Win32_Release_UNICODE"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W4 /WX /GX /Ox /Ot /Oa /Ow /Og /Oi /Op /I "..\\" /I "..\..\\" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /Yu"stdafx.h" /FD /c
# SUBTRACT BASE CPP /Os
# ADD CPP /nologo /MT /W4 /WX /Ox /Ot /Oa /Ow /Og /Oi /Op /I "..\\" /I "..\..\\" /D "NDEBUG" /D "UNICODE" /D "_UNICODE" /D "_MBCS" /D "_LIB" /D "WIN32" /D "QHTM_NO_LINK_LIBS" /FR /Yu"stdafx.h" /FD /c
# SUBTRACT CPP /Os
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\..\..\bin\QHTMs.lib"
# ADD LIB32 /nologo /out:"..\..\..\bin\QHTMUs.lib"

!ENDIF 

# Begin Target

# Name "QHTM Static Link - Win32 Release"
# Name "QHTM Static Link - Win32 Debug"
# Name "QHTM Static Link - Win32 Debug UNICODE"
# Name "QHTM Static Link - Win32 Release UNICODE"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Display"

# PROP Default_Filter ""
# Begin Group "Form"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\FormDisplayElementABC.h
# End Source File
# Begin Source File

SOURCE=..\HTMLButtonSection.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLButtonSection.h
# End Source File
# Begin Source File

SOURCE=..\HTMLComboSection.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLComboSection.h
# End Source File
# Begin Source File

SOURCE=..\HTMLEditSection.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLEditSection.h
# End Source File
# Begin Source File

SOURCE=..\HTMLFormObjectABC.h
# End Source File
# Begin Source File

SOURCE=..\HTMLListboxSection.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLListboxSection.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\FocusControl.h
# End Source File
# Begin Source File

SOURCE=..\htmlcontrol.cpp
# End Source File
# Begin Source File

SOURCE=..\htmlcontrolsection.cpp
# End Source File
# Begin Source File

SOURCE=..\htmlhorizontalrulesection.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLHorizontalRuleSection.h
# End Source File
# Begin Source File

SOURCE=..\htmlimagesection.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLImageSection.h
# End Source File
# Begin Source File

SOURCE=..\htmlsection.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLSection.h
# End Source File
# Begin Source File

SOURCE=..\htmlsectionabc.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLSectionABC.h
# End Source File
# Begin Source File

SOURCE=..\htmlsectioncreator.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLSectionCreator.h
# End Source File
# Begin Source File

SOURCE=..\htmlsectionlink.cpp
# End Source File
# Begin Source File

SOURCE=..\HtmlSectionLink.h
# End Source File
# Begin Source File

SOURCE=..\htmltablesection.cpp
# End Source File
# Begin Source File

SOURCE=..\htmltablesection.h
# End Source File
# Begin Source File

SOURCE=..\htmltextsection.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLTextSection.h
# End Source File
# Begin Source File

SOURCE=..\MessageReflector.cpp
# End Source File
# Begin Source File

SOURCE=..\parentsection.cpp
# End Source File
# Begin Source File

SOURCE=..\ParentSection.h
# End Source File
# Begin Source File

SOURCE=..\qhtmcontrolsection.cpp
# End Source File
# Begin Source File

SOURCE=..\QHTMControlSection.h
# End Source File
# Begin Source File

SOURCE=..\scrollcontainer.cpp
# End Source File
# Begin Source File

SOURCE=..\ScrollContainer.h
# End Source File
# Begin Source File

SOURCE=..\sectionabc.cpp
# End Source File
# Begin Source File

SOURCE=..\sectionabc.h
# End Source File
# Begin Source File

SOURCE=..\SubClassControl.cpp
# End Source File
# Begin Source File

SOURCE=..\tablelayout.cpp
# End Source File
# Begin Source File

SOURCE=..\tablelayout.h
# End Source File
# Begin Source File

SOURCE=..\tipwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\TipWindow.h
# End Source File
# End Group
# Begin Group "HTML Document"

# PROP Default_Filter ""
# Begin Group "Utility Functions"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\decodecharset.cpp
# End Source File
# Begin Source File

SOURCE=..\getcolourfromstring.cpp
# End Source File
# Begin Source File

SOURCE=..\getdisplayworkarea.cpp
# End Source File
# Begin Source File

SOURCE=..\getfontsize.cpp
# End Source File
# Begin Source File

SOURCE=..\getfontsizeaspixels.cpp
# End Source File
# Begin Source File

SOURCE=..\getnumberparameter.cpp
# End Source File
# Begin Source File

SOURCE=..\ParseStyles.cpp
# End Source File
# End Group
# Begin Group "HTML Form"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\HTMLForm.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLForm.h
# End Source File
# Begin Source File

SOURCE=..\HTMLFormField.h
# End Source File
# Begin Source File

SOURCE=..\HTMLFormInput.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLFormInput.h
# End Source File
# Begin Source File

SOURCE=..\HTMLFormObjectABC.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLOption.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLOption.h
# End Source File
# Begin Source File

SOURCE=..\HTMLSelect.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLSelect.h
# End Source File
# Begin Source File

SOURCE=..\HTMLTextArea.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLTextArea.h
# End Source File
# Begin Source File

SOURCE=..\MessageReflector.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\defaults.cpp
# End Source File
# Begin Source File

SOURCE=..\Defaults.h
# End Source File
# Begin Source File

SOURCE=..\fontinfo.cpp
# End Source File
# Begin Source File

SOURCE=..\FontInfo.h
# End Source File
# Begin Source File

SOURCE=..\htmlanchor.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLAnchor.h
# End Source File
# Begin Source File

SOURCE=..\htmlblockquote.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLBlockQuote.h
# End Source File
# Begin Source File

SOURCE=..\HTMLBreak.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLBreak.h
# End Source File
# Begin Source File

SOURCE=..\HTMLDefinitionList.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLDefinitionList.h
# End Source File
# Begin Source File

SOURCE=..\HTMLDefinitionListItem.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLDefinitionListItem.h
# End Source File
# Begin Source File

SOURCE=..\htmldocument.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLDocument.h
# End Source File
# Begin Source File

SOURCE=..\htmldocumentobject.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLDocumentObject.h
# End Source File
# Begin Source File

SOURCE=..\HTMLFontDef.h
# End Source File
# Begin Source File

SOURCE=..\htmlgetalignmentfromstring.cpp
# End Source File
# Begin Source File

SOURCE=..\htmlhorizontalrule.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLHorizontalRule.h
# End Source File
# Begin Source File

SOURCE=..\htmlimage.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLImage.h
# End Source File
# Begin Source File

SOURCE=..\HTMLLink.h
# End Source File
# Begin Source File

SOURCE=..\htmllist.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLList.h
# End Source File
# Begin Source File

SOURCE=..\htmllistitem.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLListItem.h
# End Source File
# Begin Source File

SOURCE=..\htmlparagraph.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLParagraph.h
# End Source File
# Begin Source File

SOURCE=..\htmlparagraphobject.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLParagraphObject.h
# End Source File
# Begin Source File

SOURCE=..\htmlparse.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLParse.h
# End Source File
# Begin Source File

SOURCE=..\htmlparsebase.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLParseBase.h
# End Source File
# Begin Source File

SOURCE=..\htmltable.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLTable.h
# End Source File
# Begin Source File

SOURCE=..\htmltablecell.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLTableCell.h
# End Source File
# Begin Source File

SOURCE=..\htmltextblock.cpp
# End Source File
# Begin Source File

SOURCE=..\HTMLTextBlock.h
# End Source File
# Begin Source File

SOURCE=..\qhtmimage.cpp
# End Source File
# Begin Source File

SOURCE=..\QHTMImage.h
# End Source File
# End Group
# Begin Group "Utility"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Background.h
# End Source File
# Begin Source File

SOURCE=..\Color.cpp
# End Source File
# Begin Source File

SOURCE=..\Color.h
# End Source File
# Begin Source File

SOURCE=..\DrawTiledBitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\HandleMouseWheelRegisteredMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\loadtextfile.cpp
# End Source File
# Begin Source File

SOURCE=..\mouse.cpp
# End Source File
# Begin Source File

SOURCE=..\parsewindowstyle.cpp
# End Source File
# Begin Source File

SOURCE=..\striremove.cpp
# End Source File
# Begin Source File

SOURCE=..\stristr.cpp
# End Source File
# Begin Source File

SOURCE=..\strremove.cpp
# End Source File
# Begin Source File

SOURCE=..\TextABC.h
# End Source File
# Begin Source File

SOURCE=..\Utils.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\aquireimage.cpp
# End Source File
# Begin Source File

SOURCE=..\AquireImage.h
# End Source File
# Begin Source File

SOURCE=..\config.h
# End Source File
# Begin Source File

SOURCE=..\cursor.cpp
# End Source File
# Begin Source File

SOURCE=..\cursor.h
# End Source File
# Begin Source File

SOURCE=..\enablecooltips.cpp
# End Source File
# Begin Source File

SOURCE=..\gotourl.cpp
# End Source File
# Begin Source File

SOURCE=..\htmlprint.cpp
# End Source File
# Begin Source File

SOURCE=..\QHTM.h
# End Source File
# Begin Source File

SOURCE=..\qhtm.rc
# End Source File
# Begin Source File

SOURCE=..\qhtm_gethtmlheight.cpp
# End Source File
# Begin Source File

SOURCE=..\qhtm_includes.h
# End Source File
# Begin Source File

SOURCE=..\QHTM_Initialize.cpp
# End Source File
# Begin Source File

SOURCE=..\qhtm_messagebox.cpp
# End Source File
# Begin Source File

SOURCE=..\qhtm_renderhtml.cpp
# End Source File
# Begin Source File

SOURCE=..\qhtm_sethtmlbutton.cpp
# End Source File
# Begin Source File

SOURCE=..\qhtm_setresources.cpp
# End Source File
# Begin Source File

SOURCE=..\qhtm_trace.cpp
# End Source File
# Begin Source File

SOURCE=..\QHTM_Trace.h
# End Source File
# Begin Source File

SOURCE=..\QHTM_Types.h
# End Source File
# Begin Source File

SOURCE=..\qhtm_uninitialize.cpp
# End Source File
# Begin Source File

SOURCE=..\QHTM_WndProc.cpp
# End Source File
# Begin Source File

SOURCE=..\registerwindow.cpp
# End Source File
# Begin Source File

SOURCE=..\resource.h
# End Source File
# Begin Source File

SOURCE=..\resourceloader.cpp
# End Source File
# Begin Source File

SOURCE=..\ResourceLoader.h
# End Source File
# Begin Source File

SOURCE=..\simplestring.cpp
# End Source File
# Begin Source File

SOURCE=..\SimpleString.h
# End Source File
# Begin Source File

SOURCE=..\smallstringhash.h
# End Source File
# Begin Source File

SOURCE=..\StaticString.h
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
