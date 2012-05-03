// XWindow_HelpScreen.cpp: implementation of the _XWindow_HelpScreen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_HelpScreen.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static int HS_ROW_INTERVAL = 15;

_XWindow_HelpScreen::_XWindow_HelpScreen()
{
	m_imageLeftTopEdge		= NULL;
	m_imageLeftDownEdge		= NULL;
	m_imageRightTopEdge		= NULL;
	m_imageRightDownEdge	= NULL;

	m_imageTitleLeftBar		= NULL;
	m_imageTitleCenterBar	= NULL;
	m_imageTitleRightBar	= NULL;
	
	m_imageArrow1			= NULL;
	m_imageArrow2			= NULL;
	m_imageArrow3			= NULL;
	m_imageArrow4			= NULL;
	m_imageArrow5			= NULL;
	m_imageArrowBody1		= NULL;
	
	m_imageMiniMap1			= NULL;
	m_imageMiniMap2			= NULL;
	m_imageMiniMap3			= NULL;
	m_imageMiniMap4			= NULL;
	m_imageMiniMap5			= NULL;

	for( int i = 0; i < _XDEF_TOTALHELPSCREENBOX; ++i )
	{
		m_stringBoxTitle[i] = NULL;
		m_stringBoxDescription[i] = NULL;
		m_nHelpDescriptionBoxHeight[i] = -1;
	}
	
}

_XWindow_HelpScreen::~_XWindow_HelpScreen()
{
	SAFE_DELETE( m_imageLeftTopEdge );
	SAFE_DELETE( m_imageLeftDownEdge );
	SAFE_DELETE( m_imageRightTopEdge );
	SAFE_DELETE( m_imageRightDownEdge );

	SAFE_DELETE( m_imageTitleLeftBar );
	SAFE_DELETE( m_imageTitleCenterBar );
	SAFE_DELETE( m_imageTitleRightBar );

	SAFE_DELETE( m_imageArrow1 );
	SAFE_DELETE( m_imageArrow2 );
	SAFE_DELETE( m_imageArrow3 );
	SAFE_DELETE( m_imageArrow4 );
	SAFE_DELETE( m_imageArrow5 );
	SAFE_DELETE( m_imageArrowBody1 );
	
	SAFE_DELETE( m_imageMiniMap1 );
	SAFE_DELETE( m_imageMiniMap2 );
	SAFE_DELETE( m_imageMiniMap3 );
	SAFE_DELETE( m_imageMiniMap4 );
	SAFE_DELETE( m_imageMiniMap5 );

//	for( int i = 0; i < _XDEF_TOTALHELPSCREENBOX; ++i )
//	{
//		SAFE_DELETE(m_stringBoxTitle[i]);
//		SAFE_DELETE(m_stringBoxDescription[i]);
//	}

}

BOOL _XWindow_HelpScreen::Initialize( void )
{	
	int resourceindex1 = -1;
	if( g_LanguageType == _XLANGUAGE_TYPE_ENGLISH )
	{
		resourceindex1 = g_MainInterfaceTextureArchive.FindResource( "us_tutorial_01.tga" );
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN )
	{
		resourceindex1 = g_MainInterfaceTextureArchive.FindResource( "rs_tutorial_01.tga" );
	}
	else
	{
		resourceindex1 = g_MainInterfaceTextureArchive.FindResource( "tutorial_01.tga" );
	}
	
	int resourceindex2 = g_MainInterfaceTextureArchive.FindResource("mi_npcquestwin_05.tga");
	int questresourceindex2 = g_MainInterfaceTextureArchive.FindResource( "MI_quest2.tga" );
	int charinfointerface = g_MainInterfaceTextureArchive.FindResource( g_MI_CharTextureName );
		
	// 노란색 박스 6개 - 1.상태창 2.미니맵 3.퀵슬롯창 4.운기요상,메신져,전서구창 5.행낭버튼창 6.인물정보버튼창 
	// 흰색 박스 6개 - 노란색 박스에 대한 설명창 
	
/////////////////////////////////////////////////////////////////////////////////////////////////
//										1.상태창 노란색 박스								   //
/////////////////////////////////////////////////////////////////////////////////////////////////
	// 박스 왼쪽 위 모서리 부분  ( 10 X 10 )
	_XImageStatic* pYellowBox1LeftUpEdge = new _XImageStatic;
	pYellowBox1LeftUpEdge->Create(0, 0, 10, 10, &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox1LeftUpEdge->SetClipRect(4, 132, 14, 142);
	InsertChildObject(pYellowBox1LeftUpEdge );
	
	// 박스 위쪽 라인 ( 216 X 7 )
	_XImageStatic* pYellowBox1UpLine = new _XImageStatic;
	pYellowBox1UpLine->Create(10, 0, 226, 7, &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox1UpLine->SetClipRect(201, 126, 202, 133);
	pYellowBox1UpLine->SetScale(216.0f, 1.0f );
	InsertChildObject(pYellowBox1UpLine );
	
	// 박스 오른쪽 위 모서리 부분 ( 13 X 10 )
	_XImageStatic* pYellowBox1RightUpEdge = new _XImageStatic;
	pYellowBox1RightUpEdge->Create(226, 0, 239, 10, &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox1RightUpEdge->SetClipRect(15, 132, 28, 142);
	InsertChildObject(pYellowBox1RightUpEdge );
	
	// 박스 오른쪽 라인 ( 10 X 68 )
	_XImageStatic* pYellowBox1RightLine = new _XImageStatic;
	pYellowBox1RightLine->Create(229, 10, 239, 78, &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox1RightLine->SetClipRect(197, 115, 207, 116);
	pYellowBox1RightLine->SetScale( 1.0f, 68.0f );
	InsertChildObject(pYellowBox1RightLine );
	
	// 박스 오른쪽 아래 모서리 ( 13 X 13 )
	_XImageStatic* pYellowBox1RightDownEdge = new _XImageStatic;
	pYellowBox1RightDownEdge->Create(226, 78, 239, 91, &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox1RightDownEdge->SetClipRect( 45, 132, 58, 145);
	InsertChildObject(pYellowBox1RightDownEdge );
	
	// 박스 아래쪽 라인 ( 216 X 10 )
	_XImageStatic* pYellowBox1DownLine = new _XImageStatic;
	pYellowBox1DownLine->Create(10, 81, 226, 91, &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox1DownLine->SetClipRect(201, 123, 202, 133);
	pYellowBox1DownLine->SetScale(216.0f, 1.0f );
	InsertChildObject(pYellowBox1DownLine );
	
	// 박스 왼쪽 아래 모서리 ( 10 X 13 ) 
	_XImageStatic* pYellowBox1LeftDownEdge = new _XImageStatic;
	pYellowBox1LeftDownEdge->Create(0, 78, 10, 91, &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox1LeftDownEdge->SetClipRect(34, 132, 44, 145);
	InsertChildObject(pYellowBox1LeftDownEdge );
	
	// 박스 왼쪽 라인 ( 7 X 68 )
	_XImageStatic* pYellowBox1LeftLine = new _XImageStatic;
	pYellowBox1LeftLine->Create(0, 10, 7, 78, &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox1LeftLine->SetClipRect(200, 115, 207, 116);
	pYellowBox1LeftLine->SetScale( 1.0f, 68.0f );
	InsertChildObject(pYellowBox1LeftLine );
	
/////////////////////////////////////////////////////////////////////////////////////////////////
//										2.미니맵 노란색 박스								   //
/////////////////////////////////////////////////////////////////////////////////////////////////
	// 박스 왼쪽 위 모서리 부분  ( 13 X 10 )
	_XImageStatic* pYellowBox2LeftUpEdge = new _XImageStatic;
	pYellowBox2LeftUpEdge->Create( gnWidth-(1024-862), 0, gnWidth-(1024-875), 10, &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox2LeftUpEdge->SetClipRect(1, 132, 14, 142);
	InsertChildObject(pYellowBox2LeftUpEdge );
	
	// 박스 위쪽 라인 ( 139 X 7 )
	_XImageStatic* pYellowBox2UpLine = new _XImageStatic;
	pYellowBox2UpLine->Create(gnWidth-(1024-875), 0, gnWidth-(1024-1014), 7, &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox2UpLine->SetClipRect(201, 126, 202, 133);
	pYellowBox2UpLine->SetScale(139.0f, 1.0f );
	InsertChildObject(pYellowBox2UpLine );
	
	// 박스 오른쪽 위 모서리 부분 ( 10 X 10 )
	_XImageStatic* pYellowBox2RightUpEdge = new _XImageStatic;
	pYellowBox2RightUpEdge->Create(gnWidth-(1024-1014), 0, gnWidth-(1024-1024), 10, &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox2RightUpEdge->SetClipRect(15, 132, 25, 142);
	InsertChildObject(pYellowBox2RightUpEdge );
	
	// 박스 오른쪽 라인 ( 7 X 195 )
	_XImageStatic* pYellowBox2RightLine = new _XImageStatic;
	pYellowBox2RightLine->Create(gnWidth-(1024-1017), 10, gnWidth-(1024-1024), 205, &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox2RightLine->SetClipRect(197, 115, 204, 116);
	pYellowBox2RightLine->SetScale( 1.0f, 195.0f );
	InsertChildObject(pYellowBox2RightLine );
	
	// 박스 오른쪽 아래 모서리 ( 10 X 13 )
	_XImageStatic* pYellowBox2RightDownEdge = new _XImageStatic;
	pYellowBox2RightDownEdge->Create(gnWidth-(1024-1014), 205, gnWidth-(1024-1024), 218, &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox2RightDownEdge->SetClipRect( 45, 132, 55, 145);
	InsertChildObject(pYellowBox2RightDownEdge );
	
	// 박스 아래쪽 라인 ( 139 X 10 )
	_XImageStatic* pYellowBox2DownLine = new _XImageStatic;
	pYellowBox2DownLine->Create( gnWidth-(1024-875), 208, gnWidth-(1024-1014), 218, &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox2DownLine->SetClipRect(201, 123, 202, 133);
	pYellowBox2DownLine->SetScale(139.0f, 1.0f );
	InsertChildObject(pYellowBox2DownLine );
	
	// 박스 왼쪽 아래 모서리 ( 13 X 13 ) 
	_XImageStatic* pYellowBox2LeftDownEdge = new _XImageStatic;
	pYellowBox2LeftDownEdge->Create(gnWidth-(1024-862), 205, gnWidth-(1024-875), 218, &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox2LeftDownEdge->SetClipRect(31, 132, 44, 145);
	InsertChildObject(pYellowBox2LeftDownEdge );
	
	// 박스 왼쪽 라인 ( 10 X 195 )
	_XImageStatic* pYellowBox2LeftLine = new _XImageStatic;
	pYellowBox2LeftLine->Create(gnWidth-(1024-862), 10, gnWidth-(1024-872), 205, &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox2LeftLine->SetClipRect(197, 115, 207, 116);
	pYellowBox2LeftLine->SetScale( 1.0f, 195.0f );
	InsertChildObject(pYellowBox2LeftLine );
	
	
/////////////////////////////////////////////////////////////////////////////////////////////////
//										3.퀵슬롯창 노란색 박스								   //
/////////////////////////////////////////////////////////////////////////////////////////////////
	// 박스 왼쪽 위 모서리 부분  ( 13 X 13 )
	_XImageStatic* pYellowBox3LeftUpEdge = new _XImageStatic;
	pYellowBox3LeftUpEdge->Create( gnWidth-(1024-375), gnHeight-(768-713), gnWidth-(1024-388), gnHeight-(768-726), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox3LeftUpEdge->SetClipRect(1, 129, 14, 142);
	InsertChildObject(pYellowBox3LeftUpEdge );
	
	// 박스 위쪽 라인 ( 445 X 10 )
	_XImageStatic* pYellowBox3UpLine = new _XImageStatic;
	pYellowBox3UpLine->Create( gnWidth-(1024-388), gnHeight-(768-713), gnWidth-(1024-833), gnHeight-(768-723), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox3UpLine->SetClipRect(201, 123, 202, 133);
	pYellowBox3UpLine->SetScale(445.0f, 1.0f );
	InsertChildObject(pYellowBox3UpLine );
	
	// 박스 오른쪽 위 모서리 부분 ( 13 X 13 )
	_XImageStatic* pYellowBox3RightUpEdge = new _XImageStatic;
	pYellowBox3RightUpEdge->Create( gnWidth-(1024-833), gnHeight-(768-713), gnWidth-(1024-846), gnHeight-(768-726), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox3RightUpEdge->SetClipRect(15, 129, 28, 142);
	InsertChildObject(pYellowBox3RightUpEdge );
	
	// 박스 오른쪽 라인 ( 10 X 32 )
	_XImageStatic* pYellowBox3RightLine = new _XImageStatic;
	pYellowBox3RightLine->Create(gnWidth-(1024-836), gnHeight-(768-726), gnWidth-(1024-846), gnHeight-(768-758), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox3RightLine->SetClipRect(197, 115, 207, 116);
	pYellowBox3RightLine->SetScale( 1.0f, 32.0f );
	InsertChildObject(pYellowBox3RightLine );
	
	// 박스 오른쪽 아래 모서리 ( 13 X 10 )
	_XImageStatic* pYellowBox3RightDownEdge = new _XImageStatic;
	pYellowBox3RightDownEdge->Create(gnWidth-(1024-833), gnHeight-(768-758), gnWidth-(1024-846), gnHeight-(768-768), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox3RightDownEdge->SetClipRect( 45, 132, 58, 142);
	InsertChildObject(pYellowBox3RightDownEdge );
	
	// 박스 아래쪽 라인 ( 445 X 7 )
	_XImageStatic* pYellowBox3DownLine = new _XImageStatic;
	pYellowBox3DownLine->Create( gnWidth-(1024-388), gnHeight-(768-761), gnWidth-(1024-833), gnHeight-(768-768), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox3DownLine->SetClipRect(201, 123, 202, 130);
	pYellowBox3DownLine->SetScale( 445.0f, 1.0f );
	InsertChildObject(pYellowBox3DownLine );
	
	// 박스 왼쪽 아래 모서리 ( 13 X 10 ) 
	_XImageStatic* pYellowBox3LeftDownEdge = new _XImageStatic;
	pYellowBox3LeftDownEdge->Create(gnWidth-(1024-375), gnHeight-(768-758), gnWidth-(1024-388), gnHeight-(768-768), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox3LeftDownEdge->SetClipRect(31, 132, 44, 142);
	InsertChildObject(pYellowBox3LeftDownEdge );
	
	// 박스 왼쪽 라인 ( 10 X 32 )
	_XImageStatic* pYellowBox3LeftLine = new _XImageStatic;
	pYellowBox3LeftLine->Create(gnWidth-(1024-375), gnHeight-(768-726), gnWidth-(1024-385), gnHeight-(768-758), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox3LeftLine->SetClipRect(197, 115, 207, 116);
	pYellowBox3LeftLine->SetScale( 1.0f, 32.0f );
	InsertChildObject(pYellowBox3LeftLine );
		
/////////////////////////////////////////////////////////////////////////////////////////////////
//							4.운기요상,메신져,전서구창 노란색 박스							   //
/////////////////////////////////////////////////////////////////////////////////////////////////
	// 박스 왼쪽 위 모서리 부분  ( 13 X 13 )
	_XImageStatic* pYellowBox4LeftUpEdge = new _XImageStatic;
	pYellowBox4LeftUpEdge->Create( gnWidth-(1024-843), gnHeight-(768-717), gnWidth-(1024-856), gnHeight-(768-730), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox4LeftUpEdge->SetClipRect(1, 129, 14, 142);
	InsertChildObject(pYellowBox4LeftUpEdge );
	
	// 박스 위쪽 라인 ( 50 X 10 )
	_XImageStatic* pYellowBox4UpLine = new _XImageStatic;
	pYellowBox4UpLine->Create( gnWidth-(1024-856), gnHeight-(768-717), gnWidth-(1024-906), gnHeight-(768-727), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox4UpLine->SetClipRect(201, 123, 202, 133);
	pYellowBox4UpLine->SetScale( 50.0f, 1.0f );
	InsertChildObject(pYellowBox4UpLine );
	
	// 박스 오른쪽 위 모서리 부분 ( 10 X 13 )
	_XImageStatic* pYellowBox4RightUpEdge = new _XImageStatic;
	pYellowBox4RightUpEdge->Create( gnWidth-(1024-906), gnHeight-(768-717), gnWidth-(1024-916), gnHeight-(768-730), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox4RightUpEdge->SetClipRect(15, 129, 25, 142);
	InsertChildObject(pYellowBox4RightUpEdge );
	
	// 박스 오른쪽 라인 ( 7 X 28 )
	_XImageStatic* pYellowBox4RightLine = new _XImageStatic;
	pYellowBox4RightLine->Create( gnWidth-(1024-909), gnHeight-(768-730), gnWidth-(1024-916), gnHeight-(768-758), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox4RightLine->SetClipRect(197, 115, 207, 116);
	pYellowBox4RightLine->SetScale( 1.0f, 28.0f );
	InsertChildObject(pYellowBox4RightLine );
	
	// 박스 오른쪽 아래 모서리 ( 10 X 10 )
	_XImageStatic* pYellowBox4RightDownEdge = new _XImageStatic;
	pYellowBox4RightDownEdge->Create(gnWidth-(1024-906), gnHeight-(768-758), gnWidth-(1024-916), gnHeight-(768-768), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox4RightDownEdge->SetClipRect( 45, 132, 55, 142);
	InsertChildObject(pYellowBox4RightDownEdge );
	
	// 박스 아래쪽 라인 ( 50 X 7 )
	_XImageStatic* pYellowBox4DownLine = new _XImageStatic;
	pYellowBox4DownLine->Create( gnWidth-(1024-856), gnHeight-(768-761), gnWidth-(1024-906), gnHeight-(768-768), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox4DownLine->SetClipRect(201, 123, 202, 130);
	pYellowBox4DownLine->SetScale( 50.0f, 1.0f );
	InsertChildObject(pYellowBox4DownLine );
	
	// 박스 왼쪽 아래 모서리 ( 13 X 10 ) 
	_XImageStatic* pYellowBox4LeftDownEdge = new _XImageStatic;
	pYellowBox4LeftDownEdge->Create(gnWidth-(1024-843), gnHeight-(768-758), gnWidth-(1024-856), gnHeight-(768-768), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox4LeftDownEdge->SetClipRect(31, 132, 44, 142);
	InsertChildObject(pYellowBox4LeftDownEdge );
	
	// 박스 왼쪽 라인 ( 10 X 28 )
	_XImageStatic* pYellowBox4LeftLine = new _XImageStatic;
	pYellowBox4LeftLine->Create(gnWidth-(1024-843), gnHeight-(768-730), gnWidth-(1024-853), gnHeight-(768-758), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox4LeftLine->SetClipRect(197, 115, 207, 116);
	pYellowBox4LeftLine->SetScale( 1.0f, 28.0f );
	InsertChildObject(pYellowBox4LeftLine );

#ifdef _XTS_ITEMMALLBROWSER	// 아이템몰
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//									5.행낭버튼창 노란색 박스								   //
	/////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	// 박스 위쪽 라인 ( 50 X 10 )
	_XImageStatic* pYellowBox5UpLine2 = new _XImageStatic;
//	pYellowBox5UpLine2->Create( gnWidth-(1024-876), gnHeight-(768-708+5), gnWidth-(1024-910), gnHeight-(768-718+5), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5UpLine2->Create( gnWidth-(1024-876), gnHeight-(768-708+5-10), gnWidth-(1024-910), gnHeight-(768-718+5), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5UpLine2->SetClipRect(201, 123, 202, 133);
	pYellowBox5UpLine2->SetScale( 35.0f, 1.0f );
	InsertChildObject(pYellowBox5UpLine2 );

	// 박스 왼쪽 라인 ( 7 X 20 )
	_XImageStatic* pYellowBox5LeftLine1 = new _XImageStatic;
//	pYellowBox5LeftLine1->Create(gnWidth-(1024-916-50), gnHeight-(768-721-22), gnWidth-(1024-923-50), gnHeight-(768-741-22), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5LeftLine1->Create(gnWidth-(1024-916-50+97), gnHeight-(768-721-22+45), gnWidth-(1024-923-50), gnHeight-(768-741-22), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5LeftLine1->SetClipRect(200, 115, 207, 116);
	pYellowBox5LeftLine1->SetScale( 1.0f, 20.0f );
	InsertChildObject(pYellowBox5LeftLine1 );

	// 박스 왼쪽 아래 모서리 ( 10 X 10 ) 
	_XImageStatic* pYellowBox5LeftDownEdge1 = new _XImageStatic;
//	pYellowBox5LeftDownEdge1->Create(gnWidth-(1024-916-50), gnHeight-(768-732-22), gnWidth-(1024-926-50), gnHeight-(768-742-22), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5LeftDownEdge1->Create(gnWidth-(1024-916-50+97), gnHeight-(768-732-22+44), gnWidth-(1024-926-50), gnHeight-(768-742-22), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5LeftDownEdge1->SetClipRect( 34, 132, 44, 142);
	InsertChildObject(pYellowBox5LeftDownEdge1 );
	
	// 박스 왼쪽 위 모서리 부분  ( 13 X 13 )
	_XImageStatic* pYellowBox5LeftUpEdge = new _XImageStatic;
//	pYellowBox5LeftUpEdge->Create( gnWidth-(1024-913-50), gnHeight-(768-708-22), gnWidth-(1024-926-50), gnHeight-(768-721-22), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5LeftUpEdge->Create( gnWidth-(1024-913-50+97), gnHeight-(768-708-22+42), gnWidth-(1024-926-50), gnHeight-(768-721-22), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5LeftUpEdge->SetClipRect(1, 129, 14, 142);
	InsertChildObject(pYellowBox5LeftUpEdge );
	
	// 박스 위쪽 라인 ( (28+50) X 10 )
	_XImageStatic* pYellowBox5UpLine = new _XImageStatic;
//	pYellowBox5UpLine->Create( gnWidth-(1024-926-50), gnHeight-(768-708-22), gnWidth-(1024-954), gnHeight-(768-718-22), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5UpLine->Create( gnWidth-(1024-926-50+100), gnHeight-(768-708-22+42), gnWidth-(1024-954), gnHeight-(768-718-22), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5UpLine->SetClipRect(201, 123, 202, 133);
	pYellowBox5UpLine->SetScale( 78.0f, 1.0f );
	InsertChildObject(pYellowBox5UpLine );
	
	// 박스 오른쪽 위 모서리 부분 ( 10 X 13 )
	_XImageStatic* pYellowBox5RightUpEdge = new _XImageStatic;
//	pYellowBox5RightUpEdge->Create( gnWidth-(1024-953), gnHeight-(768-708-22), gnWidth-(1024-963), gnHeight-(768-721-22), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5RightUpEdge->Create( gnWidth-(1024-953), gnHeight-(768-708-22+42), gnWidth-(1024-963), gnHeight-(768-721-22), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5RightUpEdge->SetClipRect(15, 129, 25, 142);
	InsertChildObject(pYellowBox5RightUpEdge );
	
	// 박스 오른쪽 라인 ( 7 X 37 )
	_XImageStatic* pYellowBox5RightLine = new _XImageStatic;
//	pYellowBox5RightLine->Create( gnWidth-(1024-956), gnHeight-(768-721-22), gnWidth-(1024-963), gnHeight-(768-758), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5RightLine->Create( gnWidth-(1024-956), gnHeight-(768-721-22+42), gnWidth-(1024-963), gnHeight-(768-758), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5RightLine->SetClipRect(197, 115, 204, 116);
	pYellowBox5RightLine->SetScale( 1.0f, 59.0f );
	InsertChildObject(pYellowBox5RightLine );
	
	// 박스 오른쪽 아래 모서리 ( 10 X 10 )
	_XImageStatic* pYellowBox5RightDownEdge = new _XImageStatic;
	pYellowBox5RightDownEdge->Create(gnWidth-(1024-953), gnHeight-(768-758), gnWidth-(1024-963), gnHeight-(768-768), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5RightDownEdge->SetClipRect( 45, 132, 55, 142);
	InsertChildObject(pYellowBox5RightDownEdge );
	
	// 박스 아래쪽 라인 ( 28 X 7 )
	_XImageStatic* pYellowBox5DownLine = new _XImageStatic;
	pYellowBox5DownLine->Create( gnWidth-(1024-926), gnHeight-(768-761), gnWidth-(1024-954), gnHeight-(768-768), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5DownLine->SetClipRect(201, 123, 202, 130);
	pYellowBox5DownLine->SetScale( 28.0f, 1.0f );
	InsertChildObject(pYellowBox5DownLine );
	
	// 박스 왼쪽 아래 모서리 ( 10 X 10 ) 
	_XImageStatic* pYellowBox5LeftDownEdge = new _XImageStatic;
	pYellowBox5LeftDownEdge->Create(gnWidth-(1024-916), gnHeight-(768-758), gnWidth-(1024-926), gnHeight-(768-768), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5LeftDownEdge->SetClipRect( 34, 132, 44, 142);
	InsertChildObject(pYellowBox5LeftDownEdge );
	
	// 박스 왼쪽 라인 ( 7 X 37 )
	_XImageStatic* pYellowBox5LeftLine = new _XImageStatic;
	pYellowBox5LeftLine->Create(gnWidth-(1024-916), gnHeight-(768-721), gnWidth-(1024-923), gnHeight-(768-758), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5LeftLine->SetClipRect(200, 115, 207, 116);
	pYellowBox5LeftLine->SetScale( 1.0f, 37.0f );
	InsertChildObject(pYellowBox5LeftLine );
	
	// 박스 오른쪽 위 모서리 부분 ( 10 X 13 )
	_XImageStatic* pYellowBox5RightUpEdge1 = new _XImageStatic;
	pYellowBox5RightUpEdge1->Create( gnWidth-(1024-910), gnHeight-(768-713), gnWidth-(1024-920), gnHeight-(768-726), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5RightUpEdge1->SetClipRect(15, 129, 25, 142);
	InsertChildObject(pYellowBox5RightUpEdge1 );
	
#else
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//									5.행낭버튼창 노란색 박스								   //
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// 박스 왼쪽 위 모서리 부분  ( 13 X 13 )
	_XImageStatic* pYellowBox5LeftUpEdge = new _XImageStatic;
	pYellowBox5LeftUpEdge->Create( gnWidth-(1024-913), gnHeight-(768-708), gnWidth-(1024-926), gnHeight-(768-721), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5LeftUpEdge->SetClipRect(1, 129, 14, 142);
	InsertChildObject(pYellowBox5LeftUpEdge );
	
	// 박스 위쪽 라인 ( 28 X 10 )
	_XImageStatic* pYellowBox5UpLine = new _XImageStatic;
	pYellowBox5UpLine->Create( gnWidth-(1024-926), gnHeight-(768-708), gnWidth-(1024-954), gnHeight-(768-718), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5UpLine->SetClipRect(201, 123, 202, 133);
	pYellowBox5UpLine->SetScale( 28.0f, 1.0f );
	InsertChildObject(pYellowBox5UpLine );
	
	// 박스 오른쪽 위 모서리 부분 ( 10 X 13 )
	_XImageStatic* pYellowBox5RightUpEdge = new _XImageStatic;
	pYellowBox5RightUpEdge->Create( gnWidth-(1024-953), gnHeight-(768-708), gnWidth-(1024-963), gnHeight-(768-721), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5RightUpEdge->SetClipRect(15, 129, 25, 142);
	InsertChildObject(pYellowBox5RightUpEdge );
	
	// 박스 오른쪽 라인 ( 7 X 37 )
	_XImageStatic* pYellowBox5RightLine = new _XImageStatic;
	pYellowBox5RightLine->Create( gnWidth-(1024-956), gnHeight-(768-721), gnWidth-(1024-963), gnHeight-(768-758), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5RightLine->SetClipRect(197, 115, 204, 116);
	pYellowBox5RightLine->SetScale( 1.0f, 37.0f );
	InsertChildObject(pYellowBox5RightLine );
	
	// 박스 오른쪽 아래 모서리 ( 10 X 10 )
	_XImageStatic* pYellowBox5RightDownEdge = new _XImageStatic;
	pYellowBox5RightDownEdge->Create(gnWidth-(1024-953), gnHeight-(768-758), gnWidth-(1024-963), gnHeight-(768-768), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5RightDownEdge->SetClipRect( 45, 132, 55, 142);
	InsertChildObject(pYellowBox5RightDownEdge );
	
	// 박스 아래쪽 라인 ( 28 X 7 )
	_XImageStatic* pYellowBox5DownLine = new _XImageStatic;
	pYellowBox5DownLine->Create( gnWidth-(1024-926), gnHeight-(768-761), gnWidth-(1024-954), gnHeight-(768-768), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5DownLine->SetClipRect(201, 123, 202, 130);
	pYellowBox5DownLine->SetScale( 28.0f, 1.0f );
	InsertChildObject(pYellowBox5DownLine );
	
	// 박스 왼쪽 아래 모서리 ( 10 X 10 ) 
	_XImageStatic* pYellowBox5LeftDownEdge = new _XImageStatic;
	pYellowBox5LeftDownEdge->Create(gnWidth-(1024-916), gnHeight-(768-758), gnWidth-(1024-926), gnHeight-(768-768), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5LeftDownEdge->SetClipRect( 34, 132, 44, 142);
	InsertChildObject(pYellowBox5LeftDownEdge );
	
	// 박스 왼쪽 라인 ( 7 X 37 )
	_XImageStatic* pYellowBox5LeftLine = new _XImageStatic;
	pYellowBox5LeftLine->Create(gnWidth-(1024-916), gnHeight-(768-721), gnWidth-(1024-923), gnHeight-(768-758), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox5LeftLine->SetClipRect(200, 115, 207, 116);
	pYellowBox5LeftLine->SetScale( 1.0f, 37.0f );
	InsertChildObject(pYellowBox5LeftLine );
#endif
	
	
/////////////////////////////////////////////////////////////////////////////////////////////////
//									6.인물정보버튼창 노란색 박스							   //
/////////////////////////////////////////////////////////////////////////////////////////////////
	// 박스 왼쪽 위 모서리 부분  ( 13 X 13 )
	_XImageStatic* pYellowBox6LeftUpEdge = new _XImageStatic;
	pYellowBox6LeftUpEdge->Create( gnWidth-(1024-959), gnHeight-(768-694), gnWidth-(1024-972), gnHeight-(768-707), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox6LeftUpEdge->SetClipRect(1, 129, 14, 142);
	InsertChildObject(pYellowBox6LeftUpEdge );
	
	// 박스 위쪽 라인 ( 37 X 10 )
	_XImageStatic* pYellowBox6UpLine = new _XImageStatic;
	pYellowBox6UpLine->Create( gnWidth-(1024-972), gnHeight-(768-694), gnWidth-(1024-1009), gnHeight-(768-704), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox6UpLine->SetClipRect(201, 123, 202, 133);
	pYellowBox6UpLine->SetScale( 37.0f, 1.0f );
	InsertChildObject(pYellowBox6UpLine );
	
	// 박스 오른쪽 위 모서리 부분 ( 13 X 13 )
	_XImageStatic* pYellowBox6RightUpEdge = new _XImageStatic;
	pYellowBox6RightUpEdge->Create( gnWidth-(1024-1009), gnHeight-(768-694), gnWidth-(1024-1022), gnHeight-(768-707), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox6RightUpEdge->SetClipRect(15, 129, 28, 142);
	InsertChildObject(pYellowBox6RightUpEdge );
	
	// 박스 오른쪽 라인 ( 10 X 42 )
	_XImageStatic* pYellowBox6RightLine = new _XImageStatic;
	pYellowBox6RightLine->Create( gnWidth-(1024-1012), gnHeight-(768-707), gnWidth-(1024-1022), gnHeight-(768-749), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox6RightLine->SetClipRect(197, 115, 207, 116);
	pYellowBox6RightLine->SetScale( 1.0f, 42.0f );
	InsertChildObject(pYellowBox6RightLine );
	
	// 박스 오른쪽 아래 모서리 ( 13 X 13 )
	_XImageStatic* pYellowBox6RightDownEdge = new _XImageStatic;
	pYellowBox6RightDownEdge->Create( gnWidth-(1024-1009), gnHeight-(768-749), gnWidth-(1024-1022), gnHeight-(768-762), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox6RightDownEdge->SetClipRect( 45, 132, 58, 145);
	InsertChildObject(pYellowBox6RightDownEdge );
	
	// 박스 아래쪽 라인 ( 37 X 10 )
	_XImageStatic* pYellowBox6DownLine = new _XImageStatic;
	pYellowBox6DownLine->Create( gnWidth-(1024-972), gnHeight-(768-752), gnWidth-(1024-1009), gnHeight-(768-762), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox6DownLine->SetClipRect(201, 123, 202, 133);
	pYellowBox6DownLine->SetScale( 37.0f, 1.0f );
	InsertChildObject(pYellowBox6DownLine );
	
	// 박스 왼쪽 아래 모서리 ( 10 X 13 ) 
	_XImageStatic* pYellowBox6LeftDownEdge = new _XImageStatic;
	pYellowBox6LeftDownEdge->Create(gnWidth-(1024-962), gnHeight-(768-749), gnWidth-(1024-972), gnHeight-(768-762), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox6LeftDownEdge->SetClipRect( 34, 132, 44, 145);
	InsertChildObject(pYellowBox6LeftDownEdge );
	
	// 박스 왼쪽 라인 ( 7 X 42 )
	_XImageStatic* pYellowBox6LeftLine = new _XImageStatic;
	pYellowBox6LeftLine->Create(gnWidth-(1024-962), gnHeight-(768-707), gnWidth-(1024-972), gnHeight-(768-749), &g_MainInterfaceTextureArchive, resourceindex1);
	pYellowBox6LeftLine->SetClipRect(200, 115, 207, 116);
	pYellowBox6LeftLine->SetScale( 1.0f, 42.0f );
	InsertChildObject(pYellowBox6LeftLine );
	


	// 박스 왼쪽 위 모서리 부분  ( 9 X 9 )
	m_imageLeftTopEdge = new _XImageStatic;
	m_imageLeftTopEdge->Create(0, 0, 9, 9, &g_MainInterfaceTextureArchive, resourceindex1);
	m_imageLeftTopEdge->SetClipRect( 58, 105, 67, 114);
	
	// 박스 왼쪽 아래 모서리 부분  ( 9 X 9 )
	m_imageLeftDownEdge = new _XImageStatic;
	m_imageLeftDownEdge->Create(0, 0, 9, 9, &g_MainInterfaceTextureArchive, resourceindex1);
	m_imageLeftDownEdge->SetClipRect( 80, 104, 89, 113);
	
	// 박스 오른쪽 위 모서리 부분  ( 9 X 9 )
	m_imageRightTopEdge = new _XImageStatic;
	m_imageRightTopEdge->Create(0, 0, 9, 9, &g_MainInterfaceTextureArchive, resourceindex1);
	m_imageRightTopEdge->SetClipRect( 68, 105, 77, 114);
	
	// 박스 오른쪽 아래 모서리 부분  ( 9 X 9 )
	m_imageRightDownEdge = new _XImageStatic;
	m_imageRightDownEdge->Create(0, 0, 9, 9, &g_MainInterfaceTextureArchive, resourceindex1);
	m_imageRightDownEdge->SetClipRect( 90, 104, 99, 113);
	
	// 제목 바탕 왼쪽 부분  ( 5 X 26 )
	m_imageTitleLeftBar = new _XImageStatic;
	m_imageTitleLeftBar->Create(0, 0, 5, 26, &g_MainInterfaceTextureArchive, questresourceindex2);
	m_imageTitleLeftBar->SetClipRect( 1, 229, 6, 255);
	
	// 제목 바탕 가운데 부분  ( 56 X 26 )
	m_imageTitleCenterBar = new _XImageStatic;
	m_imageTitleCenterBar->Create(0, 0, 56, 26, &g_MainInterfaceTextureArchive, questresourceindex2);
	m_imageTitleCenterBar->SetClipRect( 6, 229, 62, 255);
	
	// 제목 바탕 오른쪽 부분  ( 5 X 26 )
	m_imageTitleRightBar = new _XImageStatic;
	m_imageTitleRightBar->Create(0, 0, 5, 26, &g_MainInterfaceTextureArchive, questresourceindex2);
	m_imageTitleRightBar->SetClipRect( 62, 229, 67, 255);
	
	// 화살표1 ( 27 X 32 )
	m_imageArrow1 = new _XImageStatic;
	m_imageArrow1->Create( 0, 0, 27, 32, &g_MainInterfaceTextureArchive, resourceindex2);
	m_imageArrow1->SetClipRect(446, 7, 473, 39);
	
	// 화살표2 ( 29 X 29 )
	m_imageArrow2 = new _XImageStatic;
	m_imageArrow2->Create( 0, 0, 29, 29, &g_MainInterfaceTextureArchive, resourceindex2);
	m_imageArrow2->SetClipRect(457, 49, 486, 78);
	
	// 화살표3 ( 43 X 29 )
	m_imageArrow3 = new _XImageStatic;
	m_imageArrow3->Create( 0, 0, 43, 29, &g_MainInterfaceTextureArchive, resourceindex2);
	m_imageArrow3->SetClipRect(443, 49, 486, 78);
	
	// 화살표4 ( 47 X 43 )
	m_imageArrow4 = new _XImageStatic;
	m_imageArrow4->Create( 0, 0, 47, 43, &g_MainInterfaceTextureArchive, resourceindex2);
	m_imageArrow4->SetClipRect(441, 88, 488, 131);
	
	// 화살표5 ( 29 X 41 )
	m_imageArrow5 = new _XImageStatic;
	m_imageArrow5->Create( 0, 0, 29, 41, &g_MainInterfaceTextureArchive, resourceindex2);
	m_imageArrow5->SetClipRect( 449, 149, 478, 190);
		
	// 화살표몸체 1 ( 20 X 13 )
	m_imageArrowBody1 = new _XImageStatic;
	m_imageArrowBody1->Create( 0, 0, 20, 13, &g_MainInterfaceTextureArchive, resourceindex2);
	m_imageArrowBody1->SetClipRect( 441, 88, 461, 101);
	
	//미니맵 설명 아이콘 1
	m_imageMiniMap1 = new _XImageStatic;
	m_imageMiniMap1->Create( 0, 0, 20, 13, &g_MainInterfaceTextureArchive, charinfointerface);
	m_imageMiniMap1->SetClipRect( 184, 233, 207, 256);
	
	//미니맵 설명 아이콘 2
	m_imageMiniMap2 = new _XImageStatic;
	m_imageMiniMap2->Create( 0, 0, 20, 13, &g_MainInterfaceTextureArchive, charinfointerface);
	m_imageMiniMap2->SetClipRect( 183, 194, 192, 203);
	
	//미니맵 설명 아이콘 3
	m_imageMiniMap3 = new _XImageStatic;
	m_imageMiniMap3->Create( 0, 0, 20, 13, &g_MainInterfaceTextureArchive, charinfointerface);
	m_imageMiniMap3->SetClipRect( 216, 194, 226, 204);
	
	//미니맵 설명 아이콘 4
	m_imageMiniMap4 = new _XImageStatic;
	m_imageMiniMap4->Create( 0, 0, 20, 13, &g_MainInterfaceTextureArchive, charinfointerface);
	m_imageMiniMap4->SetClipRect( 195, 213, 214, 232);
	
	//미니맵 설명 아이콘 5
	m_imageMiniMap5 = new _XImageStatic;
	m_imageMiniMap5->Create( 0, 0, 20, 13, &g_MainInterfaceTextureArchive, resourceindex1);
	m_imageMiniMap5->SetClipRect( 220, 3, 249, 25);
	

	return TRUE;
}

void _XWindow_HelpScreen::Draw( _XGUIObject*& pfocusobject )
{
	if( m_ShowWindow )
	{
		_XDrawSolidBar( 3, 3, 233, 85, D3DCOLOR_ARGB(128,213,213,163) );
		_XDrawSolidBar( gnWidth-(1024-868), 3, gnWidth-(1024-1021), 212, D3DCOLOR_ARGB(128,213,213,163) );
		_XDrawSolidBar( gnWidth-(1024-381), gnHeight-(768-719), gnWidth-(1024-840), gnHeight-(768-765), D3DCOLOR_ARGB(128,213,213,163) );
		_XDrawSolidBar( gnWidth-(1024-849), gnHeight-(768-723), gnWidth-(1024-913), gnHeight-(768-765), D3DCOLOR_ARGB(128,213,213,163) );
#ifdef _XTS_ITEMMALLBROWSER	// 아이템몰
		_XDrawSolidBar( gnWidth-(1024-869), gnHeight-(768-692), gnWidth-(1024-960), gnHeight-(768-717), D3DCOLOR_ARGB(128,213,213,163) );
		_XDrawSolidBar( gnWidth-(1024-919), gnHeight-(768-717), gnWidth-(1024-960), gnHeight-(768-765), D3DCOLOR_ARGB(128,213,213,163) );
#else
		_XDrawSolidBar( gnWidth-(1024-919), gnHeight-(768-714), gnWidth-(1024-960), gnHeight-(768-765), D3DCOLOR_ARGB(128,213,213,163) );
#endif
		_XDrawSolidBar( gnWidth-(1024-965), gnHeight-(768-700), gnWidth-(1024-1016), gnHeight-(768-756), D3DCOLOR_ARGB(128,213,213,163) );
	}

	_XWindow::Draw(pfocusobject);	
	
	if( !this->m_ShowWindow ) return;

	DrawAllBox();	
}


void _XWindow_HelpScreen::DrawHelpDescriptionBox( int left, int top, int right, int bottom )
{
	
	_XDrawSolidBar( left+3, top+3, right-2, bottom-2, D3DCOLOR_ARGB(200,0,0,0) );

	m_imageLeftTopEdge->Draw( left-2, top-2 );
	m_imageLeftDownEdge->Draw( left-2, bottom-6 );
	m_imageRightTopEdge->Draw( right-6, top-2 );
	m_imageRightDownEdge->Draw( right-6, bottom-6 );

	// 위쪽 라인
	_XDrawLine2D( left+7, top-1, right-6, top-1, D3DCOLOR_ARGB(255,0,0,0), D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawLine2D( left+7, top  , right-6, top  , D3DCOLOR_ARGB(255,199,199,199), D3DCOLOR_ARGB(255,199,199,199) );
	_XDrawLine2D( left+7, top+1, right-6, top+1, D3DCOLOR_ARGB(255,199,199,199), D3DCOLOR_ARGB(255,199,199,199) );
	_XDrawLine2D( left+7, top+2, right-6, top+2, D3DCOLOR_ARGB(255,0,0,0), D3DCOLOR_ARGB(255,0,0,0) );
	// 아래쪽 라인
	_XDrawLine2D( left+7, bottom+1, right-6, bottom+1, D3DCOLOR_ARGB(255,0,0,0), D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawLine2D( left+7, bottom  , right-6, bottom  , D3DCOLOR_ARGB(255,199,199,199), D3DCOLOR_ARGB(255,199,199,199) );
	_XDrawLine2D( left+7, bottom-1, right-6, bottom-1, D3DCOLOR_ARGB(255,199,199,199), D3DCOLOR_ARGB(255,199,199,199) );
	_XDrawLine2D( left+7, bottom-2, right-6, bottom-2, D3DCOLOR_ARGB(255,0,0,0), D3DCOLOR_ARGB(255,0,0,0) );
	// 왼쪽 라인
	_XDrawLine2D( left-1, top+7, left-1, bottom-6, D3DCOLOR_ARGB(255,0,0,0), D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawLine2D( left  , top+7, left  , bottom-6, D3DCOLOR_ARGB(255,199,199,199), D3DCOLOR_ARGB(255,199,199,199) );
	_XDrawLine2D( left+1, top+7, left+1, bottom-6, D3DCOLOR_ARGB(255,199,199,199), D3DCOLOR_ARGB(255,199,199,199) );
	_XDrawLine2D( left+2, top+7, left+2, bottom-6, D3DCOLOR_ARGB(255,0,0,0), D3DCOLOR_ARGB(255,0,0,0) );
	// 오른쪽 라인
	_XDrawLine2D( right+1, top+7, right+1, bottom-6, D3DCOLOR_ARGB(255,0,0,0), D3DCOLOR_ARGB(255,0,0,0) );
	_XDrawLine2D( right  , top+7, right  , bottom-6, D3DCOLOR_ARGB(255,199,199,199), D3DCOLOR_ARGB(255,199,199,199) );
	_XDrawLine2D( right-1, top+7, right-1, bottom-6, D3DCOLOR_ARGB(255,199,199,199), D3DCOLOR_ARGB(255,199,199,199) );
	_XDrawLine2D( right-2, top+7, right-2, bottom-6, D3DCOLOR_ARGB(255,0,0,0), D3DCOLOR_ARGB(255,0,0,0) );
	
}

void _XWindow_HelpScreen::DrawAllBox( void )
{
	// 상태창
	if( m_stringBoxTitle[_XHS_HELPBOX_STATUS] &&  m_stringBoxDescription[_XHS_HELPBOX_STATUS] )
	{
		// 바탕 박스
		DrawHelpDescriptionBox( 3, 118, 3+248, 118+m_nHelpDescriptionBoxHeight[0] );

		//화살표
		m_imageArrow1->Draw( 190, 86 );

		// 제목 바탕
		m_imageTitleLeftBar->Draw( 3+2, 118+8 );
		m_imageTitleCenterBar->SetScale( 4.2142f, 1.0f);
		m_imageTitleCenterBar->Draw( 3+2+5, 118+8 );
		m_imageTitleRightBar->Draw( 3+2+5+236, 118+8 );
		
		// 제목
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 	
		g_XBaseFont->Puts( 3+2+10, 118+8+ 7, m_stringBoxTitle[_XHS_HELPBOX_STATUS]);
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);

		// 설명
		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Puts_SeparateNewLineVersion( 3+9, 118+8+26+5, m_stringBoxDescription[_XHS_HELPBOX_STATUS], 230, 1.0, 3 );
		g_XBaseFont->Flush();
		
	}
	
	// 미니맵
	if( m_stringBoxTitle[_XHS_HELPBOX_MINIMAP] &&  m_stringBoxDescription[_XHS_HELPBOX_MINIMAP] )
	{
		// 바탕 박스
		DrawHelpDescriptionBox( gnWidth-(1024-591), 4, gnWidth-(1024-(591+248)), 4+m_nHelpDescriptionBoxHeight[1]+105 );
		
		//화살표
		m_imageArrow2->Draw( gnWidth-(1024-840), 32 );

		//미니맵 버튼 설명
		m_imageMiniMap1->Draw( gnWidth-(1024-(591+9)), 4+m_nHelpDescriptionBoxHeight[1] );
		m_imageMiniMap2->Draw( gnWidth-(1024-(591+9)), 4+m_nHelpDescriptionBoxHeight[1]+30 );
		m_imageMiniMap3->Draw( gnWidth-(1024-(591+19)), 4+m_nHelpDescriptionBoxHeight[1]+30 );
		m_imageMiniMap4->Draw( gnWidth-(1024-(591+9)), 4+m_nHelpDescriptionBoxHeight[1]+50 );
		m_imageMiniMap5->Draw( gnWidth-(1024-(591+9)), 4+m_nHelpDescriptionBoxHeight[1]+75 );
		
		// 제목 바탕
		m_imageTitleLeftBar->Draw( gnWidth-(1024-(591+2)), 4+8 );
		m_imageTitleCenterBar->SetScale( 4.2142f, 1.0f); // (248-12)/56
		m_imageTitleCenterBar->Draw( gnWidth-(1024-(591+2+5)), 4+8 );
		m_imageTitleRightBar->Draw( gnWidth-(1024-(591+2+5+236)), 4+8 );
		
		// 제목
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 	
		g_XBaseFont->Puts( gnWidth-(1024-(591+2+10)), 4+8+7, m_stringBoxTitle[_XHS_HELPBOX_MINIMAP]);
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
		
		// 설명
		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Puts_SeparateNewLineVersion( gnWidth-(1024-(591+9)), 4+8+26+5, m_stringBoxDescription[_XHS_HELPBOX_MINIMAP], 230, 1.0, 3 );
		g_XBaseFont->Puts( gnWidth-(1024-(591+50)), 4+m_nHelpDescriptionBoxHeight[1]+3, _XGETINTERFACETEXT(ID_STRING_NEW_2680));
		g_XBaseFont->Puts( gnWidth-(1024-(591+50)), 4+m_nHelpDescriptionBoxHeight[1]+28, _XGETINTERFACETEXT(ID_STRING_NEW_2681));
		g_XBaseFont->Puts( gnWidth-(1024-(591+50)), 4+m_nHelpDescriptionBoxHeight[1]+53, _XGETINTERFACETEXT(ID_STRING_NEW_2682));
		g_XBaseFont->Puts( gnWidth-(1024-(591+50)), 4+m_nHelpDescriptionBoxHeight[1]+78, _XGETINTERFACETEXT(ID_STRING_NEW_2683));
		g_XBaseFont->Flush();
	}

	// 퀵슬롯
	if( m_stringBoxTitle[_XHS_HELPBOX_QUICKSLOT] &&  m_stringBoxDescription[_XHS_HELPBOX_QUICKSLOT] )
	{
		// 바탕 박스
		DrawHelpDescriptionBox( gnWidth-(1024-98), gnHeight-(768-(763-m_nHelpDescriptionBoxHeight[2])), gnWidth-(1024-(98+242)), gnHeight-(768-(763)) );	
		
		//화살표
		m_imageArrow3->Draw( gnWidth-(1024-340), gnHeight-(768-(720)) );

		// 제목 바탕
		m_imageTitleLeftBar->Draw( gnWidth-(1024-(98+2)), gnHeight-(768-(763-m_nHelpDescriptionBoxHeight[2]+8)) );
		m_imageTitleCenterBar->SetScale( 4.1071f, 1.0f); // (242-12)/56
		m_imageTitleCenterBar->Draw( gnWidth-(1024-(98+2+5)), gnHeight-(768-(763-m_nHelpDescriptionBoxHeight[2]+8)) );
		m_imageTitleRightBar->Draw( gnWidth-(1024-(98+2+5+230)), gnHeight-(768-(763-m_nHelpDescriptionBoxHeight[2]+8)) );
		
		// 제목
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 	
		g_XBaseFont->Puts( gnWidth-(1024-(98+2+10)), gnHeight-(768-(763-m_nHelpDescriptionBoxHeight[2]+8+7)), m_stringBoxTitle[_XHS_HELPBOX_QUICKSLOT]);
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
		
		// 설명
		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Puts_SeparateNewLineVersion( gnWidth-(1024-(98+9)), gnHeight-(768-(763-m_nHelpDescriptionBoxHeight[2]+8+26+5)), m_stringBoxDescription[_XHS_HELPBOX_QUICKSLOT], 224, 1.0, 3 );
		g_XBaseFont->Flush();
	}

	// 운기요상, 메신져, 전서구 
	// 바탕 박스
	int tempheight = gnHeight-(70 + m_nHelpDescriptionBoxHeight[3] + m_nHelpDescriptionBoxHeight[4] + m_nHelpDescriptionBoxHeight[5]);
	DrawHelpDescriptionBox( gnWidth-(1024-371), tempheight, gnWidth-(1024-(371+214)), gnHeight-70 );
	
	//화살표
	m_imageArrow4->Draw( gnWidth-(1024-831), gnHeight-(768-(682)) );
	m_imageArrowBody1->SetScale( 12.4f, 1.0f);
	m_imageArrowBody1->Draw( gnWidth-(1024-583), gnHeight-(768-(682)) );

	//운기요상
	if( m_stringBoxTitle[_XHS_HELPBOX_MEDETATIOH] &&  m_stringBoxDescription[_XHS_HELPBOX_MEDETATIOH] )
	{		
		// 제목 바탕
		m_imageTitleLeftBar->Draw( gnWidth-(1024-(371+2)), tempheight+8 );
		m_imageTitleCenterBar->SetScale( 3.6071f, 1.0f); // (214-12)/56
		m_imageTitleCenterBar->Draw( gnWidth-(1024-(371+2+5)), tempheight+8 );
		m_imageTitleRightBar->Draw( gnWidth-(1024-(371+2+5+202)), tempheight+8 );
		
		// 제목
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 	
		g_XBaseFont->Puts( gnWidth-(1024-(371+2+10)), tempheight+8+7, m_stringBoxTitle[_XHS_HELPBOX_MEDETATIOH]);
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
		
		// 설명
		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Puts_SeparateNewLineVersion( gnWidth-(1024-(371+9)), tempheight+8+26+5, m_stringBoxDescription[_XHS_HELPBOX_MEDETATIOH], 196, 1.0, 3 );
		g_XBaseFont->Flush();
		
	}
	//메신져
	if( m_stringBoxTitle[_XHS_HELPBOX_MESSENGER] &&  m_stringBoxDescription[_XHS_HELPBOX_MESSENGER] )
	{		
		tempheight = gnHeight-(768-(698 - m_nHelpDescriptionBoxHeight[4] - m_nHelpDescriptionBoxHeight[5]));
		// 제목 바탕
		m_imageTitleLeftBar->Draw( gnWidth-(1024-(371+2)), tempheight+8 );
		m_imageTitleCenterBar->SetScale( 3.6071f, 1.0f); // (214-12)/56
		m_imageTitleCenterBar->Draw( gnWidth-(1024-(371+2+5)), tempheight+8 );
		m_imageTitleRightBar->Draw( gnWidth-(1024-(371+2+5+202)), tempheight+8 );
		
		// 제목
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 	
		g_XBaseFont->Puts( gnWidth-(1024-(371+2+10)), tempheight+8+7, m_stringBoxTitle[_XHS_HELPBOX_MESSENGER]);
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
		
		// 설명
		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Puts_SeparateNewLineVersion( gnWidth-(1024-(371+9)), tempheight+8+26+5, m_stringBoxDescription[_XHS_HELPBOX_MESSENGER], 196, 1.0, 3 );
		g_XBaseFont->Flush();
		
	}
	//전서구
	if( m_stringBoxTitle[_XHS_HELPBOX_MAIL] &&  m_stringBoxDescription[_XHS_HELPBOX_MAIL] )
	{		
		tempheight = gnHeight-(768-(698 - m_nHelpDescriptionBoxHeight[5]));
		// 제목 바탕
		m_imageTitleLeftBar->Draw( gnWidth-(1024-(371+2)), tempheight+8 );
		m_imageTitleCenterBar->SetScale( 3.6071f, 1.0f); // (214-12)/56
		m_imageTitleCenterBar->Draw( gnWidth-(1024-(371+2+5)), tempheight+8 );
		m_imageTitleRightBar->Draw( gnWidth-(1024-(371+2+5+202)), tempheight+8 );
		
		// 제목
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 	
		g_XBaseFont->Puts( gnWidth-(1024-(371+2+10)), tempheight+8+7, m_stringBoxTitle[_XHS_HELPBOX_MAIL]);
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
		
		// 설명
		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Puts_SeparateNewLineVersion( gnWidth-(1024-(371+9)), tempheight+8+26+5, m_stringBoxDescription[_XHS_HELPBOX_MAIL], 196, 1.0, 3 );
		g_XBaseFont->Flush();
	}
	
#ifdef _XTS_ITEMMALLBROWSER	// 아이템몰
	// 행낭, 옵션, 아이템몰, 아이템 보관함
	tempheight = gnHeight-(768-(652-m_nHelpDescriptionBoxHeight[7]-m_nHelpDescriptionBoxHeight[8]-m_nHelpDescriptionBoxHeight[9]-m_nHelpDescriptionBoxHeight[10]));
	//DrawHelpDescriptionBox( gnWidth-(1024-(589)), tempheight, 589+206,  680 );
	DrawHelpDescriptionBox( gnWidth-(1024-(589)), tempheight, gnWidth-(1024-(589+206)),  gnHeight-(768-(680)) );
	
	//화살표
	m_imageArrow4->Draw( gnWidth-(1024-(901)), gnHeight-(768-(667)) );
	m_imageArrowBody1->SetScale( 5.35f, 1.0f);
	m_imageArrowBody1->Draw( gnWidth-(1024-(794)), gnHeight-(768-(667)) );
	
	//행낭
	if( m_stringBoxTitle[_XHS_HELPBOX_INVEN] &&  m_stringBoxDescription[_XHS_HELPBOX_INVEN] )
	{				
		
		// 제목 바탕
		m_imageTitleLeftBar->Draw( gnWidth-(1024-(589+2)), tempheight+8 );
		m_imageTitleCenterBar->SetScale( 3.4642f, 1.0f); // (206-12)/56
		m_imageTitleCenterBar->Draw( gnWidth-(1024-(589+2+5)), tempheight+8 );
		m_imageTitleRightBar->Draw( gnWidth-(1024-(589+2+5+194)), tempheight+8 );
		
		// 제목
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 	
		g_XBaseFont->Puts( gnWidth-(1024-(589+2+10)), tempheight+8+7, m_stringBoxTitle[_XHS_HELPBOX_INVEN]);
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
		
		// 설명
		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Puts_SeparateNewLineVersion( gnWidth-(1024-(589+9)), tempheight+8+26+5, m_stringBoxDescription[_XHS_HELPBOX_INVEN], 194, 1.0, 3 );
		g_XBaseFont->Flush();		
	}
	//옵션
	if( m_stringBoxTitle[_XHS_HELPBOX_OPTION] &&  m_stringBoxDescription[_XHS_HELPBOX_OPTION] )
	{						
		tempheight = gnHeight-(768-(652 - m_nHelpDescriptionBoxHeight[8]-m_nHelpDescriptionBoxHeight[9]-m_nHelpDescriptionBoxHeight[10]));
		// 제목 바탕
		m_imageTitleLeftBar->Draw( gnWidth-(1024-(589+2)), tempheight+8 );
		m_imageTitleCenterBar->SetScale( 3.4642f, 1.0f); // (206-12)/56
		m_imageTitleCenterBar->Draw( gnWidth-(1024-(589+2+5)), tempheight+8 );
		m_imageTitleRightBar->Draw( gnWidth-(1024-(589+2+5+194)), tempheight+8 );
		
		// 제목
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 	
		g_XBaseFont->Puts( gnWidth-(1024-(589+2+10)), tempheight+8+7, m_stringBoxTitle[_XHS_HELPBOX_OPTION]);
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
		
		// 설명
		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Puts_SeparateNewLineVersion( gnWidth-(1024-(589+9)), tempheight+8+26+5, m_stringBoxDescription[_XHS_HELPBOX_OPTION], 194, 1.0, 3 );
		g_XBaseFont->Flush();		
	}
	//아이템 상점
	if( m_stringBoxTitle[_XHS_HELPBOX_ITEMMALL] &&  m_stringBoxDescription[_XHS_HELPBOX_ITEMMALL] )
	{						
		tempheight = gnHeight-(768-(652 - m_nHelpDescriptionBoxHeight[9]-m_nHelpDescriptionBoxHeight[10]));
		// 제목 바탕
		m_imageTitleLeftBar->Draw( gnWidth-(1024-(589+2)), tempheight+8 );
		m_imageTitleCenterBar->SetScale( 3.4642f, 1.0f); // (206-12)/56
		m_imageTitleCenterBar->Draw( gnWidth-(1024-(589+2+5)), tempheight+8 );
		m_imageTitleRightBar->Draw( gnWidth-(1024-(589+2+5+194)), tempheight+8 );
		
		// 제목
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 	
		g_XBaseFont->Puts( gnWidth-(1024-(589+2+10)), tempheight+8+7, m_stringBoxTitle[_XHS_HELPBOX_ITEMMALL]);
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
		
		// 설명
		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Puts_SeparateNewLineVersion( gnWidth-(1024-(589+9)), tempheight+8+26+5, m_stringBoxDescription[_XHS_HELPBOX_ITEMMALL], 194, 1.0, 3 );
		g_XBaseFont->Flush();		
	}
	//아이템 보관함
	if( m_stringBoxTitle[_XHS_HELPBOX_ITEMBOX] &&  m_stringBoxDescription[_XHS_HELPBOX_ITEMBOX] )
	{						
		tempheight = gnHeight-(768-(652 - m_nHelpDescriptionBoxHeight[10]));
		// 제목 바탕
		m_imageTitleLeftBar->Draw( gnWidth-(1024-(589+2)), tempheight+8 );
		m_imageTitleCenterBar->SetScale( 3.4642f, 1.0f); // (206-12)/56
		m_imageTitleCenterBar->Draw( gnWidth-(1024-(589+2+5)), tempheight+8 );
		m_imageTitleRightBar->Draw( gnWidth-(1024-(589+2+5+194)), tempheight+8 );
		
		// 제목
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 	
		g_XBaseFont->Puts( gnWidth-(1024-(589+2+10)), tempheight+8+7, m_stringBoxTitle[_XHS_HELPBOX_ITEMBOX]);
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
		
		// 설명
		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Puts_SeparateNewLineVersion( gnWidth-(1024-(589+9)), tempheight+8+26+5, m_stringBoxDescription[_XHS_HELPBOX_ITEMBOX], 194, 1.0, 3 );
		g_XBaseFont->Flush();		
	}
#else
	// 행낭, 옵션
	tempheight = gnHeight-(768-(652-m_nHelpDescriptionBoxHeight[7]-m_nHelpDescriptionBoxHeight[8]));
	DrawHelpDescriptionBox( gnWidth-(1024-(589)), tempheight, gnWidth-(1024-(589+206)), gnHeight-(768-(680)) );
	
	//화살표
	m_imageArrow4->Draw( gnWidth-(1024-(901)), gnHeight-(768-(667)) );
	m_imageArrowBody1->SetScale( 5.35f, 1.0f);
	m_imageArrowBody1->Draw( gnWidth-(1024-(794)), gnHeight-(768-(667)) );
	
	//행낭
	if( m_stringBoxTitle[_XHS_HELPBOX_INVEN] &&  m_stringBoxDescription[_XHS_HELPBOX_INVEN] )
	{				
		
		// 제목 바탕
		m_imageTitleLeftBar->Draw( gnWidth-(1024-(589+2)), tempheight+8 );
		m_imageTitleCenterBar->SetScale( 3.4642f, 1.0f); // (206-12)/56
		m_imageTitleCenterBar->Draw( gnWidth-(1024-(589+2+5)), tempheight+8 );
		m_imageTitleRightBar->Draw( gnWidth-(1024-(589+2+5+194)), tempheight+8 );
		
		// 제목
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 	
		g_XBaseFont->Puts( gnWidth-(1024-(589+2+10)), tempheight+8+7, m_stringBoxTitle[_XHS_HELPBOX_INVEN]);
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
		
		// 설명
		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Puts_SeparateNewLineVersion( gnWidth-(1024-(589+9)), tempheight+8+26+5, m_stringBoxDescription[_XHS_HELPBOX_INVEN], 194, 1.0, 3 );
		g_XBaseFont->Flush();		
	}
	//옵션
	if( m_stringBoxTitle[_XHS_HELPBOX_OPTION] &&  m_stringBoxDescription[_XHS_HELPBOX_OPTION] )
	{						
		tempheight = gnHeight-(768-(652 - m_nHelpDescriptionBoxHeight[8]));
		// 제목 바탕
		m_imageTitleLeftBar->Draw( gnWidth-(1024-(589+2)), tempheight+8 );
		m_imageTitleCenterBar->SetScale( 3.4642f, 1.0f); // (206-12)/56
		m_imageTitleCenterBar->Draw( gnWidth-(1024-(589+2+5)), tempheight+8 );
		m_imageTitleRightBar->Draw( gnWidth-(1024-(589+2+5+194)), tempheight+8 );
		
		// 제목
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 	
		g_XBaseFont->Puts( gnWidth-(1024-(589+2+10)), tempheight+8+7, m_stringBoxTitle[_XHS_HELPBOX_OPTION]);
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
		
		// 설명
		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Puts_SeparateNewLineVersion( gnWidth-(1024-(589+9)), tempheight+8+26+5, m_stringBoxDescription[_XHS_HELPBOX_OPTION], 194, 1.0, 3 );
		g_XBaseFont->Flush();
		
	}
#endif

	
	//인물창
	if( m_stringBoxTitle[_XHS_HELPBOX_CINFO] &&  m_stringBoxDescription[_XHS_HELPBOX_CINFO] )
	{		
		tempheight = gnHeight-(768-(675-m_nHelpDescriptionBoxHeight[6]));
		
		DrawHelpDescriptionBox( gnWidth-(1024-(799)), tempheight, gnWidth-(1024-(799+212)), gnHeight-(768-(657)) );
		
		//화살표
		m_imageArrow5->Draw( gnWidth-(1024-(977)), gnHeight-(768-(658)) );
		
		// 제목 바탕
		m_imageTitleLeftBar->Draw( gnWidth-(1024-(799+2)), tempheight+8 );
		m_imageTitleCenterBar->SetScale( 3.5714f, 1.0f); // (212-12)/56
		m_imageTitleCenterBar->Draw( gnWidth-(1024-(799+2+5)), tempheight+8 );
		m_imageTitleRightBar->Draw( gnWidth-(1024-(799+2+5+200)), tempheight+8 );
		
		// 제목
		g_XBaseFont->SetBoldMode(TRUE);
		g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 212, 0)); 	
		g_XBaseFont->Puts( gnWidth-(1024-(799+2+10)), tempheight+8+7, m_stringBoxTitle[_XHS_HELPBOX_CINFO]);
		g_XBaseFont->Flush();
		g_XBaseFont->SetBoldMode(FALSE);
		
		// 설명
		g_XBaseFont->SetColor(_XSC_DEFAULT);		
		g_XBaseFont->Puts_SeparateNewLineVersion( gnWidth-(1024-(799+9)), tempheight+8+26+5, m_stringBoxDescription[_XHS_HELPBOX_CINFO], 192, 1.0, 3 );
		g_XBaseFont->Flush();
		
	}
	
}


void _XWindow_HelpScreen::SetHelpDescriptionBox( int index, LPTSTR title, LPTSTR contents )
{
	// 상태창,미니맵,단축창,운기요상,메신져,전서구,인물창,행낭,옵션
	if( title )
	{
		m_stringBoxTitle[index] = NULL;
		m_stringBoxTitle[index] = title;
	}
	
	if( contents )
	{
		m_stringBoxDescription[index] = NULL;
		m_stringBoxDescription[index] = contents;
	}

	switch(index) {
	case _XHS_HELPBOX_STATUS:
		{
			if( m_stringBoxTitle[index] && m_stringBoxDescription[index] )
			{
				m_nHelpDescriptionBoxHeight[0] = 0;
				m_nHelpDescriptionBoxHeight[0] += ( HS_ROW_INTERVAL * g_XBaseFont->GetRowCountNewLineVersion(m_stringBoxDescription[_XHS_HELPBOX_STATUS],230) );
				m_nHelpDescriptionBoxHeight[0] += 42; 
			}
		}
		break;
	case _XHS_HELPBOX_MINIMAP:
		{
			if( m_stringBoxTitle[index] && m_stringBoxDescription[index] )
			{
				m_nHelpDescriptionBoxHeight[1] = 0;
				m_nHelpDescriptionBoxHeight[1] += ( HS_ROW_INTERVAL * g_XBaseFont->GetRowCountNewLineVersion(m_stringBoxDescription[_XHS_HELPBOX_MINIMAP],230) );
				m_nHelpDescriptionBoxHeight[1] += 42; 
			}
		}
		break;
	case _XHS_HELPBOX_QUICKSLOT:
		{
			if( m_stringBoxTitle[index] && m_stringBoxDescription[index] )
			{
				m_nHelpDescriptionBoxHeight[2] = 0;
				m_nHelpDescriptionBoxHeight[2] += ( HS_ROW_INTERVAL * g_XBaseFont->GetRowCountNewLineVersion(m_stringBoxDescription[_XHS_HELPBOX_QUICKSLOT],224) );
				m_nHelpDescriptionBoxHeight[2] += 42; 
			}
		}
		break;
	case _XHS_HELPBOX_MEDETATIOH:
		{
			if( m_stringBoxTitle[index] && m_stringBoxDescription[index] )
			{
				m_nHelpDescriptionBoxHeight[3] = 0;
				m_nHelpDescriptionBoxHeight[3] += ( HS_ROW_INTERVAL * g_XBaseFont->GetRowCountNewLineVersion(m_stringBoxDescription[_XHS_HELPBOX_MEDETATIOH],196) );
				m_nHelpDescriptionBoxHeight[3] += 34; 
			}
		}
		break;
	case _XHS_HELPBOX_MESSENGER:
		{
			if( m_stringBoxTitle[index] && m_stringBoxDescription[index] )
			{
				m_nHelpDescriptionBoxHeight[4] = 0;
				m_nHelpDescriptionBoxHeight[4] += ( HS_ROW_INTERVAL * g_XBaseFont->GetRowCountNewLineVersion(m_stringBoxDescription[_XHS_HELPBOX_MESSENGER],196) );
				m_nHelpDescriptionBoxHeight[4] += 26; 
			}
		}
		break;
	case _XHS_HELPBOX_MAIL:
		{
			if( m_stringBoxTitle[index] && m_stringBoxDescription[index] )
			{
				m_nHelpDescriptionBoxHeight[5] = 0;
				m_nHelpDescriptionBoxHeight[5] += ( HS_ROW_INTERVAL * g_XBaseFont->GetRowCountNewLineVersion(m_stringBoxDescription[_XHS_HELPBOX_MAIL],196) );
				m_nHelpDescriptionBoxHeight[5] += 34; 
			}
		}
		break;
	case _XHS_HELPBOX_CINFO:
		{
			if( m_stringBoxTitle[index] && m_stringBoxDescription[index] )
			{
				m_nHelpDescriptionBoxHeight[6] = 0;
				m_nHelpDescriptionBoxHeight[6] += ( HS_ROW_INTERVAL * g_XBaseFont->GetRowCountNewLineVersion(m_stringBoxDescription[_XHS_HELPBOX_CINFO],192) );
				m_nHelpDescriptionBoxHeight[6] += 42; 
			}
		}
		break;
	case _XHS_HELPBOX_INVEN:
		{
			if( m_stringBoxTitle[index] && m_stringBoxDescription[index] )
			{
				m_nHelpDescriptionBoxHeight[7] = 0;
				m_nHelpDescriptionBoxHeight[7] += ( HS_ROW_INTERVAL * g_XBaseFont->GetRowCountNewLineVersion(m_stringBoxDescription[_XHS_HELPBOX_INVEN],194) );
				m_nHelpDescriptionBoxHeight[7] += 34; 
			}
		}
		break;
	case _XHS_HELPBOX_OPTION:
		{
			if( m_stringBoxTitle[index] && m_stringBoxDescription[index] )
			{
				m_nHelpDescriptionBoxHeight[8] = 0;
				m_nHelpDescriptionBoxHeight[8] += ( HS_ROW_INTERVAL * g_XBaseFont->GetRowCountNewLineVersion(m_stringBoxDescription[_XHS_HELPBOX_OPTION],194) );
				m_nHelpDescriptionBoxHeight[8] += 34; 
			}
		}
		break;
#ifdef _XTS_ITEMMALLBROWSER	// 아이템몰
	case _XHS_HELPBOX_ITEMMALL:
		{
			if( m_stringBoxTitle[index] && m_stringBoxDescription[index] )
			{
				m_nHelpDescriptionBoxHeight[9] = 0;
				m_nHelpDescriptionBoxHeight[9] += ( HS_ROW_INTERVAL * g_XBaseFont->GetRowCountNewLineVersion(m_stringBoxDescription[_XHS_HELPBOX_ITEMMALL],194) );
				m_nHelpDescriptionBoxHeight[9] += 34; 
			}
		}
		break;
	case _XHS_HELPBOX_ITEMBOX:
		{
			if( m_stringBoxTitle[index] && m_stringBoxDescription[index] )
			{
				m_nHelpDescriptionBoxHeight[10] = 0;
				m_nHelpDescriptionBoxHeight[10] += ( HS_ROW_INTERVAL * g_XBaseFont->GetRowCountNewLineVersion(m_stringBoxDescription[_XHS_HELPBOX_ITEMBOX],194) );
				m_nHelpDescriptionBoxHeight[10] += 34; 
			}
		}
		break;
#endif
	}
}