// XWindow_CBResultNotice.cpp: implementation of the _XWindow_CBResultNotice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XWindow_CBResultNotice.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_CBResultNotice::_XWindow_CBResultNotice()
{
	m_nCastleNameTextId = 0;
	memset(m_strOrgName, 0, sizeof(m_strOrgName));
	m_bMonsterWin = FALSE;
}

_XWindow_CBResultNotice::~_XWindow_CBResultNotice()
{

}

BOOL _XWindow_CBResultNotice::Initialize()
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_manor01.tga");

	_XImageStatic* pLeftDecoImage = new _XImageStatic;
	pLeftDecoImage->Create(0, 22, 63, 22+235, &g_MainInterfaceTextureArchive, resourceindex);
	pLeftDecoImage->SetClipRect(3, 20, 86, 254);
	InsertChildObject(pLeftDecoImage);

	_XImageStatic* pRightDecoImage = new _XImageStatic;
	pRightDecoImage->Create(502, 22, 502+63, 22+235, &g_MainInterfaceTextureArchive, resourceindex);
	pRightDecoImage->SetClipRect(94, 20, 176, 254);
	InsertChildObject(pRightDecoImage);

	_XImageStatic* pUpBar = new _XImageStatic;
	pUpBar->Create(83, 37, 83+1, 37+4, &g_MainInterfaceTextureArchive, resourceindex);
	pUpBar->SetClipRect(85, 35, 86, 39);
	pUpBar->SetScale(419.f, 1.0f);
	InsertChildObject(pUpBar);

	_XImageStatic* pBottomBar = new _XImageStatic;
	pBottomBar->Create(83, 252, 83+1, 252+4, &g_MainInterfaceTextureArchive, resourceindex);
	pBottomBar->SetClipRect(85, 250, 86, 254);
	pBottomBar->SetScale(419.f, 1.0f);
	InsertChildObject(pBottomBar);

	_XImageStatic* pUpDecoImage = new _XImageStatic;
	pUpDecoImage->Create(195, 0, 195+195, 51, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("mi_manor02.tga"));
	pUpDecoImage->SetClipRect(59, 204, 254, 254);
	InsertChildObject(pUpDecoImage);

	// 버튼 (56x20)
	_XBTN_STRUCTURE btnstruct = 
	{	 TRUE, 
		{ 262, 220 }, 
		{ 56, 20 }, 
		_XDEF_CBRESULTNOTICE_CLOSE,
		g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" ),
		g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" ),
		g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" ),
		&g_MainInterfaceTextureArchive 
	};

	_XButton* btnOk = new _XButton;
	btnOk->Create( btnstruct );
	btnOk->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 191, 196, 247, 216 );
	btnOk->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  191, 169, 247, 189 );
	btnOk->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  191, 223, 247, 243 );
	btnOk->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_OK );	// _T("확인")
	btnOk->SetButtonTextColor( 0xFFE0D7BA );
	InsertChildObject( btnOk );

	return TRUE;
}

void _XWindow_CBResultNotice::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}

void _XWindow_CBResultNotice::Draw( _XGUIObject*& pfocusobject )
{
	if(this->m_ShowWindow)
	{
		_XDrawSolidBar(m_WindowPosition.x+82, m_WindowPosition.y+41, m_WindowPosition.x+504, m_WindowPosition.y+254, 0x7f000000);
	}

	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)	return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	// TODO
	g_XLargeFont->SetBoldMode(TRUE);
	g_XLargeFont->SetGrowMode(_XFONT_EDGEMODE_EDGE, D3DCOLOR_ARGB(255, 144, 93, 2));
	g_XLargeFont->SetColor(D3DCOLOR_ARGB(255, 237, 229, 193));
	if(m_bMonsterWin)
	{
		g_XLargeFont->PrintAlign(m_WindowPosition.x+293, m_WindowPosition.y+54, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3296));//많은 영웅단들의 공격에도 불구하고
		g_XLargeFont->PrintAlign(m_WindowPosition.x+293, m_WindowPosition.y+103, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3297), _XGETINTERFACETEXT(m_nCastleNameTextId));//어느 누구도 %s을
		g_XLargeFont->PrintAlign(m_WindowPosition.x+293, m_WindowPosition.y+133, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3298));//함락시키지 못하였습니다.
		g_XLargeFont->PrintAlign(m_WindowPosition.x+293, m_WindowPosition.y+163, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3299), _XGETINTERFACETEXT(m_nCastleNameTextId));//%s의 세력이 더 커지기 전에
		g_XLargeFont->PrintAlign(m_WindowPosition.x+293, m_WindowPosition.y+193, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3300));//다시 한번 그들을 제압해 주시기 바랍니다.
	}
	else
	{
		g_XLargeFont->PrintAlign(m_WindowPosition.x+293, m_WindowPosition.y+54, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3301));//축하합니다.
		g_XLargeFont->PrintAlign(m_WindowPosition.x+293, m_WindowPosition.y+103, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3302), _XGETINTERFACETEXT(m_nCastleNameTextId));//%s 장원전에서
		g_XLargeFont->PrintAlign(m_WindowPosition.x+293, m_WindowPosition.y+133, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3303), m_strOrgName);//%s(이)가 승리하였습니다.
		g_XLargeFont->PrintAlign(m_WindowPosition.x+293, m_WindowPosition.y+163, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3304), _XGETINTERFACETEXT(m_nCastleNameTextId));//지금부터 %s은
		g_XLargeFont->PrintAlign(m_WindowPosition.x+293, m_WindowPosition.y+193, 1.0f, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_NEW_3305), m_strOrgName);//%s의 소유가 됩니다.
	}
	g_XLargeFont->Flush();
	g_XLargeFont->DisableGrowMode();
	g_XLargeFont->SetBoldMode(FALSE);
}

BOOL _XWindow_CBResultNotice::Process( _XGUIObject*& pfocusobject )
{	
	if(!this->m_Enable)
		return FALSE;

	if(!this->m_ShowWindow)
		return FALSE;

	// TODO

	if( !_XWindow::Process(pfocusobject) ) 
		return FALSE;

	return TRUE;
}

void _XWindow_CBResultNotice::ShowWindow(BOOL show)
{
	if(!show)
	{
		m_nCastleNameTextId = 0;
		memset(m_strOrgName, 0, sizeof(m_strOrgName));
		m_bMonsterWin = FALSE;
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_CBResultNotice::SetCastleName(int index)
{
	m_nCastleNameTextId = index;
}

void _XWindow_CBResultNotice::SetOrgName(TCHAR* orgname, BOOL bMonsterWin, int orgtype)
{
	memset(m_strOrgName, 0, sizeof(m_strOrgName));

	if(orgtype == 0)
	{
		sprintf(m_strOrgName, _XGETINTERFACETEXT(ID_STRING_ORGANIZATION_ORGANIZATIONINFO2), orgname);	//%s단
	}
	else if(orgtype == 1)
	{	
		sprintf(m_strOrgName, _XGETINTERFACETEXT(ID_STRING_NEW_3125), orgname);	//%s회
	}

	m_bMonsterWin = bMonsterWin;
}