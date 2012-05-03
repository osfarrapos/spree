// XWindow_Help.cpp: implementation of the _XWindow_Help class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XWindow_Help.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XSR_STRINGHEADER.H"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_Help::_XWindow_Help()
{

}

_XWindow_Help::~_XWindow_Help()
{

}

BOOL _XWindow_Help::Initialize( void )
{
	m_SettingOn = FALSE;
	m_TutorialState = 0;	// zerolevel
	m_MiniGameState = MT_NONE;
	m_WindowPosition.x += gnWidth-1024;
	m_WindowPosition.y += gnHeight-768;

//	int closebuttonindex = g_MainInterfaceTextureArchive.FindResource( "window_closebutton.tga" );
//	
//	_XBTN_STRUCTURE minimizebtnstruct = { TRUE, { 192, 0 },{ 12, 12 }, _XDEF_CI_CLOSEBUTTON, 
//		closebuttonindex,closebuttonindex,closebuttonindex, 
//		&g_MainInterfaceTextureArchive };
//	_XButton* minimizebutton = new _XButton;
//	minimizebutton->Create( minimizebtnstruct );
//	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 0,  0, 12, 12 );
//	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  12, 0, 24, 12 );
//	minimizebutton->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  24, 0, 36, 16 );
//	InsertChildObject( minimizebutton );
	// modify end
	
	// 미니게임(운기조식)에 쓰일 이미지 -----------------------------------------------------------------------------------------------┓
	for( int i = 0; i < 5; i++ )
	{
		m_Mini_Meditation1[i].Create( m_WindowPosition.x+5, m_WindowPosition.y+(i*44)+53,
									  m_WindowPosition.x+5+31, m_WindowPosition.y+(i*44)+53+31,
									  &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("help.tga") );
		m_Mini_Meditation1[i].SetClipRect((i* 32), 0, (i* 32)+31, 32);
	}
	// -------------------------------------------------------------------------------------------------------------------------┛

	_XBTN_STRUCTURE btnstruct = 
	{
		TRUE, {442, 286}, {46, 17},
		_XDEF_TRAINING_HELP_CLOSE,
		g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
		g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
		g_MainInterfaceTextureArchive.FindResource("etcwindow_button.tga"),
		&g_MainInterfaceTextureArchive
	};
	m_CloseButton = new _XButton;
	m_CloseButton->Create(btnstruct);
	m_CloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 0, 13, 47, 30);
	m_CloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 0, 30, 47, 48);
	m_CloseButton->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 0, 47, 47, 64);
	m_CloseButton->SetButtonTextColor(D3DCOLOR_ARGB(255, 189, 188, 175));
	m_CloseButton->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_COMMON_OK);	// _T("확인")
	InsertChildObject(m_CloseButton);
	
	SetWindowMoveMode(_XWMOVE_FIXED);

	return TRUE;
}

void _XWindow_Help::Draw( _XGUIObject*& pfocusobject )
{
	_XWindow::Draw(pfocusobject);	

	if( !this->m_ShowWindow ) return;

	switch(m_MiniGameState) 
	{
	case MT_MEDITATION:
		{
			for( int i = 0; i < 5; i++ )
			{
				m_Mini_Meditation1[i].m_Position.x = m_WindowPosition.x+5;
				m_Mini_Meditation1[i].m_Position.y = m_WindowPosition.y+(i*44)+53;
				m_Mini_Meditation1[i].DrawWithRegion();
			}
			DrawMeditationText();				
			_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y, 
				m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, _XSC_DEFAULT_WHITE );
		}
		break;

	case MT_DUMMY :
		{
			DrawDummyText();
			_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y, 
				m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, _XSC_DEFAULT_WHITE );
		}
		break;
		
	case MT_SPELL :
		{
			DrawSpellText();
			_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y, 
				m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, _XSC_DEFAULT_WHITE );
		}
		break;
	case MT_TUTORIAL :		// zerolevel
		{
			DrawTutorialText();			
		}
		break;
	}
}

BOOL _XWindow_Help::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow )
	{
		m_SettingOn = FALSE;
		return FALSE;
	}

	if( !this->m_Enable  ) return FALSE;
	
	if( !m_SettingOn )
	{		
		switch(m_MiniGameState) 
		{
		case MT_MEDITATION:
			{
				// 사이즈 조정
				SetUseBorderImage(FALSE);
				SIZE tmpsize;
				tmpsize.cx = 500;
				tmpsize.cy = 308;
				m_WindowPosition.x = (gnWidth-tmpsize.cx)/2;
				m_WindowPosition.y = (gnHeight-tmpsize.cy)/2;						
				SetWindowSize(tmpsize);
				//m_CloseButton->MoveWindow(442, 286);
				m_SettingOn = TRUE;
			}
			break;
			
		case MT_DUMMY :
			{
				// 사이즈 조정
				SetUseBorderImage(FALSE);
				SIZE tmpsize;
				tmpsize.cx = 500;
				tmpsize.cy = 308;
				m_WindowPosition.x = (gnWidth-tmpsize.cx)/2;
				m_WindowPosition.y = (gnHeight-tmpsize.cy)/2;						
				SetWindowSize(tmpsize);
				m_CloseButton->MoveWindow(m_WindowPosition.x+442, m_WindowPosition.y+286);
				m_SettingOn = TRUE;
			}
			break;
			
		case MT_SPELL :
			{
				// 사이즈 조정
				SetUseBorderImage(FALSE);
				SIZE tmpsize;
				tmpsize.cx = 500;
				tmpsize.cy = 308;
				m_WindowPosition.x = (gnWidth-tmpsize.cx)/2;
				m_WindowPosition.y = (gnHeight-tmpsize.cy)/2;
				SetWindowSize(tmpsize);
				m_CloseButton->MoveWindow(m_WindowPosition.x+442, m_WindowPosition.y+286);
				m_SettingOn = TRUE;
			}
			break;
		case MT_TUTORIAL :		// zerolevel
			{
				// 사이즈 조정
				SIZE tmpsize;
				tmpsize.cx = 387;
				tmpsize.cy = 182;
				 
				SetWindowSize(tmpsize);
				XProc_ZeroLevel* pZeroLevelProcess = &((CEmperorOfDragonsApp*)gApp)->m_proc_zerolevel;
				int index = pZeroLevelProcess->GetTextureZero()->FindResource( "talkfaceback.tga" );
				SetUseBorderImage(TRUE);								
				SetBorderImage(pZeroLevelProcess->GetTextureZero(), index);
				MoveWindow( ((gnWidth>>1)-(tmpsize.cx>>1)), ((gnHeight>>1)-(tmpsize.cy>>1))-150 );
				m_borderstatic.SetClipRect( 0, 280, 387, 462 );
				
				m_CloseButton->MoveWindow(m_WindowPosition.x+170, m_WindowPosition.y+155);
				g_MainWindowManager.SetTopWindow(this);
				m_SettingOn = TRUE;
			}
			break;
		}		
	}

	if( !_XWindow::Process(pfocusobject) ) return FALSE;

	return TRUE;
}

void _XWindow_Help::SetWindowSize( SIZE size )
{
	m_WindowSize = size;
	m_solidborderstatic.MoveWindow( m_WindowPosition.x, m_WindowPosition.y, 
		m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy );
}

void _XWindow_Help::DrawMeditationText()
{
	TCHAR szText[256];	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2822) );//_T("운기조식을 시작하면 '단전'에 대기중인 '대약'이 '경락'을 통해 몸을 한 바퀴 돕니다.") );	
	g_XBaseFont->SetColor( 0xFFFFFF00 );
	g_XBaseFont->Puts( m_WindowPosition.x + 5 +2, m_WindowPosition.y + 5 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2823) );//_T("'대약'이 '경혈' 위를 지나가며 일치하는 순간 마우스 버튼을 클릭하세요.") );	
	g_XBaseFont->SetColor( 0xFFFFFF00 );
	g_XBaseFont->Puts( m_WindowPosition.x + 5 +2, m_WindowPosition.y + 19 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2824) );//_T("빨강색 경혈(經穴)") );	
	g_XBaseFont->SetColor( 0xFFFF0000 );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 53 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2825) );//_T("필수적인 경혈로서 ") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 67 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2826) );//_T("<마우스 왼쪽 버튼 클릭>") );	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Puts( m_WindowPosition.x + 148, m_WindowPosition.y + 67 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2827) );//_T("에 대응합니다.") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 290 +2, m_WindowPosition.y + 67 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2828) );//_T("파란색 경혈(經穴)") );	
	g_XBaseFont->SetColor( 0xFF0000FF );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 97 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2829) );//_T("부가적인 경혈로서 ") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 111 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2830) );//_T("<마우스 오른쪽 버튼 클릭>") );	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Puts( m_WindowPosition.x + 148 +2, m_WindowPosition.y + 111 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2831) );//_T("에 대응합니다.") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 302 +2, m_WindowPosition.y + 111 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2832) );//_T("대약(大藥)") );	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 141 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2833) );//_T("몸 안의 경락(經絡)을 따라 움직이는") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 155 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2834) );//_T("기(氣) 덩어리") );	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Puts( m_WindowPosition.x + 240 +2, m_WindowPosition.y + 155 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2835) );//_T("입니다.") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 318 +2, m_WindowPosition.y + 155 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2836) );//_T("경락(經絡)") );	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 185 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2837) );//_T("경혈과 경혈을 연결한") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 199 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2838) );//_T("대약(大藥)의 이동 경로") );	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Puts( m_WindowPosition.x + 164 +2, m_WindowPosition.y + 199 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2839) );//_T("입니다.") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 296 +2, m_WindowPosition.y + 199 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2840) );//_T("단전(丹田)") );	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 229 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2841) );//_T("기(氣)를 집중시키는 경혈로서") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 37 +2, m_WindowPosition.y + 243 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2842) );//_T("운기조식이 시작되는 지점") );	
	g_XBaseFont->SetColor( 0xFFFFFFFF );
	g_XBaseFont->Puts( m_WindowPosition.x + 210 +2, m_WindowPosition.y + 243 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2843) );//_T("입니다.") );	
	g_XBaseFont->SetColor( 0xFF999999 );
	g_XBaseFont->Puts( m_WindowPosition.x + 353 +2, m_WindowPosition.y + 243 +2, szText );
	
	sprintf( szText, "%s", _XGETINTERFACETEXT(ID_STRING_NEW_2844) );//_T("※주의: 운기조식이 시작된 후에는 끝날 때까지 도중에 멈출 수 없습니다.") );	
	g_XBaseFont->SetColor( 0xFFFFFF00 );
	g_XBaseFont->Puts( m_WindowPosition.x + 5, m_WindowPosition.y + 293, szText );
	g_XBaseFont->Flush();	
}

void _XWindow_Help::DrawDummyText(void)
{
	g_XBaseFont->SetColor(0xFF999999);
	g_XBaseFont->Puts(m_WindowPosition.x+5+2, m_WindowPosition.y+19+2, _XGETINTERFACETEXT(ID_STRING_NEW_2845) );//"더미치기 도움말");
	g_XBaseFont->Flush();
}

void _XWindow_Help::DrawSpellText(void)
{
	g_XBaseFont->SetColor(0xFF999999);
	g_XBaseFont->Puts(m_WindowPosition.x+5+2, m_WindowPosition.y+19+2, _XGETINTERFACETEXT(ID_STRING_NEW_2846) );//"기공 수련 도움말");
	g_XBaseFont->Flush();
}

void _XWindow_Help::DrawTutorialText(void)
{
	//this->SetBorderColorFactor( D3DCOLOR_ARGB( 100, 0,0,0 ) );

	switch(m_TutorialState) 
	{
	case 1:
		g_XBaseFont->SetColor(0xFF351507);
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF9B7527 );
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+20, _XGETINTERFACETEXT(ID_STRING_NEW_2847) );//"                    [ 시점 고치기 ]");		
		g_XBaseFont->DisableGrowMode();
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+50, _XGETINTERFACETEXT(ID_STRING_NEW_2848) );//"마우스의 오른쪽클릭을 누른 채로 움직이면");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+65, _XGETINTERFACETEXT(ID_STRING_NEW_2849) );//"시점을 변환할 수 있습니다.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+80, _XGETINTERFACETEXT(ID_STRING_NEW_2850) );//"마우스의 휠을 이용해 멀리 보거나 가까이 볼 수도 있습니다.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+110, _XGETINTERFACETEXT(ID_STRING_NEW_2851) );//"가장 가까이 보도록 만들면, 1인칭모드가 됩니다.");
		g_XBaseFont->Flush();		
		break;
	case 2:
		g_XBaseFont->SetColor(0xFF351507);
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF9B7527 );
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+20, _XGETINTERFACETEXT(ID_STRING_NEW_2852) );//"                    [ 미니맵 보기 ]");		
		g_XBaseFont->DisableGrowMode();
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+50, _XGETINTERFACETEXT(ID_STRING_NEW_2853) );//"화면의 오른쪽 상단에 미니맵이 위치해 있습니다.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+65, _XGETINTERFACETEXT(ID_STRING_NEW_2854) );//"확대하려면 F2키를,");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+80, _XGETINTERFACETEXT(ID_STRING_NEW_2855) );//"축소하려면 F3키를,");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+95, _XGETINTERFACETEXT(ID_STRING_NEW_2856) );//"큰 맵을 보시려면 □ 버튼을 눌러주세요.");
		g_XBaseFont->Flush();
		break;	
	case 3:
		g_XBaseFont->SetColor(0xFF351507);
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF9B7527 );
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+20, _XGETINTERFACETEXT(ID_STRING_NEW_2857) );//"                      [ 무공이란 ]");		
		g_XBaseFont->DisableGrowMode();
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+50, _XGETINTERFACETEXT(ID_STRING_NEW_2858) );//"무공은 일종의 전투 기술로");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+65, _XGETINTERFACETEXT(ID_STRING_NEW_2859) );//"하나의 무공은 몇 개의 초식으로 이루어져 있습니다.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+80, _XGETINTERFACETEXT(ID_STRING_NEW_2860) );//"무공의 단계가 상승할수록 초식의 숫자가 늘어나며,");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+95, _XGETINTERFACETEXT(ID_STRING_NEW_2861) );//"위력도 강력해 집니다.");
		g_XBaseFont->Flush();
		break;	
	case 4:
		g_XBaseFont->SetColor(0xFF351507);
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF9B7527 );
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+20, _XGETINTERFACETEXT(ID_STRING_NEW_2862) );//"                    [ 무공의 사용 ]");		
		g_XBaseFont->DisableGrowMode();
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+50, _XGETINTERFACETEXT(ID_STRING_NEW_2863) );//"왼쪽 하단의 큇슬롯에 무공이 등록되어 있으면,");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+65, _XGETINTERFACETEXT(ID_STRING_NEW_2864) );//"1~9번까지의 숫자키를 이용해 무공을 선택할 수 있습니다.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+80, _XGETINTERFACETEXT(ID_STRING_NEW_2865) );//"그 후 원하는 상대를 클릭하면 무공을 시전합니다.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+95, _XGETINTERFACETEXT(ID_STRING_NEW_2866) );//"(현재는 1번이 무공입니다.)");
		g_XBaseFont->Flush();
		break;	
	case 5:
		g_XBaseFont->SetColor(0xFF351507);
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF9B7527 );
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+20, _XGETINTERFACETEXT(ID_STRING_NEW_2867) );//"                      [ 차지스킬 ]");		
		g_XBaseFont->DisableGrowMode();
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+50, _XGETINTERFACETEXT(ID_STRING_NEW_2868) );//"기를 모아 상대에게 타격을 주는 것을");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+65, _XGETINTERFACETEXT(ID_STRING_NEW_2869) );//"'차지스킬'이라고 합니다.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+80, _XGETINTERFACETEXT(ID_STRING_NEW_2870) );//"차지스킬은 기를 오랫동안 모아 사용함으로써,");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+95, _XGETINTERFACETEXT(ID_STRING_NEW_2871) );//"더욱 강력한 위력을 발휘할 수 있습니다.");
		g_XBaseFont->Flush();
		break;	
	case 6:
		g_XBaseFont->SetColor(0xFF351507);
		g_XBaseFont->SetGrowMode( _XFONT_EDGEMODE_EDGE, 0xFF9B7527 );
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+20, _XGETINTERFACETEXT(ID_STRING_NEW_2872) );//"                  [ 차지 스킬의 사용 ]");	
		g_XBaseFont->DisableGrowMode();
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+50, _XGETINTERFACETEXT(ID_STRING_NEW_2873) );//"차지스킬을 사용하기 위해서는 먼저, 무공창에 등록해 놓은");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+65, _XGETINTERFACETEXT(ID_STRING_NEW_2874) );//"차지 스킬의 숫자키를 눌러, 기를 모읍니다.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+80, _XGETINTERFACETEXT(ID_STRING_NEW_2875) );//"(현재는 2번이 차지 스킬 입니다.)");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+110, _XGETINTERFACETEXT(ID_STRING_NEW_2876) );//"숫자키를 누르고 있으면 케릭터 머리 위에 파란색 막대가");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+125, _XGETINTERFACETEXT(ID_STRING_NEW_2877) );//"가득 차는 것이 보일 것 입니다.");
		g_XBaseFont->Puts(m_WindowPosition.x+27, m_WindowPosition.y+140, _XGETINTERFACETEXT(ID_STRING_NEW_2878) );//"이 때 누르기를 중지하면, 차지 스킬이 시전됩니다.");
		g_XBaseFont->Flush();
		break;	
	}	
}