//WW2Protocol.h
//서버와 클라이언트 사이의 약속 프로토콜 
 /*************************************************
  World War 2  Bettle Net Packet

  작성시작 일자 : 2001. 5. 18 (금)
  
	메세지 이름 규칙
	 cCMSG_XXXXX
	 c : class
	  C : Client (S : Server) (더미서버의 메세지이름에는 제외됨)
	   MSG : (기냥 메세지니까 ^^)
	      _XXXXX : 메세지의 이름
	------------------------------------------------------------------------

 -  더미서버를 위한 메세지 정의 부분
	(유저는 cMSG_DISDMS메세지가 오기 전까지 리스트를 받아 저장하고 cMSG_DISDMS메세지가 오면 종료한다.)
	cMSG_SEVLIST : 서버의 리스트중 하나를 보낸다. 클라이언트는  cMSG_DISDMS 가 올때까지 계속 recv한다. 
	cMSG_DISDMS  : 서버 리스트 전송이 끝남을 알려준다.
 -  배너 광고를 표시할 경우 
	처음 로그인 할때 한꺼번에 받느냐, 아님 각 페이지에 따라 받는냐..
	(초반에 한번에 받는것이 ^^)
  
 -	클라이언트 메세지 정의 부분 
	cCMSG_LOGIN			:  (0x1001)로그인 정보 날림
	cCMSG_NEWID			:  (0x1002)새로운 아뒤 신청
	cCMSG_CHGPW			:  (0x1003)패스워드를 바꾼다.
	cCMSG_CHAT			:  (0x1004)채팅 메세지(통보)
	cCMSG_CREATEFIELD   :  (0x1005)채널을 만든다.
	cCMSG_JOINFIELD     :  (0x1006)타 채널에 참가한다. 
	cCMSG_LISTFIELD     :  (0x1007)채널 리스트를 요청하다. 
	cCMSG_CREATEBATTLE  :  (0x1008)겜방을 만든다. 
	cCMSG_CREATERESULT  :  (0x1009)겜방을 성공적으로 만들었는지 확인해준다.. (통보)
	cCMSG_CANCELBATTLE  :  (0x1010)겜방주인이 방을 뽀겠다. (통보)
	cCMSG_JOINBATTLE    :  (0x1011)겜방에 조인한다.
	cCMSG_INFOBATTLE    :  (0x1012)대기중인 겜방의 정보를 수렴한다. (통보)
	cCMSG_LAUNCHBATTLE  :  (0x1013)게임을 실행한다. 
	cCMSG_EXITBATTLE    :  (0x1014)겜방에서 나온다. (통보)(대기실임.. 서버에서는 어느 채널로 들어갈지 정해서 보내준다.)
	cCMSG_ENDBATTLE     :  (0x1015)겜방에서 나온다. (통보)(겜중에 나온다. 결과를 여기에 포함)
	(서버에서는 그 겜방이 겜중인지, 겜대기 중인지 알고있다.)
	cCMSG_LISTBATTLE    :  (0x1016)겜방리스트를 요구한다.
	cCMSG_LISTRANK		:  (0x1018)랭킹을 요구한다.
	cCMSG_FINDRANK		:  (0x1019)랭킹리스트에서 찾는다.
	cCMSG_REDBANNER		:  (0x1020)배너를 받을 준비를 마치고 서버에게 요청 (cSMSG_TRANSFILE이 돌아온다.)	
	cCMSG_LOGOUT		:  (0x1021)서버를 나간다. (통보)
	cCMSG_JOINRESULT	:  (0x1022)겜에 참여한 결과를 돌려준다.(아마 거의 성공하겠지만.. 못할경우도 생길지도.. dp가 얼마나 무서운데. -_-;)
	cCMSG_INIT			:  (0x1023)서버로 오기전에 ...
	********* 기타기능 *****************************	
	cCMSG_FIND			:  (0x1051)다른 유저를 찾는다.
	cCMSG_SAY			:  (0x1052)상대방에게 귓말로 이야기한다.(통보 - 겜 하는 사람, 혹은 그 반대도 가능하게... ^^)
	cCMSG_KICK			:  (0x1053)겜방의 주인은 다른 사람을 강퇴할수 있다.(통보)
	cCMSG_PROFILE		:  (0x1054)상대방의 정보를 요청한다.
	cCMSG_INVITE		:  (0x1055)겜방으로 초청한다. 

  
 -	서버 메세지 정의 부분 
	cSMSG_LOGIN			:  (0x2001)로그인 결과 돌려줌
	cSMSG_NEWID			:  (0x2002)새로운 아뒤 신청 결과
	cSMSG_CHGPW			:  (0x2003)패스워드 바꾼 결과를 돌려준다.
	cSMSG_CHAT			:  (0x2004)채팅 메세지(통보)
	cSMSG_CREATEFIELD	:  (0x2005)채널 만든 결과를 돌려준다. 
	cSMSG_JOINFIELD		:  (0x2006)채널참가 결과를 돌려준다. (응답/통보)
	cSMSG_UPDATEFIELD	:  (0x2007)채널의 정보를 갱신한다. (통보)
	cSMSG_LISTFIELD		:  (0x2008)채널 리스트를 준다.
	cSMSG_CREATEBATTLE  :  (0x2009)겜방의 생성결과를 돌려준다.
	cSMSG_JOINBATTLE    :  (0x2010)겜방에 조인결과를 돌려준다. 
	cSMSG_LISTBATTLE    :  (0x2011)겜방리스트를 돌려준다.
	cSMSG_LISTRANK		:  (0x2012)랭킹을 돌려준다.
	cSMSG_FINDRANK		:  (0x2013)랭킹리스트에서 특정유저의 정보를 검색해서 돌려준다. 
	cSMSG_INFOFIELD		:  (0x2014)20명씩 잘라서 유저정보를 보내준다.
	cSMSG_INIT			:  (0x2015)초기 정보를 확인한다.
	********* 기타기능 *****************************
	cSMSG_FIND			:  (0x2051)찾은 유저의 결과를 돌려준다. 
	cSMSG_SAY			:  (0x2052)상대방에게 귓말내용을 전달한다. (통보)
	cSMSG_KICK			:  (0x2053)강퇴시킨다.(통보)
	cSMSG_PROFILE		:  (0x2054)요청한 상대의 정보를 보여준다. 
	cSMSG_DIS           :  (0x2055)관리자가 유저를 강퇴시킨다.(기간을 정할건가???)
	cSMSG_TRANSFILE		:  (0x2056)파일 전송한다. 
	cSMSG_INVITE		:  (0x2057)초청 메세지를 보내준다. 


	추가작업 : 2001. 05. 26 (금)
	내    용 : 음.. 기타기능을 제외한 나머지 패킷을 다 정의했다. 
			   드림으로 넘어가 봐야 알겠지만.. 일단은 완료!!
			   낼부터는 서버의 관리 코딩에 푹 빠지겠군. --;

	추가작업 : 2001. 05. 29 (화)
	내    용 : 아래의 내용이 추가, 변경되었다.(cSMSG_LOGIN중에서...)	
		#define dLOGIN_NOTLOGIN		0x0002	//	ID 혹은 PW가 일치하지 않는다. 
		#define dLOGIN_FAIL			0x0008	//	하여간 로그인 불가능 ^^;;

	추가작업 : 2001. 05. 30 (수)
	내    용 : 
		#define dDREATEFIELD_USEDNAME	0x0002	//	방이름이 존재한다.
		//	자신의 계급에 대한 내용.. updatefield,cSMSG_LISTRANK,cSMSG_FINDRANK에 포함된내용임..
		BYTE	imclass;	//	계급을 넘긴다. 
		dSMSG_INFOFIELD	추가..

	추가작업 : 2001. 05. 31 (목)
	내    용 : cCMSG_CREATEBATTLE내용부분이 수정되었다.
			   dLOGIN_FULL  추가됨.

	추가작업 : 2001. 06. 01 (금)
	내    용 : 필요없는 시리얼 정보 삭제.
			   cSMSG_LOGIN / cSMSG_CREATEFIELD / cSMSG_JOINFIELD / cSMSG_CREATEBATTLE / cSMSG_JOINBATTLE
			   cSMSG_CHAT에 char id[dMAXUSERIDSIZE]; 추가됨.//
			   cCMSG_INIT, cSMSG_INIT 추가 login시 확인하는 정보를 몇가지 옴겼다.

	추가작업 : 2001. 06. 02 (토)
	내    용 : 채팅메세지의  fieldserial을 삭제했다. 
				joinfield에 fieldname 추가 
				cCMSG_RESULTBATTLE 삭제 
				cCMSG_ENDBATTLE에 last추가 

	추가작업 : 2001. 06. 04 (월)
	내    용 : 아래의 내용이 추가,변경되었다.
				#define	dJOINFIELD_DONTENER	0x0005	//	들어갈수 있는 상태가 아니다.	
				#define dJOINFIELD_NOMORE	0X0006	//
				
2001.07.24 화요일
 수정: 대기중 게임방파괴는 호스트나 이외에나 모두 dEXITBATTLE로 통일 
 *************************************************/

#ifndef _WW2PROTOCOL_H
#define _WW2PROTOCOL_H
#pragma pack(1) 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//CLASS 정의
#define MODULE_TCPINIT				10
#define MODULE_TCPSERVICE			20
#define MODULE_TCPTHREAD			30

#define SendQueueSize				(8192)
#define WM_DIALOG					WM_USER+1
#define WM_CHATLIST					WM_USER+2
#define	WM_JOINFIELD				WM_USER+3
#define	WM_CREATEFIELD				WM_USER+4
#define WM_NEWID					WM_USER+5
#define WM_CHANGEPW					WM_USER+6
#define WM_CREATEBATTLE				WM_USER+7
#define WM_USERCHATUSERS			WM_USER+8
#define WM_BATTLELIST				WM_USER+9
#define WM_BATTLELISTPAGE			WM_USER+10
#define WM_INFO2BATTLE				WM_USER+11
#define WM_FINDRANK					WM_USER+12
#define WM_UPDATEFIELD				WM_USER+13
#define WM_LOGIN					WM_USER+14
#define WM_CONNECTINIT				WM_USER+15
#define WM_JOINBATTLE				WM_USER+16
#define WM_LISTRANK					WM_USER+17
#define WM_BANNER_RECEVE			WM_USER+18
#define WM_SAY						WM_USER+19
#define	WM_ERROR_NANGGEK			WM_USER+20
#define WM_FILESIZE                 WM_USER+21
#define WM_PROGRESS_SIZE            WM_USER+22

//#define COMMANDLENGTH (9)
typedef unsigned char   BYTE;
typedef unsigned short	WORD;
typedef unsigned long	DWORD;

#define dMAXCDKEYSIZE				17

#define	dMAXUSERIDSIZE				17
#define	dMAXUSERPWSIZE				17

#define	dMAXFIELDNAMESIZE			21

#define	dMAXBATTLENAMESIZE			21
#define	dMAXBATTLEPWSIZE			17
#define	dMAXBATTLEMAPNAME			17

#define dMAXCHATSIZE				128
/***********************************************************************
 *없어서 임시방편으로 정함 
 *죄다 17로 놓음 5.26 토욜
 ***********************************************************************/
//#define dMAXFIELDLIST				30						//5.28 오후 수정
#define dMAXFIELD					30						//5.28	30에 
//#define dMAXRANKLIST				20						//5.28 정식 내용.
//#define dMAXUSERLIST				20		//	필드들어갈때 한번에 보내는 유저수.., 5.30
//#define dMAXBATTLELIST				30		//	한번에 보낼수 있는 최대 겜방 리스트 수.....6.12
/**********************************************************************/
#define dIN_OUT				0		//	이눔은 나간다......6.12
#define dIN_IN				1		//	이눔은 들어온다.......6.12

//-------------------------------------------------------------------------------------6.15	첨가 
#define dIN					0
#define dOUT				1


#define dDBBACKUP_CYCLE		7	//	기준 : 일
#define dDELETEID_CYCLE		3	//	기준 : 달
#define dLIVECODE_CYCLE		4	//	기준 : 분

#define dMAXTIMES			6	//	시간을 기록하는 자리수

#define dMAXCDKEYSIZE		17


#define	dMAXFIELDLIST_S		12		//	원래 한화면에 뿌려지는 갯수
#define	dMAXBATTLELIST_S	12		//	원래 한화면에 뿌려지는 갯수
//#define dMAXFIELDLIST		dMAXFIELDLIST_S*2		//	한번에 보낼수 있는 최대 채널 리스트 수
#define dMAXFIELDLIST		14		//	한번에 보낼수 있는 최대 채널 리스트 수 20에서 14로 7.3

//#define dMAXBATTLELIST		dMAXBATTLELIST_S*2		//	한번에 보낼수 있는 최대 겜방 리스트 수
#define dMAXBATTLELIST		14		//	한번에 보낼수 있는 최대 겜방 리스트 수


#define dMAXRANKLIST_S		20		//	한번에 보낼수 있는 최대 랭킹 리스트 수
#define dMAXUSERLIST_S		20		//	필드들어갈때 한번에 보내는 유저수..
#define dMAXRANKLIST		20		//	한번에 보낼수 있는 최대 랭킹 리스트 수 ,6.26  요청에 의해 바꿈
#define dMAXUSERLIST		dMAXUSERLIST_S+10		//	필드들어갈때 한번에 보내는 유저수..

#define dMAXBANNER_SIZE		1024*4				//7.4추가 배너 최대 Size
//----------------------------------------------------------------------------------------------
//	 계급에 대하여...
#define dCLASS_1			1	//	이병
#define dCLASS_2			2	//	일병
#define dCLASS_3			3	//	상병
#define dCLASS_4			4	//	병장
#define dCLASS_11			11	//	하사
#define dCLASS_12			12	//	중사
#define dCLASS_13			13	//	상사
#define dCLASS_21			21	//	소위
#define dCLASS_22			22	//	중위
#define dCLASS_23			23	//	대위
#define dCLASS_31			31	//	소령
#define dCLASS_32			32	//	중령
#define dCLASS_33			33	//	대령
#define dCLASS_43			43	//	준장
#define dCLASS_44			44	//	소장
#define dCLASS_45			45	//	중장
#define dCLASS_46			46	//	대장
#define dCLASS_47			47	//	원수




#define MAXDATA											(8192)



typedef struct cMSG_BASE{
	DWORD				size;			//패킷의 길이 저장
	WORD				type;			//패킷의 내용을 정의 
}cmsg_base;

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 더미서버내용 >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define dCMSG_SRVLIST									0x0001

typedef struct cMSG_SERVLIST{
	cmsg_base			base;
	char				name[17];
	char				ip[17];
	WORD				port;	//	hyun - add  (06. 09)
}cmsg_servlist;
/*-----------------------------------------------------------
	 패킷 종류  : 요청 
	 설     명  : 서버와의 초기 대화
	 응답 패킷  : cSMSG_INIT
	 기     타  :																		//6.4일 추가
-----------------------------------------------------------*/
//#define dCMSG_INIT		0x1023
#define dCMSG_INIT		0x00A1								//8.22일 더미로 이전함
typedef struct cCMSG_INIT 
{
	cmsg_base			base;
	WORD				version;
	BYTE				banner;			//	Banner의 Version
	
}ccmsg_init;

/*-----------------------------------------------------------
	 패킷 종류  : 응답
	 설     명  : 서버와의 초기 대화
	 응답 패킷  : 
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_INIT		0x00B1
	#define dINIT_SUCCESS		0
	#define dINIT_NOTVERSION	1
	#define dINIT_NOTBANNER		2
	#define dINIT_BOTH			3

typedef struct cSMSG_INIT 
{
	cmsg_base			base;
	BYTE				result;

}csmsg_init;

#define dCMSG_REDBANNER		0x00A2

typedef struct cCMSG_REDBANNER 
{
	cmsg_base		base;
}ccmsg_redbanner;

#define	dSMSG_INFOBANNER		0x00B2

typedef	struct cSMSG_INFOBANNER
{
	cmsg_base			base;
	DWORD				FileSize;				//	화일의 크기
	BYTE				BannerVersion;			//	배너 버젼
	WORD				tot;

}csmsg_infobanner;

#define dSMSG_TRANSBANNER		0x00B3

typedef struct cSMSG_TRANSBANNER
{
	cmsg_base			base;
	WORD	cur;						//	총 나눠진 수 / 현재 번호 
	WORD	f_size;						//	data의 사이즈가 있다.
	BYTE	data[dMAXBANNER_SIZE];		//	4kb씩 담아서 보낸다.

}csmsg_transbanner;


#define dCMSG_REDPATCH		0x00A3

typedef struct cCMSG_REDPATCH
{
	cmsg_base		base;
}ccmsg_redpatch;

#define	dSMSG_INFOPATCH		0x00B4

typedef	struct cSMSG_INFOPATCH
{
	cmsg_base				base;
	DWORD					FileSize;				//	화일의 크기
	BYTE					PatchVersion;			//	패치 버젼
	WORD					tot;
}csmsg_infopatch;

#define dSMSG_TRANSPATCH		0x00B5

typedef struct cSMSG_TRANSPATCH
{
	cmsg_base				base;
	WORD					cur;						//	총 나눠진 수 / 현재 번호 
	WORD					f_size;						//	data의 사이즈가 있다.
	BYTE					data[dMAXBANNER_SIZE];		//	.단 1byte만 남으면 어쩌지 ㅡㅡ;

}csmsg_transpatch;

#define dCMSG_DISDMS									0x0002			//더미서버와의 접속이 끊어짐을 서버가 알려줌 
//------------------------------------------------------------------------------7.31
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<더미서버내용 끝<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

typedef struct FIELDINFO{
	char	name[dMAXFIELDNAMESIZE];	//	채널의 이름
	char	hostid[dMAXUSERIDSIZE];		//	채널 주인 

	BYTE	usercount;				//	채널에 현재 참가중인 인원
	DWORD	times;						//	생성된 시간
}sfieldinfo;


typedef struct BATTLEINFO{
	char	name[dMAXBATTLENAMESIZE];	//	배틀의 이름
	BYTE	maxcount;					//	배틀 최대 인원
	BYTE	usercount;					//	배틀에 현재 참가중인 인원(깡통 포함)
	DWORD	times;						//	생성된 시간


	char	mapname[dMAXBATTLEMAPNAME];
	WORD	wsize,hsize;//	타일 사이즈
	BYTE	terrain;					//	지형
	BYTE	secret;				//	공개(0) / 비공개(1) 

}sbattleinfo;

//(((((((((((((((((((((((((((((((((((((((((((((본서버내용 시작))))))))))))))))))))))))))))))))))))))))))))))))))))))

/*-----------------------------------------------------------
	 패킷 종류  : 요청 
	 설     명  : 로그인을 요청한다. 
	 응답 패킷  : cSMSG_LOGIN
	 기     타  :													//6.4 버전 베너 빠짐.
-----------------------------------------------------------*/

typedef struct cMSG_LOGIN
{
	cmsg_base			base;
	char				id[dMAXUSERIDSIZE];
	char				pw[dMAXUSERPWSIZE];
	char				cdkey[dMAXCDKEYSIZE];		//	CDKEY의 길이도 물어왔어야 했는데 --;
	DWORD				index;						//	CDKEY의 인덱스 (있을지는 모르지...^^)
}cmsg_login;

/*-----------------------------------------------------------
	 패킷 종류  : 요청 
	 설     명  : 새로운 계정을 만든다.
	 응답 패킷  : cSMSG_NEWID
	 기     타  : 
-----------------------------------------------------------*/
#define dCMSG_NEWID										0x1002
typedef struct cCMSG_NEWID
{
	cmsg_base			base;
	char				id[dMAXUSERIDSIZE];
	char				pw[dMAXUSERPWSIZE];
	char				cdkey[dMAXCDKEYSIZE];
	DWORD				index;
}ccmsg_newid;

/*-----------------------------------------------------------
	 패킷 종류  : 요청 
	 설     명  : 패스워드를 바꾼다.
	 응답 패킷  : cSMSG_CHGPW
	 기     타  : 
-----------------------------------------------------------*/
#define dCMSG_CHGPW		0x1003

typedef struct cCMSG_CHGPW 
{
	cmsg_base			base;
	char				id[dMAXUSERIDSIZE];			//6.20 추가

	char				oldpw[dMAXUSERPWSIZE];
	char				newpw[dMAXUSERPWSIZE];
	char				cdkey[dMAXCDKEYSIZE];
	DWORD				index;
	
}ccmsg_chgpw;

/*-----------------------------------------------------------
	 패킷 종류  : 통보
	 설     명  : 메세지를 전달한다.
	 응답 패킷  : cSMSG_CHAT
	 기     타  : 
-----------------------------------------------------------*/
#define dCMSG_CHAT										0x1004
typedef struct cCMSG_CHAT
{
	cmsg_base			base;
	char	msg[dMAXCHATSIZE];
}ccmsg_chat;

/*-----------------------------------------------------------
	 패킷 종류  : 요청
	 설     명  : 채널을 만든다.
	 응답 패킷  : cSMSG_CREATEFIELD
	 기     타  :	*******************5.26.토욜
-----------------------------------------------------------*/
#define	dCMSG_CREATEFIELD   0x1005
typedef struct cCMSG_CREATEFIELD
{
	cmsg_base			base;
	char				name[dMAXFIELDNAMESIZE];
}ccmsg_createfield;
/*-----------------------------------------------------------
	 패킷 종류  : 요청
	 설     명  : 타 채널에 참가한다. 
	 응답 패킷  : cSMSG_JOINFIELD
	 기     타  :   *******************5.26.토욜
-----------------------------------------------------------*/
#define dCMSG_JOINFIELD		0x1006
typedef struct cCMSG_JOINFIELD
{
	cmsg_base			base;
	char name[dMAXFIELDNAMESIZE];
}ccmsg_joinfield;
/*-----------------------------------------------------------
	 패킷 종류  : 요청
	 설     명  : 채널 리스트를 요청하다. 
	 응답 패킷  : cSMSG_LISTFIELD
	 기     타  :   *********************5.26.토욜
-----------------------------------------------------------*/
#define dCMSG_LISTFIELD		0x1007
	#define dLISTFIELD_UPDATE		0	//	새롭게 갱신된 내용을 요청한다.
	#define dLISTFIELD_DEL			1	//	삭제될 데이타를 요구한다.
	#define dLISTFIELD_ADD			2	//	새롭게 추가된 내용을 요구한다.
	#define dLISTFIELD_ONE			3	//	요청한 페이지가 넘어간다.


	#define dLISTFIELD_ALL			4	//	모든 내용을 새롭게 받는다.


	//	이 경우 클라이언트는 받아서 필터링 과정을 거쳐야 한다. 
	//	이미 넘어온 필드의 정보가 페이지를 분리하는 과정에서 중복 포함될 가능성이 있다.  
typedef struct cCMSG_LISTFIELD
{
	cmsg_base			base;
//	BYTE				sep;									//6.17 sunday추가	731삭제
//	BYTE				page;									//6.15 추가. 1 base의 페이지 요청번호 731 delete

}ccmsg_listfield;

/*-----------------------------------------------------------
	 패킷 종류  : 요청
	 설     명  : 겜방을 만든다. 
	 응답 패킷  : cSMSG_CREATEBATTLE
	 기     타  :   *********************5.26.토욜
-----------------------------------------------------------*/
#define dCMSG_CREATEBATTLE	0x1008

	#define dTERRAIN_GRASSLAND	0x0000	//	초원지형
	#define	dTERRAIN_DESERT		0x0001	//	사막지형

typedef struct cCMSG_CREATEBATTLE
{
	cmsg_base			base;
	//	겜방의 주인은 이거 보낸놈이니 추가하지 말고..
	char				name[dMAXBATTLENAMESIZE];
	char				pw[dMAXBATTLEPWSIZE];
	char				mapname[dMAXBATTLEMAPNAME];
	BYTE				type;									//	게임 유형
	WORD				wsize,hsize;							//	타일 사이즈
	BYTE				terrain;								//	지형
	BYTE				secret;	//	hyun - add  (06. 09)		//	공개(0) / 비공개(1) 
	DWORD				mapdate;
//	WORD	year;												//	맵화일의 작성 년
//	BYTE	month,day,hour,minute,second;						//	맵화일의 작성 월,일,시,분
	BYTE	maxplayer;											//	최대 수용 인원 6.19 추가 ㅡㅡ

	char				address[17];
	WORD				port;
}ccmsg_createbattle;
/*-----------------------------------------------------------
	 패킷 종류  : 통보
	 설     명  : 겜방을 성공적으로 만들었는지 확인해준다.
	 응답 패킷  : 
	 기     타  : 
-----------------------------------------------------------*/
#define	dCMSG_CREATERESULT		0x1009

	#define dCREATEBATTLE_MAKEIT	0
	#define dCREATEBATTLE_NOTMAKE	1

typedef struct cCMSG_CREATERESULT
{
	cmsg_base			base;
	BYTE				result;


}ccmsg_createresult;

/*-----------------------------------------------------------
	 패킷 종류  : 통보
	 설     명  : 겜방주인이 방을 뽀갠다.
	 응답 패킷  : 
	 기     타  : 
-----------------------------------------------------------*/
#define dCMSG_CANCELBATTLE		0x1010
	
typedef struct cCMSG_CANCELBATTLE
{
	cmsg_base			base;

	//	자신이 주인일 경우만 넘기니까 검색해서 죽여 +_+ !!
	
}ccmsg_cancelbattle;
/*-----------------------------------------------------------
	 패킷 종류  : 요청
	 설     명  : 겜방에 조인한다.
	 응답 패킷  : cSMSG_JOINBATTLE
	 기     타  : 
-----------------------------------------------------------*/
#define dCMSG_JOINBATTLE	0x1011

typedef struct cCMSG_JOINBATTLE
{
	cmsg_base			base;
	char				name[dMAXBATTLENAMESIZE];
	char				pw[dMAXBATTLEPWSIZE];

}ccmsg_joinbattle;
/*-----------------------------------------------------------
	 패킷 종류  : 통보
	 설     명  : 대기중인 겜방의 정보를 수렴한다. 호스트만 보낸다.
	 응답 패킷  : 
	 기     타  : 
-----------------------------------------------------------*/
#define dCMSG_INFOBATTLE		0x1012

typedef struct cCMSG_INFOBATTLE
{
	cmsg_base			base;
	BYTE				maxplayer;
	BYTE				currentplayer;	//	컴터도 같이 포함해서 넘긴다.
							//	실 유저의 수는 서버에서 다시 관리한다. 

}ccmsg_infobattle;
/*-----------------------------------------------------------
	 패킷 종류  : 통보
	 설     명  : 게임을 실행한다. 
	 응답 패킷  : 
	 기     타  : 
-----------------------------------------------------------*/
#define dCMSG_LAUNCHBATTLE		0x1013

typedef struct cCMSG_LAUNCHBATTLE 
{
	cmsg_base			base;
	//	겜방의 주인만이 이 메세지를 넘긴다. 
	
}ccmsg_launchbattle;
/*-----------------------------------------------------------
	 패킷 종류  : 통보
	 설     명  : 겜방에서 나온다. 
					(대기실임.. 서버에서는 어느 채널로 들어갈지 정해서 보내준다.)
					(만약 방주인이 깬경우에도 이 메세지를 넘긴다.)
	 응답 패킷  : 
	 기     타  : 
-----------------------------------------------------------*/
#define dCMSG_EXITBATTLE		0x1014

typedef struct cCMSG_EXITBATTLE
{
	cmsg_base				base;
	//	누구든 겜 대기방에서 나오면 이 메세지를 보내다. 
	
}ccmsg_exitbattle;
/*-----------------------------------------------------------
	 패킷 종류  : 통보
	 설     명  : 겜방에서 나온다. (겜중에 나온다. 결과는 resultbattle에 포함)
					(서버에서는 그 겜방이 겜중인지, 겜대기 중인지 알고있다.)
	 응답 패킷  : 
	 기     타  : 

class cCMSG_ENDBATTLE : public cMSG_BASE{
public:
	BYTE	last;	//	0 - 먼저 나온사람 , 1 - 마직막에 결과값을 가진사람.

	RESULTBATTLE		rb[8];

	cCMSG_ENDBATTLE()	{type=dCMSG_ENDBATTLE;}
};


-----------------------------------------------------------*/
#define dCMSG_ENDBATTLE		0x1015
typedef struct	RESULTBATTLE{
	char	id[dMAXUSERIDSIZE];
	WORD	result;		//	0	패배 / 1	승리
}sresultbattle;

typedef struct cCMSG_ENDBATTLE
{
	cmsg_base				base;
	BYTE					last;	//	0 - 먼저 나온사람 , 1 - 마직막에 결과값을 가진사람.
	BYTE					count;	//유저수
	sresultbattle			sresult[8];	//	여기에 겜 결과를 받는다.
}ccmsg_endbattle;
/*-----------------------------------------------------------
	 패킷 종류  : 요청
	 설     명  : 겜방리스트를 요구한다.
	 응답 패킷  : dSMSG_LISTBATTLE
	 기     타  : 
-----------------------------------------------------------*/
#define dCMSG_LISTBATTLE		0x1016



typedef struct cCMSG_LISTBATTLE
{
	cmsg_base				base;
	BYTE					sep;
	BYTE					page;
	BYTE						type;


}ccmsg_listbattle;
/*----------------------------------------------------------

	 패킷 종류  : 통보
	 설     명  : 겜의 결과를 돌려준다.
	 응답 패킷  :
	 기     타  : 
	 #define dCMSG_RESULTBATTLE		0x1017
	 2001.6.4 일 패킷변환에 따른 삭제.........
-----------------------------------------------------------*/


/*-----------------------------------------------------------
	 패킷 종류  : 요청
	 설     명  : 랭킹을 요구한다.
	 응답 패킷  : cSMSG_LISTRANK
	 기     타  : 
-----------------------------------------------------------*/
#define dCMSG_LISTRANK		0x1018

typedef struct cCMSG_LISTRANK
{
	cmsg_base			base;
	BYTE				page;//6.26 추가

}ccmsg_listrank;
/*-----------------------------------------------------------
	 패킷 종류  : 요청
	 설     명  : 랭킹리스트에서 찾는다.
	 응답 패킷  : cSMSG_FINDRANK
	 기     타  : 
-----------------------------------------------------------*/
#define dCMSG_FINDRANK		0x1019

typedef struct cCMSG_FINDRANK
{
	cmsg_base			base;
	char	id[dMAXUSERIDSIZE];

}ccmsg_findrank;

/*-----------------------------------------------------------
	 패킷 종류  : 통보
	 설     명  : 서버를 나간다.
	 응답 패킷  :
	 기     타  : 
-----------------------------------------------------------*/
#define dCMSG_LOGOUT	0x1021
typedef struct cCMSG_LOGOUT
{
	cmsg_base			base;
}ccmsg_logout;
/*-----------------------------------------------------------
	 패킷 종류  : 통보
	 설     명  : 겜에 참여한 결과를 돌려준다.
				(아마 거의 성공하겠지만.. 못할경우도 생길지도.. dp가 얼마나 무서운데. -_-;)
	 응답 패킷  :
	 기     타  : 
-----------------------------------------------------------*/
#define dCMSG_JOINRESULT	0x1022

#define dJOINRESULT_SUCCESS		0x0000
#define dJOINRESULT_FAIL		0x0001

typedef struct cCMSG_JOINRESULT
{
	cmsg_base			base;
	BYTE				result;
}ccmsg_joinresult;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//	Server Message  /////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

/*-----------------------------------------------------------
	 패킷 종류  : 응답
	 설     명  : 로그인의 결과를 돌려준다.
	 응답 패킷  :
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_LOGIN		0x2001


//	지현 - 기냥 제꺼 다 같다 붙였습니다. 저랑 틀리더군요.
	#define dLOGIN_SUCCESS		0x0000	//	정상적으로 로그인
	#define dLOGIN_NOTLOGIN		0x0002	//	ID 혹은 PW가 일치하지 않는다. 
	#define dLOGIN_LOGINED		0x0003	//	누군가 같은 ID로 로그인 되어있는 상태이다.
	#define dLOGIN_USECDKEY		0x0004	//	CDKEY가 사용중이다. 
	#define dLOGIN_NOTCDKEY		0x0005	//	CDKEY가 일치하지 않는다. 
	#define dLOGIN_CLOSECDKEY	0x0006	//	이 CDKEY는 관리자에 의해 막혀있다.
	#define dLOGIN_FULL			0x0008	//	들어갈 채널이 없다.
	#define dLOGIN_FAIL			0x0009	//	하여간 로그인 불가능 ^^;;

typedef struct cSMSG_LOGIN
{
	cmsg_base			base;
	BYTE				result;
	char	fieldname[dMAXFIELDNAMESIZE];		//	들어간 채널의 이름. 영어로 32자
//	WORD	fieldserial;			//	채널 시리얼

/*	
	char	fieldname[dMAXFIELDNAMESIZE];		//	들어간 채널의 이름. 영어로 32자
	WORD	fieldserial;			//	채널 시리얼
	WORD	usercount;				//	총 접속 인원,
	WORD	battlecount,fieldcount;	//	게임 수,채널 수

	첨에 어떤 내용이 나올지는 생각해 봐야한다. 
	뭐, 사실 이런 내용은 필요없겠지만, 참가된 채널에 대한 정보만 넘겨줘두 상관없는데...
*/
}csmsg_login;

/*-----------------------------------------------------------
	 패킷 종류  : 응답 
	 설     명  : 새로운 계정을 만든 결과를 돌려준다.
	 응답 패킷  : cSMSG_NEWID
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_NEWID		0x2002

#define dNEWID_SUCCESS		0x0000	//	새로운 아뒤 등록 성공
#define dNEWID_USEID		0x0001	//	존재하는 아뒤
#define dNEWID_USECDKEY		0x0002	//	CDKEY가 사용중이다. 
#define dNEWID_NOTCDKEY		0x0003	//	CDKEY가 일치하지 않는다. 
#define dNEWID_CLOSECDKEY	0x0004	//	이 CDKEY는 관리자에 의해 막혀있다.
#define dNEWID_FAIL			0x0005	//	서버의 사정으로 생성 불가.(DB가 맛이 간 경우 등등)
	
typedef struct cSMSG_MEWID
{
	cmsg_base		base;
	BYTE			result;

}csmsg_newid;

/*-----------------------------------------------------------
	 패킷 종류  : 응답
	 설     명  : 패스워드 바꾼 결과를 돌려준다.
	 응답 패킷  : 
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_CHGPW		0x2003

	#define dCHGPW_SUCCESS		0x0000	//	패스워드를 정상적으로 교체
	#define dCHGPW_USEDID		0x0001	//	아이디가 사용중이다.
	#define dCHGPW_USECDKEY		0x0002	//	cdkey가 사용중이다.
	#define dCHGPW_NOTCDKEY		0x0003	//	cdkey가 일치하지 않는다.
	#define	dCHGPW_CLOSECDKEY	0x0004	//	cdkey가 막혀있다.
	#define dCHGPW_NOTPW		0x0005	//	기존의 패스워드와 일치하지 않는다.
	#define dCHGPW_FAIL			0x0006	//	서버의 사정으로 변경 불가.

typedef struct cSMSG_CHGPW 
{
	cmsg_base			base;
	BYTE				result;

}csmsg_chgpw;


/*----------------------
-------------------------------------
	 패킷 종류  : 이런 이거 채팅 메시지구만 
	 설     명  : 
	 응답 패킷  : 
	 기     타  : 
-----------------------------------------------------------*/

#define dSMSG_CHAT		0x2004
typedef struct cSMSG_CHAT
{
	cmsg_base			base;
	char				msg[dMAXCHATSIZE];
}csmsg_chat;

/*-----------------------------------------------------------
	 패킷 종류  : 응답
	 설     명  : 채널 만든 결과를 돌려준다.
	 응답 패킷  :
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_CREATEFIELD		0x2005

	#define dCREATEFIELD_SUCCESS	0x0000
/*	#define dCREATEFIELD_FULL		0x0001	//	최대 생성 수를 넘어서서 더이상 생성할 수 없다.
	#define dCREATEFIELD_FAIL		0x0002	//	서버의 사정으로 생성될 수 없다. (이런 경우가 있을까 --;)
*/	//	하나가 빠졌군요.
	#define dCREATEFIELD_FULL		0x0001	//	최대 생성 수를 넘어서서 더이상 생성할 수 없다.
	#define dCREATEFIELD_USEDNAME	0x0002	//	방이름이 존재한다.
	#define dCREATEFIELD_FAIL		0x0003	//	서버의 사정으로 생성될 수 없다. (이런 경우가 있을까 --;)

typedef struct cSMSG_CREATEFIELD
{
	cmsg_base			base;
	BYTE				result;
	char				name[dMAXFIELDNAMESIZE];


}csmsg_createfield;

/*-----------------------------------------------------------
	 패킷 종류  : 응답/통보
	 설     명  : 채널참가 결과를 돌려준다. 
	 응답 패킷  :
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_JOINFIELD		0x2006

	#define	dJOINFIELD_SUCCESS	0x0000
	#define	dJOINFIELD_FULL		0x0001	//	인원초과로 들어갈 수 없다.
	#define	dJOINFIELD_NOTPW		0x0002	//	패스워드가 틀렸다.
	#define	dJOINFIELD_NOTNAME	0x0003	//	참가할 방이름이 없다.
	#define	dJOINFIELD_FAIL		0x0004	//	알쥐?! -_-+
	#define	dJOINFIELD_DONTENER	0x0005	//	들어갈수 있는 상태가 아니다.	
	#define dJOINFIELD_NOMORE	0X0006	//	


typedef struct cSMSG_JOINFIELD
{
	cmsg_base					base;
	BYTE						result;
//	WORD						serial;		이놈은 석윤님의 충고로 빼기로 한 부분인데...
	char	fieldname[dMAXFIELDNAMESIZE];	//	클라이언트의 요청없이 이 메세지가 가기도 하기 때문
}csmsg_joinfield;
/*-----------------------------------------------------------
	 패킷 종류  : 통보
	 설     명  : 채널의 정보를 갱신한다.
	 응답 패킷  :
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_UPDATEFIELD		0x2007

typedef struct cSMSG_UPDATEFIELD
{
	cmsg_base				base;
	char					id[dMAXUSERIDSIZE];
	BYTE					imclass;	//	계급을 넘긴다.5.31 추가
	WORD					win,lost,draw;					//전적 8.2 추가 
	BYTE					in;			//	1 들어옴,0 나감
	//	기타 전적에 대한 내용도 들어가야할까?
}csmsg_updatefield;
/*-----------------------------------------------------------
	 패킷 종류  : 응답
	 설     명  : 채널 리스트를 준다.
	 응답 패킷  :
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_LISTFIELD		0x2008
	//BYTE  cur 값으로
	
typedef struct cSMSG_LISTFIELD
{
	cmsg_base				base;
	
	BYTE					count;	//	총 페이지수, 자신의 페이지 번호, 자신의 리스트수
	sfieldinfo				sField[dMAXFIELDLIST];
}csmsg_listfield;
/*-----------------------------------------------------------
	 패킷 종류  : 응답
	 설     명  : 겜방의 생성결과를 돌려준다.
	 응답 패킷  :
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_CREATEBATTLE		0x2009

	#define cCREATEBATTE_SUCCESS	0x0000
	#define cCREATEBATTE_USEDNAME	0x0001	//	같은 이름의 겜방이 존재한다.
	#define cCREATEBATTE_FULL		0x0002	//	더이상 겜방을 만들수 없다.
	#define cCREATEBATTE_FAIL		0x0003	//	실패 (글쎄 이런경우 없다니까 -_-;)

typedef struct cSMSG_CREATEBATTLE 
{
	cmsg_base				base;
	BYTE					result;
	char	name[dMAXBATTLENAMESIZE];	//6.23 추가

//	WORD					serial;			//	서버가 준 시리얼번호
//	시리얼이 다 삭제된건 석윤님이 이야기하셔서 고친건데...	
}csmsg_createbattle;
/*-----------------------------------------------------------
	 패킷 종류  : 응답
	 설     명  : 겜방에 조인결과를 돌려준다.
	 응답 패킷  :
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_JOINBATTLE		0x2010

	#define dJOINBATTLE_SUCCESS		0x0000
	#define dJOINBATTLE_FULL		0x0001		//	인원이 넘 많아서 안됨.
	#define dJOINBATTLE_NOTNAME		0x0002		//	이름이 틀리다.
	#define dJOINBATTLE_NOTPW		0x0003		//	패스워드가 틀리다.
	#define dJOINBATTLE_RUN			0x0004		//	겜중인 방이다. 어딜 -_-+
	#define dJOINBATTLE_FAIL		0x0005		//	과연 이메세질 보낼때가 있을까 --

typedef struct cSMSG_JOINBATTLE
{
	cmsg_base				base;
	BYTE					result;
//	WORD					serial;	마찬가지로 다 삭제됨.
	char					address[17];
	WORD					port;
	DWORD					mapdate;


}csmsg_joinbattle;
/*-----------------------------------------------------------
	 패킷 종류  : 응답
	 설     명  : 겜방리스트를 돌려준다.
	 응답 패킷  :
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_LISTBATTLE	0x2011
	#define dLISTBATTLE_UPDATE		0		//	새롭게 갱신된 내용을 요청한다.
//	#define dLISTBATTLE_DEL			1		//	삭제될 데이타를 요구한다.
//	#define	dLISTBATTLE_ADD			2		//	최근 추가된 내용을 받는다.
	#define dLISTBATTLE_ONE			3		//	요청한 페이지만 넘어간다.

	#define dLISTBATTLE_ALL			4		//	모든 내용을 새롭게 받는다.

typedef struct cSMSG_LISTBATTLE 
{
	cmsg_base				base;
	BYTE	sep;
	BYTE	tot,cur,count;	//	총 페이지수, 자신의 페이지 번호, 자신의 리스트수
	BYTE	type;
 
	sbattleinfo				sbattle[dMAXBATTLELIST];

}csmsg_listbattle;
/*-----------------------------------------------------------
	 패킷 종류  : 응답
	 설     명  : 랭킹을 돌려준다.
	 응답 패킷  :
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_LISTRANK	0x2012

typedef struct cSMSG_LISTRANK
{
	cmsg_base				base;
	//	추가
	BYTE		cur,count;	//	총 페이지수, 자신의 페이지 번호, 자신의 리스트수 6.26총페이지 삭제

	WORD					rank[dMAXRANKLIST];
	char					id[dMAXRANKLIST][dMAXUSERIDSIZE];
	BYTE					imclass[dMAXRANKLIST];
	WORD					win[dMAXRANKLIST];
	WORD					lost[dMAXRANKLIST];
	WORD					draw[dMAXRANKLIST];	//8.02
	DWORD					score[dMAXRANKLIST];
	

}csmsg_listrank;
/*-----------------------------------------------------------
	 패킷 종류  : 응답
	 설     명  : 랭킹리스트에서 특정유저의 정보를 검색해서 돌려준다. 
	 응답 패킷  :
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_FINDRANK		0x2013

typedef struct cSMSG_FINDRANK 
{
	cmsg_base				base;

	//	순번/ 아뒤 / 승 / 패 / 점수
	WORD					rank;
	char					id[dMAXUSERIDSIZE];
	BYTE					imclass;	//	지현... 0 이면 못찾은것이다.
	WORD					win;
	WORD					lost;
	WORD					draw;	//8.02
	DWORD					score;
}csmsg_findrank;

//*-----------------------------------------------------------------------------------------------------------------------------*
//* client가 보내고 Server가 받는 프로토콜																						*
//*-----------------------------------------------------------------------------------------------------------------------------*


#define dCMSG_LOGIN										0x1001

/*-----------------------------------------------------------
	 패킷 종류  : 통보
	 설     명  : 20명씩 잘라서 유저정보를 보내준다.
	 응답 패킷  :
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_INFOFIELD		0x2014
typedef struct USERINFO{
	char	id[dMAXUSERIDSIZE];
	WORD	win,lost,draw;
	BYTE	imclass;				//	계급을 넘긴다.
}suserinfo;

typedef struct cSMSG_INFOFIELD
{
	cmsg_base					base;
	BYTE		count;	//	총 페이지수, 자신의 페이지 번호, 자신의 리스트수
	suserinfo	sUserInfo[dMAXUSERLIST];

}csmsg_infofield;


/*
	cSMSG_FIND			:  (0x2051)찾은 유저의 결과를 돌려준다. 
	cSMSG_SAY			:  (0x2052)상대방에게 귓말내용을 전달한다. (통보)
	cSMSG_KICK			:  (0x2053)강퇴시킨다.(통보)
	cSMSG_PROFILE		:  (0x2054)요청한 상대의 정보를 보여준다. 
	cSMSG_DIS           :  (0x2055)관리자가 유저를 강퇴시킨다.(기간을 정할건가???)
	cSMSG_TRANSFILE		:  (0x2056)파일 전송한다. 
	cSMSG_INVITE		:  (0x2057)초청 메세지를 보내준다. 
*/
/*-----------------------------------------------------------
	 패킷 종류  : 요청
	 설     명  : 살아있냐?
	 응답 패킷  : cCMSG_LIVE
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_LIVE			0x2017

typedef struct cSMSG_LIVE
{
	cmsg_base				base;

}csmsg_live;

/*-----------------------------------------------------------
	 패킷 종류  : 응답
	 설     명  : 살아있다고 대답하기
	 요청 패킷  : cSMSG_LIVE
	 기     타  : 
-----------------------------------------------------------*/
#define dCMSG_LIVE			0x1024

typedef struct cCMSG_LIVE
{
	cmsg_base				base;
	
}ccmsg_live;

/*-----------------------------------------------------------
	 패킷 종류  : 응답
	 설     명  : 변경된 필드들
	 요청 패킷  : cCMSG_LISTBATTLE
	 기     타  : 6.16
-----------------------------------------------------------*/
#define dSMSG_CHGFIELD		0x2020

typedef struct cSMSG_CHGFIELD {
	cmsg_base			base;
	BYTE	count;
	char	name[dMAXFIELDLIST][dMAXFIELDNAMESIZE];
	BYTE	usercount[dMAXFIELDLIST];
}csmsg_chgfield;

/*-----------------------------------------------------------
	 패킷 종류  : 응답
	 설     명  : 변경된 겜방들
	 요청 패킷  : cCMSG_LISTBATTLE
	 기     타  : 6.16
-----------------------------------------------------------*/
#define dSMSG_CHGBATTLE		0x2021

typedef struct cSMSG_CHGBATTLE {
	cmsg_base			base;
	BYTE	count;
	char	name[dMAXBATTLELIST][dMAXFIELDNAMESIZE];
	BYTE	usercount[dMAXBATTLELIST],maxusercount[dMAXBATTLELIST];
	BYTE	bStart;	//	0 - 대기 , 1 - 시작됨.				6.23
	BYTE	type;

}csmsg_chgbattle;

/*-----------------------------------------------------------
	 패킷 종류  : 응답
	 설     명  : 삭제된 필드들
	 요청 패킷  : cCMSG_LISTFIELD
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_DELFIELD		0x2018

typedef struct cSMSG_DELFIELD
{
	cmsg_base			base;
	BYTE				sep,count;
	char	name[dMAXFIELDLIST][dMAXFIELDNAMESIZE];
	DWORD	times[dMAXFIELDLIST];

}csmsg_delfield;

/*-----------------------------------------------------------
	 패킷 종류  : 응답
	 설     명  : 겜방의 세세한 정보
	 요청 패킷  : cCMSG_INFO2BATTLE
	 기     타  : 
-----------------------------------------------------------*/
#define dSMSG_INFO2BATTLE		0x2022
	#define dINFO2BATTLE_SUCCESS		0
	#define dINFO2BATTLE_NOTFIND		1

typedef struct cSMSG_INFO2BATTLE
{
	cmsg_base			base;
//	BYTE				result;
	char	mapname[dMAXBATTLEMAPNAME];
//	BYTE	type;			//	게임 유형
	WORD	wsize,hsize;	//	타일 사이즈
	BYTE	terrain;		//	지형
	BYTE	secret;			//	공개(0) / 비공개(1) 
//	BYTE	maxplayer;		//	최대 수용 인원
	char	address[17];
	WORD	port;

	
}csmsg_info2battle;


/*-----------------------------------------------------------
	 패킷 종류  : 요청
	 설     명  : 방의 세세한 정보를 요청
	 응답 패킷  : cSMSG_INFO2BATTLE
	 기     타  : 
-----------------------------------------------------------*/
#define dCMSG_INFO2BATTLE		0x1025

typedef struct cCMSG_INFO2BATTLE
{
	cmsg_base			base;
	char name[dMAXBATTLENAMESIZE];
}ccmsg_info2battle;
/*-----------------------------------------------------------
	패킷 : 귓속말

  ----------------------------------------------------------*/
#define dCMSG_SAY				0x1052

typedef struct cCMSG_SAY
{
	cmsg_base			base;
	char				id[dMAXUSERIDSIZE];
	char				msg[dMAXCHATSIZE];
}ccmsg_say;

#define dSMSG_SAY			0x2052
typedef struct cSMSG_SAY
{
	cmsg_base			base;
	char				msg[dMAXCHATSIZE];
}csmsg_say;

#define dSMSG_TRANSFILE		0x2056
typedef struct cSMSG_TRANSFILE
{
	cmsg_base			base;	
	WORD	cur;					//	총 나눠진 수 / 현재 번호 
	WORD	size;						//	data의 사이즈가 있다.
	BYTE	data[dMAXBANNER_SIZE];
	
}csmsg_transfile;

#define	dSMSG_ASKTRANS		0x2057

typedef struct cSMSG_ASKTRANS			
{
	cmsg_base			base;
	
	DWORD				FileSize;				//	화일의 크기
	BYTE				BannerVersion;			//	배너 버젼
	WORD				tot;				//7.5 오전 TRNASFILE에서 옮겨옴
}csmsg_asktrans;


typedef struct tagRoom
{
	char		strName[21];									//대화방의 이름을 위한 문자열 변수
	int			numUser;										//대화방안의 유저 숫자를 위한 정수형변수
	BOOL		bCode;											//대화방의 암호가 걸려있는지의 상태를 체크 ,TRUE면 암호가 걸린것
}Room;

typedef struct tagUser
{
	char	id[dMAXUSERIDSIZE];
	WORD	win,lost,draw;
	BYTE	Mclass;

}tUser;
//dSMSG_LISTBATTLE
typedef struct tagBattle
{
	char		strName[dMAXFIELDNAMESIZE];
	BYTE	/*	추가된...*/	maxcount;				//	배틀 최대 인원
	BYTE					usercount;				//	겜방에 현재 참가중인 인원
	DWORD					times;
	char					mapname[dMAXBATTLEMAPNAME];
	WORD	wsize,hsize;//	타일 사이즈
	BYTE	terrain;					//	지형
	BYTE	secret;						//	공개(0) / 비공개(1) 

}BRoom;
typedef struct tagBattleRank
{
	WORD					rank;
	char					id[dMAXUSERIDSIZE];
	BYTE					imclass;
	WORD					win;
	WORD					lost;
	WORD					draw; //8.02
	DWORD					score;
}TBRank;
typedef struct tagFindRank
{
	
	WORD					rank;
	char					id[dMAXUSERIDSIZE];
	BYTE					imclass;	//	지현... 0 이면 못찾은것이다.
	WORD					win;
	WORD					lost;
	WORD					draw; //8.02
	DWORD					score;
}FindedRank;


//========================================================================================
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


typedef enum tagStateString
{
	BLOGIN,
	BDIALOG,
	AMUTE
}StateString;

typedef enum tagStateClient
{
	OUTROOM,
	INCHATROOM,
	INGAMEROOM,
	BEGAME
}StateClient;

typedef struct tagServerInfo
{
	char IP[255];
	long port;
}ServerInfo;	//접속하려는 서버의 정보를 담는 구조체




typedef struct tag_SendQueue
{
	int CurrentPos;
	int DesPos;

	int PacketSize[SendQueueSize];
	char *Packet[SendQueueSize];
}_SendQueue;



//임시적인 시뮬레이션 값들 
#define WM_BATTLEROOM_SUCCESS	WM_USER+100
#define WM_BATTLEROOM_CANCEL	WM_USER+101
#define WM_BATTLELAUNCH			WM_USER+102
#define WM_END_BATTLE			WM_USER+103
#define WM_BTN_FINDRNAK			WM_USER+104
#define WM_SEND_EXITBATTLE		WM_USER+105
#endif

#pragma pack()

/*-------------------------------------------------------------------------------------------------------------------------------------*
변경된 내용들(6. 14)


cSMSG_INFOFIELD / dSMSG_UPDATEFIELD (ping정보 삭제)

-----------------------------------------------------------------------------------------
common.h에서 수정 혹은 추가된 사항들

//	일주일마다 백업(월요일 11시)을 하고 한달에 한번 정도 백업하고 일주일마다 생성된 화일은 삭제한다. 

(기존의 dIN_IN과 dIN_OUT대신 아래 내용으로 변경되었당)
#define dIN					0
#define dOUT				1


#define dDBBACKUP_CYCLE		7	//	기준 : 일
#define dDELETEID_CYCLE		3	//	기준 : 달
#define dLIVECODE_CYCLE		4	//	기준 : 분

#define dMAXTIMES			6	//	시간을 기록하는 자리수

#define dMAXCDKEYSIZE		17


#define	dMAXFIELDLIST_S		12		//	원래 한화면에 뿌려지는 갯수
#define	dMAXBATTLELIST_S	12		//	원래 한화면에 뿌려지는 갯수
#define dMAXFIELDLIST		dMAXFIELDLIST_S*2		//	한번에 보낼수 있는 최대 채널 리스트 수
#define dMAXBATTLELIST		dMAXBATTLELIST_S*2		//	한번에 보낼수 있는 최대 겜방 리스트 수

#define dMAXRANKLIST_S		20		//	한번에 보낼수 있는 최대 랭킹 리스트 수
#define dMAXUSERLIST_S		20		//	필드들어갈때 한번에 보내는 유저수..
#define dMAXRANKLIST		dMAXRANKLIST_S+10		//	한번에 보낼수 있는 최대 랭킹 리스트 수
#define dMAXUSERLIST		dMAXUSERLIST_S+10		//	필드들어갈때 한번에 보내는 유저수..

#define dCLASS_41			41	//	준장
#define dCLASS_42			42	//	소장
#define dCLASS_43			43	//	중장
#define dCLASS_44			44	//	대장
#define dCLASS_45			45	//	원수

-----------------------------------------------------------------------------------------

cCMSG_CREATEBATTLE에 아래 추가
	#define dBATTLETYPE_1		1
	#define dBATTLETYPE_2		2
	#define dBATTLETYPE_3		3
	#define dBATTLETYPE_4		4
	#define dBATTLETYPE_5		5

-----------------------------------------------------------------------------------------

#define dCMSG_LISTFIELD		0x1007

	#define dLISTFIELD_UPDATE		0	//	새롭게 갱신된 내용을 요청한다.
	#define dLISTFIELD_ALL			121	//	모든 내용을 새롭게 받을 예정이다. 이 경우 처음 일정량만 받게 된다. 
	#define dLISTFIELD_DEL			120	//	삭제될 데이타를 요구한다.
	//	이 경우 클라이언트는 받아서 필터링 과정을 거쳐야 한다. 
	//	이미 넘어온 필드의 정보가 페이지를 분리하는 과정에서 중복 포함될 가능성이 있다.  
class cCMSG_LISTFIELD : public cMSG_BASE{
public:
	BYTE	page;

	cCMSG_LISTFIELD()	{size=sizeof(cCMSG_LISTFIELD);type=dCMSG_LISTFIELD;}
};

-----------------------------------------------------------------------------------------

#define dCMSG_LISTBATTLE		0x1016
	
	#define dLISTBATTLE_UPDATE		0		//	새롭게 갱신된 내용을 요청한다.
	#define dLISTBATTLE_ALL			9999		//	모든 내용을 새롭게 받을 예정이다. 이 경우 처음 일정량만 받게 된다. 
	#define dLISTBATTLE_DEL			10000	//	삭제될 데이타를 요구한다.
	//	이 경우 클라이언트는 받아서 필터링 과정을 거쳐야 한다. 
	//	이미 넘어온 배틀의 정보가 페이지를 분리하는 과정에서 중복 포함될 가능성이 있다.  
class cCMSG_LISTBATTLE : public cMSG_BASE{
public:
	WORD	page;

	cCMSG_LISTBATTLE()	{size=sizeof(cCMSG_LISTBATTLE);type=dCMSG_LISTBATTLE;}
};

-----------------------------------------------------------------------------------------

-----------------------------------------------------------
	 패킷 종류  : 요청
	 설     명  : 살아있냐?
	 응답 패킷  : cCMSG_LIVE
	 기     타  : 
-----------------------------------------------------------
#define dSMSG_LIVE			0x2017

class cSMSG_LIVE : public cMSG_BASE{
public:

	cSMSG_LIVE()	{size=sizeof(cSMSG_LIVE);type=dSMSG_LIVE;}
};

-----------------------------------------------------------
	 패킷 종류  : 응답
	 설     명  : 살아있다고 대답하기
	 요청 패킷  : cSMSG_LIVE
	 기     타  : 
-----------------------------------------------------------
#define dCMSG_LIVE			0x1024

class cCMSG_LIVE : public cMSG_BASE{
public:

	cCMSG_LIVE()	{size=sizeof(cCMSG_LIVE);type=dCMSG_LIVE;}
};

-----------------------------------------------------------------------------------------

<DB Table>

///////////////////////////////////////////////
//	User Table 
///////////////////////////////////////////////
char	id			17
char	pw			17
BYTE	imclass
word	win
word	lost
dword	score
word	f_win
word	f_lost
dword	f_score
word	lastdate		//	3개월이 지나면 자른다. to backup db
byte	isconnect

///////////////////////////////////////////////
//	Cdkey Table 
///////////////////////////////////////////////
dword	index
char	cdkey		17
byte	lock		(0 - open, 1 - close)



 *-------------------------------------------------------------------------------------------------------------------------------------*/