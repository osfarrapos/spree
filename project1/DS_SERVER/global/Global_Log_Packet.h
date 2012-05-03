#pragma once

#pragma pack( push, q98asfas )
#pragma pack(1)

#include "..\Global\Global_Log_Define.h"



// 연결이 끊겼을때 내부적으로만 쓰는 패킷
//#define MSG_NO_DISCONNECT					0
//struct MSG_DISCONNECT						//# 연결이 끊겼을 때마지막 패킷 #
//{
//	short				sLength : 12 ;		// -2048 ~ 2047
//	short				sCrypto : 2 ;		// 0 : false , 나머지는 true
//	short				sCompressed : 2 ;	//0 : false , 나머지는 true
//	
//	u_char				cMessage ;	
//	void*				pDSComm ;
//} ;


#define MSG_NO_SERVER_STATE					1
struct MSG_SERVER_STATE {
	u_short		sLength ;	
	u_char		cMessage ;
	
	_ServerState		log_ServerState ;
};

#define MSG_NO_ITEM						2
struct MSG_ITEM {
	u_short		sLength ;	
	u_char		cMessage ;

	_ItemLog	log_Item ;
};


#define MSG_NO_CHARACTER				3
struct MSG_CHARACTER {
	u_short		sLength ;	
	u_char		cMessage ;

	_CharacLog	log_Charac ;
};

#define MSG_NO_LOGINLOG				4
struct MSG_LOGINLOG {
	u_short		sLength ;	
	u_char		cMessage ;

	_LoginLog	log_Login ;
};

#pragma pack( pop, q98asfas )