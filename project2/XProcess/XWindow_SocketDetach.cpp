// XWindow_SocketDetach.cpp: implementation of the XWindow_SocketDetach class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XMessageWindow.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_SocketDetach.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static int _XDEF_INVENTORYITEMPOSX = 11;
static int _XDEF_INVENTORYITEMPOSY = 71;

static int _XDEF_INVENTORYSLOT_WIDTH = 5;
static int _XDEF_INVENTORYSLOT_HEIGHT = 5;

_XWindow_SocketDetach::_XWindow_SocketDetach() : m_pSocketitemListBox(NULL), m_bHaveMouseSlot(FALSE), m_IndependentItem(NULL), 
												m_nIndependentSlot(0), m_nSocketLocation(-1), m_nDeatchIndex(0), m_nSocketIndex(-1), m_bWindowLock(FALSE)
{
	m_FuncProcessPtr = &_XWindow_SocketDetach::Process_None;
}

_XWindow_SocketDetach::~_XWindow_SocketDetach()
{

}

BOOL _XWindow_SocketDetach::Initialize()
{
	int inventoryimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );

	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 184, 71, &g_MainInterfaceTextureArchive, inventoryimageindex );	
	pTitleBar->SetClipRect( 0, 125, 183, 195 );
	InsertChildObject( pTitleBar );

	_XImageStatic* pBackBoard = new _XImageStatic;
	pBackBoard->Create( 0, 60, 184, 271, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" ) );	
	pBackBoard->SetClipRect( 0, 1, 183, 211 );
	InsertChildObject( pBackBoard );

	_XImageStatic* pSocketItemView = new _XImageStatic;
	pSocketItemView->Create( 7, 26, 176, 141, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	pSocketItemView->SetClipRect( 4, 140, 172, 254 );
	InsertChildObject( pSocketItemView );
	
	// 장식물 슬롯 ----------------------------------------------------------------------------------------------------=
	int socketslotimg = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	_XImageStatic* pSocketSlot_1 = new _XImageStatic;
	pSocketSlot_1->Create( 105, 70, 137, 102, &g_MainInterfaceTextureArchive, socketslotimg );
	pSocketSlot_1->SetClipRect( 191, 0, 222, 31 );	
	InsertChildObject( pSocketSlot_1 );

	_XImageStatic* pSocketSlot_2 = new _XImageStatic;
	pSocketSlot_2->Create( 140, 70, 172, 102, &g_MainInterfaceTextureArchive, socketslotimg );
	pSocketSlot_2->SetClipRect( 191, 0, 222, 31 );	
	InsertChildObject( pSocketSlot_2 );

	_XImageStatic* pSocketSlot_3 = new _XImageStatic;
	pSocketSlot_3->Create( 105, 105, 137, 137, &g_MainInterfaceTextureArchive, socketslotimg );
	pSocketSlot_3->SetClipRect( 191, 0, 222, 31 );	
	InsertChildObject( pSocketSlot_3 );
	_XImageStatic* pSocketSlot_4 = new _XImageStatic;
	pSocketSlot_4->Create( 140, 105, 172, 137, &g_MainInterfaceTextureArchive, socketslotimg );
	pSocketSlot_4->SetClipRect( 191, 0, 222, 31 );	
	InsertChildObject( pSocketSlot_4 );
	// ----------------------------------------------------------------------------------------------------------------=

	// 장식물 속성 리스트박스 -----------------------------------------------------------------------------------------=	
	int scrollbarresourceindex = g_MainInterfaceTextureArchive.FindResource( "MI_chat_quick.tga" );
	_XLISTBOX_STRUCTURE listboxstructure = 
	{
		TRUE,								// 윈도우 활성화
		{ 9,   144 },						// 윈도우 좌표
		{ 165, 92 },						// 윈도우 사이즈
		NULL,								// 윈도우 아이디
		-1,									// 윈도우 border 이미지 인덱스
		&g_MainInterfaceTextureArchive,		// Texture 리스트
		_T(" "),							// 윈도우 타이틀
		14,									// 리스트 아이템 라인 피치
		6,									// 리스트 박스에 보이는 최대 라인수
		_XLSTYLE_RIGHTSCROLLBAR,			// 스크롤바의 위치
		
		// Scroll bar property
		{13, 92},
		12, 
		22,
		20,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex,
		scrollbarresourceindex
	};
	
	m_pSocketitemListBox = new _XListBox;
	m_pSocketitemListBox->Create(listboxstructure);
	m_pSocketitemListBox->SetBorderColorFactor( 0x80000000 );
	
	POINT coloffset = { 5, 5 };
	m_pSocketitemListBox->SetColumnOffset(0, coloffset);
	m_pSocketitemListBox->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_pSocketitemListBox->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
	m_pSocketitemListBox->SetColumnWidth(0, 160 );
	m_pSocketitemListBox->SetTrackWidth(160);
	m_pSocketitemListBox->SetTrackHeight(92);
	m_pSocketitemListBox->SetTrackBarColor( D3DCOLOR_ARGB(128,0,0,0) );	
	m_pSocketitemListBox->EnableSelectItem(FALSE);
	m_pSocketitemListBox->SetAlwaysShowScrollBarMode(FALSE);
	
	_XVScrollBar* pScrollBar = m_pSocketitemListBox->GetScrollBarObject();
	if( pScrollBar )
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 12, 12 },
			{ 24, 0, 36, 12 },
			{ 0,  12, 12, 24 },
			{ 24, 12, 36, 24 },
			{ 40,157, 52, 179}
		};
		pScrollBar->SetClipRect( cliprectstructure );	
	}
	
	InsertChildObject(m_pSocketitemListBox);	
	// ----------------------------------------------------------------------------------------------------------------=

	// 버튼들 ---------------------------------------------------------------------------------------------------------=
	int applybtnindex = g_MainInterfaceTextureArchive.FindResource( "mi_skillwin.tga" );
	_XBTN_STRUCTURE socketpluginbtnstruct = 
	{ 
		TRUE, { 9, 240  }, { 108, 24 }, 
		_XDEF_SOCKETDETACH_PROCESSBUTTON, applybtnindex, applybtnindex, applybtnindex, &g_MainInterfaceTextureArchive 
	};
	m_pProcessButton = new _XButton;
	m_pProcessButton->Create( socketpluginbtnstruct );	
	m_pProcessButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_pProcessButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_DROPITEM); // 물품 내리기
	m_pProcessButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 176, 110, 200 );
	m_pProcessButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 152, 110, 176 ); 
	m_pProcessButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  2, 201, 110, 225 );
	m_pProcessButton->EnableWindow(FALSE);
	m_pProcessButton->ShowWindow(FALSE);
	InsertChildObject( m_pProcessButton );	

	applybtnindex = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	_XBTN_STRUCTURE applycanclebtnstruct = 
	{ 
		TRUE, { 117, 240  }, { 60, 24 }, 
		_XDEF_SOCKETDETACH_CANCEL, applybtnindex, applybtnindex, applybtnindex, &g_MainInterfaceTextureArchive
	};
	_XButton* pCancleButton = new _XButton;
	pCancleButton->Create( applycanclebtnstruct );	
	pCancleButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	pCancleButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CANCLE);		// _T("취소")
	pCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
	pCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  187, 152, 247, 176 );
	pCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	InsertChildObject( pCancleButton );

	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	_XBTN_STRUCTURE closebtnstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_SOCKETDETACH_CLOSE,
									  charinfointerface,charinfointerface,charinfointerface, 
									  &g_MainInterfaceTextureArchive };
	_XButton* pClosebutton = new _XButton;
	pClosebutton->Create( closebtnstruct );	
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( pClosebutton );

	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 153, 2 }, { 12, 12 }, _XDEF_SOCKETDETACH_HELP,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );

	// ----------------------------------------------------------------------------------------------------------------=
	for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT; i++ )
	{
		m_SocketItemList[i].Reset();
	}

	m_SlotPosition[0].x = 105;
	m_SlotPosition[0].y = 70;
	m_SlotPosition[1].x = 140;
	m_SlotPosition[1].y = 70;
	m_SlotPosition[2].x = 105;
	m_SlotPosition[2].y = 105;
	m_SlotPosition[3].x = 140;
	m_SlotPosition[3].y = 105;	

	// Model View Camera Setting =========================================
	m_SocketModelViewCamera.Set( 45, 83.0f/60.0f, 0.01f, 180.0f);
	m_SocketModelViewCamera.Set(0.0f, 0.0f, 0.0f);	
	m_SocketModelViewCamera.SetDistanceLimits( 0.01, 30.0f );
	m_SocketModelViewCamera.SetDistance( 1.0f );	
	m_SocketModelViewCamera.UpdateViewMatrix();
	m_SocketModelViewCamera.UpdateProjMatrix();

	// ViewPortReposition - for Model View
	m_SocketModelViewPort.X = (DWORD)(m_WindowPosition.x + 14);
	m_SocketModelViewPort.Y = (DWORD)(m_WindowPosition.y + 73);
	m_SocketModelViewPort.Width  = 83;
	m_SocketModelViewPort.Height = 60;
	m_SocketModelViewPort.MinZ = 0.0f;
	m_SocketModelViewPort.MaxZ = 1.0f;

	m_SocketRotateAngle = 0.0f;

	return TRUE;
}

void _XWindow_SocketDetach::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_SocketDetach::MoveWindow( int X, int Y )
{			
	_XWindow::MoveWindow( X, Y );

	m_SocketModelViewPort.X = (DWORD)(m_WindowPosition.x + 14);
	m_SocketModelViewPort.Y = (DWORD)(m_WindowPosition.y + 73);
}

void _XWindow_SocketDetach::ShowWindow(BOOL show)
{
	if(show)
	{
		if(g_pInventory_Window)
		{
			if(!g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = TRUE;
		}
	}
	else
	{
		// 창고 사용 끝나면 Inventory lock 해제
		if(g_pInventory_Window)
		{
			if(g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = FALSE;
		}

		if( g_pDefaultTooltip_Window )
		{
			if( g_pDefaultTooltip_Window->GetShowStatus() )						
				g_pDefaultTooltip_Window->ShowWindow( FALSE );
		}

		ReBuild( MODE_NONE );

		g_pInventory_Window->ResetSocketIndexList();
		g_pInventory_Window->m_UsedLifeItemIndex = 0;
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_SocketDetach::Draw( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return;

	_XWindow::Draw(pfocusobject);

	int tempx = m_WindowPosition.x + 9;
	int tempy = m_WindowPosition.y + 68;

	if( m_IndependentItem )
	{
		for( int i = 0; i < m_IndependentItem->Get_m_ucSocketCount(); i++ )
		{
			tempx = m_WindowPosition.x+m_SlotPosition[i].x;
			tempy = m_WindowPosition.y+m_SlotPosition[i].y;
			_XDrawRectAngle( tempx, tempy, tempx+32, tempy+32, 0.0f,D3DCOLOR_ARGB(255,114,98,87) );
		}
		
		for( i = 0 ; i < _XDEF_SOCKETITEM_MAXCOUNT ; i++)
		{
			if( m_SocketItemList[i].Get_m_sID() )
				g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+m_SlotPosition[i].x+1, m_WindowPosition.y+m_SlotPosition[i].y+1, &m_SocketItemList[i]);
		}
	}
	
	DrawSelectedItem();
	if( m_IndependentItem )
	{
		DrawSocketPlugInionItem();

		g_XBaseFont->SetColor(_XSC_DEFAULT);
		int tempLength = strlen( _XGameItem::GetItemName(m_IndependentItem->Get_m_cType(), m_IndependentItem->Get_m_sID() ));
		if( tempLength > 16 )
		{

		}
		else
		{
			g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+40, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemName(m_IndependentItem->Get_m_cType(), m_IndependentItem->Get_m_sID() ) );		
		}	

		g_XBaseFont->Flush();
	}


	for( int i = 0; i < 4; i++ )
	{
		if( m_nSocketIndex == i )
		{
			_XDrawRectAngle(m_WindowPosition.x+m_SlotPosition[m_nSocketIndex].x,
					m_WindowPosition.y+m_SlotPosition[m_nSocketIndex].y,
					m_WindowPosition.x+m_SlotPosition[m_nSocketIndex].x+31,
					m_WindowPosition.y+m_SlotPosition[m_nSocketIndex].y+31, 1.0f, D3DCOLOR_ARGB( 255, 204, 204, 204) );
		}
		else
		{
			_XDrawRectAngle(m_WindowPosition.x+m_SlotPosition[m_nSocketIndex].x,
					m_WindowPosition.y+m_SlotPosition[m_nSocketIndex].y,
					m_WindowPosition.x+m_SlotPosition[m_nSocketIndex].x+31,
					m_WindowPosition.y+m_SlotPosition[m_nSocketIndex].y+31, 1.0f, D3DCOLOR_ARGB( 0, 0, 0, 0));
		}
	}	
	

	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
	g_XBaseFont->SetColor(0xFFFFFFFF);
	g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 3, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3687) );//장식제거도구
	g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1)+1, m_WindowPosition.y + 3, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3687) );//장식제거도구
	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();
}

void _XWindow_SocketDetach::ReBuild( SocketDetachMode Mode )
{
	for( int i = 0; i < 8; i++ )
		memset( m_ListBoxTextList[i], 0, sizeof(TCHAR)*255 );

	switch(Mode)
	{
	case MODE_NONE :
		{
			m_IndependentItem = NULL;
			m_bWindowLock = FALSE;

			for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT; i++ )
				m_SocketItemList[i].Reset();

			m_pProcessButton->ShowWindow(FALSE);
			m_pProcessButton->EnableWindow(FALSE);
			m_nIndependentSlot = 0;
			m_nSocketIndex= -1;

			//if( g_pInventory_Window )
			//{
				//g_pInventory_Window->ResetSocketIndexList();
				//g_pInventory_Window->m_UsedLifeItemIndex = 0;
			//}
			/*if( m_pChargeGaugeWindow )
			{
				m_pChargeGaugeWindow->ShowWindow(FALSE);
				m_pChargeGaugeWindow = NULL;
			}*/

			sprintf( m_ListBoxTextList[0], _XGETINTERFACETEXT(ID_STRING_NEW_3688));//"장식물을 선택하여 주십시오."
			sprintf( m_ListBoxTextList[1], _XGETINTERFACETEXT(ID_STRING_NEW_3689));//"한번에 한개의 장식물만 분리할 수 있습니다."

			m_pSocketitemListBox->DeleteAllItem();

			if( m_pSocketitemListBox )
			{
				m_pSocketitemListBox->DeleteAllItem();

				_XStringSplitInfo splitinfo;
				int viewsize = 146;

				for( int i = 0; i < 8; i++ )
				{
					if( strlen( m_ListBoxTextList[i] ) )
					{
						if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
						{
							g_XBaseFont->SplitString(m_ListBoxTextList[i], viewsize, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);		
						}
						else
						{
							g_XBaseFont->SplitString(m_ListBoxTextList[i], viewsize, splitinfo, 0);		
						}
						TCHAR	splitstring[256];		
						for(int i = 0; i < splitinfo.splitcount ; i++)
						{
							memset(splitstring, 0, sizeof(TCHAR)*256);
							strncpy(splitstring, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
							
							m_pSocketitemListBox->InsertListItem( splitstring, _XSC_DEFAULT );
							m_pSocketitemListBox->SetItemCommandID( m_pSocketitemListBox->GetItemCount()-1, 0 );
							m_pSocketitemListBox->SetItemAttrib( m_pSocketitemListBox->GetItemCount()-1, 0, _XSC_DEFAULT );
						}
					}
				}		
			}

		}
		break;
	case MODE_READY :
		{
			m_pProcessButton->EnableWindow(FALSE);
			m_pProcessButton->SetButtonText(_XGETINTERFACETEXT(ID_STRING_NEW_3690));//"장식물 분리"
			//m_pProcessButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_DROPITEM); // 물품 내리기
			m_pProcessButton->ShowWindow(TRUE);

			for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT; i++ )
			{
				if( m_SocketItemList[i].Get_m_sID() > 0 )
				{
#ifdef _XTS_ITEM_OPTIMIZATION
					if( _XGI_FC_WEAPON == GetEqualItemType(m_IndependentItem->Get_m_cType()) )
#else
					if( m_IndependentItem->Get_m_cType() == _XGI_FC_WEAPON || m_IndependentItem->Get_m_cType() == _XGI_FC_WEAPON2 || m_IndependentItem->Get_m_cType() == _XGI_FC_WEAPON3 )
#endif
					{
						map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sSkill_Effect_ID1);
						if(iter_table != g_CharacterStateTable.end())
						{
							TCHAR temp_buf[255];
							if( g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sAP_Per1 )
							{
								sprintf( temp_buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sAP_Per1 );	// "%d초마다 "
								strcat( m_ListBoxTextList[i], temp_buf );
							}						
							if( g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].cProb1 )
							{
								sprintf( temp_buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].cProb1 );	// "%d%%확률로 "
								strcat( m_ListBoxTextList[i], temp_buf );
							}

							sprintf( temp_buf, g_CharacterStateTable[g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sSkill_Effect_ID1]->concept, 
								g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sValue1 );
							strcat( m_ListBoxTextList[i], temp_buf );
						}
						iter_table = g_CharacterStateTable.find(g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sSkill_Effect_ID2);
						if(iter_table != g_CharacterStateTable.end())
						{
							TCHAR temp_buf[255];
							if( g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sAP_Per2 )
							{
								sprintf( temp_buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sAP_Per2 );	// "%d초마다 "
								strcat( m_ListBoxTextList[i+4], temp_buf );
							}						
							if( g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].cProb2 )
							{
								sprintf( temp_buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].cProb2 );	// "%d%%확률로 "
								strcat( m_ListBoxTextList[i+4], temp_buf );
							}
							
							sprintf( temp_buf, 
									 g_CharacterStateTable[g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sSkill_Effect_ID2]->concept, 
									 g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sValue2 );
							strcat( m_ListBoxTextList[i+4], temp_buf );
						}
					}
					else if( m_IndependentItem->Get_m_cType() == _XGI_FC_CLOTHES || m_IndependentItem->Get_m_cType() == _XGI_FC_CLOTHES2 || m_IndependentItem->Get_m_cType() == _XGI_FC_CLOTHES3 )
					{
						map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sSkill_Effect_ID3);
						if(iter_table != g_CharacterStateTable.end())
						{
							TCHAR temp_buf[255];
							if( g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sAP_Per3 )
							{
								sprintf( temp_buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sAP_Per3 );	// "%d초마다 "
								strcat( m_ListBoxTextList[i], temp_buf );
							}						
							if( g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].cProb3 )
							{
								sprintf( temp_buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].cProb3 );	// "%d%%확률로 "
								strcat( m_ListBoxTextList[i], temp_buf );
							}
							
							sprintf( temp_buf, 
									 g_CharacterStateTable[g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sSkill_Effect_ID3]->concept, 
									 g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sValue3 );
							strcat( m_ListBoxTextList[i], temp_buf );
						}

						iter_table = g_CharacterStateTable.find(g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sSkill_Effect_ID4);
						if(iter_table != g_CharacterStateTable.end())
						{
							TCHAR temp_buf[255];
							if( g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sAP_Per4 )
							{
								sprintf( temp_buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_SECOND), g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sAP_Per4 );	// "%d초마다 "
								strcat( m_ListBoxTextList[i+4], temp_buf );
							}						
							if( g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].cProb4 )
							{
								sprintf( temp_buf, _XGETINTERFACETEXT(ID_STRING_ITEMTOOLTIP_RATE), g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].cProb4 );	// "%d%%확률로 "
								strcat( m_ListBoxTextList[i+4], temp_buf );
							}
							
							sprintf( temp_buf, 
									 g_CharacterStateTable[g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sSkill_Effect_ID4]->concept, 
									 g_SocketItemProperty[m_SocketItemList[i].Get_m_sID()].sValue4 );
							strcat( m_ListBoxTextList[i+4], temp_buf );
						}
					}
				}
			}

			if( m_pSocketitemListBox )
				m_pSocketitemListBox->DeleteAllItem();

		}
		break;
	}

	/*if( m_pSocketitemListBox )
	{
		m_pSocketitemListBox->DeleteAllItem();

		_XStringSplitInfo splitinfo;
		int viewsize = 146;

		for( int i = 0; i < 8; i++ )
		{
			if( strlen( m_ListBoxTextList[i] ) )
			{
				if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
				{
					g_XBaseFont->SplitString(m_ListBoxTextList[i], viewsize, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);		
				}
				else
				{
					g_XBaseFont->SplitString(m_ListBoxTextList[i], viewsize, splitinfo, 0);		
				}
				TCHAR	splitstring[256];		
				for(int i = 0; i < splitinfo.splitcount ; i++)
				{
					memset(splitstring, 0, sizeof(TCHAR)*256);
					strncpy(splitstring, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
					
					m_pSocketitemListBox->InsertListItem( splitstring, _XSC_DEFAULT );
					m_pSocketitemListBox->SetItemCommandID( m_pSocketitemListBox->GetItemCount()-1, 0 );
					m_pSocketitemListBox->SetItemAttrib( m_pSocketitemListBox->GetItemCount()-1, 0, _XSC_DEFAULT );
				}
			}
		}		
	}*/

}
BOOL _XWindow_SocketDetach::Process( _XGUIObject*& pfocusobject )
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	if( m_bWindowLock )
		return FALSE;

	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if(mousestate && !pfocusobject )
	{
		// mouse left down
		if( mousestate->bButton[0] && CheckMousePosition() )
		{
			m_WindowMouseState = _XW_STATE_LEFTCLICK;

			//소켓을 클릭했는지 체크 // 더러운 소스,,
			if( !(g_pInventory_Window->m_bHaveMouseSlot) &&
				scrnpos->x > m_WindowPosition.x + 105 && scrnpos->x < m_WindowPosition.x + 140 &&
				scrnpos->z > m_WindowPosition.y + 70 && scrnpos->z < m_WindowPosition.y + 105 ) //1번 소켓
			{
				SetListItem(0);				
			}
			else if( !(g_pInventory_Window->m_bHaveMouseSlot) &&
				scrnpos->x > m_WindowPosition.x + 140 && scrnpos->x < m_WindowPosition.x + 175 &&
				scrnpos->z > m_WindowPosition.y + 70 && scrnpos->z < m_WindowPosition.y + 105 ) //2번 소켓
			{
				SetListItem(1);
			}
			else if( !(g_pInventory_Window->m_bHaveMouseSlot) &&
				scrnpos->x > m_WindowPosition.x + 105 && scrnpos->x < m_WindowPosition.x + 140 &&
				scrnpos->z > m_WindowPosition.y + 105 && scrnpos->z < m_WindowPosition.y + 140 ) //3번 소켓
			{
				SetListItem(2);
			}
			else if( !(g_pInventory_Window->m_bHaveMouseSlot) &&
				scrnpos->x > m_WindowPosition.x + 140 && scrnpos->x < m_WindowPosition.x + 175 &&
				scrnpos->z > m_WindowPosition.y + 105 && scrnpos->z < m_WindowPosition.y + 140 ) //4번 소켓
			{
				SetListItem(3);
			}

			if(g_pInventory_Window && g_pInventory_Window->GetShowStatus() &&	// 인벤토리에서 드래그 했을 경우
				g_pInventory_Window->m_bHaveMouseSlot && g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART &&
#ifdef _XTS_ITEM_OPTIMIZATION
				( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType()) ||
				_XGI_FC_CLOTHES == GetEqualItemType(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType())) )
#else
				( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType() == _XGI_FC_WEAPON || 
				g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType() == _XGI_FC_CLOTHES ||
				g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType() == _XGI_FC_WEAPON2 || 
				g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType() == _XGI_FC_CLOTHES2 ||
				g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType() == _XGI_FC_WEAPON3 ||
				g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_cType() == _XGI_FC_CLOTHES3) )
#endif
			{
				if( m_IndependentItem )
					ReBuild( MODE_NONE );

				if( g_pLocalUser->m_UserItemList[ g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber ].Get_m_cSecond() == _XGI_SC_CLOTHES_BACKPACK || //행낭
					g_pLocalUser->m_UserItemList[ g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber ].Get_m_ucSocketCount() == 0 ) //소켓이 없는 물품,,
				{
					TCHAR messagestring[256];
					memset( messagestring, 0, sizeof(TCHAR)*256 );

					g_MainWindowManager.DeleteAllWindow(_XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
				
					_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
					sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NEW_3691)); //"장식물을 제거할 수 없는 물품 입니다."
					pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  NULL, messagestring, TRUE);	

					return FALSE;
				}

				m_IndependentItem = &g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber];
				m_nIndependentSlot = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;
				g_pInventory_Window->m_UsedLifeItemIndex = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;	//슬롯번호 저장
				g_pInventory_Window->m_iSocketIndexList[0] = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;
				PlayItemEquipSound( m_IndependentItem->Get_m_cType(), m_IndependentItem->Get_m_sID() );

				for( int i = 0; i < _XDEF_SOCKETITEM_MAXCOUNT/*g_pLocalUser->m_UserItemList[m_LastDraggingSlot].m_ucSocketCount*/; i++ )
				{
					if( g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_sSocket(i) )
						SetSocketItem( i, g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_sSocket(i) );
				}

				g_pInventory_Window->m_bHaveMouseSlot = FALSE;
				g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();	

				ReBuild( MODE_READY );
			}
		}
	}

	if(!_XWindow::Process(pfocusobject)) 
		return FALSE;

	return TRUE;
}

void _XWindow_SocketDetach::SetListItem( int nIndex )
{
	if( m_SocketItemList[nIndex].Get_m_sID() > 0 )
	{
		m_pProcessButton->EnableWindow(TRUE);

		m_nSocketIndex = nIndex;

		m_pSocketitemListBox->DeleteAllItem();

		_XStringSplitInfo splitinfo;
		int viewsize = 146;
		
		if( strlen( m_ListBoxTextList[nIndex] ) )
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				g_XBaseFont->SplitString(m_ListBoxTextList[nIndex], viewsize, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);		
			}
			else
			{
				g_XBaseFont->SplitString(m_ListBoxTextList[nIndex], viewsize, splitinfo, 0);		
			}
			TCHAR	splitstring[256];		
			for(int i = 0; i < splitinfo.splitcount ; i++)
			{
				memset(splitstring, 0, sizeof(TCHAR)*256);
				strncpy(splitstring, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
				
				m_pSocketitemListBox->InsertListItem( splitstring, _XSC_DEFAULT );
				m_pSocketitemListBox->SetItemCommandID( m_pSocketitemListBox->GetItemCount()-1, 0 );
				m_pSocketitemListBox->SetItemAttrib( m_pSocketitemListBox->GetItemCount()-1, 0, _XSC_DEFAULT );
			}
		}

		if( strlen( m_ListBoxTextList[nIndex+4] ) )
		{
			if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
			{
				g_XBaseFont->SplitString(m_ListBoxTextList[nIndex+4], viewsize, splitinfo, 0, _XSTRINGSPLITFUNCTION_SPACECHAR);		
			}
			else
			{
				g_XBaseFont->SplitString(m_ListBoxTextList[nIndex+4], viewsize, splitinfo, 0);		
			}
			TCHAR	splitstring[256];		
			for(int i = 0; i < splitinfo.splitcount ; i++)
			{
				memset(splitstring, 0, sizeof(TCHAR)*256);
				strncpy(splitstring, splitinfo.splitpoint[i], splitinfo.splitlength[i]);
				
				m_pSocketitemListBox->InsertListItem( splitstring, _XSC_DEFAULT );
				m_pSocketitemListBox->SetItemCommandID( m_pSocketitemListBox->GetItemCount()-1, 0 );
				m_pSocketitemListBox->SetItemAttrib( m_pSocketitemListBox->GetItemCount()-1, 0, _XSC_DEFAULT );
			}
		}
	}
	else
	{
		m_nSocketIndex = -1;
		m_pProcessButton->EnableWindow(FALSE);
		m_pSocketitemListBox->DeleteAllItem();
	}
}

void _XWindow_SocketDetach::Process_None()
{
}

int _XWindow_SocketDetach::VacancySlot()
{
	int count			= _XINVENTORY_SLOTTYPE_POCKETSTART; //처음엔 포켓
	int totalcount = _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART;

#ifdef _XDEF_CASHBACKPACK_080404_MAGVIPER //Author : 양희왕 //breif : 행낭 확장
	if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() > 0)
	{
		totalcount = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART;
		if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() > 0)
			totalcount =  _XINVENTORY_SLOTTYPE_FIRST_CASHBACKSTART;
	}

	//기본 행낭 검사
	for(; count < totalcount; count++ )
	{
		if( g_pLocalUser->m_UserItemList[count].Get_m_sID() == 0 )
		{
			m_nSocketLocation = count;
			return count;
		}
	}

	for(count = g_pInventory_Window->GetCashBackpackStartSlotPosition(); count < g_pLocalUser->m_nCashBackPackMaxValue; count++ )
	{
		if( g_pLocalUser->m_UserItemList[count].Get_m_sID() == 0 )
		{
			m_nSocketLocation = count;
			return count;
		}
	}
#else
	if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK2].Get_m_sID() != 0)
	{
		totalcount = _XINVENTORY_SLOTTYPE_TOTALCOUNT;
	}
	else if(g_pLocalUser->m_UserItemList[_XINVENTORY_SLOTTYPE_BACKPACK1].Get_m_sID() != 0)
	{
		totalcount = _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART;
	}

	//인벤토리 전체 검색
	for( int row = 0; row < _XDEF_INVENTORYSLOT_HEIGHT; row++ )
	{
		for( int column = 0; column < _XDEF_INVENTORYSLOT_WIDTH; column++ )
		{
			if( count >= totalcount ) break;

			if( g_pLocalUser->m_UserItemList[count].Get_m_sID() == 0 )
			{
				m_nSocketLocation = count;
				return count;
			}
			count++;			
		}

		if( count >= totalcount ) break;

		//끝까지 다 돌았다... 주머니를 바꿔주고 다시 검사를 한다
		if( count == _XINVENTORY_SLOTTYPE_FIRST_BACKPACKSTART || count == _XINVENTORY_SLOTTYPE_SECOND_BACKPACKSTART)
		{
			row = -1;	//초기화 row++ 해주기때문
			column = 0; 
		}
	}
#endif

	return -1; //빈슬롯이 없다.. 에러코드
}


//-----------------------------------------------------------------------------------------------------------
int _XWindow_SocketDetach::GetSlotIndex(void)
{
/*	if( m_MsgSocketPuluInOpen ) 
		return -1;
	ScrnPos* scrnpos = gpInput->GetMousePrevScreenPos();
	
	for(int i = 0 ; i < _XDEF_SOCKETITEM_MAXCOUNT ; i++)
	{
		if(scrnpos->x > m_WindowPosition.x + m_SlotPosition[i].x && scrnpos->x < m_WindowPosition.x + m_SlotPosition[i].x + 30)
		{
			if(scrnpos->z > m_WindowPosition.y + m_SlotPosition[i].y && scrnpos->z < m_WindowPosition.y + m_SlotPosition[i].y + 30)
			{
				if( m_SocketItemList[i].m_sID )
					return i;
			}
		}
	}*/
	return -1;
}

void _XWindow_SocketDetach::DrawDragIcon(void)
{
	_XMessageWindow* pMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MESSAGEWINDOW);
	if(pMessageBox)
	{
		if(pMessageBox->GetShowStatus())
			return;
	}	
	
	if(m_bHaveMouseSlot)
	{
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
		g_pInventory_Window->DrawIcon(TRUE, scrnpos->x - 16 , scrnpos->z - 16, (_XUserItem*)&m_MouseSlotItem, FALSE);
		
		((CEmperorOfDragonsApp*)gApp)->ReadyCursor(_XMOUSETYPE_SELECT);
	}	
}

void _XWindow_SocketDetach::DrawSelectedItem(void)
{
	if(m_MouseSlotItem.Get_m_sID() > 0)
	{
		int selectedslot = m_MouseSlotItem.m_InventoryNumber;
		
		if( selectedslot > -1 )
		{
			_XDrawRectAngle(m_WindowPosition.x+m_SlotPosition[selectedslot].x,
				m_WindowPosition.y+m_SlotPosition[selectedslot].y,
				m_WindowPosition.x+m_SlotPosition[selectedslot].x+31,
				m_WindowPosition.y+m_SlotPosition[selectedslot].y+31, 1.0f, _XSC_ITEM_SELECT);		
			
			if(m_bHaveMouseSlot)
			{
				// disable icon image
				_XDrawSolidBar( m_WindowPosition.x+m_SlotPosition[selectedslot].x+1,
					m_WindowPosition.y+m_SlotPosition[selectedslot].y+1,
					m_WindowPosition.x+m_SlotPosition[selectedslot].x+30, 
					m_WindowPosition.y+m_SlotPosition[selectedslot].y+30, D3DCOLOR_ARGB(160, 0, 0, 0));
			}					
		}		
	}
}

void _XWindow_SocketDetach::DrawSocketPlugInionItem()
{
	D3DXMATRIX			rotmat;
	D3DXMatrixIdentity(&rotmat);
	
	m_SocketRotateAngle++;
	D3DXMatrixRotationY(&rotmat, _X_RAD(m_SocketRotateAngle));

#ifdef _XTS_ITEM_OPTIMIZATION
	char cType = GetEqualItemType(m_IndependentItem->Get_m_cType());
	if( cType == _XGI_FC_WEAPON )
#else
	if( m_IndependentItem->Get_m_cType() == _XGI_FC_WEAPON || m_IndependentItem->Get_m_cType() == _XGI_FC_WEAPON2 || m_IndependentItem->Get_m_cType() == _XGI_FC_WEAPON3 )
#endif
	{
		D3DXMATRIX			zrotmat;
		D3DXMatrixRotationZ(&zrotmat, _X_RAD(45.0f));
		
		D3DXMatrixMultiply( &rotmat, &rotmat, &zrotmat );
	}	
	
	// Initialize Matrix
	int modelindex = GetSocketModelIndex();
	D3DXMatrixIdentity(&m_SocketRHWMatrix);
	
	// Camera/ViewPort Setting
	m_SocketModelViewCamera.UpdateViewMatrix( NULL, FALSE );
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&m_SocketModelViewCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&m_SocketModelViewCamera.GetProjMatrix());
	gpDev->SetViewport(&m_SocketModelViewPort);
	
	// Z Buffer를 clear 한다. - 현재 viewport영역만 clear된다.
	gpDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);
	
	// Rendering			
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );	
	
	
	
	FLOAT scalefactor;
#ifdef _XTS_ITEM_OPTIMIZATION
	switch(cType) 
#else
		switch(m_IndependentItem->m_cType) 
#endif
	{
#ifdef _XTS_ITEM_OPTIMIZATION
	case _XGI_FC_WEAPON:
#else
	case _XGI_FC_WEAPON:
	case _XGI_FC_WEAPON2:
	case _XGI_FC_WEAPON3:
#endif
		{
			D3DXMatrixTranslation( &m_SocketRHWMatrix, 
				-g_ItemModel_Weapon[0][modelindex]->OBB_Center.x,
				-g_ItemModel_Weapon[0][modelindex]->OBB_Center.y,
				-g_ItemModel_Weapon[0][modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_Weapon[0][modelindex]->m_ObjectRadius;
			D3DXMATRIX matRot;
			D3DXMatrixIdentity(&matRot);
			D3DXMatrixScaling(&matRot, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply( &matRot, &matRot, &rotmat );							
			D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &matRot);
			
			g_ItemModel_Weapon[0][modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
		}		
		break;
#ifdef _XTS_ITEM_OPTIMIZATION
	case _XGI_FC_CLOTHES:
#else
	case _XGI_FC_CLOTHES:
	case _XGI_FC_CLOTHES2:
	case _XGI_FC_CLOTHES3:
#endif
		{
			switch( m_IndependentItem->Get_m_cSecond() ) 
			{
			case _XGI_SC_CLOTHES_UPPER:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_UBodyType[modelindex]->OBB_Center.x,
						-g_UserModel_UBodyType[modelindex]->OBB_Center.y,
						-g_UserModel_UBodyType[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_UBodyType[modelindex]->m_ObjectRadius;
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_UBodyType[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_UBodyType[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_LOWER:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_LBodyType[modelindex]->OBB_Center.x,
						-g_UserModel_LBodyType[modelindex]->OBB_Center.y,
						-g_UserModel_LBodyType[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_LBodyType[modelindex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_LBodyType[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_LBodyType[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_GLOVES:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_LHandType[modelindex]->OBB_Center.x,
						-g_UserModel_LHandType[modelindex]->OBB_Center.y,
						-g_UserModel_LHandType[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_LHandType[modelindex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = 0.0f;
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_LHandType[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_SHOES:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_LFootType[modelindex]->OBB_Center.x,
						-g_UserModel_LFootType[modelindex]->OBB_Center.y,
						-g_UserModel_LFootType[modelindex]->OBB_Center.z	);
					scalefactor = 0.28f / g_UserModel_LFootType[modelindex]->m_ObjectRadius;					
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = scalefactor*g_UserModel_LFootType[modelindex]->OBB_Center.x;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_LFootType[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_LFootType[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);					
				}
				break;
			case _XGI_SC_CLOTHES_CAP:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_Cap[modelindex]->OBB_Center.x,
						-g_UserModel_Cap[modelindex]->OBB_Center.y,
						-g_UserModel_Cap[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_Cap[modelindex]->m_ObjectRadius;
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_Cap[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_Cap[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
				}
				break;			
			case _XGI_SC_CLOTHES_BACKPACK:
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_BackPack[modelindex]->OBB_Center.x,
						-g_UserModel_BackPack[modelindex]->OBB_Center.y,
						-g_UserModel_BackPack[modelindex]->OBB_Center.z	);
					scalefactor = 0.4f / g_UserModel_BackPack[modelindex]->m_ObjectRadius; 
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_BackPack[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_BackPack[modelindex]->RenderInterfaceItem( &m_SocketRHWMatrix );
				}
				break;			
			}
			case _XGI_SC_CLOTHES_MASK : //Author : 양희왕 //breif : 08/01/24 변경 g_UserModel_UBodyType->g_UserModel_Mask
				{
					D3DXMatrixTranslation( &m_SocketRHWMatrix, 
						-g_UserModel_Mask[modelindex]->OBB_Center.x,
						-g_UserModel_Mask[modelindex]->OBB_Center.y,
						-g_UserModel_Mask[modelindex]->OBB_Center.z	);
					scalefactor = 0.5f / g_UserModel_Mask[modelindex]->m_ObjectRadius;
					
					D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
					m_SocketRHWMatrix._41 = 0.0f;
					m_SocketRHWMatrix._42 = -(scalefactor*g_UserModel_Mask[modelindex]->OBB_Center.y);
					m_SocketRHWMatrix._43 = 0.0f;
					
					g_UserModel_Mask[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
				}
				break;
		}		
		break;
	case _XGI_FC_POTION:
		{
			D3DXMatrixTranslation( &m_SocketRHWMatrix, 
				-g_ItemModel_Potion[modelindex]->OBB_Center.x,
				-g_ItemModel_Potion[modelindex]->OBB_Center.y,
				-g_ItemModel_Potion[modelindex]->OBB_Center.z	);
			scalefactor = 0.5f / g_ItemModel_Potion[modelindex]->m_ObjectRadius;			
			
			D3DXMatrixScaling(&m_SocketRHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_SocketRHWMatrix, &m_SocketRHWMatrix, &rotmat);
			m_SocketRHWMatrix._41 = 0.0;
			m_SocketRHWMatrix._42 = -(scalefactor*g_ItemModel_Potion[modelindex]->OBB_Center.y);;
			m_SocketRHWMatrix._43 = 0;
			
			g_ItemModel_Potion[modelindex]->RenderInterfaceItem(&m_SocketRHWMatrix);
		}
		break;
	}	
	
	// View Matrix, Projection Matrix, Viewport 다시 되돌린다. ====
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetProjMatrix());
	gpDev->SetViewport(&g_MainViewPort);
}

int _XWindow_SocketDetach::GetSocketModelIndex()
{
	int index=0;
#ifdef _XTS_ITEM_OPTIMIZATION
	switch( GetEqualItemType(m_IndependentItem->Get_m_cType()) ) 
	{
	case _XGI_FC_WEAPON:
		{
			_XGI_WeaponItem_Property* WeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_IndependentItem->Get_m_cType());
			if( WeaponItem )
				index = WeaponItem[m_IndependentItem->Get_m_sID()].sModelIndex;															
		}		
		break;
	case _XGI_FC_CLOTHES:
		{
			if( m_IndependentItem->Get_m_cSecond() == _XGI_SC_CLOTHES_MASK )
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_IndependentItem->Get_m_cType());
				if( ClothesItem )
				{
					index = ClothesItem[m_IndependentItem->Get_m_sID()].sModelIndex;
					if( ClothesItem[m_IndependentItem->Get_m_sID()].cSexType ) // 여자면 인덱스 추가.
					{
						// 기존에 배열된 모델 컨테이너 영역을 모두 사용하여 새로 추가된 모델의 인덱스 계산.
						if( index >= 140 )
						{
							// 새로 추가된 여자 모델 컨테이너 는 220번 부터임. 남자는 140번부터 이므로 두번째 모델 컨테이너 구분은 80칸씩
							index+= g_MaskModel_GenderSeparateIndex2;
						}
						else
						{
							index+= g_MaskModel_GenderSeparateIndex;
						}
					}
				}
			}
			else
			{
				_XGI_ClothesItem_Property* ClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty(m_IndependentItem->Get_m_cType());
				if( ClothesItem )
				{
					index = ClothesItem[m_IndependentItem->Get_m_sID()].sModelIndex;
					if( ClothesItem[m_IndependentItem->Get_m_sID()].cSexType )  // 여자면 인덱스 추가.
						index+= g_Model_GenderSeparateIndex;
				}			
			}
		}		
		break;	
	case _XGI_FC_POTION:
		{
			_XGI_PotionItem_Property* PotionItem = (_XGI_PotionItem_Property*)GetFirstTypeItemPorperty(m_IndependentItem->Get_m_cType());
			if( PotionItem )
				index = PotionItem[m_IndependentItem->Get_m_sID()].sModelIndex;
		}
		break;
	}
#else
	switch(m_IndependentItem->m_cType) 
	{
	case _XGI_FC_WEAPON:
		{
			index = g_WeaponItemProperty[m_IndependentItem->m_sID].sModelIndex;															
		}		
		break;
	case _XGI_FC_CLOTHES:
		{
			
			index = g_ClothesItemProperty[m_IndependentItem->m_sID].sModelIndex;
			if( g_ClothesItemProperty[m_IndependentItem->m_sID].cSexType ) 
				index+= g_Model_GenderSeparateIndex;
		}		
		break;
	case _XGI_FC_WEAPON2:
		{
			index = g_WeaponItemProperty2[m_IndependentItem->m_sID].sModelIndex;															
		}		
		break;
	case _XGI_FC_CLOTHES2:
		{
			
			index = g_ClothesItemProperty2[m_IndependentItem->m_sID].sModelIndex;
			if( g_ClothesItemProperty2[m_IndependentItem->m_sID].cSexType ) 
				index+= g_Model_GenderSeparateIndex;
		}		
		break;
	case _XGI_FC_POTION:
		{			
			index = g_PotionItemProperty[m_IndependentItem->m_sID].sModelIndex;
		}
		break;
	case _XGI_FC_WEAPON3:
		{
			index = g_WeaponItemProperty3[m_IndependentItem->m_sID].sModelIndex;															
		}		
		break;
	case _XGI_FC_CLOTHES3:
		{
			
			index = g_ClothesItemProperty3[m_IndependentItem->m_sID].sModelIndex;
			if( g_ClothesItemProperty3[m_IndependentItem->m_sID].cSexType ) 
				index+= g_Model_GenderSeparateIndex;
		}		
		break;
	}
#endif
	
	return index;
}