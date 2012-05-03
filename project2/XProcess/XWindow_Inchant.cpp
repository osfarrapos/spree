// XWindow_WeaponUpgrade.cpp: implementation of the _XWindow_WeaponUpgrade class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XProc_ProcessDefine.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XMessageWindow.h"
#include "XWindow_Inchant.h"
#include "XWindow_ChargeGauge.h"
#include "XWindowObjectDefine.h"
#include "SoundEffectList.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_Inchant::_XWindow_Inchant()
{
	m_InchantApplyButton	= NULL;
	m_pApplyCancleButton	= NULL;
	//m_pClosebutton			= NULL;
	
	m_InchantStep			= US_NONE;
	m_InchantItem			= NULL;
	m_slotImage				= NULL;
}

_XWindow_Inchant::~_XWindow_Inchant()
{

}

BOOL _XWindow_Inchant::Initialize( void )
{
#ifdef _XDEF_SMELT_ITEM // Author : 양희왕
	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 184, 71, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" ) );	
	pTitleBar->SetClipRect( 0, 125, 183, 195 );
	InsertChildObject( pTitleBar );

	_XImageStatic* pBackBoard = new _XImageStatic;
	pBackBoard->Create( 0, 60, 184, 271, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" ) );	
	pBackBoard->SetClipRect( 0, 1, 183, 211 );
	InsertChildObject( pBackBoard );

	_XImageStatic* pSocketItemLeftTopView = new _XImageStatic;
	pSocketItemLeftTopView->Create( 7, 26, 99, 95, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	pSocketItemLeftTopView->SetClipRect(  4, 180, 96, 249 );
	InsertChildObject( pSocketItemLeftTopView );

	_XImageStatic* pSocketItemRightTopView = new _XImageStatic;
	pSocketItemRightTopView->Create( 176, 26, 253, 95, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	pSocketItemRightTopView->SetClipRect(  4, 180, 81, 249);
	pSocketItemRightTopView->SetScale( -1.0f, 1.0f );
	InsertChildObject( pSocketItemRightTopView );

	_XImageStatic* pSocketItemLeftDownView = new _XImageStatic;
	pSocketItemLeftDownView->Create( 7, 95, 99, 165, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	pSocketItemLeftDownView->SetClipRect(  4, 185, 96, 254 );
	InsertChildObject( pSocketItemLeftDownView );

	_XImageStatic* pSocketItemRightDownView = new _XImageStatic;
	pSocketItemRightDownView->Create( 176, 95, 253, 165, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	pSocketItemRightDownView->SetClipRect(  4, 185, 81, 254 );
	pSocketItemRightDownView->SetScale( -1.0f, 1.0f );
	InsertChildObject( pSocketItemRightDownView );

	// 버튼들 ---------------------------------------------------------------------------------------------------------=
	//int applybtnindex = g_MainInterfaceTextureArchive.FindResource( "mi_skillwin.tga" );
	int applybtnindex = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	_XBTN_STRUCTURE socketpluginbtnstruct = 
	{ 
		TRUE, { 49, 242  }, { 60, 24 }, 
		_XDEF_INCHANT_STARTBUTTON, applybtnindex, applybtnindex, applybtnindex, &g_MainInterfaceTextureArchive 
	};
	m_InchantApplyButton = new _XButton;
	m_InchantApplyButton->Create( socketpluginbtnstruct );	
	m_InchantApplyButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_InchantApplyButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_OK);		// _T("확인")
	m_InchantApplyButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 187, 152, 247, 176 );
	m_InchantApplyButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  187, 127, 247, 151 );
	m_InchantApplyButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	m_InchantApplyButton->ShowWindow(FALSE);
	InsertChildObject( m_InchantApplyButton );	
	_XBTN_STRUCTURE applycanclebtnstruct = 
	{ 
		TRUE, { 117, 242  }, { 60, 24 }, 
		_XDEF_INCHANT_CANCLEBUTTON, applybtnindex, applybtnindex, applybtnindex, &g_MainInterfaceTextureArchive
	};
	m_pApplyCancleButton = new _XButton;
	m_pApplyCancleButton->Create( applycanclebtnstruct );	
	m_pApplyCancleButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_pApplyCancleButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CANCLE);		// _T("취소")
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 187, 152, 247, 176 );
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  187, 127, 247, 151 );
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	InsertChildObject( m_pApplyCancleButton );


	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );

	_XBTN_STRUCTURE helpstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_INCHANT_HELPBUTTON,
	charinfointerface,charinfointerface,charinfointerface, 
	&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );

	m_slotImage = new _XImageStatic;
	m_slotImage->Create( 12, 172, 44, 204, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" ) );
	m_slotImage->SetClipRect( 214, 31, 246, 63 );
	InsertChildObject( m_slotImage );
#else
	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create( 0, 0, 184, 71, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" ) );	
	pTitleBar->SetClipRect( 0, 125, 183, 195 );
	InsertChildObject( pTitleBar );

	_XImageStatic* pBackBoard = new _XImageStatic;
	pBackBoard->Create( 0, 60, 184, 271, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" ) );	
	pBackBoard->SetClipRect( 0, 1, 183, 211 );
	InsertChildObject( pBackBoard );

	_XImageStatic* pSocketItemLeftView = new _XImageStatic;
	pSocketItemLeftView->Create( 7, 26, 93, 141, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	pSocketItemLeftView->SetClipRect( 4, 140, 89, 254 );
	InsertChildObject( pSocketItemLeftView );

	_XImageStatic* pSocketItemRightView = new _XImageStatic;
	pSocketItemRightView->Create( 176, 26, 261, 141, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	pSocketItemRightView->SetClipRect( 4, 140, 88, 254 );
	pSocketItemRightView->SetScale( -1.0f, 1.0f );
	InsertChildObject( pSocketItemRightView );

	int mainimagebackindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );
	
	
	m_slotImage = new _XImageStatic;
	m_slotImage->Create( 12, 172, 44, 204, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" ) );
	m_slotImage->SetClipRect( 214, 31, 246, 63 );
	InsertChildObject( m_slotImage );

	// 버튼들 ---------------------------------------------------------------------------------------------------------=
	int applybtnindex = g_MainInterfaceTextureArchive.FindResource( "mi_skillwin.tga" );
	_XBTN_STRUCTURE socketpluginbtnstruct = 
	{ 
		TRUE, { 9, 242  }, { 104, 20 }, 
		_XDEF_INCHANT_STARTBUTTON, applybtnindex, applybtnindex, applybtnindex, &g_MainInterfaceTextureArchive 
	};
	m_InchantApplyButton = new _XButton;
	m_InchantApplyButton->Create( socketpluginbtnstruct );	
	m_InchantApplyButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_InchantApplyButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  2, 152, 110, 176 );
	m_InchantApplyButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 2, 176, 110, 200 ); 
	m_InchantApplyButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  2, 201, 110, 225 );
	m_InchantApplyButton->ShowWindow(FALSE);
	InsertChildObject( m_InchantApplyButton );	

	applybtnindex = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	_XBTN_STRUCTURE applycanclebtnstruct = 
	{ 
		TRUE, { 117, 242  }, { 60, 24 }, 
		_XDEF_INCHANT_CANCLEBUTTON, applybtnindex, applybtnindex, applybtnindex, &g_MainInterfaceTextureArchive
	};
	m_pApplyCancleButton = new _XButton;
	m_pApplyCancleButton->Create( applycanclebtnstruct );	
	m_pApplyCancleButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_pApplyCancleButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CANCLE);		// _T("취소")
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY, 187, 152, 247, 176 );
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  187, 127, 247, 151 );
	m_pApplyCancleButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	InsertChildObject( m_pApplyCancleButton );

	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
	/*_XBTN_STRUCTURE closebtnstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_INCHANT_CANCLEBUTTON,
									  charinfointerface,charinfointerface,charinfointerface, 
									  &g_MainInterfaceTextureArchive };
	m_pClosebutton = new _XButton;
	m_pClosebutton->Create( closebtnstruct );	
		
	m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL,  96, 238, 108, 250 );
	m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  108, 238, 120, 250 );
	m_pClosebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  120, 238, 132, 250 );
	InsertChildObject( m_pClosebutton );*/

	
	_XBTN_STRUCTURE helpstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_INCHANT_HELPBUTTON,
		charinfointerface,charinfointerface,charinfointerface, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
	// ----------------------------------------------------------------------------------------------------------------=	

	// Model View Camera Setting =========================================
	m_InchantItemModelViewCamera.Set( 45, 154.0f/60.0f, 0.01f, 180.0f);
	m_InchantItemModelViewCamera.Set(0.0f, 0.0f, 0.0f);	
	m_InchantItemModelViewCamera.SetDistanceLimits( 0.01, 30.0f );
	m_InchantItemModelViewCamera.SetDistance( 1.0f );	
	m_InchantItemModelViewCamera.UpdateViewMatrix();
	m_InchantItemModelViewCamera.UpdateProjMatrix();

	// ViewPortReposition - for Model View
	m_InchantItemModelViewPort.X = (DWORD)(m_WindowPosition.x + 14);
	m_InchantItemModelViewPort.Y = (DWORD)(m_WindowPosition.y + 73);
	m_InchantItemModelViewPort.Width  = 154;
	m_InchantItemModelViewPort.Height = 60;
	m_InchantItemModelViewPort.MinZ = 0.0f;
	m_InchantItemModelViewPort.MaxZ = 1.0f;

	m_InchantItemtRotateAngle = 0.0f;
#endif
	
	return TRUE;
}

void _XWindow_Inchant::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_Inchant::Draw( _XGUIObject*& pfocusobject )
{
	if(!this->m_ShowWindow)	return;
	_XWindow::Draw(pfocusobject);

	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

#ifdef _XDEF_SMELT_ITEM // Author : 양희왕
	switch(m_InchantStep) 
	{		
	case US_NONE :
	case US_READY :
	case US_STEP1 :	// 재료를 올린 상태
		{
			m_slotImage->Draw();
			
			g_XBaseFont->SetColor(_XSC_DEFAULT);
			g_XBaseFont->Puts_Separate( m_WindowPosition.x+54, m_WindowPosition.y+174, m_InchantInfoText, 114, 1.0, 3 );
		}
		break;
	case US_STEP2 :
		{
			g_XBaseFont->SetColor(_XSC_DEFAULT);
			g_XBaseFont->Puts_Separate( m_WindowPosition.x+15, m_WindowPosition.y+174, m_InchantInfoText, 158, 1.0, 3 );
		}
		break;
	default :
		{
			g_XBaseFont->SetColor(_XSC_DEFAULT);
			g_XBaseFont->Puts_Separate( m_WindowPosition.x+20, m_WindowPosition.y+174, m_InchantInfoText, 150, 1.0, 3 );
		}
		break;
	}	

	if( m_InchantItem )
	{
		g_XBaseFont->SetClippingArea(m_WindowPosition.x, m_WindowPosition.y, 
									 m_WindowPosition.x + m_WindowSize.cx - 16, m_WindowPosition.y + m_WindowSize.cy );
		int nCurrentDrawPosY = 34;
		int nDrawPosX = m_WindowPosition.x + 18; //기본 찍는 위치,,

		g_XBaseFont->PutsAlign( m_WindowPosition.x + m_WindowSize.cx/2, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3443) ); //[제련 정보]

		TCHAR tempString[128];
		memset(tempString, 0, sizeof(tempString));
		sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3433) ); //무기:

		int nNextPosX = strlen(tempString) * 6; //옆에 붙는 글씨 찍을 좌표인데,, 한글자가 보통 6픽셀정도..

		nCurrentDrawPosY += 20;
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->PutsAlign(nDrawPosX, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

		g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_InchantItem->Get_m_cType(), m_InchantItem->Get_m_sID()));

		if( _XGameItem::GetItemNick(m_InchantItem->Get_m_cType(), m_InchantItem->Get_m_sID() ) )
		{
			memset(tempString, 0, sizeof(tempString));
			sprintf( tempString, "%s %s", _XGameItem::GetItemNick(m_InchantItem->Get_m_cType(), m_InchantItem->Get_m_sID()) , 
												 _XGameItem::GetItemName(m_InchantItem->Get_m_cType(), m_InchantItem->Get_m_sID()) );

			g_XBaseFont->PutsAlign(nDrawPosX + nNextPosX + 4, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString ); //+4 는 띄어쓰기 할려고
		}
		else
		{
			memset(tempString, 0, sizeof(tempString));
			sprintf( tempString, "%s", _XGameItem::GetItemName(m_InchantItem->Get_m_cType(), m_InchantItem->Get_m_sID())); 
			g_XBaseFont->PutsAlign(nDrawPosX + nNextPosX + 4, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
		}

		int nDrawSize = (m_WindowPosition.x+m_WindowSize.cx) - (nDrawPosX + nNextPosX + 4) - 16;
		int templength = g_XBaseFont->GetRowCount( tempString, nDrawSize); //지금 무기를 찍었는데 무기 줄을 세보자
		
		if( templength > 1 ) //2줄이상인가??
		{
			if(mousestate)
			{
				if(scrnpos->x > nDrawPosX + nNextPosX + 4 && scrnpos->x < (m_WindowPosition.x+m_WindowSize.cx) - 16 && //무기 찍은 위치 rect
				   scrnpos->z > m_WindowPosition.y + nCurrentDrawPosY  && scrnpos->z < m_WindowPosition.y + nCurrentDrawPosY + 16)
				{
					//글씨 색으로,, 위에 덮어서 찍는다
					g_InterfaceTooltip.SetToolTipString( nDrawPosX + nNextPosX + 4, m_WindowPosition.y + nCurrentDrawPosY, tempString, _XGameItem::GetGradeColor(m_InchantItem->Get_m_cType(), m_InchantItem->Get_m_sID()) );
				}
			}
		}

		memset(tempString, 0, sizeof(tempString));
		sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3444), m_InchantItem->Get_m_ucStrength()+1 ); //단계: %d 단계
		
		nCurrentDrawPosY += 16;
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
		
		LPTSTR  param1 = _XGameItem::GetItemName( g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].cType, g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].sID );
		int		param2 = g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].ucCount;

		memset(tempString, 0, sizeof(tempString)); 
		sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3445), param1, param2 ); //재료: %s %d개

		nCurrentDrawPosY += 16;
		g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

		nDrawSize = (m_WindowPosition.x+m_WindowSize.cx) - (m_WindowPosition.x + 18) - 16;
		templength = g_XBaseFont->GetRowCount( tempString, nDrawSize); //지금은 재료의 줄수
		if( templength > 1 ) //2줄이상인가??
		{
			if(mousestate)
			{
				if(scrnpos->x > m_WindowPosition.x + 18 && scrnpos->x < m_WindowPosition.x + m_WindowSize.cx - 16 && //찍은 위치 rect
				   scrnpos->z > m_WindowPosition.y + nCurrentDrawPosY  && scrnpos->z < m_WindowPosition.y + nCurrentDrawPosY + 16)
				{
					//글씨 색으로,, 위에 덮어서 찍는다
					g_InterfaceTooltip.SetToolTipString( m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, tempString, _XSC_DEFAULT );
				}
			}
		}

		memset(tempString, 0, sizeof(tempString));
		sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3446), g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].uiPrice ); //비용: %d 금
		
		nCurrentDrawPosY += 16;
		g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

		//강도를 넣을 부분
		nCurrentDrawPosY += 16;
		g_XBaseFont->SetColor( 0xFF00F064 );
		
		memset(tempString, 0, sizeof(tempString));
		sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3447) ); //강도
		g_XBaseFont->PutsAlign(m_WindowPosition.x + 18, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
		g_XBaseFont->Flush();

		nNextPosX = strlen(tempString) * 6; //강도 글자 넓이 계산

		g_XBaseFont->SetColor( _XSC_DEFAULT );
		memset(tempString, 0, sizeof(tempString));

		sprintf( tempString, ":" ); 
		g_XBaseFont->PutsAlign(nDrawPosX + nNextPosX, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );

		nNextPosX += 6; // : 글자가 늘어서 6 더해준다

		int nSmallFontDraw = 0;
		for( int i = 0; i < m_InchantItem->m_usCurMaxProtect; i++)
		{
			if( m_InchantItem->m_usCurProtect > i )
			{
				_XDrawSolidBar( nDrawPosX + nNextPosX + 4 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
					nDrawPosX + nNextPosX + 4 +2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,0,240,100));
			}
			else
			{
				_XDrawSolidBar( nDrawPosX + nNextPosX + 4 +(i*3), m_WindowPosition.y + nCurrentDrawPosY,
					nDrawPosX + nNextPosX + 4 +2+(i*3), m_WindowPosition.y + nCurrentDrawPosY+11, D3DCOLOR_ARGB(255,255,255,255));
			}
			nSmallFontDraw = (i*3);
		}
		g_XSmallFont->SetColor( _XSC_DEFAULT );
		memset(tempString, 0, sizeof(tempString));
		sprintf( tempString, "(%d/%d)", m_InchantItem->m_usCurProtect, 
										m_InchantItem->m_usCurMaxProtect);

		g_XSmallFont->PutsAlign(nDrawPosX + nNextPosX + 4 + 10 + nSmallFontDraw, m_WindowPosition.y + nCurrentDrawPosY, _XFONT_ALIGNTYPE_LEFT, tempString );
		g_XSmallFont->Flush();	

		if( m_InchantItem->m_usCurProtect > 0 )
		{
			g_XBaseFont->SetColor( 0xFF00F064 );

			memset(tempString, 0, sizeof(tempString));
			sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3448) );  //실패 시 무기 깨지지 않음

			templength = g_XBaseFont->GetRowCount( tempString, 150 ); 
			if( templength > 1 )
				nCurrentDrawPosY += 16;
			else
				nCurrentDrawPosY += 28;

			g_XBaseFont->Puts_SeparateAlign( m_WindowPosition.x+18, m_WindowPosition.y + nCurrentDrawPosY, tempString, 150, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
		}
		else
		{
			g_XBaseFont->SetColor( 0xFFFA4141 );

			memset(tempString, 0, sizeof(tempString));
			sprintf( tempString, _XGETINTERFACETEXT(ID_STRING_NEW_3449) );  //실패 시 무기 깨짐	

			templength = g_XBaseFont->GetRowCount( tempString, 150 ); 
			if( templength > 1 )
				nCurrentDrawPosY += 16;
			else
				nCurrentDrawPosY += 28;

			g_XBaseFont->Puts_SeparateAlign( m_WindowPosition.x+18, m_WindowPosition.y + nCurrentDrawPosY, tempString, 150, _XFONT_ALIGNTYPE_LEFT, 1.0, 3 );
		}

		g_XBaseFont->SetColor(_XSC_DEFAULT);

		switch(m_InchantStep) 
		{
		case US_NONE :
		case US_READY :
		case US_STEP1 :	// 재료를 올린 상태
			{
				g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+13, m_WindowPosition.y+173, &m_InchantStuff[0], FALSE);
				_XDrawSolidBar( m_WindowPosition.x+13, m_WindowPosition.y+173, m_WindowPosition.x+45, m_WindowPosition.y+205, D3DCOLOR_ARGB(160, 0, 0, 0) );
				g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+13, m_WindowPosition.y+173, &m_InchantStuff[1]);
			}
			break;
		}

		g_XBaseFont->DisableClippingArea();
	}

	g_XBaseFont->Flush();
#else
	_XDrawSolidBar( m_WindowPosition.x+8, m_WindowPosition.y+144, m_WindowPosition.x+172, m_WindowPosition.y+235, 0xFF1A1819 );
	_XDrawRectAngle( m_WindowPosition.x+8, m_WindowPosition.y+144, m_WindowPosition.x+172, m_WindowPosition.y+235, 0.0f, 0xFF000000 );

	m_slotImage->Draw();
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	g_XBaseFont->Puts_Separate( m_WindowPosition.x+54, m_WindowPosition.y+174, m_InchantInfoText, 114, 1.0, 3 );

	if( m_InchantItem )
	{
		DrawInchantItem();
		
		g_XBaseFont->SetColor(_XGameItem::GetGradeColor(m_InchantItem->Get_m_cType(), m_InchantItem->Get_m_sID()));
#ifdef _XTS_ITEM_V20
		if( _XGameItem::GetItemNick(m_InchantItem->Get_m_cType(), m_InchantItem->Get_m_sID() ) )
#else
		int tempLength = strlen( _XGameItem::GetItemNick(m_InchantItem->m_cType, m_InchantItem->Get_m_sID()) );
		if( tempLength > 1 )
#endif
		{

			g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+32, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemNick(m_InchantItem->Get_m_cType(), m_InchantItem->Get_m_sID() ) );
			g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+47, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemName(m_InchantItem->Get_m_cType(), m_InchantItem->Get_m_sID() ) );
		}
		else
		{
			g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+40, _XFONT_ALIGNTYPE_CENTER, _XGameItem::GetItemName(m_InchantItem->Get_m_cType(), m_InchantItem->Get_m_sID() ) );
		}

		TCHAR tempSTr[128];
		sprintf( tempSTr, _XGETINTERFACETEXT(ID_STRING_INCHANT_2396), m_InchantItem->Get_m_ucStrength() +1 );
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		g_XBaseFont->PutsAlign(m_WindowPosition.x+91, m_WindowPosition.y+151, _XFONT_ALIGNTYPE_CENTER, tempSTr );

		g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+13, m_WindowPosition.y+173, &m_InchantStuff[0], FALSE);
		_XDrawSolidBar( m_WindowPosition.x+13, m_WindowPosition.y+173, m_WindowPosition.x+45, m_WindowPosition.y+205, D3DCOLOR_ARGB(160, 0, 0, 0) );
		g_pInventory_Window->DrawIcon(TRUE, m_WindowPosition.x+13, m_WindowPosition.y+173, &m_InchantStuff[1]);
	}
	g_XBaseFont->Flush();
#endif
}

BOOL _XWindow_Inchant::Process( _XGUIObject*& pfocusobject )
{
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	MouseState* mousestate = gpInput->GetMouseState();
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

	if(mousestate)
	{
		if(!pfocusobject)		// focus 가 없을 때 
		{
			if(mousestate->bButton[0])
			{
				// mouse left down
				if(CheckMousePosition())
				{
					// Focus
					if(scrnpos->x > m_WindowPosition.x+12 && scrnpos->x < m_WindowPosition.x+44)
					{
						if(scrnpos->z > m_WindowPosition.y+172 && scrnpos->z < m_WindowPosition.y+203)
						{
							//pfocusobject = _XDEF_DUMMYID_INCHANT;
							m_WindowMouseState = _XW_STATE_NONE;
						}
					}
					else
					{
						m_WindowMouseState = _XW_STATE_LEFTCLICK; // 2004.06.16->oneway48 insert
					}
					
					if(g_pInventory_Window)	// 인벤토리에서 드래그 했을 경우
					{
						if(g_pInventory_Window->GetShowStatus())
						{
							if(g_pInventory_Window->m_bHaveMouseSlot && g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART)
							{
								if(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber].Get_m_sID() > 0  && 
								   g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber >= _XINVENTORY_SLOTTYPE_POCKETSTART)
								{
#ifdef _XTS_ITEM_OPTIMIZATION
									if( _XGI_FC_WEAPON == GetEqualItemType(g_pLocalUser->m_UserItemList[ g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber ].Get_m_cType()) )
#else
									if( g_pLocalUser->m_UserItemList[ g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber ].Get_m_cType() == _XGI_FC_WEAPON ||
										g_pLocalUser->m_UserItemList[ g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber ].Get_m_cType() == _XGI_FC_WEAPON2 ||
										g_pLocalUser->m_UserItemList[ g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber ].Get_m_cType() == _XGI_FC_WEAPON3 )
#endif
									{
										// -----------------------------------------------------------------------------------------------=
										if( m_InchantItem )		// 이미 아이템이 올려져 있는 경우
										{
											MessagePrint(5);
											return FALSE;
										}
										else		// 제련할 item 이 없을 경우
										{
#ifdef _XDEF_PERIODITEM_20061027
											if(ProcessInchantCashItem(g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber))
											{
												// 기간제 아이템이다. - message handler에서 처리
												return TRUE;
											}
											else
											{
												// 기간제 아이템이 아님
												return ProcessInchant();
											}
#else
											// 제련이 가능한 Item 인지 검사
											if( CheckWeaponGrade(g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber) == 1 )
											{
												m_InchantItem = &g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber];
												g_pInventory_Window->m_UsedLifeItemIndex = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;
												SetItemInchantStep(US_READY);
												g_pInventory_Window->m_bHaveMouseSlot = FALSE;
												g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();	
											}
											else
												return FALSE;
											
											return TRUE;
#endif
										}
									}
									else if( g_pLocalUser->m_UserItemList[ g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber ].Get_m_cType() == _XGI_FC_LIFE )
									{
										if( m_InchantItem )
										{							
											if( m_InchantStuff[1].Get_m_sID() != 0 )
											{
												// 이미 다른 아이템이 있음
												MessagePrint(6);
												return FALSE;
											}
											else
											{
												// 재료 갯수 검사 --------------------------------------------------------------------------------=
												int check = CheckStuffCount(g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber);
												if( check == 1 )
												{
													m_InchantStuff[1].Copy(g_pLocalUser->m_UserItemList[g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber]);
													m_InchantStuff[1].Set_m_ucCount(m_InchantStuff[0].Get_m_ucCount());
													g_pInventory_Window->m_iSocketIndexList[0] = g_pInventory_Window->m_MouseSlotItem.m_InventoryNumber;
													SetItemInchantStep(US_STEP1);
													g_pInventory_Window->m_bHaveMouseSlot = FALSE;
													g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
													PlayItemEquipSound( m_InchantItem->Get_m_cType(), m_InchantItem->Get_m_sID() );
												}
												else if( check == -1 )
												{
													MessagePrint(7);									
												}								
												else if( check == -2 )
												{
													MessagePrint(3);
												}
												// -----------------------------------------------------------------------------------------------=
												return TRUE;
											}
										}
										else
										{
											MessagePrint(0);							
											return FALSE;
										}
									}
									else
									{
										MessagePrint(0);						
										return FALSE;
									}
								}
							}
						}
					}

					if( g_pInventory_Window )
					{
						if( g_pInventory_Window->GetShowStatus())
						{
							g_pInventory_Window->m_bDrawSelectedItem = FALSE;
							if( g_pDefaultTooltip_Window )
							{
								if( g_pDefaultTooltip_Window->GetShowStatus() )						
									g_pDefaultTooltip_Window->ShowWindow( FALSE );
							}
						}
					}
				} // if(CheckMousePosition())
				else
				{
					if( g_pDefaultTooltip_Window )
					{
						if( g_pDefaultTooltip_Window->GetShowStatus() )						
							g_pDefaultTooltip_Window->ShowWindow( FALSE );
					}
				}
			}
		}
	}
#ifdef	_XDEF_SMELT_ITEM //Author : 양희왕 
	switch(m_InchantStep) 
	{
	case US_NONE :
	case US_READY :
	case US_STEP1 :	// 재료를 올린 상태
		{
			if( m_InchantStuff[0].Get_m_sID() > 0 )
			{	
				MouseState* mousestate = gpInput->GetMouseState();
				ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

				if(mousestate)
				{
					if(CheckMousePosition())
					{
						if(scrnpos->x > m_WindowPosition.x+12 && scrnpos->x < m_WindowPosition.x+44)
						{
							if(scrnpos->z > m_WindowPosition.y+172 && scrnpos->z < m_WindowPosition.y+203)
							{
								if( g_pDefaultTooltip_Window )
								{							
									if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_INCHANT, 0, m_InchantStuff[0].Get_m_cType(), m_InchantStuff[0].Get_m_sID() ) )
									{
										g_pDefaultTooltip_Window->ShowWindow( TRUE );
									}
								}
							}
						}
						else
						{
							if( g_pDefaultTooltip_Window->GetShowStatus() )						
								g_pDefaultTooltip_Window->ShowWindow( FALSE );
						}
					}
				}
			}
		}
		break;
	}
#else
	if( m_InchantStuff[0].Get_m_sID() > 0 )
	{	
		MouseState* mousestate = gpInput->GetMouseState();
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();

		if(mousestate)
		{
			if(CheckMousePosition())
			{
				if(scrnpos->x > m_WindowPosition.x+12 && scrnpos->x < m_WindowPosition.x+44)
				{
					if(scrnpos->z > m_WindowPosition.y+172 && scrnpos->z < m_WindowPosition.y+203)
					{
						if( g_pDefaultTooltip_Window )
						{							
							if( g_pDefaultTooltip_Window->SetTooltipMode( TMODE_INCHANT, 0, m_InchantStuff[0].Get_m_cType(), m_InchantStuff[0].Get_m_sID() ) )
							{
								g_pDefaultTooltip_Window->ShowWindow( TRUE );
							}
						}
					}
				}
				else
				{
					if( g_pDefaultTooltip_Window->GetShowStatus() )						
						g_pDefaultTooltip_Window->ShowWindow( FALSE );
				}
			}
		}
	}
#endif

	if(!_XWindow::Process(pfocusobject)) return FALSE;	
	
	return TRUE;
}

void _XWindow_Inchant::ShowWindow(BOOL show)
{
	if(show)
	{
		if(g_pInventory_Window)
		{
			if(!g_pInventory_Window->m_bLockInventory)
				g_pInventory_Window->m_bLockInventory = TRUE;

			g_pInventory_Window->SetInchantWindowPtr(this);
				if( show )	
			if( g_pDefaultTooltip_Window)
				g_pDefaultTooltip_Window->SetInchantWindowPtr( this );
	
		}
	}
	else
	{
		// 사용 끝나면 Inventory lock 해제
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
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_Inchant::MoveWindow( int X, int Y )
{
	_XWindow::MoveWindow( X, Y );
	m_InchantItemModelViewPort.X = (DWORD)(m_WindowPosition.x + 14);
	m_InchantItemModelViewPort.Y = (DWORD)(m_WindowPosition.y + 73);
}

void _XWindow_Inchant::DrawInchantItem( void )
{
	if( !m_InchantItem )	return;

	D3DXMATRIX			rotmat;
	D3DXMatrixIdentity(&rotmat);
	
	m_InchantItemtRotateAngle++;
	D3DXMatrixRotationY(&rotmat, _X_RAD(m_InchantItemtRotateAngle));

#ifdef _XTS_ITEM_OPTIMIZATION
	if( _XGI_FC_WEAPON == GetEqualItemType(m_InchantItem->Get_m_cType()) )
#else
	if( m_InchantItem->Get_m_cType() == _XGI_FC_WEAPON || m_InchantItem->Get_m_cType() == _XGI_FC_WEAPON2 || m_InchantItem->Get_m_cType() == _XGI_FC_WEAPON3 )
#endif
	{
		D3DXMATRIX			zrotmat;
		D3DXMatrixRotationZ(&zrotmat, _X_RAD(90.0f));
		
		D3DXMatrixMultiply( &rotmat, &rotmat, &zrotmat );
	}	
	
	// Initialize Matrix
	int nID = m_InchantItem->Get_m_sID();

	int modelindex = GetInchantItemModelIndex();
	D3DXMatrixIdentity(&m_InchantItemRHWMatrix);
	D3DXMatrixTranslation( &m_InchantItemRHWMatrix, 
		-g_ItemModel_Weapon[0][modelindex]->OBB_Center.x,
		-g_ItemModel_Weapon[0][modelindex]->OBB_Center.y,
		-g_ItemModel_Weapon[0][modelindex]->OBB_Center.z	);
	
	// Camera/ViewPort Setting
	m_InchantItemModelViewCamera.UpdateViewMatrix( NULL, FALSE );
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&m_InchantItemModelViewCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&m_InchantItemModelViewCamera.GetProjMatrix());
	gpDev->SetViewport(&m_InchantItemModelViewPort);
	
	// Z Buffer를 clear 한다. - 현재 viewport영역만 clear된다.
	gpDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0xFF000000, 1.0f, 0);
	
	// Rendering			
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );	
	
	
	
	FLOAT scalefactor;
	switch(m_InchantItem->Get_m_cType()) 
	{
	case _XGI_FC_WEAPON:
	case _XGI_FC_WEAPON2:
	case _XGI_FC_WEAPON3:
		{
			scalefactor = 1.0f / g_ItemModel_Weapon[0][modelindex]->m_ObjectRadius;
			D3DXMATRIX matRot;
			D3DXMatrixIdentity(&matRot);
			D3DXMatrixScaling(&matRot, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply( &matRot, &matRot, &rotmat );							
			D3DXMatrixMultiply(&m_InchantItemRHWMatrix, &m_InchantItemRHWMatrix, &matRot);
			
			g_ItemModel_Weapon[0][modelindex]->RenderInterfaceItem(&m_InchantItemRHWMatrix);
		}		
		break;
	}	
	
	// View Matrix, Projection Matrix, Viewport 다시 되돌린다. ====
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX *)&g_LodTerrain.m_3PCamera.GetProjMatrix());
	gpDev->SetViewport(&g_MainViewPort);
}

int _XWindow_Inchant::GetInchantItemModelIndex(void)
{
	if( !m_InchantItem ) return 0;

#ifdef _XTS_ITEM_OPTIMIZATION
	if( _XGI_FC_WEAPON == GetEqualItemType(m_InchantItem->Get_m_cType()) )
	{
		_XGI_WeaponItem_Property* tempWeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty(m_InchantItem->Get_m_cType());
		if( tempWeaponItem )
			return tempWeaponItem[m_InchantItem->Get_m_sID()].sModelIndex;
	}
	return 0;
#else
	int index;
	switch(m_InchantItem->m_cType) 
	{
	case _XGI_FC_WEAPON:
		{
			index = g_WeaponItemProperty[m_InchantItem->m_sID].sModelIndex;															
		}		
		break;
	case _XGI_FC_WEAPON2:
		{
			index = g_WeaponItemProperty2[m_InchantItem->m_sID].sModelIndex;															
		}		
		break;
	case _XGI_FC_WEAPON3:
		{
			index = g_WeaponItemProperty3[m_InchantItem->m_sID].sModelIndex;															
		}		
		break;
	default:
		index = 0;
		break;
	}
	return index;
#endif
}

void _XWindow_Inchant::SetItemInchantStep(InchantStep step)
{
	m_InchantStep = step;
	RebuildStuffInfo();

	switch(m_InchantStep) 
	{
	case US_NONE :
		{
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
			m_slotImage->ShowWindow(TRUE);
#endif
			m_InchantItem = NULL;

			m_InchantStuff[0].Reset();
			m_InchantStuff[1].Reset();
			m_InchantStuff[2].Reset();

			m_InchantApplyButton->ShowWindow(FALSE);
			m_InchantApplyButton->EnableWindow(TRUE);
			m_pApplyCancleButton->EnableWindow(TRUE);		

			if( g_pInventory_Window )
			{
				g_pInventory_Window->ResetSocketIndexList();
				g_pInventory_Window->m_UsedLifeItemIndex = 0;
			}
		}
		break;
	case US_READY :
		{
			m_InchantApplyButton->ShowWindow(TRUE);
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
			m_slotImage->ShowWindow(TRUE);
			m_InchantApplyButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_OK);
#else
			m_InchantApplyButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SOCKETPLUGIN_DROPITEM);
#endif
			m_pApplyCancleButton->EnableWindow(TRUE);
			PlayItemEquipSound( m_InchantItem->Get_m_cType(), m_InchantItem->Get_m_sID() );
		}
		break;
	case US_STEP1 :	// 재료를 올린 상태
		{
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
			m_slotImage->ShowWindow(TRUE);
#endif
			m_InchantApplyButton->EnableWindow(TRUE);
			m_InchantApplyButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_OK);

			m_pApplyCancleButton->EnableWindow(TRUE);
		}
		break;
	case US_STEP2 :	// 제련버튼 클릭시
		{
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
			m_slotImage->ShowWindow(FALSE);
#endif
			m_InchantApplyButton->EnableWindow(FALSE);
			m_pApplyCancleButton->EnableWindow(FALSE);			

			TCHAR messagestring[256];
			memset(messagestring, 0, sizeof(TCHAR)*256);
			
			int		param1 = m_InchantItem->Get_m_ucStrength()+1;
			int		param2 = g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].uiPrice;
			LPSTR	param3 = _XGameItem::GetItemName(m_InchantItem->Get_m_cType(), m_InchantItem->Get_m_sID());

			strcpy( messagestring, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_INCHANT_2104, &param1, &param2, param3 ) );

			_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_INCHANTMESSAGEWINDOW);
			pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), messagestring, TRUE, _XDEF_INCHANT_STARTYESBUTTON, _XDEF_INCHANT_STARTNOBUTTON);
		}
		break;
	case US_STEP3 :	// 제련 시작 
		{
			g_NetworkKernel.SendPacket( MSG_NO_INCHANT_CTRL, inchant_mode_str_req );
		}
		break;
	case US_STEP4 :
		{
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
			m_slotImage->ShowWindow(FALSE);
#endif
			_XWindow_ChargeGauge* pChargeGauge_Window = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
			if( pChargeGauge_Window )
			{
				if(!pChargeGauge_Window->GetShowStatus())
				{
#ifdef _XDWDEBUG
					if( _XCmdLineOption("xsuperinchant",0) == 0 )
						pChargeGauge_Window->SetDelayTime( 12000.0f );
					else
						pChargeGauge_Window->SetDelayTime( 100.0f );
#else
					pChargeGauge_Window->SetDelayTime( 12000.0f );
#endif
					
					POINT pos = this->GetWindowPos();
					pos.x += 12;
					pos.y += 216;
					pChargeGauge_Window->SetDrawPosition(pos);
					pChargeGauge_Window->SetGaugeMode( _XGAUGE_INCHANT );
					pChargeGauge_Window->ShowWindow(TRUE);
					pChargeGauge_Window->SetSelectedNumber(m_FirstResult);	// 결과값으로 이용
					g_MainWindowManager.SetTopWindow( pChargeGauge_Window );

					switch(m_FirstResult) 
					{
					case 1:		// 75%
						_XPlayInterfaceSound(ID_SR_INTERFACE_INCHANT_02_WAV);
						break;
					case 2:		// 50%
						_XPlayInterfaceSound(ID_SR_INTERFACE_INCHANT_01_WAV);
						break;
					default:
						_XPlayInterfaceSound(ID_SR_INTERFACE_INCHANT_03_WAV);
						break;	// 100%
					}
				}
			}
		}
		break;
	case US_STEP5 :
		{
			g_NetworkKernel.SendPacket( MSG_NO_INCHANT_CTRL, inchant_mode_str_complete );
		}
		break;
	default:
		{
#ifdef _XDEF_SMELT_ITEM //Author : 양희왕
			m_slotImage->ShowWindow(TRUE);
#endif
			m_InchantApplyButton->ShowWindow(FALSE);
			m_InchantApplyButton->EnableWindow(TRUE);
			m_pApplyCancleButton->EnableWindow(TRUE);		
		}
		break;
	}
}

void _XWindow_Inchant::MessagePrint(int msgindex)
{
	_XMessageWindow* pFindMessageBox = (_XMessageWindow*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
	if(pFindMessageBox)
	{
		if(pFindMessageBox->GetShowStatus())
			return;
	}
	
	_XMessageWindow* pMessageBox = _XMessageWindow::CreateMessageBox(&g_MainWindowManager, _XDEF_WTITLE_SOCKETPLUGINMESSAGEWINDOW);
	if( pMessageBox )
	{
		TCHAR messagestring[256];
		memset( messagestring, 0, sizeof(TCHAR)*256 );
		
		switch(msgindex) {
		case 0:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INCHANT_2105) );
			break;
		case 1:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INCHANT_2106)); 
			break;
		case 2:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INCHANT_2107));
			break;
		case 3:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INCHANT_2108));
			break;
		case 4:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INCHANT_2109));
			break;
		case 5:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_SOCKETPLUGIN_ERROR9));
			break;
		case 6:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_INCHANT_2110));
			break;
		case 7:
			sprintf(messagestring, _XGETINTERFACETEXT(ID_STRING_NETWORKITEMTRADE_1760));
			break;
		}
		
		pMessageBox->SetMessage(_XDEF_DLGBTNTYPE_OK,  NULL, messagestring, TRUE);		
		
		g_pInventory_Window->m_bHaveMouseSlot = FALSE;
		g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
	}	
}

void _XWindow_Inchant::ClickStartButton(void)
{
	if( m_InchantStep == US_READY )
	{
		SetItemInchantStep(US_NONE);
	}
	else if( m_InchantStep == US_STEP1 )
	{
		SetItemInchantStep(US_STEP2);
	}
}

void _XWindow_Inchant::RebuildStuffInfo(void)
{
	memset(m_InchantInfoText, 0, sizeof(TCHAR)*128);
	
	switch(m_InchantStep) 
	{
	case US_NONE :
		{
			strcpy( m_InchantInfoText, _XGETINTERFACETEXT(ID_STRING_INCHANT_2111));
		}
		break;
#ifdef	_XDEF_SMELT_ITEM //Author : 양희왕
	case US_READY :
	case US_STEP1 :	// 재료를 올린 상태
		{
			if( m_InchantItem )
			{
				LPTSTR  param1 = _XGameItem::GetItemName( g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].cType, g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].sID );
				int		param2 = g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].ucCount;

				strcpy( m_InchantInfoText, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_INCHANT_2112, param1, &param2 ) );

				m_InchantStuff[0].Reset();
				m_InchantStuff[0].Set_m_cType( g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].cType );
				m_InchantStuff[0].Set_m_sID( g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].sID );
				m_InchantStuff[0].Set_m_ucCount( g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].ucCount );
				m_InchantStuff[0].Set_m_cSecond(_XGI_SC_LIFE_CONSUMING);	// 임시

#ifdef _XDEF_EXPAND_INCHANT_TABLE_080425_KUKURI
				m_InchantStuff[2].Reset();
				m_InchantStuff[2].Set_m_cType(g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].cType);
				m_InchantStuff[2].Set_m_sID( g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].sID_2 );
				m_InchantStuff[2].Set_m_ucCount(g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].ucCount);
				m_InchantStuff[2].Set_m_cSecond(_XGI_SC_LIFE_CONSUMING);	// 임시
#endif
			}
		}
		break;
	case US_STEP2 :
		{
			sprintf( m_InchantInfoText, _XGETINTERFACETEXT(ID_STRING_NEW_3450) );			// 위 사항대로 제련을 진행합니다. 제련을 진행하시겠습니까?
		}
		break;
#else
	case US_READY :
	case US_STEP1 :	// 재료를 올린 상태
	case US_STEP2 :
		{
			if( m_InchantItem )
			{
				LPTSTR  param1 = _XGameItem::GetItemName( g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].cType, g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].sID );
				int		param2 = g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].ucCount;

				strcpy( m_InchantInfoText, g_StringDataBase.CompositeString( _XSTRINGDB_SN_EODSTRING_XSR,ID_STRING_INCHANT_2112, param1, &param2 ) );

				m_InchantStuff[0].Reset();
				m_InchantStuff[0].Set_m_cType(g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].cType);
				m_InchantStuff[0].Set_m_sID( g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].sID );
				m_InchantStuff[0].Set_m_ucCount(g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].ucCount);
				m_InchantStuff[0].Set_m_cSecond(_XGI_SC_LIFE_CONSUMING);	// 임시

#ifdef _XDEF_EXPAND_INCHANT_TABLE_080425_KUKURI
				m_InchantStuff[2].Reset();
				m_InchantStuff[2].Set_m_cType(g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].cType);
				m_InchantStuff[2].Set_m_sID( g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].sID_2 );
				m_InchantStuff[2].Set_m_ucCount(g_ItemInchantTable[m_InchantItem->Get_m_ucStrength()].ucCount);
				m_InchantStuff[2].Set_m_cSecond(_XGI_SC_LIFE_CONSUMING);	// 임시
#endif
			}
		}
		break;
#endif
	case US_STEP4 :	// 제련버튼 클릭시
		{
			strcpy( m_InchantInfoText, _XGETINTERFACETEXT(ID_STRING_INCHANT_2113));
		}
		break;
	}
}

int _XWindow_Inchant::CheckStuffCount(int selectslot)
{
	if( g_pLocalUser->m_UserItemList[selectslot].Get_m_sID() == m_InchantStuff[0].Get_m_sID() &&
		g_pLocalUser->m_UserItemList[selectslot].Get_m_cType() == m_InchantStuff[0].Get_m_cType() )
	{
		if( g_pLocalUser->m_UserItemList[selectslot].Get_m_ucCount() >= m_InchantStuff[0].Get_m_ucCount() )
			return 1;
		else
			return -1;
	}
#ifdef _XDEF_EXPAND_INCHANT_TABLE_080425_KUKURI
	else if(g_pLocalUser->m_UserItemList[selectslot].Get_m_sID() == m_InchantStuff[2].Get_m_sID() &&
		g_pLocalUser->m_UserItemList[selectslot].Get_m_cType() == m_InchantStuff[2].Get_m_cType() )
	{
		if( g_pLocalUser->m_UserItemList[selectslot].Get_m_ucCount() >= m_InchantStuff[2].Get_m_ucCount() )
			return 1;
		else
			return -1;
	}
#endif
	else
	{
		return -2;
	}
	return 0;
}

int _XWindow_Inchant::CheckWeaponGrade(int searchslot)
{
	int grade = _XGameItem::GetItemGrade(g_pLocalUser->m_UserItemList[searchslot].Get_m_cType(), g_pLocalUser->m_UserItemList[searchslot].Get_m_sID());
	switch(grade) 
	{
	case 0:
	case 10:
	case 20:
	case 1 :
	case 2 :
	case 5 :
	case 50 :	// Weapon3 기보급 강무기
	case 51 :	// Weapon3 기보급 강무기
	case 30 :	// 무기 조합으로 추가된 무기
		{
			// 미국을 제외 한 다른 나라는 모두 제련 12단계
			// 미국은 무기 업데이트 시 제련 12단계로 수정할 예정
			int inchantgrade = 11;

/*#ifdef _XENGLISH //미국도 12 제련
			inchantgrade = 8;	// 제련 9단계까지
#endif*/

			if( g_pLocalUser->m_UserItemList[searchslot].Get_m_ucStrength() > inchantgrade )	
			{
				MessagePrint(2);
				return -2;
			}
			else
				return 1;
		}
		break;
	case 3 :
	case 4 :
	case 6 :
		{
			MessagePrint(1);
			return -1;
		}
		break;
	}
	return -1;
}

BOOL _XWindow_Inchant::ProcessInchantCashItem(int slotindex)
{
#ifdef _XDEF_PERIODITEM_20061027
	if(CheckCashItem(g_pLocalUser->m_UserItemList[slotindex].Get_m_cType(), g_pLocalUser->m_UserItemList[slotindex].Get_m_sID()) == 2 || 
		CheckCashItem(g_pLocalUser->m_UserItemList[slotindex].Get_m_cType(), g_pLocalUser->m_UserItemList[slotindex].Get_m_sID()) == 3)
	{
		// 기간제 아이템일 경우 Message box 보여주기
		g_MessageBox.SetMessage(_XDEF_DLGBTNTYPE_YESNO, _T(""), _XGETINTERFACETEXT(ID_STRING_NEW_3487), TRUE, _XDEF_INCHANT_PERIODITEM_YES, _XDEF_INCHANT_PERIODITEM_NO);	// 기간제 아이템은 제련에 성공해도 기간이 소진되며 제련 실패를 할 경우 잔여 기간에 관계없이 아이템이 사라집니다. 계속하시겠습니까?

		g_pInventory_Window->m_UsedLifeItemIndex = slotindex;
		g_pInventory_Window->m_bHaveMouseSlot = FALSE;
		g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();

		return TRUE;
	}
	else
	{
		g_pInventory_Window->m_UsedLifeItemIndex = slotindex;
		g_pInventory_Window->m_bHaveMouseSlot = FALSE;
		g_pInventory_Window->m_MouseSlotItem.ResetMouseSlotItem();
		return FALSE;
	}
#else
	return TRUE;
#endif
}

BOOL _XWindow_Inchant::ProcessInchant()
{
	if( CheckWeaponGrade(g_pInventory_Window->m_UsedLifeItemIndex) == 1 )
	{
		m_InchantItem = &g_pLocalUser->m_UserItemList[g_pInventory_Window->m_UsedLifeItemIndex];
		SetItemInchantStep(US_READY);
		return TRUE;
	}
	else
		return FALSE;
}