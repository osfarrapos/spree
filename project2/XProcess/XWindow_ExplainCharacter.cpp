// XWindow_ExplainCharacter.cpp: implementation of the _XWindow_ExplainCharacter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "XWindow_ExplainCharacter.h"
#include "XProc_ProcessDefine.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_ExplainCharacter::_XWindow_ExplainCharacter()
{
	for( int i = 0; i < 12; ++i )
	{
		m_btnIcon[i] = NULL;
	}
	

}

_XWindow_ExplainCharacter::~_XWindow_ExplainCharacter()
{

}


BOOL _XWindow_ExplainCharacter::Initialize(_XTextureManager& texturearchive)
{
	
	int createcharacterindex = texturearchive.FindResource("MI_CreateCharacter.tga");
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 타이틀바01 ( 155 X 15 )
	_XImageStatic* pTitleBar01 = new _XImageStatic;
	pTitleBar01->Create( 3, 65+30, 158, 80+30, &texturearchive, createcharacterindex );
	pTitleBar01->SetClipRect( 1, 238, 156, 253 );
	InsertChildObject( pTitleBar01 );
	
	// 왼쪽위모서리01 ( 24 X 22 )
	_XImageStatic* pLeftTopEdge01 = new _XImageStatic;
	pLeftTopEdge01->Create( 0, 79+30, 24, 101+30, &texturearchive, createcharacterindex );
	pLeftTopEdge01->SetClipRect( 200, 58, 224, 80 );
	InsertChildObject( pLeftTopEdge01 );
	
	// 오른쪽쪽위모서리01 ( 22 X 22 )
	_XImageStatic* pRightTopEdge01 = new _XImageStatic;
	pRightTopEdge01->Create( 249, 79+30, 271, 101+30, &texturearchive, createcharacterindex );
	pRightTopEdge01->SetClipRect( 232, 58, 254, 80 );
	InsertChildObject( pRightTopEdge01 );
	
	// 왼쪽아래모서리01 ( 24 X 28 )
	_XImageStatic* pLeftBottomEdge01 = new _XImageStatic;
	pLeftBottomEdge01->Create( 0, 172+30, 24, 200+30, &texturearchive, createcharacterindex );
	pLeftBottomEdge01->SetClipRect( 200, 86, 224, 114 );
	InsertChildObject( pLeftBottomEdge01 );
	
	// 오른쪽아래모서리01 ( 22 X 28 )
	_XImageStatic* pRightBottomEdge01 = new _XImageStatic;
	pRightBottomEdge01->Create( 249, 172+30, 271, 200+30, &texturearchive, createcharacterindex );
	pRightBottomEdge01->SetClipRect( 232, 86, 254, 114 );
	InsertChildObject( pRightBottomEdge01 );
	
	// 위쪽라인01 ( 225 X 10 )
	_XImageStatic* pTopLine01 = new _XImageStatic;
	pTopLine01->Create( 24, 79+30, 249, 89+30, &texturearchive, createcharacterindex );
	pTopLine01->SetClipRect( 14, 212, 24, 222 );
	pTopLine01->SetScale( 22.5f, 1.0f );
	InsertChildObject( pTopLine01 );
	
	// 아래쪽라인01 ( 225 X 11 )
	_XImageStatic* pBottomLine01 = new _XImageStatic;
	pBottomLine01->Create( 24, 190+30, 249, 201+30, &texturearchive, createcharacterindex );
	pBottomLine01->SetClipRect( 14, 225, 24, 236 );
	pBottomLine01->SetScale( 22.5f, 1.0f );
	InsertChildObject( pBottomLine01 );
	
	// 왼쪽라인01 ( 10 X 71 )
	_XImageStatic* pLeftLine01 = new _XImageStatic;
	pLeftLine01->Create( 0, 101+30, 10, 172+30, &texturearchive, createcharacterindex );
	pLeftLine01->SetClipRect( 229, 200, 239, 210 );
	pLeftLine01->SetScale( 1.0f, 7.1f );
	InsertChildObject( pLeftLine01);
	
	// 오른쪽라인01 ( 10 X 71 )
	_XImageStatic* pRightLine01 = new _XImageStatic;
	pRightLine01->Create( 261, 101+30, 283, 172+30, &texturearchive, createcharacterindex );
	pRightLine01->SetClipRect( 244, 200, 254, 210 );
	pRightLine01->SetScale( 1.0f, 7.1f );
	InsertChildObject( pRightLine01);
	
	// 아이콘 박스01 ( 50 X 49 )
	_XImageStatic* pIconBox01 = new _XImageStatic;
	pIconBox01->Create( 13, 79+30+60, 63, 101+30+60, &texturearchive, createcharacterindex );
	pIconBox01->SetClipRect( 202, 3, 252, 52 );
	InsertChildObject( pIconBox01);
	
	// 아이콘 박스02 ( 50 X 49 )
	_XImageStatic* pIconBox02 = new _XImageStatic;
	pIconBox02->Create( 13+64, 79+30+60, 63+64, 101+30+60, &texturearchive, createcharacterindex );
	pIconBox02->SetClipRect( 202, 3, 252, 52 );
	InsertChildObject( pIconBox02);
	
	// 아이콘 박스03 ( 50 X 49 )
	_XImageStatic* pIconBox03 = new _XImageStatic;
	pIconBox03->Create( 13+64+64, 79+30+60, 63+64+64, 101+30+60, &texturearchive, createcharacterindex );
	pIconBox03->SetClipRect( 202, 3, 252, 52 );
	InsertChildObject( pIconBox03);
	
	// 아이콘 박스04 ( 50 X 49 )
	_XImageStatic* pIconBox04 = new _XImageStatic;
	pIconBox04->Create( 13+64+64+64, 79+30+60, 63+64+64+64, 101+30+60, &texturearchive, createcharacterindex );
	pIconBox04->SetClipRect( 202, 3, 252, 52 );
	InsertChildObject( pIconBox04);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 타이틀바02 ( 155 X 15 )
	_XImageStatic* pTitleBar02 = new _XImageStatic;
	pTitleBar02->Create( 3, 65+30+133, 158, 80+30+133, &texturearchive, createcharacterindex );
	pTitleBar02->SetClipRect( 1, 238, 156, 253 );
	InsertChildObject( pTitleBar02 );
	
	// 왼쪽위모서리02 ( 24 X 22 )
	_XImageStatic* pLeftTopEdge02 = new _XImageStatic;
	pLeftTopEdge02->Create( 0, 79+30+133, 24, 101+30+133, &texturearchive, createcharacterindex );
	pLeftTopEdge02->SetClipRect( 200, 58, 224, 80 );
	InsertChildObject( pLeftTopEdge02 );
	
	// 오른쪽쪽위모서리02 ( 22 X 22 )
	_XImageStatic* pRightTopEdge02 = new _XImageStatic;
	pRightTopEdge02->Create( 249, 79+30+133, 271, 101+30+133, &texturearchive, createcharacterindex );
	pRightTopEdge02->SetClipRect( 232, 58, 254, 80 );
	InsertChildObject( pRightTopEdge02 );
	
	// 왼쪽아래모서리02 ( 24 X 28 )
	_XImageStatic* pLeftBottomEdge02 = new _XImageStatic;
	pLeftBottomEdge02->Create( 0, 172+30+133, 24, 200+30+133, &texturearchive, createcharacterindex );
	pLeftBottomEdge02->SetClipRect( 200, 86, 224, 114 );
	InsertChildObject( pLeftBottomEdge02 );
	
	// 오른쪽아래모서리02 ( 22 X 28 )
	_XImageStatic* pRightBottomEdge02 = new _XImageStatic;
	pRightBottomEdge02->Create( 249, 172+30+133, 271, 200+30+133, &texturearchive, createcharacterindex );
	pRightBottomEdge02->SetClipRect( 232, 86, 254, 114 );
	InsertChildObject( pRightBottomEdge02 );
	
	// 위쪽라인02 ( 225 X 10 )
	_XImageStatic* pTopLine02 = new _XImageStatic;
	pTopLine02->Create( 24, 79+30+133, 249, 89+30+133, &texturearchive, createcharacterindex );
	pTopLine02->SetClipRect( 14, 212, 24, 222 );
	pTopLine02->SetScale( 22.5f, 1.0f );
	InsertChildObject( pTopLine02 );
	
	// 아래쪽라인02 ( 225 X 11 )
	_XImageStatic* pBottomLine02 = new _XImageStatic;
	pBottomLine02->Create( 24, 190+30+133, 249, 201+30+133, &texturearchive, createcharacterindex );
	pBottomLine02->SetClipRect( 14, 225, 24, 236 );
	pBottomLine02->SetScale( 22.5f, 1.0f );
	InsertChildObject( pBottomLine02 );
	
	// 왼쪽라인02 ( 10 X 71 )
	_XImageStatic* pLeftLine02 = new _XImageStatic;
	pLeftLine02->Create( 0, 101+30+133, 10, 172+30+133, &texturearchive, createcharacterindex );
	pLeftLine02->SetClipRect( 229, 200, 239, 210 );
	pLeftLine02->SetScale( 1.0f, 7.1f );
	InsertChildObject( pLeftLine02);
	
	// 오른쪽라인02 ( 10 X 71 )
	_XImageStatic* pRightLine02 = new _XImageStatic;
	pRightLine02->Create( 261, 101+30+133, 283, 172+30+133, &texturearchive, createcharacterindex );
	pRightLine02->SetClipRect( 244, 200, 254, 210 );
	pRightLine02->SetScale( 1.0f, 7.1f );
	InsertChildObject( pRightLine02);
	
	// 아이콘 박스05 ( 50 X 49 )
	_XImageStatic* pIconBox05 = new _XImageStatic;
	pIconBox05->Create( 13, 79+30+60+133, 63, 101+30+60+133, &texturearchive, createcharacterindex );
	pIconBox05->SetClipRect( 202, 3, 252, 52 );
	InsertChildObject( pIconBox05);
	
	// 아이콘 박스06 ( 50 X 49 )
	_XImageStatic* pIconBox06 = new _XImageStatic;
	pIconBox06->Create( 13+64, 79+30+60+133, 63+64, 101+30+60+133, &texturearchive, createcharacterindex );
	pIconBox06->SetClipRect( 202, 3, 252, 52 );
	InsertChildObject( pIconBox06);
	
	// 아이콘 박스07 ( 50 X 49 )
	_XImageStatic* pIconBox07 = new _XImageStatic;
	pIconBox07->Create( 13+64+64, 79+30+60+133, 63+64+64, 101+30+60+133, &texturearchive, createcharacterindex );
	pIconBox07->SetClipRect( 202, 3, 252, 52 );
	InsertChildObject( pIconBox07);
	
	// 아이콘 박스08 ( 50 X 49 )
	_XImageStatic* pIconBox08 = new _XImageStatic;
	pIconBox08->Create( 13+64+64+64, 79+30+60+133, 63+64+64+64, 101+30+60+133, &texturearchive, createcharacterindex );
	pIconBox08->SetClipRect( 202, 3, 252, 52 );
	InsertChildObject( pIconBox08);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 타이틀바03 ( 155 X 15 )
	_XImageStatic* pTitleBar03 = new _XImageStatic;
	pTitleBar03->Create( 3, 65+30+266, 158, 80+30+266, &texturearchive, createcharacterindex );
	pTitleBar03->SetClipRect( 1, 238, 156, 253 );
	InsertChildObject( pTitleBar03 );
	
	// 왼쪽위모서리03 ( 24 X 22 )
	_XImageStatic* pLeftTopEdge03 = new _XImageStatic;
	pLeftTopEdge03->Create( 0, 79+30+266, 24, 101+30+266, &texturearchive, createcharacterindex );
	pLeftTopEdge03->SetClipRect( 200, 58, 224, 80 );
	InsertChildObject( pLeftTopEdge03 );
	
	// 오른쪽쪽위모서리03 ( 22 X 22 )
	_XImageStatic* pRightTopEdge03 = new _XImageStatic;
	pRightTopEdge03->Create( 249, 79+30+266, 271, 101+30+266, &texturearchive, createcharacterindex );
	pRightTopEdge03->SetClipRect( 232, 58, 254, 80 );
	InsertChildObject( pRightTopEdge03 );
	
	// 왼쪽아래모서리03 ( 24 X 28 )
	_XImageStatic* pLeftBottomEdge03 = new _XImageStatic;
	pLeftBottomEdge03->Create( 0, 172+30+266, 24, 200+30+266, &texturearchive, createcharacterindex );
	pLeftBottomEdge03->SetClipRect( 200, 86, 224, 114 );
	InsertChildObject( pLeftBottomEdge03 );
	
	// 오른쪽아래모서리03 ( 22 X 28 )
	_XImageStatic* pRightBottomEdge03 = new _XImageStatic;
	pRightBottomEdge03->Create( 249, 172+30+266, 271, 200+30+266, &texturearchive, createcharacterindex );
	pRightBottomEdge03->SetClipRect( 232, 86, 254, 114 );
	InsertChildObject( pRightBottomEdge03 );
	
	// 위쪽라인03 ( 225 X 10 )
	_XImageStatic* pTopLine03 = new _XImageStatic;
	pTopLine03->Create( 24, 79+30+266, 249, 89+30+266, &texturearchive, createcharacterindex );
	pTopLine03->SetClipRect( 14, 212, 24, 222 );
	pTopLine03->SetScale( 22.5f, 1.0f );
	InsertChildObject( pTopLine03 );
	
	// 아래쪽라인03 ( 225 X 11 )
	_XImageStatic* pBottomLine03 = new _XImageStatic;
	pBottomLine03->Create( 24, 190+30+266, 249, 201+30+266, &texturearchive, createcharacterindex );
	pBottomLine03->SetClipRect( 14, 225, 24, 236 );
	pBottomLine03->SetScale( 22.5f, 1.0f );
	InsertChildObject( pBottomLine03 );
	
	// 왼쪽라인03 ( 10 X 71 )
	_XImageStatic* pLeftLine03 = new _XImageStatic;
	pLeftLine03->Create( 0, 101+30+266, 10, 172+30+266, &texturearchive, createcharacterindex );
	pLeftLine03->SetClipRect( 229, 200, 239, 210 );
	pLeftLine03->SetScale( 1.0f, 7.1f );
	InsertChildObject( pLeftLine03);
	
	// 오른쪽라인03 ( 10 X 71 )
	_XImageStatic* pRightLine03 = new _XImageStatic;
	pRightLine03->Create( 261, 101+30+266, 283, 172+30+266, &texturearchive, createcharacterindex );
	pRightLine03->SetClipRect( 244, 200, 254, 210 );
	pRightLine03->SetScale( 1.0f, 7.1f );
	InsertChildObject( pRightLine03);
	
	// 아이콘 박스09 ( 50 X 49 )
	_XImageStatic* pIconBox09 = new _XImageStatic;
	pIconBox09->Create( 13, 79+30+60+266, 63, 101+30+60+266, &texturearchive, createcharacterindex );
	pIconBox09->SetClipRect( 202, 3, 252, 52 );
	InsertChildObject( pIconBox09);
	
	// 아이콘 박스10 ( 50 X 49 )
	_XImageStatic* pIconBox10 = new _XImageStatic;
	pIconBox10->Create( 13+64, 79+30+60+266, 63+64, 101+30+60+266, &texturearchive, createcharacterindex );
	pIconBox10->SetClipRect( 202, 3, 252, 52 );
	InsertChildObject( pIconBox10);
	
	// 아이콘 박스11 ( 50 X 49 )
	_XImageStatic* pIconBox11 = new _XImageStatic;
	pIconBox11->Create( 13+64+64, 79+30+60+266, 63+64+64, 101+30+60+266, &texturearchive, createcharacterindex );
	pIconBox11->SetClipRect( 202, 3, 252, 52 );
	InsertChildObject( pIconBox11);
	
	// 아이콘 박스12 ( 50 X 49 )
	_XImageStatic* pIconBox12 = new _XImageStatic;
	pIconBox12->Create( 13+64+64+64, 79+30+60+266, 63+64+64+64, 101+30+60+266, &texturearchive, createcharacterindex );
	pIconBox12->SetClipRect( 202, 3, 252, 52 );
	InsertChildObject( pIconBox12);
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 상세설명 바탕 01 ( 181 X 88 )
	_XImageStatic* pBorder01 = new _XImageStatic;
	pBorder01->Create( 294, 517, 294+181, 517+88, &texturearchive, createcharacterindex );
	pBorder01->SetClipRect( 2, 1, 183, 89 );
	InsertChildObject( pBorder01);
	// 상세설명 바탕 02 ( 180 X 88 )
	_XImageStatic* pBorder02 = new _XImageStatic;
	pBorder02->Create( 294+181, 517, 294+181+180, 517+88, &texturearchive, createcharacterindex );
	pBorder02->SetClipRect( 2, 94, 182, 182 );
	InsertChildObject( pBorder02);
	

	// 아이콘 버튼을 만든다.
	int iconindex = g_MainInterfaceTextureArchive.FindResource("item_mon.tga");	// 이미지 없는 아이템들은 다 이걸로...
		
	_XBTN_STRUCTURE btnicon = { TRUE, { 25, 179 }, { 30, 30 }, _XDEF_EXPLAINCHARACTER_BTN_01,
		iconindex,iconindex,iconindex, 
		&g_MainInterfaceTextureArchive };

	// 무기 아이콘을 표시한다.
	m_btnIcon[0] = new _XButton;
	m_btnIcon[0]->Create(btnicon);
	m_btnIcon[0]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 1, 31, 31 );
	m_btnIcon[0]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 1, 31, 31 );
	InsertChildObject(m_btnIcon[0]);
	
	++btnicon.commandid;	
	btnicon.position.x += 64;
	m_btnIcon[1] = new _XButton;
	m_btnIcon[1]->Create(btnicon);
	m_btnIcon[1]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 1, 31, 31 );
	m_btnIcon[1]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 1, 31, 31 );
	InsertChildObject(m_btnIcon[1]);
	
	++btnicon.commandid;
	btnicon.position.x += 64;
	m_btnIcon[2] = new _XButton;
	m_btnIcon[2]->Create(btnicon);
	m_btnIcon[2]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 1, 31, 31 );
	m_btnIcon[2]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 1, 31, 31 );
	InsertChildObject(m_btnIcon[2]);
	
	++btnicon.commandid;
	btnicon.position.x += 64;
	m_btnIcon[3] = new _XButton;
	m_btnIcon[3]->Create(btnicon);
	m_btnIcon[3]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 1, 31, 31 );
	m_btnIcon[3]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 1, 31, 31 );
	InsertChildObject(m_btnIcon[3]);
	
	// 의상 아이콘을 표시한다.
	++btnicon.commandid;
	btnicon.position.x = 27;
	btnicon.position.y += 133;
	m_btnIcon[4] = new _XButton;
	m_btnIcon[4]->Create(btnicon);
	m_btnIcon[4]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 1, 31, 31 );
	m_btnIcon[4]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 1, 31, 31 );
	InsertChildObject(m_btnIcon[4]);
	
	++btnicon.commandid;
	btnicon.position.x += 64;
	m_btnIcon[5] = new _XButton;
	m_btnIcon[5]->Create(btnicon);
	m_btnIcon[5]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 1, 31, 31 );
	m_btnIcon[5]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 1, 31, 31 );
	InsertChildObject(m_btnIcon[5]);
	
	++btnicon.commandid;
	btnicon.position.x += 64;
	m_btnIcon[6] = new _XButton;
	m_btnIcon[6]->Create(btnicon);
	m_btnIcon[6]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 1, 31, 31 );
	m_btnIcon[6]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 1, 31, 31 );
	InsertChildObject(m_btnIcon[6]);
	
	++btnicon.commandid;
	btnicon.position.x += 64;
	m_btnIcon[7] = new _XButton;
	m_btnIcon[7]->Create(btnicon);
	m_btnIcon[7]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 1, 31, 31 );
	m_btnIcon[7]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 1, 31, 31 );
	InsertChildObject(m_btnIcon[7]);
	
	
	// 무공 아이콘을 표시한다.
	++btnicon.commandid;
	btnicon.position.x = 27;
	btnicon.position.y += 133;
	m_btnIcon[8] = new _XButton;
	m_btnIcon[8]->Create(btnicon);
	m_btnIcon[8]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 1, 31, 31 );
	m_btnIcon[8]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 1, 31, 31 );
	InsertChildObject(m_btnIcon[8]);
	
	++btnicon.commandid;
	btnicon.position.x += 64;
	m_btnIcon[9] = new _XButton;
	m_btnIcon[9]->Create(btnicon);
	m_btnIcon[9]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 1, 31, 31 );
	m_btnIcon[9]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 1, 31, 31 );
	InsertChildObject(m_btnIcon[9]);
	
	++btnicon.commandid;
	btnicon.position.x += 64;
	m_btnIcon[10] = new _XButton;
	m_btnIcon[10]->Create(btnicon);
	m_btnIcon[10]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 1, 31, 31 );
	m_btnIcon[10]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 1, 31, 31 );
	InsertChildObject(m_btnIcon[10]);
	
	++btnicon.commandid;
	btnicon.position.x += 64;
	m_btnIcon[11] = new _XButton;
	m_btnIcon[11]->Create(btnicon);
	m_btnIcon[11]->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 1, 1, 31, 31 );
	m_btnIcon[11]->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  1, 1, 31, 31 );
	InsertChildObject(m_btnIcon[11]);
	

	return TRUE;
}

void _XWindow_ExplainCharacter::DestroyWindow()
{	
	_XWindow::DestroyWindow();
}

void _XWindow_ExplainCharacter::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)
		return;
	
	if( m_SelectedZone >= 1 )
	{			
		switch(m_SelectedZone) {
		case 1:	// 숭산 - 소림사
			{
				g_XLargeFont->SetColor(0xFFD7BC65);
				g_XLargeFont->PrintAlign( 10, 124, 1.0f, _XFONT_ALIGNTYPE_LEFT, "%s - %s", g_ZoneInfoTable[m_SelectedZone-1].zoneshortname,_XGETINTERFACETEXT(ID_STRING_GROUPNAME_03) );
				g_XLargeFont->Flush();		
								
				g_XBaseFont->SetColor( 0xFFD7BC65 );
				g_XBaseFont->SetBoldMode( TRUE );
				g_XBaseFont->PrintAlign( 25, 213, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3643), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03) );
				g_XBaseFont->PrintAlign( 25, 346, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3644), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03) );
				g_XBaseFont->PrintAlign( 25, 479, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3645), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_03) );
				g_XBaseFont->Flush();		
				g_XBaseFont->SetBoldMode( FALSE );
			}
			break;
		case 2:	// 천진 - 개방
			{
				g_XLargeFont->SetColor(0xFFD7BC65);
				g_XLargeFont->PrintAlign( 10, 124, 1.0f, _XFONT_ALIGNTYPE_LEFT, "%s - %s", g_ZoneInfoTable[m_SelectedZone-1].zoneshortname,_XGETINTERFACETEXT(ID_STRING_GROUPNAME_01) );
				g_XLargeFont->Flush();		
								
				g_XBaseFont->SetColor( 0xFFD7BC65 );
				g_XBaseFont->SetBoldMode( TRUE );
				g_XBaseFont->PrintAlign( 25, 213, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3643), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01) );
				g_XBaseFont->PrintAlign( 25, 346, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3644), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01) );
				g_XBaseFont->PrintAlign( 25, 479, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3645), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_01) );
				g_XBaseFont->Flush();		
				g_XBaseFont->SetBoldMode( FALSE );
			}
			break;
		case 3: // 태산 - 녹림맹
			{
				g_XLargeFont->SetColor(0xFFD7BC65);
				g_XLargeFont->PrintAlign( 10, 124, 1.0f, _XFONT_ALIGNTYPE_LEFT, "%s - %s", g_ZoneInfoTable[m_SelectedZone-1].zoneshortname,_XGETINTERFACETEXT(ID_STRING_GROUPNAME_04) );
				g_XLargeFont->Flush();	
								
				g_XBaseFont->SetColor( 0xFFD7BC65 );
				g_XBaseFont->SetBoldMode( TRUE );
				g_XBaseFont->PrintAlign( 25, 213, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3643), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04) );
				g_XBaseFont->PrintAlign( 25, 346, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3644), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04) );
				g_XBaseFont->PrintAlign( 25, 479, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3645), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_04) );
				g_XBaseFont->Flush();		
				g_XBaseFont->SetBoldMode( FALSE );
			}
			break;
		case 4: // 자하동 - 비궁
			{
				g_XLargeFont->SetColor(0xFFD7BC65);
				g_XLargeFont->PrintAlign( 10, 124, 1.0f, _XFONT_ALIGNTYPE_LEFT, "%s - %s", g_ZoneInfoTable[m_SelectedZone-1].zoneshortname,_XGETINTERFACETEXT(ID_STRING_GROUPNAME_02) );
				g_XLargeFont->Flush();		
								
				g_XBaseFont->SetColor( 0xFFD7BC65 );
				g_XBaseFont->SetBoldMode( TRUE );
				g_XBaseFont->PrintAlign( 25, 213, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3643), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02) );
				g_XBaseFont->PrintAlign( 25, 346, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3644), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02) );
				g_XBaseFont->PrintAlign( 25, 479, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3645), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_02) );
				g_XBaseFont->Flush();		
				g_XBaseFont->SetBoldMode( FALSE );
			}
			break;
		case 5: // 무당산 - 무당
			{
				g_XLargeFont->SetColor(0xFFD7BC65);
				g_XLargeFont->PrintAlign( 10, 124, 1.0f, _XFONT_ALIGNTYPE_LEFT, "%s - %s", g_ZoneInfoTable[m_SelectedZone-1].zoneshortname,_XGETINTERFACETEXT(ID_STRING_GROUPNAME_05) );
				g_XLargeFont->Flush();		
				
				
				g_XBaseFont->SetColor( 0xFFD7BC65 );
				g_XBaseFont->SetBoldMode( TRUE );
				g_XBaseFont->PrintAlign( 25, 213, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3643), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05) );
				g_XBaseFont->PrintAlign( 25, 346, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3644), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05) );
				g_XBaseFont->PrintAlign( 25, 479, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3645), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_05) );
				g_XBaseFont->Flush();		
				g_XBaseFont->SetBoldMode( FALSE );
			}
			break;
		case 6: // 염마산 - 마교
			{
				g_XLargeFont->SetColor(0xFFD7BC65);
				g_XLargeFont->PrintAlign( 10, 124, 1.0f, _XFONT_ALIGNTYPE_LEFT, "%s - %s", g_ZoneInfoTable[m_SelectedZone-1].zoneshortname,_XGETINTERFACETEXT(ID_STRING_GROUPNAME_06) );
				g_XLargeFont->Flush();		
								
				g_XBaseFont->SetColor( 0xFFD7BC65 );
				g_XBaseFont->SetBoldMode( TRUE );
				g_XBaseFont->PrintAlign( 25, 213, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3643), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06) );
				g_XBaseFont->PrintAlign( 25, 346, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3644), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06) );
				g_XBaseFont->PrintAlign( 25, 479, 1.0f, _XFONT_ALIGNTYPE_LEFT, _XGETINTERFACETEXT(ID_STRING_NEW_3645), _XGETINTERFACETEXT(ID_STRING_GROUPNAME_06) );
				g_XBaseFont->Flush();		
				g_XBaseFont->SetBoldMode( FALSE );
			}
			break;
		}
	}

}

BOOL _XWindow_ExplainCharacter::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	
	if(!this->m_ShowWindow)
		return FALSE;
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

extern void ChangeModel( DWORD lparam, char firstType, char facemaskmode = 0 );
void _XWindow_ExplainCharacter::SimulationButtonClick( int buttonnumber )
{	
	switch(buttonnumber) {
	case 1: // 무기 모델 교체하기
	case 2:
	case 3:
	case 4:
		{
			
		//	DWORD lparam = (_XINVENTORY_SLOTTYPE_WEAPON_1 * 100000) + g_pLocalUser->m_UserItemList[cFrom].m_sID;
		//	ChangeModel( lparam, g_pLocalUser->m_UserItemList[cFrom].m_cType );			
		}
		break;
	case 5: // 의상 교체하기
	case 6:
	case 7:
	case 8:
		{
		}
		break;
	case 9:	// 무공 시전하기 
	case 10:
	case 11:
	case 12:
		{
		}
		break;
	}
}
