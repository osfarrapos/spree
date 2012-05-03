// TcpResource.h: interface for the TcpResource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPRESOURCE_H__0B0BBF46_502C_11D5_893F_5254AB171A1A__INCLUDED_)
#define AFX_TCPRESOURCE_H__0B0BBF46_502C_11D5_893F_5254AB171A1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include <afxmt.h>
#include "WW2Protocol.h"
class TcpResource  
{

public:
	TcpResource();
	virtual ~TcpResource();
public:



public:
	FindedRank							sFindedRank;

	int									roomSize;
	int									userSize;
	BOOL								bInit;
	BOOL								bLogin;
	BOOL								bNewid;
	BOOL								bChangePW;
	BOOL								bJoinChat;
	BOOL								bCreateChat;
	BOOL								bCreateGame;
	BOOL								bJoinGame;
	BOOL								bChatList;
	BOOL								bChatUsers;
	BOOL								bGameList;

	int				iInit;
	int				iLogin;
	int				iNewid;
	int				iChangePW;
	int				iJoinChat;
	int				iCreateChat;
	int				iCreateGame;
	int				iJoinGame;
	int				iChatList;
	int				iChatUsers;
	int				iGameList;

	

	CArray<Room,Room>					servRoom;						//채팅방정보를 셋팅할 배열(tagRoom구조체를 바탕으로)
	CArray<tUser,tUser>					roomUsers;						//chatting room 안의 user들의 list
	CArray<BRoom,BRoom>					listBattle;						//battle list
	CArray<BRoom,BRoom>					listBattlePage;
	CArray<TBRank,TBRank>				listRank;


	CCriticalSection					servRoomlock;

	CCriticalSection					roomUserslock;
	CCriticalSection					listBattlelock;
	CCriticalSection					listBattlePagelock;
	CCriticalSection					listRanklock;

	CArray<BRoom,BRoom>					listBattleType1;
	CArray<BRoom,BRoom>					listBattleType2;
	CArray<BRoom,BRoom>					listBattleType3;
	CArray<BRoom,BRoom>					listBattleType4;
	CArray<BRoom,BRoom>					listBattleType5;

	CCriticalSection					listBattleType1lock;
	CCriticalSection					listBattleType2lock;
	CCriticalSection					listBattleType3lock;
	CCriticalSection					listBattleType4lock;
	CCriticalSection					listBattleType5lock;
	
	CString LoginID;								//접속한 아이디를  셋팅할 변수
	CString curChannel;								//위에 LoginID랑 비슷한 가이사꾸다. 무슨함수로 셋팅하던간에 현재 있는 대화방의 이름이다.

	
};

#endif // !defined(AFX_TCPRESOURCE_H__0B0BBF46_502C_11D5_893F_5254AB171A1A__INCLUDED_)
