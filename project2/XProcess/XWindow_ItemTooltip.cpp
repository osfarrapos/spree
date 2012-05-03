// XWindow_ItemTooltip.cpp: implementation of the _XWindow_ItemTooltip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_ItemTooltip.h"
#include "XWindow_NPCTrade.h" // 2004.06.16->oneway48 insert
#include "XWindow_RepairItem.h" // 2004.06.29->oneway48 insert
#include "XSR_STRINGHEADER.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_ItemTooltip::_XWindow_ItemTooltip()
{
	m_ParentWindow = NULL;
	m_HoveredSlotIndex = -1;
	
	m_Category = -1;
	m_ID = -1;
}

_XWindow_ItemTooltip::~_XWindow_ItemTooltip()
{

}


BOOL _XWindow_ItemTooltip::Initialize(void)
{
	m_ParentWindow	= NULL;
	m_HoveredSlotIndex = -1;
	SetWindowMoveMode(_XWMOVE_FIXED);
		
	memset( m_DescClassString, 0, sizeof(TCHAR)*16 );// 계열 -> 무기 분류
	strncpy(m_DescClassString, _T("무기분류"), 15);
	
	memset( m_DescPowerString, 0, sizeof(TCHAR)*16 );// 공격력
	strncpy(m_DescPowerString, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_ATTACKDAMAGE), 15);
	
	memset( m_DescDurabilityString, 0, sizeof(TCHAR)*16 );// 내구성
	strncpy(m_DescDurabilityString, _T("내구성"), 15);
	
	memset( m_DescStrengthString, 0, sizeof(TCHAR)*16 );// 강도
	strncpy(m_DescStrengthString, _T("강도"), 15);
	
	memset( m_DescBalanceString, 0, sizeof(TCHAR)*16 );// 완성도 2004.06.14->oneway48 modify
	strncpy(m_DescBalanceString, _T("완성도"), 15);
	
	memset( m_DescWeightString, 0, sizeof(TCHAR)*16 );// 무게
	strncpy(m_DescWeightString, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_WEIGHT), 15);

	// 2004.06.11->oneway48 insert 
	memset( m_DescGenderString, 0, sizeof(TCHAR)*16 );// 성별
	strncpy(m_DescGenderString, _T("성별"), 15);
	
	// 2004.06.14->oneway48 insert 
	memset( m_DescPrice, 0, sizeof(TCHAR)*16 );// 가격
	strncpy(m_DescPrice, _T("가격"), 15);

	// 2004.05.12->oneway48 insert
	memset( m_DescDefenseString, 0, sizeof(TCHAR)*16 );// 방어력
	strncpy(m_DescDefenseString, _T("방어력"), 15);
	
	memset( m_DescWarmthString, 0, sizeof(TCHAR)*16 );// 보온효과
	strncpy(m_DescWarmthString, _T("보온효과"), 15);
	
	memset( m_DescResistanceString, 0, sizeof(TCHAR)*16 );// 속성 저항력
	strncpy(m_DescResistanceString, _T("속성 저항력"), 15);
	
	memset( m_DescRollString, 0, sizeof(TCHAR)*16 );// 역할
	strncpy(m_DescRollString, _T("역할"), 15);
	
	memset( m_DescEffectString, 0, sizeof(TCHAR)*16 );// 효능
	strncpy(m_DescEffectString, _T("효능"), 15);
	
	memset( m_DescFeatureString, 0, sizeof(TCHAR)*16 );// 특징
	strncpy(m_DescFeatureString, _T("특징"), 15);
	// insert end

	memset( m_DescHeartString, 0, sizeof(TCHAR)*16 );// 심기
	strncpy(m_DescHeartString, _T("지혜"), 15);
	
	memset( m_DescStaminaString, 0, sizeof(TCHAR)*16 );// 정기 
	strncpy(m_DescStaminaString, _T("근력"), 15);
	
	memset( m_DescLevelString, 0, sizeof(TCHAR)*16 );// 내공수위
	strncpy(m_DescLevelString, _T("성취 단계"), 15); // 2004.06.18->oneway48 insert
	
	memset( m_DescRefine1String, 0, sizeof(TCHAR)*16 );// 제련 속성 1
	strncpy(m_DescRefine1String, _T("제련 속성 1"), 15);
	
	memset( m_DescRefine2String, 0, sizeof(TCHAR)*16 );// 제련 속성 2
	strncpy(m_DescRefine2String, _T("제련 속성 2"), 15);
	
	memset( m_DescAdditionalEffect1String, 0, sizeof(TCHAR)*16 );// 부가 효과 1
	strncpy(m_DescAdditionalEffect1String, _T("부가 효과 1"), 15);
	
	memset( m_DescAdditionalEffect2String, 0, sizeof(TCHAR)*16 );// 부가 효과 2
	strncpy(m_DescAdditionalEffect2String, _T("부가 효과 2"), 15);
	
	memset( m_DescAdditionalEffect3String, 0, sizeof(TCHAR)*16 );// 부가 효과 3
	strncpy(m_DescAdditionalEffect3String, _T("부가 효과 3"), 15);
	
	memset( m_DescSetEffect1String, 0, sizeof(TCHAR)*16 );// 세트 효과 1 
	strncpy(m_DescSetEffect1String, _T("세트 효과 1"), 15);
	
	memset( m_DescSetEffect2String, 0, sizeof(TCHAR)*16 );// 세트 효과 2
	strncpy(m_DescSetEffect2String, _T("세트 효과 2"), 15);
	
	memset( m_DescSetEffect3String, 0, sizeof(TCHAR)*16 );// 세트 효과 3
	strncpy(m_DescSetEffect3String, _T("세트 효과 3"), 15);
	
	memset( m_DescSetEffect4String, 0, sizeof(TCHAR)*16 );// 세트 효과 4
	strncpy(m_DescSetEffect4String, _T("세트 효과 4"), 15);
	
	memset( m_DescSetEffect5String, 0, sizeof(TCHAR)*16 );// 세트 효과 5
	strncpy(m_DescSetEffect5String, _T("세트 효과 5"), 15);
	
	return TRUE;
}

void _XWindow_ItemTooltip::DestroyWindow(void)
{

	_XWindow::DestroyWindow();
}

void _XWindow_ItemTooltip::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow)
		return;

	// 2004.06.16->oneway48 insert
	_XWindow_NPCTrade* pNPCTrade_Window = (_XWindow_NPCTrade*)g_MainWindowManager.FindWindow(_XDEF_WTITLE_NPCTRADE);
	_XWindow_RepairItem* pRepairItem_Window = (_XWindow_RepairItem*)g_MainWindowManager.FindWindow( _XDEF_WTITLE_REPAIRITEM ); // 2004.06.29->oneway48 insert

	// 2004.05.12->oneway48 insert
	_XDrawSolidBar( m_WindowPosition.x,  m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx, m_WindowPosition.y+m_WindowSize.cy , D3DCOLOR_ARGB(127,0,0,0) );
	
	// Border edge...
	_XDrawRectAngle(m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, _XSC_DEFAULT_BLACK);
	_XDrawRectAngle(m_WindowPosition.x+1, m_WindowPosition.y+1, m_WindowPosition.x+m_WindowSize.cx-2, m_WindowPosition.y+m_WindowSize.cy-2, 0.0f, D3DCOLOR_ARGB(255,96,96,96));
	_XDrawRectAngle(m_WindowPosition.x+2, m_WindowPosition.y+2, m_WindowPosition.x+m_WindowSize.cx-3, m_WindowPosition.y+m_WindowSize.cy-3, 0.0f, _XSC_DEFAULT_BLACK);
	
	// 2004.05.12->oneawy48 modify
	// 물품 이름 정보 박스 
	_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+6, m_WindowPosition.x+194, m_WindowPosition.y+22 , D3DCOLOR_ARGB(127,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+5, m_WindowPosition.x+194, m_WindowPosition.y+22 , 0.0f, _XSC_DEFAULT );
	
	// 물품 설명 정보 박스
	_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+25, m_WindowPosition.x+194, m_WindowPosition.y+56 , D3DCOLOR_ARGB(100,0,0,0) );
	_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+24, m_WindowPosition.x+194, m_WindowPosition.y+56 , 0.0f, _XSC_DEFAULT_BLACK );
		
	if(m_HoveredSlotIndex > -1 || (m_Category > -1 && m_ID > -1) )
	{				
		if( m_HoveredSlotIndex > -1 )
		{
			m_Category	= g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_cType;
			m_ID		= g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_sID;
		}

		switch(m_Category)
		{
		case _XGI_FC_WEAPON :			
			{
				// 물품 기본 정보 박스
				_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+59, m_WindowPosition.x+194, m_WindowPosition.y+150 , D3DCOLOR_ARGB(100,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+58, m_WindowPosition.x+194, m_WindowPosition.y+150 , 0.0f, _XSC_DEFAULT_BLACK );
				
				// 물품 요구 정보 박스
				_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+153, m_WindowPosition.x+194, m_WindowPosition.y+199 , D3DCOLOR_ARGB(100,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+152, m_WindowPosition.x+194, m_WindowPosition.y+199 , 0.0f, _XSC_DEFAULT_BLACK );
			

				// 물품 이름 정보
				g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, g_WeaponItemProperty[m_ID].cName);
				g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, g_WeaponItemProperty[m_ID].cName);

				
				// 물품 설명 정보 : 2004.06.16->oneway48 insert
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				
				g_XBaseFont->Puts_Separate( m_WindowPosition.x + 14, m_WindowPosition.y + 28, g_WeaponItemProperty[m_ID].cItemInfo, 180 );

				// 기본 정보
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 61, m_DescClassString );// 계열 -> 무기 분류
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 76, m_DescPowerString );// 공격력
				//g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 91, m_DescDurabilityString );// 내구성
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 91, _T("예기") );// 내구성
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 106, m_DescStrengthString );// 강도
				//g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 121, m_DescBalanceString );// 균형
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 121, _T("완성도") );// 균형
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 136, m_DescWeightString );// 무게
				
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 155, m_DescHeartString );// 심기
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 170, m_DescStaminaString );// 정기 
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 185, m_DescLevelString );// 내공수위

				TCHAR	classstring[16];			
				memset(classstring, 0, sizeof(TCHAR) * 16);
				switch(g_WeaponItemProperty[ m_ID ].cSecondType) 
				{			
				case _XGI_SC_WEAPON_SHORT :
					{
						switch(g_WeaponItemProperty[m_ID].cThirdType)
						{
						case _XGI_TC_WEAPON_SWORD		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_SWORD));	break;
						case _XGI_TC_WEAPON_KNIFE		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_KNIFE));	break;			
						case _XGI_TC_WEAPON_CLUB		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_CLUB));	break;		
						case _XGI_TC_WEAPON_HIT			 : strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_HIT));	break;
						case _XGI_TC_WEAPON_FIST		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_FIST));	break;
						case _XGI_TC_WEAPON_AXE			 : strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_AXE));	break;
						case _XGI_TC_WEAPON_SECRET		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_SECRETWEAPON)); break;			
						case _XGI_TC_WEAPON_RING		 : strcpy(classstring, "륜"); break;
						}
					}				
					break;
				case _XGI_SC_WEAPON_LONG :
					{
						switch(g_WeaponItemProperty[m_ID].cThirdType)
						{
						case _XGI_TC_WEAPON_GLAIVE		: strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_GLAIVE));  break;					
						case _XGI_TC_WEAPON_SPEAR		: strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_SPEAR));	break;					
						case _XGI_TC_WEAPON_STAFF		: strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_STAFF));	break;					
						}
					}
					break;
				}
				
				g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, classstring); //무기 분류
				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76, 1.0f, "%d - %d", //공격력 
					g_WeaponItemProperty[m_ID].sMinPower, 
					g_WeaponItemProperty[m_ID].sMaxPower);			
				
				// 2004.06.19->oneway48 close3 
				FLOAT percent = 0;			
				FLOAT maxdur = g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usMaxDur;	
//				FLOAT maxdur = 0;
//				maxdur = g_WeaponItemProperty[g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_sID].usDurability;

				
				if( g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usMaxDur != 0 )
					percent = (FLOAT)g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usCurDur / maxdur * 100.0f; // 2004.06.14->oneway48 insert

				if( percent >= 80.0f )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("무결")); 
				}
				else if( percent < 80 && percent >= 60.0f )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("우수")); 
				}
				else if( percent < 60 && percent >= 40.0f )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("양호")); 
				}
				else if( percent < 40 && percent >= 20.0f )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("이상")); 
				}
				else
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("수리")); 
				}
				
				//g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 91, 1.0f, "%d/%d" , 
				//	g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usCurDur, g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usMaxDur);// 내구성
				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 106, 1.0f, "%d" , g_WeaponItemProperty[m_ID].cStrength);

				switch(g_WeaponItemProperty[m_ID].cBalance) {
				case 25:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 121, _T("천품")); 
					break;
				case 20:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 121, _T("상품")); 
					break;
				case 15:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 121, _T("중품")); 
					break;
				case 10:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 121, _T("하품")); 
					break;
				case 5:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 121, _T("가품")); 
					break;
				}
				//g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 121, 1.0f, "%d" , g_WeaponItemProperty[m_ID].cBalance);

				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 136, 1.0f, "%d" , g_WeaponItemProperty[m_ID].sClanPoint1); // 2004.06.14->oneway48 insert


				if( CheckWisdom( _XGI_FC_WEAPON, m_ID ) )
				{
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 155, _T("요구치 만족")); 
				}
				else
				{
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 0, 210, 255 ) );
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 155, _T("정확도 하락")); 
				}
				
				if( CheckMuscularStrength( _XGI_FC_WEAPON, m_ID ) )
				{
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 170, _T("요구치 만족")); 
				}
				else
				{
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 0, 210, 255 ) );
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 170, _T("내구력 감소")); 
				}
				
				if( CheckLevel( _XGI_FC_WEAPON, m_ID ) )
				{
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 185, _T("요구치 만족")); 
				}
				else
				{
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 0, 210, 255 ) );
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 185, _T("공격력 하락")); 
				}
				
				// 2004.06.14->oneway48 insert
				if(pNPCTrade_Window && pNPCTrade_Window->GetShowStatus())
				{
					// 물품 가격 박스
					g_XBaseFont->SetColor(_XSC_DEFAULT);	// 2004.06.17->oneway48 insert
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+202, m_WindowPosition.x+194, m_WindowPosition.y+220 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+201, m_WindowPosition.x+194, m_WindowPosition.y+220 , 0.0f, _XSC_DEFAULT_BLACK );
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 205, m_DescPrice );// 가격
					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 205, 1.0f, "%d" , g_WeaponItemProperty[m_ID].uiPrice/5);
					
				}
				else if( pRepairItem_Window && pRepairItem_Window->GetShowStatus())// 2004.06.29->oneway48 insert
				{
					// 물품 수리 가격 박스
					if( pRepairItem_Window->m_bRepairWeapon )
					{
						int shortdur = g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usMaxDur - g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usCurDur;
						int needmoney = (shortdur-1)/15 + 1;
						
						g_XBaseFont->SetColor(_XSC_DEFAULT);	
						_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+202, m_WindowPosition.x+194, m_WindowPosition.y+220 , D3DCOLOR_ARGB(100,0,0,0) );
						_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+201, m_WindowPosition.x+194, m_WindowPosition.y+220 , 0.0f, _XSC_DEFAULT_BLACK );
						g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 205, _T("수리 가격") );// 가격
						
						if( needmoney == 1 )
						{
							g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 205, _T("수리 필요 없음") );
						}
						else
						{
							g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 205, 1.0f, "%d" , needmoney);
						}
					}

				}
				
		
				// 2004.05.18->oneway48 modify : 데이타가 들어오면 적용 
	/*
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				if(g_WeaponItemProperty[m_ID].제련속성)
				{	
					// 물품 제련 정보 박스
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+202, m_WindowPosition.x+194, m_WindowPosition.y+233 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+201, m_WindowPosition.x+194, m_WindowPosition.y+233 , 0.0f, _XSC_DEFAULT_BLACK );
					
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 204, m_DescRefine1String );// 제련 속성 1
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 219, m_DescRefine2String );// 제련 속성 2				
				}
				
				if(g_WeaponItemProperty[m_ID].부과효과)
				{						
					// 부과 효과 정보 박스
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+236, m_WindowPosition.x+194, m_WindowPosition.y+282 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+235, m_WindowPosition.x+194, m_WindowPosition.y+282 , 0.0f, _XSC_DEFAULT_BLACK );
					
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,219,196,136));
					
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 239, m_DescAdditionalEffect1String );// 부가 효과 1 
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 254, m_DescAdditionalEffect2String );// 부가 효과 2
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 269, m_DescAdditionalEffect3String );// 부가 효과 3
				}

				
				if(g_WeaponItemProperty[m_ID].세트효과)
				{		
					// 세트 효과 정보 박스
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+285, m_WindowPosition.x+194, m_WindowPosition.y+361 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+284, m_WindowPosition.x+194, m_WindowPosition.y+361 , 0.0f, _XSC_DEFAULT_BLACK );
						
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,0,255,0));
					
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 288, m_DescSetEffect1String );// 세트 효과 1
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 303, m_DescSetEffect2String );// 세트 효과 2
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 318, m_DescSetEffect3String );// 세트 효과 3
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 333, m_DescSetEffect4String );// 세트 효과 4
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 348,m_DescSetEffect5String);// 세트 효과 5
				}
	*/				
				// modify end
			}
			
			break;

 		case _XGI_FC_CLOTHES :
			{			
				// 물품 기본 정보
				_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+59, m_WindowPosition.x+194, m_WindowPosition.y+150 + 15 , D3DCOLOR_ARGB(100,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+58, m_WindowPosition.x+194, m_WindowPosition.y+150 + 15 , 0.0f, _XSC_DEFAULT_BLACK );
				
				// 물품 요구 정보
				_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+153 + 15, m_WindowPosition.x+194, m_WindowPosition.y+199 + 15, D3DCOLOR_ARGB(100,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+152 + 15, m_WindowPosition.x+194, m_WindowPosition.y+199 + 15 , 0.0f, _XSC_DEFAULT_BLACK );
		
				// 물품 이름 정보
				g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, g_ClothesItemProperty[m_ID].cName);
				g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, g_ClothesItemProperty[m_ID].cName);
				
				// 물품 설명 정보 : 2004.06.16->oneway48 insert
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				
				g_XBaseFont->Puts_Separate( m_WindowPosition.x + 14, m_WindowPosition.y + 28, g_ClothesItemProperty[m_ID].cItemInfo, 180 );
				
				// insert end

				// 기본 정보
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 61, m_DescGenderString );// 성별 2004.06.11->oneway48 insert
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 61 + 15, m_DescStrengthString );// 강도
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 76 + 15, m_DescWeightString );// 무게
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 91 + 15, m_DescDefenseString );// 방어력
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 106 + 15, _T("착용감") );// 내구성
				//g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 106 + 15, m_DescDurabilityString );// 내구성
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 121 + 15, m_DescWarmthString );// 보온효과
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 136 + 15, m_DescResistanceString );// 속성 저항력
				
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 155 + 15, m_DescHeartString );// 심기
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 170 + 15, m_DescStaminaString );// 정기 
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 185 + 15, m_DescLevelString );// 내공수위

				// 2004.06.11->oneway48 
				if( g_ClothesItemProperty[m_ID].cSexType == 0 )  // 남자
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("남자")); 
				}
				else if(g_ClothesItemProperty[m_ID].cSexType == 1)  // 여자
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("여자")); 
				}
				else // 공통
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("공통")); 
				}

				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 61 + 15, 1.0f, "%d" , g_ClothesItemProperty[m_ID].cStrength); // 강도

				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76 + 15, 1.0f, "%d" , g_ClothesItemProperty[m_ID].sClanPoint1); // 2004.06.14->oneway48 insert

				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 91 + 15, 1.0f, "%d" , g_ClothesItemProperty[m_ID].sDefence); // 방어력
			
				
				// 2004.06.16->oneway48 insert 
				FLOAT percent = 0;				
				FLOAT maxdur = g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usMaxDur;
				//maxdur = g_ClothesItemProperty[g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_sID].usDurability;
				
				if( maxdur != 0 )
					percent = (FLOAT)g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usCurDur / maxdur * 100.0f; // 2004.06.14->oneway48 insert

				if( percent >= 80.0f )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 106+ 15, _T("완전무결")); 
				}
				else if( percent < 80 && percent >= 60.0f )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 106+ 15, _T("우수하다")); 
				}
				else if( percent < 60 && percent >= 40.0f )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 106+ 15, _T("양호하다")); 
				}
				else if( percent < 40 && percent >= 20.0f )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 106+ 15, _T("조악하다")); 
				}
				else
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 106+ 15, _T("수리요망")); 
				}
				

			//	g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 106+ 15, 1.0f, "%d/%d" , 
			//		g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usCurDur, g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usMaxDur);// 내구성
				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 121 + 15, 1.0f, "%d" , g_ClothesItemProperty[m_ID].cWarm);// 보온효과
				
				
				// 2004.06.14->oneway48 insert
				if( CheckWisdom( _XGI_FC_CLOTHES, m_ID ) )
				{
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 155+ 15, _T("요구치 만족")); 
					//g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 155+ 15, 1.0f, "%d" , g_ClothesItemProperty[m_ID].sReqJung);
				}
				else
				{
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 0, 210, 255 ) );
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 155+ 15,  _T("회피도 하락")); 
				}

				if( CheckMuscularStrength( _XGI_FC_CLOTHES, m_ID ) )
				{
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 170+ 15, _T("요구치 만족")); 
					//g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 170+ 15, 1.0f, "%d" , g_ClothesItemProperty[m_ID].sReqSim);
				}
				else
				{
					g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 0, 210, 255 ) );
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 170+ 15,_T("내구력 감소")); 
				}

				g_XBaseFont->SetColor(_XSC_DEFAULT);
				
				g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 185+ 15, _T("요구치 만족")); 
				//g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 185+ 15, 1.0f, "%d" , g_ClothesItemProperty[m_ID].sReqLevel);
				
				// 2004.06.14->oneway48 insert
				if(pNPCTrade_Window && pNPCTrade_Window->GetShowStatus())
				{
					// 물품 가격 박스
					g_XBaseFont->SetColor(_XSC_DEFAULT);	// 2004.06.17->oneway48 insert
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+217, m_WindowPosition.x+194, m_WindowPosition.y+235 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+216, m_WindowPosition.x+194, m_WindowPosition.y+235 , 0.0f, _XSC_DEFAULT_BLACK );
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 220, m_DescPrice );// 가격
					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 220, 1.0f, "%d" , g_ClothesItemProperty[m_ID].uiPrice/5);
				}
				else if( pRepairItem_Window && pRepairItem_Window->GetShowStatus())// 2004.06.29->oneway48 insert
				{
					// 물품 수리 가격 박스
					if( !pRepairItem_Window->m_bRepairWeapon )
					{
						int shortdur = g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usMaxDur - g_pLocalUser->m_UserItemList[m_HoveredSlotIndex].m_usCurDur;
						int needmoney = (shortdur-1)/15 + 1;
						
						g_XBaseFont->SetColor(_XSC_DEFAULT);	// 2004.06.17->oneway48 insert
						_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+217, m_WindowPosition.x+194, m_WindowPosition.y+235 , D3DCOLOR_ARGB(100,0,0,0) );
						_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+216, m_WindowPosition.x+194, m_WindowPosition.y+235 , 0.0f, _XSC_DEFAULT_BLACK );
						g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 220, _T("수리 가격") );// 가격
						
						if( needmoney == 1 )
						{
							g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 220, _T("수리 필요 없음") );
						}
						else
						{
							g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 220, 1.0f, "%d" , needmoney);
						}				
					}
					
				}

				// 2004.05.18->oneway48 modify
	/*
				if(g_ClothesItemProperty[m_ID].제련속성)
				{	
					// 물품 제련 정보
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+202, m_WindowPosition.x+194, m_WindowPosition.y+233  + 15, D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+201, m_WindowPosition.x+194, m_WindowPosition.y+233  + 15, 0.0f, _XSC_DEFAULT_BLACK );
					
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 204 + 15, m_DescRefine1String );// 제련 속성 1
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 219 + 15, m_DescRefine2String );// 제련 속성 2
					  
				}
				
				if(g_ClothesItemProperty[m_ID].부과효과)
				{						
					// 부과 효과 정보 박스
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+236, m_WindowPosition.x+194, m_WindowPosition.y+282 + 15 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+235, m_WindowPosition.x+194, m_WindowPosition.y+282 + 15 , 0.0f, _XSC_DEFAULT_BLACK );
					
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,219,196,136));
					  
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 239 + 15, m_DescAdditionalEffect1String );// 부가 효과 1 
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 254 + 15, m_DescAdditionalEffect2String );// 부가 효과 2
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 269 + 15, m_DescAdditionalEffect3String );// 부가 효과 3
				}

				
				if(g_ClothesItemProperty[m_ID].세트효과)
				{		
					// 세트 효과 정보 박스
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+285, m_WindowPosition.x+194, m_WindowPosition.y+361 + 15 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+284, m_WindowPosition.x+194, m_WindowPosition.y+361 + 15, 0.0f, _XSC_DEFAULT_BLACK );
				
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,0,255,0));
					  
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 288 + 15, m_DescSetEffect1String );// 세트 효과 1
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 303 + 15, m_DescSetEffect2String );// 세트 효과 2
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 318 + 15, m_DescSetEffect3String );// 세트 효과 3
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 333 + 15, m_DescSetEffect4String );// 세트 효과 4
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 348 + 15,m_DescSetEffect5String);// 세트 효과 5
				}
	*/				
				// modify end
			}			
			break;

		case _XGI_FC_POTION :
				{
				// 물품 기본 정보
				_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+59, m_WindowPosition.x+194, m_WindowPosition.y+120 , D3DCOLOR_ARGB(100,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+58, m_WindowPosition.x+194, m_WindowPosition.y+120 , 0.0f, _XSC_DEFAULT_BLACK );
				

				// 물품 이름 정보
				g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, g_PotionItemProperty[m_ID].cName);
				g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, g_PotionItemProperty[m_ID].cName);

				
				// 물품 설명 정보 : 2004.06.16->oneway48 insert
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				
				g_XBaseFont->Puts_Separate( m_WindowPosition.x + 14, m_WindowPosition.y + 28, g_PotionItemProperty[m_ID].cItemInfo, 180 );

				// 기본 정보1
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 61, m_DescRollString );// 역할
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 76, m_DescEffectString );// 효능
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 91, m_DescFeatureString );// 특징
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 106, m_DescWeightString );// 무게
				
				// 2004.06.15->oneway48 insert
				switch(g_PotionItemProperty[m_ID].cSecondType) 
				{
				case 0:
					{
						g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("내상 치료제") );
						switch(g_PotionItemProperty[m_ID].cApplyType) 
						{					
						case 0:
							g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76 , 1.0f, "내상 %d 감소" , g_PotionItemProperty[m_ID].sApplyValue); // 무게 
							break;
						case 1:
							g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76 , 1.0f, "내상 %d%% 감소" , g_PotionItemProperty[m_ID].sApplyValue); // 무게 
							break;
						}
					}
					break;
				case 1:
					{
						g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("외상 치료제") );
						switch(g_PotionItemProperty[m_ID].cApplyType) 
						{					
						case 0:
							g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76 , 1.0f, "외상 %d 감소" , g_PotionItemProperty[m_ID].sApplyValue); // 무게 
							break;
						case 1:
							g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76 , 1.0f, "외상 %d%% 감소" , g_PotionItemProperty[m_ID].sApplyValue); // 무게 
							break;
						}
					}
					break;
				case 2:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("피로 회복제") );
					break;
				}
				
				switch(g_PotionItemProperty[m_ID].cClan) 
				{
				case 0:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("공통 사용") );
					break;
				case 1:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("개방 전용") );
					break;
				case 2:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("비궁 전용") );
					break;
				case 3:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("소림 전용") );
					break;
				case 4:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("녹림 전용") );
					break;
				case 5:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("무당 전용") );
					break;
				case 6:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("마교 전용") );
					break;
				case 7:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 91, _T("세가 전용") );
					break;
				}

				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 106, 1.0f, "%d" , g_PotionItemProperty[m_ID].sClanPoint1);// 무게

				if(pNPCTrade_Window && pNPCTrade_Window->GetShowStatus())
				{
					// 물품 가격 박스
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+123, m_WindowPosition.x+194, m_WindowPosition.y+141 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+122, m_WindowPosition.x+194, m_WindowPosition.y+141 , 0.0f, _XSC_DEFAULT_BLACK );
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 126, m_DescPrice );// 가격
					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 126, 1.0f, "%d" , g_PotionItemProperty[m_ID].uiPrice/5);
					
				}
				// insert end
			}

			break;
			
		case _XGI_FC_QUEST :
			{
				g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
				
				// 물품 이름 정보
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, g_QuestItemProperty[m_ID].cName);
				g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, g_QuestItemProperty[m_ID].cName);
				
				// 물품 설명 정보 : 2004.06.16->oneway48 insert
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				
				g_XBaseFont->Puts_Separate( m_WindowPosition.x + 14, m_WindowPosition.y + 28, g_QuestItemProperty[m_ID].cItemInfo, 180 );
			}

			break;
			
			//2004.06.20->oneway48 insert
		case _XGI_FC_RESOURCE :	
			{
				g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
				
				// 물품 이름 정보
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, g_ResourceItemProperty[m_ID].cName);
				g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, g_ResourceItemProperty[m_ID].cName);
				
				// 물품 설명 정보 
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				g_XBaseFont->Puts_Separate( m_WindowPosition.x + 14, m_WindowPosition.y + 28, g_ResourceItemProperty[m_ID].cItemInfo, 180 );

				if(pNPCTrade_Window && pNPCTrade_Window->GetShowStatus())
				{
					// 물품 가격 박스
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+59, m_WindowPosition.x+194, m_WindowPosition.y+77 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+58, m_WindowPosition.x+194, m_WindowPosition.y+77 , 0.0f, _XSC_DEFAULT_BLACK );
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 62, m_DescPrice );// 가격
					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 62, 1.0f, "%d" , g_ResourceItemProperty[m_ID].uiPrice/5);
				}
			}
			break;
			
		default :
			g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);

			// 물품 이름 정보
			g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, "NONAME");
			g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, "NONAME");
			
			break;
		}		
	}

	g_XBaseFont->Flush();
}

BOOL _XWindow_ItemTooltip::Process(_XGUIObject*& pfocusobject)
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
				if(CheckMousePosition())
					this->ShowWindow(FALSE);
			}
		}
	}
	
	if(!_XWindow::Process(pfocusobject))
		return FALSE;
	
	return TRUE;
}

void _XWindow_ItemTooltip::SetWindowPosition(void)
{
	POINT			position;
	POINT			parentposition = m_ParentWindow->GetWindowPos();
	SIZE			parentsize = m_ParentWindow->GetWindowSize();
	
	position.x = parentposition.x + parentsize.cx + 6;
	position.y = parentposition.y + 3;
	MoveWindow(position.x , position.y);
	
	CheckOutside();
}

void _XWindow_ItemTooltip::CheckOutside(void)
{
	BOOL		reposition = FALSE;
	POINT		position;
	POINT		parentposition = m_ParentWindow->GetWindowPos();
	SIZE		parentsize = m_ParentWindow->GetWindowSize();
	
	if(m_WindowPosition.x < 0)
	{
		// 왼쪽에서 짤림
		position.x = parentposition.x + parentsize.cx + 6;
		position.y = parentposition.y + 3;
		reposition = TRUE;
	}
	if(m_WindowPosition.x + m_WindowSize.cx >= gnWidth)
	{
		// 오른쪽에서 짤림
		position.x = parentposition.x - m_WindowSize.cx - 6;
		position.y = parentposition.y + 3;
		reposition = TRUE;
	}
	
	if(reposition)
	{
		MoveWindow(position.x, position.y);
	}
}

void _XWindow_ItemTooltip::ShowWindow(BOOL show)
{
	if(show)
	{
		m_pMyManager->SetTopWindow(this);		
	}
	
	_XWindow::ShowWindow(show);
}

void _XWindow_ItemTooltip::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);

	//m_ItemBorder.MoveWindow( X + 6, Y + 5 );
}


// 2004.06.11->oneway48 insert
BOOL _XWindow_ItemTooltip::CheckLevel(_XGI_FirstCategory type, int uniqid )
{
	if( type == _XGI_FC_WEAPON)
	{
		if(g_pLocalUser->m_CharacterInfo.level >= g_WeaponItemProperty[uniqid].sReqLevel) 
			return TRUE;
	}
	return FALSE;
}

BOOL _XWindow_ItemTooltip::CheckWisdom(_XGI_FirstCategory type, int uniqid)
{
	if( type == _XGI_FC_WEAPON )
	{
		if(g_pLocalUser->m_CharacterInfo.dexterity >= g_WeaponItemProperty[uniqid].sReqSim ) 
			return TRUE;
	}
	else if( type == _XGI_FC_CLOTHES )
	{
		if(g_pLocalUser->m_CharacterInfo.dexterity >= g_ClothesItemProperty[uniqid].sReqSim ) 
			return TRUE;
	}
	return FALSE;
}

BOOL _XWindow_ItemTooltip::CheckMuscularStrength(_XGI_FirstCategory type, int uniqid)
{
	if( type == _XGI_FC_WEAPON)
	{
		if(g_pLocalUser->m_CharacterInfo.strength >= g_WeaponItemProperty[uniqid].sReqJung) 
			return TRUE;
	}
	else if( type == _XGI_FC_CLOTHES )
	{
		if(g_pLocalUser->m_CharacterInfo.strength >= g_ClothesItemProperty[uniqid].sReqJung) 
			return TRUE;
	}
	return FALSE;
}
