// XWindow_ChangeName.cpp: implementation of the _XWindow_ChangeName class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_ChangeName.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_ChangeName::_XWindow_ChangeName()
{

}

_XWindow_ChangeName::~_XWindow_ChangeName()
{

}

BOOL _XWindow_ChangeName::Initialize(_XTextureManager& texturearchive1, _XTextureManager& texturearchive2)
{
	_XWindow::SetDrawBorder( FALSE );

	int btnimageindex = texturearchive1.FindResource("MI_Login_00.tga");
	int changenameimageindex = texturearchive2.FindResource("MI_System.tga");

	m_ChangeNameEdit.ResetContainer();
	m_ChangeNameEdit.MoveWindow(m_WindowPosition.x+72, m_WindowPosition.y+34);
	m_ChangeNameEdit.ResizeWindow(109, 22);
	m_ChangeNameEdit.ShowWindow(FALSE);
	m_ChangeNameEdit.SetMaxLength(12);
	//m_ChangeNameEdit.SetReturnKeyCallback(_XIMECallback_IDEditReturn);
	//m_ChangeNameEdit.SetEscapeKeyCallback(_XIMECallback_IDEditEscape);
	m_ChangeNameEdit.SetNextIMEContainerHandle(&m_ChangeNameEdit);
	//m_ChangeNameEdit.SetFocus();	//활성화 하지 말것. 키보드 먹통됨.

	_XImageStatic* pChangeNameBorder = new _XImageStatic;
	pChangeNameBorder->Create( 0, 22, 183, 84, &texturearchive2, changenameimageindex );	
	pChangeNameBorder->SetClipRect( 0, 149, 183, 211 );
	InsertChildObject( pChangeNameBorder );

	_XImageStatic* pChangeNameIMEBorder = new _XImageStatic;
	pChangeNameIMEBorder->Create( 65, 29, 174, 51, &texturearchive1, btnimageindex );	
	pChangeNameIMEBorder->SetClipRect( 111, 29, 220, 51 );
	InsertChildObject( pChangeNameIMEBorder );

	_XImageStatic* pChangeNameLeftTitle = new _XImageStatic;
	pChangeNameLeftTitle->Create( 0, 0, 92, 24, &texturearchive1, btnimageindex );	
	pChangeNameLeftTitle->SetClipRect( 150, 117, 242, 141 );
	InsertChildObject( pChangeNameLeftTitle );

	_XImageStatic* pChangeNameRightTitle = new _XImageStatic;
	pChangeNameRightTitle->Create( 183, 0, 183+91, 24, &texturearchive1, btnimageindex );	
	pChangeNameRightTitle->SetClipRect( 150, 117, 241, 141 );
	pChangeNameRightTitle->SetScale( -1.0f, 1.0f );
	InsertChildObject( pChangeNameRightTitle );

	_XBTN_STRUCTURE changenamebtn = 
	{
		TRUE, 
		{33, 54 }, {60, 24},
		_XDEF_CN_CHANGENAMEYES,
		changenameimageindex,
		changenameimageindex,
		changenameimageindex,
		&texturearchive2
	};

	_XButton* pChangedNameYes = new _XButton;
	pChangedNameYes->Create(changenamebtn);
	pChangedNameYes->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 188, 128, 246, 151 );
	pChangedNameYes->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  188, 153, 246, 177 );
	pChangedNameYes->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  188, 178, 246, 202 );
	pChangedNameYes->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_YES );	
	pChangedNameYes->SetButtonTextColor( _XSC_DEFAULT );
	InsertChildObject(pChangedNameYes);

	changenamebtn.commandid++;
	changenamebtn.position.x+=66;
	
	_XButton* pChangedNameNo = new _XButton;
	pChangedNameNo->Create(changenamebtn);
	pChangedNameNo->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 188, 128, 246, 151 );
	pChangedNameNo->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  188, 153, 246, 177 );
	pChangedNameNo->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  188, 178, 246, 202 );
	pChangedNameNo->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_NO );
	pChangedNameNo->SetButtonTextColor( _XSC_WARNING );
	InsertChildObject(pChangedNameNo);
	
	SetWindowMoveMode(_XWMOVE_FIXED);
	
	return TRUE;
}

void _XWindow_ChangeName::DestroyWindow(void)
{
	m_ChangeNameEdit.ShowWindow(FALSE);
	_XWindow::DestroyWindow();
}

void _XWindow_ChangeName::Draw(_XGUIObject*& pfocusobject)
{
	if(!this->m_ShowWindow)
		return;
	
	_XWindow::Draw(pfocusobject);

	m_ChangeNameEdit.Draw();

	g_XBaseFont->SetColor( 0xFFD7D7D7 );
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+92, m_WindowPosition.y+6, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2778) );	//_T("인물 이름 변경")
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	g_XBaseFont->SetColor( 0xFFD5C68D );
	g_XBaseFont->PutsAlign(m_WindowPosition.x+60, m_WindowPosition.y+34, _XFONT_ALIGNTYPE_RIGHT, _XGETINTERFACETEXT(ID_STRING_MATCH_2560) );
	g_XBaseFont->Flush();
}

BOOL _XWindow_ChangeName::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	
	if(!this->m_ShowWindow)
		return FALSE;

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	if(!pfocusobject)
	{
		if(m_ChangeNameEdit.CheckMousePosition() )
			m_ChangeNameEdit.SetFocus();
	}
	
	return TRUE;	
}

void _XWindow_ChangeName::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
	m_ChangeNameEdit.MoveWindow(m_WindowPosition.x+72, m_WindowPosition.y+46);
}

void _XWindow_ChangeName::ShowWindow(BOOL show)
{
	if(show)
	{
		m_ChangeNameEdit.ClearBuffer();
		m_ChangeNameEdit.SetFocus();
	}
	_XWindow::ShowWindow(show);
}

void _XWindow_ChangeName::ChangeNameFiltering()
{
	TCHAR ChangeNameBuff[128];
	strcpy( ChangeNameBuff, m_ChangeNameEdit.GetText() );				
	
	if( strlen(ChangeNameBuff) <= 0 )
	{
		g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_CREATECHARACTER_EMPTYNAME, TRUE );
	}
	else if( strlen(ChangeNameBuff) <= 1 )
	{
		g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, ID_STRING_CREATECHARACTER_SHORTNAME, TRUE );
	}
	else
	{			
		TCHAR _HeadFiltering[25][128];
		memset( _HeadFiltering, 0, sizeof(TCHAR)*23*128);
		
		strcpy( _HeadFiltering[0], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1783) );
		strcpy( _HeadFiltering[1], _T("NINEDRAGONS") );
		strcpy( _HeadFiltering[2], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1784) );
		strcpy( _HeadFiltering[3], _T("INDY21") );
		strcpy( _HeadFiltering[4], _T("INDY") );
		strcpy( _HeadFiltering[5], _T("GM") );
		strcpy( _HeadFiltering[6], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1785) );
		strcpy( _HeadFiltering[7], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1786) );
		strcpy( _HeadFiltering[8], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1787) );
		strcpy( _HeadFiltering[9], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1788) );
		
		strcpy( _HeadFiltering[10], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03) );
		strcpy( _HeadFiltering[11], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01) );
		strcpy( _HeadFiltering[12], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02) );
		strcpy( _HeadFiltering[13], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05) );
		strcpy( _HeadFiltering[14], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1789) );
		strcpy( _HeadFiltering[15], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06) );
		strcpy( _HeadFiltering[16], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1790) );
		strcpy( _HeadFiltering[17], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04) );
		strcpy( _HeadFiltering[18], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_07) );
		strcpy( _HeadFiltering[19], _XGETINTERFACETEXT(ID_STRING_GROUPNAME_00) );
		
		strcpy( _HeadFiltering[20], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1791) );
		strcpy( _HeadFiltering[21], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1792) );
		strcpy( _HeadFiltering[22], _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1793) );
		strcpy( _HeadFiltering[23], _T("9DRAGONS") );
		strcpy( _HeadFiltering[24], _T("9D") );
		
		TCHAR warnstring[512];
		strcpy(warnstring, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_FILTERNAME));
		
		LPTSTR tempcharacternamestring = _strupr( _strdup( ChangeNameBuff ) );
		
		if( tempcharacternamestring )
		{	
			
			for( int idx = 0; idx < 25; idx++ )
			{					
				if( strncmp( tempcharacternamestring, _HeadFiltering[idx], strlen(_HeadFiltering[idx]) ) == 0 )
				{
					TCHAR messagestring[128];
					sprintf( messagestring, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_1794), _HeadFiltering[idx] );	
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, messagestring, TRUE );
					free( tempcharacternamestring );
					return;
				}
			}
			free( tempcharacternamestring );
		}
		
		wchar_t invalidcharacter = 0;
		_XCREATECHARACTERRESULT namestringcheckresult;
				
		if(g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || 
		   g_LanguageType == _XLANGUAGE_TYPE_JAPANESE || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		{
			namestringcheckresult = _XCheckUsableCharacterName_Unicode( m_ChangeNameEdit.GetUnicodeText(), invalidcharacter );
		}
		else
		{
			namestringcheckresult = _XCheckUsableCharacterName( m_ChangeNameEdit.GetText() );
		}
		
		switch( namestringcheckresult ) 
		{
		case _XCREATECHARACTERRESULT_STRINGISNULL :
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_MEMORYERROR), TRUE );	// _T("인물의 이름이 너무 짧거나 메모리 할당에 실패했습니다.")
				m_ChangeNameEdit.SetFocus();					
			}
			break;
		case _XCREATECHARACTERRESULT_LENGTHISZERO :
			{
				// 인물의 이름이 너무 짧습니다. 한글은 1~6자 영문은 2~12자까지 지을 수 있습니다.
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_SHORTNAME), TRUE );
				m_ChangeNameEdit.SetFocus();
			}
			break;
		case _XCREATECHARACTERRESULT_INCLUDESPACE :
			{
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_BLANK), TRUE );	// _T("인물 이름에 공백은 사용하실수 없습니다.")
				m_ChangeNameEdit.SetFocus();
			}
			break;
		case _XCREATECHARACTERRESULT_INVALIDCHARACTER :
			{
				TCHAR message[512];
				memset( message, 0x00, 512 );
				sprintf( message, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_SPECIALCHAR), g_LastErrorCheckedCharacterName );	// "[%s]와(과) 같은 확장 한글, 한자, 특수문자는 사용하실 수 없습니다."
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );
				m_ChangeNameEdit.SetFocus();
			}
			break;
		case _XCREATECHARACTERRESULT_SINGLEKOREANCHARACTER :
			{
				TCHAR message[512];
				memset( message, 0x00, 512 );
				sprintf( message, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_ERRORCHAR), g_LastErrorCheckedCharacterName );	// "[%s]와(과) 같은 한글 낱자는 사용하실 수 없습니다."
				g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );
				m_ChangeNameEdit.SetFocus();
			}
			break;
		case _XCREATECHARACTERRESULT_NOVALIDASCIICHARACTER :
			{
				TCHAR message[512];
				memset( message, 0x00, 512 );

				if(g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE)
				{
					TCHAR tempstring[10];
					memset( tempstring, 0, sizeof(TCHAR) * 10 );
					itoa( invalidcharacter, tempstring, 16 );
					TCHAR tempstring2[10];
					memset( tempstring2, 0, sizeof(TCHAR) * 10 );
					strcpy( tempstring2, _T("0X") );
					strcat( tempstring2, tempstring );

					sprintf( message, _XGETINTERFACETEXT(ID_STRING_MAINGAMEMESSAGEHANDLER_ERRORASCII), tempstring2 );	// "[%s]와(과) 같은 ASCII 문자는 사용하실 수 없습니다. (기호문자는 -,_,[,]만 가능합니다)"
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );
					m_ChangeNameEdit.SetFocus();
				}
				else
				{
					sprintf( message, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_ERRORASCII), g_LastErrorCheckedCharacterName );	// "[%s]와(과) 같은 ASCII 문자는 사용하실 수 없습니다. (기호문자는 -,_,[,]만 가능합니다)"
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );
					m_ChangeNameEdit.SetFocus();
				}
			}
			break;
		case _XCREATECHARACTERRESULT_NOERROR :
			{
				TCHAR resultstring[256]; 
				memset( resultstring, 0x00, 256 );
				strcpy( resultstring, m_ChangeNameEdit.GetText() );
				g_WordFilteringManager.Filtering( resultstring, _XDEF_WORDFILETERINGTYPE_NAME | _XDEF_WORDFILETERINGTYPE_ALWAYS, TRUE );
				
				// 필터링 통과
				if( strcmpi( resultstring, m_ChangeNameEdit.GetText() ) == 0 )
				{
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_YESNO, ID_STRING_NEW_2778, TRUE, _XDEF_CN_CHANGENAMEREALYES );
				}				
				else
				{
					TCHAR message[512];
					memset( message, 0x00, 512 );
					sprintf( message, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_FILTERCHAR), g_WordFilteringManager.GetLastFilterdWord() );	// "%s(은)는 사용하실 수 없는 단어입니다."
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, warnstring, message, TRUE );
					m_ChangeNameEdit.SetFocus();
				}
			}
			break;
		}
	}
}