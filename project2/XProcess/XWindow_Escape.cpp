// XWindow_Escape.cpp: implementation of the _XWindow_Escape class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

#include "XWindow_Escape.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_Escape::_XWindow_Escape()
{
	m_pChargeGaugeWindow = NULL;
	m_SelectedVillageStringID	= -1;	//현재 선택된 지역에서 이동될 마을 이름
	m_SelectedAreaID	= -1;			//현재 선택된 지영게서 이동될 영역 아이디

	memset( m_SelectedVillageName, 0, sizeof(TCHAR)*512 );

}

_XWindow_Escape::~_XWindow_Escape()
{
	_XEscapeInfo* currentescapeinfo = NULL;
	list <_XEscapeInfo*>::iterator iter_escapeinfo;
	for(iter_escapeinfo = m_listEscapeInfo.begin() ; iter_escapeinfo != m_listEscapeInfo.end() ; ++iter_escapeinfo)
	{	
		currentescapeinfo = *iter_escapeinfo;
		if(currentescapeinfo)
		{
			SAFE_DELETE(currentescapeinfo);
			iter_escapeinfo = m_listEscapeInfo.erase(iter_escapeinfo);
		}
	}

	m_listEscapeInfo.clear();
}

BOOL _XWindow_Escape::Initialize(void)
{
	
	int mainframeresourceindex	= g_MainInterfaceTextureArchive.FindResource("MI_main_back01.tga");
	int messengerresourceindex1	= g_MainInterfaceTextureArchive.FindResource( "MI_Messenger1.tga" );
	int messengerresourceindex2	= g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" );
	
	// 총 크기  183 X 144
	
	// 타이틀 바 ( 183 X 18 )
	_XImageStatic* pTitleBar = new _XImageStatic;
	pTitleBar->Create(0, 0, 183, 18, &g_MainInterfaceTextureArchive, mainframeresourceindex);
	pTitleBar->SetClipRect(0, 125, 183, 143);	
	InsertChildObject( pTitleBar);	
	
	// 왼쪽 라인 ( 3 X 123 )
	_XImageStatic* pLeftBorder = new _XImageStatic;
	pLeftBorder->Create(0, 18, 3, 141, &g_MainInterfaceTextureArchive, mainframeresourceindex);
	pLeftBorder->SetClipRect(0, 195, 3, 227);
	pLeftBorder->SetScale(1.0f, 3.84375f);
	InsertChildObject(pLeftBorder);
	
	// 오른쪽 라인 ( 3 X 123 )
	_XImageStatic* pRightBorder = new _XImageStatic;
	pRightBorder->Create(180, 18, 183, 141, &g_MainInterfaceTextureArchive, mainframeresourceindex);
	pRightBorder->SetClipRect(0, 195, 3, 227);
	pRightBorder->SetScale(1.0f, 3.84375f);
	InsertChildObject(pRightBorder);
	
	// 바닥 라인 ( 183 X 3 )
	_XImageStatic* pBottomLine = new _XImageStatic;
	pBottomLine->Create(0, 141, 183, 144, &g_MainInterfaceTextureArchive, mainframeresourceindex);
	pBottomLine->SetClipRect(0, 227, 183, 230);
	InsertChildObject(pBottomLine);
	
	// 왼쪽박스 이미지 (  85X 110 )
	_XImageStatic* pLeftBox = new _XImageStatic;
	pLeftBox->Create( 6, 28, 91, 138, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	pLeftBox->SetClipRect( 5, 181, 90, 254 );
	pLeftBox->SetScale( 1.0f, 1.5068493150f );
	InsertChildObject( pLeftBox );
	
	// 오른쪽박스 이미지 (  85X 110 )
	_XImageStatic* pRightBox = new _XImageStatic;
	pRightBox->Create( 91+85, 28, 91, 138, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_Messenger2.tga" ) );	
	pRightBox->SetClipRect( 5, 181, 90, 254 );
	pRightBox->SetScale( -1.0f, 1.5068493150f );
	InsertChildObject( pRightBox );

		
	// 취소 버튼 ( 56 X 20 )
	_XBTN_STRUCTURE canclebtnstruct = { TRUE, { 62, 104 }, { 56, 20 }, _XDEF_ESCAPE_BTN_CANCLE,
		messengerresourceindex1,messengerresourceindex1,messengerresourceindex1, &g_MainInterfaceTextureArchive };
	_XButton* pBtnCancle	= new _XButton;
	pBtnCancle->Create( canclebtnstruct );
	pBtnCancle->SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 191, 196, 247, 216 );
	pBtnCancle->SetButtonImageClipRect( _XBUTTONIMAGE_READY,  191, 169, 247, 189 );
	pBtnCancle->SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  191, 223, 247, 243 );
	pBtnCancle->SetButtonTextID( _XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_CANCEL );	// _T("취소")
	pBtnCancle->SetButtonTextColor( 0xFFE0D7BA );
	InsertChildObject( pBtnCancle );	
	

	// 바탕 타일 생성
	m_BorderTile.Create( m_WindowPosition.x + 1, m_WindowPosition.y + 18, m_WindowPosition.x + 183, m_WindowPosition.y + 144);
	m_BorderTile.SetTexture( &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "MI_BorderTile.tga" ) );
	m_BorderTile.SetFaceColor( D3DCOLOR_ARGB( 128, 255,255,255 ) );
	
	FLOAT ufactor = 182.0f/128.0f;
	FLOAT vfactor = 144.0f/128.0f;
	
	m_BorderTile.ChangeUV( 0.0f,ufactor,ufactor,0.0f, 0.0f,0.0f,vfactor,vfactor );
	
	LoadEscapeInfo();
	return TRUE;
}

void _XWindow_Escape::DestroyWindow()
{
	_XWindow::DestroyWindow();
}

void _XWindow_Escape::Draw(_XGUIObject*& pfocusobject)
{
	if(this->m_ShowWindow)
	{
		m_BorderTile.RenderAlphaBlend(D3DCOLOR_ARGB( 128, 255,255,255 ));		
	}
	_XWindow::Draw(pfocusobject);
	if( !this->m_ShowWindow ) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;

	if( m_SelectedVillageName[0] != 0 )
	{
		g_XBaseFont->SetColor(0xFFC2A84E);
		g_XBaseFont->PutsAlign( m_WindowPosition.x + (m_WindowSize.cx>>1), m_WindowPosition.y + 55, _XFONT_ALIGNTYPE_CENTER, m_SelectedVillageName );
		g_XBaseFont->Flush();
	}
				

	if( m_pChargeGaugeWindow )
		m_pChargeGaugeWindow->Draw();
}

BOOL _XWindow_Escape::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	
	if(!this->m_ShowWindow)
		return FALSE;
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	if( m_SelectedAreaID != -1 )
	{
		if( m_pChargeGaugeWindow )
		{
			if(m_pChargeGaugeWindow->GetShowStatus())
			{
				if( m_pChargeGaugeWindow->GetGaugeDelayTime() != 0 )
				{

					if( g_LocalSystemTime - m_pChargeGaugeWindow->GetElapsedTime() >= m_pChargeGaugeWindow->GetGaugeDelayTime() )
					{
						// 플레이어 이동
						//_XArea* pArea = g_AreaManager.GetArea( _XDEF_AREACATEGORY_PLAYERSPAWN, 33 );
						_XArea* pArea = g_AreaManager.GetArea( _XDEF_AREACATEGORY_PLAYERSPAWN, m_SelectedAreaID );
						
						if( pArea )
						{
							this->ShowWindow(FALSE);
							
							g_DrawFadeInOutFlag = TRUE;
							
							for( g_FadeInOutLevel = 0;  g_FadeInOutLevel <= 255;  g_FadeInOutLevel += 24 )
							{			
								((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.Draw();
							}
							
							if(g_FadeInOutLevel >= 255)
							{
								g_FadeInOutLevel = 255;			
							}
							g_DrawFadeInOutFlag = FALSE;
							
							if( pArea->m_ShapeType == _XDEF_AREASHAPETYPE_BOX )
							{				
								g_pLocalUser->m_Position.x = ( pArea->m_Point[0].x + pArea->m_Point[2].x )/2;
								g_pLocalUser->m_Position.z = ( pArea->m_Point[0].z + pArea->m_Point[2].z )/2;
							}
							else
							{
								g_pLocalUser->m_Position.x = pArea->m_CenterPoint.x;
								g_pLocalUser->m_Position.z = pArea->m_CenterPoint.z;
							}
							
							_XOBBData*  pUnderObb = NULL;
							g_pLocalUser->m_Position.y = g_LodTerrain.GetTerrainAndRideOBBHeight( g_pLocalUser->m_Position, pUnderObb );
							g_pLocalUser->m_Velocity.y = 0.0f;
							
							g_pLocalUser->m_FinalTargetPosition = g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
							
							g_pLocalUser->SetAnimation(_XACTION_IDLE);
							g_pLocalUser->UpdatePosition();
							
							g_LodTerrain.m_3PCamera.SetTargetPosition( 
								D3DXVECTOR3(g_pLocalUser->m_Position.x, g_pLocalUser->m_Position.y +_XDEF_CHARACTERAVERAGEHEIGHT_CAMERA, g_pLocalUser->m_Position.z ) );
							
							// reset camera rotate & zoom velocity...
							g_LodTerrain.m_3PCamera.m_CameraVelocity = D3DXVECTOR3( 0.0f,0.0f,0.0f );
							g_LodTerrain.m_3PCamera.m_ZoominoutVelocity = 0.0f;	
							
							g_LodTerrain.m_3PCamera.UpdateViewMatrix( &g_LodTerrain );
							g_LodTerrain.m_3PCamera.UpdateProjMatrix();
							
							g_LodTerrain.m_ObjectQuadTree.UpdateCamera( g_LodTerrain.m_3PCamera );
							g_LodTerrain.RebuildLevel(FALSE);
							
							_XWindow_WorldMinimap* pMinimap_Window = (_XWindow_WorldMinimap*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MINIMAPWINDOW);
							if(pMinimap_Window)
							{
								float rotateangle = _XMeshMath_GetRotateRadian(g_pLocalUser->m_Position, g_pLocalUser->m_TargetPosition);
								pMinimap_Window->SetRotateArrow(rotateangle + _X_PI);
								pMinimap_Window->SetLocalUserPosition();
							}
							
							g_pLocalUser->m_FinalTargetPosition = g_pLocalUser->m_TargetPosition = g_pLocalUser->m_Position;
							g_pLocalUser->m_pGoundCollideOBB = pUnderObb;
							g_pLocalUser->m_PrevPosition = g_pLocalUser->m_Position;
							
							
							
							g_DrawFadeInOutFlag = TRUE;
							for( g_FadeInOutLevel = 255;  g_FadeInOutLevel >= 0;  g_FadeInOutLevel -= 24 )
							{		
								((CEmperorOfDragonsApp*)gApp)->m_proc_maingame.Draw();
							}
							
							if(g_FadeInOutLevel <= 0)
							{
								g_FadeInOutLevel = 0;	
							}
							g_DrawFadeInOutFlag = FALSE;
						}//if( pArea )
					}//if( g_LocalSystemTime - m_pChargeGaugeWindow->GetElapsedTime() >= m_pChargeGaugeWindow->GetGaugeDelayTime() )
				}//if( m_pChargeGaugeWindow->GetGaugeDelayTime() != 0 )
			}//if(m_pChargeGaugeWindow->GetShowStatus())
		}//if( m_pChargeGaugeWindow )
	}//if( m_SelectedAreaID != -1 )

	return TRUE;
}

void _XWindow_Escape::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
	m_BorderTile.MoveWindow( m_WindowPosition.x + 1, m_WindowPosition.y + 18, m_WindowPosition.x + 183, m_WindowPosition.y + 144 );
	if( m_pChargeGaugeWindow )
	{
		POINT pos = this->GetWindowPos();
		pos.x += 12;
		pos.y += 81;
		m_pChargeGaugeWindow->SetDrawPosition(pos);
	}
}

void _XWindow_Escape::ShowWindow(BOOL show)
{	
	if( show )
	{
		SetSelectedEscapeInfo();
		m_pChargeGaugeWindow = (_XWindow_ChargeGauge*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_CHARGEGAUGE);
		if( m_pChargeGaugeWindow )
		{
			if(!m_pChargeGaugeWindow->GetShowStatus())
			{
				//m_pChargeGaugeWindow->SetDelayTime( 12000.0f );
				//Author : 양희왕 //breif : 탈출인데.. 일본은 10분으로 늘려달랜다
				#if defined(_XJAPANESE)
				m_pChargeGaugeWindow->SetDelayTime( 600000.0f ); //6백초..
				#else
				m_pChargeGaugeWindow->SetDelayTime( 300000.0f );
				#endif
				
				POINT pos = this->GetWindowPos();
				pos.x += 12;
				pos.y += 81;
				m_pChargeGaugeWindow->SetDrawPosition(pos);
				m_pChargeGaugeWindow->SetGaugeMode( _XGAUGE_ESCAPE );
				m_pChargeGaugeWindow->ShowWindow(TRUE);
				g_MainWindowManager.SetTopWindow( m_pChargeGaugeWindow );
			}
		}

	}
	else
	{
		m_SelectedVillageStringID	= -1;	//현재 선택된 지역에서 이동될 마을 이름
		m_SelectedAreaID	= -1;			//현재 선택된 지영게서 이동될 영역 아이디
		
		if( m_pChargeGaugeWindow )
		{
			m_pChargeGaugeWindow->ShowWindow(FALSE);
			m_pChargeGaugeWindow = NULL;
		}

	}
	_XWindow::ShowWindow(show);
}



BOOL _XWindow_Escape::LoadEscapeInfo( void )
{	
	_XMEMORYUSECHECKREADY

	_XMEMORYUSECHECKSTART
		
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;
	
	FILE* fileptr = NULL;	
	fileptr = g_ScriptArchive.GetPackedFile( _T("escape.txt") );
	
	if( !fileptr ) return FALSE;
	
	TCHAR buffer[1024];
	int tempindex = 0;
	_XEscapeInfo* newescapeinfo = NULL;
	do 
	{
		if(feof(fileptr))
			break;
		
		memset(buffer, 0, sizeof(TCHAR)*1024);
		if(!fgets(buffer, 1024, fileptr))
			break;
		
		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;
		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			newescapeinfo = new _XEscapeInfo;
			sscanf( buffer, "%d %d %d %d %d %d %d", 
				&newescapeinfo->currentmapid,				// 현재 맵 인덱스
				&newescapeinfo->village_stringid_01,		// 공통(낭인)
				&newescapeinfo->village_stringid_02,		// 백도
				&newescapeinfo->village_stringid_03,		// 흑도
				&newescapeinfo->areaid_common,				// 공통
				&newescapeinfo->areaid_white,				// 백도
				&newescapeinfo->areaid_black				// 백도
				);
			m_listEscapeInfo.push_back(newescapeinfo);
						
		}
	} while(!feof(fileptr));
	
	
	g_ScriptArchive.ClosePackage();
	_XMEMORYUSECHECKEND("Load escapeinfo package script");
	return TRUE;
}

//현재 선택된 지역에서 이동될 마을 이름과 영역 아이디를 얻어 온다.
void _XWindow_Escape::SetSelectedEscapeInfo(void)
{	
	memset( m_SelectedVillageName, 0, sizeof(TCHAR)*512 );

	_XEscapeInfo* currentescapeinfo = NULL;
	list <_XEscapeInfo*>::iterator iter_escapeinfo;
	for(iter_escapeinfo = m_listEscapeInfo.begin() ; iter_escapeinfo != m_listEscapeInfo.end() ; ++iter_escapeinfo)
	{
		currentescapeinfo = *iter_escapeinfo;
		if( currentescapeinfo )
		{
			// 현재 지역이 탈출 정보에 있는지 검색한다.
			if( currentescapeinfo->currentmapid == g_CurrentZoneInfoIndex )
			{
				//플래이어가 낭인인지, 백도인지 흑도인지 확인한다.
				int grouptype = g_pLocalUser->GetGroupType();
				if( grouptype == _XGROUPTYPE_NONE )
				{
					m_SelectedVillageStringID = currentescapeinfo->village_stringid_01;
					m_SelectedAreaID = currentescapeinfo->areaid_common;
					
					sprintf( m_SelectedVillageName, _T("[ %s ]"), _XGETINTERFACETEXT(m_SelectedVillageStringID) );
				}
				else if( grouptype == _XGROUPTYPE_WHITE)
				{
					m_SelectedVillageStringID = currentescapeinfo->village_stringid_02;
					m_SelectedAreaID = currentescapeinfo->areaid_white;
					sprintf( m_SelectedVillageName, _T("[ %s ]"), _XGETINTERFACETEXT(m_SelectedVillageStringID) );
				}
				else if( grouptype == _XGROUPTYPE_BLACK)
				{
					m_SelectedVillageStringID = currentescapeinfo->village_stringid_03;
					m_SelectedAreaID = currentescapeinfo->areaid_black;
					sprintf( m_SelectedVillageName, _T("[ %s ]"), _XGETINTERFACETEXT(m_SelectedVillageStringID) );
				}
				break;
					
			}
		}
	}
}
