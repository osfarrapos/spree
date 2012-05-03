// XWindow_CastleBattleTimeChange.cpp: implementation of the _XWindow_CastleBattleTimeChange class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_CastleBattleTimeChange.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_CastleBattleTimeChange::_XWindow_CastleBattleTimeChange()
{
	m_btnTimeChange			= NULL;
	
	m_listboxTimeChange1	= NULL;
	m_listboxTimeChange2	= NULL;
	m_listboxTimeChange3	= NULL;

	for( int i = 0; i < 3; ++i )
	{
		m_ChangeTime[i]	= 0; //적용된 변경 시간 
		m_tempChangeTime[i] = 0; //적용할 변경 시간 
	}

	m_pCurCastleInfo		= NULL;

}

_XWindow_CastleBattleTimeChange::~_XWindow_CastleBattleTimeChange()
{

}

BOOL _XWindow_CastleBattleTimeChange::Initialize(void)
{
	
	int mainframeresourceindex	= g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga");
	int charinforesourceindex	= g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	int messengerresourceindex1	= g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
	int chattingresourceindex	= g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	
	// 총 크기  183 X 167
	
	// 타이틀 바 ( 183 X 18 )
	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create(0, 0, 183, 18, &g_MainInterfaceTextureArchive, mainframeresourceindex);
	pTitleBar->SetClipRect(0, 125, 183, 143);	
	InsertChildObject( pTitleBar);	
	
	// 왼쪽 라인 ( 3 X 146 )
	_XImageStatic* pLeftBorder = new _XImageStatic;
	pLeftBorder->Create(0, 18, 3, 18+146, &g_MainInterfaceTextureArchive, mainframeresourceindex);
	pLeftBorder->SetClipRect(0, 195, 3, 227);
	pLeftBorder->SetScale(1.0f, 4.5625f);
	InsertChildObject(pLeftBorder);
	
	// 오른쪽 라인 ( 3 X 146 )
	_XImageStatic* pRightBorder = new _XImageStatic;
	pRightBorder->Create(180, 18, 183, 18+146, &g_MainInterfaceTextureArchive, mainframeresourceindex);
	pRightBorder->SetClipRect(0, 195, 3, 227);
	pRightBorder->SetScale(1.0f, 4.5625f);
	InsertChildObject(pRightBorder);
	
	// 바닥 라인 ( 183 X 3 )
	_XImageStatic* pBottomLine = new _XImageStatic;
	pBottomLine->Create(0, 18+146, 183, 167, &g_MainInterfaceTextureArchive, mainframeresourceindex);
	pBottomLine->SetClipRect(0, 227, 183, 230);
	InsertChildObject(pBottomLine);
	
	// 시간 표시 박스  ( 65 X 20 )
	_XImageStatic* pTimeBox1= new _XImageStatic;
	pTimeBox1->Create(109, 33, 174, 53, &g_MainInterfaceTextureArchive, messengerresourceindex1);
	pTimeBox1->SetClipRect(34, 230, 141, 250);
	pTimeBox1->SetScale( 0.607476635f, 1.0f );
	InsertChildObject(pTimeBox1);
	
	// 시간 표시 박스  ( 65 X 20 )
	_XImageStatic* pTimeBox2= new _XImageStatic;
	pTimeBox2->Create(109, 33+37, 174, 33+37+20, &g_MainInterfaceTextureArchive, messengerresourceindex1);
	pTimeBox2->SetClipRect(34, 230, 141, 250);
	pTimeBox2->SetScale( 0.607476635f, 1.0f );
	InsertChildObject(pTimeBox2);
	
	// 시간 표시 박스  ( 65 X 20 )
	_XImageStatic* pTimeBox3= new _XImageStatic;
	pTimeBox3->Create(109, 33+37+37, 174, 33+37+37+20, &g_MainInterfaceTextureArchive, messengerresourceindex1);
	pTimeBox3->SetClipRect(34, 230, 141, 250);
	pTimeBox3->SetScale( 0.607476635f, 1.0f );
	InsertChildObject(pTimeBox3);
	
	
	// 장원전 시간변경 닫기 버튼  ( 12 X 12 )
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_CASTLEBATTLE_TIMECHANGE_BTN_CLOSE,
		charinforesourceindex,charinforesourceindex,charinforesourceindex, 	&g_MainInterfaceTextureArchive };
	_XButton* pBtnClose = new _XButton;
	pBtnClose->Create( closebtnstruct );		
	pBtnClose->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pBtnClose->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );	
	pBtnClose->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pBtnClose );
	
	// 변경 버튼 ( 56 X 20 )
	_XBTN_STRUCTURE changebtnstruct = { TRUE, { 19, 139 }, { 56, 20 }, _XDEF_CASTLEBATTLE_TIMECHANGE_BTN_CHANGE,
		messengerresourceindex1,messengerresourceindex1,messengerresourceindex1, &g_MainInterfaceTextureArchive };
	m_btnTimeChange	= new _XButton;
	m_btnTimeChange->Create( changebtnstruct );
	m_btnTimeChange->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 191, 196, 247, 216 );
	m_btnTimeChange->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  191, 169, 247, 189 );
	m_btnTimeChange->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  191, 223, 247, 243 );
	m_btnTimeChange->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_2794 );	// _T("변경")
	m_btnTimeChange->SetButtonTextColor( 0xFFE0D7BA );
	InsertChildObject( m_btnTimeChange );	
	
	// 취소 버튼 ( 56 X 20 )
	_XBTN_STRUCTURE canclebtnstruct = { TRUE, { 106, 139 }, { 56, 20 }, _XDEF_CASTLEBATTLE_TIMECHANGE_BTN_CLOSE,
		messengerresourceindex1,messengerresourceindex1,messengerresourceindex1, &g_MainInterfaceTextureArchive };
	_XButton* pBtnCancle	= new _XButton;
	pBtnCancle->Create( canclebtnstruct );
	pBtnCancle->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 191, 196, 247, 216 );
	pBtnCancle->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  191, 169, 247, 189 );
	pBtnCancle->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  191, 223, 247, 243 );
	pBtnCancle->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_CANCEL );	// _T("취소")
	pBtnCancle->SetButtonTextColor( 0xFFE0D7BA );
	InsertChildObject( pBtnCancle );	
	
	// 리스트 보여주기 버튼1 ( 12 X 12 )
	_XBTN_STRUCTURE listdownbtnstruct = { TRUE, { 158, 37 }, { 12, 12 }, _XDEF_CASTLEBATTLE_TIMECHANGE_BTN_LISTDOWN1,
		chattingresourceindex,chattingresourceindex,chattingresourceindex, &g_MainInterfaceTextureArchive };
	_XButton* pBtnListDown1	= new _XButton;
	pBtnListDown1->Create( listdownbtnstruct );
	pBtnListDown1->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 12, 12, 24 );
	pBtnListDown1->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 12, 12, 24, 24 );
	pBtnListDown1->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 24, 12, 36, 24 );
	InsertChildObject( pBtnListDown1 );	
	
	listdownbtnstruct.commandid = _XDEF_CASTLEBATTLE_TIMECHANGE_BTN_LISTDOWN2;
	listdownbtnstruct.position.y += 37;	
	_XButton* pBtnListDown2	= new _XButton;
	pBtnListDown2->Create( listdownbtnstruct );
	pBtnListDown2->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 12, 12, 24 );
	pBtnListDown2->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 12, 12, 24, 24 );
	pBtnListDown2->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 24, 12, 36, 24 );
	InsertChildObject( pBtnListDown2 );	
	
	listdownbtnstruct.commandid = _XDEF_CASTLEBATTLE_TIMECHANGE_BTN_LISTDOWN3;
	listdownbtnstruct.position.y += 37;	
	_XButton* pBtnListDown3	= new _XButton;
	pBtnListDown3->Create( listdownbtnstruct );
	pBtnListDown3->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0, 12, 12, 24 );
	pBtnListDown3->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 12, 12, 24, 24 );
	pBtnListDown3->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK, 24, 12, 36, 24 );
	InsertChildObject( pBtnListDown3 );	
	
	// ---------------------------------------------------------------------------------------------------------------------=		
	// 시간설정 선택에 사용될 리스트 박스 1
	_XLISTBOX_STRUCTURE timelistboxstructure = 
	{
		TRUE,											// 윈도우 활성화
		{ 110, 53 },									// 윈도우 좌표
		{ 63, 90 },										// 윈도우 사이즈
		_XDEF_CASTLEBATTLE_TIMECHANGE_LISTBOX_TIME1,	// 윈도우 command 아이디
		-1,												// 윈도우 border image
		&g_MainInterfaceTextureArchive,					// Texture list
		_T(" "),										// 윈도우 타이틀
		15,												// 리스트 아이템 라인 피치
		6,												// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_NOSCROLLBAR,							// 스크롤바의 위치
	};
	m_listboxTimeChange1 = new _XListBox;
	m_listboxTimeChange1->Create(timelistboxstructure);
	
	POINT columnoffset1 = { 18, 2 };
	m_listboxTimeChange1->SetColumnOffset(0, columnoffset1);			// column offset	
	m_listboxTimeChange1->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);		// column style (아이콘)	
	m_listboxTimeChange1->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxTimeChange1->SetColumnWidth(0, 63);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	
	//m_listboxTimeChange1->SetBorderColorFactor( 0xB1000000 );
	m_listboxTimeChange1->SetBorderColorFactor( 0xFF000000 );
	m_listboxTimeChange1->SetSelectedImagePosition(0, 3);
	m_listboxTimeChange1->SetSelectedImageHeight(15);
	m_listboxTimeChange1->SetTrackWidth(63);
	m_listboxTimeChange1->SetTrackHeight(15);
	m_listboxTimeChange1->ShowWindow(FALSE);
	InsertChildObject(m_listboxTimeChange1);
	
	// ---------------------------------------------------------------------------------------------------------------------=		
	// 시간설정 선택에 사용될 리스트 박스 2
	timelistboxstructure.commandid = _XDEF_CASTLEBATTLE_TIMECHANGE_LISTBOX_TIME2;
	timelistboxstructure.position.y += 37;

	m_listboxTimeChange2 = new _XListBox;
	m_listboxTimeChange2->Create(timelistboxstructure);
	
	m_listboxTimeChange2->SetColumnOffset(0, columnoffset1);			// column offset	
	m_listboxTimeChange2->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);		// column style (아이콘)	
	m_listboxTimeChange2->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxTimeChange2->SetColumnWidth(0, 63);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	
	m_listboxTimeChange2->SetBorderColorFactor( 0xFF000000 );
	m_listboxTimeChange2->SetSelectedImagePosition(0, 3);
	m_listboxTimeChange2->SetSelectedImageHeight(15);
	m_listboxTimeChange2->SetTrackWidth(63);
	m_listboxTimeChange2->SetTrackHeight(15);
	m_listboxTimeChange2->ShowWindow(FALSE);
	InsertChildObject(m_listboxTimeChange2);
	
	// ---------------------------------------------------------------------------------------------------------------------=		
	// 시간설정 선택에 사용될 리스트 박스 3
	timelistboxstructure.commandid = _XDEF_CASTLEBATTLE_TIMECHANGE_LISTBOX_TIME3;
	timelistboxstructure.position.y += 37;
	
	m_listboxTimeChange3 = new _XListBox;
	m_listboxTimeChange3->Create(timelistboxstructure);
	
	m_listboxTimeChange3->SetColumnOffset(0, columnoffset1);			// column offset	
	m_listboxTimeChange3->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);		// column style (아이콘)	
	m_listboxTimeChange3->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);		// column align type
	m_listboxTimeChange3->SetColumnWidth(0, 63);						// column width (column offset과 맞지 않거나 부족해도 상관없음)
	
	m_listboxTimeChange3->SetBorderColorFactor( 0xFF000000 );
	m_listboxTimeChange3->SetSelectedImagePosition(0, 3);
	m_listboxTimeChange3->SetSelectedImageHeight(15);
	m_listboxTimeChange3->SetTrackWidth(63);
	m_listboxTimeChange3->SetTrackHeight(15);
	m_listboxTimeChange3->ShowWindow(FALSE);
	InsertChildObject(m_listboxTimeChange3);
	

	// 장원전 사자후 바탕 타일 생성
	m_BorderTile.Create( m_WindowPosition.x + 1, m_WindowPosition.y + 18, m_WindowPosition.x + 183, m_WindowPosition.y + 167);
	m_BorderTile.SetTexture( &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_BorderTile.tga" ) );
	m_BorderTile.SetFaceColor( D3DCOLOR_ARGB( 128, 255,255,255 ) );
	
	FLOAT ufactor = 182.0f/128.0f;
	FLOAT vfactor = 148.0f/128.0f;
	
	m_BorderTile.ChangeUV( 0.0f,ufactor,ufactor,0.0f, 0.0f,0.0f,vfactor,vfactor );
	
	return TRUE;
}

void _XWindow_CastleBattleTimeChange::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_CastleBattleTimeChange::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
		m_BorderTile.RenderAlphaBlend(D3DCOLOR_ARGB( 128, 255,255,255 ));
		
		_XDrawSolidBar( m_WindowPosition.x+8, m_WindowPosition.y+24, m_WindowPosition.x+176, m_WindowPosition.y+133 , D3DCOLOR_ARGB(100,0,0,0) );
		_XDrawRectAngle( m_WindowPosition.x+8, m_WindowPosition.y+24, m_WindowPosition.x+176, m_WindowPosition.y+133 , 0.0f, 0xFF000000 );
		
		_XDrawLine2D( m_WindowPosition.x+10, m_WindowPosition.y + 59, m_WindowPosition.x+173, m_WindowPosition.y + 59, D3DCOLOR_ARGB(255,117,117,119), D3DCOLOR_ARGB(255,117,117,119));
		_XDrawLine2D( m_WindowPosition.x+10, m_WindowPosition.y + 96, m_WindowPosition.x+173, m_WindowPosition.y + 96, D3DCOLOR_ARGB(255,117,117,119), D3DCOLOR_ARGB(255,117,117,119));
	}
	_XWindow::Draw(pfocusobject);
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
	g_XBaseFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, 0xFF000000);
	g_XBaseFont->PrintAlign( m_WindowPosition.x+91, m_WindowPosition.y+3, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3252));	//_T("장원전 시간 변경")
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();
	
	g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 224, 150, 45));
	g_XBaseFont->PrintAlign( m_WindowPosition.x+14, m_WindowPosition.y+29, 1.0f, _XFONT_ALIGNTYPE_LEFT,_XGETINTERFACETEXT(ID_STRING_NEW_3219), 1);	//%d차 장원전
	g_XBaseFont->PrintAlign( m_WindowPosition.x+14, m_WindowPosition.y+65, 1.0f, _XFONT_ALIGNTYPE_LEFT,_XGETINTERFACETEXT(ID_STRING_NEW_3219), 2);
	g_XBaseFont->PrintAlign( m_WindowPosition.x+14, m_WindowPosition.y+101, 1.0f, _XFONT_ALIGNTYPE_LEFT,_XGETINTERFACETEXT(ID_STRING_NEW_3219), 3);
	
	g_XBaseFont->SetColor( _XSC_DEFAULT );	
	g_XBaseFont->PrintAlign( m_WindowPosition.x+14, m_WindowPosition.y+29+18, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3253));		//_T("시작 시간")
	g_XBaseFont->PrintAlign( m_WindowPosition.x+14, m_WindowPosition.y+65+18, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3253));		//_T("시작 시간")
	g_XBaseFont->PrintAlign( m_WindowPosition.x+14, m_WindowPosition.y+101+18, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3253));	//_T("시작 시간")

	
	for( int i = 0; i < 3; ++i )
	{
		g_XBaseFont->PrintAlign( m_WindowPosition.x+134, m_WindowPosition.y+38+(i*37), 1.0f, _XFONT_ALIGNTYPE_CENTER,"%02d:%02d",m_tempChangeTime[i]/100, m_tempChangeTime[i]%100);	
	}


	g_XBaseFont->Flush();

	
	if( m_listboxTimeChange1->GetShowStatus() )
		m_listboxTimeChange1->Draw();
	if( m_listboxTimeChange2->GetShowStatus() )
		m_listboxTimeChange2->Draw();
	if( m_listboxTimeChange3->GetShowStatus() )
		m_listboxTimeChange3->Draw();
	

}

BOOL _XWindow_CastleBattleTimeChange::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	
	if(!this->m_ShowWindow)
		return FALSE;
	
	MouseState* mousestate = gpInput->GetMouseState();
	if(mousestate)
	{
		if(!pfocusobject)
		{
			if(mousestate->bButton[0])
			{
				if( !m_listboxTimeChange1->CheckMousePosition() ) 
					m_listboxTimeChange1->ShowWindow(FALSE);
				
				if( !m_listboxTimeChange2->CheckMousePosition() ) 
					m_listboxTimeChange2->ShowWindow(FALSE);
				
				if( !m_listboxTimeChange3->CheckMousePosition() ) 
					m_listboxTimeChange3->ShowWindow(FALSE);
			}
		}
	}
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

void _XWindow_CastleBattleTimeChange::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
	m_BorderTile.MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + 18, m_WindowPosition.x + 183, m_WindowPosition.y + 167 );
}

void _XWindow_CastleBattleTimeChange::ShowWindow(BOOL show)
{	
	if( show )
	{
		SetListItem();
		m_btnTimeChange->EnableWindow(TRUE);
		for( int i = 0; i < 3; ++i )
		{
			m_tempChangeTime[i] = m_ChangeTime[i]; //적용할 변경 시간 
		}
	}
	_XWindow::ShowWindow(show);
}

void _XWindow_CastleBattleTimeChange::ChangeTime(void)
{
	m_pCurCastleInfo = g_CBManager.GetCastleInfo(g_pLocalUser->m_nCastleMapId);
	if(m_pCurCastleInfo)
	{
		for(int i = 0 ; i < 3 ; ++i)
			m_ChangeTime[i] = m_pCurCastleInfo->m_tWarSchedule[0].tDate.dwHour;
	}
	m_btnTimeChange->EnableWindow(TRUE);
}

void _XWindow_CastleBattleTimeChange::SetListItem()
{
	TCHAR timestring[64];
	
	int hour[10], minute[10], count = 0;
	memset(hour, 0, sizeof(hour));
	memset(minute, 0, sizeof(minute));
	
	m_listboxTimeChange1->DeleteAllItem();
	m_listboxTimeChange2->DeleteAllItem();
	m_listboxTimeChange3->DeleteAllItem();

	g_CBManager.GetChallengeTime(g_pLocalUser->m_nCastleMapId, hour, minute, count);

	for(int i = 0 ; i < count ; ++i)
	{
		memset(timestring, 0, sizeof(timestring));
		sprintf(timestring, _T("%02d:%02d"), hour[i], minute[i]);

		int commandid = hour[i]*100 + minute[i];

		m_listboxTimeChange1->InsertListItem(_T(""), 0, commandid);	
		m_listboxTimeChange1->SetItemText(i, 0, timestring);
		m_listboxTimeChange1->SetItemAttrib(i, 0, _XSC_DEFAULT);
		m_listboxTimeChange1->SetItemAttribHighlight(i, 0, _XSC_DEFAULT_HIGHLIGHT);

		memset(timestring, 0, sizeof(timestring));
		sprintf(timestring, _T("%02d:%02d"), hour[i], minute[i]);

		m_listboxTimeChange2->InsertListItem(_T(""), 0, commandid);	
		m_listboxTimeChange2->SetItemText(i, 0, timestring);
		m_listboxTimeChange2->SetItemAttrib(i, 0, _XSC_DEFAULT);
		m_listboxTimeChange2->SetItemAttribHighlight(i, 0, _XSC_DEFAULT_HIGHLIGHT);

		memset(timestring, 0, sizeof(timestring));
		sprintf(timestring, _T("%02d:%02d"), hour[i], minute[i]);

		m_listboxTimeChange3->InsertListItem(_T(""), 0, commandid);	
		m_listboxTimeChange3->SetItemText(i, 0, timestring);
		m_listboxTimeChange3->SetItemAttrib(i, 0, _XSC_DEFAULT);
		m_listboxTimeChange3->SetItemAttribHighlight(i, 0, _XSC_DEFAULT_HIGHLIGHT);
	}

	m_ChangeTime[0] = m_tempChangeTime[0] = hour[0]*100 + minute[0];
	m_ChangeTime[1] = m_tempChangeTime[1] = hour[0]*100 + minute[0];
	m_ChangeTime[2] = m_tempChangeTime[2] = hour[0]*100 + minute[0];
}