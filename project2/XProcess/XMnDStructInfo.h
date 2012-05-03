#pragma once
#include <algorithm>

//V 군웅 창에서 쓰이는 정보
struct MnDGroupList
{
	int nMnDClass; //스승, 일등제자,, 이런거 표현
	TCHAR szName[64]; //케릭터 아이디
	int nLevel; //유저 레벨
	DWORD nDiscipleContribution; //상대방 제자들 기여도
	int nCurrentZone; //존 인덱스 번호.. 어느 지역에 있는지
	int nPlayerState; //플레이어 상태 접속 중인지,, 그런거

	MnDGroupList::MnDGroupList()
	{
		nMnDClass = 0;
		memset(szName, 0, sizeof(szName));
		nLevel = 0;
		nDiscipleContribution = 0;
		nCurrentZone = 0;
		nPlayerState = 0;
	}

	bool operator< ( const MnDGroupList* rhs )
	{
		return nMnDClass < rhs->nMnDClass;
	}

};

//정렬 할려고 쓴건데,, 맴버 템플릿은 VC6 이 제대로 지원을 하지 못한다..
//그냥 사용 하면됨 STL 에 물리면 링크에러 그럼 static 으로 해야하나??
template<typename T, T MnDGroupList::*p>
bool MnDGroupSort( MnDGroupList* lhs, MnDGroupList* rhs )
{
	return lhs->*p < rhs->*p;
}

static bool MnDGroupClassSort( MnDGroupList* lhs, MnDGroupList* rhs ) //스승 제자 순으로 정렬
{
	return lhs->nMnDClass < rhs->nMnDClass;
}

//NPC 사제정보창, 메신저 제자 대화창의 스승정보로 같이쓴다
struct MasterInfo
{
	int nRank; //등급
	TCHAR szMasterNickName[64]; // 사호
	TCHAR szMasterName[64]; // 스승 이름
	int nMoonpaindex; //문파 정보
	int nClassInfo; //직책 정보
	int nOrganizationClass; //영웅단 단주인지 단원인지 정보

	MasterInfo::MasterInfo()
	{
		nRank = 0;
		memset( szMasterNickName, 0, sizeof(szMasterNickName));
		memset( szMasterName, 0, sizeof(szMasterName));
		nMoonpaindex = 0;
		nClassInfo = 0;
		nOrganizationClass = 0;
	}
};

//제자 정보 메신저 대화장에 제자 정보
struct DiscipleInfo
{
	int nLevel; //레벨
	TCHAR szName[64]; //케릭터 아이디
	int nStartIndex; //시작 마을 숫자로 받아서 스트링으로 표시
	int nClassInfo; //직책 정보

	DiscipleInfo::DiscipleInfo()
	{
		nLevel = 0;
		memset( szName, 0, sizeof(szName));
		nStartIndex = 0;
		nClassInfo = 0;
	}
};