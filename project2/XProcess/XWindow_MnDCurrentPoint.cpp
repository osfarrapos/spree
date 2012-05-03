// XWindow_MnDCurrentPoint.cpp: implementation of the _XWindow_MnDCurrentPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindowObjectDefine.h"
#include "XSR_STRINGHEADER.H"
#include "XTextureAnimation.h"
#include "XWindow_MnDCurrentPoint.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MnDCurrentPoint::_XWindow_MnDCurrentPoint()
{
	m_pTitleImage = NULL;
	m_pFullPointStartAni = NULL;
	m_pFullPointAni = NULL;

	for( int i = 0; i < 3; i++ )
	{
		for(int j = 0; j < 10; j++ )
		{
			m_pNumberAni[i][j] = NULL;
		}

		m_nDrawNumber[i] = 0;
	}

}

_XWindow_MnDCurrentPoint::~_XWindow_MnDCurrentPoint()
{

}

BOOL _XWindow_MnDCurrentPoint::Initialize()
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_bloodpoint.tga");

	if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE)
	{
		m_pTitleImage = new _XImageStatic;
		m_pTitleImage->Create(-10, 0, 7, 15, &g_MainInterfaceTextureArchive, resourceindex);
		m_pTitleImage->SetClipRect(53, 78, 81, 93);
		InsertChildObject(m_pTitleImage);
	}
	else
	{
		m_pTitleImage = new _XImageStatic; //한자 이미지
		m_pTitleImage->Create(1, 0, 18, 15, &g_MainInterfaceTextureArchive, resourceindex);
		m_pTitleImage->SetClipRect(53, 78, 70, 93);
		InsertChildObject(m_pTitleImage);
	}

	//999 애니
	m_pFullPointStartAni = new _XTextureAnimation( 0, 0, 64, 24, this, &g_MainInterfaceTextureArchive, resourceindex, 3, 2, 4, 0, 0);
	m_pFullPointStartAni->SetAniLoopCount(1);
	m_pFullPointStartAni->ShowWindow(FALSE);

	m_pFullPointAni = new _XTextureAnimation( 0, 0, 64, 24, this, &g_MainInterfaceTextureArchive, resourceindex, 3, 1, 3, 0, 100);
	m_pFullPointAni->ShowWindow(FALSE);

	//숫자애니
	for( int i = 0; i < 3; i++ )
	{
		for(int j = 0; j < 10; j++ )
		{
			//0이 맨 뒤에 있어서,,
			if( j == 0 )
				m_pNumberAni[i][j] = new _XTextureAnimation( 0, 0, 13, 19, this, &g_MainInterfaceTextureArchive, resourceindex, 4, 1, 4, 0, 9*19 );
			else
				m_pNumberAni[i][j] = new _XTextureAnimation( 0, 0, 13, 19, this, &g_MainInterfaceTextureArchive, resourceindex, 4, 1, 4, 0, (j-1)*19 );

			m_pNumberAni[i][j]->SetAniLoopCount(1);
			m_pNumberAni[i][j]->SetDrawFirstAni(true); //첫번째 이미지로 돌림
			m_pNumberAni[i][j]->SetDrawEndAni(true); //애니메이션이 아니더라도 계속 그림을 유지
			m_pNumberAni[i][j]->ShowWindow(FALSE);

			if( i == 0 )
				m_pNumberAni[i][j]->SetDrawPostion( m_WindowPosition.x + 18, m_WindowPosition.y - 2 );
			else if( i == 1 )
				m_pNumberAni[i][j]->SetDrawPostion( m_WindowPosition.x + 32, m_WindowPosition.y - 2 );
			else if( i == 2 )
				m_pNumberAni[i][j]->SetDrawPostion( m_WindowPosition.x + 41, m_WindowPosition.y - 2 );
		}

		m_nDrawNumber[i] = 0;
	}
	
	return TRUE;
}

void _XWindow_MnDCurrentPoint::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_MnDCurrentPoint::Draw( _XGUIObject*& pfocusobject )
{	
	if( !this->m_ShowWindow ) return;

	_XWindow::Draw(pfocusobject);

	//포인트가 999일 때
	if( g_pLocalUser->m_pLocalMnDInfomation.nCurKillCount == 999 )
	{
		if( m_pFullPointStartAni->IsEndAnimate() ) //애니가 끝났는가?
		{
			m_pFullPointAni->Update(0.1); //이걸 showwindow 하는 건 패킷 받을 때 처리를 해주도록 한다
		}
		else
			m_pFullPointStartAni->Update(0.1);  //이걸 showwindow 하는 건 패킷 받을 때 처리를 해주도록 한다
	}
	else 
	{
		m_pNumberAni[0][m_nDrawNumber[0]]->Update(0.1);
		m_pNumberAni[1][m_nDrawNumber[1]]->Update(0.1);
		m_pNumberAni[2][m_nDrawNumber[2]]->Update(0.1);
	}
}

BOOL _XWindow_MnDCurrentPoint::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_MnDCurrentPoint::SetNumberProcess()
{
	int nHundred = g_pLocalUser->m_pLocalMnDInfomation.nCurKillCount/100; //백의자리
	int nTen = (g_pLocalUser->m_pLocalMnDInfomation.nCurKillCount%100)/10; //십의자리
	int nOne = (g_pLocalUser->m_pLocalMnDInfomation.nCurKillCount%100)%10; //일의 자리

	m_nDrawNumber[0] = nHundred;
	m_nDrawNumber[1] = nTen;
	m_nDrawNumber[2] = nOne;

	//모두 한번 끄자 그 숫자만 나와야돼..
	for( int i = 0; i < 3; i++ )
	{
		for(int j = 0; j < 10; j++ )
		{
			m_pNumberAni[i][j]->ShowWindow(FALSE);
		}
	}

	m_pNumberAni[0][m_nDrawNumber[0]]->ShowWindow(TRUE); //현재 숫자 자리수는 켜준다
	m_pNumberAni[1][m_nDrawNumber[1]]->ShowWindow(TRUE); //현재 숫자 자리수는 켜준다
	m_pNumberAni[2][m_nDrawNumber[2]]->ShowWindow(TRUE); //현재 숫자 자리수는 켜준다

	//그런데,,
	if( nHundred == 0 ) //백의자리나 십의 자리가 0일땐,, 꺼야한다
	{
		for(int j = 0; j < 10; j++ )
		{
			m_pNumberAni[0][j]->ShowWindow(FALSE);
		}

		if( nTen == 0 )
		{
			for(int j = 0; j < 10; j++ )
			{
				m_pNumberAni[1][j]->ShowWindow(FALSE);
			}
		}
	}
}

void _XWindow_MnDCurrentPoint::ShowWindow( BOOL show )
{
	_XWindow::ShowWindow(show);
}

void _XWindow_MnDCurrentPoint::MoveWindow( int X, int Y )
{			
	_XWindow::MoveWindow( X, Y );
}