// XWindow_TradeItemTooltip.cpp: implementation of the _XWindow_TradeItemTooltip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "XWindow_TradeItemTooltip.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_TradeItemTooltip::_XWindow_TradeItemTooltip()
{
	m_ParentWindow = NULL;
	m_HoveredSlotIndex = -1;
	
	m_Category1 = 0;
	m_Category3 = 0;

//	m_RotateAngle = 0.0f;
//	m_ShowSignalTime = 0;
}	

_XWindow_TradeItemTooltip::~_XWindow_TradeItemTooltip()
{

}

BOOL _XWindow_TradeItemTooltip::Initailize(void)
{
	SetWindowMoveMode(_XWMOVE_FIXED);
	
	memset( m_DescClassString, 0, sizeof(TCHAR)*16 );// 계열 -> 무기 분류
	strcpy(m_DescClassString, _T("무기분류"));
	
	memset( m_DescPowerString, 0, sizeof(TCHAR)*16 );// 공격력
	strcpy(m_DescPowerString, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_ATTACKDAMAGE));
	
	memset( m_DescDurabilityString, 0, sizeof(TCHAR)*16 );// 내구성
	strcpy(m_DescDurabilityString, _T("내구성"));

	// 2004.06.11->oneway48 insert 
	memset( m_DescGenderString, 0, sizeof(TCHAR)*16 );// 성별
	strcpy(m_DescGenderString, _T("성별"));
	
	// 2004.06.14->oneway48 insert 
	memset( m_DescPrice, 0, sizeof(TCHAR)*16 );// 가격
	strcpy(m_DescPrice, _T("가격"));
	
	memset( m_DescStrengthString, 0, sizeof(TCHAR)*16 );// 강도
	strcpy(m_DescStrengthString, _T("강도"));
	
	memset( m_DescBalanceString, 0, sizeof(TCHAR)*16 );// 균형
	strcpy(m_DescBalanceString, _T("균형"));
	
	memset( m_DescWeightString, 0, sizeof(TCHAR)*16 );// 무게
	strcpy(m_DescWeightString, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_WEIGHT));
	
	memset( m_DescDefenseString, 0, sizeof(TCHAR)*16 );// 방어력
	strcpy(m_DescDefenseString, _T("방어력"));
	
	memset( m_DescWarmthString, 0, sizeof(TCHAR)*16 );// 보온효과
	strcpy(m_DescWarmthString, _T("보온효과"));
	
	memset( m_DescResistanceString, 0, sizeof(TCHAR)*16 );// 속성 저항력
	strcpy(m_DescResistanceString, _T("속성 저항력"));
	
	memset( m_DescRollString, 0, sizeof(TCHAR)*16 );// 역할
	strcpy(m_DescRollString, _T("역할"));
	
	memset( m_DescEffectString, 0, sizeof(TCHAR)*16 );// 효능
	strcpy(m_DescEffectString, _T("효능"));
	
	memset( m_DescFeatureString, 0, sizeof(TCHAR)*16 );// 특징
	strcpy(m_DescFeatureString, _T("특징"));
	
	memset( m_DescHeartString, 0, sizeof(TCHAR)*16 );// 지혜
	strcpy(m_DescHeartString, _T("지혜"));
	
	memset( m_DescStaminaString, 0, sizeof(TCHAR)*16 );// 근력 
	strcpy(m_DescStaminaString, _T("근력"));
	
	memset( m_DescLevelString, 0, sizeof(TCHAR)*16 );// 내공수위
	strcpy(m_DescLevelString, _T("성취 단계")); // 2004.06.18->oneway48 insert
	
	memset( m_DescRefine1String, 0, sizeof(TCHAR)*16 );// 제련 속성 1
	strcpy(m_DescRefine1String, _T("제련 속성 1"));
	
	memset( m_DescRefine2String, 0, sizeof(TCHAR)*16 );// 제련 속성 2
	strcpy(m_DescRefine2String, _T("제련 속성 2"));
	
	memset( m_DescAdditionalEffect1String, 0, sizeof(TCHAR)*16 );// 부가 효과 1
	strcpy(m_DescAdditionalEffect1String, _T("부가 효과 1"));
	
	memset( m_DescAdditionalEffect2String, 0, sizeof(TCHAR)*16 );// 부가 효과 2
	strcpy(m_DescAdditionalEffect2String, _T("부가 효과 2"));
	
	memset( m_DescAdditionalEffect3String, 0, sizeof(TCHAR)*16 );// 부가 효과 3
	strcpy(m_DescAdditionalEffect3String, _T("부가 효과 3"));
	
	memset( m_DescSetEffect1String, 0, sizeof(TCHAR)*16 );// 세트 효과 1 
	strcpy(m_DescSetEffect1String, _T("세트 효과 1"));
	
	memset( m_DescSetEffect2String, 0, sizeof(TCHAR)*16 );// 세트 효과 2
	strcpy(m_DescSetEffect2String, _T("세트 효과 2"));
	
	memset( m_DescSetEffect3String, 0, sizeof(TCHAR)*16 );// 세트 효과 3
	strcpy(m_DescSetEffect3String, _T("세트 효과 3"));
	
	memset( m_DescSetEffect4String, 0, sizeof(TCHAR)*16 );// 세트 효과 4
	strcpy(m_DescSetEffect4String, _T("세트 효과 4"));
	
	memset( m_DescSetEffect5String, 0, sizeof(TCHAR)*16 );// 세트 효과 5
	strcpy(m_DescSetEffect5String, _T("세트 효과 5"));
	
/*
	memset(m_DescNameString, 0, sizeof(TCHAR)*16);
	strncpy(m_DescNameString, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_NAME), 15);

	memset(m_DescClassString, 0, sizeof(TCHAR)*16);
	strncpy(m_DescClassString, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_CLASS), 15);

	memset(m_DescPowerString, 0, sizeof(TCHAR)*16);
	strncpy(m_DescPowerString, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_ATTACKDAMAGE), 15);

	memset(m_DescSuccessRateString, 0, sizeof(TCHAR)*16);
	strncpy(m_DescSuccessRateString, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_ATTACKRATING), 15);

	memset(m_DescWeightString, 0, sizeof(TCHAR)*16);
	strncpy(m_DescWeightString, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_WEIGHT), 15);

	memset(m_DescRequireString, 0, sizeof(TCHAR)*16);
	strncpy(m_DescRequireString, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_EXP), 15);

	memset(m_DescEffectString, 0, sizeof(TCHAR)*16);
	strncpy(m_DescEffectString, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_EFFECT), 15);

	m_ModelViewCamera.Set(5, 45./124., EPSILON3, 180.0f);
	m_ModelViewCamera.Set(0.0f, 0.0f, 0.0f);
	m_ModelViewCamera.UpdateViewMatrix( NULL, FALSE );
	m_ModelViewCamera.UpdateProjMatrix();

	m_ItemBorder.Create( m_WindowPosition.x + 6, 
						 m_WindowPosition.y + 5, 
						 m_WindowPosition.x + 6 + 62, 
						 m_WindowPosition.y + 5 + 142,
						 &g_MainInterfaceTextureArchive, g_MainInterfaceTextureArchive.FindResource( "inven_Itemtooltipborder.tga" ) );	
	m_ItemBorder.SetClipRect( 0, 0, 62, 142 );
*/
	return TRUE;
}

void _XWindow_TradeItemTooltip::DestroyWindow(void)
{
	_XWindow::DestroyWindow();
}

void _XWindow_TradeItemTooltip::Draw(_XGUIObject*& pfocusobject)
{
	_XWindow::Draw(pfocusobject);

	if(!this->m_ShowWindow)
		return;

	
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
		
//	if(m_HoveredSlotIndex > -1)
	{				
		int uniqid = m_Category3;

		switch(m_Category1)
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
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, g_WeaponItemProperty[uniqid].cName);
				g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, g_WeaponItemProperty[uniqid].cName);
				
				// 물품 설명 정보 : 2004.06.16->oneway48 insert
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				
				// 2004.06.15->oneway48 insert
				int count = 0;
				for(int i = 0; i < 57; i++)
				{
					if( g_WeaponItemProperty[uniqid].cItemInfo[i] == 0 )
						break;
					count++;
				}
				
				if( count <= 30 )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 28, g_WeaponItemProperty[uniqid].cItemInfo);
				}
				else
				{
					TCHAR messagestring[64];
					memset(messagestring,0,sizeof(TCHAR)*64);
					for(int i = 0; i < 30; i++)
						messagestring[i] = g_WeaponItemProperty[uniqid].cItemInfo[i];
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 28, messagestring);
					
					memset(messagestring,0,sizeof(TCHAR)*64);
					for( i = 0; i < count-30; i++)
						messagestring[i] = g_WeaponItemProperty[uniqid].cItemInfo[30+i];
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 43, messagestring);
				}
				// insert end

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
				switch(g_WeaponItemProperty[ uniqid ].cSecondType) 
				{			
				case _XGI_SC_WEAPON_SHORT :
					{
						switch(g_WeaponItemProperty[uniqid].cThirdType)
						{
						case _XGI_TC_WEAPON_SWORD		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_SWORD));	break;
						case _XGI_TC_WEAPON_KNIFE		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_KNIFE));	break;			
						case _XGI_TC_WEAPON_CLUB		: strcpy(classstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_CLUB));	break;		
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
						switch(g_WeaponItemProperty[uniqid].cThirdType)
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
					g_WeaponItemProperty[uniqid].sMinPower, 
					g_WeaponItemProperty[uniqid].sMaxPower);			
				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 91, 1.0f, "%d" , g_WeaponItemProperty[uniqid].usDurability);// 내구성
				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 106, 1.0f, "%d" , g_WeaponItemProperty[uniqid].cStrength);
				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 106, 1.0f, "%d" , g_WeaponItemProperty[uniqid].cStrength);
				
				switch(g_WeaponItemProperty[uniqid].cBalance) {
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
				//g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 121, 1.0f, "%d" , g_WeaponItemProperty[uniqid].cBalance);

				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 136, 1.0f, "%d" , g_WeaponItemProperty[uniqid].sClanPoint1);
				
				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 155, 1.0f, "%d" , g_WeaponItemProperty[uniqid].sReqJung);
				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 170, 1.0f, "%d" , g_WeaponItemProperty[uniqid].sReqSim);
				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 185, 1.0f, "%d" , g_WeaponItemProperty[uniqid].sReqLevel);
					
				// 2004.06.14->oneway48 insert
				// 물품 가격 박스
				_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+202, m_WindowPosition.x+194, m_WindowPosition.y+220 , D3DCOLOR_ARGB(100,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+201, m_WindowPosition.x+194, m_WindowPosition.y+220 , 0.0f, _XSC_DEFAULT_BLACK );
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 205, m_DescPrice );// 가격
				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 205, 1.0f, "%d" , g_WeaponItemProperty[uniqid].uiPrice);

				// 2004.05.18->oneway48 modify : 데이타가 들어오면 적용 
	/*
				if(g_WeaponItemProperty[uniqid].제련속성)
				{	
					// 물품 제련 정보 박스
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+202, m_WindowPosition.x+194, m_WindowPosition.y+233 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+201, m_WindowPosition.x+194, m_WindowPosition.y+233 , 0.0f, _XSC_DEFAULT_BLACK );
					
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 204, m_DescRefine1String );// 제련 속성 1
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 219, m_DescRefine2String );// 제련 속성 2				
				}
				
				if(g_WeaponItemProperty[uniqid].부과효과)
				{						
					// 부과 효과 정보 박스
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+236, m_WindowPosition.x+194, m_WindowPosition.y+282 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+235, m_WindowPosition.x+194, m_WindowPosition.y+282 , 0.0f, _XSC_DEFAULT_BLACK );
					
					g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,219,196,136));
					
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 239, m_DescAdditionalEffect1String );// 부가 효과 1 
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 254, m_DescAdditionalEffect2String );// 부가 효과 2
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 269, m_DescAdditionalEffect3String );// 부가 효과 3
				}

				
				if(g_WeaponItemProperty[uniqid].세트효과)
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
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+153, m_WindowPosition.x+194, m_WindowPosition.y+199 + 15, D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+152, m_WindowPosition.x+194, m_WindowPosition.y+199 + 15 , 0.0f, _XSC_DEFAULT_BLACK );
					
					//2004.06.25->oneway48 insert
					TCHAR messagestring[64];
					memset(messagestring,0,sizeof(TCHAR)*64);
					switch(g_ClothesItemProperty[uniqid].cClan) 
					{
					case 0:
						strcpy(messagestring,_T("공통 사용")); 
						break;
					case 1:
						strcpy(messagestring,_T("개방 전용")); 
						break;
					case 2:
						strcpy(messagestring,_T("비궁 전용")); 
						break;
					case 3:
						strcpy(messagestring,_T("소림 전용")); 
						break;
					case 4:
						strcpy(messagestring,_T("녹림 전용")); 
						break;
					case 5:
						strcpy(messagestring,_T("무당 전용")); 
						break;
					case 6:
						strcpy(messagestring,_T("마교 전용")); 
						break;
					case 7:
						strcpy(messagestring,_T("세가 전용")); 
						break;
					}


					// 물품 이름 정보
					g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, g_ClothesItemProperty[uniqid].cName);
					g_XBaseFont->Print(m_WindowPosition.x + 15, m_WindowPosition.y + 9, 1.0f, "%s (%s)" , g_ClothesItemProperty[uniqid].cName,messagestring);
					//g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, g_ClothesItemProperty[uniqid].cName);

					
					// 물품 설명 정보 : 2004.06.16->oneway48 insert
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					
					// 2004.06.15->oneway48 insert
					int count = 0;
					for(int i = 0; i < 57; i++)
					{
						if( g_ClothesItemProperty[uniqid].cItemInfo[i] == 0 )
							break;
						count++;
					}
					
					if( count <= 30 )
					{
						g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 28, g_ClothesItemProperty[uniqid].cItemInfo);
					}
					else
					{
						TCHAR messagestring[64];
						memset(messagestring,0,sizeof(TCHAR)*64);
						for(int i = 0; i < 30; i++)
							messagestring[i] = g_ClothesItemProperty[uniqid].cItemInfo[i];
						g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 28, messagestring);
						
						memset(messagestring,0,sizeof(TCHAR)*64);
						for( i = 0; i < count-30; i++)
							messagestring[i] = g_ClothesItemProperty[uniqid].cItemInfo[30+i];
						g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 43, messagestring);
					}
					// insert end



					// 기본 정보
					g_XBaseFont->SetColor(_XSC_DEFAULT);
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 61, m_DescGenderString );// 성별 2004.06.11->oneway48 insert 
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 61 + 15, m_DescStrengthString );// 강도
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 76 + 15, m_DescWeightString );// 무게
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 91 + 15, m_DescDefenseString );// 방어력
					//g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 106 + 15, m_DescDurabilityString );// 내구성
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 106 + 15, _T("착용감") );// 내구성
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 121 + 15, m_DescWarmthString );// 보온효과
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 136 + 15, m_DescResistanceString );// 속성 저항력
					
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 155 + 15, m_DescHeartString );// 심기
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 170 + 15, m_DescStaminaString );// 정기 
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 185 + 15, m_DescLevelString );// 내공수위
					
					// 2004.06.11->oneway48 
					if( g_ClothesItemProperty[uniqid].cSexType == 0 )  // 남자
					{
						g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("남자")); 
					}
					else if(g_ClothesItemProperty[uniqid].cSexType == 1)  // 여자
					{
						g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("여자")); 
					}
					else // 공통
					{
						g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("공통")); 
					}
						
					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 61 + 15, 1.0f, "%d" , g_ClothesItemProperty[uniqid].cStrength); // 강도

					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76 + 15, 1.0f, "%d" , g_ClothesItemProperty[uniqid].sClanPoint1); // 무게 

					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 91 + 15, 1.0f, "%d" , g_ClothesItemProperty[uniqid].sDefence); // 방어력
					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 106 + 15, 1.0f, "%d" , g_ClothesItemProperty[uniqid].usDurability);// 내구성
					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 121 + 15, 1.0f, "%d" , g_ClothesItemProperty[uniqid].cWarm);// 보온효과
					
					// 2004.06.14->oneway48 insert
					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 155+ 15, 1.0f, "%d" , g_ClothesItemProperty[uniqid].sReqJung);
					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 170+ 15, 1.0f, "%d" , g_ClothesItemProperty[uniqid].sReqSim);
					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 185+ 15, 1.0f, "%d" , g_ClothesItemProperty[uniqid].sReqLevel);

					// 물품 가격 박스
					_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+217, m_WindowPosition.x+194, m_WindowPosition.y+235 , D3DCOLOR_ARGB(100,0,0,0) );
					_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+216, m_WindowPosition.x+194, m_WindowPosition.y+235 , 0.0f, _XSC_DEFAULT_BLACK );
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 220, m_DescPrice );// 가격
					g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 220, 1.0f, "%d" , g_ClothesItemProperty[uniqid].uiPrice);
					
				
				// 2004.05.18->oneway48 modify
	/*				
					if(g_ClothesItemProperty[uniqid].제련속성)
					{	
						// 물품 제련 정보
						_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+202, m_WindowPosition.x+194, m_WindowPosition.y+233  + 15, D3DCOLOR_ARGB(100,0,0,0) );
						_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+201, m_WindowPosition.x+194, m_WindowPosition.y+233  + 15, 0.0f, _XSC_DEFAULT_BLACK );
					
						g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 204 + 15, m_DescRefine1String );// 제련 속성 1
						g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 219 + 15, m_DescRefine2String );// 제련 속성 2
					  
					}
						
					if(g_ClothesItemProperty[uniqid].부과효과)
					{						
						// 부과 효과 정보 박스
						_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+236, m_WindowPosition.x+194, m_WindowPosition.y+282 + 15 , D3DCOLOR_ARGB(100,0,0,0) );
						_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+235, m_WindowPosition.x+194, m_WindowPosition.y+282 + 15 , 0.0f, _XSC_DEFAULT_BLACK );
						  
						g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,219,196,136));
							
						g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 239 + 15, m_DescAdditionalEffect1String );// 부가 효과 1 
						g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 254 + 15, m_DescAdditionalEffect2String );// 부가 효과 2
						g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 269 + 15, m_DescAdditionalEffect3String );// 부가 효과 3
					}
												
					if(g_ClothesItemProperty[uniqid].세트효과)
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
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, g_PotionItemProperty[uniqid].cName);
				g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, g_PotionItemProperty[uniqid].cName);

				
				// 물품 설명 정보 : 2004.06.16->oneway48 insert
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				
				// 2004.06.15->oneway48 insert
				int count = 0;
				for(int i = 0; i < 57; i++)
				{
					if( g_PotionItemProperty[uniqid].cItemInfo[i] == 0 )
						break;
					count++;
				}
				
				if( count <= 30 )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 28, g_PotionItemProperty[uniqid].cItemInfo);
				}
				else
				{
					TCHAR messagestring[64];
					memset(messagestring,0,sizeof(TCHAR)*64);
					for(int i = 0; i < 30; i++)
						messagestring[i] = g_PotionItemProperty[uniqid].cItemInfo[i];
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 28, messagestring);
					
					memset(messagestring,0,sizeof(TCHAR)*64);
					for( i = 0; i < count-30; i++)
						messagestring[i] = g_PotionItemProperty[uniqid].cItemInfo[30+i];
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 43, messagestring);
				}
				// insert end


				// 기본 정보
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 61, m_DescRollString );// 역할
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 76, m_DescEffectString );// 효능
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 91, m_DescFeatureString );// 특징
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 106, m_DescWeightString );// 무게
				
				// 2004.06.15->oneway48 insert
				switch(g_PotionItemProperty[uniqid].cSecondType) 
				{
				case 0:
					{
						g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("내상 치료제") );
						switch(g_PotionItemProperty[uniqid].cApplyType) 
						{					
						case 0:
							g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76 , 1.0f, "내상 %d 감소" , g_PotionItemProperty[uniqid].sApplyValue); // 무게 
							break;
						case 1:
							g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76 , 1.0f, "내상 %d%% 감소" , g_PotionItemProperty[uniqid].sApplyValue); // 무게 
							break;
						}
					}
					break;
				case 1:
					{
						g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("외상 치료제") );
						switch(g_PotionItemProperty[uniqid].cApplyType) 
						{					
						case 0:
							g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76 , 1.0f, "외상 %d 감소" , g_PotionItemProperty[uniqid].sApplyValue); // 무게 
							break;
						case 1:
							g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 76 , 1.0f, "외상 %d%% 감소" , g_PotionItemProperty[uniqid].sApplyValue); // 무게 
							break;
						}
					}
					break;
				case 2:
					g_XBaseFont->Puts(m_WindowPosition.x + 100, m_WindowPosition.y + 61, _T("피로 회복제") );
					break;
				}

				// 2004.06.17->oneway48 insert

				switch(g_PotionItemProperty[uniqid].cClan) 
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

				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 106, 1.0f, "%d" , g_PotionItemProperty[uniqid].sClanPoint1);// 무게

				// insert end

				// 물품 가격 박스
				_XDrawSolidBar( m_WindowPosition.x+6,  m_WindowPosition.y+123, m_WindowPosition.x+194, m_WindowPosition.y+141 , D3DCOLOR_ARGB(100,0,0,0) );
				_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+122, m_WindowPosition.x+194, m_WindowPosition.y+141 , 0.0f, _XSC_DEFAULT_BLACK );
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 126, m_DescPrice );// 가격
				g_XBaseFont->Print(m_WindowPosition.x + 100, m_WindowPosition.y + 126, 1.0f, "%d" , g_PotionItemProperty[uniqid].uiPrice);
			}
			
			break;
			
		case _XGI_FC_QUEST :
			{
				g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);
				
				// 물품 이름 정보
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, g_QuestItemProperty[uniqid].cName);
				g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, g_QuestItemProperty[uniqid].cName);
				
				// 물품 설명 정보 : 2004.06.16->oneway48 insert
				g_XBaseFont->SetColor(_XSC_DEFAULT);
				
				// 2004.06.15->oneway48 insert
				int count = 0;
				for(int i = 0; i < 57; i++)
				{
					if( g_QuestItemProperty[uniqid].cItemInfo[i] == 0 )
						break;
					count++;
				}
				
				if( count <= 30 )
				{
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 28, g_QuestItemProperty[uniqid].cItemInfo);
				}
				else
				{
					TCHAR messagestring[64];
					memset(messagestring,0,sizeof(TCHAR)*64);
					for(int i = 0; i < 30; i++)
						messagestring[i] = g_QuestItemProperty[uniqid].cItemInfo[i];
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 28, messagestring);
					
					memset(messagestring,0,sizeof(TCHAR)*64);
					for( i = 0; i < count-30; i++)
						messagestring[i] = g_QuestItemProperty[uniqid].cItemInfo[30+i];
					g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 43, messagestring);
				}
				// insert end
			}

			break;
			
		default :
			{
				g_XBaseFont->SetColor(_XSC_DEFAULT_HIGHLIGHT);

				// 물품 이름 정보
				g_XBaseFont->Puts(m_WindowPosition.x + 14, m_WindowPosition.y + 9, "NONAME");
				g_XBaseFont->Puts(m_WindowPosition.x + 15, m_WindowPosition.y + 9, "NONAME");
			}
			
			break;
		}		
	}

	g_XBaseFont->Flush();

/*
	// Border edge...
	_XDrawRectAngle( m_WindowPosition.x, m_WindowPosition.y, m_WindowPosition.x+m_WindowSize.cx-1, m_WindowPosition.y+m_WindowSize.cy-1, 0.0f, _XSC_DEFAULT_BLACK );
	_XDrawRectAngle( m_WindowPosition.x+1, m_WindowPosition.y+1, m_WindowPosition.x+m_WindowSize.cx-2, m_WindowPosition.y+m_WindowSize.cy-2, 0.0f, D3DCOLOR_ARGB(255,96,96,96) );
	_XDrawRectAngle( m_WindowPosition.x+2, m_WindowPosition.y+2, m_WindowPosition.x+m_WindowSize.cx-3, m_WindowPosition.y+m_WindowSize.cy-3, 0.0f, _XSC_DEFAULT_BLACK );
	
	m_ItemBorder.Draw();
	//_XDrawSolidBar( m_WindowPosition.x+6, m_WindowPosition.y+6, m_WindowPosition.x+66, m_WindowPosition.y+146 ,D3DCOLOR_ARGB(64,0,0,0) );
	//_XDrawRectAngle( m_WindowPosition.x+5, m_WindowPosition.y+5, m_WindowPosition.x+66, m_WindowPosition.y+146, 0.0f, _XSC_DEFAULT_BLACK );

	switch(m_Category1)
	{
	case _XGI_FC_WEAPON :
		g_XBaseFont->SetColor( _XSC_DEFAULT );
		g_XBaseFont->Puts(m_WindowPosition.x + 74, m_WindowPosition.y + 10, m_DescNameString );
		g_XBaseFont->Puts(m_WindowPosition.x + 140, m_WindowPosition.y + 10, 
						 g_WeaponItemProperty[m_Category3].cName);
		
		g_XBaseFont->SetColor( _XSC_INFORMATION );
		g_XBaseFont->Puts(m_WindowPosition.x + 74, m_WindowPosition.y + 40, m_DescClassString );
		g_XBaseFont->Puts(m_WindowPosition.x + 74, m_WindowPosition.y + 53, m_DescPowerString );
		g_XBaseFont->Puts(m_WindowPosition.x + 74, m_WindowPosition.y + 66, m_DescSuccessRateString );
		g_XBaseFont->Puts(m_WindowPosition.x + 74, m_WindowPosition.y + 79, m_DescWeightString );
		g_XBaseFont->Puts(m_WindowPosition.x + 74, m_WindowPosition.y + 92, m_DescRequireString );
		g_XBaseFont->Puts(m_WindowPosition.x + 74, m_WindowPosition.y + 105, m_DescEffectString );
		g_XBaseFont->Puts(m_WindowPosition.x + 74, m_WindowPosition.y + 118, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_BUYPRICE) );

		TCHAR	classstring[16];			
		memset( classstring, 0, sizeof(TCHAR) * 16 );
		switch( g_WeaponItemProperty[m_Category3].cSecondType ) 
		{			
		case _XGI_SC_WEAPON_SHORT				: 
			{
				///////////////////////////////////////////////////////////////////////////////////////////////////
				// _XGI_SC_WEAPON_SHORT
				switch( g_WeaponItemProperty[m_Category3].cThirdType )
				{
				case _XGI_TC_WEAPON_SWORD		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_SWORD));	break;
				case _XGI_TC_WEAPON_KNIFE		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_KNIFE) );	break;
				case _XGI_TC_WEAPON_HIT			 : strcpy(classstring, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_HIT) );	break;
				case _XGI_TC_WEAPON_FIST		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_FIST) );	break;
				case _XGI_TC_WEAPON_AXE			 : strcpy(classstring, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_AXE) );	break;
				case _XGI_TC_WEAPON_SECRET		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_SECRETEWEAPON) ); break;
				case _XGI_TC_WEAPON_DOUBLE		 : strcpy(classstring, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_DOUBLEHAND) );	break;					
				}
			}				
			break;
		case _XGI_SC_WEAPON_LONG				:
			{
				///////////////////////////////////////////////////////////////////////////////////////////////////
				// _XGI_SC_WEAPON_LONG
				switch( g_WeaponItemProperty[m_Category3].cThirdType )
				{
				case _XGI_TC_WEAPON_GLAIVE		: strcpy(classstring, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_GLAIVE) );  break;					
				case _XGI_TC_WEAPON_SPEAR		: strcpy(classstring, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_SPEAR) );	break;					
				case _XGI_TC_WEAPON_CLUB		: strcpy(classstring, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_CLUB) );	break;					
				case _XGI_TC_WEAPON_STAFF		: strcpy(classstring, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_STAFF) );	break;					
				case _XGI_TC_WEAPON_AXE			: strcpy(classstring, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_BATTLEAXE) );	break;					
				case _XGI_TC_WEAPON_RAKE		: strcpy(classstring, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_RAKE) );	break;					
				}
			}
			break;

		default :
			break;
		}
		g_XBaseFont->Puts(m_WindowPosition.x + 140, m_WindowPosition.y + 40, classstring );

		g_XBaseFont->Print( m_WindowPosition.x + 140, m_WindowPosition.y + 53, 1.0f, "%d - %d", 
						   g_WeaponItemProperty[m_Category3].sMinPower, 
						   g_WeaponItemProperty[m_Category3].sMaxPower);

		g_XBaseFont->Puts(m_WindowPosition.x + 140, m_WindowPosition.y + 79, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_NORMAL));
		g_XBaseFont->Print(m_WindowPosition.x + 140, m_WindowPosition.y + 118, 1.0f, "%d", 
						  g_WeaponItemProperty[m_Category3].uiPrice );
		
		break;
	case _XGI_FC_CLOTHES :
		g_XBaseFont->SetColor( _XSC_DEFAULT );
		g_XBaseFont->Puts(m_WindowPosition.x + 74, m_WindowPosition.y + 10, m_DescNameString );
		g_XBaseFont->Puts(m_WindowPosition.x + 140, m_WindowPosition.y + 10, 
						 g_ClothesItemProperty[m_Category3].cName);

		g_XBaseFont->Puts(m_WindowPosition.x + 74, m_WindowPosition.y + 40, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_BUYPRICE));
		g_XBaseFont->Print(m_WindowPosition.x + 140, m_WindowPosition.y + 40, 1.0f, "%d", 
						  g_ClothesItemProperty[m_Category3].uiPrice );

		TCHAR		genderstring[16];
		if(g_ClothesItemProperty[m_Category3].cSexType == _XGENDER_MAN)
			strncpy(genderstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_MAN), 15);
		else
			strncpy(genderstring, g_StrManager.GetString(ID_STRING_ITEMTOOLTIP_WOMAN), 15);

		g_XBaseFont->Puts(m_WindowPosition.x + 74, m_WindowPosition.y + 70, genderstring);
		
		break;
	case _XGI_FC_POTION :
		g_XBaseFont->SetColor( _XSC_DEFAULT );
		g_XBaseFont->Puts(m_WindowPosition.x + 74, m_WindowPosition.y + 10, m_DescNameString );
		g_XBaseFont->Puts(m_WindowPosition.x + 140, m_WindowPosition.y + 10, 
						 g_PotionItemProperty[m_Category3].cName);

		g_XBaseFont->Puts(m_WindowPosition.x + 74, m_WindowPosition.y + 40, g_StrManager.GetString(ID_STRING_TRADEITEMTOOLTIP_BUYPRICE));
		g_XBaseFont->Print(m_WindowPosition.x + 140, m_WindowPosition.y + 40, 1.0f, "%d", 
						  g_PotionItemProperty[m_Category3].uiPrice );
		break;
	default :
		g_XBaseFont->SetColor( _XSC_DEFAULT );
		g_XBaseFont->Puts(m_WindowPosition.x + 74, m_WindowPosition.y + 10, m_DescNameString );
		g_XBaseFont->Puts(m_WindowPosition.x + 140, m_WindowPosition.y + 10, 
			"NONAME");
		break;
	}
	g_XBaseFont->Flush();

	RenderItem();
*/
}

BOOL _XWindow_TradeItemTooltip::Process(_XGUIObject*& pfocusobject)
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
				if(CheckMousePosition())
					this->ShowWindow(FALSE);
		}
	}

	if(!_XWindow::Process(pfocusobject))
		return FALSE;

	return TRUE;
}

void _XWindow_TradeItemTooltip::ShowWindow(BOOL show)
{
	if(show)
	{
		m_pMyManager->SetTopWindow(this);
/*
		m_ModelViewPort.X = (DWORD)(m_WindowPosition.x + 12);
		m_ModelViewPort.Y = (DWORD)(m_WindowPosition.y + 12);
		m_ModelViewPort.Width = 50;
		m_ModelViewPort.Height = 130;
		m_ModelViewPort.MinZ = 0.0f;
		m_ModelViewPort.MaxZ = 0.0f;
*/
	}

	_XWindow::ShowWindow(show);
}

void _XWindow_TradeItemTooltip::MoveWindow(int X, int Y)
{
	_XWindow::MoveWindow(X, Y);
/*
	m_ItemBorder.MoveWindow( X + 6, Y + 5 );

	m_ModelViewPort.X = (DWORD)(m_WindowPosition.x + 12);
	m_ModelViewPort.Y = (DWORD)(m_WindowPosition.y + 12);
	m_ModelViewPort.Width = 50;
	m_ModelViewPort.Height = 130;
	m_ModelViewPort.MinZ = 0.0f;
	m_ModelViewPort.MaxZ = 0.0f;
*/
}
/*
void _XWindow_TradeItemTooltip::RenderItem(void)
{
	D3DXMATRIX		rotmat;
	D3DXMatrixIdentity(&rotmat);

	m_RotateAngle += 4.0f;
	D3DXMatrixRotationY(&rotmat, _X_RAD(m_RotateAngle));
	
	D3DXMatrixIdentity(&m_RHWMatrix);

	m_ModelViewCamera.UpdateViewMatrix( NULL, FALSE );
	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&m_ModelViewCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&m_ModelViewCamera.GetProjMatrix());
	gpDev->SetViewport(&m_ModelViewPort);

	gpDev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
	gpDev->SetRenderState( D3DRS_ALPHAREF, g_AlphaRefLevel );

	_XGENDERTYPE gender = _XGENDER_MAN;
	int			genderseperator = 0;
	int			groupseperator = 0;

	switch(m_Category1)
	{
	case _XGI_FC_WEAPON :
		{
			FLOAT scalefactor;
			if(g_WeaponItemProperty[m_Category3].cSecondType == _XGI_SC_WEAPON_SHORT)
			{
				if(g_WeaponItemProperty[m_Category3].cThirdType == _XGI_TC_WEAPON_FIST)
				{
					if(g_WeaponItemProperty[m_Category3].sWeaponLevel >= 1 && g_WeaponItemProperty[m_Category3].sWeaponLevel <= 15)
					{
						// 장갑
						scalefactor = 0.5f/(g_ItemModel_Weapon[g_WeaponItemProperty[m_Category3].sModelIndex]->OBB_Extent[1] + g_ItemModel_Weapon[g_WeaponItemProperty[m_Category3].sModelIndex]->OBB_Extent[1]);
						D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
						D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
						m_RHWMatrix._41 = - 0.03f;
						m_RHWMatrix._42 = 0.0f;
						m_RHWMatrix._43 = 0.0f;
						
						g_ItemModel_Weapon[g_WeaponItemProperty[m_Category3].sModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
						break;
					}
				}
			}
			scalefactor = 1.2f/(g_ItemModel_Weapon[g_WeaponItemProperty[m_Category3].sModelIndex]->OBB_Extent[2] + g_ItemModel_Weapon[g_WeaponItemProperty[m_Category3].sModelIndex]->OBB_Extent[2]);
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
			m_RHWMatrix._41 = - 0.03f;
			m_RHWMatrix._42 = -0.5f;
			m_RHWMatrix._43 = 0.0f;
			
			g_ItemModel_Weapon[g_WeaponItemProperty[m_Category3].sModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
	case _XGI_FC_CLOTHES :
		{
			switch(g_ClothesItemProperty[m_Category3].cSecondType)
			{
			case _XGI_SC_CLOTHES_UPPER :
				{
					FLOAT scalefactor = 1.5f; 
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -1.4f;
					m_RHWMatrix._43 = 1.5f;
					
					g_UserModel_UBodyType[g_ClothesItemProperty[m_Category3].sModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_LOWER :
				{
					FLOAT scalefactor = 1.5f; 
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -0.8f;
					m_RHWMatrix._43 = 1.5f;

					g_UserModel_LBodyType[g_ClothesItemProperty[m_Category3].sModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_GLOVES :
				{
					FLOAT scalefactor = 2.0f; 
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = 0.0f;
					m_RHWMatrix._43 = 1.5f;

					g_UserModel_LHandType[g_ClothesItemProperty[m_Category3].sModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
				}
				break;
			case _XGI_SC_CLOTHES_SHOES :
				{
					FLOAT scalefactor = 1.5f; 
					D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
					D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &rotmat);
					m_RHWMatrix._41 = 0.0f;
					m_RHWMatrix._42 = -0.2f;
					m_RHWMatrix._43 = 1.5f;
					
					g_UserModel_LFootType[g_ClothesItemProperty[m_Category3].sModelIndex]->RenderInterfaceItem(&m_RHWMatrix);
				}
				break;
			default :
				break;
			}
		}
		break;
	case _XGI_FC_POTION :
		{
			FLOAT scalefactor = 5.5f;
			D3DXMATRIX p_rotmat;
			
			D3DXMatrixScaling(&m_RHWMatrix, scalefactor, scalefactor, scalefactor);
			D3DXMatrixRotationX(&p_rotmat, _X_RAD(90));
			D3DXMatrixMultiply(&m_RHWMatrix, &m_RHWMatrix, &p_rotmat);
			m_RHWMatrix._41 = 0.0f;
			m_RHWMatrix._42 = -0.2f;
			m_RHWMatrix._43 = 0.0f;
			
			g_ItemModel_Potion[m_Category3]->RenderInterfaceItem(&m_RHWMatrix);
		}
		break;
		
	default :
		break;
	}

	gpDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&g_LodTerrain.m_3PCamera.GetViewMatrix());
	gpDev->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&g_LodTerrain.m_3PCamera.GetProjMatrix());
	gpDev->SetViewport(&g_MainViewPort);
}
*/