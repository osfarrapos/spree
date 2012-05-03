// XWindow_AddSaveItem.cpp: implementation of the _XWindow_AddSaveItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
//#include "XKernel.h"
//#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_Warehouse.h" // 2004.06.02->oneway48 insert
#include "XWindow_AddSaveItem.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_AddSaveItem::_XWindow_AddSaveItem()
{
	m_btnSeleted = NULL;
	m_btnCancel = NULL;

	
	for(int i = 0 ; i < 9 ; i++)
	{
		m_checkbtnList[i] = NULL;

        if( i < 6 ) //Author : 양희왕 //창고때문에 추가
            m_pUseImageBoder[i] = NULL;
	}
	
	m_SelectedItemIndex = -1;
	m_AddStorageSize	= 0;
	m_AddStorageTerm	= 0;
	m_AddStorageCost	= 0;
	m_nSelectTab		= 0;
}

_XWindow_AddSaveItem::~_XWindow_AddSaveItem()
{

}

BOOL _XWindow_AddSaveItem::Initialize( void )
{	
	SetDrawBorder( FALSE );
	// Create GUI Object =================================================================

	// Border---------------------------------------------------------------------------------------------=
	int addsaveitemimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Main_Back01.tga" );

	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create(0, 0, 184, 70, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
	pTitleBar->SetClipRect( 0, 125, 183, 194 );
	InsertChildObject(pTitleBar);

	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create( 0, 69, 184, 269, &g_MainInterfaceTextureArchive, 
						   g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" ));
	pBottomBorder->SetClipRect( 0, 10,  183, 211 );
	InsertChildObject( pBottomBorder );	
	// ---------------------------------------------------------------------------------------------------=
	
	// Line ----------------------------------------------------------------------------------------------=
	addsaveitemimageindex = g_MainInterfaceTextureArchive.FindResource( "mi_npcquestwin_01.tga" );

	_XImageStatic* pTopWindowLine = new _XImageStatic;
	pTopWindowLine->Create( 5, 66, 178, 68, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
	pTopWindowLine->SetClipRect( 21, 490, 195, 493 );
	InsertChildObject( pTopWindowLine );

	_XImageStatic* pLeftWindowLine = new _XImageStatic;
	pLeftWindowLine->Create( 5, 69, 7, 234, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
	pLeftWindowLine->SetClipRect( 495, 283, 498, 449 );
	InsertChildObject( pLeftWindowLine );


	_XImageStatic* pRightWindowLine = new _XImageStatic;
	pRightWindowLine->Create( 176, 69, 178, 234, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
	pRightWindowLine->SetClipRect( 501, 283, 504, 449 );
	InsertChildObject( pRightWindowLine );
	
	_XImageStatic* pBottomWindowLine = new _XImageStatic;
	pBottomWindowLine->Create( 5, 235, 178, 237, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
	pBottomWindowLine->SetClipRect( 21, 500, 195, 503 );
	InsertChildObject( pBottomWindowLine );	
	// ---------------------------------------------------------------------------------------------------=

	// Image ----------------------------------------------------------------------------------=
#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : 양희왕 //breif : 창고 확장 .. 어쩔까.. 하드코딩..
    if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
        	
	    for(int i = 0 ; i < 6 ; i++)
	    {
		    m_pUseImageBoder[i] = new _XImageStatic;
	    }

		m_pUseImageBoder[0]->Create( 11, 72, 23, 121, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		m_pUseImageBoder[0]->SetClipRect( 451, 284, 463, 334 );	
		InsertChildObject( m_pUseImageBoder[0] );
		m_pUseImageBoder[1]->Create( 23, 72, 36, 121, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		m_pUseImageBoder[1]->SetClipRect( 472, 284, 485, 334 );	
		InsertChildObject( m_pUseImageBoder[1] );
		
		m_pUseImageBoder[2]->Create( 11, 127, 23, 176, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		m_pUseImageBoder[2]->SetClipRect( 451, 284, 463, 334 );	
		InsertChildObject( m_pUseImageBoder[2]);
		m_pUseImageBoder[3]->Create( 23, 127, 36, 176, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		m_pUseImageBoder[3]->SetClipRect( 472, 284, 485, 334 );	
		InsertChildObject( m_pUseImageBoder[3]);
		
		m_pUseImageBoder[4]->Create( 11, 182, 23, 231, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		m_pUseImageBoder[4]->SetClipRect( 451, 284, 463, 334 );	
		InsertChildObject( m_pUseImageBoder[4]);
		m_pUseImageBoder[5]->Create( 23, 182, 36, 231, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		m_pUseImageBoder[5]->SetClipRect( 472, 284, 485, 334 );	
		InsertChildObject( m_pUseImageBoder[5] );
	}
	else
	{
        for(int i = 0 ; i < 3 ; i++)
	    {
		    m_pUseImageBoder[i] = new _XImageStatic;
	    }
		
		m_pUseImageBoder[0]->Create( 11, 72, 44, 121, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		m_pUseImageBoder[0]->SetClipRect( 451, 284, 485, 334 );	
		InsertChildObject( m_pUseImageBoder[0] );
		
		m_pUseImageBoder[1]->Create( 11, 127, 44, 176, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		m_pUseImageBoder[1]->SetClipRect( 451, 284, 485, 334 );	
		InsertChildObject( m_pUseImageBoder[1] );
		
		m_pUseImageBoder[2]->Create( 11, 182, 44, 231, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		m_pUseImageBoder[2]->SetClipRect( 451, 284, 485, 334 );	
		InsertChildObject( m_pUseImageBoder[2] );
	}	
#else
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		_XImageStatic* pUseImageBoder1Left = new _XImageStatic;
		pUseImageBoder1Left->Create( 11, 72, 23, 121, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		pUseImageBoder1Left->SetClipRect( 451, 284, 463, 334 );	
		InsertChildObject( pUseImageBoder1Left );
		_XImageStatic* pUseImageBoder1Right = new _XImageStatic;
		pUseImageBoder1Right->Create( 23, 72, 36, 121, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		pUseImageBoder1Right->SetClipRect( 472, 284, 485, 334 );	
		InsertChildObject( pUseImageBoder1Right );
		
		_XImageStatic* pUseImageBoder2Left = new _XImageStatic;
		pUseImageBoder2Left->Create( 11, 127, 23, 176, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		pUseImageBoder2Left->SetClipRect( 451, 284, 463, 334 );	
		InsertChildObject( pUseImageBoder2Left );
		_XImageStatic* pUseImageBoder2Right = new _XImageStatic;
		pUseImageBoder2Right->Create( 23, 127, 36, 176, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		pUseImageBoder2Right->SetClipRect( 472, 284, 485, 334 );	
		InsertChildObject( pUseImageBoder2Right );
		
		_XImageStatic* pUseImageBoder3Left = new _XImageStatic;
		pUseImageBoder3Left->Create( 11, 182, 23, 231, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		pUseImageBoder3Left->SetClipRect( 451, 284, 463, 334 );	
		InsertChildObject( pUseImageBoder3Left );
		_XImageStatic* pUseImageBoder3Right = new _XImageStatic;
		pUseImageBoder3Right->Create( 23, 182, 36, 231, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		pUseImageBoder3Right->SetClipRect( 472, 284, 485, 334 );	
		InsertChildObject( pUseImageBoder3Right );
	}
	else
	{
		_XImageStatic* pUseImageBoder1 = new _XImageStatic;
		pUseImageBoder1->Create( 11, 72, 44, 121, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		pUseImageBoder1->SetClipRect( 451, 284, 485, 334 );	
		InsertChildObject( pUseImageBoder1 );
		
		_XImageStatic* pUseImageBoder2 = new _XImageStatic;
		pUseImageBoder2->Create( 11, 127, 44, 176, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		pUseImageBoder2->SetClipRect( 451, 284, 485, 334 );	
		InsertChildObject( pUseImageBoder2 );
		
		_XImageStatic* pUseImageBoder3 = new _XImageStatic;
		pUseImageBoder3->Create( 11, 182, 44, 231, &g_MainInterfaceTextureArchive, addsaveitemimageindex );
		pUseImageBoder3->SetClipRect( 451, 284, 485, 334 );	
		InsertChildObject( pUseImageBoder3 );
	}	
#endif
	// ----------------------------------------------------------------------------------------=
	
#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : 양희왕 //breif : 창고 확장
	SetProcess( &_XWindow_AddSaveItem::OnFirstTabProcess );
	SetDraw( &_XWindow_AddSaveItem::OnFirstDraw );
	
	int nTabButtonCheckImage = g_MainInterfaceTextureArchive.FindResource( "mi_npcname.tga" );
	int nTabButtonNormalImage = g_MainInterfaceTextureArchive.FindResource( "mi_npcname2.tga" );
	
	_XCHECKBTN_STRUCTURE tabbtnstruct = { TRUE, 
		                                  { 7, 25 }, { 39, 39 },
										  _XDEF_ADDSAVEITEMWINDOW_POCKET1,
										  nTabButtonNormalImage,
										  nTabButtonCheckImage,
										  &g_MainInterfaceTextureArchive };

	for( int j = 0; j < 2; j++ )
	{
		m_pTabButton[j] = new _XCheckButton;
		m_pTabButton[j]->Create( tabbtnstruct );		
		m_pTabButton[j]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 0, 154,  40, 194 );
		m_pTabButton[j]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  196, 4,  236, 44 );
		m_pTabButton[j]->SetButtonSwapMode(TRUE);
		m_pTabButton[j]->ShowWindow(TRUE);
		if( j == 0 )
			m_pTabButton[j]->SetCheck( TRUE );
		else
			m_pTabButton[j]->SetCheck( FALSE );

		InsertChildObject( m_pTabButton[j] );

		tabbtnstruct.position.x += 43;
		tabbtnstruct.commandid++;
	}

	if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
	{
		m_pTabButton[1]->ShowWindow(FALSE);		// US:창고확장은 하되 신청은 안되도록.
	}

#else
	// Icon Button ----------------------------------------------------------------------------=
	_XImageStatic* pWareHouseUseIcon = new _XImageStatic;
	// 윈도우의 좌표를 더하지 않는다.
	pWareHouseUseIcon->Create( 7, 25, 46, 64, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "mi_npcname.tga" ) );
	pWareHouseUseIcon->SetClipRect( 196, 4, 236, 44 );
	InsertChildObject( pWareHouseUseIcon );
#endif
	// ----------------------------------------------------------------------------------------=

	// close, help Button ---------------------------------------------------------------------=	
	addsaveitemimageindex = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
		
	_XBTN_STRUCTURE helpstruct = { TRUE, { 166, 2 }, { 12, 12 }, _XDEF_ADDSAVEITEMWINDOW_HELP,
		addsaveitemimageindex,addsaveitemimageindex,addsaveitemimageindex, 
		&g_MainInterfaceTextureArchive };
	_XButton* pHelpbutton = new _XButton;
	pHelpbutton->Create( helpstruct );	
	
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 240, 88, 252, 100 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  240, 100, 252, 112 );
	pHelpbutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  240, 112, 252, 124 );
	InsertChildObject( pHelpbutton );
	// ----------------------------------------------------------------------------------------=

	// 사용하기, 취소 Button ------------------------------------------------------------------=
	addsaveitemimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );
	_XBTN_STRUCTURE Usebtnstruct = { TRUE, { 32, 239 }, { 61, 25 }, _XDEF_ADDSAVEITEMWINDOW_OK,
		addsaveitemimageindex,addsaveitemimageindex,addsaveitemimageindex, 
		&g_MainInterfaceTextureArchive };
	m_btnSeleted = new _XButton;
	m_btnSeleted->Create( Usebtnstruct );	
	
	m_btnSeleted->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
	m_btnSeleted->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  187, 152, 247, 176 );	
	m_btnSeleted->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	m_btnSeleted->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ADDSAVEITEM_USING );	// 창고열기
	InsertChildObject( m_btnSeleted );

	_XBTN_STRUCTURE Canclebtnstruct = { TRUE, { 95, 239 }, { 61,  25 }, _XDEF_ADDSAVEITEMWINDOW_CLOSE,
		addsaveitemimageindex,addsaveitemimageindex,addsaveitemimageindex, 
		&g_MainInterfaceTextureArchive };
	_XButton* pCancleutton = new _XButton;
	pCancleutton->Create( Canclebtnstruct );	
	
	pCancleutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 187, 127, 247, 151 );
	pCancleutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  187, 152, 247, 176 );	
	pCancleutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  187, 177, 247, 201 );
	pCancleutton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_CANCLE );	// _T("취소")
	InsertChildObject( pCancleutton );
	// ----------------------------------------------------------------------------------------=	
	// Check Button ---------------------------------------------------------------------------=		
	_XCHECKBTN_STRUCTURE checkbtnstruct = { TRUE, { 156, 74}, { 15, 15 }, 
		_XDEF_ADDSAVEITEMWINDOW_CHECKBTN1,
		addsaveitemimageindex,addsaveitemimageindex,
		&g_MainInterfaceTextureArchive };

	for(int i = 0 ; i < 3 ; i++)
	{
		m_checkbtnList[i] = new _XCheckButton;
		m_checkbtnList[i]->Create(checkbtnstruct);
		m_checkbtnList[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216, 31, 230, 45 );
		m_checkbtnList[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
		InsertChildObject(m_checkbtnList[i]);
		
		checkbtnstruct.commandid++;
		checkbtnstruct.position.y += 16;
	}

	checkbtnstruct.position.y = 129;
	for( i = 3 ; i < 6 ; i++)
	{
		m_checkbtnList[i] = new _XCheckButton;
		m_checkbtnList[i]->Create(checkbtnstruct);
		m_checkbtnList[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216, 31, 230, 45 );
		m_checkbtnList[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
		InsertChildObject(m_checkbtnList[i]);
		
		checkbtnstruct.commandid++;
		checkbtnstruct.position.y += 16;
	}
	
	checkbtnstruct.position.y = 184;
	for( i = 6 ; i < 9 ; i++)
	{
		m_checkbtnList[i] = new _XCheckButton;
		m_checkbtnList[i]->Create(checkbtnstruct);
		m_checkbtnList[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216, 31, 230, 45 );
		m_checkbtnList[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
		InsertChildObject(m_checkbtnList[i]);
		
		checkbtnstruct.commandid++;
		checkbtnstruct.position.y += 16;
	}
	// ----------------------------------------------------------------------------------------=	
	return TRUE;
}

void _XWindow_AddSaveItem::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}


void _XWindow_AddSaveItem::Draw( _XGUIObject*& pfocusobject )
{
	
	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	_XWindow::Draw(pfocusobject);

#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : 양희왕 //breif : 창고 확장 
	(this->*(_XWindow_AddSaveItem::m_CurrentDraw))(); //요런 모양인건.. 함수를 static 으로 선언안하기 위해
#else
	// Draw Box ------------------------------------------------------------------------------------------------------------------------------------=
	_XDrawRectAngle( m_WindowPosition.x+10, m_WindowPosition.y+71, m_WindowPosition.x+173, m_WindowPosition.y+122, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+10, m_WindowPosition.y+126, m_WindowPosition.x+173, m_WindowPosition.y+177, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+10, m_WindowPosition.y+181, m_WindowPosition.x+173, m_WindowPosition.y+232, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	// ---------------------------------------------------------------------------------------------------------------------------------------------=

	// Draw Text --------------------------------------------------------------------------------------------------------=		
	g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		g_XBaseFont->PutsAlign( m_WindowPosition.x+22, m_WindowPosition.y +84, _XFONT_ALIGNTYPE_CENTER,  _T("20") );
		g_XBaseFont->PutsAlign( m_WindowPosition.x+22, m_WindowPosition.y +100 , _XFONT_ALIGNTYPE_CENTER,  _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_20)+2 );		
		g_XBaseFont->PutsAlign( m_WindowPosition.x+22, m_WindowPosition.y +139 , _XFONT_ALIGNTYPE_CENTER,  _T("30") );		
		g_XBaseFont->PutsAlign( m_WindowPosition.x+22, m_WindowPosition.y +155 , _XFONT_ALIGNTYPE_CENTER,  _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_30)+2 );		
		g_XBaseFont->PutsAlign( m_WindowPosition.x+22, m_WindowPosition.y +194 , _XFONT_ALIGNTYPE_CENTER,  _T("40") );
		g_XBaseFont->PutsAlign( m_WindowPosition.x+22, m_WindowPosition.y +210 , _XFONT_ALIGNTYPE_CENTER,  _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_40)+2 );		
	}
	else
	{
		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y +84 , _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_20) );	// _T("20칸")
		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y +100 , _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_USE) );	// _T("사용")
		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y +139 , _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_30) );	// _T("30칸")
		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y +155 , _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_USE) );	// _T("사용")
		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y +194 , _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_40) );	// _T("40칸")
		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y +210 , _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_USE) );	// _T("사용")
	}

	_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
	
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	int drawpositionx = 47;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		drawpositionx = 39;

	if( !pWarehouse_Window->m_bTerminateWarehouse ) // 만료기간이 아닐 때 
	{		
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +76, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE1) );	// _T("120일 사용: 2800")
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +92, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE2) );	//_T("240일 사용: 5200") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +108, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE3) );	//_T("365일 사용: 7200") 
		
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +131, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE4) );	//_T("120일 사용: 4300") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +147, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE5) );	//_T("240일 사용: 7900") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +163, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE6) );	//_T("365일 사용:10800") 
		
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +186, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE7) );	//_T("120일 사용: 5700") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +202, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE8) );	//_T("240일 사용:10500") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +218, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE9) );	//_T("365일 사용:14400") 
		
	}
	else // 만료기간일 때 
	{
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +76, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE10) );	//_T("120일 사용 : 3800")
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +92, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE11) );	//_T("240일 사용 : 6200") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +108, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE12) );	//_T("365일 사용 : 8200") 
		
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +131, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE13) );	//_T("120일 사용 : 5800") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +147, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE14) );	//_T("240일 사용 : 9400") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +163, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE15) );	//_T("365일 사용 :12300") 
		
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +186, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE16) );	//_T("120일 사용 : 7700")
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +202, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE17) );	//_T("240일 사용 :12500") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +218, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE18) );	//_T("365일 사용 :16400") 
	}
	g_XBaseFont->Flush();
	// ------------------------------------------------------------------------------------------------------------------=	
#endif
}

BOOL _XWindow_AddSaveItem::Process( _XGUIObject*& pfocusobject )
{	
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	if( !_XWindow::Process(pfocusobject) ) return FALSE;

#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : 양희왕 //breif : 창고 확장 너무 보기어렵게 한거같다..
	(this->*(_XWindow_AddSaveItem::m_CurrnetProcess))(); //요런 모양인건.. 함수를 static 으로 선언안하기 위해
#endif 
	
	/*if( gpInput->GetMouseState()->bButton[0] )
	{
		int selecteditem = GetListIndex(); 
	
		if( selecteditem != -1 )
		{
			int commandid = _XDEF_ADDSAVEITEMWINDOW_CHECKBTN1;// 2004.05.24->oneway48 insert
			for(int i = 0 ; i < 9 ; i++)
			{
				if(selecteditem > -1)
				{
					if(selecteditem == i)
					{
						m_checkbtnList[i]->SetCheck(TRUE);
						m_SelectedItemIndex = selecteditem;
						PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(commandid+i, 0), (LPARAM)gHWnd);// 2004.05.24->oneway48 insert
					}
					else
					{
						m_checkbtnList[i]->SetCheck(FALSE);
					}
				}
				else
				{
					m_checkbtnList[i]->SetCheck(FALSE);
				}
			}
		}
	}*/

	
	if( gpInput->GetMouseState()->bButton[1] )
	{		
		// 창고 추가 시 마우스 오른쪽 버튼은 선택 취소 
		ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
		if( ((scrnpos->x > m_WindowPosition.x+5 )   && 
			(scrnpos->x < m_WindowPosition.x+m_WindowSize.cx-5) &&
			(scrnpos->z > m_WindowPosition.y+68 )   && 
			(scrnpos->z < m_WindowPosition.y+m_WindowSize.cy-33) ) )
		{
			Reset();
		}
	}

	return TRUE;
}	

void _XWindow_AddSaveItem::OnFirstTabProcess()
{
}

void _XWindow_AddSaveItem::OnSecondTabProcess()
{
}

void _XWindow_AddSaveItem::OnFirstDraw()
{
	// Draw Box ------------------------------------------------------------------------------------------------------------------------------------=
	_XDrawRectAngle( m_WindowPosition.x+10, m_WindowPosition.y+71, m_WindowPosition.x+173, m_WindowPosition.y+122, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+10, m_WindowPosition.y+126, m_WindowPosition.x+173, m_WindowPosition.y+177, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+10, m_WindowPosition.y+181, m_WindowPosition.x+173, m_WindowPosition.y+232, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	// ---------------------------------------------------------------------------------------------------------------------------------------------=

	// Draw Text --------------------------------------------------------------------------------------------------------=		
	g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		g_XBaseFont->PutsAlign( m_WindowPosition.x+22, m_WindowPosition.y +84, _XFONT_ALIGNTYPE_CENTER,  _T("20") );
		g_XBaseFont->PutsAlign( m_WindowPosition.x+22, m_WindowPosition.y +100 , _XFONT_ALIGNTYPE_CENTER,  _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_20)+2 );		
		g_XBaseFont->PutsAlign( m_WindowPosition.x+22, m_WindowPosition.y +139 , _XFONT_ALIGNTYPE_CENTER,  _T("30") );		
		g_XBaseFont->PutsAlign( m_WindowPosition.x+22, m_WindowPosition.y +155 , _XFONT_ALIGNTYPE_CENTER,  _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_30)+2 );		
		g_XBaseFont->PutsAlign( m_WindowPosition.x+22, m_WindowPosition.y +194 , _XFONT_ALIGNTYPE_CENTER,  _T("40") );
		g_XBaseFont->PutsAlign( m_WindowPosition.x+22, m_WindowPosition.y +210 , _XFONT_ALIGNTYPE_CENTER,  _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_40)+2 );		
	}
	else
	{
		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y +84 , _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_20) );	// _T("20칸")
		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y +100 , _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_USE) );	// _T("사용")
		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y +139 , _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_30) );	// _T("30칸")
		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y +155 , _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_USE) );	// _T("사용")
		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y +194 , _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_40) );	// _T("40칸")
		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y +210 , _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_USE) );	// _T("사용")
	}

	_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
	
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	int drawpositionx = 47;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		drawpositionx = 39;

	if( !pWarehouse_Window->m_bTerminateWarehouse ) // 만료기간이 아닐 때 
	{		
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +76, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE1) );	// _T("120일 사용: 2800")
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +92, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE2) );	//_T("240일 사용: 5200") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +108, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE3) );	//_T("365일 사용: 7200") 
		
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +131, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE4) );	//_T("120일 사용: 4300") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +147, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE5) );	//_T("240일 사용: 7900") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +163, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE6) );	//_T("365일 사용:10800") 
		
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +186, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE7) );	//_T("120일 사용: 5700") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +202, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE8) );	//_T("240일 사용:10500") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +218, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE9) );	//_T("365일 사용:14400") 
		
	}
	else // 만료기간일 때 
	{
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +76, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE10) );	//_T("120일 사용 : 3800")
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +92, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE11) );	//_T("240일 사용 : 6200") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +108, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE12) );	//_T("365일 사용 : 8200") 
		
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +131, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE13) );	//_T("120일 사용 : 5800") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +147, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE14) );	//_T("240일 사용 : 9400") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +163, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE15) );	//_T("365일 사용 :12300") 
		
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +186, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE16) );	//_T("120일 사용 : 7700")
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +202, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE17) );	//_T("240일 사용 :12500") 
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +218, _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_TYPE18) );	//_T("365일 사용 :16400") 
	}

	g_XBaseFont->Flush();
}

void _XWindow_AddSaveItem::OnSecondDraw()
{
	// Draw Box ------------------------------------------------------------------------------------------------------------------------------------=
	_XDrawRectAngle( m_WindowPosition.x+10, m_WindowPosition.y+71, m_WindowPosition.x+173, m_WindowPosition.y+122, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+10, m_WindowPosition.y+126, m_WindowPosition.x+173, m_WindowPosition.y+177, 0.0f, D3DCOLOR_ARGB(255,0,0,0) );
	// ---------------------------------------------------------------------------------------------------------------------------------------------=

	// Draw Text --------------------------------------------------------------------------------------------------------=		
	g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
	}
	else
	{
		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y +84 , _T("50칸") );	// 
		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y +100 , _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_USE) );	// _T("사용")
		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y +139 , _T("60칸") );	//
		g_XBaseFont->Puts( m_WindowPosition.x + 16, m_WindowPosition.y +155 , _XGETINTERFACETEXT(ID_STRING_ADDSAVEITEM_USE) );	// _T("사용")
	}

	_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
	
	g_XBaseFont->SetColor(_XSC_DEFAULT);
	int drawpositionx = 47;
	if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
		drawpositionx = 39;

	//if( !pWarehouse_Window->m_bTerminateWarehouse ) // 만료기간이 아닐 때  
	{		
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +76, _T("4달(120일) 6500금"));
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +92, _T("8달(240일) 11900금"));
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +108, _T("1년(365일) 16300금"));
		
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +131, _T("4달(120일) 7800금"));
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +147, _T("8달(240일) 14300금"));
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +163, _T("1년(365일) 19600금"));
		
	}
/*	else // 만료기간일 때 //이건 재계약이란 뜻??
	{
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +76, _T("4달(120일) 6500금"));
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +92, _T("8달(240일) 11900금"));
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +108, _T("1년(365일) 16300금"));
		
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +131, _T("4달(120일) 7800금"));
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +147, _T("8달(240일) 14300금"));
		g_XBaseFont->Puts( m_WindowPosition.x + drawpositionx, m_WindowPosition.y +163, _T("1년(365일) 19600금"));
	}*/

	g_XBaseFont->Flush();
}

void _XWindow_AddSaveItem::SetSelectedIndex( int index )
{
#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : 양희왕 //breif : 창고 확장
	for( int i = 0; i < 9; i++ ) // 체크버튼 프로세스를 돌고 난뒤 실행됨..
	{
		if( i != index) //선택된 인덱스 이외에 체크는 삭제한다.
			m_checkbtnList[i]->SetCheck(FALSE);
	}

	if( m_checkbtnList[index]->GetCheck() ) //현재 인덱스가 체크 되있다면
		m_SelectedItemIndex = index + (m_nSelectTab*9); //첫번째 탭인지 두번째 탭인지
	else
	{
		m_SelectedItemIndex = -1;
		m_AddStorageCost = 0;
	}
#else
	m_SelectedItemIndex = index; //0 을 기준

	for( int i = 0; i < 9; i++ )
	{
		m_checkbtnList[i]->SetCheck(i == m_SelectedItemIndex);
	}
#endif
}

int _XWindow_AddSaveItem::GetListIndex()
{
	int selectedlistnumber	= -1;
	ScrnPos* scrnpos	= gpInput->GetMouseVirScreenPos();
	int rowpoint		= m_WindowPosition.y + 52	;

	if( selectedlistnumber == -1 )
	{
		
		for( int row = 0; row < 3 ; row++ )
		{
			if( ((scrnpos->x > m_WindowPosition.x + 11  ) && (scrnpos->x < m_WindowPosition.x + 194) && 
				(scrnpos->z > rowpoint )    && (scrnpos->z < rowpoint+14) ) )
			{
				selectedlistnumber = row;
				_XDrawSolidBar( m_WindowPosition.x + 13, rowpoint, m_WindowPosition.x + 193, rowpoint+14, D3DCOLOR_ARGB(80,255,255,255) );
				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_SELECT );
			}
			rowpoint += 14;
			if( selectedlistnumber != -1 ) return selectedlistnumber;
		}

		rowpoint = m_WindowPosition.y + 137;
		for( row = 3; row < 6 ; row++ )
		{
			if( ((scrnpos->x > m_WindowPosition.x + 11  ) && (scrnpos->x < m_WindowPosition.x + 194) && 
				(scrnpos->z > rowpoint )    && (scrnpos->z < rowpoint+14) ) )
			{
				selectedlistnumber = row;
				_XDrawSolidBar( m_WindowPosition.x + 13, rowpoint, m_WindowPosition.x + 193, rowpoint+14, D3DCOLOR_ARGB(80,255,255,255) );
				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_SELECT );
			}
			rowpoint += 14;
			if( selectedlistnumber != -1 ) return selectedlistnumber;
		}

		rowpoint = m_WindowPosition.y + 222;
		for( row = 6; row < 9 ; row++ )
		{
			if( ((scrnpos->x > m_WindowPosition.x + 11  ) && (scrnpos->x < m_WindowPosition.x + 194) && 
				(scrnpos->z > rowpoint )    && (scrnpos->z < rowpoint+14) ) )
			{
				selectedlistnumber = row;
				_XDrawSolidBar( m_WindowPosition.x + 13, rowpoint, m_WindowPosition.x + 193, rowpoint+14, D3DCOLOR_ARGB(80,255,255,255) );
				((CEmperorOfDragonsApp*)gApp)->ReadyCursor( _XMOUSETYPE_SELECT );
			}
			rowpoint += 14;
			if( selectedlistnumber != -1 ) return selectedlistnumber;
		}
	}
	
	return -1;
	
}

void _XWindow_AddSaveItem::Reset()
{
	for(int i = 0 ; i < 9 ; i++)
	{				
		m_checkbtnList[i]->SetCheck(FALSE);		
	}
	
	m_SelectedItemIndex = -1;
	m_AddStorageSize	= 0;
	m_AddStorageTerm	= 0;
	m_AddStorageCost	= 0;
}

void _XWindow_AddSaveItem::CheckUseSlot(void)
{
	for( int i = 0; i < 9; i++ )
		m_checkbtnList[i]->EnableWindow(FALSE);

	_XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);
	if( pWarehouse_Window )
	{
		if( pWarehouse_Window->m_TerminateLastItemIndex >= 20 )
		{
			if( pWarehouse_Window->m_TerminateLastItemIndex >= 30 )
			{
				for( int i = 6; i < 9; i++ )
					m_checkbtnList[i]->EnableWindow(TRUE);
			}
			else
			{
				for( int i = 3; i < 9; i++ )
					m_checkbtnList[i]->EnableWindow(TRUE);				
			}			
		}
		else
		{
			for( int i = 0; i < 9; i++ )
				m_checkbtnList[i]->EnableWindow(TRUE);
		}
	}		
}


void _XWindow_AddSaveItem::ShowWindow(BOOL show)
{
	_XWindow::ShowWindow(show);

	//첫번째 탭 으로 돌린다
	#ifdef _XDEF_WAREHOUSE_ADDITION_080402_MAGVIPER //Author : 양희왕 //breif : 창고 확장
	SetSelectTab(0);
	#endif

	if(show)
	{
		if(!g_pInventory_Window->m_bLockInventory)
			g_pInventory_Window->m_bLockInventory = TRUE;
	}
	else
	{
		Reset();

		if(g_pInventory_Window->m_bLockInventory)
			g_pInventory_Window->m_bLockInventory = FALSE;
	}
}

void _XWindow_AddSaveItem::SetSelectTab( const int nNumber )
{
    _XWindow_Warehouse* pWarehouse_Window = (_XWindow_Warehouse*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_WAREHOUSE);

	m_nSelectTab = nNumber; 
	for( int i = 0; i < 2; i++ )
	{
		if( i == nNumber ) //선택한 탭과 같은 건 열고 다른건 닫는다
			m_pTabButton[i]->SetCheck(TRUE);
		else
			m_pTabButton[i]->SetCheck(FALSE);
	}

	for( i = 0 ; i < 9 ; i++)
	{				
		m_checkbtnList[i]->SetCheck(FALSE);	
        m_checkbtnList[i]->EnableWindow(TRUE);
	}

	switch( m_nSelectTab )
	{
	case 0 :
		{
            if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
            {
                m_pUseImageBoder[4]->ShowWindow(TRUE);
                m_pUseImageBoder[5]->ShowWindow(TRUE);
            }
            else
            {
                m_pUseImageBoder[2]->ShowWindow(TRUE);
            }

            if( pWarehouse_Window->m_TerminateLastItemIndex >= 40 ) //마지막 아이템슬롯이 40 이상이라면 첫번째 탭 창고 추가는 모두 선택할수 없다.
            {
                for( i = 0 ; i < 9 ; i++)
	            {				
                    m_checkbtnList[i]->EnableWindow(FALSE);
	            }
            }
            else if( pWarehouse_Window->m_TerminateLastItemIndex >= 30 )
            {
                for( i = 0 ; i < 6 ; i++)
	            {				
                    m_checkbtnList[i]->EnableWindow(FALSE);
	            }
            }
            else if( pWarehouse_Window->m_TerminateLastItemIndex >= 20 )
            {
                for( i = 0 ; i < 3 ; i++)
	            {				
                    m_checkbtnList[i]->EnableWindow(FALSE);
	            }
            }

			if( m_SelectedItemIndex > -1 && m_SelectedItemIndex < 9) // 선택한 버튼이 있다..
			{
				m_checkbtnList[m_SelectedItemIndex]->SetCheck(TRUE);
			}

			for( int i = 6; i < 9; i++ )
			{
				m_checkbtnList[i]->ShowWindow(TRUE);
			}
			SetProcess( &_XWindow_AddSaveItem::OnFirstTabProcess );
			SetDraw( &_XWindow_AddSaveItem::OnFirstDraw );
		}
		break;
	case 1 :
		{
            if( g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE || g_LanguageType == _XLANGUAGE_TYPE_ENGLISH || g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
            {
                m_pUseImageBoder[4]->ShowWindow(FALSE);
                m_pUseImageBoder[5]->ShowWindow(FALSE);
            }
            else
            {
                m_pUseImageBoder[2]->ShowWindow(FALSE);
            }

            if( pWarehouse_Window->m_TerminateLastItemIndex >= 50 ) //마지막 아이템슬롯이 50 이상이라면
            {
                for( i = 0 ; i < 3 ; i++)
	            {				
                    m_checkbtnList[i]->EnableWindow(FALSE);
	            }
            }

			if( m_SelectedItemIndex > 8 )
			{
				int nIndex = m_SelectedItemIndex%9;
				m_checkbtnList[nIndex]->SetCheck(TRUE);
			}
			for( int i = 6; i < 9; i++ ) // 마지막 세개는 없음..
			{
				m_checkbtnList[i]->ShowWindow(FALSE);
			}
			SetProcess( &_XWindow_AddSaveItem::OnSecondTabProcess );
			SetDraw( &_XWindow_AddSaveItem::OnSecondDraw );
		}
		break;
	}
}