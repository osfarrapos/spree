// XWindow_CreateCharacter.cpp: implementation of the _XWindow_CreateCharacter class.
//
// Created Date		: 2003.05.05
// Author			: Sohyun, Park (kukuri)
// Desc				: Character Create Window
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "XWindow_CreateCharacter.h"
#include "XProc_ProcessDefine.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

extern void _XDrawSolidBar (FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color);


void __stdcall _XCallback_CreateButton_Gender( POINT pos, SIZE size )
{
	g_XBaseFont->SetColor( 0xFFD5C68D );
	g_XBaseFont->Puts( 32, gnHeight-64, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_SELECTSEX) );
	g_XBaseFont->Flush();
}

void __stdcall _XCallback_CreateButton_Face( POINT pos, SIZE size )
{
	g_XBaseFont->SetColor( 0xFFD5C68D );
	g_XBaseFont->Puts( 32, gnHeight-64, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_CHANGEFACE) );
	g_XBaseFont->Flush();
}

void __stdcall _XCallback_CreateButton_Hair( POINT pos, SIZE size )
{
	g_XBaseFont->SetColor( 0xFFD5C68D );
	g_XBaseFont->Puts( 32, gnHeight-64, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_SELECTHAIR) );
	g_XBaseFont->Flush();
}

void __stdcall _XCallback_CreateButton_Cloth( POINT pos, SIZE size )
{
	g_XBaseFont->SetColor( 0xFFD5C68D );
	g_XBaseFont->Puts( 32, gnHeight-64, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_SELECTCLOTHES) );
	g_XBaseFont->Flush();
}

void __stdcall _XCallback_CreateButton_Vitality( POINT pos, SIZE size )
{
	g_XBaseFont->SetColor( 0xFFD5C68D );
	g_XBaseFont->Puts( 32, gnHeight-95, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_STRENGTH) );	// ±Ù·Â
	g_XBaseFont->Puts( 33, gnHeight-95, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_STRENGTH) );		
	g_XBaseFont->Puts_SeparateAlign( 32, gnHeight-78, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2200), gnWidth/2, _XFONT_ALIGNTYPE_LEFT );	
	g_XBaseFont->Flush();
}

void __stdcall _XCallback_CreateButton_Zen( POINT pos, SIZE size )
{
	g_XBaseFont->SetColor( 0xFFD5C68D );
	g_XBaseFont->Puts( 32, gnHeight-95, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_INNERENERGY) );	// Áø±â
	g_XBaseFont->Puts( 33, gnHeight-95, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_INNERENERGY) );	
	g_XBaseFont->Puts_SeparateAlign( 32, gnHeight-78, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2201), gnWidth/2, _XFONT_ALIGNTYPE_LEFT );
	g_XBaseFont->Flush();
}

void __stdcall _XCallback_CreateButton_Soul( POINT pos, SIZE size )
{
	g_XBaseFont->SetColor( 0xFFD5C68D );
	g_XBaseFont->Puts( 32, gnHeight-95, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_HEART) );		// ÁöÇý
	g_XBaseFont->Puts( 33, gnHeight-95, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_HEART) );
	g_XBaseFont->Puts_SeparateAlign( 32, gnHeight-78, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2202), gnWidth/2, _XFONT_ALIGNTYPE_LEFT );
	g_XBaseFont->Flush();
}

void __stdcall _XCallback_CreateButton_Heart( POINT pos, SIZE size )
{
	g_XBaseFont->SetColor( 0xFFD5C68D );
	g_XBaseFont->Puts( 32, gnHeight-95, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_HEALTHENERGY) );	// °Ç°­
	g_XBaseFont->Puts( 33, gnHeight-95, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_HEALTHENERGY) );
	g_XBaseFont->Puts_SeparateAlign( 32, gnHeight-78, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2203), gnWidth/2, _XFONT_ALIGNTYPE_LEFT );
	g_XBaseFont->Flush();
}

void __stdcall _XCallback_CreateButton_Stamina( POINT pos, SIZE size )
{
	g_XBaseFont->SetColor( 0xFFD5C68D );
	g_XBaseFont->Puts( 32, gnHeight-95, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_QUICKNESS) );	// ¹ÎÃ¸
	g_XBaseFont->Puts( 33, gnHeight-95, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_QUICKNESS) );
	g_XBaseFont->Puts_SeparateAlign( 32, gnHeight-78, _XGETINTERFACETEXT(ID_STRING_DEFAULTTOOLTIP_2204), gnWidth/2, _XFONT_ALIGNTYPE_LEFT );
	g_XBaseFont->Flush();	
}

void __stdcall _XCallback_CreateButton_SelectZone( POINT pos, SIZE size )
{
	g_XBaseFont->SetColor( 0xFFD5C68D );
	g_XBaseFont->Puts( 32, gnHeight-64, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_SELECTZONE) );
	g_XBaseFont->Flush();
}

void __stdcall _XCallback_CreateCharacterButton( POINT pos, SIZE size )
{
	g_XBaseFont->SetColor( 0xFFD5C68D );
	g_XBaseFont->Puts( 32, gnHeight-64, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_CREATECHAR) );
	g_XBaseFont->Flush();
}

void __stdcall _XCallback_CancelCreateCharacterButton( POINT pos, SIZE size )
{
	g_XBaseFont->SetColor( 0xFFD5C68D );
	g_XBaseFont->Puts( 32, gnHeight-64, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_RETURN) );
	g_XBaseFont->Flush();
}

void __stdcall _XCallback_ReturnToLobbyButton( POINT pos, SIZE size )
{
	g_XBaseFont->SetColor( 0xFFD5C68D );
	g_XBaseFont->Puts( 32, gnHeight-64, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_2474) );
	g_XBaseFont->Flush();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_CreateCharacter::_XWindow_CreateCharacter()
{
	m_ManButton = NULL;
	m_WomanButton = NULL;

	m_FaceLeftButton = NULL;
	m_FaceRightButton = NULL;
	m_HairLeftButton = NULL;
	m_HairRightButton = NULL;
	m_ClothLeftButton = NULL;
	m_ClothRightButton = NULL;

	m_VitalityPlusButton = NULL;
	m_VitalityMinusButton = NULL;
	m_ZenPlusButton = NULL;
	m_ZenMinusButton = NULL;
	m_SoulPlusButton = NULL;
	m_SoulMinusButton = NULL;
	m_HeartPlusButton = NULL;
	m_HeartMinusButton = NULL;
	m_StaminaPlusButton = NULL;
	m_StaminaMinusButton = NULL;

	m_pAreaSelectCheckButton[0] = NULL;
	m_pAreaSelectCheckButton[1] = NULL;
	m_pAreaSelectCheckButton[2] = NULL;
	m_pAreaSelectCheckButton[3] = NULL;
	m_pAreaSelectCheckButton[4] = NULL;
	m_pAreaSelectCheckButton[5] = NULL;	

	m_FaceTypeIndex		= 1;
	m_HairTypeIndex		= 1;
	m_ClothTypeIndex	= 1;
}

_XWindow_CreateCharacter::~_XWindow_CreateCharacter()
{

}

void __stdcall _XIMECallback_NameEditReturn( _XIMEContainer* pIMEContainer )
{
	_XIMEKERNEL.SetFocus( NULL );
//	PostMessage(gHWnd, WM_COMMAND, MAKEWPARAM(_XPROCDEF_IDEDITENTER, 0), (LPARAM)gHWnd);
}


BOOL _XWindow_CreateCharacter::Initialize(_XTextureManager& texturearchive)
{
	_XWindow::SetDrawBorder( FALSE );

	int btnimageindex = texturearchive.FindResource("MI_Login_00.tga");
	int edgeimageindex = texturearchive.FindResource("MI_Login_01.tga");

	_XImageStatic* pTitle = new _XImageStatic;
	pTitle->Create( 0, 0, 196, 23, &texturearchive, edgeimageindex );
	pTitle->SetClipRect( 0, 19, 196, 42 );
	InsertChildObject( pTitle );

	_XImageStatic* pZoneTitle = new _XImageStatic;
	pZoneTitle->Create( 0, 340, 196, 363, &texturearchive, edgeimageindex );
	pZoneTitle->SetClipRect( 0, 19, 196, 42 );
	InsertChildObject( pZoneTitle );

	_XImageStatic* pZoneTitle2 = new _XImageStatic;
	pZoneTitle2->Create( 0, 363, 196, 442, &texturearchive, edgeimageindex );
	pZoneTitle2->SetClipRect( 0, 42, 196, 122 );
	pZoneTitle2->SetScale(1.0f, 1.45f);
	InsertChildObject( pZoneTitle2 );

	_XImageStatic* pInfoEdge_Top = new _XImageStatic;
	pInfoEdge_Top->Create( 25, 176, 167, 177, &texturearchive, edgeimageindex );
	pInfoEdge_Top->SetClipRect( 0, 0, 142, 1 );
	InsertChildObject( pInfoEdge_Top );
	
	_XImageStatic* pInfoEdge_Bottom = new _XImageStatic;
	pInfoEdge_Bottom->Create( 25, 298, 167, 299, &texturearchive, edgeimageindex );
	pInfoEdge_Bottom->SetClipRect( 0, 2, 142, 3 );
	InsertChildObject( pInfoEdge_Bottom );
	
	_XImageStatic* pInfoEdge_Left = new _XImageStatic;
	pInfoEdge_Left->Create( 24, 177, 25, 298, &texturearchive, edgeimageindex );
	pInfoEdge_Left->SetClipRect( 252, 0, 253, 121 );
	InsertChildObject(pInfoEdge_Left);
	
	_XImageStatic* pInfoEdge_Right = new _XImageStatic;
	pInfoEdge_Right->Create( 167, 177, 168, 298, &texturearchive, edgeimageindex );
	pInfoEdge_Right->SetClipRect( 255, 0, 256, 121 );
	InsertChildObject(pInfoEdge_Right);

	_XImageStatic* pInfoBarEdge_Left = new _XImageStatic;
	pInfoBarEdge_Left->Create( 1, 23, 3, 336, &texturearchive, btnimageindex );
	pInfoBarEdge_Left->SetClipRect( 254, 0, 256, 236 );
	pInfoBarEdge_Left->SetScale( 1.0f, 314.0f / 236.0f );
	InsertChildObject( pInfoBarEdge_Left );
	
	_XImageStatic* pInfoBarEdge_Right = new _XImageStatic;
	pInfoBarEdge_Right->Create( 195, 23, 196, 336, &texturearchive, btnimageindex );
	pInfoBarEdge_Right->SetClipRect( 254, 0, 256, 236 );
	pInfoBarEdge_Right->SetScale( -1.0f, 314.0f / 236.0f );
	InsertChildObject( pInfoBarEdge_Right );

	_XImageStatic* pInfoBarEdge_Bottom = new _XImageStatic;	
	pInfoBarEdge_Bottom->Create( 0, 336, 193, 319, &texturearchive, btnimageindex );
	
	pInfoBarEdge_Bottom->SetClipRect( 0, 252, 196, 256 );
	InsertChildObject( pInfoBarEdge_Bottom );	
		
	_XCHECKBTN_STRUCTURE manbtn = 
	{ 
		TRUE, { 24, 82 }, 
		{ 14, 14 }, 
		_XDEF_CC_MANBUTTON,
		edgeimageindex,edgeimageindex,
		&texturearchive 
	};

	m_ManButton = new _XCheckButton;
	m_ManButton->Create(manbtn);
	m_ManButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 198,89, 212, 103 );
	m_ManButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  198,73, 212, 87 );
	m_ManButton->SetButtonSwapMode( FALSE );
	m_ManButton->SetCheck( TRUE );
	//m_ManButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SKILLTOOLTIP_MALE );
	m_ManButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Gender );
	InsertChildObject(m_ManButton);

	_XCHECKBTN_STRUCTURE womanbtn = 
	{ 
		TRUE, { 101, 82 }, 
		{ 14, 14 }, 
		_XDEF_CC_WOMENBUTTON,
		edgeimageindex,edgeimageindex,
		&texturearchive 
	};
	
	m_WomanButton = new _XCheckButton;
	m_WomanButton->Create(womanbtn);
	m_WomanButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 198,89, 212, 103 );
	m_WomanButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  198,73, 212, 87 );
	m_WomanButton->SetButtonSwapMode( FALSE );
	m_WomanButton->SetCheck( FALSE );
	//m_WomanButton->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_SKILLTOOLTIP_FEMALE );
	m_WomanButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Gender );
	InsertChildObject(m_WomanButton);

	// Face
	_XBTN_STRUCTURE leftbtn = 
	{
		TRUE, 
		{95, 107}, {12, 12},
		_XDEF_CC_FACELEFTBUTTON,
		btnimageindex,
		btnimageindex,
		btnimageindex,
		&texturearchive
	};
	m_FaceLeftButton = new _XButton;
	m_FaceLeftButton->Create(leftbtn);
	m_FaceLeftButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 147, 180, 159, 192 );
	m_FaceLeftButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  147, 192, 159, 204 );
	m_FaceLeftButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  147, 204, 156, 216 );
	m_FaceLeftButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Face );
	InsertChildObject(m_FaceLeftButton);

	_XBTN_STRUCTURE rightbtn = 
	{
		TRUE, 
		{145, 107 }, {12, 12},
		_XDEF_CC_FACERIGHTBUTTON,
		btnimageindex,
		btnimageindex,
		btnimageindex,
		&texturearchive
	};
	m_FaceRightButton = new _XButton;
	m_FaceRightButton->Create(rightbtn);
	m_FaceRightButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 159, 180, 171, 192 );
	m_FaceRightButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  159, 192, 171, 204 );
	m_FaceRightButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  159, 204, 171, 216 );
	m_FaceRightButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Face );
	InsertChildObject(m_FaceRightButton);

	// Hair
	leftbtn.position.x = 95;
	leftbtn.position.y = 130;
	leftbtn.commandid = _XDEF_CC_HAIRLEFTBUTTON;
	m_HairLeftButton = new _XButton;
	m_HairLeftButton->Create(leftbtn);
	m_HairLeftButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 147, 180, 159, 192 );
	m_HairLeftButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  147, 192, 159, 204 );
	m_HairLeftButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  147, 204, 156, 216 );
	m_HairLeftButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Hair );
	InsertChildObject(m_HairLeftButton);

	rightbtn.position.x = 145;
	rightbtn.position.y = 130;
	rightbtn.commandid = _XDEF_CC_HAIRRIGHTBUTTON;
	m_HairRightButton = new _XButton;
	m_HairRightButton->Create(rightbtn);
	m_HairRightButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 159, 180, 171, 192 );
	m_HairRightButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  159, 192, 171, 204 );
	m_HairRightButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  159, 204, 171, 216 );
	m_HairRightButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Hair );
	InsertChildObject(m_HairRightButton);

	// Cloth
	leftbtn.position.x = 95;
	leftbtn.position.y = 151;
	leftbtn.commandid = _XDEF_CC_CLOTHLEFTBUTTON;
	m_ClothLeftButton = new _XButton;
	m_ClothLeftButton->Create(leftbtn);
	m_ClothLeftButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 147, 180, 159, 192 );
	m_ClothLeftButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  147, 192, 159, 204 );
	m_ClothLeftButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  147, 204, 156, 216 );
	m_ClothLeftButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Cloth );
	InsertChildObject(m_ClothLeftButton);
	
	rightbtn.position.x = 145;
	rightbtn.position.y = 151;
	rightbtn.commandid = _XDEF_CC_CLOTHRIGHTBUTTON;
	m_ClothRightButton = new _XButton;
	m_ClothRightButton->Create(rightbtn);
	m_ClothRightButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 159, 180, 171, 192 );
	m_ClothRightButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  159, 192, 171, 204 );
	m_ClothRightButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  159, 204, 171, 216 );
	m_ClothRightButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Cloth );
	InsertChildObject(m_ClothRightButton);

	// Vitality
	_XBTN_STRUCTURE plusbtn = 
	{
		TRUE, 
		{128, 185}, {12, 12},
		_XDEF_CC_ADDBUTON1,
		btnimageindex,
		btnimageindex,
		btnimageindex,
		&texturearchive
	};
	m_VitalityPlusButton = new _XButton;
	m_VitalityPlusButton->Create(plusbtn);
	m_VitalityPlusButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 147, 143, 159, 155 );
	m_VitalityPlusButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  147, 155, 159, 167 );
	m_VitalityPlusButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  147, 167, 159, 179 );
	m_VitalityPlusButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Vitality );
	InsertChildObject(m_VitalityPlusButton);

	_XBTN_STRUCTURE minusbtn = 
	{
		TRUE, 
		{141, 185}, {12, 12},
		_XDEF_CC_SUBBUTON1,
		btnimageindex,
		btnimageindex,
		btnimageindex,
		&texturearchive
	};
	m_VitalityMinusButton = new _XButton;
	m_VitalityMinusButton->Create(minusbtn);
	m_VitalityMinusButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 159, 143, 171, 155 );
	m_VitalityMinusButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  159, 155, 171, 167 );
	m_VitalityMinusButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  159, 167, 171, 179 );
	m_VitalityMinusButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Vitality );
	InsertChildObject(m_VitalityMinusButton);

	// Zen
	plusbtn.position.x = 128;
	plusbtn.position.y = 208;
	plusbtn.commandid = _XDEF_CC_ADDBUTON2;
	m_ZenPlusButton = new _XButton;
	m_ZenPlusButton->Create(plusbtn);
	m_ZenPlusButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 147, 143, 159, 155 );
	m_ZenPlusButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  147, 155, 159, 167 );
	m_ZenPlusButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  147, 167, 159, 179 );
	m_ZenPlusButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Zen );
	InsertChildObject(m_ZenPlusButton);

	minusbtn.position.x = 141;
	minusbtn.position.y = 208;
	minusbtn.commandid = _XDEF_CC_SUBBUTON2;
	m_ZenMinusButton = new _XButton;
	m_ZenMinusButton->Create(minusbtn);
	m_ZenMinusButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 159, 143, 171, 155 );
	m_ZenMinusButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  159, 155, 171, 167 );
	m_ZenMinusButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  159, 167, 171, 179 );
	m_ZenMinusButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Zen );
	InsertChildObject(m_ZenMinusButton);

	// Soul
	plusbtn.position.x = 128;
	plusbtn.position.y = 230;
	plusbtn.commandid = _XDEF_CC_ADDBUTON3;
	m_SoulPlusButton = new _XButton;
	m_SoulPlusButton->Create(plusbtn);
	m_SoulPlusButton->EnableWindow( TRUE );
	m_SoulPlusButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 147, 143, 159, 155 );
	m_SoulPlusButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  147, 155, 159, 167 );
	m_SoulPlusButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  147, 167, 159, 179 );
	m_SoulPlusButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Soul );
	InsertChildObject(m_SoulPlusButton);
	
	minusbtn.position.x = 141;
	minusbtn.position.y = 230;
	minusbtn.commandid = _XDEF_CC_SUBBUTON3;
	m_SoulMinusButton = new _XButton;
	m_SoulMinusButton->Create(minusbtn);
	m_SoulMinusButton->EnableWindow( TRUE );
	m_SoulMinusButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 159, 143, 171, 155 );
	m_SoulMinusButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  159, 155, 171, 167 );
	m_SoulMinusButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  159, 167, 171, 179 );
	m_SoulMinusButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Soul );
	InsertChildObject(m_SoulMinusButton);

	// Heart
	plusbtn.position.x = 128;
	plusbtn.position.y = 252;
	plusbtn.commandid = _XDEF_CC_ADDBUTON4;
	m_HeartPlusButton = new _XButton;
	m_HeartPlusButton->Create(plusbtn);
	m_HeartPlusButton->EnableWindow( TRUE );
	m_HeartPlusButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 147, 143, 159, 155 );
	m_HeartPlusButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  147, 155, 159, 167 );
	m_HeartPlusButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  147, 167, 159, 179 );
	m_HeartPlusButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Heart );
	InsertChildObject(m_HeartPlusButton);
	
	minusbtn.position.x = 141;
	minusbtn.position.y = 252;
	minusbtn.commandid = _XDEF_CC_SUBBUTON4;
	m_HeartMinusButton = new _XButton;
	m_HeartMinusButton->Create(minusbtn);
	m_HeartMinusButton->EnableWindow( TRUE );
	m_HeartMinusButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 159, 143, 171, 155 );
	m_HeartMinusButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  159, 155, 171, 167 );
	m_HeartMinusButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  159, 167, 171, 179 );
	m_HeartMinusButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Heart );
	InsertChildObject(m_HeartMinusButton);

	// Stamina
	plusbtn.position.x = 128;
	plusbtn.position.y = 274;
	plusbtn.commandid = _XDEF_CC_ADDBUTON5;
	m_StaminaPlusButton = new _XButton;
	m_StaminaPlusButton->Create(plusbtn);
	m_StaminaPlusButton->EnableWindow( TRUE );
	m_StaminaPlusButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 147, 143, 159, 155 );
	m_StaminaPlusButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  147, 155, 159, 167 );
	m_StaminaPlusButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  147, 167, 159, 179 );
	m_StaminaPlusButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Stamina );
	InsertChildObject(m_StaminaPlusButton);
	
	minusbtn.position.x = 141;
	minusbtn.position.y = 274;
	minusbtn.commandid = _XDEF_CC_SUBBUTON5;
	m_StaminaMinusButton = new _XButton;
	m_StaminaMinusButton->Create(minusbtn);
	m_StaminaMinusButton->EnableWindow( TRUE );
	m_StaminaMinusButton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 159, 143, 171, 155 );
	m_StaminaMinusButton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  159, 155, 171, 167 );
	m_StaminaMinusButton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  159, 167, 171, 179 );
	m_StaminaMinusButton->SetCallBackFunction_ToolTip( _XCallback_CreateButton_Stamina );
	InsertChildObject(m_StaminaMinusButton);

	// Create
	_XBTN_STRUCTURE createbtn = 
	{
		TRUE, 
		{gnWidth - (1024 - 886), gnHeight - (768-660)}, {110, 25},
		//{42, 453}, {110, 25},
		_XDEF_CC_CREATEBUTTON,
		btnimageindex,
		btnimageindex,
		btnimageindex,
		&texturearchive
	};	
	m_CreateButton.Create(createbtn);
	m_CreateButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 34, 191, 144, 216 );
	m_CreateButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  34, 191, 144, 216 );
	m_CreateButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  34, 164, 144, 189 );
	m_CreateButton.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_CREATECHARACTER_CREATE );
	m_CreateButton.SetButtonTextColor( 0xFFFFFFFF );
	m_CreateButton.SetCallBackFunction_ToolTip( _XCallback_CreateCharacterButton );
	
	// Cancel
	_XBTN_STRUCTURE cancelbtn = 
	{
		TRUE, 
		{gnWidth - (1024 - 886), gnHeight - (768-685)}, {110, 25},
		//{42, 482}, {110, 25},
		_XDEF_CC_CANCELBUTTON,
		btnimageindex,
		btnimageindex,
		btnimageindex,
		&texturearchive
	};	
	m_CancelButton.Create(cancelbtn);
	m_CancelButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 34, 191, 144, 216 );
	m_CancelButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  34, 191, 144, 216 );
	m_CancelButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  34, 164, 144, 189 );
	m_CancelButton.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_CREATECHARACTER_CANCLE );
	m_CancelButton.SetButtonTextColor( 0xFFFFFFFF );
	m_CancelButton.SetCallBackFunction_ToolTip( _XCallback_CancelCreateCharacterButton );

	// Cancel
	_XBTN_STRUCTURE exitbtn = 
	{
		TRUE, 
		{ gnWidth - (1024 - 886), gnHeight - (768-710) }, {110, 25},
		_XDEF_CC_EXITBUTTON,
		btnimageindex,
		btnimageindex,
		btnimageindex,
		&texturearchive
	};	
	m_ExitButton.Create(exitbtn);
	m_ExitButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 34, 191, 144, 216 );
	m_ExitButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  34, 191, 144, 216 );
	m_ExitButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  34, 164, 144, 189 );
	m_ExitButton.SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_CREATECHARACTER_EXIT );
	m_ExitButton.SetButtonTextColor( 0xFFFFFFFF );
	m_ExitButton.SetCallBackFunction_ToolTip( _XCallback_ReturnToLobbyButton );
	
	_XImageStatic* pNameBorder = new _XImageStatic;
	pNameBorder->Create( 59, 30, m_WindowPosition.x + 151, m_WindowPosition.y + 106, &texturearchive, edgeimageindex );	
	pNameBorder->SetClipRect( 4, 150, 113, 172 );
	InsertChildObject(pNameBorder);

	_XImageStatic* pKiBorder = new _XImageStatic;
	pKiBorder->Create( 52, 305, 140, 331, &texturearchive, btnimageindex );
	pKiBorder->SetClipRect( 31, 221, 122, 250 );
	InsertChildObject(pKiBorder);

	POINT btnposarray[6] = 
	{
		{ 16,  402 },
		{ 16,  369 },
		{ 101, 369 },
		{ 101, 402 },
		{ 16, 435 },
		{ 101, 435 }
	};
	
	_XCHECKBTN_STRUCTURE checkbtnstruct = { TRUE, { 16, 369 }, 
											{ 75, 34 }, 
											_XDEF_CC_AREACHECKBUTTON1,
											edgeimageindex,edgeimageindex,
											&texturearchive };

	for( int i = 0; i < 4; i++ )
	{
		checkbtnstruct.position = btnposarray[i];
		m_pAreaSelectCheckButton[i] = new _XCheckButton;
		m_pAreaSelectCheckButton[i]->Create( checkbtnstruct );	
		m_pAreaSelectCheckButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 3,  180, 78, 214 );
		m_pAreaSelectCheckButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  84, 180, 159,214 );
		m_pAreaSelectCheckButton[i]->SetButtonSwapMode( TRUE );
		m_pAreaSelectCheckButton[i]->SetButtonText( g_ZoneInfoTable[i].zoneshortname );
		m_pAreaSelectCheckButton[i]->SetButtonTextColor( 0xFF797778, 0xFFE5CA7D, 0xFFFFFFFF );
		m_pAreaSelectCheckButton[i]->SetCallBackFunction_ToolTip( _XCallback_CreateButton_SelectZone );

		m_pAreaSelectCheckButton[i]->EnableWindow( TRUE );
		
		//m_pAreaSelectCheckButton[i]->SetCheck( (i == 1) );

		InsertChildObject( m_pAreaSelectCheckButton[i] );

		checkbtnstruct.commandid++;
	}

	//Author : ¾çÈñ¿Õ //breif : ÀÏº» ¼Ò¸²,  ºñ±Ã ¼±ÅÃ X
	/*if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		m_pAreaSelectCheckButton[0]->ShowWindow(FALSE);
		m_pAreaSelectCheckButton[3]->ShowWindow(FALSE);
	}*/
	
	if( g_bNewClanQuest )
	{
		for( int i = 4; i < 6; i++ )
		{
			checkbtnstruct.position = btnposarray[i];
			m_pAreaSelectCheckButton[i] = new _XCheckButton;
			m_pAreaSelectCheckButton[i]->Create( checkbtnstruct );	
			m_pAreaSelectCheckButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 3,  180, 78, 214 );
			m_pAreaSelectCheckButton[i]->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  84, 180, 159,214 );
			m_pAreaSelectCheckButton[i]->SetButtonSwapMode( TRUE );
			m_pAreaSelectCheckButton[i]->SetButtonText( g_ZoneInfoTable[i].zoneshortname );
			m_pAreaSelectCheckButton[i]->SetButtonTextColor( 0xFF797778, 0xFFE5CA7D, 0xFFFFFFFF );
			m_pAreaSelectCheckButton[i]->SetCallBackFunction_ToolTip( _XCallback_CreateButton_SelectZone );		
			InsertChildObject( m_pAreaSelectCheckButton[i] );
			
			checkbtnstruct.commandid++;
		}		
	}

	m_NameEdit.ResetContainer(); 
	m_NameEdit.MoveWindow(m_WindowPosition.x + 77, m_WindowPosition.y + 35);
	
	m_NameEdit.ResizeWindow(80,  12);
	m_NameEdit.ShowWindow(FALSE);
	m_NameEdit.SetReturnKeyCallback(_XIMECallback_NameEditReturn);
	m_NameEdit.SetMaxLength(12);
	_XIMEKERNEL.SetFocus( &m_NameEdit );

	SetWindowMoveMode(_XWMOVE_FIXED);

	m_BonusPoint = 10;

	m_Parameter[0] = _XDEF_DEFALTPARAMETER_1;
	m_Parameter[1] = _XDEF_DEFALTPARAMETER_2;
	m_Parameter[2] = _XDEF_DEFALTPARAMETER_3;
	m_Parameter[3] = _XDEF_DEFALTPARAMETER_4;
	m_Parameter[4] = _XDEF_DEFALTPARAMETER_5;

	m_MenuEdge_Top.Create( gnWidth - (1024 - 885), gnHeight - (768 - 657), gnWidth - (1024 - 997), gnHeight - (768 - 658), &texturearchive, edgeimageindex );
	m_MenuEdge_Top.SetClipRect( 3, 7, 115, 8 );
	
	m_MenuEdge_Bottom.Create( gnWidth - (1024 - 885), gnHeight - (768 - 738), gnWidth - (1024 - 997), gnHeight - (768 - 739), &texturearchive, edgeimageindex );
	m_MenuEdge_Bottom.SetClipRect( 3, 11, 115, 12 );
	
	m_MenuEdge_Left.Create( gnWidth - (1024 - 884), gnHeight - (768 - 658), gnWidth - (1024 - 885), gnHeight - (768 - 738) , &texturearchive, edgeimageindex );
	m_MenuEdge_Left.SetClipRect( 244, 0, 245, 80 );
	
	m_MenuEdge_Right.Create( gnWidth - (1024 - 997), gnHeight - (768 - 658), gnWidth - (1024 - 998), gnHeight - (768 - 738), &texturearchive, edgeimageindex );
	m_MenuEdge_Right.SetClipRect( 247, 0, 248, 80 );
	
	return TRUE;
}

void _XWindow_CreateCharacter::DestroyWindow()
{
	m_NameEdit.ShowWindow(FALSE);
	
	_XWindow::DestroyWindow();
}

void _XWindow_CreateCharacter::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
		_XDrawSolidBar( m_WindowPosition.x+1, m_WindowPosition.y+23, m_WindowPosition.x+193, m_WindowPosition.y+331, 0x80000000 );

	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)
		return;

	DrawParameter();
	
	m_NameEdit.Draw();

	m_MenuEdge_Top.Draw();
	m_MenuEdge_Bottom.Draw();
	m_MenuEdge_Left.Draw();
	m_MenuEdge_Right.Draw();

	m_CreateButton.Draw();
	m_CancelButton.Draw();
	m_ExitButton.Draw();
}

BOOL _XWindow_CreateCharacter::Process(_XGUIObject*& pfocusobject)
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
				if(CheckNameEditBoxArea())
				{
					m_NameEdit.SetFocus();
				}
				//else
				//	_XIMEKERNEL.SetFocus( NULL );
			}
		}
	}
	
	m_CreateButton.Process(pfocusobject);
	m_CancelButton.Process(pfocusobject);
	m_ExitButton.Process(pfocusobject);

	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

void _XWindow_CreateCharacter::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
	
	m_NameEdit.MoveWindow(m_WindowPosition.x + 77, m_WindowPosition.y + 35);
}

BOOL _XWindow_CreateCharacter::CheckNameEditBoxArea(void)
{
	ScrnPos* scrnpos = gpInput->GetMouseVirScreenPos();
	
	if(scrnpos->x > (m_WindowPosition.x + 62) &&
		scrnpos->x < (m_WindowPosition.x + 166) &&
		scrnpos->z > (m_WindowPosition.y + 31) &&
		scrnpos->z < (m_WindowPosition.y + 48))
		return TRUE;
	
	return FALSE;
}

void _XWindow_CreateCharacter::DrawParameter(void)
{
	g_XBaseFont->SetColor( 0xFFD4D4D4 );	
	g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF000000 );
	g_XBaseFont->PutsAlign(m_WindowPosition.x + 94, m_WindowPosition.y + 6, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_2475) );
	
	g_XBaseFont->SetColor( 0xFFE5D7BC );	
	g_XBaseFont->Puts(m_WindowPosition.x + 65, m_WindowPosition.y + 313, _XGETINTERFACETEXT(ID_STRING_NEW_2630) );

	g_XBaseFont->SetColor( 0xFFD1D1D1 );	
	g_XBaseFont->PutsAlign(m_WindowPosition.x + 94, m_WindowPosition.y + 345, _XFONT_ALIGNTYPE_CENTER, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_STARTZONE) );

	g_XBaseFont->Flush();
	g_XBaseFont->DisableGrowMode();

	g_XBaseFont->SetColor( 0xFFD3C78D );	
	g_XBaseFont->Puts(m_WindowPosition.x + 25, m_WindowPosition.y + 35, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_NAME), 1.0f );
	g_XBaseFont->Puts(m_WindowPosition.x + 26, m_WindowPosition.y + 35, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_NAME), 1.0f );
	
	g_XBaseFont->SetColor( 0xFFD5D4D2 );	
	g_XBaseFont->Puts(m_WindowPosition.x + 25, m_WindowPosition.y + 64,  _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_SEX), 1.0f );
	g_XBaseFont->Puts(m_WindowPosition.x + 25, m_WindowPosition.y + 106,  _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_FACE), 1.0f );
	g_XBaseFont->Puts(m_WindowPosition.x + 25, m_WindowPosition.y + 129, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_HEAD), 1.0f );
	g_XBaseFont->Puts(m_WindowPosition.x + 25, m_WindowPosition.y + 150, _XGETINTERFACETEXT(ID_STRING_CREATECHARACTER_CLOTHES), 1.0f );

	g_XBaseFont->SetColor( 0xFFBBBAB5 );	
	// ±Ù·Â	
	g_XBaseFont->Puts(m_WindowPosition.x + 32, m_WindowPosition.y + 185,  _XGETINTERFACETEXT(ID_STRING_CINFO_STR), 1.0f );	
	// Áø±â	
	g_XBaseFont->Puts(m_WindowPosition.x + 32, m_WindowPosition.y + 208,  _XGETINTERFACETEXT(ID_STRING_CINFO_INNERENERGY), 1.0f );
	// ÁöÇý	
	g_XBaseFont->Puts(m_WindowPosition.x + 32, m_WindowPosition.y + 230,  _XGETINTERFACETEXT(ID_STRING_CINFO_HEART), 1.0f );
	// °Ç°­	
	g_XBaseFont->Puts(m_WindowPosition.x + 32, m_WindowPosition.y + 252,  _XGETINTERFACETEXT(ID_STRING_CINFO_HEALTHENERGY), 1.0f );
	// ¹ÎÃ¸
	g_XBaseFont->Puts(m_WindowPosition.x + 32, m_WindowPosition.y + 274,  _XGETINTERFACETEXT(ID_STRING_CINFO_DEX), 1.0f );

	g_XBaseFont->SetColor( 0xFFBAB484 );	

	// ±Ù·Â	
	g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 185, 1.0f, _T("%d"), m_Parameter[0] );	
	// Áø±â	
	g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 208, 1.0f, _T("%d"), m_Parameter[1] );
	// ÁöÇý	
	g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 230, 1.0f, _T("%d"), m_Parameter[2] );
	// °Ç°­	
	g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 252, 1.0f, _T("%d"), m_Parameter[3] );
	// ¹ÎÃ¸
	g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 274, 1.0f, _T("%d"), m_Parameter[4] );
	
	g_XBaseFont->SetColor( 0xFFD7BC65 );		
	g_XBaseFont->PrintAlign(m_WindowPosition.x + 121, m_WindowPosition.y + 106, 1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%2d/8"), m_FaceTypeIndex );	
	g_XBaseFont->PrintAlign(m_WindowPosition.x + 121, m_WindowPosition.y + 129, 1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%2d/21"), m_HairTypeIndex );	
	g_XBaseFont->PrintAlign(m_WindowPosition.x + 121, m_WindowPosition.y + 150, 1.0f, _XFONT_ALIGNTYPE_CENTER, _T("%2d/3"), m_ClothTypeIndex );	

	g_XBaseFont->SetColor( 0xFFD7BC65 );		
	g_XBaseFont->Puts(m_WindowPosition.x + 43, m_WindowPosition.y + 83,  _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_MALE), 1.0f );
	g_XBaseFont->Puts(m_WindowPosition.x + 120, m_WindowPosition.y + 83,  _XGETINTERFACETEXT(ID_STRING_SKILLTOOLTIP_FEMALE), 1.0f );

	// º¸³Ê½º Æ÷ÀÎÆ®	
	g_XBaseFont->PrintAlign(m_WindowPosition.x + 112, m_WindowPosition.y + 313, 1.0f, _XFONT_ALIGNTYPE_CENTER, "%d", m_BonusPoint );
	g_XBaseFont->Flush();
}