// XSetItemInfomation.cpp: implementation of the XSetItemInfomation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "EODEXTDEF_GlobalObject.h"
#include "XWindowObjectDefine.h"
#include "XProc_ProcessDefine.h"
#include "XSR_STRINGHEADER.H"

#include "XMLPaser.h"
#include "XSetItemInfomation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XSetItemInfomation::XSetItemInfomation(void) : m_nAccouteredItem(0), m_nSkillStringCount(0), m_nSetItemStringCount(0), 
											   m_mpData(), m_ltWearItem(), m_ltEffectID(), m_mpSkillInfo()
{
}

XSetItemInfomation::~XSetItemInfomation(void)
{
	map<int, SetInfo*>::iterator iter;

	if( m_mpData.size() > 0 )
	{
		for( iter = m_mpData.begin(); iter != m_mpData.end(); ++iter )
		{
			SetInfo* pSetInfo = iter->second;

			if( pSetInfo )
			{
                delete pSetInfo;
				pSetInfo = NULL;
			}
		}
		m_mpData.clear();
	}

	m_ltEffectID.clear();
	m_mpSkillInfo.clear();
	m_ltWearItem.clear();
}

HRESULT XSetItemInfomation::LoadXMLTable( XMLPaser& XMLPaser, LPTSTR pTableName )
{
	if( !XMLPaser.ND_isThereTable( pTableName ) )
	{
		return S_FALSE;
	}
	
	int TableRowCount = XMLPaser.ND_nGetTableRowCount( pTableName );

	if(TableRowCount <=0)	
		return S_FALSE;

    TableRowCount--;


	//예외 처리 해야하는데.. 안했음..
	map<int, SetInfo*>::iterator infoiter;
	HRESULT hr;
    int nGroup = 0;
	m_ltEffectID.clear();

	for( int i = 0; i < TableRowCount; i++ )
	{
		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Group", i, &nGroup ) ))
			return (hr);

		//아이템 정보
		ItemInfo* pItemInfo = new ItemInfo;

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Item Type", i, &pItemInfo->nType) ))
			return (hr);

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Item ID", i, &pItemInfo->nID) ))
			return (hr);

		//아이템 세부 정보
		ItemDetailInfo* pDetailInfo = new ItemDetailInfo;
		SkillInfo* pSkillInfo_1 = new SkillInfo;
		SkillInfo* pSkillInfo_2 = new SkillInfo;
		SkillInfo* pSkillInfo_3 = new SkillInfo;

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "In Item", i, &pDetailInfo->nAccouteredItem ) ))
			return (hr);

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Effect ID", i, &pDetailInfo->nEffectID ) ))
			return (hr);

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Skill ID_1", i, &pSkillInfo_1->nSkillID) ))
			return (hr);

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Skill Value_1", i, &pSkillInfo_1->nSkillValue) ))
			return (hr);

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Skill ID_2", i, &pSkillInfo_2->nSkillID) ))
			return (hr);

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Skill Value_2", i, &pSkillInfo_2->nSkillValue) ))
			return (hr);

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Skill ID_3", i, &pSkillInfo_3->nSkillID) ))
			return (hr);

		if( FAILED( hr = XMLPaser.ND_hrGetTableValue( pTableName, "Skill Value_3", i, &pSkillInfo_3->nSkillValue) ))
			return (hr);

		if( pDetailInfo->nEffectID )
			m_ltEffectID.push_back( pDetailInfo->nEffectID );

		pDetailInfo->ltSkillInfo.push_back( pSkillInfo_1 );
		pDetailInfo->ltSkillInfo.push_back( pSkillInfo_2 );
		pDetailInfo->ltSkillInfo.push_back( pSkillInfo_3 );

		infoiter = m_mpData.find( nGroup ); 

		//같은 키값 존재 // 버그를 양산 할 수 있지만 스크립트가 절대 안틀리길 바라며
		if( infoiter != m_mpData.end() )
		{
			SetInfo* pSetInfo = infoiter->second;

			//아이템 정보 저장
			pSetInfo->nGroupID = nGroup;
			pSetInfo->vtItem.push_back( pItemInfo );
			pSetInfo->ltItemInfo.push_back( pDetailInfo );
		}
		else
		{
			SetInfo* pSetInfo = new SetInfo;

			pSetInfo->nGroupID = nGroup;
			pSetInfo->vtItem.push_back( pItemInfo );
			pSetInfo->ltItemInfo.push_back( pDetailInfo );

			m_mpData[nGroup] = pSetInfo;
		}
	}

	return S_OK;
}

int XSetItemInfomation::SearchGroupItem(int nGroup)
{
	map<int, SetInfo*>::iterator iter;
	iter = m_mpData.find( nGroup );

	if( iter == m_mpData.end() )
		return false;

	SetInfo* pSetInfo = iter->second;

	vector<ItemInfo*>::iterator vtiter = pSetInfo->vtItem.begin(); //아이템 정보 가져오기
	m_nAccouteredItem = 0; //몇개를 입고있는지
	m_ltWearItem.clear();

	for(; vtiter != pSetInfo->vtItem.end(); ++vtiter)
	{
		for( int i = 0; i < _XINVENTORY_SLOTTYPE_POCKETSTART; i++)
		{
			if( i == 11 ) // i 가 11번일때.. W 무기 밑에 있을 때.. 이땐 입고있는 걸로 생각 안한다
				continue;

			if( g_pLocalUser->m_UserItemList[i].Get_m_cType() == (*vtiter)->nType &&
				g_pLocalUser->m_UserItemList[i].Get_m_sID() == (*vtiter)->nID ) // 이게 같으면 착용 중
			{
				ItemInfo info;
				info.nType = (*vtiter)->nType;
				info.nID = (*vtiter)->nID;
				m_ltWearItem.push_back( info ); //현재 착용중인 정보에 저장
				m_nAccouteredItem++;
			}
		}
	}

	return m_nAccouteredItem;
}

int XSetItemInfomation::SetGroupSkillString( int nGroup, int nAccouter, SIZE TooltipSize, _ITEMTOOLTIP_INFO& TooltipInfo )
{
	map<int, SetInfo*>::iterator iter;
	iter = m_mpData.find( nGroup );

	if( iter == m_mpData.end() )
		return 0;

	SetInfo* pSetInfo = iter->second;

	//iter 를 이중으로 써야한다.. 스킬은 전부 적용이 되야하기때문임..
	//와,, 보기 열라 불편하다..
	list<ItemDetailInfo*>::iterator Detailiter = pSetInfo->ltItemInfo.begin(); //아이템세부정보..
	list<SkillInfo*>::iterator Skilliter; //아이템정보안에 스킬정보

	m_nSkillStringCount = 0;

	for(int i = 0; Detailiter != pSetInfo->ltItemInfo.end(); ++Detailiter)
	{
        //08년 6월 11일 스킬은 전부다 나오게 바뀌었다..
		//if( (*Detailiter)->nAccouteredItem > nAccouter ) //착용한 개수가 초과되면 멈춘다
		//	break;

		for( Skilliter = (*Detailiter)->ltSkillInfo.begin(); Skilliter != (*Detailiter)->ltSkillInfo.end(); ++Skilliter )
		{
			if((*Skilliter)->nSkillID > 0 ) // 스킬 아이디가 0 이 아니면..
			{
				_snprintf( TooltipInfo.SetItemSkill_Data_Buf[i], sizeof(TooltipInfo.SetItemSkill_Data_Buf[i]),
					g_CharacterStateTable[(*Skilliter)->nSkillID]->concept, (*Skilliter)->nSkillValue);

				int length = g_XBaseFont->GetRowCount( TooltipInfo.SetItemSkill_Data_Buf[i], TooltipSize.cx-16 );
				i++;
				m_nSkillStringCount += length; //한줄일땐 length는 1 두줄일떄는 2
			}
		}
	}

	return m_nSkillStringCount;
}

int XSetItemInfomation::SetGroupItemString( int nGroup, _ITEMTOOLTIP_INFO& TooltipInfo )
{
	map<int, SetInfo*>::iterator iter;
	iter = m_mpData.find( nGroup );

	if( iter == m_mpData.end() )
		return 0;

	SetInfo* pSetInfo = iter->second;

	vector<ItemInfo*>::iterator vtiter = pSetInfo->vtItem.begin(); //아이템 정보 가져오기

	m_nSetItemStringCount = 0; //스트링 위치 체크

	for(; vtiter != pSetInfo->vtItem.end(); ++vtiter)
	{
		switch( (*vtiter)->nType ) //타입에 따라 이름을 불러오는 방식이 틀려..
		{
		case _XGI_FC_WEAPON:
		case _XGI_FC_WEAPON2:
		case _XGI_FC_WEAPON3:
			{
				_XGI_WeaponItem_Property* tempWeaponItem = (_XGI_WeaponItem_Property*)GetFirstTypeItemPorperty((*vtiter)->nType);
				if( _XGameItem::GetItemNameAddtoString((*vtiter)->nType, (*vtiter)->nID) ) //  이게 아마 _XTS_ITEM_V20 이상이면 이렇게 되는듯
				{
					_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]),
						"%s %s", _XGameItem::GetItemNameAddtoString((*vtiter)->nType,(*vtiter)->nID), tempWeaponItem[(*vtiter)->nID].cName );
				}
				else
				{
					if((*vtiter)->nType == _XGI_FC_WEAPON2)
					{
						_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]), 
						"%s", g_WeaponItemProperty2[(*vtiter)->nID].cName );
					}
					else if((*vtiter)->nType == _XGI_FC_WEAPON3)
					{
						_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]),
						"%s", g_WeaponItemProperty3[(*vtiter)->nID].cName );
					}
					else
					{
						_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]), 
							"%s", tempWeaponItem[(*vtiter)->nID].cName );
					}
				}
				
				m_nSetItemStringCount++;
			}
			break;
		case _XGI_FC_CLOTHES:
		case _XGI_FC_CLOTHES2:
		case _XGI_FC_CLOTHES3:
			{
				_XGI_ClothesItem_Property* tempClothesItem = (_XGI_ClothesItem_Property*)GetFirstTypeItemPorperty((*vtiter)->nType);
				if( _XGameItem::GetItemNameAddtoString((*vtiter)->nType, (*vtiter)->nID) )
				{
					_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]), 
						"%s %s", _XGameItem::GetItemNameAddtoString((*vtiter)->nType,(*vtiter)->nID), tempClothesItem[(*vtiter)->nID].cName);
				}
				else
				{
					if((*vtiter)->nType == _XGI_FC_CLOTHES2)
					{
						_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]), 
							"%s", g_ClothesItemProperty2[(*vtiter)->nID].cName  );
					}
					else if((*vtiter)->nType == _XGI_FC_CLOTHES3)
					{
						_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]),
							"%s", g_ClothesItemProperty3[(*vtiter)->nID].cName  );
					}
					else
					{
						_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]),
							"%s", tempClothesItem[(*vtiter)->nID].cName );
					}
				}		
				m_nSetItemStringCount++;
			}
			break;
		case _XGI_FC_ACCESSORY :
			{
				_snprintf( TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount], sizeof(TooltipInfo.SetItem_Data_Buf[m_nSetItemStringCount]), 
					"%s", g_AccessoryItemProperty[(*vtiter)->nID].cName );
				m_nSetItemStringCount++;
			}
			break;
		default :
			{
				//여긴 오면 안됨..
			}
			break;
		}
	}

	return m_nSetItemStringCount;
}

void XSetItemInfomation::DrawToolTip( int nGroup, int DrawX, int DrawY, _ITEMTOOLTIP_INFO& TooltipInfo )
{
	map<int, SetInfo*>::iterator iter;
	iter = m_mpData.find( nGroup );

	if( iter == m_mpData.end() )
		return;

	SetInfo* pSetInfo = iter->second;

	vector<ItemInfo*>::iterator vtiter = pSetInfo->vtItem.begin(); //아이템 정보 가져오기
	list<ItemInfo>::iterator ltiter;  //현재 입고있는 옷 정보
	
	//이중 iter -_-ㅋㅋㅋㅋ
	for(int i = 0; vtiter != pSetInfo->vtItem.end(); i++, ++vtiter)
	{
		g_XBaseFont->SetColor(_XSC_DEFAULT);
		for(ltiter = m_ltWearItem.begin(); ltiter != m_ltWearItem.end(); ++ltiter )
		{
			if( (*vtiter)->nType == (*ltiter).nType &&
				(*vtiter)->nID == (*ltiter).nID )
			{
				//있으면 색을 변경 해주고.. 소스 썩는다..
				g_XBaseFont->SetColor(0xFFFFD901);	
				break;
			}
		}

		//아이템 이름 차례대로 그리는거..
		g_XBaseFont->PutsAlign( DrawX, DrawY, _XFONT_ALIGNTYPE_LEFT, TooltipInfo.SetItem_Data_Buf[i] );
		DrawY += 16;

		g_XBaseFont->Flush();
	}
}

void XSetItemInfomation::DrawSkillToolTip( int nGroup, int DrawX, int DrawY, SIZE TooltipSize, int &nCurrentDrawYModify, _ITEMTOOLTIP_INFO& TooltipInfo )
{
    map<int, SetInfo*>::iterator iter;
	iter = m_mpData.find( nGroup );

	if( iter == m_mpData.end() )
		return;

	SetInfo* pSetInfo = iter->second;

	//iter 를 이중으로 써야한다.. 스킬은 전부 적용이 되야하기때문임..
	//와,, 보기 열라 불편하다..
	list<ItemDetailInfo*>::iterator Detailiter = pSetInfo->ltItemInfo.begin(); //아이템세부정보..
	list<SkillInfo*>::iterator Skilliter; //아이템정보안에 스킬정보

	for(int i = 0; Detailiter != pSetInfo->ltItemInfo.end(); ++Detailiter) 
	{
		for( Skilliter = (*Detailiter)->ltSkillInfo.begin(); Skilliter != (*Detailiter)->ltSkillInfo.end(); ++Skilliter )
		{
			if((*Skilliter)->nSkillID > 0 ) // 스킬 아이디가 0 이 아니면..
			{
                if( (*Detailiter)->nAccouteredItem > m_nAccouteredItem ) //착용한 개수가 초과
                    g_XBaseFont->SetColor(_XSC_DEFAULT);
                else
                    g_XBaseFont->SetColor(D3DCOLOR_ARGB(255,3,253,62));

				//줄바꿈을 할수 있게 바꿔야한다. 08/09/04
				g_XBaseFont->Puts_Separate( DrawX, DrawY, TooltipInfo.SetItemSkill_Data_Buf[i], TooltipSize.cx - 16, 1.0f, 3 );
				int templength = g_XBaseFont->GetRowCount( TooltipInfo.SetItemSkill_Data_Buf[i], TooltipSize.cx - 16 );
         
                i++;
                DrawY = DrawY + (16 * templength);
                nCurrentDrawYModify = nCurrentDrawYModify + (16 * templength); //이건,,, 디폴트 툴팁에  m_CurrentDrawPosY 이건데,, 스킬 밑에도 다른 스트링이 있어서 유지해 줘야한다
			}
		}
	}
}

bool XSetItemInfomation::GetWearItem( int nGroup, int nType, int nID )
{
	map<int, SetInfo*>::iterator iter;
	iter = m_mpData.find( nGroup );

	if( iter == m_mpData.end() )
		return false;

	SetInfo* pSetInfo = iter->second;

	vector<ItemInfo*>::iterator vtiter = pSetInfo->vtItem.begin(); //아이템 정보 가져오기

	for(; vtiter != pSetInfo->vtItem.end(); ++vtiter)
	{
		if( ((*vtiter)->nType == nType) && ((*vtiter)->nID == nID) ) //타입이랑 아이디가 같음
		{
			return true;
		}
	}

	return false;
}

void XSetItemInfomation::GroupItemSkillUpdate()
{
	map<int, int>::iterator iter;
	list<int>::iterator ltiter;
	m_mpSkillInfo.clear();

	for( int i = 0; i < _XINVENTORY_SLOTTYPE_POCKETSTART; i++)
	{
		if( i == 11 ) // i 가 11번일때.. W 무기 밑에 있을 때.. 이땐 입고있는 걸로 생각 안한다
			continue;

		int nGroup = _XGameItem::GetSetItem(g_pLocalUser->m_UserItemList[i].Get_m_cType(), g_pLocalUser->m_UserItemList[i].Get_m_sID());
		if( nGroup == 0 )
			continue; 

		iter = m_mpSkillInfo.find( nGroup ); //해당 그룹이 있는지

		if( iter != m_mpSkillInfo.end() ) // 해당 그룹 발견
		{
			int nCount = iter->second; //해당그룹의 입읏 옷의 개수를 받아오고
			nCount++;
			m_mpSkillInfo[nGroup] = nCount; // 개수를 증가 시켜준다..
		}
		else
		{
			m_mpSkillInfo[nGroup] = 1; // X 그룹을 첫번째로 발견..
		}
	} //대입 끝..

	//전체 이펙트 삭제
	for(ltiter = m_ltEffectID.begin(); ltiter != m_ltEffectID.end(); ++ltiter )
	{
		int nEffectID = (*ltiter);
		if( g_pLocalUser->m_ModelDescriptor.m_EffectManager )
		{
			if( g_pLocalUser->m_ModelDescriptor.m_EffectManager->FindInstance(nEffectID) )
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(nEffectID);
		}
	}

	for(iter = m_mpSkillInfo.begin(); iter != m_mpSkillInfo.end(); ++iter)
	{
		int nSetGroup = iter->first; // 몇번째 그룹인가..
		int nAccoutered = iter->second; //그룹의 옷을 몇개 입고 있나..

		UpdateGroupSkillInfo( nSetGroup, nAccoutered );
	}

	m_mpSkillInfo.clear();
}

void XSetItemInfomation::UpdateGroupSkillInfo(int nGroup, int nAccouter )
{
	map<int, SetInfo*>::iterator iter;
	iter = m_mpData.find( nGroup );

	if( iter == m_mpData.end() )
		return;

	SetInfo* pSetInfo = iter->second;

	//iter 를 이중으로 써야한다.. 스킬은 전부 적용이 되야하기때문임..
	//와,, 보기 열라 불편하다..
	list<ItemDetailInfo*>::iterator Detailiter = pSetInfo->ltItemInfo.begin(); //아이템정보..
	list<SkillInfo*>::iterator Skilliter;  //아이템정보안에 스킬정보

	for(; Detailiter != pSetInfo->ltItemInfo.end(); ++Detailiter)
	{
		if( (*Detailiter)->nAccouteredItem > nAccouter ) //착용한 개수가 초과되면 멈춘다
			break;

		if( (*Detailiter)->nEffectID > 0 ) // 이펙트 출력
		{
			int nEffectID = (*Detailiter)->nEffectID;
			if( !g_pLocalUser->m_ModelDescriptor.m_EffectManager->FindInstance(nEffectID) )
			{
				g_pLocalUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
																					nEffectID, &g_pLocalUser->m_ModelDescriptor.m_Position, NULL, TRUE );
			}			
		}

		for(Skilliter = (*Detailiter)->ltSkillInfo.begin(); Skilliter != (*Detailiter)->ltSkillInfo.end(); ++Skilliter )
		{
			if((*Skilliter)->nSkillID <= 0 ) // 스킬 아이디가 0 이 아니면..
				continue;

			int nID = (*Skilliter)->nSkillID;
			int nValue = (*Skilliter)->nSkillValue;
			
			//스킬 업데이트..
			map <int, _XCharacterStateTable*>::iterator iter_table = g_CharacterStateTable.find(nID);
			if(iter_table == g_CharacterStateTable.end())
				continue;
			
			if(!(g_CharacterStateTable[nID]->effect_prop == _XCL_EFFECT_PROP_GENERAL ||
			     g_CharacterStateTable[nID]->effect_prop == _XCL_EFFECT_PROP_ADDITIVE))
				continue;

			switch(g_CharacterStateTable[nID]->where)
			{
			case en_eff_where_strength :		// 근력
				{
					g_pLocalUser->m_StateList.AddStrengthBonus(nID, nValue);
				}
				break;
			case en_eff_where_zen :				// 진기
				{
					g_pLocalUser->m_StateList.AddZenBonus(nID, nValue);
				}
				break;
			case en_eff_where_intelligence :	// 지혜
				{
					g_pLocalUser->m_StateList.AddIntBonus(nID, nValue);
				}
				break;
			case en_eff_where_constitution :	// 건강
				{
					g_pLocalUser->m_StateList.AddConstitutionBonus(nID, nValue);
				}
				break;
			case en_eff_where_dexterity :		// 민첩
				{
					g_pLocalUser->m_StateList.AddDexBonus(nID, nValue);
				}
				break;
			case en_eff_where_all_stat :
				{
					g_pLocalUser->m_StateList.AddStrengthBonus(nID, nValue);
					g_pLocalUser->m_StateList.AddZenBonus(nID, nValue);
					g_pLocalUser->m_StateList.AddIntBonus(nID, nValue);
					g_pLocalUser->m_StateList.AddConstitutionBonus(nID, nValue);
					g_pLocalUser->m_StateList.AddDexBonus(nID, nValue);
				}
				break;
			case en_eff_where_attack_rate :			// 물리 공성
				g_pLocalUser->m_StateList.AddPAttackRateBonus(nID, nValue);
				break;
			case en_eff_where_avoid_rate :			// 물리 회피
				g_pLocalUser->m_StateList.AddPAvoidRateBonus(nID, nValue);
				break;
			case en_eff_where_attack_damage :		// 물리 공격
				g_pLocalUser->m_StateList.AddPAttackDamageBonus(nID, nValue);
				break;
			case en_eff_where_defence :				// 물리 방어
				g_pLocalUser->m_StateList.AddDefenceBonus(nID, nValue);
				break;
			case en_eff_where_m_attack_rate :		// 기공 공성
				g_pLocalUser->m_StateList.AddMAttackRateBonus(nID, nValue);
				break;
			case en_eff_where_m_avoid_rate :		// 기공 회피
				g_pLocalUser->m_StateList.AddMAvoidRateBonus(nID, nValue);
				break;
			case en_eff_where_m_attack_damage :		// 기공 공격
				g_pLocalUser->m_StateList.AddMAttackDamageBonus(nID, nValue);
				break;
			case en_eff_where_m_defence :			// 기공 방어
				g_pLocalUser->m_StateList.AddMDefenceBonus(nID, nValue);
				break;
			case en_eff_where_max_life :			// 최대 생명
				g_pLocalUser->m_StateList.AddMaxLifeBonus(nID, nValue);				
				break;
			case en_eff_where_max_force :			// 최대 내공
				g_pLocalUser->m_StateList.AddMaxForceBonus(nID, nValue);
				break;
			case en_eff_where_max_concentration :	// 최대 의지
				g_pLocalUser->m_StateList.AddMaxConcentrationBonus(nID, nValue);
				break;
			case en_eff_where_move_speed :			// 이동 속도
				g_pLocalUser->m_StateList.AddMoveSpeedBonus(nID, nValue);
				break;
			case en_eff_where_weapon_dam :			// 무기 공격력 증가
				g_pLocalUser->m_StateList.AddWeaponDamage(nID, nValue);
				break;
			case en_eff_where_weapon_min_dam :		// 무기 최소 공격력 증가
				g_pLocalUser->m_StateList.AddWeaponMinDamage(nID, nValue);
				break;
			case en_eff_where_weapon_max_dam :		// 무기 최대 공격력 증가
				g_pLocalUser->m_StateList.AddWeaponMaxDamage(nID, nValue);
				break;
			case en_eff_where_resist_minus :		// 음의 저항력
				g_pLocalUser->m_StateList.AddResistMinus(nID, nValue);
				break;
			case en_eff_where_resist_plus :			// 양의 저항력
				g_pLocalUser->m_StateList.AddResistPlus(nID, nValue);
				break;
			case en_eff_where_resist_hon :			// 혼의 저항력
				g_pLocalUser->m_StateList.AddResistHon(nID, nValue);
				break;
			case en_eff_where_resist_sin :			// 신의 저항력
				g_pLocalUser->m_StateList.AddResistSin(nID, nValue);
				break;
			case en_eff_where_resist_all :			// 모든 저항력 증가
				g_pLocalUser->m_StateList.AddResistAll(nID, nValue);
				break;
			case en_eff_where_add_minus_damage :		// 음의 데미지 추가
				g_pLocalUser->m_StateList.AddAttackDamageMinus(nID, nValue);
				break;
			case en_eff_where_add_plus_damage :			// 양의 데미지 추가
				g_pLocalUser->m_StateList.AddAttackDamagePlus(nID, nValue);
				break;
			case en_eff_where_add_hon_damage :			// 혼의 데미지 추가
				g_pLocalUser->m_StateList.AddAttackDamageHon(nID, nValue);
				break;
			case en_eff_where_add_sin_damage :			// 신의 데미지 추가
				g_pLocalUser->m_StateList.AddAttackDamageSin(nID, nValue);
				break;
			}
		}
	}
	//음....
}

void XSetItemInfomation::UpdateAnotherUserEffect( _XUser* pUser, int nEffectID, int nEffectID1 )
{
	list<int>::iterator ltiter;

	//전체 이펙트 삭제
	for(ltiter = m_ltEffectID.begin(); ltiter != m_ltEffectID.end(); ++ltiter )
	{
		int nTempEffectID = (*ltiter);
		if( pUser->m_ModelDescriptor.m_EffectManager )
		{
			if( pUser->m_ModelDescriptor.m_EffectManager->FindInstance(nTempEffectID) )
				pUser->m_ModelDescriptor.m_EffectManager->DeleteInstance(nTempEffectID);
		}
	}

	if( nEffectID > 0 )
	{
		if( !pUser->m_ModelDescriptor.m_EffectManager->FindInstance(nEffectID) )
			pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
																		nEffectID, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE );
	}
	if( nEffectID1 > 1 )
	{
		if( !pUser->m_ModelDescriptor.m_EffectManager->FindInstance(nEffectID1) )
			pUser->m_ModelDescriptor.m_EffectManager->InsertNewInstance( &g_CharacterEffectGroupManager,
																		nEffectID1, &pUser->m_ModelDescriptor.m_Position, NULL, TRUE );
	}
}