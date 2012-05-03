// XSetItemInfomation.h: interface for the XSetItemInfomation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XSETITEMINFOMATION_H__99F6C2C5_9051_4BD5_9C4F_9D13682A61C2__INCLUDED_)
#define AFX_XSETITEMINFOMATION_H__99F6C2C5_9051_4BD5_9C4F_9D13682A61C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <vector>
#include <list>
using namespace std;

#include "XWindow_DefaultTooltip.h"

class XSetItemInfomation  
{
public:
	XSetItemInfomation(void);
	~XSetItemInfomation(void);

	HRESULT LoadXMLTable( XMLPaser& XMLPaser, LPTSTR pTableName );
	
	void GroupItemSkillUpdate(); //스킬 업데이트 할때 쓴다.
	
	int SearchGroupItem(int nGroup); //해당 그룹의 아이템을 몇개나 착용하고 있는지
	int SetGroupSkillString( int nGroup, int nAccouter, SIZE TooltipSize, _ITEMTOOLTIP_INFO& TooltipInfo ); //해당 그룹의 착용 개수로 스트링을 저장하자 //툴팁에 쓰인다 //리턴값은 몇줄인지
	int SetGroupItemString( int nGroup, _ITEMTOOLTIP_INFO& TooltipInfo ); // 해당 그룹의 스트링 //툴팁에 쓰인다 //리턴값은 몇줄 인지
	void DrawToolTip( int nGroup, int DrawX, int DrawY, _ITEMTOOLTIP_INFO& TooltipInfo ); //툴팁 색깔 표시 때문에 이렇게 해야하나,, 방법이 없음 아아아 고쳐야한다..
    void DrawSkillToolTip( int nGroup, int DrawX, int DrawY, SIZE TooltipSize, int &nCurrentDrawYModify, _ITEMTOOLTIP_INFO& TooltipInfo ); //스킬 툴팁을 전부그려줘야 하므로..
	
	bool GetWearItem( int nGroup, int nType, int nID ); // 현재 옷을 입고있는가?? 

	void UpdateAnotherUserEffect( _XUser* pUser, int nEffectID, int nEffectID1 ); // 다른 케릭터의 이펙트

public :
	const int GetSetItemAccoutered() const { return m_nAccouteredItem; }
	const int GetSkillStringCount() const { return m_nSkillStringCount; }
	const int GetSetItemStringCount() const { return m_nSetItemStringCount; }

private :
	void UpdateGroupSkillInfo(int nGroup, int nAccouter ); // 스킬 정보 그룹 아이디, 착용 개수

private:
	struct ItemInfo
	{
		int nType; //아이템의 타입
		int nID; //아이템의 아이디
	};

	struct SkillInfo
	{
		int nSkillID; //스킬 아이디
		int nSkillValue; //스킬 정보
	};
	
	struct ItemDetailInfo
	{
		int nAccouteredItem; //디버깅용 착용한 아이템 개수
        int nEffectID; //이펙트 아이디
		list<SkillInfo*> ltSkillInfo; //스킬 정보

		~ItemDetailInfo()
		{
			list<SkillInfo*>::iterator iter;

			for( iter = ltSkillInfo.begin(); iter != ltSkillInfo.end(); ++iter )
			{
				delete (*iter);
				(*iter) = NULL;
			}
			ltSkillInfo.clear();
		}
	};

	struct SetInfo
	{
		int nGroupID; // 그룹 아이디 디버깅용
		vector<ItemInfo*> vtItem; // 아이템의 종류
		list<ItemDetailInfo*> ltItemInfo; //착용한 개수에 따라 이펙트가 틀려짐 // 스킬은 증가됨..

		~SetInfo()
		{
			nGroupID = 0;
			vector<ItemInfo*>::iterator vtiter = vtItem.begin();
			list<ItemDetailInfo*>::iterator ltiter = ltItemInfo.begin();

			for(; vtiter != vtItem.end(); ++vtiter )
			{
				delete (*vtiter);
				(*vtiter) = NULL;
			}

			for(; ltiter != ltItemInfo.end(); ++ltiter )
			{
				delete (*ltiter);
				(*ltiter) = NULL;
			}

			vtItem.clear();
			ltItemInfo.clear();
		}
	};

private: //최악의 소스를 향하여..
	//툴팁을 누를 때 마다 갱신됨
    int m_nAccouteredItem; //착용한 아이템 개수
	int m_nSkillStringCount; //스킬 스트링 개수
	int m_nSetItemStringCount; //세트 아이템 스트링 개수 

	map<int, SetInfo*>	m_mpData; //그룹아이디, 그룹정보
	list<ItemInfo> m_ltWearItem; //현채 착용한 아이템 정보 저장// 아 이거 있음 안되는데.. //툴팁 색깔 때문에 쓰인다..
	
	list<int> m_ltEffectID; // 세트아이템 이펙트 모음.. Update 시 삭제해준다
	map<int, int> m_mpSkillInfo; //그룹아이디, 해당그룹 옷을 몇개 착용했는가.. //스킬업데이트할때 쓰임
};

#endif // !defined(AFX_XSETITEMINFOMATION_H__99F6C2C5_9051_4BD5_9C4F_9D13682A61C2__INCLUDED_)
