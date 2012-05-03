// XWindow_ServerUnification.cpp: implementation of the _XWindow_ServerUnification class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_ServerUnification.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_ServerUnification::_XWindow_ServerUnification()
{
	m_TextInfo			= NULL;
	m_StepWindowSize.cx = m_StepWindowSize.cy	= 0;
	for(int i = 0 ; i < 12 ; i++)
	{
		m_checkbtnList[i] = NULL;
	}
	
	srand((unsigned)time( NULL ));// 추후 삭제
}

_XWindow_ServerUnification::~_XWindow_ServerUnification()
{

}

BOOL _XWindow_ServerUnification::Initialize(_XTextureManager& texturearchive1, _XTextureManager& texturearchive2)
{
	_XWindow::SetDrawBorder( FALSE );
	// XProc_ServerUnification InitializeProcess()에서 미리 텍스쳐를 쓰겠다고 등록해 놔야 쓸 수 있음
	int btnimageindex = texturearchive1.FindResource("MI_Login_00.tga");
	int edgeimageindex = texturearchive1.FindResource("MI_Login_01.tga");
	int zonenameindex = texturearchive1.FindResource("MI_Login_03.tga");
	int	titleimageindex = texturearchive2.FindResource("MI_Main_Back01.tga");
	int zonebtnimageindex = texturearchive2.FindResource("MI_npcquestwin_01.tga");

	_XImageStatic* pImageStatic = new _XImageStatic;
	//m_GroupTitle_Left
	pImageStatic->Create( -2, 0, 97, 43, &texturearchive1, btnimageindex );
	pImageStatic->SetClipRect( 33, 119, 133, 162 );
	InsertChildObject( pImageStatic );

	pImageStatic = new _XImageStatic;
	//m_GroupTitle_Right
	pImageStatic->Create( 198, 0, 298, 43, &texturearchive1, btnimageindex );
	pImageStatic->SetClipRect( 33, 119, 133, 162 );
	pImageStatic->SetScale( -1.0f, 1.0f );
	InsertChildObject( pImageStatic );

	pImageStatic = new _XImageStatic;	
	pImageStatic->Create( 47, 8, 148, 34, &texturearchive1, zonenameindex );	
	pImageStatic->SetClipRect( 0, 61, 101, 87 );
	InsertChildObject( pImageStatic );

	m_MenuEdge_Top.Create( 885, 666, 997, 667, &texturearchive1, edgeimageindex );
	m_MenuEdge_Top.SetClipRect( 3, 7, 115, 8 );
	
	m_MenuEdge_Bottom.Create( 885, 748, 997, 749, &texturearchive1, edgeimageindex );
	m_MenuEdge_Bottom.SetClipRect( 3, 11, 115, 12 );
	
	m_MenuEdge_Left.Create( 884, 667, 885, 748, &texturearchive1, edgeimageindex );
	m_MenuEdge_Left.SetClipRect( 244, 0, 245, 81 );
	
	m_MenuEdge_Right.Create( 997, 667, 998, 748, &texturearchive1, edgeimageindex );
	m_MenuEdge_Right.SetClipRect( 247, 0, 248, 81 );

	// Check Button ---------------------------------------------------------------------------=		
	_XCHECKBTN_STRUCTURE checkbtnstruct = { TRUE, { 0, 0}, { 14, 14 }, 
		_XDEF_SU_CHECKBTN1,
		edgeimageindex,edgeimageindex,
		&texturearchive1 };

	for( int i = 0; i < 12; i++ )
	{
		m_checkbtnList[i] = new _XCheckButton;
		m_checkbtnList[i]->Create( checkbtnstruct );	
		m_checkbtnList[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 198,89, 212, 103 );
		m_checkbtnList[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  198,73, 212, 87 );
		m_checkbtnList[i]->ShowWindow(FALSE);
		InsertChildObject(m_checkbtnList[i]);

		checkbtnstruct.commandid++;
	}
	
	m_TitleLeft.Create( 373, 178, 373+180, 178+23, &texturearchive2, titleimageindex );
	m_TitleLeft.SetClipRect( 0, 0, 180, 24 );
	
	m_TitleRight.Create( 732, 178, 732+180, 178+23, &texturearchive2, titleimageindex );
	m_TitleRight.SetClipRect( 0, 0, 180, 24 );
	m_TitleRight.SetScale( -1.0f, 1.0f );

	m_BoxLeft.Create( 379, 206, 379+4, 206+19, &texturearchive1, zonenameindex );
	m_BoxLeft.SetClipRect( 63, 236, 67, 255 );

	m_BoxCenter.Create( 383, 206, 384, 206+19, &texturearchive1, zonenameindex );
	m_BoxCenter.SetClipRect( 69, 236, 70, 255 );
	m_BoxCenter.SetScale( 338.f,1.f);

	m_BoxRight.Create( 721, 206, 721+4, 206+19, &texturearchive1, zonenameindex );
	m_BoxRight.SetClipRect( 95, 236, 99, 255 );		

	m_WidthLine.Create( 0, 0, 1, 3, &texturearchive1, zonenameindex );
	m_WidthLine.SetClipRect( 65, 230, 66, 233 );
	m_WidthLine.SetScale( 350.f, 1.f );

	m_HeightLine.Create( 0, 0, 2, 15, &texturearchive1, zonenameindex );
	m_HeightLine.SetClipRect( 65, 207 , 67, 222 );

	m_ServerName.Create( 0, 0, 80, 17, &texturearchive2, zonebtnimageindex );
	m_ServerName.SetClipRect( 429, 39, 509, 56 );
	
	m_SelectBorderBox.Create(82, 460, 278, 563, &texturearchive1, edgeimageindex );
	m_SelectBorderBox.SetClipRect( 0, 19, 196, 122);

	m_SelectWarehouseBox.Create(93, 499, 155, 547, &texturearchive1, zonenameindex );
	m_SelectWarehouseBox. SetClipRect( 0, 206, 62, 256);
	
	_XBTN_STRUCTURE nextgamebtn = 
	{
		TRUE, 
		{886, 669}, {110, 25},
		_XDEF_SU_NEXTBUTTON,
		btnimageindex,
		btnimageindex,
		btnimageindex,
		&texturearchive1
	};	
	m_NextButton.Create(nextgamebtn);
	m_NextButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 34, 191, 144, 216 );
	m_NextButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  34, 191, 144, 216 );
	m_NextButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  34, 164, 144, 189 );
	m_NextButton.SetButtonText( _XGETINTERFACETEXT(ID_STRING_NEW_2792) );	// _T("다음 단계")
	m_NextButton.SetGrowTextMode( TRUE );
	
	_XBTN_STRUCTURE prevbtn = 
	{
		TRUE, 
		{886, 695}, {110, 25},
		_XDEF_SU_PREVBUTTON,
		btnimageindex,
		btnimageindex,
		btnimageindex,
		&texturearchive1
	};	
	m_PrevButton.Create(prevbtn);
	m_PrevButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 34, 191, 144, 216 );
	m_PrevButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  34, 191, 144, 216 );
	m_PrevButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  34, 164, 144, 189 );
	m_PrevButton.SetButtonText( _XGETINTERFACETEXT(ID_STRING_NEW_2793) );	//_T("이전 단계")
	m_PrevButton.SetGrowTextMode( TRUE );
	
	_XBTN_STRUCTURE exitbtn = 
	{
		TRUE, 
		{886, 721 }, {110, 25},
		_XDEF_SU_EXITBUTTON,
		btnimageindex,
		btnimageindex,
		btnimageindex,
		&texturearchive1
	};	
	m_ExitButton.Create(exitbtn);
	m_ExitButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 34, 191, 144, 216 );
	m_ExitButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  34, 191, 144, 216 );
	m_ExitButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  34, 164, 144, 189 );
	m_ExitButton.SetButtonText( _XGETINTERFACETEXT(ID_STRING_COMMON_EXIT) ); //_T("종료")
	m_ExitButton.SetGrowTextMode( TRUE );

	_XBTN_STRUCTURE changetbtn = 
	{
		TRUE, 
		{0, 0 }, {40, 17},
		_XDEF_SU_CHANGEBTN1,
		btnimageindex,
		btnimageindex,
		btnimageindex,
		&texturearchive1
	};	

	for( i = 0; i < 3; i++ )
	{
		m_ChangeButton[i] = NULL;
		m_ChangeButton[i] = new _XButton;
		m_ChangeButton[i]->Create(changetbtn);
		m_ChangeButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  169, 233, 209, 250 );
		m_ChangeButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 127, 233, 167, 250 );		
		m_ChangeButton[i]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  211, 233, 251, 250 );
		m_ChangeButton[i]->SetButtonText( _XGETINTERFACETEXT(ID_STRING_NEW_2794) );	// _T("변경")
		m_ChangeButton[i]->ShowWindow(FALSE);
		changetbtn.commandid++;
		InsertChildObject(m_ChangeButton[i]);
	}


	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;

	FILE* licensefile = g_ScriptArchive.GetPackedFile( _T("ServerunificationLicense.txt") );
	
	if( !licensefile )
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	TCHAR buffer[256];
	
	if( feof( licensefile ) ) 
	{
		g_ScriptArchive.ClosePackage();
		return FALSE;
	}
	
	int count = 0, index = 0;
	TCHAR tempSaveBuf[128];

	do 
	{
		if( feof(licensefile) ) break;
		
		memset( buffer, 0, sizeof(TCHAR)*256 );
		if( !fgets( buffer, 255, licensefile ) )
		{
			break;
		}		
		
		if( strncmp( buffer, _T("_XM_EOS"), 7 ) == 0 ) break;
		
		if( buffer[0] != _T(';') )
		{
			if( strncmp( buffer, _T("TITLECOUNT"), 10 ) == 0 )
			{
				sscanf(buffer, "%s%d", &tempSaveBuf, &count);
				m_TextInfo = new ServerUnificationInfo[count];
			}
			else if( strncmp( buffer, _T("TITLE"), 5 ) == 0  )
			{
				if( count > 0 && index < count )
				{
					if( !fgets( buffer, 255, licensefile ) )
						break;
					m_TextInfo[index].reset();
					strcpy( m_TextInfo[index].Title, buffer );

					if( !fgets( buffer, 255, licensefile ) )
						break;
					sscanf( buffer, "%d", &m_TextInfo[index].InfomationLineCount );
					for( int i = 0; i < m_TextInfo[index].InfomationLineCount; i++ )
					{
						if( !fgets( buffer, 255, licensefile ) )
							break;
						strcpy( m_TextInfo[index].Infomation[i], buffer );
					}
					index++;
				}
			}			
		}
		
	} while( !feof(licensefile) );
	
	g_ScriptArchive.ClosePackage();

	// 통합서버명
	memset(m_UnificationServerName, 0, sizeof(TCHAR)*3*128);
	strcpy( m_UnificationServerName[0], _XGETINTERFACETEXT(ID_STRING_NEW_2949) );	//"태산 서버"	// add string
	strcpy( m_UnificationServerName[1], _XGETINTERFACETEXT(ID_STRING_NEW_2950) );	//"황하 서버" 
	strcpy( m_UnificationServerName[2], _XGETINTERFACETEXT(ID_STRING_NEW_2951) );	//"적벽 서버" 

	SetWindowMoveMode(_XWMOVE_FIXED);

	SetUnificationStep(SU_READRULE);
	
	return TRUE;
}

void _XWindow_ServerUnification::DestroyWindow(void)
{
	_XWindow::DestroyWindow();
}

void _XWindow_ServerUnification::Draw(_XGUIObject*& pfocusobject )
{
	if(!this->m_ShowWindow)
		return;

	if(this->m_ShowWindow)
		_XDrawSolidBar( m_WindowPosition.x+1, m_WindowPosition.y+23, m_WindowPosition.x+193, m_WindowPosition.y+317, 0x1E000000 );

	m_MenuEdge_Top.Draw(pfocusobject);
	m_MenuEdge_Bottom.Draw(pfocusobject);
	m_MenuEdge_Left.Draw(pfocusobject);
	m_MenuEdge_Right.Draw(pfocusobject);

	m_NextButton.Draw(pfocusobject);
	m_PrevButton.Draw(pfocusobject);
	m_ExitButton.Draw(pfocusobject);	

	_XDrawSolidBar( m_TitleLeft.m_Position.x, m_TitleLeft.m_Position.y+25, m_TitleLeft.m_Position.x+m_StepWindowSize.cx, m_TitleLeft.m_Position.y+m_StepWindowSize.cy, D3DCOLOR_ARGB(178, 0, 0, 0) );
	
	_XDrawRectAngle( m_TitleLeft.m_Position.x, m_TitleLeft.m_Position.y+22, m_TitleLeft.m_Position.x+m_StepWindowSize.cx, m_TitleLeft.m_Position.y+m_StepWindowSize.cy, 0.0f,0xFF000000 );
	_XDrawRectAngle( m_TitleLeft.m_Position.x+1, m_TitleLeft.m_Position.y+23, m_TitleLeft.m_Position.x+m_StepWindowSize.cx-1, m_TitleLeft.m_Position.y+m_StepWindowSize.cy-1, 0.0f,0xFF9F9A84 );
	_XDrawRectAngle( m_TitleLeft.m_Position.x+2, m_TitleLeft.m_Position.y+24, m_TitleLeft.m_Position.x+m_StepWindowSize.cx-2, m_TitleLeft.m_Position.y+m_StepWindowSize.cy-2, 0.0f,0xFF040000 );
	_XDrawRectAngle( m_TitleLeft.m_Position.x+3, m_TitleLeft.m_Position.y+25, m_TitleLeft.m_Position.x+m_StepWindowSize.cx-3, m_TitleLeft.m_Position.y+m_StepWindowSize.cy-3, 0.0f,0xFF1A211A );

	m_TitleLeft.Draw(pfocusobject);
	m_TitleRight.Draw(pfocusobject);

	if( m_UnificationStep != SU_READRULE )
	{
		m_BoxLeft.Draw(pfocusobject);
		m_BoxCenter.Draw(pfocusobject);
		m_BoxRight.Draw(pfocusobject);
	}

	switch(m_UnificationStep) 
	{
	case SU_READRULE :
			DrawReadRule();
		break;
	case SU_SELECTSERVER :
			DrawSelectServer();				
		break;
	case SU_SELECTCHARAC :
		{
			m_SelectBorderBox.Draw(pfocusobject);
			m_SelectWarehouseBox.Draw(pfocusobject);

			DrawSelectCharac();
			
			// ToolTip
			for( int i = 0; i < 12; i++ )
			{
				if( m_checkbtnList[i]->GetShowStatus() )
				{
					if( m_checkbtnList[i]->CheckMousePosition() )
					{
						g_XBaseFont->SetColor( 0xFFD5C68D );
						if( i < 3 )					
						{
							g_XBaseFont->Puts( m_TitleLeft.m_Position.x, gnHeight-64, _XGETINTERFACETEXT(ID_STRING_NEW_2799) );
							g_XBaseFont->Puts( m_TitleLeft.m_Position.x, gnHeight-44, _T("선택하지 않은 창고는 복구가 불가능 합니다.") );
						}
						else
							g_XBaseFont->Puts( m_TitleLeft.m_Position.x, gnHeight-64, _XGETINTERFACETEXT(ID_STRING_NEW_2800) );	// _T("이전할 캐릭터 최대 3개를 선택하세요.")
					}
				}			
			}
		}
		break;
	case SU_CHECKCHARAC :
			DrawCheckCharac();		
		break;
	case SU_FINISH :
		{
			DrawFinish();
			g_XLargeFont->SetColor( D3DCOLOR_ARGB( 255, 255, 170, 48 ) );
			g_XLargeFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );			
			g_XLargeFont->PutsAlign( (gnWidth>>1), 670, _XFONT_ALIGNTYPE_CENTER, _T("통합이 완료되었습니다.") );
			g_XLargeFont->Flush();			
		}
		break;
	}
	g_XBaseFont->Flush();

	_XWindow::Draw(pfocusobject);
}

BOOL _XWindow_ServerUnification::Process(_XGUIObject*& pfocusobject )
{
	if(!this->m_Enable)
		return FALSE;
	
	if(!this->m_ShowWindow)
		return FALSE;

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	m_NextButton.Process(pfocusobject);
	m_PrevButton.Process(pfocusobject);
	m_ExitButton.Process(pfocusobject);
	
	return TRUE;
}

void _XWindow_ServerUnification::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
}

void _XWindow_ServerUnification::SetUnificationStep(UNIFICATIONSTEP step)
{
	m_UnificationStep = step;
	for( int i = 0; i < 12; i++ )
	{
		m_checkbtnList[i]->ShowWindow(FALSE);
		m_checkbtnList[i]->SetCheck(FALSE);
		m_checkbtnList[i]->EnableWindow(FALSE);
	}

	for( i = 0; i < 3; i++ )
		m_ChangeButton[i]->ShowWindow(FALSE);	

	switch(m_UnificationStep) 
	{
	case SU_READRULE :
		{
			m_StepWindowSize.cx = 358;
			m_StepWindowSize.cy = 264;

			m_NextButton.ShowWindow(TRUE);
			m_PrevButton.ShowWindow(TRUE);
			m_ExitButton.ShowWindow(TRUE);
			m_PrevButton.EnableWindow(FALSE);
			m_ExitButton.SetButtonText( _XGETINTERFACETEXT(ID_STRING_COMMON_EXIT) );	// _T("종료")
		}
		break;
	case SU_SELECTSERVER :
		{
			m_StepWindowSize.cy = 281;
			m_SelectServerIndex = -1;
			
			m_PrevButton.EnableWindow(TRUE);

			// 배치 ----------------------------------------------------------------------------┓			
			m_checkbtnList[0]->ShowWindow(TRUE);
			m_checkbtnList[0]->MoveWindow(m_TitleLeft.m_Position.x+24, m_TitleLeft.m_Position.y+58);

			for( i = 0; i < 3; i++ )
			{
				m_checkbtnList[i+1]->ShowWindow(TRUE);
				m_checkbtnList[i+1]->MoveWindow(m_TitleLeft.m_Position.x+24, m_TitleLeft.m_Position.y+105+(67*i));				
			}
			// ---------------------------------------------------------------------------------┛

			// 활성화 --------------------------------------------------------------------------┓						
			m_checkbtnList[0]->EnableWindow(FALSE);
			for( i = 0; i < 3; i++ )
			{
				if( m_Simple_Group_Info.GroupInfo[i].ucMoveState )
					m_checkbtnList[i+1]->EnableWindow(FALSE);
				else
				{
					int charcount = 0;
					for( int j = 0; j < 3; j++ )
					{
						charcount += m_Simple_Group_Info.GroupInfo[i].ucNoCharac[j];
					}

					if( charcount > 0 )
						m_checkbtnList[i+1]->EnableWindow(TRUE);
					else
						m_checkbtnList[i+1]->EnableWindow(FALSE);
				}
			}		
			// ---------------------------------------------------------------------------------┛
		}
		break;
	case SU_SELECTCHARAC :
		{
			m_StepWindowSize.cy = 254;
			m_SelectWarehouseIndex = -1;
			for(int i = 0; i < 3; i++ )
				m_SelectCharacterIndex[i] = -1;
			
			// 배치 ----------------------------------------------------------------------------┓
			for( i = 0; i < 3; i++ )
			{
				m_checkbtnList[i]->ShowWindow(TRUE);
				m_checkbtnList[i]->MoveWindow(m_TitleLeft.m_Position.x+110, m_TitleLeft.m_Position.y+78+(67*i));				
			}

			int interval = m_TitleLeft.m_Position.y+58;
			for( i = 3; i < 12; i++ )
			{
				m_checkbtnList[i]->ShowWindow(TRUE);
				m_checkbtnList[i]->MoveWindow(m_TitleLeft.m_Position.x+329, interval );
				if( (i%3) == 2 )
					interval+= 27;
				else
					interval+= 20;
			}
			// ---------------------------------------------------------------------------------┛

			// Enable Check Button -------------------------------------------------------------┓
			for( i = 0; i < 9; i++ )
			{
				if( m_Detail_Group_Info.DetailInfo[i].cOldGroupIndex )
				{
					m_checkbtnList[i+3]->EnableWindow(TRUE);
					m_checkbtnList[i/3]->EnableWindow(TRUE);
				}
				else
					m_checkbtnList[i+3]->ShowWindow(FALSE);
			}	
			// ---------------------------------------------------------------------------------┛
		}
		break;
	case SU_CHECKCHARAC :
		{
			m_StepWindowSize.cy = 121;
			m_PrevButton.EnableWindow(FALSE);
			m_NextButton.EnableWindow(FALSE);
			m_ExitButton.EnableWindow(FALSE);
			m_ExitButton.SetButtonText( _XGETINTERFACETEXT(ID_STRING_COMMON_EXIT) );
			
			bool flag = false;
			for( i = 0; i < 3; i++ )
			{
				if( m_MoveResult.ucState[i] )
				{
					flag = true;
					m_ChangeButton[i]->MoveWindow(m_TitleLeft.m_Position.x+187, m_TitleLeft.m_Position.y+58+(i*20) );
					m_ChangeButton[i]->ShowWindow(TRUE);
				}
			}	
			if( !flag )
				SetUnificationStep(SU_FINISH);				
		}
		break;
	case SU_FINISH :
		{
			m_StepWindowSize.cy = 121;
			m_PrevButton.EnableWindow(FALSE);
			m_NextButton.EnableWindow(FALSE);
			m_ExitButton.EnableWindow(TRUE);
			m_ExitButton.SetButtonText( _XGETINTERFACETEXT(ID_STRING_COMMON_EXIT) );
		}
		break;
	}
}

void _XWindow_ServerUnification::DrawReadRule(void)
{	
	g_XBaseFont->SetColor( 0xFFD7D7D7 );
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+180, m_TitleLeft.m_Position.y+6 , _XFONT_ALIGNTYPE_CENTER, m_TextInfo[0].Title );
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();
	
	int currentdrawline = 147;
	for( int i = 1; i < 5; i++ )
	{
		g_XBaseFont->PutsAlign(76, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[i].Title );
		currentdrawline+= 20;
	}
	
	currentdrawline = m_TitleLeft.m_Position.y + 37;			
	for( i = 0; i < m_TextInfo[0].InfomationLineCount; i++ )
	{
		g_XBaseFont->Puts(m_TitleLeft.m_Position.x+8, currentdrawline, _T("※"));
		if( strlen(m_TextInfo[0].Infomation[i]) )
		{
			g_XBaseFont->Puts_Separate( m_TitleLeft.m_Position.x+24, currentdrawline, m_TextInfo[0].Infomation[i], 320, 1.0f, 3 );
			int length = g_XBaseFont->GetRowCount( m_TextInfo[0].Infomation[i], 320 );			
			currentdrawline += length * 16;
		}
		currentdrawline += 16;
	}
}

void _XWindow_ServerUnification::DrawSelectServer(void)
{
	g_XBaseFont->SetColor( 0xFFD7D7D7 );
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+180, m_TitleLeft.m_Position.y+6 , _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_2179) );	// _T("서버 선택")
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();
	
	// 왼쪽 메뉴 텍스트 ----------------------------------------------------------------------------┓	
	int currentdrawline = 147;

	g_XBaseFont->SetColor( 0xFFFEFEFE );
	g_XBaseFont->PutsAlign(76, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[1].Title );
	g_XBaseFont->PutsAlign(77, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[1].Title );

	currentdrawline += 20;

	g_XBaseFont->SetColor( 0xFFD7D7D7 );
	for( int i = 0; i < m_TextInfo[1].InfomationLineCount; i++ )
	{
		if( strlen(m_TextInfo[1].Infomation[i]) )
		{
			g_XBaseFont->Puts_Separate( 84, currentdrawline, m_TextInfo[1].Infomation[i], 196, 1.0f, 3 );
			int length = g_XBaseFont->GetRowCount( m_TextInfo[1].Infomation[i], 196 );			
			currentdrawline += length * 16;
		}
	}

	currentdrawline += 10;
	g_XBaseFont->SetColor( 0xFFFEFEFE );
	for( i = 2; i < 5; i++ )
	{
		g_XBaseFont->PutsAlign(76, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[i].Title );
		currentdrawline+= 20;
	}
	// ---------------------------------------------------------------------------------------------┛	

	m_HeightLine.Draw(m_BoxLeft.m_Position.x + 56, m_BoxLeft.m_Position.y + 2);
	m_HeightLine.Draw(m_BoxLeft.m_Position.x + 146, m_BoxLeft.m_Position.y + 2);
	m_HeightLine.Draw(m_BoxLeft.m_Position.x + 210, m_BoxLeft.m_Position.y + 2);

	m_WidthLine.Draw(m_TitleLeft.m_Position.x + 4, m_TitleLeft.m_Position.y + 51);
	m_WidthLine.Draw(m_TitleLeft.m_Position.x + 4, m_TitleLeft.m_Position.y + 77);
	m_WidthLine.Draw(m_TitleLeft.m_Position.x + 4, m_TitleLeft.m_Position.y + 144);
	m_WidthLine.Draw(m_TitleLeft.m_Position.x + 4, m_TitleLeft.m_Position.y + 211);	

	currentdrawline = m_TitleLeft.m_Position.y+32;
	g_XBaseFont->SetColor( 0xFFD7D7D7 );
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+35, currentdrawline, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_SELECT) );	// _T("선택")
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+110, currentdrawline, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2801) );	// _T("서버")
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+185, currentdrawline, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2802) );	// _T("캐릭터 수")
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+291, currentdrawline, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2803) );	// _T("통합서버 이름")

	currentdrawline += 25;
	int lineinterval = 3;

	m_ServerName.Draw( m_TitleLeft.m_Position.x + 68, currentdrawline );
	if( m_Simple_Group_Info.ucGroup_0_NoCharac == 0 )
		g_XBaseFont->SetColor( 0xFF838383 );
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+108, currentdrawline+lineinterval, _XFONT_ALIGNTYPE_CENTER, g_szServerNameString[0] );
	g_XBaseFont->SetColor( 0xFFD7D7D7 );
	g_XBaseFont->PrintAlign(m_TitleLeft.m_Position.x+185, currentdrawline+lineinterval, 1.0f, _XFONT_ALIGNTYPE_LEFT, "%d", m_Simple_Group_Info.ucGroup_0_NoCharac );
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+244, currentdrawline+lineinterval, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_LOGINSERVER_SERVER1) );
	if( m_Simple_Group_Info.ucGroup_0_NoCharac > 0 )
	{
		g_XBaseFont->SetColor( 0xFFE4BE15 );	
		g_XBaseFont->PutsAlign( m_TitleLeft.m_Position.x+250+g_XBaseFont->GetWidth(_XGETINTERFACETEXT(ID_STRING_LOGINSERVER_SERVER1)), 
			currentdrawline+lineinterval, _XFONT_ALIGNTYPE_LEFT,  _XGETINTERFACETEXT(ID_STRING_NEW_2804) ); //_T("(완료)")
		g_XBaseFont->SetColor( 0xFFD7D7D7 );
	}	

	currentdrawline += 27;
	for( i = 0; i < 9; i++ )
	{
		m_ServerName.Draw( m_TitleLeft.m_Position.x + 68, currentdrawline );
		if( m_Simple_Group_Info.GroupInfo[i/3].ucNoCharac[i%3] == 0 )
			g_XBaseFont->SetColor( 0xFF838383 );
		g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+108, currentdrawline+lineinterval, _XFONT_ALIGNTYPE_CENTER, g_szServerNameString[i+1] );
		g_XBaseFont->SetColor( 0xFFD7D7D7 );
		g_XBaseFont->PrintAlign(m_TitleLeft.m_Position.x+185, currentdrawline+lineinterval, 1.0f, _XFONT_ALIGNTYPE_LEFT, "%d", m_Simple_Group_Info.GroupInfo[i/3].ucNoCharac[i%3] );
		if( (i % 3) == 1 )
		{
			g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+244, currentdrawline+lineinterval, _XFONT_ALIGNTYPE_LEFT, m_UnificationServerName[i/3] );

			if( m_Simple_Group_Info.GroupInfo[i/3].ucMoveState )
			{
				g_XBaseFont->SetColor( 0xFFE4BE15 );	
				g_XBaseFont->PutsAlign( m_TitleLeft.m_Position.x+250+g_XBaseFont->GetWidth(m_UnificationServerName[i/3]), 
					currentdrawline+lineinterval, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_2804) );	// _T("(완료)")
				g_XBaseFont->SetColor( 0xFFD7D7D7 );
			}	
		}
		
		if( (i % 3) == 2 ) 
			currentdrawline+= 27;
		else
			currentdrawline += 20;
	}
}

void _XWindow_ServerUnification::DrawSelectCharac(void)
{
	g_XBaseFont->SetColor( 0xFFD7D7D7 );
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+180, m_TitleLeft.m_Position.y+6 , _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2805) );//_T("캐릭터 / 창고 선택")
	g_XBaseFont->PutsAlign(m_SelectBorderBox.m_Position.x+98, m_SelectBorderBox.m_Position.y+6 , _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2806) ); //_T("최종 선택한 창고/캐릭터")
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	// 왼쪽 메뉴 텍스트 ----------------------------------------------------------------------------┓	
	int currentdrawline = 147;

	g_XBaseFont->SetColor( 0xFFFEFEFE );
	g_XBaseFont->PutsAlign(76, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[1].Title );
	currentdrawline += 20;

	g_XBaseFont->PutsAlign(76, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[2].Title );
	g_XBaseFont->PutsAlign(77, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[2].Title );
	currentdrawline += 20;

	g_XBaseFont->SetColor( 0xFFD7D7D7 );
	for( int i = 0; i < m_TextInfo[2].InfomationLineCount; i++ )
	{
		if( strlen(m_TextInfo[2].Infomation[i]) )
		{
			g_XBaseFont->Puts_Separate( 84, currentdrawline, m_TextInfo[2].Infomation[i], 196, 1.0f, 3 );
			int length = g_XBaseFont->GetRowCount( m_TextInfo[2].Infomation[i], 196 );			
			currentdrawline += length * 16;
		}
	}
	
	g_XBaseFont->SetColor( 0xFFFEFEFE );
	for( i = 0; i < 3; ++i )
	{
		if( m_SelectCharacterIndex[i] > -1 )
		{
			g_XBaseFont->PutsAlign( m_SelectWarehouseBox.m_Position.x+76, m_SelectWarehouseBox.m_Position.y+2+(i*17), _XFONT_ALIGNTYPE_LEFT, 
									m_Detail_Group_Info.DetailInfo[m_SelectCharacterIndex[i]].cName );
		}
	}

	if( m_SelectWarehouseIndex > -1 )
	{
		g_XBaseFont->PutsAlign( m_SelectWarehouseBox.m_Position.x+32, m_SelectWarehouseBox.m_Position.y+12, _XFONT_ALIGNTYPE_CENTER, 
								g_szServerNameString[(m_SelectServerIndex-1)*3+m_SelectWarehouseIndex+1] );
		g_XBaseFont->PutsAlign( m_SelectWarehouseBox.m_Position.x+32, m_SelectWarehouseBox.m_Position.y+28, _XFONT_ALIGNTYPE_CENTER, 
								_XGETINTERFACETEXT(ID_STRING_NEW_2696) );	// _T("창고")
	}

	currentdrawline += 115;	// 이미지 공간

	g_XBaseFont->SetColor( 0xFFFEFEFE );
	g_XBaseFont->PutsAlign(76, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[3].Title );
	currentdrawline += 20;
	g_XBaseFont->PutsAlign(76, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[4].Title );
	// ---------------------------------------------------------------------------------------------┛


	m_HeightLine.Draw(m_BoxLeft.m_Position.x + 90, m_BoxLeft.m_Position.y + 2);
	m_HeightLine.Draw(m_BoxLeft.m_Position.x + 124, m_BoxLeft.m_Position.y + 2);
	m_HeightLine.Draw(m_BoxLeft.m_Position.x + 206, m_BoxLeft.m_Position.y + 2);
	m_HeightLine.Draw(m_BoxLeft.m_Position.x + 310, m_BoxLeft.m_Position.y + 2);

	m_WidthLine.Draw(m_TitleLeft.m_Position.x + 4, m_TitleLeft.m_Position.y + 51); 
	m_WidthLine.Draw(m_TitleLeft.m_Position.x + 4, m_TitleLeft.m_Position.y + 118);
	m_WidthLine.Draw(m_TitleLeft.m_Position.x + 4, m_TitleLeft.m_Position.y + 185);

	currentdrawline = m_TitleLeft.m_Position.y+32;
	g_XBaseFont->SetColor( 0xFFFEFEFE );
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+54, currentdrawline, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2801) );	// _T("서버")
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+115, currentdrawline, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2696) );	// _T("창고")
	
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+173, currentdrawline, _XFONT_ALIGNTYPE_CENTER, _T("캐릭터명") );	// _T("캐릭터")
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+265, currentdrawline, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2807) );	// _T("성취단계")
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+335, currentdrawline, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_SELECT) );	// _T("선택")

	currentdrawline += 25;
	int lineinterval = 3;	

	for( int j = 0; j < 3; ++j )
	{
		int flagindex = -1;

		for( i = 0; i < 3; i++ )
		{
			if( m_Detail_Group_Info.DetailInfo[j*3+i].cOldGroupIndex )
			{		
				if( flagindex == -1 )
					flagindex = j*3+i;

				g_XBaseFont->SetColor( 0xFFD7D7D7 );
				g_XBaseFont->PutsAlign( m_TitleLeft.m_Position.x+175, currentdrawline+lineinterval, _XFONT_ALIGNTYPE_CENTER, 
					m_Detail_Group_Info.DetailInfo[j*3+i].cName );
				
				int tempstep = 0, templevel = 0;
				if( g_pDefaultTooltip_Window )
				{
					tempstep = g_pDefaultTooltip_Window->GetInnerLevel(m_Detail_Group_Info.DetailInfo[j*3+i].sLevel); // 성 ( 1 - 12 )
					templevel = ( m_Detail_Group_Info.DetailInfo[j*3+i].sLevel + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
				}
				g_XBaseFont->PrintAlign( m_TitleLeft.m_Position.x+221, currentdrawline+lineinterval, 1.0f, _XFONT_ALIGNTYPE_LEFT, "%s %d%s", 
					g_LevelName[templevel], tempstep, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL) ); // 내공 수위	
			}
			currentdrawline += 20;
		}
		if( flagindex > -1 )
		{
			m_ServerName.Draw( m_TitleLeft.m_Position.x+8, currentdrawline-40 );
			if( !m_checkbtnList[j]->IsButtonEnabled() )
				g_XBaseFont->SetColor( 0xFF838383 );
				
			g_XBaseFont->PutsAlign( m_TitleLeft.m_Position.x+50, currentdrawline-40+lineinterval, _XFONT_ALIGNTYPE_CENTER, 
				g_szServerNameString[m_Detail_Group_Info.DetailInfo[flagindex].cOldGroupIndex] );
		}
		currentdrawline+= 7;
	}	
}

void _XWindow_ServerUnification::DrawCheckCharac(void)
{
	g_XBaseFont->SetColor( 0xFFD7D7D7 );
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+180, m_TitleLeft.m_Position.y+6 , _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2795) );//_T("서버 통합 완료")
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	// 왼쪽 메뉴 텍스트 ----------------------------------------------------------------------------┓	
	int currentdrawline = 147;
	g_XBaseFont->SetColor( 0xFFFEFEFE );
	g_XBaseFont->PutsAlign(76, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[1].Title );
	currentdrawline += 20;
	g_XBaseFont->PutsAlign(76, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[2].Title );
	currentdrawline += 20;
	g_XBaseFont->PutsAlign(76, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[3].Title );
	g_XBaseFont->PutsAlign(77, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[3].Title );
	currentdrawline += 20;

	g_XBaseFont->SetColor( 0xFFD7D7D7 );
	for( int i = 0; i < m_TextInfo[3].InfomationLineCount; i++ )
	{
		if( strlen(m_TextInfo[3].Infomation[i]) )
		{
			g_XBaseFont->Puts_Separate( 84, currentdrawline, m_TextInfo[3].Infomation[i], 196, 1.0f, 3 );
			int length = g_XBaseFont->GetRowCount( m_TextInfo[3].Infomation[i], 196 );			
			currentdrawline += length * 16;
		}
	}

	currentdrawline += 10;
	g_XBaseFont->SetColor( 0xFFFEFEFE );
	g_XBaseFont->PutsAlign(76, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[4].Title );
	// ---------------------------------------------------------------------------------------------┛
	
	m_HeightLine.Draw(m_BoxLeft.m_Position.x + 90, m_BoxLeft.m_Position.y + 2);
	m_HeightLine.Draw(m_BoxLeft.m_Position.x + 225, m_BoxLeft.m_Position.y + 2);

	m_WidthLine.Draw(m_TitleLeft.m_Position.x + 4, m_TitleLeft.m_Position.y + 51);

	currentdrawline = m_TitleLeft.m_Position.y+32;
	g_XBaseFont->SetColor( 0xFFFEFEFE );
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+58, currentdrawline, _XFONT_ALIGNTYPE_CENTER,  _XGETINTERFACETEXT(ID_STRING_NEW_2696) );	// _T("창고")
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+168, currentdrawline, _XFONT_ALIGNTYPE_CENTER, _T("캐릭터명") );	// _T("캐릭터")
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+292, currentdrawline, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2807) );	// _T("성취단계")
	
	if( m_SelectWarehouseIndex > -1 )
		g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+56, m_TitleLeft.m_Position.y+76, _XFONT_ALIGNTYPE_CENTER, g_szServerNameString[(m_SelectServerIndex-1)*3+m_SelectWarehouseIndex+1] );


	currentdrawline += 25;
	int lineinterval = 3;
	for( i = 0; i < 3; i++ )
	{
		if( strlen(m_MoveResult.ucCompleteName[i]) )
		{
			if( m_MoveResult.ucState[i] == 1 )		///< 변경할
				g_XBaseFont->SetColor( 0xFFFFFF00 );
			else if( m_MoveResult.ucState[i] == 3 )	///< 변경된
				g_XBaseFont->SetColor( 0xFF00FF00 );
			else									///< 변경할 필요없는
				g_XBaseFont->SetColor( 0xFFD7D7D7 );
			g_XBaseFont->PutsAlign( m_TitleLeft.m_Position.x+105, currentdrawline+lineinterval, _XFONT_ALIGNTYPE_LEFT, 
				m_MoveResult.ucCompleteName[i] );
			
			int tempstep = 0, templevel = 0;
			if( g_pDefaultTooltip_Window )
			{
				tempstep = g_pDefaultTooltip_Window->GetInnerLevel(m_MoveResult.usInnerLevel[i]); // 성 ( 1 - 12 )
				templevel = ( m_MoveResult.usInnerLevel[i] + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
			}
			g_XBaseFont->SetColor( 0xFFD7D7D7 );
			g_XBaseFont->PrintAlign( m_TitleLeft.m_Position.x+296, currentdrawline+lineinterval, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%s %d%s", 
				g_LevelName[templevel], tempstep, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL) ); // 내공 수위	

			currentdrawline+=20;
		}
	}
}

void _XWindow_ServerUnification::DrawFinish(void)
{
	g_XBaseFont->SetColor( 0xFFD7D7D7 );
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_FULLEDGE, 0xFF000000 );
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+180, m_TitleLeft.m_Position.y+6 , _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2795) ); //_T("서버 통합 완료")
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	// 왼쪽 메뉴 텍스트 ----------------------------------------------------------------------------┓	
	int currentdrawline = 147;
	g_XBaseFont->SetColor( 0xFFFEFEFE );

	for( int i = 1; i < 4; i++ )
	{
		g_XBaseFont->PutsAlign(76, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[i].Title );		
		currentdrawline += 20;
	}

	g_XBaseFont->PutsAlign(76, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[4].Title );
	g_XBaseFont->PutsAlign(77, currentdrawline, _XFONT_ALIGNTYPE_LEFT, m_TextInfo[4].Title );
	currentdrawline += 20;

	g_XBaseFont->SetColor( 0xFFD7D7D7 );
	for( i = 0; i < m_TextInfo[4].InfomationLineCount; i++ )
	{
		if( strlen(m_TextInfo[4].Infomation[i]) )
		{
			g_XBaseFont->Puts_Separate( 84, currentdrawline, m_TextInfo[4].Infomation[i], 196, 1.0f, 3 );
			int length = g_XBaseFont->GetRowCount( m_TextInfo[4].Infomation[i], 196 );			
			currentdrawline += length * 16;
		}
	}
	// ---------------------------------------------------------------------------------------------┛

	m_HeightLine.Draw(m_BoxLeft.m_Position.x + 90, m_BoxLeft.m_Position.y + 2);
	m_HeightLine.Draw(m_BoxLeft.m_Position.x + 225, m_BoxLeft.m_Position.y + 2);

	m_WidthLine.Draw(m_TitleLeft.m_Position.x + 4, m_TitleLeft.m_Position.y + 51);

	currentdrawline = m_TitleLeft.m_Position.y+32;
	g_XBaseFont->SetColor( 0xFFFEFEFE );
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+58, currentdrawline, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2696) );	// 창고
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+168, currentdrawline, _XFONT_ALIGNTYPE_CENTER, _T("캐릭터명") );	// 캐릭터
	g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+292, currentdrawline, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_2807) );	// _T("성취단계")
	
	if( m_SelectWarehouseIndex > -1 )
		g_XBaseFont->PutsAlign(m_TitleLeft.m_Position.x+56, m_TitleLeft.m_Position.y+76, _XFONT_ALIGNTYPE_CENTER, g_szServerNameString[(m_SelectServerIndex-1)*3+m_SelectWarehouseIndex+1] );


	currentdrawline += 25;
	int lineinterval = 3;
	for( i = 0; i < 3; i++ )
	{
		if( strlen(m_MoveResult.ucCompleteName[i]) )
		{
			g_XBaseFont->SetColor( 0xFFD7D7D7 );
			g_XBaseFont->PutsAlign( m_TitleLeft.m_Position.x+105, currentdrawline+lineinterval, _XFONT_ALIGNTYPE_LEFT, 
				m_MoveResult.ucCompleteName[i] );
			
			int tempstep = 0, templevel = 0;
			if( g_pDefaultTooltip_Window )
			{
				tempstep = g_pDefaultTooltip_Window->GetInnerLevel(m_MoveResult.usInnerLevel[i]); // 성 ( 1 - 12 )
				templevel = ( m_MoveResult.usInnerLevel[i] + 11 ) / 12 ;		// 단계 구하기.( 0 - 20 )
			}
			g_XBaseFont->PrintAlign( m_TitleLeft.m_Position.x+296, currentdrawline+lineinterval, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%s %d%s", 
				g_LevelName[templevel], tempstep, _XGETINTERFACETEXT(ID_STRING_SELECTCHARACTER_SUBLEVEL) ); // 내공 수위	

			currentdrawline+=20;
		}
	}
}

BOOL _XWindow_ServerUnification::GetSelectCharacterCount(void)
{
	int selectedid = 0;
	for( int i = 0; i < 3; ++i )
	{
		if( m_SelectCharacterIndex[i] > -1 )
		{
			selectedid++;
		}
	}
	int maxcharactercount = m_Detail_Group_Info.ucCharacCount;
	if( maxcharactercount > 3 ) maxcharactercount = 3;
	if( selectedid < maxcharactercount )
		return FALSE;
	else
		return TRUE;
}

void _XWindow_ServerUnification::SetSimpleGroupInfo(MSG_UG_SIMPLE_GROUP_INFO* pInfo)
{
	memset( &m_Simple_Group_Info, 0, sizeof(MSG_UG_SIMPLE_GROUP_INFO) );
	memcpy( &m_Simple_Group_Info, pInfo, sizeof(MSG_UG_SIMPLE_GROUP_INFO) );
}

void _XWindow_ServerUnification::SetDetailGroupInfo(MSG_UG_DETAIL_GRP_INFO* pInfo)
{
	memset( &m_Detail_Group_Info, 0, sizeof(MSG_UG_DETAIL_GRP_INFO) );
	m_Detail_Group_Info.ucCharacCount = pInfo->ucCharacCount;

	int index = 0;
	int count = 0;

	for( int i = 0; i < 9; ++i )	
	{
		if( count >= m_Detail_Group_Info.ucCharacCount ) break;

		if( pInfo->DetailInfo[i].cOldGroupIndex > 0 )
		{
			index = (pInfo->DetailInfo[i].cOldGroupIndex-1)%3*3+pInfo->DetailInfo[i].ucCharacIndex;
			
			m_Detail_Group_Info.DetailInfo[index].cOldGroupIndex = pInfo->DetailInfo[i].cOldGroupIndex;
			m_Detail_Group_Info.DetailInfo[index].ucCharacIndex = pInfo->DetailInfo[i].ucCharacIndex;
			m_Detail_Group_Info.DetailInfo[index].sLevel = pInfo->DetailInfo[i].sLevel;
			strcpy( m_Detail_Group_Info.DetailInfo[index].cName, pInfo->DetailInfo[i].cName );
			count++;
		}		
	}

	SetUnificationStep(SU_SELECTCHARAC);
}

void _XWindow_ServerUnification::SetMoveResult(MSG_UG_MOVE_RESULT_COMPLETE* pInfo)
{
	memset( &m_MoveResult, 0, sizeof(MSG_UG_MOVE_RESULT_COMPLETE) );
	memcpy( &m_MoveResult, pInfo, sizeof(MSG_UG_MOVE_RESULT_COMPLETE) );
	SetUnificationStep(SU_CHECKCHARAC);
}

void _XWindow_ServerUnification::SetChangeNameResult(MSG_UG_CHANGE_NAME_RESULT* pInfo)
{
	if( pInfo->ucSuccess > 0 )	// 실패 
	{
		g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("해당 캐릭터 명은 이미 사용 중 입니다.") );
	}
	else	// 성공 
	{
		m_MoveResult.ucState[pInfo->ucIndex] = 3;
		strcpy( m_MoveResult.ucCompleteName[pInfo->ucIndex], pInfo->ucNewName );		
		m_ChangeButton[pInfo->ucIndex]->ShowWindow(FALSE);

		bool flag = false;
		for( int i = 0; i < 3; ++i )
		{
			if( m_ChangeButton[i]->GetShowStatus() )
				flag = true;
		}
		if( !flag )
		{
			m_NextButton.EnableWindow(TRUE);
		}			
	}
}

void _XWindow_ServerUnification::ClickCheckButton(char index)
{
	switch(m_UnificationStep)
	{
	case SU_SELECTSERVER :		// 체크버튼 4개 사용 0 ~ 3
		{
			if( m_SelectServerIndex > -1 )
			{
				if( m_SelectServerIndex == index )
					m_SelectServerIndex = -1;
				else
				{
					m_checkbtnList[m_SelectServerIndex]->SetCheck(FALSE);
					m_SelectServerIndex = index;
				}					
			}
			else
				m_SelectServerIndex = index;	
		}
		break;
	case SU_SELECTCHARAC :		// 체크버튼 12개 사용 0 ~ 2 : 3 ~ 11
		{
			if( index < 3 )	// 창고 체크 버튼
			{
				if( m_SelectWarehouseIndex > -1 )
				{
					if( m_SelectWarehouseIndex == index )
						m_SelectWarehouseIndex = -1;
					else
					{
						m_checkbtnList[m_SelectWarehouseIndex]->SetCheck(FALSE);
						m_SelectWarehouseIndex = index;
					}
				}
				else
					m_SelectWarehouseIndex = index;				
			}
			else			// 캐릭 체크 버튼
			{
				bool emptyflag = true;
				int emptyindex = - 1;

				for( int i = 2; i >= 0; --i )
				{
					if( m_SelectCharacterIndex[i] > -1 )
					{
						if( m_SelectCharacterIndex[i] == index-3 )	// 선택 해제 시
						{
							m_SelectCharacterIndex[i] = -1;
							emptyflag = false;
							break;
						}
					}
					else
						emptyindex = i;		// 비어있는 곳이 있으면 인덱스 저장	
				}

				if( emptyflag && emptyindex > -1 )	// 비어 있음
				{
					m_SelectCharacterIndex[emptyindex] = index-3;	// index 가 3부터 시작되기 땜에 3을 뺌						
					emptyflag = false;
					break;
				}

				if( emptyflag )	// 더이상 선택할 수 없음 
				{
					m_checkbtnList[index]->SetCheck(FALSE);
					// 메세지 처리
					g_MessageBox.SetMessage( _XDEF_DLGBTNTYPE_OK, _T(""), _T("3개의 캐릭터만 선택하실 수 있습니다.") );	
				}
			}
		}
		break;
	}
}
