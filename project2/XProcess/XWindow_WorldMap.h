// XWindow_WorldMap.h: interface for the XWindow_WorldMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XWINDOW_WORLDMAP_H__D1D4838F_B55B_4819_82B0_B4D272379C5E__INCLUDED_)
#define AFX_XWINDOW_WORLDMAP_H__D1D4838F_B55B_4819_82B0_B4D272379C5E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XWindow.h"

#include <map>
#include <vector>
#include <string>
using namespace std;

#include "XTextureAnimation.h"

struct MapInfo
{
	int nID; //맵 아이디
	int	nName; //맵이름
	
	POINT	IconPos; //아이콘의 좌표
	int	nIconIndex; // 아이콘 그림

	int nMapInfo_1; //맵 텍스트 정보
	DWORD	dwMapInfoColor_1; // 맵텍스트 컬러

	int nMapInfo_2; //맵 텍스트 정보
	DWORD	dwMapInfoColor_2; // 맵텍스트 컬러

	vector<int>	vtLinkID; //링크 맵 아이디
	BOOL bEnable; //해당맵을 사용할지 여부

	_XImageStatic* pMapImage; //해당 미니맵
	LPDIRECT3DTEXTURE9 pImageTex; 

	MapInfo()
	{
		vtLinkID.clear();
		pMapImage = NULL;
		pImageTex = NULL;
	}
};

class XWindow_WorldMap : public _XWindow
{
public:
	XWindow_WorldMap();
	virtual ~XWindow_WorldMap();

	BOOL	Initialize(XMLPaser& m_xXML);
	void	DestroyWindow( void );
	
	void	Draw( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );
	BOOL	Process( _XGUIObject*& pfocusobject = g_CurrentFocusedObject );

	void	ShowWindow(BOOL show);
	void	MoveWindow( int X, int Y );

	BOOL	SetShowWindowProcess( BOOL bShow );

	void    SetKey( const int nKey );
	void	SetDrawPostion( int nX, int nY );
	void	FirstPostion( int nX, int nY );
	
private :
	BOOL	LoadXMLTable(XMLPaser& m_xXML, LPTSTR pTableName );
	BOOL	LoadFile( FILE* fp );
	
	BOOL	SetAlphaProcess(BOOL bShow);
	BOOL	SetMiniMapImage(_XPackageArchive& TextureArchive, MapInfo& MapData );
	void	SetButton( const int nID, const MapInfo& MapData ); 
	void	SetStringColor( const char* szString, DWORD& dwColor );
	void	SetCharPostion(int nKey);

private :
	LPDIRECT3DTEXTURE9 m_pWorldmapTex;
	_XImageStatic*	m_pWorldMapImage;
	_XTextureAnimation* m_pCharinfo; //케릭터 위치

	_XImageStatic* m_pBoard;

	map<int, MapInfo*>	m_mpData; //아이디, 맵정보
	
	int m_nMaxLinkMapCount;
	int m_nFindKey;

	//텍스쳐 에니메이션
	vector<_XTextureAnimation*> vtTextureAni; //텍스쳐 에니메이션 셋팅

	POINT m_nPos;
	POINT m_nFirstPos;

	BOOL m_bAlphaProcess;
};


//inline-------------------------------------------------------------------------------------
inline BOOL XWindow_WorldMap::SetShowWindowProcess( BOOL bShow )
{
	m_bAlphaProcess = bShow;
	if( m_bAlphaProcess )
	{
		m_pWorldMapImage->m_FColor = D3DCOLOR_ARGB( 0, 255, 255, 255 );
		ShowWindow(TRUE);
		
#ifdef _ACCLAIM_RUBICONADSYSTEM	
		g_ViewRubiconAD = FALSE;
		::ShowWindow(g_hRubiconADWindow, SW_HIDE);
#endif
	}
	else
	{
		m_pWorldMapImage->m_FColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
#ifdef _ACCLAIM_RUBICONADSYSTEM
		if( (g_pLocalUser->m_AccountItemcType >= 0 && g_pLocalUser->m_AccountItemsID > 0) || 
			g_RubiconADFreeLevel > g_pLocalUser->m_CharacterInfo.Get_innerlevel() )
		{
			g_ViewRubiconAD = FALSE;
			::ShowWindow(g_hRubiconADWindow, SW_HIDE);
		}
		else
		{
			g_ViewRubiconAD = TRUE;
			::ShowWindow(g_hRubiconADWindow, SW_SHOW);
		}
#endif
	}

	return m_bAlphaProcess;
}

inline void XWindow_WorldMap::SetDrawPostion( int nX, int nY )
{
	m_nPos.x = nX;
	m_nPos.y = nY;
}

inline void XWindow_WorldMap::FirstPostion( int nX, int nY )
{
	m_nFirstPos.x = nX;
	m_nFirstPos.y = nY;
}

//개방 333
//소림사 335
//무당 337
//녹림 336
//비궁 334
//마교 338
//합비 2166
//북경 3486
//정주 3491
//석가장 3492
//제남 3493
//대동 3494
//남창 3495
//항주 3496
//요동 3002
//북해빙궁 3418
//포달랍궁 3488
//혈사평	3224
//패왕동 3225

#endif // !defined(AFX_XWINDOW_WORLDMAP_H__D1D4838F_B55B_4819_82B0_B4D272379C5E__INCLUDED_)
