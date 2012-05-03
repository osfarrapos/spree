// XNPCScript.h: interface for the _XNPCScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XNPCSCRIPT_H__F6F5B7CC_7E9F_4726_A470_254789548AE8__INCLUDED_)
#define AFX_XNPCSCRIPT_H__F6F5B7CC_7E9F_4726_A470_254789548AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "XItem.h"
#include "XList.h"
#include "XNPC.h"
#include "XProc_ProcessDefine.h"


class _XNPCScriptItem : public _XItem  
{
public:
	BOOL  enable;

	BYTE  scriptclass;
	int	  tempvalue1;
	BYTE  compareclass;						// 조건식에서 모두 만족, 하나 이상 만족 
	int	  tempvalue2;
	
	int	  condition1;
	int	  condition2;
	int	  condition3;	//2004.03.10->oneway48 insert : 아이템 리스트가 3개로 늘어남 
	int	  condition4;
	int	  condition5;
	int	  condition6;		 
	int	  condition7;
	int	  condition8;
	int	  condition9;		 
	int	  condition10;		 
	int	  condition11;
	int	  condition12;	//2004.11.17->oneway48 insert : 스킬 리스트가 12개로 늘어남
	int	  condition13;	//2005.01.07->oneway48 insert : 스킬 리스트가 13개로 늘어남

	int   quantitynumber1;
	int   quantitynumber2;
	int   quantitynumber3;
	int   quantitynumber4;
	int   quantitynumber5;
	LPSTR quantitystring;

	LPSTR messagestring;
	
	LPSTR		strMessage1[5];				// 대화시 일반일 때 대화 저장 , 질문&답변 시 질문 저장 
	LPSTR		strMessage2[5];				// 대화시 단골일 때 대화 저장 , 질문&답변 시 답변 저장 
	LPSTR		strMessage3[5];				// 대화시 영웅이 있을 때 대화 저장
	
	_XList		m_SubScriptConditionList;	// 조건 정보를 가지기 위한 리스트 (_XNPCScriptConditionItem를 저장할 때 쓰인다.)

public:
	_XNPCScriptItem();
	virtual ~_XNPCScriptItem();

	void SetMessageString( LPSTR _message );
	void SetQuantityString( LPSTR _message );

	
	_XNPCCONDITION_RESULT ProcessCondition();
	
	void SetstrMessage3(int index, LPSTR _message);
	void SetstrMessage2(int index, LPSTR _message);
	void SetstrMessage1(int index, LPSTR _message);	
};


class _XNPCMainScript : public _XItem
{

public:
	int			m_UniqueID;
	TCHAR		scriptname[32];
	
	BYTE		compareclass;						// and, or
	int			tempvalue3;
		
	LPSTR		messagestring1;
	LPSTR		messagestring2;
	LPSTR		messagestring3;
	LPSTR		messagestring4;
	LPSTR		messagestring5;

	int			soundindex1;
	int			soundindex2;
	int			soundindex3;
	int			soundindex4;
	int			soundindex5;
	
	LPSTR		strPrevGreetingMessage[5];			// 인사말 전 대사를 저장
	LPSTR		strGreetingGeneralMessage[5];		// 일반일 때 인사말 저장 
	LPSTR		strGreetingHostilityMessage[5];		// 적대일 때 인사말 저장
	LPSTR		strGreetingCustomerMessage[5];		// 단골일 때 인사말 저장 
	LPSTR		strGreetingHeroStayMessage[5];		// 영웅이 있을 때 인사말 저장
	
	//2004.07.05->oneway48 insert
	LPSTR		strGoodbyeGeneralMessage;	// 일반일 때 헤어질 때 대사 저장
	LPSTR		strGoodbyeHostilityMessage;	// 적대일 때 헤어질 때 대사 저장
	LPSTR		strGoodbyeCustomerMessage;	// 단골일 때 헤어질 때 대사 저장
	LPSTR		strGoodbyeHeroStayMessage;	// 영웅이 있을 때 헤어질 때 대사 저장
	int			indexGoodbyeSound[4];	// 0:일반 1:적대 2:단골 3:영웅
	//insert end

	int			indexPrevGreetingSound[5];		
	int			indexGreetingGeneralSound[5];
	int			indexGreetingHostilitySound[5];
	int			indexGreetingCustomerSound[5];
	int			indexGreetingHeroStaySound[5];
	
	_XList		m_SubScriptList;	
	int			m_EventType;	//이벤트 종류 0:무영검-패밀리마트 1:봉인유저 2:퀴즈맞추기
	
	// 처음 인사말을 할 때 일반,적대,단골,영웅 판별하기 위한 조건 리스트 
	_XList		m_NPCScriptConditionList;
	
public:
	_XNPCCONDITION_RESULT ProcessNPCCondition();
	_XNPCMainScript();
	virtual ~_XNPCMainScript();

	void SetIndexGreetingHeroStaySound(int index, int soundfileindex);
	void SetIndexGreetingCustomerSound(int index, int soundfileindex);
	void SetIndexGreetingHostilitySound(int index, int soundfileindex);
	void SetIndexGreetingGeneralSound(int index, int soundfileindex);
	void SetIndexPrevGreetingSound(int index, int soundfileindex);	
	void SetIndexGoodByeSound(int index, int soundfileindex);//2004.07.05->oneway48 insert	
	void SetGreetingHeroStayMessage(int index, LPSTR _message);
	void SetGreetingCustomerMessage(int index, LPSTR _message);
	void SetGreetingHostilityMessage(int index, LPSTR _message);
	void SetGreetingGeneralMessage(int index, LPSTR _message);
	void SetPrevGreetingMessage( int index, LPSTR _message );
	void SetGoodByeMessage(int index, LPSTR _message);//2004.07.05->oneway48 insert	

	void SetScriptName( LPSTR _scriptname );

	void SetMessageString( LPSTR _message, int index );
	void SetSoundIndex(int index, int soundfileindex);

	void insertSubScriptItem( _XNPCScriptItem* scriptitem );
	_XNPCScriptItem* getSubScriptItem( int index );

	BOOL LoadScript( FILE* FilePtr );	
};



#endif // !defined(AFX_XNPCSCRIPT_H__F6F5B7CC_7E9F_4726_A470_254789548AE8__INCLUDED_)
