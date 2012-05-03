// XWindow_MoveVillage.cpp: implementation of the _XWindow_MoveVillage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_MoveVillage.h"
#include "XSR_STRINGHEADER.H"
#include "Xwindow_Escape.h"
#include "XWindow_MatchPR.h"

extern void ReplaceCharacter(LPTSTR _string, TCHAR srcchar, TCHAR changechar);

BOOL __stdcall _VillageListBoxItemMouseHoverCallback( int param1, int param2, int XPos, int YPos )
{
	if( param2 == -1 )
	{
		
	}
	else
	{
		_XWindow_MoveVillage* pMoveVillage_Window = (_XWindow_MoveVillage*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_MOVEVILLAGE);
		if(pMoveVillage_Window)
		{
			LPTSTR szitemtext = pMoveVillage_Window->m_lstboxVillage->GetItemText( param2, 0 );
			if( szitemtext )
			{
				int strwidth = g_XBaseFont->GetWidth( szitemtext );
				
				if( strwidth >= 89 )
				{
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
					_XDrawSolidBar( XPos, YPos, XPos+strwidth+4, YPos+15 , D3DCOLOR_ARGB(255,0,0,0) );
					_XDrawRectAngle( XPos, YPos, XPos+strwidth+4, YPos+15 , 0.0f, D3DCOLOR_ARGB(255,170,164,154) );
						
					g_XBaseFont->Puts( XPos+0, YPos+2, szitemtext );
						
					if( param2 == pMoveVillage_Window->m_lstboxVillage->GetSelectedItem() )
					{
						g_XBaseFont->Puts( XPos+1, YPos+2, szitemtext );
					}
					
					g_XBaseFont->Flush();
				}
			}
		}
	}
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_MoveVillage::_XWindow_MoveVillage()
{
	m_lstboxVillage		= NULL;
	m_btnMove			= NULL;
	m_btnCancel			= NULL;
	m_pMapTexture		= NULL;
	m_CurrentMapIndex	= 0;
	m_SelectedMoveVillageIndex = -1;
	m_SelectedInvenNumber = -1;

	m_SelectedMapIndex	= -1;
	m_nMaxIndex			= 0;
	m_fScaleRate		= 0;

	m_MarkSmall			= NULL;
	m_MarkLarge			= NULL;
}

_XWindow_MoveVillage::~_XWindow_MoveVillage()
{

}

BOOL _XWindow_MoveVillage::LoadMoveVillageInfo(void)
{
	if( !g_ScriptArchive.ReOpenPackage() )
			return FALSE;
	
#ifndef _XDWDEBUG
	if( g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"deleteoldscript", 1, FALSE ) )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);
		_XDeleteFile(_T("Move_Village.xms"));
	}		
#endif
	
	BOOL bLoadFromFile = g_RegManager._XRegReadInt(ID_REGSTR_MASTERKEY,ID_REGSTR_INDY21KEY,"loadseparatedscript", 0, FALSE );
	
	FILE* fileptr = NULL;
	
	
	TCHAR scriptfilename[32];
	
	memset( scriptfilename, 0, sizeof(TCHAR) * 32 );
	strcpy( scriptfilename, _T("Move_Village.xms") );
	
#ifdef _XTESTSERVER
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( scriptfilename, "rt" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;			
			fileptr = g_ScriptArchive.GetPackedFile( scriptfilename );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( scriptfilename );
	}
#else	
	if( bLoadFromFile )
	{
		_XSetDefaultPath(_XDEF_DEFAULTPATH_SCRIPTER);	
		fileptr = fopen( scriptfilename, "rt" );
		
		if( !fileptr )
		{
			bLoadFromFile = FALSE;
			fileptr = g_ScriptArchive.GetPackedFile( scriptfilename );
		}
	}
	else
	{
		fileptr = g_ScriptArchive.GetPackedFile( scriptfilename );
	}
#endif
	
	if( !fileptr )
	{
		_XFatalError("Can't load Move_Village");
		if( !bLoadFromFile )
			g_ScriptArchive.ClosePackage();
		return FALSE;
	}

	TCHAR buffer[256];
	TCHAR tempname_1[128], tempname_2[128];

	do 
	{
		if(feof(fileptr))
			break;

		memset(buffer, 0, sizeof(buffer));
		if(!fgets(buffer, 256, fileptr))
			break;

		if(strncmp(buffer, _T("_XM_EOS"), 7) == 0)
			break;
		
		if((strlen(buffer) > 0) && buffer[0] != _T(';') && buffer[0] != _T('\n') && buffer[0] != _T('\r'))
		{
			_XMoveVillageInfo* newinfo = new _XMoveVillageInfo;
#ifdef _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI
			sscanf(buffer, "%d %s %s %d %d %d %d %d %d %d %d %d", &newinfo->id, tempname_1, tempname_2, &newinfo->mapid, &newinfo->strmapid,
				&newinfo->strvillageid, &newinfo->areaid, &newinfo->property, &newinfo->level, &newinfo->innerlevel,
				&newinfo->posx, &newinfo->posy);
#else
			sscanf(buffer, "%d %s %s %d %d %d %d %d %d %d", &newinfo->id, tempname_1, tempname_2, &newinfo->mapid, &newinfo->strmapid,
				&newinfo->strvillageid, &newinfo->areaid, &newinfo->property, &newinfo->level, &newinfo->innerlevel);
#endif
			
#ifdef _XTAIWANESE
			// 대만은 _ 캐릭터가 한자와 중복되어 제거됨. 공백을 사용 안함.
#else
			ReplaceCharacter(tempname_1, _T('_'), _T(' ')); //Author : 양희왕 //breif : 미국 표국이동 들어가면서 추가
			ReplaceCharacter(tempname_2, _T('_'), _T(' '));
#endif

			if(m_nMaxIndex < newinfo->id)
				m_nMaxIndex = newinfo->id;	// max index 저장하기

			newinfo->posx *= m_fScaleRate;
			newinfo->posy *= m_fScaleRate;

			SetMapImage( newinfo->mapid, newinfo->pMapTexture );

			m_MoveVillageInfo.push_back(newinfo);
		}
	} while(!feof(fileptr));

	if(bLoadFromFile) 
		fclose(fileptr);
	else 
		g_ScriptArchive.ClosePackage();

	return TRUE;
}

BOOL _XWindow_MoveVillage::BuildMapInfo()
{
	vector <_XMoveVillageInfo*>::iterator iter_info;
	for(iter_info = m_MoveVillageInfo.begin() ; iter_info != m_MoveVillageInfo.end() ; ++iter_info)
	{
		_XMoveVillageInfo* villageinfo = *iter_info;
		if(villageinfo)
		{
			map <int, _XMapInfo*>::iterator iter_map = m_MapInfo.find(villageinfo->mapid);
			if(iter_map != m_MapInfo.end())
			{
				_XMapInfo* mapinfo = iter_map->second;
				mapinfo->villageindex = villageinfo->id-1;
			}
			else
			{
				_XMapInfo* newmapinfo = new _XMapInfo;
				newmapinfo->mapid = villageinfo->mapid;
				newmapinfo->strmapid = villageinfo->strmapid;
				newmapinfo->villageindex = villageinfo->id-1;
				newmapinfo->expand = FALSE;
				m_MapInfo[villageinfo->mapid] = newmapinfo;
			}
		}
	}

	return TRUE;
}

BOOL _XWindow_MoveVillage::Initialize()
{
	int resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_skillwin.tga");
	int main_resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_main_back01.tga");

#ifdef _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI
	_XImageStatic* pTitleLeftBar = new _XImageStatic;
	pTitleLeftBar->Create(0, 0, 180, 18, &g_MainInterfaceTextureArchive, main_resourceindex);
	pTitleLeftBar->SetClipRect(0, 125, 180, 143);
	InsertChildObject(pTitleLeftBar);

	_XImageStatic* pTitleCenterBar = new _XImageStatic;
	pTitleCenterBar->Create(180, 0, 472, 18, &g_MainInterfaceTextureArchive, main_resourceindex);
	pTitleCenterBar->SetClipRect(77, 125, 179, 143);
	FLOAT scale = 292.0f / 102.0f;
	pTitleCenterBar->SetScale(scale, 1.0f);
	InsertChildObject(pTitleCenterBar);

	_XImageStatic* pTitleRightBar = new _XImageStatic;
	pTitleRightBar->Create(472, 0, 544, 18, &g_MainInterfaceTextureArchive, main_resourceindex);
	pTitleRightBar->SetClipRect(110, 125, 183, 143);
	InsertChildObject(pTitleRightBar);

	_XImageStatic* pLeftBorder_1 = new _XImageStatic;
	pLeftBorder_1->Create(0, 18, 2, 277, &g_MainInterfaceTextureArchive, main_resourceindex);
	pLeftBorder_1->SetClipRect(0, 195, 3, 227);
	scale = 259.0f/32.0f;
	pLeftBorder_1->SetScale(1.0f, scale);
	InsertChildObject(pLeftBorder_1);

	_XImageStatic* pRightBorder_1 = new _XImageStatic;
	pRightBorder_1->Create(542, 18, 545, 277, &g_MainInterfaceTextureArchive, main_resourceindex);
	pRightBorder_1->SetClipRect(0, 195, 3, 227);
	pRightBorder_1->SetScale(1.0f, scale);
	InsertChildObject(pRightBorder_1);

	_XImageStatic* pLeftCorner = new _XImageStatic;
	pLeftCorner->Create(0, 277, 2, 280, &g_MainInterfaceTextureArchive, main_resourceindex);
	pLeftCorner->SetClipRect(0, 227, 3, 230);
	InsertChildObject(pLeftCorner);

	_XImageStatic* pRightCorner = new _XImageStatic;
	pRightCorner->Create(542, 277, 545, 280, &g_MainInterfaceTextureArchive, main_resourceindex);
	pRightCorner->SetClipRect(180, 227, 183, 230);
	InsertChildObject(pRightCorner);

	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create(3, 277, 179, 280, &g_MainInterfaceTextureArchive, main_resourceindex);
	pBottomBorder->SetClipRect(3, 227, 180, 230);
	scale = 539.0f/177.0f;
	pBottomBorder->SetScale(scale, 1.0f);
	InsertChildObject(pBottomBorder);

	m_BorderTitle.Create(m_WindowPosition.x+3, m_WindowPosition.y+18, m_WindowPosition.x+542, m_WindowPosition.y+277);
	m_BorderTitle.SetTexture(&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("MI_BorderTile.tga"));
	m_BorderTitle.SetFaceColor(D3DCOLOR_ARGB(128, 255, 255, 255));
	
	FLOAT ufactor = 539.0f / 128.0f;
	FLOAT vfactor = 259.0f / 128.0f;

	m_BorderTitle.ChangeUV(0.0f, ufactor, ufactor, 0.0f, 0.0f, 0.0f, vfactor, vfactor);

#else

	_XImageStatic* pTitleLeftBar = new _XImageStatic;
	pTitleLeftBar->Create(0, 0, 180, 18, &g_MainInterfaceTextureArchive, main_resourceindex);
	pTitleLeftBar->SetClipRect(0, 125, 180, 143);
	InsertChildObject(pTitleLeftBar);

	_XImageStatic* pTitleCenterBar = new _XImageStatic;
	pTitleCenterBar->Create(180, 0, 302, 18, &g_MainInterfaceTextureArchive, main_resourceindex);
	pTitleCenterBar->SetClipRect(77, 125, 180, 143);
	FLOAT scale = 122.0f / 103.0f;
	pTitleCenterBar->SetScale(scale, 1.0f);
	InsertChildObject(pTitleCenterBar);

	_XImageStatic* pTitleRightBar = new _XImageStatic;
	pTitleRightBar->Create(302, 0, 374, 18, &g_MainInterfaceTextureArchive, main_resourceindex);
	pTitleRightBar->SetClipRect(110, 125, 183, 143);
	InsertChildObject(pTitleRightBar);

	_XImageStatic* pLeftBorder_1 = new _XImageStatic;
	pLeftBorder_1->Create(0, 18, 2, 277, &g_MainInterfaceTextureArchive, main_resourceindex);
	pLeftBorder_1->SetClipRect(0, 195, 3, 227);
	scale = 259.0f/32.0f;
	pLeftBorder_1->SetScale(1.0f, scale);
	InsertChildObject(pLeftBorder_1);

	_XImageStatic* pRightBorder_1 = new _XImageStatic;
	pRightBorder_1->Create(372, 18, 375, 277, &g_MainInterfaceTextureArchive, main_resourceindex);
	pRightBorder_1->SetClipRect(0, 195, 3, 227);
	pRightBorder_1->SetScale(1.0f, scale);
	InsertChildObject(pRightBorder_1);

	_XImageStatic* pLeftCorner = new _XImageStatic;
	pLeftCorner->Create(0, 277, 2, 280, &g_MainInterfaceTextureArchive, main_resourceindex);
	pLeftCorner->SetClipRect(0, 227, 3, 230);
	InsertChildObject(pLeftCorner);

	_XImageStatic* pRightCorner = new _XImageStatic;
	pRightCorner->Create(372, 277, 375, 280, &g_MainInterfaceTextureArchive, main_resourceindex);
	pRightCorner->SetClipRect(180, 227, 183, 230);
	InsertChildObject(pRightCorner);

	_XImageStatic* pBottomBorder = new _XImageStatic;
	pBottomBorder->Create(3, 277, 179, 280, &g_MainInterfaceTextureArchive, main_resourceindex);
	pBottomBorder->SetClipRect(3, 227, 180, 230);
	scale = 369.0f/177.0f;
	pBottomBorder->SetScale(scale, 1.0f);
	InsertChildObject(pBottomBorder);

	m_BorderTitle.Create(m_WindowPosition.x+3, m_WindowPosition.y+18, m_WindowPosition.x+372, m_WindowPosition.y+277);
	m_BorderTitle.SetTexture(&g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("MI_BorderTile.tga"));
	m_BorderTitle.SetFaceColor(D3DCOLOR_ARGB(128, 255, 255, 255));
	
	FLOAT ufactor = 369.0f / 128.0f;
	FLOAT vfactor = 259.0f / 128.0f;

	m_BorderTitle.ChangeUV(0.0f, ufactor, ufactor, 0.0f, 0.0f, 0.0f, vfactor, vfactor);
#endif

	// map image
	m_imgMap.Create(0, 0, 242, 242, &g_MainInterfaceTextureArchive, -1);
	m_imgMap.SetClipRect( 0, 0, 512, 512 );
	//scale = 242.0f/512.0f;(gnWidth>>1)
	scale = 242.0f/(float)(gnWidth>>1);
	m_imgMap.SetScale(scale, scale);

	m_fScaleRate = scale;

	// list box
	int scrollbar_resourceindex = g_MainInterfaceTextureArchive.FindResource("mi_chat_quick.tga");
	_XLISTBOX_STRUCTURE listboxstruct = 
	{
		TRUE,							// 윈도우 활성화
		{26, 29},						// 윈도우 좌표 
		{89, 198},						// 윈도우 사이즈
		_XDEF_MOVEVILLAGE_LISTBOX,		// 윈도우 아이디
		-1,								// 윈도우 볼더 이미지 인덱스
		&g_MainInterfaceTextureArchive,	// 텍스쳐리스트
		_T(""),							// 윈도우 타이틀 
		16,								// 리스트 아이템 라인 피치
		14,								// 리스트 박스에 보이는 최대 라인수 
		_XLSTYLE_RIGHTSCROLLBAR,		// 스크롤바의 위치 

		// Scroll bar property
		{12, 198},						// 스크롤바 사이즈
		12,								// 스크롤바 상하버튼 사이즈
		22,								// 스크롤바 트랙바 사이즈
		150,							// 전체 리스트 갯수 <설정 필요없슴>			
		scrollbar_resourceindex,
		scrollbar_resourceindex,
		scrollbar_resourceindex,
		scrollbar_resourceindex,
		scrollbar_resourceindex
	};

#ifdef _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI
	listboxstruct.position.x = 14;
	listboxstruct.position.y = 20;
	listboxstruct.windowsize.cx = 280;
	listboxstruct.windowsize.cy = 222;

	listboxstruct.scrollbarsize.cy = 222;
#endif

	m_lstboxVillage = new _XListBox;
	m_lstboxVillage->Create(listboxstruct);

#ifndef _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI	// renewal이 선택되지 않았을때만 작동
	m_lstboxVillage->SetSelectItemMouseHoverCallBack(_VillageListBoxItemMouseHoverCallback);
#endif

	_XVScrollBar* pScrollBar = m_lstboxVillage->GetScrollBarObject();
	if( pScrollBar )
	{
		_XSCRBAR_IMAGECLIPRECTSTRUCTURE cliprectstructure = 
		{
			{ 0,  0, 12, 12 },
			{ 24, 0, 36, 12 },
			{ 0,  12, 12, 24 },
			{ 24, 12, 36, 24 },
			{ 40,157, 52, 179}			
		};
		pScrollBar->SetClipRect( cliprectstructure );	
	}

	POINT columnoffset = {0, 2};

#ifdef _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI
	columnoffset.x = 5;
//	columnoffset.y = 9;
#endif

	m_lstboxVillage->SetColumnStyle(0, _XLCOLUMNSTYLE_STRING);
	m_lstboxVillage->SetColumnOffset(0, columnoffset);
	m_lstboxVillage->SetAlignType(0, _XFONT_ALIGNTYPE_LEFT);
//	m_lstboxVillage->SetColumnWidth(0, 2);

	m_lstboxVillage->SetColumnWidth(0, 261);

	m_lstboxVillage->SetTrackWidth(260);
	m_lstboxVillage->SetTrackHeight(14);

	InsertChildObject(m_lstboxVillage);

	_XBTN_STRUCTURE btnstruct =
	{
		TRUE, 
		{14, 242}, 
		{108, 24},
		_XDEF_MOVEVILLAGE_MOVEBUTTON,
		resourceindex,
		resourceindex,
		resourceindex,
		&g_MainInterfaceTextureArchive
	};

#ifdef _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI
	btnstruct.position.x = 20;
	btnstruct.position.y = 249;
#endif

	m_btnMove = new _XButton;
	m_btnMove->Create(btnstruct);
	m_btnMove->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 176, 110, 200); 
	m_btnMove->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 152, 110, 176);
	m_btnMove->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 200, 110, 224);
	m_btnMove->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_NEW_3043);
	m_btnMove->SetGrowTextMode(TRUE, 0xFF000000);
	m_btnMove->SetButtonTextColor(D3DCOLOR_ARGB(255, 216, 216, 216));
	InsertChildObject(m_btnMove);

#ifdef _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI
	btnstruct.position.x = 157;
	btnstruct.position.y = 249;
	btnstruct.commandid = _XDEF_MOVEVILLAGE_CANCEL;

	m_btnCancel = new _XButton;
	m_btnCancel->Create(btnstruct);
	m_btnCancel->SetButtonImageClipRect(_XBUTTONIMAGE_NORMAL, 2, 176, 110, 200); 
	m_btnCancel->SetButtonImageClipRect(_XBUTTONIMAGE_READY, 2, 152, 110, 176);
	m_btnCancel->SetButtonImageClipRect(_XBUTTONIMAGE_CLICK, 2, 200, 110, 224);
	m_btnCancel->SetButtonTextID(_XSTRINGDB_SN_EODSTRING_XSR, ID_STRING_ITEMDROPCONFIRM_CANCEL);
	m_btnCancel->SetGrowTextMode(TRUE, 0xFF000000);
	m_btnCancel->SetButtonTextColor(D3DCOLOR_ARGB(255, 216, 216, 216));
	InsertChildObject(m_btnCancel);
#endif

#ifdef _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI
	m_MarkSmall = new _XImageStatic;
	m_MarkSmall->Create(0, 0, 32, 32, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Tutorial_01.tga"));
	m_MarkSmall->SetClipRect(60, 115, 92, 147);

	m_MarkLarge = new _XImageStatic;
	m_MarkLarge->Create(0, 0, 32, 32, &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource("Tutorial_01.tga"));
	m_MarkLarge->SetClipRect(94, 115, 126, 147);
#endif

	LoadMoveVillageInfo();
	BuildMapInfo();
	
	return TRUE;
}

void _XWindow_MoveVillage::DestroyWindow()
{
	vector <_XMoveVillageInfo*>::iterator iter_info;
	for(iter_info = m_MoveVillageInfo.begin() ; iter_info != m_MoveVillageInfo.end() ; ++iter_info)
	{
		_XMoveVillageInfo* currentinfo = *iter_info;
		//Author : 양희왕 //breif : 추가
		if( currentinfo->pMapTexture )
		{
			currentinfo->pMapTexture->Release();
			currentinfo->pMapTexture = NULL;
		}

		SAFE_DELETE(currentinfo);
	}
	m_MoveVillageInfo.clear();
	
	map <int, _XMapInfo*>::iterator iter_map;
	for(iter_map = m_MapInfo.begin() ; iter_map != m_MapInfo.end() ; ++iter_map)
	{
		_XMapInfo* currentmapinfo = iter_map->second;
		SAFE_DELETE(currentmapinfo);
	}
	m_MapInfo.clear();

//	SAFE_RELEASE(m_pMapTexture);

	SAFE_DELETE(m_MarkSmall);
	SAFE_DELETE(m_MarkLarge);

	_XWindow::DestroyWindow();
}

void _XWindow_MoveVillage::Draw(_XGUIObject*& pfocusobject)
{
	if(m_ShowWindow)
	{
		m_BorderTitle.RenderAlphaBlend(D3DCOLOR_ARGB(128, 255, 255, 255));

#ifdef _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI
		_XDrawSolidBar(m_WindowPosition.x+15, m_WindowPosition.y+20, m_WindowPosition.x+294, m_WindowPosition.y+242, D3DCOLOR_ARGB(127, 0, 0, 0));
		_XDrawRectAngle(m_WindowPosition.x+14, m_WindowPosition.y+19, m_WindowPosition.x+294, m_WindowPosition.y+242, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
#else
		_XDrawSolidBar(m_WindowPosition.x+15, m_WindowPosition.y+26, m_WindowPosition.x+120, m_WindowPosition.y+235, D3DCOLOR_ARGB(127, 0, 0, 0));
		_XDrawRectAngle(m_WindowPosition.x+14, m_WindowPosition.y+25, m_WindowPosition.x+120, m_WindowPosition.y+235, 0.0f, D3DCOLOR_ARGB(255, 0, 0, 0));
#endif
	}

	_XWindow::Draw(pfocusobject);

#ifdef _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI
	if(m_ShowWindow)
	{
		m_imgMap.Draw(m_WindowPosition.x+297, m_WindowPosition.y+22, m_pMapTexture);

		if(m_SelectedMoveVillageIndex >= 0)
		{
			if(m_SelectedMoveVillageIndex <= m_nMaxIndex)
			{
				m_MarkLarge->Draw(m_WindowPosition.x+297+m_MoveVillageInfo[m_SelectedMoveVillageIndex]->posx - 16,  
					m_WindowPosition.y+22+m_MoveVillageInfo[m_SelectedMoveVillageIndex]->posy - 16);
				m_MarkSmall->Draw(m_WindowPosition.x+297+m_MoveVillageInfo[m_SelectedMoveVillageIndex]->posx - 16,  
					m_WindowPosition.y+22+m_MoveVillageInfo[m_SelectedMoveVillageIndex]->posy - 16);
			}
		}
	}
#else
	if(m_ShowWindow)
	{
		m_imgMap.Draw(m_WindowPosition.x+126, m_WindowPosition.y+22, m_pMapTexture);
	}
#endif

	if(!this->m_ShowWindow)
		return;
	if(m_WindowAnimationType != _XW_ANIMTYPE_NONE)
		return;

	// TODO
}

BOOL _XWindow_MoveVillage::Process(_XGUIObject*& pfocusobject)
{
	if(!this->m_Enable)
		return FALSE;
	if(!this->m_ShowWindow)
		return FALSE;

	// TODO

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_MoveVillage::ShowWindow(BOOL show)
{
	if(show)
	{
		map <int, _XMapInfo*>::iterator iter_map;
		for(iter_map = m_MapInfo.begin() ; iter_map != m_MapInfo.end() ; ++iter_map)
		{
			_XMapInfo* mapinfo = iter_map->second;
			if(mapinfo)
			{
				mapinfo->expand = FALSE;
			}
		}

		//SetMapImage(1);
		m_pMapTexture = NULL;
		SetListItem();
		m_lstboxVillage->SetScrollPos(0);
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_MoveVillage::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
	m_BorderTitle.MoveWindow(X+3, Y+18);
}

BOOL _XWindow_MoveVillage::SetMapImage(int mapindex, LPDIRECT3DTEXTURE9& pTexture)
{
	//if(m_CurrentMapIndex == mapindex)
	//	return TRUE;

	TCHAR	mapnamestr[_MAX_PATH];
	memset(mapnamestr, 0, sizeof(mapnamestr));

	if(g_LanguageType == _XLANGUAGE_TYPE_KOREAN)
	{
		strcpy(mapnamestr, "Mini");
	}
	else if(g_LanguageType == _XLANGUAGE_TYPE_VIETNAMESE)
	{
		strcpy(mapnamestr, "vn_Mini");
	}
	else if(g_LanguageType == _XLANGUAGE_TYPE_ENGLISH)
	{
		strcpy(mapnamestr, "us_Mini");
	}
	else if(g_LanguageType == _XLANGUAGE_TYPE_TAIWANESE)
	{
		strcpy(mapnamestr, "tw_Mini");
	}
	else if( g_LanguageType == _XLANGUAGE_TYPE_JAPANESE )
	{
		strcpy( mapnamestr, "jp_Mini" );
	}
	else if(g_LanguageType == _XLANGUAGE_TYPE_RUSSIAN)
	{
		strcpy(mapnamestr, "rs_Mini");
	}
	else
	{
		strcpy(mapnamestr, "Mini");
	}

	strcat(mapnamestr, g_ZoneInfoTable[mapindex-1].zonefilename);

	LPTSTR cutpoint = strstr(mapnamestr, "xtv");
	if(cutpoint)
	{
		*cutpoint = NULL;
	}

	strcat(mapnamestr, "tga");
	
	_XPackageArchive texturearchive;

	TCHAR packagefilename[_MAX_PATH];
	packagefilename[0] = NULL;
	strcpy( packagefilename, gModulePath );
	strcat( packagefilename, _T("\\Data\\Texture\\") );
	strcat( packagefilename, _XDEF_TEXTUREFILE_A );

	if(!texturearchive.OpenPackage(packagefilename))
		return FALSE;

	int resourceindex = texturearchive.FindResource(mapnamestr);
	if(resourceindex < 0)
	{
		strcpy(mapnamestr, "Mini");
		strcat(mapnamestr, g_ZoneInfoTable[mapindex-1].zonefilename);

		LPTSTR cutpoint = strstr(mapnamestr, "xtv");
		if(cutpoint)
		{
			*cutpoint = NULL;
		}

		resourceindex = texturearchive.FindResource(mapnamestr);
		strcat(mapnamestr, "tga");

		if(resourceindex < 0)
			return FALSE;
	}

	int filesize = texturearchive.GetPackedFileSize(resourceindex);
	FILE* fp = texturearchive.GetPackedFile(resourceindex);

	LPBYTE pImageVFMemory = NULL;
	pImageVFMemory = new BYTE[filesize];
	if(!pImageVFMemory)
	{
		texturearchive.FinalizePackage();
		return FALSE;
	}

	if(fread(pImageVFMemory, filesize, 1, fp) < 1)
	{
		SAFE_DELETE_ARRAY(pImageVFMemory);
		texturearchive.FinalizePackage();
		return FALSE;
	}

	texturearchive.FinalizePackage();

	//SAFE_RELEASE(m_pMapTexture);
	SAFE_RELEASE(pTexture);

	if( FAILED( D3DXCreateTextureFromFileInMemoryEx( gpDev, pImageVFMemory, filesize, 
		D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
		D3DX_FILTER_NONE, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
		D3DX_FILTER_NONE, //D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
		0, NULL, NULL, &pTexture) ) )//&m_pMapTexture ) ) )
	{
		SAFE_DELETE_ARRAY(pImageVFMemory);
		return FALSE;
	}

	SAFE_DELETE_ARRAY(pImageVFMemory);

	//m_CurrentMapIndex = mapindex;

	return TRUE;
}

#ifdef _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI

void _XWindow_MoveVillage::SetListItem()
{
	m_lstboxVillage->DeleteAllItem();
	m_lstboxVillage->SetScrollPos(0);

	m_SelectedMapIndex = -1;
	m_SelectedMoveVillageIndex = -1;

	int count = 0;

	vector <_XMoveVillageInfo*>::iterator iter_info;
	for(iter_info = m_MoveVillageInfo.begin() ; iter_info != m_MoveVillageInfo.end() ; ++iter_info)
	{
		_XMoveVillageInfo* villageinfo = *iter_info;
		if(villageinfo)
		{
			BOOL bInsertLevel = FALSE, bInsertProperty = FALSE, bInsert = FALSE;

			if(!bInsert)
			{
				if(villageinfo->innerlevel <= g_pLocalUser->m_CharacterInfo.Get_innerlevel())
				{
					bInsertLevel = TRUE;
				}

				if(villageinfo->property == (int)g_pLocalUser->GetGroupType())
				{
					bInsertProperty = TRUE;
				}
				else
				{
					bInsertProperty = FALSE;
				}
				
				if(villageinfo->property == 0 || g_pLocalUser->GetGroupType() == _XGROUPTYPE_NONE)	// 전부or낭인
				{
					bInsertProperty = TRUE;		
				}

				if(bInsertLevel && bInsertProperty)
				{
					bInsert = TRUE;
				}
			}

			if(bInsert)
			{
				DWORD commandid = villageinfo->id;

				TCHAR name[256];
				memset(name, 0, sizeof(name));

				LPTSTR strTempMap = _XGETINTERFACETEXT(villageinfo->strmapid);
				LPTSTR strTempVillage = _XGETINTERFACETEXT(villageinfo->strvillageid);

#ifdef _XTAIWANESE
			// 대만은 _ 캐릭터가 한자와 중복되어 제거됨. 공백을 사용 안함.
#else				
				ReplaceCharacter(strTempMap, _T('_'), _T(' ')); //Author : 양희왕 //breif : 미국 표국이동 들어가면서 추가
				ReplaceCharacter(strTempVillage, _T('_'), _T(' '));
#endif

				sprintf(name, "*%s:%s(Lv.%d)", strTempMap, strTempVillage, villageinfo->innerlevel);

				m_lstboxVillage->InsertListItem(_T(""), 0, commandid);
				m_lstboxVillage->SetItemText(count, 0, name);

				m_lstboxVillage->SetItemAttrib(count, 0, D3DCOLOR_ARGB(255, 230, 230, 230));
				m_lstboxVillage->SetItemAttribHighlight(count, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

				count++;
			}
		}
	}
}

void _XWindow_MoveVillage::ProcessListItem(DWORD commandid)
{
	if(commandid > m_nMaxIndex)
		return;

	int oldmapindex = m_SelectedMapIndex;

	m_SelectedMapIndex = m_MoveVillageInfo[commandid-1]->mapid;
	m_SelectedMoveVillageIndex = commandid-1;

	if(oldmapindex != m_SelectedMapIndex)
	{
		// 지도 바꾸기 Author : 양희왕 //breif : 변경
		if( m_MoveVillageInfo[commandid-1]->pMapTexture != NULL )
			m_pMapTexture = m_MoveVillageInfo[commandid-1]->pMapTexture;
		//SetMapImage(m_SelectedMapIndex);
	}

	_XDWINPRINT("select village : %d [%s]", commandid, _XGETINTERFACETEXT(m_MoveVillageInfo[commandid-1]->strvillageid));
}

#else

void _XWindow_MoveVillage::SetListItem()
{
	m_lstboxVillage->DeleteAllItem();
//	m_lstboxVillage->SetScrollPos(0);

	int count = 0;
	int start = 0, end = 0;
	DWORD commandid = 0;

	map <int, _XMapInfo*>::iterator iter_map;
	for(iter_map = m_MapInfo.begin() ; iter_map != m_MapInfo.end() ; ++iter_map)
	{
		BOOL bInsert = FALSE;
		BOOL bVillageCheck = FALSE;
		int property = 0;
		int level = 0;

		_XMapInfo* mapinfo = iter_map->second;
		if(mapinfo)
		{
			// 같은 맵에서 흑도 백도 영역이 있을 수 있으므로 
			// 같은 맵이 나오면 하나만 검사하고 중지하는 것이 아니라
			// 끝까지 검사해서 맵을 선택할지 하지 않을지를 검사한다.
			vector <_XMoveVillageInfo*>::iterator iter_info;
			for(iter_info = m_MoveVillageInfo.begin() ; iter_info != m_MoveVillageInfo.end() ; ++iter_info)
			{
				_XMoveVillageInfo* info = *iter_info;
				if(info)
				{
					if(mapinfo->mapid == info->mapid)
					{
						property = info->property;
						level = info->level;

						if( !bInsert )
						{
							if(property == (int)g_pLocalUser->GetGroupType())
							{
								bInsert = TRUE;
							}
							else
							{
								bInsert = FALSE;
							}
							
							if(property == 0 || g_pLocalUser->GetGroupType() == _XGROUPTYPE_NONE)	// 전부or낭인
							{
								bInsert = TRUE;		
							}
						}
						else
						{
							break;
						}					
					}
				}
			}

			if(bInsert)
				commandid = mapinfo->mapid;		// 상위 16(0)+하위 16(map id)
			else
				commandid = 0;			// 선택 안되도록

			TCHAR name[256];
			memset(name, 0, sizeof(name));
			sprintf(name, "+%s[%s%s]", _XGETINTERFACETEXT(mapinfo->strmapid), g_LevelName[level], _XGETINTERFACETEXT(ID_STRING_NEW_3485));//이상

			m_lstboxVillage->InsertListItem(_T(""), 0, commandid);
			m_lstboxVillage->SetItemText(count, 0, name);

			if(commandid > 0)
			{
				m_lstboxVillage->SetItemAttrib(count, 0, D3DCOLOR_ARGB(255, 230, 230, 230));
				m_lstboxVillage->SetItemAttribHighlight(count, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			else
			{
#ifdef _XDEF_MOVEVILLAGE_RENEWAL_070814_KUKURI
				m_lstboxVillage->SetItemAttrib(count, 0, D3DCOLOR_ARGB(255, 152, 29, 29));
#else
				m_lstboxVillage->SetItemAttrib(count, 0, D3DCOLOR_ARGB(255, 80, 80, 80));
#endif
			}

			count++;

			start = mapinfo->villageindex;
			if(mapinfo->mapid == 1)
			{
				end = -1;
			}


			// 맵을 선택해서 마을을 펼칠 때에 
			// 마을마다 흑도인지 백도인지 문파와 레벨을
			// 다시 검사해서 마을을 선택할 수 있을지 여부를 검사한다.
			if(mapinfo->expand && bInsert)
			{
				bVillageCheck = FALSE;
				for(int i = end ; i < start ; ++i)
				{
					TCHAR name[256];
					memset(name, 0, sizeof(name));
					sprintf(name, " -%s", _XGETINTERFACETEXT(m_MoveVillageInfo[i+1]->strvillageid));
					
					if(m_MoveVillageInfo[i+1]->property == (int)g_pLocalUser->GetGroupType())
					{
						bVillageCheck = TRUE;
					}
					else
					{
						bVillageCheck = FALSE;
					}
					
					if(m_MoveVillageInfo[i+1]->property == 0 || g_pLocalUser->GetGroupType() == _XGROUPTYPE_NONE)	// 전부or낭인
					{
						bVillageCheck = TRUE;		
					}

					if( bVillageCheck )
					{
						commandid = (DWORD)(1<<16) | (i+1);		// 상위 16(1)+하위 16(villageinfo index)

						m_lstboxVillage->InsertListItem(_T(""), 0, commandid);
						m_lstboxVillage->SetItemText(count, 0, name);
						m_lstboxVillage->SetItemAttrib(count, 0, D3DCOLOR_ARGB(255, 230, 230, 230));
						m_lstboxVillage->SetItemAttribHighlight(count, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
					}
					else
					{
						commandid = 0;		// 선택 안되도록
						
						m_lstboxVillage->InsertListItem(_T(""), 0, commandid);
						m_lstboxVillage->SetItemText(count, 0, name);
						m_lstboxVillage->SetItemAttrib(count, 0, D3DCOLOR_ARGB(255, 80, 80, 80));
					}

					count++;
				}
			}

			end = mapinfo->villageindex;
		}
	}
}

void _XWindow_MoveVillage::ProcessListItem(DWORD commandid)
{
	DWORD prop = (commandid & 0xFFFF0000) >> 16;
	if(prop == 0)
	{
		// 맵 선택
		if(!m_MapInfo[commandid]->expand)
			m_MapInfo[commandid]->expand = TRUE;
		else
			m_MapInfo[commandid]->expand = FALSE;

		SetMapImage(commandid);

		// 리스트 박스 다시 졍렬
		SetListItem();
		m_SelectedMoveVillageIndex = -1;
	}
	else if(prop == 1)
	{
		// 마을 선택
		DWORD index = (commandid & 0x0000FFFF);
		_XDWINPRINT("select village : %d [%s]", index, _XGETINTERFACETEXT(m_MoveVillageInfo[index]->strvillageid));
		
//		if( index != 0 )
			m_SelectedMoveVillageIndex = index;
	}
}

#endif

BOOL _XWindow_MoveVillage::CheckUse()
{	 
	BOOL bUse = TRUE;

	_XWindow_Escape* pEscape_Window = (_XWindow_Escape*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_ESCAPE);	
	CEmperorOfDragonsApp* pApp = ((CEmperorOfDragonsApp*)gApp);

	if(g_pLocalUser->m_BattleMode == _XBATTLE_PK)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3679), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//대전 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( g_pLocalUser->m_bIsBossBattle )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3680), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//결 진행중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if(g_pLocalUser->m_CurrentSelectedSkillType == _XSI_USEFULTYPE_HIDDENMA &&
		(g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_NONE && g_pLocalUser->m_CurrentBattleState != _XUSER_STATE_LAST))	// 내공심법 시전중
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1580), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//운기요상 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}		
	else if( g_pLocalUser->m_bReadingBook )	// 비급 읽는 중이면 리턴
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1581), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//비급을 읽는 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( g_pLocalUser->m_bSocketPlugIn )	// 소켓 장착 중이면 리턴
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1582), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//장식물을 다는 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( g_pLocalUser->m_bGambleBoxOpen )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1583), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//패왕궤를 여는 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( g_pLocalUser->m_bInternalUse )	// 영약 복용 중이면 리턴
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1584), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//영약복용 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( g_pLocalUser->m_PersonalStoreState != _XUSER_PERSONALSTORE_NONE )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_MAINGAME_1828), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//노점상태에서는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if(g_pLocalUser->GetMotionClass() == _XACTION_DIE || g_pLocalUser->GetMotionClass() == _XACTION_DYING)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3681), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//빈사나 혼수 상태 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( g_pLocalUser->m_CharacterLog.isTrade )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3682), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//교환 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if(g_pLocalUser->m_UserState ==  _XDEF_USERSTATE_TRAINING)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3683), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//수련 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( (g_pLocalUser->m_UserState >= _XDEF_USERSTATE_MATCH_MAKETABLE && g_pLocalUser->m_UserState <= _XDEF_USERSTATE_MATCH_WAITEXIT) )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3684), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//비무 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( pEscape_Window->GetShowStatus() )
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3313), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//탈출 중에는 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( pApp->m_proc_maingame.m_bNPCDialogMode || pApp->m_proc_maingame.m_bNPCDialogWideView || pApp->m_proc_maingame.m_bFunctionalObjectWideView)
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_LOCALUSER_1585), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//지금은 사용할 수 없습니다.
		bUse = FALSE;
	}
	else if( g_pLocalUser->GetEnterMatchEvent() ) //Author : 양희왕 //breif : 영웅대회 에서 막기..
	{
		g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3739), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//지금은 사용할 수 없습니다.
		bUse = FALSE;
	}
	
	_XWindow_MatchPR* pMatchPR_Window = (_XWindow_MatchPR*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_MATCHPR);
	if( pMatchPR_Window)
	{
		if( pMatchPR_Window->GetShowStatus() )
		{
			g_NetworkKernel.InsertChatString(_XGETINTERFACETEXT(ID_STRING_NEW_3686), _XDEF_CHATMESSAGECOLOR_SYSTEMLOSS);//비무 홍보 중에는 사용할 수 없습니다.
			bUse = FALSE;
		}
	}

	return bUse;
}